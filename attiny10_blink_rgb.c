/*
 * attiny10-rgb-hello.c
 *
 * Drive an RGB LED using Attiny10.
 * 
 * Author : electronut Labs
 * Website: electronut.in
 */ 


// define CPU speed - actual speed is set using CLKPSR in main()
#define F_CPU 8000000UL

#include <avr/io.h>

#include<avr/pgmspace.h>
#include<avr/interrupt.h>

volatile unsigned char pwmCount = 0;
volatile unsigned long int counter0 = 0;
volatile unsigned char colorIndex= 0;
const unsigned long int colorInterval = 32768;

// colors
const unsigned char nColors = 5;
// cyan, magenta, orange, purple, yellow
const unsigned char red[]   PROGMEM = {  0, 255,  255,  75, 255};
const unsigned char green[] PROGMEM = {255,   0,   70,   0, 255};
const unsigned char blue[]  PROGMEM = {255, 255,    0, 130,   0};
volatile unsigned char rgb[] = {255, 255, 255};

// interrupt for Compare A
ISR(TIM0_COMPA_vect)
{
  PORTB = 0;
	// turn on/off according to PWM value
	if (pwmCount > rgb[0]) {
		PORTB &= ~(1 << PB0);
	}
	else {
		PORTB |= (1 << PB0);
	}
	if (pwmCount > rgb[1]) {
		PORTB &= ~(1 << PB1);
	}
	else {
		PORTB |= (1 << PB1);
	}
	if (pwmCount > rgb[2]) {
		PORTB &= ~(1 << PB2);
	}
	else {
		PORTB |= (1 << PB2);
	}
	// increment PWM count
	pwmCount++;
	
	// increment interval counter
	counter0++;
}

int main(void)
{	
	// Set CPU speed by setting clock prescaler:
	// CCP register must first be written with the correct signature - 0xD8
	CCP = 0xD8;
	//  CLKPS[3:0] sets the clock division factor
	CLKPSR = 0;

	// set up Output Compare A
	// WGM[3:0] is set to 0010
	// prescaler is set to clk/8 (010)
	TCCR0A = 0;
	TCCR0B = (1 << 1) | (1 << WGM02);
	// set Output Compare A value
	OCR0A = 39;
	// enable Output Compare A Match interrupt
	TIMSK0 |= (1 << OCIE0A); 
	
	// enable interrupts
	sei();
	
	// set pin directions
	DDRB = 1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3;
  PORTB = 0;
	
	// set initial RGB value
	rgb[0] = pgm_read_byte(&red[colorIndex]);
	rgb[1] = pgm_read_byte(&green[colorIndex]);
	rgb[2] = pgm_read_byte(&blue[colorIndex]);
	
  // main loop
  while (1) 
  {
		// change color every N cycles
		if(counter0 % colorInterval == 0) {
			colorIndex = (colorIndex + 1) % nColors;
			// set current RGB value
			rgb[0] = pgm_read_byte(&red[colorIndex]);
			rgb[1] = pgm_read_byte(&green[colorIndex]);
			rgb[2] = pgm_read_byte(&blue[colorIndex]);
		}
  }
}
