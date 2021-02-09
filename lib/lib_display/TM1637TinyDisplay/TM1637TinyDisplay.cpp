//  TM1637 Tiny Display
//  Arduino tiny library for TM1637 LED Display
//
//  Author: Jason A. Cox - @jasonacox - https://github.com/jasonacox
//  Date: 27 June 2020
//
//  Based on TM1637Display library at https://github.com/avishorp/TM1637
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include <TM1637TinyDisplay.h>
#include <Arduino.h>

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] PROGMEM = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

// ASCII Map - Index 0 starts at ASCII 32
const uint8_t asciiToSegment[] PROGMEM = {
   0b00000000, // 032 (Space)
   0b00110000, // 033 !
   0b00100010, // 034 "
   0b01000001, // 035 #
   0b01101101, // 036 $
   0b01010010, // 037 %
   0b01111100, // 038 &
   0b00100000, // 039 '
   0b00111001, // 040 (
   0b00001111, // 041 )
   0b00100001, // 042 *
   0b01110000, // 043 +
   0b00001000, // 044 ,
   0b01000000, // 045 -
   0b00001000, // 046 .
   0b01010010, // 047 /
   0b00111111, // 048 0
   0b00000110, // 049 1
   0b01011011, // 050 2
   0b01001111, // 051 3
   0b01100110, // 052 4
   0b01101101, // 053 5
   0b01111101, // 054 6
   0b00000111, // 055 7
   0b01111111, // 056 8
   0b01101111, // 057 9
   0b01001000, // 058 :
   0b01001000, // 059 ;
   0b00111001, // 060 <
   0b01001000, // 061 =
   0b00001111, // 062 >
   0b01010011, // 063 ?
   0b01011111, // 064 @
   0b01110111, // 065 A
   0b01111100, // 066 B
   0b00111001, // 067 C
   0b01011110, // 068 D
   0b01111001, // 069 E
   0b01110001, // 070 F
   0b00111101, // 071 G
   0b01110110, // 072 H
   0b00000110, // 073 I
   0b00011110, // 074 J
   0b01110110, // 075 K
   0b00111000, // 076 L
   0b00010101, // 077 M
   0b00110111, // 078 N
   0b00111111, // 079 O
   0b01110011, // 080 P
   0b01100111, // 081 Q
   0b00110001, // 082 R
   0b01101101, // 083 S
   0b01111000, // 084 T
   0b00111110, // 085 U
   0b00011100, // 086 V
   0b00101010, // 087 W
   0b01110110, // 088 X
   0b01101110, // 089 Y
   0b01011011, // 090 Z
   0b00111001, // 091 [
   0b01100100, // 092 (backslash)
   0b00001111, // 093 ]
   0b00100011, // 094 ^
   0b00001000, // 095 _
   0b00100000, // 096 `
   0b01110111, // 097 a
   0b01111100, // 098 b
   0b01011000, // 099 c
   0b01011110, // 100 d
   0b01111001, // 101 e
   0b01110001, // 102 f
   0b01101111, // 103 g
   0b01110100, // 104 h
   0b00000100, // 105 i
   0b00011110, // 106 j
   0b01110110, // 107 k
   0b00011000, // 108 l
   0b00010101, // 109 m
   0b01010100, // 110 n
   0b01011100, // 111 o
   0b01110011, // 112 p
   0b01100111, // 113 q
   0b01010000, // 114 r
   0b01101101, // 115 s
   0b01111000, // 116 t
   0b00011100, // 117 u
   0b00011100, // 118 v
   0b00101010, // 119 w
   0b01110110, // 120 x
   0b01101110, // 121 y
   0b01011011, // 122 z
   0b00111001, // 123 {
   0b00110000, // 124 |
   0b00001111, // 125 }
   0b01000000, // 126 ~
   0b00000000  // 127 
};

static const uint8_t minusSegments = 0b01000000;
static const uint8_t degreeSegments = 0b01100011;

