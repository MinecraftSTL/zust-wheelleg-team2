/*
 * Leg.h
 *
 *  Created on: 2025Äê1ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_LEG_LEG_H_
#define CODE_LEG_LEG_H_

#include "Sys.h"

const pwm_channel_enum servo_rb = ATOM2_CH0_P33_4;
const pwm_channel_enum servo_rf = ATOM2_CH1_P33_5;
const pwm_channel_enum servo_lf = ATOM2_CH2_P33_6;
const pwm_channel_enum servo_lb = ATOM2_CH3_P33_7;

void Leg_init();
void Leg_set_duty(float rb, float rf, float lf, float lb);

#endif /* CODE_LEG_LEG_H_ */
