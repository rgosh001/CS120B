/*
 * rgosh001_lab1_part3.c
 *
 * Created: 4/1/2015 6:58:11 PM
 *  Author: Rashid Goshtasbi & Brendan Cheng
 */ 


#include <avr/io.h>

int main(void)

{

	DDRA = 0x00;
	DDRC = 0xFF;
	
	unsigned char inc = 0x04;
	while(1)
	{
		inc = 0x04;
		if (PINA & 0x01)
		{
			inc--;
		}
		if (PINA & 0x02)
		{ 
			inc--;
		}
		if (PINA & 0x04)
		{
			inc--;
		}
		if (PINA & 0x08)
		{
			inc--;
		}
		if (inc == 0x00)
		{
			PORTC = (0x40 | inc);
			//PORTC = 0x40;
		}
		else
		{
			PORTC = inc;
		}
	}
	return 0;
}