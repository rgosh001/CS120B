/*
 * rgosh001_lab2_part2.c
 *
 * Created: 4/8/2015 7:46:15 PM
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
	DDRC = 0xFF;
	unsigned char tmp = 0x00;
	
	while(1)
	{
		PORTC = 0x00;
		tmp = (PINA & 0x0F);
		if(tmp == 1 || tmp == 2)
		{
			PORTC = 0x20;
		}
		else if(tmp == 3 || tmp == 4)
		{
			PORTC = 0x30;
		}
		else if(tmp == 5 || tmp == 6)
		{
			PORTC = 0x38;
		}
		else if(tmp >= 7 && tmp <= 9)
		{
			PORTC = 0x1C;
		}
		else if(tmp >= 10 && tmp <= 12)
		{
			PORTC = 0x3E;
		}
		else if(tmp >= 13 && tmp <= 15)
		{
			PORTC = 0x3F;
		}
		
		
		unsigned char tmp2 = PORTC;
		tmp = PINA;
		if(GetBit(tmp, 5))
		{
			if (GetBit(tmp, 4))
			{
				if (!(GetBit(tmp, 6)))
				{
					tmp2 = SetBit(tmp2, 7, 1);
					PORTC = tmp2;
				}
			}
		}
		
		unsigned char temp3 = (PINA & 0x0F);
		if(temp3 <= 4)
		{
			PORTC = (PORTC | 0x40);
		}
	}
	
	return 0;
}