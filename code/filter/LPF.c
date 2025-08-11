/*
 * LPF.c
 *
 *  Created on: 2025年3月1日
 *      Author: minec
 */

#include "Filter.h"

// 初始化滤波器参数
// cutoff_freq: 截止频率(Hz)
// sample_freq: 采样频率(Hz)
void LPF_init(Filter* this, float cutoff_freq, float sample_freq) {
    // 计算时间常数RC
    float RC = 1.f / (2.f * PI * cutoff_freq);
    // 计算采样间隔
    float dt = 1.f / sample_freq;
    // 计算滤波系数alpha
    this->alpha = dt / (RC + dt);
    // 初始化历史数据
    this->x = NAN;
    this->y = NAN;
}

// 更新滤波器状态并返回滤波后的值
float lpf(Filter* this, float input) {
    zf_assert(!!this);
    // 应用一阶低通滤波公式
    this->y = isnan(this->y) ? input : this->alpha*input+(1.0f-this->alpha)*this->y;
    this->x = input;
    return this->y;
}
