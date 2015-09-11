ARMGNU ?= /usr/bin/arm-none-eabi
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding
gcc: badTimer.hex badTimer.bin kernel.img
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
badTimer.o : badTimer.c
	$(ARMGNU)-gcc $(COPS) -c badTimer.c -o badTimer.o
badTimer.elf : memmap badTimer.o
	$(ARMGNU)-ld badTimer.o -T memmap -o badTimer.elf
	$(ARMGNU)-objdump -D badTimer.elf > badTimer.list
badTimer.bin : badTimer.elf
	$(ARMGNU)-objcopy badTimer.elf -O binary badTimer.bin
badTimer.hex : badTimer.elf
	$(ARMGNU)-objcopy badTimer.elf -O ihex badTimer.hex
kernel.img : badTimer.elf
	$(ARMGNU)-objcopy badTimer.elf -O binary kernel.img
