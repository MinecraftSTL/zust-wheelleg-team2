/*
 * LPF.c
 *
 *  Created on: 2025��3��1��
 *      Author: minec
 */

#include "Filter.h"

void HighPassFilter_Init(Filter* filter, float cutoff_freq, float sample_freq) {
    // ����ʱ�䳣��RC
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // ����������
    float dt = 1.0f / sample_freq;
    // �����˲�ϵ��alpha
    filter->alpha = RC / (RC + dt);
    // ��ʼ����ʷ����
    filter->x = 0.0f;
    filter->y = 0.0f;
}

// �����˲���״̬�������˲����ֵ
float HighPassFilter_Update(Filter* filter, float input) {
    // ���������ֵ
    float delta_x = input - filter->x;
    // Ӧ�ø�ͨ�˲���ʽ
    float output = filter->alpha * (filter->y + delta_x);
    // ������ʷ����
    filter->y = output;
    filter->x = input;
    return output;
}
