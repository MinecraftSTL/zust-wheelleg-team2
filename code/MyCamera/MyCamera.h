#ifndef __MyCamera_H
#define __MyCamera_H

typedef struct{
    uint8 image[MT9V03X_IMAGE_SIZE];
    uint16 h, w;
}Image;

extern int g_camera_mid_err;

extern Image imageShow;

void MyCamera_Init(void);
void MyCamera_Show(uint16 start_y);
uint8 Image_get(Image *this, uint16 y, uint16 x);
void Image_set(Image *this, uint16 y, uint16 x, uint8 value);
void Image_fromCamera(Image *this, uint8 *mt9v03x_image);
void Image_clone(Image *this, Image *target);
void Image_cut(Image *this, Image *target, uint16 h, uint16 w);
void Image_zoom(Image *this, Image *target, float zoom);
void Image_binaryzation(Image *this, Image *target, uint16 r, int16 deltaT);
#endif
