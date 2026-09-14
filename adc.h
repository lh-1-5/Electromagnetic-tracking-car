#ifndef __ADC_H_
#define __ADC_H_

#include "zf_common_headfile.h"

/****************************************ADC宏定义*******************************************/
#define ADC_LEFT_2			    ADC_CH4_P14   							 //最左边电感引脚
#define ADC_LEFT_1            ADC_CH13_P05                  //左边电感引脚
//#define ADC_MID                ADC_CH14_P06              //中间电感
#define ADC_RIGHT_1         ADC_CH14_P06                          //右边电感引脚
#define ADC_RIGHT_2         ADC_CH9_P01   
#define ADC_CAP             ADC_CH8_P00      //电容adc值
//#define ADC_LEFT_2			ADC_CH0_P10     							 //最左边电感引脚
//#define ADC_LEFT_1          ADC_CH9_P01                  //左边电感引脚
//#define ADC_MID             ADC_CH13_P05                         //中间电感
//#define ADC_RIGHT_1         ADC_CH14_P06                         //右边电感引脚
//#define ADC_RIGHT_2         ADC_CH4_P14


typedef struct 
{
    uint16 l;         //左
    uint16 ml;        //左中
	uint16 m;
    uint16 mr;        //右中
    uint16 r;         //右
    uint16 cap;       //电容
}ADC_Val;            //adc初始值

extern ADC_Val     AdcVal;	    //初始电感值


void Adc_Struct_Init(void);
void All_Adc_Init(void);
void All_Adc_Get(void);


#endif
