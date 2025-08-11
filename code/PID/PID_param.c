/*
 * param.c
 *
 *  Created on: 2024��12��23��
 *      Author: minec
 */
#include "PID_param.h"

PID pitch, Vx;

void PID_param_init(){
    PID_init(&pitch, 0, 0, 0, 10000, 2000);
    PID_init(&Vx, 0, 0, 0, 10000, 100000);
}
