/*
 * Gyro.c
 *
 *  Created on: 2024Äê12ÔÂ24ÈÕ
 *      Author: minec
 */

#include <math.h>
#include "zf_common_headfile.h"

const float k = 13;

float aAx = 0, aAy = 0, aAz = 0;
float vAx = 0, vAy = 0, vAz = 0;
float vAx_ = 0, vAy_ = 0, vAz_ = 0;
float xAx = 0, xAy = 0, xAz = 0;
float downAy = 0, VxDownAy = 0;

void downAy_init(){
    mpu6050_get_acc();
    downAy = (180.0 / PI)*atan(imu660ra_acc_transition(mpu6050_acc_y)/imu660ra_acc_transition(mpu6050_acc_z))+k;
}
