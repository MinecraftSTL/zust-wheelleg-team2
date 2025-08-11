/*
 * LPF.c
 *
 *  Created on: 2025��3��1��
 *      Author: minec
 */

#include "Filter.h"

void HighPassFilter_Init(Filter* this, float cutoff_freq, float sample_freq) {
    // ����ʱ�䳣��RC
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // ����������
    float dt = 1.0f / sample_freq;
    // �����˲�ϵ��alpha
    this->alpha = RC / (RC + dt);
    // ��ʼ����ʷ����
    this->x = 0.0f;
    this->y = 0.0f;
}

// �����˲���״̬�������˲����ֵ
float HighPassFilter_Update(Filter* this, float input) {
    // ���������ֵ
    float delta_x = input - this->x;
    // Ӧ�ø�ͨ�˲���ʽ
    this->y = this->alpha * (this->y + delta_x);
    // ������ʷ����
    this->x = input;
    return this->y;
}
