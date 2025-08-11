/*
 * PID_param.h
 *
 *  Created on: 2024��12��23��
 *      Author: minec
 */

#ifndef CODE_PID_PID_PARAM_H_
#define CODE_PID_PID_PARAM_H_

#include "Sys.h"
#include "Pid.h"

extern PID PID_vAy, PID_xAy, PID_vXx, PID_WvAz, PID_vAz;

void PID_param_init();

#endif /* CODE_PID_PID_PARAM_H_ */
