/*
 * Menu_param.c
 *
 *  Created on: 2025��4��21��
 *      Author: minec
 */

#include "Menu_param.h"

#pragma section all "cpu0_dsram"

Page menu_main;
Page menu_main_carStatus;
Page menu_main_config;
Page menu_main_config_volume;
Page menu_main_config_mod;
Page menu_main_config_mod_gyro;
Page menu_main_config_mod_foc;
Page menu_main_config_mod_flash;
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
Page menu_main_arg_k_v_bridge;
Page menu_main_arg_k_camera;
Page menu_main_arg_k_camera_vignette;
Page menu_main_arg_k_camera_vignette_k;
Page menu_main_arg_k_camera_bin;
Page menu_main_arg_k_camera_bin_status;
Page menu_main_arg_k_camera_bin_deltaT;
Page menu_main_arg_k_camera_trapezoid;
Page menu_main_arg_k_camera_trapezoid_k;
Page menu_main_arg_k_camera_trapezoid_y;
Page menu_main_arg_k_camera_start;
Page menu_main_arg_k_camera_start_maxYAdd;
Page menu_main_arg_k_camera_eigenvalue;
Page menu_main_arg_k_camera_eigenvalue_inf;
Page menu_main_arg_k_camera_eigenvalue_inf_bly2RDL;
Page menu_main_arg_k_camera_eigenvalue_inf_RD2IErr;
Page menu_main_arg_k_camera_eigenvalue_facingErr;
Page menu_main_arg_k_camera_eigenvalue_straight;
Page menu_main_arg_k_camera_eigenvalue_straight_yMin;
Page menu_main_arg_k_camera_eigenvalue_straight_step;
Page menu_main_arg_k_camera_eigenvalue_straight_err;
Page menu_main_arg_k_camera_eigenvalue_setLineY;
Page menu_main_arg_k_camera_eigenvalue_err;
Page menu_main_arg_k_camera_eigenvalue_err_y;
Page menu_main_arg_k_camera_eigenvalue_err_deltaY;
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
Page menu_main_arg_k_camera_e_circle_line;
Page menu_main_arg_k_camera_e_ramp;
Page menu_main_arg_k_camera_e_ramp_s;
Page menu_main_arg_k_camera_e_ramp_x;
Page menu_main_arg_k_camera_e_ramp_y;
Page menu_main_arg_k_camera_e_ramp_z;
Page menu_main_arg_k_camera_e_ramp_k;
Page menu_main_arg_k_camera_e_barrier;
Page menu_main_arg_k_camera_e_barrier_y0;
Page menu_main_arg_k_camera_e_barrier_y1;
Page menu_main_arg_k_camera_e_barrier_zMax;
Page menu_main_arg_k_camera_e_barrier_zMin;
Page menu_main_arg_k_camera_e_barrier_t;
Page menu_main_arg_k_camera_e_bridge;
Page menu_main_arg_k_camera_e_bridge_y;
Page menu_main_arg_k_camera_e_bridge_kPitchX;
Page menu_main_arg_k_camera_e_bridge_ti;
Page menu_main_arg_k_camera_e_bridge_to;
Page menu_main_arg_k_camera_e_bridge_s;
Page menu_main_arg_k_camera_e_bridge_z;
Page menu_main_arg_k_camera_e_bridge_detect;
Page menu_main_arg_k_camera_e_other;
Page menu_main_arg_k_camera_e_other_bPow;
Page menu_main_arg_k_camera_show;
Page menu_main_arg_k_camera_show_pInC1;
Page menu_main_arg_k_camera_show_wait;
Page menu_main_arg_k_jump;
Page menu_main_arg_k_jump_step0;
Page menu_main_arg_k_jump_step1;
Page menu_main_arg_k_jump_step2;
Page menu_main_arg_PID;
PidPage menu_main_arg_PID_vAy;
PidPage menu_main_arg_PID_xAy;
PidPage menu_main_arg_PID_vVx;
PidPage menu_main_arg_PID_vAz;
PidPage menu_main_arg_PID_turn;
Page menu_main_arg_filter;
Page menu_main_arg_filter_speed;
Page menu_main_arg_filter_turn;
Page menu_main_arg_filter_xAx0;
Page menu_main_arg_filter_xAx1;
Page menu_main_debug;
Page menu_main_debug_wheelClear;
Page menu_main_debug_flash;
Page menu_main_debug_flash_write;
Page menu_main_debug_flash_read;
Page menu_main_debug_flash_clear;
Page menu_main_debug_vofa;
Page menu_main_debug_vofa_send;
Page menu_main_debug_vofa_receive;
Page menu_main_debug_wifiIm;
Page menu_main_debug_fs;
Page menu_main_debug_fs_en;
Page menu_main_debug_fs_speed;
Page menu_main_debug_flz;
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
Page menu_main_debug_fcd;
Page menu_main_debug_jump;
Page menu_main_about;

