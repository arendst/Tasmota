/*
 * gpio_helper.c
 *
 * Created: 22/1/2016 15:28:48
 *  Author: akatran
 */ 

#include "gpio_helper.h"

/**
* Get the port corresponding in portnum. Default is PORTC.
*/
PORT_t * GPIO_getPort(int pinnum, uint8_t * pin_bm)
//PORT_t * GPIO_getPort(int portnum)
{
	PORT_t * port = &PORTC;
	if ( (pinnum >= XMEGA_PORTA_PIN0) && (pinnum<= XMEGA_PORTA_PIN7) )
	{
		port = &PORTA;
		*pin_bm = (1<<pinnum);
	}else if ( (pinnum >= XMEGA_PORTB_PIN0) && (pinnum<= XMEGA_PORTB_PIN7) )
	{
		port = &PORTB;
		*pin_bm = (1<<(pinnum-8));
	}else if ( (pinnum >= XMEGA_PORTC_PIN0) && (pinnum<= XMEGA_PORTC_PIN7) )
	{
		port = &PORTC;
		*pin_bm = (1<<(pinnum-16));
	}else if ( (pinnum >= XMEGA_PORTD_PIN0) && (pinnum<= XMEGA_PORTD_PIN7) )
	{
		port = &PORTD;
		*pin_bm = (1<<(pinnum-24));
	}else if ( (pinnum >= XMEGA_PORTE_PIN0) && (pinnum<= XMEGA_PORTE_PIN7) )
	{
		port = &PORTE;
		*pin_bm = (1<<(pinnum-32));
	}else if ( (pinnum >= XMEGA_PORTF_PIN0) && (pinnum<= XMEGA_PORTF_PIN7) )
	{
		port = &PORTF;
		*pin_bm = (1<<(pinnum-40));
	}

	return port;

}