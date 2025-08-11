/*
 * process.h
 *
 *  Created on: 2023��12��13��
 *      Author: ¬����
 */

#ifndef CODE_GYROSCOPE_PROCESS_H_
#define CODE_GYROSCOPE_PROCESS_H_

#include "MyHeadfile.h"
extern float pitch, roll, yaw;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float halfT);
float LPF2_T2(float xin);

#endif /* CODE_GYROSCOPE_PROCESS_H_ */
