/*
 * PidPage.c
 *
 *  Created on: 2025Äê3ÔÂ11ÈÕ
 *      Author: minec
 */

#include "PidPage.h"

void PidPage_init(PidPage *this, char name[], PID *pid){
    ListPage_init(&this->Pid, name, 3, (Page*[]){&this->Kp, &this->Ki, &this->Kd});
    FFloatPage_init(&this->Kp, "Kp", &pid->Kp, 0, 10000);
    FFloatPage_init(&this->Ki, "Ki", &pid->Ki, 0, 10000);
    FFloatPage_init(&this->Kd, "Kd", &pid->Kd, 0, 10000);
}

