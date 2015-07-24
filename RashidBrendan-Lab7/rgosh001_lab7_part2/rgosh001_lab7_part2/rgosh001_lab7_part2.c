/*
 * Brendan Cheng (bchen022@ucr.edu)
 * Rashid Goshtasbi (rgosh001@ucr.edu)
 * Section: 022 W 6:10-9:00
 * Assignment: Lab3 Exercise1 Part2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/interrupt.h>
#include <stdio.h>
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.c"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\bit.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\timer.h"

char * string = "CS120B is Legend... wait for it DARY! ";
unsigned char count = 0;
unsigned char output[16];

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
//--------End Task scheduler data structure----------------


enum states {init} state;
int tick_funct(state)
{
	for (int j = 0; j < 16; ++j)
	{
		output[j] = string[(count+j) % 38];
	}
	count = (count + 1) % 38;
	LCD_DisplayString(1, output);
	return state;
}

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	
	//Period for the tasks
	//unsigned long int SMTick1_calc = 1000;
	//Calculating GCD
	//DONT NEED TO DO
	//GCD of all tasks
	unsigned long int GCD = 20;
	//GCD periods for scheduler
	//unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick1_period = 20;
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
	LCD_init();
	//LCD_ClearScreen();
	
	unsigned short i;
	while(1)
	{
		// Scheduler code
		for ( i = 0; i < numTasks; i++ )
		{
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period )
			{
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		//tick_funct();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}