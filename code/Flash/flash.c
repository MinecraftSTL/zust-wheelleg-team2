/*
 * Flashbluetooth.c
 *
 *  Created on: 2024��10��16��
 *      Author: SJX
 */

/*
 * ��Կ0x0d000721 ����0:0
 * �ٶȻ�PID��������ռ�0:32-47  ������32-39����40-47
 * ת��PID��������ռ�0:48-63
 * ��׼�ٶȲ�������ռ�0:64-71
 */
#include "Flash.h"

uint8 flashStatus = 1;                         //flash���ñ�־λ��Ϊ1�������ã�Ϊ0Ĭ������,externȫ�ֱ���

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
    if(flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type != FLASH_KEY)                //��д��״̬������Կ�Ǻϣ���flash���ݶ���
    {
        zf_log(0, "!=FLASH_KEY");
        return;
    }
    //������
    zero_my_gyro_x = flash_union_buffer[0].float_type;
    zero_my_gyro_y = flash_union_buffer[1].float_type;
    zero_my_gyro_z = flash_union_buffer[2].float_type;
    printf("%f,%f,%f\r\n",zero_my_gyro_x,zero_my_gyro_y,zero_my_gyro_z);
}

void Flash_WriteAllVal(void)
{
    flash_buffer_clear();
    flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type = FLASH_KEY;
    //������
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
