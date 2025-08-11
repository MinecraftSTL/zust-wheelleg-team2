/*
 * PidPage.c
 *
 *  Created on: 2025Äê3ÔÂ11ÈÕ
 *      Author: minec
 */

#include "PidPage.h"

void PidPage_init(PidPage *this, char name[], PID *pid){
    ListPage_init(&this->Pid, name, (Page*[]){&this->Kp, &this->Ki, &this->Kd, &this->Max_I, &this->Max_Out, NULL});
    FloatPage_init(&this->Kp, "Kp", &pid->Kp, 0, 10000);
    FloatPage_init(&this->Ki, "Ki", &pid->Ki, 0, 10000);
    FloatPage_init(&this->Kd, "Kd", &pid->Kd, 0, 10000);
    FloatPage_init(&this->Max_I, "Max_I", &pid->Max_I, 0, 999999);
    FloatPage_init(&this->Max_Out, "Max_Out", &pid->Max_Out, 0, 999999);
    this->Max_I.extends.floatValue.dot = this->Max_Out.extends.floatValue.dot = 6;
}


Page *PidPage_getRoot(PidPage *this){
    return &this->Pid;
}
