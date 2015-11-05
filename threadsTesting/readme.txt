*****************************************************************************
** Building the 3 demos **

Copy each main to ../chibios/raspberry/main.c  
	
	cp main1.c ../chibios/rasberry/main.c
	cp main2.c ../chibios/rasberry/main.c
	cp main3.c ../chibios/rasberry/main.c

Then go to ../chibios/rasberry/ and compile it with 'make'
		
		cd ../chibios/rasberry/
		make

Once the build is done use the build/ch.bin file rename it in 
raspberryFiles/kernel.img

	mv build/ch.bin ../rasberryFiles/kernel.img

Put this files in your rasberry and your ready to go.
****************************************************************************
