#include "SevenSegmentFun.h"

const PROGMEM uint8_t levelVerticalMap[4] = {
  TM1637_CHAR_HOR_LEVEL_0,
  TM1637_CHAR_HOR_LEVEL_1,
  TM1637_CHAR_HOR_LEVEL_2,
  TM1637_CHAR_HOR_LEVEL_3
};

SevenSegmentFun::SevenSegmentFun(uint8_t pinClk, uint8_t pinDIO) :
  SevenSegmentExtended(pinClk, pinDIO)
{
  randomSeed(analogRead(0));
};

// 9 levels ( 0, 12.5, 25, 37.4, 50, 62.5, 75.0, 87.5, 100)
void  SevenSegmentFun::printLevelVertical(uint8_t level, bool leftToRight) {

  level = (level > 100?100:level);
  float levelScale = (100 / (TM1637_MAX_COLOM * 2.0));

  uint8_t barsOn = float(level) / levelScale;

  uint8_t d = leftToRight == true?0:TM1637_MAX_COLOM-1;

  for (uint8_t i=0; i < TM1637_MAX_COLOM; i++) {
    if (barsOn - (2 * (i + 1)) >= 0) {
      _rawBuffer[d] = TM1637_CHAR_VERT_LEVEL_II;
    } else if (barsOn - (2 * (i + 1)) >= 1) {
      _rawBuffer[d] = (leftToRight == true)?TM1637_CHAR_VERT_LEVEL_I0:TM1637_CHAR_VERT_LEVEL_0I;
    } else {
      _rawBuffer[d] = 0;
    }
    if (leftToRight == true) {d++;}
    else {d--;};
  };
  printRaw(_rawBuffer, TM1637_MAX_COLOM, 0);
};


void  SevenSegmentFun::printLevelVertical(uint8_t level, bool leftToRight, uint8_t symbol) {
  level /= (100 / TM1637_MAX_COLOM);
  level = (level > TM1637_MAX_COLOM)?TM1637_MAX_COLOM:level;

  for( uint8_t i=0; i < TM1637_MAX_COLOM;i++) {
    if ( ( i < level && leftToRight ) || ( i >= ( TM1637_MAX_COLOM - level ) && !leftToRight) ) {
      _rawBuffer[i] = symbol;
    } else {
      _rawBuffer[i] = 0;
    }
  };

  printRaw(_rawBuffer, TM1637_MAX_COLOM, 0);

};

void  SevenSegmentFun::nightrider(uint8_t repeats, uint16_t d, uint8_t symbol) {

  uint8_t buffer[((TM1637_MAX_COLOM * 2) - 1)] = {0, };

  for ( uint8_t i= 0; i < ((TM1637_MAX_COLOM * 2) - 1); i++) {
    if ( i == (TM1637_MAX_COLOM - 1) ) {
      buffer[i] = symbol;
    } else {
      buffer[i] = 0;
    }
  };

  for( int8_t r=0; r < repeats; r++) {
    for (int8_t i = (TM1637_MAX_COLOM - 1); i > 0; i--) {
      printRaw( &buffer[i],4,0);
      delay(d);
    };
    for ( int8_t i = 0; i < (TM1637_MAX_COLOM - 1); i++) {
      printRaw( &buffer[i], TM1637_MAX_COLOM, 0);
      delay(d);
    };
  }
}
// 5 levels ( 0, 33, 66, 100)
void  SevenSegmentFun::printLevelHorizontal( uint8_t levels[4] ) {
  for (uint8_t i=0; i < TM1637_MAX_COLOM;i++) {
    levels[i] /= (100 / 3);
    levels[i] = ( levels[i] > 3)?3:levels[i];
    _rawBuffer[i] =  pgm_read_byte_near(levelVerticalMap + levels[i]);
  };
  printRaw(_rawBuffer, TM1637_MAX_COLOM, 0);
}

void  SevenSegmentFun::scrollingText(const char* str, uint8_t repeats) {

    uint8_t buffer[TM1637_MAX_CHARS];

    size_t length = encode(&buffer[3], str, TM1637_MAX_CHARS);

    uint8_t paddingBegin = TM1637_MAX_COLOM - 1;
    uint8_t totalPadding = (2 * paddingBegin) + 1;

    // padd with three spaces in front
    for(uint8_t i=0; i < paddingBegin; i++) {
      buffer[i] = 0;
    };
    // padd with four spaces at the end (empty scree)
    for(uint8_t i = length + paddingBegin; i < length + totalPadding ; i++) {
      buffer[i] = 0;
    };

    for( ; repeats != 0; repeats--) {
      printRaw(buffer, length + totalPadding);
      if ( repeats > 250 ) { // more than 250 -> forever
        repeats++;
      }
    };
}

