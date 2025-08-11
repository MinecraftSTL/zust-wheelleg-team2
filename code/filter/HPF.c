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
void HPF_Init(Filter* this, float cutoff_freq, float sample_freq) {
    // ����ʱ�䳣��RC
    float RC = 1.f / (2.f * PI * cutoff_freq);
    // ����������
    float dt = 1.f / sample_freq;
    // �����˲�ϵ��alpha
    this->alpha = RC / (RC + dt);
    // ��ʼ����ʷ����
    HPF_clear(this);
}

void HPF_clear(Filter* this){
    this->x = NAN;
    this->y = 0.f;
}

// �����˲���״̬�������˲����ֵ
float hpf(Filter* this, float input) {
    zf_assert(!!this);
    // ���������ֵ
    float delta_x = isnan(this->x) ? 0 : input-this->x;
    // Ӧ�ø�ͨ�˲���ʽ
    this->y = this->alpha * (isnan(this->y)?0:this->y + delta_x);
    // ������ʷ����
    this->x = input;
    return this->y;
}
