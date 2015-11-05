ARMGNU ?= /usr/bin/arm-none-eabi
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding
gcc: uart.hex uart.bin kernel.img
all : gcc clang
clean: 
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s
vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o
uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o
uart.elf : memmap vectors.o uart.o
	$(ARMGNU)-ld vectors.o uart.o -T memmap -o uart.elf
	$(ARMGNU)-objdump -D uart.elf > uart.list
uart.bin : uart.elf
	$(ARMGNU)-objcopy uart.elf -O binary uart.bin
uart.hex : uart.elf
	$(ARMGNU)-objcopy uart.elf -O ihex uart.hex
kernel.img : uart.elf
	$(ARMGNU)-objcopy uart.elf -O binary kernel.img
