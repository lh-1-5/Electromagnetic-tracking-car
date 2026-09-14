/*
 * Pid.c
 *
 *  Created on: 2024年10月25日
 *      Author: 26742
 */

#include "Pid.h"


PidObject     Pid_Speed_l;
PidObject     Pid_Speed_r;
Pid_Turn_Loop	 Pid_Turn;
PidObject     gyro_loop;

/**************************实现函数*******************************************
*函数原型：     PidInit(PidObject *pid, float desired, float KP, float KI, float KD, float Max, float Min)
*功　　能：     位置式/增量式PID初始化
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
void PidInit(PidObject *pid, float desired, float KP, float KI, float KD, float Max, float Min)
{
    pid->error = 0;
    pid->preverror = 0;
    pid->integ = 0;
		pid->prev_integ = 0;
		pid->lasterror = 0;
    pid->deriv = 0;
    pid->desired = desired;
    pid->kp = KP;
    pid->ki = KI;
    pid->kd = KD;
    pid->MaxValue = Max;
    pid->MinValue = Min;
}

/**************************实现函数*******************************************
*函数原型：     pidout(PidObject *pid, float measure)
*功　　能：     位置式PID输出
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
// float pidout(PidObject *pid, float measure)
// {
//     pid->error =  pid->desired - measure;
// //	pid->error =  measure - pid->desired;
//     pid->integ += pid->error;
//     pid->deriv = pid->error - pid->preverror;
//     pid->preverror = pid->error;

//     if (pid->integ > 20000)
//         pid->integ = 20000;
//     else if (pid->integ < -20000)
//         pid->integ = -20000;

//     pid->out = (float)(pid->kp * pid->error + pid->ki * pid->integ + pid->kd * pid->deriv);

//     // 输出限幅
//     if (pid->out > pid->MaxValue)
//         pid->out = pid->MaxValue;
//     if (pid->out < pid->MinValue)
//         pid->out = pid->MinValue;

//     return pid->out;
// }

//float pidout(PidObject *pid, float measure)

//		{
//    pid->error = pid->desired - measure;
//    pid->prev_integ = pid->integ; // 保存积分项累加前的值

//    // 积分项累加并限幅

////    if (fabs(pid->error) > ERROR_THRESHOLD) {
////        pid->integ = 0; // 或保持当前值不更新
////    } else {
////        pid->integ += pid->error; // 正常积分
////    }

//    pid->integ += pid->error;

//    if (pid->integ > INTEG_MAX) {
//        pid->integ = INTEG_MAX;
//    } else if (pid->integ < INTEG_MIN) {
//        pid->integ = INTEG_MIN;
//    }

//    // 计算微分项并更新历史误差
//    pid->deriv = pid->error - pid->preverror;
//    pid->preverror = pid->error;

//    // 计算输出
//    pid->out = (float)(pid->kp * pid->error + pid->ki * pid->integ + pid->kd * pid->deriv);

//    // 输出限幅与抗积分饱和处理
//    if (pid->out > pid->MaxValue) {
//        pid->out = pid->MaxValue;
//        if (pid->error > 0) {
//            pid->integ = pid->prev_integ; // 回退积分项
//            // 重新应用积分限幅
//            if (pid->integ > INTEG_MAX) pid->integ = INTEG_MAX;
//            else if (pid->integ < INTEG_MIN) pid->integ = INTEG_MIN;
//        }
//    } else if (pid->out < pid->MinValue) {
//        pid->out = pid->MinValue;
//        if (pid->error < 0) {
//            pid->integ = pid->prev_integ;
//            if (pid->integ > INTEG_MAX) pid->integ = INTEG_MAX;
//            else if (pid->integ < INTEG_MIN) pid->integ = INTEG_MIN;
//        }
//    }

//    return pid->out;
//}


float pidout(PidObject *pid, float measure)

		{
    pid->error = pid->desired - measure;
    pid->prev_integ = pid->integ; // 保存积分项累加前的值
    pid->inc_out =pid->ki * pid->integ;        //pi乘积输出
    // 积分项累加并限幅

//    if (fabs(pid->error) > ERROR_THRESHOLD) {
//        pid->integ = 0; // 或保持当前值不更新
//    } else {
//        pid->integ += pid->error; // 正常积分
//    }

    pid->integ += pid->error;

    if (pid->inc_out > INTEG_MAX) {
        pid->inc_out = INTEG_MAX;
    } else if (pid->inc_out< INTEG_MIN) {
        pid->inc_out = INTEG_MIN;
    }

    // 计算微分项并更新历史误差
    pid->deriv = pid->error - pid->preverror;
    pid->preverror = pid->error;

    // 计算输出
    pid->out = (float)(pid->kp * pid->error + pid->ki * pid->integ + pid->kd * pid->deriv);

    // 输出限幅与抗积分饱和处理
    if (pid->out > pid->MaxValue) {
        pid->out = pid->MaxValue;
        if (pid->error > 0) {
            pid->integ = pid->prev_integ; // 回退积分项
            // 重新应用积分限幅
            if (pid->inc_out> INTEG_MAX) pid->inc_out = INTEG_MAX;
            else if (pid->inc_out< INTEG_MIN) pid->inc_out = INTEG_MIN;
        }
    } else if (pid->out < pid->MinValue) {
        pid->out = pid->MinValue;
        if (pid->error < 0) {
            pid->integ = pid->prev_integ;
            if (pid->inc_out > INTEG_MAX) pid->inc_out = INTEG_MAX;
            else if (pid->inc_out < INTEG_MIN) pid->inc_out = INTEG_MIN;
        }
    }

    return pid->out;
}
///**************************实现函数*******************************************
//*函数原型：     pidout(PidObject *pid, float measure)
//*功　　能：     增量式PID输出
//*输入参数：     无
//*输出参数：     无
//*作者：
//*时间：        2026.1.28
//*****************************************************************************/
//float ic_pidout(PidObject *pid, float measure)
//{
//	  pid->error = pid->desired - measure;
//		//pid->add = (float)(pid->kp * (pid->error - pid->preverror) +  pid->ki * pid->error + pid->kd * (pid->error -2*pid->preverror +	pid->lasterror));
//		pid->lasterror = pid->preverror;
//		pid->preverror = pid->error;
//		pid->out += pid->add;
//		// 输出限幅
//		 if ((pid->out >= pid->MaxValue && pid->add > 0) ||
//        (pid->out <= pid->MinValue && pid->add < 0)) {
//        // 正在饱和且试图更饱和：忽略这次累加
//        // 什么也不做，out 保持不变
//    } else {
//        pid->out += pid->add;
//    }

