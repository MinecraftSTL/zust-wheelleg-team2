#include "MyHeadfile.h"

int g_camera_mid_err;

Image image;
Image image1;
BinImage binImage;
Image cutShowImage;
Rgb565Image showImage;

int16 lStartPoint[2];
int16 rStartPoint[2];

int16 lLine[MT9V03X_H*3][2];
uint16 lLineL = 0;
int8 lLineDir[MT9V03X_H*3];
int16 rLine[MT9V03X_H*3][2];
uint16 rLineL = 0;
int8 rLineDir[MT9V03X_H*3];
int16 lBorder[MT9V03X_H];
int16 rBorder[MT9V03X_H];

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
        if(lStartPoint[0]&&rStartPoint[0]){
            Rgb565Image_set(&showImage, lStartPoint[0], lStartPoint[1], RGB565_RED);
            Rgb565Image_set(&showImage, rStartPoint[0], rStartPoint[1], RGB565_RED);
        }
        ips200_show_rgb565_image(0, start_y, showImage.image, showImage.w, showImage.h, showImage.w, showImage.h, 0);
    }
}
uint8 inline Image_get(Image *this, uint16 y, uint16 x){
    return this->image[x + this->w*y];
}
void inline Image_set(Image *this, uint16 y, uint16 x, uint8 value){
    this->image[x + this->w*y] = value;
}
void Image_fromCamera(Image *this, uint8 *mt9v03x_image)
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
void inline Rgb565Image_set(Rgb565Image *this, uint16 y, uint16 x, uint16 value){
    this->image[x + this->w*y] = value;
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
            Image_set(target,i,j,BinImage_get(&binImage, i, j));
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
    uint8 color = Image_get(this->image,y+this->r,x+this->r);
    if(color == 0 || color == 0xFF){
        return color;
    }else{
        return color>threshold ? 0xFF : 0;
    }
}
void BinImage_set(BinImage *this, uint16 y, uint16 x, uint8 value){
    zf_assert(this && this->image && y < this->h && x < this->w);
    Image_set(this->image, y+this->r, x+this->r, value?0xFF:0);
}

void BinImage_drawRectan(BinImage *this)
{

    uint16 i = 0;
    for (i = 0; i < this->h; i++)     //给左边0、1列和右边186、187列画黑框
    {
        BinImage_set(this,i,0,0);
        BinImage_set(this,i,1,0);
        BinImage_set(this,i,this->w - 1,0);
        BinImage_set(this,i,this->w - 2,0);

    }
    for (i = 0; i < this->w; i++)     //给上方0、1行画黑框
    {
        BinImage_set(this,0,i,0);
        BinImage_set(this,1,i,0);
    }
}
void BinImage_getStartPoint(BinImage *this, int16 lStartPoint[2], int16 rStartPoint[2]){
    uint16 y = this->h-2;
    uint16 x = this->w/2;
    for(uint16 i = x; i >= 1; --i){
        if(!BinImage_get(this, y, i) && BinImage_get(this, y, i+1) && !BinImage_get(this, y, i-1) && BinImage_get(this, y, i+2)){
            lStartPoint[0]=y;
            lStartPoint[1]=i;
            break;
        }
    }
    for(uint16 i = x-1; i < this->w-1; ++i){
        if(!BinImage_get(this, y, i) && BinImage_get(this, y, i-1) && !BinImage_get(this, y, i+1) && BinImage_get(this, y, i-2)){
            rStartPoint[0]=y;
            rStartPoint[1]=i;
            break;
        }
    }
}
const int16 bly[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},};
void Pos_clone(const int16 this[2], int16 target[2]){
    target[0]=this[0];
    target[1]=this[1];
}
void Pos_add(const int16 a[2], const int16 b[2], int16 sum[2]){
    sum[0]=a[0]+b[0];
    sum[1]=a[1]+b[1];
}
uint8 Pos_equal(const int16 a[2], const int16 b[2]){
    return a[0]==b[0]&&a[1]==b[1];
}


void Image_Process(void)
{
    if(mt9v03x_finish_flag == 1)              //判断一幅图像是否接收完成
    {
        Image_fromCamera(&image, mt9v03x_image);               //图像转载 37ns一帧
        mt9v03x_finish_flag = 0;
//        Image_cut(&image, &image1,0,0);
//        Image_zoom(&image1, &image,1);
        BinImage_init(&binImage, &image, 3, -4);
        lStartPoint[0]=rStartPoint[0]=0;
        BinImage_getStartPoint(&binImage, lStartPoint, rStartPoint);
        if(lStartPoint[0]&&rStartPoint[0]){
            BinImage_drawRectan(&binImage);
        }
//            //八领域   圆环3ms一帧  直线1.8ms一帧
//            Search_Line_BLY((uint16)USE_num, image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &Endline);
////            Growth_Direction();
//            get_left(data_stastics_l);    //左边界提取
//            get_right(data_stastics_r);   //右边界提取
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

