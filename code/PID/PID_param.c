/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "PID_param.h"

PID motorL, motorR, pitch, Vy;
float TVyX = 0, tVy_ = 0, tdVy_ = 0;
uint32 TVyTms = 0;
const uint32 TVyTMax = 1000;

void PID_param_init(){
    PID_init(&motorL, 10, 1, 10, 10000, 6000);
    PID_init(&motorR, 10, 1, 10, 10000, 6000);
    PID_init(&pitch, 10, 0, 300, 10000, 2000);
    PID_init(&Vy, 100, 0, 20, 10000, 20000);
}