TM1637TinyDisplay::TM1637TinyDisplay(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay, unsigned int scrollDelay)
{
  // Copy the pin numbers
  m_pinClk = pinClk;
  m_pinDIO = pinDIO;
  m_bitDelay = bitDelay;
  m_scrollDelay = scrollDelay;

  // Set the pin direction and default value.
  // Both pins are set as inputs, allowing the pull-up resistors to pull them up
  pinMode(m_pinClk, INPUT);
  pinMode(m_pinDIO,INPUT);
  digitalWrite(m_pinClk, LOW);
  digitalWrite(m_pinDIO, LOW);
}

void TM1637TinyDisplay::setBrightness(uint8_t brightness, bool on)
{
  m_brightness = (brightness & 0x07) | (on? 0x08 : 0x00);
}

void TM1637TinyDisplay::setScrolldelay(unsigned int scrollDelay)
{
  m_scrollDelay = scrollDelay;
}

void TM1637TinyDisplay::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
  // Write COMM1
  start();
  writeByte(TM1637_I2C_COMM1);
  stop();

  // Write COMM2 + first digit address
  start();
  writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

  // Write the data bytes
  for (uint8_t k=0; k < length; k++)
    writeByte(segments[k]);

  stop();

  // Write COMM3 + brightness
  start();
  writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
  stop();
}

void TM1637TinyDisplay::clear()
{
  uint8_t data[] = { 0, 0, 0, 0 };
  setSegments(data);
}

void TM1637TinyDisplay::showNumber(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
  showNumberDec(num, 0, leading_zero, length, pos);
}

void TM1637TinyDisplay::showNumber(double num, uint8_t decimal_length, uint8_t length, uint8_t pos)
{
  int num_len = 0;              
  int inum = abs((int)num);  
  int decimal_places = 0;       
  double value = 0.0;
  bool negative = false; 
  bool leading_zero = false; 
  uint8_t digits[4] = {0,0,0,0};  // output array to render

  // determine length of whole number part of num
  while(inum != 0) {  
    inum = inum / 10;
    num_len++;
  }
  if(num < 0) {
    num_len++; // make space for negative
    negative = true;
  }
  if(abs(num)<1) {
    num_len++; // make space for 0. prefix
    leading_zero = true;
  }
  // make sure we can display number otherwise show overflow
  if(num_len > length) {
    showString("----", length, pos); // overflow symbol
    return;
  }
  // how many decimal places can we show?
  decimal_places = length - num_len; 
  if(decimal_places > decimal_length) {
    decimal_places = decimal_length;
  }
  // construct whole number representation of num
  value = num;
  for(int x=0; x < decimal_places; x++) {
    value = value * 10.00;
  }
  if(num>0) value = value + 0.5; // round up
  if(num<0) value = value - 0.5; // round down
  inum = abs((int)value);

  // render display array
  if (inum == 0 && !leading_zero) {
    digits[length-1] = encodeDigit(0);
  }
  else {		
    int decimal_pos = length - 1 - decimal_places + pos;
    for(int i = length-1; i >= 0; --i) {
      uint8_t digit = inum % 10;
      
      if (digit == 0 && inum == 0 && 
           (leading_zero == false || (i < decimal_pos))
         )
          // Blank out any leading zeros except for 0.x case
        digits[i] = 0;
      else
        digits[i] = encodeDigit(digit);
      if(i == decimal_pos && decimal_length > 0) {
        digits[i] += 0b10000000; // add decimal point
      }
      inum /= 10;
    }
  }
  // add negative sign for negative number
  if(negative) {
    digits[pos] = minusSegments;
  }
  setSegments(digits, length, pos);
}

