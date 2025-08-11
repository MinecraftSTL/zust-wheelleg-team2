/*
 * Beep.c
 *
 *  Created on: 2024��12��23��
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
void beepShort(){
    beep(1);
}
void beepMid(){
    beep(10);
}
void beepLong(){
    beep(100);
}
