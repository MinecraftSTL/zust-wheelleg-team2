/*
 * Wifi_Image.c
 *
 *  Created on: 2024��10��26��
 *      Author: sun
 */
#include "WifiImage.h"
#include "MyCamera.h"

uint8 g_wifi_image_open_flag = 0;

Rgb565Image wifiImage;

/***********************************************
* @brief : wifiͼ����ʼ��
* @param : ��
* @return: ��
* @date  : 2024.10.27
* @author: SJX
************************************************/
void Wifi_Image_Init(void)
{
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
        printf("connect wifi failed.\r\n");
        if(key_is_pressed()){
            return;
        }
        system_delay_ms(100);                                                   // ��ʼ��ʧ�� �ȴ� 100ms
        if(key_is_pressed()){
            return;
        }
    }

    printf("module version:%s\r\n",wifi_spi_version);                          // ģ��̼��汾
    printf("module mac    :%s\r\n",wifi_spi_mac_addr);                         // ģ�� MAC ��Ϣ
    printf("module ip     :%s\r\n",wifi_spi_ip_addr_port);                     // ģ�� IP ��ַ

    // zf_device_wifi_spi.h �ļ��ڵĺ궨����Ը���ģ������(����) WIFI ֮���Ƿ��Զ����� TCP ������������ UDP ����
    if(1 != WIFI_SPI_AUTO_CONNECT)                                              // ���û�п����Զ����� ����Ҫ�ֶ�����Ŀ�� IP
    {
        while(wifi_spi_socket_connect(                                          // ��ָ��Ŀ�� IP �Ķ˿ڽ��� TCP ����
            "TCP",                                                              // ָ��ʹ��TCP��ʽͨѶ
            WIFI_SPI_TARGET_IP,                                                 // ָ��Զ�˵�IP��ַ����д��λ����IP��ַ
            WIFI_SPI_TARGET_PORT,                                               // ָ��Զ�˵Ķ˿ںţ���д��λ���Ķ˿ںţ�ͨ����λ��Ĭ����8080
            WIFI_SPI_LOCAL_PORT))                                               // ָ�������Ķ˿ں�
        {
            // ���һֱ����ʧ�� ����һ���ǲ���û�н�Ӳ����λ
            printf("Connect TCP Servers error, try again.\r\n");
            if(key_is_pressed()){
                return;
            }
            system_delay_ms(100);                                               // ��������ʧ�� �ȴ� 100ms
            if(key_is_pressed()){
                return;
            }
        }
    }
    g_wifi_image_open_flag = 1;
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
        Rgb565Image_mode(&showImage, &wifiImage);
        uint16 x = 5, y = 0;
        Rgb565Image_showUintColor(&wifiImage,wifiImage.w-x*6,8*y++,0x00f8,fps.fps,3);
        Rgb565Image_showUintColor(&wifiImage,wifiImage.w-x*6,8*y++,0x00f8,cameraStatus,2);
        Rgb565Image_showIntColor(&wifiImage,wifiImage.w-x*6,8*y++,0x00f8,cameraErr,3);
        Rgb565Image_showIntColor(&wifiImage,wifiImage.w-x*6,8*y++,0x00f8,(Encoder_speed_l+Encoder_speed_r)/2,4);
        Rgb565Image_showIntColor(&wifiImage,wifiImage.w-x*6,8*y++,0x00f8,(int)cameraV,4);
        seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_RGB565, wifiImage.image, wifiImage.w, wifiImage.h);
        seekfree_assistant_camera_send();
    }
}


