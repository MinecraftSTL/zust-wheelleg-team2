/***********************************************
* @brief : 描述        车子启动停止状态
* @date  : 修改日期     2024.10.24
* @author: 作者        SJX
************************************************/

#include "Status.h"

uint64 allRunMs = 0;

CarStatus carStatus = CAR_STOP;

uint8 angleProtect = 1;
uint8 fpsProtect = 1;
uint8 timeProtect = 0;

uint64 timeProtectT = 0;
uint8 kill = 0;

uint64 timeProtectNowT = 0;

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

    timeProtectNowT = 0;
    kill = 0;

    showPInC1 = carStatus != CAR_RUN;
    if(carStatus == CAR_RUN){
        binEn = 1;
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
    if(timeProtect && !kill && carStatus >= CAR_RUN && timeProtectNowT != 0){
        uint64 t = allRunMs - timeProtectNowT;
        if(timeProtectT != 0 && t >= timeProtectT){
            kill = 1;
        }
    }
}

void CarStatus_CameraStatus_zebra(){
    if(carStatus < CAR_RUN){
        return;
    }
    if(timeProtectNowT){
        uint64 t = allRunMs - timeProtectNowT;
        if(timeProtectT == 0 || t < timeProtectT){
            timeProtectT = t;
        }
    }else{
        timeProtectNowT = allRunMs;
    }
}
