#include "MyCamera.h"

#pragma section all "cpu1_dsram"

Image image;
Image image1;
Rgb565Image showImage;

int binDeltaT = 0;
float trapezoidK = 0.8;
int trapezoidY = 20;
int maxStartYAdd = 20;
int bly2RDL = 3;
float RD2IErr = 0.7854;
float facingErr = 0.5236;
int setLineY = 15;
int straightYMin = 30;
int straightStep = 10;
float straightErr = 0.087266;
float statusJump = 3;
float statusK = 0.98;
int crossX = 15;
int zebraY = 110;
int zebraStartT = 10000;
int zebraStopT = 500;
int circleX = 7;
int circleY = 80;
float circleLine = 0.4;
int rampX = 200000;
int barrierY0 = 15;
int barrierY1 = 15;
int barrierT = 400;
int bridgeY = 5;
int bridgeTI = 1000;
int bridgeTO = 2000;
int bridgeX = 100000;
int bridgeZ = -100;
#define cameraAngle (5)
int errY = 60;
int errDeltaY = 30;
uint8 showPInC1 = 1;
uint8 showWait = 0;

float bendV = 500;
float circleV = 400;
float rampV = 0;
float bridgeV = 100;

float cameraV = 0;

int16 lStart[2];
int16 rStart[2];

int16 lLine[MAX_BLY][2];
uint8 lLineDir[MAX_BLY];
uint16 lLineL = 0;
int16 rLine[MAX_BLY][2];
uint8 rLineDir[MAX_BLY];
uint16 rLineL = 0;
int16 lrMeet;

uint16 lBorder[MT9V03X_H];
uint16 rBorder[MT9V03X_H];
uint16 mLine[MT9V03X_H];

uint8 lStraight;
uint8 rStraight;

float lRadDir[MAX_BLY];
uint16 lRadDirLine[MAX_BLY];
uint16 lRadDirN;
float rRadDir[MAX_BLY];
uint16 rRadDirLine[MAX_BLY];
uint16 rRadDirN;

uint16 lInfLine[MAX_BLY];
float lInfRad[MAX_BLY];
uint16 lInfN;
uint16 rInfLine[MAX_BLY];
float rInfRad[MAX_BLY];
uint16 rInfN;

uint8 camera_process_cnt = 0;

int camera_err = 0;              //摄像头中线偏差

void MyCamera_Init(void)
{
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips200_show_string(0, 16, "init success.");
}

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
void Image_fromCamera(Image *this, uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W])
{
    this->w = MT9V03X_W;
    this->h = MT9V03X_H;
    memcpy(this->image, mt9v03x_image, MT9V03X_IMAGE_SIZE);
}
void Image_clear(Image *this){
    memset(&this->image, 0x00, sizeof(uint8)*this->h*this->w);
}
void Image_clone(Image *this, Image *target)
{
    zf_assert(this && target);
    target->w = this->w;
    target->h = this->h;
    memcpy(target->image, this->image, this->h*this->w*sizeof(uint8));
}
void Image_cut(Image *this, uint16 x0, uint16 y0, uint16 x1, uint16 y1){
    zf_assert(!!this);
    uint16 w_ = this->w, h_ = this->h, w = x1-x0, h = y1-y0;
    for(uint16 i = 0; i < h; ++i){
        for(uint16 j = 0; j < w; ++j){
            this->w = w_;
            this->h = h_;
            uint8 value = Image_get(this,j+x0,i+y0);
            this->w = w;
            this->h = h;
            Image_set(this,j,i,value);
        }
    }
}
void Image_zoom(Image *this, Image *target, float zoom){
    zf_assert(this && target && zoom > 0.0f);
    target->w = (this->w-1)*zoom+1;
    target->h = (this->h-1)*zoom+1;
    for(uint16 i = 0; i < target->w; ++i){
        for(uint16 j = 0; j < target->h; ++j){
            Image_set(target,i,j,Image_get(this,i/zoom,j/zoom));
        }
    }
}
void Image_toRgb565Image(Image *this, Rgb565Image *target){
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
    memcpy(target->image, this->image, this->h*this->w*sizeof(uint16));
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
    for(int i = -r; i <= r; ++i){
        Rgb565Image_set(this, x-i, y-i, color);
        Rgb565Image_set(this, x+i, y-i, color);
    }
}

uint8 Fast_OTSU(Image *this)
{
    static int16 isFirstBinary = 1;                       // 首次二值化标志位
    static int16 threshold;                               // 二值化阈值
    int16        minGrayValue = 0, maxGrayValue = 255;    // 最小、最大有效灰度值（用来减少遍历）
    // int16 Pix_Dis = (ROW_DIS * MT9V03X_CSI_W) / 2;      //去除图像未参与识别的部分

    // 以下变量要注意数据溢出，根据实际的分辨率修改数据类型。
    uint32 grayHist[256] = {0};    // 灰度直方图（这里至少要用int16，否则一种灰度出现的次数太多就会溢出）
    int32       sumOfPixel     = this->h * this->w;                 // 像素总数
    int32       sumOfGrayValue = 0;                         // 整幅图像的总灰度值
    int32       sumOfBackPixel = 0, sumOfForcePixel = 0;    // 前后像素总数
    int32       sumOfBackGray = 0, sumOfForceGray = 0;      // 前后总灰度值

    float w0, w1, u0, u1;                   // 背景、前景像素比例
    float maxVariance = -1, tmpVariance;    // 类间方差

    for (int i = 0; i < sumOfPixel; ++i)    // 用一重循环可以避免计算数组下标时带入乘法，降低了复杂度
    {
        ++grayHist[this->image[i]];    // 获得灰度直方图，统计0~255每个灰度出现了几次（这个算法大概90%的时间花在这里）
    }
    while (minGrayValue < 256 && 0 == grayHist[minGrayValue])    // 搜索最小有效灰度值
    {
        ++minGrayValue;
    }
    while (maxGrayValue >= minGrayValue && 0 == grayHist[maxGrayValue])    // 搜索最大有效灰度值
    {
        --maxGrayValue;
    }
    if (maxGrayValue - minGrayValue <= 1)    // 如果灰度值不超过2种(当然这种情况是几乎不可能的)
    {
        return (uint8) minGrayValue;    // 直接算出最佳阈值
    }
    for (int16 i = minGrayValue; i <= maxGrayValue; ++i) {
        sumOfGrayValue += i * grayHist[i];    // 计算总灰度值
    }
    for (int16 i = minGrayValue; i <= maxGrayValue; ++i)    // 求方差的过程简化为一重循环，大大降低了算法复杂度
    {
        sumOfBackPixel += grayHist[i];        // 背景总像素数
        sumOfBackGray  += i * grayHist[i];    // 背景总灰度值
        if (!isFirstBinary)  // 不是第一次二值化，本次阈值在上次阈值的附近（这样能快几十us）
        {
            if (i < threshold - 5 && maxGrayValue > threshold)    // 和上次阈值的差距 > 某个值，不计算类间方差。
            {
                continue;
            }
            if (i > threshold + 5 && minGrayValue < threshold) {
                return (uint8) threshold;    // 最大类间方差对应的灰度值，认为是最佳分割阈值
            }
        }                                          // 计算类间方差
        sumOfForcePixel = sumOfPixel - sumOfBackPixel;                 // 前景像素总数
        sumOfForceGray  = sumOfGrayValue - sumOfBackGray;              // 前景总灰度值
        w0              = (float) sumOfBackPixel / sumOfPixel;         // 背景像素比例
        w1              = 1 - w0;                                      // 前景像素比例
        u0              = (float) sumOfBackGray / sumOfBackPixel;      // 背景灰度均值
        u1              = (float) sumOfForceGray / sumOfForcePixel;    // 前景灰度均值
        // 代入u=w0*u0+w1*u1，类间方差w0*(u-u0)^2+w1*(u-u1)^2可化简为w0*w1*(u0-u1)^2
        tmpVariance     = w0 * w1 * (u0 - u1) * (u0 - u1);    // 类间方差
        if (tmpVariance > maxVariance) {
            maxVariance = tmpVariance;    // 记录最大类间方差
            threshold   = i;              // 记录最大类间方差对应的灰度值
        }
    }
    isFirstBinary = 0;           // 完成第一次二值化，清空标志位
    return (uint8) threshold;    // 最大类间方差对应的灰度值，认为是最佳分割阈值
}

