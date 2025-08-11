#ifndef  _Motor_H
#define  _Motor_H

#define MIDDLE_LINE_MODE    1   //1�ǲ�ʹ��Ȩ�أ�2��ʹ��Ȩ��

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
