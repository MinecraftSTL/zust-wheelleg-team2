#ifndef  _Motor_H
#define  _Motor_H

#define MIDDLE_LINE_MODE    1   //1�ǲ�ʹ��Ȩ�أ�2��ʹ��Ȩ��

#include "PIN.H"
#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"
#include "PID.h"
#include "MyEncoder.h"
#include "MyCamera.h"

#define MOTOR_PWM_MAX 9900
#define MOTOR_PWM_MIN -8000

extern PID Motor_Speed_PID_Left;
extern PID Motor_Speed_PID_Right;
extern PID Turn_Speed_PID;
extern PID Straight_Speed_PID;

extern int target_left,target_right;
extern float V0 ;
extern float basic_V0;
extern int pwm_left, pwm_right;

// �������     ˫���޷� ���ݷ�Χ�� [-32768,32767]
// ����˵��     x               ���޷�������
// ����˵��     a               �޷���Χ��߽�
// ����˵��     b               �޷���Χ�ұ߽�
// ���ز���     int             �޷�֮�������
// ʹ��ʾ��     int dat = func_limit_ab(500, -300, 400);        //���ݱ�������-300��+400֮��  ��˷��صĽ����400
// ��ע��Ϣ
#define     func_limit_ab(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

void Motor_Init(void);
void MotorSetPWM(int pwm_left,int pwm_right);
void MotorCtrl(void);
void Turn_Ctrl(void);
void Motor_Stop(void);

#endif
