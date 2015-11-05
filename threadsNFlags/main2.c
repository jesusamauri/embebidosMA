/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"
#include "chprintf.h"

#define TEST_WA_SIZE        THD_WA_SIZE(4096)
static BSEMAPHORE_DECL (SemSerialPort, 0);

/*************************************************************************/
/*WORKING AREA  																												 */
/*************************************************************************/
Thread* point1;
Thread* point2;
Thread* point3;
Thread* point4;
Thread* point5;
Thread* point6;

static WORKING_AREA(waThread1, 128);
static WORKING_AREA(waThread2,128);
static WORKING_AREA(waThread3,128);
static WORKING_AREA(waThread4,128);
static WORKING_AREA(waThread5,128);
static WORKING_AREA(waThread6,128);

/*************************************************************************/
/*THREADS  																															 */
/*************************************************************************/
/*THREAD 1*/ 
static msg_t Thread1(void *p) {
  (void)p;
	int contThread1 = 0;
  chRegSetThreadName("blinker");

		while(contThread1 < 10000){
			contThread1++;
		}
		chEvtSignal(point3, 3); /*TODO*/ 
		chEvtSignal(point4, 3); /*TODO*/ 
		contThread1 = 0;

  return 0;
}
/*END OF THREAD 1*/ 

/*THREAD 2*/ 
static msg_t Thread2(void *p) {
  (void)p;
  chRegSetThreadName("blinker2");

	uint32_t buttA;
	while(1){

		buttA = palReadPad(GPIO23_PORT, GPIO23_PAD);
		if(buttA){
			chEvtSignal(point5, 5);
			chEvtSignal(point6, 5);
		}
	}	
  return 0;
}
/*END OF THREAD 2*/ 

/*THREAD 3*/ 
static msg_t Thread3(void *p) {
  (void)p;
  chRegSetThreadName("blinker3");
	chEvtWaitOne(3);
  palSetPad(GPIO27_PORT, GPIO27_PAD);

  return 0;
}
/*END OF THREAD 3*/ 

/*THREAD 4*/ 
static msg_t Thread4(void *p) {
  (void)p;
  chRegSetThreadName("blinker4");
	chEvtWaitOne(3);
	chBSemWait(&SemSerialPort);
  chprintf((BaseSequentialStream *)&SD1, "EVENT 1 HAS OCCURRED\r\n");
	chBSemSignal(&SemSerialPort);
  return 0;
}
/*END OF THREAD 4*/ 

/*THREAD 5*/ 
static msg_t Thread5(void *p) {
  (void)p;
  chRegSetThreadName("blinker5");
	chEvtWaitOne(5);
  palSetPad(GPIO22_PORT, GPIO22_PAD);
  return 0;
}
/*END OF THREAD 5*/ 

/*THREAD 6*/ 
static msg_t Thread6(void *p) {
  (void)p;
  chRegSetThreadName("blinker6");
	chEvtWaitOne(5);
	chBSemWait(&SemSerialPort);
  chprintf((BaseSequentialStream *)&SD1, "EVENT 2 HAS OCCURRED\r\n");
	chBSemSignal(&SemSerialPort);
  return 0;
}
/*END OF THREAD 6*/ 



/*
 * APPLICATION ENTRY POINT.
 */
int main(void) {
  halInit();
  chSysInit();
/* * SERIAL PORT INITIALIZATION.
   */
  sdStart(&SD1, NULL); 
  chprintf((BaseSequentialStream *)&SD1, "Main (SD1 started)\r\n");
  /*
   * CREATES SEMAPHORE 
   */
	/*chBSemInit(SemSerialPort, 0);*/

  /*
   *DECLARE I/O
   */
  palSetPadMode(GPIO27_PORT, GPIO27_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO22_PORT, GPIO22_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO23_PORT, GPIO23_PAD, PAL_MODE_INPUT);

  /*
   * CREATES THREADS 1 AND 2
   */
  point1 = chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO  , Thread1, NULL);
  point2 = chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO  , Thread2, NULL);
  point3 = chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO  , Thread3, NULL);
  point4 = chThdCreateStatic(waThread4, sizeof(waThread4), NORMALPRIO  , Thread4, NULL);
  point5 = chThdCreateStatic(waThread5, sizeof(waThread5), NORMALPRIO  , Thread5, NULL);
  point6 = chThdCreateStatic(waThread6, sizeof(waThread6), NORMALPRIO  , Thread6, NULL);

  /*
   * EVENTS SERVICING LOOP.
   */
  chThdWait(chThdSelf());

  return 0;
}
