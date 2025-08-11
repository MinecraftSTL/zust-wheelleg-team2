#ifndef PID_H
#define PID_H

typedef struct {
    float Kp, Ki, Kd;
    float Ek_, Ek_sum;
    float Max_I, Max_Out;
    float OUT;
} PID;

/*****************************************************函数部分**************************************************************/
void PID_Init(PID *this, float kp, float ki, float kd, float max_I, float max_Out);
float pid(PID *PID, float TargetValue, float ActualValue);
void PID_clear(PID *this);

#endif
