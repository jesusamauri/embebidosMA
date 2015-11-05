//-------------------------------------------------------------------------
// Special Thanks to Kevin Sangeelee www.susa.net for its awesome code (2012)
//-------------------------------------------------------------------------

//Physical addresses to configure the ARM Timer of the RPi B
#include "bcm2835-RPI.h"
//----------------------------------------------------------

//External GPIO Register set
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
//--------------------------

//External UART Register set
volatile unsigned int* uart = (unsigned int*)UART_BASE;
//--------------------------

//External BSC0 Register set
volatile unsigned int* i2c = (unsigned int*)BSC0_BASE;
//--------------------------

//External ARM TIMER Register set
volatile unsigned int* armtime = (unsigned int*)ARM_TIMER;
//-------------------------------

//To sent an ACSII character-------------
void uart_putc(unsigned int c)
{
    while(1)
    {
        if(uart[AUX_MU_LSR_REG]&0x20) 
            break;
    }
    uart[AUX_MU_IO_REG]=c;
}
//---------------------------------------

/*Prints a string in the UART*/
void print_string (char *pt)
{
    while(*pt)
    {
        uart_putc(*pt);
        pt++;
    }
}

//Waiting for end of the TX or RX sequence
unsigned int wait_i2c_done()
{
	//Timer Configuration Part 1 millisecond
	armtime[ATIMER_CTL]=0x003E0000;  /* Reset timer */ 
	armtime[ATIMER_LOD]=0x000003E7;  /* 1ms second delay */ 
	armtime[ATIMER_RLD]=0x000003E7;  /* 1ms second delay */ 
	armtime[ATIMER_DIV]=0x000000F9;  /* 249 */ 
	armtime[ATIMER_CLI]=0x00000000;  /* clear flag for interrupts  */ 
	armtime[ATIMER_CTL]=0x003E0082;  /* reset and enable the timer "082", */ 
																	 /* "0A2" and interrupts */ 
  
	/* Wait till done (0.05 sec), let's use a timeout just in case */
  unsigned int timeout = 50;
	unsigned int oneSec;
    while((!(i2c[BSC0_STAT]&(1<<1))) && --timeout)  /* mask to check status reg */ 
	{
        while(1)
		{
			oneSec=armtime[ATIMER_RIS];   /* shows the interrupt flag bit in the lsb */ 
			if(oneSec&0x01) break;  /* if the lsb = 1 break otherwise keep in the while */ 
		}
		armtime[ATIMER_CLI]=0x00000000; /* clear flag for interrupts  */ 
    }
		if(timeout == 0){
			print_string("error 60");
			return 0; //ERROR!	
		} 
	return 1; //Good to Go
}
//-------------------------------------------------------------------------

//Reading from RTC (I2C device Address, Lenght of the reading (max 16), Address WORD, DATA[])
void readI2C(unsigned int addr, unsigned short dL, unsigned int wAddr, unsigned int* bufPt)
{
	i2c[BSC0_CTL]=0x00008080;  //b15= enable i2c, b7 = start transfer
	i2c[BSC0_STAT]=0x00000302; //!b9 = CLK stretch time out, !b8= ACK error
	i2c[BSC0_DLEN]=1; // #bytes to read or to write
	i2c[BSC0_SADD]=addr; /* slave address*/ 
	i2c[BSC0_FIFO]=wAddr; //0
	while(!(wait_i2c_done())) {}
	i2c[BSC0_DLEN]=dL;
	i2c[BSC0_STAT]=0x00000302; //Clear Status(TODO)only bit 1 and 2
	i2c[BSC0_CTL]=0x00008081;  //Read from I2C bus (TODO)
	while(!(wait_i2c_done())) {}
	int lenght;
	for(lenght=0; lenght<dL; lenght++)
	{
		*bufPt = i2c[BSC0_FIFO];/* fifo is the data register */
		bufPt++;
	}
	//*bufPt = 0;
}
//-------------------------------------------------------------------------

