/*
 * Process.c
 *
 *  Created on: 2025��3��15��
 *      Author: minec
 */

#include "Process.h"

Step getStep(uint32 this[], uint32 time){
    Step ret = {0, time};
    for(; this[ret.step]; ++ret.step){
        if(ret.time < this[ret.step]){
            break;
        }else{
            ret.time -= this[ret.step];
        }
    }
    return ret;
}
