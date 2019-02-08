#ifndef _MINI_TFT_WING_H
#define _MINI_TFT_WING_H

#include "Adafruit_seesaw.h"

#define TFTWING_ADDR 0x5E
#define TFTWING_RESET_PIN 8

#define TFTWING_BACKLIGHT_ON  0       // inverted output!
#define TFTWING_BACKLIGHT_OFF 0xFFFF  // inverted output!


#define TFTWING_BUTTON_UP_PIN 2
#define TFTWING_BUTTON_UP (1UL << TFTWING_BUTTON_UP_PIN)

#define TFTWING_BUTTON_DOWN_PIN 4
#define TFTWING_BUTTON_DOWN (1UL << TFTWING_BUTTON_DOWN_PIN)

#define TFTWING_BUTTON_LEFT_PIN 3
#define TFTWING_BUTTON_LEFT (1UL << TFTWING_BUTTON_LEFT_PIN)

#define TFTWING_BUTTON_RIGHT_PIN 7
#define TFTWING_BUTTON_RIGHT (1UL << TFTWING_BUTTON_RIGHT_PIN)

#define TFTWING_BUTTON_SELECT_PIN 11
#define TFTWING_BUTTON_SELECT (1UL << TFTWING_BUTTON_SELECT_PIN)

#define TFTWING_BUTTON_A_PIN 10
#define TFTWING_BUTTON_A (1UL << TFTWING_BUTTON_A_PIN)

#define TFTWING_BUTTON_B_PIN 9
#define TFTWING_BUTTON_B (1UL << TFTWING_BUTTON_B_PIN)

#define TFTWING_BUTTON_ALL (TFTWING_BUTTON_UP | TFTWING_BUTTON_DOWN | TFTWING_BUTTON_LEFT | TFTWING_BUTTON_RIGHT | TFTWING_BUTTON_SELECT | TFTWING_BUTTON_A | TFTWING_BUTTON_B)

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with mini tft wing variant of seesaw helper IC
*/
/**************************************************************************/
class Adafruit_miniTFTWing : public Adafruit_seesaw {
public:
	Adafruit_miniTFTWing() {};
	~Adafruit_miniTFTWing() {};

    bool begin(uint8_t addr = TFTWING_ADDR, int8_t flow=-1);

	void setBacklight(uint16_t value);
	void setBacklightFreq(uint16_t freq);
    void tftReset(bool rst = true);
    uint32_t readButtons();
};

#endif
