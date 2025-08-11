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
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
        printf("connect wifi failed.\r\n");
        if(key_is_pressed()){
            return;
        }
        system_delay_ms(100);                                                   // 初始化失败 等待 100ms
        if(key_is_pressed()){
            return;
        }
    }

    printf("module version:%s\r\n",wifi_spi_version);                          // 模块固件版本
    printf("module mac    :%s\r\n",wifi_spi_mac_addr);                         // 模块 MAC 信息
    printf("module ip     :%s\r\n",wifi_spi_ip_addr_port);                     // 模块 IP 地址

    // zf_device_wifi_spi.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接
    if(1 != WIFI_SPI_AUTO_CONNECT)                                              // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_spi_socket_connect(                                          // 向指定目标 IP 的端口建立 TCP 连接
            "TCP",                                                              // 指定使用TCP方式通讯
            WIFI_SPI_TARGET_IP,                                                 // 指定远端的IP地址，填写上位机的IP地址
            WIFI_SPI_TARGET_PORT,                                               // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
            WIFI_SPI_LOCAL_PORT))                                               // 指定本机的端口号
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            printf("Connect TCP Servers error, try again.\r\n");
            if(key_is_pressed()){
                return;
            }
            system_delay_ms(100);                                               // 建立连接失败 等待 100ms
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


