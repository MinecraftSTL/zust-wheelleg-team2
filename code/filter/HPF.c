/*
 * LPF.c
 *
 *  Created on: 2025年3月1日
 *      Author: minec
 */

#include "Filter.h"

void HighPassFilter_Init(Filter* this, float cutoff_freq, float sample_freq) {
    // 计算时间常数RC
    float RC = 1.0f / (2.0f * PI * cutoff_freq);
    // 计算采样间隔
    float dt = 1.0f / sample_freq;
    // 计算滤波系数alpha
    this->alpha = RC / (RC + dt);
    // 初始化历史数据
    this->x = 0.0f;
    this->y = 0.0f;
}

// 更新滤波器状态并返回滤波后的值
float HighPassFilter_Update(Filter* this, float input) {
    // 计算输入差值
    float delta_x = input - this->x;
    // 应用高通滤波公式
    this->y = this->alpha * (this->y + delta_x);
    // 更新历史数据
    this->x = input;
    return this->y;
}
