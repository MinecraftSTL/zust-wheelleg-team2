#include "sys.h"

const uint8 long_press_time = 50;

uint8 pressed_=0;
uint16 pressed_time[4] = {0};

int8 speed_qidong=0;

uint8 Button_Pressed(void){
    uint8 pressed = 0;
    if(!gpio_get_level(K1))
    {
        pressed |= 0x01;
    }
    if(!gpio_get_level(K2))
    {
        pressed |= 0x02;
    }
    if(!gpio_get_level(K3))
    {
        pressed |= 0x04;
    }
    if(!gpio_get_level(K4))
    {
        pressed |= 0x08;
    }
    return pressed;
}
uint8 Button_Press(void)//°´¼üÉ¨Ãè
{
    uint8 pressed = Button_Pressed();
    uint8 ret = ~pressed_ & pressed;
    pressed_ = pressed;
    for(int i = 0; i < 4; ++i){
        if(pressed_time[i]>=long_press_time){
            ret|=1<<i;
        }
    }
    if(ret){
        beep_on();
        system_delay (10000, 4000);
        beep_off();
        system_delay (10000, 1000);
    }
    return ret;
}
