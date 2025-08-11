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
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // ����������
    float dt = 1.0f / sample_freq;
    // �����˲�ϵ��alpha
    this->alpha = dt / (RC + dt);
    // ��ʼ����һ�����ֵΪ0���ɸ�����Ҫ�޸ĳ�ʼֵ��
    this->x = 0.0f;
    this->y = 0.0f;
}

// �����˲���״̬�������˲����ֵ
float lpf(Filter* this, float input) {
    // Ӧ��һ�׵�ͨ�˲���ʽ
    this->y = isnan(this->y) ? input : this->alpha*input+(1.0f-this->alpha)*this->y;
    this->x = input;
    return this->y;
}
