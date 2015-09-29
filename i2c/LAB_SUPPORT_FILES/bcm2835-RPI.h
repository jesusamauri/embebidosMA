#ifndef RPI_GPIO_H
#define RPI_GPIO_H

/* The base address of the GPIO peripheral (ARM Physical Address) */

#define IO_PERIPHERALS		0x20000000UL
#define SYSTIMERCLO			0x20003004
#define IRQ_BASE			(IO_PERIPHERALS + 0x00B200)		//
#define ARM_TIMER			(IO_PERIPHERALS + 0x00B400)		//
#define GPIO_PADS			(IO_PERIPHERALS + 0x100000)		//Not in use (DO NOT TOUCH IT)
#define CLOCK_BASE			(IO_PERIPHERALS + 0x101000)		//Not in use (DO NOT TOUCH IT)
#define GPIO_BASE			(IO_PERIPHERALS + 0x200000)		//
#define BSC0_BASE			(IO_PERIPHERALS + 0x205000)		//(TODO) 0x214000, pag 160 DS
#define GPIO_PWM			(IO_PERIPHERALS + 0x20C000)		//Not supported
#define UART_BASE			(IO_PERIPHERALS + 0X215000)
#define SPI_BASE			(IO_PERIPHERALS + 0X215000)

//-------------BSC0_BASE-------------------
#define BSC0_CTL			0 //Data register 
#define BSC0_STAT			1 //status register and error clea reg
#define BSC0_DLEN			2 //holds i2c slave address 
#define BSC0_SADD			3 //control register 
#define BSC0_FIFO			4 //flag register 
#define BSC0_DIV			5 //interrupt fifo lvl select register|j 
#define BSC0_DEL			6 //interrupt mask set clear register 
#define BSC0_CLKT			7 //raw interrupt status register 
//-------------BSC0_BASE-------------------


//-------------UART_BASE-------------------
#define AUX_IRQ				0  //used to check pending interrupts 
#define AUX_ENABLES     	1 //enables SPI 1 & 2, and mini UART
#define AUX_MU_IO_REG   	16 //write and read data from FIFO's
#define AUX_MU_IER_REG  	17 //use to enable interrupts
#define AUX_MU_IIR_REG  	18 //shows the interrupt status
#define AUX_MU_LCR_REG  	19 //control register,baudrate reg
#define AUX_MU_MCR_REG  	20 //control modem signals
#define AUX_MU_LSR_REG  	21 //shows the data status
#define AUX_MU_MSR_REG  	22 //shows the modem status
#define AUX_MU_SCRATCH  	23 //single byte storage
#define AUX_MU_CNTL_REG 	24 //extra useful features
#define AUX_MU_STAT_REG 	25 //internal status of the mini UART
#define AUX_MU_BAUD_REG 	26 //access to 16-bit baudrate counter
// 27 - 31 are RESERVERD
//-------------UART_BASE-------------------


//--------------SPI_BASE-------------------
#define AUX_IRQ				0
#define AUX_ENABLES     	1
#define AUX_SPI0_CNTL0_REG	32
#define AUX_SPI0_CNTL1_REG	33
#define AUX_SPI0_STAT_REG	34
// 35 is RESERVED
#define AUX_SPI0_IO_REG		36
#define AUX_SPI0_PEEK_REG	37
// 38 - 47 are RESERVED
#define AUX_SPI1_CNTL0_REG	48
#define AUX_SPI1_CNTL1_REG	49
#define AUX_SPI1_STAT_REG	50
// 51 is RESERVED
#define AUX_SPI1_IO_REG		52
#define AUX_SPI1_PEEK_REG	53
//--------------SPI_BASE-------------------


//--------------IRQ_BASE-------------------
#define IRQ_BASIC 			0  //R/W
#define IRQ_PEND1 			1  //R/W
#define IRQ_PEND2 			2  //R/W
#define IRQ_FIQ_CONTROL		3  //R/W
#define IRQ_EN1				4  //R/W
#define IRQ_EN2				5  //R/W
#define IRQ_EN_BASIC		6  //R/W
#define IRQ_DIS1			7  //R/W
#define IRQ_DIS2			8  //R/W
#define IRQ_DIS_BASIC 		9  //R/W
//--------------IRQ_BASE-------------------


//--------------ARM_TIMER------------------
#define ATIMER_LOD 			0  //R/W	0
#define ATIMER_VAL			1  //R 		4
#define ATIMER_CTL			2  //R/W	8
#define ATIMER_CLI			3  //W		C
#define ATIMER_RIS			4  //R 		10
#define ATIMER_MIS			5  //R 		14
#define ATIMER_RLD			6  //R/W	18
#define ATIMER_DIV			7  //R/W	1C
#define ATIMER_CNT			8  //R/W	20
//--------------ARM_TIMER------------------


//--------------GPIO_BASE------------------
#define GPIO_GPFSEL0    	0  //R/W
#define GPIO_GPFSEL1    	1  //R/W
#define GPIO_GPFSEL2    	2  //R/W
#define GPIO_GPFSEL3    	3  //R/W
#define GPIO_GPFSEL4    	4  //R/W
#define GPIO_GPFSEL5    	5  //R/W
// 6 is RESERVED
#define GPIO_GPSET0     	7  //W
#define GPIO_GPSET1     	8  //W
// 9 is RESERVED
#define GPIO_GPCLR0     	10 //W
#define GPIO_GPCLR1     	11 //W
// 12 is RESERVED
#define GPIO_GPLEV0     	13 //R
#define GPIO_GPLEV1     	14 //R
// 15 is RESERVED
#define GPIO_GPEDS0     	16 //R/W
#define GPIO_GPEDS1     	17 //R/W
// 18 is RESERVED
#define GPIO_GPREN0     	19 //R/W
#define GPIO_GPREN1     	20 //R/W
// 21 is RESERVED
#define GPIO_GPFEN0    		22 //R/W
#define GPIO_GPFEN1     	23 //R/W
// 24 is RESERVED
#define GPIO_GPHEN0     	25 //R/W
#define GPIO_GPHEN1     	26 //R/W
// 27 is RESERVED
#define GPIO_GPLEN0     	28 //R/W
#define GPIO_GPLEN1     	29 //R/W
// 30 is RESERVED
#define GPIO_GPAREN0    	31 //R/W
#define GPIO_GPAREN1    	32 //R/W
// 33 is RESERVED
#define GPIO_GPAFEN0    	34 //R/W
#define GPIO_GPAFEN1    	35 //R/W
// 36 is RESERVED
#define GPIO_GPPUD      	37 //R/W
#define GPIO_GPPUDCLK0  	38 //R/W
#define GPIO_GPPUDCLK1  	39 //R/W
// 40 is RESERVED
#define TEST            	44 //R/W *4 bits
//--------------GPIO_BASE------------------

#endif
