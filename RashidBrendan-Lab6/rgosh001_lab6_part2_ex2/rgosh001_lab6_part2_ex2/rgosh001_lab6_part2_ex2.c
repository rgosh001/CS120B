/*
 * rgosh001_lab6_part2_ex1.c
 *
 * Created: 5/12/2015 2:31:22 PM
 *  Author: Rashid Goshtasbi
 */ 


#include <avr/io.h>
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\PMW.c"

unsigned char count = 0;
double arr[] = {261.63, 293.66, 329.63, 344.23, 392.00, 440, 493.88, 523.25};
volatile unsigned char TimerFlag = 0;
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

enum SM_States { init, init_wait, init_back_wait, sound, up_wait, down_wait} SM1_State;

unsigned char button0 = 0x00;
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char total = 0;
unsigned char temp = 0x04;

void PWM_State_machine() {
	button0 = ~PINA & 0x01; //connects PA0 to A0
	button1 = ~PINA & 0x02; //connects PA1 to A1
	button2 = ~PINA & 0x04; //connects PA1 to A2
	
	switch(SM1_State) { // Transitions
		
		case init:
		if (button0 && !button1 && !button2)
		{
			SM1_State = init_wait;
		}
		else if(!button0)
		{
			SM1_State = init;
		}
		break;
		
		case init_wait:
		if (button0 && !button1 && !button2)
		{
			SM1_State = init_wait;
		}
		else if (!button0 && !button1 && !button2)
		{
			SM1_State = sound;
		}
		break;
		
		case init_back_wait:
		if (button0)
		{
			SM1_State = init_back_wait;
		}
		else if (!button0)
		{
			SM1_State = init;
		}
		break;
		
		case sound:
		if (button0)
		{
			SM1_State = init_back_wait;
		}
		else if (button1 && !button2)
		{
			SM1_State = up_wait;
			if(count < 7)
			{
				count++;
			}
		}
		else if (!button1 && button2)
		{
			SM1_State = down_wait;
			if(count > 0)
			{
				count--;
			}
		}
		else if (!button0 && !button1 && !button2)
		{
			SM1_State = sound;
		}
		break;
		
		case up_wait:
		if (button1 && !button2)
		{
			SM1_State = up_wait;
		}
		else if (!button1 && !button2)
		{
			SM1_State = sound;
		}
		break;
		
		case down_wait:
		if (!button1 && button2)
		{
			SM1_State = down_wait;
		}
		else if (!button1 && !button2)
		{
			SM1_State = sound;
		}
		break;
		
		default:
		SM1_State = init;
	} // Transitions

	switch(SM1_State) { // State actions
		// init, init_wait, init_back_wait, sound, up_wait, down_wait
		case init:
		set_PWM(0);
		break;
		
		case init_wait:
		set_PWM(0);
		break;
		
		case init_back_wait:
		set_PWM(0);
		break;
		
		case sound:
		set_PWM(arr[count]);
		break;
		
		case up_wait:
		break;
		
		case down_wait:
		break;

	} // State actions

}



int main(void)
{
	DDRB = 0xFF;
	PORTB = 0xFF;
	
	DDRA = 0x00;
	PORTA = 0xFF;
	
	//TimerSet(2);
	//TimerOn();
	
	SM1_State = init;
	
	PWM_on();
    while(1)
    {
		PWM_State_machine();
		//while(!TimerFlag) {}
		//TimerFlag = 0;
    }
}