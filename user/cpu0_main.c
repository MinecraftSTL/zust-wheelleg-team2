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
* 文件名称          cpu0_main
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
#include "cpu0_main.h"

#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************
const uint16 PIT00ms = 5;
const uint16 PIT01ms = 5;
const uint16 PIT10ms = 5;

uint8 pressed = 0;

Page menu_main;
Page menu_main_PID;
Page menu_main_PID_pitch;
Page menu_main_PID_pitch_Kp;
Page menu_main_PID_pitch_Ki;
Page menu_main_PID_pitch_Kd;
Page menu_main_PID_Vx;
Page menu_main_PID_Vx_Kp;
Page menu_main_PID_Vx_Ki;
Page menu_main_PID_Vx_Kd;
Page menu_main_carRun;
Page menu_main_debug;
Page menu_main_debug_fs;
Page menu_main_debug_fs_en;
Page menu_main_debug_fs_speed;
Page menu_main_debug_fv;
Page menu_main_debug_fv_en;
Page menu_main_debug_fv_v;
Page menu_main_debug_fl;
Page menu_main_debug_fl_en;
Page menu_main_debug_fl_rb;
Page menu_main_debug_fl_rf;
Page menu_main_debug_fl_lf;
Page menu_main_debug_fl_lb;
Page menu_main_debug_fwp;
Page menu_main_debug_fwp_en;
Page menu_main_debug_fwp_lx;
Page menu_main_debug_fwp_lz;
Page menu_main_debug_fwp_rx;
Page menu_main_debug_fwp_rz;

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    beep_init();
    ips200_init(IPS200_TYPE_SPI);
    gyro_init(PIT10ms);
    key_init(PIT00ms);
    small_driver_uart_init();
    Leg_init();
    MyEncoder_Init();
    PID_param_init();
    pit_ms_init(CCU60_CH0, PIT00ms);
    pit_ms_init(CCU61_CH0, PIT10ms);
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    ListPage_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", 3, (Page*[]){
        &menu_main_PID,
        &menu_main_carRun,
        &menu_main_debug,
    });
    ListPage_init(&menu_main_PID, "PID", 2, (Page*[]){
        &menu_main_PID_pitch,
        &menu_main_PID_Vx,
    });
    ListPage_init(&menu_main_PID_pitch, "pitch", 3, (Page*[]){
        &menu_main_PID_pitch_Kp,
        &menu_main_PID_pitch_Ki,
        &menu_main_PID_pitch_Kd,
    });
    FFloatPage_init(&menu_main_PID_pitch_Kp, "Kp", &pitch.Kp, 0, 10000);
    FFloatPage_init(&menu_main_PID_pitch_Ki, "Ki", &pitch.Ki, 0, 10000);
    FFloatPage_init(&menu_main_PID_pitch_Kd, "Kd", &pitch.Kd, 0, 10000);
    ListPage_init(&menu_main_PID_Vx, "Vx", 3, (Page*[]){
        &menu_main_PID_Vx_Kp,
        &menu_main_PID_Vx_Ki,
        &menu_main_PID_Vx_Kd,
    });
    FFloatPage_init(&menu_main_PID_Vx_Kp, "Kp", &Vx.Kp, 0, 10000);
    FFloatPage_init(&menu_main_PID_Vx_Ki, "Ki", &Vx.Ki, 0, 10000);
    FFloatPage_init(&menu_main_PID_Vx_Kd, "Kd", &Vx.Kd, 0, 10000);
    BoolPage_init(&menu_main_carRun, "run", &car_run, 0x03);
    ListPage_init(&menu_main_debug, "debug", 4, (Page*[]){
        &menu_main_debug_fs,
        &menu_main_debug_fl,
        &menu_main_debug_fv,
        &menu_main_debug_fwp,
    });
    ListPage_init(&menu_main_debug_fs, "forceSpeed", 2, (Page*[]){
        &menu_main_debug_fs_en,
        &menu_main_debug_fs_speed,
    });
    BoolPage_init(&menu_main_debug_fs_en, "enable", &fsEn, 0x03);
    IntPage_init(&menu_main_debug_fs_speed, "speed", &fsSpeed, -10000, 10000);
    ListPage_init(&menu_main_debug_fv, "forceV", 2, (Page*[]){
        &menu_main_debug_fv_en,
        &menu_main_debug_fv_v,
    });
    BoolPage_init(&menu_main_debug_fv_en, "enable", &fvEn, 0x03);
    IntPage_init(&menu_main_debug_fv_v, "V", &fvV, -100, 100);
    ListPage_init(&menu_main_debug_fl, "forceLeg", 5, (Page*[]){
        &menu_main_debug_fl_en,
        &menu_main_debug_fl_rb,
        &menu_main_debug_fl_rf,
        &menu_main_debug_fl_lf,
        &menu_main_debug_fl_lb,
    });
    BoolPage_init(&menu_main_debug_fl_en, "enable", &flEn, 0x03);
    FFloatPage_init(&menu_main_debug_fl_rb, "rt_bk", &flRb, -3.14, 3.14);
    FFloatPage_init(&menu_main_debug_fl_rf, "rt_fd", &flRf, -3.14, 3.14);
    FFloatPage_init(&menu_main_debug_fl_lf, "lt_fd", &flLf, -3.14, 3.14);
    FFloatPage_init(&menu_main_debug_fl_lb, "lt_bk", &flLb, -3.14, 3.14);
    menu_main_debug_fl_lb.extends.fFloatValue.dot
    =menu_main_debug_fl_lf.extends.fFloatValue.dot
    =menu_main_debug_fl_rf.extends.fFloatValue.dot
    =menu_main_debug_fl_rb.extends.fFloatValue.dot=1;
    ListPage_init(&menu_main_debug_fwp, "forceWPos", 5, (Page*[]){
        &menu_main_debug_fwp_en,
        &menu_main_debug_fwp_lx,
        &menu_main_debug_fwp_lz,
        &menu_main_debug_fwp_rx,
        &menu_main_debug_fwp_rz,
    });
    BoolPage_init(&menu_main_debug_fwp_en, "enable", &fwpEn, 0x03);
    IntPage_init(&menu_main_debug_fwp_lx, "lt_x", &fwpLx, -MAX_X, MAX_X);
    IntPage_init(&menu_main_debug_fwp_lz, "lt_z", &fwpLz, -MAX_Z, 0);
    IntPage_init(&menu_main_debug_fwp_rx, "rt_x", &fwpRx, -MAX_X, MAX_X);
    IntPage_init(&menu_main_debug_fwp_rz, "rt_z", &fwpRz, -MAX_Z, 0);

    beepLong();
    PageKey_print(&menu_main, 0);
    for(;;){
        // 此处编写需要循环执行的代码
        if(pressed){
            beepShort();
            PageKey_press(&menu_main, pressed);
            PageKey_print(&menu_main, 0);
            pressed=0;
        }
        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
