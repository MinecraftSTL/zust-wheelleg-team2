/*
 * gyroscope.c
 *
 *  Created on: 2023年10月28日
 *      Author: 卢劲涵
 */
#include "Gyro.h"
#include "Beep.h"

float Sum_my_gyro_z = 0,Sum_my_gyro_y = 0,Sum_my_gyro_x = 0,Sum_my_acc_z = 0,Sum_my_acc_y = 0,Sum_my_acc_x = 0;
float zero_my_gyro_z = 0,zero_my_gyro_y = 0,zero_my_gyro_x = 0,zero_my_acc_z = 0,zero_my_acc_y = 0,zero_my_acc_x = 0;
float gyro_z = 0,gyro_y = 0,gyro_x = 0,acc_z = 0,acc_y = 0,acc_x = 0;
float Lowpass_Alpha = 0.3;

void gyro_init()
{
    my_gyroscope_init();
    Set_LowpassFilter_Range_ICM42688(ICM42688_AFS_8G, ICM42688_AODR_1000HZ, ICM42688_GFS_1000DPS, ICM42688_GODR_1000HZ);
}
void gyro_set(){
    system_delay_ms(1000);
    int i = 0;
    Sum_my_gyro_z=0;
    Sum_my_gyro_y=0;
    Sum_my_gyro_x=0;

    pit_all_close();
    beep_start();
    for(i=0;i<1000;++i)
    {
        system_delay_ms(1);
        my_get_gyro();
        Sum_my_gyro_z+=my_gyro_z;
        Sum_my_gyro_y+=my_gyro_y;
        Sum_my_gyro_x+=my_gyro_x;
    }
    Pit_init();
    zero_my_gyro_z=Sum_my_gyro_z/1000;
    zero_my_gyro_y=Sum_my_gyro_y/1000;
    zero_my_gyro_x=Sum_my_gyro_x/1000;
    Flash_WriteAllVal();
}

void get_gyro()
{
	int16 new_gyro_x = 0;
    int16 new_gyro_z = 0;
	int16 new_gyro_y = 0;
    
	new_gyro_z=my_gyro_z-zero_my_gyro_z;

//    if(((new_gyro_z>-2)&&(new_gyro_z<2)))
//    {
//        new_gyro_z = 0;
//    }
//    (new_gyro_z) > (3000) ? (3000) : ((new_gyro_z) < -(3000) ? -(3000) : (new_gyro_z));//限幅

    gyro_z=new_gyro_z * PI / 180.0;

    //acc_z = Lowpass_Alpha * acc_z + (1 - Lowpass_Alpha) * (float)my_acc_z;
    acc_z = my_acc_z * Gravity_G;

    new_gyro_y=my_gyro_y-zero_my_gyro_y;
//    if((new_gyro_y>-3)&&(new_gyro_y<3))
//    {
//        new_gyro_y = 0;
//    }
    gyro_y=new_gyro_y * PI / 180.0;

   // acc_y = Lowpass_Alpha * acc_y + (1 - Lowpass_Alpha) * (float)my_acc_y;
    acc_y = my_acc_y * Gravity_G;

    new_gyro_x=my_gyro_x-zero_my_gyro_x;
//    if((new_gyro_x>-2)&&(new_gyro_x<2))
//    {
//        new_gyro_x = 0;
//    }
    gyro_x=new_gyro_x * PI / 180.0;

   // acc_x = Lowpass_Alpha * acc_x + (1 - Lowpass_Alpha) * (float)my_acc_x;
    acc_x = my_acc_x * Gravity_G;

}
//float gyro_z_res = 0,gyro_x_res = 0,gyro_y_res = 0;
void get_gyorscope_data(void)
{
    //陀螺仪数据
    get_gyro();
//    printf("%f, %f, %f\r\n", gyro_x,gyro_y,gyro_z);
//    gyro_x_res = LPF2_T2(gyro_x);
//	gyro_y_res = LPF2_T2(gyro_y);
//    gyro_z_res = LPF2_T2(gyro_z);

    IMUupdate(gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z, PIT10ms/2000.);
}

void Update_GyroData(void)
{
    my_get_acc();
    my_get_gyro();
    get_gyorscope_data();
//    printf("%f, %f, %f\r\n", pitch, roll, yaw);
}
