/*	Rashid Goshtasbi & rgosh001@ucr.edu:
 *	Brendan Cheng & bchen022@ucr.edu:
 *	Lab Section: 22
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [PART 1: EXERCISE 1]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.c"

// TimerISR() sets this to 1. C programmer should clear to 0.
volatile unsigned char TimerFlag = 0;
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s
	TCCR1B = 0x0B;
	// AVR output compare register OCR1A.
	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	OCR1A = 125;
	// AVR timer interrupt mask register
	// bit1: OCIE1A -- enables compare match interrupt
	TIMSK1 = 0x02;
	//Initialize avr counter
	TCNT1=0;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;
	//Enable global interrupts: 0x80: 1000000
	SREG |= 0x80;
}
void TimerOff() {
	// bit3bit1bit0=000: timer off
	TCCR1B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1
	// (every 1 ms per TimerOn settings)
	// Count down to 0 rather than up to TOP (results in a more efficient comparison)
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		// Call the ISR that the user uses
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum SM_States { init, stay, inc, dec } SM1_State;

unsigned char button0 = 0x00;
unsigned char button1 = 0x00;
unsigned char total = 0;

void TickFct_State_machine_1() {
	button0 = ~PINA & 0x01; //connects PA0 to A0
	button1 = ~PINA & 0x02; //connects PA1 to A1
	
	switch(SM1_State) { // Transitions
		
		case init:
		if (1) {
			SM1_State = stay;
		}
		break;
		
		case stay:
		if (button0 && button1) {
			SM1_State = init;
		}
		else if (button0 && !button1) {
			SM1_State = inc;
			if (total < 9)
			{
				total = total + 1;
			}
		}
		else if (!button0 && button1) {
			SM1_State = dec;
			if (total > 0)
			{
				total = total - 1;
			}
		}
		else {
			SM1_State = stay;
		}
		break;
		
		case inc:
		if (button0 && !button1)
		{
			SM1_State = inc;
			if (total < 9)
			{
				total = total + 1;
			}
		}
		else if (!button0 && !button1)
		{
			SM1_State = stay;
		}
		break;
		
		case dec:
		if (!button0 && button1)
		{
			SM1_State = dec;
			if (total > 0)
			{
				total = total - 1;
			}
		}
		else if (!button0 && !button1)
		{
			SM1_State = stay;
		}
		break;
	
		default:
		SM1_State = init;
	} // Transitions

	switch(SM1_State) { // State actions
		case init:
		total = 0;
		LCD_ClearScreen();
		break;
		
		case stay:
		LCD_Cursor(1);
		LCD_WriteData(total + '0');
		break;
		
		case inc:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(total + '0');
		break;
		
		case dec:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(total + '0');
		break;

		default: // ADD default behaviour below
		break;
	} // State actions

}

int main(void) {

	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(1000);
	TimerOn();
	
	SM1_State = init; // Initial state
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	while(1) {
		TickFct_State_machine_1();
		while(!TimerFlag);
		TimerFlag = 0;

	} // while (1)
} // Main