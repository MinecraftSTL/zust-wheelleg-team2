/*
 * Process.c
 *
 *  Created on: 2025��3��15��
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
