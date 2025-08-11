/*
 * Gyro.h
 *
 *  Created on: 2024��12��24��
 *      Author: minec
 */

#ifndef CODE_GYRO_GYRO_H_
#define CODE_GYRO_GYRO_H_

extern float aAx, aAy, aAz;
extern float vAx, vAy, vAz;
extern float xAx, xAy, xAz;
extern float aXx, aXy, aXz;
extern float downAy;

extern float kZero;

void gyro_init(uint16 pitMs);
void gyro_get_acc();
void gyro_get_gyro();
void gyro_get();

void downAy_reset();

#endif /* CODE_GYRO_GYRO_H_ */
