#ifndef  _Motor_H
#define  _Motor_H

#define MIDDLE_LINE_MODE    1   //1是不使用权重，2是使用权重

#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"
#include "../pid/Pid.h"
#include "../myEncoder/MyEncoder.h"
#include "../myCamera/MyCamera.h"

#include "small_driver_uart_control.h"

#define MOTOR_PWM_MAX 5000
#define MOTOR_PWM_MIN -5000

void MotorSetPWM(int pwm_left,int pwm_right);

#endif
