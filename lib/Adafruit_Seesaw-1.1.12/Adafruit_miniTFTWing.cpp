#include "Adafruit_miniTFTWing.h"

/**************************************************************************/
/*! 
    @brief  set up the miniTFTWing
    @param  addr optional address the seesaw chip can be found on
    @param  flow optional flow control pin to use
    @returns true on success, false on error
*/
/**************************************************************************/
bool Adafruit_miniTFTWing::begin(uint8_t addr, int8_t flow) {
  if (! Adafruit_seesaw::begin(addr, flow)) {
    return false;
  }

  pinMode(TFTWING_RESET_PIN, OUTPUT);
  pinModeBulk(TFTWING_BUTTON_ALL, INPUT_PULLUP);
  return true;
}

/**************************************************************************/
/*! 
    @brief  set the value of the backlight
    @param  value the backlight value to set NOTE: 0xFFFF is all the way on 0x0000 is off.
*/
/**************************************************************************/
void Adafruit_miniTFTWing::setBacklight(uint16_t value){
    uint8_t cmd[] = {0x00, (uint8_t)(value >> 8), (uint8_t)value};
    this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_PWM, cmd, 3);
}

/**************************************************************************/
/*! 
    @brief  set the PWM frequency for the backlight
    @param  freq the frequency to set the backlight to
*/
/**************************************************************************/
void Adafruit_miniTFTWing::setBacklightFreq(uint16_t freq){
    uint8_t cmd[] = {0x0, (uint8_t)(freq >> 8), (uint8_t)freq};
	this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_FREQ, cmd, 3);
}

/**************************************************************************/
/*! 
    @brief  reset the TFT screen by setting the value of the reset pin
    @param  rst the value to set the reset pin to
*/
/**************************************************************************/
void Adafruit_miniTFTWing::tftReset(bool rst){
   digitalWrite(TFTWING_RESET_PIN, rst); 
}

/**************************************************************************/
/*! 
    @brief  read all buttons on the wing and return as a 32 bit integer
    @returns the value of the buttons
*/
/**************************************************************************/
uint32_t Adafruit_miniTFTWing::readButtons(){
    return digitalReadBulk(TFTWING_BUTTON_ALL);
}
