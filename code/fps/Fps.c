/*
 * Fps.c
 *
 *  Created on: 2025Äê3ÔÂ23ÈÕ
 *      Author: MCSTL
 */

#include "Fps.h"

void Fps_init(Fps *this, uint32 period){
    zf_assert(0 < period && period <= 1000);
    this->sepNum = 1000/period;
    memset(this->f, 0x0, sizeof(uint16)*1001);
    this->fps = 0;
    this->p = 0;
}

void Fps_scan(Fps *this){
    this->fps += this->f[this->p];
    this->p=(this->p+1)%this->sepNum;
    this->fps -= this->f[this->p];
    this->f[this->p]=0;
}

void Fps_add(Fps *this, uint16 fNum){
    this->f[this->p]+=fNum;
}
