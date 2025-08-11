/*
 * process.h
 *
 *  Created on: 2023年12月13日
 *      Author: 卢劲涵
 */

#ifndef CODE_GYROSCOPE_PROCESS_H_
#define CODE_GYROSCOPE_PROCESS_H_

#include "MyHeadfile.h"

extern float Gyro_Kp; // 这里的KpKi是用于调整加速度计修正陀螺仪的速度

extern float Gyro_Ki;

extern float pitch, roll, yaw;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float halfT);

#endif /* CODE_GYROSCOPE_PROCESS_H_ */
