/* 
 * File:   compatiblity.h
 * Author: purinda
 *
 * Created on 24 June 2012, 3:08 PM
 */

 /**
 * @file compatibility.h
 * Class declaration for SPI helper files
 */

 /**
 * Example of compatibility.h class declaration for timing functions portability
 *
 * @defgroup Porting_Timing Porting: Timing
 *
 * 
 * @{
 */
 
#ifndef COMPATIBLITY_H
#define	COMPATIBLITY_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

void __msleep(int milisec);
void __usleep(int milisec);
void __start_timer();
long __millis();

#ifdef	__cplusplus
}
#endif

#endif	/* COMPATIBLITY_H */

/*@}*/	