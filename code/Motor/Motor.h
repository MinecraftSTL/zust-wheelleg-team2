#ifndef  _Motor_H
#define  _Motor_H

#define MIDDLE_LINE_MODE    1   //1是不使用权重，2是使用权重

#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"
#include "Pid.h"
#include "MyEncoder.h"
#include "MyCamera.h"

#include "small_driver_uart_control.h"

#define MOTOR_PWM_MAX 5000
#define MOTOR_PWM_MIN -5000

extern float targetV;

extern uint8 Motor_zero;

extern uint8 Motor_dirL, Motor_dirR;

void MotorSetPWM(int16 pwm_left,int16 pwm_right);
void MotorZero();
void MotorPit();

#endif
