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
void HPF1_Init(Filter1* this, float cutoffFreq, float sampleFreq) {
    // ����ʱ�䳣��RC
    float RC = 1.f / (2.f * PI * cutoffFreq);
    // ����������
    float dt = 1.f / sampleFreq;
    // �����˲�ϵ��alpha
    this->alpha = RC / (RC + dt);
    // ��ʼ����ʷ����
    HPF1_clear(this);
}

void HPF1_clear(Filter1* this){
    this->x = NAN;
    this->y = 0.f;
}

// �����˲���״̬�������˲����ֵ
float hpf1(Filter1* this, float input) {
    zf_assert(!!this);
    // ���������ֵ
    float delta_x = isnan(this->x) ? 0 : input-this->x;
    // Ӧ�ø�ͨ�˲���ʽ
    this->y = this->alpha * (isnan(this->y)?0:this->y + delta_x);
    // ������ʷ����
    this->x = input;
    return this->y;
}
