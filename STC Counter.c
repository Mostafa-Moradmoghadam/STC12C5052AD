#include <reg51.h>
#include <math.h>
#include <intrins.h>
//#include <stdio.h>
#include "STC12C5052AD.h"
#include "TM1637.h"
#include "MH_OS.h"
#include "Task.h"

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

#define FOSC 16000000L
#define T1MS (65536-FOSC/1000)      //1ms timer calculation method in 1T mode



///////////////////////////////////////////////////////////////
void delay_ms(unsigned int) ;
void InitADC(void);
void InitINT1(void);
unsigned char GetADCResult(unsigned char ch);
char read_temp(void);

///////////////////////////////////////////////////////////////
unsigned char set_temp=30;
unsigned char rotary_valu=0;
unsigned char rotary_flag=0;

///////////////////////////////////////////////////////////////
void tm0_isr()interrupt 1 using 1 
{
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
Task_IncTick();
}
/**************************************************************/
void exint1() interrupt 2           //(location at 0013H)
{
	if(rotary_flag == 1)
			{
			if((P3&(1<<4))==0 && rotary_valu < 40)
				rotary_valu++;
			else if(rotary_valu > 0)
				rotary_valu--;	
			}
}

/**************************************************************/
void main ()
{
int i=0;
//****************** OUTPUT **********************//
	
//****************** SET_OUT **********************//		
P1M1|=(1<<7)|(1<<4)|(1<<3); //BUZZER, Heater, RO_Pump  
//****************** SET_LOW **********************//
P1&=~((1<<7)|(1<<4)|(1<<3));	
//****************** INPUT **********************//
P1M0|=(1<<2)|(1<<0); //Low_pressure, FB_Heater, 
P3M0|=(1<<2)|(1<<5)|(1<<7)|(1<<4); //SW_Rotary,CLK_Rotary,FD_RO_Pump, FB_UV
	
TM1637_init();
InitADC();
InitINT1();
//Set_Left_Right();
	
set_temp = read_temp();
if (set_temp > 50 || set_temp < 10)
	set_temp=0;
TM1637_DisplayDecimal_Small(set_temp);
P1|=(1<<7);
delay_ms(2000);
 P1&=~(1<<7);
Task_RegisterNew(LED_Task);
Task_RegisterNew(Read_Temp_Task);	
Task_RegisterNew(Control_Task);		
Task_RegisterNew(Menu_Task);
Task_RegisterNew(ERR_Task);
Task_RunCoOperativeSystem();
	
while(1);
	{
//	unsigned char i=0;
//	P1|=(1<<7);	
//	delay_ms(10);
//	P1&=~(1<<7);
//		TM1637_DisplayDecimal_Big_Err();
//		TM1637_DisplayDecimal_LED();
//	delay_ms(100);
//  TM1637_DisplayDecimal_Small_symbol(2);while(1);
//	TM1637_DisplayDecimal_Big(read_temp());
//	delay_ms(10);
//	TM1637_DisplayDecimal_Small(rotary_counter);		
	}
}
/**************************************************************/
void Timer0_Int(void) 
{
	AUXR = 0x8e;		//Timer clock is 1T mode
	TMOD = 0x01;		//Set timer work mode
  TL0 = T1MS;                     //initial timer0 low byte
  TH0 = T1MS >> 8;                //initial timer0 high byte
	
	TF0 = 0;		//Clear TF0 flag
	TR0 = 1;		//Timer0 start run
	ET0=1;
	EA=1;	
}
/**************************************************************/

//calculate by stc software
void delay_ms(unsigned int delay)		//@8.000MHz
{
	while(delay--)
		{	
		unsigned char i, j;
			i = 8;
			j = 141;
			do
			{
				while (--j);
			} while (--i);

		}
}


/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC(void)
{        
		P1M1|=(1<<1);//Set P1.1 as Open-Drain mode
		P1M0|=(1<<1);
    ADC_DATA = 0;                   //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    delay_ms(2);                       //ADC power-on and delay
}

/*----------------------------
Get ADC result
----------------------------*/
unsigned char GetADCResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_DATA;                //Return ADC result
}
/*----------------------------
Get ADC result
----------------------------*/
char read_temp(void)
{
	float average = 0;
	float steinhart ;	
	int i=0;
	for (i=0; i< 5; i++) 
		{
    average += GetADCResult(1);
		delay_ms(20);	 
		}	
	
  average /= 5 ;
	if(average >  170  || average < 60) // When the NTC sensor is disconnected or shorted, this function have to return -1
		return -1;												// NTC is 20k ohm for 10C and 3.5K ohm for 50C, So the voltage for this valu is 3.3V and 1.2V
	
  average = 255 / average - 1 ;
  average = 10000.0 / average ; 
  steinhart = average / 10000.0 ;      // (R/Ro)
  steinhart = log(steinhart) ;         // ln(R/Ro)
  steinhart /= 3950.0 ;                // 1/B * ln(R/Ro)
  steinhart += 1.0 / (25.0 + 273.15) ; // + (1/To)
  steinhart = 1.0 / steinhart ;        // Invert
  steinhart -= 273.15 ;                // convert to C
  return (steinhart) ;
}
/*----------------------------
//External interrupt1 service routin
----------------------------*/
void InitINT1(void)
{
    IT1 = 1;                        //set INT1 int type (1:Falling only 0:Low level)
    EX1 = 1;                        //enable INT1 interruptt
    EA = 1;   											//open global interruptt switch
}
/****************************************************************/
