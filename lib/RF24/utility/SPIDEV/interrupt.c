/*
Interrupts functions extruded from wiringPi library by Oitzu.

wiringPi Copyright (c) 2012 Gordon Henderson
https://projects.drogon.net/raspberry-pi/wiringpi
wiringPi is free software: GNU Lesser General Public License
see <http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/stat.h>
#include "interrupt.h"
#include <pthread.h>

//#define delay(x) bcm2835_delay(x)

static pthread_mutex_t pinMutex = PTHREAD_MUTEX_INITIALIZER ;
static volatile int    pinPass = -1 ;

pthread_t threadId [64];

// sysFds:
//      Map a file descriptor from the /sys/class/gpio/gpioX/value
static int sysFds [64] =
{
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
} ;

// ISR Data
static void (*isrFunctions [64])(void) ;

int waitForInterrupt (int pin, int mS)
{
  int fd, x ;
  uint8_t c ;
  struct pollfd polls ;

  if ((fd = sysFds [pin]) == -1)
    return -2 ;

// Setup poll structure

  polls.fd     = fd ;
  polls.events = POLLPRI ;      // Urgent data!

// Wait for it ...
  x = poll (&polls, 1, mS) ;

// Do a dummy read to clear the interrupt
//      A one character read appars to be enough.
//      Followed by a seek to reset it.

  (void)read (fd, &c, 1) ;
  lseek (fd, 0, SEEK_SET) ;

  return x ;
}


int piHiPri (const int pri)
{
  struct sched_param sched ;

  memset (&sched, 0, sizeof(sched)) ;

  if (pri > sched_get_priority_max (SCHED_RR))
    sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
  else
    sched.sched_priority = pri ;

  return sched_setscheduler (0, SCHED_RR, &sched) ;
}


void *interruptHandler (void *arg)
{
  int myPin ;

  (void)piHiPri (55) ;  // Only effective if we run as root

  myPin   = pinPass ;
  pinPass = -1 ;

  for (;;)
    if (waitForInterrupt (myPin, -1) > 0){
      pthread_mutex_lock (&pinMutex) ;
      isrFunctions [myPin] () ;
      pthread_mutex_unlock (&pinMutex) ;
	  pthread_testcancel(); //Cancel at this point if we have an cancellation request.
    }

  return NULL ;
}


int attachInterrupt (int pin, int mode, void (*function)(void))
{
  const char *modeS ;
  char fName   [64] ;
  char  pinS [8] ;
  pid_t pid ;
  int   count, i ;
  char  c ;
  int   bcmGpioPin ;

  bcmGpioPin = pin ;

  if (mode != INT_EDGE_SETUP)
  {
    /**/ if (mode == INT_EDGE_FALLING)
      modeS = "falling" ;
    else if (mode == INT_EDGE_RISING)
      modeS = "rising" ;
    else
      modeS = "both" ;

    sprintf (pinS, "%d", bcmGpioPin) ;

    if ((pid = fork ()) < 0)    // Fail
      return printf("wiringPiISR: fork failed: %s\n", strerror (errno)) ;

    if (pid == 0)       // Child, exec
    {
      /**/ if (access ("/usr/local/bin/gpio", X_OK) == 0)
      {
        execl ("/usr/local/bin/gpio", "gpio", "edge", pinS, modeS, (char *)NULL) ;
        return printf ("wiringPiISR: execl failed: %s\n", strerror (errno)) ;
      }
      else if (access ("/usr/bin/gpio", X_OK) == 0)
      {
        execl ("/usr/bin/gpio", "gpio", "edge", pinS, modeS, (char *)NULL) ;
        return printf ("wiringPiISR: execl failed: %s\n", strerror (errno)) ;
      }
      else
        return printf ("wiringPiISR: Can't find gpio program\n") ;
    }
    else                // Parent, wait
      wait (NULL) ;
  }

  if (sysFds [bcmGpioPin] == -1)
  {
    sprintf (fName, "/sys/class/gpio/gpio%d/value",bcmGpioPin);
    if ((sysFds [bcmGpioPin] = open (fName, O_RDWR)) < 0)
      return printf ("wiringPiISR: unable to open %s: %s\n", fName, strerror (errno)) ;
  }

    ioctl (sysFds [bcmGpioPin], FIONREAD, &count) ;
  for (i = 0 ; i < count ; ++i)
    read (sysFds [bcmGpioPin], &c, 1) ;

  isrFunctions [pin] = function ;

  pthread_mutex_lock (&pinMutex) ;
    pinPass = pin ;
    pthread_create (&threadId[bcmGpioPin], NULL, interruptHandler, NULL) ;
    while (pinPass != -1)
      delay (1) ;
  pthread_mutex_unlock (&pinMutex) ;

  return 0 ;
}

int detachInterrupt (int pin)
{
	char  pinS [8];
    const char *modeS = "none";
	pid_t pid ;
	
	if (!pthread_cancel(threadId[pin])) //Cancel the thread
	{
	 return 0;
	}
	
	if (!close(sysFds[pin])) //Close filehandle
	{
		return 0;
	}
	
	/* Set wiringPi to 'none' interrupt mode */

    sprintf (pinS, "%d", pin) ;

    if ((pid = fork ()) < 0)    // Fail
      return printf("wiringPiISR: fork failed: %s\n", strerror (errno)) ;

    if (pid == 0)       // Child, exec
    {
      /**/ if (access ("/usr/local/bin/gpio", X_OK) == 0)
      {
        execl ("/usr/local/bin/gpio", "gpio", "edge", pinS, modeS, (char *)NULL) ;
        return printf ("wiringPiISR: execl failed: %s\n", strerror (errno)) ;
      }
      else if (access ("/usr/bin/gpio", X_OK) == 0)
      {
        execl ("/usr/bin/gpio", "gpio", "edge", pinS, modeS, (char *)NULL) ;
        return printf ("wiringPiISR: execl failed: %s\n", strerror (errno)) ;
      }
      else
        return printf ("wiringPiISR: Can't find gpio program\n") ;
    }
    else                // Parent, wait
      wait (NULL) ;
	  
	return 1;
}

void rfNoInterrupts(){
  pthread_mutex_lock (&pinMutex) ;
}

void rfInterrupts(){
  pthread_mutex_unlock (&pinMutex) ;
}