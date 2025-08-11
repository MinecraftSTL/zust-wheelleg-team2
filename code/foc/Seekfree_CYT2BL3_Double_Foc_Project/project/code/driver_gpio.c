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
* �ļ�����          driver_gpio
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

#include "driver_gpio.h"

uint32 gpio_loop_count = 0;

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ GPIOѭ����⺯��
// ����˵��     void
// ���ز���     void  
// ʹ��ʾ��     driver_gpio_loop();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void driver_gpio_loop(void)
{
    static uint8 high_freq_count = (100 / DRIVER_RESPONSE_CYCLE);
    static uint8 low_freq_count  = (500 / DRIVER_RESPONSE_CYCLE);
    
    gpio_loop_count ++;
    
    key_scanner();                                                              // ����ɨ��
      
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS)                                 // ����������� ��ִ������
    {
        key_clear_state(KEY_1);                                                 // ������±�־λ

        motor_zero_calibration();                                               // �����λ����
    } 
      
    if(battery_value.protect_flag == 1 && battery_value.battery_state == BATTERY_ERROR)     // ��ص͵�ѹ����  
    {
        if(gpio_loop_count % high_freq_count == 0)                              // 100ms��Ƶ��˸ (������Ĭ����ʱ10ms)
        {
            gpio_toggle_level(BOARD_LED);                                       
        }
    }
    else if(motor_left.encoder_state == ENCODER_ERROR || motor_right.encoder_state == ENCODER_ERROR)          
    {   
        if(gpio_loop_count % low_freq_count == 0)                                           
        {
            gpio_toggle_level(BOARD_LED);                                       // 500ms��Ƶ��˸ (������Ĭ����ʱ10ms)
        }
    }
    else if((motor_left.locked_value.protect_flag == 1) && (motor_left.motor_protect_state == PROTECT_MODE || motor_right.motor_protect_state == PROTECT_MODE))  
    {
        if(gpio_loop_count % low_freq_count == 0)                                           
        {
            gpio_toggle_level(BOARD_LED);                                       // 500ms��Ƶ��˸ (������Ĭ����ʱ10ms)
        }       
    }
    else if((motor_left.driver_mode == SENSORLESS) && (motor_left.sensorless_state != SENSORLESS_CLOSE_LOOP || motor_right.sensorless_state != SENSORLESS_CLOSE_LOOP))  
    {
        if(gpio_loop_count % low_freq_count == 0)                                           
        {
            gpio_toggle_level(BOARD_LED);                                       // 500ms��Ƶ��˸ (������Ĭ����ʱ10ms)
        }       
    }
    else     
    {
        gpio_set_level(BOARD_LED, 0);                                           // ���������� ����
    }
    
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ GPIO ���ܳ�ʼ��
// ����˵��     void
// ���ز���     void  
// ʹ��ʾ��     driver_gpio_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void driver_gpio_init(void)
{
    gpio_init(BOARD_LED, GPO, 0, GPO_PUSH_PULL);                                // ����LED�Ƴ�ʼ��
    
    key_init(10);                                                               // ���ذ�����ʼ��
}







