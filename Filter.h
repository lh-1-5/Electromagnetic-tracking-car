#ifndef __FILTER_H_
#define __FILTER_H_

#include "zf_common_headfile.h"

#define FILTER_LEFT_2		0
#define FILTER_LEFT_1		1
#define FILTER_MID		    2
#define FILTER_RIGHT_1		3
#define FILTER_RIGHT_2		4

typedef struct
{
	float u0;
	float u1;
	float y0;
	float y1;
	float aa;
	float bb;
	float cc;
	float yy;
}LOW_Pass_Filter;

typedef struct 
{
    float l;    
    float ml; 
	float m;
    float mr;
    float r;
}Filter_Val;          //adc滤波值

extern Filter_Val Filter;	
extern LOW_Pass_Filter		low_pass_filter[5];

void FilterVal_Struct_Init(void);

void  Low_Pass_Filter_Init(LOW_Pass_Filter *Low_pass_filter);
float Adc_Val_Filter(LOW_Pass_Filter *low_pass_filter , uint16 num);

void Filter_All_Init(void);
void Filter_All_Val(void);

#endif
