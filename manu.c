#include "manu.h"
/*
*使用注意事项：要打开eeprom初始化使能，本文件已经初始化
*uint8 iap_read_byte(uint32 addr)读取函数
*void iap_write_byte(uint32 addr, uint8 byte);写入一个字节函数
void iap_init(void);初始化函数
void iap_write_buff(uint32 addr, uint8 *buf, uint16 len)一次性写多个字节
void iap_erase_page(uint32 addr)擦除所写页
*擦除完，则所有的值是1
*
*
*/

char textmunu[30];//用于给本文件屏幕输出的数组
/*菜单传出全局变量，用于输出给转向环速度环初始化*/
uint16 kp_munuout,kp_3_munuout,kd_munuout,max_munuout,min_munuout,xianfumax_munuout,basic_munuout,kpspeed_munuout,kdspeed_munuout,kispeed_munuout;
uint16 kpangle_munuout,kdangle_munuout,kiangle_munuout,maxangle_munuout,minangle_munuout,desiredangle_munuout;//角速度环
uint16 run_munuout,show_munuout,start_cap_munuout,ben_enter_munuout,ben_angle_munuout,start_distance_munuout;//全局状态变量

char cursor=10;//光标数字调参个数最大十个

/*
本函数是菜单入口，使用方法是直接放入初始化函数
参数1：为转向环结构体指针
参数2:速度环结构体l轮子
参数3：速度环结构体r轮子
pPid_Turn_Loop pid,
*/
int Manu_one(void)
{	
	char button_flag=1,ok_button=0;//初始化为第一行，确认标志

/*初始化，将eeprom的值先保存下来*/
		init_read_eepromdata();


		/*优化点*/
		main_manu_xianshi(&ok_button);
	
	
	while(1)
	{
/***********************************************/
		if(ok_button!=0)//进入二级菜单入口
		{
	switch(ok_button)
	{
	case 1:
		{
			Manu_two_adjust();//Data,
			
			/*优化点 */
			main_manu_xianshi(&ok_button);
		}break;
	case 2:
		{

			 Manu_two_adjustangle();//

			/*优化点*/
			 main_manu_xianshi(&ok_button);
			
		}break;
	case 3:
		{
			 Manu_two_adjuconfig();

			 main_manu_xianshi(&ok_button);
			 
		}break;
	case 4:
		{
			black_manu();
			main_manu_xianshi(&ok_button);

		
		}break;
		case 5:
		{
			black_manu();
			main_manu_xianshi(&ok_button);

		
		}break;
		case 6:
		{
			black_manu();
			main_manu_xianshi(&ok_button);
		}break;
		case 7:
		{
			black_manu();
			main_manu_xianshi(&ok_button);

		
		}break;
		case 8:
		{
			black_manu();
			main_manu_xianshi(&ok_button);

		
		}break;
		case 9:
		{
			black_manu();
			main_manu_xianshi(&ok_button);

		
		}break;
		case 10:
		{
			black_manu();
			main_manu_xianshi(&ok_button);
		}break;

		/*返回回来重新显示界面*/

	}
/***********************************************/			
			
		}
			

		if(gpio_get_level(IO_P36)==0)//按下增加按键，选项下移
		{
			 Manu_keydelay(IO_P36);

//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P36)==0);
		  system_delay_ms(10);
		  */
			button_flag++;
			if(button_flag==9)
			{
				button_flag=1;
			}
				
		}
		if(gpio_get_level(IO_P37)==0)//按下减小按键，选项上移
		{
			Manu_keydelay(IO_P37);

			//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P37)==0);
		  	system_delay_ms(10);
			*/
			button_flag--;
			if(button_flag==0)
			{
				button_flag=8;
			}
		}
		
		if(gpio_get_level(IO_P45)==0)//返回按键，返回主函数，并且将调整好的值全部传给eeprom
		{
			Manu_keydelay(IO_P45);

			//优化点，按键延时优化

			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P45)==0);
		  system_delay_ms(10);	
		  */		
		/*写入eeprom，擦除加写入*/
		iap_erase_page(Erase_EEPROM);//用于擦除第一页的eeprom的值
		iap_write_byte(kp_munu_addr,kp_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kp_munu_addr+1,kp_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kp_3_munu_addr,kp_3_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kp_3_munu_addr+1,kp_3_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kd_munu_addr,kd_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kd_munu_addr+1,kd_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(max_munu_addr,max_munuout);	//低八位	，下一行高八位					
		iap_write_byte(max_munu_addr+1,max_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(min_munu_addr,min_munuout);	//低八位	，下一行高八位					
		iap_write_byte(min_munu_addr+1,min_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(xianfumax_munu_addr,xianfumax_munuout);	//低八位	，下一行高八位					
		iap_write_byte(xianfumax_munu_addr+1,xianfumax_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(basic_munu_addr,basic_munuout);	//低八位	，下一行高八位					
		iap_write_byte(basic_munu_addr+1,basic_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kpspeed_munu_addr,kpspeed_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kpspeed_munu_addr+1,kpspeed_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kdspeed_munu_addr,kdspeed_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kdspeed_munu_addr+1,kdspeed_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kispeed_munu_addr,kispeed_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kispeed_munu_addr+1,kispeed_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据


/*角速度环存值*/
		iap_write_byte(kpangle_munu_addr,kpangle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kpangle_munu_addr+1,kpangle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kiangle_munu_addr,kiangle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kiangle_munu_addr+1,kiangle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(kdangle_munu_addr,kdangle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(kdangle_munu_addr+1,kdangle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(maxangle_munu_addr,maxangle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(maxangle_munu_addr+1,maxangle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(minangle_munu_addr,minangle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(minangle_munu_addr+1,minangle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(desiredangle_munu_addr,desiredangle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(desiredangle_munu_addr+1,desiredangle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据

		iap_write_byte(run_munu_addr,run_munuout);	//低八位	，下一行高八位					
		iap_write_byte(run_munu_addr+1,run_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(show_munu_addr,show_munuout);	//低八位	，下一行高八位					
		iap_write_byte(show_munu_addr+1,show_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(start_cap_munu_addr,start_cap_munuout);	//低八位	，下一行高八位					
		iap_write_byte(start_cap_munu_addr+1,start_cap_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(ben_enter_munu_addr,ben_enter_munuout);	//低八位	，下一行高八位					
		iap_write_byte(ben_enter_munu_addr+1,ben_enter_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(ben_angle_munu_addr,ben_angle_munuout);	//低八位	，下一行高八位					
		iap_write_byte(ben_angle_munu_addr+1,ben_angle_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据
		iap_write_byte(start_distance_munu_addr,start_distance_munuout);	//低八位	，下一行高八位					
		iap_write_byte(start_distance_munu_addr+1,start_distance_munuout>>8);//这两行代码用于将变量的数据存入eeprom，以便下次能观看到此时的数据





			return button_flag;
		}
		if(gpio_get_level(IO_P46)==0)//确认按键，进入下一级菜单
		{
			Manu_keydelay(IO_P46);

			//优化点，按键延时优化

			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P46)==0);
		  system_delay_ms(10);
		  	*/		
		ok_button=button_flag;
			
		}

		Manu_show_cursor(button_flag);		


	/*//优化点：光标显示函数	
		switch(button_flag)//光标
		{
			case 1:
			{
					ips114_show_string(0, 16*0, "*"); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串

			}break;
			case 2:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, "*"); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
			}break;
					case 3:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, "*"); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
			}break;
					case 4:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, "*"); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
			}break;
					case 5:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, "*"); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
			}break;
					case 6:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, "*"); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
			}break;		
					case 7:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, "*"); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
			}break;			
					case 8:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, "*"); 			//显示字符串
			}break;			
			
		}
	*/	
	}
	
	
	
}

/**************************************************************
本函数用于参数调整，由 Manu_one()函数引用


***************************************************************/
int Manu_two_adjust(void)//pPid_Turn_Loop pid,
{
/*弄几个全局变量，本文件要把eeprom的值传入全局变量
两个字节65535有些变量除掉10，有些除100看需要参数的范围


*/
static uint16 kp_Local,kp_3_Local,kd_Local,max_Local,min_Local,xianfumax_Local,basic_Local,kpspeed_Local,kdspeed_Local,kispeed_Local;
	static float kp_Local_float,kp_3_Local_float,kd_Local_float,max_Local_float,min_Local_float,xianfumax_Local_float,basic_Local_float,kpspeed_Local_float,kdspeed_Local_float,kispeed_Local_float;//用于显示小数

	char button_flag=1,ok_button=0;//初始化为第一行，确认标志范围为1到8
/*优化点*/
	 Manu_2_show();//第一个二级菜单页面显示

	
  while(1)
  {
		if(ok_button!=0)//进入参数改变接口
		{
	switch(ok_button)
	{
	case 1:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "kp:"); 			//3
			ips114_show_string(8, 16*3, "kp_elast:"); 			//3
			kp_Local=Specific_para_adjust( kp_munuout,kp_munu_addr,kp_Local,kp_munu_k);
			kp_munuout=kp_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
		}break;
	case 2:
		{
	
		ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "kp_3:");	
		ips114_show_string(8, 16*3, "kp_3_elast:"); 			
		kp_3_Local=Specific_para_adjust( kp_3_munuout,kp_3_munu_addr,kp_3_Local,kp_3_munu_k);
		kp_3_munuout=kp_3_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

		}break;
	case 3:
		{
				//
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "kd:");	
		ips114_show_string(8, 16*3, "kd_elast:"); 			//3
		kd_Local=Specific_para_adjust( kd_munuout,kd_munu_addr,kd_Local,kd_munu_k);
		kd_munuout=kd_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

		}break;
	case 4:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "max:");	

			ips114_show_string(8, 16*3, "max_elast:"); 			//3
			max_Local=Specific_para_adjust( max_munuout,max_munu_addr,max_Local,max_munu_k);
			max_munuout=max_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
	
		}break;
	case 5:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "min:");	

			ips114_show_string(8, 16*3, "min_elast:"); 			//3
			min_Local=Specific_para_adjust( min_munuout,min_munu_addr,min_Local,min_munu_k);
			min_munuout=min_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case


		}break;	
	case 6:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "xianfumax:");	

			ips114_show_string(8, 16*3, "xianfumax_elast:"); 			//3
			xianfumax_Local=Specific_para_adjust( xianfumax_munuout,xianfumax_munu_addr,xianfumax_Local,xianfumax_munu_k);
			xianfumax_munuout=xianfumax_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case


			
		}break;	
	case 7:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "basic:");	

			ips114_show_string(8, 16*3, "basic_elast:"); 			//3
			basic_Local=Specific_para_adjust( basic_munuout,basic_munu_addr,basic_Local,basic_munu_k);
			basic_munuout=basic_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case

			
			
		}break;
	case 8:
		{
		ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "kpspeed:");	

		ips114_show_string(8, 16*3, "kpspeed_elast:"); 			//3
		kpspeed_Local=Specific_para_adjust(kpspeed_munuout,kpspeed_munu_addr,kpspeed_Local,kpspeed_munu_k);
		kpspeed_munuout=kpspeed_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

			
		}break;
		case 9:
		{		ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "kdspeed:");	

		ips114_show_string(8, 16*3, "kdspeed_elast:"); 			//3
		kdspeed_Local=Specific_para_adjust( kdspeed_munuout,kdspeed_munu_addr,kdspeed_Local,kdspeed_munu_k);
		kdspeed_munuout=kdspeed_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

			
		}break;
		case 10:
		{
		ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "kispeed:");

		ips114_show_string(8, 16*3, "kispeed_elast:"); 			//3
		kispeed_Local=Specific_para_adjust( kispeed_munuout,kispeed_munu_addr,kispeed_Local,kispeed_munu_k);
		kispeed_munuout=kispeed_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

			
			
		}break;			
	}
//返回回来继续显示
/*优化点*/
Manu_2_show();//第一个二级菜单页面显示

		}
		/*
	



		*/
		
		if(gpio_get_level(IO_P45)==0)//返回按键，返回主菜单
		{
			Manu_keydelay(IO_P45);

			//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P45)==0);
		  system_delay_ms(10);
		  */
		  
		//	ok_button=1;

			return button_flag;
		}
		if(gpio_get_level(IO_P46)==0)//确认按键，此确认用于进入具体参数调整数值
		{
			Manu_keydelay(IO_P46);

			//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P46)==0);
		  system_delay_ms(10);	
			*/

		ok_button=button_flag;
			
		}
		if(gpio_get_level(IO_P36)==0)//按下增加按键，选项下移
		{
			Manu_keydelay(IO_P36);

			//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P36)==0);
		  system_delay_ms(10);
		  */
			button_flag++;
			if(button_flag>cursor)
			{
				button_flag=1;
			}
				
		}
		if(gpio_get_level(IO_P37)==0)//按下减小按键，选项上移
		{
			Manu_keydelay(IO_P37);

			//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P37)==0);
		  system_delay_ms(10);*/
			button_flag--;
			if(button_flag==0)
			{
				button_flag=cursor;
			}
		}	
		
		
		Manu_show_cursor(button_flag);		
	/*优化点：光标显示函数	
		switch(button_flag)//显示光标函数
		{
			case 1:
			{
					ips114_show_string(0, 16*0, "*"); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串

			}break;
			case 2:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, "*"); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串

			}break;
					case 3:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, "*"); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串

			}break;
					case 4:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, "*"); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串

			}break;
					case 5:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, "*"); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串

			}break;
					case 6:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, "*"); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串
			}break;		
					case 7:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, "*"); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串
			}break;			
			case 8:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, "*"); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串
			}break;			
			case 9:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, "*"); 			//显示字符串
					ips114_show_string(0, 16*9, " "); 			//显示字符串
			}break;			
			case 10:
			{
					ips114_show_string(0, 16*0, " "); 			//显示字符串
					ips114_show_string(0, 16*1, " "); 			//显示字符串
					ips114_show_string(0, 16*2, " "); 			//显示字符串
					ips114_show_string(0, 16*3, " "); 			//显示字符串
					ips114_show_string(0, 16*4, " "); 			//显示字符串
					ips114_show_string(0, 16*5, " "); 			//显示字符串
					ips114_show_string(0, 16*6, " "); 			//显示字符串
					ips114_show_string(0, 16*7, " "); 			//显示字符串
					ips114_show_string(0, 16*8, " "); 			//显示字符串
					ips114_show_string(0, 16*9, "*"); 			//显示字符串
			}break;			
			
		}
	*/	
  }
	
	return button_flag;
}


