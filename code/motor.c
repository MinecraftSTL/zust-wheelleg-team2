/*
作者：Charon and 快乐牌小刀片
       未经授权禁止转售
 */

#include "sys.h"

float   target_speed   =   240;   //基础速度   205
float   bend_speed     =   260;   //直线速度   240
float   S_speed        =   230;  //S弯速度    200
float   annulus_speed  =   210;   //环岛速度   170
float   hill_speed     =   700 ;  //上坡速度

float straight_Kp=1.8;
float straight_Kd=2.40;
float bend_Kp=2.30;
float bend_Kd=2.4;
float arc_Kp=2.56;
float arc_Kd=1.74;

float   S_number       =   0.03;   //S弯偏差基数   0.2
float   target_number  =  0.03;  //基础偏差基数   0.45
struct PID motor_l = {
        0,0,0,
        -2000,2000,
        -2000,2000,
        0,
        12.1,  //65    //50    //50
        0.7,  //2.2    //2     //8
        70, // 70    //80    //60
};
struct PID motor_r = {
        0,0,0,
        -2000,2000,
        -2000,2000,
        0,
        12.1,  //65    //50    //50
        0.7,  //2.2    //2     //8
        70, // 70    //80    //60
};
struct PID motor_turn = {
        0,0,0,
        -1000,1000,
        -1200,1200,
        0,
        0, //1.5
        0,
        0, //0.5 //0.8
};
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机输出
//-------------------------------------------------------------------------------------------------------------------
void PWM_motor(float motor_1,float motor_2)
{
    //右轮占空比
    if(motor_2>=0){
        pwm_set_duty(PWM_R2, 0);
        pwm_set_duty(PWM_R1, motor_2);
    }
    else
    {
        pwm_set_duty(PWM_R2, -motor_2);
        pwm_set_duty(PWM_R1, 0);
    }

//左轮占空比赋值
    if(motor_1>=0){
        pwm_set_duty(PWM_L2, motor_1);
        pwm_set_duty(PWM_L1, 0);
    }
    else
    {
        pwm_set_duty(PWM_L2, 0);
        pwm_set_duty(PWM_L1, -motor_1);
    }
}

float Motor_PID(struct PID *pid, float target_val, float actual_val){
    float err=target_val-actual_val;
    pid->sum+=err;
    pid->sum=func_limit_ab(pid->sum, pid->sum_min, pid->sum_max);

    float out=pid->Kp*err +
                pid->Ki*pid->sum +
                pid->Kd*(err-pid->err_) +
                pid->Kdd*(err-2*pid->err_+pid->err__);
    pid->err__ = pid->err_;
    pid->err_ = err;

    out=func_limit_ab(out, pid->out_min, pid->out_max);

    return out;
}

float Motor_l_PID(float actual_val, float turn)
{
    return Motor_PID(&motor_l, motor_speed_choose(motor_turn.err_)*(1-turn*1e-2), actual_val);
}

float Motor_r_PID(float actual_val, float turn)
{
    return Motor_PID(&motor_r, motor_speed_choose(motor_turn.err_)*(1+turn*1e-2), actual_val);
}
void Motor_t_PID(float actual_val, float *ret){// idk why this cant return
    turn_pd_choose(&motor_turn);
    *ret = Motor_PID(&motor_turn, 0, actual_val);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机初始化
//-------------------------------------------------------------------------------------------------------------------
void motor_init(void)
{
    pwm_init(PWM_R2, 7000,0);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R1, 7000,0);                                                  // PWM 通道初始化频率 7KHz 占空比初始为 0

    pwm_init(PWM_L2, 7000,0);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L1, 7000,0);
}
//根据路况选择速度
float motor_speed_choose(float turn_err)
{
    float pid_target_val;
    if(xunxian==0||tingche_flag==1){
        pid_target_val=0;
    }
    else if(annulus_L_Flag==1)
    {
        pid_target_val=annulus_speed-(float)turn_err*target_number;
    }
    else if(annulus_R_Flag==1)
    {
        pid_target_val=annulus_speed-(float)turn_err*target_number;
    }

    else if(bend_straight_flag==1)
    {
        pid_target_val=bend_speed;

    }
    else if  (S_road_Flag==1)
    {
         pid_target_val=S_speed-(float)turn_err*S_number;
    }
    else if(hill_flag==1)
    {
        pid_target_val=hill_speed;
    }
    else
    {
        pid_target_val=target_speed-(float)turn_err*target_number;
    }
//    pid_target_val=20;//debug test
    return pid_target_val;
}

void turn_pd_choose(struct PID *PID_turn){
    if((annulus_L_Flag==0||annulus_R_Flag==0)||bend_straight_flag==1){
        PID_turn->Kp=straight_Kp;//1.2
        PID_turn->Kd=straight_Kd;//0.5
    }
    else
    {
        PID_turn->Kp=bend_Kp;//1.5   //1.3
        PID_turn->Kd=bend_Kd;//0.8     //2
    }
    if(annulus_L_memory>=1||annulus_R_memory>=1)
    {
        PID_turn->Kp=arc_Kp;//1.2
        PID_turn->Kd=arc_Kd;//0.5
    }
    PID_turn->Kp=straight_Kp;//debug test
    PID_turn->Kd=straight_Kd;//debug test
}
