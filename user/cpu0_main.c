/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "cpu0_main.h"

#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************

uint8 pressed[KEY_NUM] = {0};

Page menu_main;
Page menu_main_carRun;
Page menu_main_arg;
Page menu_main_arg_k;
Page menu_main_arg_k_kZero;
Page menu_main_arg_k_speedF;
Page menu_main_arg_k_camera;
Page menu_main_arg_k_camera_bin;
Page menu_main_arg_k_camera_bin_deltaT;
Page menu_main_arg_k_camera_i;
Page menu_main_arg_k_camera_i_bly2RDR;
Page menu_main_arg_k_camera_i_RD2IE;
Page menu_main_arg_k_camera_IGFE;
Page menu_main_arg_k_camera_straight;
Page menu_main_arg_k_camera_straight_step;
Page menu_main_arg_k_camera_straight_err;
Page menu_main_arg_k_camera_e;
Page menu_main_arg_k_camera_e_cross;
Page menu_main_arg_k_camera_e_cross_y;
Page menu_main_arg_k_camera_e_cross_x;
Page menu_main_arg_k_camera_e_circle;
Page menu_main_arg_k_camera_e_circle_y;
Page menu_main_arg_k_camera_e_circle_x;
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
Page menu_main_arg_k_turnA;
Page menu_main_arg_PID;
PidPage menu_main_arg_PID_vAy;
PidPage menu_main_arg_PID_xAy;
PidPage menu_main_arg_PID_vVx;
PidPage menu_main_arg_PID_vAz;
PidPage menu_main_arg_PID_turn;
PidPage menu_main_arg_PID_xAx;
Page menu_main_mod;
Page menu_main_mod_gyro;
Page menu_main_debug;
Page menu_main_debug_wifiIm;
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

