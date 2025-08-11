/*
 * gyroscope.h
 *
 *  Created on: 2023Äê10ÔÂ28ÈÕ
 *      Author: Â¬¾¢º­
 */

#ifndef CODE_GYRO_GYRO_H_
#define CODE_GYRO_GYRO_H_

#include "Gyro_Process.h"
#include "Pit.h"
#include "icm42688.h"
#include "Beep.h"

#define my_gyroscope_init() Init_ICM42688()
#define my_get_acc() Get_Acc_ICM42688()
#define my_get_gyro() Get_Gyro_ICM42688()
//#define my_gyro_transition(x) icm42688_gyro_transition(x)
//#define my_acc_transition(x) icm42688_acc_transition(x)

#define my_gyro_z icm42688_gyro_z
#define my_gyro_y icm42688_gyro_y
#define my_gyro_x icm42688_gyro_x
#define my_acc_z icm42688_acc_z
#define my_acc_y icm42688_acc_y
#define my_acc_x icm42688_acc_x

#define tran_gyro_z my_gyro_z
#define tran_gyro_y my_gyro_y
#define tran_gyro_x my_gyro_x
#define tran_acc_z my_acc_z
#define tran_acc_y my_acc_y
#define tran_acc_x my_acc_x

#define Gravity_G 9.80665f

#define Gyro_Kp_back 50.f
#define Gyro_Kp_used 1.f

void Gyro_init();
void Gyro_set();
void get_gyro();
void get_gyorscope_data(void);
void Update_GyroData(void);
void Gyro_back();
extern float gyro_z,gyro_y,gyro_x;
//extern float gyro_z_res,gyro_x_res,gyro_y_res;
extern float acc_z,acc_y,acc_x;

extern float zero_my_gyro_z,zero_my_gyro_y ,zero_my_gyro_x,zero_my_acc_z,zero_my_acc_y,zero_my_acc_x;
extern float Sum_my_gyro_z,Sum_my_gyro_y,Sum_my_gyro_x,Sum_my_acc_z,Sum_my_acc_y,Sum_my_acc_x;


#endif /* CODE_GYRO_GYRO_H_ */
