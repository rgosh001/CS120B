/*
 * rgosh001_lab3_part1.cpp
 *
 * Created: 4/21/2015 2:51:01 PM
 *  Author: Rashid Goshtasbi
 */ 


#include <avr/io.h>

int main(void)
{
	//configure port B's 8 pins as ouputs
	DDRB = 0xFF; //setted as outputs
	PORTB = 0x00; //initialized to 0
    while(1)
    {
        //write to port B's 8 pins with 00001111
		PORTB = 0x0F;
    }
}