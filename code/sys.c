/*
���ߣ�Charon and ������С��Ƭ
       δ����Ȩ��ֹת��
 */



#include "sys.h"

char annulus_open_or_close_flag = 0;
char cross_open_or_close_flag = 0;
char image_open_or_close_flag = 0;
char barrier__open_or_close_flag = 0;
char flash_open_or_close_flag = 0;
char s_road_open_or_close_flag = 0;

/*****************
 * ***************
 //////��С���˷�///
 * **************
 ***************/
float slope_calculate (uint8 begin, uint8 end,int * border)
{
    float xsum = 0, ysum = 0,xysum = 0, x2sum = 0;
    int16  i = 0;
    float result = 0;
    static float resultlast;
    for (i = begin; i < end; i++)
    {
        xsum += i;
        ysum += border[i];
        xysum += i * (border[i]);
        x2sum += i * i;
    }
    if ((end - begin) * x2sum - xsum * xsum) //�жϳ����Ƿ�Ϊ��
    {
    result = ((end - begin) * xysum - xsum * ysum) / ((end - begin) * x2sum - xsum * xsum);
    resultlast = result;
    }
    else
    {
        result = resultlast;
    }
    return result;
}
/************
 ************
 **����б�ʽؾ�**
 ************
 ************/
void caculate_distance(uint8 start,uint8 end,int *border,float *slope_new,float *distance_new)
{
    uint16 i, num = 0;
    uint16 xsum = 0,ysum = 0;
            float y_average, x_average;
            num = 0;
            xsum = 0;
            ysum = 0;
            y_average = 0;
            x_average = 0;
            for (i = start; i < end; i++)
            {
                xsum += i;
                ysum += border[i];
                num++;
            }
            // �������ƽ����
                if(num)
            {
                x_average = (float)(xsum / num);
                y_average = (float)(ysum / num);
                y_average = (float)(ysum / num);
                }
                /*����б��*/
                *slope_new = slope_calculate(start,end,border);//б��
                *distance_new = y_average - (*slope_new)*x_average;//�ؾ�
}
void beep_on(void)
{
    gpio_high (Beep);
    //gpio_init(Beep, GPO, 0, GPO_PUSH_PULL);
}
void beep_off(void)
{
    gpio_low (Beep);
}
void beep_flip(void)
{
    gpio_toggle_level(Beep);
    //gpio_set_level (Beep, 0);
}




