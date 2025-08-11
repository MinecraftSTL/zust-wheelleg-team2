/*
 * Sys.c
 *
 *  Created on: 2024年10月27日
 *      Author: sun
 */
#include "Sys.h"

// 曲率计算函数
/***********************************************
* @brief : 曲率计算函数
* @param : array 需要计算的数组
*          start 数组起始点 推荐从3开始谨防出现不必要的bug
*          size 需要计算的个数
* @return: double 计算出的曲率值
* @date  :2024年11月4日10:06:55
* @author:SJX
* @exp   :Calculate_Curvature(left, i, 10);
************************************************/
float Calculate_Curvature(int array[], int start, int size)
{
    float total_curvature = 0.0;
    int i = 0;
    for (i = start; i < start + size; i++)
    {
        // 当前点和前后点的坐标
        float x1 = i - 1, y1 = array[i - 1];
        float x2 = i, y2 = array[i];
        float x3 = i + 1, y3 = array[i + 1];

        // 分子部分
        float numerator = (x3 - x2) * (y1 - y2) - (y3 - y2) * (x1 - x2);

        // 分母部分
        float denominator = pow(pow(x3 - x2, 2) + pow(y3 - y2, 2), 1.5);

        // 避免除以零
        if (denominator != 0)
        {
            total_curvature += fabs(numerator / denominator);
        }
    }

    // 返回平均曲率
    return total_curvature / (size);
}

int collinear(double a[2],double b[2],double c[2])//判断三点是否共线，共线返回1
{
      double k1,k2;
      double kx1,ky1,kx2,ky2;
      if(a[0]==b[0]&&b[0]==c[0])  return 1;//三点横坐标都相等，共线
      else
        {
          kx1=b[0]-a[0];
          kx2=b[0]-c[0];
          ky1=b[1]-a[1];
          ky2=b[1]-a[1];
          k1=ky1/kx1;
          k2=ky2/kx2;
          if(k1==k2) return 1;//AB与BC斜率相等，共线
           else  return 0;//不共线
         }
}
double curvature(double a[2],double b[2],double c[2])//double为数据类型，
{                                                    //数组a[2]为点a的坐标信息，a[0]为a的x坐标，a[1]为a的y坐标
       double cur;//求得的曲率
       if(collinear(a,b,c)==1)//判断三点是否共线
       {
        cur=0.0;//三点共线时将曲率设为某个值，0
        }
       else
      {
       double radius;//曲率半径
       double dis,dis1,dis2,dis3;//距离
       double cosA;//ab确定的边所对应的角A的cos值
       dis1=hypot(a[0]-b[0],a[1]-b[1]);
       dis2=hypot(a[0]-c[0],a[1]-c[1]);
       dis3=hypot(b[0]-c[0],b[1]-c[1]);
       dis=dis2*dis2+dis3*dis3-dis1*dis1;
       cosA=dis/(2*dis2*dis3);//余弦定理
       radius=0.5*dis1/cosA;
       cur=1/radius;
      }
       return cur;
}
float Q_rsqrt( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

uint16 Bits_reverse(uint16 value, int bits) {
    uint16 reversed = 0;
    for (int i = 0; i < bits; ++i) {
        if (value & (0x01 << i)) {
            reversed |= (0x01 << (bits - 1 - i));
        }
    }
    return reversed;
}

float Angle_normalize(float theta) {
    float r = fmodf(theta + PI, 2*PI);  // 平移并取模
    r = fmodf(r + 2*PI, 2*PI);          // 确保余数非负
    return r - PI;                        // 逆向平移
}
float NormalizeAngle_toPi2(float theta) {
    theta = fabsf(theta); // 转换到 [0, π]
    return (PI - fabsf(2 * theta - PI)) / 2.f;
}

uint8 String_startWith(const char this[], const char start[]){
    for(uint32 i=0; start[i]; ++i){
        if(this[i] != start[i]){
            return 0;
        }
    }
    return 1;
}
uint32 String_hash(const char *this, uint32 mod) {
    uint64 hash = 0;
    uint8 c;
    while ((c = *this++)) {
        hash = (hash * 31 + c) % mod; // 使用位掩碼限制為15位
    }
    return (uint32)hash;
}
