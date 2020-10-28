.PHONY: all clean program program-fuses reset halgen pprogram

DEVICE := attiny13
DUDE_DEV := t13
CLOCK := 128000
OBJS := main.o

CC := avr-gcc
CFLAGS := -std=c11 -g3 -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wshadow

PROGRAMMER := avrispmkII
PGM_SPEED := 1kHz

all: main main.bin

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

reset:
	avrdude -c $(PROGRAMMER) -p $(DUDE_DEV) -B $(PGM_SPEED)

program: main.hex
	avrdude -c $(PROGRAMMER) -p $(DUDE_DEV) -B $(PGM_SPEED) -U lfuse:w:0x71:m -U hfuse:w:0xff:m		# Clockrate 9.6 MHz
	avrdude -c $(PROGRAMMER) -p $(DUDE_DEV) -B 10kHz -U flash:w:main.hex:i
	avrdude -c $(PROGRAMMER) -p $(DUDE_DEV) -B $(PGM_SPEED) -U lfuse:w:0x77:m -U hfuse:w:0xff:m		# Clockrate 128 kHz

program-fuses:
	avrdude -c $(PROGRAMMER) -p $(DUDE_DEV) -B $(PGM_SPEED) -U lfuse:w:0x77:m -U hfuse:w:0xff:m

pprogram: main.bin
	minipro -p ATTINY13 -c code -s -w main.bin
	minipro -p ATTINY13 -e -c config -w fuse.conf

clean:
	rm -f main.hex main.bin main $(OBJS)

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.hex: main
	avr-objcopy -j .text -j .data -O ihex main $@

main.bin: main
	avr-objcopy -j .text -j .data -O binary main $@

halgen: hal.xml
	../HALGen/halgen -a avr -o hal.h hal.xml
