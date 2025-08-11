/*
 * Gyro.c
 *
 *  Created on: 2024Äê12ÔÂ24ÈÕ
 *      Author: minec
 */

#include <math.h>
#include "zf_common_headfile.h"

float aAx = 0, aAy = 0, aAz = 0;
float vAx = 0, vAy = 0, vAz = 0;
float vAx_ = 0, vAy_ = 0, vAz_ = 0;
float xAx = 0, xAy = 0, xAz = 0;
float aXx = 0, aXy =0, aXz = 0;
float downAy = 0;

uint16 gyro_pitMs = 0;

const float k = 13;

void gyro_init(uint16 pitMs){
    gyro_pitMs = pitMs;
    icm20602_init();
}
void gyro_get_acc(){
    icm20602_get_acc();
    aXx = imu660ra_acc_transition(icm20602_acc_x);
    aXy = imu660ra_acc_transition(icm20602_acc_y);
    aXz = imu660ra_acc_transition(icm20602_acc_z);
}
void gyro_get_gryo(){
    icm20602_get_gyro();
    vAx = (imu660ra_gyro_transition(icm20602_gyro_x))*gyro_pitMs/1000;
    vAy = (imu660ra_gyro_transition(icm20602_gyro_y))*gyro_pitMs/1000;
    vAz = (imu660ra_gyro_transition(icm20602_gyro_z))*gyro_pitMs/1000;
    xAx += vAx;
    xAy += vAy;
    xAz += vAz;
    aAx = vAx - vAx_;
    aAy = vAy - vAy_;
    aAz = vAz - vAz_;
    vAx_ = vAx;
    vAy_ = vAy;
    vAz_ = vAz;
}

void downAy_init(){
    gyro_get_acc();
    downAy = (180.0 / PI)*atan(aXy/aXz)+k;
}
