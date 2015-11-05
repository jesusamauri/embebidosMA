*****************************************************************************
** ChibiOS/RT port for BCM2835 / ARM1176JZF-S
*****************************************************************************

** TARGET **

The demo runs on an Raspberry Pi RevB (BCM2835) board.

** Build Procedure **

This demo was built using the Yagarto GCC toolchain. Just type 'make'
and the executable will be built to build/ch.bin. 

** Building the 3 demos **

You modify each main to main.c and then compile it with 'make'
use the build/ch.bin file rename it in raspberryFiles/kernel.img

	mv build/ch.bin ../rasberryFiles/kernel.img

Put this files in your rasberry and your ready to go.
****************************************************************************
