/*
 * IMU.h
 *
 *  Created on: 2024Äê10ÔÂ23ÈÕ
 *      Author: 26742
 */

#ifndef CODE_IMU_H_
#define CODE_IMU_H_

#include "zf_common_headfile.h"

extern float angle_speed;                     //???
extern float yaw;                             //??
extern float Zgyro;
extern float Zgyro_speed;
// void Yaw_Zero_Floating(void);
void Angle_Speed_Get(void);
void Yaw_Get(void);
void Zgyro_Get(void);


#endif /* CODE_IMU_H_ */
