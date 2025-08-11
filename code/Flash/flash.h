#ifndef __FLASH_H
#define __FLASH_H

#include "zf_driver_flash.h"
#include "Menu.h"
#include "Gyro.h"

#define FLASH_KEY       0x09100721              //ºÏ≤È√‹‘ø

void Flash_init(void);
void Flash_WriteAllVal(void);
void Flash_clear();

void Flash_menuClear();
void Flash_pageAllRead();
void Flash_pageAllWrite();
uint8 Flash_pageAllCheck();

extern uint8 flashStatus;

#endif
