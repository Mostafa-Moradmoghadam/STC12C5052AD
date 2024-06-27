#include "Task.h"

void ERR_Task(RunMail_Type runMail,unsigned char *paramsData)
{	/*			
  switch(Task_GetStepParam(paramsData))
  {
   case 0:
		 {
			if( P1&(1<<4))
					{									
					if((P1&(1<<0)) == 0) //HT Falt
							{	
							if(ERR_flag & Heater_ERR_Flag) 
								ERR_flag|=Heater;
							else
								ERR_flag|=Heater_ERR_Flag;
							}
					else
							{
							ERR_flag&=~Heater_ERR_Flag;	
							}
					}
				else
					{
					ERR_flag&=~Heater_ERR_Flag;	
					}					
		if( P1&(1<<3))
					{			
					if((P3&(1<<7)) == 0 && (P3&(1<<5)) == 0) //UV Falt
							{
							if(ERR_flag & UV_ERR_Flag)
								ERR_flag|=UV;
							else
								ERR_flag|=UV_ERR_Flag;
							}
					else
							{
							ERR_flag&=~UV_ERR_Flag;
							}
					if(P3&(1<<5)) //Pump Falt
							{
							if(ERR_flag & Pump_ERR_Flag)
								ERR_flag|=Pump;
							else
								ERR_flag|=Pump_ERR_Flag;
							}
					else
							{
							ERR_flag&=~Pump_ERR_Flag;
							}
					}
		Task_Delay_ms(300,0);			 
			break;	 
		 }   
	}
	*/
}
/*********************************************************************/

void LED_Task(RunMail_Type runMail,unsigned char *paramsData)
{	
			
  switch(Task_GetStepParam(paramsData))
  {
   case 0:
		 {
			TM1637_DisplayDecimal_LED(Life,1);
			if( ERR_flag & 7 )
					{
						if(Speaker_Mute == 0)
								P1|=(1<<7);
						TM1637_DisplayDecimal_Big_Err();
					}
			if(ERR_flag & Heater && ERR_flag > 4 && Duble_ERR == 0)
					{		

							{	
							TM1637_DisplayDecimal_LED(Heater,1);
							TM1637_DisplayDecimal_Small_symbol(Heater);
							Duble_ERR = 1;
							Task_Delay_ms(500,1);
							break;
							}
					}
			if(ERR_flag & UV)
					{
					Speaker_Mute=0;
					TM1637_DisplayDecimal_LED(UV,1);
					TM1637_DisplayDecimal_Small_symbol(UV);
					}
			if(ERR_flag & Pump)
					{
					Speaker_Mute=0;
					TM1637_DisplayDecimal_LED(Pump,1);
					TM1637_DisplayDecimal_Small_symbol(Pump);
					}
			Duble_ERR = 0;
			Task_Delay_ms(500,1);
			break;		 
		 }
   case 1:
		 {
			TM1637_DisplayDecimal_LED(Life,0);
			if(ERR_flag & 7)
					P1&=~(1<<7);			 
			Task_Delay_ms(500,0);
			break;	 
		 }
	}
	
}
/*********************************************************************/
void Read_Temp_Task(RunMail_Type runMail,unsigned char *paramsData)
{
  switch(Task_GetStepParam(paramsData))
  {
   case 0:
		 {
			current_temp=read_temp();
			if(current_temp == -1 && (ERR_flag & 7) == 0)
				TM1637_DisplayDecimal_Big(100);
			else if(	(ERR_flag & 7) == 0)
				TM1637_DisplayDecimal_Big(current_temp);
			Task_Delay_ms(500,0);	
			break;		 
		 }
	}
	
}
/*********************************************************************/
void Control_Task(RunMail_Type runMail,unsigned char *paramsData)
{
if( P1&(1<<4))
					{									
					if((P1&(1<<0)) == 0) //HT Falt
							{	
							if(ERR_flag & Heater_ERR_Flag) 
								ERR_flag|=Heater;
							else
								ERR_flag|=Heater_ERR_Flag;
							}
					else
							{
							ERR_flag&=~Heater_ERR_Flag;	
							}
					}					
		if( P1&(1<<3))
					{			
					if((P3&(1<<7)) == 0 && (P3&(1<<5)) == 0) //UV Falt
							{
							if(ERR_flag & UV_ERR_Flag)
								{ERR_flag|=UV;Speaker_Mute=0;}
							else
								ERR_flag|=UV_ERR_Flag;
							}
					else
							{
							ERR_flag&=~UV_ERR_Flag;
							}
					if(P3&(1<<5)) //Pump Falt
							{
							if(ERR_flag & Pump_ERR_Flag)
								{ERR_flag|=Pump;Speaker_Mute=0;}
							else
								ERR_flag|=Pump_ERR_Flag;
							}
					else
							{
							ERR_flag&=~Pump_ERR_Flag;
							}
					}	
	
	if((P1&(1<<2)) != 0 && stop_start == 1 && (ERR_flag &(UV | Pump)) == 0) //Low Presure
	//if((P1&(1<<2)) != 0 && stop_start == 1) //Low Presure
			{
			if(Delay_ON < 30)	
					Delay_ON++;
			if(Delay_ON == 30)
				{
					P1|=(1<<3);
				}

			}
	else
			{
			P1&=~(1<<3);
			Delay_ON=0;
			}
	P1&=~(1<<4);
  switch(Task_GetStepParam(paramsData))
  {
		
   case 0:
		 {
			if((P1&(1<<2)) != 0 && current_temp > -1 && stop_start == 1 && (ERR_flag & 7) == 0 && (P1&(1<<3)) != 0) 
			{
			if( Heater_PWM < (signed)(set_temp - current_temp )+1 )
				P1|=(1<<4);
			Heater_PWM++;
			if(Heater_PWM > 4)
				Heater_PWM=0;
			}
			Task_Delay_ms(200,0);			 
			break;		 
		}
	}
	
}

