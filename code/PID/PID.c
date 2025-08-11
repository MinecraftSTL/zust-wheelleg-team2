/*
 * pid.c
 *
 * Created on: 2024��1��24��
 *     Author: ������
 */
#include <Pid.h>
#include "sys.h"

void PID_init(PID *this, float kp, float ki, float kd, float max_I, float max_Out)
{
    PID_clear(this);
    this->Kp = kp;
    this->Ki = ki;
    this->Kd = kd;
    this->Max_I = max_I;
    this->Max_Out = max_Out;
}

float pid(PID *this, float TargetValue, float ActualValue)
{
    float ret;
    float Ek = TargetValue - ActualValue;
    this->Ek_sum += Ek;
    this->Ek_sum = func_limit(this->Ek_sum, this->Max_I);
    if(isnan(this->Ek_sum)){
        this->Ek_sum=0;
    }

    ret = this->Kp * Ek +
             this->Ki * this->Ek_sum +
             (isnan(this->Ek_)?0:(this->Kd * (Ek - this->Ek_)));
    this->Ek_ = Ek;
    ret = func_limit(ret, this->Max_Out);
    return ret;
}

void PID_clear(PID *this)
{
    this->Ek_ = NAN; // �ϴ�ƫ��ֵ��ʼ��
    this->Ek_sum = 0; // ���ϴ�ƫ��ֵ��ʼ��
}
