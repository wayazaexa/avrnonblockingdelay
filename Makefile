CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRSIZE=avr-size
OBJISP=avrdude
MCU=atmega328p
CFLAGS=-Wall -Wextra  -Wundef -pedantic \
		-I C:\avr\avr\include \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU}
LDFLAGS=-mmcu=$(MCU)
PORT=/dev/ttyACM0
BIN=avrdemo
OUT=${BIN}.hex
SOURCES = main.c


ifeq ($(DEBUG), 1)
	OUTPUTDIR=bin/debug
else
	OUTPUTDIR=bin/release
endif

OBJS = $(SOURCES:.c=.o)

all: $(OUT)

%.o:%.c
	$(CC) $(CFLAGS) -MD -o $@ -c $<

%.lss: %.elf
	$(OBJDUMP) -h -S -s $< > $@

%.elf: $(OBJS)
	$(CC) -Wl,-Map=$(@:.elf=.map) $(LDFLAGS) -o $@ $^
	$(AVRSIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

isp: ${BIN}.hex
	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<

clean:
	@rm -f "$(OUT)"  *.map *.P *.d *.o
		   	
.PHONY: clean dirs