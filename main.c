/*
 * timer.c
 *
 * Created: 3/13/2020 1:27:15 PM
 * Author : runne
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// If using 8 Mhz internal reference...
//
// TCCR0B bit field : xxxx xPPP : PPP are prescaler bits
// 
// First 2 columns from datasheet Table 14-9
// Third column is timer period x 255
// -----------------------------------------------------------
// | PPP | Prescaler | 8 bit timer overflow interrupt period |
// -----------------------------------------------------------
// | 000 | NC/paused | 
// | 001 | 1		 | 
// | 010 | 8		 | ~243  us
// | 011 | 64        | ~1.95 ms
// | 100 | 256       | ~7.8  ms
// | 101 | 1024      | ~31.1 ms
// | 110 | ext fedge |
// | 111 | ext redge |
// -----------------------------------------------------------

volatile unsigned int counter = 0;

ISR(TIMER0_OVF_vect)
{
	counter++;
}

int main(void)
{
	DDRB = 1<<PORTB0;
	
	TCCR0A = 0x00;
	TCCR0B = 0b00000101; // prescaler 1024, see above
	TIMSK0 = 1<<TOIE0;
	
	sei();
	
    while (1) 
    {
		// 
		// If PPP = 101 => 1024 prescaler, toggle every 3.11 seconds (100 x timer ovf interrupt period)
		if (counter > 100) 
		{
			PORTB ^= 1<<PORTB0;
			counter = 0;
		}
    }
}

