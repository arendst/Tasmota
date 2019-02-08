#ifndef _TFT_SHIELD_18_H
#define _TFT_SHIELD_18_H

#include "Adafruit_seesaw.h"

#define TFTSHIELD_ADDR 0x2E
#define TFTSHIELD_RESET_PIN 3

#define TFTSHIELD_BACKLIGHT_ON  0xFFFF
#define TFTSHIELD_BACKLIGHT_OFF 0x0000

#define TFTSHIELD_BUTTON_UP_PIN 5
#define TFTSHIELD_BUTTON_UP (1UL << TFTSHIELD_BUTTON_UP_PIN)

#define TFTSHIELD_BUTTON_DOWN_PIN 8
#define TFTSHIELD_BUTTON_DOWN (1UL << TFTSHIELD_BUTTON_DOWN_PIN)

#define TFTSHIELD_BUTTON_LEFT_PIN 6
#define TFTSHIELD_BUTTON_LEFT (1UL << TFTSHIELD_BUTTON_LEFT_PIN)

#define TFTSHIELD_BUTTON_RIGHT_PIN 9
#define TFTSHIELD_BUTTON_RIGHT (1UL << TFTSHIELD_BUTTON_RIGHT_PIN)

#define TFTSHIELD_BUTTON_IN_PIN 7
#define TFTSHIELD_BUTTON_IN (1UL << TFTSHIELD_BUTTON_IN_PIN)

#define TFTSHIELD_BUTTON_1_PIN 10
#define TFTSHIELD_BUTTON_1 (1UL << TFTSHIELD_BUTTON_1_PIN)

#define TFTSHIELD_BUTTON_2_PIN 11
#define TFTSHIELD_BUTTON_2 (1UL << TFTSHIELD_BUTTON_2_PIN)

#define TFTSHIELD_BUTTON_3_PIN 14
#define TFTSHIELD_BUTTON_3 (1UL << TFTSHIELD_BUTTON_3_PIN)

#define TFTSHIELD_BUTTON_ALL (TFTSHIELD_BUTTON_UP | TFTSHIELD_BUTTON_DOWN | TFTSHIELD_BUTTON_LEFT \
                            | TFTSHIELD_BUTTON_RIGHT | TFTSHIELD_BUTTON_IN | TFTSHIELD_BUTTON_1 \
                            | TFTSHIELD_BUTTON_2 | TFTSHIELD_BUTTON_3)

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with 1.8" tft shield variant of seesaw helper IC
*/
/**************************************************************************/
class Adafruit_TFTShield18 : public Adafruit_seesaw {
public:
	Adafruit_TFTShield18() {};
	~Adafruit_TFTShield18() {};

    bool begin(uint8_t addr = TFTSHIELD_ADDR, int8_t flow=-1);

	void setBacklight(uint16_t value);
	void setBacklightFreq(uint16_t freq);
    void tftReset(bool rst = true);
    uint32_t readButtons();
};

#endif