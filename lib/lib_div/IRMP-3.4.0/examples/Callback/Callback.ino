/*
 *  Callback.cpp
 *
 *  Uses a callback function which is called every time a complete IR command was received.
 *  This example additionally filters commands from a remote control named WM010 sending NEC commands
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
#include "PinDefinitionsAndMore.h"

#define IRMP_PROTOCOL_NAMES         1 // Enable protocol number mapping to protocol strings - needs some FLASH. Must before #include <irmp*>
#define IRMP_USE_COMPLETE_CALLBACK  1 // Enable callback functionality

// Enables protocols manually
//#define IRMP_SUPPORT_SIRCS_PROTOCOL      1
#define IRMP_SUPPORT_NEC_PROTOCOL        1
//#define IRMP_SUPPORT_SAMSUNG_PROTOCOL    1
//#define IRMP_SUPPORT_KASEIKYO_PROTOCOL   1

//#define IRMP_SUPPORT_JVC_PROTOCOL        1
//#define IRMP_SUPPORT_NEC16_PROTOCOL      1
//#define IRMP_SUPPORT_NEC42_PROTOCOL      1
//#define IRMP_SUPPORT_MATSUSHITA_PROTOCOL 1
//#define IRMP_SUPPORT_DENON_PROTOCOL      1
//#define IRMP_SUPPORT_RC5_PROTOCOL        1
//#define IRMP_SUPPORT_RC6_PROTOCOL        1
//#define IRMP_SUPPORT_IR61_PROTOCOL       1
//#define IRMP_SUPPORT_GRUNDIG_PROTOCOL    1
//#define IRMP_SUPPORT_SIEMENS_PROTOCOL    1
//#define IRMP_SUPPORT_NOKIA_PROTOCOL      1

/*
 * After setting the modifiers we can include the code.
 */
#include <irmp.c.h>

IRMP_DATA irmp_data;

void handleReceivedIRData();

void setup()
{
    // initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    //Enable auto resume and pass it the address of your extra buffer
    irmp_init();
    irmp_irsnd_LEDFeedback(true); // Enable receive signal feedback at LED_BUILTIN
    irmp_register_complete_callback_function(&handleReceivedIRData);

    Serial.print(F("Ready to receive IR signals  of protocols: "));
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
     * Put your code here
     */
}

/*
 * Here we know, that data is available.
 * Since this function is executed in Interrupt handler context, make it short and do not use delay() etc.
 * In order to enable other interrupts you can call sei() (enable interrupt again) after getting data.
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR handleReceivedIRData()
#elif defined(ESP32)
void IRAM_ATTR handleReceivedIRData()
#else
void handleReceivedIRData()
#endif
{
    irmp_get_data(&irmp_data);
#if !defined(ARDUINO_ARCH_MBED)
    interrupts(); // enable interrupts
#endif

    /*
     * Filter for commands from the WM010 IR Remote
     */
    if (irmp_data.address == 0xF708)
    {
        /*
         * Skip repetitions of command
         */
        if (!(irmp_data.flags & IRMP_FLAG_REPETITION))
        {
            /*
             * Evaluate IR command
             */
            switch (irmp_data.command)
            {
            case 0x48:
                digitalWrite(LED_BUILTIN, HIGH);
                break;
            case 0x0B:
                digitalWrite(LED_BUILTIN, LOW);
                break;
            default:
                break;
            }
        }
    }

    irmp_result_print(&irmp_data);
}