void Image_binaryzation(Image *this, int16 deltaT){
    zf_assert(!!this);
    int16 threshold = Fast_OTSU(this);
    threshold += deltaT;
    for(uint16 i = 0; i < this->h*this->w; ++i){
        this->image[i]=this->image[i]>threshold?0xFF:0;
    }
}
void Image_drawBlackFrame(Image *this)
{
    for (uint16 i = 0; i < this->h; i++)
    {
        Image_set(this,0,i,0);
        Image_set(this,this->w - 1,i,0);

    }
    for (uint16 i = 0; i < this->w; i++)
    {
        Image_set(this,i,0,0);
        Image_set(this,i,this->h - 1,0);
    }
}
uint16 Image_getTrapezoidX(Image *this, uint16 y){
    if(this->h > 1+trapezoidY+y){
        return (this->h-1-trapezoidY-y)*trapezoidK;
    }else{
        return 0;
    }
}
void Image_drawBlackTrapezoid(Image *this){
    for (uint16 i = 0; i < this->w; i++)
    {
        Image_set(this,i,0,0);
        Image_set(this,i,this->h - 1,0);
    }
    for(uint16 i=0; i<this->h; ++i){
        for(uint16 j=0; j<=Image_getTrapezoidX(this, i); ++j){
            Image_set(this, j, i, 0);
            Image_set(this, this->w-1-j, i, 0);
        }
    }
}

void Image_getStartPoint(Image *this, uint16 maxYAdd, int16 lStart[2], int16 rStart[2]){
    int16 y = this->h-2;

    while(maxYAdd--){
        uint8 inWhite = 0;
        int16 whiteBorder[MT9V03X_W][3];
        uint16 whiteBorderLen = 0, maxWhiteWidth = 0;
        for(int16 i=0; i<this->w; ++i){
            if(!Image_get(this, i, y)){
                if(Image_get(this, i+1, y) &&!Image_get(this, i-1, y) && Image_get(this, i+2, y)){
                    if(!inWhite){
                        inWhite = 1;
                    }
                    whiteBorder[whiteBorderLen][0] = i;
                }else if(Image_get(this, i-1, y) && !Image_get(this, i+1, y) && Image_get(this, i-2, y)){
                    if(inWhite){
                        inWhite = 0;
                        whiteBorder[whiteBorderLen][1] = i;
                        whiteBorder[whiteBorderLen][2] = i - whiteBorder[whiteBorderLen][0];
                        if(whiteBorder[whiteBorderLen][2] > maxWhiteWidth){
                            maxWhiteWidth = whiteBorder[whiteBorderLen][2];
                        }
                        ++whiteBorderLen;
                    }
                }
            }
        }
        if(maxWhiteWidth > 0){
            for(int16 i=0; i<whiteBorderLen; ++i){
                if(whiteBorder[i][2] == maxWhiteWidth){
                    lStart[0]=whiteBorder[i][0];
                    rStart[0]=whiteBorder[i][1];
                    lStart[1]=rStart[1]=y;
                    return;
                }
            }
        }
        --y;
    }
}

