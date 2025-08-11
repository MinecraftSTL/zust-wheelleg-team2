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
#include <Flash.h>

uint8 g_flash_enable_flag = 1;                         //flash���ñ�־λ��Ϊ1�������ã�Ϊ0Ĭ������,externȫ�ֱ���

void Flash_Init(void)
{
    if(g_flash_enable_flag == 1)
    {
        uint32 flash_Wrote_status = 0;                     //flash��д��״̬������ԿΪ0d000721���������������д�룬���ݿ���
        uint8 flash_check_flag;
        flash_check_flag = flash_check(0,0);

        if(flash_check_flag == 1)
        {
            flash_buffer_clear();
            flash_read_page_to_buffer(0,0);
            flash_Wrote_status = flash_union_buffer[0].uint32_type;
            if(flash_Wrote_status == FLASH_KEY)                //��д��״̬������Կ�Ǻϣ���flash���ݶ���
            {
                //������
                zero_my_gyro_x = flash_union_buffer[32].float_type;
                zero_my_gyro_y = flash_union_buffer[33].float_type;
                zero_my_gyro_z = flash_union_buffer[34].float_type;
            }
        }
    }
}

void Flash_WriteAllVal(void)
{
    flash_buffer_clear();
    flash_union_buffer[0].uint32_type = FLASH_KEY;
    //������
    flash_union_buffer[32].float_type = zero_my_gyro_x;
    flash_union_buffer[33].float_type = zero_my_gyro_y;
    flash_union_buffer[34].float_type = zero_my_gyro_z;

    flash_write_page_from_buffer(0, 0);
    flash_buffer_clear();
}