/***********************************************/
/*
二级菜单角度环参数调整函数

*/

int Manu_two_adjustangle(void)//pPid_Turn_Loop pid,
{
/*弄几个全局变量，本文件要把eeprom的值传入全局变量
两个字节65535有些变量除掉10，有些除100看需要参数的范围
*/
static uint16 kpangle_Local,kdangle_Local,kiangle_Local,maxangle_Local,minangle_Local,desiredangle_Local;

	char button_flag=1,ok_button=0;//初始化为第一行，确认标志范围为1到8
	 manu_2_angle();
/*优化点*/
	
	
  while(1)
  {
		if(ok_button!=0)//进入参数改变接口
		{
	switch(ok_button)
	{
	case 1:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "kpangle:"); 			//3
			ips114_show_string(8, 16*3, "kpangle_elast:"); 			//3
			kpangle_Local=Specific_para_adjust( kpangle_munuout,kpangle_munu_addr,kpangle_Local,kpangle_munu_k);
			kpangle_munuout=kpangle_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
		}break;
	case 2:
		{
	
		ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "kiangle:");	
		ips114_show_string(8, 16*3, "kiangle_elast:"); 			//3
		kiangle_Local=Specific_para_adjust( kiangle_munuout,kiangle_munu_addr,kiangle_Local,kiangle_munu_k);
		kiangle_munuout=kiangle_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

		}break;
	case 3:
		{
				//
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "kdangle:");	
		ips114_show_string(8, 16*3, "kdangle_elast:"); 			//3
		kdangle_Local=Specific_para_adjust( kdangle_munuout,kdangle_munu_addr,kdangle_Local,kdangle_munu_k);
		kdangle_munuout=kdangle_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

		}break;
	case 4:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "maxangle:");	

			ips114_show_string(8, 16*3, "maxangle_elast:"); 			//3
			maxangle_Local=Specific_para_adjust( maxangle_munuout,maxangle_munu_addr,maxangle_Local,maxangle_munu_k);
			maxangle_munuout=maxangle_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
	
		}break;
	case 5:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "minangle:");	

			ips114_show_string(8, 16*3, "minangle_elast:"); 			//3
			minangle_Local=Specific_para_adjust( minangle_munuout,minangle_munu_addr,minangle_Local,minangle_munu_k);
			minangle_munuout=minangle_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case


		}break;	
	case 6:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "desiredangle:");	

			ips114_show_string(8, 16*3, "desiredangle:"); 			//3
			desiredangle_Local=Specific_para_adjust( desiredangle_munuout,desiredangle_munu_addr,desiredangle_Local,desiredangle_munu_k);
			desiredangle_munuout=desiredangle_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case


			
		}break;	
	case 7:
		{
	nothing_function(&ok_button);
		}break;
	case 8:
		{
	nothing_function(&ok_button);
		}break;
		case 9:
		{		
			nothing_function(&ok_button);
		}break;
		case 10:
		{
	nothing_function(&ok_button);

		}break;			
	}
