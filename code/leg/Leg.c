/*
 * Leg.c
 *
 *  Created on: 2025年1月14日
 *      Author: minec
 */
#include "Leg.h"

const uint32 freq = 300;

const float defaultLegX = 0, defaultLegZ = -30;
float targetLegX, targetLegZ;

float defaultRollAlpha = 0.02;

uint8 rollBalance = 0;

void Leg_init(){
    pwm_init(servo_rb, freq, PWM_DUTY_MAX/2);
    pwm_init(servo_rf, freq, PWM_DUTY_MAX/2);
    pwm_init(servo_lf, freq, PWM_DUTY_MAX/2);
    pwm_init(servo_lb, freq, PWM_DUTY_MAX/2);
    Leg_set_duty(0,0,0,0);

    targetLegX = defaultLegX;
    targetLegZ = defaultLegZ;
}

struct LegServoAngle Pos_toServoAngle(float x, float z){
    struct LegServoAngle ret;

    float DBuW = hypot(DFB/2-x,z);
    float ABBuW = acosf((DBuW*DBuW+LBU*LBU-LBD*LBD)/(2*DBuW*LBU));
    float AWBuFu = atan2f(-z, (DFB/2-x));
    ret.b = PI - (ABBuW+AWBuFu);
//    printf("%d, %d, %d, %lf\n",-z, (DFB/2-x), -z/(DFB/2-x), AWBuFu);
//    printf("%lf, %lf, %lf, %lf\n",DBuW, ABBuW, AWBuFu, ret.b);

    float DFuW = hypot(DFB/2+x,z);
    float AFFuW = acosf((DFuW*DFuW+LFU*LFU-LFD*LFD)/(2*DFuW*LFU));
    float AWFuBu = atan2f(-z, (DFB/2+x));
    ret.f = PI - (AFFuW+AWFuBu);

    return ret;
}

void Pos_limit(float *x, float *z){
    *x = func_limit(*x, LEG_MAX_X);
    *z = func_limit_ab(*z, -LEG_MAX_Z, -LEG_MIN_Z);
}
void Servo_limit(float *a){
    *a = func_limit_ab(*a, -1.57, 1.57);
}

float Roll_toPosZ(float roll, float lza){
    float AGC = PI-roll;
    float AWG = atan2f(lza, DLR)- AGC;
    return DLR*tanf(AWG);
}

uint32 Radian_toPwmDuty(float rad){
    return PWM_DUTY_MAX/(1000/freq)*(1+rad/PI/PI*5.6);
}

void Leg_set_zero(){
    pwm_set_duty(servo_rb, Radian_toPwmDuty(0));
    pwm_set_duty(servo_rf, Radian_toPwmDuty(0));
    pwm_set_duty(servo_lf, Radian_toPwmDuty(0));
    pwm_set_duty(servo_lb, Radian_toPwmDuty(0));
}
void Leg_set_duty(float rb, float rf, float lf, float lb){
    Servo_limit(&rb);
    Servo_limit(&rf);
    Servo_limit(&lf);
    Servo_limit(&lb);
    pwm_set_duty(servo_rb, Radian_toPwmDuty(rb+0.8));
    pwm_set_duty(servo_rf, Radian_toPwmDuty(-rf+0.5));
    pwm_set_duty(servo_lf, Radian_toPwmDuty(lf-0.1));
    pwm_set_duty(servo_lb, Radian_toPwmDuty(-lb));
}

void Leg_set_pos(float lx, float lz, float rx, float rz){
    Pos_limit(&lx, &lz);
    Pos_limit(&rx, &rz);
    struct LegServoAngle l = Pos_toServoAngle(lx, lz);
    struct LegServoAngle r = Pos_toServoAngle(rx, rz);
//    printf("%lf, %lf, %lf, %lf\n",r.b, r.f, l.b, l.f);
    Leg_set_duty(r.b, r.f, l.f, l.b);
}

const uint32 jumpStep[] = {
    140,//伸腿
    100,//收腿
    0,
};

uint32 jumpTime = 32767;

void jumpPit(uint32 period, float *legZ){
    Step step = getStep(jumpStep, jumpTime);
    switch(step.step){
        case 0:
            *legZ = -140;
            break;
        case 1:
            *legZ = -30;
            break;
        default:
            return;
    }
    jumpTime+=period;
}
void jump(){
    Step step = getStep(jumpStep, jumpTime);
    if(step.step >= 2){
        jumpTime = 0;
    }
}
