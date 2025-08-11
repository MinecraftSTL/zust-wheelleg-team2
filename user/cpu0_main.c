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
#include "sys.h"
#include "zf_common_headfile.h"

#include "zf_device_ips200.h"
#include "motor.h"
#include "encoder.h"
#include "camera.h"


#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��
// *************************** ����Ӳ������˵�� ***************************
// ���������Ҷ���������ͷ ��Ӧ��������ͷ�ӿ� ��ע������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      TXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_TX �궨��
//      RXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_RX �궨��
//      PCLK                �鿴 zf_device_mt9v03x.h �� MT9V03X_PCLK_PIN �궨��
//      VSY                 �鿴 zf_device_mt9v03x.h �� MT9V03X_VSYNC_PIN �궨��
//      D0-D7               �鿴 zf_device_mt9v03x.h �� MT9V03X_DATA_PIN �궨�� �Ӹö��忪ʼ�������˸�����
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// ����2��IPSģ��
// *************************** ����Ӳ������˵�� ***************************
//      ģ��ܽ�            ��Ƭ���ܽ�
//      BL                  �鿴 zf_device_ips200_parallel8.h �� IPS200_BL_PIN �궨��  Ĭ�� P15_3
//      CS                  �鿴 zf_device_ips200_parallel8.h �� IPS200_CS_PIN �궨��  Ĭ�� P15_5
//      RST                 �鿴 zf_device_ips200_parallel8.h �� IPS200_RST_PIN �궨�� Ĭ�� P15_1
//      RS                  �鿴 zf_device_ips200_parallel8.h �� IPS200_RS_PIN �궨��  Ĭ�� P15_0
//      WR                  �鿴 zf_device_ips200_parallel8.h �� IPS200_WR_PIN �궨��  Ĭ�� P15_2
//      RD                  �鿴 zf_device_ips200_parallel8.h �� IPS200_RD_PIN �궨��  Ĭ�� P15_4
//      D0-D7               �鿴 zf_device_ips200_parallel8.h �� IPS200_Dx_PIN �궨��  Ĭ�� P11_9/P11_10/P11_11/P11_12/P13_0/P13_1/P13_2/P13_3
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ



// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����� �����İ���������� �嵽��
// 2.����ͷ�������������ͷ�ӿ� ע������2��IPSģ�����������Ļ�ӿ�
// 3.�����ϵ� ���ߺ��İ�������Ϻ��ϵ� ���İ尴�¸�λ����
// 4.��Ļ����ʾ��ʼ����ϢȻ����ʾ����ͷͼ��
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�
// **************************** �������� ****************************

#define IPS200_TYPE     (IPS200_TYPE_SPI)                                       // ˫������ ���������� ����궨����д IPS200_TYPE_PARALLEL8
                                                                                // �������� SPI ������ ����궨����д IPS200_TYPE_SPI
float jiaodu=0;
float jd_sum=0;
uint8 tingche_flag=0;
#define Beep    P33_10                          //������
int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();
    gpio_init(P33_10,GPO,0,GPO_PUSH_PULL);// ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    gpio_init(Beep, GPO, 1, GPO_PUSH_PULL);
    gpio_init(P21_4, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P21_5, GPO, 1, GPO_PUSH_PULL);

    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "ips200 init success.");

    ips200_show_string(0, 0, "motor init.");
    motor_init();
    ips200_show_string(0, 0, "ecounter init.");
    ecounter_init();
    ips200_show_string(0, 0, "motor, servo, ecounter");
    ips200_show_string(0, 20, "init success.");

    ips200_show_string(0, 0, "mt9v03x init.");
    for(;;)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 0, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // ����ʱ�������Ʊ�ʾ�쳣
    }
    ips200_show_string(0, 0, "init success.");
    pit_ms_init(CCU60_CH0, 5);           //�ٶȻ��ж�
    pit_ms_init(CCU60_CH1, 2);           //ת���ж�
    pit_ms_init(CCU61_CH0, 10);           //��ť������ʱ���ж�
    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	gpio_low(Beep);
	gpio_high(P21_4);
	ips200_clear();
	for(;;)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���
	    if(mt9v03x_finish_flag){
            image_process();
    //	    ips200_clear();
    //	    ips200_displayimage03x((const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H);
            ips200_show_gray_image(0, 0, imag, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);     // ��ʾ��ֵ��ͼ��
            for(int i=0; i<120; ++i){
                ips200_draw_point(left[i],i,RGB565_RED);
                ips200_draw_point(right[i],i,RGB565_BLUE);
                ips200_draw_point(middle[i],i,RGB565_GREEN);
            }
            gpio_toggle_level(P21_5);
	    }
        // �˴���д��Ҫѭ��ִ�еĴ���
	    //����
//        PWM_motor(
//            func_limit(motor_l_out,1500),
//            func_limit(motor_r_out,1500)
//        );
	    //���ж�
//	    PWM_motor(
//	        -func_limit(1000+(middle[118]-93)*10, 1500),
//	        -func_limit(1000-(middle[118]-93)*10, 1500)
//	    );
//	    getspeed();
//        ips200_show_int(0,120,middle[118],8);
	    ips200_show_int(0,160,Encoder_speed_l,8);
        ips200_show_int(0,180,Encoder_speed_r,8);
        ips200_show_int(0,200,Encoder_speed_l-Encoder_speed_r,8);
	}
	return 0;
}

#pragma section all restore
// **************************** �������� ****************************
// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1����Ļ����ʾ
//      ���ʹ��������ԣ��������Ҫ�õ�ع��� �����Ļ�������ŵ�ѹ
//      �����Ļ�ǲ��ǲ��λ���� ������Ŷ�Ӧ��ϵ
//      �����Ӧ���Ŷ���ȷ ���һ���Ƿ������Ų��β��� ��Ҫ��ʾ����
//      �޷���ɲ��β�������һ��GPIO���̽���Ļ����IO��ʼ��ΪGPIO��ת��ƽ �����Ƿ��ܿ�
// ����2����ʾ reinit ����
//      �������Ƿ�����
//      ���幩���Ƿ�ʹ�õ�������ĵ�ع���
// ����2����ʾͼ������ ��λ
//      �������ͷ�ź����Ƿ����ɶ�
