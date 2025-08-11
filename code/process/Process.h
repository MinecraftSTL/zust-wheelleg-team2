/*
 * Process.h
 *
 *  Created on: 2025Äê3ÔÂ15ÈÕ
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
