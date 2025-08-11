/*
 * Beep.h
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */

#ifndef CODE_BEEP_BEEP_H_
#define CODE_BEEP_BEEP_H_

#include "zf_common_headfile.h"

extern uint32 beepTime;

void beepStart();
void beepStop();

void beep(uint32 time);
void beepSTime();
void beepMTime();
void beepLTime();


#endif /* CODE_BEEP_BEEP_H_ */
