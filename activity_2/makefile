ARMGNU ?= /usr/bin/arm-none-eabi
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding
gcc: interrupt.hex interrupt.bin kernel.img
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
interrupt.o : interrupt.c
	$(ARMGNU)-gcc $(COPS) -c interrupt.c -o interrupt.o
interrupt.elf : memmap vectors.o interrupt.o
	$(ARMGNU)-ld vectors.o interrupt.o -T memmap -o interrupt.elf
	$(ARMGNU)-objdump -D interrupt.elf > interrupt.list
interrupt.bin : interrupt.elf
	$(ARMGNU)-objcopy interrupt.elf -O binary interrupt.bin
interrupt.hex : interrupt.elf
	$(ARMGNU)-objcopy interrupt.elf -O ihex interrupt.hex
kernel.img : interrupt.elf
	$(ARMGNU)-objcopy interrupt.elf -O binary kernel.img
