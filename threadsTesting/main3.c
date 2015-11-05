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
#include "string.h"

#define TEST_WA_SIZE        THD_WA_SIZE(4096)
#define MAILBOX_MAX_ELEMENTS 55
#define TIME_CICLE 					500
#define TIME_DELAY 					100

/*************************************************************************/
/*MAILBOXES  																									          	*/
/*************************************************************************/
Mailbox mbp;
msg_t* 	buf;

/*************************************************************************/
/*FUNCTION DECLARATION  																						  		*/
/*************************************************************************/
int getDirection(uint32_t buttA, uint32_t buttB, uint32_t buttC, uint32_t buttD);

/*************************************************************************/
/*WORKING AREA  																												 */
/*************************************************************************/
static WORKING_AREA(waThread1, 128);
static WORKING_AREA(waThread2,128);
//POINTERS TO THREAD
Thread* point1;
Thread* point2;

/*************************************************************************/
/*THREADS  																															 */
/*************************************************************************/

/*THREAD 2*/ 
static msg_t Thread2(void *p) {
 	//GET THE SYSTEM TIME 
	systime_t now = chTimeNow();
	//MAILBOX HAVE TO BE INITILIZE IN THIS THREAD OTHERWISE IT WILL NOT WORK
  chprintf((BaseSequentialStream *)&SD1, "Begging\r\n");
	(void)p;
	// VARIABLE FOR  IO ports
	uint32_t buttA;
	uint32_t buttB;
	uint32_t buttC;
	uint32_t buttD;
	//SET IO PORTSJ
  palSetPadMode(GPIO27_PORT, GPIO27_PAD, PAL_MODE_INPUT);
  palSetPadMode(GPIO22_PORT, GPIO22_PAD, PAL_MODE_INPUT);
  palSetPadMode(GPIO23_PORT, GPIO23_PAD, PAL_MODE_INPUT);
  palSetPadMode(GPIO24_PORT, GPIO24_PAD, PAL_MODE_INPUT);
	//
	//MAILBOX INNER VARIABLES
	msg_t statusSend;
	msg_t statusReceive;
  char bufReceive[10];
	int dir;
	char cDir;
	//	
	while(TRUE){
		now += MS2ST(TIME_CICLE);
		//get IO PORTS values
		buttA = palReadPad(GPIO27_PORT, GPIO27_PAD);
		buttB = palReadPad(GPIO22_PORT, GPIO22_PAD);
		buttC = palReadPad(GPIO23_PORT, GPIO23_PAD);
		buttD = palReadPad(GPIO24_PORT, GPIO24_PAD);
		//GET DIRECTION 
		dir = getDirection(buttA, buttB, buttC, buttD);
		cDir = dir;
		
		//POST A MESSAGE IN THE MAILBOX
  	//CHPRINTF((BASESEQUENTIALSTREAM *)&SD1, "%D",LETTER_COUNT);
		chMBReset(&mbp);//Reset the mailbox otherwise will cause troubles
		statusSend = chMBPost(&mbp,(msg_t)cDir, TIME_IMMEDIATE);//post in mailbox
	
		if(statusSend == RDY_OK){//the post was succesful

			//SLEEP UNTIL TIME_CICLE IS ACHIEVED
			chThdSleepUntil(now);
			//SUCESSFULL FETCH OF MESSAGE FROM THREAD 1
			statusReceive = chMBFetch(&mbp, (msg_t*)&bufReceive, TIME_IMMEDIATE);
			//OK MESSAGE INSIDE ANSWER FROM THREAD1
			if(statusReceive == RDY_OK){
				
					//SUCESSFULL FETCH OF ANSWER

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
	//GET THE SYSTEM TIME
	systime_t now = chTimeNow();
  (void)p;
	//MAILBOX INNER VARIABLES
	msg_t statusSend;
	msg_t statusReceive;
  char buf_send[] = "ok" ;
	char receive[26];
  char bufReceive[26];

	while(TRUE){
		//DELAY TO AVOID MIXING DATA
		chThdSleepMilliseconds(TIME_DELAY);
		//FIXED TIME TO WAKE UP EVERY TIME_CICLE
		now += MS2ST(TIME_CICLE);
		//POST A MESSAGE IN THE MAILBOX
		statusReceive = chMBFetch(&mbp, (msg_t*)&bufReceive, TIME_INFINITE);

		if(statusReceive == RDY_OK){
			//CONVERT INTEGER TO PREDEFINED STRING
			strcpy(receive, bufReceive);
			if(receive[0] ==  '0'){
				
				strcpy(receive, "up");

			}
			else if(receive[0] ==  '1'){
				
				strcpy(receive , "down");

			}
			else if(receive[0] ==  '2'){
				
				strcpy(receive , "left");

			}
			else if(receive[0] ==  '3'){
				
				strcpy(receive , "right");

			}
			else if(receive[0] ==  '4'){
				
				strcpy(receive , "-");

			}
			else{
				
				strcpy(receive , "error");

			}
			//PRINT DIRECTION
  		chprintf((BaseSequentialStream *)&SD1, "%s\r\n",receive);
			//SEND A MESSAGE OF RECEIVE PACKET
			statusSend = chMBPost(&mbp,(msg_t)buf_send, TIME_INFINITE);
			//SEND PACKET WAS SUCCESFULLY SEND
			if(statusSend == RDY_OK){
				//SLEEP UNTIL TIME_CICLE IS ACHIEVED
				chThdSleepUntil(now);

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
  chprintf((BaseSequentialStream *)&SD1, "Main (SD1 started)\r\n");

	/*
   * CREATES THE MAILBOX. 
   */
	chThdSleepMilliseconds(TIME_DELAY);
	chMBInit(&mbp, buf, MAILBOX_MAX_ELEMENTS);

  /*
   * CREATES THREADS
   */
  point1 = chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  point2 = chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
  /*
   * EVENTS SERVICING LOOP.
   */
  chThdWait(chThdSelf());

  return 0;
}
//CHECK WHICH SIGNAL IS ON AND SEND AN INT FROM O TO 4
int getDirection(uint32_t buttA, uint32_t buttB, uint32_t buttC, uint32_t buttD){

	if(buttA)
		return 48;
	else if(buttB)
		return 49;
	else if(buttC)
		return 50;
	else if(buttD)
		return 51;	
return 52;

}
	
