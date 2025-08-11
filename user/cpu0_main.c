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
Page menu_main_carStatus;
Page menu_main_config;
Page menu_main_config_volume;
Page menu_main_config_mod;
Page menu_main_config_mod_gyro;
Page menu_main_arg;
Page menu_main_arg_k;
Page menu_main_arg_k_zero;
Page menu_main_arg_k_zero_k;
Page menu_main_arg_k_zero_x;
Page menu_main_arg_k_pitchX;
Page menu_main_arg_k_v;
Page menu_main_arg_k_v_straight;
Page menu_main_arg_k_v_bend;
Page menu_main_arg_k_v_circle;
Page menu_main_arg_k_v_ramp;
Page menu_main_arg_k_v_barrier;
Page menu_main_arg_k_v_bridge;
Page menu_main_arg_k_camera;
Page menu_main_arg_k_camera_bin;
Page menu_main_arg_k_camera_bin_deltaT;
Page menu_main_arg_k_camera_trapezoid;
Page menu_main_arg_k_camera_trapezoid_k;
Page menu_main_arg_k_camera_trapezoid_y;
Page menu_main_arg_k_camera_start;
Page menu_main_arg_k_camera_start_maxYAdd;
Page menu_main_arg_k_camera_inf;
Page menu_main_arg_k_camera_inf_bly2RDL;
Page menu_main_arg_k_camera_inf_RD2IErr;
Page menu_main_arg_k_camera_facingErr;
Page menu_main_arg_k_camera_straight;
Page menu_main_arg_k_camera_straight_yMin;
Page menu_main_arg_k_camera_straight_step;
Page menu_main_arg_k_camera_straight_err;
Page menu_main_arg_k_camera_status;
Page menu_main_arg_k_camera_status_now;
Page menu_main_arg_k_camera_status_jump;
Page menu_main_arg_k_camera_status_k;
Page menu_main_arg_k_camera_e;
Page menu_main_arg_k_camera_e_zebra;
Page menu_main_arg_k_camera_e_zebra_y;
Page menu_main_arg_k_camera_e_zebra_t;
Page menu_main_arg_k_camera_e_zebra_s;
Page menu_main_arg_k_camera_e_cross;
Page menu_main_arg_k_camera_e_cross_x;
Page menu_main_arg_k_camera_e_cross_y;
Page menu_main_arg_k_camera_e_circle;
Page menu_main_arg_k_camera_e_circle_x;
Page menu_main_arg_k_camera_e_circle_y;
Page menu_main_arg_k_camera_e_circle_line;
Page menu_main_arg_k_camera_e_ramp;
Page menu_main_arg_k_camera_e_ramp_s;
Page menu_main_arg_k_camera_e_ramp_z;
Page menu_main_arg_k_camera_e_barrier;
Page menu_main_arg_k_camera_e_barrier_y0;
Page menu_main_arg_k_camera_e_barrier_y1;
Page menu_main_arg_k_camera_e_barrier_t;
Page menu_main_arg_k_camera_e_bridge;
Page menu_main_arg_k_camera_e_bridge_xAx;
Page menu_main_arg_k_camera_e_bridge_x;
Page menu_main_arg_k_camera_e_bridge_y;
Page menu_main_arg_k_camera_e_bridge_kPitchX;
Page menu_main_arg_k_camera_e_bridge_ti;
Page menu_main_arg_k_camera_e_bridge_to;
Page menu_main_arg_k_camera_e_bridge_s;
Page menu_main_arg_k_camera_e_bridge_z;
Page menu_main_arg_k_camera_setLineY;
Page menu_main_arg_k_camera_err;
Page menu_main_arg_k_camera_err_y;
Page menu_main_arg_k_camera_err_deltaY;
Page menu_main_arg_k_camera_show;
Page menu_main_arg_k_camera_show_pInC1;
Page menu_main_arg_k_camera_show_wait;
Page menu_main_arg_k_jump;
Page menu_main_arg_k_jump_step0;
Page menu_main_arg_k_jump_step1;
Page menu_main_arg_k_jump_step2;
Page menu_main_arg_k_jump_step3;
Page menu_main_arg_PID;
PidPage menu_main_arg_PID_vAy;
PidPage menu_main_arg_PID_xAy;
PidPage menu_main_arg_PID_vVx;
PidPage menu_main_arg_PID_vAz;
PidPage menu_main_arg_PID_turn;
Page menu_main_arg_filter;
Page menu_main_arg_filter_speed;
Page menu_main_arg_filter_turn;
Page menu_main_arg_filter_xAx;
Page menu_main_arg_filter_xAx0;
Page menu_main_debug;
Page menu_main_debug_wheelClear;
Page menu_main_debug_wifiIm;
Page menu_main_debug_fs;
Page menu_main_debug_fs_en;
Page menu_main_debug_fs_speed;
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
Page menu_main_debug_frb;
Page menu_main_debug_jump;

