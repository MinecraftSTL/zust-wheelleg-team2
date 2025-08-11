#include "MYENCODER.h"

int16 Encoder_speed_l = 0;
int16 Encoder_speed_r = 0;

int switch_encoder_num = 0;
int switch_encoder_change_num = 0;

void MyEncoder_Init(void)
{
//    encoder_dir_init(TIM2_ENCODER, ENCODER_L, ENCODER_DIR_L);//左轮编码器
//    encoder_dir_init(TIM6_ENCODER, ENCODER_R, ENCODER_DIR_R);//右轮编码器
    encoder_quad_init(TIM3_ENCODER, Switch_ENCODER_L, Switch_ENCODER_R);
//    encoder_dir_init(TIM3_ENCODER, Switch_ENCODER_L, Switch_ENCODER_R);

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器采集
//  @param      gptn    ：编码器对应编号
//  @param      n       ：n次均值滤波
//  @param      direct  ：1或者0 决定 编码器的正负号
//  @return     int16
//  @note       要是编码器方向反了就把direct改一下 1或者0
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
    encoder_clear_count(gptn);    //编码器清空

    return Coder/n;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      获得转速
//  @param      gptn：编码器对应编号
//  @param      n   ：n次均值滤波
//  @return     int16
//  @note       里面的自加，自减根据实际情况调，车往前进是两个编码器值都为正
//-------------------------------------------------------------------------------------------------------------------
void GetSpeed(void)
{
 // 获取编码器的值
    small_driver_get_speed();
    Encoder_speed_l = -motor_value.receive_left_speed_data;
    Encoder_speed_r = motor_value.receive_right_speed_data;
//    Encoder_speed_r = -Encoder_MTM(TIM3_ENCODER,1,1);
};

/***********************************************
* @brief : 获取旋转编码器值
* @param : void
* @return: void
* @date  : 2024年11月6日12:23:25
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
* @brief : 旋转编码器获取函数，仅用于旋转编码器
* @param : void
* @return: void
* @date  : 2024年11月6日12:26:53
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
