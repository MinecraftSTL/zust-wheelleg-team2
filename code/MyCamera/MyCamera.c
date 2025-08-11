#include "MyHeadfile.h"

int g_camera_mid_err;

Image image;
Image image1;
BinImage binImage;
Image cutShowImage;
Rgb565Image showImage;

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

//int16 luInflection[MT9V03X_W*3][2];
//uint16 luInflectionN;
//int16 ldInflection[MT9V03X_W*3][2];
//uint16 ldInflectionN;
//int16 ruInflection[MT9V03X_W*3][2];
//uint16 ruInflectionN;
//int16 rdInflection[MT9V03X_W*3][2];
//uint16 rdInflectionN;

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

void MyCamera_Show(uint16 start_y)
{
    if(camera_process_cnt){
        camera_process_cnt=0;
        Image_cut(&image, &cutShowImage, binImage.r, binImage.r);
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
        Rgb565Image_set(&showImage, lStartPoint[0], lStartPoint[1], RGB565_RED);
        Rgb565Image_set(&showImage, rStartPoint[0], rStartPoint[1], RGB565_RED);
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, mLine[i], RGB565_BLUE);
        }
//        for(uint16 i=0; i<luInflectionN; ++i){
//            Rgb565Image_set(&showImage, luInflection[i][0], luInflection[i][1], RGB565_RED);
//        }
//        for(uint16 i=0; i<ldInflectionN; ++i){
//            Rgb565Image_set(&showImage, ldInflection[i][0], ldInflection[i][1], RGB565_RED);
//        }
//        for(uint16 i=0; i<ruInflectionN; ++i){
//            Rgb565Image_set(&showImage, ruInflection[i][0], ruInflection[i][1], RGB565_RED);
//        }
//        for(uint16 i=0; i<rdInflectionN; ++i){
//            Rgb565Image_set(&showImage, rdInflection[i][0], rdInflection[i][1], RGB565_RED);
//        }
        for(uint16 i=0; i<lInflectionN; ++i){
            Rgb565Image_mark(&showImage, lInflection[i][0], lInflection[i][1], RGB565_RED, 2);
        }
        for(uint16 i=0; i<rInflectionN; ++i){
            Rgb565Image_mark(&showImage, rInflection[i][0], rInflection[i][1], RGB565_RED, 2);
        }
        ips200_show_rgb565_image(0, start_y, showImage.image, showImage.w, showImage.h, showImage.w, showImage.h, 0);
    }
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
void Rgb565Image_mark(Rgb565Image *this, uint16 y, uint16 x, uint16 color, uint16 size){
    for(int i = -size; i <= size; ++i){
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
    this->h = image->h - r*2;
    this->w = image->w - r*2;
    this->r = r;
    this->deltaT = deltaT;
}
uint8 BinImage_get(BinImage *this, uint16 y, uint16 x){
    zf_assert(this && this->image && y < this->h && x < this->w);
    uint64 sum = 0;
    for(uint16 i = y; i<=y+2*this->r; ++i){
        for(uint16 j = x; j<=x+2*this->r; ++j){
            sum += Image_get(this->image,i,j);
        }
    }
    int16 threshold = sum/((this->r*2+1)*(this->r*2+1));
    threshold+=this->deltaT;
    return Image_get(this->image,y+this->r,x+this->r)>threshold;
}
uint8 Bly_BinImage_get(BinImage *this, uint16 y, uint16 x){
    if(y < 2 || x < 2 || x >= this->w-2){
        return 0;
    }
    return BinImage_get(this, y, x);
}
void BinImage_getStartPoint(BinImage *this, uint16 lStartPoint[2], uint16 rStartPoint[2]){
    uint16 y = this->h-1;
    uint16 x = this->w/2;

    lStartPoint[0]=y;
    for(uint16 i = x; i >= 1; --i){
        lStartPoint[1]=i;
        if(!BinImage_get(this, y, i) && BinImage_get(this, y, i+1) && !BinImage_get(this, y, i-1) && BinImage_get(this, y, i+2)){
            break;
        }
    }
    rStartPoint[0]=y;
    for(uint16 i = x-1; i < this->w-1; ++i){
        rStartPoint[1]=i;
        if(!BinImage_get(this, y, i) && BinImage_get(this, y, i-1) && !BinImage_get(this, y, i+1) && BinImage_get(this, y, i-2)){
            break;
        }
    }
}
const int8 Bly_SEED[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
//const int8 Bly_SEED[8][2] = {{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1}};
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
        int16 r_sub_l = rLine[*lLineL-1][0] - lLine[*lLineL-1][0];
        if(lStop || r_sub_l>0){
            goto BinImage_bly_lEnd;
        }
        for(uint8 i=0; i<8; ++i){
            uint8 dir = (lDir[*lLineL-1]+6+i)&0x07;
            lLine[*lLineL][0] = lLine[*lLineL-1][0] + Bly_SEED[dir][0];
            lLine[*lLineL][1] = lLine[*lLineL-1][1] + Bly_SEED[dir][1];
            uint8 nextDir = (dir+1)&0x07;
            int16 lineNext[2];
            lineNext[0] = lLine[*lLineL-1][0] + Bly_SEED[nextDir][0];
            lineNext[1] = lLine[*lLineL-1][1] + Bly_SEED[nextDir][1];
            if(lLine[*lLineL][0] < 0 || lLine[*lLineL][1] < 0 || lLine[*lLineL][0] >= this->h || lLine[*lLineL][1] >= this->w ||
                    lineNext[0] < 0 || lineNext[1] < 0 || lineNext[0] >= this->h || lineNext[1] >= this->w){
                continue;
            }
            if(!BinImage_get(this, lLine[*lLineL][0], lLine[*lLineL][1]) && BinImage_get(this, lineNext[0], lineNext[1])){
                lDir[*lLineL] = dir;
                ++*lLineL;
                goto BinImage_bly_lEnd;
            }
        }
        lStop = 1;
        BinImage_bly_lEnd:;
        if(rStop || r_sub_l<0){
            goto BinImage_bly_rEnd;
        }
        for(uint8 i=0; i<8; ++i){
            uint8 dir = (rDir[*rLineL-1]+2+(8-i))&0x07;
            rLine[*rLineL][0] = rLine[*rLineL-1][0] + Bly_SEED[dir][0];
            rLine[*rLineL][1] = rLine[*rLineL-1][1] + Bly_SEED[dir][1];
            uint8 nextDir = (dir+7)&0x07;
            int16 lineNext[2];
            lineNext[0] = rLine[*rLineL-1][0] + Bly_SEED[nextDir][0];
            lineNext[1] = rLine[*rLineL-1][1] + Bly_SEED[nextDir][1];
            if(rLine[*rLineL][0] < 0 || rLine[*rLineL][1] < 0 || rLine[*rLineL][0] >= this->h || rLine[*rLineL][1] >= this->w ||
                    lineNext[0] < 0 || lineNext[1] < 0 || lineNext[0] >= this->h || lineNext[1] >= this->w){
                continue;
            }
            if(!BinImage_get(this, rLine[*rLineL][0], rLine[*rLineL][1]) && BinImage_get(this, lineNext[0], lineNext[1])){
                rDir[*rLineL] = dir;
                ++*rLineL;
                goto BinImage_bly_lEnd;
            }
        }
        rStop = 1;
        BinImage_bly_rEnd:;
        if (abs(rLine[*rLineL-1][0] - lLine[*lLineL-1][0]) < 2
            && abs(rLine[*rLineL-1][1] - lLine[*lLineL-1][1] < 2)){
            *meet = (rLine[*rLineL-1][0] + lLine[*lLineL-1][0])>>1;
            break;
        }
    }
}
void BinImage_blyToBorder(BinImage *this, int16 line[MT9V03X_W*3][2], uint16 lineL, uint8 dir, uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = dir?(this->w-1):0;
    }
    for(uint16 i=0; i<lineL; ++i){
        if(dir?(line[i][1] < ret[line[i][0]]):(line[i][1] > ret[line[i][0]])){
            ret[line[i][0]]=line[i][1];
        }
    }
}
void BinImage_borderToMiddle(BinImage *this, int16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
}

