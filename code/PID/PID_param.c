/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID motorL, motorR, pitch, Vz;

void PID_param_init(){
    PID_Init(&motorL, 0, 0, 0, 10000, 6000);
    PID_Init(&motorR, 0, 0, 0, 10000, 6000);
    PID_Init(&pitch, 0, 0, 0, 10000, 1);
    PID_Init(&Vz, 0, 0, 0, 10000, 6000);
}
