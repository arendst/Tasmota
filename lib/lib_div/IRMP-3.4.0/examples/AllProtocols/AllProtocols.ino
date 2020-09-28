/*
 *  AllProtocols.cpp
 *
 *  Accepts 40 protocols concurrently
 *  If you specify F_INTERRUPTS to 20000 at line 86 (default is 15000) it supports LEGO + RCMM protocols, but disables PENTAX and GREE protocols.
 *  if you see performance issues, you can disable MERLIN Protocol at line 88.
 *
 *  Uses a callback function which is called every time a complete IR command was received.
 *  Prints data to LCD connected parallel at pin 4-9 or serial at pin A4, A5
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

/*
 * Activate the type of LCD you use
 */
//#define USE_PARALELL_LCD
//#define USE_SERIAL_LCD
/*
 * Define the size of your LCD
 */
#if !defined(USE_SERIAL_LCD)
#define USE_1602_LCD
#endif
//#define USE_2004_LCD

/*
 * Imports and definitions for LCD
 */
#if defined(USE_SERIAL_LCD)
#include <LiquidCrystal_I2C.h> // Use an up to date library version which has the init method
#endif
#if defined(USE_PARALELL_LCD)
#include <LiquidCrystal.h>
#endif

#if defined(USE_1602_LCD)
// definitions for a 1602 LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2
#endif
#if defined(USE_2004_LCD)
// definitions for a 2004 LCD
#define LCD_COLUMNS 20
#define LCD_ROWS 4
#endif

#include <Arduino.h>

#include "PinDefinitionsAndMore.h"

/*
 * Set input pin and output pin definitions etc.
 */
#define IRMP_PROTOCOL_NAMES              1 // Enable protocol number mapping to protocol strings - needs some program memory ~ 420 bytes here
#define IRMP_USE_COMPLETE_CALLBACK       1 // Enable callback functionality

#if __SIZEOF_INT__ == 4
#define F_INTERRUPTS                     20000 // Instead of default 15000 to support LEGO + RCMM protocols
#else
//#define F_INTERRUPTS                     20000 // Instead of default 15000 to support LEGO + RCMM protocols, but this in turn disables PENTAX and GREE protocols :-(
//#define IRMP_32_BIT                       1 // This enables MERLIN protocol, but decreases performance for AVR.
#endif

#include <irmpSelectAllProtocols.h>  // This enables all possible protocols
//#define IRMP_SUPPORT_SIEMENS_PROTOCOL 1

/*
 * After setting the definitions we can include the code and compile it.
 */
#include <irmp.c.h>

IRMP_DATA irmp_data;

#if defined(USE_SERIAL_LCD) && defined(USE_PARALELL_LCD)
#error Cannot use parallel and serial LCD simultaneously
#endif

#if defined(USE_SERIAL_LCD)
LiquidCrystal_I2C myLCD(0x27, LCD_COLUMNS, LCD_ROWS);  // set the LCD address to 0x27 for a 20 chars and 2 line display
#endif
#if defined(USE_PARALELL_LCD)
LiquidCrystal myLCD(4, 5, 6, 7, 8, 9);
#endif

#if defined(__AVR__) && !(defined(__AVR_ATmega4809__) || defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__))
// For cyclically display of VCC
#include "ADCUtils.h"
#define MILLIS_BETWEEN_VOLTAGE_PRINT 5000
#endif

void handleReceivedIRData();
void irmp_result_print_LCD();

bool volatile sIRMPDataAvailable = false;

#if defined(__AVR__) && !(defined(__AVR_ATmega4809__) || defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__))
uint32_t volatile sMillisOfLastVoltagePrint;
#endif

void setup()
{
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217) \
    || defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__)
    delay(2000); // To be able to connect Serial monitor after reset or power on and before first printout
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    irmp_init();
    irmp_irsnd_LEDFeedback(true); // Enable receive signal feedback at LED_BUILTIN
    irmp_register_complete_callback_function(&handleReceivedIRData);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    irmp_print_active_protocols(&Serial);
#if defined(ARDUINO_ARCH_STM32)
    Serial.println(F("at pin " IRMP_INPUT_PIN_STRING)); // the internal pin numbers are crazy for the STM32 Boards library
#else
    Serial.println(F("at pin " STR(IRMP_INPUT_PIN)));
#endif

#if defined(__AVR__) && !(defined(__AVR_ATmega4809__) || defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__))
    getVCCVoltageMillivoltSimple(); // to initialize ADC mux and reference
#endif

#if defined(USE_SERIAL_LCD)
    myLCD.init();
    myLCD.clear();
    myLCD.backlight();
#endif
#if defined(USE_PARALELL_LCD)
    myLCD.begin(LCD_COLUMNS, LCD_ROWS);
#endif

#if defined(USE_SERIAL_LCD) || defined(USE_PARALELL_LCD)
    myLCD.print(F("IRMP all  v" VERSION_IRMP));
    myLCD.setCursor(0, 1);
    myLCD.print(F(__DATE__));
#endif
}

