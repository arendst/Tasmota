/*
 * ADCUtils.cpp
 *
 * ADC utility functions. Conversion time is defined as 0.104 milliseconds for 16 MHz Arduinos in ADCUtils.h.
 *
 *  Copyright (C) 2016-2020  Armin Joachimsmeyer
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
 */

#include "ADCUtils.h"
#if defined(__AVR__) && defined(ADATE)

// Union to speed up the combination of low and high bytes to a word
// it is not optimal since the compiler still generates 2 unnecessary moves
// but using  -- value = (high << 8) | low -- gives 5 unnecessary instructions
union Myword {
    struct {
        uint8_t LowByte;
        uint8_t HighByte;
    } byte;
    uint16_t UWord;
    int16_t Word;
    uint8_t *BytePointer;
};

/*
 * Conversion time is defined as 0.104 milliseconds for 16 MHz Arduino by ADC_PRESCALE in ADCUtils.h.
 */
uint16_t readADCChannel(uint8_t aChannelNumber) {
    Myword tUValue;
    ADMUX = aChannelNumber | (DEFAULT << SHIFT_VALUE_FOR_REFERENCE);

    // ADCSRB = 0; // Only active if ADATE is set to 1.
    // ADSC-StartConversion ADIF-Reset Interrupt Flag - NOT free running mode
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADIF) | ADC_PRESCALE);

    // wait for single conversion to finish
    loop_until_bit_is_clear(ADCSRA, ADSC);

    // Get value
    tUValue.byte.LowByte = ADCL;
    tUValue.byte.HighByte = ADCH;
    return tUValue.UWord;
    //    return ADCL | (ADCH <<8); // needs 4 bytes more
}

/*
 * Conversion time is defined as 0.104 milliseconds for 16 MHz Arduino by ADC_PRESCALE in ADCUtils.h.
 */
uint16_t readADCChannelWithReference(uint8_t aChannelNumber, uint8_t aReference) {
    Myword tUValue;
    ADMUX = aChannelNumber | (aReference << SHIFT_VALUE_FOR_REFERENCE);

    // ADCSRB = 0; // Only active if ADATE is set to 1.
    // ADSC-StartConversion ADIF-Reset Interrupt Flag - NOT free running mode
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADIF) | ADC_PRESCALE);

    // wait for single conversion to finish
    loop_until_bit_is_clear(ADCSRA, ADSC);

    // Get value
    tUValue.byte.LowByte = ADCL;
    tUValue.byte.HighByte = ADCH;
    return tUValue.UWord;
}

/*
 * @return original ADMUX register content for optional later restoring values
 * All experimental values are acquired by using the ADCSwitchingTest example from this library
 */
uint8_t checkAndWaitForReferenceAndChannelToSwitch(uint8_t aChannelNumber, uint8_t aReference) {
    uint8_t tOldADMUX = ADMUX;
    /*
     * Must wait >= 7 us if reference has to be switched from 1.1 volt/INTERNAL to VCC/DEFAULT (seen on oscilloscope)
     * This is done after the 2 ADC clock cycles required for Sample & Hold :-)
     *
     * Must wait >= 7600 us for Nano board  >= 6200 for Uno board if reference has to be switched from VCC/DEFAULT to 1.1 volt/INTERNAL
     * Must wait >= 200 us if channel has to be switched to 1.1 volt internal channel if S&H was at 5 Volt
     */
    uint8_t tNewReference = (aReference << SHIFT_VALUE_FOR_REFERENCE);
    ADMUX = aChannelNumber | tNewReference;
    if ((tOldADMUX & MASK_FOR_ADC_REFERENCE) != tNewReference && aReference == INTERNAL) {
        /*
         * Switch reference from DEFAULT to INTERNAL
         */
        delayMicroseconds(8000); // experimental value is >= 7600 us for Nano board and 6200 for UNO board
    } else if ((tOldADMUX & 0x0F) != aChannelNumber) {
        if (aChannelNumber == ADC_1_1_VOLT_CHANNEL_MUX) {
            /*
             * Internal 1.1 Volt channel requires  <= 200 us for Nano board
             */
            delayMicroseconds(200);
        } else {
            /*
             * 100 kOhm requires < 100 us, 1 MOhm requires 120 us S&H switching time
             */
            delayMicroseconds(120); // experimental value is <= 1100 us for Nano board
        }
    }
    return tOldADMUX;
}

