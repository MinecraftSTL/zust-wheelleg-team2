#include "MyHeadfile.h"

#pragma section all "cpu1_dsram"

Image image;
Image image1;
BinImage binImage;
Image cutShowImage;
Rgb565Image showImage;

int binR = 4;
int binDeltaT = -2;
int compareErr = 16;
int bly2RDR = 3;
float RD2IE = 0.7854;
float IGFE = 0.5236;
float facingErr = 0.5236;
int crossY = 20;
int crossX = 10;
int circleY = 5;
int circleX = 5;
int errY = 20;
int errDeltaY = 70;
uint8 showPInC1 = 1;
uint8 showBin = 0;

uint16 lStartPoint[2];
uint16 rStartPoint[2];

int16 lLine[MT9V03X_W*3][2];
uint8 lLineDir[MT9V03X_W*3];
uint16 lLineL = 0;
int16 rLine[MT9V03X_W*3][2];
uint8 rLineDir[MT9V03X_W*3];
uint16 rLineL = 0;
int16 lrMeet;

uint16 lBorder[MT9V03X_H];
uint16 rBorder[MT9V03X_H];
uint16 mLine[MT9V03X_H];

float lRadDir[MT9V03X_W*3];
int16 lRadDirPos[MT9V03X_W*3][2];
uint16 lRadDirN;
float rRadDir[MT9V03X_W*3];
int16 rRadDirPos[MT9V03X_W*3][2];
uint16 rRadDirN;

int16 lInflection[MT9V03X_W*3][2];
float lInflectionDir[MT9V03X_W*3];
uint16 lInflectionN;
int16 rInflection[MT9V03X_W*3][2];
float rInflectionDir[MT9V03X_W*3];
uint16 rInflectionN;

CameraStatus status = NONE;

uint8 camera_process_cnt = 0;

int camera_err = 0;              //摄像头中线偏差

#pragma section all restore
#pragma section all "cpu1_psram"

void MyCamera_Init(void)
{
    ips200_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips200_show_string(0, 16, "init success.");
}

