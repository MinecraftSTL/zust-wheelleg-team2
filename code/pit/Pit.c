/*
 * Pit.c
 *
 *  Created on: 2025Äê3ÔÂ16ÈÕ
 *      Author: minec
 */
#include "Pit.h"

const uint32 PIT00ms = 1;
const uint32 PIT01ms = 5;
const uint32 PIT10ms = 1;

void Pit_init(){
    pit_ms_init(CCU60_CH0, PIT00ms);
//    pit_ms_init(CCU60_CH1, PIT01ms);
    pit_ms_init(CCU61_CH0, PIT10ms);
}