uint16_t readADCChannelWithOversample(uint8_t aChannelNumber, uint8_t aOversampleExponent) {
    return readADCChannelWithReferenceOversample(aChannelNumber, DEFAULT, aOversampleExponent);
}

/*
 * Conversion time is defined as 0.104 milliseconds for 16 MHz Arduino by ADC_PRESCALE in ADCUtils.h.
 */
uint16_t readADCChannelWithReferenceOversample(uint8_t aChannelNumber, uint8_t aReference, uint8_t aOversampleExponent) {
    uint16_t tSumValue = 0;
    ADMUX = aChannelNumber | (aReference << SHIFT_VALUE_FOR_REFERENCE);

    ADCSRB = 0; // Free running mode. Only active if ADATE is set to 1.
    // ADSC-StartConversion ADATE-AutoTriggerEnable ADIF-Reset Interrupt Flag
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIF) | ADC_PRESCALE);

    for (uint8_t i = 0; i < _BV(aOversampleExponent); i++) {
        /*
         * wait for free running conversion to finish.
         * Do not wait for ADSC here, since ADSC is only low for 1 ADC Clock cycle on free running conversion.
         */
        loop_until_bit_is_set(ADCSRA, ADIF);

        ADCSRA |= _BV(ADIF); // clear bit to recognize next conversion has finished
        // Add value
        tSumValue += ADCL | (ADCH << 8); // using myWord does not save space here
        // tSumValue += (ADCH << 8) | ADCL; // this does NOT work!
    }
    ADCSRA &= ~_BV(ADATE); // Disable auto-triggering (free running mode)
    return (tSumValue >> aOversampleExponent);
}

/*
 * Use ADC_PRESCALE32 which gives 26 us conversion time and good linearity for 16 MHz Arduino
 */
uint16_t readADCChannelWithReferenceOversampleFast(uint8_t aChannelNumber, uint8_t aReference, uint8_t aOversampleExponent) {
    uint16_t tSumValue = 0;
    ADMUX = aChannelNumber | (aReference << SHIFT_VALUE_FOR_REFERENCE);

    ADCSRB = 0; // Free running mode. Only active if ADATE is set to 1.
    // ADSC-StartConversion ADATE-AutoTriggerEnable ADIF-Reset Interrupt Flag
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIF) | ADC_PRESCALE32);

    for (uint8_t i = 0; i < _BV(aOversampleExponent); i++) {
        /*
         * wait for free running conversion to finish.
         * Do not wait for ADSC here, since ADSC is only low for 1 ADC Clock cycle on free running conversion.
         */
        loop_until_bit_is_set(ADCSRA, ADIF);

        ADCSRA |= _BV(ADIF); // clear bit to recognize next conversion has finished
        // Add value
        tSumValue += ADCL | (ADCH << 8); // using myWord does not save space here
        // tSumValue += (ADCH << 8) | ADCL; // this does NOT work!
    }
    ADCSRA &= ~_BV(ADATE); // Disable auto-triggering (free running mode)
    return (tSumValue >> aOversampleExponent);
}

/*
 * Returns sum of all sample values
 * Conversion time is defined as 0.104 milliseconds for 16 MHz Arduino by ADC_PRESCALE in ADCUtils.h.
 */
