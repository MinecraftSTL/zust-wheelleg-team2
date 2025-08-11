/*
作者：Charon and 快乐牌小刀片
       未经授权禁止转售
 */

#include "sys.h"

const uint16 speed_limit = 3000;

float Kl = 0.09;//0.12;//因为驱动不平衡,根据坡道情况调

float target_speed   = 50;//240;   //基础速度   205
float straight_speed = 70;//260;   //直线速度   240
float S_speed        = 50;//230;  //S弯速度    200
float annulus_speed  = 50;//210;   //环岛速度   170
float hill_speed     = 200;//700;  //上坡速度

float straight_Kp=0.3;
float straight_Kd=10;
float bend_Kp=0.7;
float bend_Kd=20;
float arc_Kp=2.5;
float arc_Kd=0;

int debug_forceSpeedEn = 1;
float debug_forceSpeed = 40;
int debug_forceTurnEn = 1;
float debug_forceTurnKp = 1.8;
float debug_forceTurnKd = 60;

float   S_number       =   0.03;   //S弯偏差基数   0.2
float   target_number  =  0.03;  //基础偏差基数   0.45
struct PID motor_l = {
        0,0,
        -10000,10000,
        -2000,2000,
        50,  //65    //50    //50
        2,  //2.2    //2     //8
        60, // 70    //80    //60
};
struct PID motor_r = {
        0,0,
        -10000,10000,
        -2000,2000,
        65,  //65    //50    //50
        8,  //2.2    //2     //8
        80, // 70    //80    //60
};
struct PID motor_turn = {
        0,0,
        -1000,1000,
        -1200,1200,
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
                pid->Kd*(err-pid->err_);
    pid->err_ = err;

    out=func_limit_ab(out, pid->out_min, pid->out_max);

    return out;
}

float Motor_l_PID(float actual_val, float turn)
{
    float i;
    i = Motor_PID(&motor_l, motor_speed_choose(motor_turn.err_)*(1-turn*1e-2), actual_val);
    return i;
//    return Motor_PID(&motor_l, motor_speed_choose(motor_turn.err_)-turn, actual_val);
}

float Motor_r_PID(float actual_val, float turn)
{
    return Motor_PID(&motor_r, motor_speed_choose(motor_turn.err_)*(1+turn*1e-2), actual_val);
//    return Motor_PID(&motor_r, motor_speed_choose(motor_turn.err_)+turn, actual_val);
}
void Motor_t_PID(float actual_val, float *ret){// idk why this cant use return
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
    if(!xunxian||tingche_flag){
        pid_target_val=0;
        return pid_target_val;
    }
    else if(annulus_L_Flag||annulus_R_Flag)
    {
        pid_target_val=annulus_speed-(float)turn_err*target_number;
    }
    else if(straight_flag)
    {
        pid_target_val=straight_speed;
    }
    else if(S_road_Flag)
    {
         pid_target_val=S_speed-(float)turn_err*S_number;
//    }else if(hill_flag){
//        pid_target_val=hill_speed;
    }
    else
    {
        pid_target_val=target_speed-(float)turn_err*target_number;
    }
    if(debug_forceSpeedEn){
        pid_target_val=debug_forceSpeed;
    }
    return pid_target_val;
}

void turn_pd_choose(struct PID *PID_turn){
    if(straight_flag){
        PID_turn->Kp=straight_Kp;//1.2
        PID_turn->Kd=straight_Kd;//0.5
    }else{
        PID_turn->Kp=bend_Kp;//1.5   //1.3
        PID_turn->Kd=bend_Kd;//0.8     //2
    }
    if(annulus_L_Flag||annulus_R_Flag){
        PID_turn->Kp=arc_Kp;//1.2
        PID_turn->Kd=arc_Kd;//0.5
    }
    if(debug_forceTurnEn){
        PID_turn->Kp=debug_forceTurnKp;
        PID_turn->Kd=debug_forceTurnKd;
    }
}