//返回回来继续显示
/*优化点*/
 manu_2_angle();


		}
		
		
		if(gpio_get_level(IO_P45)==0)//返回按键，返回主菜单
		{
			Manu_keydelay(IO_P45);

			//优化点，按键延时优化
			
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P45)==0);
		  system_delay_ms(10);*/			
		//	ok_button=1;

			return button_flag;
		}
		if(gpio_get_level(IO_P46)==0)//确认按键，此确认用于进入具体参数调整数值
		{

			Manu_keydelay(IO_P46);

			//优化点，按键延时优化
			/*
			system_delay_ms(10);
			while(gpio_get_level(IO_P46)==0);
		  system_delay_ms(10);*/			
		ok_button=button_flag;
			
		}
		if(gpio_get_level(IO_P36)==0)//按下增加按键，选项下移
		{
			Manu_keydelay(IO_P36);

			//优化点，按键延时优化
		 
		 
		 
		  button_flag++;
			if(button_flag>cursor)
			{
				button_flag=1;
			}
				
		}
		if(gpio_get_level(IO_P37)==0)//按下减小按键，选项上移
		{
			Manu_keydelay(IO_P37);

			//优化点，按键延时优化
			button_flag--;
			if(button_flag==0)
			{
				button_flag=cursor;
			}
		}	
		
		 Manu_show_cursor(button_flag);

		

  }
	
	return button_flag;
}

