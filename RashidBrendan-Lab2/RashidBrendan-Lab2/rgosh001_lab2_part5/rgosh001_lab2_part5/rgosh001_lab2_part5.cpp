/*
 * rgosh001_lab2_part4.c
 *
 * Created: 4/15/2015 2:46:15 PM
 *  Author: Rashid Goshtasbi & Brendan Cheng
 */ 


unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k)
{
	return ((x & (0x01 << k)) != 0);
}


#include <avr/io.h>

int main(void)
{
    while(1)
    {
		unsigned char tempD = PIND;
		unsigned char tempB = PINB;
		unsigned char tempDB = 0x00;
		
		PORTB = 0x00;
		
		if (GetBit(tempD, 6))
		{
			tempDB = SetBit(tempDB, 7, 1);
		}
		if (GetBit(tempD, 5))
		{
			tempDB = SetBit(tempDB, 6, 1);
		}
		if (GetBit(tempD, 4))
		{
			tempDB = SetBit(tempDB, 5, 1);
		}
		if (GetBit(tempD, 3))
		{
			tempDB = SetBit(tempDB, 4, 1);
		}
		if (GetBit(tempD, 2))
		{
			tempDB = SetBit(tempDB, 3, 1);
		}
		if (GetBit(tempD, 1))
		{
			tempDB = SetBit(tempDB, 2, 1);
		}
		if (GetBit(tempD, 0))
		{
			tempDB = SetBit(tempDB, 1, 1);
		}
		if (GetBit(tempB, 0))
		{
			tempDB = SetBit(tempDB, 0, 1);
		}
		
        if(tempDB >= 0x46)
		{
			tempB = SetBit(tempB, 1, 1);
		}
		if ((tempDB > 0x05) && (tempDB < 0x46)) //weight is >5 && <70
		{
			tempB = SetBit(tempB, 1, 0);
			tempB = SetBit(tempB, 2, 1);
		}
		
		PORTB = tempB;
    }
}