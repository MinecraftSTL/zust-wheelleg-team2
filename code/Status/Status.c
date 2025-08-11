/***********************************************
* @brief : 描述        车子启动停止状态
* @date  : 修改日期     2024.10.24
* @author: 作者        SJX
************************************************/

#include "Status.h"

Car_Status carStatus = status_car_stop;

/***********************************************
* @brief : 停车
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void Car_stop(void)
{
    beepMid();
    carStatus = status_car_stop;
    cameraStatus = NONE;
}

/***********************************************
* @brief : 启动
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void Car_start(void)
{
    beepMid();
    carStatus = status_car_start;
}
