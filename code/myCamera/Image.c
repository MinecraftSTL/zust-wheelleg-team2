/*
 * Image.c
 *
 *  Created on: 2025年7月15日
 *      Author: minec
 */
#include "Image.h"

ips200_font_size_enum static_Image_font = IPS200_6X8_FONT;
uint16 static_Image_bgcolor = 0x0000;
uint8 static_Image_bgcolorA = 1;

uint8 inline Image_get(Image *this, uint16 x, uint16 y){
    if(x >= this->w || y >= this->h){
        return 0;
    }
    return this->image[x + this->w*y];
}
void inline Image_set(Image *this, uint16 x, uint16 y, uint8 value){
    if(x >= this->w || y >= this->h){
        return;
    }
    this->image[x + this->w*y] = value;
}
void Image_fromCamera(Image *this, uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W]){
    zf_assert(!!this);
    this->w = MT9V03X_W;
    this->h = MT9V03X_H;
    memcpy(this->image, mt9v03x_image, MT9V03X_IMAGE_SIZE);
}
void Image_clear(Image *this){
    zf_assert(!!this);
    memset(&this->image, 0x00, sizeof(uint8)*this->w*this->h);
}
void Image_clone(Image *this, Image *target)
{
    zf_assert(this && target);
    target->w = this->w;
    target->h = this->h;
    memcpy(target->image, this->image, sizeof(uint8)*this->w*this->h);
}
void Image_cut(Image *this, uint16 l, uint16 r, uint16 t, uint16 b){
    zf_assert(!!this);
    if(!(l||r||t||b)){
        return;
    }
    zf_assert(this->w-l-r>=0);
    zf_assert(this->h-t-b>=0);
    uint16 w = this->w-l-r, h = this->h-t-b;
    for(uint16 i = 0; i < h; ++i){
        memmove(&this->image[i*w], &this->image[l+this->w*(t+i)], sizeof(uint8)*w);
    }
    this->w = w;
    this->h = h;
}
void Image_zoom(Image *this, Image *target, float zoom){
    zf_assert(this && target && zoom > 0.0f);
    if(zoom == 1){
        return;
    }
    target->w = (this->w-1)*zoom+1;
    target->h = (this->h-1)*zoom+1;
    for(uint16 i = 0; i < target->w; ++i){
        for(uint16 j = 0; j < target->h; ++j){
            Image_set(target,i,j,Image_get(this,i/zoom,j/zoom));
        }
    }
}
void Image_toRgb565Image(Image *this, Rgb565Image *target){
    zf_assert(!!this);
    target->w = this->w;
    target->h = this->h;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = this->image[i]>>(8-5)<<(5+6)|this->image[i]>>(8-6)<<5|this->image[i]>>(8-5);
    }
}
void Rgb565Image_clone(Rgb565Image *this, Rgb565Image *target)
{
    zf_assert(this && target);
    target->w = this->w;
    target->h = this->h;
    memcpy(target->image, this->image, sizeof(uint16)*this->w*this->h);
}
void Rgb565Image_clear(Rgb565Image *this){
    zf_assert(!!this);
    memset(&this->image, 0x0000, sizeof(uint16)*this->h*this->w);
}
void Rgb565Image_mode(Rgb565Image *this, Rgb565Image *target){
    zf_assert(this && target);
    target->w = this->w;
    target->h = this->h;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = (this->image[i] << 8) | (this->image[i] >> 8);
    }
}
uint16 inline Rgb565Image_get(Rgb565Image *this, uint16 x, uint16 y){
    if(x >= this->w || y >= this->h){
        return 0;
    }
    return this->image[x + this->w*y];
}
void inline Rgb565Image_set(Rgb565Image *this, uint16 x, uint16 y, uint16 value){
    if(x >= this->w || y >= this->h){
        return;
    }
    this->image[x + this->w*y] = value;
}
void Rgb565Image_mark(Rgb565Image *this, uint16 x, uint16 y, uint16 color, uint16 r){
    zf_assert(!!this);
    for(int i = -r; i <= r; ++i){
        Rgb565Image_set(this, x-i, y-i, color);
        Rgb565Image_set(this, x+i, y-i, color);
    }
}
void Rgb565Image_circle(Rgb565Image *this, uint16 x, uint16 y, uint16 color, uint16 r){
    zf_assert(!!this);
    uint16 rs = r * 0.707;
    for(int16 i = -rs; i <= rs; ++i){
        float d = sqrtf(r*r-i*i);
        if(isnan(d)){
            continue;
        }
        Rgb565Image_set(this, x+d, y+i, color);
        Rgb565Image_set(this, x-d, y+i, color);
        Rgb565Image_set(this, x-i, y+d, color);
        Rgb565Image_set(this, x+i, y-d, color);
    }
}
void Rgb565Image_showCharColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const char dat)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < this->w);
    zf_assert(y < this->h);

    uint8 i = 0, j = 0;

    switch(static_Image_font)
    {
        case IPS200_6X8_FONT:
        {
            for(i = 0; 6 > i; i ++)
            {
                // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                uint8 temp_top = ascii_font_6x8[dat - 32][i];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        Rgb565Image_set(this, x + i, y + j, color);
                    }
                    else if(!static_Image_bgcolorA)
                    {
                        Rgb565Image_set(this, x + i, y + j, static_Image_bgcolor);
                    }
                    temp_top >>= 1;
                }
            }
        }break;
        case IPS200_8X16_FONT:
        {
            for(i = 0; 8 > i; i ++)
            {
                uint8 temp_top = ascii_font_8x16[dat - 32][i];
                uint8 temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        Rgb565Image_set(this, x + i, y + j, color);
                    }
                    else if(!static_Image_bgcolorA)
                    {
                        Rgb565Image_set(this, x + i, y + j, static_Image_bgcolor);
                    }
                    temp_top >>= 1;
                }
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_bottom & 0x01)
                    {
                        Rgb565Image_set(this, x + i, y + j, color);
                    }
                    else if(!static_Image_bgcolorA)
                    {
                        Rgb565Image_set(this, x + i, y + j + 8, static_Image_bgcolor);
                    }
                    temp_bottom >>= 1;
                }
            }
        }break;
        case IPS200_16X16_FONT:
        {
            // 暂不支持
        }break;
    }
}
void Rgb565Image_showStringColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const char dat[])
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < this->w);
    zf_assert(y < this->h);

    for(uint16 j = 0; dat[j] != '\0'; ++j)
    {
        switch(static_Image_font)
        {
            case IPS200_6X8_FONT:   Rgb565Image_showCharColor(this, x + 6 * j, y, color, dat[j]); break;
            case IPS200_8X16_FONT:  Rgb565Image_showCharColor(this, x + 8 * j, y, color, dat[j]); break;
            case IPS200_16X16_FONT: break;                                      // 暂不支持
        }
    }
}
void Rgb565Image_showIntColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const int32 dat, uint8 num)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < this->w);
    zf_assert(y < this->h);
    zf_assert(0 < num);
    zf_assert(10 >= num);

    int32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    Rgb565Image_showStringColor(this, x, y, color, (const char *)&data_buffer);
}
void Rgb565Image_showUintColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const uint32 dat, uint8 num)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < this->w);
    zf_assert(y < this->h);
    zf_assert(0 < num);
    zf_assert(10 >= num);

    uint32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    Rgb565Image_showStringColor(this, x, y, color, (const char *)&data_buffer);
}
void Rgb565Image_showFloatColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const double dat, uint8 num, uint8 pointnum)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 那么一般是屏幕显示的时候超过屏幕分辨率范围了
    zf_assert(x < this->w);
    zf_assert(y < this->h);
    zf_assert(0 < num);
    zf_assert(8 >= num);
    zf_assert(0 < pointnum);
    zf_assert(6 >= pointnum);

    double dat_temp = dat;
    double offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    for(; 0 < num; num --)
    {
        offset *= 10;
    }
    dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    func_double_to_str(data_buffer, dat_temp, pointnum);
    Rgb565Image_showStringColor(this, x, y, color, data_buffer);
}
