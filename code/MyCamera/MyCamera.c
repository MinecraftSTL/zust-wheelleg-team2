#include "MyCamera.h"

#pragma section all "cpu1_dsram"

Image image;
Image image1;
Image inverseImage;
Rgb565Image showImage;

int binDeltaT = 0;
int inverseY = 50;
float inverseX = 0.004;
int bly2RDL = 3;
float RD2IErr = 0.7854;
float facingErr = 0.5236;
int setLineY = 15;
int StraightYMin = 40;
int StraightStep = 15;
float StraightErr = 0.349;
int zebraY = 70;
int crossY = 20;
int crossX = 15;
int circleY = 5;
int circleX = 5;
int errY = 60;
int errDeltaY = 30;
uint8 showPInC1 = 1;
uint8 showWait = 0;

Inverse inverse;

uint16 lStartPoint[2];
uint16 rStartPoint[2];

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
int16 lRadDirPos[MAX_BLY][2];
uint16 lRadDirN;
float rRadDir[MAX_BLY];
int16 rRadDirPos[MAX_BLY][2];
uint16 rRadDirN;

int16 lInflection[MAX_BLY][2];
float lInflectionDir[MAX_BLY];
uint16 lInflectionN;
int16 rInflection[MAX_BLY][2];
float rInflectionDir[MAX_BLY];
uint16 rInflectionN;

CameraStatus cameraStatus = NONE;

uint8 camera_process_cnt = 0;

int camera_err = 0;              //摄像头中线偏差

#pragma section all restore
#pragma section all "cpu1_psram"

void MyCamera_Init(void)
{
    ips200_show_string(0, 0, "mt9v03x init.");
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

uint8 inline Image_get(Image *this, uint16 y, uint16 x){
    if(y >= this->h || x >= this->w){
        return 0;
    }
    return this->image[x + this->w*y];
}
void inline Image_set(Image *this, uint16 y, uint16 x, uint8 value){
    if(y >= this->h || x >= this->w){
        return;
    }
    this->image[x + this->w*y] = value;
}
void Image_fromCamera(Image *this, uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W])
{
    this->h = MT9V03X_H;
    this->w = MT9V03X_W;
    memcpy(this->image, mt9v03x_image, MT9V03X_IMAGE_SIZE);
}
void Image_clear(Image *this){
    for(int i=0; i<this->h*this->w; ++i){
        this->image[i]=0x00;
    }
}
void Image_clone(Image *this, Image *target)
{
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    memcpy(target->image, this->image, this->h*this->w);
}
void Image_cut(Image *this, Image *target, uint16 y0, uint16 x0, uint16 y1, uint16 x1){
    zf_assert(this && target);
    target->h = y1-y0;
    target->w = x1-x0;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i+y0,j+x0));
        }
    }
}
void Image_zoom(Image *this, Image *target, float zoom){
    zf_assert(this && target && zoom > 0.0f);
    target->h = (this->h-1)*zoom+1;
    target->w = (this->w-1)*zoom+1;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i/zoom,j/zoom));
        }
    }
}
void Image_toRgb565Image(Image *this, Rgb565Image *target){
    target->h = this->h;
    target->w = this->w;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = this->image[i]>>(8-5)<<(5+6)|this->image[i]>>(8-6)<<5|this->image[i]>>(8-5);
    }
}
void Rgb565Image_clone(Rgb565Image *this, Rgb565Image *target)
{
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    memcpy(target->image, this->image, this->h*this->w*2);
}
void Rgb565Image_mode(Rgb565Image *this, Rgb565Image *target){
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = (this->image[i] << 8) | (this->image[i] >> 8);
    }
}
uint16 inline Rgb565Image_get(Rgb565Image *this, uint16 y, uint16 x){
    if(y >= this->h || x >= this->w){
        return 0;
    }
    return this->image[x + this->w*y];
}
void inline Rgb565Image_set(Rgb565Image *this, uint16 y, uint16 x, uint16 value){
    if(y >= this->h || x >= this->w){
        return;
    }
    this->image[x + this->w*y] = value;
}
void Rgb565Image_mark(Rgb565Image *this, uint16 y, uint16 x, uint16 color, uint16 r){
    for(int i = -r; i <= r; ++i){
        Rgb565Image_set(this, y-i, x-i, color);
        Rgb565Image_set(this, y+i, x-i, color);
    }
}

