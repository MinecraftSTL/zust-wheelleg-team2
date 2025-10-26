/*
 * myIps200.c
 *
 *  Created on: 2025年4月5日
 *      Author: MCSTL
 */
#include "MyIps200.h"

void ips200_init_spi(){
    ips200_init(IPS200_TYPE_SPI);
    ips200_clear();
}

void ips200_reset_color(){
    ips200_set_pencolor(IPS200_DEFAULT_PENCOLOR);
}

void ips200_set_pencolor(const uint16 color){
    ips200_set_color(color, IPS200_DEFAULT_BGCOLOR);
}
void ips200_show_char_color(uint16 x, uint16 y, const char dat, const uint16 color){
    ips200_set_pencolor(color);
    ips200_show_char(x,y,dat);
    ips200_reset_color();
}
void ips200_show_string_color(uint16 x, uint16 y, const char dat[], const uint16 color){
    ips200_set_pencolor(color);
    ips200_show_string(x,y,dat);
    ips200_reset_color();
}
void ips200_show_char_invert(uint16 x, uint16 y, const char dat, uint8 x0, uint8 x1, uint8 y0, uint8 y1){
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < ips200_width_max);
    zf_assert(y < ips200_height_max);

    uint8 i = 0, j = 0;

    if(IPS200_TYPE_SPI == ips200_display_type)
    {
        IPS200_CS(0);
    }
    switch(ips200_display_font)
    {
        case IPS200_6X8_FONT:
        {
            uint16 display_buffer[6*8];
            ips200_set_region(x, y, x + 5, y + 7);
            for(i = 0; 6 > i; i ++)
            {
                // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                uint8 temp_top = ascii_font_6x8[dat - 32][i];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01 ^ (x0 <= i && i < x1 && y0 <= j && j < y1))
                    {
                        display_buffer[i + j * 6] = (ips200_pencolor);
                    }
                    else
                    {
                        display_buffer[i + j * 6] = (ips200_bgcolor);
                    }
                    temp_top >>= 1;
                }
            }
            ips200_write_16bit_data_array(display_buffer, 6*8);
        }break;
        case IPS200_8X16_FONT:
        {
            uint16 display_buffer[8*16];
            ips200_set_region(x, y, x + 7, y + 15);
            for(i = 0; 8 > i; i ++)
            {
                uint8 temp_top = ascii_font_8x16[dat - 32][i];
                uint8 temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; 8 > j; j ++)
                {
                    if((temp_top & 0x01) ^ (x0 <= i && i < x1 && y0 <= j && j < y1))
                    {
                        display_buffer[i + j * 8] = (ips200_pencolor);
                    }
                    else
                    {
                        display_buffer[i + j * 8] = (ips200_bgcolor);
                    }
                    temp_top >>= 1;
                }
                for(j = 0; 8 > j; j ++)
                {
                    if((temp_bottom & 0x01) ^ (x0 <= i && i < x1 && y0 <= j + 8 && j < y1 + 8))
                    {
                        display_buffer[i + j * 8 + 8 * 8] = (ips200_pencolor);
                    }
                    else
                    {
                        display_buffer[i + j * 8 + 8 * 8] = (ips200_bgcolor);
                    }
                    temp_bottom >>= 1;
                }
            }
            ips200_write_16bit_data_array(display_buffer, 8 * 16);
        }break;
        case IPS200_16X16_FONT:
        {
            zf_assert(0);// 暂不支持
        }break;
    }
    if(IPS200_TYPE_SPI == ips200_display_type)
    {
        IPS200_CS(1);
    }
}
void ips200_show_string_invert (uint16 x, uint16 y, const char dat[], uint8 x0, uint8 x1, uint8 y0, uint8 y1)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < ips200_width_max);
    zf_assert(y < ips200_height_max);

    for(uint16 j = 0; dat[j] != '\0'; ++j)
    {
        switch(ips200_display_font)
        {
            case IPS200_6X8_FONT:
                ips200_show_char_invert(x + 6 * j, y, dat[j],
                        6*j<=x0 ? x0<6*(j+1) ? x0-6*j : 6 : 0,
                        6*j<=x1 ? x1<6*(j+1) ? x1-6*j : 6 : 0,
                                y0, y1);
                break;
            case IPS200_8X16_FONT:
                ips200_show_char_invert(x + 8 * j, y, dat[j],
                        8*j<=x0 ? x0<8*(j+1) ? x0-8*j : 8 : 0,
                        8*j<=x1 ? x1<8*(j+1) ? x1-8*j : 8 : 0,
                                y0, y1);
                break;
            case IPS200_16X16_FONT: zf_assert(0);break;                                      // 暂不支持
        }
    }
}
