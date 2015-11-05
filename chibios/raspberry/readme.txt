*****************************************************************************
** ChibiOS/RT port for BCM2835 / ARM1176JZF-S
*****************************************************************************

** TARGET **

The demo runs on an Raspberry Pi RevB (BCM2835) board.

** Build Procedure **

This demo was built using the Yagarto GCC toolchain. 
Type 'make' and the executable will be built to build/ch.bin. 
(you need to follow first the procedure in the README.md at the
beggining of this collections of laboratories in order to run this)

use the build/ch.bin file rename it in raspberryFiles/kernel.img

	mv build/ch.bin ../rasberryFiles/kernel.img

Put this files in your rasberry and your ready to go.
****************************************************************************
