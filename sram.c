#include <stdint.h>

#include "sram.h"
#include "spi.h"

void sram_assert_addr(struct sram_t *, uint16_t);
void sram_assert_data(struct sram_t *, uint8_t);
uint8_t sram_read_data(struct sram_t *);

void sram_init(struct sram_t * sram) {
  // WE# output, HIGH = read
  *sram->we_ddr |= sram->we_mask;
  *sram->we_port |= sram->we_mask;

  // ADDR shift register storage latch
  // output, low = ready for rising edge trigger.
  *sram->addr_latch_ddr |= sram->addr_latch_mask;
  *sram->addr_latch_port &= ~(sram->addr_latch_mask);

  spi_init();
}

uint8_t sram_read_8(struct sram_t * sram, uint16_t addr) {
    sram_assert_addr(sram, addr);
    return sram_read_data(sram);
}

void sram_write_8(struct sram_t * sram, uint16_t addr, uint8_t data) {
    sram_assert_addr(sram, addr);
    sram_assert_data(sram, data);
    // delay required approx 7..14 ns; meh.
    *sram->we_port &= ~(sram->we_mask); // write enable
    // t-wp write pulse >= 9ns for 12ns SRAM
    *sram->we_port |= sram->we_mask; // write disable
}

void sram_assert_addr(struct sram_t * sram, uint16_t addr) {
  spi_send_8((uint8_t)(addr >> 8));
  spi_send_8((uint8_t)addr);
  *sram->addr_latch_port |= sram->addr_latch_mask; // shift latch rising edge
  *sram->addr_latch_port &= ~sram->addr_latch_mask; // shift latch normally low
}

void sram_assert_data(struct sram_t * sram, uint8_t data) {
  volatile uint8_t * port;
  uint8_t mask;
  for (uint8_t i = 0; i < SRAM_DATA_PORT_COUNT; i++) {
    port = sram->data_ports[i];
    mask = sram->data_masks[i];
    *sram->data_ddrs[i] |= mask; // DDR HIGH = write
    *port = (*port & ~mask) | (data & mask); // set data
  }
}

uint8_t sram_read_data(struct sram_t * sram) {
  uint8_t data = 0x00;
  for (uint8_t i = 0; i < SRAM_DATA_PORT_COUNT; i++) {
    *sram->data_ddrs[i] &= ~sram->data_masks[i]; // DDR low = read
    data |= (*sram->data_pins[i] & sram->data_masks[i]);
  }
  return data;
}
