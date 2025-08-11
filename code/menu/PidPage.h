/*
 * PidPage.h
 *
 *  Created on: 2025Äê3ÔÂ11ÈÕ
 *      Author: minec
 */

#ifndef CODE_MENU_PIDPAGE_H_
#define CODE_MENU_PIDPAGE_H_

#include "../pid/Pid.h"
#include "Menu.h"

typedef struct PidPage{
    Page Pid;
    Page Kp, Ki, Kd, Max_I, Max_Out;
}PidPage;

void PidPage_init(PidPage *this, char name[], PID *pid);
Page *PidPage_getRoot(PidPage *this);

#endif /* CODE_MENU_PIDPAGE_H_ */
