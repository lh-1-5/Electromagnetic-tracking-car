#include "Data.h"

int16  fuya = 700;
int16  duty = 500;
volatile float Turn = 0.0f;                   //转向环PID运算得到的输出 作用到速度环目标值
volatile int16 speed_l, Lastspeed_l = 0;                  //速度环输出结果 作用到PWM
volatile int16 speed_r, Lastspeed_r = 0;
volatile float gyroOut = 0.0f;                //角速度环输出 
volatile float basic = 0.0f;                  //速度环基础速度


Flag_Struct     flag;       //标志位结构体
State           Sta;        //状态机结构体
First_Run       firstRun;   //首次执行标志

void Flag_Struct_Init(void)
{
    flag.Run = 0;           //运行状态 充电充满即运行
    flag.Loop   = 0;
	flag.show  = 0;   //屏幕
	flag.start = 0;
	flag.stop = 0;
	flag.charge = 0;
	flag.speed = 1;
	flag.Stra=0;
	
}

void State_Struct_Init(void)
{
    Sta.Circle_Process = 0;
	Sta.Circle_Count   = 0;
	Sta.Stra_Count = 0;
	   
}

void FirstRun_Struct_Init(void)
{
    firstRun.speedOut = 1;
    firstRun.encoder = 1;
    firstRun.gyro = 1;
}

 