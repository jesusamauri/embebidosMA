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
#define BSEMAPHORE_DECL (SemSerialPort, 0)

/*************************************************************************/
/*WORKING AREA  																												 */
/*************************************************************************/
Thread* point1;
Thread* point2;

static WORKING_AREA(waThread1, 128);
static WORKING_AREA(waThread2,128);

/*************************************************************************/
/*THREADS  																															 */
/*************************************************************************/
/*THREAD 2*/ 
static msg_t Thread2(void *p) {
  (void)p;
  chRegSetThreadName("blinker2");
  chprintf((BaseSequentialStream *)&SD1, "This is 2\r\n");
  return 0;
}
/*END OF THREAD 2*/ 

/*THREAD 1*/ 
static msg_t Thread1(void *p) {
  (void)p;
  chRegSetThreadName("blinker");
  chprintf((BaseSequentialStream *)&SD1, "This is 1\r\n");
  return 0;
}
/*END OF THREAD 1*/ 

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
	chBSemIni(SemSerialPort, 0);

  /*
   * CREATES THREADS 1 AND 2
   */
  point1 = chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO  , Thread1, NULL);
  point2 = chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO + 1 , Thread2, NULL);

  /*
   * EVENTS SERVICING LOOP.
   */
  chThdWait(chThdSelf());

  return 0;
}
