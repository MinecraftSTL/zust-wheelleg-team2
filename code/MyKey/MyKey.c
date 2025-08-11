/*
 * MyKey..c
 *
 *  Created on: 2025��3��11��
 *      Author: minec
 */



#include "zf_common_debug.h"

#include "zf_device_key.h"

static uint32               scanner_period = 5;                                 // ������ɨ������
static uint32               key_press_time[KEY_NUMBER];                         // �����źų���ʱ��
static key_state_enum       key_state[KEY_NUMBER];                              // ����״̬

static const gpio_pin_enum  key_index[KEY_NUMBER] = KEY_LIST;                   // �����б�

//-------------------------------------------------------------------------------------------------------------------
// �������     ����״̬ɨ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     key_scanner();
// ��ע��Ϣ     �������������ѭ������ PIT �ж���
//-------------------------------------------------------------------------------------------------------------------
void my_key_scanner (void)
{
    uint8 i = 0;
    for(i = 0; KEY_NUMBER > i; i ++)
    {
        key_state[i] = KEY_RELEASE;
        if(KEY_RELEASE_LEVEL != gpio_get_level(key_index[i])){
            ++ key_press_time[i];
            if(key_press_time[i] == KEY_LONG_PRESS_PERIOD / scanner_period){
                key_state[i] = KEY_LONG_PRESS;
            }
        }else{
            if(key_press_time[i]){
                if(key_press_time[i] < KEY_LONG_PRESS_PERIOD / scanner_period){
                    key_state[i] = KEY_SHORT_PRESS;
                }
                key_press_time[i] = 0;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ����״̬
// ����˵��     key_n           ��������
// ���ز���     key_state_enum  ����״̬
// ʹ��ʾ��     key_get_state(KEY_1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
key_state_enum my_key_get_state (key_index_enum key_n)
{
    return key_state[key_n];
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���ָ������״̬
// ����˵��     key_n           ��������
// ���ز���     void            ��
// ʹ��ʾ��     key_clear_state(KEY_1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void my_key_clear_state (key_index_enum key_n)
{
    key_state[key_n] = KEY_RELEASE;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������а���״̬
// ����˵��     void            ��
// ���ز���     void            ��
// ʹ��ʾ��     key_clear_all_state();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void my_key_clear_all_state (void)
{
    for(uint8 i = 0; i < KEY_NUMBER; i ++){
        key_state[i] = KEY_RELEASE;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʼ��
// ����˵��     period          ����ɨ������ �Ժ���Ϊ��λ
// ���ز���     void
// ʹ��ʾ��     key_init(10);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void my_key_init (uint32 period)
{
    zf_assert(0 < period);
    uint8 loop_temp = 0;
    for(loop_temp = 0; KEY_NUMBER > loop_temp; loop_temp ++)
    {
        gpio_init(key_index[loop_temp], GPI, GPIO_HIGH, GPI_PULL_UP);
        key_state[loop_temp] = KEY_RELEASE;
    }
    scanner_period = period;
}
