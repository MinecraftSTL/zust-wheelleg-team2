#include "MyHeadfile.h"

int g_camera_mid_err;

Image image;
Image image1;
BinImage binImage;
Image cutShowImage;
Rgb565Image showImage;

uint16 lStartPoint[2];
uint16 rStartPoint[2];

uint16 lLine[MT9V03X_W*3][2];
uint8 lLineDir[MT9V03X_W*3];
uint16 lLineL = 0;
uint16 rLine[MT9V03X_W*3][2];
uint8 rLineDir[MT9V03X_W*3];
uint16 rLineL = 0;
uint16 lrMeet[2];
uint8 L_Stop_Flag = 0;
uint8 R_Stop_Flag = 0;
uint16 lBorder[MT9V03X_H];
uint16 rBorder[MT9V03X_H];
uint16 mLine[MT9V03X_H];

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
//        for(uint16 i=0; i<lLineL; ++i){
//            Rgb565Image_set(&showImage, lLine[i][0], lLine[i][1], RGB565_YELLOW);
//        }
//        for(uint16 i=0; i<rLineL; ++i){
//            Rgb565Image_set(&showImage, rLine[i][0], rLine[i][1], RGB565_YELLOW);
//        }
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, lBorder[i], RGB565_GREEN);
        }
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, rBorder[i], RGB565_GREEN);
        }
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, mLine[i], RGB565_BLUE);
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
    if(y < 2 || x < 2 || y>=this->h-2 || x >= this->w-2){
        return 0;
    }
    return BinImage_get(this, y, x);
}
void BinImage_getStartPoint(BinImage *this, uint16 lStartPoint[2], uint16 rStartPoint[2]){
    uint16 y = this->h-3;
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

void BinImage_bly(BinImage *this, uint16 maxL, uint16 lLine[MT9V03X_W*3][2], uint16 rLine[MT9V03X_W*3][2],
        uint8 lDir[MT9V03X_W*3], uint8 rDir[MT9V03X_W*3], uint16 *lLineL, uint16 *rLineL, uint16 *meet,
        uint16 lStart[2], uint16 rStart[2])
{

    uint8 i = 0, j = 0;
    //左边变量
    uint16 search_filds_l[8][2] = { { 0 } };
    uint16 center_point_l[2] = {  0 };
    uint16 index_l = 0;
    uint16 temp_l[8][2] = { {  0 } };
    uint16 l_data_statics;//统计左边
    //定义八个邻域
    static int8 seeds_l[8][2] = { {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1} };
    //        static int8 seeds_l[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {+1,0}, {1,1}, {0,1}, {-1,1} };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是顺时针

    //右边变量
    uint16 search_filds_r[8][2] = { {  0 } };
    uint16 center_point_r[2] = { 0 };//中心坐标点
    uint16 index_r = 0;//索引下标
    uint16 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//统计右边
    //定义八个邻域
    static int8 seeds_r[8][2] = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}, };
    //        static int8 seeds_r[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
    //        static int8 seeds_l[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
    //        static int8 seeds_r[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {+1,0}, {1,1}, {0,1}, {-1,1} };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是逆时针

    l_data_statics = 0;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = 0;//统计找到了多少个点，方便后续把点全部画出来

    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = lStart[0];//x
    center_point_l[1] = lStart[1];//y
    center_point_r[0] = rStart[0];//x
    center_point_r[1] = rStart[1];//y

       //开启邻域循环
    while (maxL--)
    {

       //左边
       for (i = 0; i < 8; i++)//传递8F坐标
       {
           search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
           search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
       }
       //中心坐标点填充到已经找到的点内
       lLine[l_data_statics][0] = center_point_l[0];//x
       lLine[l_data_statics][1] = center_point_l[1];//y
       l_data_statics++;//索引加一

       //右边
       for (i = 0; i < 8; i++)//传递8F坐标
       {
           search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
           search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
       }
       //中心坐标点填充到已经找到的点内
       rLine[r_data_statics][0] = center_point_r[0];//x
       rLine[r_data_statics][1] = center_point_r[1];//y


       index_l = 0;//先清零，后使用
       for (i = 0; i < 8; i++)
       {
           temp_l[i][0] = 0;//先清零，后使用
           temp_l[i][1] = 0;//先清零，后使用
       }

       //左边判断
       for (i = 0; i < 8; i++)
       {
           if(search_filds_l[(i + 1) & 7][0] >= this->h){
               continue;
           }
           if (!Bly_BinImage_get(this,search_filds_l[i][0],search_filds_l[i][1])
               && Bly_BinImage_get(this,search_filds_l[(i + 1) & 7][0],search_filds_l[(i + 1) & 7][1]))
           {
               temp_l[index_l][0] = search_filds_l[(i)][0];
               temp_l[index_l][1] = search_filds_l[(i)][1];
               index_l++;
               lDir[l_data_statics - 1] = (i);//记录生长方向
           }

           if (index_l)
           {
               //更新坐标点
               center_point_l[0] = temp_l[0][0];//x
               center_point_l[1] = temp_l[0][1];//y
               for (j = 0; j < index_l; j++)
               {
                   if (center_point_l[1] > temp_l[j][1])
                   {
                       center_point_l[0] = temp_l[j][0];//x
                       center_point_l[1] = temp_l[j][1];//y
                   }
               }
           }

       }
       if ((rLine[r_data_statics][0]== rLine[r_data_statics-1][0]&& rLine[r_data_statics][0] == rLine[r_data_statics - 2][0]
           && rLine[r_data_statics][1] == rLine[r_data_statics - 1][1] && rLine[r_data_statics][1] == rLine[r_data_statics - 2][1])
           ||(lLine[l_data_statics-1][0] == lLine[l_data_statics - 2][0] && lLine[l_data_statics-1][0] == lLine[l_data_statics - 3][0]
               && lLine[l_data_statics-1][1] == lLine[l_data_statics - 2][1] && lLine[l_data_statics-1][1] == lLine[l_data_statics - 3][1]))
       {
    //                printf("三次进入同一个点，退出\n");
           break;
       }
       if (abs(rLine[r_data_statics][0] - lLine[l_data_statics - 1][0]) < 2
           && abs(rLine[r_data_statics][1] - lLine[l_data_statics - 1][1] < 2)
           )
       {
    //                printf("\n左右相遇退出\n");
           *meet = (rLine[r_data_statics][1] + lLine[l_data_statics - 1][1]) >> 1;//取出最高点
    //                printf("\n在y=%d处退出\n",*Endline);
           break;
       }
       if ((rLine[r_data_statics][1] < lLine[l_data_statics - 1][1]))
       {
    //                printf("\n如果左边比右边高了，左边等待右边\n");
           continue;//如果左边比右边高了，左边等待右边
       }
       if (lDir[l_data_statics - 1] == 7
           && (rLine[r_data_statics][1] > lLine[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
       {
    //                printf("\n左边开始向下了，等待右边，等待中... \n");
           center_point_l[0] = (uint8)lLine[l_data_statics - 1][0];//x
           center_point_l[1] = (uint8)lLine[l_data_statics - 1][1];//y
           l_data_statics--;
       }
       r_data_statics++;//索引加一

       index_r = 0;//先清零，后使用
       for (i = 0; i < 8; i++)
       {
           temp_r[i][0] = 0;//先清零，后使用
           temp_r[i][1] = 0;//先清零，后使用
       }

       //右边判断
       for (i = 0; i < 8; i++)
       {
           if(search_filds_r[(i + 1) & 7][0] >= this->h){
               continue;
           }
           if (!Bly_BinImage_get(this,search_filds_r[i][0],search_filds_r[i][1])
               && Bly_BinImage_get(this,search_filds_r[(i + 1) & 7][0],search_filds_r[(i + 1) & 7][1]))
           {
               temp_r[index_r][0] = search_filds_r[(i)][0];
               temp_r[index_r][1] = search_filds_r[(i)][1];
               index_r++;//索引加一
               rDir[r_data_statics - 1] = (i);//记录生长方向
    //                    printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
           }
           if (index_r)
           {

               //更新坐标点
               center_point_r[0] = temp_r[0][0];//x
               center_point_r[1] = temp_r[0][1];//y
               for (j = 0; j < index_r; j++)
               {
                   if (center_point_r[1] > temp_r[j][1])
                   {
                       center_point_r[0] = temp_r[j][0];//x
                       center_point_r[1] = temp_r[j][1];//y
                   }
               }

           }
       }
    }

    //取出循环次数
    *lLineL = l_data_statics;
    *rLineL = r_data_statics;
}
void BinImage_blyToBorder(BinImage *this, uint16 line[MT9V03X_W*3][2], uint16 lineL, uint8 dir, uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = dir?(this->w-1):0;
    }
    for(uint16 i=0; i<lineL; ++i){
        uint16 y = line[i][0];
        if(dir?(line[i][1] < ret[y]):(line[i][1] > ret[y])){
            ret[y]=line[i][1];
        }
    }
}
void BinImage_borderToMiddle(BinImage *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
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
            BinImage_bly(&binImage, binImage.h*3, lLine, rLine, lLineDir, rLineDir,
                    &lLineL, &rLineL, lrMeet, lStartPoint, rStartPoint);
            BinImage_blyToBorder(&binImage, lLine, lLineL, 0, lBorder);
            BinImage_blyToBorder(&binImage, rLine, rLineL, 1, rBorder);
            BinImage_borderToMiddle(&binImage, lBorder, rBorder, mLine);
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

