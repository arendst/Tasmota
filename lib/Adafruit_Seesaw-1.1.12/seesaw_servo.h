#ifndef _SEESAW_SERVO_H
#define _SEESAW_SERVO_H

#include "Adafruit_seesaw.h"

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for seesaw servo interface
*/
/**************************************************************************/
class seesaw_Servo {

 public:
  /**************************************************************************/
  /*! 
      @brief  class constructor
      @param ss the seesaw object to use
  */
  /**************************************************************************/
  seesaw_Servo(Adafruit_seesaw *ss) {
  	_ss = ss;
  	_attached = false;
  }

  ~seesaw_Servo() {}
  bool begin(uint8_t addr = SEESAW_ADDRESS, int8_t flow=-1);

  uint8_t attach(int pin);           
  uint8_t attach(int pin, int min, int max);

  /**************************************************************************/
  /*! 
      @brief  set attached to false
  */
  /**************************************************************************/
  void detach() { _attached = false; }
  void write(int value);             
  void writeMicroseconds(int value);
  int read();                        

  /**************************************************************************/
  /*! 
      @brief  get current value in microseconds
      @returns  current pulse width in microseconds for this servo
  */
  /**************************************************************************/
  int readMicroseconds() { return _sval/3.2768; }
  
  /**************************************************************************/
  /*! 
      @brief  check if the servo is attached yet
      @returns  true if this servo is attached, otherwise false 
  */
  /**************************************************************************/
  bool attached() { return _attached; }

private:
   Adafruit_seesaw *_ss;
   bool _attached;
   uint16_t _sval;
   uint8_t _pin;
   uint16_t min;   
   uint16_t max;  
};

#endif