void Inverse_init(Inverse *this, uint16 y ,float x){
    this->y = y;
    this->x = x;
}
void Inverse_imagePosToPos(Inverse *this, Image *image, int16 y0, int16 x0, int16 *y1, int16 *x1){
    *y1 = y0;
    *x1 = (x0-image->w/2)*(1-this->x*(y0-this->y))+image->w/2;
}
void Inverse_imagePosFromPos(Inverse *this, Image *image, int16 y0, int16 x0, int16 *y1, int16 *x1){
    *y1 = y0;
    *x1 = (x0-image->w/2)/(1-this->x*(y0-this->y))+image->w/2;
}
void Inverse_image(Inverse *this, Image *image, Image *target){
    zf_assert(this && target);
    target->h = image->h;
    target->w = image->w;
    for(uint16 i=0; i<image->h; ++i){
        for(uint16 j=0; j<image->w; ++j){
            int16 y,x;
            Inverse_imagePosFromPos(this, image, i, j, &y, &x);
            Image_set(target, i, j, Image_get(image, y, x));
        }
    }
}
void Inverse_bly(Inverse *this, Image *image, int16 bly[MAX_BLY][2], uint16 blyL){
    for(uint16 i=0; i<blyL; ++i){
        int16 y,x;
        Inverse_imagePosToPos(this, image, bly[i][0], bly[i][1], &y, &x);
        bly[i][0] = y;
        bly[i][1] = x;
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
        if (isFirstBinary)               // 第一次二值化
        {
            goto GRAY_VARIANCE;
        } else    // 不是第一次二值化，本次阈值在上次阈值的附近（这样能快几十us）
        {
            if (i < threshold - 5 && maxGrayValue > threshold)    // 和上次阈值的差距 > 某个值，不计算类间方差。
            {
                continue;
            }
            if (i > threshold + 5 && minGrayValue < threshold) {
                return (uint8) threshold;    // 最大类间方差对应的灰度值，认为是最佳分割阈值
            }
        }
    GRAY_VARIANCE:                                                     // 计算类间方差
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
void Image_getStartPoint(Image *this, uint16 lStartPoint[2], uint16 rStartPoint[2]){
    uint16 y = this->h-2;
    uint16 x = this->w/2;

    lStartPoint[0]=y;
    for(lStartPoint[1] = x; lStartPoint[1] >= 2; --lStartPoint[1]){
        if(!Image_get(this, y, lStartPoint[1]) && Image_get(this, y, lStartPoint[1]+1) &&
                !Image_get(this, y, lStartPoint[1]-1) && Image_get(this, y, lStartPoint[1]+2)){
            break;
        }
    }
    rStartPoint[0]=y;
    for(rStartPoint[1] = x-1; rStartPoint[1] < this->w-2; ++rStartPoint[1]){
        if(!Image_get(this, y, rStartPoint[1]) && Image_get(this, y, rStartPoint[1]-1) &&
                !Image_get(this, y, rStartPoint[1]+1) && Image_get(this, y, rStartPoint[1]-2)){
            break;
        }
    }
}
void Image_drawRectan(Image *this)
{

    uint16 i = 0;
    for (i = 0; i < this->h; i++)     //给左边0、1列和右边186、187列画黑框
    {
        Image_set(this,i,0,0);
        Image_set(this,i,this->w - 1,0);

    }
    for (i = 0; i < this->w; i++)     //给上方0、1行画黑框
    {
        Image_set(this,0,i,0);
        Image_set(this,this->h - 1,i,0);
    }
}
const int8 Bly_SEED[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
void Image_bly(Image *this, uint16 maxL, int16 lLine[MAX_BLY][2], int16 rLine[MAX_BLY][2],
        uint8 lDir[MAX_BLY], uint8 rDir[MAX_BLY], uint16 *lLineL, uint16 *rLineL, int16 *meet,
        uint16 lStart[2], uint16 rStart[2])
{
    uint8 lStop = 0, rStop = 0;

    lLine[0][0] = lStart[0];
    lLine[0][1] = lStart[1];
    rLine[0][0] = rStart[0];
    rLine[0][1] = rStart[1];
    lDir[0] = rDir[0] = 2;

    *lLineL = *rLineL = 1;
    while(maxL--){
        int16 r_sub_l = rLine[*rLineL-1][0] - lLine[*lLineL-1][0];
        if(!lStop && r_sub_l<=0){
            uint8 lStep = 0;
            for(uint8 i=0; i<8; ++i){
                lDir[*lLineL] = (lDir[*lLineL-1]+6+i)&0x07;
                lLine[*lLineL][0] = lLine[*lLineL-1][0] + Bly_SEED[lDir[*lLineL]][0];
                lLine[*lLineL][1] = lLine[*lLineL-1][1] + Bly_SEED[lDir[*lLineL]][1];
                uint8 dir = (lDir[*lLineL]+1)&0x07;
                int16 dot[2];
                dot[0] = lLine[*lLineL-1][0] + Bly_SEED[dir][0];
                dot[1] = lLine[*lLineL-1][1] + Bly_SEED[dir][1];
                if(lLine[*lLineL][0] < 0 || lLine[*lLineL][1] < 0 || lLine[*lLineL][0] >= this->h || lLine[*lLineL][1] >= this->w ||
                        dot[0] < 0 || dot[1] < 0 || dot[0] >= this->h || dot[1] >= this->w){
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
        if(!rStop && r_sub_l>=0){
            uint8 rStep = 0;
            for(uint8 i=0; i<8; ++i){
                rDir[*rLineL] = (rDir[*rLineL-1]+2+(8-i))&0x07;
                rLine[*rLineL][0] = rLine[*rLineL-1][0] + Bly_SEED[rDir[*rLineL]][0];
                rLine[*rLineL][1] = rLine[*rLineL-1][1] + Bly_SEED[rDir[*rLineL]][1];
                uint8 dir = (rDir[*rLineL]+7)&0x07;
                int16 dot[2];
                dot[0] = rLine[*rLineL-1][0] + Bly_SEED[dir][0];
                dot[1] = rLine[*rLineL-1][1] + Bly_SEED[dir][1];
                if(rLine[*rLineL][0] < 0 || rLine[*rLineL][1] < 0 || rLine[*rLineL][0] >= this->h || rLine[*rLineL][1] >= this->w ||
                        dot[0] < 0 || dot[1] < 0 || dot[0] >= this->h || dot[1] >= this->w){
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
        if (lStop && rStop){
            break;
        }
        if (abs(rLine[*rLineL-1][0] - lLine[*lLineL-1][0]) < 2
            && abs(rLine[*rLineL-1][1] - lLine[*lLineL-1][1]) < 2){
            *meet = (rLine[*rLineL-1][0] + lLine[*lLineL-1][0])>>1;
            break;
        }
    }
}
void Image_blyToBorder(Image *this, uint8 dir, int16 line[MAX_BLY][2], uint16 lineL, uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = dir?(this->w-1):0;
    }
    for(uint16 i=0; i<lineL; ++i){
        if(dir?(line[i][1] < ret[line[i][0]]):(line[i][1] > ret[line[i][0]])){
            ret[line[i][0]]=line[i][1];
        }
    }
}

uint8 inline Image_borderXIsLose(Image *this, uint16 y, uint16 x, uint8 dir){
    return dir?x >= this->w-1:x < 1;
}
uint8 inline Image_borderIsLose(Image *this, uint16 border[MT9V03X_H], uint16 y, uint8 dir){
    return Image_borderXIsLose(this, y, border[y], dir);
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
uint8 inline Image_inverseBorderXIsLose(Image *this, Inverse *inverse, uint16 y, uint16 x, uint8 dir){
    if(Image_borderXIsLose(this, y, x, dir)){
        return 1;
    }
    int16 iy, ix;
    Inverse_imagePosFromPos(inverse, this, y, x, &iy, &ix);
    if(Image_borderXIsLose(this, iy, ix, dir)){
        return 1;
    }
    return 0;
}
uint8 inline Image_inverseBorderIsLose(Image *this, Inverse *inverse, uint16 border[MT9V03X_H], uint16 y, uint8 dir){
    return Image_inverseBorderXIsLose(this, inverse, y, border[y], dir);
}
uint8 inline Image_inverseBorderIsAllLose(Image *this, Inverse *inverse, uint16 border[MT9V03X_H], uint16 y0, uint16 y1, uint8 dir){
    for(uint16 i=y0; i<=y1; ++i){
        if(!Image_inverseBorderIsLose(this, inverse, border, i, dir)){
            return 0;
        }
    }
    return 1;
}
uint8 inline Image_inverseBorderIsNoneLose(Image *this, Inverse *inverse, uint16 border[MT9V03X_H], uint16 y0, uint16 y1, uint8 dir){
    for(uint16 i=y0; i<=y1; ++i){
        if(Image_inverseBorderIsLose(this, inverse, border, i, dir)){
            return 0;
        }
    }
    return 1;
}

void Image_borderToMiddle(Image *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
}
void Image_borderSetCLine(Image *this, uint16 border[MT9V03X_H], uint16 y0, uint16 x0, uint16 y1, uint16 x1){
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
        border[i] = (uint16)(slope*(i-y0)+x0);
    }
}

void leastSquares(uint16 pos[MAX_BLY][2], uint16 n, float *slope, float *intercept) {
    // 初始化无效值
    *slope = NAN;
    *intercept = 0;

    // 检查有效点数
    if (n < 2) {
        return;
    }

    float sum_x = 0.f, sum_y = 0.f, sum_xy = 0.f, sum_x2 = 0.f;

    // 累加计算
    for (uint16 i = 0; i < n; ++i) {
        float x = pos[i][0];
        float y = pos[i][1];
        sum_y += y;
        sum_x += x;
        sum_xy += x * y;
        sum_x2 += x * x;
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

void Image_borderSetULine(Image *this, uint16 border[MT9V03X_H], uint16 y){
    if(y+setLineY >= this->h-1){
        return;
    }
    uint16 pos[MAX_BLY][2], n;
    for(n = 0; n < setLineY; ++n){
        pos[n][0] = y+n;
        pos[n][1] = border[y+n];
    }
    float slope, intercept;
    leastSquares(pos, n, &slope, &intercept);
    for(uint16 i = y; i >= 0; --i){
        float x = slope*i+intercept;
        border[i] = (uint16)func_limit_ab(x, 0, this->w-1);
        if (i == 0) break; // 防止uint16下溢
    }
}
void Image_borderSetDLine(Image *this, uint16 border[MT9V03X_H], uint16 y){
    if(y < setLineY){
        return;
    }
    uint16 pos[MAX_BLY][2], n;
    for(n = 0; n < setLineY; ++n){
        pos[n][0] = y-n;
        pos[n][1] = border[y-n];
    }
    float slope, intercept;
    leastSquares(pos, n, &slope, &intercept);
    for(uint16 i = y; i < this->h; ++i){
        float x = slope*i+intercept;
        border[i] = (uint16)func_limit_ab(x, 0, this->w-1);
    }
}
uint8 Image_borderIsStraight(Image *this, uint16 border[MT9V03X_H], uint8 dir){
    if(!Image_borderIsNoneLose(this, border, 1, this->h-2, dir)){
        return 0;
    }
    float all = atan2f(border[this->h-2]-border[1],this->h-3);
    for(uint16 i=StraightYMin; i+StraightStep<this->h-1; i+=StraightStep){
        if(fabsf(all-atan2f(border[i+StraightStep]-border[i],StraightStep)) > StraightErr){
            return 0;
        }
    }
    return 1;
}
void Image_blyCutByBorder(Image *this, uint16 border[MT9V03X_H], uint8 dir, int16 line[MAX_BLY][2], uint16 *lineL){
    uint16 yMin;
    for(yMin = 0; yMin<this->h; ++yMin){
        if(!Image_borderIsLose(this, border, yMin, dir)){
            break;
        }
    }
    for(uint16 i=0; i<*lineL; ++i){
        if(line[i][0] == yMin && line[i][1] == border[yMin]){
            *lineL = i+1;
            break;
        }
    }
}

void Image_blyToRadDir(Image *this, int16 bly[MAX_BLY][2], uint16 blyL, uint16 l,
        float ret[MAX_BLY], int16 retPos[MAX_BLY][2], uint16 *retN){
    *retN = (blyL-1)/l;
    for(uint16 i=0; i<*retN; ++i){
        ret[i]=atan2f(-(bly[(i+1)*l][0]-bly[i*l][0]), bly[(i+1)*l][1]-bly[i*l][1]);
        for(uint16 j=i*l; j<=(i+1)*l; ++j){
            if(bly[j][0] == 0 || bly[j][0] == this->h-1 || Image_inverseBorderXIsLose(this, &inverse, bly[j][0], bly[j][1], 0) || Image_inverseBorderXIsLose(this, &inverse, bly[j][0], bly[j][1], 1)){
                ret[i] = NAN;
                break;
            }
        }
        retPos[i][0] = (bly[i*l][0]+bly[(i+1)*l][0])>>1;
        retPos[i][1] = (bly[i*l][1]+bly[(i+1)*l][1])>>1;
    }
}
void RadDir_toInflection(float this[MAX_BLY], int16 thisPos[MAX_BLY][2], uint16 thisN, float err,
        int16 ret[MAX_BLY][2], float retRad[MAX_BLY], uint16 *retN){
    *retN = 0;
    for(uint16 i = 1; i < thisN-1; ++i){
        if(isnan(this[i-1]) || isnan(this[i+1])){
            continue;
        }
        if(PI/2-NormalizeAngle_toPi2(Angle_normalize(this[i+1]-this[i-1])) <= err){
            ret[*retN][0] = thisPos[i][0];
            ret[*retN][1] = thisPos[i][1];
//            retRad[*retN] = atan2f(sinf(this[i+1])-sinf(this[i-1]),cosf(this[i+1])-cosf(this[i-1]));
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
int16 Inflection_getFirstFacing(int16 this[MAX_BLY][2], float thisRad[MAX_BLY], uint16 thisN, uint8 facing){
    for(uint16 i=0; i<thisN; ++i){
        if(Inflection_getFacing(thisRad[i])==facing){
            return i;
        }
    }
    return -1;
}

void Image_zebraCrossing(Image *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 y){
    int edge_left_num = 0;
    int edge_right_num = 0;
    int edge_sum = 0;
    for(uint16 zebra_row = y; zebra_row < y+3; zebra_row++)
    {
        for(uint16 i=lBorder[zebra_row];i<this->w/2;i++)
        {
            if(Image_get(this, zebra_row, i) && Image_get(this, zebra_row, i+1) &&
                    !Image_get(this, zebra_row, i+2) && !Image_get(this, zebra_row,i+3))
            {
                edge_left_num++;
            }
        }
        for(uint16 i=rBorder[zebra_row];i>this->w/2;i--)
        {
            if(Image_get(this, zebra_row, i) && Image_get(this, zebra_row, i-1) &&
                    !Image_get(this, zebra_row, i-2) && !Image_get(this, zebra_row,i-3))
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
    if(edge_sum >= 16 && edge_left_num > 5 && edge_right_num > 5)                      //停车
    {
        beepMid();
        cameraStatus = IN_ZEBRA;
    }
    switch(cameraStatus){
        case IN_ZEBRA:
            beepMid();
            cameraStatus = OUT_ZEBRA;
            Car_stop();
            break;
    }
}
void Image_cross(Image *this, int16 lInf[MAX_BLY][2], float lInfRad[MAX_BLY], uint16 lInfN,
        int16 rInf[MAX_BLY][2], float rInfRad[MAX_BLY], uint16 rInfN,
        uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H]){
    switch(cameraStatus){
        case NONE:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3 &&
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4 &&
                    Image_borderIsLose(this, lBorder, lInf[0][0]-crossX, 0) &&
                    Image_borderIsLose(this, lBorder, lInf[0][0]-crossX-1, 0) &&
                    Image_borderIsLose(this, rBorder, rInf[0][0]-crossX, 1) &&
                    Image_borderIsLose(this, rBorder, rInf[0][0]-crossX-1, 1)
                    )
            {
                beepMid();
                cameraStatus = IN_CROSS;
            }
            break;
        case IN_CROSS:
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 3 &&
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 4){
                Image_borderSetULine(this, lBorder, lInf[0][0]);
                Image_borderSetULine(this, rBorder, rInf[0][0]);
            }
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2 &&
                    rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1 &&
                    lInf[1][0]+crossX+1 < this->h && rInf[1][0]+crossX+1 < this->h){
                beepMid();
                cameraStatus = CROSS;
            }else if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 ||
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                beepMid();
                cameraStatus = OUT_CROSS;
            }
            break;
        case CROSS:
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2){
                Image_borderSetCLine(this, lBorder, lInf[0][0], lInf[0][1], lInf[1][0], lInf[1][1]);
            }
            if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1){
                Image_borderSetCLine(this, rBorder, rInf[0][0], rInf[0][1], rInf[1][0], rInf[1][1]);
            }
            if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2 ||
                    rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                beepMid();
                cameraStatus = OUT_CROSS;
            }
            break;
        case OUT_CROSS:
            if(lInfN > 1 && Inflection_getFacing(lInfRad[0]) == 3 && Inflection_getFacing(lInfRad[1]) == 2){
                Image_borderSetCLine(this, lBorder, lInf[0][0], lInf[0][1], lInf[1][0], lInf[1][1]);
            }else if(lInfN > 0 && Inflection_getFacing(lInfRad[0]) == 2){
                Image_borderSetDLine(this, lBorder, lInf[0][0]);
            }
            if(rInfN > 1 && Inflection_getFacing(rInfRad[0]) == 4 && Inflection_getFacing(rInfRad[1]) == 1){
                Image_borderSetCLine(this, rBorder, rInf[0][0], rInf[0][1], rInf[1][0], rInf[1][1]);
            }else if(rInfN > 0 && Inflection_getFacing(rInfRad[0]) == 1){
                Image_borderSetDLine(this, rBorder, rInf[0][0]);
            }
            if(!(lInfN > 0 && (Inflection_getFacing(lInfRad[0]) == 2 || Inflection_getFacing(lInfRad[0]) == 3) ||
                    rInfN > 0 && (Inflection_getFacing(rInfRad[0]) == 1 || Inflection_getFacing(rInfRad[0]) == 4))){
                beepMid();
                cameraStatus = NONE;
            }
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
    Inverse_image(&inverse, &image, &inverseImage);
    Image_toRgb565Image(&inverseImage, &showImage);
    for(uint16 i=0; i<lLineL; ++i){
        Rgb565Image_set(&showImage, lLine[i][0], lLine[i][1], RGB565_YELLOW);
    }
    for(uint16 i=0; i<rLineL; ++i){
        Rgb565Image_set(&showImage, rLine[i][0], rLine[i][1], RGB565_YELLOW);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, i, lBorder[i], RGB565_GREEN);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, i, rBorder[i], RGB565_GREEN);
    }
    for(uint16 i=0; i<showImage.h; ++i){
        Rgb565Image_set(&showImage, i, mLine[i], i >= errY && i < errY+errDeltaY ? RGB565_RED : RGB565_BLUE);
    }
    int16 y, x;
    Inverse_imagePosToPos(&inverse, &image, lStartPoint[0], lStartPoint[1], &y, &x);
    Rgb565Image_set(&showImage, y, x, RGB565_RED);
    Inverse_imagePosToPos(&inverse, &image, rStartPoint[0], rStartPoint[1], &y, &x);
    Rgb565Image_set(&showImage, y, x, RGB565_RED);
    for(uint16 i=0; i<lInflectionN; ++i){
        Rgb565Image_mark(&showImage, lInflection[i][0], lInflection[i][1], RGB565_RED, 2);
        Rgb565Image_set(&showImage, lInflection[i][0]-sinf(lInflectionDir[i])*6, lInflection[i][1]+cosf(lInflectionDir[i])*6, RGB565_RED);
    }
    for(uint16 i=0; i<rInflectionN; ++i){
        Rgb565Image_mark(&showImage, rInflection[i][0], rInflection[i][1], RGB565_RED, 2);;
        Rgb565Image_set(&showImage, rInflection[i][0]-sinf(rInflectionDir[i])*6, rInflection[i][1]+cosf(rInflectionDir[i])*6, RGB565_RED);
    }
}
void MyCamera_Show(uint16 x, uint16 y)
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
        Image_cut(&image, &image1, 0, 1, image.h, image.w-1);
//        Image_cut(&image1, &image, 10, 30, image1.h-20, image1.w-30);
        Image_binaryzation(&image, binDeltaT);
        Image_getStartPoint(&image, lStartPoint, rStartPoint);
        Image_drawRectan(&image);
        Inverse_init(&inverse, inverseY, inverseX);
        Image_bly(&image, image.h*3, lLine, rLine, lLineDir, rLineDir,
                &lLineL, &rLineL, &lrMeet, lStartPoint, rStartPoint);
        Inverse_bly(&inverse, &image, lLine, lLineL);
        Inverse_bly(&inverse, &image, rLine, rLineL);
        Image_blyToBorder(&image, 0, lLine, lLineL, lBorder);
        Image_blyToBorder(&image, 1, rLine, rLineL, rBorder);
        Image_blyCutByBorder(&image, lBorder, 0, lLine, &lLineL);
        Image_blyCutByBorder(&image, rBorder, 1, rLine, &rLineL);
        Image_blyToRadDir(&image, lLine, lLineL, bly2RDL, lRadDir, lRadDirPos, &lRadDirN);
        Image_blyToRadDir(&image, rLine, rLineL, bly2RDL, rRadDir, rRadDirPos, &rRadDirN);
        RadDir_toInflection(lRadDir, lRadDirPos, lRadDirN, RD2IErr, lInflection, lInflectionDir, &lInflectionN);
        RadDir_toInflection(rRadDir, rRadDirPos, rRadDirN, RD2IErr, rInflection, rInflectionDir, &rInflectionN);
        lStraight = Image_borderIsStraight(&image, lBorder, 0);
        rStraight = Image_borderIsStraight(&image, rBorder, 1);
        Image_zebraCrossing(&image, lBorder, rBorder, zebraY);
        Image_cross(&image, lInflection, lInflectionDir, lInflectionN, rInflection, rInflectionDir, rInflectionN, lBorder, rBorder);
//        SysTimer_Stop();
//        printf("%d\r\n",GetPastTime());
//
//            Left_Straight();              //左直线判断
//            Right_Straight();             //右直线判断
//
        Image_borderToMiddle(&image, lBorder, rBorder, mLine);
//            Middle_Empty();
////            Bend_Straight_Opinion();        //判断是否是直线
        camera_err = Image_middleToErr(&image, mLine, errY, errDeltaY);
////            printf("%d\r\n",g_camera_mid_err);
////            printf("%d ,%d ,%d\r\n ",target_left,target_right,g_camera_mid_err);
//            camera_process_cnt++;
//            camera_process_cnt_show++;
//        }
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
