/*	Rashid Goshtasbi & rgosh001@ucr.edu:
 *	Brendan Cheng & bchen022@ucr.edu:
 *	Lab Section: 22
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned char TimerFlag = 0;
unsigned char ThreeLED_value = 0x00;
unsigned char BlinkingLED_value = 0x00;
unsigned char speaker_var = 0x00;

void TimerISR()
{
	TimerFlag = 1;
}

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

enum sounds {sound_off, sound_on} sound;
void speaker()
{
	unsigned char button = ~PINA & 0x04; //connects PA0 to A0
	switch(sound)
	{
		case sound_off:
		if(button)
		{
			sound = sound_on;
		}
		else
		{
			sound = sound_off;
		}
		break;
		
		case sound_on:
		if(1)
		{
			sound = sound_off;
		}
		break;
		
		default:
		sound = sound_off;
		break;
	}
	
	switch (sound)
	{
		case sound_off:
		speaker_var = 0x00;
		break;
		
		case sound_on:
		speaker_var = 0x10;
		break;
		
	}
}

enum three_states {led_b0, led_b1, led_b2} three_state;
void ThreeLEDs()
{
	switch (three_state)
	{
		case led_b0:
		if(1)
		{
			three_state = led_b1;
		}
		break;
		
		case led_b1:
		if(1)
		{
			three_state = led_b2;
		}
		break;
		
		case led_b2:
		if(1)
		{
			three_state = led_b0;
		}
		break;
		
		default:
		three_state = led_b0;
	}
	
	switch(three_state)
	{
		case led_b0:
		ThreeLED_value = 0x01;
		break;
		
		case led_b1:
		ThreeLED_value = 0x02;
		break;
		
		case led_b2:
		ThreeLED_value = 0x04;
		break;
		
		default:
		break;
	}
}

enum blinking_states {blink_on, blink_off} blinking_state;
void BlinkingLEDs()
{
	switch (blinking_state)
	{
		case blink_on:
		if(1)
		{
			blinking_state = blink_off;
		}
		break;
		
		case blink_off:
		if(1)
		{
			blinking_state = blink_on;
		}
		break;
		
		default:
		blinking_state = blink_off;
	}
	
	switch(blinking_state)
	{
		case blink_on:
		BlinkingLED_value = 0x08;
		break;
		
		case blink_off:
		BlinkingLED_value = 0x00;
		break;
		
		default:
		break;
	}
}

enum combine_states {repeat} combine_state;
void CombineLEDs()
{
	switch (combine_state)
	{
		case repeat:
		if(1)
		{
			combine_state = repeat;
		}
		break;
		
		default:
		combine_state = repeat;
		break;
	}
	
	switch (combine_state)
	{
		case repeat:
		//unsigned char temp;
		//temp = (ThreeLED_value & BlinkingLED_value);
		PORTB = (ThreeLED_value | BlinkingLED_value | speaker_var);
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(2);
	TimerOn();
	three_state = led_b0;
	blinking_state = blink_on;
	combine_state = repeat;
	
	unsigned char three_count = 0;
	int blink_count = 0;
	
    while(1)
    {
		three_count++;
		blink_count++;
		speaker();
		if(blink_count == 500)
		{
			BlinkingLEDs();
			blink_count = 0;
		}
		if(three_count == 150)
		{
			ThreeLEDs();
			three_count = 0;
		}
		CombineLEDs();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}