/*
  Keeloq.h - Crypto library
  Written by Frank Kienast in November, 2010
*/
#ifndef Keeloq_lib_h
#define Keeloq_lib_h
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Keeloq
{
  public:
    Keeloq(
		const unsigned long keyHigh,
		const unsigned long keyLow );
		
    unsigned long encrypt( const unsigned long data );
    unsigned long decrypt( const unsigned long data );
    
  private:
    unsigned long _keyHigh;
	unsigned long _keyLow;
};

#endif		/*Keeloq_lib_h*/
