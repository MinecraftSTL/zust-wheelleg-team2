/***********************************************
* @brief : 描述        车子启动停止状态
* @date  : 修改日期     2024.10.24
* @author: 作者        SJX
************************************************/

#include "Status.h"

CarStatus carStatus = CAR_STOP;

/***********************************************
* @brief : 停车
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void CarStatus_set(CarStatus this)
{
    beepMid();
    carStatus = this;
    CameraStatus_set(NONE);
    carRunTick = 0;
}
