/*
 * param.c
 *
 *  Created on: 2024��12��23��
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
