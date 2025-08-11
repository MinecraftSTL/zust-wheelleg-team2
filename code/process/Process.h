/*
 * Process.h
 *
 *  Created on: 2025��3��15��
 *      Author: minec
 */

#ifndef CODE_PROCESS_PROCESS_H_
#define CODE_PROCESS_PROCESS_H_

#include <Sys.h>

typedef struct{
    uint8 step;
    uint32 time;
}Step;

Step getStep(const uint32 this[], uint32 time);

#endif /* CODE_PROCESS_PROCESS_H_ */
