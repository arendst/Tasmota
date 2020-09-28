/*
 *  SimpleSender.cpp
 *
 *  Sends Samsung protocol frames.
 *  Is able to send IR protocol data of 15 main protocols.
 *
 *      Sony SIRCS
 *      NEC + APPLE
 *      Samsung + Samsg32
 *      Kaseikyo
 *
 *      Plus 11 other main protocols
 *      JVC, NEC16, NEC42, Matsushita, DENON, Sharp, RC5, RC6 & RC6A, IR60 (SDA2008) Grundig, Siemens Gigaset, Nokia
 *
 *  To disable one of them or to enable other protocols, specify this before the "#include <irmp.c.h>" line.
 *  If you get warnings of redefining symbols, just ignore them or undefine them first (see Interrupt example).
 *  The exact names can be found in the library file irmpSelectAllProtocols.h (see Callback example).
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
 * Set library modifiers first to set output pin etc.
 */
#include "PinDefinitionsAndMore.h"
//#define IR_OUTPUT_IS_ACTIVE_LOW
#define IRSND_IR_FREQUENCY          38000

#define IRSND_PROTOCOL_NAMES        1 // Enable protocol number mapping to protocol strings - requires some FLASH.

#include <irsndSelectMain15Protocols.h>
/*
 * After setting the definitions we can include the code and compile it.
 */
#include <irsnd.c.h>

IRMP_DATA irsnd_data;

void setup()
{
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
#if defined(ESP8266)
    Serial.println(); // to separate it from the internal boot output
#endif

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    irsnd_init();
    irmp_irsnd_LEDFeedback(true); // Enable send signal feedback at LED_BUILTIN

#if defined(ARDUINO_ARCH_STM32)
    Serial.println(F("Ready to send IR signals at pin " IRSND_OUTPUT_PIN_STRING)); // the internal pin numbers are crazy for the STM32 Boards library
#else
    Serial.println(F("Ready to send IR signals at pin " STR(IRSND_OUTPUT_PIN)));
#endif

//#define SEND_SAMSUNG // else send NEC
#ifdef SEND_SAMSUNG
    /*
     * Send Samsung32
     */
    irsnd_data.protocol = IRMP_SAMSUNG32_PROTOCOL;
    irsnd_data.address = 0x0707;
    irsnd_data.command = 0xFB04; // For my Samsung, the high byte is the inverse of the low byte
    irsnd_data.flags = 0; // repeat frame 0 time
#else
    /*
     * Send NEC
     */
    irsnd_data.protocol = IRMP_NEC_PROTOCOL;
    irsnd_data.address = 0x0707;
    irsnd_data.command = 0xFB; // The required inverse of the 8 bit command is added by the send routine.
    irsnd_data.flags = 2; // repeat frame 2 times
#endif

    irsnd_send_data(&irsnd_data, true);
    irsnd_data_print(&Serial, &irsnd_data);

}

void loop()
{
    delay(5000);
    irsnd_data.command++;
#ifdef SEND_SAMSUNG
    // For my Samsung the high byte is the inverse of the low byte
    irsnd_data.command = ((~tNextCommand) << 8) | tNextCommand;
#endif
    irsnd_send_data(&irsnd_data, true); // true = wait for frame to end. This stores timer state and restores it after sending
    irsnd_data_print(&Serial, &irsnd_data);
}
