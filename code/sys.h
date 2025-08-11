/*
作者：Charon and 快乐牌小刀片
       未经授权禁止转售
 */


#ifndef CODE_SYS_H_
#define CODE_SYS_H_

#include "zf_common_headfile.h"
#include "motor.h"
#include "camera.h"
#include "encoder.h"
#include "isr.h"
#include "key.h"
#include "cpu0_main.h"

float slope_calculate (uint8 begin, uint8 end,int * border);
void caculate_distance(uint8 start,uint8 end,int *border,float *slope_new,float *distance_new);
void beep_on(void);
void beep_off(void);
void beep_flip(void);

#define Beep    P33_10                          //蜂鸣器

extern float motor_l_out,motor_r_out;
extern char annulus_open_or_close_flag;
extern char cross_open_or_close_flag;
extern char image_open_or_close_flag;
extern char barrier__open_or_close_flag;
extern char flash_open_or_close_flag;
extern char meum_close_flag;
extern char s_road_open_or_close_flag;
extern int8 speed_qidong;
extern char ngszd;
extern float straight_k_err;
extern float straight_k_1;
extern float straight_k_2;
extern float serve_out_pwm;
extern char hill_flag;

#endif /* CODE_SYS_H_ */
