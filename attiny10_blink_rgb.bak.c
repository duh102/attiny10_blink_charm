#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

// Set the CPU prescaler to expect no divison (8MHz)
#define F_CPU 8000000UL

uint8_t loop_ddrb[] = {
  0b1001,
  0b1010,
  0b1100,
};

uint8_t loop_portb[] = {
  0b0001,
  0b0010,
  0b0100,
};

#define COUNT_MAX 3
uint8_t counter = 0;
uint8_t time0 = 0;
uint8_t time1 = 0;

int main() {
  // Allow changing the clock prescaler
  CCP = 0xD8;
  // Set the clock prescaler to 0, meaning no divison of the clock source
  CLKPSR = 0;
  while (1) {
    PORTB = 0;
    counter = (counter+1) % COUNT_MAX;
    DDRB = loop_ddrb[counter];
    PORTB = loop_portb[counter];
  }
  return 0;
}