//Writing into RTC (I2C device Address, Lenght of the writing (max 16), Address WORD, DATA[])
void writeI2C(unsigned int addr, unsigned short dL, unsigned int wAddr, unsigned int* bufPt)
{
	i2c[BSC0_SADD]=addr;
	i2c[BSC0_DLEN]=dL;
	i2c[BSC0_FIFO]=wAddr;
	int lenght;
	for(lenght=0; lenght<dL; lenght++)
	{
		i2c[BSC0_FIFO] = *bufPt;
		bufPt++;
	}
	i2c[BSC0_STAT]=0x00000302; //Clear Status
	i2c[BSC0_CTL]=0x00008080;  //Write into I2C bus
	while(!(wait_i2c_done())) {}
}
//-------------------------------------------------------------------------

int main (void)
{
    //GPIO Configuration Part 
    gpio[GPIO_GPFSEL1]&=~(0x0000003F); //pin 1 and 0
    gpio[GPIO_GPFSEL1]|=(0x00000024); //pin 1 and 0
    //-----------------------

    //UART Configuration Part
    uart[AUX_ENABLES]=0x00000001; //enables UART
    uart[AUX_MU_IER_REG]=0x00000000; //Interrupts are not enalbe
    uart[AUX_MU_CNTL_REG]=0x00000000; //no extra useful features
    uart[AUX_MU_LCR_REG]=0x00000003; //writes in reserved bits(useless)
    uart[AUX_MU_MCR_REG]=0x00000000; //bit 1= 0  UART1_RTS line is high
    uart[AUX_MU_IER_REG]=0x00000000; //Interrupts already off
    uart[AUX_MU_IIR_REG]=0x000000C6; //read only, and 6 would not possible
    uart[AUX_MU_BAUD_REG]=0x0000010E; // 270
    unsigned int aux0;
    gpio[GPIO_GPFSEL1]|=(9<<13);
    gpio[GPIO_GPPUD]=0x00000000;
    for(aux0=0;aux0<600;aux0++) {}// wait 250 microsec
    gpio[GPIO_GPPUDCLK0]|=(3<<14);
    for(aux0=0;aux0<600;aux0++) {}// wait 250 microsec
    gpio[GPIO_GPPUDCLK0]=0x00000000;
    uart[AUX_MU_CNTL_REG]=0x00000003;
    //-----------------------

    //TC7 Address (default 1001101b -> 0x4D)
    //RTC Address (default 1101000b -> 0x68)

    //Read from RTC
    unsigned int decoyR[7] = {0};
		 /* (address, Data lenght, word address, pointer to data)*/ 
    readI2C(0x68, 7, 0, decoyR);

    //Write on RTC (not needed now)
    /*unsigned int decoyW[7] = {
    0x85, //0
    0x45, //1
    0x32, //2
    0x05, //3
    0x11, //4
    0x10, //5
    0x15  //6
	}
    */
    //writeI2C(0x68, 7, 0, decoyW);
	
	unsigned int aux1;
    unsigned int aux2;

    while(1)
    {
		//Display RTC
    	uart_putc((decoyR[4]>>4)&0x03); //d-
    	uart_putc(decoyR[4]&0x0F); //-d
    	uart_putc(0x27); // "/"
    	uart_putc((decoyR[5]>>4)&0x01); //m-
    	uart_putc(decoyR[5]&0x0F); //-m
    	uart_putc(0x27); // "/"
    	uart_putc((decoyR[6]>>4)&0x0F); //y-
    	uart_putc(decoyR[6]&0x0F); //-y
    	uart_putc(0x20); // "_"
    	uart_putc((decoyR[2]>>4)&0x01); //h-
    	uart_putc(decoyR[2]&0x0F); //-h
    	uart_putc(0x3A); // ":"
    	uart_putc((decoyR[1]>>4)&0x03); //m-
    	uart_putc(decoyR[1]&0x0F); //-m
    	uart_putc(0x3A); // ":"
    	uart_putc((decoyR[0]>>4)&0x03); //s-
    	uart_putc(decoyR[0]&0x0F); //-s
    	readI2C(0x68, 7, 0, decoyR);
    	uart_putc(0x0D);
    	//Read from RTC
    }
    return(0);
}
