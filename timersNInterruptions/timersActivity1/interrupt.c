//-------------------------------------------------------------------------
// Special Thanks to David Welch dwelch@dwelch.com for its awesome code (2012)
extern void enable_irq (void);
//This little function saved the actual state of the registers in the Processor
//so that the interrupt can use them freely and when the interrupt is done, the
//processor will be able to return to its previous state (before the interrupt)
//-------------------------------------------------------------------------

//Physical addresses to configure the ARM Timer of the RPi B
#include "bcm2835-RPI.h"
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

    //----MISSING LINES--
        //insert the UART configuration here (using pointers)
    //----MISSING LINES--

    //----MISSING LINES--
        //insert the TX & RX Pins configuration here (using pointers)
    //----MISSING LINES--

    armtime[ATIMER_CTL]=0x003E0000; //Reset
    armtime[ATIMER_LOD]=0x000F423F; //1000000-1 this is 1 second
    armtime[ATIMER_RLD]=0x000F423F; //1000000-1 this is 1 second
    armtime[ATIMER_DIV]=0x000000F9; //249
    armtime[ATIMER_CLI]=0x00000000; //Clear Flag
    armtime[ATIMER_CTL]=0x003E0082; //Reset and enable the timer

    irq[IRQ_EN_BASIC]=0x00000001; //Enables the interrupt state

    enable_irq();   //this will save the Processors state before each interrupt signal
                    //and it will trigger the c_irq_handler

    while(1){
        //----MAIN LOOP CODE---
        //Now this is your new main
        //Display the min and sec here
        //like mm:ss
        //you can update the value as often as you want or
        //everytime the "second" variable changes
        //---------------------
    }
    return(0);
}