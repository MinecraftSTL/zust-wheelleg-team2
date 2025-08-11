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
#include "key.h"
#include "isr.h"

#include "motor.h"
#include "camera.h"

#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� enableInterrupts(); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� disableInterrupts(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� enableInterrupts(); �������жϵ���Ӧ��


// **************************** �������� ****************************

struct PagePageValue main_menu;
struct PagePageValue menu_arg;
struct PagePageValue menu_arg_PID;
struct PagePageValue menu_arg_PID_motorL;
struct FloatPageValue menu_arg_PID_motorL_Kp;
struct FloatPageValue menu_arg_PID_motorL_Ki;
struct FloatPageValue menu_arg_PID_motorL_Kd;
struct FloatPageValue menu_arg_PID_motorL_Kdd;
struct PagePageValue menu_arg_PID_motorR;
struct FloatPageValue menu_arg_PID_motorR_Kp;
struct FloatPageValue menu_arg_PID_motorR_Ki;
struct FloatPageValue menu_arg_PID_motorR_Kd;
struct FloatPageValue menu_arg_PID_motorR_Kdd;
struct PagePageValue menu_arg_PID_turn;
struct FloatPageValue menu_arg_PID_turn_Ki;
struct PagePageValue menu_arg_PID_turn_straight;
struct FloatPageValue menu_arg_PID_turn_straight_Kp;
struct FloatPageValue menu_arg_PID_turn_straight_Kd;
struct PagePageValue menu_arg_PID_turn_bend;
struct FloatPageValue menu_arg_PID_turn_bend_Kp;
struct FloatPageValue menu_arg_PID_turn_bend_Kd;
struct PagePageValue menu_arg_PID_turn_arc;
struct FloatPageValue menu_arg_PID_turn_arc_Kp;
struct FloatPageValue menu_arg_PID_turn_arc_Kd;
struct PagePageValue menu_arg_V;
struct FloatPageValue menu_arg_V_target;
struct FloatPageValue menu_arg_V_straight;
struct FloatPageValue menu_arg_V_S;
struct FloatPageValue menu_arg_V_annulus;
struct PagePageValue menu_arg_V_hill;
struct PagePageValue menu_debug;
struct BooleanPageValue menu_debug_forceSpeedEn;
struct FloatPageValue menu_debug_forceSpeed;
struct BooleanPageValue menu_debug_forceTurnEn;
struct FloatPageValue menu_debug_forceTurnKp;
struct FloatPageValue menu_debug_forceTurnKd;
struct BooleanPageValue menu_start;

