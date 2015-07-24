/*	Rashid Goshtasbi & rgosh001@ucr.edu:
 *	Brendan Cheng & bchen022@ucr.edu:
 *	Lab Section: 22
 *	Assignment: Lab #3  Exercise #1
 *	Exercise Description: [PART 1: EXERCISE 1]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

enum b_States {b_s0, b_s1} b_State;
	
	unsigned char button = 0x00;
	unsigned char led = 0x00;
	
void button_tick()
{
	button = ~PINA & 0x01;
	
	switch(b_State) {
		// Transitions
		case b_s0:
		if (button) {
			b_State = b_s1;
		}
		else if (!button) {
			b_State = b_s0;
		}
		break;
		
		case b_s1:
		if (button) {
			b_State = b_s0;
		}
		else if (!button) {
			b_State = b_s1;
		}
		break;
		
		default:
		b_State = b_s0;
		break;
	} // Transitions

	switch(b_State) { // State actions
		case b_s0:
		led = 0x01;
		PORTB = led;
		break;
		
		case b_s1:
		led = 0x02;
		PORTB = led;
		break;
		
		default:
		//led = 0x01;
		//PORTB = led;
		break;
	} // State actions
}

int main() {
	DDRA = 0x00; PORTA = 0xFF;// Initialize inputs
	DDRB = 0xFF; PORTB = 0x01;// Initialize outputs
	b_State = b_s0; // Indicates initial call
	while(1) {
		button_tick();
	}
}