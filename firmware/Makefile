DEVICE = atmega16a
CLOCK = 16000000

FUSES = -U lfuse:w:0xfe:m -U hfuse:w:0x99:m 
AVRDUDE = avrdude -c  usbasp -p m16

OBJ = usbdrv.o usbdrvasm.o i2cdrv.o i2ctester.o
CFLAGS  = -Iusbdrv
COMPILE = avr-gcc -Wall -Os $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: i2ctester.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

fuse:
	$(AVRDUDE) $(FUSES)

i2ctester.elf: 	$(OBJ)
	$(COMPILE) -o i2ctester.elf $(OBJ)

i2ctester.hex: i2ctester.elf
	rm -f i2ctester.hex
	avr-objcopy -j .text -j .data -O ihex i2ctester.elf i2ctester.hex

flash: all
	$(AVRDUDE) -U flash:w:i2ctester.hex:i

clean:
	rm -f i2ctester.hex i2ctester.elf $(OBJ)