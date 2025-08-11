#ifndef MyCamera_H
#define MyCamera_H

#include "MyHeadfile.h"
#include "Image.h"

#define MAX_BLY ((MT9V03X_W)*4)

typedef enum{
    CAMERA_STATUS_NONE,
    I_ZEBRA,
    O_ZEBRA,
    I_CROSS,
    R_CROSS,
    O_CROSS,
    I_LBLOCK,
    O_LBLOCK,
    I_RBLOCK,
    O_RBLOCK,
    I_LCIRCLE,
    TI_LCIRCLE,
    R_LCIRCLE,
    TO_LCIRCLE,
    O_LCIRCLE,
    OR_CROSS_LCIRCLE,
    I_RCIRCLE,
    TI_RCIRCLE,
    R_RCIRCLE,
    TO_RCIRCLE,
    O_RCIRCLE,
    OR_CROSS_RCIRCLE,
    RAMP,
    I_BARRIER,
    R_BARRIER,
    O_BARRIER,
    OR_RAMP_BARRIER,
    I_BRIDGE,
    I_LBRIDGE,
    O_LBRIDGE,
    I_RBRIDGE,
    O_RBRIDGE,
    O_BRIDGE,
    CAMERA_STATUS_NUMBER,
}CameraStatus;

extern int cameraErr;

extern Rgb565Image showImage;

extern int cutL;
extern int cutR;
extern int cutT;
extern int cutB;
extern float shadowK;
extern float vignetteK;
extern uint8 binEn;
extern int binDeltaT;
extern float trapezoidK;
extern int trapezoidY;
extern int startMaxYAdd;
extern int bly2RDL;
extern int reBlyY;
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
extern int elementYMin;
extern int elementX;
extern int zebraY;
extern int zebraT;
extern int zebraS;
extern int crossX;
extern int blockY0;
extern int blockY1;
extern int circleX;
extern int rampS;
extern int rampY;
extern float rampZ;
extern float rampK;
extern int barrierY0;
extern int barrierY10;
extern int barrierY11;
extern int barrierT;
extern float bridgeKPitchX;
extern int bridgeTI;
extern int bridgeTO;
extern int bridgeS;
extern float bridgeZ;
extern int bridgeDetectL;
extern float otherBPow;
extern int errY;
extern int errDeltaY;

extern uint8 showPInC1;
extern uint8 showWait;

extern int showCARL_a;
extern int showCARL_x00;
extern int showCARL_x01;
extern int showCARL_y0;
extern int showCARL_x10;
extern int showCARL_x11;
extern int showCARL_y1;

extern uint8 zebraEn;
extern uint8 crossEn;
extern uint8 lBlockEn;
extern uint8 rBlockEn;
extern uint8 lCircleEn;
extern uint8 rCircleEn;
extern uint8 rampEn;
extern uint8 barrierEn;
extern uint8 bridgeEn;
extern uint8 orCLcEn;
extern uint8 orCRcEn;
extern uint8 orRBEn;

extern float bendV;
extern float circleV;
extern float rampDV;
extern float barrierV0;
extern float barrierV1;
extern float bridgeV0;
extern float bridgeV1;

extern int bendErrY;
extern int blockErrY;
extern int circleErrY;

extern float cameraV;
extern uint16 cameraY;

extern CameraStatus cameraStatus;
extern uint64 carRunMs;

void MyCamera_Init(void);
void Image_showCamera(uint16 x, uint16 y);
void CameraStatus_set(CameraStatus this);
void CameraStatus_clear();
void Camera_pit(uint32 period, int16 speed);

#endif
