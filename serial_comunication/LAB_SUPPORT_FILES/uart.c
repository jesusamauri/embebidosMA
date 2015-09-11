//-------------------------------------------------------------------------
// Special Thanks to David Welch dwelch@dwelch.com for its awesome code (2012)
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
//-------------------------------------------------------------------------

//Physical addresses to configure the TX&RX Pins of the RPi B
#define GPFSEL1     0x20200004
#define GPSET0      0x2020001C
#define GPCLR0      0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068
//------------------------------------------------------------

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//((250,000,000/115200)/8)-1 = 270

//To sent an ACSII character-------------
void uart_putc (unsigned int c)
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) 
            break;
    }
    PUT32(AUX_MU_IO_REG,c);
}
//---------------------------------------

//To recieve an ACSII character----------
unsigned int uart_getc (void)
{
	int t = 0x310000;
    while(t > 0)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) 
            break;
	t--;	
    }
    return GET32(AUX_MU_IO_REG);
}
//---------------------------------------

//Prints a string in the UART------------
void print_string (char *pt)
{
    while(*pt)
    {
        uart_putc(*pt);
        pt++;
    }
}
//---------------------------------------

//Reads a strings from the UART----------
void read_string (char* bufPt, unsigned short max) 
{
    int length = 0;
    char character;
    character = uart_getc();
    while(character != 0x0D) //carriage feed
    {
        if(character == 0x08) //backspace
        {
            if(length)
            {
                bufPt--;
                length--;
                uart_putc(0x08); //backspace
            }
        }
        else if(length < max)
        {
            *bufPt = character;
            bufPt++;
            length++;
            uart_putc(character);
        }
        character = uart_getc();
    }
    *bufPt = 0;
}
//---------------------------------------

//------------------------------------------------------------------------
int main (void)
{
    //UART configuration
    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);
    //------------------

    //TX and RX Pins configuration
    //**We need to set this pint to alt function 5 because 
    //**the datasheet doesnt explain how to use the alt function 0
    unsigned int aux;
    aux=GET32(GPFSEL1);
    aux&=~(7<<12); //gpio14
    aux|=2<<12;    //alt5
    aux&=~(7<<15); //gpio15
    aux|=2<<15;    //alt5
    PUT32(GPFSEL1,aux);

    PUT32(GPPUD,0);
    for(aux=0;aux<150;aux++) // We need to wait about 150 microseconds to let it load
    {
        dummy(aux);  //this works like a NOPE instruction
    }
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(aux=0;aux<150;aux++) // We need to wait about 150 microseconds to let it load
    {
        dummy(aux);  //this works like a NOPE instruction
    }

    PUT32(GPPUDCLK0,0);
    PUT32(AUX_MU_CNTL_REG,3);
    //----------------------------

    // Test
    char msg[] = "Welcome to Simon_dice";
    char wait[] = "[Press any button to start ";
    char decoy[15] = {0x77,0x64,0x77,0x73,0x61,0x64,0x64};
	char play[] = "Level ";
	char nivel = 0X30;
	char up[] = "UP ";
	char down[] = "DOWN ";
	char left[] = "left ";
	char right[] = "right ";
	char replay[] = "Presiona una tecla para volver a jugar ";
    print_string(msg);
    print_string(wait);
    // ----
    aux = 0; 
    while(1){
        aux = uart_getc();
	
	if(aux != 0){
	int cont = 7;
	int loser = 0;
	int i ;
	int r ;
	nivel = 0x30;
		for(i = 0; i < cont ; i++){
			//Print level, #nivel, "."
			print_string(play);
			uart_putc(nivel++);
			uart_putc(0x2E);

			//PRINTING INSTRUCTIONS
			for (r = 0; r <= i; r++){
				switch(decoy[r]){
					case 0x77:
						print_string(up);
					break;
					case 0x64:
							print_string(right);
					break;
					case 0x61:
							print_string(left);
					break;
					case 0x73:
							print_string(down);
					break;
				}

			}
				    uart_putc(0x0D); //carriage feed
				    uart_putc(0x0A); //line feed

			for( r = 0; (r <= i) && (loser == 0); r++){
				loser = 1;
					char temp = uart_getc();
					uart_putc(temp);
					if (temp == decoy[r]){
						loser = 0;
					}
			}
			if(loser){
				aux = 0;
				i = cont;
			}
		}	
	
	print_string(replay);
         }  
	
    }

    return(0);
}
