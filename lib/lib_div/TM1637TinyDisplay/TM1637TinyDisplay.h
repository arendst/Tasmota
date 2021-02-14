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

#ifndef __TM1637TINYDISPLAY__
#define __TM1637TINYDISPLAY__

// Include PROGMEM Support
#include <inttypes.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#define pgm_read_byte(addr)                                                    \
  (*(const unsigned char *)(addr)) // workaround for non-AVR
#endif

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define BRIGHT_LOW  0x00
#define BRIGHT_0    0x00
#define BRIGHT_1    0x01
#define BRIGHT_2    0x02
#define BRIGHT_3    0x03
#define BRIGHT_4    0x04
#define BRIGHT_5    0x05
#define BRIGHT_6    0x06
#define BRIGHT_7    0x07
#define BRIGHT_HIGH 0x0f

#define ON    1
#define OFF   0

#define DEFAULT_BIT_DELAY     100
#define DEFAULT_SCROLL_DELAY  100

#define FRAMES(a)     sizeof(a)/4
#define TIME_MS(t)    t
#define TIME_S(t)     t*1000

class TM1637TinyDisplay {

public:
  //! Initialize a TM1637TinyDisplay object, setting the clock and
  //! data pins.
  //!
  //! @param pinClk - The number of the digital pin connected to the clock pin of the module
  //! @param pinDIO - The number of the digital pin connected to the DIO pin of the module
  //! @param bitDelay - The delay, in microseconds, between bit transition on the serial
  //!                   bus connected to the display
  TM1637TinyDisplay(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY, unsigned int scrollDelay = DEFAULT_SCROLL_DELAY);

  //! Sets the brightness of the display.
  //!
  //! The setting takes effect when a command is given to change the data being
  //! displayed.
  //!
  //! @param brightness A number from 0 (lowest brightness) to 7 (highest brightness)
  //! @param on Turn display on or off
  void setBrightness(uint8_t brightness, bool on = true);

  //! Sets the delay used to scroll string text (in ms).
  //!
  //! The setting takes effect when a showString() command send an argument with over
  //! four characters.
  //!
  //! @param scrollDelay A number in milliseconds (default is 200)
  void setScrolldelay(unsigned int scrollDelay = 200);

  //! Display arbitrary data on the module
  //!
  //! This function receives raw segment values as input and displays them. The segment data
  //! is given as a byte array, each byte corresponding to a single digit. Within each byte,
  //! bit 0 is segment A, bit 1 is segment B etc.
  //! The function may either set the entire display or any desirable part on its own. The first
  //! digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length
  //! argument is the number of digits to be set. Other digits are not affected.
  //!
  //! @param segments An array of size @ref length containing the raw segment values
  //! @param length The number of digits to be modified
  //! @param pos The position from which to start the modification (0 - leftmost, 3 - rightmost)
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

  //! Clear the display
  void clear();

