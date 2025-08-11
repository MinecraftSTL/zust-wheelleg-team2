/***********************************************
* @brief : ����        ��������ֹͣ״̬
* @date  : �޸�����     2024.10.24
* @author: ����        SJX
************************************************/

#include "Status.h"

uint64 allRunMs = 0;

CarStatus carStatus = CAR_STOP;

void CarStatus_set(CarStatus this)
{
    carStatus = this;
    CarStatus_update();
}
void CarStatus_update(){
    beepMid();
    if(allRunMs < 1000){
        beepLong();
        carStatus = CAR_STOP;
        return;
    }
    CameraStatus_clear();
    carRunMs = 0;
    wheelClear = 0;
    if(carStatus == CAR_RUN){
        binStatus = 1;
    }
    showPInC1 = carStatus != CAR_RUN;
    if(carStatus){
        binStatus = 1;
    }
}
void CarStatus_menu_update(Page *page){
    CarStatus_update();
}
void CarStatus_add(){
    if(carStatus < CAR_RUN){
        CarStatus_set(carStatus+1);
    }
}
void CarStatus_sub(){
    if(carStatus > CAR_STOP){
        CarStatus_set(carStatus-1);
    }
}
void CarStatus_stop(){
    CarStatus_set(CAR_STOP);
}
void CarStatus_pit(uint32 period){
    allRunMs += period;
}
