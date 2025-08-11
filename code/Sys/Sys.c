/*
 * Sys.c
 *
 *  Created on: 2024��10��27��
 *      Author: sun
 */
#include "Sys.h"

// ���ʼ��㺯��
/***********************************************
* @brief : ���ʼ��㺯��
* @param : array ��Ҫ���������
*          start ������ʼ�� �Ƽ���3��ʼ�������ֲ���Ҫ��bug
*          size ��Ҫ����ĸ���
* @return: double �����������ֵ
* @date  :2024��11��4��10:06:55
* @author:SJX
* @exp   :Calculate_Curvature(left, i, 10);
************************************************/
float Calculate_Curvature(int array[], int start, int size)
{
    float total_curvature = 0.0;
    int i = 0;
    for (i = start; i < start + size; i++)
    {
        // ��ǰ���ǰ��������
        float x1 = i - 1, y1 = array[i - 1];
        float x2 = i, y2 = array[i];
        float x3 = i + 1, y3 = array[i + 1];

        // ���Ӳ���
        float numerator = (x3 - x2) * (y1 - y2) - (y3 - y2) * (x1 - x2);

        // ��ĸ����
        float denominator = pow(pow(x3 - x2, 2) + pow(y3 - y2, 2), 1.5);

        // ���������
        if (denominator != 0)
        {
            total_curvature += fabs(numerator / denominator);
        }
    }

    // ����ƽ������
    return total_curvature / (size);
}

int collinear(double a[2],double b[2],double c[2])//�ж������Ƿ��ߣ����߷���1
{
      double k1,k2;
      double kx1,ky1,kx2,ky2;
      if(a[0]==b[0]&&b[0]==c[0])  return 1;//��������궼��ȣ�����
      else
        {
          kx1=b[0]-a[0];
          kx2=b[0]-c[0];
          ky1=b[1]-a[1];
          ky2=b[1]-a[1];
          k1=ky1/kx1;
          k2=ky2/kx2;
          if(k1==k2) return 1;//AB��BCб����ȣ�����
           else  return 0;//������
         }
}
double curvature(double a[2],double b[2],double c[2])//doubleΪ�������ͣ�
{                                                    //����a[2]Ϊ��a��������Ϣ��a[0]Ϊa��x���꣬a[1]Ϊa��y����
       double cur;//��õ�����
       if(collinear(a,b,c)==1)//�ж������Ƿ���
       {
        cur=0.0;//���㹲��ʱ��������Ϊĳ��ֵ��0
        }
       else
      {
       double radius;//���ʰ뾶
       double dis,dis1,dis2,dis3;//����
       double cosA;//abȷ���ı�����Ӧ�Ľ�A��cosֵ
       dis1=hypot(a[0]-b[0],a[1]-b[1]);
       dis2=hypot(a[0]-c[0],a[1]-c[1]);
       dis3=hypot(b[0]-c[0],b[1]-c[1]);
       dis=dis2*dis2+dis3*dis3-dis1*dis1;
       cosA=dis/(2*dis2*dis3);//���Ҷ���
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
    float r = fmodf(theta + PI, 2*PI);  // ƽ�Ʋ�ȡģ
    r = fmodf(r + 2*PI, 2*PI);          // ȷ�������Ǹ�
    return r - PI;                        // ����ƽ��
}
float NormalizeAngle_toPi2(float theta) {
    theta = fabsf(theta); // ת���� [0, ��]
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
        hash = (hash * 31 + c) % mod; // ʹ��λ�ڴa���ƞ�15λ
    }
    return (uint32)hash;
}
