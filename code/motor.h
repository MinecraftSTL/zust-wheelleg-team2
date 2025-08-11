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

extern float target_speed   ;   //基础速度   205
extern float bend_speed     ;   //直线速度   240
extern float S_speed        ;  //S弯速度    200
extern float annulus_speed  ;   //环岛速度   170
extern float hill_speed     ;   //上坡速度

extern float straight_Kp;
extern float straight_Kd;
extern float bend_Kp;
extern float bend_Kd;
extern float arc_Kp;
extern float arc_Kd;
extern uint8 annulus_L_memory, annulus_R_memory;

//PID参数
struct PID{
    float err_;
    float sum;
    float sum_min;
    float sum_max;
    float out_min;
    float out_max;
    float Kp,Ki,Kd;
};


float Motor_l_PID(float actual_val, float turn);
float Motor_r_PID(float actual_val, float turn);
void PWM_motor(float motor_1,float motor_2);
void motor_init(void);
float motor_speed_choose(float turn_err);
void turn_pd_choose(struct PID *PID_turn);

extern struct PID motor_l;
extern struct PID motor_r;
extern struct PID motor_turn;

#endif /* CODE_MOTOR_H_ */
