#ifndef __MANU_H
#define __MANU_H
#include "zf_common_headfile.h"
//以下均是eeprom的地址，数字偶数位为高位，技术位置为低位，掉电不丢失
#define kp_munu_addr             (0x00000000)//转向环，kp
#define kp_3_munu_addr           (0x00000002)//转向环，kp的三次方
#define kd_munu_addr             (0x00000004)//转向环，kd
#define max_munu_addr            (0x00000006)//转向环最大值
#define min_munu_addr            (0x00000008)//转向环最小值
#define xianfumax_munu_addr      (0x0000000a) //速度环限幅
#define basic_munu_addr          (0x0000000c)//速度环基本值
#define kpspeed_munu_addr        (0x0000000e)//速度环kp
#define kdspeed_munu_addr        (0x00000010)//速度环最值占空比
#define kispeed_munu_addr        (0x00000012)//速度环最值占空比

#define kpangle_munu_addr              (0x00000014)//角速度环p
#define kdangle_munu_addr              (0x00000016)//d
#define kiangle_munu_addr              (0x00000018)//i
#define maxangle_munu_addr             (0x0000001a)//最大值
#define minangle_munu_addr             (0x0000001c)//最小值
#define desiredangle_munu_addr         (0x0000001e)//期望

#define run_munu_addr         (0x00000020)//期望
#define show_munu_addr         (0x00000022)//期望
#define start_cap_munu_addr         (0x00000024)//期望
#define ben_enter_munu_addr         (0x00000026)//期望
#define	ben_angle_munu_addr								(0x00000028)//
#define	start_distance_munu_addr					(0x0000002a)//


// run_Local,show_Local,start_cap_Local,ben_enter_Local;






#define Erase_EEPROM        (0x00000000)//擦除第一页，用于确认更新数据时擦除eeprom的的地址
/*更改值系数，最小值*/
/*
#define kp_munu_k             (0.1)//转向环，kp
#define kp_3_munu_k           (0.1)//转向环，kp的三次方
#define kd_munu_k             (0.1)//转向环，kd
#define max_munu_k            (0.1)//转向环最大值
#define min_munu_k            (0.1)//转向环最小值
#define xianfumax_munu_k      (0.1) //速度环限幅
#define basic_munu_k          (0.1)//速度环基本值
#define kpspeed_munu_k        (0.1)//速度环kp
#define kdspeed_munu_k        (0.1)//速度环最值占空比
#define kispeed_munu_k        (0.1)//速度环最值占空比

#define kpangle_munu_k              (0.1)//角速度环p
#define kdangle_munu_k              (0.1)//d
#define kiangle_munu_k              (0.1)//i
#define maxangle_munu_k             (0.1)//最大值
#define minangle_munu_k             (0.1)//最小值
#define desiredangle_munu_k         (0.1)//期望

*/

// PidInit(&Pid_Speed_l, 120.0, 40.0, 0.5, 0.0, 2000.0, -2000.0);
	// PidInit(&Pid_Speed_r, 120.0, 40.0, 0.5, 0.0, 2000.0, -2000.0);
	// //速度环
	// PidInit(&gyro_loop,   0.0, 10.0, 0.0, 0.0, 5000.0, -5000.0);	//角速度环
	// Pid_Turn_Init(&Pid_Turn, 0.0, 0.0000, 5.0, 0.2, 0.0,1000.0, -1000.0);	//转向环

#define kp_munu_k             (0.1)//转向环，kp
#define kp_3_munu_k           (0.00001)//转向环，kp的三次方
#define kd_munu_k             (0.01)//转向环，kd
#define max_munu_k            (100.0)//转向环最大值
#define min_munu_k            (100.0)//转向环最小值
#define xianfumax_munu_k      (500) //速度环限幅
#define basic_munu_k          (1.0)//速度环基本值
#define kpspeed_munu_k        (1.0)//速度环kp
#define kdspeed_munu_k        (0.1)//速度环d
#define kispeed_munu_k        (0.01)//速度环i

#define kpangle_munu_k              (0.01)//角速度环p
#define kdangle_munu_k              (0.01)//d
#define kiangle_munu_k              (0.00002)//i
#define maxangle_munu_k             (1000)//最大值
#define minangle_munu_k             (1000)//最小值
#define desiredangle_munu_k         (0.1)//期望

#define run_munu_k         (1)//运行状态
#define show_munu_k         (1)//屏幕显示状态
#define start_cap_munu_k         (0.1)//开始电量
#define ben_enter_munu_k         (1)//苯环阈值
#define ben_angle_munu_k         (10)//苯环阈值
#define start_distance_munu_k         (100)


















/*菜单传出全局变量，用于输出给转向环速度环初始化*/
extern uint16 kp_munuout,kp_3_munuout,kd_munuout,max_munuout,min_munuout,xianfumax_munuout,basic_munuout,kpspeed_munuout,kdspeed_munuout,kispeed_munuout;
extern uint16 kpangle_munuout,kdangle_munuout,kiangle_munuout,maxangle_munuout,minangle_munuout,desiredangle_munuout;//角速度环
extern uint16 run_munuout,show_munuout,start_cap_munuout,ben_enter_munuout,ben_angle_munuout,start_distance_munuout;//全局状态变量


int Manu_one(void);//
int Manu_two_adjust(void);//
//int Manu_two_(void);//待加入选项
uint16 Specific_para_adjust(uint16 _munuout,uint32 _munu_addr,uint16  _Local,float _mannu_k);

int Manu_two_adjustangle(void);//角速度环参数调整函数
int Manu_two_adjuconfig(void);//全局状态量

void Manu_show_cursor(uint8 cursor);//显示光标函数
void Manu_keydelay(gpio_pin_enum pin);
void black_manu(void);
void main_manu_xianshi(char *qingchu);
void init_read_eepromdata(void);
void Manu_2_show(void);//第一个二级菜单页面显示
void manu_2_angle(void);
void manu_3_state(void);

void nothing_function(char *ok_button);//未定义界面



#endif


