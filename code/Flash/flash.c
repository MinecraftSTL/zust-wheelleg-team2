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
#include <Flash.h>

uint8 g_flash_enable_flag = 1;                         //flash启用标志位，为1代表启用，为0默认启用,extern全局变量

void Flash_Init(void)
{
    if(g_flash_enable_flag == 1)
    {
        uint32 flash_Wrote_status = 0;                     //flash曾写入状态符，密钥为0d000721，若符合则代表曾写入，数据可信
        uint8 flash_check_flag;
        flash_check_flag = flash_check(0,0);

        if(flash_check_flag == 1)
        {
            flash_buffer_clear();
            flash_read_page_to_buffer(0,0);
            flash_Wrote_status = flash_union_buffer[0].uint32_type;
            if(flash_Wrote_status == FLASH_KEY)                //曾写入状态符与密钥吻合，将flash数据读出
            {
                //陀螺仪
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
    //陀螺仪
    flash_union_buffer[32].float_type = zero_my_gyro_x;
    flash_union_buffer[33].float_type = zero_my_gyro_y;
    flash_union_buffer[34].float_type = zero_my_gyro_z;

    flash_write_page_from_buffer(0, 0);
    flash_buffer_clear();
}
