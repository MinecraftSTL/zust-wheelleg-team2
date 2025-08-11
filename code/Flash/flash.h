#ifndef __FLASH_H
#define __FLASH_H

#include <Motor.h>
#include "../pid/Pid.h"
#include "zf_driver_flash.h"

#define FLASH_KEY       0x09100721              //ºÏ≤È√‹‘ø

void Flash_Init(void);
void Flash_WriteAllVal(void);
void Flash_clear();

void Flash_pageAllRead();
void Flash_pageAllWrite();

extern uint8 flashStatus;

#endif
