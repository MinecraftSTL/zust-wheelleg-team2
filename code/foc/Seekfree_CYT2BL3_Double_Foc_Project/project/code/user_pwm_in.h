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
* �ļ�����          user_pwm_in
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT2BL3
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2025-05-12       pudding            first version
********************************************************************************************************************/

#ifndef _USER_PWM_IN_H_
#define _USER_PWM_IN_H_

#include "zf_common_headfile.h"


#define MOTOR_LEFT_PWM_IN_PIN                   (P07_1)                         // ���������ʹ�õ�GOIO

#define MOTOR_LEFT_PWM_IN_HSIOM                 (P7_1_TCPWM0_TR_ONE_CNT_IN45)   // ���������ʹ�õ�GOIO�Ĺ��ܸ������

#define MOTOR_LEFT_PWM_IN_CLOCK                 (PCLK_TCPWM0_CLOCKS15)          // ���������ʹ�õĶ�ʱ��ʱ��ͨ��

#define MOTOR_LEFT_PWM_IN_TIMER                 (TCPWM0_GRP0_CNT15)             // ���������ʹ�õĶ�ʱ��ͨ��

#define MOTOR_LEFT_PWM_IN_ISR                   (tcpwm_0_interrupts_15_IRQn)    // ���������ʹ�õĶ�ʱ���ж�ͨ��

#define MOTOR_LEFT_PWM_IN_OVERTIME_CLOCK        (PCLK_TCPWM0_CLOCKS20)          // ��������� ��ʱ ʹ�õĶ�ʱ��ʱ��ͨ��

#define MOTOR_LEFT_PWM_IN_OVERTIME_TIMER        (TCPWM0_GRP0_CNT20)             // ��������� ��ʱ ʹ�õĶ�ʱ��ͨ��

#define MOTOR_LEFT_PWM_IN_OVERTIME_ISR          (tcpwm_0_interrupts_20_IRQn)    // ��������� ��ʱ ʹ�õĶ�ʱ���ж�ͨ��


#define MOTOR_RIGHT_PWM_IN_PIN                  (P07_0)                         // �Ҳ�������ʹ�õ�GOIO

#define MOTOR_RIGHT_PWM_IN_HSIOM                (P7_0_TCPWM0_TR_ONE_CNT_IN780)  // �Ҳ�������ʹ�õ�GOIO�Ĺ��ܸ������

#define MOTOR_RIGHT_PWM_IN_CLOCK                (PCLK_TCPWM0_CLOCKS260)         // �Ҳ�������ʹ�õĶ�ʱ��ʱ��ͨ��

#define MOTOR_RIGHT_PWM_IN_TIMER                (TCPWM0_GRP1_CNT4)              // �Ҳ�������ʹ�õĶ�ʱ��ͨ��

#define MOTOR_RIGHT_PWM_IN_ISR                  (tcpwm_0_interrupts_260_IRQn)   // �Ҳ�������ʹ�õĶ�ʱ���ж�ͨ��

#define MOTOR_RIGHT_PWM_IN_OVERTIME_CLOCK       (PCLK_TCPWM0_CLOCKS19)          // �Ҳ������� ��ʱ ʹ�õĶ�ʱ��ʱ��ͨ��

#define MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER       (TCPWM0_GRP0_CNT19)             // �Ҳ������� ��ʱ ʹ�õĶ�ʱ��ͨ��

#define MOTOR_RIGHT_PWM_IN_OVERTIME_ISR         (tcpwm_0_interrupts_19_IRQn)    // �Ҳ������� ��ʱ ʹ�õĶ�ʱ���ж�ͨ��


typedef struct
{
    uint16 pwm_high_level;
    uint16 pwm_low_level;
    uint16 pwm_period_num;
    float  get_duty;
}user_in_struct;


extern user_in_struct motor_left_pwm_in_data;
extern user_in_struct motor_right_pwm_in_data;


void pwm_in_init(void);



#endif