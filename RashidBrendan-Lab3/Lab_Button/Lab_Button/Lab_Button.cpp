/*
 * Lab_Button.cpp
 *
 * Created: 4/21/2015 3:32:41 PM
 *  Author: Rashid Goshtasbi
 */ 


#include <avr/io.h>

int main(void)
{
	// Configure PORTA as input, initialize to 1s
	// Configure PORTB as outputs, initialize to 0s
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char led = 0x00;
	unsigned char button = 0x00;
	while(1)
	{
		// if PA0 is 1, set PB1PB0=01, else =10
		// 1. Read inputs
		// button is connected to A0
		button = ~PINA & 0x01;
		// 2. Perform Computation
		// True if button is pressed
		if (button) {
			// Sets B to bbbbbb01
			// (clear rightmost 2 bits, then set to 01)
			led = (led & 0xFC) | 0x01;
		}
		else {
			// Sets B to bbbbbb10
			// (clear rightmost 2 bits, then set to 10)
			led = (led & 0xFC) | 0x02;
		}
		// 3. Write output
		PORTB = led;
	}
}