#if 1

/*
*二级菜单，用于调整全局状态标志位
*包含电量，入环阈值，屏幕显示标志位，运行标志位
*run,show,start_cap,ben_enter
*
*
*
*/




int Manu_two_adjuconfig(void)//调整全局状态变量
{
/*弄几个全局变量，本文件要把eeprom的值传入全局变量
两个字节65535有些变量除掉10，有些除100看需要参数的范围
*/
static uint16 run_Local,show_Local,start_cap_Local,ben_enter_Local,ben_angle_Local,start_distance_Local;

	char button_flag=1,ok_button=0;//初始化为第一行，确认标志范围为1到8
	 manu_3_state();
/*优化点*/
	
	
  while(1)
  {
		if(ok_button!=0)//进入参数改变接口
		{
	switch(ok_button)
	{
	case 1:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "run:"); 			//3
			ips114_show_string(8, 16*3, "run_elast:"); 			//3
			run_Local=Specific_para_adjust( run_munuout,run_munu_addr,run_Local,run_munu_k);
			run_munuout=run_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
		}break;
	case 2:
		{
	
		ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "show:");	
		ips114_show_string(8, 16*3, "show_elast:"); 			//3
		show_Local=Specific_para_adjust( show_munuout,show_munu_addr,show_Local,show_munu_k);
		show_munuout=show_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

		}break;
	case 3:
		{
				//
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "start_cap:");	
		ips114_show_string(8, 16*3, "start_cap_elast:"); 			//3
		start_cap_Local=Specific_para_adjust(start_cap_munuout,start_cap_munu_addr,start_cap_Local,start_cap_munu_k);
		start_cap_munuout=start_cap_Local;
		ok_button=0;//标志位置回零，防止立刻再次进入此case

		}break;
	case 4:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "ben_enter:");	

			ips114_show_string(8, 16*3, "ben_enter_elast:"); 			//3
			ben_enter_Local=Specific_para_adjust( ben_enter_munuout,ben_enter_munu_addr,ben_enter_Local,ben_enter_munu_k);
			ben_enter_munuout=ben_enter_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
	
		}break;
	case 5:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "ben_angle:");	

			ips114_show_string(8, 16*3, "ben_angle_elast:"); 			//3
			ben_angle_Local=Specific_para_adjust( ben_angle_munuout,ben_angle_munu_addr,ben_angle_Local,ben_angle_munu_k);
			ben_angle_munuout=ben_angle_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
		}break;	
	case 6:
		{
			ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "start_distance:");	

			ips114_show_string(8, 16*3, "start_distance_elast:"); 			//3
			start_distance_Local=Specific_para_adjust( start_distance_munuout,start_distance_munu_addr,start_distance_Local,start_distance_munu_k);
			start_distance_munuout=start_distance_Local;
			ok_button=0;//标志位置回零，防止立刻再次进入此case
		}break;	
	case 7:
		{
		nothing_function(&ok_button);
		}break;
	case 8:
		{
	nothing_function(&ok_button);
		}break;
		case 9:
		{		
		nothing_function(&ok_button);
		}break;
		case 10:
		{
	 nothing_function(&ok_button);
		}break;			
	}
