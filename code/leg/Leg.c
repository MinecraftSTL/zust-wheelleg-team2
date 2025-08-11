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

struct ServoAngle Pos_toServoAngle(int32 x, int32 z){
    struct ServoAngle ret;

    double DBuW = hypot(DFB-x,z);
    double ABBuW = acos((DBuW*DBuW+LBU*LBU-LBD*LBD)/(2*DBuW*LBU));
    double AWBuFu = atan((double)-z/(DFB/2-x));
    if(AWBuFu < 0){
        AWBuFu = AWBuFu + PI;
    }
    else if(isnan(AWBuFu)){
        AWBuFu = PI/2;
    }
    ret.b = PI - (ABBuW+AWBuFu);
//    printf("%d, %d, %d, %lf\n",-z, (DFB/2-x), -z/(DFB/2-x), AWBuFu);
//    printf("%lf, %lf, %lf, %lf\n",DBuW, ABBuW, AWBuFu, ret.b);

    double DFuW = hypot(DFB+x,z);
    double AFFuW = acos((DFuW*DFuW+LFU*LFU-LFD*LFD)/(2*DFuW*LFU));
    double AWFuBu = atan((double)-z/(DFB/2+x));
    if(AWFuBu < 0){
        AWFuBu = AWFuBu + PI;
    }
    else if(isnan(AWFuBu)){
        AWFuBu = PI/2;
    }
    ret.f = PI - (AFFuW+AWFuBu);

////    x-=DFB/2;
//    x=-x;
//    z=-z;
//
//    double a = 2*x*LBU;
//    double b = 2*z*LBU;
//    double c = x*x + z*z + LBU*LBU - LBD*LBD;
//    ret.b = 2*atan((b+sqrt(a*a+b*b-c*c))/(a+c));
//
//    double d = 2*(x-DFB)*LFU;
//    double e = 2*z*LFU;
//    double f = (x-DFB)*(x-DFB) + z*z + LFU*LFU - LFD*LFD;
//    ret.f = 2*atan((e+sqrt(d*d+e*e-f*f))/(d+f));
//    ret.f=PI-ret.f;

    return ret;
}

uint8 Pos_isLegal(int32 x, int32 z, const struct ServoAngle a){
    if(a.f<0 || a.f > PI/2){
        return 0;
    }
    if(a.b<0 || a.b > PI/2){
        return 0;
    }
    if(x < -MAX_X || x > MAX_X){
        return 0;
    }
    if(-z < 0 || -z > MAX_Z){
        return 0;
    }
    return 1;
}

uint32 Radian_toPwmDuty(double rad){
    return PWM_DUTY_MAX/(1000/freq)*(1+rad/PI/PI*2.8);
}

void Leg_set_duty(double rb, double rf, double lf, double lb){
    pwm_set_duty(servo_rb, Radian_toPwmDuty(rb+1.5));
    pwm_set_duty(servo_rf, Radian_toPwmDuty(-rf+1.1));
    pwm_set_duty(servo_lf, Radian_toPwmDuty(lf+1.5));
    pwm_set_duty(servo_lb, Radian_toPwmDuty(-lb+0.9));
}

void Leg_set_pos(int32 lx, int32 lz, int32 rx, int32 rz){
    struct ServoAngle l = Pos_toServoAngle(lx, lz);
    struct ServoAngle r = Pos_toServoAngle(rx, rz);
//    printf("%lf, %lf, %lf, %lf\n",r.b, r.f, l.b, l.f);
    if(!Pos_isLegal(lx, lz, l) || !Pos_isLegal(rx, rz, r)){
        return;
    }
    Leg_set_duty(r.b, r.f, l.b, l.f);
}
