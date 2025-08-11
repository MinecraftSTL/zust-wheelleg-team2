/*
 * PidPage.h
 *
 *  Created on: 2025��3��11��
 *      Author: minec
 */

#ifndef CODE_MENU_PIDPAGE_H_
#define CODE_MENU_PIDPAGE_H_

#include "Menu.h"
#include "PID.h"

typedef struct PidPage{
    Page Pid;
    Page Kp, Ki, Kd;
}PidPage;

void PidPage_init(PidPage *this, char name[], PID *pid);

#endif /* CODE_MENU_PIDPAGE_H_ */
