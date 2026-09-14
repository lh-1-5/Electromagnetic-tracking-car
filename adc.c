#include "adc.h"

/**************************ADC**************************/
ADC_Val     AdcVal;	    //初始电感值

/**************************实现函数*******************************************
*函数原型：     Adc_Val_Init(void)
*功　　能：     结构体ADC成员（采样值）初始化
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Adc_Struct_Init(void)
{
    AdcVal.l = 0;
    AdcVal.ml= 0;
	AdcVal.m = 0;
    AdcVal.mr= 0;
    AdcVal.r = 0;
	AdcVal.cap = 0;
}

/**************************实现函数*******************************************
*函数原型：     All_adc_Init(void)
*功　　能：     ADC通道初始化
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void All_Adc_Init(void)
{
	adc_init(ADC_LEFT_2, ADC_12BIT);
	adc_init(ADC_LEFT_1, ADC_12BIT);
//	adc_init(ADC_MID   , ADC_12BIT);
	adc_init(ADC_RIGHT_1, ADC_12BIT);
	adc_init(ADC_RIGHT_2, ADC_12BIT);
	adc_init(ADC_CAP    , ADC_12BIT);
}

/**************************实现函数*******************************************
*函数原型：     All_adc_Get(void)
*功　　能：     电感初始值读取
*输入参数：     无
*输出参数：     无
*注意:          只执行一次
*作者：
*时间：        2024.10.23
*****************************************************************************/
void All_Adc_Get(void)
{
	AdcVal.l  = adc_convert(ADC_LEFT_2);
	AdcVal.ml = adc_convert(ADC_LEFT_1);
//	AdcVal.m  = adc_convert(ADC_MID);
	AdcVal.mr = adc_convert(ADC_RIGHT_1);
	AdcVal.r  = adc_convert(ADC_RIGHT_2);
}


