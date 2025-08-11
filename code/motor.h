/*
���ߣ�Charon and ������С��Ƭ
       δ����Ȩ��ֹת��
 */


#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_


//=====================================================�궨��====================================================
//��ˢ����м����˿ڣ�ò����Щ�˿������⣬���������ɻ�������˿�
#define PWM_R2               (ATOM1_CH4_P02_4)
#define PWM_R1               (ATOM1_CH5_P02_5)
#define PWM_L2            (ATOM1_CH6_P02_6)
#define PWM_L1             (ATOM1_CH7_P02_7)

//#define DIR_R               (P02_4)
//#define PWM_R               (ATOM0_CH5_P02_5)
//#define DIR_L               (P02_6)
//#define PWM_L               (ATOM0_CH7_P02_7)

extern float   target_speed   ;   //�����ٶ�   205
extern float   bend_speed     ;   //ֱ���ٶ�   240
extern float   S_speed        ;  //S���ٶ�    200
extern float   annulus_speed  ;   //�����ٶ�   170
extern float   hill_speed     ;   //�����ٶ�

//extern float   S_number       ;   //S��ƫ�����   0.2
//extern float   target_number  ;  //����ƫ�����   0.45

//PID����
typedef struct{
    float pid_actual_val_;  //�ϴ�ֵ
    float pid_sum;          //���ֻ���
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
