/*
 * Leg.c
 *
 *  Created on: 2025��1��14��
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
    *x = func_limit(*x, LEG_MAX_X);
    *z = func_limit_ab(*z, -LEG_MAX_Z, -LEG_MIN_Z);
}
void Servo_limit(float *a){
    *a = func_limit_ab(*a, -PI/2, PI/2);
}

int32 Roll_toPosZ(float roll, float lza){
    float AGC = PI-roll;
    float AWG = atan2f(lza, DLR)- AGC;
    return DLR*tanf(AWG);
}

uint32 Radian_toPwmDuty(float rad){
    return PWM_DUTY_MAX/(1000/freq)*(1+rad/PI/PI*5.6);
}

void Leg_set_duty(float rb, float rf, float lf, float lb){
    Servo_limit(&rb);
    Servo_limit(&rf);
    Servo_limit(&lf);
    Servo_limit(&lb);
    pwm_set_duty(servo_rb, Radian_toPwmDuty(rb+0.6));
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

const uint32 preRetractLegTime = 2000;
const float preRetractLegZ = -30;
const uint32 extendLegTime = 200;
const float extendLegZ = -130;
const uint32 retractLegTime = 200;
const float retractLegZ = -30;

uint32 jumpTime = 0;

void jumpPit(uint32 period, float *legZ){
    if(jumpTime){
        ++jumpTime;
        if(jumpTime*period <= preRetractLegTime){
            *legZ = preRetractLegZ;
        }else if(jumpTime*period <= preRetractLegTime+extendLegTime){
            *legZ = extendLegZ;
        }else if(jumpTime*period <= preRetractLegTime+extendLegTime+retractLegTime){
            *legZ = retractLegZ;
        }else{
            jumpTime = 0;
        }
    }
}
void jump(){
    if(!jumpTime){
        jumpTime = 1;
    }
}