uint16_t readADCChannelWithReferenceMultiSamples(uint8_t aChannelNumber, uint8_t aReference, uint8_t aNumberOfSamples) {
    uint16_t tSumValue = 0;
    ADMUX = aChannelNumber | (aReference << SHIFT_VALUE_FOR_REFERENCE);

    ADCSRB = 0; // Free running mode. Only active if ADATE is set to 1.
    // ADSC-StartConversion ADATE-AutoTriggerEnable ADIF-Reset Interrupt Flag
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIF) | ADC_PRESCALE);

    for (uint8_t i = 0; i < aNumberOfSamples; i++) {
        /*
         * wait for free running conversion to finish.
         * Do not wait for ADSC here, since ADSC is only low for 1 ADC Clock cycle on free running conversion.
         */
        loop_until_bit_is_set(ADCSRA, ADIF);

        ADCSRA |= _BV(ADIF); // clear bit to recognize next conversion has finished
        // Add value
        tSumValue += ADCL | (ADCH << 8); // using myWord does not save space here
        // tSumValue += (ADCH << 8) | ADCL; // this does NOT work!
    }
    ADCSRA &= ~_BV(ADATE); // Disable auto-triggering (free running mode)
    return tSumValue;
}

/*
 * use ADC_PRESCALE32 which gives 26 us conversion time and good linearity
 * @return the maximum of aNumberOfSamples measurements.
 */
uint16_t readADCChannelWithReferenceMax(uint8_t aChannelNumber, uint8_t aReference, uint16_t aNumberOfSamples) {
    uint16_t tADCValue = 0;
    uint16_t tMaximum = 0;
    ADMUX = aChannelNumber | (aReference << SHIFT_VALUE_FOR_REFERENCE);

    ADCSRB = 0; // Free running mode. Only active if ADATE is set to 1.
    // ADSC-StartConversion ADATE-AutoTriggerEnable ADIF-Reset Interrupt Flag
    ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIF) | ADC_PRESCALE32);

    for (uint16_t i = 0; i < aNumberOfSamples; i++) {
        /*
         * wait for free running conversion to finish.
         * Do not wait for ADSC here, since ADSC is only low for 1 ADC Clock cycle on free running conversion.
         */
        loop_until_bit_is_set(ADCSRA, ADIF);

        ADCSRA |= _BV(ADIF); // clear bit to recognize next conversion has finished
        // check value
        tADCValue = ADCL | (ADCH << 8);
        if (tADCValue > tMaximum) {
            tMaximum = tADCValue;
        }
    }
    ADCSRA &= ~_BV(ADATE); // Disable auto-triggering (free running mode)
    return tMaximum;
}

/*
 * use ADC_PRESCALE32 which gives 26 us conversion time and good linearity
 */
uint16_t readADCChannelWithReferenceMaxMicros(uint8_t aChannelNumber, uint8_t aReference, uint16_t aMicrosecondsToAquire) {
    uint16_t tNumberOfSamples = aMicrosecondsToAquire / 26;
    return readADCChannelWithReferenceMax(aChannelNumber, aReference, tNumberOfSamples);
}

/*
 * aMaxRetries = 255 -> try forever
 * @return (tMax + tMin) / 2
 */
uint16_t readUntil4ConsecutiveValuesAreEqual(uint8_t aChannelNumber, uint8_t aDelay, uint8_t aAllowedDifference,
        uint8_t aMaxRetries) {
    int tValues[4];
    int tMin;
    int tMax;

    tValues[0] = readADCChannel(aChannelNumber);
    for (int i = 1; i < 4; ++i) {
        delay(aDelay); // Only 3 delays!
        tValues[i] = readADCChannel(aChannelNumber);
    }

    do {
        // find min and max
        tMin = 1024;
        tMax = 0;
        for (int i = 0; i < 4; ++i) {
            if (tValues[i] < tMin) {
                tMin = tValues[i];
            }
            if (tValues[i] > tMax) {
                tMax = tValues[i];
            }
        }
        /*
         * check for terminating condition
         */
        if ((tMax - tMin) <= aAllowedDifference) {
            break;
        } else {
//            Serial.print("Difference=");
//            Serial.println(tMax - tMin);

            // move values
            for (int i = 0; i < 3; ++i) {
                tValues[i] = tValues[i + 1];
            }
            // and wait
            delay(aDelay);
            tValues[3] = readADCChannel(aChannelNumber);
        }
        if (aMaxRetries != 255) {
            aMaxRetries--;
        }
    } while (aMaxRetries > 0);

    return (tMax + tMin) / 2;
}

