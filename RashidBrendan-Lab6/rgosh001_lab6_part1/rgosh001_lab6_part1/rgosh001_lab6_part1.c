/*
 * rgosh001_lab6_part1.c
 *
 * Created: 5/6/2015 7:10:09 PM
 *  Author: Rashid Goshtasbi
 */ 


#include <avr/io.h>

void ADC_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}

int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	
	DDRD = 0xFF;
	PORTD = 0x00;
	
	unsigned char pb_value = 0x00;
	unsigned char pd_value = 0x00;
	


	ADC_init();
    while(1)
    {
		unsigned short adc_value = ADC;
		
		pb_value = (char)adc_value;
		pd_value = (char)(adc_value >> 8);
		
		PORTB = pb_value;
		PORTD = pd_value;
		
		
		//unsigned short my_short = adc_value;
		//unsigned char my_char = (char)my_short; // my_char = 0xCD
		//my_char = (char)(my_short >> 4); // my_char = 0xBC
		
		
        //TODO:: Please write your application code 
    }
}