//返回回来继续显示
/*优化点*/
  manu_3_state();


		}
		
		
		if(gpio_get_level(IO_P45)==0)//返回按键，返回主菜单
		{
			Manu_keydelay(IO_P45);

			return button_flag;
		}
		if(gpio_get_level(IO_P46)==0)//确认按键，此确认用于进入具体参数调整数值
		{

			Manu_keydelay(IO_P46);		
		ok_button=button_flag;
			
		}
		if(gpio_get_level(IO_P36)==0)//按下增加按键，选项下移
		{
			Manu_keydelay(IO_P36);

			//优化点，按键延时优化
		 
		 
		 
		  button_flag++;
			if(button_flag>cursor)
			{
				button_flag=1;
			}
				
		}
		if(gpio_get_level(IO_P37)==0)//按下减小按键，选项上移
		{
			Manu_keydelay(IO_P37);

			//优化点，按键延时优化
			button_flag--;
			if(button_flag==0)
			{
				button_flag=cursor;
			}
		}	
		
		 Manu_show_cursor(button_flag);

		

  }
	
	return button_flag;
}

#endif






























/*进入具体参数调整界面
*为了增加代码复用性而产生的函数
*用于二级菜单下面的函数
*输入参数1：全局变量
*输入参数2：eeprom的地址数值传入
*输入参数3：二级菜单下的局部变量地址传入

*注意：两个字符串显示函数不包括在此函数内需要在此函数前加对应显示字符串的函数语句
用处是提示此次更改数据的数据名字是什么
例如：	
	ips114_clear(RGB565_BLACK);//先清屏幕
	ips114_show_string(8, 16*0, "kp:"); 			
	ips114_show_string(8, 16*3, "kp_elast:"); 			
*注意：加在此函数后面
	ok_button=0;//标志位置回零，防止立刻再次进入此case

*///uint16

