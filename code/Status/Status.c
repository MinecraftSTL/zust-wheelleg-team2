/***********************************************
* @brief : ����        ��������ֹͣ״̬
* @date  : �޸�����     2024.10.24
* @author: ����        SJX
************************************************/

#include "Status.h"

Car_Status carStatus = status_car_stop;

/***********************************************
* @brief : ͣ��
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void Car_Stop(void)
{
    beepMid();
    carStatus = status_car_stop;
}

/***********************************************
* @brief : ����
* @param : void
* @return: void
* @date  : 2024_10
* @author: SJX
************************************************/
void Car_Start(void)
{
    beepMid();
    carStatus = status_car_start;
}
