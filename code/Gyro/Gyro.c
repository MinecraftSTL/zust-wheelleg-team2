/*
 * Gyro.c
 *
 *  Created on: 2024Äê12ÔÂ24ÈÕ
 *      Author: minec
 */

#include <math.h>
#include "zf_common_headfile.h"

const float k = 13;

float Ax = 0, Ay = 0, Az = 0;
float downAx = 0;

void downAx_init(){
    mpu6050_get_acc();
    downAx = (180.0 / PI)*atan(imu660ra_acc_transition(mpu6050_acc_y)/imu660ra_acc_transition(mpu6050_acc_z))+k;
}