void core1_main(void)
{
    gpio_init(P20_8, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P20_9, GPO, 0, GPO_PUSH_PULL);
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������

    // �˴���д�û����� ���������ʼ�������

    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    struct PageKey main_menued = new_PageKey("main", PAGE_TYPE, &main_menu);
    PagePageValue_init(&main_menu, &(struct PageKey[]){
        new_PageKey("arg", PAGE_TYPE, &menu_arg),
        new_PageKey("debug", PAGE_TYPE, &menu_debug),
        new_PageKey("start", BOOL_TYPE, &menu_start),
    }, 3);
    PagePageValue_init(&menu_arg, &(struct PageKey[]){
        new_PageKey("PID", PAGE_TYPE, &menu_arg_PID),
        new_PageKey("V", PAGE_TYPE, &menu_arg_V),
    }, 2);
    PagePageValue_init(&menu_arg_PID, &(struct PageKey[]){
        new_PageKey("motorL", PAGE_TYPE, &menu_arg_PID_motorL),
        new_PageKey("motorR", PAGE_TYPE, &menu_arg_PID_motorR),
        new_PageKey("turn", PAGE_TYPE, &menu_arg_PID_turn),
    }, 3);
    PagePageValue_init(&menu_arg_PID_motorL, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_motorL_Kp),
        new_PageKey("Ki", FLOAT_TYPE, &menu_arg_PID_motorL_Ki),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_motorL_Kd),
        new_PageKey("Kdd", FLOAT_TYPE, &menu_arg_PID_motorL_Kdd),
    }, 4);
    FloatPageValue_init(&menu_arg_PID_motorL_Kp, &(motor_l.Kp), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorL_Ki, &(motor_l.Ki), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorL_Kd, &(motor_l.Kd), -1000., 1000., 10);
    FloatPageValue_init(&menu_arg_PID_motorL_Kdd, &(motor_l.Kdd), -1000., 1000., 10);
    PagePageValue_init(&menu_arg_PID_motorR, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_motorR_Kp),
        new_PageKey("Ki", FLOAT_TYPE, &menu_arg_PID_motorR_Ki),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_motorR_Kd),
        new_PageKey("Kdd", FLOAT_TYPE, &menu_arg_PID_motorR_Kdd),
    }, 4);
    FloatPageValue_init(&menu_arg_PID_motorR_Kp, &(motor_r.Kp), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorR_Ki, &(motor_r.Ki), -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_motorR_Kd, &(motor_r.Kd), -1000., 1000., 10);
    FloatPageValue_init(&menu_arg_PID_motorR_Kdd, &(motor_r.Kdd), -1000., 1000., 10);
    PagePageValue_init(&menu_arg_PID_turn, &(struct PageKey[]){
        new_PageKey("Ki", FLOAT_TYPE, &menu_arg_PID_turn_Ki),
        new_PageKey("Kdd", FLOAT_TYPE, new_FloatPageValue(&(motor_turn.Kdd), -1000., 1000., 10)),
        new_PageKey("straight", PAGE_TYPE, &menu_arg_PID_turn_straight),
        new_PageKey("bend", PAGE_TYPE, &menu_arg_PID_turn_bend),
        new_PageKey("arc", PAGE_TYPE, &menu_arg_PID_turn_arc),
    }, 5);
    FloatPageValue_init(&menu_arg_PID_turn_Ki, &(motor_turn.Ki), -1., 1., 0.01);
    PagePageValue_init(&menu_arg_PID_turn_straight, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_turn_straight_Kp),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_turn_straight_Kd),
    }, 2);
    FloatPageValue_init(&menu_arg_PID_turn_straight_Kp, &straight_Kp, -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_turn_straight_Kd, &straight_Kd, -1000., 1000., 5);
    PagePageValue_init(&menu_arg_PID_turn_bend, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_turn_bend_Kp),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_turn_bend_Kd),
    }, 2);
    FloatPageValue_init(&menu_arg_PID_turn_bend_Kp, &bend_Kp, -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_turn_bend_Kd, &bend_Kd, -1000., 1000., 5);
    PagePageValue_init(&menu_arg_PID_turn_arc, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_arg_PID_turn_arc_Kp),
        new_PageKey("Kd", FLOAT_TYPE, &menu_arg_PID_turn_arc_Kd),
    }, 2);
    FloatPageValue_init(&menu_arg_PID_turn_arc_Kp, &arc_Kp, -100., 100., 0.1);
    FloatPageValue_init(&menu_arg_PID_turn_arc_Kd, &arc_Kd, -1000., 1000., 5);
    PagePageValue_init(&menu_arg_V, &(struct PageKey[]){
        new_PageKey("target", FLOAT_TYPE, &menu_arg_V_target),
        new_PageKey("straight", FLOAT_TYPE, &menu_arg_V_straight),
        new_PageKey("S", FLOAT_TYPE, &menu_arg_V_S),
        new_PageKey("annulus", FLOAT_TYPE, &menu_arg_V_annulus),
        new_PageKey("hill", FLOAT_TYPE, &menu_arg_V_hill),
    }, 5);
    FloatPageValue_init(&menu_arg_V_target, &target_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_straight, &straight_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_S, &S_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_annulus, &annulus_speed, -500., 500., 25);
    FloatPageValue_init(&menu_arg_V_hill, &hill_speed, -500., 500., 25);
    PagePageValue_init(&menu_debug, &(struct PageKey[]){
        new_PageKey("forceVEn", BOOL_TYPE, &menu_debug_forceSpeedEn),
        new_PageKey("forceV", FLOAT_TYPE, &menu_debug_forceSpeed),
        new_PageKey("forceTurnEn", BOOL_TYPE, &menu_debug_forceTurnEn),
        new_PageKey("forceTurnKp", FLOAT_TYPE, &menu_debug_forceTurnKp),
        new_PageKey("forceTurnKd", FLOAT_TYPE, &menu_debug_forceTurnKd),
    }, 5);
    BooleanPageValue_init(&menu_debug_forceSpeedEn, &debug_forceSpeedEn);
    FloatPageValue_init(&menu_debug_forceSpeed, &debug_forceSpeed, -500., 500., 25);
    BooleanPageValue_init(&menu_debug_forceTurnEn, &debug_forceTurnEn);
    FloatPageValue_init(&menu_debug_forceTurnKp, &debug_forceTurnKp, -100., 100., 0.1);
    FloatPageValue_init(&menu_debug_forceTurnKd, &debug_forceTurnKd, -9990., 9990., 5);
    BooleanPageValue_init(&menu_start, &speed_qidong);
    system_delay_ms(1000);
    beep_once();
    PageKey_print(&main_menued);
    for(;;)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        uint8 pressed = Button_Press();
        if(pressed){
            PageKey_press(&main_menued, pressed);
            PageKey_print(&main_menued);
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
    del_PageKey(&main_menu);
}
#pragma section all restore
