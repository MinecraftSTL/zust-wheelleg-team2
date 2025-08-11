/*
作者：Charon and 快乐牌小刀片
       未经授权禁止转售
 */


#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_


//=====================================================宏定义====================================================
//有刷电机有几个端口，貌似有些端口有问题，如果有问题可换个电机端口
#define PWM_R2               (ATOM1_CH4_P02_4)
#define PWM_R1               (ATOM1_CH5_P02_5)
#define PWM_L2            (ATOM1_CH6_P02_6)
#define PWM_L1             (ATOM1_CH7_P02_7)

//#define DIR_R               (P02_4)
//#define PWM_R               (ATOM0_CH5_P02_5)
//#define DIR_L               (P02_6)
//#define PWM_L               (ATOM0_CH7_P02_7)

extern float   target_speed   ;   //基础速度   205
extern float   bend_speed     ;   //直线速度   240
extern float   S_speed        ;  //S弯速度    200
extern float   annulus_speed  ;   //环岛速度   170
extern float   hill_speed     ;   //上坡速度

//extern float   S_number       ;   //S弯偏差基数   0.2
//extern float   target_number  ;  //基础偏差基数   0.45

//PID参数
typedef struct{
    float pid_actual_val_;  //上次值
    float pid_sum;          //积分缓存
    float pid_sum_min;
    float pid_sum_max;
    float pid_out_min;
    float pid_out_max;
    float pid_Kp,pid_Ki,pid_Kd;
}PID;


float Motor_l_PID(float actual_val );
float Motor_r_PID(float actual_val );
void PWM_motor(float motor_1,float motor_2);
void motor_init(void);
float motor_speed_choose(void);
void motor_r_speed_choose(void);
void motor_l_speed_choose(void);

extern PID motor_l;
extern PID motor_r;

#endif /* CODE_MOTOR_H_ */
