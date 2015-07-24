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
	unsigned char adc_max_div2 = 0x20;

	ADC_init();
    while(1)
    {
		unsigned short adc_value = ADC;
		if(adc_value >= adc_max_div2)
		{
			pb_value = (char)adc_value;
			pd_value = (char)(adc_value >> 8);
			
			PORTB = pb_value;
			PORTD = pd_value;
		}
		else
		{
			PORTB = 0x00;
			PORTD = 0x00;
		}
    }
}