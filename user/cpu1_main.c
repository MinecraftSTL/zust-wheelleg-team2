/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

#include "menu.h"

#include "PID.h"
#include "MyEncoder.h"

#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

// **************************** 代码区域 ****************************

uint8 pressed = 0;

Page main_menu;
Page menu_arg;
Page menu_arg_PID;
Page menu_arg_PID_motorL;
Page menu_arg_PID_motorL_Kp;
Page menu_arg_PID_motorL_Ki;
Page menu_arg_PID_motorL_Kd;
Page menu_arg_PID_motorR;
Page menu_arg_PID_motorR_Kp;
Page menu_arg_PID_motorR_Ki;
Page menu_arg_PID_motorR_Kd;
Page menu_arg_PID_turn;
Page menu_arg_PID_turn_Ki;
Page menu_arg_PID_turn_straight;
Page menu_arg_PID_turn_straight_Kp;
Page menu_arg_PID_turn_straight_Kd;
Page menu_arg_PID_turn_bend;
Page menu_arg_PID_turn_bend_Kp;
Page menu_arg_PID_turn_bend_Kd;
Page menu_arg_PID_turn_arc;
Page menu_arg_PID_turn_arc_Kp;
Page menu_arg_PID_turn_arc_Kd;
Page menu_arg_V;
Page menu_arg_V_target;
Page menu_arg_V_straight;
Page menu_arg_V_S;
Page menu_arg_V_annulus;
Page menu_arg_V_hill;
Page menu_debug;
Page menu_debug_forceSpeedEn;
Page menu_debug_forceSpeed;
Page menu_debug_forceTurnEn;
Page menu_debug_forceTurnKp;
Page menu_debug_forceTurnKd;
Page menu_start;
Page menu_testInt;

PID motor_l;
PID motor_r;
PID motor_turn;
float target_speed;
float straight_speed;
float S_speed;
float annulus_speed;
float hill_speed;
uint8 qidong;
int32 testInt;

void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等
    main_menu.parent = NULL;
    ListPage_init(&main_menu, "main", 4, &(Page*[]){
        &menu_arg,
        &menu_start,
        &menu_arg_PID_turn_Ki,
        &menu_testInt,
    });
    FloatPage_init(&menu_arg_PID_turn_Ki, "Ki", &(motor_turn.Kd), -1000., 1000.);
    IntPage_init(&menu_testInt, "i", &testInt, -10000000, 100000000);
    ListPage_init(&menu_arg, "arg", 1, &(Page*[]){
        &menu_arg_PID,
    });
    ListPage_init(&menu_arg_PID, "PID", 2, &(Page*[]){
        &menu_arg_PID_motorL,
        &menu_arg_PID_motorR,
    });
    ListPage_init(&menu_arg_PID_motorL, "motorL", 3, &(Page*[]){
        &menu_arg_PID_motorL_Kp,
        &menu_arg_PID_motorL_Ki,
        &menu_arg_PID_motorL_Kd,
    });
    FloatPage_init(&menu_arg_PID_motorL_Kp, "Kp", &(motor_l.Kp), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorL_Ki, "Ki", &(motor_l.Ki), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorL_Kd, "Kd", &(motor_l.Kd), -1000., 1000.);
    ListPage_init(&menu_arg_PID_motorR, "motorR", 3, &(Page*[]){
        &menu_arg_PID_motorR_Kp,
        &menu_arg_PID_motorR_Ki,
        &menu_arg_PID_motorR_Kd,
    });
    FloatPage_init(&menu_arg_PID_motorR_Kp, "Kp", &(motor_r.Kp), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorR_Ki, "Ki", &(motor_r.Ki), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorR_Kd, "Kd", &(motor_r.Kd), -1000., 1000.);

    BoolPage_init(&menu_start, "start", &qidong, 0x03);
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    PageKey_print(&main_menu);
    for(;;){
        // 此处编写需要循环执行的代码
        if(pressed){
            PageKey_press(&main_menu, pressed);
            PageKey_print(&main_menu);
            pressed=0;
        }
        ips200_show_int(0, 300, testInt,9);
//        printf("%d", switch_encoder_num);
        // 此处编写需要循环执行的代码
    }
}
#pragma section all restore
// **************************** 代码区域 ****************************
