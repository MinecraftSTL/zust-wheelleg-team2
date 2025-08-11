/*********************************************************************************************************************
* CYT2BL3 Opensourec Library ���� CYT2BL3 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT2BL3 ��Դ���һ����
*
* CYT2BL3 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          motor_flash
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT2BL3
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2025-01-03       pudding            first version
********************************************************************************************************************/

#include "motor_flash.h"



//-------------------------------------------------------------------------------------------------------------------
// �������     ��� FLASH ���� ��ȡ
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_flash_read();
// ��ע��Ϣ       
//-------------------------------------------------------------------------------------------------------------------
void motor_flash_read(void)
{
    flash_read_page_to_buffer(MOTOR_FLASH_SECTOR, MOTOR_FLASH_PAGE, SAVE_FLASH_LENGTH);         // ��ȡ flash ҳ

    if(flash_union_buffer[SAVE_FLASH_LENGTH - 1].uint32_type != 0x4068)
    {
        motor_flash_write();
    }
    else
    {
         motor_left.zero_location                    =    flash_union_buffer[0].uint32_type ;  
         motor_left.rotation_direction               =    flash_union_buffer[1].uint32_type ;  
         motor_left.pole_pairs                       =    flash_union_buffer[2].uint32_type ;  
         motor_right.zero_location                   =    flash_union_buffer[3].uint32_type ;  
         motor_right.rotation_direction              =    flash_union_buffer[4].uint32_type ;  
         motor_right.pole_pairs                      =    flash_union_buffer[5].uint32_type ;  
    }

}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� FLASH ���� д��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_flash_write();
// ��ע��Ϣ       
//-------------------------------------------------------------------------------------------------------------------
void motor_flash_write(void)
{
    flash_union_buffer[0].uint32_type  = (uint32)motor_left.zero_location                ;  
    flash_union_buffer[1].uint32_type  = (uint32)motor_left.rotation_direction           ;  
    flash_union_buffer[2].uint32_type  = (uint32)motor_left.pole_pairs                   ;  
    flash_union_buffer[3].uint32_type  = (uint32)motor_right.zero_location               ;  
    flash_union_buffer[4].uint32_type  = (uint32)motor_right.rotation_direction          ;  
    flash_union_buffer[5].uint32_type  = (uint32)motor_right.pole_pairs                  ;  
    
    flash_union_buffer[SAVE_FLASH_LENGTH - 1].uint32_type = 0x4068;                             // flash У������ ����ȡflashʱУ��λ���� ����flashд��Ĭ�ϲ���
    
    flash_erase_page(MOTOR_FLASH_SECTOR, MOTOR_FLASH_PAGE);                                     // ���� flash ҳ

    flash_write_page_from_buffer(MOTOR_FLASH_SECTOR, MOTOR_FLASH_PAGE, SAVE_FLASH_LENGTH);      // д�� flash ҳ
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� FLASH ������ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_flash_init();
// ��ע��Ϣ       
//-------------------------------------------------------------------------------------------------------------------
void motor_flash_init(void)
{    
    flash_init();
    
    // δ������������ֱ�Ӷ�ȡ �ᴥ��NOROOT�����߱��� ����������������⴦�� ���Ῠ��
    motor_flash_read();                                                                          // ��ȡ flash
}










