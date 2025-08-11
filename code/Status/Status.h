/*
 * Test.h
 *
 *  Created on: 2024Äê10ÔÂ24ÈÕ
 *      Author: SJX
 */
#ifndef    __SUATUS_H
#define    __SUATUS_H

#include "MyHeadFile.h"

typedef enum
{
    CAR_STOP,
    CAR_START,
    CAR_BALANCE,
    CAR_RUN,
}CarStatus;

typedef enum
{
    close_status,
    open_status,
}Status_Flag;

extern CarStatus carStatus;

void CarStatus_set(CarStatus this);


#endif
