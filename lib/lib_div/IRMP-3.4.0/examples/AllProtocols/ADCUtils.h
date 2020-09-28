/*
 * ADCUtils.h
 *
 *  Copyright (C) 2016-2021  Armin Joachimsmeyer
 *  Email: armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-Utils https://github.com/ArminJo/Arduino-Utils.
 *
 *  ArduinoUtils is free software: you can redistribute it and/or modify
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

#ifndef SRC_ADCUTILS_H_
#define SRC_ADCUTILS_H_

#if defined(__AVR__) && (! defined(__AVR_ATmega4809__))
#include <Arduino.h>
#if defined(ADATE)

// PRESCALE4 => 13 * 4 = 52 microseconds per ADC conversion at 1 MHz Clock => 19,2 kHz
#define ADC_PRESCALE2    1 // 26 microseconds per ADC conversion at 1 MHz
#define ADC_PRESCALE4    2 // 52 microseconds per ADC conversion at 1 MHz
// PRESCALE8 => 13 * 8 = 104 microseconds per ADC sample at 1 MHz Clock => 9,6 kHz
#define ADC_PRESCALE8    3 // 104 microseconds per ADC conversion at 1 MHz
#define ADC_PRESCALE16   4 // 13/208 microseconds per ADC conversion at 16/1 MHz - degradations in linearity at 16 MHz
#define ADC_PRESCALE32   5 // 26/416 microseconds per ADC conversion at 16/1 MHz - very good linearity at 16 MHz
#define ADC_PRESCALE64   6 // 52 microseconds per ADC conversion at 16 MHz
#define ADC_PRESCALE128  7 // 104 microseconds per ADC conversion at 16 MHz --- Arduino default

// definitions for 0.1 ms conversion time
#if (F_CPU == 1000000)
#define ADC_PRESCALE ADC_PRESCALE8
#elif (F_CPU == 8000000)
#define ADC_PRESCALE ADC_PRESCALE64
#elif (F_CPU == 16000000)
#define ADC_PRESCALE ADC_PRESCALE128
#endif

/*
 * Reference shift values are complicated for ATtinyX5 since we have the extra register bit REFS2
 * in ATTinyCore, this bit is handled programmatical and therefore the defines are different.
 * To keep my library small, I use the changed defines.
 * After including this file you can not call the ATTinyCore readAnalog functions reliable, if you specify references other than default!
 */
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// defines are for ADCUtils.cpp, they can be used WITHOUT bit reordering
#undef DEFAULT
#undef EXTERNAL
#undef INTERNAL1V1
#undef INTERNAL
#undef INTERNAL2V56
#undef INTERNAL2V56_EXTCAP

#define DEFAULT 0
#define EXTERNAL 4
#define INTERNAL1V1 8
#define INTERNAL INTERNAL1V1
#define INTERNAL2V56 9
#define INTERNAL2V56_EXTCAP 13

#define SHIFT_VALUE_FOR_REFERENCE REFS2
#define MASK_FOR_ADC_REFERENCE (_BV(REFS0) | _BV(REFS1) | _BV(REFS2))
#else // AVR_ATtiny85
#define SHIFT_VALUE_FOR_REFERENCE REFS0
#define MASK_FOR_ADC_REFERENCE (_BV(REFS0) | _BV(REFS1))
#endif

// Temperature channel definitions - 1 LSB / 1 degree Celsius
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define ADC_TEMPERATURE_CHANNEL_MUX 15
#define ADC_1_1_VOLT_CHANNEL_MUX    12
#define ADC_GND_CHANNEL_MUX         13

#elif defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#define ADC_ISCR_CHANNEL_MUX         3
#define ADC_TEMPERATURE_CHANNEL_MUX 11
#define ADC_1_1_VOLT_CHANNEL_MUX    12
#define ADC_GND_CHANNEL_MUX         14
#define ADC_VCC_4TH_CHANNEL_MUX     13

#elif defined(__AVR_ATmega328P__)
#define ADC_TEMPERATURE_CHANNEL_MUX  8
#define ADC_1_1_VOLT_CHANNEL_MUX    14
#define ADC_GND_CHANNEL_MUX         15

#elif defined(__AVR_ATmega32U4__)
#define ADC_TEMPERATURE_CHANNEL_MUX 0x27
#define ADC_1_1_VOLT_CHANNEL_MUX    0x1E
#define ADC_GND_CHANNEL_MUX         0x1F

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define ADC_1_1_VOLT_CHANNEL_MUX    0x1E
#define ADC_GND_CHANNEL_MUX         0x1F
#define INTERNAL INTERNAL1V1

#else
#error "No temperature channel definitions specified for this AVR CPU"
#endif

uint16_t readADCChannel(uint8_t aChannelNumber);
uint16_t readADCChannelWithReference(uint8_t aChannelNumber, uint8_t aReference);
uint16_t readADCChannelWithOversample(uint8_t aChannelNumber, uint8_t aOversampleExponent);
uint16_t readADCChannelWithReferenceOversample(uint8_t aChannelNumber, uint8_t aReference, uint8_t aOversampleExponent);
uint16_t readADCChannelWithReferenceOversampleFast(uint8_t aChannelNumber, uint8_t aReference, uint8_t aOversampleExponent);
uint16_t readADCChannelWithReferenceMultiSamples(uint8_t aChannelNumber, uint8_t aReference, uint8_t aNumberOfSamples);
uint16_t readADCChannelWithReferenceMax(uint8_t aChannelNumber, uint8_t aReference, uint16_t aNumberOfSamples);
uint16_t readADCChannelWithReferenceMaxMicros(uint8_t aChannelNumber, uint8_t aReference, uint16_t aMicrosecondsToAquire);
uint16_t readUntil4ConsecutiveValuesAreEqual(uint8_t aChannelNumber, uint8_t aDelay, uint8_t aAllowedDifference,
        uint8_t aMaxRetries);

uint8_t checkAndWaitForReferenceAndChannelToSwitch(uint8_t aChannelNumber, uint8_t aReference);

float getVCCVoltageSimple(void);
uint16_t getVCCVoltageMillivoltSimple(void);
float getTemperatureSimple(void);
float getVCCVoltage(void);
uint16_t getVCCVoltageMillivolt(void);
void printVCCVoltageMillivolt(Print* aSerial);
float getTemperature(void);

#endif // defined(ADATE)
#endif //  defined(__AVR__)
#endif /* SRC_ADCUTILS_H_ */

#pragma once
