#include <avr/io.h>
#include <stdint.h>

#include "sram.h"
#include "uart.h"

int main() {

  uart_init();
  stdout = &uart_output;
  stdin = &uart_input;

  struct sram_t sram = (struct sram_t){
    .data_ddrs = {&DDRC, &DDRD},
    .data_ports = {&PORTC, &PORTD},
    .data_pins = {&PINC, &PIND},
    .data_masks = {0b00111111, 0b11000000},
    .addr_latch_ddr = &DDRB,
    .addr_latch_port = &PORTB,
    .addr_latch_mask = (1<<1),
    .we_ddr = &DDRD,
    .we_port = &PORTD,
    .we_mask = 1<<2,
  };

  sram_init(&sram);

  while (1) {
    printf("Type text into RAM, press enter:");
    uint16_t addr = 0x0000;
    char input = 0;
    while (input != '\r') {
      input = getchar();
      putchar(input);
      if (input == '\r') putchar('\n');
      sram_write_8(&sram, addr, input);
      addr++;
    }

    // read from RAM
    uint8_t data;
    uint16_t limit = addr;
    for (uint16_t addr = 0x0000; addr < limit; addr++) {
      data = sram_read_8(&sram, addr);
      printf("R 0x%04x --> 0x%02x %c\n", addr, data, data);
    }
  }

}
