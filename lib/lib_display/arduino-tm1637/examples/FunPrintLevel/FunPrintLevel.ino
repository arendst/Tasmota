#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "SevenSegmentFun.h"

// define clock and digital input pins
#define PIN_CLK   4
#define PIN_DIO   5

// initialize TM1637 Display objects
SevenSegmentFun    display(PIN_CLK, PIN_DIO);


void setup() {
  // initialize the display
  display.begin();
}

void loop() {

  // increase level from 0 to 100
  for (uint8_t i=0; i <= 100; i+=25) {
    display.printLevelVertical(i);
    delay(100);
  };
  // decrease level from 100 to 0
  for (int8_t i=100; i >=0; i-=25) {
    display.printLevelVertical(i);
    delay(100);
  }

  // initialize horizontal level counters
  int8_t i,j,k,l;
  uint8_t levels[4];


  // repeat 5 times
  for ( uint8_t r=0; r < 4; r++) {
    // increae i and k, decrease j and l
    for (i=0,j=100,k=0,l=100; i <= 100; i+=33, j-=33, k+=33, l-=33) {
        levels[0] = i; levels[1] = j; levels[2] = k; levels[3] = l;
        display.printLevelHorizontal(levels);
        delay(100);
    }
    // increae j and l, decrease i and k
    for (i=100,j=0,k=100,l=0; i >= 0; i-=33, j+=33, k-=33, l+=33) {
        levels[0] = i; levels[1] = j; levels[2] = k; levels[3] = l;
        display.printLevelHorizontal(levels);
        delay(100);
    }
  }

}
