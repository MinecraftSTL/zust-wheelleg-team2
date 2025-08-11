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

extern uint64 allRunMs;

extern CarStatus carStatus;

extern uint8 angleProtect;
extern uint8 fpsProtect;
extern uint8 timeProtect;
extern uint8 circleProtect;

extern uint8 kill;

extern uint64 timeProtectT;
extern uint64 timeProtectNowT;

extern int circleProtectN;
extern uint64 circleProtectT;
extern int circleProtectNowN;

void CarStatus_set(CarStatus this);
void CarStatus_update();
void CarStatus_menu_update(Page *page);
void CarStatus_add();
void CarStatus_sub();
void CarStatus_stop();
void CarStatus_pit(uint32 period);
void CarStatus_CameraStatus_zebra();
void CarStatus_CameraStatus_circle();

#endif
