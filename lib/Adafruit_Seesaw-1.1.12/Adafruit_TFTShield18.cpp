#include "Adafruit_TFTShield18.h"

/**************************************************************************/
/*! 
    @brief  set up the TFT shield
    @param  addr optional address the seesaw chip can be found on
    @param  flow optional flow control pin to use
    @returns true on success, false on error
*/
/**************************************************************************/
bool Adafruit_TFTShield18::begin(uint8_t addr, int8_t flow){
    if (! Adafruit_seesaw::begin(addr, flow)) {
        return false;
    }

    pinMode(TFTSHIELD_RESET_PIN, OUTPUT);
    pinModeBulk(TFTSHIELD_BUTTON_ALL, INPUT_PULLUP);
    return true;
}

/**************************************************************************/
/*! 
    @brief  set the intensity of the backlight
    @param  value to set the backlight to, 
			0x0000 = 0% 	(TFTSHIELD_BACKLIGHT_OFF)
			  to
			0xFFFF = 100%	(TFTSHIELD_BACKLIGHT_ON)

*/
/**************************************************************************/
void Adafruit_TFTShield18::setBacklight(uint16_t value){
    uint8_t cmd[] = {0x00, (uint8_t)(value >> 8), (uint8_t)value};
    this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_PWM, cmd, 3);
}

/**************************************************************************/
/*! 
    @brief  set the PWM frequency for the backlight
    @param  freq the frequency to set the backlight to
*/
/**************************************************************************/
void Adafruit_TFTShield18::setBacklightFreq(uint16_t freq){
    uint8_t cmd[] = {0x0, (uint8_t)(freq >> 8), (uint8_t)freq};
	this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_FREQ, cmd, 3);
}

/**************************************************************************/
/*! 
    @brief  reset the TFT screen by setting the value of the reset pin
    @param  rst the value to set the reset pin to
*/
/**************************************************************************/
void Adafruit_TFTShield18::tftReset(bool rst){
   digitalWrite(TFTSHIELD_RESET_PIN, rst); 
}

/**************************************************************************/
/*! 
    @brief  read all buttons on the wing and return as a 32 bit integer
    @returns the value of the buttons
*/
/**************************************************************************/
uint32_t Adafruit_TFTShield18::readButtons(){
    return digitalReadBulk(TFTSHIELD_BUTTON_ALL);
}