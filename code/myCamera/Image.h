/*
 * Image.h
 *
 *  Created on: 2025Äê7ÔÂ15ÈÕ
 *      Author: MCSTL
 */

#ifndef CODE_IMAGE_IMAGE_H_
#define CODE_IMAGE_IMAGE_H_
#include "zf_device_mt9v03x.h"
#include "zf_device_ips200.h"

typedef struct{
    uint8 image[MT9V03X_IMAGE_SIZE];
    uint16 w, h;
}Image;
typedef struct{
    uint16 image[MT9V03X_IMAGE_SIZE];
    uint16 h, w;
}Rgb565Image;

uint8 Image_get(Image *this, uint16 x, uint16 y);
void Image_set(Image *this, uint16 x, uint16 y, uint8 value);
void Image_fromCamera(Image *this, uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W]);
void Image_clear(Image *this);
void Image_clone(Image *this, Image *target);
void Image_cut(Image *this, uint16 x0, uint16 y0, uint16 x1, uint16 y1);
void Image_zoom(Image *this, Image *target, float zoom);
uint16 Rgb565Pixel_fromW8Pixel(uint8 w8);
uint16 Rgb565Pixel_mix(uint16 b, uint16 t, uint8 alpha);
void Image_toRgb565Image(Image *this, Rgb565Image *target);
void Rgb565Image_clone(Rgb565Image *this, Rgb565Image *target);
void Rgb565Image_clear(Rgb565Image *this);
void Rgb565Image_mode(Rgb565Image *this, Rgb565Image *target);
uint16 Rgb565Image_get(Rgb565Image *this, uint16 x, uint16 y);
void Rgb565Image_set(Rgb565Image *this, uint16 x, uint16 y, uint16 value);
void Rgb565Image_mark(Rgb565Image *this, uint16 x, uint16 y, uint16 color, uint16 r);
void Rgb565Image_circle(Rgb565Image *this, uint16 x, uint16 y, uint16 color, uint16 r);
void Rgb565Image_showCharColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const char dat);
void Rgb565Image_showStringColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const char dat[]);
void Rgb565Image_showIntColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const int32 dat, uint8 num);
void Rgb565Image_showUintColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const uint32 dat, uint8 num);
void Rgb565Image_showFloatColor (Rgb565Image *this, uint16 x, uint16 y, uint16 color, const double dat, uint8 num, uint8 pointnum);

#endif /* CODE_IMAGE_IMAGE_H_ */
