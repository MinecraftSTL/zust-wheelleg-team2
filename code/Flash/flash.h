#ifndef __FLASH_H
#define __FLASH_H

#include "../pid/Pid.h"
#include "zf_driver_flash.h"
#include "MOTOR.h"

#define FLASH_KEY       0x0d000721              //ºÏ≤È√‹‘ø

void Flash_Init(void);
void Flash_WriteAllVal(void);

extern uint8 g_flash_enable_flag;

#endif
