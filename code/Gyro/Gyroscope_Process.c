/*
 * process.c
 *
 *  Created on: 2023��12��13��
 *      Author: ¬����
 */
#include "Gyroscope_Process.h"

#include "Sys.h"

#define Kp 10.0f // �����KpKi�����ڵ������ٶȼ����������ǵ��ٶ�

#define Ki 0.004f

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��ʼ��̬��Ԫ��������ƪ�����ᵽ�ı任��Ԫ����ʽ����

float exInt = 0, eyInt = 0, ezInt = 0; // ��ǰ�ӼƲ�õ��������ٶ��������ϵķ���

// ���õ�ǰ��̬��������������������ϵķ��������Ļ���
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float halfT) // g�������ǣ�a��Ӽ�

{
        float q0temp, q1temp, q2temp, q3temp; // ��Ԫ���ݴ���������΢�ַ���ʱҪ��

        float normr; // ʸ����ģ����Ԫ���ķ���

        float vx, vy, vz; // ��ǰ��̬��������������������ϵķ���

        float ex, ey, ez; // ��ǰ�ӼƲ�õ��������ٶ��������ϵķ���

        // ���õ�ǰ��̬��������������������ϵķ��������

        // �Ȱ���Щ�õõ���ֵ���

        float q0q0 = q0 * q0;

        float q0q1 = q0 * q1;

        float q0q2 = q0 * q2;

        float q1q1 = q1 * q1;

        float q1q3 = q1 * q3;

        float q2q2 = q2 * q2;

        float q2q3 = q2 * q3;

        float q3q3 = q3 * q3;

        if (ax * ay * az != 0) // �Ӽƴ�����������״̬ʱ��������̬���㣬��Ϊ�������ĸ���������
        {

                normr = Q_rsqrt(ax * ax + ay * ay + az * az); // ��λ�����ٶȼƣ�

                ax = ax * normr; // �������������Ҳ����Ҫ�޸�KP��������Ϊ�����һ����

                ay = ay * normr;

                az = az * normr;

                // �õ�ǰ��̬������������������ϵķ�����

                // �ο�����nϵת������������bϵ������Ԫ����ʾ�ķ������Ҿ�������м��ǣ�����һ�����ᵽ��

                vx = 2 * (q1q3 - q0q2);

                vy = 2 * (q0q1 + q2q3);

                vz = q0q0 - q1q1 - q2q2 + q3q3;

                // �����õ������������������������������Ա�ʾ��һ���

                // ԭ�����������Ϊ���������ǵ�λ������sin0����0

                // ����Ҫ�Ǽн���180����~�����û���

                ex = (ay * vz - az * vy);

                ey = (az * vx - ax * vz);

                ez = (ax * vy - ay * vx);

                exInt = exInt + ex * Ki; // �������л���

                eyInt = eyInt + ey * Ki;

                ezInt = ezInt + ez * Ki;

                // adjusted gyroscope measurements

                gx = gx + Kp * ex + exInt; // �����PI�󲹳��������ǣ����������Ư��

                gy = gy + Kp * ey + eyInt;

                gz = gz + Kp * ez + ezInt; // �����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

                // ���������̬�ĸ��£�Ҳ������Ԫ��΢�ַ��̵����

                q0temp = q0; // �ݴ浱ǰֵ���ڼ���

                q1temp = q1; // ���ϴ�������㷨���û��ע��������⣬�ڴ˸���

                q2temp = q2;

                q3temp = q3;

                // ����һ�ױϿ��ⷨ�����֪ʶ�ɲμ���������������Ե���ϵͳ��P212

                q0 = q0temp + (-q1temp * gx - q2temp * gy - q3temp * gz) * halfT;

                q1 = q1temp + (q0temp * gx + q2temp * gz - q3temp * gy) * halfT;

                q2 = q2temp + (q0temp * gy - q1temp * gz + q3temp * gx) * halfT;

                q3 = q3temp + (q0temp * gz + q1temp * gy - q2temp * gx) * halfT;

                // ��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕȣ����������Դ�����������任

                normr = Q_rsqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);

                q0 = q0 * normr;

                q1 = q1 * normr;

                q2 = q2 * normr;

                q3 = q3 * normr;

                // ��Ԫ����ŷ���ǵ�ת������ʽ�Ƶ�������һ

                // ����YAW��������ڼ��ٶȼƶ���û���������ã���˴˴�ֱ���������ǻ��ִ���

                xAy = asinf(-2 * q1 * q3 + 2 * q0 * q2); // xAy

                xAx = atan2f(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1); // roll

                xAz = -atan2f(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1);

        }
}
