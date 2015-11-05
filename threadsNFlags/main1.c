#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"
#include "chprintf.h"
#include <chbsem.h>

#define SHELL_WA_SIZE       THD_WA_SIZE(4096)

#define TEST_WA_SIZE        THD_WA_SIZE(4096);
static BSEMAPHORE_DECL(semaforo, 0);

static WORKING_AREA(waThread1, 128);
static WORKING_AREA(waThread2, 128);
static WORKING_AREA(waThread3, 128);

static msg_t Thread1(void *p) {
  (void)p;
  chRegSetThreadName("tread1");
  while(1){
    chBSemWait(&semaforo);
    chprintf((BaseSequentialStream *)&SD1, "This is thread1\r\n");
    chBSemSignal(&semaforo);
  }
  return 0;
}

static msg_t Thread2(void *p) {
  (void)p;
  chRegSetThreadName("thread2");
  while(1){
    chBSemWait(&semaforo);
    chprintf((BaseSequentialStream *)&SD1, "This is thread2\r\n");
    chBSemSignal(&semaforo);
  }
  return 0;
}

static msg_t Thread3(void *p) {
  (void)p;
  chRegSetThreadName("thread3");
  while(1){
    chBSemWait(&semaforo);
    chprintf((BaseSequentialStream *)&SD1, "This is thread3\r\n");
    chBSemSignal(&semaforo);
  }
  return 0;
}


/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();
  //chBSemInit(&semaforo, FALSE); //revisar por pointer

  /*
   * Serial port initialization.
   */
  sdStart(&SD1, NULL); 
  chprintf((BaseSequentialStream *)&SD1, "Main (SD1 started)\r\n");

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO, Thread3, NULL);

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
