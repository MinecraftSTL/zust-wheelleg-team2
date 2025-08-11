#include "sys.h"
char button_choice=0;
int8 speed_qidong=1;//should be zero, opened by munu
uint16 Button_Get(void)//°´¼üÉ¨Ãè
{
    uint16 return_flag = 0;
    if(gpio_get_level(K1) == 0)
    {
        button_choice=1;
        return_flag |= 0x01;
        ips200_clear();
        beep_on();
        system_delay (1000, 1000);
        beep_off();
    }
    if(gpio_get_level(K2) == 0)
    {

        button_choice=2;
        return_flag |= 0x02;
        ips200_clear();
        beep_on();
        system_delay (1000, 1000);
        beep_off();
    }
    if(gpio_get_level(K3) == 0)
    {
        button_choice=4;
        return_flag |= 0x04;
        ips200_clear();
        beep_on();
        system_delay (1000, 1000);
        beep_off();
    }
    if(gpio_get_level(K4) == 0)
    {
        button_choice=4;
        return_flag |= 0x08;
        ips200_clear();
        beep_on();
        system_delay (1000, 1000);
        beep_off();
    }
    while(gpio_get_level(K1)||gpio_get_level(K2)||gpio_get_level(K3)||gpio_get_level(K4) == 0);
   // gpio_set_level(Beep, 0);
    return return_flag;
}
