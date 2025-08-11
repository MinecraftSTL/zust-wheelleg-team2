/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID pitch, Vx;

void PID_param_init(){
    PID_init(&pitch, 200, 0, 800, 10000, 4000);
    PID_init(&Vx, 40, 0.2, 0, 1000000, 100000);
}
