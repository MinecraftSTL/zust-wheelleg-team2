/*
 * Gyro.h
 *
 *  Created on: 2024Äê12ÔÂ24ÈÕ
 *      Author: minec
 */

#ifndef CODE_GYRO_GYRO_H_
#define CODE_GYRO_GYRO_H_

extern float aAx, aAy, aAz;
extern float vAx, vAy, vAz;
extern float vAx_, vAy_, vAz_;
extern float xAx, xAy, xAz;
extern float aXx, aXy, aXz;
extern float downAy;

void gyro_init(uint16 pitMs);
void gyro_get_acc();
void gyro_get_gyro();
void gyro_get();

void downAy_autoSet();

#endif /* CODE_GYRO_GYRO_H_ */
