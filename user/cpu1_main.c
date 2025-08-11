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
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

#include "menu.h"

#include "PID.h"
#include "MyEncoder.h"

#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

// **************************** �������� ****************************

uint8 pressed = 0;

Page main_menu;
Page menu_arg;
Page menu_arg_PID;
Page menu_arg_PID_motorL;
Page menu_arg_PID_motorL_Kp;
Page menu_arg_PID_motorL_Ki;
Page menu_arg_PID_motorL_Kd;
Page menu_arg_PID_motorR;
Page menu_arg_PID_motorR_Kp;
Page menu_arg_PID_motorR_Ki;
Page menu_arg_PID_motorR_Kd;
Page menu_arg_PID_turn;
Page menu_arg_PID_turn_Ki;
Page menu_arg_PID_turn_straight;
Page menu_arg_PID_turn_straight_Kp;
Page menu_arg_PID_turn_straight_Kd;
Page menu_arg_PID_turn_bend;
Page menu_arg_PID_turn_bend_Kp;
Page menu_arg_PID_turn_bend_Kd;
Page menu_arg_PID_turn_arc;
Page menu_arg_PID_turn_arc_Kp;
Page menu_arg_PID_turn_arc_Kd;
Page menu_arg_V;
Page menu_arg_V_target;
Page menu_arg_V_straight;
Page menu_arg_V_S;
Page menu_arg_V_annulus;
Page menu_arg_V_hill;
Page menu_debug;
Page menu_debug_forceSpeedEn;
Page menu_debug_forceSpeed;
Page menu_debug_forceTurnEn;
Page menu_debug_forceTurnKp;
Page menu_debug_forceTurnKd;
Page menu_start;
Page menu_testInt;

PID motor_l;
PID motor_r;
PID motor_turn;
float target_speed;
float straight_speed;
float S_speed;
float annulus_speed;
float hill_speed;
uint8 qidong;
int32 testInt;

void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    main_menu.parent = NULL;
    ListPage_init(&main_menu, "main", 4, &(Page*[]){
        &menu_arg,
        &menu_start,
        &menu_arg_PID_turn_Ki,
        &menu_testInt,
    });
    FloatPage_init(&menu_arg_PID_turn_Ki, "Ki", &(motor_turn.Kd), -1000., 1000.);
    IntPage_init(&menu_testInt, "i", &testInt, -10000000, 100000000);
    ListPage_init(&menu_arg, "arg", 1, &(Page*[]){
        &menu_arg_PID,
    });
    ListPage_init(&menu_arg_PID, "PID", 2, &(Page*[]){
        &menu_arg_PID_motorL,
        &menu_arg_PID_motorR,
    });
    ListPage_init(&menu_arg_PID_motorL, "motorL", 3, &(Page*[]){
        &menu_arg_PID_motorL_Kp,
        &menu_arg_PID_motorL_Ki,
        &menu_arg_PID_motorL_Kd,
    });
    FloatPage_init(&menu_arg_PID_motorL_Kp, "Kp", &(motor_l.Kp), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorL_Ki, "Ki", &(motor_l.Ki), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorL_Kd, "Kd", &(motor_l.Kd), -1000., 1000.);
    ListPage_init(&menu_arg_PID_motorR, "motorR", 3, &(Page*[]){
        &menu_arg_PID_motorR_Kp,
        &menu_arg_PID_motorR_Ki,
        &menu_arg_PID_motorR_Kd,
    });
    FloatPage_init(&menu_arg_PID_motorR_Kp, "Kp", &(motor_r.Kp), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorR_Ki, "Ki", &(motor_r.Ki), -100., 100.);
    FloatPage_init(&menu_arg_PID_motorR_Kd, "Kd", &(motor_r.Kd), -1000., 1000.);

    BoolPage_init(&menu_start, "start", &qidong, 0x03);
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    PageKey_print(&main_menu);
    for(;;){
        // �˴���д��Ҫѭ��ִ�еĴ���
        if(pressed){
            PageKey_press(&main_menu, pressed);
            PageKey_print(&main_menu);
            pressed=0;
        }
        ips200_show_int(0, 300, testInt,9);
//        printf("%d", switch_encoder_num);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
#pragma section all restore
// **************************** �������� ****************************
