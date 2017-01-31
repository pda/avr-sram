#include <stdint.h>

struct blinker_t {
  volatile uint8_t * port;
  volatile uint8_t * ddr;
  uint8_t mask;
};

void blinker_init(struct blinker_t * b);
void blinker_on(struct blinker_t * b);
void blinker_off(struct blinker_t * b);
