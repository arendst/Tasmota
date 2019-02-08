#ifndef _SEESAW_MOTOR_H
#define _SEESAW_MOTOR_H

#include "Adafruit_seesaw.h"

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for seesaw motor interface
*/
/**************************************************************************/
class seesaw_Motor {
public:
	 /**************************************************************************/
	/*! 
	  @brief  class constructor
	  @param ss the seesaw object to use
	*/
	/**************************************************************************/
	seesaw_Motor(Adafruit_seesaw *ss) {
		_ss = ss;
		_pina = -1;
		_pinb = -1;
		_throttle = 0;
	}

	~seesaw_Motor() {}

	/**************************************************************************/
	/*! 
	  @brief  attach the motor to the specified PWM pins
	  @param pina the positive pin to use
	  @param pinb the negative pin to use
	*/
	/**************************************************************************/
	void attach(int pina, int pinb){
		_pina = pina;
		_pinb = pinb;
	}

	/**************************************************************************/
	/*! 
	  @brief  set the throttle
	  @param value the throttle value to set between -1 and 1. Passing 0 will turn the motor off.
	*/
	/**************************************************************************/
	void throttle(float value){
		if(_pina < 0 || _pinb < 0) return;

		value = constrain(value, -1.0, 1.0);
		_throttle = value;
		uint16_t absolute = fabs(value) * 65535;

		if(value > 0){
			_ss->analogWrite(_pina, 0);
			_ss->analogWrite(_pinb, absolute);
		}
		else if(value < 0){
			_ss->analogWrite(_pina, absolute);
			_ss->analogWrite(_pinb, 0);
		}
		else{
			//both are off
			_ss->analogWrite(_pina, 0);
			_ss->analogWrite(_pinb, 0);
		}
	}

	/**************************************************************************/
	/*! 
	  @brief  get the current throttle value
	  @returns the current throttle value between -1 and 1
	*/
	/**************************************************************************/
	float getThrottle(){
		return _throttle;
	}

private:
	Adafruit_seesaw *_ss;
	int8_t _pina, _pinb;
	float _throttle;
};

#endif