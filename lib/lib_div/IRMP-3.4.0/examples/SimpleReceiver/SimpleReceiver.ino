/*
 *  SimpleReceiver.cpp
 *
 *  Receives IR protocol data of 15 main protocols.
 *
 *  *****************************************************************************************************************************
 *  To access the library files from your sketch, you have to first use `Sketch > Show Sketch Folder (Ctrl+K)` in the Arduino IDE.
 *  Then navigate to the parallel `libraries` folder and select the library you want to access.
 *  The library files itself are located in the `src` sub-directory.
 *  If you did not yet store the example as your own sketch, then with Ctrl+K you are instantly in the right library folder.
 *  *****************************************************************************************************************************
 *
 *
 *  The following IR protocols are enabled by default:
 *      Sony SIRCS
 *      NEC + APPLE
 *      Samsung + Samsg32
 *      Kaseikyo
 *
 *      Plus 11 other main protocols by including irmpMain15.h instead of irmp.h
 *      JVC, NEC16, NEC42, Matsushita, DENON, Sharp, RC5, RC6 & RC6A, IR60 (SDA2008) Grundig, Siemens Gigaset, Nokia
 *
 *  To disable one of them or to enable other protocols, specify this before the "#include <irmp.h>" line.
 *  If you get warnings of redefining symbols, just ignore them or undefine them first (see Interrupt example).
 *  The exact names can be found in the library file irmpSelectAllProtocols.h (see Callback example).
 *
 *  Copyright (C) 2019  Armin Joachimsmeyer
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
#include "PinDefinitionsAndMore.h"

#define IRMP_PROTOCOL_NAMES 1 // Enable protocol number mapping to protocol strings - requires some FLASH. Must before #include <irmp*>

#include <irmpSelectMain15Protocols.h>  // This enables 15 main protocols
//#define IRMP_SUPPORT_NEC_PROTOCOL        1 // this enables only one protocol

#ifdef ALTERNATIVE_IRMP_FEEDBACK_LED_PIN
#define IRMP_FEEDBACK_LED_PIN   ALTERNATIVE_IRMP_FEEDBACK_LED_PIN
#endif
/*
 * After setting the definitions we can include the code and compile it.
 */
#include <irmp.c.h>

IRMP_DATA irmp_data;

void setup() {
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
#if defined(ESP8266)
    Serial.println(); // to separate it from the internal boot output
#endif

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    irmp_init();

    Serial.print(F("Ready to receive IR signals of protocols: "));
    irmp_print_active_protocols(&Serial);
#if defined(ARDUINO_ARCH_STM32)
    Serial.println(F("at pin " IRMP_INPUT_PIN_STRING));
#else
    Serial.println(F("at pin " STR(IRMP_INPUT_PIN)));
#endif

#ifdef ALTERNATIVE_IRMP_FEEDBACK_LED_PIN
    irmp_irsnd_LEDFeedback(true); // Enable receive signal feedback at ALTERNATIVE_IRMP_FEEDBACK_LED_PIN
    Serial.print(F("IR feedback pin is " STR(ALTERNATIVE_IRMP_FEEDBACK_LED_PIN)));
#endif

}

void loop() {
    /*
     * Check if new data available and get them
     */
    if (irmp_get_data(&irmp_data)) {
        /*
         * Skip repetitions of command
         */
        if (!(irmp_data.flags & IRMP_FLAG_REPETITION)) {
            /*
             * Here data is available and is no repetition -> evaluate IR command
             */
            switch (irmp_data.command) {
            case 0x48:
                digitalWrite(LED_BUILTIN, LOW);
                delay(4000);
                break;
            default:
                digitalWrite(LED_BUILTIN, HIGH);
                break;
            }
        }
        irmp_result_print(&irmp_data);
    }
}