uint8 inline Image_get(Image *this, uint16 y, uint16 x){
    if(y >= this->h || x >= this->w){
        return 0;
    }
    return this->image[x + this->w*y];
}
void inline Image_set(Image *this, uint16 y, uint16 x, uint8 value){
    if(y >= this->h || x >= this->w){
        return;
    }
    this->image[x + this->w*y] = value;
}
void Image_fromCamera(Image *this, uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W])
{
    this->h = MT9V03X_H;
    this->w = MT9V03X_W;
    memcpy(this->image, mt9v03x_image, MT9V03X_IMAGE_SIZE);
}
void Image_clear(Image *this){
    for(int i=0; i<this->h*this->w; ++i){
        this->image[i]=0x00;
    }
}
void Image_clone(Image *this, Image *target)
{
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    memcpy(target->image, this->image, this->h*this->w);
}
void Image_cut(Image *this, Image *target, uint16 y0, uint16 x0, uint16 y1, uint16 x1){
    zf_assert(this && target);
    target->h = y1-y0;
    target->w = x1-x0;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i+y0,j+x0));
        }
    }
}
void Image_zoom(Image *this, Image *target, float zoom){
    zf_assert(this && target && zoom > 0.0f);
    target->h = (this->h-1)*zoom+1;
    target->w = (this->w-1)*zoom+1;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i/zoom,j/zoom));
        }
    }
}
void Image_toRgb565Image(Image *this, Rgb565Image *target){
    target->h = this->h;
    target->w = this->w;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = this->image[i]>>(8-5)<<(5+6)|this->image[i]>>(8-6)<<5|this->image[i]>>(8-5);
    }
}
void Rgb565Image_clone(Rgb565Image *this, Rgb565Image *target)
{
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    memcpy(target->image, this->image, this->h*this->w*2);
}
void Rgb565Image_mode(Rgb565Image *this, Rgb565Image *target){
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = (this->image[i] << 8) | (this->image[i] >> 8);
    }
}
uint16 inline Rgb565Image_get(Rgb565Image *this, uint16 y, uint16 x){
    if(y >= this->h || x >= this->w){
        return 0;
    }
    return this->image[x + this->w*y];
}
void inline Rgb565Image_set(Rgb565Image *this, uint16 y, uint16 x, uint16 value){
    if(y >= this->h || x >= this->w){
        return;
    }
    this->image[x + this->w*y] = value;
}
void Rgb565Image_mark(Rgb565Image *this, uint16 y, uint16 x, uint16 color, uint16 r){
    for(int i = -r; i <= r; ++i){
        Rgb565Image_set(this, y-i, x-i, color);
        Rgb565Image_set(this, y+i, x-i, color);
    }
}
void Image_binaryzation(Image *this, Image *target, uint16 r, int16 deltaT){
    zf_assert(this && target);
    BinImage binImage;
    BinImage_init(&binImage, this, r, deltaT);
    target->h = binImage.h;
    target->w = binImage.w;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,BinImage_get(&binImage, i, j)?0xFF:0);
        }
    }
}
void BinImage_init(BinImage *this, Image *image, uint16 r, int16 deltaT){
    this->image = image;
    memset(&this->cache.image, 0, sizeof(this->cache.image));
    this->h = image->h - r*2;
    this->w = image->w - r*2;
    this->cache.h = this->h;
    this->cache.w = this->w;
    this->r = r;
    this->deltaT = deltaT;
}
uint8 BinImage_get(BinImage *this, uint16 y, uint16 x){
    zf_assert(this && this->image && y < this->h && x < this->w);
    uint8 ret = Image_get(&this->cache, y, x);
    if(!ret){
        uint32 sum = 0;
        for(uint16 i=y; i<=y+2*this->r; ++i){
            for(uint16 j=x; j<=x+2*this->r; ++j){
                sum += Image_get(this->image, i, j);
            }
        }
        int16 threshold = sum/((2*this->r+1)*(2*this->r+1));
        threshold+=this->deltaT;
        ret = (Image_get(this->image,y+this->r,x+this->r)>threshold)+1;
        Image_set(&this->cache, y, x, ret);
    }
    return ret-1;
}
/**
 * 函数功能：      差比和
 * 特殊说明：      用于爬线算法找起点
 * 形  参：        int16 a                  数值较大的灰度值
 *                int16 b                   数值较小的灰度值
 *                uint8 compare_value       差比和阈值
 *
 * 示例：          Compare_Num(image[start_row][i + 5], image[start_row][i], Compare_Value)；
 * 返回值：        大于阈值返回1，否则返回0.
 */
