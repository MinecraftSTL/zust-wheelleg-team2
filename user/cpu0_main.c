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

uint8 pressed[KEY_NUM] = {0};

Page menu_main;
Page menu_main_carRun;
Page menu_main_arg;
Page menu_main_arg_k;
Page menu_main_arg_k_kZero;
Page menu_main_arg_k_speedF;
Page menu_main_arg_k_lx2s;
Page menu_main_arg_k_lx2s_a;
Page menu_main_arg_k_lx2s_k;
Page menu_main_arg_k_jump;
Page menu_main_arg_k_jump_step0;
Page menu_main_arg_k_jump_step1;
Page menu_main_arg_k_jump_step2;
Page menu_main_arg_k_jump_step3;
Page menu_main_arg_k_camera;
Page menu_main_arg_k_camera_horizon;
Page menu_main_arg_k_camera_deltaH;
Page menu_main_arg_k_turnA;
Page menu_main_arg_PID;
PidPage menu_main_arg_PID_vAy;
PidPage menu_main_arg_PID_xAy;
PidPage menu_main_arg_PID_vVx;
PidPage menu_main_arg_PID_vAz;
PidPage menu_main_arg_PID_turn;
PidPage menu_main_arg_PID_xAx;
Page menu_main_set;
Page menu_main_set_gyro;
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
Page menu_main_debug_ffRow;
Page menu_main_debug_jump;

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    beep_init();
    Flash_Init();
    ips200_init(IPS200_TYPE_SPI);
    gyro_init();
    my_key_init(PIT00ms);
    small_driver_uart_init();
    Leg_init();
    MyEncoder_Init();
    MyCamera_Init();
    PID_param_init();
    Pit_init();
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    ListPage_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", 4, (Page*[]){
        &menu_main_carRun,
        &menu_main_arg,
        &menu_main_set,
        &menu_main_debug,
    });
    IntPage_init(&menu_main_carRun, "car_run", &g_Car_Status, 0, 2);
    menu_main_carRun.open = 9;
    ListPage_init(&menu_main_arg, "arg", 2, (Page*[]){
        &menu_main_arg_k,
        &menu_main_arg_PID,
    });
    ListPage_init(&menu_main_arg_k, "k", 3, (Page*[]){
        &menu_main_arg_k_kZero,
        &menu_main_arg_k_jump,
        &menu_main_arg_k_camera,
        &menu_main_arg_k_turnA,
    });
    FloatPage_init(&menu_main_arg_k_kZero, "kZero", &kZero, -30, 30);
    menu_main_arg_k_kZero.extends.floatValue.dot = 1;
    ListPage_init(&menu_main_arg_k_jump, "jump", 4, (Page*[]){
        &menu_main_arg_k_jump_step0,
        &menu_main_arg_k_jump_step1,
        &menu_main_arg_k_jump_step2,
        &menu_main_arg_k_jump_step3,
    });
    IntPage_init(&menu_main_arg_k_jump_step0, "0", &(jumpStep[0]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step1, "1", &(jumpStep[1]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step2, "2", &(jumpStep[2]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step3, "3", &(jumpStep[3]), 0, 1000);
    ListPage_init(&menu_main_arg_k_camera, "camera", 2, (Page*[]){
        &menu_main_arg_k_camera_horizon,
        &menu_main_arg_k_camera_deltaH,
    });
    IntPage_init(&menu_main_arg_k_camera_horizon, "horizon", &camera_horizon, 0, image_h-1);
    IntPage_init(&menu_main_arg_k_camera_deltaH, "deltaH", &delta_camera_horizon, 0, image_h-1);
    FloatPage_init(&menu_main_arg_k_turnA, "turn_a", &Filter_turn.alpha, 0, 1);
    ListPage_init(&menu_main_arg_PID, "PID", 6, (Page*[]){
        PidPage_getRoot(&menu_main_arg_PID_vAy),
        PidPage_getRoot(&menu_main_arg_PID_xAy),
        PidPage_getRoot(&menu_main_arg_PID_vVx),
        PidPage_getRoot(&menu_main_arg_PID_vAz),
        PidPage_getRoot(&menu_main_arg_PID_turn),
        PidPage_getRoot(&menu_main_arg_PID_xAx),
    });
    PidPage_init(&menu_main_arg_PID_vAy, "vAy", &PID_WvAy);
    PidPage_init(&menu_main_arg_PID_xAy, "xAy", &PID_WxAy);
    PidPage_init(&menu_main_arg_PID_vVx, "vVx", &PID_vVx);
    PidPage_init(&menu_main_arg_PID_vAz, "vAz", &PID_vAz);
    PidPage_init(&menu_main_arg_PID_turn, "turn", &PID_WvAz);
    PidPage_init(&menu_main_arg_PID_xAx, "xAx", &PID_xAx);
    ListPage_init(&menu_main_set, "set", 1, (Page*[]){
        &menu_main_set_gyro,
    });
    FuncPage_init(&menu_main_set_gyro, "gyro", gyro_set);
    ListPage_init(&menu_main_debug, "debug", 6, (Page*[]){
        &menu_main_debug_fs,
        &menu_main_debug_fl,
        &menu_main_debug_fv,
        &menu_main_debug_fwp,
        &menu_main_debug_ffRow,
        &menu_main_debug_jump,
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
    IntPage_init(&menu_main_debug_fv_v, "V", &fvV, -1000, 1000);
    ListPage_init(&menu_main_debug_fl, "forceLeg", 5, (Page*[]){
        &menu_main_debug_fl_en,
        &menu_main_debug_fl_rb,
        &menu_main_debug_fl_rf,
        &menu_main_debug_fl_lf,
        &menu_main_debug_fl_lb,
    });
    BoolPage_init(&menu_main_debug_fl_en, "enable", &flEn, 0x03);
    FloatPage_init(&menu_main_debug_fl_rb, "rt_bk", &flRb, -1.57, 1.57);
    FloatPage_init(&menu_main_debug_fl_rf, "rt_fd", &flRf, -1.57, 1.57);
    FloatPage_init(&menu_main_debug_fl_lf, "lt_fd", &flLf, -1.57, 1.57);
    FloatPage_init(&menu_main_debug_fl_lb, "lt_bk", &flLb, -1.57, 1.57);
    menu_main_debug_fl_lb.extends.floatValue.dot
    =menu_main_debug_fl_lf.extends.floatValue.dot
    =menu_main_debug_fl_rf.extends.floatValue.dot
    =menu_main_debug_fl_rb.extends.floatValue.dot=1;
    ListPage_init(&menu_main_debug_fwp, "forceWPos", 5, (Page*[]){
        &menu_main_debug_fwp_en,
        &menu_main_debug_fwp_lx,
        &menu_main_debug_fwp_lz,
        &menu_main_debug_fwp_rx,
        &menu_main_debug_fwp_rz,
    });
    BoolPage_init(&menu_main_debug_fwp_en, "enable", &fwpEn, 0x03);
    FloatPage_init(&menu_main_debug_fwp_lx, "lt_x", &fwpLx, -LEG_MAX_X, LEG_MAX_X);
    FloatPage_init(&menu_main_debug_fwp_lz, "lt_z", &fwpLz, -LEG_MAX_Z, -LEG_MIN_Z);
    FloatPage_init(&menu_main_debug_fwp_rx, "rt_x", &fwpRx, -LEG_MAX_X, LEG_MAX_X);
    FloatPage_init(&menu_main_debug_fwp_rz, "rt_z", &fwpRz, -LEG_MAX_Z, -LEG_MIN_Z);
    menu_main_debug_fwp_lx.extends.floatValue.dot
    =menu_main_debug_fwp_lz.extends.floatValue.dot
    =menu_main_debug_fwp_rx.extends.floatValue.dot
    =menu_main_debug_fwp_rz.extends.floatValue.dot=2;
    BoolPage_init(&menu_main_debug_ffRow, "ffRow", &ffRow, 0x03);
    FuncPage_init(&menu_main_debug_jump, "jump", jump);

    beepLong();
    ips200_clear();
    PageKey_print(&menu_main, 0);
    for(;;){
        // 此处编写需要循环执行的代码
        PageKey_press(&menu_main, pressed);
        PageKey_print(&menu_main, 0);
        MyCamera_Show(200);
//        printf("%d\n", g_camera_mid_err);
//        printf("%f, %f, %f\r\n", pitch, roll, yaw);
        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
