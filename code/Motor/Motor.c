#include "Motor.h"

float targetV = 800;

uint8 Motor_dirL = 1, Motor_dirR = 0;

int16 motorPwmLeft = 0, motorPwmRight = 0;

uint8 zero = 0;

/***********************************************
* @brief : PWM���
* @param : void
* @return: void
* @date  : 2024��10��3��19:58:54
* @author: Ǯ�Ľ�
************************************************/
void MotorSetPWM(int16 pwm_left,int16 pwm_right)
{
    //����������pwm�����޷�
    if(Motor_dirL){
        pwm_left = -pwm_left;
    }
    if(Motor_dirR){
        pwm_right = -pwm_right;
    }
    pwm_left = func_limit_ab(pwm_left, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
    pwm_right = func_limit_ab(pwm_right, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
    //���Ƶ������ת��ת��
    //����

//    printf("%d, %d\r\n", pwm_left, pwm_right);
    motorPwmLeft = pwm_right;
    motorPwmRight = pwm_left;
}

void MotorZero(){
    zero = 1;
}

void MotorPit(){
    small_driver_set_duty(motorPwmLeft, motorPwmRight);
}
