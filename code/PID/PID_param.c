/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID PID_WvAy, PID_WxAy, PID_vVx, PID_LPitch, PID_xAx;

void PID_param_init(){
//    PID_init(&PID_WPitch, 200, 0, 800, 10000, 4000);
//    PID_init(&PID_vVx, 0.2, 0, 40, 10000, 10000);
//    PID_init(&PID_LPitch, 0.5, 0, 1, 10000, 100);
    PID_init(&PID_WvAy, 1, 0, 0, 10000, 4000);
    PID_init(&PID_WxAy, 80, 0, 400, 10000, 10000);
    PID_init(&PID_vVx, 80, 0.4, 0, 1000000, 10000);
    PID_init(&PID_LPitch, 0, 0, 0, 10000, 100);
    PID_init(&PID_xAx, 0.1, 0.05, 0.2, 10000, LEG_MAX_Z-LEG_MIN_Z);
}
