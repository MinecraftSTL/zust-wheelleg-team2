/*
 * LPF0.c
 *
 *  Created on: 2025年4月11日
 *      Author: minec
 */
#include "Filter.h"

Filter0* LPF0_init(Filter0* this, float deltaMax) {
    this->deltaMax = deltaMax;
    // 初始化历史数据
    LPF1_clear(this);

    return this;
}

void LPF0_clear(Filter0* this){
    this->y = NAN;
}

// 更新滤波器状态并返回滤波后的值
float lpf0(Filter0* this, float input) {
    float delta = input - this->y;
    if(isnan(this->y)){
        this->y=input;
    }else if(delta > this->deltaMax){
        this->y+=this->deltaMax;
    }else if(delta < this->deltaMax){
        this->y-=this->deltaMax;
    }else{
        this->y=input;
    }
    return this->y;
}
