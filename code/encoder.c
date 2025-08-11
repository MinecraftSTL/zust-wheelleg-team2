/*
���ߣ�Charon and ������С��Ƭ
       δ����Ȩ��ֹת��
 */

#include "sys.h"
int16 Encoder_speed_l=0;
int16 Encoder_speed_r=0;

//void encoder_filter(void)
//{
//  Encoder_speed_l=-encoder_get_count(ENCODER_l);
//  Encoder_speed_r=encoder_get_count(ENCODER_r);
////  printf("%d,%d\r\n",Encoder_speed_l,200);
////  printf("%d,%d\r\n",Encoder_speed_r,200);
//  encoder_clear_count(ENCODER_l);
//  encoder_clear_count(ENCODER_r);
//
//}
//
//void encoder_init(void)
//{
//
//    encoder_quad_init(ENCODER_l , ENCODER_l_A  , ENCODER_l_B );
//    encoder_quad_init(ENCODER_r , ENCODER_r_A ,  ENCODER_r_B );
//
//}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ʼ��
//  @note
//-------------------------------------------------------------------------------------------------------------------
void ecounter_init(void){

    encoder_dir_init(TIM5_ENCODER, ENCODER_R, ENCODER_DIR_R);//���ֱ�����
    encoder_dir_init(TIM2_ENCODER, ENCODER_L, ENCODER_DIR_L);//���ֱ�����

};
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
    int16 CoderOut = 0;
    switch(gptn)
    {
        case TIM2_ENCODER:
            for(int i = 0;i < n;i++)
            {
                if(direct){
                Coder -=  encoder_get_count(TIM2_ENCODER);
                }
                else
                {
                Coder +=  encoder_get_count(TIM2_ENCODER);
                }

            }
            CoderOut = Coder/n;
            break;
        case TIM5_ENCODER:
            for(int i = 0;i < n;i++)
            {
                if(direct){
                    Coder +=  encoder_get_count(TIM5_ENCODER);
                }
                else
                {
                    Coder -=  encoder_get_count(TIM5_ENCODER);
                }

            }
            CoderOut = Coder/n;
            break;
        default:
            break;
    }
    encoder_clear_count(gptn);    //���������

    return CoderOut;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ת��
//  @param      gptn����������Ӧ���
//  @param      n   ��n�ξ�ֵ�˲�
//  @return     int16
//  @note       ������Լӣ��Լ�����ʵ�������������ǰ��������������ֵ��Ϊ��
//-------------------------------------------------------------------------------------------------------------------
void getspeed(void)
{
 // ��ȡ��������ֵ
    Encoder_speed_l = -Encoder_MTM(TIM2_ENCODER,3,1);
    Encoder_speed_r = -Encoder_MTM(TIM5_ENCODER,3,1);
    //JustFloat_Test();
};
