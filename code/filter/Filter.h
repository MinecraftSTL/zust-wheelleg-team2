/*
 * Filter.h
 *
 *  Created on: 2025年3月1日
 *      Author: MCSTL
 */

#ifndef CODE_FILTER_FILTER_H_
#define CODE_FILTER_FILTER_H_

#include "Sys.h"

typedef struct {
    float delta;     // 滤波系数
    float y;
}Filter0;
typedef struct {
    float alpha;     // 滤波系数
    float x, y;
}Filter1;
typedef struct {
    float b0, b1, b2; // 分子系数
    float a1, a2;     // 分母系数
    float x1, x2;     // 输入历史状态（x[n-1], x[n-2]）
    float y1, y2;     // 输出历史状态（y[n-1], y[n-2]）
}Filter2;

Filter0* LPF0_init(Filter0* this, float deltaMax);
void LPF0_clear(Filter0* this);
float lpf0(Filter0* this, float input);
Filter1* LPF1_init(Filter1* this, float cutoffFreq, float sampleFreq);
void LPF1_clear(Filter1* this);
float lpf1(Filter1* this, float input);
Filter2* LPF2_init(Filter2* this, float cutoffFreq, float Q, float sampleFreq);
float lpf2(Filter2* this, float input);
Filter1* HPF1_init(Filter1* this, float cutoffFreq, float sampleFreq);
void HPF1_clear(Filter1* this);
float hpf1(Filter1* this, float input);

void Filter_param_init();

extern Filter0 Filter0_xAx;
extern Filter1 Filter1_vXx, Filter1_turn, Filter1_speed;

#endif /* CODE_FILTER_FILTER_H_ */
