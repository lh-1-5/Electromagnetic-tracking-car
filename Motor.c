/*
 * Motor.c
 *
 *  Created on: 2024年10月24日
 *      Author: 26742
 */

#include "Motor.h"


/**************************ENCODER**************************/
volatile int16 Encoder_l = 0;                   //编码器采样值
volatile int16 Encoder_r = 0;  

/**************************实现函数*******************************************
*函数原型：     Motor_Init(void)
*功　　能：     电机占空比以及编码器引脚初始化
*输入参数：     无
*输出参数：     无
*作者：
*时间：        2024.10.23
*****************************************************************************/
void Motor_Init(void)
{
	  gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, MOTOR_FREQ, 0);      
		gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, MOTOR_FREQ, 0);        
//		pwm_init(PWM_L1, MOTOR_FREQ, 0);                                                 // PWM 通道 L1 初始化频率 17KHz 占空比初始为 0
//    pwm_init(PWM_L2, MOTOR_FREQ, 0);                                                 // PWM 通道 L2 初始化频率 17KHz 占空比初始为 0
//    pwm_init(PWM_R1, MOTOR_FREQ, 0);                                                 // PWM 通道 R1 初始化频率 17KHz 占空比初始为 0
//    pwm_init(PWM_R2, MOTOR_FREQ, 0);   
	    pwm_init(PWM_fy, 50,0);                                                   
	
		encoder_dir_init(ENCODER_DIR_L, ENCODER_DIR_DIR_L, ENCODER_DIR_PULSE_L);   	//初始化编码器模块与引脚 带方向增量编码器模式
    encoder_dir_init(ENCODER_DIR_R, ENCODER_DIR_DIR_R, ENCODER_DIR_PULSE_R);    //初始化编码器模块与引脚 带方向增量编码器模式
}


/*********************************************************************************************************
 ** 函 数 名: void motor_encoder(int8 *encoder_l, int8 *encoder_r)
 ** 功能说明: 读取左右电机的编码器的值
 **          使用指针传参 减少全局变量的使用
 ** 形    参:  encoder：电机编码器的值
 **           
 ** 返 回 值: 无
 ********************************************************************************************************/
void Motor_Encoder(int16 *encoder_l, int16 *encoder_r)
{
	static int16 elLast, erLast;
	
    // 编码器
  *encoder_l = -encoder_get_count(ENCODER_DIR_L);          // 采集对应编码器数据
	*encoder_r = encoder_get_count(ENCODER_DIR_R); 

    if(firstRun.encoder)
    {
        elLast = *encoder_l;
				erLast = *encoder_r;
        firstRun.encoder = 0;
    }

	*encoder_l = *encoder_l * 0.8 + elLast * 0.2;
	*encoder_r = *encoder_r * 0.8 + erLast * 0.2;

    elLast = *encoder_l;
		erLast = *encoder_r;
	
    encoder_clear_count(ENCODER_DIR_L);                                		// 清空编码器计数
    encoder_clear_count(ENCODER_DIR_R);                             		// 清空编码器计数
}

/********************************************************************************************************
** 函数名:  void motor_set_pwm(uint16 pwm_l, uint16 pwm_r)
** 功能说明: 控制电机转速和方向
** 形    参: pwm 电机PWM输出 PID运算之后的pwm_put值
**
** 返 回 值: 无
**备注：用于hip4082电机驱动
*******************************************************************************************************/
void motor_set_pwwhip4082(int16 pwm_l, int16 pwm_r)
{

    if(pwm_l >= 0)                                             // 左侧正转
    {
        gpio_set_level(DIR_L, GPIO_LOW); 
                                     // DIR输出高电平
            pwm_set_duty(PWM_L, pwm_l+DEAD_ZONE_L);                   // 计算占空比

    }
    else                                                       // 左侧反转
    {
       gpio_set_level(DIR_L, GPIO_HIGH);                                          // DIR输出低电平
            pwm_set_duty(PWM_L,  -pwm_l-DEAD_ZONE_L);                   // 计算占空比
    }

    if(pwm_r >= 0)                                             // 右侧正转
    {
            gpio_set_level(DIR_R, GPIO_LOW);                                         // DIR输出高电平
            pwm_set_duty(PWM_R, pwm_r+DEAD_ZONE_L);    
        
    }
    else                                                        // 右侧反转
    {
             gpio_set_level(DIR_R, GPIO_HIGH);                                         // DIR输出高电平
            pwm_set_duty(PWM_R, -pwm_r-DEAD_ZONE_L);        
    }
}

//    if(pwm_l >= 0)                                             // 左侧正转
//    {
//        pwm_set_duty(PWM_L1, pwm_l+DEAD_ZONE_L);                            // 计算占空比
//        pwm_set_duty(PWM_L2, 0);                               // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
//        // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
//    }
//    else                                                       // 左侧反转
//    {
//        pwm_set_duty(PWM_L1, 0);                               // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
//        pwm_set_duty(PWM_L2, -pwm_l-DEAD_ZONE_L);                          // 计算占空比
//    }

//    if(pwm_r >= 0)                                             // 右侧正转
//    {
//        pwm_set_duty(PWM_R1, pwm_r);                             // 计算占空比
//        pwm_set_duty(PWM_R2, 0);                                // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
////		pwm_set_duty(PWM_R1, 0);                             // 计算占空比
////        pwm_set_duty(PWM_R2, pwm_r);     
//    }
//    else                                                        // 右侧反转
//    {
//        pwm_set_duty(PWM_R1, 0);                                // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
//        pwm_set_duty(PWM_R2, -pwm_r);                           // 计算占空比
////		pwm_set_duty(PWM_R1, -pwm_r);                                // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
////        pwm_set_duty(PWM_R2, 0);                           // 计算占空比
//    }
//}

