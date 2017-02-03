#include <avr/io.h>

#include "spi.h"

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