void loop()
{
    if (sIRMPDataAvailable)
    {
        sIRMPDataAvailable = false;

        /*
         * Serial output
         * takes 2 milliseconds at 115200
         */
        irmp_result_print(&irmp_data);

#if defined(USE_SERIAL_LCD) || defined(USE_PARALELL_LCD)
#  if defined(USE_SERIAL_LCD)
        disableIRTimerInterrupt(); // disable timer interrupt, since it disturbs the serial output
#  endif
        irmp_result_print_LCD();
#  if defined(USE_SERIAL_LCD)
        enableIRTimerInterrupt();
#  endif
#endif
    }

#if defined(__AVR__) && defined(ADATE)
    /*
     * Periodically print VCC
     */
    if (millis() - sMillisOfLastVoltagePrint > MILLIS_BETWEEN_VOLTAGE_PRINT)
    {
        sMillisOfLastVoltagePrint = millis();
        uint16_t tVCC = getVCCVoltageMillivoltSimple();

        Serial.print(F("VCC="));
        Serial.print(tVCC);
        Serial.println(F("mV"));

#  if defined(USE_SERIAL_LCD) || defined(USE_PARALELL_LCD)
        myLCD.setCursor(10, 0);
        myLCD.print(' ');
        myLCD.print(tVCC / 1000);
        myLCD.print('.');
        myLCD.print(((tVCC + 5) / 10) % 100);
        myLCD.print('V');
#  endif
    }
#endif
}

/*
 * Here we know, that data is available.
 * Since this function is executed in Interrupt handler context, make it short and do not use delay() etc.
 * In order to enable other interrupts you can call interrupts() (enable interrupt again) after getting data.
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR handleReceivedIRData()
#elif defined(ESP32)
void IRAM_ATTR handleReceivedIRData()
#else
void handleReceivedIRData()
#endif
{

#if defined(__AVR__) && defined(ADATE)
    // reset voltage display timer
    sMillisOfLastVoltagePrint = millis();
#endif

    /*
     * Just print the data to Serial and LCD
     */
    irmp_get_data(&irmp_data);
    sIRMPDataAvailable = true;
}

/*
 * LCD output for 1602 and 2004 LCDs
 * 40 - 55 Milliseconds per initial output for a 1602 LCD
 * for a 2004 LCD the initial clearing adds 55 ms.
 * The expander runs at 100 kHz :-(
 * 8 milliseconds for 8 bit; 10 ms for 16 bit code output
 * 3 milliseconds for repeat output
 *
 */
void irmp_result_print_LCD()
{
#if defined(USE_SERIAL_LCD) || defined(USE_PARALELL_LCD)
    static uint8_t sLastProtocolIndex;
    static uint16_t sLastProtocolAddress;

#  if (LCD_ROWS >= 4)
    static uint8_t sLastCommandPrintPosition = 13;

    const uint8_t tStartRow = 2;

#  else
    static uint16_t sLastCommand;
    static uint8_t sLastCommandPrintPosition;

    const uint8_t tStartRow = 0;
    bool tDisplayWasCleared = false;
#  endif

    /*
     * Print only if protocol or address has changed
     */
    if (sLastProtocolIndex != irmp_data.protocol || sLastProtocolAddress != irmp_data.address)
    {
        sLastProtocolIndex = irmp_data.protocol;
        sLastProtocolAddress = irmp_data.address;
#  if (LCD_ROWS >= 4)
        // clear data lines
        myLCD.setCursor(0, tStartRow);
        myLCD.print(F("                    "));
        myLCD.setCursor(0, tStartRow + 1);
        myLCD.print(F("                    "));
#  else
        myLCD.clear();
        tDisplayWasCleared = true;
#  endif

        /*
         * Show protocol name
         */
        myLCD.setCursor(0, tStartRow);
        myLCD.print(F("P="));
#  if defined(__AVR__)
        const char* tProtocolStringPtr = (char*) pgm_read_word(&irmp_protocol_names[irmp_data.protocol]);
        myLCD.print((__FlashStringHelper *) (tProtocolStringPtr));
#  else
        myLCD.print(irmp_protocol_names[irmp_data.protocol]);
#  endif

        /*
         * Show address
         */
        myLCD.setCursor(0, tStartRow + 1);
        myLCD.print(F("A=0x"));
        myLCD.print(irmp_data.address, HEX);

#  if (LCD_COLUMNS > 16)
        /*
         * Print prefix of command here, since it is constant string
         */
        myLCD.setCursor(9, tStartRow + 1);
        myLCD.print(F("C=0x"));
#  endif
    }
    else
    {
        /*
         * Show or clear repetition flag
         */
#  if (LCD_COLUMNS > 16)
        myLCD.setCursor(18, tStartRow + 1);
#  else
        myLCD.setCursor(15, tStartRow + 1);
#  endif
        if (irmp_data.flags & IRMP_FLAG_REPETITION)
        {
            myLCD.print('R');
            return; // Since it is a repetition, printed data has not changed
        }
        else
        {
            myLCD.print(' ');
        }
    }

    /*
     * Command prefix
     */
    uint16_t tCommand = irmp_data.command;

#  if (LCD_COLUMNS <= 16)
    // check if prefix position must change
    if (tDisplayWasCleared || (sLastCommand > 0x100 && tCommand < 0x100) || (sLastCommand < 0x100 && tCommand > 0x100))
    {
        sLastCommand = tCommand;
        /*
         * Print prefix of command
         */
        myLCD.setCursor(9, tStartRow + 1);

        /*
         * Print prefix for 8/16 bit commands
         */
        if (tCommand >= 0x100)
        {
            myLCD.print(F("0x"));
            sLastCommandPrintPosition = 11;
        }
        else
        {
            myLCD.print(F("C=0x"));
            sLastCommandPrintPosition = 13;
        }
    }
#  endif

    /*
     * Command data
     */
    myLCD.setCursor(sLastCommandPrintPosition, tStartRow + 1);
    if (irmp_data.command < 0x10)
    {
        // leading 0
        myLCD.print('0');
    }
    myLCD.print(tCommand, HEX);

#endif // defined(USE_SERIAL_LCD) || defined(USE_PARALELL_LCD)
}

