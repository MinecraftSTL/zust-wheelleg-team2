#include "MOTOR.H"

//Limit函数
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

PID Motor_Speed_PID_Left;
PID Motor_Speed_PID_Right;
PID Turn_Speed_PID;
PID Straight_Speed_PID;

int target_left = 0,target_right = 0;          //左右轮的目标速度的值

float V0 = 0;
//float turn_V0 = 70;
//float straight_V0 = 100;
float basic_V0 = 120;
int pwm_left = 0,pwm_right = 0;

/***********************************************
* @brief : 电机初始化
* @param : void
* @return: void
* @date  : 2024年10月3日19:54:24
* @author: 钱文杰
************************************************/
void Motor_Init(void)
{
    pwm_init(MOTOR_LEFT_1,12500,0);                   //初始化左电机
    pwm_init(MOTOR_LEFT_2,12500,0);
    pwm_init(MOTOR_RIGHT_1,12500,0);                  //初始化右电机
    pwm_init(MOTOR_RIGHT_2,12500,0);
}
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

//    pwm_left = 1000;
//    pwm_right = 1000;
    if(pwm_left >= 0)
    {
        pwm_set_duty(MOTOR_LEFT_1,pwm_left);
        pwm_set_duty(MOTOR_LEFT_2,0);
    }
    else
    {
        pwm_set_duty(MOTOR_LEFT_1,0);
        pwm_set_duty(MOTOR_LEFT_2,-pwm_left);
    }
    //右电机
    if(pwm_right >= 0)
    {
        pwm_set_duty(MOTOR_RIGHT_1,pwm_right);
        pwm_set_duty(MOTOR_RIGHT_2,0);
    }
    else
    {
        pwm_set_duty(MOTOR_RIGHT_1,0);
        pwm_set_duty(MOTOR_RIGHT_2,-pwm_right);
    }
}
