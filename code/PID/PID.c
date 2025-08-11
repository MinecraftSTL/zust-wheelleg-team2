/*
 * pid.c
 *
 * Created on: 2024年1月24日
 *     Author: 潘申奇
 */
#include"PID.h"

#include "sys.h"

#define MY_ABS(num) (((num) > 0) ? (num) : -(num))
#define LIMIT_VAL(a,min,max) ((a)<(min)?(min):((a)>(max)?(max):(a)))

void PID_Init(PID *this, float kp, float ki, float kd, float max_I, float max_Out)
{
    this->Ek_ = 0;
    this->Ek_sum = 0;
    this->Kp = kp;
    this->Ki = ki;
    this->Kd = kd;
    this->OUT = 0;
    this->Max_I = max_I;
    this->Max_Out = max_Out;
}

float pid(PID *PID, float TargetValue, float ActualValue)
{
    float ret;
    float Ek = TargetValue - ActualValue;
    PID->Ek_sum += Ek;
    PID->Ek_sum = func_limit(PID->Ek_sum, PID->Max_I);

    ret = (PID->Kp * Ek) +
             (PID->Ki * PID->Ek_sum) +
             (PID->Kd * (Ek - PID->Ek_));
    PID->Ek_ = Ek;
    ret = func_limit(ret, PID->Max_Out);
    return ret;
}

void PID_clear(PID *this)
{
    this->Ek_ = 0; // 上次偏差值初始化
    this->Ek_sum = 0; // 上上次偏差值初始化
    this->OUT = 0;
}
