/*
 * LPF0.c
 *
 *  Created on: 2025��4��11��
 *      Author: MinecraftSTL
 */
#include "Filter.h"

Filter0* LPF0_init(Filter0* this, float deltaMax) {
    this->delta = deltaMax;
    // ��ʼ����ʷ����
    LPF1_clear(this);

    return this;
}

void LPF0_clear(Filter0* this){
    this->y = NAN;
}

// �����˲���״̬�������˲����ֵ
float lpf0(Filter0* this, float input) {
    float delta = input - this->y;
    if(isnan(this->y)){
        this->y=input;
    }else if(delta > this->delta){
        this->y+=this->delta;
    }else if(delta < -this->delta){
        this->y-=this->delta;
    }else{
        this->y=input;
    }
    return this->y;
}