uint16 Specific_para_adjust(uint16 _munuout,uint32 _munu_addr,uint16  _Local,float _mannu_k)
{

	_Local=(iap_read_byte(_munu_addr+1)<<8)|(iap_read_byte(_munu_addr));//读取原本函数，并且显示出来

//	ips114_show_string(8, 16*3, "kp_elast:"); 			//3
	ips114_show_uint16(8, 16*4,(iap_read_byte(_munu_addr+1)<<8)|(iap_read_byte(_munu_addr)));
	if(_munuout!=_Local)
	{
		_Local=_munuout;
	}


	ips114_show_uint16(8, 16*1, _Local);

	//加减原本数据
	while(1)
	{
		if(gpio_get_level(IO_P37)==0)//加一按键
		{
			Manu_keydelay(IO_P37);

			//优化点，按键延时优化
		  _Local++;
			
		}
		 if(gpio_get_level(IO_P36)==0)//减一按键
		{
			Manu_keydelay(IO_P36);

			//优化点，按键延时优化
		  _Local--;
			
		}
		if(gpio_get_level(IO_P46)==0)//确认按键，后续统一一起存入eeprom，然后返回参数设置选择
		{
			Manu_keydelay(IO_P46);

			//优化点，按键延时优化			
		  return _Local;

		}
		if(gpio_get_level(IO_P45)==0)//返回按键，返回上一级菜单函数，
		{
			Manu_keydelay(IO_P45);

			//优化点，按键延时优化	
		//	ok_button=1;
			return _Local;
		}


		
		ips114_show_uint16(8, 16*1, _Local);
		_munuout=_Local;
		ips114_show_uint16(8, 16*2, _munuout);
		ips114_show_float(8,16*5,_munuout*_mannu_k,6,6);

//		ok_button=0;//标志位置回零，防止立刻再次进入此case
	}
}