/*
 * !!! Function without handling of switched reference and channel.!!!
 * Use it ONLY if you only call getVCCVoltageSimple() or getVCCVoltageMillivoltSimple() in your program.
 */
float getVCCVoltageSimple(void) {
    // use AVCC with (optional) external capacitor at AREF pin as reference
    float tVCC = readADCChannelWithReferenceMultiSamples(ADC_1_1_VOLT_CHANNEL_MUX, DEFAULT, 4);
    return ((1023 * 1.1 * 4) / tVCC);
}

/*
 * !!! Function without handling of switched reference and channel.!!!
 * Use it ONLY if you only call getVCCVoltageSimple() or getVCCVoltageMillivoltSimple() in your program.
 */
uint16_t getVCCVoltageMillivoltSimple(void) {
    // use AVCC with external capacitor at AREF pin as reference
    uint16_t tVCC = readADCChannelWithReferenceMultiSamples(ADC_1_1_VOLT_CHANNEL_MUX, DEFAULT, 4);
    return ((1023L * 1100 * 4) / tVCC);
}

/*
 * !!! Function without handling of switched reference and channel.!!!
 * Use it ONLY if you only use INTERNAL reference (call getTemperatureSimple()) in your program.
 */
float getTemperatureSimple(void) {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    return 0.0;
#else
// use internal 1.1 volt as reference
    float tTemp = (readADCChannelWithReferenceMultiSamples(ADC_TEMPERATURE_CHANNEL_MUX, INTERNAL, 4) - 317);
    return (tTemp * (4 / 1.22));
#endif
}

float getVCCVoltage(void) {
    return (getVCCVoltageMillivolt() / 1000.0);
}

/*
 * Read value of 1.1 volt internal channel using VCC as reference.
 * Handles reference and channel switching by introducing the appropriate delays.
 */
uint16_t getVCCVoltageMillivolt(void) {
    uint8_t tOldADMUX = checkAndWaitForReferenceAndChannelToSwitch(ADC_1_1_VOLT_CHANNEL_MUX, DEFAULT);
    uint16_t tVCC = readADCChannelWithReferenceOversample(ADC_1_1_VOLT_CHANNEL_MUX, DEFAULT, 2);
    ADMUX = tOldADMUX;
    /*
     * Do not wait for reference to settle here, since it may not be necessary
     */
    return ((1023L * 1100) / tVCC);
}

void printVCCVoltageMillivolt(Print *aSerial) {
    aSerial->print(F("VCC="));
    aSerial->print(getVCCVoltageMillivolt());
    aSerial->println(" mV");
}

/*
 * Handles reference and channel switching by introducing the appropriate delays.
 */
float getTemperature(void) {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    return 0.0;
#else
    // use internal 1.1 volt as reference
    uint8_t tOldADMUX = checkAndWaitForReferenceAndChannelToSwitch(ADC_TEMPERATURE_CHANNEL_MUX, INTERNAL);
    float tTemp = (readADCChannelWithReferenceOversample(ADC_TEMPERATURE_CHANNEL_MUX, INTERNAL, 2) - 317);
    ADMUX = tOldADMUX;
    return (tTemp / 1.22);
#endif
}
#elif defined(ARDUINO_ARCH_APOLLO3)
    void ADCUtilsDummyToAvoidBFDAssertions(){
        ;
    }
#endif // defined(__AVR__)
