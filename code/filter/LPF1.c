/*
 * LPF.c
 *
 *  Created on: 2025��3��1��
 *      Author: minec
 */

#include "Filter.h"

// ��ʼ���˲�������
// cutoff_freq: ��ֹƵ��(Hz)
// sample_freq: ����Ƶ��(Hz)
Filter1* LPF1_init(Filter1* this, float cutoffFreq, float sampleFreq) {
    // ����ʱ�䳣��RC
    float RC = 1.f / (2.f * PI * cutoffFreq);
    // ����������
    float dt = 1.f / sampleFreq;
    // �����˲�ϵ��alpha
    this->alpha = dt / (RC + dt);
    // ��ʼ����ʷ����
    LPF1_clear(this);

    return this;
}

void LPF1_clear(Filter1* this){
    this->x = NAN;
    this->y = NAN;
}

// �����˲���״̬�������˲����ֵ
float lpf1(Filter1* this, float input) {
    zf_assert(!!this);
    // Ӧ��һ�׵�ͨ�˲���ʽ
    this->y = isnan(this->y) ? input : this->alpha*input+(1.0f-this->alpha)*this->y;
    this->x = input;
    return this->y;
}
