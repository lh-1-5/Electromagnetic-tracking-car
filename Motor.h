/*
 * Motor.h
 *
 *  Created on: 2024Äê10ÔÂ24ÈÕ
 *      Author: 26742
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#include "zf_common_headfile.h"


#define DIR_R               (IO_P52 )
#define PWM_R               (PWMB_CH2_P51)
#define DIR_L               (IO_P50 )
#define PWM_L               (PWMB_CH4_P53)
//#define PWM_L1                          (PWMB_CH1_P50)
//#define PWM_L2                          (PWMB_CH4_P53)
//#define PWM_R1                          (PWMB_CH2_P51)
//#define PWM_R2                          (PWMB_CH3_P52)
                                                // 控制频率为50HZ，最高支持300HZ
#define PWM_fy             								 (PWMA_CH2P_P12)

//#define PWM_R1                          (PWMB_CH1_P50)
//#define PWM_R2                          (PWMB_CH4_P53)
//#define PWM_L1                          (PWMB_CH2_P51)
//#define PWM_L2                          (PWMB_CH3_P52)

#define ENCODER_DIR_L                 	(TIM0_ENCOEDER)                        
#define ENCODER_DIR_DIR_L              	(IO_P35)            				 
#define ENCODER_DIR_PULSE_L            	(TIM0_ENCOEDER_P34)

#define ENCODER_DIR_R                 	(TIM3_ENCOEDER)                         
#define ENCODER_DIR_DIR_R           		(IO_P13)             				
#define ENCODER_DIR_PULSE_R       			(TIM3_ENCOEDER_P04)            		

#define MOTOR_FREQ          17000      //pwm??
#define DEAD_ZONE_L			0         //??
#define DEAD_ZONE_R			0
extern volatile int16 Encoder_l;
extern volatile int16 Encoder_r;

void Motor_Init(void);
void Motor_Encoder(int16 *encoder_l, int16 *encoder_r);
void motor_set_pwwhip4082(int16 pwm_l, int16 pwm_r);

#endif /* CODE_MOTOR_H_ */
