/*
 * Leg.c
 *
 *  Created on: 2025年1月14日
 *      Author: MinecraftSTL
 */
#include "Leg.h"

const uint32 freq = 300;

const float Leg_angle0Z = -45;

const float defaultLegX = 0, defaultLegZ = -30;
float targetLegX, targetLegZ;

uint8 rollBalance = 0;
float rollBalanceK = 1;

float LegDRb = 0.65, LegDRf = -0.675, LegDLf = 0.65, LegDLb = -0.575;

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
    *a = func_limit(*a, LEG_MAX_R);
}

float Roll_toPosZ(float roll, float lza){
    float AGC = PI-roll;
    float AWG = atan2f(lza, DLR)- AGC;
    return DLR*tanf(AWG);
}

uint32 Radian_toPwmDuty(float rad){
    return PWM_DUTY_MAX/(1000/freq)*(1+rad/PI/PI*5.6);
}

void Leg_set_duty(float rb, float rf, float lf, float lb){
    if(!fLz){
        Servo_limit(&rb);
        Servo_limit(&rf);
        Servo_limit(&lf);
        Servo_limit(&lb);
        rb += LegDRb;
        rf += LegDRf;
        lf += LegDLf;
        lb += LegDLb;
    }
    pwm_set_duty(servo_rb, Radian_toPwmDuty(rb));
    pwm_set_duty(servo_rf, Radian_toPwmDuty(-rf));
    pwm_set_duty(servo_lf, Radian_toPwmDuty(lf));
    pwm_set_duty(servo_lb, Radian_toPwmDuty(-lb));
}
void Leg_set_pos(float lx, float lz, float rx, float rz){
    Pos_limit(&lx, &lz);
    Pos_limit(&rx, &rz);
    struct LegServoAngle l = Pos_toServoAngle(lx, lz);
    struct LegServoAngle r = Pos_toServoAngle(rx, rz);
    Leg_set_duty(r.b, r.f, l.f, l.b);
}

const int jumpStep[] = {
    0,//预收腿
    150,//伸腿
    300,//收腿
    -1,
};
float jumpLegZMin = -30, jumpLegZMax = -147;

uint32 jumpTime = 32767;

void jumpPit(uint32 period, float *legX, float *legZ){
    Step step = getStep(jumpStep, jumpTime);
    switch(step.step){
        case 0:
            *legZ = jumpLegZMin;
            break;
        case 1:
            *legZ = jumpLegZMax;
            break;
        case 2:
            *legZ = jumpLegZMin;
            if(*legX > 0){
                *legX = 0;
            }
            break;
        default:
            return;
    }
    jumpTime+=period;
}
void jump(){
    Step step = getStep(jumpStep, jumpTime);
    if(step.step > 2){
        jumpTime = 0;
    }
}
