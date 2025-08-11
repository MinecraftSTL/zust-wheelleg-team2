#ifndef __MyCamera_H
#define __MyCamera_H

#include "MyHeadfile.h"

#define MAX_BLY ((MT9V03X_W)*3)

typedef enum{
    NONE,
    I_ZEBRA,
    O_ZEBRA,
    IN_CROSS,
    CROSS,
    O_CROSS,
    I_LCIRCLE,
    PTI_LCIRCLE,
    TI_LCIRCLE,
    LCIRCLE,
    TO_LCIRCLE,
    PO_LCIRCLE,
    O_LCIRCLE,
    OR_CROSS_LCIRCLE,
    I_RCIRCLE,
    PTI_RCIRCLE,
    TI_RCIRCLE,
    RCIRCLE,
    TO_RCIRCLE,
    PO_RCIRCLE,
    O_RCIRCLE,
    OR_CROSS_RCIRCLE,
    CAMERA_STATUS_NUMBER,
}CameraStatus;

typedef struct{
    uint8 image[MT9V03X_IMAGE_SIZE];
    uint16 h, w;
}Image;
typedef struct{
    uint16 image[MT9V03X_IMAGE_SIZE];
    uint16 h, w;
}Rgb565Image;
extern int camera_err;

extern Rgb565Image showImage;

extern int binDeltaT;
extern int maxStartYAdd;
extern int bly2RDL;
extern float RD2IErr;
extern float facingErr;
extern int invY;
extern float invK;
extern int setLineY;
extern int straightYMin;
extern int straightStep;
extern float straightErr;
extern int statusJump;
extern int zebraY;
extern int crossX;
extern int circleX;
extern int circleY;
extern float circleLine;
extern int errY;
extern int errDeltaY;
extern uint8 showPInC1;
extern uint8 showWait;

extern CameraStatus cameraStatus;

void MyCamera_Init(void);
void Image_showCamera(uint16 x, uint16 y);
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
void Rgb565Image_mark(Rgb565Image *this, uint16 y, uint16 x, uint16 color, uint16 r);
void Image_zoom(Image *this, Image *target, float zoom);
void Image_binaryzation(Image *this, int16 deltaT);
#endif
