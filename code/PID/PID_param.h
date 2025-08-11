/*
 * PID_param.h
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */

#ifndef CODE_PID_PID_PARAM_H_
#define CODE_PID_PID_PARAM_H_

#include <Sys.h>
#include "Pid.h"

extern PID PID_WvAy, PID_WxAy, PID_vVx, PID_WvAz, PID_vAz;

void PID_param_init();

#endif /* CODE_PID_PID_PARAM_H_ */
