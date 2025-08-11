/*
 * Leg.c
 *
 *  Created on: 2025Äê1ÔÂ14ÈÕ
 *      Author: minec
 */
#include "Leg.h"

const uint32 pwmF = 300;

void Leg_init(){
    pwm_init(servo_rb, pwmF, PWM_DUTY_MAX/2);
    pwm_init(servo_rf, pwmF, PWM_DUTY_MAX/2);
    pwm_init(servo_lf, pwmF, PWM_DUTY_MAX/2);
    pwm_init(servo_lb, pwmF, PWM_DUTY_MAX/2);
    Leg_set_duty(0,0,0,0);
}

uint32 Radian_toPwmDuty(float rad){
    return (rad/PI+0.5)*PWM_DUTY_MAX;
}

void Leg_set_duty(float rb, float rf, float lf, float lb){
    pwm_set_duty(servo_rb, Radian_toPwmDuty(rb-0.07));
    pwm_set_duty(servo_rf, Radian_toPwmDuty(-rf-0.21));
    pwm_set_duty(servo_lf, Radian_toPwmDuty(lf-0.06));
    pwm_set_duty(servo_lb, Radian_toPwmDuty(-lb-0.24));
}
