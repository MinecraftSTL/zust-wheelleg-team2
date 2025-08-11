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

#ifndef _driver_adc_h_
#define _driver_adc_h_

#include "zf_common_headfile.h"


#define BAT_ADC                         (ADC0_CH18_P07_2)               // ��ص�ѹ�������


#define VOLTAGE_CONVERSION_COEFFICIENT  (0.00886446)                    // ��ص�ѹת��ϵ�� 12λADC�ɼ�ֵ ֱ�ӳ��Ը�ֵ��ɵó���ص�ѹ 


typedef enum
{
    BATTERY_HEALTH,                                                     // ��ص�ѹ����    
    
    BATTERY_WARNING,                                                    // ��ص�ѹ����
    
    BATTERY_ERROR                                                       // ��ص�ѹ�쳣
}battery_state_enum;


typedef struct
{
    uint8                       protect_flag;                           // ��ص�ѹ����ʹ�ܱ�־λ               
    
    float                       protect_voltage_min;                    // ���ڵ�о��͵�ѹֵ       Ĭ��3.5 
    
    float                       protect_voltage_max;                    // ���ڵ�о��ߵ�ѹֵ       Ĭ��4.3     
    
    float                       worning_voltage;                        // ���ڵ�о������ѹֵ       Ĭ��3.9
    
    float                       voltage_rectify_coefficient;            // ��ص�ѹУ׼ϵ�� ����ʵ��Ӳ������ Ĭ��0.9959968

    float                       battery_voltage;                        // ��ص�ǰ��ѹ
    
    battery_state_enum          battery_state;                          // ��ص�ǰ״̬
    
}battery_value_struct;


extern battery_value_struct battery_value;


void driver_adc_loop(void);

void driver_adc_init(void);


#endif
