/*
���ߣ�Charon and ������С��Ƭ
       δ����Ȩ��ֹת��
 */



#include "sys.h"
float   target_speed   =   240;   //�����ٶ�   205
float   bend_speed     =   260;   //ֱ���ٶ�   240
float   S_speed        =   230;  //S���ٶ�    200
float   annulus_speed  =   210;   //�����ٶ�   170
float   hill_speed     =   700 ;  //�����ٶ�


#define   ws_speed          700   //��ˢ�����ٶ�  700


float   S_number       =   0.03;   //S��ƫ�����   0.2
float   target_number  =  0.03;  //����ƫ�����   0.45
PID motor_l = {
        0,0,
        -1000,1000,
        -2000,2000,
        12.1,  //65    //50    //50
        0.7,  //2.2    //2     //8
        1.4 // 70    //80    //60
};
PID motor_r = {
        0,0,
        -1000,1000,
        -2000,2000,
        12.1,  //65    //50    //50
        0.7,  //2.2    //2     //8
        1.4 // 70    //80    //60
};
PID motor_turn = {
        0,0,
        -1000,1000,
        -1000,1000,
        1.,  //65    //50    //50
        0,  //2.2    //2     //8
        0.1 // 70    //80    //60
};
//-------------------------------------------------------------------------------------------------------------------
// �������     ������
//-------------------------------------------------------------------------------------------------------------------
void PWM_motor(float motor_1,float motor_2)
{
    //����ռ�ձ�
    if(motor_2>=0){
        pwm_set_duty(PWM_R2, motor_2);
        pwm_set_duty(PWM_R1, 0);
    }
    else
    {
        pwm_set_duty(PWM_R2, 0);
        pwm_set_duty(PWM_R1, -motor_2);
    }

//����ռ�ձȸ�ֵ
    if(motor_1>=0){
        pwm_set_duty(PWM_L2, 0);
        pwm_set_duty(PWM_L1, motor_1);
    }
    else
    {
        pwm_set_duty(PWM_L2, -motor_1);
        pwm_set_duty(PWM_L1, 0);
    }
}

float Motor_PID(PID *pid, float target_val, float actual_val){
    //    motor_l.pid_target_val=200;//debug test
    float pid_err=target_val-actual_val;
    pid->pid_sum+=pid_err;
    pid->pid_sum=func_limit_ab(pid->pid_sum, pid->pid_sum_min, pid->pid_sum_max);

    float pid_out=motor_l.pid_Kp*pid_err +
                    motor_l.pid_Ki*(pid->pid_sum) +
                    motor_l.pid_Kd*(actual_val-pid->pid_actual_val_);
    pid->pid_actual_val_ = actual_val;
    pid_out=func_limit_ab(pid_out, pid->pid_out_min, pid->pid_out_max);

    return pid_out;
}

float Motor_l_PID(float actual_val, float turn)
{
    return Motor_PID(&motor_l, motor_speed_choose()-turn, actual_val);
}

float Motor_r_PID(float actual_val, float turn)
{
    return Motor_PID(&motor_r, motor_speed_choose()+turn, actual_val);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����ʼ��
//-------------------------------------------------------------------------------------------------------------------
void motor_init(void)
{
    pwm_init(PWM_R2, 7000,0);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_R1, 7000,0);                                                  // PWM ͨ����ʼ��Ƶ�� 7KHz ռ�ձȳ�ʼΪ 0

    pwm_init(PWM_L2, 7000,0);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_L1, 7000,0);
}
//����·��ѡ���ٶ�
float motor_speed_choose(void)
{
    float pid_target_val;
    if(xunxian==0||tingche_flag==1){
        pid_target_val=0;
       // pwm_set_duty(WS_PWM_L,700);                // ����ռ�ձ�
    }
    else if(annulus_L_Flag==1)
    {
        pid_target_val=annulus_speed;//-(float)servo.pid_err*target_number;//
          //  pwm_set_duty(WS_PWM_L,700);
    }
    else if(annulus_R_Flag==1)
    {
        pid_target_val=annulus_speed;//-(float)servo.pid_err*target_number;//
          //  pwm_set_duty(WS_PWM_L,700);
    }

    else if(bend_straight_flag==1)
    {
        pid_target_val=bend_speed;
     // pwm_set_duty(WS_PWM_L,700);

    }
    else if  (S_road_Flag==1)
    {
         pid_target_val=S_speed;//-(float)servo.pid_err*S_number;//
        // pwm_set_duty(WS_PWM_L,700);
    }
    else if(hill_flag==1)
    {
        pid_target_val=hill_speed;
    }
    else
    {
        pid_target_val=target_speed;//-(float)servo.pid_err*target_number;//
        //pwm_set_duty(WS_PWM_L,700);
    }
    pid_target_val=50;//debug test
    return pid_target_val;
}
