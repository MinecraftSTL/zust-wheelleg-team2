/*
 * Fps.c
 *
 *  Created on: 2025Äê3ÔÂ23ÈÕ
 *      Author: minec
 */

#include "Fps.h"

uint16 sepNum;

uint8 f[1000] = {0};
uint16 fps = 0;
uint16 p = 0;

void Fps_init(uint32 period){
    sepNum = 1000/period;
}

void Fps_scan(){
    fps += f[p];
    ++p;
    p%=sepNum;
    fps -= f[p];
    f[p]=0;
}

void Fps_add(uint8 fNum){
    f[p]+=fNum;
}
