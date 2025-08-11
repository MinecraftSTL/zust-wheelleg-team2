/*
作者：Charon and 快乐牌小刀片
       未经授权禁止转售
 */


#include "sys.h"

/*************==============================================================================================************
 * 计算偏差值，送给PID进行调整
 ***********************
 *************===============================================================================================***********/
#include "zf_common_headfile.h"

//--------------------------
//摄像头前瞻量，根据摄像头位置设置，数字代表权重大小，例如第1个数为1代表第0行的权重为1
const int weight[120]={
0,0,0,0,0,0,0,0,0,0,//0-10
0,0,0,0,0,0,0,0,0,0,//11-20
0,0,0,0,0,0,0,0,0,0,//21-30
2,2,2,2,2,2,2,2,2,2,//30-40
4,4,4,4,4,4,4,4,4,4,//40-50
6,6,6,6,6,6,6,6,6,6,//51-60
7,7,7,7,7,7,7,7,7,7,//61-70
25,25,25,25,25,25,25,25,25,25,//71-80
14,14,14,14,14,8,8,8,8,8,//81-90
6,6,6,6,6,4,4,4,4,4,//91-100
2,2,2,2,2,2,2,2,2,2,//101-110
2,2,2,2,2,2,2,2,2,2,//111-120
};//1-120

int piancha = 0;
uint8 xunxian =0;
uint8 bend_straight_flag=0;

#define White 255  //定义图像黑
#define Black 0    //定义图像白

#define Left 1
#define Right 2
int x,y;
float parameterB,parameterA;   //y=parameterB*x+parameterA
int left[120]={2};       //左边界数组   从118开始存  存储的顺序是第一行就存在left[1]中      //提取的线在爬出线的列数上+1
int right[120]={185};    //右边界数组   从118开始存  存储的顺序是第一行就存在right[1]中     //提取的线在爬出线的列数上-1
int middle[120]={93};    //中线数组
int Endline=1;           //截止行
int WhiteNum=0;
int X1,Y1;//左下补线点（圆环）
uint8 right_lost_num=0;        //右边丢线数
uint8 left_lost_num=0;         //左边丢线数
uint8 right_lost_num2=0;        //右边丢线数
uint8 left_lost_num2=0;         //左边丢线数
uint8 right_lost_num3=0;        //右边丢线数
uint8 left_lost_num3=0;         //左边丢线数
uint8 S_right_lost_num=0;        //S右边丢线数
uint8 S_left_lost_num=0;         //S左边丢线数
uint8 imag[MT9V03X_H][MT9V03X_W];          //二值化图像数组
uint8 threshold_value=175;
char threshold_value_add=0;
char threshold_open_or_close=1;
uint32 image_process_time=0;   //图像处理时间
//生长方向
uint8 left_2_growth_direction=0;    //左边生长方向2的个数
uint8 left_5_growth_direction=0;    //左边生长方向5的个数
uint8 left_6_growth_direction=0;    //左边生长方向6的个数
uint8 right_2_growth_direction=0;   //右边生长方向2的个数
uint8 right_5_growth_direction=0;   //右边生长方向5的个数
uint8 left_3_growth_direction=0;    //左边生长方向5的个数
uint8 right_6_growth_direction=0;    //左边生长方向6的个数
uint8 right_3_growth_direction=0;   //右边生长方向2的个数
uint8 l_growth_direction_flag=0;    //左边生长方向2标志位
uint8 r_growth_direction_flag=0;    //右边生长方向2标志位
uint8 l_growth_direction_flag35=0;    //左边生长方向35标志位
uint8 r_growth_direction_flag35=0;    //右边生长方向35标志位
uint8 l_growth_direction_flag6=0;    //左边生长方向35标志位
uint8 r_growth_direction_flag6=0;    //右边生长方向35标志位

uint8 Right_straight_flag=0; //右直线
uint8 Left_straight_flag=0; //左直线

uint8 annulus_L_Flag=0;       //左圆环标志
uint8 annulus_R_Flag=0;       //右圆环标志
uint8 annulus_L_memory=0;     //左圆环状态
uint8 annulus_R_memory=0;     //右圆环状态

uint8 zebra_crossing_flag=0;//斑马线标志
uint8 Left_garage_flag=0;   //左车库标志
uint8 Left_garage_memory=0; //左车库状态

//陀螺仪
int16 gyro_start=0;    //陀螺仪起始值
int16 gyro_end=0;      //陀螺仪起始值

//圆环凸起点
uint8 roundabout_X=0;    //圆环凸起点列坐标
uint8 roundabout_Y=0;    //圆环凸起点行坐标
uint8 roundabout_Flag=0; //圆环凸起点标志

//出环识别点
uint8 Exit_loop_X=0;    //圆环出环列坐标
uint8 Exit_loop_Y=0;    //圆环出环行坐标
uint8 Exit_loop_Flag=0; //圆环出环标志

uint8 Crossroad_Flag=0;       //十字标志
uint8 Crossroad_memory=0;     //十字状态
uint8 Finish_Flag=0;          //处理完成标识位
//丢线
uint8 Lost_left_Flag=0;         //左边丢线标志
uint8 Lost_right_Flag=0;        //右边丢线标志
uint8 Lost_left_Flag2=0;         //左边丢线标志
uint8 Lost_right_Flag2=0;        //右边丢线标志
uint8 Lost_left_Flag3=0;         //左边丢线标志
uint8 Lost_right_Flag3=0;        //右边丢线标志
uint8 Lost_point_L_scan_line=0; //左边丢线扫描线在第几行
uint8 Lost_point_R_scan_line=0; //右边丢线扫描线在第几行


//十字拐点
uint8 ten_inflexion_down_l=0;    //十字左下拐点行坐标
uint8 ten_inflexion_down_r=0;    //十字右下拐点行坐标
uint8 ten_inflexion_up_l=0;      //十字左上拐点行坐标
uint8 ten_inflexion_up_r=0;      //十字右上拐点行坐标

//十字拐点标志
uint8 ten_inflexion_down_l_flag=0;    //十字左下拐点列坐标
uint8 ten_inflexion_down_r_flag=0;    //十字右下拐点列坐标
uint8 ten_inflexion_up_l_flag=0;      //十字左上拐点列坐标
uint8 ten_inflexion_up_r_flag=0;      //十字右上拐点列坐标

//左下拐点
uint8 Lower_left_inflection_X =0;      //左下拐点列坐标
uint8 Lower_left_inflection_Y =0;      //左下拐点行坐标
uint8 Lower_left_inflection_Flag=0;    //左下拐点标志

//右下拐点
uint8 Lower_right_inflection_X =0;      //右下拐点列坐标
uint8 Lower_right_inflection_Y =0;      //右下拐点行坐标
uint8 Lower_right_inflection_Flag=0;    //右下拐点标志

//左上拐点
uint8 Upper_left_inflection_X =0;      //左上拐点列坐标
uint8 Upper_left_inflection_Y =0;      //左上拐点行坐标
uint8 Upper_left_inflection_Flag=0;    //左上拐点标志

//右上拐点
uint8 Upper_right_inflection_X =0;      //右上拐点列坐标
uint8 Upper_right_inflection_Y =0;      //右上拐点行坐标
uint8 Upper_right_inflection_Flag=0;    //右上拐点标志

//S道标志
uint8 S_left_lost_Flag=0;               //左边s道丢线标志
uint8 S_right_lost_Flag=0;              //右边s道丢线标志
uint8 S_left_inflection_Flag=0;         //左边s道标志
uint8 S_right_inflection_Flag=0;        //右边s道标志
uint8 S_road_Flag=0;                    //s道标志
uint8 S_road_memory=0;                  //s道状态

//坡道判断
char hill_flag=0;
char podaogeshu=1;//坡道个数
extern int paodao_time_juge;

uint8 road_width[120]={   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //赛道宽度
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          138, 138, 141, 141, 144, 144, 147, 147, 150, 150 };

/*=========================================绝对值函数=======================================================*/
float absolute(float z)
{
    z = z< 0 ? (-z) : z;
    return z;
}
/*================================================================================================*/
int16 limit_a_b(int x, int a, int b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return (int16)x;
}


int16 limit1(int16 x, int16 y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                   return x;
}


uint8 original_image[image_h][image_w];     //转载图像数组
/*=========================================进行图像转载========================================================*/
void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num     1   //1 不压缩，2 压缩一倍
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          //
    {
        for (j = 0; j <image_w; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
            line++;
        }
        line = 0;
        row++;
    }
}
/*=======================================进行图像大津法========================================================*/
uint8 OtsuThreshold(uint8 *image, uint16 col, uint16 row)
{

#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }




    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }

   return Threshold;
}
/*=======================================进行图像二值化========================================================*/
void binaryzation(void)
{
  uint8 i,j;
if(threshold_open_or_close==1)
 threshold_value = OtsuThreshold(original_image[0], image_w, image_h);
else
 threshold_value = OtsuThreshold(original_image[0], image_w, image_h)+threshold_value_add;
  for(i = 0;i<image_h;i++)
  {

      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>threshold_value)//175
              imag[i][j] = white_pixel;
          else
              imag[i][j] = black_pixel;

      }

  }

}

