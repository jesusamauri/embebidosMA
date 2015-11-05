
//Physical addresses to configure the ARM Timer of the RPi B
#include "bcm2835-RPI.h"
//----------------------------------------------------------

//External GPIO Register set
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
//--------------------------

//External ARM TIMER Register set
volatile unsigned int* armtime = (unsigned int*)ARM_TIMER;
volatile unsigned int timeout;
int main(void)
{

    //GPIO Configuration Part  (Switch this to pointers)
    gpio[GPIO_GPFSEL1] &= ~(7<<10);
    gpio[GPIO_GPFSEL1] |= (1<<18);
        //Timer Configuration Part 10 seconds  (ERROR HERE)
    armtime[ATIMER_CTL]=0x003E0000;//posible error con valores
    armtime[ATIMER_LOD]=0x00989680-1;
    armtime[ATIMER_RLD]=0x00989680-1;
    armtime[ATIMER_DIV]=0x000000F9;
    armtime[ATIMER_CLI]=0x00000000;
    armtime[ATIMER_CTL]=0x003E00A2;//posibles error de reseteo.

while(1){
	//Toggle Sequence Part  (Switch this to pointers)
	gpio[GPIO_GPSET0]=(1<<16);
	//PUT32(GPSET0,1<<16);
        while(1) if(armtime[ATIMER_RIS]) break;
        armtime[ATIMER_CLI]=0;
        gpio[GPIO_GPCLR0]=(1<<16);
        while(1) if(armtime[ATIMER_RIS]) break;
        armtime[ATIMER_CLI]=0;
	//-----------------------------------------------
        }
    return(0);
}
