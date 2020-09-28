/*
 *  ReceiveAndSend.cpp
 *
 *  Serves as a IR remote macro expander
 *  Receives Samsung32 protocol and on receiving a specified input frame, it sends multiple Samsung32 frames.
 *  This serves as a Netflix-key emulation for my old Samsung H5273 TV.
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

// ATMEL ATTINY85
// Piezo speaker must have a 270 Ohm resistor in series for USB programming and running at the Samsung TV.
// IR LED has a 270 Ohm resistor in series.
//                                                    +-\/-+
//                                   !RESET (5) PB5  1|    |8  Vcc
// USB+ 3.6V Z-Diode, 1.5kOhm to VCC  Piezo (3) PB3  2|    |7  PB2 (2) TX Debug output
// USB- 3.6V Z-Diode              IR Output (4) PB4  3|    |6  PB1 (1) Feedback LED
//                                              GND  4|    |5  PB0 (0) IR Input
//                                                    +----+
/*
 * Pin mapping table for different platforms
 *
 * Platform     IR input    IR output   Tone
 * -----------------------------------------
 * DEFAULT/AVR  3           4           5
 * ATtinyX5     0           4           3
 * ATtin167     9           8           5 // Digispark pro number schema
 * ATtin167     3           2           7
 * SAMD21       3           4           5
 * ESP8266      14 // D5    12 // D6    %
 * ESP32        15          4           %
 * BluePill     PA6         PA7         PA3
 * APOLLO3      11          12          5
 */

#include <Arduino.h>

/*
 * Set library modifiers first to set input and output pin etc.
 */
#include "PinDefinitionsAndMore.h"
//#define IR_OUTPUT_IS_ACTIVE_LOW
#define IRSND_IR_FREQUENCY          38000

#define IRMP_PROTOCOL_NAMES         1 // Enable protocol number mapping to protocol strings - requires some FLASH.
#define IRSND_PROTOCOL_NAMES        1 // Enable protocol number mapping to protocol strings - requires some FLASH.

#define IRMP_SUPPORT_SAMSUNG_PROTOCOL     1
#define IRSND_SUPPORT_SAMSUNG_PROTOCOL    1

/*
 * After setting the definitions we can include the code and compile it.
 */
#define USE_ONE_TIMER_FOR_IRMP_AND_IRSND // otherwise we get an error on AVR platform: redefinition of 'void __vector_8()
#include <irmp.c.h>
#include <irsnd.c.h>

IRMP_DATA irmp_data;
IRMP_DATA irsnd_data;

void sendSamsungSmartHubMacro(bool aDoSelect);
void IRSendWithDelay(uint16_t aCommand, uint16_t aDelayMillis);

