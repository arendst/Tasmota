
#include "compatibility.h"


static uint32_t mtime, seconds, useconds;
//static struct timeval start, end;
struct timespec start, end;

/**********************************************************************/
/**
 * This function is added in order to simulate arduino delay() function
 * @param milisec
 */
void __msleep(int milisec)
{
	struct timespec req;// = {0};
	req.tv_sec = (time_t) milisec / 1000;
	req.tv_nsec = (milisec % 1000 ) * 1000000L;
	//nanosleep(&req, (struct timespec *)NULL);	
	clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);
}

void __usleep(int microsec)
{
	struct timespec req;// = {0};
	req.tv_sec = (time_t) microsec/ 1000000;
	req.tv_nsec = (microsec / 1000000) * 1000;
	//nanosleep(&req, (struct timespec *)NULL);
	clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);	
}

/**
 * This function is added in order to simulate arduino millis() function
 */

 
void __start_timer()
{
	//gettimeofday(&start, NULL);
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
}

uint32_t __millis()
{
	//gettimeofday(&end, NULL);
	clock_gettime(CLOCK_MONOTONIC_RAW,&end);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = (end.tv_nsec - start.tv_nsec)/1000;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;	
	return mtime;
}
