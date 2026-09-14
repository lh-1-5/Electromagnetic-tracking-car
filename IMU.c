/*
 * IMU.c
 *
 *  Created on: 2024年10月23日
 *      Author: 26742
 */

#include "IMU.h"

/**************************IMU963变量定义**************************/
// float Zgyro_Zero = 0.0f;                            			//零漂数值
// float gyro_flag = 0.0f;
// float Zgyro = 0.0f;                //去零漂后的值， 上次角度， 角速度, 上次角速度

float angle_speed = 0.0f;
float angleSpeed_last = 0.0f;
float yaw = 0.0f;                                         // 航向角， 防止优化 volatile 
float yaw_last = 0.0f;
float Zgyro_speed = 0.0f;
float ZgyroSpeed_last = 0.0f;
float Zgyro_last=0.0f;
float Zgyro=0.0f;
/**************************实现函数*******************************************
*函数原型：     Yaw_Zero_Floating(void)
*功　　能：     计算icm963ra的yaw零漂值（后续打算再进行角速度均值）
*输入参数：     无
*输出参数：     无
*注意:        只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
// void Yaw_Zero_Floating(void)
// {
// 	unsigned int i = 0;
//     Zgyro_Zero = 0;
//     for (i = 0; i < 1000; i++)
//     {
//         imu660ra_get_gyro();            // 获取陀螺仪角速度
//         Zgyro_Zero += imu660ra_gyro_z;
//         system_delay_ms(5);
//     }
//     Zgyro_Zero /= 1000;                 //均值滤波
//     gyro_flag = 1;
// }

/**************************实现函数*******************************************
*函数原型：     Angle_Speed_Get(void)
*功　　能：     角速度获取
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Angle_Speed_Get(void)
{
	imu660ra_get_gyro();
	angle_speed = imu660ra_gyro_transition(imu660ra_gyro_z);                //转换°/s
	Zgyro_speed = imu660ra_gyro_transition(imu660ra_gyro_y);   
	if(firstRun.gyro)
	{
		angleSpeed_last = angle_speed;
		ZgyroSpeed_last = Zgyro_speed;
		firstRun.gyro = 0;
	}
	Zgyro_speed = Zgyro_speed*0.7 + ZgyroSpeed_last*0.3;
     ZgyroSpeed_last = Zgyro_speed;
	angle_speed = angle_speed*0.7 + angleSpeed_last*0.3;
	angleSpeed_last = angle_speed;
}

/**************************实现函数*******************************************
*函数原型：     Yaw_get(void)
*功　　能：     icm963ra航向角（yaw）获取（对角速度进行积分）
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Yaw_Get(void)
{
	yaw_last = yaw;                                            //赋值上次角度
	yaw -= angle_speed * 0.001;                               //当前航向角
	yaw = yaw*0.8 + yaw_last*0.2;                          //滤波
}
/**************************实现函数*******************************************
*函数原型：     Yaw_get(void)
*功　　能：     icm660ra仰角获取（对角速度进行积分）
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/

void Zgyro_Get(void)
{
    Zgyro_last = Zgyro;                                            //赋值上次角度
    Zgyro += Zgyro_speed * 0.00001;                               //当前航向角
    Zgyro = Zgyro*0.95+  ZgyroSpeed_last*0.05;                          //滤波
}