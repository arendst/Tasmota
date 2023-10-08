/*
 * gpio.cpp
 *
 * Created: 20/1/2016 11:57:21
 *  Author: akatran
 */ 

//#include "gpio_helper.h"
#include "gpio.h"
#include <stdlib.h>

void GPIO::open(int port, int DDR)
{
	uint8_t pin;
	PORT_t * p = GPIO_getPort(port,&pin);
	if (DDR==0)
	{
		p->DIRCLR=pin;
	}else if (DDR==1)
	{
		p->DIRSET = pin;	
	}
}

void GPIO::close(int port)
{
	// Nothing to do with close;
}

int read(int port)
{
	uint8_t pin;
	PORT_t * p = GPIO_getPort(port,&pin);
	return p->IN;
}

void GPIO::write(int port,int value)
{
	uint8_t pin;
	PORT_t * p = GPIO_getPort(port,&pin);
	if (value==0)
	{
		p->OUTCLR=pin;
	}else if (value==1)
	{
		p->OUTSET = pin;	
	}
	
}

