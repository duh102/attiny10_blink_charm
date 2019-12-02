#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

uint8_t loop_ddrb[] = {
  0b011,
  0b011,
  0b101,
  0b101,
  0b110,
  0b110,
};

uint8_t loop_portb[] = {
  0b010,
  0b001,
  0b001,
  0b100,
  0b100,
  0b010,
};

#define COUNT_MAX 6
#define OVERLOOP_RESET 100
uint8_t bright = 0;
uint8_t counter = 0;
uint8_t overLoops = 0;
uint8_t displayLoop = 0;
uint8_t delays = 0;

int main() {
  while (1) {
    for(displayLoop = 0; displayLoop < 3; displayLoop++) {
      counter = (bright + displayLoop) % COUNT_MAX;
      DDRB = loop_ddrb[counter];
      PORTB = loop_portb[counter];
    }
    PORTB = 0;
    overLoops++;
    if(overLoops == OVERLOOP_RESET) {
      overLoops = 0;
      bright = (bright+1) % COUNT_MAX;
    }
  }
  return 0;
}
