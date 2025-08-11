/*
 * param.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include <Pid_param.h>

PID PID_WvAy, PID_WxAy, PID_vVx, PID_WvAz, PID_vAz;

void PID_param_init(){
    PID_init(&PID_WvAy, 10, 0, 0, 32500, 5000);
    PID_init(&PID_WxAy, 30, 0, 0, 10000, 10000);
    PID_init(&PID_vVx, 40, 0.1, 0, 50000, 20000);
    if(PID_vVx.Ki != 0){
        PID_vVx.Max_I = PID_vVx.Max_Out/PID_vVx.Ki;
    }
    PID_init(&PID_WvAz, 8, 0, 16, 10000, 10000);
    PID_init(&PID_vAz, 10, 0, 0, 10000, 10000);
}