void core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    beep_init();
    ips200_init(IPS200_TYPE_SPI);
    Flash_Init();
    my_key_init(PIT00ms);
    gyro_init();
    MyCamera_Init();
    Fps_init(PIT00ms);
//    Wifi_Image_Init();
    small_driver_uart_init();
    Leg_init();
    MyEncoder_Init();
    PID_param_init();
    Pit_init();
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    ListPage_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", (Page*[]){
        &menu_main_carStatus,
        &menu_main_config,
        &menu_main_arg,
        &menu_main_debug,
        NULL
    });
    EnumPage_init(&menu_main_carStatus, "carStatus", &carStatus, (char*[]){
        "stop",
        "start",
        "balance",
        "run",
        ""
    });
    menu_main_carStatus.extends.enumValue.update = CarStatus_update;
    ListPage_init(&menu_main_config, "config", (Page*[]){
        &menu_main_config_volume,
        &menu_main_config_mod,
        NULL
    });
    IntPage_init(&menu_main_config_volume, "volume", &volume, 0, 3);
    ListPage_init(&menu_main_config_mod, "mod", (Page*[]){
        &menu_main_config_mod_gyro,
        NULL
    });
    FuncPage_init(&menu_main_config_mod_gyro, "gyro", gyro_set);
    ListPage_init(&menu_main_arg, "arg", (Page*[]){
        &menu_main_arg_k,
        &menu_main_arg_PID,
        &menu_main_arg_filter,
        NULL
    });
    ListPage_init(&menu_main_arg_k, "k", (Page*[]){
        &menu_main_arg_k_zero,
        &menu_main_arg_k_pitchX,
        &menu_main_arg_k_v,
        &menu_main_arg_k_camera,
        &menu_main_arg_k_jump,
        NULL
    });
    ListPage_init(&menu_main_arg_k_zero, "zero", (Page*[]){
        &menu_main_arg_k_zero_k,
        &menu_main_arg_k_zero_x,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_zero_k, "k", &kZero, -30, 30);
    FloatPage_init(&menu_main_arg_k_zero_x, "x", &xZero, -LEG_MAX_X, LEG_MAX_X);
    menu_main_arg_k_zero_k.extends.floatValue.dot = menu_main_arg_k_zero_x.extends.floatValue.dot = 2;
    FloatPage_init(&menu_main_arg_k_pitchX, "pitchX", &kPitchX, -100, 100);
    ListPage_init(&menu_main_arg_k_v, "v", (Page*[]){
        &menu_main_arg_k_v_straight,
        &menu_main_arg_k_v_bend,
        &menu_main_arg_k_v_circle,
        &menu_main_arg_k_v_ramp,
        &menu_main_arg_k_v_barrier,
        &menu_main_arg_k_v_bridge,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_v_straight, "straight", &targetV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bend, "bend", &bendV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_circle, "circle", &circleV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_ramp, "ramp", &rampV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_barrier, "barrier", &barrierV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bridge, "bridge", &bridgeV, -10000, 10000);
    menu_main_arg_k_v_straight.extends.floatValue.dot = menu_main_arg_k_v_bend.extends.floatValue.dot = menu_main_arg_k_v_circle.extends.floatValue.dot =
            menu_main_arg_k_v_ramp.extends.floatValue.dot = menu_main_arg_k_v_bridge.extends.floatValue.dot = 4;
    ListPage_init(&menu_main_arg_k_camera, "camera", (Page*[]){
        &menu_main_arg_k_camera_bin,
        &menu_main_arg_k_camera_trapezoid,
        &menu_main_arg_k_camera_inf,
        &menu_main_arg_k_camera_facingErr,
        &menu_main_arg_k_camera_straight,
        &menu_main_arg_k_camera_status,
        &menu_main_arg_k_camera_e,
        &menu_main_arg_k_camera_setLineY,
        &menu_main_arg_k_camera_err,
        &menu_main_arg_k_camera_show,
        NULL
    });
    ListPage_init(&menu_main_arg_k_camera_bin, "bin", (Page*[]){
        &menu_main_arg_k_camera_bin_deltaT,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_bin_deltaT, "deltaT", &binDeltaT, -256, 255);
    ListPage_init(&menu_main_arg_k_camera_trapezoid, "black", (Page*[]){
        &menu_main_arg_k_camera_trapezoid_k,
        &menu_main_arg_k_camera_trapezoid_y,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_camera_trapezoid_k, "k", &trapezoidK, 0, 2147483648);
    IntPage_init(&menu_main_arg_k_camera_trapezoid_y, "y", &trapezoidY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_start, "start", (Page*[]){
        &menu_main_arg_k_camera_start_maxYAdd,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_start_maxYAdd, "maxYAdd", &maxStartYAdd, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_inf, "inf", (Page*[]){
        &menu_main_arg_k_camera_inf_bly2RDL,
        &menu_main_arg_k_camera_inf_RD2IErr,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_inf_bly2RDL, "bly2RDL", &bly2RDL, 0, MAX_BLY);
    FloatPage_init(&menu_main_arg_k_camera_inf_RD2IErr, "RD2IErr", &RD2IErr, 0, 1.57);
    FloatPage_init(&menu_main_arg_k_camera_facingErr, "facingErr", &facingErr, 0, 0.7854);
    menu_main_arg_k_camera_facingErr.extends.floatValue.dot = 0;
    ListPage_init(&menu_main_arg_k_camera_straight, "straight", (Page*[]){
        &menu_main_arg_k_camera_straight_yMin,
        &menu_main_arg_k_camera_straight_step,
        &menu_main_arg_k_camera_straight_err,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_straight_yMin, "yMin", &straightYMin, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_camera_straight_step, "step", &straightStep, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_straight_err, "err", &straightErr, 0, 1.57);
    ListPage_init(&menu_main_arg_k_camera_status, "status", (Page*[]){
        &menu_main_arg_k_camera_status_now,
        &menu_main_arg_k_camera_status_jump,
        &menu_main_arg_k_camera_status_k,
        NULL
    });
    EnumPage_init(&menu_main_arg_k_camera_status_now, "now", &cameraStatus, (char*[]){
            "NONE",
            "I_ZEBRA",
            "O_ZEBRA",
            "IN_CROSS",
            "R_CROSS",
            "O_CROSS",
            "I_LCIRCLE",
            "PTI_LCIRCLE",
            "TI_LCIRCLE",
            "R_LCIRCLE",
            "TO_LCIRCLE",
            "PO_LCIRCLE",
            "O_LCIRCLE",
            "OR_CROSS_LCIRCLE",
            "I_RCIRCLE",
            "PTI_RCIRCLE",
            "TI_RCIRCLE",
            "R_RCIRCLE",
            "TO_RCIRCLE",
            "PO_RCIRCLE",
            "O_RCIRCLE",
            "OR_CROSS_RCIRCLE",
            "RAMP",
            "I_BARRIER",
            "R_BARRIER",
            "O_BARRIER",
            "I_BRIDGE",
            "I_LBRIDGE",
            "O_LBRIDGE",
            "I_RBRIDGE",
            "O_RBRIDGE",
            "O_BRIDGE",
            ""
        });
    FloatPage_init(&menu_main_arg_k_camera_status_jump, "jump", &statusJump, 0, 0xFF);
    FloatPage_init(&menu_main_arg_k_camera_status_k, "k", &statusK, 0, 1);
    ListPage_init(&menu_main_arg_k_camera_e, "element", (Page*[]){
        &menu_main_arg_k_camera_e_zebra,
        &menu_main_arg_k_camera_e_cross,
        &menu_main_arg_k_camera_e_circle,
        &menu_main_arg_k_camera_e_barrier,
        &menu_main_arg_k_camera_e_bridge,
        NULL
    });
    ListPage_init(&menu_main_arg_k_camera_e_zebra, "zebra", (Page*[]){
        &menu_main_arg_k_camera_e_zebra_y,
        &menu_main_arg_k_camera_e_zebra_t,
        &menu_main_arg_k_camera_e_zebra_s,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_zebra_y, "y", &zebraY, 0, MT9V03X_H-1-3);
    IntPage_init(&menu_main_arg_k_camera_e_zebra_t, "t", &zebraT, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_zebra_s, "s", &zebraS, 0, 10000000);
    ListPage_init(&menu_main_arg_k_camera_e_cross, "cross", (Page*[]){
        &menu_main_arg_k_camera_e_cross_x,
        &menu_main_arg_k_camera_e_cross_y,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_cross_x, "x", &crossX, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_cross_y, "y", &crossY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_circle, "circle", (Page*[]){
        &menu_main_arg_k_camera_e_circle_x,
        &menu_main_arg_k_camera_e_circle_y,
        &menu_main_arg_k_camera_e_circle_line,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_circle_x, "x", &circleX, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_circle_y, "y", &circleY, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_circle_line, "line", &circleLine, 0, 1);
    menu_main_arg_k_camera_e_circle_line.extends.floatValue.dot = 0;
    ListPage_init(&menu_main_arg_k_camera_e_ramp, "ramp", (Page*[]){
        &menu_main_arg_k_camera_e_ramp_s,
        &menu_main_arg_k_camera_e_ramp_z,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_ramp_s, "s", &rampS, 0, 10000000);
    FloatPage_init(&menu_main_arg_k_camera_e_ramp_z, "z", &rampZ, -LEG_MIN_Z, -LEG_MAX_Z);
    ListPage_init(&menu_main_arg_k_camera_e_barrier, "barrier", (Page*[]){
        &menu_main_arg_k_camera_e_barrier_y0,
        &menu_main_arg_k_camera_e_barrier_y1,
        &menu_main_arg_k_camera_e_barrier_t,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y0, "y0", &barrierY0, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y1, "y1", &barrierY1, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_t, "t", &barrierT, 0, 100000);
    ListPage_init(&menu_main_arg_k_camera_e_bridge, "bridge", (Page*[]){
        &menu_main_arg_k_camera_e_bridge_xAx,
        &menu_main_arg_k_camera_e_bridge_x,
        &menu_main_arg_k_camera_e_bridge_y,
        &menu_main_arg_k_camera_e_bridge_kPitchX,
        &menu_main_arg_k_camera_e_bridge_ti,
        &menu_main_arg_k_camera_e_bridge_to,
        &menu_main_arg_k_camera_e_bridge_s,
        &menu_main_arg_k_camera_e_bridge_z,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_camera_e_bridge_xAx, "xAxAlpha", &defaultRollAlpha, 0, 1);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_x, "x", &bridgeX, 0, MT9V03X_W/2);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_y, "y", &bridgeY, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_bridge_kPitchX, "kPitchX", &bridgeKPitchX, -100, 100);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_ti, "ti", &bridgeTI, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_to, "to", &bridgeTO, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_s, "s", &bridgeS, 0, 10000000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_z, "z", &bridgeZ, -LEG_MIN_Z, -LEG_MAX_Z);
    IntPage_init(&menu_main_arg_k_camera_setLineY, "setLineY", &setLineY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_err, "err", (Page*[]){
        &menu_main_arg_k_camera_err_y,
        &menu_main_arg_k_camera_err_deltaY,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_err_y, "y", &errY, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_camera_err_deltaY, "deltaY", &errDeltaY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_show, "show", (Page*[]){
        &menu_main_arg_k_camera_show_pInC1,
        &menu_main_arg_k_camera_show_wait,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_show_pInC1, "pInC1", &showPInC1, 0x03);
    BoolPage_init(&menu_main_arg_k_camera_show_wait, "wait", &showWait, 0x03);
    ListPage_init(&menu_main_arg_k_jump, "jump", (Page*[]){
        &menu_main_arg_k_jump_step0,
        &menu_main_arg_k_jump_step1,
        &menu_main_arg_k_jump_step2,
        &menu_main_arg_k_jump_step3,
        NULL
    });
    IntPage_init(&menu_main_arg_k_jump_step0, "0", &(jumpStep[0]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step1, "1", &(jumpStep[1]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step2, "2", &(jumpStep[2]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step3, "3", &(jumpStep[3]), 0, 1000);
    ListPage_init(&menu_main_arg_PID, "PID", (Page*[]){
        &menu_main_arg_PID_vAy,
        &menu_main_arg_PID_xAy,
        &menu_main_arg_PID_vVx,
        &menu_main_arg_PID_vAz,
        &menu_main_arg_PID_turn,
        NULL
    });
    PidPage_init(&menu_main_arg_PID_vAy, "vAy", &PID_WvAy);
    PidPage_init(&menu_main_arg_PID_xAy, "xAy", &PID_WxAy);
    PidPage_init(&menu_main_arg_PID_vVx, "vVx", &PID_vVx);
    PidPage_init(&menu_main_arg_PID_vAz, "vAz", &PID_vAz);
    PidPage_init(&menu_main_arg_PID_turn, "turn", &PID_WvAz);
    ListPage_init(&menu_main_arg_filter, "filter", (Page*[]){
        &menu_main_arg_filter_turn,
        &menu_main_arg_filter_speed,
        &menu_main_arg_filter_xAx,
        &menu_main_arg_filter_xAx0,
        NULL
    });
    FloatPage_init(&menu_main_arg_filter_turn, "turn", &Filter1_turn.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_speed, "speed", &Filter1_speed.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_xAx, "xAx", &Filter1_xAx.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_xAx0, "xAx0", &Filter0_xAx.deltaMax, 0, 10000);
    ListPage_init(&menu_main_debug, "debug", (Page*[]){
        &menu_main_debug_wheelClear,
        &menu_main_debug_wifiIm,
        &menu_main_debug_fs,
        &menu_main_debug_fl,
        &menu_main_debug_fwp,
        &menu_main_debug_frb,
        &menu_main_debug_jump,
        NULL
    });
    BoolPage_init(&menu_main_debug_wheelClear, "wClear", &wheelClear, 0x03);
    FuncPage_init(&menu_main_debug_wifiIm, "wifiIm", Wifi_Image_Init);
    ListPage_init(&menu_main_debug_fs, "forceSpeed", (Page*[]){
        &menu_main_debug_fs_en,
        &menu_main_debug_fs_speed,
        NULL
    });
    BoolPage_init(&menu_main_debug_fs_en, "enable", &fsEn, 0x03);
    IntPage_init(&menu_main_debug_fs_speed, "speed", &fsSpeed, -10000, 10000);
    ListPage_init(&menu_main_debug_fl, "forceLeg", (Page*[]){
        &menu_main_debug_fl_en,
        &menu_main_debug_fl_rb,
        &menu_main_debug_fl_rf,
        &menu_main_debug_fl_lf,
        &menu_main_debug_fl_lb,
        NULL
    });
    BoolPage_init(&menu_main_debug_fl_en, "enable", &flEn, 0x03);
    FloatPage_init(&menu_main_debug_fl_rb, "rt_bk", &flRb, -1.57, 1.57);
    FloatPage_init(&menu_main_debug_fl_rf, "rt_fd", &flRf, -1.57, 1.57);
    FloatPage_init(&menu_main_debug_fl_lf, "lt_fd", &flLf, -1.57, 1.57);
    FloatPage_init(&menu_main_debug_fl_lb, "lt_bk", &flLb, -1.57, 1.57);
    ListPage_init(&menu_main_debug_fwp, "forceWPos", (Page*[]){
        &menu_main_debug_fwp_en,
        &menu_main_debug_fwp_lx,
        &menu_main_debug_fwp_lz,
        &menu_main_debug_fwp_rx,
        &menu_main_debug_fwp_rz,
        NULL
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
    BoolPage_init(&menu_main_debug_frb, "frb", &fRb, 0x03);
    FuncPage_init(&menu_main_debug_jump, "jump", jump);

    beepLong();
    ips200_clear();
    for(;;){
        // 此处编写需要循环执行的代码
        PageKey_press(&menu_main, pressed);
        PageKey_print(&menu_main, 0);
        Image_showCamera(0, 200);
        ips200_show_uint(188,200,fps,4);
        ips200_show_uint(188,216,cameraStatus,3);
//        printf("%d\n", g_camera_mid_err);
//        printf("%f, %f, %f\r\n", pitch, roll, yaw);
        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