/*
优化尺寸
*显示光标函数

*/
void Manu_show_cursor(uint8 cursor)
{

	//先清屏幕
	ips114_show_string(0, 16*0, " "); 			//显示字符串
	ips114_show_string(0, 16*1, " "); 			//显示字符串
	ips114_show_string(0, 16*2, " "); 			//显示字符串
	ips114_show_string(0, 16*3, " "); 			//显示字符串
	ips114_show_string(0, 16*4, " "); 			//显示字符串
	ips114_show_string(0, 16*5, " "); 			//显示字符串
	ips114_show_string(0, 16*6, " "); 			//显示字符串
	ips114_show_string(0, 16*7, " "); 			//显示字符串
	ips114_show_string(0, 16*8, " "); 			//显示字符串
	ips114_show_string(0, 16*9, " "); 			//显示字符串

	switch(cursor)//显示光标函数
	{
		case 1:	
		{
				ips114_show_string(0, 16*0, "*"); 			//显示字符串

		}break;
		case 2:
		{
			ips114_show_string(0, 16*1, "*"); 			//显示字符串

		}break;
		case 3:
		{
			ips114_show_string(0, 16*2, "*"); 			//显示字符串

		}break;
		case 4:
		{
			ips114_show_string(0, 16*3, "*"); 			//显示字符串

		}break;
		case 5:
		{
			ips114_show_string(0, 16*4, "*"); 			//显示字符串

		}break;
		case 6:
		{
			ips114_show_string(0, 16*5, "*"); 			//显示字符串
		}break;		
		case 7:
		{
				ips114_show_string(0, 16*6, "*"); 			//显示字符串

		}break;			
		case 8:
		{	
				ips114_show_string(0, 16*7, "*"); 			//显示字符串
				
		}break;			
		case 9:
		{

				ips114_show_string(0, 16*8, "*"); 			//显示字符串
		}break;			
		case 10:
		{
			ips114_show_string(0, 16*9, "*"); 			//显示字符串

		}break;			
		
	}
	
}




/*
按键延时函数优化
*/

void Manu_keydelay(gpio_pin_enum pin)
{
system_delay_ms(10);
while(gpio_get_level(pin)==0);
  system_delay_ms(10);
}

/*
空白页函数优化
*/

void black_manu(void)
{
ips114_clear(RGB565_BLACK);//先清屏幕
			ips114_show_string(8, 16*0, "Nothing");
			ips114_show_string(8, 16*3, "Nothing"); 			//3

			system_delay_ms(1000); 
			ips114_clear(RGB565_BLACK);//先清屏幕

}
/*
回归主菜单，显示屏幕函数组件优化
参数是用于将输入值清零
*/
void main_manu_xianshi(char *qingchu)
{
	*qingchu=0;
	ips114_clear(RGB565_BLACK);//先清屏幕

	ips114_show_string(10, 16*0, "Adjust param"); 			//显示字符串
	ips114_show_string(10, 16*1, "Adjust paramangle"); 			//显示字符串
	ips114_show_string(10, 16*2, "Adjust state"); 			//显示字符串
	ips114_show_string(10, 16*3, "c"); 			//显示字符串
	ips114_show_string(10, 16*4, "d"); 			//显示字符串
	ips114_show_string(10, 16*5, "e"); 			//显示字符串
	ips114_show_string(10, 16*6, "f"); 			//显示字符串
	ips114_show_string(10, 16*7, "g"); 			//显示字符串
}


void Manu_2_show(void)//第一个二级菜单页面显示
{

ips114_clear(RGB565_BLACK);//先清屏幕
ips114_show_string(10, 16*0, "kp:"); 			//
ips114_show_string(10, 16*1, "kp_3:"); 			//
ips114_show_string(10, 16*2, "kd:"); 			//
ips114_show_string(10, 16*3, "max:"); 			//
ips114_show_string(10, 16*4, "min:"); 			//限幅
ips114_show_string(10, 16*5, "xianfumax:"); 			//速度环限幅
ips114_show_string(10, 16*6, "basic"); 			//速度换基础目标值
ips114_show_string(10, 16*7, "kpspeed"); 			//显示字符串
ips114_show_string(10, 16*8, "kdspeed"); 			//速度环kd
ips114_show_string(10, 16*9, "kispeed"); 			//速度环ki

}


/*第二个二级菜单显示*/
void manu_2_angle(void)
{
ips114_clear(RGB565_BLACK);//先清屏幕
ips114_show_string(10, 16*0, "kpangle:"); 			//
ips114_show_string(10, 16*1, "kiangle:"); 			//
ips114_show_string(10, 16*2, "kdangle:"); 			//
ips114_show_string(10, 16*3, "maxangle:"); 			//
ips114_show_string(10, 16*4, "minangle:"); 			//
ips114_show_string(10, 16*5, "desiredangle:"); 		//
ips114_show_string(10, 16*6, " "); 			//
ips114_show_string(10, 16*7, " "); 			//
ips114_show_string(10, 16*8, " "); 			//
ips114_show_string(10, 16*9, " "); 			//run_Local,show_Local,start_cap_Local,ben_enter_Local

}