int16 Compare_Num(int16 a, int16 b, uint8 err)               //****
{
    if((((a - b) << 7) / (a + b)) > err)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void BinImage_getStartPoint(Image *this, uint16 lStartPoint[2], uint16 rStartPoint[2], uint8 err){
    uint16 y = this->h-1;
    uint16 x = this->w/2;

    lStartPoint[0]=y;
    for(lStartPoint[1] = x; lStartPoint[1] >= 2; --lStartPoint[1]){
        if(Compare_Num(Image_get(this, y, lStartPoint[1]+5), Image_get(this, y, lStartPoint[1]), err)){
            break;
        }
    }
    rStartPoint[0]=y;
    for(rStartPoint[1] = x; rStartPoint[1] < this->w-2; ++rStartPoint[1]){
        if(Compare_Num(Image_get(this, y, rStartPoint[1]-5), Image_get(this, y, rStartPoint[1]), err)){
            break;
        }
    }
}
uint8 BinImage_blyGet(BinImage *this, uint16 y, uint16 x){
    if(y < 2 || x < 2 || x >= this->w-2){
        return 0;
    }
    return BinImage_get(this, y, x);
}
const int8 Bly_SEED[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
void BinImage_bly(BinImage *this, uint16 maxL, int16 lLine[MT9V03X_W*3][2], int16 rLine[MT9V03X_W*3][2],
        uint8 lDir[MT9V03X_W*3], uint8 rDir[MT9V03X_W*3], uint16 *lLineL, uint16 *rLineL, int16 *meet,
        uint16 lStart[2], uint16 rStart[2])
{
    uint8 lStop = 0, rStop = 0;

    lLine[0][0] = lStart[0];
    lLine[0][1] = lStart[1];
    rLine[0][0] = rStart[0];
    rLine[0][1] = rStart[1];
    lDir[0] = rDir[0] = 2;

    *lLineL = *rLineL = 1;
    while(maxL--){
        int16 r_sub_l = rLine[*rLineL-1][0] - lLine[*lLineL-1][0];
        if(!lStop && r_sub_l<=0){
            uint8 lStep = 0;
            for(uint8 i=0; i<8; ++i){
                lDir[*lLineL] = (lDir[*lLineL-1]+6+i)&0x07;
                lLine[*lLineL][0] = lLine[*lLineL-1][0] + Bly_SEED[lDir[*lLineL]][0];
                lLine[*lLineL][1] = lLine[*lLineL-1][1] + Bly_SEED[lDir[*lLineL]][1];
                uint8 dir = (lDir[*lLineL]+1)&0x07;
                int16 dot[2];
                dot[0] = lLine[*lLineL-1][0] + Bly_SEED[dir][0];
                dot[1] = lLine[*lLineL-1][1] + Bly_SEED[dir][1];
                if(lLine[*lLineL][0] < 0 || lLine[*lLineL][1] < 0 || lLine[*lLineL][0] >= this->h || lLine[*lLineL][1] >= this->w ||
                        dot[0] < 0 || dot[1] < 0 || dot[0] >= this->h || dot[1] >= this->w){
                    continue;
                }
                if(!BinImage_blyGet(this, lLine[*lLineL][0], lLine[*lLineL][1]) && BinImage_blyGet(this, dot[0], dot[1])){
                    ++*lLineL;
                    lStep = 1;
                    break;
                }
            }
            if(!lStep){
                lStop = 1;
            }
        }
        if(!rStop && r_sub_l>=0){
            uint8 rStep = 0;
            for(uint8 i=0; i<8; ++i){
                rDir[*rLineL] = (rDir[*rLineL-1]+2+(8-i))&0x07;
                rLine[*rLineL][0] = rLine[*rLineL-1][0] + Bly_SEED[rDir[*rLineL]][0];
                rLine[*rLineL][1] = rLine[*rLineL-1][1] + Bly_SEED[rDir[*rLineL]][1];
                uint8 dir = (rDir[*rLineL]+7)&0x07;
                int16 dot[2];
                dot[0] = rLine[*rLineL-1][0] + Bly_SEED[dir][0];
                dot[1] = rLine[*rLineL-1][1] + Bly_SEED[dir][1];
                if(rLine[*rLineL][0] < 0 || rLine[*rLineL][1] < 0 || rLine[*rLineL][0] >= this->h || rLine[*rLineL][1] >= this->w ||
                        dot[0] < 0 || dot[1] < 0 || dot[0] >= this->h || dot[1] >= this->w){
                    continue;
                }
                if(!BinImage_blyGet(this, rLine[*rLineL][0], rLine[*rLineL][1]) && BinImage_blyGet(this, dot[0], dot[1])){
                    ++*rLineL;
                    rStep = 1;
                    break;
                }
            }
            if(!rStep){
                rStop = 1;
            }
        }
        if (lStop && rStop){
            break;
        }
        if (abs(rLine[*rLineL-1][0] - lLine[*lLineL-1][0]) < 2
            && abs(rLine[*rLineL-1][1] - lLine[*lLineL-1][1]) < 2){
            *meet = (rLine[*rLineL-1][0] + lLine[*lLineL-1][0])>>1;
            break;
        }
    }
}
void BinImage_blyToBorder(BinImage *this, uint8 dir, int16 line[MT9V03X_W*3][2], uint16 lineL, uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = dir?(this->w-1):0;
    }
    for(uint16 i=0; i<lineL; ++i){
        if(dir?(line[i][1] < ret[line[i][0]]):(line[i][1] > ret[line[i][0]])){
            ret[line[i][0]]=line[i][1];
        }
    }
}

uint8 inline BinImage_borderXIsLose(BinImage *this, uint16 x, uint8 dir){
    return dir?x >= this->w-1:x < 1;
}
uint8 inline BinImage_borderIsLose(BinImage *this, uint16 border[MT9V03X_H], uint16 y, uint8 dir){
    return BinImage_borderXIsLose(this, border[y], dir);
}
uint8 inline BinImage_borderIsAllLose(BinImage *this, uint16 border[MT9V03X_H], uint16 y0, uint16 y1, uint8 dir){
    for(uint16 i=y0; i<=y1; ++i){
        if(!BinImage_borderIsLose(this, border, i, dir)){
            return 0;
        }
    }
    return 1;
}
uint8 inline BinImage_blyXIsLose(BinImage *this, uint16 x, uint8 dir){
    return dir?x >= this->w-2:x < 2;
}
uint8 inline BinImage_blyIsLose(BinImage *this, uint16 border[MT9V03X_H], uint16 y, uint8 dir){
    return BinImage_blyXIsLose(this, border[y], dir);
}
uint8 inline BinImage_blyIsAllLose(BinImage *this, uint16 border[MT9V03X_H], uint16 y0, uint16 y1, uint8 dir){
    for(uint16 i=y0; i<=y1; ++i){
        if(!BinImage_blyIsLose(this, border, i, dir)){
            return 0;
        }
    }
    return 1;
}
void BinImage_borderToMiddle(BinImage *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
}
void Border_setLine(uint16 this[MT9V03X_H], uint16 y0, uint16 x0, uint16 y1, uint16 x1){
    for(uint16 i = y0; i <= y1; ++i){
        this[i] = (uint16)((float)(x1-x0)/(y1-y0)*(i-y0)+x0);
    }
}

void BinImage_blyCutByBorder(BinImage *this, uint16 border[MT9V03X_H], uint8 dir, int16 line[MT9V03X_W*3][2], uint16 *lineL){
    uint16 yMin;
    for(yMin = 0; yMin<this->h; ++yMin){
        if(!BinImage_borderIsLose(this, border, yMin, dir)){
            break;
        }
    }
    for(uint16 i=0; i<*lineL; ++i){
        if(line[i][0] == yMin && line[i][1] == border[yMin]){
            *lineL = i+1;
            break;
        }
    }
}

void BinImage_blyToRadDir(BinImage *this, int16 bly[MT9V03X_W*3][2], uint16 blyL, uint16 l,
        float ret[MT9V03X_W*3], int16 retPos[MT9V03X_W*3][2], uint16 *retN){
    *retN = (blyL-1)/l;
    for(uint16 i=0; i<*retN; ++i){
        ret[i]=atan2f(-(bly[(i+1)*l][0]-bly[i*l][0]), bly[(i+1)*l][1]-bly[i*l][1]);
        for(uint16 j=i*l; j<=(i+1)*l; ++j){
            if(BinImage_blyXIsLose(this, bly[j][1], 0) || BinImage_blyXIsLose(this, bly[j][1], 1)){
                ret[i] = NAN;
                break;
            }
        }
        retPos[i][0] = (bly[i*l][0]+bly[(i+1)*l][0])>>1;
        retPos[i][1] = (bly[i*l][1]+bly[(i+1)*l][1])>>1;
    }
}
void RadDir_toInflection(float this[MT9V03X_W*3], int16 thisPos[MT9V03X_W*3][2], uint16 thisN, float err,
        int16 ret[MT9V03X_W*3][2], float retRad[MT9V03X_W*3], uint16 *retN){
    *retN = 0;
    for(uint16 i = 1; i < thisN-1; ++i){
        if(isnan(this[i-1]) || isnan(this[i+1])){
            continue;
        }
        if(PI/2-NormalizeAngle_toPi2(Angle_normalize(this[i+1]-this[i-1])) <= err){
            ret[*retN][0] = thisPos[i][0];
            ret[*retN][1] = thisPos[i][1];
            retRad[*retN] = atan2f(sinf(this[i+1])-sinf(this[i-1]),cosf(this[i+1])-cosf(this[i-1]));
//            retRad[*retN] = (this[i+1]+Angle_normalize(this[i-1]+PI))/2;
//            if(fabs(this[i+1]-this[i-1])>PI){
//                if(this[i+1] >= 0){
//                    retRad[*retN] += PI;
//                }else{
//                    retRad[*retN] -= PI;
//                }
//            }
            ++*retN;
            i+=2;
        }
    }
}
uint8 Inflection_getFacing(float rad){
    for(uint8 i=0; i<4; ++i){
        if(fabsf(Angle_normalize(PI/4+i*PI/2)-rad)<=facingErr){
            return i+1;
        }
    }
    return 0;
}

void BinImage_zebraCrossing(BinImage *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 y){

    uint8 zebra_row;
    int i;
    int edge_left_num = 0;
    int edge_right_num = 0;
    int edge_sum = 0;
    for(zebra_row = y; zebra_row < y+3; zebra_row++)
    {
        for(i=lBorder[zebra_row];i<this->w/2;i++)
        {
            if(BinImage_get(this, zebra_row, i) && BinImage_get(this, zebra_row, i+1) &&
                    !BinImage_get(this, zebra_row, i+2) && !BinImage_get(this, zebra_row,i+3))
            {
                edge_left_num++;
            }
        }
        for(i=rBorder[zebra_row];i>this->w/2;i--)
        {
            if(BinImage_get(this, zebra_row, i) && BinImage_get(this, zebra_row, i-1) &&
                    !BinImage_get(this, zebra_row, i-2) && !BinImage_get(this, zebra_row,i-3))
            {
                edge_right_num++;
            }
        }
    }
    edge_sum = edge_left_num + edge_right_num;
    if(edge_right_num > 200)
        edge_right_num = 0;
    if(edge_left_num > 200)
        edge_left_num = 0;
    if(edge_sum >= 16 && edge_left_num > 5 && edge_right_num > 5)                      //停车
    {
        beepShort();
        status = IN_ZEBRA;
    }else if(status == IN_ZEBRA){
        beepShort();
        status = OUT_ZEBRA;
        Car_Stop();
        status = NONE;
    }
}
void BinImage_cross(BinImage *this, int16 lInf[MT9V03X_W*3][2], float lInfRad[MT9V03X_W*3], uint16 lInfN,
        int16 rInf[MT9V03X_W*3][2], float rInfRad[MT9V03X_W*3], uint16 rInfN,
        uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H]){
    switch(status){
        case NONE:
            if(lInfN > 1 && rInfN > 1 &&
                    lInf[0][0] - lInf[1][0] >= crossY && rInf[0][0] - rInf[1][0] >= crossY &&
                    Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(rInfRad[0]) == 4 &&
                    Inflection_getFacing(lInfRad[1]) == 2 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    BinImage_blyIsAllLose(this, lBorder, lInf[1][0]+crossX, lInf[0][0]-crossX , 0) &&
                    BinImage_blyIsAllLose(this, rBorder, rInf[1][0]+crossX, rInf[0][0]-crossX , 1)
                    )
            {
                beepShort();
                status = IN_CROSS;
            }
            break;
        case IN_CROSS:
            if(lInfN > 1 && rInfN > 1 &&
                    lInf[0][0] - lInf[1][0] >= crossY && rInf[0][0] - rInf[1][0] >= crossY &&
                    Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(rInfRad[0]) == 4 &&
                    Inflection_getFacing(lInfRad[1]) == 2 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    BinImage_blyIsAllLose(this, lBorder, lInf[1][0]+crossX, lInf[0][0]-crossX , 0) &&
                    BinImage_blyIsAllLose(this, rBorder, rInf[1][0]+crossX, rInf[0][0]-crossX , 1)){
                Border_setLine(lBorder, lInf[1][0], lInf[1][1], lInf[0][0], lInf[0][1]);
                Border_setLine(rBorder, rInf[1][0], rInf[1][1], rInf[0][0], rInf[0][1]);
            }
            printf("%d,%d,%d,%d,%d,%d\r\n",lInfN,rInfN,Inflection_getFacing(lInfRad[0]),Inflection_getFacing(rInfRad[0]),
                    BinImage_blyIsAllLose(this, lBorder, lInf[0][0]+crossX, this->h-1, 0),
                            BinImage_blyIsAllLose(this, rBorder, rInf[0][0]+crossX, this->h-1, 1));
            if(lInfN > 0 && rInfN > 0 &&
                    (Inflection_getFacing(lInfRad[0]) == 2 || Inflection_getFacing(rInfRad[0]) == 1) &&
                    BinImage_blyIsAllLose(this, lBorder, lInf[0][0]+crossX, this->h-1, 0) &&
                    BinImage_blyIsAllLose(this, rBorder, rInf[0][0]+crossX, this->h-1, 1)
                    ){
                beepShort();
                status = OUT_CROSS;
            }
            if(lInfN == 0 && rInfN == 0){
                beepShort();
                status = NONE;
            }
            break;
        case OUT_CROSS:
            if(lInfN > 0 && rInfN > 0){
                if(Inflection_getFacing(lInfRad[0]) == 2){
                    Border_setLine(lBorder, lInf[0][0], lInf[0][1], this->h-1, lBorder[this->h-1]);
                }else if(Inflection_getFacing(lInfRad[1]) == 2){
                    Border_setLine(lBorder, lInf[1][0], lInf[1][1], this->h-1, lBorder[this->h-1]);
                }
                if(Inflection_getFacing(rInfRad[0]) == 1){
                    Border_setLine(rBorder, rInf[0][0], rInf[0][1], this->h-1, rBorder[this->h-1]);
                }else if(Inflection_getFacing(rInfRad[1]) == 1){
                    Border_setLine(rBorder, rInf[1][0], rInf[1][1], this->h-1, rBorder[this->h-1]);
                }
            }
            if(lInfN == 0 && rInfN == 0){
                beepShort();
                status = NONE;
            }
            break;
    }
}
void BinImage_lCircle(BinImage *this, int16 lInf[MT9V03X_W*3][2], float lInfRad[MT9V03X_W*3], uint16 lInfN,
        int16 rInf[MT9V03X_W*3][2], float rInfRad[MT9V03X_W*3], uint16 rInfN,
        uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H]){
    switch(status){
        case NONE:
            if(lInfN > 1 && rInfN == 1 &&
                    Inflection_getFacing(lInfRad[0]) == 3 &&
                    Inflection_getFacing(lInfRad[1]) == 4 &&
                    BinImage_blyXIsLose(this, lBorder[lInf[0][0]-circleX], 0) &&
                    Inflection_getFacing(rInfRad[0]) == 3
                    ){
                beepShort();
                status = IN_L_CIRCLE;
            }
            break;
        case IN_L_CIRCLE:
            if(lInfN > 1 &&
                    Inflection_getFacing(lInfRad[0]) == 3 &&
                    Inflection_getFacing(lInfRad[1]) == 4 &&
                    BinImage_blyXIsLose(this, lBorder[lInf[0][0]-circleX], 0)){
                Border_setLine(lBorder, lInf[1][0], lInf[1][1], lInf[0][0], lInf[0][1]);
            }
            if(rInfN == 1 &&
                    (lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 ||
                    lInfN > 1 && Inflection_getFacing(lInfRad[1]) == 2) &&
                    Inflection_getFacing(rInfRad[0]) == 3 &&
                    BinImage_blyXIsLose(this, lBorder[lInf[1][0]+circleX], 0)
                    ){
                beepShort();
                status = TL_L_CIRCLE;
            }
            break;
        case TL_L_CIRCLE:
            if(lInfN > 0 && (Inflection_getFacing(lInfRad[0]) == 2 || Inflection_getFacing(lInfRad[0]) == 1)){
                Border_setLine(rBorder, lInf[0][0], lInf[0][1], this->h-1, rBorder[this->h-1]);
            }else if(lInfN > 1 && (Inflection_getFacing(lInfRad[1]) == 2 || Inflection_getFacing(lInfRad[0]) == 1)){
                Border_setLine(rBorder, lInf[1][0], lInf[1][1], this->h-1, rBorder[this->h-1]);
            }
            if(lInfN == 0 && rInfN == 0){
                beepShort();
                status = L_CIRCLE;
            }
            break;
        case L_CIRCLE:
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4){
                beepShort();
                status = TR_L_CIRCLE;
            }
            break;
        case TR_L_CIRCLE:
            if(rInfN > 0){
                uint16 y;
                uint8 lastLose = 1;
                for(y=1; y<this->h; ++y){
                    if(BinImage_blyIsLose(this, lBorder, y, 0)){
                        if(!lastLose){
                            break;
                        }
                    }else{
                        lastLose = 0;
                    }
                }
                Border_setLine(rBorder, y-1, lBorder[y-1], rInf[0][0], rInf[0][1]);
            }
            if(rInfN == 1 &&
                    (lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 ||
                    lInfN > 1 && Inflection_getFacing(lInfRad[1]) == 2) &&
                    Inflection_getFacing(rInfRad[0]) == 3 &&
                    BinImage_blyXIsLose(this, lBorder[lInf[1][0]+circleX], 0)
                    ){
                beepShort();
                status = OUT_L_CIRCLE;
            }
            break;
        case OUT_L_CIRCLE:
            if(lInfN > 0 && (Inflection_getFacing(lInfRad[0]) == 2 || Inflection_getFacing(lInfRad[0]) == 1)){
                Border_setLine(lBorder, lInf[0][0], lInf[0][1], this->h-1, lBorder[this->h-1]);
            }else if(lInfN > 1 && (Inflection_getFacing(lInfRad[1]) == 2 || Inflection_getFacing(lInfRad[0]) == 1)){
                Border_setLine(lBorder, lInf[1][0], lInf[1][1], this->h-1, lBorder[this->h-1]);
            }
            if(lInfN == 0 && rInfN == 0){
                beepShort();
                status = NONE;
            }
            break;
    }
}

