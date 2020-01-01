/*
 * https://github.com/mrshu/GPIOlib
 * Copyright (c) 2011, Copyright (c) 2011 mr.Shu
 * All rights reserved.
 *
 * Modified on 24 June 2012, 11:06 AM
 * File:   gpio.h
 * Author: purinda (purinda@gmail.com)
 *
 */

#ifndef H
#define	H

#include <cstdio>
#include <map>
#include <stdexcept>

/** Specific excpetion for SPI errors */
class GPIOException : public std::runtime_error {
	public:
		explicit GPIOException(const std::string& msg) :  std::runtime_error(msg) { }
};

/**
 * @file gpio.h
 * \cond HIDDEN_SYMBOLS
 * Class declaration for GPIO helper files
 */


/**
 * Example GPIO.h file
 *
 * @defgroup GPIO GPIO Example
 *
 * See RF24_arch_config.h for additional information
 * @{
 */

typedef int GPIOfdCache_t;

class GPIO {
public:

	/* Constants */
	static const int DIRECTION_OUT = 1;
	static const int DIRECTION_IN = 0;

	static const int OUTPUT_HIGH = 1;
	static const int OUTPUT_LOW = 0;

	GPIO();

	/**
	 * Similar to Arduino pinMode(pin,mode);
     * @param port
     * @param DDR
     */
	static void open(int port, int DDR);
	/**
	 *
     * @param port
     */
	static void close(int port);
	/**
	 * Similar to Arduino digitalRead(pin);
     * @param port
     * @param value
     */
	static int read(int port);
	/**
	* Similar to Arduino digitalWrite(pin,state);
	* @param port
	* @param value
	*/
	static void write(int port,int value);

	virtual ~GPIO();

private:
  /* fd cache */
  static std::map<int,GPIOfdCache_t> cache;
};
/**
 * \endcond
 */
/*@}*/
#endif	/* H */
