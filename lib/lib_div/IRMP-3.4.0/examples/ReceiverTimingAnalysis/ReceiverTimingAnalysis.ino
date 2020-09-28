/*
 *  ReceiverTimingAnalysis.cpp
 *
 *  This program enables the pin change interrupt at pin 3 and waits for NEC (or other Pulse-Distance-Coding) IR Signal.
 *  It measures the pulse and pause times of the incoming signal and computes some statistics for it.
 *
 *  Observed values:
 *  Delta of each signal type is around 50 up to 100 and at low signals up to 200. TSOP is better, especially at low IR signal level.
 *  VS1838      Mark Excess -50 to +50 us
 *  TSOP31238   Mark Excess 0 to +50
 *
 *
 *  Copyright (C) 2019-2020  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of IRMP https://github.com/ukw100/IRMP.
 *  This file is part of Arduino-IRremote https://github.com/z3t0/Arduino-IRremote.
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

#define IR_INPUT_PIN    3

/*
 * Helper macro for getting a macro definition as string
 */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

void measureTimingISR(void);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL) || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));

#if defined(EICRA) && defined(EIFR) && defined(EIMSK)
    // enable interrupt on pin3 on both edges for ATmega328
    EICRA |= _BV(ISC10);
    // clear interrupt bit
    EIFR |= 1 << INTF1;
    // enable interrupt on next change
    EIMSK |= 1 << INT1;
#else
    attachInterrupt(digitalPinToInterrupt(IR_INPUT_PIN), measureTimingISR, CHANGE);
#endif
    Serial.println(F("Ready to analyze NEC IR signal at pin " STR(IR_INPUT_PIN)));
    Serial.println();
}

uint8_t ISREdgeCounter = 0;
volatile uint32_t LastMicros;
struct timingStruct
{
    uint16_t minimum;
    uint8_t indexOfMinimum;
    uint16_t maximum;
    uint8_t indexOfMaximum;
    uint16_t average;

    uint16_t SumForAverage;
    uint8_t SampleCount;
//    uint8_t LastPrintedCount;
};

struct timingStruct Mark;
struct timingStruct ShortSpace;
struct timingStruct LongSpace;

/*
 * Compute minimum, maximum and average
 */
void processTmingValue(struct timingStruct *aTimingStruct, uint16_t aValue)
{
    if (aTimingStruct->SampleCount == 0)
    {
        // initialize values
        aTimingStruct->minimum = 0xFFFF;
        aTimingStruct->maximum = 0;
        aTimingStruct->SumForAverage = 0;
    }

    if (aTimingStruct->minimum > aValue)
    {
        aTimingStruct->minimum = aValue;
        aTimingStruct->indexOfMinimum = aTimingStruct->SampleCount;
    }
    if (aTimingStruct->maximum < aValue)
    {
        aTimingStruct->maximum = aValue;
        aTimingStruct->indexOfMaximum = aTimingStruct->SampleCount;
    }

    aTimingStruct->SampleCount++;
    aTimingStruct->SumForAverage += aValue;
    aTimingStruct->average = (aTimingStruct->SumForAverage + (aTimingStruct->SampleCount / 2)) / aTimingStruct->SampleCount;

}

void printTimingValues(struct timingStruct *aTimingStruct, const char *aCaption)
{
//    if (aTimingStruct->LastPrintedCount != aTimingStruct->SampleCount)
//    {
//        aTimingStruct->LastPrintedCount = aTimingStruct->SampleCount;
    Serial.print(aCaption);
    Serial.print(F(": SampleCount="));
    Serial.print(aTimingStruct->SampleCount);
    Serial.print(F(" Minimum="));
    Serial.print(aTimingStruct->minimum);
    Serial.print(F(" @"));
    Serial.print(aTimingStruct->indexOfMinimum);
    Serial.print(F(" Maximum="));
    Serial.print(aTimingStruct->maximum);
    Serial.print(F(" @"));
    Serial.print(aTimingStruct->indexOfMaximum);
    Serial.print(F(" Delta="));
    Serial.print(aTimingStruct->maximum - aTimingStruct->minimum);
    Serial.print(F("   Average="));
    Serial.print(aTimingStruct->average);

    Serial.println();
//    }
}

void loop()
{
    if (Mark.SampleCount >= 32)
    {
        /*
         * This check enables statistics for longer protocols like Kaseikyo/Panasonics
         */
#if !defined(ARDUINO_ARCH_MBED)
        noInterrupts();
#endif
        uint32_t tLastMicros = LastMicros;
#if !defined(ARDUINO_ARCH_MBED)
        interrupts();
#endif
        uint32_t tMicrosDelta = micros() - tLastMicros;

        if (tMicrosDelta > 10000)
        {
            // NEC signal ended just now
            Serial.println();
            printTimingValues(&Mark, "Mark      ");
            printTimingValues(&ShortSpace, "ShortSpace");
            printTimingValues(&LongSpace, "LongSpace ");

            /*
             * Print analysis of mark and short spaces
             */
            Serial.println(F("Analysis  :"));
            Serial.print(F(" Average (Mark + ShortSpace)/2="));
            int16_t MarkAndShortSpaceAverage = (Mark.average + ShortSpace.average) / 2;
            Serial.print(MarkAndShortSpaceAverage);
            Serial.print(F("us   Delta (to NEC standard 560)="));
            Serial.print(MarkAndShortSpaceAverage - 560);
            Serial.print(F("us\r\n Mark - Average -> MarkExcess="));
            Serial.print((int16_t) Mark.average - MarkAndShortSpaceAverage);
            Serial.print(F("us"));
            Serial.println();
            Serial.println();

            Mark.SampleCount = 0; // used as flag for not printing the results more than once
        }
    }
}

/*
 * The interrupt handler.
 * Just add to the appropriate timing structure.
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR measureTimingISR()
#elif defined(ESP32)
void IRAM_ATTR measureTimingISR()
#else
#  if defined(EICRA) && defined(EIFR) && defined(EIMSK)
ISR(INT1_vect)
#  else
void measureTimingISR()
#  endif
#endif
{
    uint32_t tMicros = micros();
    uint32_t tMicrosDelta = tMicros - LastMicros;
    LastMicros = tMicros;
    /*
     * read level and give feedback
     */
    uint8_t tInputLevel = digitalRead(IR_INPUT_PIN);
#if defined(__AVR_ATtiny3217__)
    digitalWrite(LED_BUILTIN, (PinStatus)!tInputLevel);
#else
    digitalWrite(LED_BUILTIN, !tInputLevel);
#endif

    if (tMicrosDelta > 10000)
    {
        // gap > 10 ms detected, reset counter to first detected edge and initialize timing structures
        ISREdgeCounter = 1;
        LongSpace.SampleCount = 0;
        ShortSpace.SampleCount = 0;
        Mark.SampleCount = 0;
    }
    else
    {
        ISREdgeCounter++;
    }

    /*
     * Skip header mark and space and first bit mark and space
     */
    if (ISREdgeCounter > 4)
    {
        if (tInputLevel != LOW)
        {
            // Mark ended
            processTmingValue(&Mark, tMicrosDelta);
//            Serial.print('M');
        }
        else
        {
            // Space ended
            if (tMicrosDelta > 1000)
            {
                // long space - logical 1
                processTmingValue(&LongSpace, tMicrosDelta);
                Serial.print('1');
            }
            else
            {
                // short space - logical 0
                processTmingValue(&ShortSpace, tMicrosDelta);
                Serial.print('0');
            }
        }
    }
}
