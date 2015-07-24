/*
 * GccApplication1.c
 *
 * Created: 5/26/2015 1:07:54 PM
 *  Author: Rashid Goshtasbi
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.c"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\bit.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\timer.h"
unsigned char flash = 0;
unsigned char highscore_total = 0;

unsigned char start_l1 = 0;
unsigned char end_l1 = 0;
unsigned char choose_level_gl = 0;
unsigned char level = 1;
unsigned char highscore_gl = 0;
unsigned char * init_string = "Simon Says Hi!  Press Any Key!";
unsigned char * menu_string = "1.Start/2.Choose3.Scores///Level";
unsigned char * start_string = "Begin Game!";
unsigned char * next_level_string = "Good Job!       Next Level!";
unsigned char * wrong_string = "Sorry, Incorrect";
unsigned char * level1_continue_string = "Enter Key Press Now";
unsigned char * choose_level_string = "Enter Level On 7-Seg Disp.";

//DDRB = 0xFF; PORTB = 0x0F; TOP LEFT
//DDRC = 0xFF; PORTC = 0x0F;
	
//DDRB = 0xFF; PORTB = 0x0F; TOP RIGHT
//DDRC = 0xFF; PORTC = 0xF0;
	
//DDRB = 0x0F; PORTB = 0xF0; BOTTOM LEFT
//DDRC = 0xFF; PORTC = 0x0F;
	
//DDRB = 0x0F; PORTB = 0xF0; BOTTOM RIGHT
//DDRC = 0xFF; PORTC = 0xF0;

unsigned char bc_top = 0xFF;
unsigned char b_bot = 0x0F;


unsigned char b_top_left = 0x0F;
unsigned char c_top_left = 0x0F;

unsigned char b_top_right = 0x0F;
unsigned char c_top_right = 0xF0;

unsigned char b_bot_left = 0xF0;
unsigned char c_bot_left = 0x0F;

unsigned char b_bot_right = 0xF0;
unsigned char c_bot_right = 0xF0;

unsigned char b0 = 0;
unsigned char b1 = 0;
unsigned char b2 = 0;
unsigned char b3 = 0;
unsigned char b4 = 0;

enum menu_sms {init, menu, start, choose_level, highscore} menu_sm;
void menu_sm_tick()
{
	start_l1 = 0;
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (menu_sm)
	{
		case init:
		if(b0 || b1 || b2 || b3)
		{
			menu_sm = menu;
		}
		else
		{
			menu_sm = init;
		}
		break;
		
		case menu:
		if (b0 && !b1 && !b2)
		{
			start_l1 = 1;
			menu_sm = start;
		}
		else if (b1 && (!start_l1))
		{
			menu_sm = choose_level;
		}
		else if(b2 && start_l1)
		{
			menu_sm = highscore;
		}
		else if (b4)
		{
			menu_sm = init;
		}
		else
		{
			menu_sm = menu;
		}
		break;
		
		//------------------------------
		
		case start:
		if (start_l1)
		{
			menu_sm = start;
		}
		else if (end_l1 && !start_l1)
		{
			menu_sm = menu;
		}
		break;
		
		//-----------------------------
		
		case choose_level:
		if (b4)
		{
			menu_sm = init;
		}
		else if (choose_level_gl)
		{
			menu_sm = choose_level;
		}
		else
		{
			menu_sm = init;
		}
		break;
		
		case highscore:
		if (b4)
		{
			menu_sm = init;
		}
		break;
		
		default:
		menu_sm = init;
		break;
	}
	
	
	switch (menu_sm)
	{
		case init:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, init_string);
		highscore_total = 0;
		break;
		
		case menu:
		highscore_total = 0;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, menu_string);
		break;
		
		case start:
		break;
		
		case choose_level:
		choose_level_gl = 1;
		break;
		
		case highscore:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(highscore_total + '0');
		break;
		
		default:
		break;
		
	}
}

enum level1s {level1_init, blink1, level1_continue, wrong, next_level} level1;
void level1_tick()
{
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	unsigned char blink_count = 3;
	
	switch (level1)
	{
		end_l1 = 0;
		case level1_init:
		end_l1 = 1;
		if(start_l1 == 0)
		{
			level1 = level1_init;
		}
		else if (start_l1 == 1)
		{
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_DisplayString(1, start_string);
			level1 = blink1;
		}
		break;
		
		case blink1:
		level1 = level1_continue;
		break;
		
		
		case level1_continue:
		if (b0)
		{
			level1 = next_level;
		}
		else if (b1 || b2 || b3)
		{
			level1 = wrong;
		}
		else if (b4)
		{
			start_l1 = 0;
			level1 = level1_init;
		}
		else
		{
			level1 = level1_continue;
		}
		break;
		
		case next_level:
		level1 = level1_init;
		break;
		
		case wrong:
		level1 = level1_init;
		start_l1 = 1;
		break;
	}
	
	switch(level1)
	{
		case blink1:
		TimerSet(2000);
		DDRB = bc_top;
		DDRC = bc_top;
		PORTB = b_top_left;
		PORTC = c_top_left;
		break;
		
		case level1_continue:
		TimerSet(500);
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, level1_continue_string);
		break;
		
		case next_level:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, next_level_string);
		break;
		
		case wrong:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, wrong_string);
		break;	
	}
}

enum choose_ls {level_display} choose_l;
void choose_level_tick()
{
	switch(choose_l)
	{
		if (choose_level_gl)
		{
			case level_display:
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(level + '0');
			if (b0)
			{
				if (level < 5)
				{
					level++;
				}
				LCD_ClearScreen();
				LCD_Cursor(1);
				LCD_WriteData(level + '0');
			}
			else if (b1)
			{
				if (level > 1)
				{
					level--;
				}
			}
			else if (b2)
			{
				choose_level_gl = 0;
				choose_l = level_display;
			}
			break;
		}
	}
	
	switch (choose_l)
	{
		case level_display:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(level + '0');
		break;
	}
}

int main(void) //EE PROM
{
	DDRA = 0x03; PORTA = 0xFC;
	DDRD = 0xFF; PORTD = 0x00;
	
	DDRB = 0xFF; PORTB = 0xFF; //COLLUMN (LEFT->RIGHT): 0x00: ground 0 means ON
	DDRC = 0xFF; PORTC = 0x00; //ROW (TOP->BOTTOM): 0xFF: power 1 means on
	
	TimerSet(500);
	TimerOn();
	menu_sm = init;
	
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
    while(1)
    {
		menu_sm_tick();
		level1_tick();
		//choose_level_tick();
		while(!TimerFlag){}
		TimerFlag = 0;
    }
}