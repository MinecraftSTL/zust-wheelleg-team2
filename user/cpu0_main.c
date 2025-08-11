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
#include "zf_common_headfile.h"

#include "menu.h"
#include "cpu1_main.h"

#include "PID.h"
#include "zf_device_mpu6050.h"
#include "MyEncoder.h"
#include "Gyro.h"

#include "PID_param.h"

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
Page menu_main_start;
Page menu_main_PID_motorL;
Page menu_main_PID_motorL_Kp;
Page menu_main_PID_motorL_Ki;
Page menu_main_PID_motorL_Kd;
Page menu_main_PID_motorR;
Page menu_main_PID_motorR_Kp;
Page menu_main_PID_motorR_Ki;
Page menu_main_PID_motorR_Kd;
Page menu_main_PID_pitch;
Page menu_main_PID_pitch_Kp;
Page menu_main_PID_pitch_Ki;
Page menu_main_PID_pitch_Kd;
Page menu_main_PID_Vz;
Page menu_main_PID_Vz_Kp;
Page menu_main_PID_Vz_Ki;
Page menu_main_PID_Vz_Kd;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    beepStart();
    ips200_init(IPS200_TYPE_SPI);
    mpu6050_init();
    MyEncoder_Init();
    key_init(PIT00ms);
    Motor_Init();
    PID_param_init();
    pit_ms_init(CCU60_CH0, PIT00ms);
    pit_ms_init(CCU61_CH0, PIT10ms);
    beepStop();
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    menu_main.parent = NULL;
    ListPage_init(&menu_main, "main", 2, &(Page*[]){
        &menu_main_PID,
        &menu_main_start,
    });
    ListPage_init(&menu_main_PID, "PID", 4, &(Page*[]){
        &menu_main_PID_motorL,
        &menu_main_PID_motorR,
        &menu_main_PID_pitch,
        &menu_main_PID_Vz,
    });
    ListPage_init(&menu_main_PID_motorL, "motorL", 3, &(Page*[]){
        &menu_main_PID_motorL_Kp,
        &menu_main_PID_motorL_Ki,
        &menu_main_PID_motorL_Kd,
    });
    FloatPage_init(&menu_main_PID_motorL_Kp, "Kp", &motorL.Kp, -1000, 1000);
    FloatPage_init(&menu_main_PID_motorL_Ki, "Ki", &motorL.Ki, -1000, 1000);
    FloatPage_init(&menu_main_PID_motorL_Kd, "Kd", &motorL.Kd, -1000, 1000);
    ListPage_init(&menu_main_PID_motorR, "motorR", 3, &(Page*[]){
        &menu_main_PID_motorR_Kp,
        &menu_main_PID_motorR_Ki,
        &menu_main_PID_motorR_Kd,
    });
    FloatPage_init(&menu_main_PID_motorR_Kp, "Kp", &motorR.Kp, -1000, 1000);
    FloatPage_init(&menu_main_PID_motorR_Ki, "Ki", &motorR.Ki, -1000, 1000);
    FloatPage_init(&menu_main_PID_motorR_Kd, "Kd", &motorR.Kd, -1000, 1000);
    ListPage_init(&menu_main_PID_pitch, "pitch", 3, &(Page*[]){
        &menu_main_PID_pitch_Kp,
        &menu_main_PID_pitch_Ki,
        &menu_main_PID_pitch_Kd,
    });
    FloatPage_init(&menu_main_PID_pitch_Kp, "Kp", &pitch.Kp, -1000, 1000);
    FloatPage_init(&menu_main_PID_pitch_Ki, "Ki", &pitch.Ki, -1000, 1000);
    FloatPage_init(&menu_main_PID_pitch_Kd, "Kd", &pitch.Kd, -1000, 1000);
    ListPage_init(&menu_main_PID_Vz, "Vz", 3, &(Page*[]){
        &menu_main_PID_Vz_Kp,
        &menu_main_PID_Vz_Ki,
        &menu_main_PID_Vz_Kd,
    });
    FloatPage_init(&menu_main_PID_Vz_Kp, "Kp", &Vy.Kp, -1000, 1000);
    FloatPage_init(&menu_main_PID_Vz_Ki, "Ki", &Vy.Ki, -1000, 1000);
    FloatPage_init(&menu_main_PID_Vz_Kd, "Kd", &Vy.Kd, -1000, 1000);
    menu_main_start.parent = &menu_main;
    BoolPage_init(&menu_main_start, "start", &car_run, 0x03);
    PageKey_print(&menu_main);
    for(;;){
        // �˴���д��Ҫѭ��ִ�еĴ���
        if(pressed){
            PageKey_press(&menu_main, pressed);
            PageKey_print(&menu_main);
            pressed=0;
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
