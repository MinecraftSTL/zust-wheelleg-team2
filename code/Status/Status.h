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
    status_car_stop,
    status_car_gyroscope_run,
    status_car_start,
}Car_Status;

typedef enum
{
    close_status,
    open_status,
}Status_Flag;

extern Car_Status carStatus;

void Car_Stop(void);
void Car_Start(void);


#endif
