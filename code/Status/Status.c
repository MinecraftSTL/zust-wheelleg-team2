/***********************************************
* @brief : ����        ��������ֹͣ״̬
* @date  : �޸�����     2024.10.24
* @author: ����        SJX
************************************************/

#include "Status.h"

CarStatus carStatus = CAR_STOP;

/***********************************************
* @brief : ͣ��
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
