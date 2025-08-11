/***********************************************
* @brief : 描述        车子启动停止状态
* @date  : 修改日期     2024.10.24
* @author: 作者        SJX
************************************************/

#include "Status.h"

Car_Status g_Car_Status = status_car_stop ;
uint8 g_started_debug = 0;

/***********************************************
* @brief : 停车
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void Car_Stop(void)
{
    g_Car_Status = status_car_stop;                      //关闭电机，启用菜单
//  pit_disable(CCU61_CH0);
    target_left = 0;
    target_right = 0;
//    Motor_Stop();
}

/***********************************************
* @brief : 启动
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void Car_Start(void)
{
    system_delay_ms(1000);
    beepShort();
    g_Car_Status = status_car_start;
    PID_clear(&Motor_Speed_PID_Left);
    PID_clear(&Motor_Speed_PID_Right);
    PID_clear(&Turn_Speed_PID);
    PID_clear(&Straight_Speed_PID);
    pit_enable(CCU61_CH0);
}
