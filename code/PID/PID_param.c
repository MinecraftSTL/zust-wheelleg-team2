/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID PID_WPitch, PID_vVx, PID_LPitch;

void PID_param_init(){
//    PID_init(&PID_WPitch, 200, 0, 800, 10000, 4000);
//    PID_init(&PID_vVx, 0.2, 0, 40, 10000, 10000);
//    PID_init(&PID_LPitch, 0.5, 0, 1, 10000, 100);
    PID_init(&PID_WPitch, 80, 0, 400, 10000, 4000);
    PID_init(&PID_vVx, 0.2, 0, 40, 10000, 10000);
    PID_init(&PID_LPitch, 0, 0, 0, 10000, 100);
}
