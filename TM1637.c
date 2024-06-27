
#include "TM1637.h"
#include "STC12C5052AD.h"
#include <reg51.h>



const unsigned char segmentMap[4][17] = {
	  {
    0xFA, 0x22, 0x7C, 0x6E, 0xA6, 0xCE, 0xDE, 0x62, // 0-7
    0xFE, 0xEE, 0x5F, 0x73, 0x36, 0x79, 0x73, 0x17, // 8-9, A-F	
		0x00 
		},
	  {
    0xD7, 0x84, 0xE3, 0xE6, 0xB4, 0x76, 0x77, 0xC4, // 0-7
    0xF7, 0xF6, 0x5F, 0x73, 0x36, 0x79, 0x73, 0x17, // 8-9, A-F	
		0x00 
		},
	  {
    0xE7, 0x21, 0xCB, 0x6B, 0x2D, 0x6E, 0xEE, 0x23, // 0-7
    0xEF, 0x6F, 0x5F, 0x73, 0x36, 0x79, 0x73, 0x17, // 8-9, A-F	
		0x00 
		},
	  {
    0x7E, 0x48, 0x3D, 0x6D, 0x4B, 0x67, 0x77, 0x4C, // 0-7
    0x7f, 0x6f, 0x5F, 0x73, 0x36, 0x79, 0x73, 0x17, // 8-9, A-F	
		0x00 
		},
		
};
unsigned char LED_Status=0;
/***********************************/
void delay_TM1637(unsigned int delay)
{
	int i=0;
while(delay--)for( i=0;i<1;i++);
}
/***********************************/
void TM1637_dio_input(void)
{
P1M1&=~(1<<6);	
P1M0|=(1<<6);	
// data_dir &= ~(1<<sda);
}

/***********************************/
void TM1637_dio_output(void)
{
P1M1|=(1<<6);
P1M0&=~(1<<6);
//data_dir |= (1<<sda);
}
/***********************************/

unsigned char TM1637_dio_value(void)
{
delay_TM1637(100);
//return HAL_GPIO_ReadPin(GPIO_DIO[num],PIN_DIO[num]);
return	(P1&(1<<6))?1:0;	
 
}
/***********************************/
void TM1637_clk_high (void)
{
//HAL_GPIO_WritePin(GPIO_CLK[num],PIN_CLK[num],GPIO_PIN_SET);
P1|=(1<<5);	
delay_TM1637(100);
}

/***********************************/
void TM1637_clk_low (void)
{
P1&=~(1<<5);	
//HAL_GPIO_WritePin(GPIO_CLK[num],PIN_CLK[num],GPIO_PIN_RESET);
	
delay_TM1637(100);
}

/***********************************/
void TM1637_dio_high(void)
{
//HAL_GPIO_WritePin(GPIO_DIO[num],PIN_DIO[num],GPIO_PIN_SET);
P1|=(1<<6);	
delay_TM1637(100);
}
/***********************************/
void TM1637_dio_low(void)
{
//HAL_GPIO_WritePin(GPIO_DIO[num],PIN_DIO[num],GPIO_PIN_RESET);
P1&=~(1<<6);		
delay_TM1637(100);
}
/***********************************/

void TM1637_i2c_tx_bit_1 (void)
{
	TM1637_dio_output();
  TM1637_clk_low();
  TM1637_dio_high();
  TM1637_clk_high();
  TM1637_clk_low();
}

/***********************************/
void TM1637_i2c_tx_bit_0 (void)
{
	TM1637_dio_output();
  TM1637_clk_low();
  TM1637_dio_low();
  TM1637_clk_high();
  TM1637_clk_low();
}

/***********************************/
unsigned char TM1637_i2c_rx_bit(void)
{
  unsigned char i;
  TM1637_dio_input();	
  TM1637_clk_high();
	i=TM1637_dio_value();

	TM1637_clk_low(); 
  return i;
}
/***********************************/

//--------------------------------------------------------------------------------


void TM1637_i2c_start(void)
{
	TM1637_dio_output();
	TM1637_clk_high();
  TM1637_dio_high();
	TM1637_dio_low();
	delay_TM1637(100);
  TM1637_clk_low();  
}



void TM1637_i2c_stop(void)
{
	TM1637_dio_output();	
  TM1637_dio_low();
	TM1637_clk_low();
	TM1637_clk_high();
  TM1637_dio_high();  
}
/***********************************/

unsigned char TM1637_i2c_write(unsigned char Data)
{
  unsigned char i;


  for(i=0;i<8;i++)
    {

      if(Data & 0x01)
        TM1637_i2c_tx_bit_1();
      else
        TM1637_i2c_tx_bit_0();

      Data >>=1;
    }

//		i2c_tx_bit_0(); 
//		  sda_high();
//		delay();
	//	 
 return TM1637_i2c_rx_bit();
}

