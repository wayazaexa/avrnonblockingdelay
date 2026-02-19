CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRSIZE=avr-size
OBJISP=avrdude
MCU=atmega328p
CFLAGS=-Wall -Wextra  -Wundef -pedantic \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU}
LDFLAGS := -mmcu=$(MCU)
PORT=/dev/ttyACM0
NAME=program
BIN=${NAME}.hex
SOURCES = main.c millis.c

OBJS := $(SOURCES:.c=.o)

.PHONY: clean fresh all isp

all: $(BIN)

%.o:%.c
	$(COMPILE.c) -MD -o $@ $<

%.elf: $(OBJS)
	$(CC) -Wl,-Map=$(@:.elf=.map) $(LDFLAGS) -o $@ $^
	$(AVRSIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

isp: ${BIN}
	@sudo $(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<

clean:
	@rm -f $(OBJS) *.map *.P *.d *.o

fresh: clean
	@rm -rv $(BIN)