void Menu_param_init(){
    Menu_init((char*[]){"main.carStatus", "main.arg.k.camera.status", "main.debug", "main.arg.k.camera.show"});
    Page_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", (Page*[]){
        &menu_main_carStatus,
        &menu_main_config,
        &menu_main_arg,
        &menu_main_debug,
        &menu_main_about,
        NULL
    });
    EnumPage_init(&menu_main_carStatus, "carStatus", &carStatus, (char*[]){
        "stop",
        "start",
        "balance",
        "run",
        ""
    });
    menu_main_carStatus.update = CarStatus_menu_update;
    ListPage_init(&menu_main_config, "config", (Page*[]){
        &menu_main_config_volume,
        &menu_main_config_mod,
        NULL
    });
    IntPage_init(&menu_main_config_volume, "volume", &volume, 0, 3);
    ListPage_init(&menu_main_config_mod, "mod", (Page*[]){
        &menu_main_config_mod_gyro,
        &menu_main_config_mod_foc,
        &menu_main_config_mod_flash,
        NULL
    });
    FuncPage_init(&menu_main_config_mod_gyro, "gyro", Gyro_set);
    FuncPage_init(&menu_main_config_mod_foc, "foc", MotorZero);
    FuncPage_init(&menu_main_config_mod_flash, "flash", Flash_clear);
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
        &menu_main_arg_k_v_bridge,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_v_straight, "straight", &targetV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bend, "bend", &bendV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_circle, "circle", &circleV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_ramp, "ramp", &rampV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bridge, "bridge", &bridgeV, -10000, 10000);
    menu_main_arg_k_v_straight.extends.floatValue.dot = menu_main_arg_k_v_bend.extends.floatValue.dot = menu_main_arg_k_v_circle.extends.floatValue.dot =
            menu_main_arg_k_v_ramp.extends.floatValue.dot = menu_main_arg_k_v_bridge.extends.floatValue.dot = 4;
    ListPage_init(&menu_main_arg_k_camera, "camera", (Page*[]){
        &menu_main_arg_k_camera_vignette,
        &menu_main_arg_k_camera_bin,
        &menu_main_arg_k_camera_trapezoid,
        &menu_main_arg_k_camera_eigenvalue,
        &menu_main_arg_k_camera_status,
        &menu_main_arg_k_camera_e,
        &menu_main_arg_k_camera_show,
        NULL
    });
    ListPage_init(&menu_main_arg_k_camera_vignette, "vignette", (Page*[]){
        &menu_main_arg_k_camera_vignette_k,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_camera_vignette_k, "k", &vignetteK, 0, 255);
    ListPage_init(&menu_main_arg_k_camera_bin, "bin", (Page*[]){
        &menu_main_arg_k_camera_bin_status,
        &menu_main_arg_k_camera_bin_deltaT,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_bin_status, "status", &binStatus, 0x03);
    IntPage_init(&menu_main_arg_k_camera_bin_deltaT, "deltaT", &binDeltaT, -256, 255);
    ListPage_init(&menu_main_arg_k_camera_trapezoid, "trapezoid", (Page*[]){
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
    IntPage_init(&menu_main_arg_k_camera_start_maxYAdd, "maxYAdd", &startMaxYAdd, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_eigenvalue, "eigenvalue", (Page*[]){
        &menu_main_arg_k_camera_eigenvalue_inf,
        &menu_main_arg_k_camera_eigenvalue_facingErr,
        &menu_main_arg_k_camera_eigenvalue_straight,
        &menu_main_arg_k_camera_eigenvalue_setLineY,
        &menu_main_arg_k_camera_eigenvalue_err,
        NULL
    });
    ListPage_init(&menu_main_arg_k_camera_eigenvalue_inf, "inf", (Page*[]){
        &menu_main_arg_k_camera_eigenvalue_inf_bly2RDL,
        &menu_main_arg_k_camera_eigenvalue_inf_RD2IErr,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_inf_bly2RDL, "bly2RDL", &bly2RDL, 0, MAX_BLY);
    FloatPage_init(&menu_main_arg_k_camera_eigenvalue_inf_RD2IErr, "RD2IErr", &RD2IErr, 0, 1.57);
    FloatPage_init(&menu_main_arg_k_camera_eigenvalue_facingErr, "facingErr", &facingErr, 0, 0.7854);
    menu_main_arg_k_camera_eigenvalue_facingErr.extends.floatValue.dot = 0;
    ListPage_init(&menu_main_arg_k_camera_eigenvalue_straight, "straight", (Page*[]){
        &menu_main_arg_k_camera_eigenvalue_straight_yMin,
        &menu_main_arg_k_camera_eigenvalue_straight_step,
        &menu_main_arg_k_camera_eigenvalue_straight_err,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_straight_yMin, "yMin", &straightYMin, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_straight_step, "step", &straightStep, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_eigenvalue_straight_err, "err", &straightErr, 0, 1.57);
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_setLineY, "setLineY", &setLineY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_eigenvalue_err, "err", (Page*[]){
        &menu_main_arg_k_camera_eigenvalue_err_y,
        &menu_main_arg_k_camera_eigenvalue_err_deltaY,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_err_y, "y", &errY, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_err_deltaY, "deltaY", &errDeltaY, 0, MT9V03X_H);
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
            "I_CROSS",
            "R_CROSS",
            "O_CROSS",
            ""
        });
    FloatPage_init(&menu_main_arg_k_camera_status_jump, "jump", &statusJump, 0, 0xFF);
    FloatPage_init(&menu_main_arg_k_camera_status_k, "k", &statusK, 0, 1);
    ListPage_init(&menu_main_arg_k_camera_e, "element", (Page*[]){
        &menu_main_arg_k_camera_e_zebra,
        &menu_main_arg_k_camera_e_cross,
        &menu_main_arg_k_camera_e_circle,
        &menu_main_arg_k_camera_e_ramp,
        &menu_main_arg_k_camera_e_barrier,
        &menu_main_arg_k_camera_e_bridge,
        &menu_main_arg_k_camera_e_other,
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
        &menu_main_arg_k_camera_e_circle_line,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_circle_x, "x", &circleX, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_circle_line, "line", &circleLine, 0, 1);
    menu_main_arg_k_camera_e_circle_line.extends.floatValue.dot = 0;
    ListPage_init(&menu_main_arg_k_camera_e_ramp, "ramp", (Page*[]){
        &menu_main_arg_k_camera_e_ramp_s,
        &menu_main_arg_k_camera_e_ramp_x,
        &menu_main_arg_k_camera_e_ramp_y,
        &menu_main_arg_k_camera_e_ramp_z,
        &menu_main_arg_k_camera_e_ramp_k,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_ramp_s, "s", &rampS, 0, 10000000);
    IntPage_init(&menu_main_arg_k_camera_e_ramp_x, "x", &rampX, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_ramp_y, "y", &rampY, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_ramp_z, "z", &rampZ, -LEG_MAX_Z, -LEG_MIN_Z);
    FloatPage_init(&menu_main_arg_k_camera_e_ramp_k, "k", &rampK, 0, 1);
    ListPage_init(&menu_main_arg_k_camera_e_barrier, "barrier", (Page*[]){
        &menu_main_arg_k_camera_e_barrier_y0,
        &menu_main_arg_k_camera_e_barrier_y1,
        &menu_main_arg_k_camera_e_barrier_zMax,
        &menu_main_arg_k_camera_e_barrier_zMin,
        &menu_main_arg_k_camera_e_barrier_t,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y0, "y0", &barrierY0, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y1, "y1", &barrierY1, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_barrier_zMax, "zMax", &jumpLegZMax, -LEG_MAX_Z, -LEG_MIN_Z);
    FloatPage_init(&menu_main_arg_k_camera_e_barrier_zMin, "zMin", &jumpLegZMin, -LEG_MAX_Z, -LEG_MIN_Z);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_t, "t", &barrierT, 0, 100000);
    ListPage_init(&menu_main_arg_k_camera_e_bridge, "bridge", (Page*[]){
        &menu_main_arg_k_camera_e_bridge_y,
        &menu_main_arg_k_camera_e_bridge_kPitchX,
        &menu_main_arg_k_camera_e_bridge_ti,
        &menu_main_arg_k_camera_e_bridge_to,
        &menu_main_arg_k_camera_e_bridge_s,
        &menu_main_arg_k_camera_e_bridge_z,
        &menu_main_arg_k_camera_e_bridge_detect,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_bridge_y, "y", &bridgeY, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_bridge_kPitchX, "kPitchX", &bridgeKPitchX, -100, 100);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_ti, "ti", &bridgeTI, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_to, "to", &bridgeTO, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_s, "s", &bridgeS, 0, 10000000);
    FloatPage_init(&menu_main_arg_k_camera_e_bridge_z, "z", &bridgeZ, -LEG_MAX_Z, -LEG_MIN_Z);
    BoolPage_init(&menu_main_arg_k_camera_e_bridge_detect, "detectL", &bridgeDetectL, 0x03);
    ListPage_init(&menu_main_arg_k_camera_e_other, "other", (Page*[]){
        &menu_main_arg_k_camera_e_other_bPow,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_camera_e_other_bPow, "bPow", &otherBPow, 0, 100);
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
        NULL
    });
    IntPage_init(&menu_main_arg_k_jump_step0, "0", &(jumpStep[0]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step1, "1", &(jumpStep[1]), 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step2, "2", &(jumpStep[2]), 0, 1000);
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
    ListPage_init(&menu_main_arg_filter, "Filter", (Page*[]){
        &menu_main_arg_filter_turn,
        &menu_main_arg_filter_speed,
        &menu_main_arg_filter_xAx0,
        &menu_main_arg_filter_xAx1,
        NULL
    });
    FloatPage_init(&menu_main_arg_filter_turn, "turn", &Filter1_turn.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_speed, "speed", &Filter1_speed.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_xAx0, "xAx0", &Filter0_xAx.delta, 0, 10000);
    FloatPage_init(&menu_main_arg_filter_xAx1, "xAx1", &Filter1_xAx.alpha, 0, 1);
    ListPage_init(&menu_main_debug, "debug", (Page*[]){
        &menu_main_debug_wheelClear,
        &menu_main_debug_flash,
        &menu_main_debug_vofa,
        &menu_main_debug_wifiIm,
        &menu_main_debug_fs,
        &menu_main_debug_flz,
        &menu_main_debug_fl,
        &menu_main_debug_fwp,
        &menu_main_debug_frb,
        &menu_main_debug_fcd,
        &menu_main_debug_jump,
        NULL
    });
    BoolPage_init(&menu_main_debug_wheelClear, "wClear", &wheelClear, 0x03);
    ListPage_init(&menu_main_debug_flash, "flash", (Page*[]){
        &menu_main_debug_flash_write,
        &menu_main_debug_flash_read,
        &menu_main_debug_flash_clear,
        NULL
    });
    FuncPage_init(&menu_main_debug_flash_write, "write", Flash_pageAllWrite);
    FuncPage_init(&menu_main_debug_flash_read, "read", Flash_pageAllRead);
    FuncPage_init(&menu_main_debug_flash_clear, "clear", Flash_menuClear);
    ListPage_init(&menu_main_debug_vofa, "vofa", (Page*[]){
        &menu_main_debug_vofa_send,
        &menu_main_debug_vofa_receive,
        NULL
    });
    FuncPage_init(&menu_main_debug_vofa_send, "send", Vofa_pageAllSend);
    FuncPage_init(&menu_main_debug_vofa_receive, "receive", Vofa_pageAllReceive);
    FuncPage_init(&menu_main_debug_wifiIm, "wifiIm", Wifi_Image_Init);
    ListPage_init(&menu_main_debug_fs, "forceSpeed", (Page*[]){
        &menu_main_debug_fs_en,
        &menu_main_debug_fs_speed,
        NULL
    });
    BoolPage_init(&menu_main_debug_fs_en, "enable", &fsEn, 0x03);
    IntPage_init(&menu_main_debug_fs_speed, "speed", &fsSpeed, -10000, 10000);
    BoolPage_init(&menu_main_debug_flz, "legZero", &fLz, 0x03);
    ListPage_init(&menu_main_debug_fl, "forceLeg", (Page*[]){
        &menu_main_debug_fl_en,
        &menu_main_debug_fl_rb,
        &menu_main_debug_fl_rf,
        &menu_main_debug_fl_lf,
        &menu_main_debug_fl_lb,
        NULL
    });
    BoolPage_init(&menu_main_debug_fl_en, "enable", &flEn, 0x03);
    FloatPage_init(&menu_main_debug_fl_rb, "rt_bk", &flRb, -3.14, 3.14);
    FloatPage_init(&menu_main_debug_fl_rf, "rt_fd", &flRf, -3.14, 3.14);
    FloatPage_init(&menu_main_debug_fl_lf, "lt_fd", &flLf, -3.14, 3.14);
    FloatPage_init(&menu_main_debug_fl_lb, "lt_bk", &flLb, -3.14, 3.14);
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
    BoolPage_init(&menu_main_debug_fcd, "fcd", &fCd, 0x03);
    FuncPage_init(&menu_main_debug_jump, "jump", jump);
    AboutPage_init(&menu_main_about, Stl, StlNumber);
}

#pragma section all restore
