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
* �ļ�����          sensorless_control
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

#ifndef _SENSORLESS_CONTROL_H_
#define _SENSORLESS_CONTROL_H_

#include "zf_common_headfile.h"


#define MOTOR_LEFT_SENSORLESS_A_PHASE_H      TCPWM_CH48_P14_0                   // ����� A������ �������ţ����PWM��

#define MOTOR_LEFT_SENSORLESS_A_PHASE_L      P14_1                              // ����� A������ �������ţ����� or ���գ�

#define MOTOR_LEFT_SENSORLESS_B_PHASE_H      TCPWM_CH53_P18_4                   // ����� B������ �������ţ����PWM��

#define MOTOR_LEFT_SENSORLESS_B_PHASE_L      P18_5                              // ����� B������ �������ţ����� or ���գ�

#define MOTOR_LEFT_SENSORLESS_C_PHASE_H      TCPWM_CH51_P18_6                   // ����� C������ �������ţ����PWM��

#define MOTOR_LEFT_SENSORLESS_C_PHASE_L      P18_7                              // ����� C������ �������ţ����� or ���գ�


#define MOTOR_LEFT_SENSORLESS_A_TRIG         P06_0                              // ����� A�� �����źŴ�������

#define MOTOR_LEFT_SENSORLESS_B_TRIG         P06_1                              // ����� B�� �����źŴ�������

#define MOTOR_LEFT_SENSORLESS_C_TRIG         P06_2                              // ����� C�� �����źŴ�������


#define MOTOR_LEFT_SENSORLESS_DELAY_CLOCK    PCLK_TCPWM0_CLOCKS42               // ����� ��ʱ���ඨʱ�� ʱ��ͨ��

#define MOTOR_LEFT_SENSORLESS_DELAY_ISR      tcpwm_0_interrupts_42_IRQn         // ����� ��ʱ���ඨʱ�� �ж�ͨ��

#define MOTOR_LEFT_SENSORLESS_DELAY_TIMER    TCPWM0_GRP0_CNT42                  // ����� ��ʱ���ඨʱ�� ͨ��




#define MOTOR_RIGHT_SENSORLESS_A_PHASE_H     TCPWM_CH14_P00_2                   // �Ҳ��� A������ �������ţ����PWM��

#define MOTOR_RIGHT_SENSORLESS_A_PHASE_L     P00_3                              // �Ҳ��� A������ �������ţ����� or ���գ�

#define MOTOR_RIGHT_SENSORLESS_B_PHASE_H     TCPWM_CH07_P02_0                   // �Ҳ��� B������ �������ţ����PWM��

#define MOTOR_RIGHT_SENSORLESS_B_PHASE_L     P02_1                              // �Ҳ��� B������ �������ţ����� or ���գ�

#define MOTOR_RIGHT_SENSORLESS_C_PHASE_H     TCPWM_CH09_P05_0                   // �Ҳ��� C������ �������ţ����PWM��

#define MOTOR_RIGHT_SENSORLESS_C_PHASE_L     P05_1                              // �Ҳ��� C������ �������ţ����� or ���գ�

#define MOTOR_RIGHT_SENSORLESS_A_TRIG        P13_0                              // �Ҳ��� A�� �����źŴ�������

#define MOTOR_RIGHT_SENSORLESS_B_TRIG        P13_1                              // �Ҳ��� B�� �����źŴ�������

#define MOTOR_RIGHT_SENSORLESS_C_TRIG        P13_2                              // �Ҳ��� C�� �����źŴ�������

#define MOTOR_RIGHT_SENSORLESS_DELAY_CLOCK   PCLK_TCPWM0_CLOCKS41               // �Ҳ��� ��ʱ���ඨʱ�� ʱ��ͨ��

#define MOTOR_RIGHT_SENSORLESS_DELAY_ISR     tcpwm_0_interrupts_41_IRQn         // �Ҳ��� ��ʱ���ඨʱ�� �ж�ͨ��

#define MOTOR_RIGHT_SENSORLESS_DELAY_TIMER   TCPWM0_GRP0_CNT41                  // �Ҳ��� ��ʱ���ඨʱ�� ͨ��




void sensorless_motor_isr(void);

void sensorless_start_check(void);

void sensorless_trig_init(void);









#endif