const int8 Bly_SEED[8][2] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
void Image_bly(Image *this, uint16 maxL, int16 lLine[MAX_BLY][2], int16 rLine[MAX_BLY][2],
        uint8 lDir[MAX_BLY], uint8 rDir[MAX_BLY], uint16 *lLineL, uint16 *rLineL, int16 *meet,
        uint16 lStart[2], uint16 rStart[2])
{
    zf_assert(maxL <= MAX_BLY); //逆天

    uint8 lStop = 0, rStop = 0;

    *meet = -1;

    lLine[0][0] = lStart[0];
    lLine[0][1] = lStart[1];
    rLine[0][0] = rStart[0];
    rLine[0][1] = rStart[1];
    lDir[0] = rDir[0] = 2;

    *lLineL = *rLineL = 1;

    while(maxL--){
        int16 sub_r_l = rLine[*rLineL-1][1] - lLine[*lLineL-1][1];
        if(!lStop && sub_r_l<=0){
            uint8 lStep = 0;
            for(uint8 i=0; i<8; ++i){
                lDir[*lLineL] = (lDir[*lLineL-1]+6+i)&0x07;
                lLine[*lLineL][0] = lLine[*lLineL-1][0] + Bly_SEED[lDir[*lLineL]][0];
                lLine[*lLineL][1] = lLine[*lLineL-1][1] + Bly_SEED[lDir[*lLineL]][1];
                uint8 dir = (lDir[*lLineL]+7)&0x07;
                int16 dot[2];
                dot[0] = lLine[*lLineL-1][0] + Bly_SEED[dir][0];
                dot[1] = lLine[*lLineL-1][1] + Bly_SEED[dir][1];
                if(lLine[*lLineL][0] < 0 || lLine[*lLineL][1] < 0 || lLine[*lLineL][0] >= this->w || lLine[*lLineL][1] >= this->h ||
                        dot[0] < 0 || dot[1] < 0 || dot[0] >= this->w || dot[1] >= this->h){
                    continue;
                }
                if(!Image_get(this, lLine[*lLineL][0], lLine[*lLineL][1]) && Image_get(this, dot[0], dot[1])){
                    ++*lLineL;
                    lStep = 1;
                    break;
                }
            }
            if(!lStep){
                lStop = 1;
            }
        }
        if(!rStop && sub_r_l>=0){
            uint8 rStep = 0;
            for(uint8 i=0; i<8; ++i){
                rDir[*rLineL] = (rDir[*rLineL-1]+2+(8-i))&0x07;
                rLine[*rLineL][0] = rLine[*rLineL-1][0] + Bly_SEED[rDir[*rLineL]][0];
                rLine[*rLineL][1] = rLine[*rLineL-1][1] + Bly_SEED[rDir[*rLineL]][1];
                uint8 dir = (rDir[*rLineL]+1)&0x07;
                int16 dot[2];
                dot[0] = rLine[*rLineL-1][0] + Bly_SEED[dir][0];
                dot[1] = rLine[*rLineL-1][1] + Bly_SEED[dir][1];
                if(rLine[*rLineL][0] < 0 || rLine[*rLineL][1] < 0 || rLine[*rLineL][0] >= this->w || rLine[*rLineL][1] >= this->h ||
                        dot[0] < 0 || dot[1] < 0 || dot[0] >= this->w || dot[1] >= this->h){
                    continue;
                }
                if(!Image_get(this, rLine[*rLineL][0], rLine[*rLineL][1]) && Image_get(this, dot[0], dot[1])){
                    ++*rLineL;
                    rStep = 1;
                    break;
                }
            }
            if(!rStep){
                rStop = 1;
            }
        }
        if (abs(rLine[*rLineL-1][0] - lLine[*lLineL-1][0]) < 2
            && abs(rLine[*rLineL-1][1] - lLine[*lLineL-1][1]) < 2){
            *meet = sub_r_l>0 ? rLine[*rLineL-1][1] : lLine[*lLineL-1][1];
            break;
        }
        if (lStop && rStop){
            break;
        }
    }
}
void Image_blyToBorder(Image *this, uint8 dir, int16 line[MAX_BLY][2], uint16 lineL, uint16 ret[MT9V03X_H]){
//    for(uint16 i=0; i<this->h; ++i){
//        ret[i] = dir ? this->w-1 : 0;
//    }
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = dir ? this->w-1-Image_getTrapezoidX(this, i) : Image_getTrapezoidX(this, i);
    }
    for(uint16 i=0; i<lineL; ++i){
        if(0 <= line[i][1] && line[i][1] < this->h){
            if(dir?(line[i][0] < ret[line[i][1]]):(line[i][0] > ret[line[i][1]])){
                ret[line[i][1]]=line[i][0];
            }
        }
    }
}

uint8 inline Image_borderXIsLose(Image *this, uint16 x, uint16 y, uint8 dir){
//    return dir?x >= this->w-1:x < 1;
    uint16 loseX = Image_getTrapezoidX(this, y);
    return dir ? x >= this->w-1-loseX: x<=loseX;
}
uint8 inline Image_borderIsLose(Image *this, uint16 border[MT9V03X_H], uint16 y, uint8 dir){
    if(y >= this->h){
        return 1;
    }
    return Image_borderXIsLose(this, border[y], y, dir);
}
uint8 inline Image_borderIsAllLose(Image *this, uint16 border[MT9V03X_H], uint16 y0, uint16 y1, uint8 dir){
    for(uint16 i=y0; i<=y1; ++i){
        if(!Image_borderIsLose(this, border, i, dir)){
            return 0;
        }
    }
    return 1;
}
uint8 inline Image_borderIsNoneLose(Image *this, uint16 border[MT9V03X_H], uint16 y0, uint16 y1, uint8 dir){
    for(uint16 i=y0; i<=y1; ++i){
        if(Image_borderIsLose(this, border, i, dir)){
            return 0;
        }
    }
    return 1;
}

void Image_borderToMiddle(Image *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], int16 lrMeet, uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
    if(lrMeet >= 0){
        for(uint16 i=0; i<lrMeet; ++i){
            ret[i] = ret[lrMeet];
        }
    }
}
void Image_borderSetCLine(Image *this, uint16 border[MT9V03X_H], uint16 x0, uint16 y0, uint16 x1, uint16 y1){
    if(y0>y1){
        y0 ^= y1;
        y1 ^= y0;
        y0 ^= y1;
        x0 ^= x1;
        x1 ^= x0;
        x0 ^= x1;
    }
    float slope = (float)(x1-x0)/(y1-y0);
    for(uint16 i = y0; i <= y1; ++i){
        uint16 x = (uint16)(slope*(i-y0)+x0);
        if(x >= this->w || i >= this->h){
            continue;
        }
        border[i] = x;
    }
}

void leastSquares(const uint16 pos[MAX_BLY][2], const uint16 n, float *slope, float *intercept) {
    // 初始化无效值
    *slope = NAN;
    *intercept = NAN;

    // 检查有效点数
    if (n < 2) {
        return;
    }

    float sum_x = 0.f, sum_y = 0.f, sum_x2 = 0.f, sum_xy = 0.f;

    // 累加计算
    for (uint16 i = 0; i < n; ++i) {
        float x = pos[i][0];
        float y = pos[i][1];
        sum_x += x;
        sum_y += y;
        sum_x2 += x * x;
        sum_xy += x * y;
    }

    // 计算分母
    const float denominator = n * sum_x2 - sum_x * sum_x;
    if (fabsf(denominator) < 1e-10) {
        return;
    }

    // 计算浮点结果
    const float m = (n * sum_xy - sum_x * sum_y) / denominator;
    const float b = (sum_y - m * sum_x) / n;

    // 最终有效赋值
    *slope = m;
    *intercept = b;
}

void Image_borderSetSLine(Image *this, uint16 border[MT9V03X_H], uint16 pos[MT9V03X_H][2], uint16 n, uint16 y0, uint16 y1){
    float slope, intercept;
    leastSquares(pos, n, &slope, &intercept);
    if(!isnan(slope) && !isnan(intercept)){
        for(uint16 i = y0; i <= y1; ++i){
            float x = slope*i+intercept;
            if(x >= this->w || i >= this->h){
                continue;
            }
            border[i] = (uint16)func_limit_ab(x, 0, this->w-1);
        }
    }
}
void Image_borderSetULine(Image *this, uint16 border[MT9V03X_H], uint16 y){
    if(y+setLineY >= this->h-1){
        return;
    }
    uint16 pos[MT9V03X_H][2];
    for(uint16 i = 0; i < setLineY; ++i){
        pos[i][0] = y+i;
        pos[i][1] = border[y+i];
    }
    Image_borderSetSLine(this, border, pos, setLineY, 0, y);
}
void Image_borderSetDLine(Image *this, uint16 border[MT9V03X_H], uint16 y){
    if(y < setLineY){
        return;
    }
    uint16 pos[MT9V03X_H][2];
    for(uint16 i = 0; i < setLineY; ++i){
        pos[i][0] = y-i;
        pos[i][1] = border[y-i];
    }
    Image_borderSetSLine(this, border, pos, setLineY, y, this->h-1);
}

