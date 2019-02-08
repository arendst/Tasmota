#include "seesaw_servo.h"

#define MIN_PULSE 3277
#define MAX_PULSE 6554

/**************************************************************************/
/*! 
    @brief  begin the seesaw. This is only necessary if the seesaw is not already started
    @param addr the address to begin on
    @param flow the flow control pin to use
    @returns true on success, false otherwise
*/
/**************************************************************************/
bool seesaw_Servo::begin(uint8_t addr, int8_t flow)
{
	return _ss->begin(addr, flow);
}

/**************************************************************************/
/*! 
    @brief  attach the given pin to the next free channel, sets pinMode.
    @param pin the pin to use

    @returns 0
*/
/**************************************************************************/
uint8_t seesaw_Servo::attach(int pin)
{
	_pin = pin;
	//set frequency to 50 hz
	_ss->setPWMFreq(_pin, 50);
	_attached = true;
	min = MIN_PULSE;
	max = MAX_PULSE;
	return 0;
}

/**************************************************************************/
/*! 
    @brief  attach the given pin to the next free channel but also sets min and max values for writes.
    @param pin the pin to use
    @param min the minimum pulse width value in microseconds
    @param max the maximum pulse width value in microseconds
    @returns 0
*/
/**************************************************************************/
uint8_t seesaw_Servo::attach(int pin, int min, int max)
{
	attach(pin);
	this->min = min*3.2767;
	this->max = max*3.2767;

	return 0;
}

/**************************************************************************/
/*! 
    @brief  write a value. if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
    @param  value the value to write
*/
/**************************************************************************/
void seesaw_Servo::write(int value)
{
	if(value < 200){
		//angle
		uint16_t val = map(value, 0, 180, min, max);
		val = constrain(val, min, max);
		_ss->analogWrite(_pin, val);
		_sval = val;
	}
	else
		writeMicroseconds(value);
	
}

/**************************************************************************/
/*! 
    @brief  get current value
    @returns  current pulse width as an angle between 0 and 180 degrees
*/
/**************************************************************************/
int seesaw_Servo::read()
{
	return map(_sval, MIN_PULSE, MAX_PULSE, 0, 180);
}

/**************************************************************************/
/*! 
    @brief  Write pulse width in microseconds 
    @param  value the value to write
*/
/**************************************************************************/
void seesaw_Servo::writeMicroseconds(int value)
{
	uint16_t val = 3.2767 * value;
	_ss->analogWrite(_pin, val);
	_sval = val;
}   