void setup()
{
#if defined(MCUSR)
    MCUSR = 0; // To reset old boot flags for next boot
#endif

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
#if defined(ESP8266)
    Serial.println(); // to separate it from the internal boot output
#endif

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    // tone before setup, since it kills the IR timer settings
    tone(TONE_PIN, 2200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(400);
    digitalWrite(LED_BUILTIN, LOW);
    noTone(TONE_PIN);

    irmp_init();
    irmp_irsnd_LEDFeedback(true); // Enable receive signal feedback at LED_BUILTIN for receive and send

    irsnd_init();

    Serial.print(F("Ready to receive IR signals of protocols: "));
    irmp_print_active_protocols(&Serial);
#if defined(ARDUINO_ARCH_STM32)
    Serial.println(F("at pin " IRMP_INPUT_PIN_STRING)); // the internal pin numbers are crazy for the STM32 Boards library
    Serial.println(F("Ready to send IR signals at pin " IRSND_OUTPUT_PIN_STRING));// the internal pin numbers are crazy for the STM32 Boards library
#else
    Serial.println(F("at pin " STR(IRMP_INPUT_PIN)));
    Serial.println(F("Ready to send IR signals at pin " STR(IRSND_OUTPUT_PIN)));
#endif

    irsnd_data.protocol = IRMP_SAMSUNG32_PROTOCOL;
    irsnd_data.address = 0x0707;
    irsnd_data.flags = 1; // repeat frame 1 time
}

void loop()
{
    /*
     * Check if new data available and get them
     */
    if (irmp_get_data(&irmp_data))
    {
        irmp_result_print(&irmp_data);

        /*
         * Here data is available -> evaluate IR command
         */
        switch (irmp_data.command)
        {
        case 0xB847: // The play key on the bottom of my Samsung remote
            Serial.println(F("Play key detected, open Netflix"));
            sendSamsungSmartHubMacro(true);
            break;

        case 0xB54A: // The pause key on the bottom of my Samsung remote
            Serial.println(F("Pause key detected, open SmartHub"));
            sendSamsungSmartHubMacro(false);
            break;

        default:
            break;
        }
        // Flush repeats received
        irmp_get_data(&irmp_data);
    }
}

void IRSendWithDelay(uint16_t aCommand, uint16_t aDelayMillis)
{
    irsnd_data.command = aCommand;      // For my Samsung, the high byte is the inverse of the low byte, this is not checked here.
    irsnd_send_data(&irsnd_data, true); // true = wait for frame to end. This stores timer state and restores it after sending
    irsnd_data_print(&Serial,&irsnd_data);
    delay(aDelayMillis);
}

bool sMacroWasCalledBefore = false;
#define INITIAL_WAIT_TIME_APPS_READY_MILLIS 70000 // Time to let the TV load all software before Netflix can be started without an error
#define INITIAL_WAIT_TIME_SMARTHUB_READY_MILLIS 20000 // Time to let the TV load all software before SmartHub manu can be displayed

/*
 * This macro calls the last SmartHub application you selected manually
 *
 * @param aDoSelect - if true select the current app (needs longer initial wait time) else show smarthub menu
 *
 */
void sendSamsungSmartHubMacro(bool aDoSelect)
{
    uint32_t tWaitTimeAfterBoot;
    if (aDoSelect)
    {
        tWaitTimeAfterBoot = INITIAL_WAIT_TIME_APPS_READY_MILLIS;
    }
    else
    {
        tWaitTimeAfterBoot = INITIAL_WAIT_TIME_SMARTHUB_READY_MILLIS;
    }

    if (millis() < tWaitTimeAfterBoot)
    {
        // division by 1000 and printing requires much (8%) program space
        Serial.print(F("It is "));
        Serial.print(millis() / 1000);
        Serial.print(F(" seconds after boot, Samsung H5273 TV requires "));
        Serial.print(tWaitTimeAfterBoot / 1000);
        Serial.println(F(" seconds after boot to be ready for the command"));

        tone(TONE_PIN, 2200);
        delay(100);
        noTone(TONE_PIN);
        delay(100);
        tone(TONE_PIN, 2200);
        delay(100);
        noTone(TONE_PIN);

        while (millis() < tWaitTimeAfterBoot)
        {
            delay(10); // blocking wait
        }
    }

    // Do beep feedback for special key to be received
    tone(TONE_PIN, 2200);
    delay(200);
    noTone(TONE_PIN);
    irmp_init(); // restore timer for IR receive after using of tone

    Serial.println(F("Wait for \"not supported\" to disappear"));
    delay(2000);

    Serial.println(F("Start sending of Samsung IR macro"));

    IRSendWithDelay(0xE51A, 2000); // Menu and wait for the Menu to pop up

    Serial.println(F("Wait for the menu to pop up"));
    if (!sMacroWasCalledBefore)
    {
        delay(2000); // wait additional time for the Menu load
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        IRSendWithDelay(0x9E61, 250); // Down arrow. For my Samsung, the high byte of the command is the inverse of the low byte
    }

    IRSendWithDelay(0x9D62, 400); // Right arrow
    for (uint8_t i = 0; i < 2; ++i)
    {
        IRSendWithDelay(0x9E61, 250); // Down arrow
    }

    delay(250);
    IRSendWithDelay(0x9768, 1); // Enter for SmartHub

    if (aDoSelect)
    {
        Serial.println(F("Wait for SmartHub to show up, before entering current application"));
        delay(10000); // Wait not longer than 12 seconds, because smarthub menu then disappears
        IRSendWithDelay(0x9768, 1); // Enter for last application (e.g. Netflix or Amazon)
    }

    sMacroWasCalledBefore = true;
    Serial.println(F("Done"));

}
