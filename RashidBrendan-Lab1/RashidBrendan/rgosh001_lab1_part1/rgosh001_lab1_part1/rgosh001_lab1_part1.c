/*
 * rgosh001_lab1_part1.c
 *
 * Created: 4/1/2015 6:58:11 PM
 *  Author: Rashid Goshtasbi & Brendan Cheng
 */ 


#include <avr/io.h>

int main(void)

{

	DDRA = 0x00;
	DDRB = 0xFF;
	
	while(1)
	{
		if ((PINA & 0x03) == 1)
		{
			{
				PORTB = 0x01;
			}
		}
		else
		{
			PORTB = 0x00;
		}
	}
	return 0;
}