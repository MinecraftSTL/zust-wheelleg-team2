/*
 * Beep.c
 *
 *  Created on: 2024Äê12ÔÂ23ÈÕ
 *      Author: MinecraftSTL
 */
#include "Sys.h"
#include "Beep.h"

#include "isr.h"


int volume = 3;

uint32 beepTime = 0;

void beep_init(){
    gpio_init(Beep, GPO, 0, GPO_PUSH_PULL);
    gpio_init(BeepLouder, GPO, 0, GPO_PUSH_PULL);
}

void beep_start(){
    gpio_set_level(Beep, volume & 0x01);
    gpio_set_level(BeepLouder, volume & 0x02);
}
void beep_stop(){
    gpio_low(Beep);
    gpio_low(BeepLouder);
}

void beep(uint32 time){
    if(time > beepTime){
        beepTime = time;
    }
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