void TM1637TinyDisplay::showNumberDec(int num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637TinyDisplay::showNumberHex(uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void TM1637TinyDisplay::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  bool negative = false;
  if (base < 0) {
    base = -base;
    negative = true;
  }

  uint8_t digits[4];

  if (num == 0 && !leading_zero) {
    // Singular case - take care separately
    for(uint8_t i = 0; i < (length-1); i++) {
      digits[i] = 0;
    }
    digits[length-1] = encodeDigit(0);
  }
  else {		
    for(int i = length-1; i >= 0; --i) {
      uint8_t digit = num % base;

      if (digit == 0 && num == 0 && leading_zero == false)
          // Leading zero is blank
        digits[i] = 0;
      else
        digits[i] = encodeDigit(digit);
        
      if (digit == 0 && num == 0 && negative) {
        digits[i] = minusSegments;
        negative = false;
      }

      num /= base;
    }
  }
  if(dots != 0) {
    showDots(dots, digits);
  }
  setSegments(digits, length, pos);
}

void TM1637TinyDisplay::showString(const char s[], uint8_t length, uint8_t pos)
{
  uint8_t digits[4] = {0,0,0,0};

  if (strlen(s) <= 4) {
    switch (strlen(s)) {
      case 4:
        digits[3] = encodeASCII(s[3]);
        // fall through
      case 3:
        digits[2] = encodeASCII(s[2]);
        // fall through
      case 2:
        digits[1] = encodeASCII(s[1]);
        // fall through
      case 1:
        digits[0] = encodeASCII(s[0]);
        // fall through
      case 0:
        setSegments(digits, length, pos);
    }
  }
  if (strlen(s) > 4) {
    // Scroll text on display if too long
    for (int x = 0; x < 3; x++) {  // Scroll message on
      digits[0] = digits[1];
      digits[1] = digits[2];
      digits[2] = digits[3];
      digits[3] = encodeASCII(s[x]);
      setSegments(digits, length, pos);
      delay(m_scrollDelay);
    }
    for (size_t x = 3; x < strlen(s); x++) { // Scroll through string
      digits[0] = encodeASCII(s[x - 3]);
      digits[1] = encodeASCII(s[x - 2]);
      digits[2] = encodeASCII(s[x - 1]);
      digits[3] = encodeASCII(s[x]);
      setSegments(digits, length, pos);
      delay(m_scrollDelay);
    }
    for (int x = 0; x < 4; x++) {  // Scroll message off
      digits[0] = digits[1];
      digits[1] = digits[2];
      digits[2] = digits[3];
      digits[3] = 0;
      setSegments(digits, length, pos);
      delay(m_scrollDelay);
    }
  }
}

void TM1637TinyDisplay::showString_P(const char s[], uint8_t length, uint8_t pos) 
{
  uint8_t digits[4] = {0,0,0,0};

  if (strlen_P(s) <= 4) {
    switch (strlen_P(s)) {
      case 4:
        digits[3] = encodeASCII(pgm_read_byte(&s[3]));
        // fall through
      case 3:
        digits[2] = encodeASCII(pgm_read_byte(&s[2]));
        // fall through
      case 2:
        digits[1] = encodeASCII(pgm_read_byte(&s[1]));
        // fall through
      case 1:
        digits[0] = encodeASCII(pgm_read_byte(&s[0]));
        // fall through
      case 0:
        setSegments(digits, length, pos);
    }
  }
  else {
    // Scroll text on display if too long
    for (int x = 0; x < 3; x++) {  // Scroll message on
      digits[0] = digits[1];
      digits[1] = digits[2];
      digits[2] = digits[3];
      digits[3] = encodeASCII(pgm_read_byte(&s[x]));
      setSegments(digits, length, pos);
      delay(m_scrollDelay);
    }
    for (size_t x = 3; x < strlen_P(s); x++) { // Scroll through string
      digits[0] = encodeASCII(pgm_read_byte(&s[x - 3]));
      digits[1] = encodeASCII(pgm_read_byte(&s[x - 2]));
      digits[2] = encodeASCII(pgm_read_byte(&s[x - 1]));
      digits[3] = encodeASCII(pgm_read_byte(&s[x]));
      setSegments(digits, length, pos);
      delay(m_scrollDelay);
    }
    for (int x = 0; x < 4; x++) {  // Scroll message off
      digits[0] = digits[1];
      digits[1] = digits[2];
      digits[2] = digits[3];
      digits[3] = 0;
      setSegments(digits, length, pos);
      delay(m_scrollDelay);
    }
  }
}

void TM1637TinyDisplay::showLevel(unsigned int level, bool horizontal) 
{
  uint8_t digits[4] = {0,0,0,0};
  uint8_t digit = 0b00000000;

  if(level>100) level=100;

  if(horizontal) {
    // Must fit within 3 bars
    int bars = (int)((level*3)/100.0);
    if(bars == 0 && level > 0) bars = 1; // Only level=0 turns off display
    switch(bars) {
      case 1:
        digit = 0b00001000;
        break;
      case 2:
        digit = 0b01001000;
        break;
      case 3:
        digit = 0b01001001;
        break;
      default: // Keep at zero
        break;
    }
    for(int x = 0; x < 4; x++) {
      digits[x] = digit;
    }
  }
  else {
    // Must fit within 8 bars
    int bars = (int)((level*8)/100.0);
    if(bars == 0 && level > 0) bars = 1;
    for(int x = 0; x<4; x++) { // for each digit
      int left = bars-(x*2);
      if(left > 1) digits[x] = 0b00110110;
      if(left == 1) digits[x] = 0b00110000;
    }
  }
  setSegments(digits);
}

void TM1637TinyDisplay::showAnimation(const uint8_t data[][4], unsigned int frames, unsigned int ms)
{
  // Animation sequence
  for (unsigned int x = 0; x < frames; x++) {
    setSegments(data[x]);
    delay(ms);
  }
}

void TM1637TinyDisplay::showAnimation_P(const uint8_t data[][4], unsigned int frames, unsigned int ms)
{
  // Animation sequence for data stored in PROGMEM flash memory
  uint8_t digits[4] = {0,0,0,0};
  for (unsigned int x = 0; x < frames; x++) {
    for(unsigned int a = 0; a < 4; a++) {
          digits[a] = pgm_read_byte(&(data[x][a]));
    }
    setSegments(digits);
    delay(ms);
  }
}

void TM1637TinyDisplay::bitDelay()
{
  delayMicroseconds(m_bitDelay);
}

void TM1637TinyDisplay::start()
{
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
}

void TM1637TinyDisplay::stop()
{
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinDIO, INPUT);
  bitDelay();
}

