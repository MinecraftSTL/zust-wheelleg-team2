/*
 * Beep.h
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */

#ifndef CODE_BEEP_BEEP_H_
#define CODE_BEEP_BEEP_H_

#include "zf_common_headfile.h"

#define Beep P15_6
#define Beep1 P15_7

extern uint32 beepTime;

void beep_init();
void beep_start();
void beep_stop();

void beep(uint32 time);
void beepShort();
void beepMid();
void beepLong();


#endif /* CODE_BEEP_BEEP_H_ */
