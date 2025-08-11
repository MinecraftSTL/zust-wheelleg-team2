/*
作者：Charon and 快乐牌小刀片
       未经授权禁止转售
 */

#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_

//#define ENCODER_l                 (TIM2_ENCODER)                         // 正交编码器对应使用的编码器接口
//#define ENCODER_l_A               (TIM2_ENCODER_CH2_P33_6)               // A 相对应的引脚
//#define ENCODER_l_B               (TIM2_ENCODER_CH1_P33_7)               // B 相对应的引脚
//#define ENCODER_r                 (TIM5_ENCODER)                         // 正交编码器对应使用的编码器接口
//#define ENCODER_r_A               (TIM5_ENCODER_CH1_P20_3)               // A 相对应的引脚
//#define ENCODER_r_B               (TIM5_ENCODER_CH2_P20_0)               // B 相对应的引脚

#define ENCODER_L         TIM5_ENCODER_CH1_P10_3     //左编码器计数引脚
#define ENCODER_DIR_L     TIM5_ENCODER_CH2_P10_1    //左编码器方向引脚

#define ENCODER_R         TIM2_ENCODER_CH1_P33_7     //右编码器计数引脚
#define ENCODER_DIR_R     TIM2_ENCODER_CH2_P33_6    //右编码器方向引脚


//void encoder_init(void);
//void encoder_filter(void);
void ecounter_init(void);
int16 Encoder_MTM(encoder_index_enum gptn,int n,uint8 direct);
void getspeed(void);
extern int16 Encoder_speed_l;
extern int16 Encoder_speed_r;

#endif /* CODE_ENCODER_H_ */
