/*
 * LPF.c
 *
 *  Created on: 2025年3月1日
 *      Author: minec
 */

#include "Filter.h"

void LPF_init(Filter* filter, float cutoff_freq, float sample_freq) {
    // 计算时间常数RC
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // 计算采样间隔
    float dt = 1.0f / sample_freq;
    // 计算滤波系数alpha
    filter->alpha = dt / (RC + dt);
    // 初始化上一个输出值为0（可根据需要修改初始值）
    filter->y = 0.0f;
}

// 更新滤波器状态并返回滤波后的值
float lpf(Filter* filter, float input) {
    // 应用一阶低通滤波公式
    filter->y = filter->alpha * input + (1.0f - filter->alpha) * filter->y;
    return filter->y;
}
