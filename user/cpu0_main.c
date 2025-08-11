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
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
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
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
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
        // �˴���д��Ҫѭ��ִ�еĴ���
        PageKey_press(&menu_main, pressed);
        PageKey_print(&menu_main, 0);
        MyCamera_Show(200);
//        printf("%d\n", g_camera_mid_err);
//        printf("%f, %f, %f\r\n", pitch, roll, yaw);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
