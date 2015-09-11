//-------------------------------------------------------------------------
// Special Thanks to David Welch dwelch@dwelch.com for its awesome code (2012)
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
//-------------------------------------------------------------------------


//Physical addresses to configure the ACK LED of the RPi B
#define GPFSEL1     0x20200004
#define GPSET0      0x2020001C
#define GPCLR0      0x20200028
//--------------------------------------------------------

int main (void)
{
    //GPIO 16 configuration
    unsigned int aux;
    aux=GET32(GPFSEL1);
    aux&=~(7<<18);
    aux|=1<<18;
    PUT32(GPFSEL1,aux);
    //---------------------

    unsigned int pT;
    while(1)
    {
    //starts blink toggle sequence
        PUT32(GPSET0,1<<16);
        for(pT=0;pT<0x100000;pT++)
        {
            dummy(pT);  //this works like a NOPE instruction
        }
        PUT32(GPCLR0,1<<16);
        for(pT=0;pT<0x100000;pT++)
        {
            dummy(pT);  //this works like a NOPE instruction
        }
    //ends blink toggle sequence
    }
    return(0);
}