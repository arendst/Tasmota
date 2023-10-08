#ifndef SevenSegmentExtended_H
#define SevenSegmentExtended_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "SevenSegmentTM1637.h"

class SevenSegmentExtended : public SevenSegmentTM1637 {

public:
SevenSegmentExtended(uint8_t pinClk, uint8_t pinDIO);
/* Prints given time to the display
@param [in] hour          hours or minutes
@param [in] min           minutes or seconds
*/
void    printTime(uint8_t hour, uint8_t min, bool blink = false, uint16_t blinkDelay = TM1637_DEFAULT_CLOCK_BLINK_DELAY);
/* Prints given time to the display
@param [in] t             time given as an int, e.g. 1643 prints 16:43
*/
void    printTime(uint16_t t, bool blink = false, uint16_t blinkDelay = TM1637_DEFAULT_CLOCK_BLINK_DELAY);
/* Prints a number and aligns it (right by default)

  Can print both positive and negative numbers. When the the number is larger then 9999
  it will roll over to 0, the same happens for a negative number smaller then -999.

@param [in] number        the number to print to the display
@param [in] alignRight    (optional) set to true to alightright (default)
@param [in] rollOver      (optional) set to true make the number rollover*/
void    printNumber(int16_t number, bool zeroPadding = false, bool rollOver = false, bool alignRight = true);
/* Print two one or two digit numbers to the display
* Prints a number to the left and right of the display
*
@param [in] leftCounter   the number on the left side of the display
@param [in] rightcounter  the numnber on the right side of the display
@param [in] zeroPadding   optional: pad counters with zero
*/
void    printDualCounter(int8_t leftCounter, int8_t rightCounter, bool zeroPadding = false);

};
#endif
