/*
 * Beep.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */
#include "Beep.h"

#include "isr.h"

#include "Sys.h"

uint32 beepTime = 0;

void beep_init(){
    gpio_init(Beep, GPO, 0, GPO_PUSH_PULL);
}

void beep_start(){
    gpio_high(Beep);
}
void beep_stop(){
    gpio_low(Beep);
}

void beep(uint32 time){
    beepTime = time;
}
void beepShort(){
    beep(1);
}
void beepMid(){
    beep(10);
}
void beepLong(){
    beep(100);
}
