#ifndef SevenSegmentFun_H
#define SevenSegmentFun_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

// COMPILE TIME USER CONFIG ////////////////////////////////////////////////////
#define  TM1637_SNAKE_DEFAULT_DELAY         50  // Snake step delay ms
#define  TM1637_BOUNCING_BALL_DEFAULT_DELAY 100 // Bouncing ball delay ms
#define  TM1637_NIGHTRIDER_DEFAULT_DELAY    200 // Nightrider delay ms
//
//        A
//       ---
//  * F |   | B
// X     -G-
//  * E |   | C
//       ---
//        D
// X defines the colon (dots) and only applies to byte 1 (second from left)
//                BXGFEDCBA

#define TM1637_CHAR_VERT_LEVEL_I0   B00110000  // |  one bar left
#define TM1637_CHAR_VERT_LEVEL_0I   B00000110  // |  one bar right
#define TM1637_CHAR_VERT_LEVEL_II   B00110110  // || two bars

#define TM1637_CHAR_HOR_LEVEL_0    B00000000
#define TM1637_CHAR_HOR_LEVEL_1    B00001000
#define TM1637_CHAR_HOR_LEVEL_2    B01001000
#define TM1637_CHAR_HOR_LEVEL_3    B01001001

#define TM1637_CHAR_SNAKE_0        B00000001
#define TM1637_CHAR_SNAKE_1        B00000010
#define TM1637_CHAR_SNAKE_2        B00000100
#define TM1637_CHAR_SNAKE_3        B00001000
#define TM1637_CHAR_SNAKE_4        B00010000
#define TM1637_CHAR_SNAKE_5        B00100000

#define TM1637_CHAR_BALL_HIGH      B01100011
#define TM1637_CHAR_BALL_LOW       B01011100

class SevenSegmentFun : public SevenSegmentExtended {
  public:
    SevenSegmentFun(uint8_t pinClk, uint8_t pinDIO);
    void printLevelVertical(uint8_t level, bool leftToRight = true);
    void printLevelVertical(uint8_t level, bool leftToRight, uint8_t symbol);
    void printLevelHorizontal(uint8_t levels[4]);
    void scrollingText(const char* str, uint8_t repeats);
    void snake(uint8_t repeats = 1, uint16_t d = TM1637_SNAKE_DEFAULT_DELAY);
    void nightrider(uint8_t repeats = 10, uint16_t d = TM1637_NIGHTRIDER_DEFAULT_DELAY, uint8_t symbol = TM1637_CHAR_VERT_LEVEL_II);
    void bombTimer(uint8_t hours, uint8_t min, uint16_t speed = 60);
    void bombTimer(uint8_t hours, uint8_t min, uint16_t speed, char* str);
    void bouncingBall(uint16_t moves, uint16_t d, bool runForever = false);
    void printBall(const int8_t x, const int8_t y);
    void print4Bit(const uint8_t x, const uint8_t y, uint8_t symbol);

  private:
};

#endif
