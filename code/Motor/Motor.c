#include "MOTOR.H"

float targetV = 600;

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
    pwm_left = -pwm_left;
    pwm_left = func_limit_ab(pwm_left, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
    pwm_right = func_limit_ab(pwm_right, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
    //���Ƶ������ת��ת��
    //����

//    printf("%d, %d\r\n", pwm_left, pwm_right);
    small_driver_set_duty(pwm_left, pwm_right);
}
