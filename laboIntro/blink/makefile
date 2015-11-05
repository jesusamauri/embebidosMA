#Compiler variable
ARMGNU ?= /usr/bin/arm-none-eabi

#Flags for the compiler
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

#files to be make
gcc: blink.hex blink.bin kernel.img
all : gcc clang

#clean workspace before any file creation
clean: 
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

#file creation
vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o
blink.o : blink.c
	$(ARMGNU)-gcc $(COPS) -c blink.c -o blink.o
blink.elf : memmap vectors.o blink.o
	$(ARMGNU)-ld vectors.o blink.o -T memmap -o blink.elf
	$(ARMGNU)-objdump -D blink.elf > blink.list
blink.bin : blink.elf
	$(ARMGNU)-objcopy blink.elf -O binary blink.bin
blink.hex : blink.elf
	$(ARMGNU)-objcopy blink.elf -O ihex blink.hex
kernel.img : blink.elf
	$(ARMGNU)-objcopy blink.elf -O binary kernel.img