/*=======================================寻找爬线起点========================================================*/
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值
uint8 get_start_point(uint8 start_row)
{
    uint8 i = 0,l_found = 0,r_found = 0;
    //清零
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y

    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y

        //从中间往左边，先找起点
    for (i = image_w / 2; i > border_min; i--)
    {
        start_point_l[0] = i;//x
        start_point_l[1] = start_row;//y
        if (imag[start_row][i] == 255 && imag[start_row][i - 1] == 0)
        {
            //printf("找到左边起点image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }

    for (i = image_w / 2; i < border_max; i++)
    {
        start_point_r[0] = i;//x
        start_point_r[1] = start_row;//y
        if (imag[start_row][i] == 255 && imag[start_row][i + 1] == 0)
        {
            //printf("找到右边起点image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }

    if(l_found&&r_found)return 1;
    else {
        //printf("未找到起点\n");
        return 0;
    }
}

/*=======================================进行图像八邻域========================================================*/
#define USE_num image_h*3
 //存放点的x，y坐标
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线      points_l[num][0]是列坐标数组  points_l[num][1]是行坐标数组
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线      points_r[num][0]是列坐标数组  points_r[num][1]是行坐标数组
uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, int*Endline)
{

    uint8 i = 0, j = 0;
    //左边变量
    uint8 search_filds_l[8][2] = { {  0 } };
    uint8 index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = {  0 };
    uint16 l_data_statics;//统计左边
    //定义八个邻域
    static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是顺时针

    //右边变量
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//中心坐标点
    uint8 index_r = 0;//索引下标
    uint8 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//统计右边
    //定义八个邻域
    static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是逆时针

    l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

        //开启邻域循环
    while (break_flag--)
    {

        //左边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//索引加一

        //右边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        index_l = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//先清零，后使用
            temp_l[i][1] = 0;//先清零，后使用
        }

        //左边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                temp_l[index_l][0] = search_filds_l[(i)][0];
                temp_l[index_l][1] = search_filds_l[(i)][1];
                index_l++;
                dir_l[l_data_statics - 1] = (i);//记录生长方向
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
        if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
        {
            //printf("三次进入同一个点，退出\n");
            break;
        }
        if (absolute(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && absolute(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
            )
        {
            //printf("\n左右相遇退出\n");
            *Endline = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
            //printf("\n在y=%d处退出\n",*Endline);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
           // printf("\n如果左边比右边高了，左边等待右边\n");
            continue;//如果左边比右边高了，左边等待右边
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
        {
            //printf("\n左边开始向下了，等待右边，等待中... \n");
            center_point_l[0] = (uint8)points_l[l_data_statics - 1][0];//x
            center_point_l[1] = (uint8)points_l[l_data_statics - 1][1];//y
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
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//索引加一
                dir_r[r_data_statics - 1] = (i);//记录生长方向
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
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
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;

}
/*=======================================进行生长方个数提取========================================================*/
uint8 r1=0,r2=0,r3=0,r4=0,r5=0,r6=0,r7=0,r8=0;
uint8 l1=0,l2=0,l3=0,l4=0,l5=0,l6=0,l7=0,l8=0;
void growth_direction(void)
{
    uint16 i;
    r1=0;
    r2=0;
    r3=0;
    r4=0;
    r5=0;
    r6=0;
    r7=0;
    r8=0;

    l1=0;
    l2=0;
    l3=0;
    l4=0;
    l5=0;
    l6=0;
    l7=0;
    l8=0;

    /*===清零===*/
    left_2_growth_direction=0;
    right_2_growth_direction=0;
    left_5_growth_direction=0;
    right_5_growth_direction=0;
    left_3_growth_direction=0;
    right_6_growth_direction=0;
    left_6_growth_direction=0;
    right_3_growth_direction=0;
    /*===左边提取生长方向个数===*/
    for(i=0;i<=data_stastics_l;i++)
    {
        if(dir_l[i]==2)
            left_2_growth_direction++;
        if(dir_l[i]==5)
            left_5_growth_direction++;
        if(dir_l[i]==3)
            left_3_growth_direction++;
        if(dir_l[i]==1)
            l1++;
        if(dir_l[i]==2)
            l2++;
        if(dir_l[i]==3)
            l3++;
        if(dir_l[i]==4)
            l4++;
        if(dir_l[i]==5)
            l5++;
        if(dir_l[i]==6)
        {
            l6++;
            left_6_growth_direction++;
        }
        if(dir_l[i]==7)
            l7++;
        if(dir_l[i]==0)
            l8++;
    }
    /*===右边提取生长方向个数===*/
    for(i=0;i<=data_stastics_r;i++)
    {
        if(dir_r[i]==2)
          right_2_growth_direction++;
        if(dir_r[i]==5)
          right_5_growth_direction++;
        if(dir_r[i]==3)
          right_3_growth_direction++;
        if(dir_r[i]==1)
            r1++;
        if(dir_r[i]==2)
            r2++;
        if(dir_r[i]==3)
            r3++;
        if(dir_r[i]==4)
            r4++;
        if(dir_r[i]==5)
            r5++;
        if(dir_r[i]==6)
        {
            r6++;
            right_6_growth_direction++;
        }
        if(dir_r[i]==7)
            r7++;
        if(dir_r[i]==0)
            r8++;
    }


    /*左边判断*/
    if(left_2_growth_direction>20&&left_5_growth_direction>20)
    {
        l_growth_direction_flag=1;
    }
    else
    {
        l_growth_direction_flag=0;
    }
    if(left_3_growth_direction>20&&left_5_growth_direction>20)
    {
        l_growth_direction_flag35=1;
    }
    else
    {
        l_growth_direction_flag35=0;
    }
    if(left_6_growth_direction>25)
    {
        l_growth_direction_flag6=1;
    }
    else
    {
        l_growth_direction_flag6=0;
    }


    /*右边判断*/
    if(right_2_growth_direction>20&&right_5_growth_direction>20)
    {
        r_growth_direction_flag=1;
    }
    else
    {
        r_growth_direction_flag=0;
    }
    if(right_3_growth_direction>20&&right_5_growth_direction>20)
    {
        r_growth_direction_flag35=1;
    }
    else
    {
        r_growth_direction_flag35=0;
    }
    if(right_6_growth_direction>25)
    {
        r_growth_direction_flag6=1;
    }
    else
    {
        r_growth_direction_flag6=0;
    }
}

/*=======================================进行左边线提取========================================================*/
void get_left(uint16 total_L)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;

    //初始化
    for (i = 0;i<image_h;i++)
    {
        left[i] = border_min;

    }
    h = image_h - 2;               //从118行开始提取
    //左边
    for (j = 0; j < total_L; j++)
    {
        //printf("%d\n", j);
        if (points_l[j][1] == h)
        {
            left[h] = points_l[j][0]+1;   //提取的线在边界线的内侧

        }
        else continue; //每行只取一个点，没到下一行就不记录
        h--;
        if (h == 0)
        {
            break;//到最后一行退出
        }
    }
}
/*===========================================进行左下方丢线判断====================================================*/
void lost_left(void){
    uint8 i=0;
    left_lost_num=0;
    left_lost_num2=0;
    left_lost_num3=0;
    Lost_left_Flag=0;
    Lost_left_Flag2=0;
    Lost_left_Flag3=0;
    S_left_lost_num=0;
    S_left_lost_Flag=0;
    //1
    for(i=110;i>10;i--)
    {
        if(left[i]<=3)     //左边第2列等于白就丢线
        {
            left_lost_num++;
            Lost_point_L_scan_line=i+4;
        }
        if(left_lost_num>10)
        {
            Lost_left_Flag=1; //判断左边下方是否丢线
            break;
//            return;
        }
    }
    ///2
    for(i=100;i>40;i--)
    {
        if(left[i]<=3)     //右边第185列等于白就丢线
        {
            left_lost_num2++;
        }
        if(left_lost_num2>10)
        {
            Lost_left_Flag2=1; //判断左边下方是否丢线
            break;
//            return;
        }
    }
    ///3
    for(i=115;i>70;i--)
    {
        if(left[i]<=3)     //右边第185列等于白就丢线
        {
            left_lost_num3++;
        }
        if(left_lost_num3>10)
        {
            Lost_left_Flag3=1; //判断左边下方是否丢线
            break;
//            return;
        }
    }

    for(i=110;i>10;i--)
    {
        if(imag[i][2]==White)     //左边第2列等于白就丢线
        {
            S_left_lost_num++;
        }
        if(S_left_lost_num>10)
        {
            S_left_lost_Flag=1; //判断左边下方是否丢线
            break;
//            return;
        }
    }

}
/*=======================================进行右边线提取========================================================*/
void get_right(uint16 total_R)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;

    for (i = 0; i < image_h; i++)
    {
        right[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据

    }
    h = image_h - 2;
    //右边
    for (j = 0; j < total_R; j++)
    {
        if (points_r[j][1] == h)
        {
            right[h] = points_r[j][0] - 1;

        }
        else continue;//每行只取一个点，没到下一行就不记录
        h--;
        if (h == 0)break;//到最后一行退出
    }
}
/*===========================================进行右下方丢线判断====================================================*/
void lost_right(void){
    uint8 i=0;
    right_lost_num=0;
    Lost_right_Flag=0;
    right_lost_num2=0;
    Lost_right_Flag2=0;
    S_right_lost_num=0;
    S_right_lost_Flag=0;
    //1
    for(i=110;i>10;i--)
    {
        if(right[i]>=185)     //右边第185列等于白就丢线
        {
            right_lost_num++;
            Lost_point_R_scan_line=i+4;
        }
        if(right_lost_num>15)
        {
            Lost_right_Flag=1;  //判断右边下方是否丢线
            break;
//            return;
        }
    }
    //2
    for(i=100;i>40;i--)
    {
        if(right[i]>=185)     //右边第185列等于白就丢线
        {
            right_lost_num2++;
        }
        if(right_lost_num2>15)
        {
            Lost_right_Flag2=1;  //判断右边下方是否丢线
            break;
//            return;
        }
    }
    //3
    for(i=115;i>70;i--)
    {
        if(right[i]>=185)     //右边第185列等于白就丢线
        {
            right_lost_num3++;
        }
        if(right_lost_num3>15)
        {
            Lost_right_Flag3=1;  //判断右边下方是否丢线
            break;
//            return;
        }
    }

    for(i=110;i>10;i--)
    {
        if(imag[i][185]==White)     //右边第185列等于白就丢线
        {
            S_right_lost_num++;
        }
        if(S_right_lost_num>10)
        {
            S_right_lost_Flag=1;  //判断右边下方是否丢线
            break;
//            return;
        }
    }
    //2
}
/*===========================================进行中线提取===================================================*/
void middle_line(void)
{
    memset(middle,0,sizeof(middle));
    for(y=119;y>Endline;y--)           //有效行全部提取中线
    {
        middle[y]=(right[y]+left[y])/2;
    }
}
/*===========================================进行膨胀和腐蚀====================================================*/
#define threshold_max   255*5
#define threshold_min   255*2
void image_filter(uint8(*imag)[image_w])//形态学滤波，膨胀和腐蚀的思想
{
    uint16 i, j;
    uint32 num = 0;


    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //统计八个方向的像素值
            num =
                imag[i - 1][j - 1] + imag[i - 1][j] + imag[i - 1][j + 1]
                + imag[i][j - 1] + imag[i][j + 1]
                + imag[i + 1][j - 1] + imag[i + 1][j] + imag[i + 1][j + 1];


            if (num >= threshold_max && imag[i][j] == 0)
            {

                imag[i][j] = 255;//白  可以搞成宏定义，方便更改

            }
            if (num <= threshold_min && imag[i][j] == 255)
            {

                imag[i][j] = 0;//黑

            }

        }
    }

}

/*===========================================进行画黑框====================================================*/
void image_draw_rectan(uint8(*image)[image_w])
{

    uint8 i = 0;
    for (i = 0; i < image_h; i++)     //给左边0、1列和右边186、187列画黑框
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;

    }
    for (i = 0; i < image_w; i++)     //给上方0、1行画黑框
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;

    }
}


//==========================================================拐点识别====================================================

/*===========================================十字判断拐点====================================================*/
void ten_inflection(void)
{
    uint8 i;
    /*====清零====*/
    //十字拐点
    ten_inflexion_down_l=0;    //十字左下拐点列坐标
    ten_inflexion_down_r=0;    //十字右下拐点列坐标
    ten_inflexion_up_l=0;      //十字左上拐点列坐标
    ten_inflexion_up_r=0;      //十字右上拐点列坐标
    //十字拐点标志
    ten_inflexion_down_l_flag=0;    //十字左下拐点列坐标
    ten_inflexion_down_r_flag=0;    //十字右下拐点列坐标
    ten_inflexion_up_l_flag=0;      //十字左上拐点列坐标
    ten_inflexion_up_r_flag=0;      //十字右上拐点列坐标
    /*====清零====*/


    /*=====十字下拐点判断=====*/
    /*左下拐点判断*/
    for(i=image_h-20;i>40;i--)
    {
        if((left[i]-left[i-1]<3)&&(left[i-1]-left[i-2]<3)&&(left[i-3]-left[i-4]>3))
        {
          ten_inflexion_down_l=i;
          ten_inflexion_down_l_flag=1;
          break;
        }

    }
    /*右下拐点判断*/
    for(i=image_h-20;i>40;i--)
    {
        if((right[i]-right[i-1]<3)&&(right[i-2]-right[i-1]<3)&&(right[i-4]-right[i-3]>3))
        {
           ten_inflexion_down_r=i;
           ten_inflexion_down_r_flag=1;
           break;
        }

    }
    /*=====十字上拐点判断=====*/
    /*左上拐点判断*/
    for(i=image_h-15;i>60;i--)
    {
        if((left[i]-left[i+1]<3)&&(left[i+1]-left[i+2]<3)&&(left[i+3]-left[i+4]>3))
        {
            ten_inflexion_up_l=i;
            ten_inflexion_up_l_flag=1;
            break;
        }

    }
    /*右上拐点判断*/
    for(i=image_h-15;i>60;i--)
    {
        if((right[i+1]-right[i]<3)&&(right[i+2]-right[i+1]<3)&&(right[i+4]-right[i+3]>3))
        {
           ten_inflexion_up_r=i;
           ten_inflexion_up_r_flag=1;
           break;
        }

    }
}
//---------------------------------左下拐点--------------------------------
//-----------------------------第二版：用断点判断------------------------
void Lower_left(void){
    Lower_left_inflection_Flag=0;
    Lower_left_inflection_X =0;
    Lower_left_inflection_Y =0;

        for(y=image_h-3;y>Endline+10;y--){
            if(y>30){
                if((left[y]-left[y-4])>5&&left[y-4]==2&&(left[y]-left[y+2])<5&&left[y]>10)
                {
                    Lower_left_inflection_Flag=1;
                    Lower_left_inflection_X =(uint8)left[y];
                    Lower_left_inflection_Y =(uint8)y;
                    return;
                }
            }

         }
    //----------------------------第一版：用扫线方法判断--------------------------------
    //    for(y=110;y>(Endline+10);y--){
    //        for(x=left[y+1];x<186&&x>1;x++){
    //              if(imag[y][x-1]==Black&&imag[y][x]==Black&&imag[y][x+1]==White){
    //                 Lower_left_inflection_X =x;
    //                 Lower_left_inflection_Y =y;
    //              //  ips200_draw_point(Lower_left_inflection_X, Lower_left_inflection_Y , RGB565_RED);
    //                 break;
    //              }
    //              if(x>(left[y-1]+3)&&imag[y][x]==White){
    //                  Lower_left_inflection_Flag=1;
    //                  X1=Lower_left_inflection_X;
    //                  Y1=Lower_left_inflection_Y;
    //                  break;
    //                  }
    //            }
    //
    //
    //        }
}

//-------------------------------------右下拐点---------------------------------
//-----------------------------第二版：用断点判断------------------------
void Lower_right(void){
    Lower_right_inflection_Flag=0;
    Lower_right_inflection_X =0;
    Lower_right_inflection_Y =0;
    for(y=image_h-3;y>(Endline+10);y--){
        if(y>30){
            if((right[y-4]-right[y])>5&&right[y-4]==185&&(right[y+2]-right[y])<5&&right[y]<177){
                Lower_right_inflection_Flag=1;
                Lower_right_inflection_X =(uint8)right[y];
                Lower_right_inflection_Y =(uint8)y;
                return;
            }
        }

     }
    //----------------------------第一版：用扫线方法判断--------------------------------
    //（可对右下拐点识别，但是右转弯时的拐角也会被判定）
    //        for(y=115;y>(Endline+15);y--){
    //            for(x=right[y+1];x<186&&x>1;x--){
    //                  if(imag[y][x-1]==White&&imag[y][x]==Black&&imag[y][x+1]==Black){
    //                     Lower_right_inflection_X =x;
    //                     Lower_right_inflection_Y =y;
    //                    // ips200_draw_point(Lower_right_inflection_X, Lower_right_inflection_Y , RGB565_RED);
    //                     break;
    //                  }
    //                  if(x<(right[y-1]-3)&&imag[y][x]==White){
    //                      Lower_right_inflection_Flag=1;
    //                      break;
    //                      }
    //                }
    //
    //            }

}


//-------------------------------------左上拐点------------------------------------
//-----------------------------第二版：用断点判断------------------------
void Upper_left(void){
    uint8 h=image_h-3;   //h=117主要是给常规找上拐点用的
//    uint8 i;

    Upper_left_inflection_Flag=0;
    Upper_left_inflection_X =0;
    Upper_left_inflection_Y =0;
    if(Lost_left_Flag==1){
        //针对圆环写的找点方式
          if(annulus_L_Flag==1)
          {
              for(h=Lost_point_L_scan_line+5;h>(Endline+10);h--)
              {
                if((left[h]-left[h+8])>3&&left[h+8]==2&&left[h]!=2&&(left[h-4]-left[h])<3&&h<60&&left[h]>10)
                {
                   if(left[h]>93)
                   {            //针对圆环状态4补线出现的断层问题进行的尝试优化
                       Upper_left_inflection_Flag=1;
                       Upper_left_inflection_X =(uint8)left[h];
                       Upper_left_inflection_Y =h;

                       //-----------第一版：用扫线方法判断-----------
                       uint8 Find_Flag=0;
                       for(y=h;y<110;y++){
                           Find_Flag=0;
                           for(x=left[h]+10;x>70;x--)
                           {
                                 if(imag[y][x-1]==Black&&imag[y][x]==Black&&imag[y][x+1]==White)
                                 {
                                     Find_Flag=1;
                                    break;
                                 }

                           }
                           if(Find_Flag==0)
                           {
                               Upper_left_inflection_Y =y-1;
                               return;
                               }

                           }

                   }
                   else{
                       Upper_left_inflection_Flag=1;
                       Upper_left_inflection_X =(uint8)left[h];
                       Upper_left_inflection_Y =h;
                       return;
                   }
                }
             }
          }
          //这是常规的找上拐点
          else{
              for(h=Lost_point_L_scan_line+3;h>(Endline+10);h--){
                if((left[h]-left[h+4])>3&&left[h+10]==2&&left[h]!=2&&(left[h-1]-left[h])<3&&h<60&&left[h]>10){

                       Upper_left_inflection_Flag=1;
                       Upper_left_inflection_X =(uint8)left[h];
                       Upper_left_inflection_Y =h;
                       return;

                }
             }
          }

    }
    //----------------------------第一版：用扫线方法判断--------------------------------
    //        for(y=Endline+15;y<110;y++){
    //            for(x=left[y-1];x<186&&x>1;x--){
    //                  if(imag[y][x-1]==Black&&imag[y][x]==Black&&imag[y][x+1]==White){
    //                     Upper_left_inflection_X =x;
    //                     Upper_left_inflection_Y =y;
    //                     //ips200_draw_point(Upper_left_inflection_X, Upper_left_inflection_Y , RGB565_BLUE );
    //                     break;
    //                  }
    //                  if(x<(left[y-1]-3)){
    //                      Upper_left_inflection_Flag=1;
    //                      break;
    //                      }
    //                }
    //
    //
    //            }

}

//-----------------------------------右上拐点-----------------------------------
//-----------------------------第二版：用断点判断------------------------
void Upper_right(void){
    uint8 h=image_h-3;
    Upper_right_inflection_Flag=0;
    Upper_right_inflection_X =0;
    Upper_right_inflection_Y =0;
    if(Lost_right_Flag==1){
        //针对圆环写的找点方式
          if(annulus_R_Flag==1)
          {
              for(h=Lost_point_R_scan_line+5;h>(Endline+10);h--)
              {
                if((right[h+8]-right[h])>3&&right[h+8]==185&&right[h]!=185&&(right[h]-right[h-4])<3&&h<60&&right[h]<177)
                {
                   if(right[h]<93) {            //针对圆环状态4补线出现的断层问题进行的尝试优化
                       Upper_right_inflection_Flag=1;
                       Upper_right_inflection_X =(uint8)right[h];
                       Upper_right_inflection_Y =h;
                       //-----------第一版：用扫线方法判断-----------
                       uint8 Find_Flag=0;
                       for(y=h;y<110;y++){
                           Find_Flag=0;
                           for(x=right[h]+10;x>70;x--){
                                 if(imag[y][x-1]==White&&imag[y][x]==Black&&imag[y][x+1]==Black){
                                     Find_Flag=1;
                                    break;
                                 }

                               }
                           if(Find_Flag==0){
                               Upper_right_inflection_Y =y-1;
                               return;
                               }

                           }

                   }
                   //没有越过中线的上拐点
                   else{
                       Upper_right_inflection_Flag=1;
                       Upper_right_inflection_X =(uint8)right[h];
                       Upper_right_inflection_Y =h;
                       return;
                   }
                }
             }
          }
          //常规找上拐点
          else{
              for(h=Lost_point_R_scan_line+5;h>(Endline+10);h--){

                      if((right[h+3]-right[h])>15&&right[h+10]==185&&right[h]!=185&&(right[h]-right[h-1])<3&&h<60&&right[h]<177){
                          Upper_right_inflection_Flag=1;
                          Upper_right_inflection_X =(uint8)right[h];
                          Upper_right_inflection_Y =h;
                          return;

                  }

              }
          }

    }
    //----------------------------第一版：用扫线方法判断--------------------------------
    //        for(y=Endline+15;y<110;y++){
    //            for(x=right[y-1];x<186&&x>1;x++){
    //                  if(imag[y][x-1]==White&&imag[y][x]==Black&&imag[y][x+1]==Black){
    //                     Upper_right_inflection_X =x;
    //                     Upper_right_inflection_Y =y;
    //                    // ips200_draw_point(Upper_right_inflection_X, Upper_right_inflection_Y , RGB565_BLUE );
    //                     break;
    //                  }
    //                  if(x>(right[y-1]+3)){
    //                      Upper_right_inflection_Flag=1;
    //                      break;
    //                      }
    //                }
    //            }
}
void left_S_road_inflection(void)
{
  uint8 i=0;
  S_left_inflection_Flag=0;
    for(y=image_h-3;y>Endline+10;y--)
    {
        if((left[y]-left[y-8])>3&&(left[y]-left[y+8])>3&&left[y]>10)
          {
            i++;
          }
        if(i>1)
          {
            S_left_inflection_Flag=1;
          }

    }
}
void right_S_road_inflection(void)
{
    uint8 i=0;
    S_right_inflection_Flag=0;
      for(y=image_h-3;y>Endline+10;y--)
      {
          if((right[y-8]-right[y])>3&&(right[y+8]-right[y])>3&&right[y]<177)
            {
              i++;
            }
          if(i>1)
            {
              S_right_inflection_Flag=1;
            }

      }
}

//--------拐点总判断-----------
void inflection_point(void){

    Upper_left();
    Upper_right();
    Lower_left();
    Lower_right();
    left_S_road_inflection();
    right_S_road_inflection();

}

float r_k=0,l_k=0;
//==================================================右直线识别===========================================================
void right_straight(void){
        float k1,k2;
        Right_straight_flag=0;
        float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
        caculate_distance(80,100,right,&l_slope3,&l_distance3);
        k2=l_slope3;
        caculate_distance(25,45,right,&l_slope2,&l_distance2);
        k1=l_slope2;
        if(absolute(k1-k2)<0.1)
        Right_straight_flag=1;
        r_k=absolute(k1-k2);
}

//==================================================左直线识别===========================================================
void left_straight(void)
{
    float k1,k2;
    Left_straight_flag=0;
    float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
    caculate_distance(80,100,left,&l_slope3,&l_distance3);
    k2=l_slope3;
    caculate_distance(25,45,left,&l_slope2,&l_distance2);
    k1=l_slope2;
    if(absolute(k1-k2)<0.1)
    Left_straight_flag=1;
    l_k=absolute(k1-k2);

}


//=================================================十字识别=======================================================
void crossroad(void){

/*-----------------------第二版十字补线--------------------------------*/
    int l;
    uint8 start,end;           //存放拟合直线的点个列坐标
    float slope=0,distance=0;  //存放斜率和截距
    if(bend_straight_flag==0&&annulus_L_Flag==0&&annulus_R_Flag==0&&Lost_left_Flag==1&&Lost_right_Flag==1&&Lost_left_Flag3==1&&Lost_right_Flag3==1&&Crossroad_Flag==0&&Crossroad_memory==0&&ten_inflexion_down_l_flag==1&&ten_inflexion_down_r_flag==1)
    {
       Crossroad_Flag=1;
       Crossroad_memory=1;
       beep_on();
    }
    if(Crossroad_Flag==1)
    {
        /*==================进十字路口前进行补线(调用斜率截距函数和最小二乘法函数)=======================*/
        if(Crossroad_memory==1)
        {
            /*===左边补线===*/
            start=ten_inflexion_down_l+3;
            end=ten_inflexion_down_l+15;
            caculate_distance(start,end,left,&slope,&distance);
            for(l=ten_inflexion_down_l;l>2;l--)
            {
                if((slope*(l)+distance)>185)
                    left[l]=185;
                else if((slope*(l)+distance)<2)
                    left[l]=2;
                else
                    left[l]=slope*(l)+distance;
            }
            /*===右边补线===*/
            start=ten_inflexion_down_r+3;
            end=ten_inflexion_down_r+15;
            caculate_distance(start,end,right,&slope,&distance);
            for(l=ten_inflexion_down_r;l>2;l--)
            {
                if((slope*(l)+distance)>185)
                    right[l]=185;
                else if((slope*(l)+distance)<2)
                    right[l]=2;
                else
                    right[l]=slope*(l)+distance;
            }

            /*====判断是否进行状态二上拐点补线===*/
            if(ten_inflexion_up_l_flag==1&&ten_inflexion_up_r_flag==1)
            {
                Crossroad_memory=2;
            }
            if(Lost_right_Flag3==0||Lost_left_Flag3==0)
            {
                Crossroad_Flag=0;
                Crossroad_memory=0;
                beep_off();
            }
        }

        /*============进十字路口后进行补线(调用斜率截距函数和最小二乘法函数)===============*/
        if(Crossroad_memory==2)
        {
            /*===左边补线===*/
            start=ten_inflexion_up_l-15;
            end=ten_inflexion_up_l-3;
            caculate_distance(start,end,left,&slope,&distance);
            for(l=ten_inflexion_up_l;l<119;l++)
            {
                if((slope*(l)+distance)>185)
                    left[l]=185;
                else if((slope*(l)+distance)<2)
                    left[l]=2;
                else
                    left[l]=slope*(l)+distance;
            }
            /*===右边补线===*/
            start=ten_inflexion_up_r-15;
            end=ten_inflexion_up_r-3;
            caculate_distance(start,end,right,&slope,&distance);
            for(l=ten_inflexion_up_r;l<119;l++)
            {
                if((slope*(l)+distance)>185)
                    right[l]=185;
                else if((slope*(l)+distance)<2)
                    right[l]=2;
                else
                    right[l]=slope*(l)+distance;
            }
            /*====判断是否结束十字状态==*/
            if(ten_inflexion_up_l_flag==0||ten_inflexion_up_r_flag==0||Lost_right_Flag3==0||Lost_left_Flag3==0)
            {
                Crossroad_Flag=0;
                Crossroad_memory=0;
                beep_off();
            }
        }

    }

/*-----------------------第一版十字补线--------------------------------*/
//    //提前十字处理
//    if(annulus_L_Flag==0&&annulus_R_Flag==0&&Lost_left_Flag==1&&Lost_right_Flag==1&&Lower_left_inflection_Flag==1&&Lower_right_inflection_Flag==1&&Crossroad_Flag==0&&Crossroad_memory==0&&Endline<5){
//        Crossroad_Flag=1;
//        Crossroad_memory=1;
//    }
//    if(Crossroad_Flag==1)
//    {
//        //状态1 看到十字，和左右下面两个拐点
//       if(Crossroad_memory==1)
//       {
//          //方案一：强行补线
////           Addingline( 1, 45,55, 3, 117);
////           Addingline( 2, 119,55, 186, 117);
//          //方案二：延斜率补线
//
//           if(Lower_left_inflection_Flag==1)
//           {
//               Addingline1(1,Lower_left_inflection_X,Lower_left_inflection_Y);
//           }
//           else{
//               Addingline( 1, 45,55, 3, 117);
//           }
//
//           if(Lower_right_inflection_Flag==1){
//               Addingline1(2,Lower_right_inflection_X,Lower_right_inflection_Y);
//           }
//           else{
//               Addingline( 2, 119,55, 186, 117);
//           }
//
//        if ( Upper_left_inflection_Flag == 1 && Upper_right_inflection_Flag == 1)
//            {
//            Crossroad_memory=2;
//            }
//         }
//       //状态2 看到左上拐点和右上拐点，向下拉线
//       if(Crossroad_memory==2){
//           if(Upper_left_inflection_Flag==1&&Upper_right_inflection_Flag==1){
//               Addingline( 1, Upper_left_inflection_X, Upper_left_inflection_Y, 5, 118);
//               Addingline( 2,Upper_right_inflection_X, Upper_right_inflection_Y, 184,118);
//           }
//           else{
//               Addingline( 1, 45,55, 3, 117);
//               Addingline( 2, 119,55, 186, 117);
//           }
//           if(Upper_left_inflection_Flag==0&&Upper_right_inflection_Flag==0){
//               Crossroad_memory=3;
//           }
//           else return;
//       }
//       //状态3 在十字中行驶,出环遇拐点补线
//       if(Crossroad_memory==3){
////           if(Lower_left_inflection_Flag==1){
////               Addingline1(1,Lower_left_inflection_X,Lower_left_inflection_Y);
////           }
////           if(Lower_right_inflection_Flag==1){
////               Addingline1(2,Lower_right_inflection_X,Lower_right_inflection_Y);
////           }
//           if(Upper_left_inflection_Flag==1&&Upper_right_inflection_Flag==1){
//               Crossroad_memory=4;
//           }
//       }
//       //状态4 补线出十字
//       if(Crossroad_memory==4){
//           if(Upper_left_inflection_Flag==1&&Upper_right_inflection_Flag==1){
//               Addingline( 1, Upper_left_inflection_X, Upper_left_inflection_Y, 5, 118);
//               Addingline( 2,Upper_right_inflection_X, Upper_right_inflection_Y, 184,118);
//           }
//           else{
//               Addingline( 1, 45,55, 3, 117);
//               Addingline( 2, 119,55, 186, 117);
//           }
//           //退出
//           if(Upper_left_inflection_Flag==0||Upper_right_inflection_Flag==0){
//               Crossroad_Flag=0;
//               Crossroad_memory=0;
//               return;
//           }
//       }
//
//  }
}

//=============================================圆环===================================================
//================环岛识别（环岛中间凸起处判断）=====================
void roundabout_L(void){

    roundabout_X=0;
    roundabout_Y=0;
    roundabout_Flag=0;
    for(y=image_h-3;y>10;y--){
        if((left[y]-left[y-8])>5&&left[y-8]==2&&(left[y]-left[y+2])<5&&left[115]==2&&left[117]==2&&Lost_left_Flag==1){
            y+=4;
            roundabout_Flag=1;
            roundabout_X =(uint8)left[y];
            roundabout_Y =(uint8)y;
            return;
        }
     }

}
//================环岛识别（环岛中间凸起处判断）=====================
void roundabout_R(void){
    roundabout_X=0;
    roundabout_Y=0;
    roundabout_Flag=0;
    for(y=image_h-3;y>10;y--){
        if((right[y-8]-right[y])>5&&right[y-8]==185&&(right[y+2]-right[y])<5&&right[115]==185&&right[117]==185&&Lost_right_Flag==1){
            y+=4;
            roundabout_Flag=1;
            roundabout_X =(uint8)right[y];
            roundabout_Y =(uint8)y;
            return;
        }
     }

}
//================出环拐点识别=====================
void Exit_loop_L_inflection(void){
   uint8 i;
   Exit_loop_Flag=0;
   Exit_loop_X=0;
   Exit_loop_Y=0;
  //左圆环，识别右边拐点
    for(i=110;i>Endline+10;i--){
        if(right[i]<right[i+4]&&right[i]<right[i-4]&&right[i]<right[i+3]&&right[i]<right[i-3]){
            Exit_loop_Flag=1;
            Exit_loop_X=(uint8)right[i];
            Exit_loop_Y=i;
            return;
        }
    }
}
//================出环拐点识别=====================
void Exit_loop_R_inflection(void){
   uint8 i;
   Exit_loop_Flag=0;
   Exit_loop_X=0;
   Exit_loop_Y=0;
  //右圆环，识别右边拐点
    for(i=110;i>Endline+10;i--){
        if(left[i+4]<left[i]&&left[i-4]<left[i]&&left[i+3]<left[i]&&left[i-3]<left[i]){
            Exit_loop_Flag=1;
            Exit_loop_X=(uint8)left[i];
            Exit_loop_Y=i;
            return;
        }
    }
}

//================左圆环识别=====================
void annulus_L(void){

    //识别圆环//&&Upper_left_inflection_Flag==0&&&&imag[Lower_left_inflection_Y][3]==White//&&Lost_left_Flag2==1&&Upper_left_inflection_Flag==0
      if(l_growth_direction_flag==1&&r_growth_direction_flag6==0&&annulus_R_Flag==0&&Crossroad_Flag==0&&Lost_left_Flag==1&&Lost_right_Flag2==0&&Lost_left_Flag2==1&&Lower_left_inflection_Flag==1&&Right_straight_flag==1&&Lower_right_inflection_Flag==0&&annulus_L_memory==0&&annulus_L_Flag==0){
          annulus_L_Flag=1;
          annulus_L_memory =1;
          annulus_R_Flag=0;
          annulus_R_memory =0;
          Crossroad_Flag=0;
          Crossroad_memory=0;
      }

     if(annulus_L_Flag==1){

         //状态 1  识别到圆环，未识别到环岛（可强行补线，或不补线，待测试）
         if (annulus_L_memory == 1)
         {
             if(Lower_left_inflection_Flag==1){
                 Addingline1( 1, Lower_left_inflection_X, Lower_left_inflection_Y);
                //Addingline( 1, Lower_left_inflection_X, Lower_left_inflection_Y,2 , 118 );
                //roundabout();
             }
             else if(Lower_left_inflection_Flag==0){
                 annulus_L_memory = 2;
             }
         }
         //状态2 识别到圆环，环岛，并对左边进行补线
          if (annulus_L_memory == 2 )
         {
              gyro_start=jd_sum;
              roundabout_L();          //环岛
            if(roundabout_Flag==1){
                Addingline( 1, roundabout_X, roundabout_Y,points_l[1][0], points_l[1][1] );

             }

            else {
 //               Addingline( 1, 56, 38,points_l[1][0], points_l[1][1]);
            }
            if(Upper_left_inflection_Flag==1&&Upper_left_inflection_Y>27){   //改变Upper_left_inflection_Y限制间接改变入环位置，待验证
                annulus_L_memory = 3;
            }
            else return;
         }
         //状态3 到达圆环入口，封住前路，补线入环
          if (annulus_L_memory == 3 )
          {
            //  beep_on();
              if(Upper_left_inflection_Flag==1){
                  //Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,right[119] , 119);
                  if(Upper_left_inflection_X>25){
                      Addingline( 2, Upper_left_inflection_X-15,Upper_left_inflection_Y,(uint8)right[119] , 119);
                  }
                  else{
                      Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,(uint8)right[119] , 119);

                  }
                  Addingline( 2, 2,Endline,2 , Upper_left_inflection_Y-1);
                  Addingline( 1, 2,Endline,2 , Upper_left_inflection_Y-1);
                  return;
              }
              else if(Upper_left_inflection_Flag==0&&Lost_left_Flag==1&&Endline>30){
                  annulus_L_memory = 4;
              }
              else return;

          }
         //状态4 在圆环中行驶，当看到右下拐点时进入下一状态
         if (annulus_L_memory == 4)
         {
             gyro_end=jd_sum;
            Exit_loop_L_inflection();
            if(Exit_loop_Flag==0){
                return;
            }//&&(gyro_end-gyro_start)>250Exit_loop_Flag==1&&Lost_left_Flag==1
            if((gyro_end-gyro_start)>240){
                annulus_L_memory = 5;
            }

         }

         //状态5 出环时看到右下拐点,对其补线处理
         if (annulus_L_memory == 5 )
          {
             gyro_end=jd_sum;
             Exit_loop_L_inflection();
             if(Exit_loop_Flag==1){
                 if(Exit_loop_Y>55){
                     Addingline( 2, 3, Exit_loop_Y-60, Exit_loop_X, Exit_loop_Y);//此处补线结束点待测试
                     return;
                 }
                 else {
                     Addingline( 2, 3, 50, Exit_loop_X, Exit_loop_Y);//此处补线结束点待测试
                     return;
                 }
             }
             else if(Lost_left_Flag==1&&Lost_right_Flag==1&&Exit_loop_Flag==0){
                 annulus_L_memory = 6;
             }
             else return;

          }

         //状态6 出环时右下拐点消失，但是车还没完全出环，此时还需要补线处理
         if (annulus_L_memory == 6 )
          {
             gyro_end=jd_sum;
             if(Lost_left_Flag==1&&Lost_right_Flag==1){
                 Addingline( 2, 3, Endline+5, 185, 117);
                // return;
             }
             //||(Lost_left_Flag==1&&Lost_right_Flag==0)
             if(((gyro_end-gyro_start)>280)){
                 annulus_L_memory = 7;
             }
             else return;
          }
         //状态7 出环补线
         if (annulus_L_memory == 7)
          {
             gyro_end=jd_sum;
             if(Upper_left_inflection_Flag==1){
                 Addingline( 1, Upper_left_inflection_X, Upper_left_inflection_Y, 10, 118);
                 return;
             }
             else{
                 Addingline( 1, 56, 38, points_l[1][0], points_l[1][1]);
             }
             if((gyro_end-gyro_start)>338)/*||((gyro_end-gyro_start)<-300)*///||((gyro_end-gyro_start)<-300)||(Lost_right_Flag==0&&Lost_left_Flag==0
             {
                 annulus_L_memory =0;
                 annulus_L_Flag=0;
                 return;
             }
          }

     }
}

//================右圆环识别=====================
void annulus_R(void){

    //识别圆环//&&Upper_right_inflection_Flag==0//&&Lost_right_Flag2==1&&Upper&&Upper_right_inflection_Flag==0&&_right_inflection_Flag==0
      if(r_growth_direction_flag==1&&l_growth_direction_flag6==0&&Upper_right_inflection_Flag==0&&annulus_L_Flag==0&&Crossroad_Flag==0&&Lost_right_Flag==1&&Lost_right_Flag2==1&&Lost_left_Flag2==0&&Lower_right_inflection_Flag==1&&Left_straight_flag==1&&Lower_left_inflection_Flag==0&&annulus_R_memory==0&&annulus_R_Flag==0){
          annulus_R_Flag=1;
          annulus_R_memory =1;
          annulus_L_Flag=0;
          annulus_L_memory =0;
          Crossroad_Flag=0;
          Crossroad_memory=0;
      }

     if(annulus_R_Flag==1){


         //状态 1  识别到圆环，未识别到环岛（可强行补线，或不补线，待测试）
         if (annulus_R_memory == 1)
         {
             if(Lower_right_inflection_Flag==1)
             {
//                 breadth_Addingline(2);
                 Addingline1( 2, Lower_right_inflection_X, Lower_right_inflection_Y);

                //Addingline( 1, Lower_left_inflection_X, Lower_left_inflection_Y,2 , 118 );
                //roundabout();
             }
             else if(Lower_left_inflection_Flag==0){
                 annulus_R_memory = 2;
             }


         }
         //状态2 识别到圆环，环岛，并对左边进行补线
          if (annulus_R_memory == 2 )
         {
              gyro_start=jd_sum;
              roundabout_R();          //环岛
            if(roundabout_Flag==1)
            {
//                breadth_Addingline(2);
                Addingline( 2, roundabout_X, roundabout_Y,187 , 118 );

             }
            else
            {
//                breadth_Addingline(2);
 //
                Addingline( 2, 126, 50,points_r[1][0], points_r[1][1]);  //要改
            }
            if(Upper_right_inflection_Flag==1&&Upper_right_inflection_Y>30){   //改变Upper_right_inflection_Y限制间接改变入环位置，待验证
                annulus_R_memory = 3;
            }
            else return;
         }
         //状态3 到达圆环入口，封住前路，补线入环
          if (annulus_R_memory == 3 )
          {
             // beep_on();
              if(Upper_right_inflection_Flag==1){
                  //Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,right[119] , 119);
                  if(Upper_right_inflection_X<162)
                  {
                      Addingline( 1, Upper_right_inflection_X+40,Upper_right_inflection_Y,(uint8)left[119] , 119);
                  }
                  else{
                      Addingline( 1, Upper_right_inflection_X,Upper_right_inflection_Y,(uint8)left[119] , 119);

                  }
                  Addingline( 2, image_w - 2,Endline,image_w - 2 , Upper_right_inflection_Y-1);
                  Addingline( 1, image_w - 2,Endline,image_w - 2 , Upper_right_inflection_Y-1);
                  return;
              }
              else if(Upper_right_inflection_Flag==0&&Lost_right_Flag==1&&Endline>30){
                  annulus_R_memory = 4;
              }
              else return;

          }
         //状态4 在圆环中行驶，当看到右下拐点时进入下一状态
         if (annulus_R_memory == 4)
         {
           gyro_end=jd_sum;
            Exit_loop_R_inflection();
            if(Exit_loop_Flag==0){
                return;
            }//;Exit_loop_Flag==1&&Lost_left_Flag==1
            if((gyro_end-gyro_start)<-240){
                annulus_R_memory = 5;
            }

         }

         //状态5 出环时看到右下拐点,对其补线处理
         if (annulus_R_memory == 5 )
          {
             Exit_loop_R_inflection();
             if(Exit_loop_Flag==1){
                 if(Exit_loop_Y>60){
                     Addingline( 1, 187, Exit_loop_Y-60, Exit_loop_X, Exit_loop_Y);//此处补线结束点待测试
                     return;
                 }
                 else {
                     Addingline( 1, 187, 50, Exit_loop_X, Exit_loop_Y);//此处补线结束点待测试
                     return;
                 }
             }
             else if(Lost_right_Flag==1&&Lost_left_Flag==1&&Exit_loop_Flag==0){
                 annulus_R_memory = 6;
             }
             else return;

          }

         //状态6 出环时右下拐点消失，但是车还没完全出环，此时还需要补线处理
         if (annulus_R_memory == 6 )
          {
             gyro_end=jd_sum;
             if(Lost_right_Flag==1&&Lost_left_Flag==1){
                 Addingline( 1,187, Endline+5, 2, 117);
               //  return;
             }
             //(Lost_right_Flag==1&&Lost_left_Flag==0)||
             if((gyro_end-gyro_start)<-286)//320
             {
                 annulus_R_memory = 7;
             }
             else return;
          }
         //状态7 出环补线
         if (annulus_R_memory == 7)
          {
             gyro_end=jd_sum;
             if(Upper_right_inflection_Flag==1)
             {
                 Addingline( 2, Upper_right_inflection_X, Upper_right_inflection_Y, 175, 118);
                // return;
             }
             else
             {
                 Addingline( 2, 120, 38, 184, 118); // 要改
             }
             if((gyro_end-gyro_start)<-340){
                 annulus_R_memory =0;
                 annulus_R_Flag=0;
                 return;
             }

          }

     }
}


//===================================================左车库===================================================
//=========斑马线===========
void zebra_crossing(void){
      uint8 i;
//      uint8 num1;
//      uint8 num2;
      uint8 num3;
      uint8 num4;

      zebra_crossing_flag=0;
//      num1=0;
//      num2=0;
      num3=0;
      num4=0;
//
//      for(i=3;i<185;i++)
//      {
//          if(imag[Endline+5][i-1]==White&&imag[Endline+5][i]==Black&&imag[Endline+5][i+1]==Black){
//              num4++;
//          }
//
//          if(num1>5){
//              zebra_crossing_flag=1;
//          }
//      }
//
//      for(i=3;i<185;i++)
//      {
//          if(imag[Endline+15][i-1]==White&&imag[Endline+15][i]==Black&&imag[Endline+15][i+1]==Black){
//              num1++;
//          }
//
//          if(num1>5){
//              zebra_crossing_flag=1;
//          }
//      }
//
//      for(i=3;i<185;i++)
//      {
//          if(imag[60][i-1]==White&&imag[60][i]==Black&&imag[60][i+1]==Black){
//              num2++;
//          }
//
//          if(num2>5){
//              zebra_crossing_flag=1;
//          }
//      }

      for(i=3;i<185;i++)
      {//imag[100][i-1]==White&&imag[100][i]==Black&&imag[100][i+1]==Black
          if(imag[100][i-1]!= imag[100][i])
          {
              num3++;
          }
          if(imag[99][i-1]!= imag[99][i])
          {
              num4++;
          }

          if(num3>6||num4>6){
              zebra_crossing_flag=1;
          }
      }

}

//车库识别
void Garage(void){
    zebra_crossing();
    Left_garage_flag=0;
    if(zebra_crossing_flag==1)
    {
        annulus_L_Flag=0;
        annulus_L_memory=0;
        Left_garage_flag=1;
    }
    switch(Left_garage_memory){
        case 0:  //出库
            if(Lower_left_inflection_Flag==0&&Lower_right_inflection_Flag==0){
                Addingline( 2, 3, Endline+3, 184, 117);   //调节此处可调节舵机拐角
            }
            if(Lost_left_Flag==0&&Lost_right_Flag==0&&Lower_left_inflection_Flag==0&&Lower_right_inflection_Flag==0&&Endline<5){
                Left_garage_memory=1;
            }
            break;
        case 1:  //出库后正常行驶
            if(Left_garage_flag==1){
                Left_garage_memory=4;
            }
            break;
        case 2:  //第一次看到车库，补线过库
//            speed1=0.5;
//            speed2=0.5;
            if(Upper_left_inflection_Flag==1){
                Addingline2(1,Upper_left_inflection_X,Upper_left_inflection_Y);
            }
            else if(Lower_left_inflection_Flag==1){
                Addingline1(1,Lower_left_inflection_X,Lower_left_inflection_Y);
            }
            else{
                Addingline( 1, 29,67, 55, 25);  //根据实际调整
            }
            if(Lost_left_Flag==0&&Left_garage_flag==0&&Upper_left_inflection_Flag==0&&zebra_crossing_flag==0&&Lower_left_inflection_Flag==0){
                Left_garage_memory=3;
            }

            break;
        case 3:  //正常行驶
//            speed1=1.5;
//            speed2=1.5;
            if(Left_garage_flag==1){
                Left_garage_memory=4;

            }
            break;
        case 4: //再次看到车库时补线入库
//            speed1=-1.7;
//            speed2=-1.7;
            Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,(uint8)right[119] , 119);
            if((Lost_left_Flag==0&&Lost_right_Flag==0)||Endline>50){
                Left_garage_memory =5;
            }
            break;
        case 5: //刹车
//            speed1=-2.2;
//            speed2=-2;
//            if(Encoder_speed1<-5){
                Left_garage_memory=7;
//            }
            break;
        case 6://刹车
//            speed1=-0.4;
//            speed2=-0.2;
//            if(Encoder_speed1<0){
                Left_garage_memory=7;
//            }
            break;
        case 7:
//            speed1=-0.01;
//            speed2=0;
            break;
    }

}
//---------------------------------------线性拟合---------------------------------
void advanced_regression(int type, int startline1, int endline1, int startline2, int endline2)
 {
    int i = 0;
    int sumlines1 = endline1 - startline1;
    int sumlines2 = endline2 - startline2;
    int sumX = 0;
    int sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;
    float sumDown = 0;
    if (type == 0)  //拟合中线
    {
        /**计算sumX sumY**/
        for (i = startline1; i <=endline1; i++)
        {
            sumX += i;
            sumY += middle[i];
        }
        for (i = startline2; i <=endline2; i++)
        {
            sumX += i;
            sumY += middle[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x的平均值
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y的平均值
        for (i = startline1; i <= endline1; i++)
        {
            sumUp += (middle[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumUp += (middle[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;

    }
    else if (type == 1)     //拟合左线
    {
        /**计算sumX sumY**/
        for (i = startline1; i <= endline1; i++)
        {
            sumX += i;
            sumY += left[i];
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumX += i;
            sumY += left[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x的平均值
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y的平均值
        for (i = startline1; i <= endline1; i++)
        {
            sumUp += (left[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumUp += (left[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
    else if (type == 2)         //拟合右线
    {
        /**计算sumX sumY**/
        for (i = startline1; i <= endline1; i++)
        {
            sumX += i;
            sumY += right[i];
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumX += i;
            sumY += right[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x的平均值
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y的平均值
        for (i = startline1; i <= endline1; i++)
        {
            sumUp += (right[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumUp += (right[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
}
//------------------------------判断直线斜率是否相同------------------------------
int Judgment_symbol(float x, float y)
{
    int a;
    a = 0;
    if (x < 0 && y < 0) a = 1;
    if (x >= 0 && y >= 0) a = 1;
    return a;
}
//-----------------------------------------补线-----------------------------------
/*                    ===使用赛道宽度补线====                                */
void breadth_Addingline(uint8 choice)
{
    uint8 i;
    switch(choice)
    {
        case 1://左补线
            for(i=119;i>=2;i--)
            {
                if(right[i]-road_width[i]<2)
                    left[i]=2;
                else
                    left[i]=right[i]-road_width[i];
            }
            break;
        case 2://右补线
            for(i=119;i>=2;i--)
            {
                if(left[i]+road_width[i]>185)
                    right[i]=185;
                else
                    right[i]=left[i]+road_width[i];
            }
            break;
    }
}
//------------------------------------环岛状态二、三、五补线-----------------------------------
//有起点和终点的补线函数
void Addingline( uint8 choice, uint8 startX, uint8 startY, uint8 endX, uint8 endY)
{
    y = 0;

    // 直线 x = ky + b
    float k = 0;
    float b = 0;
    switch(choice)
    {
      case 1://左补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(y = startY; y < endY; y++)
            {
                if( (uint8)(k * y + b)>185)
                    left[y] = 185;
                else if( (uint8)(k * y + b)<2)
                    left[y] = 2;
                else
                   left[y] = (uint8)(k * y + b);
            }
            break;
        }

      case 2://右补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(y = startY; y < endY; y++)
            {
                if( (uint8)(k * y + b)>185)
                    right[y]=185;
                else if ( (uint8)(k * y + b)<2)
                    right[y]=2;
                else
                right[y]= (uint8)(k * y + b);

            }
            break;
        }

    }
}

//-----------------------------------------环岛状态一补线-----------------------------------
void Addingline1( uint8 choice, uint8 startX, uint8 startY)    //看到拐点延斜率向上延长
{

    // 直线 x = ky + b
    float k = 0;
    float b = 0;
    int temp=0;
    switch(choice)
    {
      case 1://左补线
        {

            k = (float)(((float)left[Lower_left_inflection_Y+1] - (float)left[Lower_left_inflection_Y+5]) /(-4));
            b = (float)((float)left[Lower_left_inflection_Y+5]- (float)(Lower_left_inflection_Y+5) * k);

            for(y = startY; y >(Endline+20); y--)
            {

             temp = (int)(k* y + b);
             if(temp<180&&temp>10)
             {
                 left[y]=temp;
             }
            }
            break;
        }

      case 2://右补线  待测试
      {

           k = (float)(((float)right[Lower_right_inflection_Y+1] - (float)right[Lower_right_inflection_Y+5]) /(-4));
           b = (float)((float)right[Lower_right_inflection_Y+5]- (float)(Lower_right_inflection_Y+5) * k);

           for(y = startY; y >(Endline+20); y--)
           {

            temp = (int)(k* y + b);
            if(temp<180&&temp>10){
                right[y]=temp;
            }
           }
           break;
       }

    }
}
//-----------------------------------------车库补线-----------------------------------
void Addingline2( uint8 choice, uint8 startX, uint8 startY)   //找到上拐点延斜率向下拉线
{

    // 直线 x = k*y + b
    float k = 0;
    float b = 0;
    int temp=0;
    switch(choice)
    {
      case 1://左补线
        {

            k = (float)(((float)left[Upper_left_inflection_Y-2] - (float)left[Upper_left_inflection_Y-5]) /3);
            b = (float)((float)left[Upper_left_inflection_Y-5]- (float)(Upper_left_inflection_Y-5) * k);

            for(y = startY; y<(image_h-3); y++)
            {

             temp = (int)(k* y + b);
             if(temp<185&&temp>2){
                 left[y]=temp;
             }

            }
            break;
        }

//     case 2://右补线  待测试
//        {
//
//            k = (float)(((float)right[Upper_right_inflection_Y-2] - (float)right[Upper_right_inflection_Y-5]) /3);
//            b = (float)((float)right[Upper_right_inflection_Y-5]- (float)(Upper_right_inflection_Y-5) * k);
//            for(y = startY; y<(image_h-3); y++)
//            {
//
//             temp = (int)(k* y + b);
//             if(temp<185&&temp>2){
//                 right[y]=temp;
//             }
//
//            }
//            break;
//        }

    }
}


//float k1,k2,k3;
//===================================================显  示===================================================
void IPS_show(void){
   int i;
//     ips200_show_string (0,170, "L");
//     ips200_show_int (10,170, Encoder_speed_l,3);
//     ips200_show_string (60,170, "R");
//     ips200_show_int (70,170, Encoder_speed_r,3);
//
     ips200_show_string (20,130, "jiaodu");
     ips200_show_float(120,130, jiaodu,5,4);
     ips200_show_string (20,150, "jd_sum");
        ips200_show_float(120,150, jd_sum,3,2);

//   ips200_show_string (20,190, "hill_flag");
//   ips200_show_float(150,190, hill_flag,5,4);
//        ips200_show_string (20,130, "p time");
//        ips200_show_float(120,130, paodao_time_juge,5,4);
//        ips200_show_string (20,150, "fuya_min");
//        ips200_show_float(120,150, fuya_min,3,2);
//        ips200_show_string (20,170, "fuya_max");
//        ips200_show_float(120,170, fuya_max,3,2);

//     ips200_show_string (0,210, "L_duty");
//     ips200_show_int (60,210,motor_l_out,4);
//     ips200_show_string (120,210, "R_duty");
//     ips200_show_int (180,210,motor_r_out,4);
  //********************图像显示*****************************
ips200_displayimage03x(imag[0], MT9V03X_W, MT9V03X_H);


  //********************边线显示*****************************
        //根据最终循环次数画出边界点
//        for (i = 0; i < data_stastics_l; i++)
//        {
//            ips200_draw_point(points_l[i][0]+2, points_l[i][1],  RGB565_GREEN);
//        }
//        for (i = 0; i < data_stastics_r; i++)
//        {
//            ips200_draw_point(points_r[i][0]-2, points_r[i][1],  RGB565_GREEN);
//        }

        for (i = Endline; i < image_h-1; i++)
        {
          //  middle[i] = (left[i] + right[i]) >> 1;//求中线

            //求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
            //当然也有多组边线的找法，但是个人感觉很繁琐，不建议
            ips200_draw_point((uint16)middle[i], (uint16)i,  RGB565_GREEN);
            ips200_draw_point((uint16)left[i], (uint16)i, RGB565_RED);
            ips200_draw_point((uint16)right[i],(uint16) i, RGB565_BLUE);

        }
          if(Endline<115){
              for(int i=119;i>Endline;i--)
               {
                   //ips200_draw_line(middle[scanning_line], 1, middle[scanning_line], 119, RGB565_RED); //中线
                   //ips200_draw_line(left[scanning_line], 1, left[scanning_line], 119, RGB565_CYAN); //左控制线
                   //ips200_draw_line(right[scanning_line], 1, right[scanning_line], 119, RGB565_YELLOW);  //右控制线
                   if(Endline>1&&Endline<120)
                     {
                       ips200_draw_line(1, (uint16)Endline, 187, (uint16)Endline,RGB565_GREEN); //截止行
                     }
                }
          }
          /*
           * 中线计算范围
           */
//         if(ngszd=0)
//         {
//          for(int i=189 ;i<220;i++)
//          {
//              ips200_draw_point(i, 90,  RGB565_RED);
//              ips200_draw_point(i, 70,  RGB565_RED);
//
//              ips200_draw_point(i, 85,  RGB565_WHITE);
//              ips200_draw_point(i, 65,  RGB565_WHITE);
//              ips200_draw_point(i, 80,  RGB565_WHITE);
//              ips200_draw_point(i, 100,  RGB565_WHITE);
//          }
//         }
//          else if(ngszd=1)
//          {
//          for(int i=189 ;i<220;i++)
//          {
//              ips200_draw_point(i, 85,  RGB565_RED);
//              ips200_draw_point(i, 65,  RGB565_RED);
//
//              ips200_draw_point(i, 90,  RGB565_WHITE);
//              ips200_draw_point(i, 70,  RGB565_WHITE);
//              ips200_draw_point(i, 80,  RGB565_WHITE);
//              ips200_draw_point(i, 100,  RGB565_WHITE);
//          }
//          }
//          else
//          {
//          for(int i=189 ;i<220;i++)
//          {
//              ips200_draw_point(i, 80,  RGB565_RED);
//              ips200_draw_point(i, 100,  RGB565_RED);
//
//              ips200_draw_point(i, 85,  RGB565_WHITE);
//              ips200_draw_point(i, 65,  RGB565_WHITE);
//              ips200_draw_point(i, 90,  RGB565_WHITE);
//              ips200_draw_point(i, 70,  RGB565_WHITE);
//          }
//          }




//  //********************拐点显示*****************************
//  //---------------左下拐点--------------------
//   if(l_growth_direction_flag)
//       ips200_show_string(100,140,"l_growth 1");
//   else
//       ips200_show_string(100,140,"l_growth 0");
//   if(left_2_growth_direction)
//       ips200_show_string(100,155,"l_growth2 1");
//   else
//       ips200_show_string(100,155,"l_growth2 0");
//             if(right_3_growth_direction)
//                 ips200_show_string(100,155,"r_growth2 1");
//             else
//                 ips200_show_string(100,155,"r_growth2 0");
//   if(right_5_growth_direction)
//       ips200_show_string(100,170,"r_growth5 1");
//   else
//       ips200_show_string(100,170,"r_growth5 0");
//                           ips200_show_string(0,130,"r_growth3");
//                           ips200_show_string(0,150,"r_growth2");
//   ips200_show_int(100, 130, left_6_growth_direction,3);
////   ips200_show_int(100, 150, right_2_growth_direction,3);
//   ips200_show_string(0,170,"r_k");
//   ips200_show_string(0,190,"l_k");
//ips200_show_float(100, 170, r_k,2,3);
//ips200_show_float(100, 190, l_k,2,3);
////
//  if(Lower_left_inflection_Flag==1){
//      ips200_show_string(0,125,"low left 1");
////      ips200_show_int(0, 140, Lower_left_inflection_X,3);
////      ips200_show_int(0, 155, Lower_left_inflection_Y,3);
//      show_point(Lower_left_inflection_X, Lower_left_inflection_Y,  RGB565_BLUE);
//  }
//  else
//  ips200_show_string(0,125,"low left 0");
//  if(Lost_right_Flag2)
//      ips200_show_string(0,180,"Lost_right 1");
//  else
//      ips200_show_string(0,180,"Lost_right 0");
//  if( Right_straight_flag)
//      ips200_show_string(120,180,"r_str 1");
//  else
//      ips200_show_string(120,180,"r_str 0");
////  //---------------右下拐点-------------------
//if(l_growth_direction_flag6==1){
//    ips200_show_string(0,130,"l6 1");
////      ips200_show_int(0, 185, Lower_right_inflection_X,3);
////      ips200_show_int(0, 200, Lower_right_inflection_Y,3);
////      ips200_draw_point(Lower_right_inflection_X, Lower_right_inflection_Y,  RGB565_BLUE);
//}
//else
//{
//    ips200_show_string(0,130,"l6 0");
//}
//  if(Lower_right_inflection_Flag==1){
//      ips200_show_string(0,130,"low right1");
////      ips200_show_int(0, 185, Lower_right_inflection_X,3);
////      ips200_show_int(0, 200, Lower_right_inflection_Y,3);
////      ips200_draw_point(Lower_right_inflection_X, Lower_right_inflection_Y,  RGB565_BLUE);
//  }
//  else
//  {
//      ips200_show_string(0,130,"low right0");
//  }
////  //---------------左上拐点-------------------
//  if(Upper_left_inflection_Flag==1){
//      ips200_show_string(100,125,"upper left");
//      ips200_show_int(100,140, Upper_left_inflection_X,3);
//      ips200_show_int(100,155, Upper_left_inflection_Y,3);
//      ips200_draw_point(Upper_left_inflection_X, Upper_left_inflection_Y,  RGB565_RED);
//  }
//  //---------------右上拐点--------------------
//  if(Upper_right_inflection_Flag==1){
//      ips200_show_string(100,170,"upper right1");
//      ips200_show_int(100, 185,Upper_right_inflection_X,3);
//      ips200_show_int(100, 200, Upper_right_inflection_Y,3);
//      ips200_draw_point(Upper_right_inflection_X, Upper_right_inflection_Y,  RGB565_RED);
//  }

//  ips200_draw_line(0, 20, 187, 20, RGB565_BLUE);
//  ips200_draw_line(0, 40, 187, 40, RGB565_BLUE);
//  ips200_draw_line(0, 60, 187, 60, RGB565_BLUE);
//  ips200_draw_line(0, 80, 187, 80, RGB565_BLUE);
//  ips200_draw_line(0, 100, 187, 100, RGB565_BLUE);
//  for(i=0;i<188;i++)
//  {
//      ips200_draw_point(i, 100,  RGB565_RED);
//  }
//  for(y=0;y<188;y++)
//  {
//      ips200_draw_point(20, y,  RGB565_RED);
//  }
//  for(y=0;y<188;y++)
//  {
//      ips200_draw_point(20, y,  RGB565_RED);
//  }
//  for(y=0;y<188;y++)
//  {
//      ips200_draw_point(20, y,  RGB565_RED);
//  }
  //********************环岛*****************************
//  if(roundabout_Flag==1){
//      ips200_show_string(130,215,"roundabout");
//      ips200_show_int(130, 230,roundabout_X,3);
//      ips200_show_int(130, 245, roundabout_Y,3);
//      ips200_draw_point(roundabout_X, roundabout_Y,  RGB565_RED);
//  }
  //********************出环岛口*****************************
//  if(Exit_loop_Flag==1){
//      ips200_show_string(130,255,"exit");
//      ips200_show_int(130, 270,Exit_loop_X,3);
//      ips200_show_int(130, 285, Exit_loop_Y,3);
//      ips200_draw_point(Exit_loop_X, Exit_loop_Y,  RGB565_RED);
//  }
  //********************直线*****************************
  if( bend_straight_flag==1){
      ips200_show_string (0, 215, "straight 1");
   //   ips200_show_float(100, 215,straight_k_err,3,3);
    ///  ips200_show_float(170, 215,k2,3,3);
  }
  else{
      ips200_show_string (0, 215, "straight 0");
  }
  ips200_show_float(100, 215,straight_k_err,3,3);

  //********************圆环*****************************
  ips200_show_string (0,230, "annulus_R");
  ips200_show_int (100,230, annulus_R_memory,3);
  ips200_show_string (0,248, "annulus_L");
  ips200_show_int (100,248, annulus_L_memory,3);

  //********************s道****************************
  ips200_show_string (0,265, "S_road");
  ips200_show_int (60,265, S_road_Flag,3);
  ips200_show_float(150,265, image_process_time,2,6);
  //********************十字*****************************
  ips200_show_string (120,230, "Crossroad");
  ips200_show_int (200,230, Crossroad_memory,2);



  //********************左车库*****************************
//  ips200_show_string (0,245, "Garage");
//  ips200_show_int (50,245, Left_garage_memory,3);

//    ips200_show_string (0,280, "shi_zi=");
//    ips200_show_int (75,280, Crossroad_Flag,3);
  //********************左丢线显示*****************************
//  ips200_show_float(0, 275,Lost_point_L_scan_line,3,3);

  //********************斑马线*****************************
//  ips200_show_string (0,260, "zebra");
//  ips200_show_int (50,260, zebra_crossing_flag,3);
  //  ips200_show_float(0,280,servo.pid_err,3,3);

  //********************停车标志*****************************
//  ips200_show_string (0,280, "ting");
//  ips200_show_int (50,280,   tingche_flag,3);
//
//  //********************陀螺仪*****************************
  ips200_show_string (0,300, "g_start");
  ips200_show_int (50,300,   gyro_start,3);
  ips200_show_string (100,300, "g_end");
  ips200_show_int (150,300,   gyro_end,3);


//  //********************速度*****************************
//  ips200_show_float(0, 275, speed1,3,3);
//  ips200_show_float(60, 275,speed2,3,3);

  //********************编码器显示*****************************
//  if( bend_straight_flag==1){
//        ips200_show_string (0, 275, "stra");
//    }
//    else{
//        ips200_show_string (0, 275, "bend");
//    }
//
//  ips200_show_float(0, 290,k1,2,3);
//  ips200_show_float(50, 290,k2,2,3);
//  ips200_show_float(100, 290,k3,2,3);

  //********************转向环误差显示*****************************
//
//  ips200_show_string (100, 280, "err");
//  ips200_show_float(150, 280, servo.pid_err,2,2);
  //ips200_show_int(0, 275, Encoder_speed_l, 5);
//  ips200_show_int(60,275, motor2.pid_actual_val, 5);
 // ips200_show_int(0, 290, Encoder_speed_r, 5);
//  ips200_show_int(60,290, motor1.pid_actual_val, 5);
  //********************调试显示*****************************
//ips200_show_int(100, 290,Endline,3);
//  ips200_show_float(100, 275,Lost_point_R_scan_line,3,3);
// ips200_draw_point(Upper_left_inflection_X, Upper_left_inflection_Y,  RGB565_BLUE);
// ips200_draw_point(Upper_right_inflection_X, Upper_right_inflection_Y,  RGB565_BLUE);


 //ips200_show_int (15,275, Lost_right_Flag,3);
  //********************陀螺仪*****************************
//  ips200_show_string (0, 200, "jd=");
//  ips200_show_float(50, 200, jd_sum,4,4);
//
//  ips200_show_string (0, 200, "Endline=");
//  ips200_show_int(80, 200, Endline,4);
}
 float straight_k_err=0;
 float straight_k_1=0;
 float straight_k_2=0;
    void  ben_straight(void)
    {
           float k1,k2;
           float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
           bend_straight_flag=0;
           caculate_distance(100,119,middle,&l_slope3,&l_distance3);
           k2=l_slope3;

           caculate_distance(45,60,middle,&l_slope2,&l_distance2);
           k1=l_slope2;

           if((absolute(k1-k2)<0.15))//&&(Lost_left_Flag==0)&&(Lost_right_Flag==0)
           bend_straight_flag=1;
           else
          bend_straight_flag=0;
           straight_k_err=absolute(k1-k2);


}

//===================================================s道===================================================
    void S_road(void)
    {
        float l_slope=0,l_distance=0;  //存放左边斜率和截距
        float r_slope=0,r_distance=0;  //存放左边斜率和截距

        if(Left_straight_flag==0&&Right_straight_flag==0&&annulus_L_Flag==0&&annulus_R_Flag==0&&S_right_lost_Flag==0&&S_left_lost_Flag==0&&S_road_Flag==0&&S_road_memory==0&&S_right_inflection_Flag==1&&S_left_inflection_Flag==1)
        {
            S_road_Flag=1;
            S_road_memory=1;
        }
        if(S_road_Flag==1)
        {
            if(S_road_memory==1)
            {
                    caculate_distance(Endline+10,image_h-3,left,&l_slope,&l_distance);
                    caculate_distance(Endline+10,image_h-3,right,&r_slope,&r_distance);
                    for(y=image_h-3;y>Endline+10;y--)
                    {

                        if((l_slope*(y)+l_distance)>185)
                            left[y]=185;
                        else if((l_slope*(y)+l_distance)<2)
                            left[y]=2;
                        else
                            left[y]=l_slope*(y)+l_distance;

                        if((r_slope*(y)+r_distance)>185)
                            right[y]=185;
                        else if((r_slope*(y)+r_distance)<2)
                            right[y]=2;
                        else
                            right[y]=r_slope*(y)+r_distance;
                    }

                /*------判断是否结束S道-------*/
                if(S_right_inflection_Flag==0&&S_left_inflection_Flag==0)
                {
                    S_road_Flag=0;
                    S_road_memory=0;
                }
            }
        }
    }

void hill_juge(void)
{
    if(hill_flag==0)//&&podaogeshu==1
    {
        hill_flag=1;
//        podaogeshu--;
    }
    if(hill_flag==1)//podaogeshu根据坡道个数改
    {
        hill_flag=2;
    }
//    if(hill_flag==2||hill_flag==1)
//    {
//        beep_on();
//    }
//    else
//    {
//        beep_off();
//    }
}
char left_annulus_out_juge,right_annulus_out_juge;//1是出现，0是消失
char annulus_out_time=0;
char l_line_level,r_line_level,l_line_level2,r_line_level2,l_line_level3,r_line_level3,l_line_level4,r_line_level4,l_line_level5,r_line_level5;//1是水平直线0不是(l_line_level,r_line_level是普通的水平判断，l_line_level2,r_line_level2是圆环的水平判断)
char l_zhangai_juge,r_zhangai_juge;//1是出现0是不出现（其实就是圆环判断）
char l_lost,r_lost,l_lost2,r_lost2;
char radian_r[2],radian_l[2],radian_r_mun,radian_l_num,l_end_point[2],r_end_point[2],l_mid_point[2],r_mid_point[2],l_mid_point2[2],r_mid_point2[2];
char l_crue,r_crue;
void level_out_juge(void)//圆环直线出现判断
{
    l_lost=0;
    r_lost=0;
    l_lost2=0;
    r_lost2=0;
    l_zhangai_juge=0;
    r_zhangai_juge=0;
    l_line_level=0;
    r_line_level=0;
    l_line_level2=0;
    r_line_level2=0;
    l_line_level3=0;
    r_line_level3=0;
    l_line_level4=0;
    r_line_level4=0;
    l_line_level5=0;
    r_line_level5=0;
    //丢线
    for (int i = 1; i < data_stastics_r-1&&points_r[i][1]>Endline+10; i++)
    {
        if(points_r[i][1]>0&&points_r[i][1]<12)
            if(points_r[i][0]>MT9V03X_W-3)
                r_lost=1;
    }
    for (int i = 1; i < data_stastics_l-1&&points_l[i][1]>Endline+10; i++)
    {
        if(points_l[i][1]>0&&points_l[i][1]<12)
            if(points_l[i][0]<3)
                l_lost=1;
    }
    for (int i = 1; i < data_stastics_r-1&&points_r[i][1]>Endline+10; i++)
    {
        if(points_r[i][1]>0&&points_r[i][1]<4)
            if(points_r[i][0]>MT9V03X_W-3)
                r_lost2=1;
    }
    for (int i = 1; i < data_stastics_l-1&&points_l[i][1]>Endline+10;i++)
    {
        if(points_l[i][1]>0&&points_l[i][1]<4)
            if(points_l[i][0]<3)
                l_lost2=1;
    }
      for (int i = 1; i < data_stastics_l-6&&points_l[i][1]>15&&points_l[i][0]<MT9V03X_W/2+2&&points_l[i][1]>Endline+10; i++)
              {
                   if((dir_l[i - 1]==6)&&(dir_l[i]==6)&&(dir_l[i+1]==6))
                   {
                       l_line_level=1;
                   }
              }
      for (int i = 1; i < data_stastics_l-6&&points_l[i][1]>4&&points_l[i][1]>Endline+10; i++)
              {
                   if((dir_l[i - 1]==6)&&(dir_l[i]==6))
                   {
                       l_line_level2=1;
                   }
              }
      for (int i = 1; i < data_stastics_l-6&&points_l[i][1]>4&&points_l[i][1]>Endline+10; i++)
              {
                   if((dir_l[i - 1]==2)&&(dir_l[i]==2))
                   {
                       l_line_level3=1;
                   }
              }
      for (int i = 1; i < data_stastics_l-6&&points_l[i][1]>4&&points_l[i][1]>Endline+10; i++)
              {
                   if((dir_l[i - 1]==6))
                   {
                       l_line_level4=1;
                   }
              }
      for (int i = 1; i < data_stastics_l-6&&points_l[i][1]>2&&points_l[i][1]>Endline+10; i++)
              {
                   if((dir_l[i - 1]==6))
                   {
                       l_line_level5=1;
                   }
              }
//      ips200_show_string(0,130,"r_line_level");
//      ips200_show_int(100,130,r_line_level,6);
//           if(r_count_num>=1)
//               right_annulus_out_juge=1;
//           else
//               right_annulus_out_juge=0;

   for (int i = 1; i < data_stastics_r-6&&points_r[i][1]>15&&points_r[i][0]>MT9V03X_W/2-2&&points_r[i][1]>Endline+10; i++)
           {
                if((dir_r[i - 1]==6)&&(dir_r[i]==6))
                {
                    r_line_level=1;
                }
           }
//   ips200_show_string(0,100,"r_line_level");
//   ips200_show_int(170,100,r_line_level,6);
//   ips200_show_string(0,120,"l_arc_on_juge[1]");
//   ips200_show_int(170,120,l_arc_on_juge[1],6);
   for (int i = 1; i < data_stastics_r-6&&points_r[i][1]>4&&points_r[i][1]>Endline+10; i++)
           {
                if((dir_r[i - 1]==6)&&(dir_r[i]==6))
                {
                    r_line_level2=1;
                }
           }
   for (int i = 1; i < data_stastics_r-6&&points_r[i][1]>4&&points_r[i][1]>Endline+10; i++)
           {
                if((dir_r[i - 1]==2)&&(dir_r[i]==2))
                {
                    r_line_level3=1;
                }
           }
   for (int i = 1; i < data_stastics_r-6&&points_r[i][1]>4&&points_r[i][1]>Endline+10; i++)
           {
                if((dir_r[i - 1]==6))
                {
                    r_line_level4=1;
                }
           }
   for (int i = 1; i < data_stastics_r-6&&points_r[i][1]>2&&points_r[i][1]>Endline+10; i++)
           {
                if((dir_r[i - 1]==6)&&(dir_r[i]==6))
                {
                    r_line_level5=1;
                }
           }
}
/*by电气 223 严建晨
* @brief 路障处理
* @注意只有当flag_l_roadblock_exist为1时才是判断其存在
 */
int l_point1_juge[5],l_point2_juge[5],l_point3_juge[5];
int l_point1[2],l_point2[2],l_point3[2];
int l_point1_location,l_point2_location,l_point3_location;
char flag_l_roadblock_deal=0;//1处理0不处理
char flag_l_roadblock_exist;//像素判断是否存在  0有1没有
void l_roadblock_deal(void)//路障处理/*路障在左边*/
{
    //
    l_point1_juge[4]=l_point1_juge[3];
    l_point1_juge[3]=l_point1_juge[2];
    l_point1_juge[2]=l_point1_juge[1];
    //
    l_point2_juge[4]=l_point2_juge[3];
    l_point2_juge[3]=l_point2_juge[2];
    l_point2_juge[2]=l_point2_juge[1];
    //
    l_point3_juge[4]=l_point3_juge[3];
    l_point3_juge[3]=l_point3_juge[2];
    l_point3_juge[2]=l_point3_juge[1];
    for(int i=1;i<data_stastics_l-5&&points_l[i][1]>Endline+10;i++)
     {
       if((dir_l[i - 1] == 5||dir_l[i - 1]==4)&&(dir_l[i]==5||dir_l[i]==4||dir_l[i]==6)&&(dir_l[i+1]==5)&&(dir_l[i +4]==6)&&(dir_l[i +5]==6)&&points_l[i+1][1]>4)//第一个点
       {
           l_point1_location=i+1;
       }
          for(int i_j=l_point1_location;i_j<data_stastics_l-4;i_j++)
          {
              if((dir_l[i_j-1]==6)&&(dir_l[i_j]==6)&&(dir_l[i_j+1]==5)&&(dir_l[i_j+2]==5||dir_l[i_j+2]==4)&&(dir_l[i_j+3]==5||dir_l[i_j+3]==4))
              {

                  l_point2_location=i_j+1;
                  l_point2_juge[1]=1;
              }
          }
      }
     for(int i=l_point2_location;i<data_stastics_l-4&&points_l[i][1]>Endline+10;i++)
     {
         if((dir_l[i - 1] == 2||dir_l[i - 1] == 3)&&dir_l[i] == 2&&dir_l[i + 1] == 3&&(dir_l[i +2] == 4))
             l_point3_location=i+1;
     }
     char flag_l=0;
     flag_l_roadblock_exist=0;
     flag_l_roadblock_deal=0;
     for(int i=l_point1_location;i<l_point2_location-4&&points_l[i][1]>Endline+10;i++)
         if(dir_l[i - 1] == 6&&dir_l[i] == 6)
             flag_l=1;
     for(int i=1;i<l_point2_location;i++)//其下面有拐点
    {
      if(dir_l[i - 1] == 2&&dir_l[i] == 2)
          flag_l=0;
    }

     for(int i=l_point2_location;i<data_stastics_l-4&&flag_l&&r_line_level2==0&&r_line_level3==0&&points_l[i][1]>Endline+10;i++)//
         if(dir_l[i - 1] == 4&&dir_l[i] == 4&&dir_l[i + 1] ==4&&points_l[l_point3_location][0]>8)
             flag_l_roadblock_exist=1;
     flag_l_roadblock_deal=flag_l_roadblock_exist;
     ips200_show_int(130,90,flag_l_roadblock_exist,6);



    if(flag_l_roadblock_deal)
         {
          //
          l_point1_juge[1]=1;
          l_point1[0]=points_l[l_point1_location][0];
          l_point1[1]=points_l[l_point1_location][1];
          //
          l_point2_juge[1]=1;
          l_point2[0]=points_l[l_point2_location][0];
          l_point2[1]=points_l[l_point2_location][1];
          //
          l_point3_juge[1]=1;
          l_point3[0]=points_l[l_point3_location][0];
          l_point3[1]=points_l[l_point3_location][1];
         }
       else
      {
        l_point1[0]=0;
        l_point1[1]=0;
        l_point2[0]=0;
        l_point2[1]=0;
        l_point3[0]=0;
        l_point3[1]=0;
       }
    l_point1_juge[0]=l_point1_juge[1]&&l_point1_juge[2]&&l_point1_juge[3]&&l_point1_juge[4];
    l_point2_juge[0]=l_point2_juge[1]&&l_point2_juge[2]&&l_point2_juge[3]&&l_point2_juge[4];
    l_point3_juge[0]=l_point3_juge[1]&&l_point3_juge[2]&&l_point3_juge[3]&&l_point3_juge[4];

 }

/*
 *@注意只有当flag_r_roadblock_exist为1时才是判断其存在
 */

int r_point1_juge[5],r_point2_juge[5],r_point3_juge[5];
int r_point1[2],r_point2[2],r_point3[2];
int r_point1_location,r_point2_location,r_point3_location;
char flag_r_roadblock_deal=0;//1处理0不处理
char flag_r_roadblock_exist=0;//像素判断是否存在  0有1没有
void r_roadblock_deal(void)//路障处理/*路障在右边*/
{
    //
    r_point1_juge[4]=r_point1_juge[3];
    r_point1_juge[3]=r_point1_juge[2];
    r_point1_juge[2]=r_point1_juge[1];
    //
    r_point2_juge[4]=r_point2_juge[3];
    r_point2_juge[3]=r_point2_juge[2];
    r_point2_juge[2]=r_point2_juge[1];
    //
    r_point3_juge[4]=r_point3_juge[3];
    r_point3_juge[3]=r_point3_juge[2];
    r_point3_juge[2]=r_point3_juge[1];
    for(int i=1;i<data_stastics_r-5&&points_r[i][1]>Endline+10;i++)
    {
      if((dir_r[i - 1] == 5||dir_r[i - 1]==4)&&(dir_r[i]==5||dir_r[i]==4||dir_r[i]==6)&&(dir_r[i+1]==5)&&(dir_r[i +4]==6)&&(dir_r[i +5]==6)&&points_r[i+1][1]>4)//第一个点
      {
          r_point1_location=i+1;
      }
         for(int i_j=r_point1_location;i_j<data_stastics_r-4;i_j++)
         {
             if((dir_r[i_j-1]==6)&&(dir_r[i_j]==6)&&(dir_r[i_j+1]==5)&&(dir_r[i_j+2]==5||dir_r[i_j+2]==4)&&(dir_r[i_j+3]==5||dir_r[i_j+3]==4))
             {

                 r_point2_location=i_j+1;
                 r_point2_juge[1]=1;
             }
         }
     }
    for(int i=r_point2_location;i<data_stastics_r-4&&points_r[i][1]>Endline+10;i++)
    {
        if((dir_r[i - 1] == 2||dir_r[i - 1] == 3)&&dir_r[i] == 2&&dir_r[i + 1] == 3&&(dir_r[i +2] == 4))
            r_point3_location=i+1;
    }
    char flag_r=0;
    flag_r_roadblock_exist=0;
    flag_r_roadblock_deal=0;
    for(int i=r_point1_location;i<r_point2_location-4&&points_r[i][1]>Endline+10;i++)
        if(dir_r[i - 1] == 6&&dir_r[i] == 6)
            flag_r=1;
    for(int i=1;i<r_point2_location&&points_r[i][1]>Endline+10;i++)//其下面有拐点
        {
          if(dir_r[i - 1] == 2&&dir_r[i] == 2)
              flag_r=0;
        }
    for(int i=r_point2_location;i<data_stastics_r-4&&flag_r&&l_line_level2==0&&l_line_level3==0&&points_r[i][1]>Endline+10;i++)
        if(dir_r[i - 1] == 4&&dir_r[i] == 4&&dir_r[i + 1] ==4&&points_r[r_point3_location][0]<MT9V03X_W-8)
            flag_r_roadblock_exist=1;
    flag_r_roadblock_deal=flag_r_roadblock_exist;
 //  ips200_show_int(130,90,flag_r_roadblock_exist,6);
    if(flag_r_roadblock_deal)
         {
          //
          r_point1_juge[1]=1;
          r_point1[0]=points_r[r_point1_location][0];
          r_point1[1]=points_r[r_point1_location][1];
          //
          r_point2_juge[1]=1;
          r_point2[0]=points_r[r_point2_location][0];
          r_point2[1]=points_r[r_point2_location][1];
          //
          r_point3_juge[1]=1;
          r_point3[0]=points_r[r_point3_location][0];
          r_point3[1]=points_r[r_point3_location][1];
         }
       else
      {
        r_point1[0]=0;
        r_point1[1]=0;
        r_point2[0]=0;
        r_point2[1]=0;
        r_point3[0]=0;
        r_point3[1]=0;
       }

    r_point1_juge[0]=r_point1_juge[1]&&r_point1_juge[2]&&r_point1_juge[3]&&r_point1_juge[4];
    r_point2_juge[0]=r_point2_juge[1]&&r_point2_juge[2]&&r_point2_juge[3]&&r_point2_juge[4];
    r_point3_juge[0]=r_point3_juge[1]&&r_point3_juge[2]&&r_point3_juge[3]&&r_point3_juge[4];

 }
void roadblock_addline(void)
{
    level_out_juge();
    r_roadblock_deal();
    l_roadblock_deal();
    if(flag_r_roadblock_deal==1&&Left_straight_flag==1&&Lost_left_Flag2==0&&Lost_right_Flag2==0)
        Addingline( 2, r_point2[0],r_point2[1], points_r[1][0], points_r[1][1]);
    if(flag_l_roadblock_deal==1&&Right_straight_flag==1&&Lost_left_Flag2==0&&Lost_right_Flag2==0)
        Addingline( 2, l_point2[0],l_point2[1], points_l[1][0], points_l[1][1]);
}
void show_point(uint8 x, uint8 y, rgb565_color_enum c)//画点
{
   for (int i = 0; i <= 1; i++)
       for (int j = 0; j <= 1; j++)
           ips200_draw_point(x + i, y + j, c);
}

//===================================================元素识别===================================================
void Element_recognition(void)
{
    inflection_point();
    ten_inflection();
    left_straight();
    right_straight();
   if(tingche==150)
   {
       hill_juge();
    if(hill_flag==0)
    {
     zebra_crossing();
//    Garage();
       crossroad();
     if(barrier__open_or_close_flag==1)
     {
         roadblock_addline();
     }
     if(s_road_open_or_close_flag==1)
     {
       S_road();
     }
     if(annulus_open_or_close_flag==1)
     {
      annulus_L();
      annulus_R();
     }
    }
   }
    middle_line();
    ben_straight();
}


void pian_cha(void)     //偏差函数
{
// //方案一，只取固定行，mini车跑没问题
//    int you_pian=0,zuo_pian=0;
//    int i;
//    if(annulus_L_Flag==1||annulus_R_Flag==1)
//    {
//        for(i=90;i>=70;i--)
//        {
//            if(biao_column>middle[i])//左正
//               zuo_pian=zuo_pian+(biao_column-middle[i]);
//            if(biao_column<middle[i])//右负
//               you_pian=you_pian+(biao_column-middle[i]);
//
//            if(i==70)
//            {
//                piancha=(zuo_pian+you_pian)/20;
//            }
//        }
//    }
//    else if(S_road_Flag==1||bend_straight_flag==0)
//    {
//        for(i=85;i>=65;i--)
//        {
//            if(biao_column>middle[i])//左正
//               zuo_pian=zuo_pian+(biao_column-middle[i]);
//            if(biao_column<middle[i])//右负
//               you_pian=you_pian+(biao_column-middle[i]);
//
//            if(i==65)
//            {
//                piancha=(zuo_pian+you_pian)/20;
//            }
//        }
//    }
//    else
//    {
//    for(i=100;i>79;i--)
//    {
//        if(biao_column>middle[i])//左正
//           zuo_pian=zuo_pian+(biao_column-middle[i]);
//        if(biao_column<middle[i])//右负
//           you_pian=you_pian+(biao_column-middle[i]);
//
//        if(i==80)
//        {
//            piancha=(zuo_pian+you_pian)/20;
//        }
//    }
//    }

// 方案二  利用权重，想提速可以上网看看权重类型的方法
    int i;
    int weight_sum=0;
    for(i=119;i >= 0;i--)
    {
        if(middle[i]!=0)
        {
     piancha+=weight[i]*(biao_column-middle[i]);
     weight_sum+=weight[i];
        }
    }
    piancha/=weight_sum;

//

}




//===================================================图像处理===================================================

void image_process(void)
{
   if(mt9v03x_finish_flag)           //判断一幅图像是否接收完成
   {
        system_start ();
        // gpio_set_level(P14_0, 0);
        Get_image(mt9v03x_image);    //图像转载
        binaryzation();              //二值化
        image_filter(imag);        //滤波
        image_draw_rectan(imag);     //画黑框

        /*===清零===*/
        data_stastics_l = 0;
        data_stastics_r = 0;
        xunxian =get_start_point(image_h - 2);
        if (xunxian) //找到起点了，再执行八邻域，没找到就一直找
        {
                //printf("正在开始八邻域\n");
                search_l_r((uint16)USE_num, imag, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &Endline);
                //printf("八邻域已结束\n");
                growth_direction();           //生长方向提取
                get_left(data_stastics_l);    //左边界提取
                get_right(data_stastics_r);   //右边界提取
                lost_left();                  //左下方丢线判断
                lost_right();                 //右下方丢线判断
                Element_recognition();        //元素判断+提取中线
                pian_cha();
                Finish_Flag=1;
        }
        image_process_time=system_getval ();//获取程序运行时间
        mt9v03x_finish_flag=0;              //清除图像采集完成标志
    }
}


