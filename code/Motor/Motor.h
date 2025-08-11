#ifndef  _Motor_H
#define  _Motor_H

#define MIDDLE_LINE_MODE    1   //1是不使用权重，2是使用权重

#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"
#include "PID.h"
#include "MyEncoder.h"
#include "MyCamera.h"

#include "small_driver_uart_control.h"

#define MOTOR_PWM_MAX 5000
#define MOTOR_PWM_MIN -5000

extern PID Motor_Speed_PID_Left;
extern PID Motor_Speed_PID_Right;
extern PID Turn_Speed_PID;
extern PID Straight_Speed_PID;

extern int target_left,target_right;
extern float V0 ;
extern float basic_V0;

// 函数简介     双边限幅 数据范围是 [-32768,32767]
// 参数说明     x               被限幅的数据
// 参数说明     a               限幅范围左边界
// 参数说明     b               限幅范围右边界
// 返回参数     int             限幅之后的数据
// 使用示例     int dat = func_limit_ab(500, -300, 400);        //数据被限制在-300至+400之间  因此返回的结果是400
// 备注信息
#define     func_limit_ab(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

void MotorSetPWM(int pwm_left,int pwm_right);

#endif
