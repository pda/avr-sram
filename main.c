#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

struct blinker_t {
  volatile uint8_t * port;
  volatile uint8_t * ddr;
  uint8_t mask;
} blinker;

int main() {
  blinker.port = &PORTB;
  blinker.ddr = &DDRB;
  blinker.mask = 1 << 5;

  *blinker.ddr |= blinker.mask;

  while (1) {
    *blinker.port |= blinker.mask;
    _delay_ms(10);
    *blinker.port &= ~blinker.mask;
    _delay_ms(990);
  }
}
