# see env.default for parameters

AVRDUDE_FLAGS = -q -q \
								-p $(ISP_MCU) \
								-c $(ISP_PROGRAMMER) \
								-P $(ISP_PORT)

CC = avr-gcc

LDFLAGS = -Wl,-Map,firmware.map

COMPILEFLAGS = -Wall \
							 -Werror \
							 -g \
							 -Os \
							 -mmcu=$(MCU) \
							 -DF_CPU=$(F_CPU)

CFLAGS = $(COMPILEFLAGS) \
				 -std=c99

ASFLAGS = $(COMPILEFLAGS)

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all
all: firmware.hex

.PHONY: clean
clean:
	rm -f .depend firmware.elf firmware.hex $(OBJECTS)

.PHONY: burn
burn: firmware.hex
	avrdude $(AVRDUDE_FLAGS) -U $<

firmware.hex: firmware.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

firmware.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

.depend: $(SOURCES)
	rm -f -- .depend
	$(CC) $(CFLAGS) -MM $^ > .depend;
	cat .depend
include .depend
