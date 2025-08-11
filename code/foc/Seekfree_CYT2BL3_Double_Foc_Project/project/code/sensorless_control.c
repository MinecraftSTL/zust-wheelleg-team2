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

#include "sensorless_control.h"


//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�pwmͨ����ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_pwm_output_init(&motor_right);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void sensorless_pwm_output_init(motor_struct *motor_value)
{  
    /*************************���� ����޸� ������ƶ˿�*************************/ 
    if(motor_value->motor_type == LEFT_MOTOR)
    {
        pwm_init(MOTOR_LEFT_SENSORLESS_A_PHASE_H, FPWM, 0);                                         // ����ͳһ��ʼ��Ϊ PWM ��� Ƶ��Ĭ��20k 0ռ�ձ�
    
        pwm_init(MOTOR_LEFT_SENSORLESS_B_PHASE_H, FPWM, 0);
        
        pwm_init(MOTOR_LEFT_SENSORLESS_C_PHASE_H, FPWM, 0);
        
        gpio_init(MOTOR_LEFT_SENSORLESS_A_PHASE_L, GPO, 0, GPO_PUSH_PULL);                          // ����ͳһ��ʼ��Ϊ GPIO ��� Ĭ�ϵ͵�ƽ
        
        gpio_init(MOTOR_LEFT_SENSORLESS_B_PHASE_L, GPO, 0, GPO_PUSH_PULL);
        
        gpio_init(MOTOR_LEFT_SENSORLESS_C_PHASE_L, GPO, 0, GPO_PUSH_PULL);   
    }
    /*************************���� ����޸� ������ƶ˿�*************************/ 
    
    
    /*************************���� �Ҳ��޸� ������ƶ˿�*************************/ 
    if(motor_value->motor_type == RIGHT_MOTOR)
    {
        pwm_init(MOTOR_RIGHT_SENSORLESS_A_PHASE_H, FPWM, 0);                                        // ����ͳһ��ʼ��Ϊ PWM ��� Ƶ��Ĭ��20k 0ռ�ձ�
        
        pwm_init(MOTOR_RIGHT_SENSORLESS_B_PHASE_H, FPWM, 0);
        
        pwm_init(MOTOR_RIGHT_SENSORLESS_C_PHASE_H, FPWM, 0);
        
        gpio_init(MOTOR_RIGHT_SENSORLESS_A_PHASE_L, GPO, 0, GPO_PUSH_PULL);                         // ����ͳһ��ʼ��Ϊ GPIO ��� Ĭ�ϵ͵�ƽ
        
        gpio_init(MOTOR_RIGHT_SENSORLESS_B_PHASE_L, GPO, 0, GPO_PUSH_PULL);
        
        gpio_init(MOTOR_RIGHT_SENSORLESS_C_PHASE_L, GPO, 0, GPO_PUSH_PULL);   
    }
    /*************************���� �Ҳ��޸� ������ƶ˿�*************************/ 
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ ���� ����ǣ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_start_sine();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint8 sensorless_start_sine(motor_struct *motor_value)
{   
    static int32 location_time_use      = 1000;                                                        // ����ǰǿ�ƶ�λ50ms
    
    static int32 traction_time_use      = func_limit_ab((int32)SENSERLESS_OPENLOOP_SINE_TURNS * SENSERLESS_OPENLOOP_SINE_TIME * 20, SENSERLESS_OPENLOOP_SINE_TIME * 20, SENSERLESS_OPENLOOP_SINE_TIME * 600);         
    
    static float sine_duty              = func_limit_ab((float)SENSERLESS_OPENLOOP_SINE_DUTY / 10000.0f, 0.05, 0.2);
    
    uint8 sine_output_state             = 0;
    
    uint16 simulate_location            = 0;
    
    if(motor_value->motor_type == LEFT_MOTOR)                                                           // �ж��Ƿ�Ϊ�����
    {
        if(motor_value->sine_output_count == 0)                                                         // ����ִ������ǣ�� ���ö�ʱ�������ҿ��ƹ���
        {    
            fast_foc_init(&motor_left_foc_driver,  ENCODER_PRECISION, OUTPUT_DUTY_MAX, 1,  0,  1);      // ���� FOST-FOC ���ƹ���
        
            motor_left_output_init(PWM_PRIOD_LOAD, 0);                                                  // ���ö�ʱ���������  ��������ʱ�������ж�
        }
        
        if((motor_value->sine_output_count) < location_time_use)                                        // ִ��ʱ��С�ڶ�λʱ�� ��ִ�ж�λ����
        {
            fast_foc_calculate(&motor_left_foc_driver, 0, sine_duty, 0);                                // FAST-FOC �̶�ռ�ձ����0�Ƕ�λ�� ���������������ֵ  
        }
        else                                                                                            // �������ʱ�������Ҫǣ������ĵ�Ƕ�
        {
            if(motor_value->motor_duty >= 0.0f)
            {
                simulate_location = ((motor_value->sine_output_count - location_time_use) * ENCODER_PRECISION / (SENSERLESS_OPENLOOP_SINE_TIME * 20)) % ENCODER_PRECISION;
            }
            else
            {
                simulate_location = ENCODER_PRECISION - ((motor_value->sine_output_count - location_time_use) * ENCODER_PRECISION / (SENSERLESS_OPENLOOP_SINE_TIME * 20)) % ENCODER_PRECISION;
            }
            
            fast_foc_calculate(&motor_left_foc_driver, simulate_location, sine_duty, 0);                // FAST-FOC ���� �������ֵ  
        }   
        
        motor_left_duty_set(motor_left_foc_driver.ouput_duty[0], motor_left_foc_driver.ouput_duty[1], motor_left_foc_driver.ouput_duty[2]);         // �������ռ�ձ�

        if((++ motor_value->sine_output_count)> (traction_time_use + location_time_use))                // ִ��ʱ�䳬�� ��λʱ�� + ǣ��ʱ�� �˳�����ǣ��
        {
            motor_left_channel_set(0, 0, 0);                                                            // �رն�ʱ�����
            
            sensorless_pwm_output_init(motor_value);                                                    // �������ö�ʱ��
            
            motor_value->sine_output_count = 0;                                                         // �������ǣ������ֵ
            
            sine_output_state = 1;                                                                      // ǣ��������־λ��λ
        }
    }
    else
    {
        if(motor_value->sine_output_count == 0)                                                         // ����ִ������ǣ�� ���ö�ʱ�������ҿ��ƹ���
        {    
            fast_foc_init(&motor_right_foc_driver,  ENCODER_PRECISION, OUTPUT_DUTY_MAX, 1,  0,  1);     // ���� FOST-FOC ���ƹ���
        
            motor_right_output_init(PWM_PRIOD_LOAD, 0);                                                 // ���ö�ʱ���������  ��������ʱ�������ж�
        }
        
        if((motor_value->sine_output_count) < location_time_use)                                        // ִ��ʱ��С�ڶ�λʱ�� ��ִ�ж�λ����
        {
            fast_foc_calculate(&motor_right_foc_driver, 0, sine_duty, 0);                               // FAST-FOC �̶�ռ�ձ����0�Ƕ�λ�� ���������������ֵ  
        }
        else                                                                                            // �������ʱ�������Ҫǣ������ĵ�Ƕ�
        {
            if(motor_value->motor_duty >= 0.0f)
            {
                simulate_location = ((motor_value->sine_output_count - location_time_use) * ENCODER_PRECISION / (SENSERLESS_OPENLOOP_SINE_TIME * 20)) % ENCODER_PRECISION;
            }
            else
            {
                simulate_location = ENCODER_PRECISION - ((motor_value->sine_output_count - location_time_use) * ENCODER_PRECISION / (SENSERLESS_OPENLOOP_SINE_TIME * 20)) % ENCODER_PRECISION;
            }
            
            fast_foc_calculate(&motor_right_foc_driver, simulate_location, sine_duty, 0);               // FAST-FOC ���� �������ֵ  
        }   
        
        motor_right_duty_set(motor_right_foc_driver.ouput_duty[0], motor_right_foc_driver.ouput_duty[1], motor_right_foc_driver.ouput_duty[2]);         // �������ռ�ձ�

        if((++ motor_value->sine_output_count) > (traction_time_use + location_time_use))               // ִ��ʱ�䳬�� ��λʱ�� + ǣ��ʱ�� �˳�����ǣ��
        {
            motor_right_channel_set(0, 0, 0);                                                           // �رն�ʱ�����
            
            sensorless_pwm_output_init(motor_value);                                                    // �������ö�ʱ��
            
            motor_value->sine_output_count = 0;                                                         // �������ǣ������ֵ
            
            sine_output_state = 1;                                                                      // ǣ��������־λ��λ
        }
    }
    return sine_output_state;                                                                           // ����ǣ��״̬
}


//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ �����źŴ��� ʹ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_trig_disable(&motor_left);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void sensorless_motor_trig_enable(motor_struct *motor_value)
{
    if(motor_value->motor_type == LEFT_MOTOR)                                   // �ж��Ƿ�Ϊ�����
    {
        exti_enable(MOTOR_LEFT_SENSORLESS_A_TRIG);                              // ���� A������źŴ��� �ж�

        exti_enable(MOTOR_LEFT_SENSORLESS_B_TRIG);                              // ���� B������źŴ��� �ж�

        exti_enable(MOTOR_LEFT_SENSORLESS_C_TRIG);                              // ���� C������źŴ��� �ж�
      
        get_port(MOTOR_LEFT_SENSORLESS_A_TRIG)->unINTR.stcField.u1IN_IN0 = 0;   // ����ж��źţ�����CYTоƬBUG���˴�����ʵ�������ö˿������ⲿ�ж��źţ�����ֲ������оƬ����ɾ����䣩 
    }
    else
    {
        exti_enable(MOTOR_RIGHT_SENSORLESS_A_TRIG);                             // ���� A������źŴ��� �ж�

        exti_enable(MOTOR_RIGHT_SENSORLESS_B_TRIG);                             // ���� B������źŴ��� �ж�

        exti_enable(MOTOR_RIGHT_SENSORLESS_C_TRIG);                             // ���� C������źŴ��� �ж�
      
        get_port(MOTOR_RIGHT_SENSORLESS_A_TRIG)->unINTR.stcField.u1IN_IN0 = 0;  // ����ж��źţ�����CYTоƬBUG���˴�����ʵ�������ö˿������ⲿ�ж��źţ�����ֲ������оƬ����ɾ����䣩 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ �����źŴ��� ʧ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_trig_disable(&motor_left);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void sensorless_motor_trig_disable(motor_struct *motor_value)
{
    if(motor_value->motor_type == LEFT_MOTOR)                                   // �ж��Ƿ�Ϊ�����
    {
        exti_disable(MOTOR_LEFT_SENSORLESS_A_TRIG);                             // �ر� A������źŴ��� �ж�
                                                                                
        exti_disable(MOTOR_LEFT_SENSORLESS_B_TRIG);                             // �ر� B������źŴ��� �ж�
                                                                                
        exti_disable(MOTOR_LEFT_SENSORLESS_C_TRIG);                             // �ر� C������źŴ��� �ж�
        
        get_port(MOTOR_LEFT_SENSORLESS_A_TRIG)->unINTR.stcField.u1IN_IN0 = 0;   // ����ж��źţ�����CYTоƬBUG���˴�����ʵ�������ö˿������ⲿ�ж��źţ�����ֲ������оƬ����ɾ����䣩
    }
    else
    {
        exti_disable(MOTOR_RIGHT_SENSORLESS_A_TRIG);                            // �ر� A������źŴ��� �ж�
                                                                                
        exti_disable(MOTOR_RIGHT_SENSORLESS_B_TRIG);                            // �ر� B������źŴ��� �ж�
                                                                                
        exti_disable(MOTOR_RIGHT_SENSORLESS_C_TRIG);                            // �ر� C������źŴ��� �ж�
        
        get_port(MOTOR_RIGHT_SENSORLESS_A_TRIG)->unINTR.stcField.u1IN_IN0 = 0;  // ����ж��źţ�����CYTоƬBUG���˴�����ʵ�������ö˿������ⲿ�ж��źţ�����ֲ������оƬ����ɾ����䣩
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������ಽ������
// ����˵��     motor_value         ����ṹ��
// ���ز���     void
// ʹ��ʾ��     motor_next_step(&motor_left);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_next_step(motor_struct *motor_value)
{
    if(motor_value->motor_duty >= 0)
    {
        motor_value->sensorless_step ++;                                        // �����������
    
        if(6 < motor_value->sensorless_step)                                    // ������������
        {
            motor_value->sensorless_step = 1;                                   // ѭ���ص���1��
        }
    }
    else
    {
        motor_value->sensorless_step --;                                        // �����������
    
        if(1 > motor_value->sensorless_step)                                    // ������������
        {
            motor_value->sensorless_step = 6;                                   // ѭ���ص���6��
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ�����ռ�ձ��������
// ����˵��     phase_a     A��ռ�ձ����   0���ر����� ��������   1~10000���������ռ�ձ� �ر����ţ�   -1���ر�������
// ����˵��     phase_b     B��ռ�ձ����   0���ر����� ��������   1~10000���������ռ�ձ� �ر����ţ�   -1���ر�������
// ����˵��     phase_c     C��ռ�ձ����   0���ر����� ��������   1~10000���������ռ�ձ� �ر����ţ�   -1���ر�������
// ���ز���     void
// ʹ��ʾ��     motor_left_sensorless_pwm_output(ouput_duty, 0, -1);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_left_sensorless_pwm_output(int16 phase_a, int16 phase_b, int16 phase_c)
{      
    phase_a = func_limit_ab(phase_a, -1, 10000);                                                // A������޷�
                                                                                                
    phase_b = func_limit_ab(phase_b, -1, 10000);                                                // B������޷�
                                                                                                
    phase_c = func_limit_ab(phase_c, -1, 10000);                                                // C������޷�
  
    gpio_low(MOTOR_LEFT_SENSORLESS_A_PHASE_L);                                                  // �ر�A������    
    
    gpio_low(MOTOR_LEFT_SENSORLESS_B_PHASE_L);                                                  // �ر�B������        
    
    gpio_low(MOTOR_LEFT_SENSORLESS_C_PHASE_L);                                                  // �ر�C������        

    pwm_set_duty(MOTOR_LEFT_SENSORLESS_A_PHASE_H, (uint16)(phase_a > 0 ? phase_a : 0));         // ����A������ռ�ձ�    
      
    pwm_set_duty(MOTOR_LEFT_SENSORLESS_B_PHASE_H, (uint16)(phase_b > 0 ? phase_b : 0));         // ����B������ռ�ձ� 
    
    pwm_set_duty(MOTOR_LEFT_SENSORLESS_C_PHASE_H, (uint16)(phase_c > 0 ? phase_c : 0));         // ����C������ռ�ձ� 
    
    if(phase_a == 0)                                                                            // �ж��Ƿ���Ҫ����A������
    {   
        gpio_high(MOTOR_LEFT_SENSORLESS_A_PHASE_L);                                             // ����A������
    }
    else if(phase_b == 0)                                                                       // �ж��Ƿ���Ҫ����B������
    {                                                                                            
        gpio_high(MOTOR_LEFT_SENSORLESS_B_PHASE_L);                                              // ����B������
    }
    else if(phase_c == 0)                                                                       // �ж��Ƿ���Ҫ����C������
    {                                                                                            
        gpio_high(MOTOR_LEFT_SENSORLESS_C_PHASE_L);                                              // ����C������
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ�Ҳ���ռ�ձ��������
// ����˵��     phase_a     A��ռ�ձ����   0���ر����� ��������   1~10000���������ռ�ձ� �ر����ţ�   -1���ر�������
// ����˵��     phase_b     B��ռ�ձ����   0���ر����� ��������   1~10000���������ռ�ձ� �ر����ţ�   -1���ر�������
// ����˵��     phase_c     C��ռ�ձ����   0���ر����� ��������   1~10000���������ռ�ձ� �ر����ţ�   -1���ر�������
// ���ز���     void
// ʹ��ʾ��     motor_right_sensorless_pwm_output(ouput_duty, 0, -1);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_right_sensorless_pwm_output(int16 phase_a, int16 phase_b, int16 phase_c)
{      
    phase_a = func_limit_ab(phase_a, -1, 10000);                                                // A������޷�
                                                                                                
    phase_b = func_limit_ab(phase_b, -1, 10000);                                                // B������޷�
                                                                                                
    phase_c = func_limit_ab(phase_c, -1, 10000);                                                // C������޷�
  
    gpio_low(MOTOR_RIGHT_SENSORLESS_A_PHASE_L);                                                 // �ر�A������    
    
    gpio_low(MOTOR_RIGHT_SENSORLESS_B_PHASE_L);                                                 // �ر�B������        
    
    gpio_low(MOTOR_RIGHT_SENSORLESS_C_PHASE_L);                                                 // �ر�C������        

    pwm_set_duty(MOTOR_RIGHT_SENSORLESS_A_PHASE_H, (uint16)(phase_a > 0 ? phase_a : 0));        // ����A������ռ�ձ�    
      
    pwm_set_duty(MOTOR_RIGHT_SENSORLESS_B_PHASE_H, (uint16)(phase_b > 0 ? phase_b : 0));        // ����B������ռ�ձ� 
    
    pwm_set_duty(MOTOR_RIGHT_SENSORLESS_C_PHASE_H, (uint16)(phase_c > 0 ? phase_c : 0));        // ����C������ռ�ձ� 
    
    if(phase_a == 0)                                                                            // �ж��Ƿ���Ҫ����A������
    {   
        gpio_high(MOTOR_RIGHT_SENSORLESS_A_PHASE_L);                                            // ����A������
    }
    else if(phase_b == 0)                                                                       // �ж��Ƿ���Ҫ����B������
    {                                                                                            
        gpio_high(MOTOR_RIGHT_SENSORLESS_B_PHASE_L);                                            // ����B������
    }
    else if(phase_c == 0)                                                                       // �ж��Ƿ���Ҫ����C������
    {                                                                                            
        gpio_high(MOTOR_RIGHT_SENSORLESS_C_PHASE_L);                                            // ����C������
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������ຯ��
// ����˵��     motor_value         ����ṹ��
// ����˵��     ouput_duty          ��Ҫ�����ռ�ձ�����
// ����˵��     trig_state          �Ƿ�����Ӧ�Ĺ����źŴ����ж�  0 ������  1 ����
// ���ز���     void
// ʹ��ʾ��     motor_commutation(&motor_left, (int16)motor_left.sensorless_duty, 1);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_commutation(motor_struct *motor_value, int16 ouput_duty, uint8 trig_state)
{
    if(motor_value->motor_type == LEFT_MOTOR)                                   // �ж��Ƿ�Ϊ�����
    {
        switch(motor_value->sensorless_step)                                    // �жϵ�ǰ��Ҫ�������λ
        {
            case 1:
            {
                motor_left_sensorless_pwm_output(ouput_duty, 0, -1);            // A ����  ��  B ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {
                    exti_enable(MOTOR_LEFT_SENSORLESS_C_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж�
                }
                
            }break;
            
            case 2:
            {
                motor_left_sensorless_pwm_output(ouput_duty, -1, 0);            // A ����  ��  C ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_LEFT_SENSORLESS_B_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 3:
            {
                motor_left_sensorless_pwm_output(-1, ouput_duty, 0);            // B ����  ��  C ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_LEFT_SENSORLESS_A_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 4:
            {
                motor_left_sensorless_pwm_output(0, ouput_duty, -1);            // B ����  ��  A ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_LEFT_SENSORLESS_C_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 5:
            {
                motor_left_sensorless_pwm_output(0, -1, ouput_duty);            // C ����  ��  A ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_LEFT_SENSORLESS_B_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 6:
            {
                motor_left_sensorless_pwm_output(-1, 0, ouput_duty);            // C ����  ��  B ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_LEFT_SENSORLESS_A_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            default:break;
        }
    }
    else
    {
        switch(motor_value->sensorless_step)                                    // �жϵ�ǰ��Ҫ�������λ
        {
            case 1:
            {
                motor_right_sensorless_pwm_output(ouput_duty, 0, -1);            // A ����  ��  B ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {
                    exti_enable(MOTOR_RIGHT_SENSORLESS_C_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж�
                }
                
            }break;
            
            case 2:
            {
                motor_right_sensorless_pwm_output(ouput_duty, -1, 0);            // A ����  ��  C ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_RIGHT_SENSORLESS_B_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 3:
            {
                motor_right_sensorless_pwm_output(-1, ouput_duty, 0);            // B ����  ��  C ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_RIGHT_SENSORLESS_A_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 4:
            {
                motor_right_sensorless_pwm_output(0, ouput_duty, -1);            // B ����  ��  A ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_RIGHT_SENSORLESS_C_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 5:
            {
                motor_right_sensorless_pwm_output(0, -1, ouput_duty);            // C ����  ��  A ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_RIGHT_SENSORLESS_B_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            case 6:
            {
                motor_right_sensorless_pwm_output(-1, 0, ouput_duty);            // C ����  ��  B ����
                
                if(trig_state)                                                  // �ж��Ƿ�����Ӧ�� �����źŴ����ж�
                {                                                                 
                    exti_enable(MOTOR_RIGHT_SENSORLESS_A_TRIG);                  // ʹ�ܶ�Ӧ��λ�� �����źŴ����ж� 
                }

            }break;
            
            default:break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ�������ʱ��������
// ����˵��     delay_time          ��ʱ����ʱ��
// ���ز���     void
// ʹ��ʾ��     motor_left_sensorless_delay_output(20);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_left_sensorless_delay_output(uint16 delay_time)
{    
    Cy_Tcpwm_Counter_SetPeriod(MOTOR_LEFT_SENSORLESS_DELAY_TIMER, delay_time);  // ���ö�ʱ������Ϊ��ʱʱ�� 

    Cy_Tcpwm_Counter_SetCounter(MOTOR_LEFT_SENSORLESS_DELAY_TIMER, 0);          // �����ǰ��ʱ������ֵ

    Cy_Tcpwm_Counter_Enable(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);                 // ʹ����ʱ���ඨʱ��
    
    Cy_Tcpwm_TriggerStart(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);                   // ������ʱ����������
}           


//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ��������������ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_left_trig_isr();
// ��ע��Ϣ     �����źŴ����жϣ����ڻ���
//-------------------------------------------------------------------------------------------------------------------
void motor_left_sensorless_delay_isr(void)
{    
    Cy_Tcpwm_Counter_ClearTC_Intr(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);           // ����ӳٻ����жϱ�־λ
    
    Cy_Tcpwm_Counter_Disable(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);                // ʧ����ʱ���ඨʱ��
    
    motor_next_step(&motor_left);                                               // �����λ����
        
    motor_commutation(&motor_left, (int16)(func_abs(motor_left.sensorless_duty) * 10000), 1);            // �����ǰ��λ �򿪶�Ӧ�Ĺ����źż���ж�
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ��������������ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_left_trig_isr();
// ��ע��Ϣ     �����źŴ����жϣ����ڻ���
//-------------------------------------------------------------------------------------------------------------------
void sensorless_motor_left_trig_isr(void)
{       
    motor_left.isr_trig_count ++;                                               // �����ź� ������������
    
    exti_flag_get(MOTOR_LEFT_SENSORLESS_A_TRIG);                                // ��� A�� �����ź�
    
    exti_flag_get(MOTOR_LEFT_SENSORLESS_B_TRIG);                                // ��� B�� �����ź�
    
    exti_flag_get(MOTOR_LEFT_SENSORLESS_C_TRIG);                                // ��� C�� �����ź�
    
    if(motor_left.sensorless_state == SENSORLESS_CLOSE_LOOP)                    // �жϵ�ǰ�Ƿ��ڱջ�ģʽ  �����ڱջ�ģʽ��ִ���κ� ���Ƴ���
    {        
        motor_left.step_change_time_us = timer_get(LEFT_MOTOR_COUNT_TIMER);     // ��ȡ���λ���ʱ�� ��λ us
        
        timer_clear(LEFT_MOTOR_COUNT_TIMER);                                    // �������ֵ
        
        motor_left.commutation_time_sum -= motor_left.commutation_time[motor_left.sensorless_step - 1];                                         // �Ƴ� �����¼�� ����ʱ��

        motor_left.commutation_time[motor_left.sensorless_step - 1] = (motor_left.step_change_time_us > 6000 ? 6000 : motor_left.step_change_time_us);                // ��¼���λ���ʱ�� �޷� 6000us

        motor_left.commutation_time_sum += motor_left.commutation_time[motor_left.sensorless_step - 1];                                         // ���ӱ��λ���ʱ�� �ܹ�����¼ ���6�εĻ����ʱ(��Ӧ360���Ƕ�)

        motor_left.filter_commutation_time_sum = (motor_left.filter_commutation_time_sum * 3 + motor_left.commutation_time_sum * 1) / 4;        // ����ʱ�����е�ͨ�˲�
               
        sensorless_motor_trig_disable(&motor_left);                             // ʧ�� �����ź� �����жϣ�������������д�ָ����λ�Ĵ����жϣ�
        
        // ����Ϊ��ʱ���ಿ�֣�����Ĵ��ز�ͬ����ʱ����������ܲ�ͬ

        // ������ʱ����ʱ���ʵ���޸ĵ������Ч��
        
        if(motor_left.isr_trig_count < 100)                                     // ����ǰ �����ź� ��������ֵС�� 100������Ϊ�ս���ջ� ִ��Ĭ����ʱ����
        {
            motor_left_sensorless_delay_output(20);                             // ��ʱ 20us ��ִ�л���
        }
        else if(motor_left.sensorless_duty < 0.4f)                             // ���û�����ռ�ձȵ��� 40%����ʱ������� ����Ƕ� �� 132��
        {
            motor_left_sensorless_delay_output(motor_left.filter_commutation_time_sum * 3 / 60);                                                // ִ����ʱ����
        }
        else                                                                    // ���û�����ռ�ձȸ��� 40%����ʱ������� ����Ƕ� �𽥴� 132������� 144�� ��Ӧ 40% ~ 100% ռ�ձ�
        {            
            float wait_time = 30.0f - (motor_left.sensorless_duty - 0.4f) * 25.0f / 0.6f;                                                       // ���� ��ʱ����ʱ��
            
            motor_left_sensorless_delay_output((uint32)((float)motor_left.filter_commutation_time_sum * wait_time / 600.0f));                   // ִ����ʱ����
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ�Ҳ�����ʱ��������
// ����˵��     delay_time          ��ʱ����ʱ��
// ���ز���     void
// ʹ��ʾ��     motor_right_sensorless_delay_output(20);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_right_sensorless_delay_output(uint16 delay_time)
{    
    Cy_Tcpwm_Counter_SetPeriod(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER, delay_time);  // ���ö�ʱ������Ϊ��ʱʱ�� 

    Cy_Tcpwm_Counter_SetCounter(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER, 0);          // �����ǰ��ʱ������ֵ

    Cy_Tcpwm_Counter_Enable(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);                 // ʹ����ʱ���ඨʱ��
    
    Cy_Tcpwm_TriggerStart(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);                   // ������ʱ����������
}  

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ�Ҳ������������ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_right_sensorless_delay_isr();
// ��ע��Ϣ     �����źŴ����жϣ����ڻ���
//-------------------------------------------------------------------------------------------------------------------
void motor_right_sensorless_delay_isr(void)
{    
    Cy_Tcpwm_Counter_ClearTC_Intr(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);          // ����ӳٻ����жϱ�־λ
    
    Cy_Tcpwm_Counter_Disable(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);               // ʧ����ʱ���ඨʱ��
    
    motor_next_step(&motor_right);                                              // �����λ����
        
    motor_commutation(&motor_right, (int16)(func_abs(motor_right.sensorless_duty) * 10000), 1);          // �����ǰ��λ �򿪶�Ӧ�Ĺ����źż���ж�
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ�Ҳ������������ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_right_trig_isr();
// ��ע��Ϣ     �����źŴ����жϣ����ڻ���
//-------------------------------------------------------------------------------------------------------------------
void sensorless_motor_right_trig_isr(void)
{
    motor_right.isr_trig_count ++;                                              // �����ź� ������������
    
    exti_flag_get(MOTOR_RIGHT_SENSORLESS_A_TRIG);                               // ��� A�� �����ź�
    
    exti_flag_get(MOTOR_RIGHT_SENSORLESS_B_TRIG);                               // ��� B�� �����ź�
    
    exti_flag_get(MOTOR_RIGHT_SENSORLESS_C_TRIG);                               // ��� C�� �����ź�
    
    if(motor_right.sensorless_state == SENSORLESS_CLOSE_LOOP)                   // �жϵ�ǰ�Ƿ��ڱջ�ģʽ  �����ڱջ�ģʽ��ִ���κ� ���Ƴ���
    {        
        motor_right.step_change_time_us = timer_get(RIGHT_MOTOR_COUNT_TIMER);   // ��ȡ���λ���ʱ�� ��λ us
        
        timer_clear(RIGHT_MOTOR_COUNT_TIMER);                                   // �������ֵ
        
        motor_right.commutation_time_sum -= motor_right.commutation_time[motor_right.sensorless_step - 1];                                         // �Ƴ� �����¼�� ����ʱ��

        motor_right.commutation_time[motor_right.sensorless_step - 1] = (motor_right.step_change_time_us > 6000 ? 6000 : motor_right.step_change_time_us);                // ��¼���λ���ʱ�� �޷� 6000us

        motor_right.commutation_time_sum += motor_right.commutation_time[motor_right.sensorless_step - 1];                                         // ���ӱ��λ���ʱ�� �ܹ�����¼ ���6�εĻ����ʱ(��Ӧ360���Ƕ�)

        motor_right.filter_commutation_time_sum = (motor_right.filter_commutation_time_sum * 3 + motor_right.commutation_time_sum) / 4;            // ����ʱ�����е�ͨ�˲�
        
        sensorless_motor_trig_disable(&motor_right);                            // ʧ�� �����ź� �����жϣ�������������д�ָ����λ�Ĵ����жϣ�
        
        // ����Ϊ��ʱ���ಿ�֣�����Ĵ��ز�ͬ����ʱ����������ܲ�ͬ

        // ������ʱ����ʱ���ʵ���޸ĵ������Ч��
        if(motor_right.isr_trig_count < 100)                                    // ����ǰ �����ź� ��������ֵС�� 100������Ϊ�ս���ջ� ִ��Ĭ����ʱ����
        {
            motor_right_sensorless_delay_output(20);                            // ��ʱ 20us ��ִ�л���
        }
        else if(motor_right.sensorless_duty < 0.4f)                             // ���û�����ռ�ձȵ��� 40%����ʱ������� ����Ƕ� �� 132��
        {
            motor_right_sensorless_delay_output(motor_right.filter_commutation_time_sum * 3 / 60);                                                // ִ����ʱ����
        }
        else                                                                    // ���û�����ռ�ձȸ��� 40%����ʱ������� ����Ƕ� �𽥴� 132������� 144�� ��Ӧ 40% ~ 100% ռ�ձ�
        {            
            float wait_time = 30.0f - (motor_right.sensorless_duty - 0.4f) * 25.0f / 0.6f;                                                      // ���� ��ʱ����ʱ��
            
            motor_right_sensorless_delay_output((uint32)((float)motor_right.filter_commutation_time_sum * wait_time / 600.0f));                 // ִ����ʱ����
        }
    }
  
  
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ �����ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_isr();
// ��ע��Ϣ     ����ʱ�� 50us�����в��ֲ�����������ʱ�䣬�����޸ĸ��ж�ִ������
//-------------------------------------------------------------------------------------------------------------------
void sensorless_motor_isr(void)
{         
    /*************************����޸�����ģʽ ����ѭ��ִ������*************************/ 
    motor_left.pit_trig_count ++;                                               // ����ֵ���� 
    
#if USER_CONTROL_MODE == 1                                                      // pwm�������ģʽ
    if(motor_left_pwm_in_data.pwm_high_level >= 1050 && motor_left_pwm_in_data.pwm_high_level < 2020)           // �жϿ�������ĸߵ�ƽ�Ƿ���� 1.05ms ����С�� 2.02ms
    {
        motor_left.motor_duty = func_limit_ab((motor_left_pwm_in_data.pwm_high_level - 1000) / 1000.0f, 0, 1);  // ����Ŀ��ռ�ձ�

        motor_left.motor_duty  = motor_left.motor_duty * (SENSERLESS_MOTOR_LEFT_DIR == 0 ? 1 : -1); 
    }
    else
    {
        motor_left.motor_duty = 0;                                                                     
    }
#endif    
    
    switch(motor_left.sensorless_state)                                         // �жϵ�ǰ �޸� ����״̬
    {                                                                           
        case SENSORLESS_OPEN_LOOP_SINE:                                         // ���� ����ǣ��״̬ 
        {                                                                       
            if(motor_left.pit_trig_count <= 500)                                // ������ǰ25ms ������Ƿ�����ת �������ת��ֱ�ӽ��뿪��BLDCǣ�� ���迪������ǣ��
            {
                motor_left.sensorless_value_last = motor_left.sensorless_value_now;                                   // �����ϴ� �����ź� ��λֵ
            
                motor_left.sensorless_value_now =  gpio_get_level(MOTOR_LEFT_SENSORLESS_A_TRIG) * 4 +                 // ���㱾�� �����ź� ��λֵ  
                                                   gpio_get_level(MOTOR_LEFT_SENSORLESS_B_TRIG) * 2 +
                                                   gpio_get_level(MOTOR_LEFT_SENSORLESS_C_TRIG);    
                
                if(motor_left.sensorless_value_last != motor_left.sensorless_value_now)                               // ��⵽���㻻��
                {
                    motor_left.common_change_phase_count ++;                    // �������ֵ����
                }                                                                
                                                                                 
                if(motor_left.pit_trig_count == 500)                            // 25ms�� �жϻ������
                {                                                                
                    if(motor_left.common_change_phase_count > 50)               // �����೬��50��(���ת�ٳ��� 2858 RPM)  ��ֱ�ӽ��뿪��BLDCǣ��
                    {                                                            
                        sensorless_pwm_output_init(&motor_left);                // �������ö�ʱ��
                                                                                 
                        motor_left.overtime_change_phase_count = 0;             // ������೬ʱ����ֵ
                                                                                 
                        motor_left.common_change_phase_count = 0;               // ��������������ֵ
                                                                                 
                        timer_start(LEFT_MOTOR_COUNT_TIMER);                    // ���� ΢����� ��ʱ��
                        
                        motor_left.sensorless_state = SENSORLESS_OPEN_LOOP_BLDC;// ����״̬ ����Ϊ ���� BLDC ǣ��״̬
                    }
                }
            }
            else                                                                // ��������ִ������ǣ��
            {
                if(sensorless_start_sine(&motor_left))                          // ִ������ǣ������ �ж����Ƿ�ǣ�����
                {
                    motor_left.overtime_change_phase_count = 0;                 // ������೬ʱ����ֵ
                    
                    motor_left.common_change_phase_count = 0;                   // ��������������ֵ
                    
                    timer_start(LEFT_MOTOR_COUNT_TIMER);                        // ���� ΢����� ��ʱ��
                        
                    motor_left.sensorless_state = SENSORLESS_OPEN_LOOP_BLDC;           // ����״̬ ����Ϊ ���� BLDC ǣ��״̬
                }
            }
        }break;
        
        case SENSORLESS_OPEN_LOOP_BLDC:                                         // ���� BLDC ǣ��״̬
        {         
            motor_left.sensorless_value_last = motor_left.sensorless_value_now; // �����ϴ� �����ź� ��λֵ
            
            motor_left.sensorless_value_now = gpio_get_level(MOTOR_LEFT_SENSORLESS_A_TRIG) * 4 +                                // ���㱾�� �����ź� ��λֵ  
                                              gpio_get_level(MOTOR_LEFT_SENSORLESS_B_TRIG) * 2 +
                                              gpio_get_level(MOTOR_LEFT_SENSORLESS_C_TRIG);         
            
            motor_left.step_change_time_us = timer_get(LEFT_MOTOR_COUNT_TIMER); // ��ȡ��ǰ ��ʱ�� ����ʱ�� 
            
            if(motor_left.sensorless_value_last != motor_left.sensorless_value_now || motor_left.step_change_time_us > 5000)    // ���ֻ��� ���� ����ʱ�䳬��5000΢�� �л���� 
            {         
                timer_clear(LEFT_MOTOR_COUNT_TIMER);                            // �����ʱ������ֵ
              
                if(motor_left.step_change_time_us > 5000 || motor_left.step_change_time_us < 500)                               // ������ 5000 ΢�� ���� ���� 500΢�� ����Ϊ���೬ʱ(����)  
                {
                    motor_left.overtime_change_phase_count ++;                  // ���೬ʱ ����ֵ����
                }
                else                                                            // ����Ϊ��������
                {       
                    motor_left.common_change_phase_count ++;                    // �������� ����ֵ����
                }
                
                motor_next_step(&motor_left);                                   // �л���һ�� 
                
                motor_commutation(&motor_left, SENSERLESS_OPENLOOP_BLDC_DUTY, 0);// ���յ�ǰ��λ��� ����������������ж�
                
                if(motor_left.overtime_change_phase_count > ((SENSERLESS_OPENLOOP_BLDC_TURNS - SENSERLESS_CLOSELOOP_CHECK) * 6))// �����೬ʱ����ֵ ���� 250�� ��Ϊǣ������ʧ�� �������ο���ǣ�� (��Ϊ�������� ȷ���ܹ���ȷ����ǣ��)
                {
                    motor_left.overtime_change_phase_count = 0;                 // ������೬ʱ����ֵ
                    
                    motor_left.pit_trig_count = 0;                              // �����ǰ���ڶ�ʱ������ֵ
                    
                    motor_commutation(&motor_left, 0, 0);                       // �ر����
                    
                    motor_left.sensorless_state = SENSORLESS_RESTART;           // ����״̬ ����Ϊ ����״̬
                }
                  
                if(motor_left.common_change_phase_count > (SENSERLESS_CLOSELOOP_CHECK * 6))                                     // ����������������� 50 �� ����Ϊ���Խ���ջ�ģʽ �������ο���ǣ�� 
                {                    
                    if(motor_left.motor_duty >= 0)
                    {
                        motor_left.sensorless_duty =  (SENSERLESS_OPENLOOP_BLDC_DUTY / 10000.0f); // ���ռ�ձȸ���ֵ ��10%ռ�ձ�����
                    }
                    else
                    {
                        motor_left.sensorless_duty = -(SENSERLESS_OPENLOOP_BLDC_DUTY / 10000.0f); // ���ռ�ձȸ���ֵ ��10%ռ�ձ�����
                    }
                    motor_left.pit_trig_count = 0;                              // �����ǰ���ڶ�ʱ������ֵ
                    
                    motor_left.isr_trig_count = 0;                              // �����ǰ�����źŴ�������ֵ
                    
                    sensorless_motor_trig_enable(&motor_left);                  // ʹ�� �����ź� �����ж�        
                    
                    timer_clear(LEFT_MOTOR_COUNT_TIMER);                        // �����ʱ������ֵ
                    
                    motor_left.sensorless_state = SENSORLESS_CLOSE_LOOP;        // ����״̬ ����Ϊ �ջ�����״̬
                }
            }
        }break;
        
        case SENSORLESS_CLOSE_LOOP:                                             // �ջ�����״̬
        {      
            if(motor_left.isr_trig_count > 500)
            {
                motor_left.motor_speed = 60000000 / (motor_left.filter_commutation_time_sum * 7);                               // ���㵱ǰ���ת��

                if(motor_left.motor_duty < 0.0f)
                {
                    motor_left.motor_speed = -motor_left.motor_speed;
                }
                
                motor_left.motor_speed_filter = (motor_left.motor_speed + motor_left.motor_speed_filter * 99.0f) / 100.0f;      // ��ת�ٽ��е�ͨ�˲�
            }
            
            if(motor_left.pit_trig_count > 1000 && motor_left.pit_trig_count % SENSERLESS_SPEED_INCREMENTAL == 0)               // ������ & ������ ����ջ� 50ms �� �����û����õļӼ��ٲ��� ִ�мӼ���
            {
                if(motor_left.sensorless_duty < motor_left.motor_duty)
                {
                    motor_left.sensorless_duty += (func_limit_ab(func_abs(motor_left.sensorless_duty) * 0.0003, 0.00005, 0.0003));  
                }
                else if(motor_left.sensorless_duty > motor_left.motor_duty)
                {
                    motor_left.sensorless_duty -= (func_limit_ab(func_abs(motor_left.sensorless_duty) * 0.0003, 0.00005, 0.0003));  
                }
                
                motor_left.sensorless_duty = func_limit_ab(motor_left.sensorless_duty, -1.0f, 1.0f);
            }
            
            if(motor_left.pit_trig_count % 20 == 0)                             // 1ms ִ��һ�� ��¼�ջ��ٶ����� ���ڸ��ٿ����ű���
            {
                motor_left.speed_record[motor_left.speed_record_count] = (int32)func_abs(motor_left.motor_speed_filter);// ���浱ǰ�ٶ�
                
                motor_left.speed_change   = motor_left.speed_record[motor_left.speed_record_count] -                    // ���� 100ms �ڵ��ٶ� �仯
                                            motor_left.speed_record[(motor_left.speed_record_count == 99 ? 0 : motor_left.speed_record_count + 1)];
                
                if(++ motor_left.speed_record_count >= 100)                     // �ٶȱ������� ѭ������
                {
                    motor_left.speed_record_count = 0;
                }     
                
                motor_left.duty_change = (int32)(motor_left.motor_duty * 10000) - motor_left.duty_record; // ����ռ�ձ� �仯
                
                motor_left.duty_record = (int32)(motor_left.motor_duty * 10000);// ���浱ǰռ�ձ�����
                
                if(func_abs(motor_left.duty_change) >= 100)
                {
                    motor_left.duty_record_count = 1;
                }
                else
                {
                    if(motor_left.duty_record_count > 0 && motor_left.duty_record_count < 1000)
                    {
                        motor_left.duty_change = 100;
                        
                        if(++ motor_left.duty_record_count == 1000)
                        {
                            motor_left.duty_record_count = 0;
                            
                            motor_left.duty_change = 0;
                        }
                    }
                }
            }
            
            if(motor_left.isr_trig_count_save == motor_left.isr_trig_count)     // �� ���㻻���ж� 50΢����δ���� ���¼
            {
                motor_left.overtime_change_phase_count ++;                      // ���೬ʱ ����ֵ����
            }
            else                                                                // �� 50΢���ڴ��� ���㻻���ж� �������¼
            {
                motor_left.overtime_change_phase_count = 0;                     // ���೬ʱ ����ֵ���
            }
            
            if((motor_left.filter_commutation_time_sum > 5000 && motor_left.pit_trig_count > 5000) ||                                   // �ջ����� 0.5s �� �����ת�ٵ��� 285RPM ����Ϊ ת�ٹ��� �ر���� 
#if     MOTOR_DISTURB_PROTECT == DRIVER_ENABLE            
               (motor_left.speed_change < -MOTOR_DISTURB_SPEED && func_abs(motor_left.duty_change) < 100 && motor_left.pit_trig_count > 5000) ||       // �ջ����� 0.5s �� �����ת��ͻȻ�½�(Ĭ�� 500RPM) ����Ϊ �ܵ��쳣���� �ر���� 
#endif                 
               (motor_left.overtime_change_phase_count > 1000)          ||                                                              // �� 50ms δ�������㻻�� ����Ϊ �����ת �ر����                
                
               (func_abs(motor_left.sensorless_duty) < 0.05)            ||                                                              // �� ռ�ձȵ��� 5%  ����Ϊ ������� �ر����
                 
               (motor_left.motor_protect_state  == PROTECT_MODE)        ||                                                              // ��������뱣��״̬ ��ر����                 
                 
               (battery_value.protect_flag == 1 && battery_value.battery_state == BATTERY_ERROR) ||                                     // ����ص�ѹ���뱣��״̬ ��ʹ����Ч ��ر����   
                   
               ((motor_left.sensorless_duty * 10000.0f) * motor_left.motor_duty < 0.0f))                                                // ��ռ�ձȷ���ı� ��ر����                                                           
            {
              
                Cy_Tcpwm_Counter_Disable(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);    // �ر� ��ʱ���� ��ʱ��
                
                sensorless_motor_trig_disable(&motor_left);                     // �ر� �����źŴ����ж�
                
                motor_commutation(&motor_left, 0, 0);                           // �ر� ���
                
                motor_left.pit_trig_count = 0;                                  // �����ǰ���ڶ�ʱ������ֵ
                
                motor_left.speed_change = 0;                                    // ������ת�ٱ仯��¼ֵ
                
                motor_left.duty_change = 0;                                     // ������ռ�ձȱ仯��¼ֵ
                
                motor_left.motor_speed = 0;                                     // ������ת������
                
                motor_left.motor_speed_filter = 0;                              // ������ת���˲�����

                motor_left.sensorless_state = SENSORLESS_RESTART;               // ����״̬ ����Ϊ ����״̬
            }
            
            motor_left.isr_trig_count_save = motor_left.isr_trig_count;         // ���浱ǰ �����źŴ�������ֵ
            

        }break;
        
        case SENSORLESS_RESTART:                                                // �������״̬
        {            
            motor_commutation(&motor_left, 0, 0);                               // ���յ�ǰ��λ���  0ռ�ձ�  �رյ��
            
            if(func_abs(motor_left.motor_duty) > 0.06 && motor_left.pit_trig_count > SENSERLESS_RESTART_TIME * 20)// ���ռ�ձ���Ч(����6%) �����Ѿ��ȴ���ָ��ʱ��  ���������������
            {
                motor_left.pit_trig_count = 0;                                  // �����ǰ���ڶ�ʱ������ֵ
                
                motor_left.sensorless_state = SENSORLESS_OPEN_LOOP_SINE;        // ����״̬ ����Ϊ ������������״̬
            }
            if(motor_left.motor_duty == 0                               ||      // ���ռ�ձ���Ч  ��ֹͣ����
               
              (motor_left.motor_protect_state  == PROTECT_MODE)         ||      // ��������뱣��״̬ ��ֹͣ����   
                
              (battery_value.protect_flag == 1 && battery_value.battery_state == BATTERY_ERROR))    // ����ص�ѹ���뱣��״̬ ��ʹ����Ч ��ֹͣ����                                                   
            {
                motor_left.sensorless_state = SENSORLESS_STOP_STATE;            // ����״̬ ����Ϊ ֹͣ״̬
            }
        }break;
        
        case SENSORLESS_STOP_STATE:                                             // ���ֹͣ״̬
        {            
            motor_commutation(&motor_left, 0, 0);                               // ���յ�ǰ��λ���  0ռ�ձ�  �رյ��
            
            if(func_abs(motor_left.motor_duty) > 0.06 && (motor_left.motor_protect_state  != PROTECT_MODE))  // ���ռ�ձ���Ч(����6%) ���δ���뱣��״̬ ���������������
            {   
                motor_left.pit_trig_count = 0;                                  // �����ǰ���ڶ�ʱ������ֵ
                
                motor_left.sensorless_state = SENSORLESS_OPEN_LOOP_SINE;        // ����״̬ ����Ϊ ������������״̬
            }
        }break;
        
        default:break;
    }
    /*************************����޸�����ģʽ ����ѭ��ִ������*************************/ 
    
    
    /*************************�Ҳ��޸�����ģʽ ����ѭ��ִ������*************************/ 
    
    motor_right.pit_trig_count ++;                                              // ����ֵ���� 
    
#if USER_CONTROL_MODE == 1                                                      // pwm�������ģʽ
    if(motor_right_pwm_in_data.pwm_high_level >= 1050 && motor_right_pwm_in_data.pwm_high_level < 2020)                // �жϿ�������ĸߵ�ƽ�Ƿ���� 1.05ms ����С�� 2.02ms
    {
        motor_right.motor_duty = func_limit_ab((motor_right_pwm_in_data.pwm_high_level - 1000) / 1000.0f, 0, 1);       // ����Ŀ��ռ�ձ�
        
        motor_right.motor_duty = motor_right.motor_duty * (SENSERLESS_MOTOR_RIGHT_DIR == 0 ? 1 : -1); 
    }
    else
    {
        motor_right.motor_duty = 0;                                                                     
    }
#endif
    
    switch(motor_right.sensorless_state)                                       // �жϵ�ǰ �޸� ����״̬
    {  
        case SENSORLESS_OPEN_LOOP_SINE:                                         // ���� ����ǣ��״̬ 
        {
            if(motor_right.pit_trig_count <= 500)                               // ������ǰ25ms ������Ƿ�����ת �������ת��ֱ�ӽ��뿪��BLDCǣ�� ���迪������ǣ��
            {
                motor_right.sensorless_value_last = motor_right.sensorless_value_now;                                   // �����ϴ� �����ź� ��λֵ
            
                motor_right.sensorless_value_now = gpio_get_level(MOTOR_RIGHT_SENSORLESS_A_TRIG) * 4 +                  // ���㱾�� �����ź� ��λֵ  
                                                   gpio_get_level(MOTOR_RIGHT_SENSORLESS_B_TRIG) * 2 +
                                                   gpio_get_level(MOTOR_RIGHT_SENSORLESS_C_TRIG);    
                
                if(motor_right.sensorless_value_last != motor_right.sensorless_value_now)                               // ��⵽���㻻��
                {
                    motor_right.common_change_phase_count ++;                   // �������ֵ����
                }
                
                if(motor_right.pit_trig_count == 500)                           // 25ms�� �жϻ������
                {
                    if(motor_right.common_change_phase_count > 50)              // �����೬��50��(���ת�ٳ��� 2858 RPM)  ��ֱ�ӽ��뿪��BLDCǣ��
                    {
                        sensorless_pwm_output_init(&motor_right);               // �������ö�ʱ��
                        
                        motor_right.overtime_change_phase_count = 0;            // ������೬ʱ����ֵ
                    
                        motor_right.common_change_phase_count = 0;              // ��������������ֵ
                        
                        timer_start(RIGHT_MOTOR_COUNT_TIMER);                   // ���� ΢����� ��ʱ��
                        
                        motor_right.sensorless_state = SENSORLESS_OPEN_LOOP_BLDC;// ����״̬ ����Ϊ ���� BLDC ǣ��״̬
                    }
                       
                }
            }
            else                                                                // ��������ִ������ǣ��
            {
                if(sensorless_start_sine(&motor_right))                         // ִ������ǣ������ �ж����Ƿ�ǣ�����
                {
                    motor_right.overtime_change_phase_count = 0;                // ������೬ʱ����ֵ
                    
                    motor_right.common_change_phase_count = 0;                  // ��������������ֵ
                    
                    timer_start(RIGHT_MOTOR_COUNT_TIMER);                       // ���� ΢����� ��ʱ��
                    
                    motor_right.sensorless_state = SENSORLESS_OPEN_LOOP_BLDC;   // ����״̬ ����Ϊ ���� BLDC ǣ��״̬
                }
            }
            
            
        }break;
        
        case SENSORLESS_OPEN_LOOP_BLDC:                                         // ���� BLDC ǣ��״̬
        {         
            motor_right.sensorless_value_last = motor_right.sensorless_value_now;// �����ϴ� �����ź� ��λֵ
            
            motor_right.sensorless_value_now = gpio_get_level(MOTOR_RIGHT_SENSORLESS_A_TRIG) * 4 +                                // ���㱾�� �����ź� ��λֵ  
                                               gpio_get_level(MOTOR_RIGHT_SENSORLESS_B_TRIG) * 2 +
                                               gpio_get_level(MOTOR_RIGHT_SENSORLESS_C_TRIG);         
            
            motor_right.step_change_time_us = timer_get(RIGHT_MOTOR_COUNT_TIMER);// ��ȡ��ǰ ��ʱ�� ����ʱ�� 
            
            if((motor_right.sensorless_value_last != motor_right.sensorless_value_now || motor_right.step_change_time_us > 5000)) // ���ֻ��� ���� ����ʱ�䳬��5000΢�� �л���� 
            {         
                timer_clear(RIGHT_MOTOR_COUNT_TIMER);                           // �����ʱ������ֵ
              
                if(motor_right.step_change_time_us > 5000 || motor_right.step_change_time_us < 500)                               // ������ 5000 ΢�� ���� ���� 500΢�� ����Ϊ���೬ʱ(����)  
                {
                    motor_right.overtime_change_phase_count ++;                 // ���೬ʱ ����ֵ����
                }
                else                                                            // ����Ϊ��������
                {       
                    motor_right.common_change_phase_count ++;                   // �������� ����ֵ����
                }
                
                motor_next_step(&motor_right);                                  // �л���һ�� 
                
                motor_commutation(&motor_right, SENSERLESS_OPENLOOP_BLDC_DUTY, 0);// ���յ�ǰ��λ��� ����������������ж�
                
                if(motor_right.overtime_change_phase_count > ((SENSERLESS_OPENLOOP_BLDC_TURNS - SENSERLESS_CLOSELOOP_CHECK) * 6)) // �����೬ʱ����ֵ ���� 250�� ��Ϊǣ������ʧ�� �������ο���ǣ�� (��Ϊ�������� ȷ���ܹ���ȷ����ǣ��)
                {
                    
                    motor_right.overtime_change_phase_count = 0;                // ������೬ʱ����ֵ
                    
                    motor_right.pit_trig_count = 0;                             // �����ǰ���ڶ�ʱ������ֵ
                    
                    motor_commutation(&motor_right, 0, 0);                      // �ر����
                    
                    motor_right.sensorless_state = SENSORLESS_RESTART;          // ����״̬ ����Ϊ ����״̬

                }
                  
                if(motor_right.common_change_phase_count > (SENSERLESS_CLOSELOOP_CHECK * 6))                                      // ����������������� 50 �� ����Ϊ���Խ���ջ�ģʽ �������ο���ǣ�� 
                {  
                    if(motor_right.motor_duty >= 0.0f)
                    {
                        motor_right.sensorless_duty =  (SENSERLESS_OPENLOOP_BLDC_DUTY / 10000.0f);// ���ռ�ձȸ���ֵ ��10%ռ�ձ�����
                    }
                    else
                    {
                        motor_right.sensorless_duty = -(SENSERLESS_OPENLOOP_BLDC_DUTY / 10000.0f);// ���ռ�ձȸ���ֵ ��10%ռ�ձ�����
                    }
                    motor_right.pit_trig_count = 0;                             // �����ǰ���ڶ�ʱ������ֵ
                    
                    motor_right.isr_trig_count = 0;                             // �����ǰ�����źŴ�������ֵ
                    
                    sensorless_motor_trig_enable(&motor_right);                 // ʹ�� �����ź� �����ж�        
                    
                    timer_clear(RIGHT_MOTOR_COUNT_TIMER);                       // �����ʱ������ֵ
                    
                    motor_right.sensorless_state = SENSORLESS_CLOSE_LOOP;       // ����״̬ ����Ϊ �ջ�����״̬
                }
            }
        }break;
        
        case SENSORLESS_CLOSE_LOOP:                                             // �ջ�����״̬
        {         
            if(motor_right.isr_trig_count > 500)
            {
                motor_right.motor_speed = 60000000 / (motor_right.filter_commutation_time_sum * 7);                             // ���㵱ǰ���ת��

                if(motor_right.motor_duty < 0.0f)
                {
                    motor_right.motor_speed = -motor_right.motor_speed;
                }
                
                motor_right.motor_speed_filter = (motor_right.motor_speed + motor_right.motor_speed_filter * 99.0f) / 100.0f;    // ��ת�ٽ��е�ͨ�˲�
            }
            
            if(motor_right.pit_trig_count > 1000 && motor_right.pit_trig_count % SENSERLESS_SPEED_INCREMENTAL == 0)              // ������ & ������ ����ջ� 50ms �� �����û����õļӼ��ٲ��� ִ�мӼ���
            {
                if(motor_right.sensorless_duty < motor_right.motor_duty)
                {
                    motor_right.sensorless_duty += (func_limit_ab(func_abs(motor_right.sensorless_duty) * 0.0003, 0.00005, 0.0003));  
                }
                else if(motor_right.sensorless_duty > motor_right.motor_duty)
                {
                    motor_right.sensorless_duty -= (func_limit_ab(func_abs(motor_right.sensorless_duty) * 0.0003, 0.00005, 0.0003));
                }
                
                motor_right.sensorless_duty = func_limit_ab(motor_right.sensorless_duty, -1.0f, 1.0f);
            }
            
            if(motor_right.pit_trig_count % 20 == 0)                            // 1ms ִ��һ�� ��¼�ջ��ٶ����� ���ڸ��ٿ����ű���
            {
                motor_right.speed_record[motor_right.speed_record_count] = (int32)func_abs(motor_right.motor_speed_filter);     // ���浱ǰ�ٶ�
                
                motor_right.speed_change   = motor_right.speed_record[motor_right.speed_record_count] -                         // ���� 100ms �ڵ��ٶ� �仯
                                            motor_right.speed_record[(motor_right.speed_record_count == 99 ? 0 : motor_right.speed_record_count + 1)];
                
                if(++ motor_right.speed_record_count >= 100)                    // �ٶȱ������� ѭ������
                {
                    motor_right.speed_record_count = 0;
                }     
                
                motor_right.duty_change = (int32)(motor_right.motor_duty * 10000) - motor_right.duty_record; // ����ռ�ձ� �仯
                
                motor_right.duty_record = (int32)(motor_right.motor_duty * 10000);// ���浱ǰռ�ձ�����
                
                if(func_abs(motor_right.duty_change) >= 100)
                {
                    motor_right.duty_record_count = 1;
                }
                else
                {
                    if(motor_right.duty_record_count > 0 && motor_right.duty_record_count < 1000)
                    {
                        motor_right.duty_change = 100;
                        
                        if(++ motor_right.duty_record_count == 1000)
                        {
                            motor_right.duty_record_count = 0;
                            
                            motor_right.duty_change = 0;
                        }
                    }
                }   
            }
            
            if(motor_right.isr_trig_count_save == motor_right.isr_trig_count)   // �� ���㻻���ж� 50΢����δ���� ���¼
            {
                motor_right.overtime_change_phase_count ++;                     // ���೬ʱ ����ֵ����
            }
            else                                                                // �� 50΢���ڴ��� ���㻻���ж� �������¼
            {
                motor_right.overtime_change_phase_count = 0;                    // ���೬ʱ ����ֵ���
            }
            
            if((motor_right.filter_commutation_time_sum > 5000  && motor_right.pit_trig_count > 5000) ||                                // �ջ����� 0.5s �� �����ת�ٵ��� 285RPM ����Ϊ ת�ٹ��� �ر���� 
#if     MOTOR_DISTURB_PROTECT == DRIVER_ENABLE                 
               (motor_right.speed_change < -MOTOR_DISTURB_SPEED && func_abs(motor_right.duty_change) < 100 && motor_right.pit_trig_count > 5000) ||       // �ջ����� 0.5s �� �����ת��ͻȻ�½�(Ĭ�� 500RPM) ����Ϊ �ܵ��쳣���� �ر���� 
#endif                 
               (motor_right.overtime_change_phase_count > 1000)         ||                                                              // �� 50ms δ�������㻻�� ����Ϊ �����ת �ر����                
                
               (func_abs(motor_right.sensorless_duty) < 0.05)           ||                                                              // �� ռ�ձȵ��� 5%  ����Ϊ ������� �ر����
                 
               (motor_right.motor_protect_state  == PROTECT_MODE)       ||                                                              // ��������뱣��״̬ ��ر����                 
                 
               (battery_value.protect_flag == 1 && battery_value.battery_state == BATTERY_ERROR) ||                                     // ����ص�ѹ���뱣��״̬ ��ʹ����Ч ��ر����   
                   
               ((motor_right.sensorless_duty * 10000.0f) * motor_right.motor_duty < 0.0f))                                              // ��ռ�ձȷ���ı� ��ر����    
            {
              
                Cy_Tcpwm_Counter_Disable(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);   // �ر� ��ʱ���� ��ʱ��
                
                sensorless_motor_trig_disable(&motor_right);                    // �ر� �����źŴ����ж�
                
                motor_commutation(&motor_right, 0, 0);                          // �ر� ���
                
                motor_right.pit_trig_count = 0;                                 // �����ǰ���ڶ�ʱ������ֵ
                
                motor_right.speed_change = 0;                                   // ������ת�ٱ仯��¼ֵ
                
                motor_right.duty_change = 0;                                    // ������ռ�ձȱ仯��¼ֵ
                
                motor_right.motor_speed = 0;                                    // ������ת������
                
                motor_right.motor_speed_filter = 0;                             // ������ת���˲�����
                
                motor_right.sensorless_state = SENSORLESS_RESTART;              // ����״̬ ����Ϊ ����״̬
            }
            
            motor_right.isr_trig_count_save = motor_right.isr_trig_count;       // ���浱ǰ �����źŴ�������ֵ
        }break;
        
        case SENSORLESS_RESTART:                                                // �������״̬
        {            
            motor_commutation(&motor_right, 0, 0);                              // ���յ�ǰ��λ���  0ռ�ձ�  �رյ��
            
            if(func_abs(motor_right.motor_duty) > 0.06 && motor_right.pit_trig_count > SENSERLESS_RESTART_TIME * 20)// ���ռ�ձ���Ч(����6%) �����Ѿ��ȴ���ָ��ʱ��  ���������������
            {
                motor_right.pit_trig_count = 0;                                 // �����ǰ���ڶ�ʱ������ֵ
                
                motor_right.sensorless_state = SENSORLESS_OPEN_LOOP_SINE;       // ����״̬ ����Ϊ ������������״̬
            }
            if(motor_right.motor_duty == 0                              ||      // ���ռ�ձ���Ч  ��ֹͣ����
               
              (motor_right.motor_protect_state  == PROTECT_MODE)        ||      // ��������뱣��״̬ ��ֹͣ����   
                
              (battery_value.protect_flag == 1 && battery_value.battery_state == BATTERY_ERROR))    // ����ص�ѹ���뱣��״̬ ��ʹ����Ч ��ֹͣ����                                                   
            {
                motor_right.sensorless_state = SENSORLESS_STOP_STATE;           // ����״̬ ����Ϊ ֹͣ״̬
            }
        }break;
        
        case SENSORLESS_STOP_STATE:                                             // ���ֹͣ״̬
        {           
            motor_commutation(&motor_right, 0, 0);                              // ���յ�ǰ��λ���  0ռ�ձ�  �رյ��
            
            if(func_abs(motor_right.motor_duty) > 0.06 && (motor_right.motor_protect_state  != PROTECT_MODE))  // ���ռ�ձ���Ч(����6%) ���δ���뱣��״̬ ���������������
            {   
                motor_right.pit_trig_count = 0;                                 // �����ǰ���ڶ�ʱ������ֵ
                
                motor_right.sensorless_state = SENSORLESS_OPEN_LOOP_SINE;       // ����״̬ ����Ϊ ������������״̬
            }
        }break;
        
        default:break;
    }
    /*************************�Ҳ��޸�����ģʽ ����ѭ��ִ������*************************/ 
}


//-------------------------------------------------------------------------------------------------------------------
// �������     �޸�ģʽ ����ʱ������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_start_check();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void sensorless_start_check(void)
{
#if SENSERLESS_WORK_MUSIC == DRIVER_ENABLE     
    
    motor_voice_output();                                                       // ��������Ƶ�ź�
    
    motor_left_phase_check(300);                                                // ������� MOS �� Ԥ�� ���ܼ�� ����û�м��������� �����Ҫ��Ϊ�ж��Ƿ�������
        
    motor_right_phase_check(300);                                               // �Ҳ����� MOS �� Ԥ�� ���ܼ�� ����û�м��������� �����Ҫ��Ϊ�ж��Ƿ�������
#endif

}

//-------------------------------------------------------------------------------------------------------------------
// �������     �޸� ������ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_trig_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void sensorless_trig_init(void)
{
    cy_stc_gpio_pin_config_t            exti_pin_cfg    = {0};                                  // �����޸й����źŴ������Ŷ���
    
    cy_stc_sysint_irq_t                 exti_irq_cfg;                                           // �����޸й����źŴ����ж϶���
    
    cy_stc_tcpwm_counter_config_t       sensorless_delay_config;                                // �����޸���ʱ���ඨʱ������
        
    cy_stc_sysint_irq_t                 sensorless_delay_irq_cfg;                               // �����޸���ʱ�����ж϶���
        
        
    /*************************���� ����޸� �����ź��źŴ���*************************/           
    exti_pin_cfg.driveMode              = CY_GPIO_DM_HIGHZ;                                     // IO����Ϊ����̬
        
    exti_pin_cfg.intEdge                = EXTI_TRIGGER_BOTH;                                    // ������ʽΪ˫�ߴ���
        
    exti_pin_cfg.intMask                = 1ul;                                                  // ����ʹ��
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_LEFT_SENSORLESS_A_TRIG), (MOTOR_LEFT_SENSORLESS_A_TRIG % 8), &exti_pin_cfg);        // ���� A�� �����ź� IO����
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_LEFT_SENSORLESS_B_TRIG), (MOTOR_LEFT_SENSORLESS_B_TRIG % 8), &exti_pin_cfg);        // ���� B�� �����ź� IO����
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_LEFT_SENSORLESS_C_TRIG), (MOTOR_LEFT_SENSORLESS_C_TRIG % 8), &exti_pin_cfg);        // ���� C�� �����ź� IO����
    
    exti_irq_cfg.intIdx     = CPUIntIdx5_IRQn;                                                  // ѡ���жϺ�
        
    exti_irq_cfg.isEnabled  = true;                                                             // �ж�ʹ��
    
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_0_IRQn + MOTOR_LEFT_SENSORLESS_A_TRIG / 8);           // ѡ�� A�� �����ź� �ж�����
    
    interrupt_init(&exti_irq_cfg, sensorless_motor_left_trig_isr, 0);                                                   // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
  
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_0_IRQn + MOTOR_LEFT_SENSORLESS_B_TRIG / 8);           // ѡ�� B�� �����ź� �ж�����
                                                                                                                        
    interrupt_init(&exti_irq_cfg, sensorless_motor_left_trig_isr, 0);                                                   // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
    
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_0_IRQn + MOTOR_LEFT_SENSORLESS_C_TRIG / 8);           // ѡ�� C�� �����ź� �ж�����
                                                                                                                        
    interrupt_init(&exti_irq_cfg, sensorless_motor_left_trig_isr, 0);                                                   // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
    
    exti_disable(MOTOR_LEFT_SENSORLESS_A_TRIG);                                                 // ��ʱʧ�� A�� �����ź� �����ж�
        
    exti_disable(MOTOR_LEFT_SENSORLESS_B_TRIG);                                                 // ��ʱʧ�� A�� �����ź� �����ж�
        
    exti_disable(MOTOR_LEFT_SENSORLESS_C_TRIG);                                                 // ��ʱʧ�� A�� �����ź� �����ж�
    /*************************���� ����޸� �����ź��źŴ���*************************/
    
    
    
    /*************************���� ����޸� �ӳٻ��ඨʱ��ʱ��*************************/
    // ��ʼ���ӳٻ������ʱ��  ��ʱ��ͨ��0   ʹ��16λ��Ƶ����ͨ��0 10��Ƶ ���8Mhzʱ�ӵ���ʱ�� 
    Cy_SysClk_PeriphAssignDivider(MOTOR_LEFT_SENSORLESS_DELAY_CLOCK, CY_SYSCLK_DIV_16_BIT, 0);  // ����ʱ���ź� ѡ��16λ0ͨ������� ��ʱ��ʱ��
      
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, 9);                                     // ����ʱ�ӷ���ϵ�� ��ǰ10��Ƶ��Ƶ��8Mhz  ����125ns��
      
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0);                                     // ʹ��ʱ��
    /*************************���� ����޸� �ӳٻ��ඨʱ��ʱ��*************************/ 
      
      
    /*************************���� ����޸� �ӳٻ��ඨʱ��*************************/   
    sensorless_delay_config.period               = 0xffff                        ;              // ��ʱ������Ϊ65535
      
    sensorless_delay_config.compare0             = 0xffff                        ;              // ��ʱ���Ƚ�ֵΪ65535
      
    sensorless_delay_config.compare1             = 0xffff                        ;              // ��ʱ���Ƚ�ֵΪ65535
      
    sensorless_delay_config.clockPrescaler       = CY_TCPWM_PRESCALER_DIVBY_8    ;              // ʱ��8��Ƶ 8Mhz����ʱ�ӱ�8��ƵΪ1Mhz ��˶�ʱ��������λΪ΢��
      
    sensorless_delay_config.runMode              = CY_TCPWM_COUNTER_ONESHOT      ;              // ������һ�� �����ֹͣ����
      
    sensorless_delay_config.countDirection       = CY_TCPWM_COUNTER_COUNT_UP     ;              // ���ϼ���
      
    sensorless_delay_config.compareOrCapture     = CY_TCPWM_COUNTER_MODE_COMPARE ;              // �Ƚ�ģʽ
      
    sensorless_delay_config.countInputMode       = CY_TCPWM_INPUT_LEVEL          ;              // �����¼�Ϊ����
      
    sensorless_delay_config.countInput           = 1uL                           ;              // �����¼�����ߵ�ƽ
      
    sensorless_delay_config.trigger0EventCfg     = CY_TCPWM_COUNTER_OVERFLOW     ;              // �жϴ����¼�0Ϊ���ʱ�����ж�
      
    sensorless_delay_config.trigger1EventCfg     = CY_TCPWM_COUNTER_OVERFLOW     ;              // �жϴ����¼�1Ϊ���ʱ�����ж�
              
    Cy_Tcpwm_Counter_Init(MOTOR_LEFT_SENSORLESS_DELAY_TIMER, &sensorless_delay_config);         // ��ʼ����ʱ������
      
    Cy_Tcpwm_Counter_Enable(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);                                 // ʹ�ܶ�ʱ��
      
    Cy_Tcpwm_Counter_SetCounter(MOTOR_LEFT_SENSORLESS_DELAY_TIMER, 0);                          // �����ʱ������ֵ
      
    Cy_Tcpwm_Counter_SetTC_IntrMask(MOTOR_LEFT_SENSORLESS_DELAY_TIMER);                         // ʹ�ܶ�ʱ���ж�
    /*************************���� ����޸� �ӳٻ��ඨʱ��*************************/   
      
      
    /***********************���� ����޸� �ӳٻ��ඨʱ���ж�***********************/     
    sensorless_delay_irq_cfg.sysIntSrc          = MOTOR_LEFT_SENSORLESS_DELAY_ISR;              // ѡ���жϴ���ԴΪ��ʱ��0��ͨ��20
      
    sensorless_delay_irq_cfg.intIdx             = CPUIntIdx6_IRQn                ;              // ѡ���жϴ���ͨ��Ϊ�û��ж�6
      
    sensorless_delay_irq_cfg.isEnabled          = true                           ;              // �����жϴ���
      
    interrupt_init(&sensorless_delay_irq_cfg, motor_left_sensorless_delay_isr, 0);              // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
    /***********************���� ����޸� �ӳٻ��ඨʱ���ж�***********************/     
    
    
    
    /*************************���� �Ҳ��޸� �����ź��źŴ���*************************/           
    exti_pin_cfg.driveMode              = CY_GPIO_DM_HIGHZ;                                     // IO����Ϊ����̬
        
    exti_pin_cfg.intEdge                = EXTI_TRIGGER_BOTH;                                    // ������ʽΪ˫�ߴ���
        
    exti_pin_cfg.intMask                = 1ul;                                                  // ����ʹ��
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_RIGHT_SENSORLESS_A_TRIG), (MOTOR_RIGHT_SENSORLESS_A_TRIG % 8), &exti_pin_cfg);        // ���� A�� �����ź� IO����
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_RIGHT_SENSORLESS_B_TRIG), (MOTOR_RIGHT_SENSORLESS_B_TRIG % 8), &exti_pin_cfg);        // ���� B�� �����ź� IO����
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_RIGHT_SENSORLESS_C_TRIG), (MOTOR_RIGHT_SENSORLESS_C_TRIG % 8), &exti_pin_cfg);        // ���� C�� �����ź� IO����
    
    exti_irq_cfg.intIdx     = CPUIntIdx5_IRQn;                                                  // ѡ���жϺ�
        
    exti_irq_cfg.isEnabled  = true;                                                             // �ж�ʹ��
    
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_0_IRQn + MOTOR_RIGHT_SENSORLESS_A_TRIG / 8);           // ѡ�� A�� �����ź� �ж�����
    
    interrupt_init(&exti_irq_cfg, sensorless_motor_right_trig_isr, 0);                                                   // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
  
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_0_IRQn + MOTOR_RIGHT_SENSORLESS_B_TRIG / 8);           // ѡ�� B�� �����ź� �ж�����
                                                                                                                        
    interrupt_init(&exti_irq_cfg, sensorless_motor_right_trig_isr, 0);                                                   // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
    
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_0_IRQn + MOTOR_RIGHT_SENSORLESS_C_TRIG / 8);           // ѡ�� C�� �����ź� �ж�����
                                                                                                                        
    interrupt_init(&exti_irq_cfg, sensorless_motor_right_trig_isr, 0);                                                   // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
    
    exti_disable(MOTOR_RIGHT_SENSORLESS_A_TRIG);                                                // ��ʱʧ�� A�� �����ź� �����ж�
        
    exti_disable(MOTOR_RIGHT_SENSORLESS_B_TRIG);                                                // ��ʱʧ�� A�� �����ź� �����ж�
        
    exti_disable(MOTOR_RIGHT_SENSORLESS_C_TRIG);                                                // ��ʱʧ�� A�� �����ź� �����ж�
    /*************************���� �Ҳ��޸� �����ź��źŴ���*************************/
    
    
    
    /*************************���� �Ҳ��޸� �ӳٻ��ඨʱ��ʱ��*************************/
    // ��ʼ���ӳٻ������ʱ��  ��ʱ��ͨ��0   ʹ��16λ��Ƶ����ͨ��0 10��Ƶ ���8Mhzʱ�ӵ���ʱ�� 
    Cy_SysClk_PeriphAssignDivider(MOTOR_RIGHT_SENSORLESS_DELAY_CLOCK, CY_SYSCLK_DIV_16_BIT, 0); // ����ʱ���ź� ѡ��16λ0ͨ������� ��ʱ��ʱ��
      
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, 9);                                     // ����ʱ�ӷ���ϵ�� ��ǰ10��Ƶ��Ƶ��8Mhz  ����125ns��
      
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0);                                     // ʹ��ʱ��
    /*************************���� �Ҳ��޸� �ӳٻ��ඨʱ��ʱ��*************************/ 
      
      
    /*************************���� �Ҳ��޸� �ӳٻ��ඨʱ��*************************/   
    sensorless_delay_config.period               = 0xffff                        ;              // ��ʱ������Ϊ65535
      
    sensorless_delay_config.compare0             = 0xffff                        ;              // ��ʱ���Ƚ�ֵΪ65535
      
    sensorless_delay_config.compare1             = 0xffff                        ;              // ��ʱ���Ƚ�ֵΪ65535
      
    sensorless_delay_config.clockPrescaler       = CY_TCPWM_PRESCALER_DIVBY_8    ;              // ʱ��8��Ƶ 8Mhz����ʱ�ӱ�8��ƵΪ1Mhz ��˶�ʱ��������λΪ΢��
      
    sensorless_delay_config.runMode              = CY_TCPWM_COUNTER_ONESHOT      ;              // ������һ�� �����ֹͣ����
      
    sensorless_delay_config.countDirection       = CY_TCPWM_COUNTER_COUNT_UP     ;              // ���ϼ���
      
    sensorless_delay_config.compareOrCapture     = CY_TCPWM_COUNTER_MODE_COMPARE ;              // �Ƚ�ģʽ
      
    sensorless_delay_config.countInputMode       = CY_TCPWM_INPUT_LEVEL          ;              // �����¼�Ϊ����
      
    sensorless_delay_config.countInput           = 1uL                           ;              // �����¼�����ߵ�ƽ
      
    sensorless_delay_config.trigger0EventCfg     = CY_TCPWM_COUNTER_OVERFLOW     ;              // �жϴ����¼�0Ϊ���ʱ�����ж�
      
    sensorless_delay_config.trigger1EventCfg     = CY_TCPWM_COUNTER_OVERFLOW     ;              // �жϴ����¼�1Ϊ���ʱ�����ж�
              
    Cy_Tcpwm_Counter_Init(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER, &sensorless_delay_config);        // ��ʼ����ʱ������
      
    Cy_Tcpwm_Counter_Enable(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);                                // ʹ�ܶ�ʱ��
      
    Cy_Tcpwm_Counter_SetCounter(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER, 0);                         // �����ʱ������ֵ
      
    Cy_Tcpwm_Counter_SetTC_IntrMask(MOTOR_RIGHT_SENSORLESS_DELAY_TIMER);                        // ʹ�ܶ�ʱ���ж�
    /*************************���� �Ҳ��޸� �ӳٻ��ඨʱ��*************************/   
      
      
    /***********************���� �Ҳ��޸� �ӳٻ��ඨʱ���ж�***********************/     
    sensorless_delay_irq_cfg.sysIntSrc          = MOTOR_RIGHT_SENSORLESS_DELAY_ISR;             // ѡ���жϴ���ԴΪ��ʱ��0��ͨ��20
      
    sensorless_delay_irq_cfg.intIdx             = CPUIntIdx6_IRQn                ;              // ѡ���жϴ���ͨ��Ϊ�û��ж�6
      
    sensorless_delay_irq_cfg.isEnabled          = true                           ;              // �����жϴ���
      
    interrupt_init(&sensorless_delay_irq_cfg, motor_right_sensorless_delay_isr, 0);             // �жϳ�ʼ�� ���ȼ� 0  ��������ȼ���
    /***********************���� �Ҳ��޸� �ӳٻ��ඨʱ���ж�***********************/     
          
    timer_init(LEFT_MOTOR_COUNT_TIMER, TIMER_US);                                               // ��ʼ�� �����������ʱ��
    
    timer_init(RIGHT_MOTOR_COUNT_TIMER, TIMER_US);                                              // ��ʼ�� �Ҳ���������ʱ��
    
    pit_us_init(LEFT_MOTOR_PIT_TIMER, 50);                                                      // ��ʼ�� ����� ��ѯ�����ж� 
}









