#include "math.h"
#include "Track.h"
#include "Motor.h"
#include "IMU.h"
Finnal_Val      Finnal;                //电感归一化后的最终值

uint16 L2_max=3600, L1_max=3600, R1_max=3600, R2_max=3600;// M_max=3500;
uint16 L2_min=0, L1_min=0, R1_min=0, R2_min=0, M_min=0;    			//四个电感的最值
int16 sum, differ;
int16 s1 =0;
int16 s2 =0;
/**************************Track**************************/
uint16 adc_sum = 0;											//滤波后四个电感的总值
float Bias = 0.0f;                                          //差比和得到的偏差,上次误差
int dir=0;
float kin  = 0.0f;		//电感权重 
float kout = 0.0f;
float last_Zgyro=0.0f;
float start_cap = 0.0f; //发车电容电压
uint8 ben_enter;        //苯环入口
float gyro=0.0f;
uint16 distance = 0;    //发车距离
int Stra_Count=0;
uint16 all_angle = 0;
int qiaoqiaoban = 1;
/**************************实现函数*******************************************
*函数原型：     FinnalVal_Struct_Init(void)
*功　　能：     结构体ADC终值成员（采样值）初始化
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void FinnalVal_Struct_Init(void)
{
    Finnal.l = 0;
    Finnal.ml= 0;
//		Finnal.m = 0;
    Finnal.mr= 0;
    Finnal.r = 0;
		Finnal.cap=0.0f;
}

/**************************实现函数*******************************************
*函数原型：     Adc_Normalize(void)
*功　　能：     （四个电感）归一化处理
*输入参数：     无
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Adc_Normalize(void)
{
    Finnal.l  = (Filter.l - L2_min)*100  / (L2_max - L2_min);
		Finnal.ml = (Filter.ml - L1_min)*100 / (L1_max - L1_min);
//		Finnal.m  = (Filter.m - M_min)*100   / (M_max - M_min);
		Finnal.mr = (Filter.mr - R1_min)*100 / (R1_max - R1_min);
		Finnal.r  = (Filter.r - R2_min)*100  / (R2_max - R2_min);
}


/***********/
/*发车 电容电压检测*/
/************/
void run_start(void)
{
	if(!flag.charge)
	{
		AdcVal.cap = adc_mean_filter_convert (ADC_CAP, 10);
		Finnal.cap = AdcVal.cap*14.0/ADC_CAP_MAX;
		if(Finnal.cap >= start_cap)	
		{
			distance = 0;  // 从进入 start 状态开始重新计时，避免被上电累计时间影响
			flag.start = 1;
			flag.charge = 1;
			flag.Run =2;
			
	
		}
		else if(Finnal.cap<=11.4)
		{
			gpio_high(IO_P07);
		}
	}
}


///**************************实现函数*******************************************
//*函数原型：     etc_set(void)

//*功　　能：     权重设置
//*输入参数：     无
//*输出参数：     无
//*注意:          
//*作者：
//*时间：        2024.10.23
//*****************************************************************************/
void etc_set(float out, float in)
{
	kout = out;
	kin  = in;
}



