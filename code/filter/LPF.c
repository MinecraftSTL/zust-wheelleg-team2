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
void LPF_init(Filter* this, float cutoff_freq, float sample_freq) {
    // ����ʱ�䳣��RC
    float RC = 1.f / (2.f * PI * cutoff_freq);
    // ����������
    float dt = 1.f / sample_freq;
    // �����˲�ϵ��alpha
    this->alpha = dt / (RC + dt);
    // ��ʼ����ʷ����
    this->x = NAN;
    this->y = NAN;
}

// �����˲���״̬�������˲����ֵ
float lpf(Filter* this, float input) {
    zf_assert(!!this);
    // Ӧ��һ�׵�ͨ�˲���ʽ
    this->y = isnan(this->y) ? input : this->alpha*input+(1.0f-this->alpha)*this->y;
    this->x = input;
    return this->y;
}
