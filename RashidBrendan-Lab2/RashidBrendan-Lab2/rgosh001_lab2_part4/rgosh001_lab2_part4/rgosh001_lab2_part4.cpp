/*
 * rgosh001_lab2_part4.c
 *
 * Created: 4/15/2015 2:46:15 PM
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
	DDRB = 0xFF;
	DDRC = 0xFF;
		
	while (1)
	{
		unsigned char count = 0x00;
		unsigned char count2 = 0x00;
		unsigned char temp = 0x00;
		unsigned char tempA = PINA;
		unsigned char tempB = 0x00;
		unsigned char tempC = 0x00;
		
		
		if(GetBit(tempA, 0))
		{
			tempC = SetBit(tempC,4,1);
		}
		if(GetBit(tempA, 1))
		{
			tempC = SetBit(tempC,5,1);
		}
		if(GetBit(tempA, 2))
		{
			tempC = SetBit(tempC,6,1);
		}
		if(GetBit(tempA, 3))
		{
			tempC = SetBit(tempC,7,1);
		}
		
		PORTC = tempC;
		
		
		if(GetBit(tempA, 4))
		{
			tempB = SetBit(tempB,0,1);
		}
		if(GetBit(tempA, 5))
		{
			tempB = SetBit(tempB,1,1);
		}
		if(GetBit(tempA, 6))
		{
			tempB = SetBit(tempB,2,1);
		}
		if(GetBit(tempA, 7))
		{
			tempB = SetBit(tempB,3,1);
		}
		
		PORTB = tempB;
		
	}
}