const uint8 Inflection_LuIn[] = {0,0,0,0,0,0,1,0};
const uint8 Inflection_LuOut[] = {0,0,0,0,1,1,0,0};
const uint8 Inflection_LdIn[] = {0,0,0,0,1,1,0,0};
const uint8 Inflection_LdOut[] = {0,0,1,0,0,0,0,0};

const uint8 Inflection_RuIn[] = {0,0,0,0,0,0,1,0};
const uint8 Inflection_RuOut[] = {0,0,0,0,1,1,0,0};
const uint8 Inflection_RdIn[] = {0,0,0,0,1,1,0,0};
const uint8 Inflection_RdOut[] = {0,0,1,0,0,0,0,0};
void Bly_toInflection(int16 this[MT9V03X_W*3][2], uint16 thisL, uint8 thisDir[MT9V03X_W*3],
        const uint8 inDir[8], const uint8 outDir[8], uint16 dirL, int16 ret[MT9V03X_W*3][2], uint16 *retN){
    *retN=0;
    for(uint16 i=2; i<thisL-2; ++i){
        uint8 t = 1;
        for(uint16 j = 0; j < dirL; ++j){
            if(!inDir[thisDir[i-1-j]] || !outDir[thisDir[i+1+j]]){
                t=0;
                break;
            }
        }
        if(!t){
            continue;
        }
        ret[*retN][0]=this[i][0];
        ret[*retN][1]=this[i][1];
        ++*retN;
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

void Image_Process(void)
{
    if(mt9v03x_finish_flag == 1)              //判断一幅图像是否接收完成
    {
        Image_fromCamera(&image, mt9v03x_image);
        mt9v03x_finish_flag = 0;
//        Image_cut(&image, &image1,0,0);
//        Image_zoom(&image1, &image,1);
        BinImage_init(&binImage, &image, 3, -4);
        lStartPoint[0]=rStartPoint[0]=0;
        BinImage_getStartPoint(&binImage, lStartPoint, rStartPoint);
        BinImage_bly(&binImage, binImage.h*3, lLine, rLine, lLineDir, rLineDir,
                &lLineL, &rLineL, &lrMeet, lStartPoint, rStartPoint);
        BinImage_blyToBorder(&binImage, lLine, lLineL, 0, lBorder);
        BinImage_blyToBorder(&binImage, rLine, rLineL, 1, rBorder);
        BinImage_borderToMiddle(&binImage, lBorder, rBorder, mLine);
//        Bly_toInflection(lLine, lLineL, lLineDir, Inflection_LuIn, Inflection_LuOut, 2, luInflection, &luInflectionN);
//        Bly_toInflection(lLine, lLineL, lLineDir, Inflection_LdIn, Inflection_LdOut, 2, ldInflection, &ldInflectionN);
//        Bly_toInflection(rLine, rLineL, rLineDir, Inflection_RuIn, Inflection_RuOut, 2, ruInflection, &ruInflectionN);
//        Bly_toInflection(rLine, rLineL, rLineDir, Inflection_RdIn, Inflection_RdOut, 2, rdInflection, &rdInflectionN);
        Bly_toRadDir(lLine, lLineL, 2, lRadDir, lRadDirPos, &lRadDirN);
        Bly_toRadDir(rLine, rLineL, 2, rRadDir, rRadDirPos, &rRadDirN);
        RadDir_toInflection(lRadDir, lRadDirPos, lRadDirN, 0.5236, lInflection, lInflectionDir, &lInflectionN);
        RadDir_toInflection(rRadDir, rRadDirPos, rRadDirN, 0.5236, rInflection, rInflectionDir, &rInflectionN);
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
        camera_process_cnt=1;
    }
}

