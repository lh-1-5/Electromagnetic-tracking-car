#include "Filter.h"

/**************************FILTER**************************/
float wc = 100.0f;			//越大跟随效果越好（采样变量变化大的时候大一点，比如交流），越小滤波效果越好（采用变量变化小的时候小一点，比如直流）
float Ts = 9.0f;			//滤波函数采样周期   20ms

LOW_Pass_Filter		low_pass_filter[5];
Filter_Val 			Filter;	    //adc数值经过滤波后的值

/**************************实现函数*******************************************
*函数原型：     Filter_Val_Init(void)
*功　　能：     结构体成员（滤波值）初始化
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void FilterVal_Struct_Init(void)
{
    Filter.l = 0.0f;
    Filter.ml= 0.0f;
	Filter.m = 0.0f;
    Filter.mr= 0.0f;
    Filter.r = 0.0f;
}

/**************************实现函数*******************************************
*函数原型：     Low_Pass_Filter_Init(LOW_Pass_Filter *Low_pass_filter)
*功　　能：     低通滤波变量初始化
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Low_Pass_Filter_Init(LOW_Pass_Filter *Low_pass_filter)
{
	Low_pass_filter->u0 = 0.0f;
	Low_pass_filter->u1 = 0.0f;
	Low_pass_filter->y0 = 0.0f;
	Low_pass_filter->y1 = 0.0f;
	Low_pass_filter->aa = 0.0f;
	Low_pass_filter->bb = 0.0f;
	Low_pass_filter->cc = 0.0f;
	Low_pass_filter->yy = 0.0f;
}

/**************************实现函数*******************************************
*函数原型：     Adc_Val_Filter(LOW_Pass_Filter *Low_pass_filter , unsigned int num)
*功　　能：     滤波值获取
*输入参数：     无
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
float Adc_Val_Filter(LOW_Pass_Filter *Low_pass_filter , uint16 num)
{
	Low_pass_filter->aa = wc * Ts / (2 + wc*Ts);
	Low_pass_filter->bb = wc * Ts / (2 + wc*Ts);
	Low_pass_filter->cc = (2 - Ts*wc) / (2 + wc*Ts);
	Low_pass_filter->u0 = num;
	Low_pass_filter->y0 = Low_pass_filter->aa * Low_pass_filter->u0 + Low_pass_filter->bb * Low_pass_filter->u1 + Low_pass_filter->cc * Low_pass_filter->y1;
	Low_pass_filter->u1 = Low_pass_filter->u0;
	Low_pass_filter->y1 = Low_pass_filter->y0;
	Low_pass_filter->yy = Low_pass_filter->y0;
	
	return Low_pass_filter->yy;
}

/**************************实现函数*******************************************
*函数原型：     Filter_All_Init(void)
*功　　能：     （四个电感）低通滤波变量初始化
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Filter_All_Init(void)
{
	Low_Pass_Filter_Init(&low_pass_filter[FILTER_LEFT_2] );
	Low_Pass_Filter_Init(&low_pass_filter[FILTER_LEFT_1] );
//	Low_Pass_Filter_Init(&low_pass_filter[FILTER_MID] );
	Low_Pass_Filter_Init(&low_pass_filter[FILTER_RIGHT_1]);
	Low_Pass_Filter_Init(&low_pass_filter[FILTER_RIGHT_2]);
}

/**************************实现函数*******************************************
*函数原型：     Filter_All_Val(void)
*功　　能：     （四个电感）低通滤波值获取
*输入参数：     无
*输出参数：     无
*注意:          
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Filter_All_Val(void)
{
	Filter.l  = Adc_Val_Filter(&low_pass_filter[FILTER_LEFT_2] , AdcVal.l);
	Filter.ml = Adc_Val_Filter(&low_pass_filter[FILTER_LEFT_1] , AdcVal.ml);
//	Filter.m  = Adc_Val_Filter(&low_pass_filter[FILTER_MID]    , AdcVal.m);
	Filter.mr = Adc_Val_Filter(&low_pass_filter[FILTER_RIGHT_1], AdcVal.mr);
	Filter.r  = Adc_Val_Filter(&low_pass_filter[FILTER_RIGHT_2], AdcVal.r);
}
