/*
 * Sys.h
 *
 *  Created on: 2024Äê10ÔÂ27ÈÕ
 *      Author: sun
 */

#ifndef CODE_SYS_SYS_H_
#define CODE_SYS_SYS_H_

#include "MyHeadFile.h"
#include <stdint.h>
#include <math.h>

float Calculate_Curvature(int array[], int start, int size) ;
double curvature(double a[2],double b[2],double c[2]);
int collinear(double a[2],double b[2],double c[2]);
float Q_rsqrt( float number );
uint16 Bits_reverse(uint16 value, int bits);
float Angle_normalize(float theta);
float NormalizeAngle_toPi2(float theta);

#endif /* CODE_SYS_SYS_H_ */
