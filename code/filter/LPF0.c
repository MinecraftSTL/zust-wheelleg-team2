/*
 * LPF0.c
 *
 *  Created on: 2025��4��11��
 *      Author: minec
 */
#include "Filter.h"

Filter0* LPF0_init(Filter0* this, float deltaMax) {
    this->deltaMax = deltaMax;
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
    }else if(delta > this->deltaMax){
        this->y+=this->deltaMax;
    }else if(delta < this->deltaMax){
        this->y-=this->deltaMax;
    }else{
        this->y=input;
    }
    return this->y;
}
