/*
* Project Name: TM1638plus 
* File: TM1638plus_common
* Description: cpp  file for common data and functions between model 1 and 2 classes
* Arduino library TM1638plus
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include "TM1638plus_common.h"


TM1638plus_common::TM1638plus_common()
{
	// Blank constructor
}

uint8_t  TM1638plus_common::HighFreqshiftin(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) 
{
    uint8_t value = 0;
    uint8_t i = 0;

    for(i = 0; i < 8; ++i) {
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
            
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
    }
    return value;
}

void TM1638plus_common::HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else    
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
            
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
    }
}
