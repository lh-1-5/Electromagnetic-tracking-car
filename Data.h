#ifndef __DATA_H
#define __DATA_H

#include "zf_common_headfile.h"

typedef struct 
{
    uint8 Run;    //程序运行标志位
    uint8 Loop;   //环岛标志位
	uint8 show;			   //屏幕显示标志位
	uint8 Qiao;
	uint8 start;
	uint8 stop;
	uint8 charge;
	uint8 speed;
	uint8 Stra;
}Flag_Struct;//标志位

typedef struct 
{
    volatile uint8  Circle_Process;           //圆环状态
    volatile uint16 Circle_Count;
	  volatile uint8 Stra_Count;
	  volatile uint8  qiaoqiaoban_Count; 
}State;//跑元素的状态机

typedef struct 
{
    uint8 speedOut;
    uint8 encoder;
    uint8 gyro;
}First_Run;//滤波第一次执行标志位

extern int16 fuya;
extern int16 duty;
extern volatile float Turn;
extern volatile int16 speed_l, Lastspeed_l;
extern volatile int16 speed_r, Lastspeed_r;
extern volatile float gyroOut;
extern volatile float basic;
extern Flag_Struct     flag;
extern State           Sta;
extern First_Run       firstRun;   //首次执行标志

void Flag_Struct_Init(void);
void State_Struct_Init(void);
void FirstRun_Struct_Init(void);

#endif