uint8 Image_borderIsStraight(Image *this, uint16 border[MT9V03X_H], uint16 yMin, uint16 yMax, uint16 step, float err, uint8 dir){
    uint8 findYMax = 0;
    for(uint16 i=yMax; i>yMin; --i){
        if(!Image_borderIsLose(this, border, i, dir)){
            yMax = i;
            findYMax = 1;
            break;
        }
    }
    if(!findYMax){
        return 0;
    }
    if(yMax - yMin < 3*step){
        return 0;
    }
    if(!Image_borderIsNoneLose(this, border, yMin, yMax, dir)){
        return 0;
    }
    float all = atan2f(yMax-yMin,border[yMin]-border[yMax]);
    if(dir?all < PI/2:all > PI/2){
        return 0;
    }
    for(uint16 i=yMin; i+step<=yMax; i+=step){
        if(fabsf(all-atan2f(step,border[i]-border[i+step])) > err){
            return 0;
        }
    }
    return 1;
}

void Image_blyToRadDir(Image *this, int16 bly[MAX_BLY][2], uint16 blyL, uint16 l,
        float ret[MAX_BLY], int16 retPosLine[MAX_BLY], uint16 *retN){
    *retN = (blyL-1)/l;
    for(uint16 i=0; i<*retN; ++i){
        ret[i]=atan2f(-(bly[(i+1)*l][1]-bly[i*l][1]), bly[(i+1)*l][0]-bly[i*l][0]);
        for(uint16 j=i*l; j<=(i+1)*l; ++j){
            if(bly[j][1] == 0 || bly[j][1] == this->h-1 || Image_borderXIsLose(this, bly[j][0], bly[j][1], 0) || Image_borderXIsLose(this, bly[j][0], bly[j][1], 1)){
                ret[i] = NAN;
                break;
            }
        }
        retPosLine[i] = i*l+(l>>1);
    }
}
void RadDir_toInflection(float this[MAX_BLY], int16 thisLine[MAX_BLY], uint16 thisN, float err,
        int16 retLine[MAX_BLY], float retRad[MAX_BLY], uint16 *retN){
    *retN = 0;
    for(uint16 i = 1; i < thisN-1; ++i){
        if(isnan(this[i-1]) || isnan(this[i+1])){
            continue;
        }
        if(fabs(Angle_normalize(this[i+1]-this[i-1])) >= PI/2-err){
            retLine[*retN] = thisLine[i];
            float nA = Angle_normalize(this[i-1]+PI);
            retRad[*retN] = Angle_normalize((this[i+1]+nA)/2+(fabsf(this[i+1]-nA)>PI?PI:0));
            ++*retN;
            i+=2;
        }
    }
}
uint8 Inflection_getFacing(float rad){
    for(uint8 i=0; i<4; ++i){
        if(fabsf(Angle_normalize(PI/4+i*PI/2)-rad)<=facingErr){
            return i+1;
        }
    }
    return 0;
}
int16 Inflection_getFirstFacing(uint16 this[MAX_BLY], float thisRad[MAX_BLY], uint16 thisN, uint8 facing){
    for(uint16 i=0; i<thisN; ++i){
        if(Inflection_getFacing(thisRad[i])==facing){
            return i;
        }
    }
    return -1;
}


