/*
 * Pid.h
 *
 *  Created on: 2024年10月25日
 *      Author: 26742
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#include "zf_common_headfile.h"

/*积分限幅*/
#define INTEG_MAX   			3000
#define INTEG_MIN   			-3000

typedef struct
{
    float kp;           //比例
    float ki;           //积分
    float kd;           //微分
    float desired;      //期望值
    float error;        //误差
    float preverror;    //前一次误差
		float lasterror;		//上上次误差
    float integ;        //误差积分
		float prev_integ;   //保存积分项累加前的值
    float deriv;        //误差微分
    float MaxValue;     //输出最大值
    float MinValue;     //输出最小值
		float add;						//增加幅度
    float out;          //输出
	  float inc_out ;       
	 float pi_out ;        //pi乘积输出
} PidObject;


typedef struct
{
    float kp1;           //三次项比例
    float kp2;           //一次项比例
    float kd;            //微分项
    float kd_gyro;       //角速度微分项
    float desired;      //期望值
    float error;        //误差
    float preverror;    //前一次误差
    float integ;        //误差积分
    float deriv;        //误差微分
    float MaxValue;     //输出最大值
    float MinValue;     //输出最小值
    float out;          //输出
} Pid_Turn_Loop;

extern  PidObject           Pid_Speed_l;
extern  PidObject           Pid_Speed_r;
extern  Pid_Turn_Loop				Pid_Turn;
extern  PidObject           gyro_loop;

void  PidInit(PidObject *pid, float desired, float KP, float KI, float KD, float Max, float Min);
float pidout(PidObject *pid, float measure);
float ic_pidout(PidObject *pid, float measure);
void  Pid_Turn_Init(Pid_Turn_Loop *pid, float desired, float KP1, float KP2, float KD1, float KD2, float Max, float Min);
float Pid_Turn_Out(Pid_Turn_Loop *pid, float measure);



#endif /* CODE_PID_H_ */