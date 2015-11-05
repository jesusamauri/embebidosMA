//-------------------------------------------------------------------------
// Special Thanks to David Welch dwelch@dwelch.com for its awesome code (2012)
extern void enable_irq (void);
//This little function saved the actual state of the registers in the Processor
//so that the interrupt can use them freely and when the interrupt is done, the
//processor will be able to return to its previous state (before the interrupt)
//-------------------------------------------------------------------------

//Physical addresses to configure the ARM Timer of the RPi B
#include "bcm2835-RPI.h"
#include <string.h>
//----------------------------------------------------------

//External GPIO Register set
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
//--------------------------

//External IRQ Register set
volatile unsigned int* irq = (unsigned int*)IRQ_BASE;
//-------------------------

//External ARM Timer Register set
volatile unsigned int* armtime = (unsigned int*)ARM_TIMER;
volatile unsigned int timeout;
//-------------------------------

//External UART Register set
volatile unsigned int* uart = (unsigned int*)UART_BASE;
//-------------------------------

volatile unsigned int minute = 11;
volatile unsigned int second = 40;

void c_irq_handler (void)
{
    //----INTERRUPT CODE---
        //The seconds and minutes will have thier logic here
    //---------------------
    armtime[ATIMER_CLI]=0x00000000; //Clear Flag This will allow the timer to trigger
                                    //the interrupt once again
}

int main (void)
{
    irq[IRQ_DIS_BASIC]=0x00000001; //Disables the interrupt state
//funcion para el envio de un caracter por serial
      void uart_putc (unsigned int c)
{
    while(1)
    {
        if(uart[AUX_MU_LSR_REG]&0x20) 
            break;
    }
    uart[AUX_MU_IO_REG]=c;
}  
//To recieve an ACSII character----------
unsigned int uart_getc (void)
{
    while(1)
    {
        if(uart[AUX_MU_LSR_REG]&0x01) 
            break;	
    }
    return uart[AUX_MU_IO_REG];
}


//Prints a string in the UART------------
void print_string (char *pt)
{
    while(*pt)
    {
        uart_putc(*pt);
        pt++;
    }
}

 //UART configuration
    uart[AUX_ENABLES]=1;
    uart[AUX_MU_IER_REG]=0;
    uart[AUX_MU_CNTL_REG]=0;
    uart[AUX_MU_LCR_REG]=3;
    uart[AUX_MU_MCR_REG]=0;
    uart[AUX_MU_IER_REG]=0;
    uart[AUX_MU_IIR_REG]=0xC6;
    uart[AUX_MU_BAUD_REG]=270;
    //------------------
    //----MISSING LINES--

    //----MISSING LINES--
        //insert the TX & RX Pins configuration here (using pointers)
		 //TX and RX Pins configuration
    //**We need to set this pint to alt function 5 because 
    //**the datasheet doesnt explain how to use the alt function 0
    
    gpio[GPIO_GPFSEL1]&=~(7<<12); //gpio14
    gpio[GPIO_GPFSEL1]|=2<<12;    //alt5
    gpio[GPIO_GPFSEL1]&=~(7<<15); //gpio15
    gpio[GPIO_GPFSEL1]|=2<<15;    //alt5
    
    gpio[GPIO_GPPUD]=0;
    int aux = 0;	
    for(aux=0;aux<150;aux++) // We need to wait about 150 microseconds to let it load
    {
        asm("nop");  //this works like a NOPE instruction
    }
    gpio[GPIO_GPPUDCLK0]=(1<<14)|(1<<15);
    for(aux=0;aux<150;aux++) // We need to wait about 150 microseconds to let it load
    {
        asm("nop");  //this works like a NOPE instruction
    }

    gpio[GPIO_GPPUDCLK0]=0;
    uart[AUX_MU_CNTL_REG]=3;
    //----MISSING LINES--

    armtime[ATIMER_CTL]=0x003E0000; //Reset
    armtime[ATIMER_LOD]=0x000F4240-1; //1000000-1 this is 1 second
    armtime[ATIMER_RLD]=0x000F4240-1; //1000000-1 this is 1 second
    armtime[ATIMER_DIV]=0x000000F9; //249
    armtime[ATIMER_CLI]=0x00000000; //Clear Flag
    armtime[ATIMER_CTL]=0x003E00A2; //Reset and enable the timer

    char tiempo[5]; //creamos nuestro vector para guardar los valores del reloj y los puntos
	tiempo[2] = 0x3A;
	tiempo[0] = 0x30;
	tiempo[1] = 0x30;
	tiempo[3] = 0x30;
	tiempo[4] = 0x30; // asignacion de valores al vector
	
	while(1){
		uart_putc(0x0D);//impresion de un salto de linea
		//impresion del reloj en el puerto serial
		////////////////////////////////////////
		uart_putc(tiempo[0]);
		uart_putc(tiempo[1]);	
		uart_putc(tiempo[2]);	
		uart_putc(tiempo[3]);	
		uart_putc(tiempo[4]);
		///////////////////////////////////////
	
		while(1){ //espera hasta que la interrupcion del timer ponga en 1 el RIS
			if(armtime[ATIMER_RIS]) 
				break;
		}
		
		armtime[ATIMER_CLI]=0x00000000;//limpia el RIS
		//sumatorias y condiciones para el reloj
		///////////////////////////////////////////////
		tiempo[4]++;
		if(tiempo[4]==0x3A){
			tiempo[3]++;
			tiempo[4]=0x30;
			if(tiempo[3]==0x36){
				tiempo[1]++;
				tiempo[3]=0x30;
				if(tiempo[1]==0x3A){
					tiempo[0]++;
					tiempo[1]=0x30;
					if(tiempo[0]==0x36)
						tiempo[0]=0x30;
				}
			}
		}
		/////////////////////////////////////////////////
    }
    return(0);
}