uint64 carRunMs = 0;
CameraStatus cameraStatus = NONE;
float statusScore[CAMERA_STATUS_NUMBER] = {0};
uint64 statusKeepMs = 0;
uint64 statusRunX = 0;
void CameraStatus_set(CameraStatus this){
    beepMid();
    memset(statusScore, 0.f, sizeof(float)*CAMERA_STATUS_NUMBER);
    cameraStatus = this;
    statusKeepMs = 0;
    statusRunX = 0;
}
void CameraStatus_addScore(CameraStatus this){
    if(++statusScore[this] >= statusJump){
        CameraStatus_set(this);
    }
}
void Image_zebraCrossing(Image *this, float *cameraV, uint16 *errY){
    uint8 zebra = 0;
    {
        int edge_left_num = 0;
        int edge_right_num = 0;
        int edge_sum = 0;
        for(uint16 zebra_row = zebraY; zebra_row < zebraY+3; zebra_row++)
        {
            for(uint16 i=lBorder[zebra_row];i<=this->w/2;i++)
            {
                if(Image_get(this, i, zebra_row) && Image_get(this, i+1, zebra_row) &&
                        !Image_get(this, i+2, zebra_row) && !Image_get(this, i+3, zebra_row))
                {
                    edge_left_num++;
                }
            }
            for(uint16 i=rBorder[zebra_row];i>this->w/2;i--)
            {
                if(Image_get(this, i, zebra_row) && Image_get(this, i-1, zebra_row) &&
                        !Image_get(this, i-2, zebra_row) && !Image_get(this,i-3, zebra_row))
                {
                    edge_right_num++;
                }
            }
        }
        edge_sum = edge_left_num + edge_right_num;
        if(edge_right_num > 200)
            edge_right_num = 0;
        if(edge_left_num > 200)
            edge_left_num = 0;
        if(edge_sum >= 16 && edge_left_num > 5 && edge_right_num > 5)
        {
            zebra = 1;
        }
    }
    switch(cameraStatus){
        case I_ZEBRA:
            if(!zebra){
                CameraStatus_addScore(O_ZEBRA);
            }
            break;
        case O_ZEBRA:
            if(statusKeepMs >= zebraStopT){
                CameraStatus_set(NONE);
                if(carStatus == CAR_RUN && carRunMs > zebraStartT){
                    CarStatus_set(CAR_BALANCE);
                }
            }
            break;
        default:
            if(zebra){
                CameraStatus_set(I_ZEBRA);
            }
            break;
    }
}
void Image_cross(Image *this, float *cameraV, uint16 *errY){
    switch(cameraStatus){
        case NONE:
        case OR_CROSS_LCIRCLE:
        case OR_CROSS_RCIRCLE:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3 && rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4 &&
                    Image_borderIsLose(this, lBorder, lLine[lInfLine[0]][1]-crossX, 0) && Image_borderIsLose(this, lBorder, lLine[lInfLine[0]][1]-crossX-1, 0) &&
                    Image_borderIsLose(this, rBorder, rLine[rInfLine[0]][1]-crossX, 1) && Image_borderIsLose(this, rBorder, rLine[rInfLine[0]][1]-crossX-1, 1)
                    )
            {
                CameraStatus_addScore(IN_CROSS);
            }
            if((lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    Image_borderIsLose(this, lBorder, lLine[lInfLine[1]][1]+crossX, 0) && Image_borderIsLose(this, lBorder, lLine[lInfLine[1]][1]+crossX+1, 0) &&
                    Image_borderIsLose(this, rBorder, lLine[lInfLine[0]][1], 1) && Image_borderIsLose(this, rBorder, lLine[lInfLine[0]][1]-1, 1)||
                    rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    Image_borderIsLose(this, rBorder, rLine[rInfLine[1]][1]+crossX, 1) && Image_borderIsLose(this, rBorder, rLine[rInfLine[1]][1]+crossX+1, 1) &&
                    Image_borderIsLose(this, lBorder, rLine[rInfLine[0]][1], 0) && Image_borderIsLose(this, lBorder, rLine[rInfLine[0]][1]-1, 0)) &&
                    lLine[lInfLine[1]][1]+crossX+1 < this->h && rLine[rInfLine[1]][1]+crossX+1 < this->h){
                CameraStatus_addScore(R_CROSS);
            }
            break;
        case IN_CROSS:
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    lLine[lInfLine[1]][1]+crossX+1 < this->h && rLine[rInfLine[1]][1]+crossX+1 < this->h){
                CameraStatus_addScore(R_CROSS);
            }else if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 ||
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                CameraStatus_addScore(O_CROSS);
            }
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3 &&
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4){
                Image_borderSetULine(this, lBorder, lLine[lInfLine[0]][1]);
                Image_borderSetULine(this, rBorder, rLine[rInfLine[0]][1]);
            }
            break;
        case R_CROSS:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 ||
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                CameraStatus_addScore(O_CROSS);
            }
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[1]][0], lLine[lInfLine[1]][1]);
            }
            if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[1]][0], rLine[rInfLine[1]][1]);
            }
            break;
        case O_CROSS:
            if(!(lInfN > 0 && (Inflection_getFacing(lInfRad[0]) == 2 || Inflection_getFacing(lInfRad[0]) == 3) ||
                    rInfN > 0 && (Inflection_getFacing(rInfRad[0]) == 1 || Inflection_getFacing(rInfRad[0]) == 4))){
                CameraStatus_addScore(NONE);
            }
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[1]][0], lLine[lInfLine[1]][1]);
            }else if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2){
                Image_borderSetDLine(this, lBorder, lLine[lInfLine[0]][1]);
            }
            if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[1]][0], rLine[rInfLine[1]][1]);
            }else if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                Image_borderSetDLine(this, rBorder, rLine[rInfLine[0]][1]);
            }
            break;
    }
}
void Image_lCircle(Image *this, float *cameraV, uint16 *errY){
    switch(cameraStatus){
        case NONE:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3 && Image_borderIsLose(this, lBorder, lLine[lInfLine[0]][1]-circleX, 0) &&
                    Image_borderIsLose(this, lBorder, lLine[lInfLine[0]][1]-circleX-1, 0) && lLine[lInfLine[0]][1] > circleY && rStraight){
                CameraStatus_addScore(I_LCIRCLE);
            }
            break;
        case I_LCIRCLE:
            if(lInfN > 0 && (Inflection_getFacing(lInfRad[0]) == 3 && lStart[1]-setLineY < lLine[lInfLine[0]][1] || Inflection_getFacing(lInfRad[0]) == 2)){
                CameraStatus_addScore(PTI_LCIRCLE);
            }
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3){
                Image_borderSetULine(this, lBorder, lLine[lInfLine[0]][1]);
            }
            *cameraV = circleV;
            break;
        case PTI_LCIRCLE:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 || lInfN > 1 && Inflection_getFacing(lInfRad[1]) == 2){
                CameraStatus_addScore(TI_LCIRCLE);
            }
            if(lInfN > 0){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lStart[0], lStart[1]);
            }
            *cameraV = circleV;
            break;
        case TI_LCIRCLE:
            if(lInfN == 0 && rInfN == 0 && lrMeet > 0){
                CameraStatus_addScore(R_LCIRCLE);
            }
            int16 x, y = -1;
            if(lInfN > 0 && fabsf(Angle_normalize(PI/2 - lInfRad[0])) <= facingErr + PI/4 &&
                    (lInfN <= 1 || (PI < lInfRad[1] || lInfRad[1] < PI*3/4-facingErr) || Inflection_getFacing(lInfRad[0]) == 3)){
                if(lInfN > 1 && fabsf(Angle_normalize(PI/2 - lInfRad[1])) <= PI/4+facingErr){
                    Image_borderSetCLine(this, rBorder, lLine[lInfLine[1]][0], lLine[lInfLine[1]][1], rStart[0], rStart[1]);
                    x = lLine[lInfLine[1]][0];
                    y = lLine[lInfLine[1]][1];
                }else if(lInfN > 0 && fabsf(Angle_normalize(PI/2 - lInfRad[0])) <= facingErr + PI/4){
                    Image_borderSetCLine(this, rBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], rStart[0], rStart[1]);
                    x = lLine[lInfLine[0]][0];
                    y = lLine[lInfLine[0]][1];
                }else if(rInfN > 0 && fabsf(Angle_normalize(PI/2 - rInfRad[0])) <= facingErr + PI/4){
                    Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rStart[0], rStart[1]);
                    x = rLine[rInfLine[0]][0];
                    y = rLine[rInfLine[0]][1];
                }
            }
            for(; y>=0; --y){
                lBorder[y] = 0;
                rBorder[y] = x;
            }
            *cameraV = circleV;
            break;
        case R_LCIRCLE:
            if(rInfN > 0 && fabsf(Angle_normalize(0 - rInfRad[0])) <= facingErr + PI/4){
                CameraStatus_addScore(TO_LCIRCLE);
            }
            *cameraV = circleV;
            break;
        case TO_LCIRCLE:
            if(rInfN == 0){
                CameraStatus_addScore(PO_LCIRCLE);
            }
            if(rInfN > 0 && fabsf(Angle_normalize(0 - rInfRad[0])) <= facingErr + PI/4){
                if(lrMeet >= 0){
                    int16 y = -1;
                    for(uint16 i=lrMeet; i<this->h; ++i){
                        if(Image_borderIsLose(this, lBorder, i, 0)){
                            y = i;
                            break;
                        }
                    }
                    if(y>=0){
                        Image_borderSetCLine(this, rBorder, 0, y, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1]);
                    }
                }
            }
            *cameraV = circleV;
            break;
        case PO_LCIRCLE:
            if(lInfN > 0 && fabsf(Angle_normalize(PI/2 - lInfRad[0])) <= facingErr + PI/4 && setLineY < lLine[lInfLine[0]][1]){
                CameraStatus_addScore(O_LCIRCLE);
            }
            if(lrMeet > 0){
                Image_borderSetCLine(this, rBorder, rBorder[lrMeet]<this->w*(1-circleLine) ? rBorder[lrMeet] : this->w*(1-circleLine), lrMeet, rStart[0], rStart[1]);
            }
            *cameraV = circleV;
            break;
        case O_LCIRCLE:
            if((lInfN == 0 || Inflection_getFacing(lInfRad[0]) == 3) && (rInfN == 0 || Inflection_getFacing(rInfRad[0]) == 4)){
                CameraStatus_addScore(NONE);
            }
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2){
                Image_borderSetDLine(this, lBorder, lLine[lInfLine[0]][1]);
            }
            break;
    }
}
void Image_rCircle(Image *this, float *cameraV, uint16 *errY){
    switch(cameraStatus){
        case NONE:
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4 && Image_borderIsLose(this, rBorder, rLine[rInfLine[0]][1]-circleX, 1) &&
                    Image_borderIsLose(this, rBorder, rLine[rInfLine[0]][1]-circleX-1, 1) && rLine[rInfLine[0]][1] > circleY && lStraight){
                CameraStatus_addScore(I_RCIRCLE);
            }
            break;
        case I_RCIRCLE:
            if(rInfN > 0 && (Inflection_getFacing(rInfRad[0]) == 4 && rStart[1]-setLineY < rLine[rInfLine[0]][1] || Inflection_getFacing(rInfRad[0]) == 1)){
                CameraStatus_addScore(PTI_RCIRCLE);
            }
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4){
                Image_borderSetULine(this, rBorder, rLine[rInfLine[0]][1]);
            }
            *cameraV = circleV;
            break;
        case PTI_RCIRCLE:
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1 || rInfN > 1 && Inflection_getFacing(rInfRad[1]) == 1){
                CameraStatus_addScore(TI_RCIRCLE);
            }
            if(rInfN > 0){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rStart[0], rStart[1]);
            }
            *cameraV = circleV;
            break;
        case TI_RCIRCLE:
            if(lInfN == 0 && rInfN == 0 && lrMeet > 0){
                CameraStatus_addScore(R_RCIRCLE);
            }
            int16 x, y=-1;
            if(rInfN > 0 && fabsf(Angle_normalize(PI/2 - rInfRad[0])) <= facingErr + PI/4 &&
                    (rInfN <= 1 || (PI/4+facingErr < rInfRad[1] || rInfRad[1] < 0) || Inflection_getFacing(rInfRad[0]) == 4)){
                if(rInfN > 1 && fabsf(Angle_normalize(PI/2 - rInfRad[1])) <= PI/4+facingErr){
                    Image_borderSetCLine(this, lBorder, rLine[rInfLine[1]][0], rLine[rInfLine[1]][1], lStart[0], lStart[1]);
                    x=rLine[rInfLine[1]][0];
                    y=rLine[rInfLine[1]][1];
                }else if(rInfN > 0 && fabsf(Angle_normalize(PI/2 - rInfRad[0])) <= facingErr + PI/4){
                    Image_borderSetCLine(this, lBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], lStart[0], lStart[1]);
                    x=rLine[rInfLine[0]][0];
                    y=rLine[rInfLine[0]][1];
                }else if(lInfN > 0 && fabsf(Angle_normalize(PI/2 - lInfRad[0])) <= facingErr + PI/4){
                    Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lStart[0], lStart[1]);
                    x=lLine[lInfLine[0]][0];
                    y=lLine[lInfLine[0]][1];
                }
            }else{
                printf("%d,%d,%f,%d\r\n",rInfN,Inflection_getFacing(rInfRad[0]),rInfRad[1],Inflection_getFacing(rInfRad[0]));
            }
            for(; y>=0; --y){
                lBorder[y] = x;
                rBorder[y] = this->w-1;
            }
            *cameraV = circleV;
            break;
        case R_RCIRCLE:
            if(lInfN > 0 && fabsf(Angle_normalize(PI - lInfRad[0])) <= facingErr + PI/4){
                CameraStatus_addScore(TO_RCIRCLE);
            }
            *cameraV = circleV;
            break;
        case TO_RCIRCLE:
            if(lInfN == 0){
                CameraStatus_addScore(PO_RCIRCLE);
            }
            if(lInfN > 0 && fabsf(Angle_normalize(PI - lInfRad[0])) <= facingErr + PI/4){
                if(lrMeet >= 0){
                    int16 y = -1;
                    for(uint16 i=lrMeet; i<this->h; ++i){
                        if(Image_borderIsLose(this, rBorder, i, 1)){
                            y = i;
                            break;
                        }
                    }
                    if(y >= 0){
                        Image_borderSetCLine(this, lBorder, this->w-1, y, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1]);
                    }
                }
            }
            *cameraV = circleV;
            break;
        case PO_RCIRCLE:
            if(rInfN > 0 && fabsf(Angle_normalize(PI/2 - rInfRad[0])) <= facingErr + PI/4 && setLineY < rLine[rInfLine[0]][1]){
                CameraStatus_addScore(O_RCIRCLE);
            }
            if(lrMeet > 0){
                Image_borderSetCLine(this, lBorder, lBorder[lrMeet]>this->w*circleLine ? lBorder[lrMeet] : this->w*circleLine, lrMeet, lStart[0], lStart[1]);
            }
            *cameraV = circleV;
            break;
        case O_RCIRCLE:
            if((lInfN == 0 || Inflection_getFacing(lInfRad[0]) == 3) && (rInfN == 0 || Inflection_getFacing(rInfRad[0]) == 4)){
                CameraStatus_addScore(NONE);
            }
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                Image_borderSetDLine(this, rBorder, rLine[rInfLine[0]][1]);
            }
            break;
    }
}
void Image_ramp(Image *this, float *cameraV, uint16 *errY){
    switch(cameraStatus){
        case RAMP:
            if(statusRunX >= rampX){
                CameraStatus_set(NONE);
            }
            *cameraV += rampV;
            break;
    }
}
void Image_barrier(Image *this, float *cameraV, uint16 *errY){
    switch(cameraStatus){
        case NONE:
            if(lInfN == 1 && Inflection_getFacing(lInfRad[0]) == 4 && lLine[lInfLine[0]][1] > barrierY0 &&
                    rInfN == 1 && Inflection_getFacing(rInfRad[0]) == 3 && rLine[rInfLine[0]][1] > barrierY0 &&
                    lrMeet > 0 && !Image_get(this, lBorder[lrMeet], lrMeet-1) && !Image_get(this, lBorder[lrMeet]+1, lrMeet-1) &&
                    !Image_get(this, rBorder[lrMeet], lrMeet-1 && !Image_get(this, rBorder[lrMeet]-1, lrMeet-1))){
                CameraStatus_addScore(I_BARRIER);
            }
            break;
        case I_BARRIER:
            if(lrMeet > barrierY1){
                CameraStatus_addScore(R_BARRIER);
            }
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 4){
                Image_borderSetULine(this, lBorder, lLine[lInfLine[0]][1]);
            }
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 3){
                Image_borderSetULine(this, rBorder, rLine[rInfLine[0]][1]);
            }
            break;
        case R_BARRIER:
            if(carStatus == CAR_RUN)
            jump();
            CameraStatus_set(O_BARRIER);
            break;
        case O_BARRIER:
            if(statusKeepMs >= barrierT){
                CameraStatus_set(NONE);
            }
            for(uint16 i=0; i<this->h; ++i){
                lBorder[i] = rBorder[i] = this->w/2;
            }
            break;
    }
}
void Image_bridge(Image *this, float *cameraV, uint16 *errY){
    switch(cameraStatus){
        case NONE:
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1){
                uint8 notRamp = 0;
                if(lInfN > 3 && Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1 &&
                        lLine[lInfLine[2]][1] > rLine[rInfLine[1]][1]){
                    notRamp = 1;
                }
                if(rInfN > 3 && Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2 &&
                        rLine[rInfLine[2]][1] > lLine[lInfLine[1]][1]){
                    notRamp = 1;
                }
                if(notRamp){
                    CameraStatus_addScore(I_BRIDGE);
                }else{
                    CameraStatus_addScore(RAMP);
                }
            }
            Filter_xAx.alpha = defaultRollAlpha;
            break;
        case I_BRIDGE:
            if(statusKeepMs >= bridgeTI || PID_vVx.Ek_ > 0){
                if(lInfN > 1 && rInfN > 1){
                    if(lInfN > 3 && Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1 &&
                            lLine[lInfLine[2]][1] > rLine[rInfLine[1]][1]){
                        CameraStatus_addScore(I_LBRIDGE);
                    }
                    if(rInfN > 3 && Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2 &&
                            rLine[rInfLine[2]][1] > lLine[lInfLine[1]][1]){
                        CameraStatus_addScore(I_RBRIDGE);
                    }
                }
            }else{
                float k = (float)statusKeepMs/bridgeTI;
                targetLegZ = (-100-defaultLegZ)*k+defaultLegZ;
                Filter_xAx.alpha = defaultRollAlpha*k;
            }
            if(lInfN > 3 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[3]][0], lLine[lInfLine[3]][1]);
            }else if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 1){
                Image_borderSetDLine(this, lBorder, lLine[lInfLine[1]][1]-bridgeY);
            }
            if(rInfN > 3 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[3]][0], rLine[rInfLine[3]][1]);
            }else if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 2){
                Image_borderSetDLine(this, rBorder, rLine[rInfLine[1]][1]-bridgeY);
            }
            *cameraV = 0;
            rollBalance = 1;
            if(PID_vVx.Ek_sum > 0){
                PID_vVx.Ek_sum = 0;
            }
            break;
        case I_LBRIDGE:
            if(lInfN == 0 || lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 1){
                CameraStatus_addScore(O_LBRIDGE);
            }
            if(lInfN > 3 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[3]][0], lLine[lInfLine[3]][1]);
            }
            if(rInfN > 3 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[3]][0], rLine[rInfLine[3]][1]);
            }
            *cameraV = bridgeV;
            targetLegZ = bridgeZ;
            Filter_xAx.alpha = defaultRollAlpha;
            if(PID_vVx.Ek_sum < 0){
                PID_vVx.Ek_sum = 0;
            }
            break;
        case O_LBRIDGE:
            if(lInfN == 0 || Inflection_getFacing(lInfRad[0]) == 3 || lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2){
                if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1){
                    CameraStatus_addScore(I_RBRIDGE);
                }else{
                    CameraStatus_addScore(O_BRIDGE);
                }
            }
            if(lInfN > 3 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[3]][0], lLine[lInfLine[3]][1]);
            }else if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 1){
                Image_borderSetDLine(this, lBorder, lLine[lInfLine[1]][1]-bridgeY);
            }
            if(rInfN > 3 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[3]][0], rLine[rInfLine[3]][1]);
            }
            *cameraV = bridgeV;
            if(PID_vVx.Ek_sum < 0){
                PID_vVx.Ek_sum = 0;
            }
            break;
        case I_RBRIDGE:
            if(rInfN == 0 || rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2){
                CameraStatus_addScore(O_RBRIDGE);
            }
            if(lInfN > 3 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[3]][0], lLine[lInfLine[3]][1]);
            }
            if(rInfN > 3 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[3]][0], rLine[rInfLine[3]][1]);
            }
            *cameraV = bridgeV;
            targetLegZ = bridgeZ;
            Filter_xAx.alpha = defaultRollAlpha;
            if(PID_vVx.Ek_sum < 0){
                PID_vVx.Ek_sum = 0;
            }
            break;
        case O_RBRIDGE:
            if(rInfN == 0 || Inflection_getFacing(rInfRad[0]) == 4 || rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 1){
                if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2){
                    CameraStatus_addScore(I_LBRIDGE);
                }else{
                    CameraStatus_addScore(O_BRIDGE);
                }
            }
            if(rInfN > 3 && Inflection_getFacing(rInfRad[0]) == 3 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    Inflection_getFacing(rInfRad[2]) == 4 && Inflection_getFacing(rInfRad[3]) == 2){
                Image_borderSetCLine(this, rBorder, rLine[rInfLine[0]][0], rLine[rInfLine[0]][1], rLine[rInfLine[3]][0], rLine[rInfLine[3]][1]);
            }else if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 2){
                Image_borderSetDLine(this, rBorder, rLine[rInfLine[1]][1]-bridgeY);
            }
            if(lInfN > 3 && Inflection_getFacing(lInfRad[0]) == 4 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    Inflection_getFacing(lInfRad[2]) == 3 && Inflection_getFacing(lInfRad[3]) == 1){
                Image_borderSetCLine(this, lBorder, lLine[lInfLine[0]][0], lLine[lInfLine[0]][1], lLine[lInfLine[3]][0], lLine[lInfLine[3]][1]);
            }
            *cameraV = bridgeV;
            break;
        case O_BRIDGE:
            if(statusKeepMs >= bridgeTO && statusRunX >= bridgeX){
                CameraStatus_set(NONE);
            }
            *cameraV = bridgeV;
            break;
        default:
            Filter_xAx.alpha = defaultRollAlpha;
    }
    if(cameraStatus < I_BRIDGE || cameraStatus > O_BRIDGE){
        targetLegZ = defaultLegZ;
        rollBalance = 0;
    }
}
void Image_other(Image *this, float *cameraV, uint16 *errY){
    int16 first;
    switch(cameraStatus){
        case NONE:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3 && lLine[lInfLine[0]][1] > circleX+1 && Image_borderIsLose(this, lBorder, lLine[lInfLine[0]][1]-circleX, 0) &&
                    Image_borderIsLose(this, lBorder, lLine[lInfLine[0]][1]-circleX-1, 0) && rStraight){
                CameraStatus_addScore(OR_CROSS_LCIRCLE);
            }
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4 && rLine[rInfLine[0]][1] > circleX+1 && Image_borderIsLose(this, rBorder, rLine[rInfLine[0]][1]-circleX, 1) &&
                    Image_borderIsLose(this, rBorder, rLine[rInfLine[0]][1]-circleX-1, 1) && lStraight){
                CameraStatus_addScore(OR_CROSS_RCIRCLE);
            }
            if(!lStraight || !rStraight){
                *cameraV = bendV;
            }
            break;
        case OR_CROSS_LCIRCLE:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3){
                Image_borderSetULine(this, lBorder, lLine[lInfLine[0]][1]);
            }
            if((lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2 ||
                    lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2) && rStraight){
                CameraStatus_addScore(PTI_LCIRCLE);
            }
            if(lInfN == 0 && rInfN == 0 || lStraight && rStraight){
                CameraStatus_addScore(NONE);
            }
            *cameraV -= 100;
            if(PID_vVx.Ek_sum < 0){
                PID_vVx.Ek_sum = 0;
            }
            break;
        case OR_CROSS_RCIRCLE:
            if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4){
                Image_borderSetULine(this, rBorder, rLine[rInfLine[0]][1]);
            }
            if((rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[0]) == 1 ||
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1) && lStraight){
                CameraStatus_addScore(PTI_RCIRCLE);
            }
            if(lInfN == 0 && rInfN == 0 || lStraight && rStraight){
                CameraStatus_addScore(NONE);
            }
            *cameraV -= 100;
            break;
    }
}

