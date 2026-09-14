#ifndef __TRACK_H_
#define __TRACK_H_

#include "zf_common_headfile.h"

//#define LOOP_ROAD     (adc_sum>120 && adc_sum<150 && ((Finnal.l>70 &&Finnal.l<80) || (Finnal.r>70&&Finnal.r<80 ))&&Finnal.mr<20&&Finnal.ml<20&&-10<Zgyro&&Zgyro<10)  //入环(Finnal.m > ben_enter)||
#define LOOP_ROAD     (adc_sum>120 && adc_sum<150 && (Finnal.l>70||Finnal.r>80)&&Finnal.mr<25&&Finnal.ml<25&&-2<Zgyro&&Zgyro<2)  //入环(Finnal.m > ben_enter)||

#define qiao_qiao     (adc_sum>20&&adc_sum<30)
#define OUT_ROAD     (Finnal.l<5 && Finnal.r<5 && Finnal.ml<5 && Finnal.mr<5)   //脱线
#define LOOP_Sta        
#define STRAIGHTLINE    0               //直线
#define INLOOP          1               //入环
#define OUTLOOP         2               //出环

#define ADC_CAP_MAX     1580            //电容最大电压对应的adc值

extern int16 s1 ;
extern int16 s2 ;
typedef struct 
{
    uint8 l;    
    uint8 ml; 
	  uint8 m; 
    uint8 mr;
    uint8 r;
    float cap;
}Finnal_Val;          //滤波后归一化的值

extern Finnal_Val      Finnal; 
extern float Bias;
extern uint16 adc_sum;
extern float kout, kin;
extern float start_cap;
extern uint8 ben_enter;    
extern uint16 distance;
extern uint16 all_angle;

void etc_set(float out, float in);
void FinnalVal_Struct_Init(void);
void Adc_Normalize(void);
void Error_Get(uint16 l2, uint16 l1, uint16 r1 ,uint16 r2, uint8 mode);
void Track_Protect(void);
void Line_Tracking(void);
void Circle_Deal(void);
void StraightLine_Deal(void);
void run_start(void);
void Stra_Deal(void);
void qiao_Deal(void);

#endif
