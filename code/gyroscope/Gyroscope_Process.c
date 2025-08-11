/*
 * process.c
 *
 *  Created on: 2023��12��13��
 *      Author: ¬����
 */
#include <gyroscope/Gyroscope_Process.h>

#define Kp 10.0f // �����KpKi�����ڵ������ٶȼ����������ǵ��ٶ�

#define Ki 0.0033f

float pitch = 0, roll = 0, yaw = 0;//0����ǰ�Ƕ�   1����һʱ�̽Ƕ�

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��ʼ��̬��Ԫ��������ƪ�����ᵽ�ı任��Ԫ����ʽ����

float exInt = 0, eyInt = 0, ezInt = 0; // ��ǰ�ӼƲ�õ��������ٶ��������ϵķ���

float I_ex, I_ey, I_ez; // ������
// ���õ�ǰ��̬��������������������ϵķ��������Ļ���
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float halfT)
{
    float q0temp, q1temp, q2temp, q3temp;
    float normr, vx, vy, vz, ex, ey, ez;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q1q1 = q1 * q1;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    if (ax * ay * az != 0)
    {
        // ������ٶ�ģ��
        float a_norm = sqrt(ax * ax + ay * ay + az * az);
        float g = 9.80665f;
        float sigma = 0.05f; // �����趨��������׼��
        float chi_square = ((a_norm - g) * (a_norm - g)) / (sigma * sigma);

        // ��̬����Ȩ��
        float alpha = 0.3f / (0.3f + sqrt(chi_square));
        float dynamic_Kp = Kp * alpha;

        // ��λ�����ٶ�
        normr = Q_rsqrt(ax * ax + ay * ay + az * az);
        ax *= normr;
        ay *= normr;
        az *= normr;

        // ������������
        vx = 2.0f * (q1q3 - q0q2);
        vy = 2.0f * (q0q1 + q2q3);
        vz = q0q0 - q1q1 - q2q2 + q3q3;

        // �������
        ex = (ay * vz - az * vy);
        ey = (az * vx - ax * vz);
        ez = (ax * vy - ay * vx);

        // ������
        I_ex += halfT * ex;
        I_ey += halfT * ey;
        I_ez += halfT * ez;
        exInt = Ki * I_ex;
        eyInt = Ki * I_ey;
        ezInt = Ki * I_ez;

        // PI����
        gx = gx + dynamic_Kp * ex + exInt;
        gy = gy + dynamic_Kp * ey + eyInt;
        gz = gz + dynamic_Kp * ez + ezInt;

        // ��Ԫ������
        q0temp = q0;
        q1temp = q1;
        q2temp = q2;
        q3temp = q3;

        q0 = q0 + (-q1temp * gx - q2temp * gy - q3temp * gz) * halfT;
        q1 = q1 + (q0temp * gx + q2temp * gz - q3temp * gy) * halfT;
        q2 = q2 + (q0temp * gy - q1temp * gz + q3temp * gx) * halfT;
        q3 = q3 + (q0temp * gz + q1temp * gy - q2temp * gx) * halfT;

        // ��λ��
        normr = Q_rsqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 *= normr;
        q1 *= normr;
        q2 *= normr;
        q3 *= normr;

        // ŷ����ת��
        pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.259f;
        roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.259f;
        yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.259f;
    }
}



float LPF2_T2(float xin)
{
   static float lpf2_yout[3] = {0};
   static float lpf2_xin[3] = {0};

   float sample_freq = 1000;
   float cutoff_freq = 1;
//   float fr = sample_freq / cutoff_freq;

   float ohm = tan(PI * cutoff_freq /sample_freq);
   float c = 1 + 1.414 * ohm  + ohm * ohm;

   float b0 = ohm * ohm /c;
   float b1 = 2.0f * b0;
   float b2 = b0;

   float a1 = 2.0f * (ohm * ohm - 1.0f) /c;
   float a2 = (1.0f - 1.414 * ohm + ohm * ohm) / c;

   lpf2_xin[2] = xin;

   lpf2_yout[2] = b0 * lpf2_xin[2] + b1 * lpf2_xin[1] + b2 * lpf2_xin[0] - a1 * lpf2_yout[1] - a2 * lpf2_yout[0];

   lpf2_xin[0] = lpf2_xin[1];
   lpf2_xin[1] = lpf2_xin[2];
   lpf2_yout[0] = lpf2_yout[1];
   lpf2_yout[1] = lpf2_yout[2];

   return lpf2_yout[2];
}
