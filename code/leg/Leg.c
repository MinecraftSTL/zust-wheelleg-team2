/*
 * Leg.c
 *
 *  Created on: 2025Äê1ÔÂ14ÈÕ
 *      Author: minec
 */
#include "Leg.h"

const uint32 freq = 300;

void Leg_init(){
    pwm_init(servo_rb, freq, PWM_DUTY_MAX/2);
    pwm_init(servo_rf, freq, PWM_DUTY_MAX/2);
    pwm_init(servo_lf, freq, PWM_DUTY_MAX/2);
    pwm_init(servo_lb, freq, PWM_DUTY_MAX/2);
    Leg_set_duty(0,0,0,0);
}

struct LegServoAngle Pos_toServoAngle(float x, float z){
    struct LegServoAngle ret;

    float DBuW = hypot(DFB/2-x,z);
    float ABBuW = acosf((DBuW*DBuW+LBU*LBU-LBD*LBD)/(2*DBuW*LBU));
    float AWBuFu = atan2f((float)-z, (DFB/2-x));
    ret.b = PI - (ABBuW+AWBuFu);
//    printf("%d, %d, %d, %lf\n",-z, (DFB/2-x), -z/(DFB/2-x), AWBuFu);
//    printf("%lf, %lf, %lf, %lf\n",DBuW, ABBuW, AWBuFu, ret.b);

    float DFuW = hypot(DFB/2+x,z);
    float AFFuW = acosf((DFuW*DFuW+LFU*LFU-LFD*LFD)/(2*DFuW*LFU));
    float AWFuBu = atan2f((float)-z, (DFB/2+x));
    ret.f = PI - (AFFuW+AWFuBu);

    return ret;
}

void Pos_limit(float *x, float *z){
    if(*x < -LEG_MAX_X){
        *x = -LEG_MAX_X;
    }
    if(*x > LEG_MAX_X){
        *x = LEG_MAX_X;
    }
    if(-*z < LEG_MIN_Z){
        *z = -LEG_MIN_Z;
    }
    if(-*z > LEG_MAX_Z){
        *z = -LEG_MAX_Z;
    }
}

int32 Roll_toPosZ(float roll, float lza){
    float AGC = PI-roll;
    float AWG = atan2(lza, DLR)- AGC;
    return DLR*tan(AWG);
}

uint32 Radian_toPwmDuty(float rad){
    return PWM_DUTY_MAX/(1000/freq)*(1+rad/PI/PI*5.6);
}

void Leg_set_duty(float rb, float rf, float lf, float lb){
    pwm_set_duty(servo_rb, Radian_toPwmDuty(rb+0.8));
    pwm_set_duty(servo_rf, Radian_toPwmDuty(-rf+0.5));
    pwm_set_duty(servo_lf, Radian_toPwmDuty(lf+0.7));
    pwm_set_duty(servo_lb, Radian_toPwmDuty(-lb+0.5));
}

void Leg_set_pos(float lx, float lz, float rx, float rz){
    Pos_limit(&lx, &lz);
    Pos_limit(&rx, &rz);
    struct LegServoAngle l = Pos_toServoAngle(lx, lz);
    struct LegServoAngle r = Pos_toServoAngle(rx, rz);
//    printf("%lf, %lf, %lf, %lf\n",r.b, r.f, l.b, l.f);
    Leg_set_duty(r.b, r.f, l.f, l.b);
}
