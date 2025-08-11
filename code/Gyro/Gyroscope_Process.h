/*
 * process.h
 *
 *  Created on: 2023Äê12ÔÂ13ÈÕ
 *      Author: Â¬¾¢º­
 */

#ifndef CODE_GYROSCOPE_PROCESS_H_
#define CODE_GYROSCOPE_PROCESS_H_

#include "Myheadfile.h"
#include <math.h>

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float halfT);

#endif /* CODE_GYROSCOPE_PROCESS_H_ */
