/*
 * process.h
 *
 *  Created on: 2023��12��13��
 *      Author: ¬����
 */

#ifndef CODE_GYROSCOPE_PROCESS_H_
#define CODE_GYROSCOPE_PROCESS_H_

#include "Myheadfile.h"
#include <math.h>

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float halfT);

#endif /* CODE_GYROSCOPE_PROCESS_H_ */
