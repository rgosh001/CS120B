/*
 * rgosh001_lab1_part4.c
 *
 * Created: 4/1/2015 6:58:11 PM
 *  Author: Rashid Goshtasbi & Brendan Cheng
 */ 


#include <avr/io.h>

int main(void)

{

	DDRA = 0x00;
	DDRB = 0x00;
	DDRD = 0x00;
	DDRC = 0xFF;
	
	/*unsigned char seata = 0x00;
	unsigned char seatb = 0x00;
	unsigned char seatc = 0x00;*/
	while(1)
	{
		if((PINA + PINB + PIND) > 140)
		{
			PORTC = 0x01;
		}
		if (PINA - PIND > 80)
		{
			PORTC = 0x02;
		}
		if (PIND - PINA > 80)
		{
			PORTC = 0x02;
		}
		if(((PINA + PINB + PIND) > 140) && ((PINA - PIND > 80) || (PIND - PINA > 80)))
		{
			PORTC = 0x03;
		}
		else
		{
			PORTC = 0x00;
		}
	}
	return 0;
}