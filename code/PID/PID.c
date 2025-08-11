/*
 * pid.c
 *
 * Created on: 2024年1月24日
 *     Author: 潘申奇
 */
#include <Pid.h>
#include <Sys.h>

void PID_init(PID *this, float kp, float ki, float kd, float max_I, float max_Out)
{
    PID_clear(this);
    this->Kp = kp;
    this->Ki = ki;
    this->Kd = kd;
    this->Max_I = max_I;
    this->Max_Out = max_Out;
}

float pid(PID *this, float TargetValue, float ActualValue)//改版pid
{
    zf_assert(!!this);
    float ret;
    float Ek = TargetValue - ActualValue;
    this->Ek_sum += this->Ki * Ek;
    this->Ek_sum = func_limit(this->Ek_sum, this->Max_I);
    if(isnan(this->Ek_sum)){
        this->Ek_sum=0;
    }

    ret = this->Kp * Ek +
             this->Ek_sum +
             (isnan(this->Ek_)?0:(this->Kd * (Ek - this->Ek_)));
    this->Ek_ = Ek;
    ret = func_limit(ret, this->Max_Out);
    return ret;
}

void PID_clone(PID *this, PID *target){
    PID_cloneArg(this, target);
    target->Ek_ = this->Ek_;
    target->Ek_sum = this->Ek_sum;
}
void PID_cloneArg(PID *this, PID *target){
    target->Kp = this->Kp;
    target->Ki = this->Ki;
    target->Kd = this->Kd;
    target->Max_I = this->Max_I;
    target->Max_Out = this->Max_Out;
}

void PID_clear(PID *this)
{
    this->Ek_ = NAN; // 上次偏差值初始化
    this->Ek_sum = 0; // 上上次偏差值初始化
}
