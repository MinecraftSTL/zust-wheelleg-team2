#include "MOTOR.H"

float targetV = 600;

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
    pwm_left = -pwm_left;
    pwm_left = func_limit_ab(pwm_left, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
    pwm_right = func_limit_ab(pwm_right, MOTOR_PWM_MIN, MOTOR_PWM_MAX);
    //控制电机正反转和转速
    //左电机

//    printf("%d, %d\r\n", pwm_left, pwm_right);
    small_driver_set_duty(pwm_left, pwm_right);
}
