/*
 * spi.cpp
 *
 * Created: 20/1/2016 10:10:39
 *  Author: akatran
 */ 

#include <avr/io.h>
#include "gpio_helper.h"
#include "spi.h"

using namespace std;

void SPI::begin(uint8_t _port)
{
	if (_port==XMEGA_SPI_PORT_C) // Select SPI on PORTC
	{
		device = &SPIC;
		port = &PORTC;
	}else if (_port==XMEGA_SPI_PORT_D) // Select SPI on PORTD
	{
		device = &SPID;
		port = &PORTD;
	}
	
	init();
}

uint8_t SPI::transfer(uint8_t tx_)
{
	register8_t data;
	device->DATA = tx_;
	while(!(device->STATUS & (1<<7)));
	data = device->DATA;
	//PORTF.OUT = data;
	return data;
}

void SPI::init()
{
	port->DIRCLR = SPI_MISO_bm;
	port->DIRSET = SPI_MOSI_bm | SPI_SCK_bm | SPI_SS_bm;
	
	//device->CTRL = 0;
	device->CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV4_gc;
	device->INTCTRL =0; //Disable interrupts

}


SPI::SPI()
{
	
}

SPI::~SPI()
{
 	
}

void operator delete(void * p) // or delete(void *, std::size_t)
{

}