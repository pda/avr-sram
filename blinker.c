#include "blinker.h"

void blinker_init(struct blinker_t * b) {
  *b->ddr |= b->mask;
}

void blinker_on(struct blinker_t * b) {
  *b->port |= b->mask;
}

void blinker_off(struct blinker_t * b) {
  *b->port &= ~b->mask;
}
