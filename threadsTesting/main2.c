/*
    hibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
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
#include "string.h"

#define TEST_WA_SIZE        THD_WA_SIZE(4096)
#define MAILBOX_MAX_ELEMENTS 55
#define TIME_CICLE 					5
#define TIME_DELAY 					2

/*************************************************************************/
/*MAILBOXES  																									          	*/
/*************************************************************************/
Mailbox 		mbp;
msg_t *buf;

Thread* point1;
Thread* point2;

/*************************************************************************/
/*WORKING AREA  																												 */
/*************************************************************************/
static WORKING_AREA(waThread1, 128);
static WORKING_AREA(waThread2,128);

/*************************************************************************/
/*THREADS  																															 */
/*************************************************************************/
/*THREAD 2*/ 
static msg_t Thread2(void *p) {
 	//get the system time 
	systime_t now = chTimeNow();
	(void)p;
	//mailbox inner variables
	msg_t status_send;
	int letter_count = 0;
	msg_t status_receive;
  char buf_receive[10];
	char letter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *n = &(letter[0]);
	
	while(TRUE){
		now += MS2ST(TIME_CICLE);
		//post a message in the mailbox
  //	chprintf((BaseSequentialStream *)&SD1, "%d",letter_count);
		chMBReset(&mbp);
		status_send = chMBPost(&mbp,(msg_t)*(n+letter_count), TIME_IMMEDIATE);//post in mailbox
		if(status_send == RDY_OK){//the post was succesful

			chThdSleepUntil(now);//wait  10 mili to check status
			status_receive = chMBFetch(&mbp, (msg_t*)&buf_receive, TIME_IMMEDIATE);//search for ok

			if(status_receive == RDY_OK){//the fetch was succesful

				//if(strncmp(buf_receive, "ok",2)){//the message receive is the expected
					
					letter_count++;

					if(letter_count == 26)//after posting the z

						letter_count = 0;

				//}else{
  				//chprintf((BaseSequentialStream *)&SD1, "buffer fetch but not ok founded\r\n");
				//}
			}else{
  		chprintf((BaseSequentialStream *)&SD1, "fail fetch 2\r\n");
			}
		}else{
  		chprintf((BaseSequentialStream *)&SD1, "fail post 2\r\n");
		}
	}
  return 0;
}
/*END OF THREAD 2*/ 

/*THREAD 1*/ 
static msg_t Thread1(void *p) {
	//get the system time
	systime_t now = chTimeNow();
  (void)p;
	//mailbox inner variables
	msg_t status_send;
	msg_t status_receive;
  char buf_send[] = "ok" ;
	char receive[26];
  char buf_receive[256];
	while(TRUE){
		chThdSleepMilliseconds(TIME_DELAY);
		now += MS2ST(TIME_CICLE);
		//post a message in the mailbox
		status_receive = chMBFetch(&mbp, (msg_t*)&buf_receive, TIME_INFINITE);
		if(status_receive == RDY_OK){

			strncpy(receive, buf_receive,1);
  		chprintf((BaseSequentialStream *)&SD1, "%s\r\n",receive);
			status_send = chMBPost(&mbp,(msg_t)buf_send, TIME_INFINITE);

			if(status_send == RDY_OK){

				chThdSleepUntil(now);//wait TIME CICLES TO WAKE

			}else{
				
  		chprintf((BaseSequentialStream *)&SD1, "fail post 1\r\n");

			}
		}else{

  		chprintf((BaseSequentialStream *)&SD1, "fail fetch 1\r\n");

		}
	}
  return 0;
}
/*END OF THREAD 1*/ 

/*
 * APPLICATION ENTRY POINT.
 */
int main(void) {
  halInit();
  chSysInit();
	/* 
 	 * SERIAL PORT INITIALIZATION.
   */
  sdStart(&SD1, NULL); 
  
	/*
   * CREATES THE MAILBOX. 
   */
	chMBInit(&mbp, buf, MAILBOX_MAX_ELEMENTS);
	
  /*
   * CREATES THREAD 1 AND 2
   */
  point1 = chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO  , Thread1, NULL);
  point2 = chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO  , Thread2, NULL);

  /*
   * EVENTS SERVICING LOOP.
   */
  chThdWait(chThdSelf());

  return 0;
}
