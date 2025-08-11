#ifndef MyCamera_H
#define MyCamera_H

#include "MyHeadfile.h"

#define MAX_BLY ((MT9V03X_W)*4)

typedef enum{
    NONE,
    I_ZEBRA,
    O_ZEBRA,
    I_CROSS,
    R_CROSS,
    O_CROSS,
    I_LCIRCLE,
    PTI_LCIRCLE,
    TI_LCIRCLE,
    R_LCIRCLE,
    TO_LCIRCLE,
    PO_LCIRCLE,
    O_LCIRCLE,
    OR_CROSS_LCIRCLE,
    I_RCIRCLE,
    PTI_RCIRCLE,
    TI_RCIRCLE,
    R_RCIRCLE,
    TO_RCIRCLE,
    PO_RCIRCLE,
    O_RCIRCLE,
    OR_CROSS_RCIRCLE,
    RAMP,
    I_BARRIER,
    R_BARRIER,
    O_BARRIER,
    I_BRIDGE,
    I_LBRIDGE,
    O_LBRIDGE,
    I_RBRIDGE,
    O_RBRIDGE,
    O_BRIDGE,
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
extern int cameraErr;

extern Rgb565Image showImage;

extern float vignetteK;
extern uint8 binStatus;
extern int binDeltaT;
extern float trapezoidK;
extern int trapezoidY;
extern int startMaxYAdd;
extern int bly2RDL;
extern float RD2IErr;
extern float facingErr;
extern int invY;
extern float invK;
extern int setLineY;
extern int straightYMin;
extern int straightStep;
extern float straightErr;
extern float statusJump;
extern float statusK;
extern int zebraY;
extern int zebraT;
extern int zebraS;
extern int crossX;
extern int crossY;
extern int circleX;
extern int circleY;
extern float circleLine;
extern int rampS;
extern int rampY;
extern float rampK;
extern int barrierY0;
extern int barrierY1;
extern int barrierT;
extern int bridgeY;
extern float bridgeKPitchX;
extern int bridgeTI;
extern int bridgeTO;
extern int bridgeS;
extern float bridgeZ;
extern uint8 bridgeDetectL;
extern int errY;
extern int errDeltaY;
extern uint8 showPInC1;
extern uint8 showWait;

extern float bendV;
extern float circleV;
extern float rampV;
extern float bridgeV;

extern float cameraV;

extern CameraStatus cameraStatus;
extern uint64 carRunMs;

void MyCamera_Init(void);
void Image_showCamera(uint16 x, uint16 y);
void CameraStatus_set(CameraStatus this);
void Camera_pit(uint32 period, int16 speed);

#endif