void  SevenSegmentFun::snake(uint8_t repeats, uint16_t d) {

  uint8_t outerEdges = (4 * TM1637_MAX_LINES) + (TM1637_MAX_COLOM * 2);
  uint8_t widthEdged = (TM1637_MAX_COLOM * 2);

  // number of repeats/rounds
  for ( uint8_t r=0; r < repeats; r++) {
    // 12 steps for all outer edges
    for (uint8_t i=0; i < outerEdges; i++) {
      for (uint8_t j=0; j < TM1637_MAX_COLOM; j++) {

        // set segments
        // top edges
        if ( i == j) {
          _rawBuffer[j] =TM1637_CHAR_SNAKE_0;
        }
        // top right edge
        else if ( i == TM1637_MAX_COLOM && j == 3) {
          _rawBuffer[j] =TM1637_CHAR_SNAKE_1;
        }
        // bottom left edge
        else if ( i == (TM1637_MAX_COLOM + 1) && j == 3) {
          _rawBuffer[j] =TM1637_CHAR_SNAKE_2;
        }
        // bottom edges
        else if ( i + j == (widthEdged + 1) ) {
          _rawBuffer[j] =TM1637_CHAR_SNAKE_3;
        }
        // bottom left edge
        else if ( i == (widthEdged + 2) && j == 0) {
          _rawBuffer[j] =TM1637_CHAR_SNAKE_4;
        }
        // top left edge
        else if ( i == (widthEdged + 3) && j == 0) {
          _rawBuffer[j] =TM1637_CHAR_SNAKE_5;
        } else {
          _rawBuffer[j] = 0;
        };
      };
      printRaw(_rawBuffer, 4, 0);
      delay(d);
    }

  };
}

void  SevenSegmentFun::bombTimer(uint8_t hours, uint8_t min, uint16_t speed) {

  // maximum speed is 60000 -> 1min per ms
  speed = (speed > 60000)?60000:speed;
  uint16_t d = 60000 / speed;

  // copy start minute
  int8_t m = min;
  int8_t h;
  for ( h=hours; h >= 0; h--) {
    for ( ; m >= 0; m--) {
      printTime((uint8_t)h, (uint8_t)m);
      delay(d);
    };
    m = 59;   // reset minute timer
  };
  blink();
};

void  SevenSegmentFun::bombTimer(uint8_t hours, uint8_t min, uint16_t speed, char* str) {
  bombTimer(hours, min, speed);
  print(str);
  blink();
};

void  SevenSegmentFun::bouncingBall(uint16_t moves, uint16_t d, bool runForever) {

  int8_t wallRight  = TM1637_MAX_COLOM - 1;
  int8_t wallBottom = TM1637_MAX_LINES;

  // start at top left
  int8_t x = 0; int8_t y = 0;
  int8_t newX; int8_t newY;
  // default speed is 1 x right and 2 y down
  int8_t vx = 1; int8_t vy = 1;
  bool flipVx = false; bool flipVy = false;

  // print initial state
  printBall(x, y);

  for ( ; moves != 0; moves--) {

    newX = x + vx;
    newY = y + vy;

    // check if ball has hit the wall
    if (newX > wallRight || newX < 0 ) {
      vx = vx * -1;
      newX = x + vx;
      // random flip other direction as well
      flipVy = (random(3) == 1);
    };

    if ( newY > wallBottom || newY < 0) {
      vy = vy * -1;
      newY = y + vy;
      // random flip other direction as well
      flipVx = (random(3) == 1);
    };

    if ( flipVy ) {
      newY = y;
    } else if ( flipVx ) {
      newX = x;
    };
    // set new coordinates
    x = newX; y = newY;

    printBall(x, y);
    delay(d);

    if (runForever) {
      moves++;
    };
  };
}

void SevenSegmentFun::printBall( const int8_t x, const int8_t y ) {
  // uint8_t buffer[4] = {0,0,0,0};
  // buffer[x] = (y > 0)?TM1637_CHAR_BALL_LOW:TM1637_CHAR_BALL_HIGH;
  // printRaw(buffer);

  uint8_t symbol = (y > 0)?TM1637_CHAR_BALL_LOW:TM1637_CHAR_BALL_HIGH;

  print4Bit(x, y, symbol);

};

void SevenSegmentFun::print4Bit( const uint8_t x, const uint8_t y, uint8_t symbol) {
  uint8_t buffer[4] = {0,0,0,0};
  buffer[x] = symbol;
  printRaw(buffer);
}
