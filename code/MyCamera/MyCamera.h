#ifndef __MyCamera_H
#define __MyCamera_H

typedef enum{
    NONE,
    IN_CROSS,
    OUT_CROSS,
}CameraStatus;

typedef struct{
    uint16 y, x;
}dot;
typedef struct{
    uint8 image[MT9V03X_IMAGE_SIZE];
    uint16 h, w;
}Image;
typedef struct{
    uint16 image[MT9V03X_IMAGE_SIZE];
    uint16 h, w;
}Rgb565Image;
typedef struct{
    Image *image;
    Image cache;
    uint16 h, w, r;
    int16 deltaT;
}BinImage;

extern int camera_err;

extern Rgb565Image showImage;

extern int binR;
extern int binDeltaT;
extern int bly2RDR;
extern float RD2IE;
extern float IGFE;
extern int invY;
extern float invK;
extern int crossY;
extern int crossX;
extern uint8 showPInC1;
extern uint8 showBin;

extern CameraStatus status;

void MyCamera_Init(void);
void MyCamera_Show(uint16 start_y);
uint8 Image_get(Image *this, uint16 y, uint16 x);
void Image_set(Image *this, uint16 y, uint16 x, uint8 value);
void Image_fromCamera(Image *this, uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W]);
void Image_clone(Image *this, Image *target);
void Image_cut(Image *this, Image *target, uint16 y0, uint16 x0, uint16 y1, uint16 x1);
void Image_toRgb565Image(Image *this, Rgb565Image *target);
void Rgb565Image_clone(Rgb565Image *this, Rgb565Image *target);
void Rgb565Image_mode(Rgb565Image *this, Rgb565Image *target);
uint16 Rgb565Image_get(Rgb565Image *this, uint16 y, uint16 x);
void Rgb565Image_set(Rgb565Image *this, uint16 y, uint16 x, uint16 value);
void Rgb565Image_mark(Rgb565Image *this, uint16 y, uint16 x, uint16 color, uint16 size);
void Image_zoom(Image *this, Image *target, float zoom);
void Image_binaryzation(Image *this, Image *target, uint16 r, int16 deltaT);
void BinImage_init(BinImage *this, Image *image, uint16 r, int16 deltaT);
uint8 BinImage_get(BinImage *this, uint16 y, uint16 x);
void BinImage_set(BinImage *this, uint16 y, uint16 x, uint8 value);
void BinImage_drawRectan(BinImage *this);
#endif
