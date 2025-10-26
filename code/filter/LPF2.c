/*
 * LPF2.c
 *
 *  Created on: 2025年4月11日
 *      Author: MinecraftSTL
 */
#include "Filter.h"

Filter2* LPF2_init(Filter2* this, float cutoffFreq, float Q, float sampleFreq) {
    // 计算截止频率对应的角频率（预畸变校正）
    float omega0 = 2 * PI * cutoffFreq / sampleFreq;
    float alpha = sinf(omega0) / (2 * Q);

    // 计算双二次系数
    float b0 = (1 - cosf(omega0)) / 2;
    float b1 = 1 - cosf(omega0);
    float b2 = (1 - cosf(omega0)) / 2;
    float a0 = 1 + alpha;
    float a1 = -2 * cosf(omega0);
    float a2 = 1 - alpha;

    // 归一化系数
    this->b0 = b0 / a0;
    this->b1 = b1 / a0;
    this->b2 = b2 / a0;
    this->a1 = a1 / a0;
    this->a2 = a2 / a0;

    // 初始化状态为0
    this->x1 = 0;
    this->x2 = 0;
    this->y1 = 0;
    this->y2 = 0;

    return this;
}

float lpf2(Filter2* this, float input) {
    // 计算当前输出
    float output =
        this->b0 * input +
        this->b1 * this->x1 +
        this->b2 * this->x2 -
        this->a1 * this->y1 -
        this->a2 * this->y2;

    // 更新历史状态
    this->x2 = this->x1;
    this->x1 = input;
    this->y2 = this->y1;
    this->y1 = output;

    return output;
}
