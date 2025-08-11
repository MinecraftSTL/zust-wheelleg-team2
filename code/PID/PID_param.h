/*
 * PID_param.h
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */

#ifndef CODE_PID_PID_PARAM_H_
#define CODE_PID_PID_PARAM_H_

#include "PID.h"
#include "Sys.h"

extern PID motorL, motorR, Vy, pitch;

void PID_param_init();

#endif /* CODE_PID_PID_PARAM_H_ */
