FLAGS = -mmcu=attiny10 -DF_CPU=8000000UL -Os -std=c99

all: attiny10_blink.hex attiny10_blink_rgb.hex

%.hex: %.elf
	avr-size -C --mcu=attiny10 $<
	avr-objcopy -O ihex $< $@

clean:
	rm -f *.elf *.hex

%.elf: %.c
	avr-gcc $(FLAGS) $^ -o $@