/**************************实现函数*******************************************
*函数原型：     Error_get(float l2, float l1, float r1 ,float r2, uint8 mode)
*功　　能：     差比和(差)（偏差获取）
*输入参数：     四个电感滤波后的值以及循迹状态
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Error_Get(uint16 l2, uint16 l1, uint16 r1 ,uint16 r2, uint8 mode)
{
	switch(mode)
	{
		case STRAIGHTLINE:
		{
			differ = kout*(l2 - r2) + kin*(l1 - r1);
//#include <AI8051U.H>
			sum = (l2 + r2) + abs(l1 - r1);
			Bias = (differ*100) / (sum+1);
		}break;

		case INLOOP:
		{
			differ = kin*(l1 -r1);
			sum  = l1 + r1;
			Bias = (differ*100) / (sum+1);
		}break;

		case OUTLOOP:
		{
			differ = l2 - r2;
			sum  = l2 + r2;
			Bias = kout*(differ*100) / (sum+1);
		}break;
	}	
}

/**************************实现函数*******************************************
*函数原型：     StraightLine_Deal(void)
*功　　能：     直线处理
*输入参数：     
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void StraightLine_Deal(void)
{
	etc_set(1.2, 1.5);
	Error_Get(Finnal.l, Finnal.ml, Finnal.mr ,Finnal.r, STRAIGHTLINE);
}


/**************************实现函数*******************************************
*函数原型：    qiao_Deal(void)
*功　　能：     圆环处理（状态机）
*输入参数：     
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void qiao_Deal(void)
{
	if(qiao_qiao)
	{
		basic=20;
		Sta.qiaoqiaoban_Count++;
			if(Sta.qiaoqiaoban_Count == 10)
				{	
					Sta.qiaoqiaoban_Count = 0;
					basic = basic_munuout*basic_munu_k;			
				if(adc_sum >40)
				{
				flag.Qiao = 0;
					}		
		}
	}
}



/**************************实现函数*******************************************
*函数原型：     Circle_Deal(void)
*功　　能：     圆环处理（状态机）
*输入参数：     
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Circle_Deal(void)
{
	
	if(Sta.Circle_Process == 0)
	{
		if(Finnal.l*0.5+Finnal.r*0.5 < 70 && Finnal.ml*0.5+Finnal.mr*0.5 < 10) //可尝试直接开启陀螺仪积分 通过角度值判断是否入环
		{
			Sta.Circle_Process = 1;
		}
	}

	else if(Sta.Circle_Process == 1)	//入环  
	{
		Sta.Circle_Process = 2;
	}
	
	else if(Sta.Circle_Process == 2)
	{
		if (yaw >= 37.0 )		//(yaw >= 20.0 || yaw <= -20.0) && Sta.Circle_Count == 20
		{
			dir=1;
			Sta.Circle_Process = 3;
		}
		else if (yaw <= -37.0 )		//(yaw >= 20.0 || yaw <= -20.0) && Sta.Circle_Count == 20
		{
			dir=2;
			Sta.Circle_Process = 3;
		}
	}

	else if(Sta.Circle_Process == 3)
	{
		if(yaw >310 || yaw <= -310)		//判断出环all_angle
		{
			
			Sta.Circle_Process = 4;
		}
	}
	
	else if(Sta.Circle_Process == 4)
	{ 
		Sta.Circle_Count++;
//		s1 = -encoder_get_count(ENCODER_DIR_L)*Sta.Circle_Count;
//    s2 = encoder_get_count(ENCODER_DIR_R)*Sta.Circle_Count;
		if(Sta.Circle_Count>70)
		{
			Sta.Circle_Count = 0;			
		 //yaw = 0;	
			Sta.Circle_Process = 5;
		}
		
	}
	
	switch (Sta.Circle_Process)
	{
		case 0:
		{
			
			Error_Get(0, Finnal.ml, Finnal.mr ,0, INLOOP);
		}break;
		case 1:
		{
			gpio_high(IO_P07);
			Error_Get(0, Finnal.ml, Finnal.mr ,0, INLOOP);
		}break;
		case 2:
		{
//			 Yaw_Get();
			if(dir==1)
			Error_Get(0, 5*Finnal.ml, Finnal.mr ,0, INLOOP);
			 else if(dir==2)
				 Error_Get(0, Finnal.ml, 5*Finnal.mr ,0, INLOOP);
			Yaw_Get();
		}break;
		case 3:
		{
//			gpio_low(IO_P07);
			Yaw_Get();		//航向角积分
			Error_Get(Finnal.l, Finnal.ml, Finnal.mr ,Finnal.r, STRAIGHTLINE);    //环内 正常循迹
		}break;
		case 4:
		{
			
		  Yaw_Get();
			if(dir==1)
			Error_Get(Finnal.l, 0,0 ,3*Finnal.r, OUTLOOP);						//出环 外电感差比和
		  else if(dir==2)
			Error_Get(2*Finnal.l, 0,0 ,Finnal.r, OUTLOOP);
		}break;
		case 5:
		{ 
			yaw=0;
			Yaw_Get();
			gpio_low(IO_P07);//关蜂鸣器
			Sta.Circle_Count = 0;
			Sta.Circle_Process = 0; 
			flag.Loop = 0;			//出环 标志位重置
		}break;
		default:
			break;
	}
}


/**************************实现函数*******************************************
*函数原型：     Line_Tracking(void)
*功　　能：     循迹函数
*输入参数：     无
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Line_Tracking(void)
{
	adc_sum = Finnal.l + Finnal.ml + Finnal.mr + Finnal.r;	//电感和
	Stra_Deal();
	StraightLine_Deal();
	Zgyro_Get();
	if( LOOP_ROAD )
		flag.Loop = 1;

	if(flag.Loop)	    
		Circle_Deal();				//环岛
	
	if(qiao_qiao)
		flag.Qiao =1;
		
	if(flag.Qiao)	
		qiao_Deal();	
}	
//  if(flag.Stra)
//	StraightLine_Deal();
	



/**************************实现函数*******************************************
*函数说明：用于脱离赛道时的停止，防止跑走
*参数：    无
*返回值：  无
*反馈：    将运行标志位置0 速度环锁0
*****************************************************************************/
void Track_Protect(void)
{
	if(OUT_ROAD)
	{
		flag.Run = 0; 
		flag.stop= 1;
	}
}	
/**************************实现函数*******************************************
*函数说明：用于循迹立体元素
*参数：    无
*返回值：  无
*反馈：    将运行标志位置0 速度环锁0
***
*/
void Stra_Deal(void)
{
    Sta.Stra_Count++;
    if(Sta.Stra_Count==5)
    {
        Sta.Stra_Count=0;
        last_Zgyro = gyro;        
        gyro = Zgyro;
        if((gyro-last_Zgyro)>5||(gyro-last_Zgyro)<-5||(Zgyro>70&&Zgyro<200))
        {
            flag.Stra = 1;
          // gpio_high(IO_P07);
//            pwm_set_duty(PWMB_CH3_P33,5000);
        }
        else
        {
            flag.Stra = 0;
//            pwm_set_duty(PWMB_CH3_P33,0);
         // gpio_low(IO_P07);
            Zgyro = 0;
        }
    }
}
