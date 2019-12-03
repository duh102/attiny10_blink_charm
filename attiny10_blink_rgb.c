// define CPU speed - actual speed is set using CLKPSR in main()
#define F_CPU 8000000UL
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

const uint8_t loop_ddrb[] PROGMEM = {
  0b0011, // d1 R
  0b0101, // d1 G
  0b1001, // d1 B
  0b0011, // d2 R
  0b0110, // d2 G
  0b1010, // d2 B
  0b0101, // d3 R
  0b0110, // d3 G
  0b1100, // d3 B
  0b1001, // d4 R
  0b1010, // d4 G
  0b1100, // d4 B
};

const uint8_t loop_portb[] PROGMEM = {
  0b0001,
  0b0001,
  0b0001,
  0b0010,
  0b0010,
  0b0010,
  0b0100,
  0b0100,
  0b0100,
  0b1000,
  0b1000,
  0b1000,
};

#define SIN_FRAMES 4
#define NUM_LEDS 12
const uint8_t sin_vals[] PROGMEM = {
  0, 49,141,212,255
};
int8_t d_status[] = {
  3,
  4,
  3,
  2,
  1,
  0,
  3,
  4,
  3,
  2,
  1,
  0,
};

volatile uint8_t d_pwm[] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};
volatile uint8_t  pwmCount = 0;
volatile uint8_t  loop_portb_mem = 0;
volatile uint16_t counter0 = 0;
const uint16_t colorInterval = 2048;

int main() {
  // CCP register must first be written with the correct signature - 0xD8
  CCP = 0xD8;
  //  CLKPS[3:0] sets the clock division factor
  CLKPSR = 0;

  // set initial RGB value
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    int8_t state = d_status[i];
    d_pwm[i] = pgm_read_byte(&sin_vals[state<0?-state:state]);
  }

  // enable interrupts
  sei();

  // main loop
  while (1)
  {
    for(uint8_t currLed = 0; currLed < NUM_LEDS; currLed++) {
      PORTB = 0;
      DDRB = pgm_read_byte(&loop_ddrb[currLed]);
      loop_portb_mem = pgm_read_byte(&loop_portb[currLed]);
      if(pwmCount < d_pwm[currLed]) {
        PORTB = loop_portb_mem;
      } else {
        PORTB = 0;
      }
    }
    // increment PWM count
    pwmCount++;
    // increment interval counter
    counter0++;
    if(counter0 % colorInterval == 0) {
      for(uint8_t i = 0; i < NUM_LEDS; i++) {
        int8_t state = d_status[i];
        state = state == -1? 0 : state == SIN_FRAMES? -(state-1) : state+1;
        d_status[i] = state;
        d_pwm[i] = pgm_read_byte(&sin_vals[state<0?-state:state]);
      }
    }
  }
  return 0;
}
