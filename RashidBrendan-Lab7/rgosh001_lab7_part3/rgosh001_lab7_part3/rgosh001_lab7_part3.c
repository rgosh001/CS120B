/*
 * Brendan Cheng (bchen022@ucr.edu)
 * Rashid Goshtasbi (rgosh001@ucr.edu)
 * Section: 022 W 6:10-9:00
 * Assignment: Lab3 Exercise1 Part2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <ucr/bit.h>
//#include <ucr/timer.h>
#include <stdio.h>
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.c"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\bit.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\timer.h"

unsigned char x = 0x1F;
unsigned char get_value;
unsigned char temp = 0;
unsigned char temp2 = 0;

//--------Task scheduler data structure--------------------
// Struct for Tasks represent a running process in our
// simple real-time operating system.
/*Tasks should have members that include: state, period, a
measurement of elapsed time, and a function pointer.*/
typedef struct _task {
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	//Task tick function
	int (*TickFct)(int);
} task;

// Returns '\0' if no key pressed,
// Else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement:

        PC4 PC5 PC6 PC7
    col  1   2   3   4
  row
PC0 1    1 | 2 | 3 | A
PC1 2    4 | 5 | 6 | B
PC2 3    7 | 8 | 9 | C
PC3 4    * | 0 | # | D
*/
unsigned char GetKeypadKey() {
	// Check keys in col 1
	// Enable col PC4 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }
	
	// Check keys in col 2
	// Enable col PC5 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }
	
	// ... *****FINISH*****
	// Check keys in col 3
	// Enable col PC6 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }
	
	// ... *****FINISH*****
	// Check keys in col 4
	// Enable col PC7 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }
	// ... *****FINISH*****
	return('\0'); // default value
}

enum states {init, get, off} state;
int tick_funct(int state)
{
	switch (state)
	{
		case init:
		if (x == 0x1F)
		{
			state = init;
		}
		else if (x != 0x1F)
		{
			state = get;
			//temp = x;
		}
		break;
		
		case get:
		if (x != 0x1F)
		{
			state = get;
			//temp = GetKeypadKey();
		}
		else if (x == 0x1F)
		{
			state = off;
		}
		break;
		
		case off:
		state = init;
		break;
		
		default:
		state = init;
		x = 0x1F;
		break;
	}
	switch (state)
	{
		case init:
		x = GetKeypadKey();
		break;
		
		case get:
		x = GetKeypadKey();
		temp = GetKeypadKey();
		break;
		
		case off:
		temp2 = temp;
		break;
		
		default:
		break;
	}

	
	return state;
}

int main(void)
{
	// PORTB set to output, outputs init 0s
	DDRB = 0xFF; PORTB = 0x00;
	// PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRC = 0xF0; PORTC = 0x0F;
	
	//Period for the tasks
	unsigned long int SMTick1_calc = 50;
	
	//Calculating GCD
	//DONT NEED TO DO
	
	//GCD of all tasks
	unsigned long int GCD = 50;
	
	//GCD periods for scheduler
	//unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick1_period = 50;
	
	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1 -- ONLY TASK
	task1.state = -1;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &tick_funct;
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	unsigned char total = 0;
	//LCD_WriteData(total + '0');
	
	unsigned short i;
	while(1)
	{ 	
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime ==
			tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state =
				tasks[i]->TickFct(tasks[i]->state);
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
		//tick_funct();
		switch (temp)
		{
			LCD_ClearScreen();
			// All 5 LEDs on
			case '\0': PORTB = 0x1F; break;
			// hex equivalent
			case '1': PORTB = 0x01; total = 1; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '2': PORTB = 0x02; total = 2; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '3': PORTB = 0x03; total = 3; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '4': PORTB = 0x04; total = 4; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '5': PORTB = 0x05; total = 5; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '6': PORTB = 0x06; total = 6; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '7': PORTB = 0x07; total = 7; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '8': PORTB = 0x08; total = 8; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '9': PORTB = 0x09; total = 9; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '0': PORTB = 0x00; total = 0; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			// . . . ***** FINISH *****
			case 'A': PORTB = 0x0A; total = 65; LCD_Cursor(1); LCD_WriteData(total); break;
			case 'B': PORTB = 0x0B; total = 66; LCD_Cursor(1); LCD_WriteData(total); break;
			case 'C': PORTB = 0x0C; total = 67; LCD_Cursor(1); LCD_WriteData(total); break;
			case 'D': PORTB = 0x0D; total = 68; LCD_Cursor(1); LCD_WriteData(total); break;
			case '*': PORTB = 0x0E; total = 42; LCD_Cursor(1); LCD_WriteData(total); break;
			case '#': PORTB = 0x0F; total = 35; LCD_Cursor(1); LCD_WriteData(total); break;
			// Should never occur. Middle LED off.
			default: PORTB = 0x1B; break;
		

			while (!TimerFlag);
			TimerFlag = 0;
		}
	}
}