bool TM1637TinyDisplay::writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    pinMode(m_pinClk, OUTPUT);
    bitDelay();

    // Set data bit
    if (data & 0x01)
      pinMode(m_pinDIO, INPUT);
    else
      pinMode(m_pinDIO, OUTPUT);

    bitDelay();

    // CLK high
    pinMode(m_pinClk, INPUT);
    bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO, INPUT);
  bitDelay();
 
  // CLK to high
  pinMode(m_pinClk, INPUT);
  bitDelay();
  uint8_t ack = digitalRead(m_pinDIO);
  if (ack == 0)
    pinMode(m_pinDIO, OUTPUT);

  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  return ack;
}

void TM1637TinyDisplay::showDots(uint8_t dots, uint8_t* digits)
{
  for(int i = 0; i < 4; ++i)
  {
      digits[i] |= (dots & 0x80);
      dots <<= 1;
  }
}

uint8_t TM1637TinyDisplay::encodeDigit(uint8_t digit)
{
  // return digitToSegment[digit & 0x0f] using PROGMEM
  return pgm_read_byte(digitToSegment + (digit & 0x0f));
}

uint8_t TM1637TinyDisplay::encodeASCII(uint8_t chr)
{
  if(chr == 176) return degreeSegments;   // Degree mark
  if(chr > 127 || chr < 32) return 0;     // Blank
  // return asciiToSegment[chr - 32] using PROGMEM
  return pgm_read_byte(asciiToSegment + (chr - 32));
}
