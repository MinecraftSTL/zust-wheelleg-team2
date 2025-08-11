#include "MOTOR.H"

//Limit函数
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

float V0 = 400;
/***********************************************
* @brief : PWM输出
* @param : void
* @return: void
* @date  : 2024年10月3日19:58:54
* @author: 钱文杰
************************************************/
void MotorSetPWM(int pwm_left,int pwm_right)
{
    //对输入电机的pwm进行限幅

    if(pwm_left>MOTOR_PWM_MAX)
        pwm_left = MOTOR_PWM_MAX;
    else if(pwm_left<MOTOR_PWM_MIN)
        pwm_left = MOTOR_PWM_MIN;
    if(pwm_right>MOTOR_PWM_MAX)
        pwm_right = MOTOR_PWM_MAX;
    else if(pwm_right<MOTOR_PWM_MIN)
        pwm_right = MOTOR_PWM_MIN;
    //控制电机正反转和转速
    //左电机

//    printf("%d, %d\r\n", pwm_left, pwm_right);
    small_driver_set_duty(-pwm_left, pwm_right);
}
