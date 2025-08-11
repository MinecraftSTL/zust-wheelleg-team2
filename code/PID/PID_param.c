/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID PID_WvAy, PID_WxAy, PID_vVx, PID_WvAz, PID_vAz, PID_xAx;

void PID_param_init(){
    PID_init(&PID_WvAy, 12, 0, 0, 10000, 5000);
    PID_init(&PID_WxAy, 12, 0, 0, 10000, 10000);
    PID_init(&PID_vVx, 60, 0.03, 0, 1000000, 100000);
    PID_init(&PID_WvAz, 1, 0, 2, 10000, 10000);
    PID_init(&PID_vAz, 80, 0, 0, 10000, 10000);
    PID_init(&PID_xAx, 3, 0, 0, 100000, (LEG_MAX_Z-LEG_MIN_Z)*1000);
}
