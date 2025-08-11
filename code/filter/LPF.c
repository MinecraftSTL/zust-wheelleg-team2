/*
 * LPF.c
 *
 *  Created on: 2025��3��1��
 *      Author: minec
 */

#include "Filter.h"

void LPF_init(Filter* filter, float cutoff_freq, float sample_freq) {
    // ����ʱ�䳣��RC
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // ����������
    float dt = 1.0f / sample_freq;
    // �����˲�ϵ��alpha
    filter->alpha = dt / (RC + dt);
    // ��ʼ����һ�����ֵΪ0���ɸ�����Ҫ�޸ĳ�ʼֵ��
    filter->y = 0.0f;
}

// �����˲���״̬�������˲����ֵ
float lpf(Filter* filter, float input) {
    // Ӧ��һ�׵�ͨ�˲���ʽ
    filter->y = filter->alpha * input + (1.0f - filter->alpha) * filter->y;
    return filter->y;
}
