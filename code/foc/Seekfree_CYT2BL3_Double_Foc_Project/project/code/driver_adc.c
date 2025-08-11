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
* �ļ�����          driver_adc
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

#include "driver_adc.h"


float battery_voltage_data = 0;

battery_value_struct battery_value;         

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ص�ѹ���
// ����˵��     void
// ���ز���     battery_state_enum  
// ʹ��ʾ��     battery_check();
// ��ע��Ϣ     ���ص�ǰ��ѹ��Ӧ�ĵ��״̬
//-------------------------------------------------------------------------------------------------------------------
battery_state_enum battery_check(float voltage_data)
{
    battery_state_enum battery_state = BATTERY_ERROR;  
    
    static uint32 error_count_num = 0;
    
    static uint32 warning_count_num = 0;
       
    for(int i = 1; i <= 3; i ++)                                               // ����ص�ѹ�Ƿ�����﮵�ص�ѹ��Χ ���Ҽ���ѹ�Ƿ����
    {
        if(voltage_data > (battery_value.protect_voltage_min * i) && voltage_data < (battery_value.protect_voltage_max * i))
        {
            if(voltage_data > (battery_value.protect_voltage_min * i) && voltage_data < (battery_value.worning_voltage * i))
            {
                battery_state = BATTERY_WARNING;                                // ��ص�ѹ������С��ֵ�����ڱ�����ֵ
            }
            else
            {
                battery_state = BATTERY_HEALTH;                                 // ��ص�ѹ����
                
                error_count_num = 0;
                
                warning_count_num = 0;
            }
            break;
        }
    }
    
    if(battery_state == BATTERY_WARNING && ++ warning_count_num < 50)
    {
        battery_state = BATTERY_HEALTH;                                         // �������50�ζ����ڱ�����ֵ����ΪӦ������
    }
    if(battery_state == BATTERY_ERROR && ++ error_count_num < 50)
    {
        battery_state = BATTERY_WARNING;                                        // �������50�ζ�������С��ֵ��ֱ�ӹرյ�Դ
    } 
    
    return battery_state;
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������ ADCѭ����⺯��
// ����˵��     void
// ���ز���     void  
// ʹ��ʾ��     driver_adc_loop();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void driver_adc_loop(void)
{       
    uint16 adc_data = 0;                                                        // ���� ��ʱ ADC �ɼ����� �洢λ��
    
    adc_data = adc_mean_filter_convert(BAT_ADC, 5);                             // 5�ξ�ֵ����
    
    battery_value.battery_voltage  = adc_data * VOLTAGE_CONVERSION_COEFFICIENT * battery_value.voltage_rectify_coefficient;     // ����ʵ�ʵ�ѹ  ADC���� * ת��ϵ�� * ����ϵ��
    
    battery_value.battery_state    = battery_check(battery_value.battery_voltage);                                              // ���ݵ�ѹ�жϵ�ǰ���״̬
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������ ADC ���ܳ�ʼ��
// ����˵��     void
// ���ز���     void  
// ʹ��ʾ��     driver_adc_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void driver_adc_init(void)
{
    adc_init(BAT_ADC, ADC_12BIT);                                               // ��ʼ�� ��ص�ѹ�ɼ��˿�
    
    battery_value.voltage_rectify_coefficient   = BATTERY_RECTIFY_COEFFICIENT;  // ���õ�ص�ѹ����ϵ��
    
    battery_value.protect_voltage_max           = BATTERY_PROTECT_VALUE_MAX;    // ���õ�ص�ѹ�����ֵ(���ڵ�о)
        
    battery_value.worning_voltage               = BATTERY_WARNING_VALUE;        // ���õ�ص�ѹ������ֵ(���ڵ�о)
    
    battery_value.protect_voltage_min           = BATTERY_PROTECT_VALUE_MIN;    // ���õ�ص�ѹ�����ֵ(���ڵ�о)
    
    battery_value.protect_flag                  = BATTERY_PROTECT;              // ���õ�ص�ѹ����ʹ��
}















