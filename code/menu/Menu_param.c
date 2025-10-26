/*
 * Menu_param.c
 *
 *  Created on: 2025Äê4ÔÂ21ÈÕ
 *      Author: MCSTL
 */

#include "Menu_param.h"

#pragma section all "cpu0_dsram"

Page menu_main;
Page menu_main_carStatus;
Page menu_main_config;
Page menu_main_config_volume;
Page menu_main_config_init;
Page menu_main_config_init_ips200;
Page menu_main_config_init_camera;
Page menu_main_config_init_gyro;
Page menu_main_config_mod;
Page menu_main_config_mod_gyro;
Page menu_main_config_mod_gyroValue;
Page menu_main_config_mod_foc;
Page menu_main_config_mod_flash;
Page menu_main_config_motor;
Page menu_main_config_motor_lDir;
Page menu_main_config_motor_rDir;
Page menu_main_config_leg;
Page menu_main_config_leg_rb;
Page menu_main_config_leg_rf;
Page menu_main_config_leg_lf;
Page menu_main_config_leg_lb;
Page menu_main_config_protect;
Page menu_main_config_protect_angle;
Page menu_main_config_protect_fps;
Page menu_main_config_protect_time;
Page menu_main_config_protect_time_en;
Page menu_main_config_protect_time_t;
Page menu_main_config_protect_time_nowT;
Page menu_main_config_protect_circle;
Page menu_main_config_protect_circle_en;
Page menu_main_config_protect_circle_n;
Page menu_main_config_protect_circle_t;
Page menu_main_config_protect_circle_nowN;
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
Page menu_main_arg_k_v_rampD;
Page menu_main_arg_k_v_barrier0;
Page menu_main_arg_k_v_barrier1;
Page menu_main_arg_k_v_bridge0;
Page menu_main_arg_k_v_bridge1;
Page menu_main_arg_k_errY;
Page menu_main_arg_k_errY_straight;
Page menu_main_arg_k_errY_bend;
Page menu_main_arg_k_errY_block;
Page menu_main_arg_k_errY_circle;
Page menu_main_arg_k_camera;
Page menu_main_arg_k_camera_cut;
Page menu_main_arg_k_camera_cut_l;
Page menu_main_arg_k_camera_cut_r;
Page menu_main_arg_k_camera_cut_t;
Page menu_main_arg_k_camera_cut_b;
Page menu_main_arg_k_camera_compensate;
Page menu_main_arg_k_camera_compensate_shadow;
Page menu_main_arg_k_camera_compensate_vignette;
Page menu_main_arg_k_camera_bin;
Page menu_main_arg_k_camera_bin_en;
Page menu_main_arg_k_camera_bin_deltaT;
Page menu_main_arg_k_camera_trapezoid;
Page menu_main_arg_k_camera_trapezoid_k;
Page menu_main_arg_k_camera_trapezoid_y;
Page menu_main_arg_k_camera_start;
Page menu_main_arg_k_camera_start_maxYAdd;
Page menu_main_arg_k_camera_start_reBlyY;
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
Page menu_main_arg_k_camera_eigenvalue_errDeltaY;
Page menu_main_arg_k_camera_status;
Page menu_main_arg_k_camera_status_now;
Page menu_main_arg_k_camera_status_jump;
Page menu_main_arg_k_camera_status_k;
Page menu_main_arg_k_camera_e;
Page menu_main_arg_k_camera_e_element;
Page menu_main_arg_k_camera_e_element_yMin;
Page menu_main_arg_k_camera_e_element_x;
Page menu_main_arg_k_camera_e_zebra;
Page menu_main_arg_k_camera_e_zebra_en;
Page menu_main_arg_k_camera_e_zebra_y;
Page menu_main_arg_k_camera_e_zebra_t;
Page menu_main_arg_k_camera_e_zebra_s;
Page menu_main_arg_k_camera_e_cross;
Page menu_main_arg_k_camera_e_cross_en;
Page menu_main_arg_k_camera_e_cross_y;
Page menu_main_arg_k_camera_e_block;
Page menu_main_arg_k_camera_e_block_lEn;
Page menu_main_arg_k_camera_e_block_rEn;
Page menu_main_arg_k_camera_e_block_y0;
Page menu_main_arg_k_camera_e_block_y1;
Page menu_main_arg_k_camera_e_circle;
Page menu_main_arg_k_camera_e_circle_lEn;
Page menu_main_arg_k_camera_e_circle_rEn;
Page menu_main_arg_k_camera_e_circle_x;
Page menu_main_arg_k_camera_e_ramp;
Page menu_main_arg_k_camera_e_ramp_en;
Page menu_main_arg_k_camera_e_ramp_s;
Page menu_main_arg_k_camera_e_ramp_y;
Page menu_main_arg_k_camera_e_ramp_z;
Page menu_main_arg_k_camera_e_ramp_k;
Page menu_main_arg_k_camera_e_barrier;
Page menu_main_arg_k_camera_e_barrier_en;
Page menu_main_arg_k_camera_e_barrier_y0;
Page menu_main_arg_k_camera_e_barrier_y10;
Page menu_main_arg_k_camera_e_barrier_y11;
Page menu_main_arg_k_camera_e_barrier_zMax;
Page menu_main_arg_k_camera_e_barrier_zMin;
Page menu_main_arg_k_camera_e_barrier_t;
Page menu_main_arg_k_camera_e_bridge;
Page menu_main_arg_k_camera_e_bridge_en;
Page menu_main_arg_k_camera_e_bridge_kPitchX;
Page menu_main_arg_k_camera_e_bridge_ti;
Page menu_main_arg_k_camera_e_bridge_to;
Page menu_main_arg_k_camera_e_bridge_s;
Page menu_main_arg_k_camera_e_bridge_z;
Page menu_main_arg_k_camera_e_bridge_detect;
Page menu_main_arg_k_camera_e_other;
Page menu_main_arg_k_camera_e_other_orCLcEn;
Page menu_main_arg_k_camera_e_other_orCRcEn;
Page menu_main_arg_k_camera_e_other_orRBEn;
Page menu_main_arg_k_camera_e_other_bPow;
Page menu_main_arg_k_camera_show;
Page menu_main_arg_k_camera_show_pInC1;
Page menu_main_arg_k_camera_show_wait;
Page menu_main_arg_k_camera_show_cARL;
Page menu_main_arg_k_camera_show_cARL_a;
Page menu_main_arg_k_camera_show_cARL_x00;
Page menu_main_arg_k_camera_show_cARL_x01;
Page menu_main_arg_k_camera_show_cARL_y0;
Page menu_main_arg_k_camera_show_cARL_x10;
Page menu_main_arg_k_camera_show_cARL_x11;
Page menu_main_arg_k_camera_show_cARL_y1;
Page menu_main_arg_k_jump;
Page menu_main_arg_k_jump_step0;
Page menu_main_arg_k_jump_step1;
Page menu_main_arg_k_jump_step2;
Page menu_main_arg_PID;
Page menu_main_arg_PID_gyro;
Page menu_main_arg_PID_gyro_Kp;
Page menu_main_arg_PID_gyro_Ki;
PidPage menu_main_arg_PID_vAy;
PidPage menu_main_arg_PID_xAy;
PidPage menu_main_arg_PID_WvXx;
PidPage menu_main_arg_PID_vXx;
PidPage menu_main_arg_PID_WvAz;
PidPage menu_main_arg_PID_vAz;
Page menu_main_arg_filter;
Page menu_main_arg_filter_speed;
Page menu_main_arg_filter_turn;
Page menu_main_arg_filter_vXx;
Page menu_main_arg_filter_xAx;
Page menu_main_arg_filter_zzzXAx;
Page menu_main_dat;
Page menu_main_dat_allRunMs;
Page menu_main_dat_encoder;
Page menu_main_dat_encoder_l;
Page menu_main_dat_encoder_r;
Page menu_main_dat_gyro;
Page menu_main_dat_gyro_gyro;
Page menu_main_dat_gyro_gyro_x;
Page menu_main_dat_gyro_gyro_y;
Page menu_main_dat_gyro_gyro_z;
Page menu_main_dat_gyro_gyro_vAx;
Page menu_main_dat_gyro_gyro_vAy;
Page menu_main_dat_gyro_gyro_vAz;
Page menu_main_dat_gyro_gyro_xAx;
Page menu_main_dat_gyro_gyro_xAy;
Page menu_main_dat_gyro_gyro_xAz;
Page menu_main_dat_gyro_acc;
Page menu_main_dat_gyro_acc_x;
Page menu_main_dat_gyro_acc_y;
Page menu_main_dat_gyro_acc_z;
Page menu_main_dat_gyro_acc_aXx;
Page menu_main_dat_gyro_acc_aXy;
Page menu_main_dat_gyro_acc_aXz;
Page menu_main_dat_kill;
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
    Menu_init((char*[]){"main.carStatus", "main.arg.k.camera.status.now", "main.dat", "main.debug",
            "main.arg.k.camera.show", "main.config.protect.time", "main.config.protect.circle"});
    Page_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", (Page*[]){
        &menu_main_carStatus,
        &menu_main_config,
        &menu_main_arg,
        &menu_main_dat,
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
        &menu_main_config_init,
        &menu_main_config_mod,
        &menu_main_config_motor,
        &menu_main_config_leg,
        &menu_main_config_protect,
        NULL
    });
    IntPage_init(&menu_main_config_volume, "volume", &volume, 0, 3);
    ListPage_init(&menu_main_config_init, "init", (Page*[]){
        &menu_main_config_init_ips200,
        &menu_main_config_init_camera,
        &menu_main_config_init_gyro,
        NULL
    });
    FuncPage_init(&menu_main_config_init_ips200, "ips200", ips200_init_spi);
    FuncPage_init(&menu_main_config_init_camera, "camera", MyCamera_Init);
    FuncPage_init(&menu_main_config_init_gyro, "gyro", Gyro_init);
    ListPage_init(&menu_main_config_mod, "mod", (Page*[]){
        &menu_main_config_mod_gyro,
        &menu_main_config_mod_gyroValue,
        &menu_main_config_mod_foc,
        &menu_main_config_mod_flash,
        NULL
    });
    FuncPage_init(&menu_main_config_mod_gyro, "gyro", Gyro_set);
    FuncPage_init(&menu_main_config_mod_gyroValue, "gyroValue", Gyro_back);
    FuncPage_init(&menu_main_config_mod_foc, "foc", MotorZero);
    FuncPage_init(&menu_main_config_mod_flash, "flash", Flash_clear);
    ListPage_init(&menu_main_config_motor, "motor", (Page*[]){
        &menu_main_config_motor_lDir,
        &menu_main_config_motor_rDir,
        NULL
    });
    BoolPage_init(&menu_main_config_motor_lDir, "lDir", &Motor_dirL, 0x03);
    BoolPage_init(&menu_main_config_motor_rDir, "rDir", &Motor_dirR, 0x03);
    ListPage_init(&menu_main_config_leg, "leg", (Page*[]){
        &menu_main_config_leg_rb,
        &menu_main_config_leg_rf,
        &menu_main_config_leg_lf,
        &menu_main_config_leg_lb,
        NULL
    });
    FloatPage_init(&menu_main_config_leg_rb, "rb", &LegDRb, -LEG_MAX_R, LEG_MAX_R);
    FloatPage_init(&menu_main_config_leg_rf, "rf", &LegDRf, -LEG_MAX_R, LEG_MAX_R);
    FloatPage_init(&menu_main_config_leg_lf, "lf", &LegDLf, -LEG_MAX_R, LEG_MAX_R);
    FloatPage_init(&menu_main_config_leg_lb, "lb", &LegDLb, -LEG_MAX_R, LEG_MAX_R);
    ListPage_init(&menu_main_config_protect, "protect", (Page*[]){
        &menu_main_config_protect_angle,
        &menu_main_config_protect_fps,
        &menu_main_config_protect_time,
        &menu_main_config_protect_circle,
        NULL
    });
    BoolPage_init(&menu_main_config_protect_angle, "angle", &angleProtect, 0x03);
    BoolPage_init(&menu_main_config_protect_fps, "fps", &fpsProtect, 0x03);
    ListPage_init(&menu_main_config_protect_time, "time", (Page*[]){
        &menu_main_config_protect_time_en,
        &menu_main_config_protect_time_t,
        &menu_main_config_protect_time_nowT,
        NULL
    });
    BoolPage_init(&menu_main_config_protect_time_en, "en", &timeProtect, 0x03);
    IntPage_init(&menu_main_config_protect_time_t, "t", &timeProtectT, -2147483648, 2147483647);
    IntPage_init(&menu_main_config_protect_time_nowT, "nowT", &timeProtectNowT, -2147483648, 2147483647);
    ListPage_init(&menu_main_config_protect_circle, "circle", (Page*[]){
        &menu_main_config_protect_circle_en,
        &menu_main_config_protect_circle_n,
        &menu_main_config_protect_circle_t,
        &menu_main_config_protect_circle_nowN,
        NULL
    });
    BoolPage_init(&menu_main_config_protect_circle_en, "en", &circleProtect, 0x03);
    IntPage_init(&menu_main_config_protect_circle_n, "n", &circleProtectN, -2147483648, 2147483647);
    IntPage_init(&menu_main_config_protect_circle_t, "t", &circleProtectT, -2147483648, 2147483647);
    IntPage_init(&menu_main_config_protect_circle_nowN, "nowN", &circleProtectNowN, -2147483648, 2147483647);
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
        &menu_main_arg_k_errY,
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
        &menu_main_arg_k_v_rampD,
        &menu_main_arg_k_v_barrier0,
        &menu_main_arg_k_v_barrier1,
        &menu_main_arg_k_v_bridge0,
        &menu_main_arg_k_v_bridge1,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_v_straight, "straight", &targetV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bend, "bend", &bendV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_circle, "circle", &circleV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_rampD, "rampD", &rampDV, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_barrier0, "barrier0", &barrierV0, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_barrier1, "barrier1", &barrierV1, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bridge0, "bridge0", &bridgeV0, -10000, 10000);
    FloatPage_init(&menu_main_arg_k_v_bridge1, "bridge1", &bridgeV1, -10000, 10000);
    menu_main_arg_k_v_straight.extends.floatValue.dot = menu_main_arg_k_v_bend.extends.floatValue.dot = menu_main_arg_k_v_circle.extends.floatValue.dot =
            menu_main_arg_k_v_rampD.extends.floatValue.dot = menu_main_arg_k_v_barrier0.extends.floatValue.dot = menu_main_arg_k_v_barrier1.extends.floatValue.dot =
                    menu_main_arg_k_v_bridge0.extends.floatValue.dot = menu_main_arg_k_v_bridge1.extends.floatValue.dot = 5;
    ListPage_init(&menu_main_arg_k_errY, "errY", (Page*[]){
        &menu_main_arg_k_errY_straight,
        &menu_main_arg_k_errY_bend,
        &menu_main_arg_k_errY_block,
        &menu_main_arg_k_errY_circle,
        NULL
    });
    IntPage_init(&menu_main_arg_k_errY_straight, "straight", &errY, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_errY_bend, "bend", &bendErrY, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_errY_block, "block", &blockErrY, 0, MT9V03X_H-1);
    IntPage_init(&menu_main_arg_k_errY_circle, "circle", &circleErrY, 0, MT9V03X_H-1);
    ListPage_init(&menu_main_arg_k_camera, "camera", (Page*[]){
        &menu_main_arg_k_camera_cut,
        &menu_main_arg_k_camera_compensate,
        &menu_main_arg_k_camera_bin,
        &menu_main_arg_k_camera_trapezoid,
        &menu_main_arg_k_camera_start,
        &menu_main_arg_k_camera_eigenvalue,
        &menu_main_arg_k_camera_status,
        &menu_main_arg_k_camera_e,
        &menu_main_arg_k_camera_show,
        NULL
    });
    ListPage_init(&menu_main_arg_k_camera_cut, "cut", (Page*[]){
        &menu_main_arg_k_camera_cut_l,
        &menu_main_arg_k_camera_cut_r,
        &menu_main_arg_k_camera_cut_t,
        &menu_main_arg_k_camera_cut_b,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_cut_l, "l", &cutL, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_cut_r, "r", &cutR, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_cut_t, "t", &cutT, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_cut_b, "b", &cutB, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_compensate, "compensate", (Page*[]){
        &menu_main_arg_k_camera_compensate_shadow,
        &menu_main_arg_k_camera_compensate_vignette,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_camera_compensate_vignette, "vignetteK", &vignetteK, 0, 255);
    FloatPage_init(&menu_main_arg_k_camera_compensate_shadow, "shadowK", &shadowK, 0, 255);
    ListPage_init(&menu_main_arg_k_camera_bin, "bin", (Page*[]){
        &menu_main_arg_k_camera_bin_en,
        &menu_main_arg_k_camera_bin_deltaT,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_bin_en, "en", &binEn, 0x03);
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
        &menu_main_arg_k_camera_start_reBlyY,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_start_maxYAdd, "maxYAdd", &startMaxYAdd, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_start_reBlyY, "reBlyY", &reBlyY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_eigenvalue, "eigenvalue", (Page*[]){
        &menu_main_arg_k_camera_eigenvalue_inf,
        &menu_main_arg_k_camera_eigenvalue_facingErr,
        &menu_main_arg_k_camera_eigenvalue_straight,
        &menu_main_arg_k_camera_eigenvalue_setLineY,
        &menu_main_arg_k_camera_eigenvalue_errDeltaY,
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
    IntPage_init(&menu_main_arg_k_camera_eigenvalue_errDeltaY, "errDeltaY", &errDeltaY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_status, "status", (Page*[]){
        &menu_main_arg_k_camera_status_now,
        &menu_main_arg_k_camera_status_jump,
        &menu_main_arg_k_camera_status_k,
        NULL
    });
    EnumPage_init(&menu_main_arg_k_camera_status_now, "now", &cameraStatus, (char*[]){
            "NONE",
            ""
        });
    FloatPage_init(&menu_main_arg_k_camera_status_jump, "jump", &statusJump, 0, 0xFF);
    FloatPage_init(&menu_main_arg_k_camera_status_k, "k", &statusK, 0, 1);
    ListPage_init(&menu_main_arg_k_camera_e, "element", (Page*[]){
        &menu_main_arg_k_camera_e_element,
        &menu_main_arg_k_camera_e_zebra,
        &menu_main_arg_k_camera_e_cross,
        &menu_main_arg_k_camera_e_block,
        &menu_main_arg_k_camera_e_circle,
        &menu_main_arg_k_camera_e_ramp,
        &menu_main_arg_k_camera_e_barrier,
        &menu_main_arg_k_camera_e_bridge,
        &menu_main_arg_k_camera_e_other,
        NULL
    });
    ListPage_init(&menu_main_arg_k_camera_e_element, "element", (Page*[]){
        &menu_main_arg_k_camera_e_element_yMin,
        &menu_main_arg_k_camera_e_element_x,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_element_yMin, "yMin", &elementYMin, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_element_x, "x", &elementX, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_zebra, "zebra", (Page*[]){
        &menu_main_arg_k_camera_e_zebra_en,
        &menu_main_arg_k_camera_e_zebra_y,
        &menu_main_arg_k_camera_e_zebra_t,
        &menu_main_arg_k_camera_e_zebra_s,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_zebra_en, "en", &zebraEn, 0x03);
    IntPage_init(&menu_main_arg_k_camera_e_zebra_y, "y", &zebraY, 0, MT9V03X_H-1-3);
    IntPage_init(&menu_main_arg_k_camera_e_zebra_t, "t", &zebraT, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_zebra_s, "s", &zebraS, 0, 10000000);
    ListPage_init(&menu_main_arg_k_camera_e_cross, "cross", (Page*[]){
        &menu_main_arg_k_camera_e_cross_en,
        &menu_main_arg_k_camera_e_cross_y,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_cross_en, "en", &crossEn, 0x03);
    IntPage_init(&menu_main_arg_k_camera_e_cross_y, "y", &crossY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_block, "block", (Page*[]){
        &menu_main_arg_k_camera_e_block_lEn,
        &menu_main_arg_k_camera_e_block_rEn,
        &menu_main_arg_k_camera_e_block_y0,
        &menu_main_arg_k_camera_e_block_y1,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_block_lEn, "lEn", &lBlockEn, 0x03);
    BoolPage_init(&menu_main_arg_k_camera_e_block_rEn, "rEn", &rBlockEn, 0x03);
    IntPage_init(&menu_main_arg_k_camera_e_block_y0, "y0", &blockY0, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_block_y1, "y1", &blockY1, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_circle, "circle", (Page*[]){
        &menu_main_arg_k_camera_e_circle_lEn,
        &menu_main_arg_k_camera_e_circle_rEn,
        &menu_main_arg_k_camera_e_circle_x,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_circle_lEn, "lEn", &lCircleEn, 0x03);
    BoolPage_init(&menu_main_arg_k_camera_e_circle_rEn, "rEn", &rCircleEn, 0x03);
    IntPage_init(&menu_main_arg_k_camera_e_circle_x, "x", &circleX, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_ramp, "ramp", (Page*[]){
        &menu_main_arg_k_camera_e_ramp_en,
        &menu_main_arg_k_camera_e_ramp_s,
        &menu_main_arg_k_camera_e_ramp_y,
        &menu_main_arg_k_camera_e_ramp_z,
        &menu_main_arg_k_camera_e_ramp_k,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_ramp_en, "en", &rampEn, 0x03);
    IntPage_init(&menu_main_arg_k_camera_e_ramp_s, "s", &rampS, 0, 10000000);
    IntPage_init(&menu_main_arg_k_camera_e_ramp_y, "y", &rampY, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_ramp_z, "z", &rampZ, -LEG_MAX_Z, -LEG_MIN_Z);
    FloatPage_init(&menu_main_arg_k_camera_e_ramp_k, "k", &rampK, 0, 1);
    ListPage_init(&menu_main_arg_k_camera_e_barrier, "barrier", (Page*[]){
        &menu_main_arg_k_camera_e_barrier_en,
        &menu_main_arg_k_camera_e_barrier_y0,
        &menu_main_arg_k_camera_e_barrier_y10,
        &menu_main_arg_k_camera_e_barrier_y11,
        &menu_main_arg_k_camera_e_barrier_zMax,
        &menu_main_arg_k_camera_e_barrier_zMin,
        &menu_main_arg_k_camera_e_barrier_t,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_barrier_en, "en", &barrierEn, 0x03);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y0, "y0", &barrierY0, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y10, "y10", &barrierY10, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_y11, "y11", &barrierY11, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_e_barrier_zMax, "zMax", &jumpLegZMax, -LEG_MAX_Z, -LEG_MIN_Z);
    FloatPage_init(&menu_main_arg_k_camera_e_barrier_zMin, "zMin", &jumpLegZMin, -LEG_MAX_Z, -LEG_MIN_Z);
    IntPage_init(&menu_main_arg_k_camera_e_barrier_t, "t", &barrierT, 0, 100000);
    ListPage_init(&menu_main_arg_k_camera_e_bridge, "bridge", (Page*[]){
        &menu_main_arg_k_camera_e_bridge_en,
        &menu_main_arg_k_camera_e_bridge_kPitchX,
        &menu_main_arg_k_camera_e_bridge_ti,
        &menu_main_arg_k_camera_e_bridge_to,
        &menu_main_arg_k_camera_e_bridge_s,
        &menu_main_arg_k_camera_e_bridge_z,
        &menu_main_arg_k_camera_e_bridge_detect,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_bridge_en, "en", &bridgeEn, 0x03);
    FloatPage_init(&menu_main_arg_k_camera_e_bridge_kPitchX, "kPitchX", &bridgeKPitchX, -100, 100);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_ti, "ti", &bridgeTI, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_to, "to", &bridgeTO, 0, 100000);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_s, "s", &bridgeS, 0, 10000000);
    FloatPage_init(&menu_main_arg_k_camera_e_bridge_z, "z", &bridgeZ, -LEG_MAX_Z, -LEG_MIN_Z);
    IntPage_init(&menu_main_arg_k_camera_e_bridge_detect, "detectL", &bridgeDetectL, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_other, "other", (Page*[]){
        &menu_main_arg_k_camera_e_other_orCLcEn,
        &menu_main_arg_k_camera_e_other_orCRcEn,
        &menu_main_arg_k_camera_e_other_orRBEn,
        &menu_main_arg_k_camera_e_other_bPow,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_e_other_orCLcEn, "orCLcEn", &orCLcEn, 0x03);
    BoolPage_init(&menu_main_arg_k_camera_e_other_orCRcEn, "orCRcEn", &orCRcEn, 0x03);
    BoolPage_init(&menu_main_arg_k_camera_e_other_orRBEn, "orRBEn", &orRBEn, 0x03);
    FloatPage_init(&menu_main_arg_k_camera_e_other_bPow, "bPow", &otherBPow, 0, 100);
    ListPage_init(&menu_main_arg_k_camera_show, "show", (Page*[]){
        &menu_main_arg_k_camera_show_pInC1,
        &menu_main_arg_k_camera_show_wait,
        &menu_main_arg_k_camera_show_cARL,
        NULL
    });
    BoolPage_init(&menu_main_arg_k_camera_show_pInC1, "pInC1", &showPInC1, 0x03);
    BoolPage_init(&menu_main_arg_k_camera_show_wait, "wait", &showWait, 0x03);
    ListPage_init(&menu_main_arg_k_camera_show_cARL, "cARL", (Page*[]){
        &menu_main_arg_k_camera_show_cARL_a,
        &menu_main_arg_k_camera_show_cARL_x00,
        &menu_main_arg_k_camera_show_cARL_x01,
        &menu_main_arg_k_camera_show_cARL_y0,
        &menu_main_arg_k_camera_show_cARL_x10,
        &menu_main_arg_k_camera_show_cARL_x11,
        &menu_main_arg_k_camera_show_cARL_y1,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_show_cARL_a, "alpha", &showCARL_a, 0, 255);
    IntPage_init(&menu_main_arg_k_camera_show_cARL_x00, "x00", &showCARL_x00, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_show_cARL_x01, "x01", &showCARL_x01, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_show_cARL_y0, "y0", &showCARL_y0, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_show_cARL_x10, "x10", &showCARL_x10, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_show_cARL_x11, "x11", &showCARL_x11, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_show_cARL_y1, "y1", &showCARL_y1, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_jump, "jump", (Page*[]){
        &menu_main_arg_k_jump_step0,
        &menu_main_arg_k_jump_step1,
        &menu_main_arg_k_jump_step2,
        NULL
    });
    IntPage_init(&menu_main_arg_k_jump_step0, "0", jumpStep+0, 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step1, "1", jumpStep+1, 0, 1000);
    IntPage_init(&menu_main_arg_k_jump_step2, "2", jumpStep+2, 0, 1000);
    ListPage_init(&menu_main_arg_PID, "PID", (Page*[]){
        &menu_main_arg_PID_gyro,
        &menu_main_arg_PID_vAy,
        &menu_main_arg_PID_xAy,
        &menu_main_arg_PID_WvXx,
        &menu_main_arg_PID_vXx,
        &menu_main_arg_PID_WvAz,
        &menu_main_arg_PID_vAz,
        NULL
    });
    ListPage_init(&menu_main_arg_PID_gyro, "gyro", (Page*[]){
        &menu_main_arg_PID_gyro_Kp,
        &menu_main_arg_PID_gyro_Ki,
        NULL
    });
    FloatPage_init(&menu_main_arg_PID_gyro_Kp, "Kp", &Gyro_Kp, 0, 10000);
    FloatPage_init(&menu_main_arg_PID_gyro_Ki, "Ki", &Gyro_Ki, 0, 10000);
    menu_main_arg_PID_gyro_Kp.extends.floatValue.dot = menu_main_arg_PID_gyro_Ki.extends.floatValue.dot = 4;
    PidPage_init(&menu_main_arg_PID_vAy, "vAy", &PID_vAy);
    PidPage_init(&menu_main_arg_PID_xAy, "xAy", &PID_xAy);
    PidPage_init(&menu_main_arg_PID_WvXx, "WvXx", &PID_WvXx);
    PidPage_init(&menu_main_arg_PID_vXx, "vXx", &PID_vXx);
    PidPage_init(&menu_main_arg_PID_WvAz, "WvAz", &PID_WvAz);
    PidPage_init(&menu_main_arg_PID_vAz, "vAz", &PID_vAz);
    ListPage_init(&menu_main_arg_filter, "Filter", (Page*[]){
        &menu_main_arg_filter_turn,
        &menu_main_arg_filter_speed,
        &menu_main_arg_filter_vXx,
        &menu_main_arg_filter_xAx,
        &menu_main_arg_filter_zzzXAx,
        NULL
    });
    FloatPage_init(&menu_main_arg_filter_turn, "turn", &Filter1_turn.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_speed, "speed", &Filter1_speed.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_vXx, "vXx", &Filter1_vXx.alpha, 0, 1);
    FloatPage_init(&menu_main_arg_filter_xAx, "xAx", &Filter0_xAx.delta, 0, 10000);
    FloatPage_init(&menu_main_arg_filter_zzzXAx, "ZZZ_xAx", &ZZZ_xAx, 0, 90);
    ListPage_init(&menu_main_dat, "dat", (Page*[]){
        &menu_main_dat_allRunMs,
        &menu_main_dat_encoder,
        &menu_main_dat_gyro,
        &menu_main_dat_kill,
        NULL
    });
    IntPage_init(&menu_main_dat_allRunMs, "allRunMs", &allRunMs, -2147483648, 2147483647);
    ListPage_init(&menu_main_dat_encoder, "encoder", (Page*[]){
        &menu_main_dat_encoder_l,
        &menu_main_dat_encoder_r,
        NULL
    });
    IntPage_init(&menu_main_dat_encoder_l, "l", &Encoder_speed_l, -2147483648, 2147483647);
    IntPage_init(&menu_main_dat_encoder_r, "r", &Encoder_speed_r, -2147483648, 2147483647);
    ListPage_init(&menu_main_dat_gyro, "gyro", (Page*[]){
        &menu_main_dat_gyro_gyro,
        &menu_main_dat_gyro_acc,
        NULL
    });
    ListPage_init(&menu_main_dat_gyro_gyro, "gyro", (Page*[]){
        &menu_main_dat_gyro_gyro_x,
        &menu_main_dat_gyro_gyro_y,
        &menu_main_dat_gyro_gyro_z,
        &menu_main_dat_gyro_gyro_vAx,
        &menu_main_dat_gyro_gyro_vAy,
        &menu_main_dat_gyro_gyro_vAz,
        &menu_main_dat_gyro_gyro_xAx,
        &menu_main_dat_gyro_gyro_xAy,
        &menu_main_dat_gyro_gyro_xAz,
        NULL
    });
    FloatPage_init(&menu_main_dat_gyro_gyro_x, "x_deg", &my_gyro_x, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_y, "y_deg", &my_gyro_y, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_z, "z_deg", &my_gyro_z, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_vAx, "vAx_rad", &gyro_x, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_vAy, "vAy_rad", &gyro_y, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_vAz, "vAz_rad", &gyro_z, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_xAx, "xAx_deg", &pitch, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_xAy, "xAy_deg", &roll, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_gyro_xAz, "xAz_deg", &yaw, -INFINITY, INFINITY);
    menu_main_dat_gyro_gyro_x.extends.floatValue.dot = menu_main_dat_gyro_gyro_y.extends.floatValue.dot = menu_main_dat_gyro_gyro_z.extends.floatValue.dot =
            menu_main_dat_gyro_gyro_vAx.extends.floatValue.dot = menu_main_dat_gyro_gyro_vAy.extends.floatValue.dot = menu_main_dat_gyro_gyro_vAz.extends.floatValue.dot =
            menu_main_dat_gyro_gyro_xAx.extends.floatValue.dot = menu_main_dat_gyro_gyro_xAy.extends.floatValue.dot = menu_main_dat_gyro_gyro_xAz.extends.floatValue.dot = 3;
    ListPage_init(&menu_main_dat_gyro_acc, "acc", (Page*[]){
        &menu_main_dat_gyro_acc_x,
        &menu_main_dat_gyro_acc_y,
        &menu_main_dat_gyro_acc_z,
        &menu_main_dat_gyro_acc_aXx,
        &menu_main_dat_gyro_acc_aXy,
        &menu_main_dat_gyro_acc_aXz,
        NULL
    });
    FloatPage_init(&menu_main_dat_gyro_acc_x, "x_g", &my_acc_x, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_acc_y, "y_g", &my_acc_y, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_acc_z, "z_g", &my_acc_z, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_acc_aXx, "aXx_mpss", &acc_x, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_acc_aXy, "aXy_mpss", &acc_y, -INFINITY, INFINITY);
    FloatPage_init(&menu_main_dat_gyro_acc_aXz, "aXz_mpss", &acc_z, -INFINITY, INFINITY);
    menu_main_dat_gyro_acc_x.extends.floatValue.dot = menu_main_dat_gyro_acc_y.extends.floatValue.dot = menu_main_dat_gyro_acc_z.extends.floatValue.dot =
            menu_main_dat_gyro_acc_aXx.extends.floatValue.dot = menu_main_dat_gyro_acc_aXy.extends.floatValue.dot = menu_main_dat_gyro_acc_aXz.extends.floatValue.dot = 3;
    BoolPage_init(&menu_main_dat_kill, "kill", &kill, 0x03);
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
    AboutPage_init(&menu_main_about, Stl, StlNumber, Zym, ZymNumber);
}

#pragma section all restore
