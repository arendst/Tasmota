#include "SevenSegmentExtended.h"


SevenSegmentExtended::SevenSegmentExtended(uint8_t pinClk, uint8_t pinDIO) :
  SevenSegmentTM1637(pinClk, pinDIO)
{ };

void SevenSegmentExtended::printTime(uint16_t t, bool blink, uint16_t blinkDelay) {
  uint16_t max = 2359;
  t = ( t > max)?max:t;

  uint8_t hour = t / 100;
  t -= (hour * 100);
  uint8_t min  = t;
  printTime(hour, min, blink);
};

void SevenSegmentExtended::printTime(uint8_t hour, uint8_t min, bool blink, uint16_t blinkDelay) {

  bool colonWasOn = getColonOn();
  setColonOn(true);

  uint8_t buffer[4];

  buffer[0] = encode(int16_t(hour / 10));
  buffer[1] = encode(int16_t(hour % 10));
  buffer[2] = encode(int16_t(min / 10));
  buffer[3] = encode(int16_t(min % 10));

  printRaw(buffer, 4, 0);

  // turn colon off and on again
  if (blink) {
    delay(blinkDelay);
    setColonOn(false);
    printRaw(buffer[1],1);
    delay(blinkDelay);
    setColonOn(true);
    printRaw(buffer[1],1);
  }

  setColonOn(colonWasOn);

};

void SevenSegmentExtended::printNumber(int16_t number, bool zeroPadding, bool rollOver, bool alignRight) {
  const int16_t maxNumber = 9999;
  const int16_t minNumber = -999;
  bool positive = true;

  // get and store sign
  if (number < 0) {
    positive = false;
  };

  // roll over if rollOver is set to true
  if (rollOver == true) {
    if (positive == true) {
      number = number % int16_t(10000);
    } else {
      number = -1 * ((-1 * number) % 1000);
    }
  // limit number by default
  } else {
    number = number > maxNumber?maxNumber:number;
    number = number < minNumber?minNumber:number;
  }

  // clear the display first
  clear();

  // align left is the default behavior, just forward to print
  if (alignRight == false) {
    print(number);
    return;
  }

  if (positive == false) {
    number *= -1;
    print("-");
  }

  if (number > 999) {
    setCursor(0, 0);
  } else if (number > 99) {
    if (zeroPadding == true) {print('0');}
    setCursor(0, 1);
  } else if (number > 9) {
    if (zeroPadding == true) {print(F("00"));}
    setCursor(0, 2);
  } else {
    if (zeroPadding == true) {print(F("000"));}
    setCursor(0, 3);
  };

  print(number);
};

// positive counter 0..99, negative counter 0..-9
void SevenSegmentExtended::printDualCounter(int8_t leftCounter, int8_t rightCounter, bool zeroPadding) {

  int8_t max = 99;
  int8_t min = -9;
  uint8_t zeroByte = encode('0');

  leftCounter = (leftCounter > max)?max:leftCounter;
  leftCounter = (leftCounter < min)?min:leftCounter;
  rightCounter = (rightCounter > max)?max:rightCounter;
  rightCounter = (rightCounter < min)?min:rightCounter;

  bool colonWasOn = getColonOn();     // get current state
  setColonOn(true);                   // turn on the colon
  home();                             // set cursor to zero

  if ( leftCounter < 10 && leftCounter >= 0) {
    if ( zeroPadding ) {
      printRaw(zeroByte,1);
      setCursor(0,1);
    } else {
      // print colon
      printRaw(TM1637_COLON_BIT,1);
    };
  };
  print(leftCounter);

  if ( rightCounter < 10 && rightCounter >= 0) {
    if ( zeroPadding ) {
      printRaw(zeroByte,2);
    }
    uint8_t pos = 2;
    printRaw(0,pos);
    setCursor(0,3);
  } else {
    setCursor(0,2);
  }
  print(rightCounter);

  // set to previous state
  setColonOn(colonWasOn);
};
