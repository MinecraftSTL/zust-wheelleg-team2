/*
 * Leg.h
 *
 *  Created on: 2025Äê1ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_LEG_LEG_H_
#define CODE_LEG_LEG_H_

#include "Sys.h"

#define servo_rb ATOM2_CH3_P11_6
#define servo_rf ATOM2_CH4_P11_9
#define servo_lf ATOM2_CH5_P11_10
#define servo_lb ATOM2_CH6_P11_11

#define DFB 35
#define LFU 60
#define LFD 90
#define LBU 60
#define LBD 90
#define DLR 160

#define LEG_MIN_Z 0
#define LEG_MAX_Z 140
#define LEG_MAX_X 50

struct LegServoAngle{
    double f, b;
};

extern const float defaultLegX, defaultLegZ;
extern float targetLegX, targetLegZ;

extern float defaultRollAlpha;

extern uint8 rollBalance;

void Leg_init();
int32 Roll_toPosZ(float roll, float lza);
void Leg_set_duty(float rb, float rf, float lf, float lb);
void Leg_set_pos(float lx, float lz, float rx, float rz);

extern const uint32 jumpStep[];
void jumpPit(uint32 period, float *legZ);
void jump();

#endif /* CODE_LEG_LEG_H_ */
