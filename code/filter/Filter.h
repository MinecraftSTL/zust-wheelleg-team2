/*
 * Filter.h
 *
 *  Created on: 2025年3月1日
 *      Author: minec
 */

#ifndef CODE_FILTER_FILTER_H_
#define CODE_FILTER_FILTER_H_

#include "Sys.h"

typedef struct {
    float alpha;     // 滤波系数
    float x;    // 上一次的输入值
    float y;    // 上一次的输出值
} Filter;

#endif /* CODE_FILTER_FILTER_H_ */
