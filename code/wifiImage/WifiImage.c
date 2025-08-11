/*
 * Wifi_Image.c
 *
 *  Created on: 2024年10月26日
 *      Author: sun
 */
#include "WifiImage.h"
#include "MyCamera.h"

uint8 g_wifi_image_open_flag = 0;

Rgb565Image wifiImage;

/***********************************************
* @brief : wifi图传初始化
* @param : 无
* @return: 无
* @date  : 2024.10.27
* @author: SJX
************************************************/
void Wifi_Image_Init(void)
{
    int i = 0;
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST) )
    {
        i++;
        printf("\r\n connect wifi failed. \r\n");
        ips200_show_string(150, 16*2, "failed");
        if(key_is_pressed()){
            return;
        }
    }
    if(i == 0)
    {
        ips200_show_string(150, 16*2, "suss");
        printf("\r\n module ip     :%s",wifi_spi_ip_addr_port);                     // 模块 IP 地址
    }
    g_wifi_image_open_flag = 1;
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
    beepMid();

}
/***********************************************
* @brief : wifi图传发送摄像头图像
* @param : 无
* @return: 无
* @date  : 2024.10.27
* @author: SJX
************************************************/
void Wifi_Image_Send_Camera(void)
{
    if(g_wifi_image_open_flag){
        Rgb565Image_mode(&showImage, &wifiImage);
        seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_RGB565, wifiImage.image, wifiImage.w, wifiImage.h);
        seekfree_assistant_camera_send();
    }
}


