/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID PID_WvAy, PID_WxAy, PID_WvAz, PID_vVx, PID_LPitch, PID_xAx;

void PID_param_init(){
//    PID_init(&PID_WPitch, 200, 0, 800, 10000, 4000);
//    PID_init(&PID_vVx, 0.2, 0, 40, 10000, 10000);
//    PID_init(&PID_LPitch, 0.5, 0, 1, 10000, 100);
//    PID_init(&PID_WvAy, 6, 0, 2, 10000, 5000);
//    PID_init(&PID_WxAy, 80, 0, 400, 10000, 10000);
//    PID_init(&PID_vVx, 8, 0.04, 0, 100000, 10000);
//    PID_init(&PID_LPitch, 0, 0, 0, 10000, 100);
//    PID_init(&PID_WvAy, 6, 0, 2, 10000, 5000);
//    PID_init(&PID_WxAy, 4, 0, 0, 10000, 10000);
//    PID_init(&PID_vVx, 16, 0.1, 0, 1000000, 100000);
//    PID_init(&PID_LPitch, 0, 0, 0, 10000, 100);
    PID_init(&PID_WvAy, 6, 0, 12, 10000, 5000);
    PID_init(&PID_WxAy, 40, 0, 20, 10000, 10000);
    PID_init(&PID_WvAz, 0, 0, 0, 10000, 10000);
    PID_init(&PID_vVx, 40, 0.03, 0, 1000000, 100000);
    PID_init(&PID_LPitch, 0, 0, 0, 10000, 100);
    PID_init(&PID_xAx, 3, 0, 0, 100000, (LEG_MAX_Z-LEG_MIN_Z)*1000);
}
