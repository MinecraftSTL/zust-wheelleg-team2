/***********************************************
* @brief : ����        ��������ֹͣ״̬
* @date  : �޸�����     2024.10.24
* @author: ����        SJX
************************************************/

#include "Status.h"

Car_Status g_Car_Status = status_car_stop ;
uint8 g_started_debug = 0;

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
    g_Car_Status = status_car_stop;
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
    g_Car_Status = status_car_start;
}