int Image_middleToErr(Image *this, int16 middle[MT9V03X_W], uint16 y, uint16 deltaY){
    int32 sum = 0;
    for(uint16 i=y; i<y+deltaY; ++i){
        sum += middle[i]-this->w/2;
    }
    return sum/deltaY;
}

void Image_processForShow(){
    Image_toRgb565Image(&image, &showImage);
    for(uint16 i=0; i<lLineL; ++i){
        Rgb565Image_set(&showImage, lLine[i][0], lLine[i][1], RGB565_YELLOW);
    }
    for(uint16 i=0; i<rLineL; ++i){
        Rgb565Image_set(&showImage, rLine[i][0], rLine[i][1], RGB565_YELLOW);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, lBorder[i], i, RGB565_GREEN);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, rBorder[i], i, RGB565_GREEN);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, mLine[i], i, i >= errY && i < errY+errDeltaY ? RGB565_RED : RGB565_BLUE);
    }
    Rgb565Image_set(&showImage, lStart[0], lStart[1], RGB565_RED);
    Rgb565Image_set(&showImage, rStart[0], rStart[1], RGB565_RED);
    for(uint16 i=0; i<lInfN; ++i){
        Rgb565Image_mark(&showImage, lLine[lInfLine[i]][0], lLine[lInfLine[i]][1], RGB565_RED, 2);
        Rgb565Image_set(&showImage, lLine[lInfLine[i]][0]+cosf(lInfRad[i])*6, lLine[lInfLine[i]][1]-sinf(lInfRad[i])*6, RGB565_RED);
    }
    for(uint16 i=0; i<rInfN; ++i){
        Rgb565Image_mark(&showImage, rLine[rInfLine[i]][0], rLine[rInfLine[i]][1], RGB565_RED, 2);;
        Rgb565Image_set(&showImage, rLine[rInfLine[i]][0]+cosf(rInfRad[i])*6, rLine[rInfLine[i]][1]-sinf(rInfRad[i])*6, RGB565_RED);
    }
}
void Image_showCamera(uint16 x, uint16 y)
{
    if(camera_process_cnt){
        if(!showPInC1){
            Image_processForShow();
            camera_process_cnt=0;
        }
        ips200_show_rgb565_image(x, y, showImage.image, showImage.w, showImage.h, showImage.w, showImage.h, 0);
        if(showPInC1){
            camera_process_cnt=0;
        }
    }
}

