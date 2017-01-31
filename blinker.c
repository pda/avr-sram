#include "blinker.h"

void blinker_init(struct blinker_t * b, volatile uint8_t * port, volatile uint8_t * ddr, uint8_t mask) {
  b->port = port;
  b->ddr = ddr;
  b->mask = mask;
  *ddr |= mask;
}

void blinker_on(struct blinker_t * b) {
  *b->port |= b->mask;
}

void blinker_off(struct blinker_t * b) {
  *b->port &= ~b->mask;
}
