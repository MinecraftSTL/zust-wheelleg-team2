/*
 * Process.c
 *
 *  Created on: 2025Äê3ÔÂ15ÈÕ
 *      Author: MCSTL
 */

#include "Process.h"

Step getStep(const int32 this[], uint32 time){
    Step ret = {0, time};
    for(; this[ret.step]>=0; ++ret.step){
        if(ret.time < this[ret.step]){
            break;
        }else{
            ret.time -= this[ret.step];
        }
    }
    return ret;
}
