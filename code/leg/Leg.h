/*
 * Leg.h
 *
 *  Created on: 2025Äê1ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_LEG_LEG_H_
#define CODE_LEG_LEG_H_

#include "Sys.h"

#define servo_rb ATOM2_CH0_P33_4
#define servo_rf ATOM2_CH1_P33_5
#define servo_lf ATOM2_CH2_P33_6
#define servo_lb ATOM2_CH3_P33_7

#define DFB 35
#define LFU 60
#define LFD 90
#define LBU 60
#define LBD 90

#define MAX_Z 120
#define MAX_X 90

struct ServoAngle{
    double f, b;
};

void Leg_init();
void Leg_set_duty(double rb, double rf, double lf, double lb);
void Leg_set_pos(int32 lx, int32 lz, int32 rx, int32 rz);

#endif /* CODE_LEG_LEG_H_ */
