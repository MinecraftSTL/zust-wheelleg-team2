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
* 开发环境          ADS v1.9.4
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

#include "menu.h"
#include "key.h"
#include "isr.h"

#include "motor.h"
#include "camera.h"

#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 enableInterrupts(); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 disableInterrupts(); 来拒绝响应任何的中断，因此需要我们自己手动调用 enableInterrupts(); 来开启中断的响应。


// **************************** 代码区域 ****************************

struct PagePageValue main_menu;
struct PagePageValue menu_arg;
struct PagePageValue menu_arg_PID;
struct PagePageValue menu_arg_PID_motorL;
struct FloatPageValue menu_arg_PID_motorL_Kp;
struct FloatPageValue menu_arg_PID_motorL_Ki;
struct FloatPageValue menu_arg_PID_motorL_Kd;
struct FloatPageValue menu_arg_PID_motorL_Kdd;
struct PagePageValue menu_arg_PID_motorR;
struct FloatPageValue menu_arg_PID_motorR_Kp;
struct FloatPageValue menu_arg_PID_motorR_Ki;
struct FloatPageValue menu_arg_PID_motorR_Kd;
struct FloatPageValue menu_arg_PID_motorR_Kdd;
struct PagePageValue menu_arg_PID_turn;
struct FloatPageValue menu_arg_PID_turn_Ki;
struct PagePageValue menu_arg_PID_turn_straight;
struct FloatPageValue menu_arg_PID_turn_straight_Kp;
struct FloatPageValue menu_arg_PID_turn_straight_Kd;
struct PagePageValue menu_arg_PID_turn_bend;
struct FloatPageValue menu_arg_PID_turn_bend_Kp;
struct FloatPageValue menu_arg_PID_turn_bend_Kd;
struct PagePageValue menu_arg_PID_turn_arc;
struct FloatPageValue menu_arg_PID_turn_arc_Kp;
struct FloatPageValue menu_arg_PID_turn_arc_Kd;
struct PagePageValue menu_arg_V;
struct FloatPageValue menu_arg_V_target;
struct FloatPageValue menu_arg_V_straight;
struct FloatPageValue menu_arg_V_S;
struct FloatPageValue menu_arg_V_annulus;
struct PagePageValue menu_arg_V_hill;
struct PagePageValue menu_debug;
struct BooleanPageValue menu_debug_forceSpeedEn;
struct FloatPageValue menu_debug_forceSpeed;
struct BooleanPageValue menu_debug_forceTurnEn;
struct FloatPageValue menu_debug_forceTurnKp;
struct FloatPageValue menu_debug_forceTurnKd;
struct BooleanPageValue menu_start;

