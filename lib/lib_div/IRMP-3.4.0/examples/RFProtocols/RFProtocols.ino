/*
 *  RFProtocols.cpp
 *
 *  Receives 2 RF protocol (433 MHz) data.
 *
 *  Copyright (C) 2019-2020  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of IRMP https://github.com/ukw100/IRMP.
 *
 *  IRMP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#include <Arduino.h>

/*
 * Set input pin and output pin definitions etc.
 */
#include "PinDefinitionsAndMore.h" // Sets input pin to 3

#define IRMP_PROTOCOL_NAMES 1 // Enable protocol number mapping to protocol strings - requires some FLASH.

#include <irmpSelectAllRFProtocols.h>  // This enables all possible RF protocols - currently 2 protocols

/*
 * After setting the definitions we can include the code and compile it.
 */
#include <irmp.c.h>

IRMP_DATA irmp_data;

void setup()
{
// initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
    // Just to know which program is running on my Arduino
#if defined(ESP8266)
    Serial.println();
#endif
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    irmp_init();
    irmp_irsnd_LEDFeedback(true); // Enable receive signal feedback at LED_BUILTIN - commented out, since we use built in LED in loop below

    Serial.print(F("Ready to receive  RF (433 MHz) signals of protocols: "));
    irmp_print_active_protocols(&Serial);
#if defined(ARDUINO_ARCH_STM32)
    Serial.println(F("at pin " IRMP_INPUT_PIN_STRING)); // the internal pin numbers are crazy for the STM32 Boards library
#else
    Serial.println(F("at pin " STR(IRMP_INPUT_PIN)));
#endif
}

void loop()
{
    /*
     * Check if new data available and get them
     */
    if (irmp_get_data(&irmp_data))
    {
        irmp_result_print(&irmp_data);
    }
}