//    // 安全限幅（因为浮点计算或初始值仍可能微弱越界）
//    if (pid->out > pid->MaxValue) pid->out = pid->MaxValue;
//    else if (pid->out < pid->MinValue) pid->out = pid->MinValue;

//    return pid->out;
//		
//}
		   /**************************实现函数*******************************************
*函数原型：     pidout(PidObject *pid, float measure)
*功　　能：     增量式PID输出
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2026.1.28
*****************************************************************************/
//float ic_pidout(PidObject *pid, float measure)
//{
//	  pid->error = pid->desired - measure;
//		pid->deriv = pid->error-pid->preverror;
//		pid->preverror = pid->error;
//		pid->inc_out = (float)(pid->kp * pid->deriv + pid->ki * pid->error);
//    // inc输出限幅
//        if (pid->inc_out > INTEG_MAX)//+
//            pid->inc_out = INTEG_MAX;
//        if (pid->inc_out < INTEG_MIN)//-
//            pid->inc_out = INTEG_MIN;
//        pid->out +=pid->inc_out;
//				// 输出限幅
//    if (pid->out > pid->MaxValue)
//        pid->out = pid->MaxValue;
//    if (pid->out < pid->MinValue)
//        pid->out = pid->MinValue;

////    pid->out_last = pid->out;

//    return pid->out;
//		
//}

/**************************实现函数*******************************************
*函数原型：     PidInit(PidObject *pid, float desired, float KP, float KI, float KD, float Max, float Min)
*功　　能：     转向环PID初始化
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Pid_Turn_Init(Pid_Turn_Loop *pid, float desired, float KP1, float KP2, float KD1, float KD2, float Max, float Min)
{
    pid->error = 0;
    pid->preverror = 0;
    pid->integ = 0;
    pid->deriv = 0;
    pid->desired = desired;
    pid->kp1 = KP1;
    pid->kp2 = KP2;
    pid->kd  = KD1;
    pid->kd_gyro = KD2;
    pid->MaxValue = Max;
    pid->MinValue = Min;
}

/**************************实现函数*******************************************
*函数原型：     Pid_Turn_Out(Pid_Turn_Loop *pid, float measure)
*功　　能：     转向环PID输出
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
float Pid_Turn_Out(Pid_Turn_Loop *pid, float measure)
{
//	pid->error =  measure - pid->desired ;
    pid->error =  pid->desired - measure;
    pid->deriv = pid->error - pid->preverror;
    pid->preverror = pid->error;

    pid->out = (float)(pid->kp1*pid->error*pid->error*pid->error + pid->kp2*pid->error + pid->kd*pid->deriv + pid->kd_gyro*angle_speed);

    // 输出限幅
    if (pid->out > pid->MaxValue)
        pid->out = pid->MaxValue;
    if (pid->out < pid->MinValue)
        pid->out = pid->MinValue;

    return pid->out;
}