/*
 * Beep.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: minec
 */

#include "isr.h"

#include "Sys.h"

uint32 beepTime = 0;

void beepStart(){
    gpio_high(Beep);
}
void beepStop(){
    gpio_low(Beep);
}

void beep(uint32 time){
    beepTime = time;
}
void beepSTime(){
    beep(1);
}
void beepMTime(){
    beep(10);
}
void beepLTime(){
    beep(100);
}
