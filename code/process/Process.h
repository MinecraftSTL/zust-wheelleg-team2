/*
 * Process.h
 *
 *  Created on: 2025Äê3ÔÂ15ÈÕ
 *      Author: MCSTL
 */

#ifndef CODE_PROCESS_PROCESS_H_
#define CODE_PROCESS_PROCESS_H_

#include "Sys.h"

typedef struct{
    uint8 step;
    uint32 time;
}Step;

Step getStep(const int32 this[], uint32 time);

#endif /* CODE_PROCESS_PROCESS_H_ */
