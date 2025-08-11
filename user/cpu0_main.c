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
const uint16 PIT00ms = 5;
const uint16 PIT01ms = 5;
const uint16 PIT10ms = 5;

uint8 pressed = 0;

Page menu_main;
Page menu_main_arg;
Page menu_main_arg_k;
Page menu_main_arg_k_kZero;
Page menu_main_arg_k_kLX2AY;
Page menu_main_arg_PID;
Page menu_main_arg_PID_vAy;
Page menu_main_arg_PID_vAy_Kp;
Page menu_main_arg_PID_vAy_Ki;
Page menu_main_arg_PID_vAy_Kd;
Page menu_main_arg_PID_xAy;
Page menu_main_arg_PID_xAy_Kp;
Page menu_main_arg_PID_xAy_Ki;
Page menu_main_arg_PID_xAy_Kd;
Page menu_main_arg_PID_vVx;
Page menu_main_arg_PID_vVx_Kp;
Page menu_main_arg_PID_vVx_Ki;
Page menu_main_arg_PID_vVx_Kd;
Page menu_main_arg_PID_lPitch;
Page menu_main_arg_PID_lPitch_Kp;
Page menu_main_arg_PID_lPitch_Ki;
Page menu_main_arg_PID_lPitch_Kd;
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
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    beep_init();
    ips200_init(IPS200_TYPE_SPI);
    gyroscope_init();
    key_init(PIT00ms);
    small_driver_uart_init();
    Leg_init();
    MyEncoder_Init();
    PID_param_init();
    pit_ms_init(CCU60_CH0, PIT00ms);
//    pit_ms_init(CCU60_CH1, PIT01ms);
    pit_ms_init(CCU61_CH0, PIT10ms);
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    ListPage_setRoot(&menu_main);
    ListPage_init(&menu_main, "main", 3, (Page*[]){
        &menu_main_arg,
        &menu_main_carRun,
        &menu_main_debug,
    });
    ListPage_init(&menu_main_arg, "arg", 2, (Page*[]){
        &menu_main_arg_k,
        &menu_main_arg_PID,
    });
    ListPage_init(&menu_main_arg_k, "k", 2, (Page*[]){
        &menu_main_arg_k_kZero,
        &menu_main_arg_k_kLX2AY,
    });
    FFloatPage_init(&menu_main_arg_k_kZero, "kZero", &kZero, -2, 2);
    menu_main_arg_k_kZero.extends.fFloatValue.dot = 1;
    FFloatPage_init(&menu_main_arg_k_kLX2AY, "kLX2AY", &kLX2AY, -2, 2);
    menu_main_arg_k_kLX2AY.extends.fFloatValue.dot = 1;
    ListPage_init(&menu_main_arg_PID, "PID", 4, (Page*[]){
        &menu_main_arg_PID_vAy,
        &menu_main_arg_PID_xAy,
        &menu_main_arg_PID_vVx,
        &menu_main_arg_PID_lPitch,
    });
    ListPage_init(&menu_main_arg_PID_vAy, "aAy", 3, (Page*[]){
        &menu_main_arg_PID_vAy_Kp,
        &menu_main_arg_PID_vAy_Ki,
        &menu_main_arg_PID_vAy_Kd,
    });
    FFloatPage_init(&menu_main_arg_PID_vAy_Kp, "Kp", &PID_WvAy.Kp, 0, 10000);
    FFloatPage_init(&menu_main_arg_PID_vAy_Ki, "Ki", &PID_WvAy.Ki, 0, 10000);
    FFloatPage_init(&menu_main_arg_PID_vAy_Kd, "Kd", &PID_WvAy.Kd, 0, 10000);
    ListPage_init(&menu_main_arg_PID_xAy, "vAy", 3, (Page*[]){
        &menu_main_arg_PID_xAy_Kp,
        &menu_main_arg_PID_xAy_Ki,
        &menu_main_arg_PID_xAy_Kd,
    });
    FFloatPage_init(&menu_main_arg_PID_xAy_Kp, "Kp", &PID_WxAy.Kp, 0, 10000);
    FFloatPage_init(&menu_main_arg_PID_xAy_Ki, "Ki", &PID_WxAy.Ki, 0, 10000);
    FFloatPage_init(&menu_main_arg_PID_xAy_Kd, "Kd", &PID_WxAy.Kd, 0, 10000);
    ListPage_init(&menu_main_arg_PID_vVx, "vVx", 3, (Page*[]){
        &menu_main_arg_PID_vVx_Kp,
        &menu_main_arg_PID_vVx_Ki,
        &menu_main_arg_PID_vVx_Kd,
    });
    FFloatPage_init(&menu_main_arg_PID_vVx_Kp, "Kp", &PID_vVx.Kp, 0, 10000);
    FFloatPage_init(&menu_main_arg_PID_vVx_Ki, "Ki", &PID_vVx.Ki, 0, 10000);
    FFloatPage_init(&menu_main_arg_PID_vVx_Kd, "Kd", &PID_vVx.Kd, 0, 10000);
    ListPage_init(&menu_main_arg_PID_lPitch, "lPitch", 3, (Page*[]){
        &menu_main_arg_PID_lPitch_Kp,
        &menu_main_arg_PID_lPitch_Ki,
        &menu_main_arg_PID_lPitch_Kd,
    });
    FFloatPage_init(&menu_main_arg_PID_lPitch_Kp, "Kp", &PID_LPitch.Kp, -10000, 10000);
    FFloatPage_init(&menu_main_arg_PID_lPitch_Ki, "Ki", &PID_LPitch.Ki, -10000, 10000);
    FFloatPage_init(&menu_main_arg_PID_lPitch_Kd, "Kd", &PID_LPitch.Kd, -10000, 10000);
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
    FFloatPage_init(&menu_main_debug_fl_rb, "rt_bk", &flRb, -1.57, 1.57);
    FFloatPage_init(&menu_main_debug_fl_rf, "rt_fd", &flRf, -1.57, 1.57);
    FFloatPage_init(&menu_main_debug_fl_lf, "lt_fd", &flLf, -3.14, 1.57);
    FFloatPage_init(&menu_main_debug_fl_lb, "lt_bk", &flLb, -1.57, 1.57);
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
    IntPage_init(&menu_main_debug_fwp_lx, "lt_x", &fwpLx, -LEG_MAX_X, LEG_MAX_X);
    IntPage_init(&menu_main_debug_fwp_lz, "lt_z", &fwpLz, -LEG_MAX_Z, -LEG_MIN_Z);
    IntPage_init(&menu_main_debug_fwp_rx, "rt_x", &fwpRx, -LEG_MAX_X, LEG_MAX_X);
    IntPage_init(&menu_main_debug_fwp_rz, "rt_z", &fwpRz, -LEG_MAX_Z, -LEG_MIN_Z);

    beepLong();
    PageKey_print(&menu_main, 0);
    for(;;){
        // �˴���д��Ҫѭ��ִ�еĴ���
        if(pressed){
            beepShort();
            PageKey_press(&menu_main, pressed);
            PageKey_print(&menu_main, 0);
            pressed=0;
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
