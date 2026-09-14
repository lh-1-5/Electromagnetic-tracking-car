/*
 * ALL_init.c
 * 系统初始化与屏幕数据显示。
 *
 * 备注：如果你看到中文注释变成乱码，通常是源码编码不匹配导致。
 * 代码本身不受影响；如果要恢复成可读中文，请用 VS Code 以 GBK/ANSI 重新打开后再另存。
 */

#include <AI8051U.H>
#include "ALL_init.h"

#include "..\\_user\\user_config.h"

#include "manu.h"

#include "zf_device_ips114.h"

char text[50];		// 字符串缓存：用于屏幕显示

/*
 * All_Init
 * 全局外设初始化：IO/ADC/滤波/屏幕/电机/定时器等。
 */
void All_Init(void)
{
	/****** IO 初始化 *******/
	gpio_init(IO_P07, GPO, GPIO_LOW,  GPO_PUSH_PULL); // LED1
	gpio_init(IO_P24, GPI, GPIO_HIGH, GPI_PULL_UP);   // 启动/模式选择输入（上拉）
	gpio_init(IO_P16, GPO, GPIO_HIGH, GPO_PUSH_PULL); // 预留输出

	/* 按键输入初始化（上拉） */
	gpio_init(IO_P36, GPI, GPIO_LOW,  GPI_PULL_UP);
	gpio_init(IO_P37, GPI, GPIO_LOW,  GPI_PULL_UP); 
	gpio_init(IO_P45, GPI, GPIO_LOW,  GPI_PULL_UP); 
	gpio_init(IO_P46, GPI, GPIO_LOW,  GPI_PULL_UP); 
	gpio_init(IO_P24, GPI, GPIO_LOW,  GPI_PULL_UP);
	// 菜单按键：P37/P36/P45/P46
	
	Flag_Struct_Init(); // 标志位结构体初始化

	iap_init(); // EEPROM/IAP 初始化
	
	/****** 外设/模块初始化（自检示例已注释） *******/
	
	// 无线串口/IMU 初始化失败时可闪灯提示
	if (wireless_uart_init())
	{
		while (1)
		{
			gpio_toggle_level(IO_P07);
			system_delay_ms(100);
		}
	}
	if (imu660ra_init())
	{
		while (1)
		{
			gpio_toggle_level(IO_P07);
			system_delay_ms(100);
		}
	}

	All_Adc_Init();		// ADC 初始化
	Filter_All_Init();	// 滤波结构初始化
	ips114_init();		// 屏幕初始化
	ips114_clear(RGB565_BLACK);
	Motor_Init();		// 电机初始化
	

	// 上电模式：IO_P24=1 进入菜单；否则从 EEPROM 读取参数
		if(gpio_get_level(IO_P24)==1)
		{
			char dummy;
			dummy = 0;
			Manu_one();		
			main_manu_xianshi(&dummy);			// 菜单显示
		}
		else
		{
			init_read_eepromdata();
			
		}
		ips114_clear(RGB565_BLACK);

	/******* 数据结构初始化 *******/
	Adc_Struct_Init();
	FilterVal_Struct_Init();
	FinnalVal_Struct_Init();
	State_Struct_Init();
	FirstRun_Struct_Init();

	start_cap = start_cap_munuout*start_cap_munu_k;
	ben_enter = ben_enter_munuout*ben_enter_munu_k;  

	flag.show = show_munu_k*show_munuout;
	flag.Run  = run_munu_k*run_munuout;
	
	basic = basic_munuout*basic_munu_k;
	PidInit(&Pid_Speed_l, basic, kpspeed_munuout*kpspeed_munu_k, kispeed_munuout*kispeed_munu_k, 0.0, 8500, -8500);
	PidInit(&Pid_Speed_r, basic, kpspeed_munuout*kpspeed_munu_k, kispeed_munuout*kispeed_munu_k, 0.0, 8500, -8500);
//	PidInit(&Pid_Speed_l, 60, 100, kispeed_munuout*kispeed_munu_k, 0.0, 7000, -7000);
//	PidInit(&Pid_Speed_r, 60, 100, kispeed_munuout*kispeed_munu_k, 0.0, 7000, -7000);
	PidInit(&gyro_loop,     0.0, kpangle_munuout*kpangle_munu_k, kiangle_munuout*kiangle_munu_k, kdangle_munuout*kdangle_munu_k,  75.0, -75.0);	    // 陀螺角速度环
	Pid_Turn_Init(&Pid_Turn,0.0, kp_3_munuout*kp_3_munu_k, kp_munuout*kp_munu_k, kd_munuout*kd_munu_k, -0.05, 2000, -2000);	// 转向环初始化
	
		
	/******* 定时器初始化 *******/
	pit_ms_init(TIM1_PIT, 1);		// 1ms 定时中断
	interrupt_set_priority(TIMER1_IRQn, 3);			// 定时器中断优先级（PID 在定时中断里跑）

	tim1_irq_handler = pit_callback_1;
}