int BinImage_middleToErr(BinImage *this, int16 middle[MT9V03X_W], uint16 y, uint16 deltaY){
    int32 sum = 0;
    for(uint16 i=y; i<y+deltaY; ++i){
        sum += middle[i]-this->w/2;
    }
    return sum/deltaY;
}

void Image_processForShow(){
    if(showBin){
        Image_binaryzation(&image1, &cutShowImage, binImage.r, binImage.deltaT);
    }else{
        Image_cut(&image1, &cutShowImage, binImage.r, binImage.r, image1.h-binImage.r, image1.w-binImage.r);
    }
    Image_toRgb565Image(&cutShowImage, &showImage);
    for(uint16 i=0; i<lLineL; ++i){
        Rgb565Image_set(&showImage, lLine[i][0], lLine[i][1], RGB565_YELLOW);
    }
    for(uint16 i=0; i<rLineL; ++i){
        Rgb565Image_set(&showImage, rLine[i][0], rLine[i][1], RGB565_YELLOW);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, i, lBorder[i], RGB565_GREEN);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, i, rBorder[i], RGB565_GREEN);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, i, mLine[i], i >= errY && i < errY+errDeltaY ? RGB565_RED : RGB565_BLUE);
    }
    Rgb565Image_set(&showImage, lStartPoint[0], lStartPoint[1], RGB565_RED);
    Rgb565Image_set(&showImage, rStartPoint[0], rStartPoint[1], RGB565_RED);
    for(uint16 i=0; i<lInflectionN; ++i){
        Rgb565Image_mark(&showImage, lInflection[i][0], lInflection[i][1], RGB565_RED, 2);
        Rgb565Image_mark(&showImage, lInflection[i][0]-sinf(lInflectionDir[i])*6, lInflection[i][1]+cosf(lInflectionDir[i])*6, RGB565_RED,1);
    }
    for(uint16 i=0; i<rInflectionN; ++i){
        Rgb565Image_mark(&showImage, rInflection[i][0], rInflection[i][1], RGB565_RED, 2);;
        Rgb565Image_mark(&showImage, rInflection[i][0]-sinf(rInflectionDir[i])*6, rInflection[i][1]+cosf(rInflectionDir[i])*6, RGB565_RED,1);
    }
}
void MyCamera_Show(uint16 x, uint16 y)
{
    if(camera_process_cnt){
        camera_process_cnt=0;
        if(!showPInC1){
            Image_processForShow();
        }
        ips200_show_rgb565_image(x, y, showImage.image, showImage.w, showImage.h, showImage.w, showImage.h, 0);
    }
}
void Image_processCamera(){
    if(mt9v03x_finish_flag)              //判断一幅图像是否接收完成
    {
//        SysTimer_Start();
        Image_fromCamera(&image, mt9v03x_image);
        mt9v03x_finish_flag = 0;
        Image_cut(&image,&image1,0,0,image.h,image.w-1);
//        Image_zoom(&image1, &image,0.5);
        BinImage_init(&binImage, &image1, binR, binDeltaT);
        Image_cut(&image1, &image, binImage.r, binImage.r, image1.h-binImage.r, image1.w-binImage.r);
        BinImage_getStartPoint(&image, lStartPoint, rStartPoint, compareErr);
        BinImage_bly(&binImage, binImage.h*3, lLine, rLine, lLineDir, rLineDir,
                &lLineL, &rLineL, &lrMeet, lStartPoint, rStartPoint);
        BinImage_blyToBorder(&binImage, 0, lLine, lLineL, lBorder);
        BinImage_blyToBorder(&binImage, 1, rLine, rLineL, rBorder);
        BinImage_blyCutByBorder(&binImage, lBorder, 0, lLine, &lLineL);
        BinImage_blyCutByBorder(&binImage, rBorder, 1, rLine, &rLineL);
        BinImage_zebraCrossing(&binImage, lBorder, rBorder, 70);
        BinImage_blyToRadDir(&binImage, lLine, lLineL, bly2RDR, lRadDir, lRadDirPos, &lRadDirN);
        BinImage_blyToRadDir(&binImage, rLine, rLineL, bly2RDR, rRadDir, rRadDirPos, &rRadDirN);
        RadDir_toInflection(lRadDir, lRadDirPos, lRadDirN, RD2IE, lInflection, lInflectionDir, &lInflectionN);
        RadDir_toInflection(rRadDir, rRadDirPos, rRadDirN, RD2IE, rInflection, rInflectionDir, &rInflectionN);
        BinImage_cross(&binImage, lInflection, lInflectionDir, lInflectionN, rInflection, rInflectionDir, rInflectionN, lBorder, rBorder);
        BinImage_lCircle(&binImage, lInflection, lInflectionDir, lInflectionN, rInflection, rInflectionDir, rInflectionN, lBorder, rBorder);
//        SysTimer_Stop();
//        printf("%d\r\n",GetPastTime());
//
//            Cross_Road();                   //十字
//            Left_Straight();              //左直线判断
//            Right_Straight();             //右直线判断
//
////            Inflection_Point();           //断点总断
//            Find_Circle();                  //找圆环
//
        BinImage_borderToMiddle(&binImage, lBorder, rBorder, mLine);
//            Middle_Empty();
////            Bend_Straight_Opinion();        //判断是否是直线
        camera_err = BinImage_middleToErr(&binImage, mLine, errY, errDeltaY);
////            printf("%d\r\n",g_camera_mid_err);
////            printf("%d ,%d ,%d\r\n ",target_left,target_right,g_camera_mid_err);
//            camera_process_cnt++;
//            camera_process_cnt_show++;
//        }
        if(showPInC1){
            Image_processForShow();
        }
        Fps_add(1);
        camera_process_cnt=1;
    }
}

#pragma section all restore
