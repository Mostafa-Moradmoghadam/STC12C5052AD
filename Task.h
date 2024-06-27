
#ifndef __TASK_H
#define __TASK_H

#include "MH_OS.h"
#include "TM1637.h"
#include <reg51.h>

#define  Pump_ERR_Flag    16
#define  Heater_ERR_Flag  32
#define  UV_ERR_Flag      64


static unsigned char counter=0,stop_start = 1,ERR_flag = 0,Heater_PWM=0,Speaker_Mute=0,Duble_ERR=0,Delay_ON=29;
static char current_temp=0;

extern unsigned char set_temp;
extern Task_t     AvailableTask[];
extern unsigned char rotary_flag,rotary_valu;

void LED_Task(RunMail_Type runMail,unsigned char *paramsData);
void Read_Temp_Task(RunMail_Type runMail,unsigned char *paramsData);
void Control_Task(RunMail_Type runMail,unsigned char *paramsData);
void Menu_Task(RunMail_Type runMail,unsigned char *paramsData);
void ERR_Task(RunMail_Type runMail,unsigned char *paramsData);
extern char read_temp(void);

#endif