/*第3个二级菜单显示，全局状态变量*/
void manu_3_state(void)
{
ips114_clear(RGB565_BLACK);//先清屏幕
ips114_show_string(10, 16*0, "run:"); 			//
ips114_show_string(10, 16*1, "show:"); 			//
ips114_show_string(10, 16*2, "start_cap:"); 			//
ips114_show_string(10, 16*3, "ben_enter:"); 			//
ips114_show_string(10, 16*4, "ben_angle"); 			//
ips114_show_string(10, 16*5, "start_distance"); 		//
ips114_show_string(10, 16*6, " "); 			//
ips114_show_string(10, 16*7, " "); 			//
ips114_show_string(10, 16*8, " "); 			//
ips114_show_string(10, 16*9, " "); 			//run_Local,show_Local,start_cap_Local,ben_enter_Local

}






void nothing_function(char *ok_button)
{
	ips114_clear(RGB565_BLACK);//先清屏幕
		ips114_show_string(8, 16*0, "Nothing");
		ips114_show_string(8, 16*3, "Nothing"); 			//3
		*ok_button=0;//标志位置回零，防止立刻再次进入此case
		system_delay_ms(1000); 

}

//初始化将eeprom的数据读出来用于初始化各个参数

void init_read_eepromdata(void)
{
		kp_munuout=(iap_read_byte(kp_munu_addr+1)<<8)|iap_read_byte(kp_munu_addr);
		kp_3_munuout=(iap_read_byte(kp_3_munu_addr+1)<<8)|iap_read_byte(kp_3_munu_addr);
		kd_munuout=(iap_read_byte(kd_munu_addr+1)<<8)|iap_read_byte(kd_munu_addr);
		max_munuout=(iap_read_byte(max_munu_addr+1)<<8)|iap_read_byte(max_munu_addr);
		min_munuout=(iap_read_byte(min_munu_addr+1)<<8)|iap_read_byte(min_munu_addr);
		xianfumax_munuout=(iap_read_byte(xianfumax_munu_addr+1)<<8)|iap_read_byte(xianfumax_munu_addr);
		basic_munuout=(iap_read_byte(basic_munu_addr+1)<<8)|iap_read_byte(basic_munu_addr);
		kpspeed_munuout=(iap_read_byte(kpspeed_munu_addr+1)<<8)|iap_read_byte(kpspeed_munu_addr);
		kdspeed_munuout=(iap_read_byte(kdspeed_munu_addr+1)<<8)|iap_read_byte(kdspeed_munu_addr);
		kispeed_munuout=(iap_read_byte(kispeed_munu_addr+1)<<8)|iap_read_byte(kispeed_munu_addr);

		kpangle_munuout=(iap_read_byte(kpangle_munu_addr+1)<<8)|iap_read_byte(kpangle_munu_addr);
		kiangle_munuout=(iap_read_byte(kiangle_munu_addr+1)<<8)|iap_read_byte(kiangle_munu_addr);
		kdangle_munuout=(iap_read_byte(kdangle_munu_addr+1)<<8)|iap_read_byte(kdangle_munu_addr);
		maxangle_munuout=(iap_read_byte(maxangle_munu_addr+1)<<8)|iap_read_byte(maxangle_munu_addr);
		minangle_munuout=(iap_read_byte(minangle_munu_addr+1)<<8)|iap_read_byte(minangle_munu_addr);
		desiredangle_munuout=(iap_read_byte(desiredangle_munu_addr+1)<<8)|iap_read_byte(desiredangle_munu_addr);

		run_munuout=(iap_read_byte(run_munu_addr+1)<<8)|iap_read_byte(run_munu_addr);
		show_munuout=(iap_read_byte(show_munu_addr+1)<<8)|iap_read_byte(show_munu_addr);
		start_cap_munuout=(iap_read_byte(start_cap_munu_addr+1)<<8)|iap_read_byte(start_cap_munu_addr);
		ben_enter_munuout=(iap_read_byte(ben_enter_munu_addr+1)<<8)|iap_read_byte(ben_enter_munu_addr);
		ben_angle_munuout=(iap_read_byte(ben_angle_munu_addr+1)<<8)|iap_read_byte(ben_angle_munu_addr);
		start_distance_munuout=(iap_read_byte(start_distance_munu_addr+1)<<8)|iap_read_byte(start_distance_munu_addr);
	
}

