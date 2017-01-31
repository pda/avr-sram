#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "blinker.h"
#include "uart.h"

int main() {

  struct blinker_t blinker = {
    .port = &PORTB,
    .ddr = &DDRB,
    .mask = 1 << 5,
  };
  blinker_init(&blinker);

  for (int i = 0; i <= 4; i++) {
    blinker_on(&blinker);
    _delay_ms(10);
    blinker_off(&blinker);
    _delay_ms(100);
  }

  uart_init();
  stdout = &uart_output;
  stdin = &uart_input;

  char input;
  while(1) {
    printf("> ");
    input = getchar();
    printf("\nInput: %c\n", input);
  }

}
