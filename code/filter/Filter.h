/*
 * Filter.h
 *
 *  Created on: 2025��3��1��
 *      Author: minec
 */

#ifndef CODE_FILTER_FILTER_H_
#define CODE_FILTER_FILTER_H_

#include "Sys.h"

typedef struct {
    float alpha;     // �˲�ϵ��
    float x, y;
}Filter;

void LPF_init(Filter* this, float cutoff_freq, float sample_freq);
float lpf(Filter* this, float input);
void HPF_Init(Filter* this, float cutoff_freq, float sample_freq);
float hpf(Filter* this, float input);

extern Filter Filter_turn, Filter_speed, Filter_xAx;

#endif /* CODE_FILTER_FILTER_H_ */
