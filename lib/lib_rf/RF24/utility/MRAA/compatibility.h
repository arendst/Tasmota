/* 
 * File:   compatiblity.h
 * Author: purinda
 *
 * Created on 24 June 2012, 3:08 PM
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

