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
Page menu_main_debug_fs_speed;
Page menu_main_debug_fs_en;

int32 fsSpeed = 0;
uint8 fsEn = 1;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
//    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    beepStart();
    ips200_init(IPS200_TYPE_SPI);
    mpu6050_init();
    key_init(PIT00ms);
    small_driver_uart_init();
    MyEncoder_Init();
    PID_param_init();
    pit_ms_init(CCU60_CH0, PIT00ms);
    pit_ms_init(CCU61_CH0, PIT10ms);
    beepStop();
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
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
    ListPage_init(&menu_main_debug, "debug", 1, (Page*[]){
        &menu_main_debug_fs,
    });
    ListPage_init(&menu_main_debug_fs, "forceSpeed", 2, (Page*[]){
        &menu_main_debug_fs_speed,
        &menu_main_debug_fs_en,
    });
    IntPage_init(&menu_main_debug_fs_speed, "speed", &fsSpeed, -10000, 10000);
    BoolPage_init(&menu_main_debug_fs_en, "enable", &fsEn, 0x03);

    beepShort();
    PageKey_print(&menu_main, 0);
    for(;;){
        // �˴���д��Ҫѭ��ִ�еĴ���
        if(pressed){
            PageKey_press(&menu_main, pressed);
            PageKey_print(&menu_main, 0);
            pressed=0;
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
