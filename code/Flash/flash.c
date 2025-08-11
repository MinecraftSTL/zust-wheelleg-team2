/*
 * Flashbluetooth.c
 *
 *  Created on: 2024年10月16日
 *      Author: SJX
 */

/*
 * 密钥0x0d000721 分配0:0
 * 速度环PID参数分配空间0:32-47  其中左32-39，右40-47
 * 转向环PID参数分配空间0:48-63
 * 基准速度参数分配空间0:64-71
 */
#include "Flash.h"

uint8 flashStatus = 1;                         //flash启用标志位，为1代表启用，为0默认启用,extern全局变量

void Flash_init(void)
{
    if(!flashStatus)
    {
        return;
    }

    if(!flash_check(0,EEPROM_PAGE_NUM-1))
    {
        zf_log(0, "!flash_check(0,EEPROM_PAGE_NUM-1)");
        return;
    }
    flash_buffer_clear();
    flash_read_page_to_buffer(0,EEPROM_PAGE_NUM-1);
    if(flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type != FLASH_KEY)                //曾写入状态符与密钥吻合，将flash数据读出
    {
        zf_log(0, "!=FLASH_KEY");
        return;
    }
    //陀螺仪
    zero_my_gyro_x = flash_union_buffer[0].float_type;
    zero_my_gyro_y = flash_union_buffer[1].float_type;
    zero_my_gyro_z = flash_union_buffer[2].float_type;
    printf("%f,%f,%f\r\n",zero_my_gyro_x,zero_my_gyro_y,zero_my_gyro_z);
}

void Flash_WriteAllVal(void)
{
    flash_buffer_clear();
    flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type = FLASH_KEY;
    //陀螺仪
    flash_union_buffer[0].float_type = zero_my_gyro_x;
    flash_union_buffer[1].float_type = zero_my_gyro_y;
    flash_union_buffer[2].float_type = zero_my_gyro_z;

    flash_write_page_from_buffer(0,EEPROM_PAGE_NUM-1);
    flash_buffer_clear();
}

void Flash_clear(){
    for(uint32 i=0; i<EEPROM_PAGE_NUM; ++i){
        flash_erase_page(0,i);
    }
}
void Flash_menuClear(){
    for(uint32 i=0; i<EEPROM_PAGE_NUM-1; ++i){
        flash_erase_page(0,i);
    }
}

void Flash_pageRead(Page *page){
    uint8 exitCode = Page_readFlash(page);
    if(exitCode == 2){
        char path[PAGE_PATH_MAX+1];
        Page_getPath(page, path);
        zf_log(0, path);
    }
}
void Flash_pageWrite(Page *page){
    uint8 exitCode = Page_writeFlash(page, 1);
    if(exitCode == 2){
        char path[PAGE_PATH_MAX+1];
        Page_getPath(page, path);
        zf_log(0, path);
    }
}
void Flash_pageAllRead(){
    Page_allSubRun(&menu_main, Flash_pageRead);
}
void Flash_pageAllWrite(){
    Page_allSubRun(&menu_main, Flash_pageWrite);
    for(uint32 i=0; i<EEPROM_PAGE_NUM-1; ++i){
        flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];
        flash_read_page(0, i, flash_union_buffer, EEPROM_PAGE_LENGTH);
        flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type = FLASH_KEY^i;
        flash_write_page(0, i, flash_union_buffer, EEPROM_PAGE_LENGTH);
    }
}