void core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
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
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    ListPage_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", (Page*[]){
        &menu_main_carRun,
        &menu_main_arg,
        &menu_main_mod,
        &menu_main_debug,
        NULL
    });
    EnumPage_init(&menu_main_carRun, "car_run", &carStatus, (char*[]){
            "stop",
            "balance",
            "run",
            "",
    });
    ListPage_init(&menu_main_arg, "arg", (Page*[]){
        &menu_main_arg_k,
        &menu_main_arg_PID,
        NULL
    });
    ListPage_init(&menu_main_arg_k, "k", (Page*[]){
        &menu_main_arg_k_kZero,
        &menu_main_arg_k_camera,
        &menu_main_arg_k_jump,
        &menu_main_arg_k_turnA,
        NULL
    });
    FloatPage_init(&menu_main_arg_k_kZero, "kZero", &kZero, -30, 30);
    menu_main_arg_k_kZero.extends.floatValue.dot = 1;
    ListPage_init(&menu_main_arg_k_camera, "camera", (Page*[]){
        &menu_main_arg_k_camera_bin,
        &menu_main_arg_k_camera_i,
        &menu_main_arg_k_camera_IGFE,
        &menu_main_arg_k_camera_straight,
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
    ListPage_init(&menu_main_arg_k_camera_i, "i", (Page*[]){
        &menu_main_arg_k_camera_i_bly2RDR,
        &menu_main_arg_k_camera_i_RD2IE,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_i_bly2RDR, "bly2RDR", &bly2RDR, 0, MT9V03X_W);
    FloatPage_init(&menu_main_arg_k_camera_i_RD2IE, "RD2IE", &RD2IE, 0, 1.57);
    menu_main_arg_k_camera_i_RD2IE.extends.floatValue.dot = 1;
    FloatPage_init(&menu_main_arg_k_camera_IGFE, "RD2IE", &IGFE, 0, 0.7854);
    menu_main_arg_k_camera_IGFE.extends.floatValue.dot = 0;
    ListPage_init(&menu_main_arg_k_camera_straight, "straight", (Page*[]){
        &menu_main_arg_k_camera_straight_step,
        &menu_main_arg_k_camera_straight_err,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_straight_step, "step", &StraightStep, 0, MT9V03X_H);
    FloatPage_init(&menu_main_arg_k_camera_straight_err, "err", &StraightErr, 0, 1.57);
    menu_main_arg_k_camera_straight_err.extends.floatValue.dot = 1;
    ListPage_init(&menu_main_arg_k_camera_e, "element", (Page*[]){
        &menu_main_arg_k_camera_e_cross,
        &menu_main_arg_k_camera_e_circle,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_setLineY, "setLineY", &setLineY, 0, MT9V03X_H);
    ListPage_init(&menu_main_arg_k_camera_e_cross, "cross", (Page*[]){
        &menu_main_arg_k_camera_e_cross_y,
        &menu_main_arg_k_camera_e_cross_x,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_cross_y, "y", &crossY, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_e_cross_x, "x", &crossX, 0, MT9V03X_W);
    ListPage_init(&menu_main_arg_k_camera_e_circle, "cross", (Page*[]){
        &menu_main_arg_k_camera_e_circle_y,
        &menu_main_arg_k_camera_e_circle_x,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_e_circle_y, "y", &circleY, 0, MT9V03X_W);
    IntPage_init(&menu_main_arg_k_camera_e_circle_x, "x", &circleX, 0, MT9V03X_W);
    ListPage_init(&menu_main_arg_k_camera_err, "err", (Page*[]){
        &menu_main_arg_k_camera_err_y,
        &menu_main_arg_k_camera_err_deltaY,
        NULL
    });
    IntPage_init(&menu_main_arg_k_camera_err_y, "y", &errY, 0, MT9V03X_H);
    IntPage_init(&menu_main_arg_k_camera_err_deltaY, "deltaY", &errDeltaY, 0, MT9V03X_W);
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
    FloatPage_init(&menu_main_arg_k_turnA, "turn_a", &Filter_turn.alpha, 0, 1);
    ListPage_init(&menu_main_arg_PID, "PID", (Page*[]){
        &menu_main_arg_PID_vAy,
        &menu_main_arg_PID_xAy,
        &menu_main_arg_PID_vVx,
        &menu_main_arg_PID_vAz,
        &menu_main_arg_PID_turn,
        &menu_main_arg_PID_xAx,
        NULL
    });
    PidPage_init(&menu_main_arg_PID_vAy, "vAy", &PID_WvAy);
    PidPage_init(&menu_main_arg_PID_xAy, "xAy", &PID_WxAy);
    PidPage_init(&menu_main_arg_PID_vVx, "vVx", &PID_vVx);
    PidPage_init(&menu_main_arg_PID_vAz, "vAz", &PID_vAz);
    PidPage_init(&menu_main_arg_PID_turn, "turn", &PID_WvAz);
    PidPage_init(&menu_main_arg_PID_xAx, "xAx", &PID_xAx);
    ListPage_init(&menu_main_mod, "mod", (Page*[]){
        &menu_main_mod_gyro,
        NULL
    });
    FuncPage_init(&menu_main_mod_gyro, "gyro", gyro_set);
    ListPage_init(&menu_main_debug, "debug", (Page*[]){
        &menu_main_debug_wifiIm,
        &menu_main_debug_fs,
        &menu_main_debug_fl,
        &menu_main_debug_fv,
        &menu_main_debug_fwp,
        &menu_main_debug_ffRow,
        &menu_main_debug_jump,
        NULL
    });
    FuncPage_init(&menu_main_debug_wifiIm, "wifiIm", Wifi_Image_Init);
    ListPage_init(&menu_main_debug_fs, "forceSpeed", (Page*[]){
        &menu_main_debug_fs_en,
        &menu_main_debug_fs_speed,
        NULL
    });
    BoolPage_init(&menu_main_debug_fs_en, "enable", &fsEn, 0x03);
    IntPage_init(&menu_main_debug_fs_speed, "speed", &fsSpeed, -10000, 10000);
    ListPage_init(&menu_main_debug_fv, "forceV", (Page*[]){
        &menu_main_debug_fv_en,
        &menu_main_debug_fv_v,
        NULL
    });
    BoolPage_init(&menu_main_debug_fv_en, "enable", &fvEn, 0x03);
    IntPage_init(&menu_main_debug_fv_v, "V", &fvV, -1000, 1000);
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
    menu_main_debug_fl_lb.extends.floatValue.dot
    =menu_main_debug_fl_lf.extends.floatValue.dot
    =menu_main_debug_fl_rf.extends.floatValue.dot
    =menu_main_debug_fl_rb.extends.floatValue.dot=1;
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
    BoolPage_init(&menu_main_debug_ffRow, "ffRow", &ffRow, 0x03);
    FuncPage_init(&menu_main_debug_jump, "jump", jump);

    beepLong();
    ips200_clear();
    PageKey_print(&menu_main, 0);
    for(;;){
        // �˴���д��Ҫѭ��ִ�еĴ���
        PageKey_press(&menu_main, pressed);
        PageKey_print(&menu_main, 0);
        MyCamera_Show(0, 200);
        ips200_show_int(188,200,fps,4);
        ips200_show_int(188,216,cameraStatus,4);
        Wifi_Image_Send_Camera();
//        printf("%d\n", g_camera_mid_err);
//        printf("%f, %f, %f\r\n", pitch, roll, yaw);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