void core1_main(void)
{
    gpio_init(P20_8, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P20_9, GPO, 0, GPO_PUSH_PULL);
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等

    // 此处编写用户代码 例如外设初始化代码等

    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    struct PageKey main_menued = new_PageKey("main", PAGE_TYPE, &main_menu);
    PagePageValue_init(&main_menu, &(struct PageKey[]){
        new_PageKey("arg", PAGE_TYPE, &menu_arg),
        new_PageKey("debug", PAGE_TYPE, &menu_debug),
        new_PageKey("start", BOOL_TYPE, &menu_start),
    }, 3);
    PagePageValue_init(&menu_arg, &(struct PageKey[]){
        new_PageKey("PID", PAGE_TYPE, &menu_arg_PID),
        new_PageKey("V", PAGE_TYPE, &menu_arg_V),
    }, 2);
    PagePageValue_init(&menu_arg_PID, &(struct PageKey[]){
        new_PageKey("motorL", PAGE_TYPE, &menu_arg_PID_motorL),
        new_PageKey("motorR", PAGE_TYPE, &menu_arg_PID_motorR),
        new_PageKey("turn", PAGE_TYPE, &menu_arg_PID_turn),
    }, 3);
    PagePageValue_init(&menu_arg_PID_motorL, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_motorL_Kp),
        new_PageKey("Ki", FLOAT_TYPE, &menu_arg_PID_motorL_Ki),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_motorL_Kd),
        new_PageKey("Kdd", FLOAT_TYPE, &menu_arg_PID_motorL_Kdd),
    }, 4);
    FloatPageValue_init(&menu_arg_PID_motorL_Kp, &(motor_l.Kp), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorL_Ki, &(motor_l.Ki), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorL_Kd, &(motor_l.Kd), -1000., 1000., 10);
    FloatPageValue_init(&menu_arg_PID_motorL_Kdd, &(motor_l.Kdd), -1000., 1000., 10);
    PagePageValue_init(&menu_arg_PID_motorR, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_motorR_Kp),
        new_PageKey("Ki", FLOAT_TYPE, &menu_arg_PID_motorR_Ki),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_motorR_Kd),
        new_PageKey("Kdd", FLOAT_TYPE, &menu_arg_PID_motorR_Kdd),
    }, 4);
    FloatPageValue_init(&menu_arg_PID_motorR_Kp, &(motor_r.Kp), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorR_Ki, &(motor_r.Ki), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorR_Kd, &(motor_r.Kd), -1000., 1000., 10);
    FloatPageValue_init(&menu_arg_PID_motorR_Kdd, &(motor_r.Kdd), -1000., 1000., 10);
    PagePageValue_init(&menu_arg_PID_turn, &(struct PageKey[]){
        new_PageKey("Ki", FLOAT_TYPE, &menu_arg_PID_turn_Ki),
        new_PageKey("Kdd", FLOAT_TYPE, new_FloatPageValue(&(motor_turn.Kdd), -1000., 1000., 10)),
        new_PageKey("straight", PAGE_TYPE, &menu_arg_PID_turn_straight),
        new_PageKey("bend", PAGE_TYPE, &menu_arg_PID_turn_bend),
        new_PageKey("arc", PAGE_TYPE, &menu_arg_PID_turn_arc),
    }, 5);
    FloatPageValue_init(&menu_arg_PID_turn_Ki, &(motor_turn.Ki), -1., 1., 0.01);
    PagePageValue_init(&menu_arg_PID_turn_straight, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_turn_straight_Kp),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_turn_straight_Kd),
    }, 2);
    FloatPageValue_init(&menu_arg_PID_turn_straight_Kp, &straight_Kp, -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_turn_straight_Kd, &straight_Kd, -1000., 1000., 5);
    PagePageValue_init(&menu_arg_PID_turn_bend, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_turn_bend_Kp),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_turn_bend_Kd),
    }, 2);
    FloatPageValue_init(&menu_arg_PID_turn_bend_Kp, &bend_Kp, -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_turn_bend_Kd, &bend_Kd, -1000., 1000., 5);
    PagePageValue_init(&menu_arg_PID_turn_arc, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_turn_arc_Kp),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_turn_arc_Kd),
    }, 2);
    FloatPageValue_init(&menu_arg_PID_turn_arc_Kp, &arc_Kp, -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_turn_arc_Kd, &arc_Kd, -1000., 1000., 5);
    PagePageValue_init(&menu_arg_V, &(struct PageKey[]){
        new_PageKey("target", FLOAT_TYPE, &menu_arg_V_target),
        new_PageKey("straight", FLOAT_TYPE, &menu_arg_V_straight),
        new_PageKey("S", FLOAT_TYPE, &menu_arg_V_S),
        new_PageKey("annulus", FLOAT_TYPE, &menu_arg_V_annulus),
        new_PageKey("hill", FLOAT_TYPE, &menu_arg_V_hill),
    }, 5);
    FloatPageValue_init(&menu_arg_V_target, &target_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_straight, &straight_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_S, &S_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_annulus, &annulus_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_hill, &hill_speed, -500., 500., 25);
    PagePageValue_init(&menu_debug, &(struct PageKey[]){
        new_PageKey("forceVEn", BOOL_TYPE, &menu_debug_forceSpeedEn),
        new_PageKey("forceV", FLOAT_TYPE, &menu_debug_forceSpeed),
        new_PageKey("forceTurnEn", BOOL_TYPE, &menu_debug_forceTurnEn),
        new_PageKey("forceTurnKp", FLOAT_TYPE, &menu_debug_forceTurnKp),
        new_PageKey("forceTurnKd", FLOAT_TYPE, &menu_debug_forceTurnKd),
    }, 5);
    BooleanPageValue_init(&menu_debug_forceSpeedEn, &debug_forceSpeedEn);
    FloatPageValue_init(&menu_debug_forceSpeed, &debug_forceSpeed, -500., 500., 25);
    BooleanPageValue_init(&menu_debug_forceTurnEn, &debug_forceTurnEn);
    FloatPageValue_init(&menu_debug_forceTurnKp, &debug_forceTurnKp, -100., 100., 0.1);
    FloatPageValue_init(&menu_debug_forceTurnKd, &debug_forceTurnKd, -9990., 9990., 5);
    BooleanPageValue_init(&menu_start, &speed_qidong);
    system_delay_ms(1000);
    beep_once();
    PageKey_print(&main_menued);
    for(;;)
    {
        // 此处编写需要循环执行的代码
        uint8 pressed = Button_Press();
        if(pressed){
            PageKey_press(&main_menued, pressed);
            PageKey_print(&main_menued);
        }
        // 此处编写需要循环执行的代码
    }
    del_PageKey(&main_menu);
}
#pragma section all restore