/*********************************************************************/
void Menu_Task(RunMail_Type runMail,unsigned char *paramsData)
{
	
  switch(Task_GetStepParam(paramsData))
  {
   case 0:
		 {
			if( (P3&(1<<2))==0)
				counter++;
			else
				counter=0;
			if(counter > 15)
					{
					if(ERR_flag & Heater)
							{
							Speaker_Mute=1;
							P1|=(1<<7);			
							counter=0;
							Task_Delay_ms(500,3);
							}
					if((ERR_flag & 7) == 0 )
							{
							stop_start=0;
							P1|=(1<<7);			
							counter=0;
							rotary_valu=set_temp;
							rotary_flag=1;					
							Task_Delay_ms(1000,1);
							}
					else
							{
							counter=0;	
							Task_Delay_ms(100,0);
							}
					}
			else
				Task_Delay_ms(100,0);
			break;		 
		 }
		 	 			 
		case 1:
		 {
			P1&=~(1<<7); 
			TM1637_DisplayDecimal_Small(rotary_valu);			
			if( (P3&(1<<2))==0)
				counter++;
			else
				counter=0;
			if(counter > 5)
					{
					P1|=(1<<7);
					counter=0;
					rotary_flag =0 ; 
					set_temp=rotary_valu;
					stop_start=1;		
					Task_Delay_ms(1000,3);
					}
			else 
				Task_Delay_ms(300,2);			 
			break;		 
		 }		
		case 2:
		 {
			TM1637_DisplayDecimal_Small_Off();			
			if( (P3&(1<<2))==0)
				counter++;
			else
				counter=0;
			Task_Delay_ms(200,1);			 
			break;		 
		 }			
		case 3:
		{
			P1&=~(1<<7);
		 if( (P3&(1<<2))!=0)
				counter++;
			else
				counter=0;
			if(counter > 3)
				{
					counter=0;
					Delay_ON =29;
					Task_Delay_ms(100,0);	
				}
			else
				Task_Delay_ms(100,3);		
			break;		 
		 }	
		
	}
	
}