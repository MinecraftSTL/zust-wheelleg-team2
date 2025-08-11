#include "MyHeadfile.h"

int g_camera_mid_err;

Image image;
Image image1;
BinImage binImage;
Image cutShowImage;
Rgb565Image showImage;

uint16 binR = 4;
int binDeltaT = -2;
uint16 bly2RDR = 2;
float RD2IE = 0.6;

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

uint8 camera_process_cnt = 0;

int camera_err = 0;              //摄像头中线偏差



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
void Image_cut(Image *this, Image *target, uint16 h, uint16 w){
    zf_assert(this && target && 2*h <= this->h && 2*w <= this->w);
    target->h = this->h - 2*h;
    target->w = this->w - 2*w;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i+h,j+w));
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
uint16 Rgb565Image_get(Rgb565Image *this, uint16 y, uint16 x){
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
    zf_assert(2 * r < this->h && 2 * r < this->w); // 确保原图足够大
    target->h = this->h - r*2;
    target->w = this->w - r*2;
    BinImage binImage;
    BinImage_init(&binImage, this, r, deltaT);
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
        uint64 sum = 0;
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
void BinImage_getStartPoint(BinImage *this, uint16 lStartPoint[2], uint16 rStartPoint[2]){
    uint16 y = this->h-1;
    uint16 x = this->w/2;

    lStartPoint[0]=y;
    for(lStartPoint[1] = x; lStartPoint[1] >= 2; --lStartPoint[1]){
        if(!BinImage_get(this, y, lStartPoint[1]) && BinImage_get(this, y, lStartPoint[1]+1) &&
                !BinImage_get(this, y, lStartPoint[1]-1) && BinImage_get(this, y, lStartPoint[1]+2)){
            break;
        }
    }
    rStartPoint[0]=y;
    for(rStartPoint[1] = x-1; rStartPoint[1] < this->w-2; ++rStartPoint[1]){
        if(!BinImage_get(this, y, rStartPoint[1]) && BinImage_get(this, y, rStartPoint[1]-1) &&
                !BinImage_get(this, y, rStartPoint[1]+1) && BinImage_get(this, y, rStartPoint[1]-2)){
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
                int16 lineNext[2];
                lineNext[0] = rLine[*rLineL-1][0] + Bly_SEED[dir][0];
                lineNext[1] = rLine[*rLineL-1][1] + Bly_SEED[dir][1];
                if(rLine[*rLineL][0] < 0 || rLine[*rLineL][1] < 0 || rLine[*rLineL][0] >= this->h || rLine[*rLineL][1] >= this->w ||
                        lineNext[0] < 0 || lineNext[1] < 0 || lineNext[0] >= this->h || lineNext[1] >= this->w){
                    continue;
                }
                if(!BinImage_blyGet(this, rLine[*rLineL][0], rLine[*rLineL][1]) && BinImage_blyGet(this, lineNext[0], lineNext[1])){
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
uint8 BinImage_borderIsLose(BinImage *this, uint16 border[MT9V03X_H], uint16 y, uint8 dir){
    return border[y] == dir?this->w-1:0;
}
void BinImage_borderToMiddle(BinImage *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
}

void Bly_toRadDir(int16 this[MT9V03X_W*3][2], uint16 thisL, uint16 l,
        float ret[MT9V03X_W*3], int16 retPos[MT9V03X_W*3][2], uint16 *retN){
    *retN = (thisL-1)/l;
    for(uint16 i=0; i<*retN; ++i){
        ret[i]=atan2f(-(this[(i+l)*l][0]-this[i*l][0]), this[(i+l)*l][1]-this[i*l][1]);
        retPos[i][0] = (this[(i+l)*l][0]+this[i*l][0])>>1;
        retPos[i][1] = (this[(i+l)*l][1]+this[i*l][1])>>1;
    }
}
void RadDir_toInflection(float this[MT9V03X_W*3], int16 thisPos[MT9V03X_W*3][2], uint16 thisN, float err,
        int16 ret[MT9V03X_W*3][2], float retRad[MT9V03X_W*3], uint16 *retN){
    *retN = 0;
    for(uint16 i = 1; i < thisN-1; ++i){
        if(PI/2-NormalizeAngle_toPi2(Angle_normalize(this[i+1]-this[i-1])) <= err){
            ret[*retN][0] = thisPos[i][0];
            ret[*retN][1] = thisPos[i][1];
            retRad[*retN] = Angle_normalize((this[i+1]-this[i-1])/2);
            ++*retN;
            i+=2;
        }
    }
}
void Image_processForShow(){
        Image_cut(&image, &cutShowImage, binImage.r, binImage.r);
//        Image_binaryzation(&image, &cutShowImage, binImage.r, binImage.deltaT);
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
            Rgb565Image_set(&showImage, i, mLine[i], RGB565_BLUE);
        }
        Rgb565Image_set(&showImage, lStartPoint[0], lStartPoint[1], RGB565_RED);
        Rgb565Image_set(&showImage, rStartPoint[0], rStartPoint[1], RGB565_RED);
        for(uint16 i=0; i<lInflectionN; ++i){
            Rgb565Image_mark(&showImage, lInflection[i][0], lInflection[i][1], RGB565_RED, 2);
        }
        for(uint16 i=0; i<rInflectionN; ++i){
            Rgb565Image_mark(&showImage, rInflection[i][0], rInflection[i][1], RGB565_RED, 2);
        }
}
void MyCamera_Show(uint16 start_y)
{
    if(camera_process_cnt){
        camera_process_cnt=0;
        Image_processForShow();
        ips200_show_rgb565_image(0, start_y, showImage.image, showImage.w, showImage.h, showImage.w, showImage.h, 0);
    }
}
void Image_processCamera(){
    if(mt9v03x_finish_flag)              //判断一幅图像是否接收完成
    {
        Image_fromCamera(&image, mt9v03x_image);
        mt9v03x_finish_flag = 0;
//        Image_cut(&image, &image1,0,0);
//        Image_zoom(&image1, &image,1);
        BinImage_init(&binImage, &image, binR, binDeltaT);
        lStartPoint[0]=rStartPoint[0]=0;
        BinImage_getStartPoint(&binImage, lStartPoint, rStartPoint);
        BinImage_bly(&binImage, binImage.h*3, lLine, rLine, lLineDir, rLineDir,
                &lLineL, &rLineL, &lrMeet, lStartPoint, rStartPoint);
        BinImage_blyToBorder(&binImage, 0, lLine, lLineL, lBorder);
        BinImage_blyToBorder(&binImage, 1, rLine, rLineL, rBorder);
        BinImage_borderToMiddle(&binImage, lBorder, rBorder, mLine);
        Bly_toRadDir(lLine, lLineL, bly2RDR, lRadDir, lRadDirPos, &lRadDirN);
        Bly_toRadDir(rLine, rLineL, bly2RDR, rRadDir, rRadDirPos, &rRadDirN);
        RadDir_toInflection(lRadDir, lRadDirPos, lRadDirN, RD2IE, lInflection, lInflectionDir, &lInflectionN);
        RadDir_toInflection(rRadDir, rRadDirPos, rRadDirN, RD2IE, rInflection, rInflectionDir, &rInflectionN);
//            Zebra_Crossing();
//            Lost_Left();                  //左下方丢线判断
//            Lost_Right();                 //右下方丢线判断
//
//            Lower_left();                   //左下断点
//            Lower_right();                  //右下断点
//            Upper_left();                   //左上断点
//            Upper_right();                  //右上断点
//
//            Cross_Road();                   //十字
//            Left_Straight();              //左直线判断
//            Right_Straight();             //右直线判断
//
////            Inflection_Point();           //断点总断
//            Find_Circle();                  //找圆环
//
//            middle_line();                  //提取中线
//            Middle_Empty();
////            Bend_Straight_Opinion();        //判断是否是直线
//            g_camera_mid_err = Camera_Get_MidErr();
////            printf("%d\r\n",g_camera_mid_err);
////            printf("%d ,%d ,%d\r\n ",target_left,target_right,g_camera_mid_err);
//            camera_process_cnt++;
//            camera_process_cnt_show++;
//        }
//        Image_processForShow();
        Fps_add(1);
        camera_process_cnt=1;
    }
}

