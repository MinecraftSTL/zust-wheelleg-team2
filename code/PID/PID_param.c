/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "Pid_param.h"

PID PID_vAy, PID_xAy, PID_WvXx, PID_vXx, PID_WvAz, PID_vAz;

void PID_param_init(){
    PID_init(&PID_vAy, 20, 0, 40, 1000, 5000);
    PID_init(&PID_xAy, 30, 0, 60, 10000, 10000);
    PID_init(&PID_WvXx, 10, 0.2, 0, 10000, 20000);
    PID_init(&PID_vXx, 0, 0, 0, 0, 5000);
    PID_init(&PID_WvAz, 8, 0, 0, 10000, 10000);
    PID_init(&PID_vAz, 12, 0, 0, 10000, 10000);
}
