/*
 * gpio_helper.h
 *
 * Created: 22/1/2016 15:29:12
 *  Author: akatran
 */ 


#ifndef GPIO_HELPER_H_
#define GPIO_HELPER_H_

#include <avr/io.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* Defines */


#define XMEGA_PORTA_PIN0   0
#define XMEGA_PORTA_PIN1   1
#define XMEGA_PORTA_PIN2   2
#define XMEGA_PORTA_PIN3   3
#define XMEGA_PORTA_PIN4   4
#define XMEGA_PORTA_PIN5   5
#define XMEGA_PORTA_PIN6   6
#define XMEGA_PORTA_PIN7   7

#define XMEGA_PORTB_PIN0   8
#define XMEGA_PORTB_PIN1   9
#define XMEGA_PORTB_PIN2  10
#define XMEGA_PORTB_PIN3  11
#define XMEGA_PORTB_PIN4  12
#define XMEGA_PORTB_PIN5  13
#define XMEGA_PORTB_PIN6  14
#define XMEGA_PORTB_PIN7  15

#define XMEGA_PORTC_PIN0  16
#define XMEGA_PORTC_PIN1  17
#define XMEGA_PORTC_PIN2  18
#define XMEGA_PORTC_PIN3  19
#define XMEGA_PORTC_PIN4  20
#define XMEGA_PORTC_PIN5  21
#define XMEGA_PORTC_PIN6  22
#define XMEGA_PORTC_PIN7  23

#define XMEGA_PORTD_PIN0  24
#define XMEGA_PORTD_PIN1  25
#define XMEGA_PORTD_PIN2  26
#define XMEGA_PORTD_PIN3  27
#define XMEGA_PORTD_PIN4  28
#define XMEGA_PORTD_PIN5  29
#define XMEGA_PORTD_PIN6  30
#define XMEGA_PORTD_PIN7  31

#define XMEGA_PORTE_PIN0  32
#define XMEGA_PORTE_PIN1  33
#define XMEGA_PORTE_PIN2  34
#define XMEGA_PORTE_PIN3  35
#define XMEGA_PORTE_PIN4  36
#define XMEGA_PORTE_PIN5  37
#define XMEGA_PORTE_PIN6  38
#define XMEGA_PORTE_PIN7  39

#define XMEGA_PORTF_PIN0  40
#define XMEGA_PORTF_PIN1  41
#define XMEGA_PORTF_PIN2  42
#define XMEGA_PORTF_PIN3  43
#define XMEGA_PORTF_PIN4  44
#define XMEGA_PORTF_PIN5  45
#define XMEGA_PORTF_PIN6  46
#define XMEGA_PORTF_PIN7  47



#define XMEGA_SPI_PORT_C 20
#define XMEGA_SPI_PORT_D 28


//void GPIO_getPort(int pinnum, PORT_t * port, uint8_t pin);
//void GPIO_getPort(int pinnum, PORT_t * port, uint8_t * pin_bm);
PORT_t * GPIO_getPort(int pinnum, uint8_t * pin_bm);

#ifdef	__cplusplus
}
#endif

#endif /* GPIO_HELPER_H_ */