/*
 * All_Str_Show
 * 根据 flag.show 切换不同页面的数据显示。
 */
void All_Str_Show(void)
{
	
	if (flag.show == 1)
	{
		adcVal_show();
	}
	if (flag.show == 2)
	{
		control_data_show();
	}
}

void adcVal_show(void)
{
	sprintf(text, "l2:%d     ", Finnal.l);
	ips114_show_string(1, 16 * 0, text);
	sprintf(text, "l1:%d     ", Finnal.ml);
	ips114_show_string(1, 16 * 1, text);
//	sprintf(text, "m:%.d      ", AdcVal.m);
//	ips114_show_string(1, 16 * 2, text);
	sprintf(text, "r1:%d     ",Finnal.mr);
	ips114_show_string(1, 16 * 3, text);
	sprintf(text, "r2:%d     ", Finnal.r);
	ips114_show_string(1, 16 * 4, text);
	sprintf(text, "adc_sum:%d     ", adc_sum);
	ips114_show_string(1, 16 * 5, text);
	sprintf(text, "cap:%.1f     ", Finnal.cap);
	ips114_show_string(1, 16 * 6, text);
	sprintf(text, "initCap:%d     ", AdcVal.cap);
	ips114_show_string(1, 16 * 7, text);
	sprintf(text, "distance:%d     ", distance);
	ips114_show_string(1, 16 * 8, text);
	sprintf(text, "flag:%d     ",flag.start);
	ips114_show_string(1, 16 * 9, text);
	sprintf(text, "run:%d     ",flag.Run);
	ips114_show_string(1, 16 *10, text);
	sprintf(text, "bias:%d     ", Bias);
	ips114_show_string(1, 16 *11, text);
	
}

void control_data_show(void)
{
	sprintf(text, "Bias:%.2f     ", Bias);
	ips114_show_string(1, 16 * 0, text);
	sprintf(text, "turn:%.2f     ", Turn);
	ips114_show_string(1, 16 * 1, text);
	sprintf(text, "gyro:%.2f     ", angle_speed);
	ips114_show_string(1, 16 * 2, text);
	sprintf(text, "gyroOut:%.2f  ", gyroOut);
	ips114_show_string(1, 16 * 3, text);
	sprintf(text, "pwml:%d  ", speed_l);
	ips114_show_string(1, 16 * 4, text);
	sprintf(text, "pwmr:%d  ", speed_r);
	ips114_show_string(1, 16 * 5, text);
//	#if ENCODER_MANUAL_TEST_MODE
//	sprintf(text, "vlT:%ld     ", (long)encoder_total_l);
//	ips114_show_string(1, 16 * 6, text);
//	sprintf(text, "vrT:%ld     ", (long)encoder_total_r);
//	ips114_show_string(1, 16 * 7, text);
//#else
	sprintf(text, "vl:%d     ", Encoder_l);
	ips114_show_string(1, 16 * 8, text);
	sprintf(text, "vr:%d     ", Encoder_r);
	ips114_show_string(1, 16 * 9, text);
//#endif
	sprintf(text, "yaw:%.2f     ", yaw);
	ips114_show_string(1, 16 * 10, text);
	sprintf(text, "yaw:%.2f     ", start_distance_munuout*start_distance_munu_k);
	ips114_show_string(1, 16 * 11, text);
	sprintf(text, "flag.run:%D     ", flag.Run);
	ips114_show_string(1, 16 * 12, text);
	sprintf(text, "flag.run:%D     ", basic);
	ips114_show_string(1, 16 * 13, text);
	 
}

