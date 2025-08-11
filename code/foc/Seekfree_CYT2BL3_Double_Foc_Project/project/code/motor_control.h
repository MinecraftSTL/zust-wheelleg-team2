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
* �ļ�����          motor_control
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

#ifndef _motor_control_h_
#define _motor_control_h_


#include "zf_common_headfile.h"


#define ENCODER_PRECISION               (32767)                 // �ű������������ֵ  32767
#define OUTPUT_DUTY_MAX                 (PWM_PRIOD_LOAD)        // ռ�ձ�������ֵ

typedef enum
{
    LEFT_MOTOR  = 0,                            // �����
    RIGHT_MOTOR = 1,                            // �Ҳ���
}motor_type_enum;

typedef enum
{
    REVERSE = -1,                               // ��ת
    FORWARD = 1,                                // ��ת
}motor_dir_enum;

typedef enum
{
    NORMAL_RUN_MODE,                            // �����������ģʽ
    PROTECT_MODE,                               // �������ģʽ(�ű��ֵ���󡢵�ص�ѹ����)
}motor_protect_enum;


typedef enum
{
    MOTOR_STOP,                                 // ���δ����
    MOTOR_RUN,                                  // �����������
}motor_run_state_enum;

typedef enum
{
    ENCODER_NORMAL,                             // �ű�������������
    ENCODER_ERROR,                              // �ű�������ֵ����
}magnetic_encoder_state_enum;

typedef enum
{
    FAST_FOC = 0,                               // ����FOC����ģʽ            ��ϴű�����ʹ��
    HALL_SIX_STEP,                              // ����������������ģʽ       ��ϻ���������ʹ��  
    SENSORLESS,                                 // �޸е��ģʽ               ��Ϲ������ʹ��
}motor_driver_mode_enum;

typedef enum
{
    SENSORLESS_IDLE = 0,                        // �޸�ģʽ ����״̬
    SENSORLESS_OPEN_LOOP_SINE,                  // �޸�ģʽ ���ҿ�������״̬
    SENSORLESS_OPEN_LOOP_BLDC,                  // �޸�ģʽ �������࿪������״̬
    SENSORLESS_CLOSE_LOOP ,                     // �޸�ģʽ �ջ�״̬
    SENSORLESS_STOP_STATE,                      // �޸�ģʽ ֹͣ״̬
    SENSORLESS_RESTART,                         // �޸�ģʽ ����״̬
}sensorless_state_enum;

typedef struct
{
    uint8   protect_flag;                       // ��ת����ʹ�ܱ�־λ        
    
    float  protect_duty_max;                   // ��ת����ռ�ձ�  0~1
    
    uint16  protect_check_time;                 // ��ת���ʱ��
}motor_locked_struct;


typedef struct
{         
//====================================================������ò���==================================================== 
    motor_type_enum                     motor_type;                 // ��ǰ�������    
  
    motor_driver_mode_enum              driver_mode;                // ��������ģʽ

    motor_locked_struct                 locked_value;               // �����ת�������ò��� 
    
    int16                               zero_location;              // ������λ��
    
    int16                               rotation_direction;         // �ű�����λ�������
    
    int16                               pole_pairs;                 // ���������
//====================================================������ò���====================================================   

    
//====================================================���ʵʱ����====================================================         
    motor_run_state_enum                run_state;                  // �������״̬
    
    float                               motor_duty;                 // ��ǰ���ռ�ձ�
 
    motor_dir_enum                      motor_set_dir;              // ��ǰ������÷���        
    motor_dir_enum                      motor_now_dir;              // ��ǰ�����ת����
    
    uint8                              forward_traction_angle;      //  ����FOCģʽ�£���ת��ǣ���Ƕ�  ��Χ 60 ~ 120 
    uint8                              reversal_traction_angle;     //  ����FOCģʽ�£���ת��ǣ���Ƕ�  ��Χ 60 ~ 120 
       
    uint8                              forward_preact_angle;        //  ����FOCģʽ�£���ת�ĳ�ǰ�Ƕ�  ��Χ 0 ~ 90 
    uint8                              reversal_preact_angle;       //  ����FOCģʽ�£���ת�ĳ�ǰ�Ƕ�  ��Χ 0 ~ 90 
    
    float                              motor_speed;                // �����ǰת��            ת�ٵ�λ��RPM   
    float                              motor_speed_filter;         // �����ǰת��(�˲�)      ת�ٵ�λ��RPM   
    
    uint32                              overload_count;             // ���ر����ƴ�
    float                              overload_conefficient;       // ����ϵ��
    
    int8                                hall_value_now;             // ��ǰ����ֵ
    int8                                hall_value_last;            // �ϴλ���ֵ
        
    int32                               menc15a_value_now;          // ��ǰ�ű������ɼ�ֵ              
    int32                               menc15a_value_offset;       // ��ǰ�ű���������ƫ��ֵ          
    int32                               menc15a_offset_integral;    // ��ǰ�ű�����ƫ��ֵ����
        
    motor_protect_enum                  motor_protect_state;        // ��ǰ�������ģʽ    
    magnetic_encoder_state_enum         encoder_state;              // ��ǰ�ű�����״̬    
  
    uint32                              lose_control_protect_count; // ʧ�ر�������ֵ    
    
    sensorless_state_enum               sensorless_state;           // �޸�ģʽ�µĵ�ǰ״̬   
    float                               sensorless_duty;           // �޸�ģʽ�µĵ���ռ�ձȲ���    
    int8                                sensorless_step;            // �޸�ģʽ����Ʋ�
    int8                                sensorless_value_now;       // ��ǰ����ֵ
    int8                                sensorless_value_last;      // �ϴι���ֵ
    
    uint32                              step_change_time_us;        // ������ʱ��¼
    uint16                              commutation_time[6];        // ���6�λ���ʱ��
    uint32                              commutation_time_sum;       // ���6�λ���ʱ���ܺ�
    uint32                              commutation_num;            // ͳ�ƻ������
    uint32                              filter_commutation_time_sum;// �˲���Ļ���ʱ��
    
    uint32                              sine_output_count;          // ����ǣ������
    uint32                              common_change_phase_count;  // �����������    
    uint32                              overtime_change_phase_count;// ���೬ʱ����
    uint32                              isr_trig_count;             // �����źŴ�������  
    uint32                              isr_trig_count_save;        // ���� �����źŴ�������ֵ
    uint32                              pit_trig_count;             // �����жϼ���    
    
    int32                               speed_record[100];          // ת�ټ�¼����
    int32                               speed_record_count;         // ת�ټ�¼�ƴ�    
    int32                               speed_change;               // ת�ٱ仯ֵ
    int32                               duty_record;                // ռ�ձȼ�¼ֵ   
    int32                               duty_record_count;          // ռ�ձȼ�¼�ƴ�
    int32                               duty_change;                // ռ�ձȱ仯ֵ    
//====================================================���ʵʱ����====================================================    
    
}motor_struct;

extern motor_struct motor_left;                 // ����������ṹ��
extern motor_struct motor_right;                // �Ҳ��������ṹ��

void motor_left_update_isr(void);               // ����������ж� 
void motor_right_update_isr(void);              // �Ҳ��������ж� 

uint8 motor_both_zero_calibration(void);         // ˫�����λ����
void  motor_zero_calibration(void);              // ��������Լ�+��λ����

void driver_cmd_forthwith(void);                // ����ָ�ʱִ�� 
void driver_cmd_loop(void);                     // ����ָ����Ӧѭ��

void motor_control_init(void);                  // ˫������Ƴ�ʼ��



#endif