/***********************************/
unsigned char TM1637_i2c_read(void)
{
  unsigned char i;
  unsigned char Data=0;
	//delay();
  for(i=0;i<8;i++)
    {
      if(TM1637_i2c_rx_bit())
        Data |= 1 << i;
    }
TM1637_i2c_rx_bit();

  return Data;
}
/***********************************/
unsigned char TM1637_init(void)
{
//TM1637_i2c_start();
//TM1637_i2c_write(0x8f);
//TM1637_i2c_stop();	
//delay(100);
TM1637_i2c_start();	
TM1637_i2c_write(0x40);
TM1637_i2c_stop();	
delay_TM1637(100);
TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	
TM1637_i2c_write(segmentMap[0][0]);	
TM1637_i2c_write(segmentMap[1][0]);	
TM1637_i2c_write(segmentMap[2][0]);	
TM1637_i2c_write(segmentMap[3][0]);	
TM1637_i2c_write(segmentMap[4][0]);
TM1637_i2c_write(0);
	
TM1637_i2c_stop();	
delay_TM1637(100);
TM1637_i2c_start();
TM1637_i2c_write(0x8f);
TM1637_i2c_stop();
return 0;
}
/***********************************/
void TM1637_DisplayDecimal_Small_symbol(unsigned int number)
{

TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	
if(number == Pump)
		{
		TM1637_i2c_write(0x1A);
		TM1637_i2c_write(0xF1);
		}
else if(number == Heater)
		{
		TM1637_i2c_write(0x14);
		TM1637_i2c_write(0xB5);
		}
else if(number == UV)
		{
		TM1637_i2c_write(0x1A);
		TM1637_i2c_write(0x97);
		}		
TM1637_i2c_stop();
return ;	
}
/***********************************/
unsigned char TM1637_DisplayDecimal_Small(unsigned char number)
{
	unsigned char i[2]={0},j=0;
if( number > 99 )
	return 0;

i[0]=number%10;number/=10;
i[1]=number;


TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	
TM1637_i2c_write(segmentMap[0][i[0]]);
TM1637_i2c_write(segmentMap[1][i[1]]);
TM1637_i2c_stop();
return 1;	
}

/***********************************/
unsigned char TM1637_DisplayDecimal_Big(unsigned char number)
{
	unsigned char i[2]={0},j=0;
if( number >99 )
		{
			TM1637_i2c_start();	
			TM1637_i2c_write(0xC2);		
			TM1637_i2c_write(0xD4);//L
			TM1637_i2c_write(0xFE); /// O
			TM1637_i2c_stop();
			return 0;
		}
i[0]=number%10;number/=10;
i[1]=number;



TM1637_i2c_start();	
TM1637_i2c_write(0xC2);	

TM1637_i2c_write(segmentMap[2][i[0]]);
TM1637_i2c_write(segmentMap[3][i[1]]);

TM1637_i2c_stop();
return 1;	
}
/***********************************/
void TM1637_DisplayDecimal_LED(unsigned char number,unsigned char status)
{
TM1637_i2c_start();	
TM1637_i2c_write(0xC5);	
if(number == Pump)
		{
		if(status == ON)
			LED_Status|= 2 ;
		else
			LED_Status&= ~(1) ;
		}
else if(number == Heater)
		{
		if(status == ON)
			LED_Status|= 4 ;
		else
			LED_Status&= ~(4) ;
		}
else if(number == UV)
		{
		if(status == ON)
			LED_Status|= 1 ;
		else
			LED_Status&= ~(1) ;
		}	
else if(number == Life)
		{
		if(status == ON)
			LED_Status|= 8 ;
		else
			LED_Status&= ~(8) ;		
		}
TM1637_i2c_write(LED_Status);		
TM1637_i2c_stop();
return ;	
}
/***********************************/
void TM1637_DisplayDecimal_Big_Err(void)
{


TM1637_i2c_start();	
TM1637_i2c_write(0xC2);	

TM1637_i2c_write(0x88);
TM1637_i2c_write(0x37);

TM1637_i2c_stop();
return ;	
}
///***********************************/
//unsigned char TM1637_Read_Key(void)
//{
//unsigned char key_value=0;
//TM1637_i2c_start();	
//TM1637_i2c_write(0X42);	
//key_value = TM1637_i2c_read();
//TM1637_i2c_stop();
//return key_value;	
//}
/***********************************/
void TM1637_DisplayDecimal_Small_Off(void)
{

TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	
TM1637_i2c_write(0);
TM1637_i2c_write(0);
TM1637_i2c_stop();
}
///***********************************/
//void TM1637_DisplayDecimal_Big_Off(void)
//{

//TM1637_i2c_start();	
//TM1637_i2c_write(0xC2);	
//TM1637_i2c_write(0);
//TM1637_i2c_write(0);
//TM1637_i2c_write(0);
//TM1637_i2c_stop();
//}