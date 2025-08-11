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

#include "motor.h"

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
struct PagePageValue menu_PID;
struct PagePageValue menu_PID_motorL;
struct FloatPageValue menu_PID_motorL_Kp;
struct FloatPageValue menu_PID_motorL_Ki;
struct FloatPageValue menu_PID_motorL_Kd;
struct PagePageValue menu_PID_motorR;
struct FloatPageValue menu_PID_motorR_Kp;
struct FloatPageValue menu_PID_motorR_Ki;
struct FloatPageValue menu_PID_motorR_Kd;
struct PagePageValue menu_PID_turn;
struct FloatPageValue menu_PID_turn_Ki;
struct PagePageValue menu_PID_turn_straight;
struct FloatPageValue menu_PID_turn_straight_Kp;
struct FloatPageValue menu_PID_turn_straight_Kd;
struct PagePageValue menu_PID_turn_bend;
struct FloatPageValue menu_PID_turn_bend_Kp;
struct FloatPageValue menu_PID_turn_bend_Kd;
struct PagePageValue menu_PID_turn_arc;
struct FloatPageValue menu_PID_turn_arc_Kp;
struct FloatPageValue menu_PID_turn_arc_Kd;

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
        new_PageKey("PID", PAGE_TYPE, &menu_PID),
        new_PageKey("start", BOOL_TYPE, new_BooleanPageValue(&(speed_qidong)))
    }, 2);
    PagePageValue_init(&menu_PID, &(struct PageKey[]){
        new_PageKey("motorL", PAGE_TYPE, &menu_PID_motorL),
        new_PageKey("motorR", PAGE_TYPE, &menu_PID_motorR),
        new_PageKey("turn", PAGE_TYPE, &menu_PID_turn),
    }, 3);
    PagePageValue_init(&menu_PID_motorL, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_PID_motorL_Kp),
        new_PageKey("Ki", FLOAT_TYPE, &menu_PID_motorL_Ki),
        new_PageKey("Kd", FLOAT_TYPE, &menu_PID_motorL_Kd),
    }, 3);
    FloatPageValue_init(&menu_PID_motorL_Kp, &(motor_l.Kp), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_motorL_Ki, &(motor_l.Ki), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_motorL_Kd, &(motor_l.Kd), 20., -20., 0.1);
    PagePageValue_init(&menu_PID_motorR, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, &menu_PID_motorR_Kp),
        new_PageKey("Ki", FLOAT_TYPE, &menu_PID_motorR_Ki),
        new_PageKey("Kd", FLOAT_TYPE, &menu_PID_motorR_Kd),
    }, 3);
    FloatPageValue_init(&menu_PID_motorR_Kp, &(motor_r.Kp), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_motorR_Ki, &(motor_r.Ki), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_motorR_Kd, &(motor_r.Kd), 20., -20., 0.1);
    PagePageValue_init(&menu_PID_turn, &(struct PageKey[]){
        new_PageKey("Ki", FLOAT_TYPE, &menu_PID_turn_Ki),
        new_PageKey("straight", PAGE_TYPE, &menu_PID_turn_straight),
        new_PageKey("bend", PAGE_TYPE, &menu_PID_turn_bend),
        new_PageKey("arc", PAGE_TYPE, &menu_PID_turn_arc),
    }, 4);
    FloatPageValue_init(&menu_PID_turn_Ki, &(motor_turn.Ki), 20., -20., 0.1);
    PagePageValue_init(&menu_PID_turn_straight, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, new_FloatPageValue(&(straight_Kp), 20., -20., 0.1)),
        new_PageKey("Kd", FLOAT_TYPE, new_FloatPageValue(&(straight_Kd), 20., -20., 0.1)),
    }, 2);
    FloatPageValue_init(&menu_PID_turn_straight_Kp, &(straight_Kp), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_turn_straight_Kp, &(straight_Kd), 20., -20., 0.1);
    PagePageValue_init(&menu_PID_turn_bend, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, new_FloatPageValue(&(bend_Kp), 20., -20., 0.1)),
        new_PageKey("Kd", FLOAT_TYPE, new_FloatPageValue(&(bend_Kd), 20., -20., 0.1)),
    }, 2);
    FloatPageValue_init(&menu_PID_turn_bend_Kp, &(bend_Kp), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_turn_bend_Kp, &(bend_Kd), 20., -20., 0.1);
    PagePageValue_init(&menu_PID_turn_arc, &(struct PageKey[]){
        new_PageKey("Kp", FLOAT_TYPE, new_FloatPageValue(&(arc_Kp), 20., -20., 0.1)),
        new_PageKey("Kd", FLOAT_TYPE, new_FloatPageValue(&(arc_Kd), 20., -20., 0.1)),
    }, 2);
    FloatPageValue_init(&menu_PID_turn_arc_Kp, &(arc_Kp), 20., -20., 0.1);
    FloatPageValue_init(&menu_PID_turn_arc_Kp, &(arc_Kd), 20., -20., 0.1);
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
