/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID motorL, motorR, Vy, pitch;

void PID_param_init(){
    PID_init(&motorL, 10, 1, 10, 10000, 6000);
    PID_init(&motorR, 10, 1, 10, 10000, 6000);
    PID_init(&pitch, 10, 0, 200, 10000, 2000);
    PID_init(&Vy, 5, 0, 1, 10000, 20000);
}
