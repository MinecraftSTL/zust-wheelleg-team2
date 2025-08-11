/*
 * Wifi_Image.c
 *
 *  Created on: 2024��10��26��
 *      Author: sun
 */
#include "WIFI_IMAGE.H"
#include "MyCamera.h"

uint8 g_wifi_image_open_flag = 1;

/***********************************************
* @brief : wifiͼ����ʼ��
* @param : ��
* @return: ��
* @date  : 2024.10.27
* @author: SJX
************************************************/
void Wifi_Image_Init(void)
{
    int i = 0;
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST) )
    {
        i++;
        g_wifi_image_open_flag = 0;
        printf("\r\n connect wifi failed. \r\n");
        ips200_show_string(150, 18*2, "failed");
        break;
    }
    if(i == 0)
    {
        ips200_show_string(150, 18*2, "suss");
        printf("\r\n module ip     :%s",wifi_spi_ip_addr_port);                     // ģ�� IP ��ַ
    }
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
    beepMid();

}
/***********************************************
* @brief : wifiͼ����������ͷͼ��
* @param : ��
* @return: ��
* @date  : 2024.10.27
* @author: SJX
************************************************/
void Wifi_Image_Send_Camera(void)
{
    if(g_wifi_image_open_flag){
        seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_RGB565, showImage.image, showImage.w, showImage.h);
        seekfree_assistant_camera_send();
    }
}


