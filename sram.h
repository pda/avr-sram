#define SRAM_DATA_PORT_COUNT 2

struct sram_t {
  // data lines spread across multiple ports,
  // with 1:1 mapping of bit position.
  volatile uint8_t * data_ddrs[SRAM_DATA_PORT_COUNT];
  volatile uint8_t * data_ports[SRAM_DATA_PORT_COUNT];
  volatile uint8_t * data_pins[SRAM_DATA_PORT_COUNT];
  uint8_t data_masks[SRAM_DATA_PORT_COUNT];

  // address shift register storage latch
  volatile uint8_t * addr_latch_ddr;
  volatile uint8_t * addr_latch_port;
  uint8_t addr_latch_mask;

  // WE# (write enable)
  volatile uint8_t * we_ddr;
  volatile uint8_t * we_port;
  uint8_t we_mask;
};

void sram_init(struct sram_t * sram);
uint8_t sram_read_8(struct sram_t * sram, uint16_t addr);
void sram_write_8(struct sram_t * sram, uint16_t addr, uint8_t data);
