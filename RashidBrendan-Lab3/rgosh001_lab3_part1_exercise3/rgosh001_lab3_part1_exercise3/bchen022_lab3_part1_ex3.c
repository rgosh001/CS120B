/*
 * Brendan Cheng (bchen022@ucr.edu)
 * Rashid Goshtasbi (rgosh001@ucr.edu)
 * Section: 022 W 6:10-9:00
 * Assignment: Lab3 Exercise1 Part3
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

enum SM1_States { init, hpressed, hreleased, unlocked, inside } SM1_State;
	
unsigned char led_c = 0x00;
unsigned char button0 = 0x00;
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char button7 = 0x00;
unsigned char led_b = 0x00;

void TickFct_State_machine_1() {
	button0 = ~PINA & 0x01; //connects button0 to A0
	button1 = ~PINA & 0x02; //connects button1 to A1
	button2 = ~PINA & 0x04; //connects button2 to A2
	button7 = ~PINA & 0xF0;
	switch(SM1_State) { // Transitions
		case init:
		if (button2) {
			SM1_State = hpressed;
		}
		else {
			SM1_State = init;
		}
		break;
		
		case hpressed:
		if (!button2) {
			SM1_State = hreleased;
		}
		else if (button2)
		{
			SM1_State = hpressed;
		}
		else {
			SM1_State = init;			
		}
		break;
		
		case hreleased:
		if (button0 || button2) {
			SM1_State = init;
		}
		else if (button1) {
			SM1_State = unlocked;
		}
		else if (!button1) {
			SM1_State = hreleased;
		}
		break;
		
		case unlocked:
		if (!button1) {
			SM1_State = inside;
		}
		else {
			SM1_State = unlocked;
		}
		break;
		
		case inside:
		if (button7) {
			SM1_State = init; 
		}
		else {
			SM1_State = inside;
		}
		break;
		

		default:
		SM1_State = init;
	} // Transitions

	switch(SM1_State) { // State actions
		case init:
		led_c = 0x00;
		PORTC = led_c;
		led_b = 0x00;
		PORTB = led_b;		
		break;
		
		case hpressed:
		led_c = 0x01;
		PORTC = led_c;
		break;
		
		case hreleased:
		led_c = 0x02;
		PORTC = led_c;
		break;
		
		case unlocked:
		led_c = 0x03; ////////////
		PORTC = led_c;
		led_b = 0x01;
		PORTB = led_b;
		break;
		
		case inside:
		led_c = 0x04;
		PORTC = led_c;
		break;
		
		default: // ADD default behaviour below
		break;
	} // State actions

}

int main(void) {

	DDRA = 0x00; //set port A as input
	PORTA = 0xFF; //set to all 1s
	DDRB = 0xFF; //set port B as output
	PORTB = 0x00; //set to all 0s
	DDRC = 0xFF; //set port C as output
	PORTC = 0x00;
	SM1_State = init; //Initial state


	while(1) {
		TickFct_State_machine_1();

	} // while (1)
} // Main