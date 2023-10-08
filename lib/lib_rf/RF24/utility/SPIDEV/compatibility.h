/* 
 * File:   compatiblity.h
 * Author: purinda
 *
 * Created on 24 June 2012, 3:08 PM
 * patch for safer monotonic clock & millis() correction for 64bit LDV 2018
 */

#ifndef COMPATIBLITY_H
#define	COMPATIBLITY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>  // for uintXX_t types	
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

void __msleep(int milisec);
void __usleep(int milisec);
void __start_timer();
uint32_t __millis();

#ifdef	__cplusplus
}
#endif

#endif	/* COMPATIBLITY_H */