void Camera_pit(uint32 period, int16 speed){
    statusKeepMs += period;
    statusRunX += speed;
    for(uint32 i = 0; i < CAMERA_STATUS_NUMBER; ++i){
        statusScore[i] *= statusK;
    }
    carRunMs += period;
}

void Image_processCamera(){
    if(mt9v03x_finish_flag)              //判断一幅图像是否接收完成
    {
//        SysTimer_Start();
        Image_fromCamera(&image, mt9v03x_image);
        mt9v03x_finish_flag = 0;
        if(!showPInC1){
            if(showWait){
                while(camera_process_cnt);
            }
        }
        Image_cut(&image, 1, 0, image.w-1, image.h);
        Image_binaryzation(&image, binDeltaT);

//        Image_drawBlackFrame(&image);
        Image_drawBlackTrapezoid(&image);
        Image_getStartPoint(&image, maxStartYAdd, lStart, rStart);
        Image_bly(&image, image.h*4, lLine, rLine, lLineDir, rLineDir,
                &lLineL, &rLineL, &lrMeet, lStart, rStart);
        Image_blyToBorder(&image, 0, lLine, lLineL, lBorder);
        Image_blyToBorder(&image, 1, rLine, rLineL, rBorder);
        Image_blyToRadDir(&image, lLine, lLineL, bly2RDL, lRadDir, lRadDirLine, &lRadDirN);
        Image_blyToRadDir(&image, rLine, rLineL, bly2RDL, rRadDir, rRadDirLine, &rRadDirN);
        RadDir_toInflection(lRadDir, lRadDirLine, lRadDirN, RD2IErr, lInfLine, lInfRad, &lInfN);
        RadDir_toInflection(rRadDir, rRadDirLine, rRadDirN, RD2IErr, rInfLine, rInfRad, &rInfN);
        lStraight = Image_borderIsStraight(&image, lBorder, straightYMin, lStart[1], straightStep, straightErr, 0);
        rStraight = Image_borderIsStraight(&image, rBorder, straightYMin, rStart[1], straightStep, straightErr, 1);

        float tempCameraV = targetV;
        uint16 tempErrY = errY;
        Image_zebraCrossing(&image, &tempCameraV, &tempErrY);
        Image_cross(&image, &tempCameraV, &tempErrY);
        Image_lCircle(&image, &tempCameraV, &tempErrY);
        Image_rCircle(&image, &tempCameraV, &tempErrY);
        Image_ramp(&image, &tempCameraV, &tempErrY);
        Image_barrier(&image, &tempCameraV, &tempErrY);
        Image_bridge(&image, &tempCameraV, &tempErrY);
        Image_other(&image, &tempCameraV, &tempErrY);
        cameraV = tempCameraV;
//
        Image_borderToMiddle(&image, lBorder, rBorder, lrMeet, mLine);
////            Bend_Straight_Opinion();        //判断是否是直线
        camera_err = Image_middleToErr(&image, mLine, tempErrY, errDeltaY);
//        printf("%d\r\n",statusRunX);
//        printf("%d\r\n",cameraStatus);
//        SysTimer_Stop();
//        printf("%d\r\n",GetPastTime());
        if(showPInC1){
            if(showWait){
                while(camera_process_cnt);
            }
            Image_processForShow();
        }
        Fps_add(1);
        camera_process_cnt=1;
    }
}

#pragma section all restore
