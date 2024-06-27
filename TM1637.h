#ifndef _TM1637_H
#define _TM1637_H

//#include "STC12C5052AD.h"
#define  Pump    2
#define  Heater  4
#define  UV      1
#define  Life    8

#define  ON    1

unsigned char TM1637_init(void);
unsigned char TM1637_DisplayDecimal_Big(unsigned char number);
unsigned char TM1637_DisplayDecimal_Small(unsigned char number);
void TM1637_DisplayDecimal_Small_Off(void);
void TM1637_DisplayDecimal_Big_Off(void);
unsigned char TM1637_Read_Key(void);
void TM1637_DisplayDecimal_Big_Err(void);
void TM1637_DisplayDecimal_Small_symbol(unsigned int number);
void TM1637_DisplayDecimal_LED(unsigned char number,unsigned char status);
#endif
