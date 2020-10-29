/* 
 * TMRh20 2015
 * 
 */

#include "gpio.h"

GPIO::GPIO() {
	// Prophet: basic members initialization
	gpio_ce_pin = -1;
	//gpio_cs_pin = -1;
	gpio_0 = NULL;
	//gpio_1 = NULL;
}

GPIO::~GPIO() {
	// Prophet: this should free memory, and unexport pins when RF24 and/or GPIO gets deleted or goes out of scope
	this->close(gpio_ce_pin);
	//this->close(gpio_cs_pin);
}

void GPIO::begin(uint8_t ce_pin, uint8_t cs_pin)
{	
	gpio_ce_pin = ce_pin;
	//gpio_cs_pin = cs_pin;
	
	// Prophet: owner can be set here, because we use our pins exclusively, and are making mraa:Gpio context persistent
	// so pins will be unexported only if close is called, or on destruction
	gpio_0 = new mraa::Gpio(ce_pin/*,0*/);
	//gpio_1 = new mraa::Gpio(cs_pin/*,0*/);
}
void GPIO::open(int port, int DDR)
{		
	if(port == gpio_ce_pin){
		gpio_0 = new mraa::Gpio(port,0);
		gpio_0->useMmap(true);
		gpio_0->dir( (mraa::Dir)DDR);
	}/*else
	if(port == gpio_cs_pin){
		gpio_1 = new mraa::Gpio(port,0);
		gpio_1->useMmap(true);
		gpio_1->dir( (mraa::Dir)DDR);
	}*/		
}

void GPIO::close(int port)
{	
	// Prophet: using same theme of working with port numbers as with GPIO::open,
	// checking for mraa::Gpio context existence to be sure, that GPIO::begin was called
	if(port == gpio_ce_pin)
	{
		if (gpio_0 != NULL)	{
			delete gpio_0;
		}
	}

	/*if(port == gpio_cs_pin) {
		if (gpio_1 != NULL)	{
			delete gpio_1;
		}
	}*/
}

int GPIO::read(int port)
{	
	if(port == gpio_ce_pin){
		return gpio_0->read();
	}/*else
	if(port == gpio_cs_pin){
		return gpio_1->read();
	}*/
	return -1;
}

void GPIO::write(int port, int value){
	
	if(port == gpio_ce_pin){
		gpio_0->write( value);
	}/*else
	if(port == gpio_cs_pin){
		gpio_1->write( value);
	}*/
}
