/*
 * LPF.c
 *
 *  Created on: 2025年3月1日
 *      Author: minec
 */

#include "Filter.h"

void HighPassFilter_Init(Filter* filter, float cutoff_freq, float sample_freq) {
    // 计算时间常数RC
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // 计算采样间隔
    float dt = 1.0f / sample_freq;
    // 计算滤波系数alpha
    filter->alpha = RC / (RC + dt);
    // 初始化历史数据
    filter->x = 0.0f;
    filter->y = 0.0f;
}

// 更新滤波器状态并返回滤波后的值
float HighPassFilter_Update(Filter* filter, float input) {
    // 计算输入差值
    float delta_x = input - filter->x;
    // 应用高通滤波公式
    float output = filter->alpha * (filter->y + delta_x);
    // 更新历史数据
    filter->y = output;
    filter->x = input;
    return output;
}
