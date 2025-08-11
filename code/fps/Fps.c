/*
 * Fps.c
 *
 *  Created on: 2025Äê3ÔÂ23ÈÕ
 *      Author: minec
 */

#include "Fps.h"

uint16 sepNum;

uint16 f[1000] = {0};
uint32 fps = 0;
uint16 p = 0;

void Fps_init(uint32 period){
    zf_assert(0 < period && period <= 1000);
    sepNum = 1000/period;
}

void Fps_scan(){
    fps += f[p];
    ++p;
    p%=sepNum;
    fps -= f[p];
    f[p]=0;
}

void Fps_add(uint16 fNum){
    f[p]+=fNum;
}
