#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "blinker.h"
#include "uart.h"

void spi_init();
void spi_send_8(uint8_t);

void assert_sram_address(uint16_t);
void assert_sram_data(uint8_t);
uint8_t read_sram_data();

const uint8_t shift_latch_mask_b = 0b00000010;
const uint8_t sram_mask_c = 0b00111111;
const uint8_t sram_mask_d = 0b11000000;
const uint8_t sram_we_mask_d = 0b00000100;

int main() {

  uart_init();
  stdout = &uart_output;
  stdin = &uart_input;

  // SRAM
  DDRB |= shift_latch_mask_b;
  DDRD |= sram_we_mask_d;
  PORTB &= ~shift_latch_mask_b; // shift latch normally low
  PORTD |= sram_we_mask_d; // WE HIGH = SRAM read mode
  spi_init();

  // write to RAM
  const char * message = "hello";
  const char * byte = message;
  uint16_t addr = 0x0008;
  while (*byte != 0) {
    assert_sram_address(addr);
    assert_sram_data(*byte);
    // delay required approx 7..14 ns; meh.
    // WE HIGH = SRAM write mode
    PORTD &= ~sram_we_mask_d;
    // t-wp write pulse >= 9ns for 12ns SRAM
    PORTD |= sram_we_mask_d; // WE HIGH = SRAM read mode
    byte++;
    addr++;
  }

  // read from RAM
  uint8_t x;
  for (uint16_t addr = 0x0000; addr <= (8 + 5 + 8); addr++) {
    assert_sram_address(addr);
    x = read_sram_data();
    printf("0x%04x => 0x%02x %c\n", addr, x, x);
  }

  while (1);

}

// PB1 = shift storage clock/latch

void spi_init() {
  DDRB &= 0b11101111; // input: MISO(4)
  DDRB |= 0b00101100; // ouput: SS(2), MOSI(3), SCK(5)

  SPCR = ((1<<SPE)|   // SPI Enable
          (0<<SPIE)|  // SPI Interupt Enable
          (0<<DORD)|  // Data Order (0:MSB first / 1:LSB first)
          (1<<MSTR)|  // Master/Slave select
          (0<<SPR1)|  // SPI Clock Rate
          (1<<SPR0)|  // SPI Clock Rate
          (0<<CPOL)|  // Clock Polarity (0:SCK low / 1:SCK hi when idle)
          (0<<CPHA)); // Clock Phase (0:leading / 1:trailing edge sampling)

  SPSR = (1<<SPI2X);  // Double Clock Rate
}

void spi_send_8(uint8_t data) {
  SPDR = data;
  while ((SPSR & (1<<SPIF)) == 0);
}

void assert_sram_address(uint16_t addr) {
  spi_send_8((uint8_t)(addr >> 8));
  spi_send_8((uint8_t)addr);
  PORTB |= shift_latch_mask_b; // shift latch rising edge
  PORTB &= ~shift_latch_mask_b; // shift latch normally low
}

void assert_sram_data(uint8_t data) {
  DDRC |= sram_mask_c;
  DDRD |= (sram_mask_d | sram_we_mask_d);
  PORTC = (PORTC & ~sram_mask_c) | (data & sram_mask_c);
  PORTD = (PORTD & ~sram_mask_d) | (data & sram_mask_d);
}

uint8_t read_sram_data() {
  DDRC &= ~sram_mask_c;
  DDRD &= ~(sram_mask_d | sram_we_mask_d);
  return (PINC & sram_mask_c) | (PIND & sram_mask_d);
}
