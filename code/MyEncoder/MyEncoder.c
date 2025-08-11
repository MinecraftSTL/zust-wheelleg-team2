#include "MYENCODER.h"

int16 Encoder_speed_l = 0;
int16 Encoder_speed_r = 0;

int switch_encoder_num = 0;
int switch_encoder_change_num = 0;

void MyEncoder_Init(void)
{
//    encoder_dir_init(TIM2_ENCODER, ENCODER_L, ENCODER_DIR_L);//���ֱ�����
//    encoder_dir_init(TIM6_ENCODER, ENCODER_R, ENCODER_DIR_R);//���ֱ�����
    encoder_quad_init(TIM3_ENCODER, Switch_ENCODER_L, Switch_ENCODER_R);
//    encoder_dir_init(TIM3_ENCODER, Switch_ENCODER_L, Switch_ENCODER_R);

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ɼ�
//  @param      gptn    ����������Ӧ���
//  @param      n       ��n�ξ�ֵ�˲�
//  @param      direct  ��1����0 ���� ��������������
//  @return     int16
//  @note       Ҫ�Ǳ����������˾Ͱ�direct��һ�� 1����0
//-------------------------------------------------------------------------------------------------------------------
int16 Encoder_MTM(encoder_index_enum gptn,int n,uint8 direct)
{
    int16 Coder = 0;
    for(int i = 0;i < n;i++){
        if(direct){
            Coder -=  encoder_get_count(gptn);
        }else{
            Coder +=  encoder_get_count(gptn);
        }
    }
    encoder_clear_count(gptn);    //���������

    return Coder/n;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ת��
//  @param      gptn����������Ӧ���
//  @param      n   ��n�ξ�ֵ�˲�
//  @return     int16
//  @note       ������Լӣ��Լ�����ʵ�������������ǰ��������������ֵ��Ϊ��
//-------------------------------------------------------------------------------------------------------------------
void GetSpeed(void)
{
 // ��ȡ��������ֵ
    small_driver_get_speed();
    Encoder_speed_l = -motor_value.receive_left_speed_data;
    Encoder_speed_r = motor_value.receive_right_speed_data;
//    Encoder_speed_r = -Encoder_MTM(TIM3_ENCODER,1,1);
};

/***********************************************
* @brief : ��ȡ��ת������ֵ
* @param : void
* @return: void
* @date  : 2024��11��6��12:23:25
* @author: SJX
************************************************/
void Get_Switch_Num(void)
{
    static int encoder_cnt, timer_cnt, last_switch_encoder_num = 0;
    timer_cnt = -My_Switch_encoder_get_count(TIM3_ENCODER);
    encoder_clear_count(TIM3_ENCODER);

    encoder_cnt += timer_cnt;
    switch_encoder_num += encoder_cnt / 4;
    encoder_cnt %= 4;

//    printf("%d, %d, %d, %d\r\n", timer_cnt, switch_encode_change_get_buff_flag,
//            last_switch_encoder_num, switch_encoder_num);
    switch_encoder_change_num = switch_encoder_num - last_switch_encoder_num;
    last_switch_encoder_num = switch_encoder_num;
}
/***********************************************
* @brief : ��ת��������ȡ��������������ת������
* @param : void
* @return: void
* @date  : 2024��11��6��12:26:53
* @author: SJX
************************************************/
int16 My_Switch_encoder_get_count (encoder_index_enum encoder_n)
{
    int16 encoder_data = 0;
    switch(encoder_n)
    {
        case TIM2_ENCODER: encoder_data = (int16)IfxGpt12_T2_getTimerValue(&MODULE_GPT120); break;
        case TIM3_ENCODER: encoder_data = (int16)IfxGpt12_T3_getTimerValue(&MODULE_GPT120); break;
        case TIM4_ENCODER: encoder_data = (int16)IfxGpt12_T4_getTimerValue(&MODULE_GPT120); break;
        case TIM5_ENCODER: encoder_data = (int16)IfxGpt12_T5_getTimerValue(&MODULE_GPT120); break;
        case TIM6_ENCODER: encoder_data = (int16)IfxGpt12_T6_getTimerValue(&MODULE_GPT120); break;
        default: encoder_data = 0;
    }
    return encoder_data;
}
