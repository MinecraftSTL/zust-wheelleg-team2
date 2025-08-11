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
void HPF_Init(Filter* this, float cutoff_freq, float sample_freq) {
    // 计算时间常数RC
    float RC = 1.f / (2.f * PI * cutoff_freq);
    // 计算采样间隔
    float dt = 1.f / sample_freq;
    // 计算滤波系数alpha
    this->alpha = RC / (RC + dt);
    // 初始化历史数据
    HPF_clear(this);
}

void HPF_clear(Filter* this){
    this->x = NAN;
    this->y = 0.f;
}

// 更新滤波器状态并返回滤波后的值
float hpf(Filter* this, float input) {
    zf_assert(!!this);
    // 计算输入差值
    float delta_x = isnan(this->x) ? 0 : input-this->x;
    // 应用高通滤波公式
    this->y = this->alpha * (isnan(this->y)?0:this->y + delta_x);
    // 更新历史数据
    this->x = input;
    return this->y;
}
