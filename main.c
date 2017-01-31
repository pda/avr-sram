#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "uart.h"

struct blinker_t {
  volatile uint8_t * port;
  volatile uint8_t * ddr;
  uint8_t mask;
} blinker;

int main() {

  uart_init();
  stdout = &uart_output;
  stdin = &uart_input;

  char input;

  while(1) {
    printf("> ");
    input = getchar();
    printf("\nInput: %c\n", input);
  }

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