  //! Display a decimal number
  //!
  //! Display the given argument as a decimal number.
  //!
  //! @param num The number to be shown
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank. NOTE: leading zero is not supported with negative numbers.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
  void showNumber(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  //! Display a decimal number with floating point
  //!
  //! Display the given argument as a decimal number. Decimal point will only show up on displays
  //! that support decimal point.
  //!
  //! @param num The number to be shown
  //! @param decimal_length Format to show only this number of digits after the decimal point.
  //!        NOTE: Anything over 4 will do best fit.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
  void showNumber(double num, uint8_t decimal_length = 4, uint8_t length = 4, uint8_t pos = 0);

  //! Display a decimal number, with dot control
  //!
  //! Display the given argument as a decimal number. The dots between the digits (or colon)
  //! can be individually controlled.
  //!
  //! @param num The number to be shown
  //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
  //!        between the digits (or colon mark, as implemented by each module). i.e.
  //!        For displays with dots between each digit:
  //!        * 0.000 (0b10000000)
  //!        * 00.00 (0b01000000)
  //!        * 000.0 (0b00100000)
  //!        * 0000. (0b00010000)
  //!        * 0.0.0.0 (0b11100000)
  //!        For displays with just a colon:
  //!        * 00:00 (0b01000000)
  //!        For displays with dots and colons colon:
  //!        * 0.0:0.0 (0b11100000)
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank. NOTE: leading zero is not supported with negative numbers.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
  void showNumberDec(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  //! Display a hexadecimal number, with dot control
  //!
  //! Display the given argument as a hexadecimal number. The dots between the digits (or colon)
  //! can be individually controlled.
  //!
  //! @param num The number to be shown
  //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
  //!        between the digits (or colon mark, as implemented by each module). i.e.
  //!        For displays with dots between each digit:
  //!        * 0.000 (0b10000000)
  //!        * 00.00 (0b01000000)
  //!        * 000.0 (0b00100000)
  //!        * 0000. (0b00010000)
  //!        * 0.0.0.0 (0b11100000)
  //!        For displays with just a colon:
  //!        * 00:00 (0b01000000)
  //!        For displays with dots and colons colon:
  //!        * 0.0:0.0 (0b11100000)
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
  void showNumberHex(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  //! Translate a single digit into 7 segment code
  //!
  //! The method accepts a number between 0 - 15 and converts it to the
  //! code required to display the number on a 7 segment display.
  //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
  //!
  //! @param digit A number between 0 to 15
  //! @return A code representing the 7 segment image of the digit (LSB - segment A;
  //!         bit 6 - segment G; bit 7 - always zero)
  uint8_t encodeDigit(uint8_t digit);

  //! Display a string
  //!
  //! Display the given string and if more than 4 characters, will scroll message on display
  //!
  //! @param s The string to be shown
  //! @param scrollDelay  The delay, in microseconds to wait before scrolling to next frame
  //! @param length The number of digits to set. 
  //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
  //! The _P function is for reading PROGMEM read-only flash memory space instead of RAM
  void showString(const char s[], uint8_t length = 4, uint8_t pos = 0);

  //! Display a string (PROGMEM space)
  //!
  //! Display the given string and if more than 4 characters, will scroll message on display
  //! This function is for reading PROGMEM read-only flash memory space instead of RAM
  //!
  //! @param s The string to be shown
  //! @param scrollDelay  The delay, in microseconds to wait before scrolling to next frame
  //! @param length The number of digits to set. 
  //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
  //! The _P function is for reading PROGMEM read-only flash memory space instead of RAM
  void showString_P(const char s[], uint8_t length = 4, uint8_t pos = 0);

  //! Display a Level Indicator (both orientations)
  //!
  //! Illumiate LEDs to provide a visual indicator of level (horizontal or vertical orientation)
  //!
  //! @param level A value between 0 and 100 (representing percentage)
  //! @param horizontal Boolean (true/false) where true = horizontal, false = vertical
  void showLevel(unsigned int level = 100, bool horizontal = true);
  
  //! Display a sequence of raw LED segment data to create an animation
  //!
  //! Play thorugh an array of raw LED segment data to create a moving pattern.  
  //!
  //! const uint8_t Example[2][4] = 
  //! {
  //!  {                // frame 1
  //!   0b00001000,                                     // digit 1
  //!   0b00000000,                                     // digit 2
  //!   0b00000000,                                     // digit 3
  //!   0b00000000                                      // digit 4
  //!  },
  //!  {                // frame 2
  //!   0b00000000,                                     // digit 1
  //!   0b00001000,                                     // digit 2
  //!   0b00000000,                                     // digit 3
  //!   0b00000000                                      // digit 4
  //!  }
  //! }
  //! @param data A multi-dimensional array containing the LED segment - data[frames][4]
  //! @param frames Number of frames in the sequence to animate
  //! @param ms Time to delay between each frame
  //! The _P function is for reading PROGMEM read-only flash memory space instead of RAM
  void showAnimation(const uint8_t data[][4], unsigned int frames = 0, unsigned int ms = 10);

  //! Display a sequence of raw LED segment data to create an animation (PROGMEM)
  //!
  //! Play thorugh an array of raw LED segment data to create a moving pattern.  
  //! This function is for reading PROGMEM read-only flash memory space instead of RAM
  //!
  //! const uint8_t Example[2][4] = 
  //! {
  //!  {                // frame 1
  //!   0b00001000,                                     // digit 1
  //!   0b00000000,                                     // digit 2
  //!   0b00000000,                                     // digit 3
  //!   0b00000000                                      // digit 4
  //!  },
  //!  {                // frame 2
  //!   0b00000000,                                     // digit 1
  //!   0b00001000,                                     // digit 2
  //!   0b00000000,                                     // digit 3
  //!   0b00000000                                      // digit 4
  //!  }
  //! }
  //! @param data A multi-dimensional array containing the LED segment - data[frames][4]
  //! @param frames Number of frames in the sequence to animate
  //! @param ms Time to delay between each frame
  void showAnimation_P(const uint8_t data[][4], unsigned int frames = 0, unsigned int ms = 10);

  //! Translate a single ASCII character into 7 segment code
  //!
  //! The method accepts a number between 0 - 255 and converts it to the
  //! code required to display the number on a 7 segment display.
  //! ASCII Characters between 0-32 and 128-255 are blank.
  //!
  //! @param chr A character ASCII value 
  //! @return A code representing the 7 segment image of the digit (LSB - segment A;
  //!         bit 6 - segment G; bit 7 - always zero)
  uint8_t encodeASCII(uint8_t chr);

protected:
   void bitDelay();

   void start();

   void stop();

   bool writeByte(uint8_t b);

   void showDots(uint8_t dots, uint8_t* digits);
   
   void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

private:
  uint8_t m_pinClk;
  uint8_t m_pinDIO;
  uint8_t m_brightness;
  unsigned int m_bitDelay;
  unsigned int m_scrollDelay;
};

#endif // __TM1637TINYDISPLAY__
