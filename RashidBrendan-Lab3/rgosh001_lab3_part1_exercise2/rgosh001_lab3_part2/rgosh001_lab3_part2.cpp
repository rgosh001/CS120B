/*	Rashid Goshtasbi & rgosh001@ucr.edu:
 *	Brendan Cheng & bchen022@ucr.edu:
 *	Lab Section: 22
 *	Assignment: Lab #3  Exercise #2
 *	Exercise Description: [PART 1: EXERCISE 2]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
enum SM1_States { init, wait, inc, dec, reset, inc_wait, dec_wait } SM1_State;

unsigned char led = 7;
unsigned char button0 = 0x00;
unsigned char button1 = 0x00;

void TickFct_State_machine_1() {
	button0 = ~PINA & 0x01; //connects PA0 to A0
	button1 = ~PINA & 0x02; //connects PA1 to A1
	
	switch(SM1_State) { // Transitions


		case init:
		if (1) {
			SM1_State = wait;
		}
		break;
		
		case wait:
		if (button0 && button1) {
			SM1_State = reset;
		}
		else if (button0 && (led < 9)) {
			SM1_State = inc;
		}
		else if (button1 && (led > 0)) {
			SM1_State = dec;
		}
		else {
			SM1_State = wait;
		}
		break;
		
		case inc:
		/*if (1) {
			SM1_State = wait;
		}*/
		if (button0)
		{
			SM1_State = inc_wait;
		}
		else
		{
			SM1_State = wait;
		}
		break;
		
		case inc_wait:
		{
			if (!button0)
			{
				SM1_State = init;
			}
			else
			{
				SM1_State = inc_wait;
			}
		}
		break;
		
		case dec:
		/*if (1) {
			SM1_State = wait;
		}*/
		if (button1)
		{
			SM1_State = dec_wait;
		}
		else
		{
			SM1_State = wait;
		}
		break;
		
		case dec_wait:
		{
			if (!button1)
			{
				SM1_State = init;
			}
			else
			{
				SM1_State = dec_wait;
			}
		}
		break;
		
		
		case reset:
		if (1) {
			SM1_State = wait;
		}
		break;
	
		default:
		SM1_State = init;
	} // Transitions

	switch(SM1_State) { // State actions
		case init:
		PORTC = led;
		break;
		
		case wait:
		led = led;
		PORTC = led;
		break;
		
		case inc:
		led = led + 1;
		PORTC = led;
		break;
		
		case inc_wait:
		PORTC = led;
		break;
		
		case dec:
		led = led - 1;
		PORTC = led;
		break;
		
		case dec_wait:
		PORTC = led;
		break;
		
		case reset:
		led = 0;
		PORTC = led;
		break;
		
		default: // ADD default behaviour below
		break;
	} // State actions

}

int main(void) {

	DDRA = 0x00; //set PORTA as input, initialize to 0
	PORTA = 0xFF;
	
	
	DDRC = 0xFF; //set portc as output, initialize to 0
	PORTC = 7;
	led = 7;
	
	
	SM1_State = init; // Initial state

	while(1) {
		TickFct_State_machine_1();

	} // while (1)
} // Main