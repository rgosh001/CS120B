/*
 * rgosh001_lab2_part1.c
 *
 * Created: 4/8/2015 6:58:11 PM
 *  Author: Rashid Goshtasbi & Brendan Cheng
 */ 


#include <avr/io.h>
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k)
{
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0xFF;
	unsigned char tmp = 0x00;
	
	while(1)
	{
		unsigned char counter = 0x00;
		unsigned char count;
		for(count = 0; count < 8; count++)
		{
			tmp = PINA;	
			if(GetBit(tmp, count))
			{
				counter++;
			}
			
			tmp = PINB;
			if (GetBit(tmp, count))
			{
				counter++;
			}
		}
		
		tmp = PINB;
		PORTC = counter;
	}
	
	return 0;
}