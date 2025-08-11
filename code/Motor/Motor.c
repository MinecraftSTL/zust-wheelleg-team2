#include "MOTOR.H"

//Limit����
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

float V0 = 400;
/***********************************************
* @brief : PWM���
* @param : void
* @return: void
* @date  : 2024��10��3��19:58:54
* @author: Ǯ�Ľ�
************************************************/
void MotorSetPWM(int pwm_left,int pwm_right)
{
    //����������pwm�����޷�

    if(pwm_left>MOTOR_PWM_MAX)
        pwm_left = MOTOR_PWM_MAX;
    else if(pwm_left<MOTOR_PWM_MIN)
        pwm_left = MOTOR_PWM_MIN;
    if(pwm_right>MOTOR_PWM_MAX)
        pwm_right = MOTOR_PWM_MAX;
    else if(pwm_right<MOTOR_PWM_MIN)
        pwm_right = MOTOR_PWM_MIN;
    //���Ƶ������ת��ת��
    //����

//    printf("%d, %d\r\n", pwm_left, pwm_right);
    small_driver_set_duty(-pwm_left, pwm_right);
}
