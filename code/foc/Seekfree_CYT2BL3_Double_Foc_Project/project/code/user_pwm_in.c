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
#include "user_pwm_in.h"

user_in_struct motor_left_pwm_in_data;
user_in_struct motor_right_pwm_in_data;


//-------------------------------------------------------------------------------------------------------------------
// �������     ��� pwm������ ��ʱ �ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_left_pwm_in_over_time_isr();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_left_pwm_in_over_time_isr(void)
{
    if(Cy_Tcpwm_Counter_GetTC_IntrMasked(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER))     // ����Ƿ��ǲ���ʱ�жϱ�־λ
    {
        Cy_Tcpwm_Counter_ClearTC_Intr(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER);        // ��������жϱ�־λ
      
        motor_left_pwm_in_data.pwm_high_level = 0;                              // ����ʱ����Ϊ�ر����
        
        motor_left_pwm_in_data.pwm_low_level = 0;
        
        motor_left_pwm_in_data.pwm_period_num = 0;
          
        motor_left_pwm_in_data.get_duty = 0;                                   
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��� pwm�������ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_left_pwm_in_isr();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_left_pwm_in_isr(void)
{
    if(Cy_Tcpwm_Counter_GetCC0_IntrMasked(MOTOR_LEFT_PWM_IN_TIMER))             // �Ƚ���0�����ж� �����⵽�½��� 
    {
        Cy_Tcpwm_Counter_ClearCC0_Intr(MOTOR_LEFT_PWM_IN_TIMER);                // ����Ƚ���0�����ж� 
    }
    
    if(Cy_Tcpwm_Counter_GetCC1_IntrMasked(MOTOR_LEFT_PWM_IN_TIMER))             // �Ƚ���1�����ж� �����⵽������ 
    {
        Cy_Tcpwm_Counter_ClearCC1_Intr(MOTOR_LEFT_PWM_IN_TIMER);                // ����Ƚ���1�����ж� 
        
        motor_left_pwm_in_data.pwm_high_level = Cy_Tcpwm_Counter_GetCompare0(MOTOR_LEFT_PWM_IN_TIMER) + 2;      // ��ȡ�Ƚ���0����ļ���ֵ(��⵽�½�����˵��������Ǹߵ�ƽʱ��) ��2����Ϊ�������������������2us
        
        motor_left_pwm_in_data.pwm_period_num = Cy_Tcpwm_Counter_GetCompare1(MOTOR_LEFT_PWM_IN_TIMER) + 2;      // ��ȡ�Ƚ���1����ļ���ֵ(��⵽��������˵�����������������ʱ��) ��2����Ϊ�������������������2us
        
        Cy_Tcpwm_Counter_SetCounter(MOTOR_LEFT_PWM_IN_TIMER, 0);                // �������ʱ�������Ĵ���ֵ
        
        Cy_Tcpwm_Counter_SetCounter(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER, 0);       // �������ʱ��ʱ��������ֵ(���е������ʾ��ȷ���������ź� ��ʱ����Ҫ���¼�ʱ)
        
        if(motor_left_pwm_in_data.pwm_high_level > motor_left_pwm_in_data.pwm_period_num) motor_left_pwm_in_data.pwm_high_level = 0;
        
        if(motor_left_pwm_in_data.pwm_period_num > 25000 || motor_left_pwm_in_data.pwm_period_num < 3000)      // Ҫ�������������� 40Hz ~ 300Hz ֮��
        {
            motor_left_pwm_in_data.pwm_high_level = 0;
            
            motor_left_pwm_in_data.pwm_period_num = 0;
            
            motor_left_pwm_in_data.get_duty = 0;
        }
        else
        {
            if((motor_left_pwm_in_data.pwm_high_level % 10) >= 7 )                                              // ���������ë�̡��������Ż�
            {
                motor_left_pwm_in_data.pwm_high_level = (motor_left_pwm_in_data.pwm_high_level / 10 + 1) * 10;  
            }
            else if((motor_left_pwm_in_data.pwm_high_level % 10) <= 3)
            {
                motor_left_pwm_in_data.pwm_high_level = (motor_left_pwm_in_data.pwm_high_level / 10) * 10;
            }
          
            motor_left_pwm_in_data.pwm_low_level = motor_left_pwm_in_data.pwm_period_num - motor_left_pwm_in_data.pwm_high_level;                          // ����͵�ƽʱ��(�͵�ƽʱ����û���õ���������Ϊ���ݹ۲�)
            
            motor_left_pwm_in_data.get_duty = (float)((float)motor_left_pwm_in_data.pwm_high_level / (float)motor_left_pwm_in_data.pwm_period_num);       // ����ռ�ձ� ��Χ 0~1 ��������
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������     �Ҳ� pwm������ ��ʱ �ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_right_pwm_in_over_time_isr();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_right_pwm_in_over_time_isr(void)
{
    if(Cy_Tcpwm_Counter_GetTC_IntrMasked(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER))    // ����Ƿ��ǲ���ʱ�жϱ�־λ
    {
        Cy_Tcpwm_Counter_ClearTC_Intr(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER);       // ��������жϱ�־λ
      
        motor_right_pwm_in_data.pwm_high_level = 0;                             // ����ʱ����Ϊ�ر����
        
        motor_right_pwm_in_data.pwm_low_level = 0;
        
        motor_right_pwm_in_data.pwm_period_num = 0;
          
        motor_right_pwm_in_data.get_duty = 0;  
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �Ҳ� pwm�������ж�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_right_pwm_in_isr();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void motor_right_pwm_in_isr(void)
{
    if(Cy_Tcpwm_Counter_GetCC0_IntrMasked(MOTOR_RIGHT_PWM_IN_TIMER))            // �Ƚ���0�����ж� �����⵽�½��� 
    {
        Cy_Tcpwm_Counter_ClearCC0_Intr(MOTOR_RIGHT_PWM_IN_TIMER);               // ����Ƚ���0�����ж� 
    }
    

    if(Cy_Tcpwm_Counter_GetCC1_IntrMasked(MOTOR_RIGHT_PWM_IN_TIMER))            // �Ƚ���1�����ж� �����⵽������ 
    {
        Cy_Tcpwm_Counter_ClearCC1_Intr(MOTOR_RIGHT_PWM_IN_TIMER);               // ����Ƚ���1�����ж� 
        
        motor_right_pwm_in_data.pwm_high_level = Cy_Tcpwm_Counter_GetCompare0(MOTOR_RIGHT_PWM_IN_TIMER) + 2;    // ��ȡ�Ƚ���0����ļ���ֵ(��⵽�½�����˵��������Ǹߵ�ƽʱ��) ��2����Ϊ�������������������2us
          
        motor_right_pwm_in_data.pwm_period_num = Cy_Tcpwm_Counter_GetCompare1(MOTOR_RIGHT_PWM_IN_TIMER) + 2;    // ��ȡ�Ƚ���1����ļ���ֵ(��⵽��������˵�����������������ʱ��) ��2����Ϊ�������������������2us
        
        Cy_Tcpwm_Counter_SetCounter(MOTOR_RIGHT_PWM_IN_TIMER, 0);               // �������ʱ�������Ĵ���ֵ
        
        Cy_Tcpwm_Counter_SetCounter(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER, 0);      // �������ʱ��ʱ��������ֵ(���е������ʾ��ȷ���������ź� ��ʱ����Ҫ���¼�ʱ)
        
        if(motor_right_pwm_in_data.pwm_high_level > motor_right_pwm_in_data.pwm_period_num) motor_right_pwm_in_data.pwm_high_level = 0;
        
        if(motor_right_pwm_in_data.pwm_period_num > 25000 || motor_right_pwm_in_data.pwm_period_num < 3000)    // Ҫ�������������� 40Hz ~ 300Hz ֮��
        {
            motor_right_pwm_in_data.pwm_high_level = 0;
            
            motor_right_pwm_in_data.pwm_period_num = 0;
            
            motor_right_pwm_in_data.get_duty = 0;
        }
        else
        {

            if((motor_right_pwm_in_data.pwm_high_level % 10) >= 7 )                                             // ���������ë�̡��������Ż�
            {
                motor_right_pwm_in_data.pwm_high_level = (motor_right_pwm_in_data.pwm_high_level / 10 + 1) * 10;
            }
            else if((motor_right_pwm_in_data.pwm_high_level % 10) <= 3)
            {
                motor_right_pwm_in_data.pwm_high_level = (motor_right_pwm_in_data.pwm_high_level / 10) * 10;
            }
            
            motor_right_pwm_in_data.pwm_low_level = motor_right_pwm_in_data.pwm_period_num - motor_right_pwm_in_data.pwm_high_level;                          // ����͵�ƽʱ��(�͵�ƽʱ����û���õ���������Ϊ���ݹ۲�)
            
            motor_right_pwm_in_data.get_duty = (float)((float)motor_right_pwm_in_data.pwm_high_level / (float)motor_right_pwm_in_data.pwm_period_num);       // ����ռ�ձ� ��Χ0-1 ��������
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������     PWM�������벶���ʼ�� 
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     sensorless_motor_isr();
// ��ע��Ϣ     �û�ʹ��pwm��Ϊ�����ź�ʱ����Ҫʹ�õ���������
//-------------------------------------------------------------------------------------------------------------------
void pwm_in_init(void)
{
    /******************************����ֲ�����******************************/
    cy_stc_gpio_pin_config_t            pwm_in_pin_cfg;                         // �������岶�����Ų�������
    cy_stc_tcpwm_counter_config_t       pwm_in_counter_cfg;                     // �������岶��ʱ����������
    cy_stc_tcpwm_counter_config_t       pwm_in_over_time_cfg;                   // �������岶��ʱ��ʱ����������
    cy_stc_sysint_irq_t                 pwm_in_irq_cfg;                         // �������岶���жϲ�������
    cy_stc_sysint_irq_t                 pwm_in_over_time_irq_cfg;               // �������岶��ʱ�жϲ�������
    /******************************����ֲ�����******************************/
    
    
    /************************����������岶����������************************/
    memset(&pwm_in_pin_cfg, 0 , sizeof(pwm_in_pin_cfg));                        // ������岶�����Ŷ������
    
    pwm_in_pin_cfg.driveMode = CY_GPIO_DM_PULLDOWN;                               // ��������ģʽΪǿ����ģʽ
    pwm_in_pin_cfg.hsiom     = MOTOR_LEFT_PWM_IN_HSIOM;                         // �������Ÿ���Ϊ��ʱ����������
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_LEFT_PWM_IN_PIN), (MOTOR_LEFT_PWM_IN_PIN % 8), &pwm_in_pin_cfg);  // ��ʼ�����岶������
    /************************����������岶����������************************/
    
    
    /***********************����������岶��ʱ��ʱ��***********************/
    // ��ʼ������ͨ��ʱ�� ʹ��16λ��Ƶ����ͨ��0 10��Ƶ ���8Mhzʱ�ӵ���ʱ�� 
    Cy_SysClk_PeriphAssignDivider(MOTOR_LEFT_PWM_IN_CLOCK, CY_SYSCLK_DIV_16_BIT, 0);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, 9); 
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0);
    // ��ʼ������ʱͨ��ʱ�� ʹ��16λ��Ƶ����ͨ��0 10��Ƶ ���8Mhzʱ�ӵ���ʱ�� 
    Cy_SysClk_PeriphAssignDivider(MOTOR_LEFT_PWM_IN_OVERTIME_CLOCK, CY_SYSCLK_DIV_16_BIT, 0);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, 9); 
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0);
    /***********************����������岶��ʱ��ʱ��***********************/

    
    /************************����������岶��ʱ��**************************/
    memset(&pwm_in_counter_cfg, 0, sizeof(pwm_in_counter_cfg));                // ������岶��ʱ���������          
                                                                                
    pwm_in_counter_cfg.period             = 0xFFFF                       ;      // ��ʱ������Ϊ65535                       
    pwm_in_counter_cfg.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_8   ; 	// ʱ��8��Ƶ 8Mhz����ʱ�ӱ�8��ƵΪ1Mhz ��˶�ʱ��������λΪ΢��
    pwm_in_counter_cfg.runMode            = CY_TCPWM_COUNTER_CONTINUOUS  ;      // ��������ģʽ
    pwm_in_counter_cfg.countDirection     = CY_TCPWM_COUNTER_COUNT_UP    ;      // ���ϼ��� 
    pwm_in_counter_cfg.debug_pause        = false                        ;      // �رա�����ʱ��ͣ������
    pwm_in_counter_cfg.compareOrCapture   = CY_TCPWM_COUNTER_MODE_CAPTURE;      // ����ģʽ
    pwm_in_counter_cfg.interruptSources   = CY_TCPWM_INT_NONE            ;      // �Զ����ж���Դ
    pwm_in_counter_cfg.capture0InputMode  = CY_TCPWM_INPUT_FALLING_EDGE  ;	// ����Ĵ���0�����¼��������½���	
    pwm_in_counter_cfg.capture0Input      = 2uL                          ;      // ѡ��ʱ��ͨ���ĸ�������
    pwm_in_counter_cfg.capture1InputMode  = CY_TCPWM_INPUT_RISING_EDGE   ;      // ����Ĵ���1�����¼�������������   
    pwm_in_counter_cfg.capture1Input      = 2uL                          ;      // ѡ��ʱ��ͨ���ĸ�������
    pwm_in_counter_cfg.countInputMode     = CY_TCPWM_INPUT_LEVEL         ;      // ��������Ϊˮƽ�ź�
    pwm_in_counter_cfg.countInput         = 1uL                          ;      // ��������Ϊ�����ߵ�ƽ(ֱ�Ӽ���)
    pwm_in_counter_cfg.trigger0EventCfg   = CY_TCPWM_COUNTER_CC0_MATCH   ;      // �ж��¼�0����Ϊ����Ĵ���0����
    pwm_in_counter_cfg.trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW    ;      // �ж��¼�1����Ϊ���ʱ����
    
    
    Cy_Tcpwm_Counter_Init(MOTOR_LEFT_PWM_IN_TIMER, &pwm_in_counter_cfg);        // ��ʼ����ʱ������
    Cy_Tcpwm_Counter_SetCC0_IntrMask(MOTOR_LEFT_PWM_IN_TIMER);                  // ʹ�ܲ���Ĵ���0�ж�
    Cy_Tcpwm_Counter_SetCC1_IntrMask(MOTOR_LEFT_PWM_IN_TIMER);                  // ʹ�ܲ���Ĵ���1�ж�
    Cy_Tcpwm_Counter_Enable(MOTOR_LEFT_PWM_IN_TIMER);                           // ʹ�ܶ�ʱ��
    Cy_Tcpwm_TriggerStart(MOTOR_LEFT_PWM_IN_TIMER);                             // ������ʱ������
    /************************����������岶��ʱ��**************************/
    
    
    /*************************����������岶���ж�***************************/
    pwm_in_irq_cfg.sysIntSrc  = MOTOR_LEFT_PWM_IN_ISR                    ;      // ѡ���жϴ���ԴΪTCPWM��ʱ��1��ͨ��7
    pwm_in_irq_cfg.intIdx     = CPUIntIdx0_IRQn                          ;      // ѡ���жϴ���ͨ��Ϊ�û��ж�0
    pwm_in_irq_cfg.isEnabled  = true                                     ;      // �����жϴ���
    interrupt_init(&pwm_in_irq_cfg, motor_left_pwm_in_isr, 0)            ;      // �жϳ�ʼ�� ���ȼ� 0
    /*************************����������岶���ж�***************************/

    
    /**********************����������岶��ʱ��ʱ��************************/
    memset(&pwm_in_over_time_cfg, 0, sizeof(pwm_in_over_time_cfg));            // ������岶��ʱ��ʱ���������       
    
    pwm_in_over_time_cfg.period             = 0xFFFF                       ;    // ��ʱ������Ϊ65535                           
    pwm_in_over_time_cfg.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_8   ;    // ʱ��8��Ƶ 8Mhz����ʱ�ӱ�8��ƵΪ1Mhz ��˶�ʱ��������λΪ΢��
    pwm_in_over_time_cfg.runMode            = CY_TCPWM_COUNTER_CONTINUOUS  ;    // ��������ģʽ
    pwm_in_over_time_cfg.countDirection     = CY_TCPWM_COUNTER_COUNT_UP    ;    // ���ϼ��� 
    pwm_in_over_time_cfg.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE;    // �Ƚ�ģʽ
    pwm_in_over_time_cfg.countInputMode     = CY_TCPWM_INPUT_LEVEL         ;    // ѡ��ʱ��ͨ���ĸ�������
    pwm_in_over_time_cfg.countInput         = 1uL                          ;    // ��������Ϊˮƽ�ź�
    pwm_in_over_time_cfg.trigger0EventCfg   = CY_TCPWM_COUNTER_OVERFLOW    ;    // �ж��¼�0����Ϊ���ʱ����
    pwm_in_over_time_cfg.trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW    ;    // �ж��¼�1����Ϊ���ʱ����
                                                                                   
    Cy_Tcpwm_Counter_Init(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER, &pwm_in_over_time_cfg);     // ��ʼ����ʱ������ 
    Cy_Tcpwm_Counter_Enable(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER);                          // ʹ�ܶ�ʱ�� 
    Cy_Tcpwm_TriggerStart(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER);                            // ������ʱ������ 
    Cy_Tcpwm_Counter_SetTC_IntrMask(MOTOR_LEFT_PWM_IN_OVERTIME_TIMER);                  // ʹ�ܼ������ж� 
    /**********************����������岶��ʱ��ʱ��************************/   
    
    
    /***********************����������岶��ʱ�ж�*************************/
    pwm_in_over_time_irq_cfg.sysIntSrc  = MOTOR_LEFT_PWM_IN_OVERTIME_ISR ;      // ѡ���жϴ���ԴΪTCPWM��ʱ��0��ͨ��18
    pwm_in_over_time_irq_cfg.intIdx     = CPUIntIdx0_IRQn                ;      // ѡ���жϴ���ͨ��Ϊ�û��ж�0
    pwm_in_over_time_irq_cfg.isEnabled  = true                           ;      // �����жϴ���
    interrupt_init(&pwm_in_over_time_irq_cfg, motor_left_pwm_in_over_time_isr, 0);      // �жϳ�ʼ�� ���ȼ� 0
    /***********************����������岶��ʱ�ж�*************************/
    
    
    
    
    /************************�����Ҳ����岶����������************************/
    memset(&pwm_in_pin_cfg, 0 , sizeof(pwm_in_pin_cfg));                        // ������岶�����Ŷ������
    
    pwm_in_pin_cfg.driveMode = CY_GPIO_DM_PULLDOWN;                               // ��������ģʽΪǿ����ģʽ
    pwm_in_pin_cfg.hsiom     = MOTOR_RIGHT_PWM_IN_HSIOM;                        // �������Ÿ���Ϊ��ʱ����������
    
    Cy_GPIO_Pin_Init(get_port(MOTOR_RIGHT_PWM_IN_PIN), (MOTOR_RIGHT_PWM_IN_PIN % 8), &pwm_in_pin_cfg);  // ��ʼ�����岶������
    /************************�����Ҳ����岶����������************************/
    
    
    /***********************�����Ҳ����岶��ʱ��ʱ��***********************/
    // ��ʼ������ͨ��ʱ�� ʹ��16λ��Ƶ����ͨ��0 10��Ƶ ���8Mhzʱ�ӵ���ʱ�� 
    Cy_SysClk_PeriphAssignDivider(MOTOR_RIGHT_PWM_IN_CLOCK, CY_SYSCLK_DIV_16_BIT, 0);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, 9); 
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0);
    // ��ʼ������ʱͨ��ʱ�� ʹ��16λ��Ƶ����ͨ��0 10��Ƶ ���8Mhzʱ�ӵ���ʱ�� 
    Cy_SysClk_PeriphAssignDivider(MOTOR_RIGHT_PWM_IN_OVERTIME_CLOCK, CY_SYSCLK_DIV_16_BIT, 0);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, 9); 
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0);
    /***********************�����Ҳ����岶��ʱ��ʱ��***********************/

    
    /************************�����Ҳ����岶��ʱ��**************************/
    memset(&pwm_in_counter_cfg, 0, sizeof(pwm_in_counter_cfg));                // ������岶��ʱ���������          
                                                                                
    pwm_in_counter_cfg.period             = 0xFFFF                       ;      // ��ʱ������Ϊ65535                       
    pwm_in_counter_cfg.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_8   ; 	// ʱ��8��Ƶ 8Mhz����ʱ�ӱ�8��ƵΪ1Mhz ��˶�ʱ��������λΪ΢��
    pwm_in_counter_cfg.runMode            = CY_TCPWM_COUNTER_CONTINUOUS  ;      // ��������ģʽ
    pwm_in_counter_cfg.countDirection     = CY_TCPWM_COUNTER_COUNT_UP    ;      // ���ϼ��� 
    pwm_in_counter_cfg.debug_pause        = false                        ;      // �رա�����ʱ��ͣ������
    pwm_in_counter_cfg.compareOrCapture   = CY_TCPWM_COUNTER_MODE_CAPTURE;      // ����ģʽ
    pwm_in_counter_cfg.interruptSources   = CY_TCPWM_INT_NONE            ;      // �Զ����ж���Դ
    pwm_in_counter_cfg.capture0InputMode  = CY_TCPWM_INPUT_FALLING_EDGE  ;	// ����Ĵ���0�����¼��������½���	
    pwm_in_counter_cfg.capture0Input      = 2uL                          ;      // ѡ��ʱ��ͨ���ĸ�������
    pwm_in_counter_cfg.capture1InputMode  = CY_TCPWM_INPUT_RISING_EDGE   ;      // ����Ĵ���1�����¼�������������   
    pwm_in_counter_cfg.capture1Input      = 2uL                          ;      // ѡ��ʱ��ͨ���ĸ�������
    pwm_in_counter_cfg.countInputMode     = CY_TCPWM_INPUT_LEVEL         ;      // ��������Ϊˮƽ�ź�
    pwm_in_counter_cfg.countInput         = 1uL                          ;      // ��������Ϊ�����ߵ�ƽ(ֱ�Ӽ���)
    pwm_in_counter_cfg.trigger0EventCfg   = CY_TCPWM_COUNTER_CC0_MATCH   ;      // �ж��¼�0����Ϊ����Ĵ���0����
    pwm_in_counter_cfg.trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW    ;      // �ж��¼�1����Ϊ���ʱ����
    
    
    Cy_Tcpwm_Counter_Init(MOTOR_RIGHT_PWM_IN_TIMER, &pwm_in_counter_cfg);       // ��ʼ����ʱ������
    Cy_Tcpwm_Counter_SetCC0_IntrMask(MOTOR_RIGHT_PWM_IN_TIMER);                 // ʹ�ܲ���Ĵ���0�ж�
    Cy_Tcpwm_Counter_SetCC1_IntrMask(MOTOR_RIGHT_PWM_IN_TIMER);                 // ʹ�ܲ���Ĵ���1�ж�
    Cy_Tcpwm_Counter_Enable(MOTOR_RIGHT_PWM_IN_TIMER);                          // ʹ�ܶ�ʱ��
    Cy_Tcpwm_TriggerStart(MOTOR_RIGHT_PWM_IN_TIMER);                            // ������ʱ������
    /************************�����Ҳ����岶��ʱ��**************************/
    
    
    /*************************�����Ҳ����岶���ж�***************************/
    pwm_in_irq_cfg.sysIntSrc  = MOTOR_RIGHT_PWM_IN_ISR                    ;     // ѡ���жϴ���ԴΪTCPWM��ʱ��1��ͨ��7
    pwm_in_irq_cfg.intIdx     = CPUIntIdx0_IRQn                          ;      // ѡ���жϴ���ͨ��Ϊ�û��ж�0
    pwm_in_irq_cfg.isEnabled  = true                                     ;      // �����жϴ���
    interrupt_init(&pwm_in_irq_cfg, motor_right_pwm_in_isr, 0)            ;     // �жϳ�ʼ�� ���ȼ� 0
    /*************************�����Ҳ����岶���ж�***************************/

    
    /**********************�����Ҳ����岶��ʱ��ʱ��************************/
    memset(&pwm_in_over_time_cfg, 0, sizeof(pwm_in_over_time_cfg));            // ������岶��ʱ��ʱ���������       
    
    pwm_in_over_time_cfg.period             = 0xFFFF                       ;    // ��ʱ������Ϊ65535                           
    pwm_in_over_time_cfg.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_8   ;    // ʱ��8��Ƶ 8Mhz����ʱ�ӱ�8��ƵΪ1Mhz ��˶�ʱ��������λΪ΢��
    pwm_in_over_time_cfg.runMode            = CY_TCPWM_COUNTER_CONTINUOUS  ;    // ��������ģʽ
    pwm_in_over_time_cfg.countDirection     = CY_TCPWM_COUNTER_COUNT_UP    ;    // ���ϼ��� 
    pwm_in_over_time_cfg.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE;    // �Ƚ�ģʽ
    pwm_in_over_time_cfg.countInputMode     = CY_TCPWM_INPUT_LEVEL         ;    // ѡ��ʱ��ͨ���ĸ�������
    pwm_in_over_time_cfg.countInput         = 1uL                          ;    // ��������Ϊˮƽ�ź�
    pwm_in_over_time_cfg.trigger0EventCfg   = CY_TCPWM_COUNTER_OVERFLOW    ;    // �ж��¼�0����Ϊ���ʱ����
    pwm_in_over_time_cfg.trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW    ;    // �ж��¼�1����Ϊ���ʱ����
                                                                                   
    Cy_Tcpwm_Counter_Init(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER, &pwm_in_over_time_cfg);    // ��ʼ����ʱ������ 
    Cy_Tcpwm_Counter_Enable(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER);                         // ʹ�ܶ�ʱ�� 
    Cy_Tcpwm_TriggerStart(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER);                           // ������ʱ������ 
    Cy_Tcpwm_Counter_SetTC_IntrMask(MOTOR_RIGHT_PWM_IN_OVERTIME_TIMER);                 // ʹ�ܼ������ж� 
    /**********************�����Ҳ����岶��ʱ��ʱ��************************/   
    
    
    /***********************�����Ҳ����岶��ʱ�ж�*************************/
    pwm_in_over_time_irq_cfg.sysIntSrc  = MOTOR_RIGHT_PWM_IN_OVERTIME_ISR;      // ѡ���жϴ���ԴΪTCPWM��ʱ��0��ͨ��18
    pwm_in_over_time_irq_cfg.intIdx     = CPUIntIdx0_IRQn                ;      // ѡ���жϴ���ͨ��Ϊ�û��ж�0
    pwm_in_over_time_irq_cfg.isEnabled  = true                           ;      // �����жϴ���
    interrupt_init(&pwm_in_over_time_irq_cfg, motor_right_pwm_in_over_time_isr, 0);     // �жϳ�ʼ�� ���ȼ� 0
    /***********************�����Ҳ����岶��ʱ�ж�*************************/
}