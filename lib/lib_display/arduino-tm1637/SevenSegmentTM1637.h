/*
  SevenSegmentTM1637 - class to control a 4 digit seven segment display with a TM1636 or TM1637 driver IC
  Created by Bram Harmsen, September 25, 2015
  Released into the public domain.
  Licence: GNU GENERAL PUBLIC LICENSE V2.0

  # Changelog

  v1.0  25-10-2015
  v1.1  04-07-2020

*/

#ifndef SevenSegmentTM1637_H
#define SevenSegmentTM1637_H

#include <Arduino.h>
#include "SevenSegmentAsciiMap.h"

// COMPILE TIME USER CONFIG ////////////////////////////////////////////////////
#define TM1637_DEBUG                  false   // true for serial debugging
#define TM1637_BEGIN_DELAY            500     // ms
#define TM1637_PRINT_BUFFER_SIZE      128     // lower if you don't need it

// Default values //////////////////////////////////////////////////////////////
#define TM1637_DEFAULT_PRINT_DELAY    300     // 300 ms delay between characters
#define TM1637_DEFAULT_BLINK_DELAY    50      // ms
#define TM1637_DEFAULT_CLOCK_BLINK_DELAY 500  // the default delay for when using printTime
#define TM1637_DEFAULT_BLINK_REPEAT   10
#define TM1637_DEFAULT_CURSOR_POS     0       // 0-MAX-1 (e.g 3)
#define TM1637_DEFAULT_COLON_ON       false   //
#define TM1637_DEFAULT_BACKLIGHT      100     // 0..100

#define TM1637_MAX_LINES    1                 // number of display lines
#define TM1637_MAX_COLOM    4                 // number of coloms (digits)
#define TM1637_MAX_CHARS    128

// PROGRAM CONFIG (ONLY CHANGE WHEN YOU KNOW WHAT YOU RE DOING:)////////////////
#define TM1637_CLK_DELAY_US 5           // clock delay for communication
// mine works with 1us, perhaps increase if display does not function ( tested upto 1ms)


// COMMANDS ////////////////////////////////////////////////////////////////////
#define TM1637_COM_SET_DATA     B01000000 // 0x40 (1) Data set
#define TM1637_COM_SET_ADR      B11000000 // 0xC0 (2) Address command set
#define TM1637_COM_SET_DISPLAY  B10000000 // 0x80 (3) Display control command set

// Data set (1) (use logical OR to contruct complete command)
#define TM1637_SET_DATA_WRITE   B00000000 // Write data to the display register
#define TM1637_SET_DATA_READ    B00000010 // Read the key scan data
#define TM1637_SET_DATA_A_ADDR  B00000000 // Automatic address increment
#define TM1637_SET_DATA_F_ADDR  B00000100 // Fixed address
#define TM1637_SET_DATA_M_NORM  B00000000 // Normal mode
#define TM1637_SET_DATA_M_TEST  B00100000 // Test mode

// Address settings (2) (use logical OR to contruct complete command)
#define TM1637_SET_ADR_00H      B0000000  // addr 00
#define TM1637_SET_ADR_01H      B0000001  // addr 01
#define TM1637_SET_ADR_02H      B0000010  // addr 02
#define TM1637_SET_ADR_03H      B0000011  // addr 03
#define TM1637_SET_ADR_04H      B0000100  // addr 04 (only TM1637)
#define TM1637_SET_ADR_05H      B0000101  // addr 05 (only TM1637)
// The command is used to set the display register address; if the address is set to 0C4H or higher, the data is ignored, until the effective address is set; when the power is on, the default is set to 00H address.

// Display control command set (use logical OR to consruct complete command)
#define TM1637_SET_DISPLAY_1    B0000000  // Pulse width 1/16 (0.0625) (0)
#define TM1637_SET_DISPLAY_2    B0000001  // Pulse width 2/16 (0.0625) (1)
#define TM1637_SET_DISPLAY_4    B0000010  // Pulse width 4/16 (0.0625) (2)
#define TM1637_SET_DISPLAY_10   B0000011  // Pulse width 10/16 (0.0625) (3)
#define TM1637_SET_DISPLAY_11   B0000100  // Pulse width 11/16 (0.0625) (4)
#define TM1637_SET_DISPLAY_12   B0000101  // Pulse width 12/16 (0.0625) (5)
#define TM1637_SET_DISPLAY_13   B0000110  // Pulse width 13/16 (0.0625) (6)
#define TM1637_SET_DISPLAY_14   B0000111  // Pulse width 14/16 (0.0625) (7)
#define TM1637_SET_DISPLAY_OFF  B0000000  // OFF
#define TM1637_SET_DISPLAY_ON   B0001000  // ON
// there are a total of 8 brighness values, plus off

// PROTOCOL SPECIFICATION
/*
* Structure
* START COMMAND ACK STOP                  set config or display
* START ADR_CMD DATA ACK STOP             sets single digit
* START ADR_CMD DATA0 .. DATAN ACK STOP   sets multiple digits when in auto mode
*
* There are basicly three things you can do:
*   1. Set some configuration values
*     - read/write mode, auto/manual address, normal/test mode
*   2. Set a (starting) address followed by 1 or N data bytes
*   3. Set the display brightness (pwm) 0-7 and on or off
*
* From the datasheet it might seem that you always have to perform all three commands; setting configuration, setting address and data bytes and display. I'v tested this and this is not true. You can just set only one of these three. But ofcourse you have to make sure that your configuration is set properly. For example if you haven't set the configuration to automatic addresses, you can't just send out 4 data bytes, it won't work. Simlilair, if your display is off and you write some data to it, it won't display. On the other hand most default setting are what you want most of the time.

*/

class SevenSegmentTM1637 : public Print {

public:
  // LIQUID CRISTAL API ///////////////////////////////////////////////////////
  // See http://playground.arduino.cc/Code/LCDAPI for more details.

  /* Constructor
  @param [in] pinClk      clock pin (any digital pin)
  @param [in] pinDIO      digital output pin (any digital pin)
  */
  SevenSegmentTM1637(uint8_t pinClk, uint8_t pinDIO);
  /* Initializes the display
  * Initializes the display, sets some text and blinks the display

  @param [in] cols      optional: number of coloms (digits)
  @param [in] rows      optional: number of rows
  */
  void    init(uint8_t cols = TM1637_MAX_COLOM, uint8_t rows = TM1637_MAX_LINES);
  /* Implemented for compatibility, see begin() above */
  void    begin(uint8_t cols = TM1637_MAX_COLOM, uint8_t rows = TM1637_MAX_LINES);
  // Print class inheritance ///////////////////////////////////////////////////
  // See https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Print.h for more details
  /* This library inherent the Print class, this means that all regular print function can be used. For example:
  * printing a number:                    print(78)
  * printint a number in BIN, OCT, HEX..: print(78, BIN)
  * printing a float:                     print(2.85)
  * printing a PROGMEM c string:          print(F("Arduino"))
  *
  * Also the more low level write() function can be used. (Actually all print function eventually call one of these write methods, every class that wants to inherent from the Print class needs to implement these)
  */
  size_t write(uint8_t byte);
  size_t write(const char* str);
  size_t write(const uint8_t* buffer, size_t size);

  /* Clears the display
  * Writes zero to all digits and segments, display off.
  */
  void    clear(void);
  /* Sets the cursor position to zero
  */
  void    home(void);
  /* Sets the cursor position to a specfic position
  *
  @param [in] col         colomn (position)
  */
  void    setCursor(uint8_t row, uint8_t col);
  // Liquid cristal optional //////////////////////////////////////////////////
  /* Sets the display backlight
  * The display has 8 PWM modes and an off mode. The function accepts a value from 0 to 100, where 80-100 are the same; full brighness.

  @param [in] value       brightness value (0..80(100))
  */
  void    setBacklight(uint8_t value);
  /* Sets the display contrast (identical to brightness)
  * This function is mainly for compatibility with the LCD API
  */
  void    setContrast(uint8_t value);

  /* Turns the display ON
  * Identical to setting the brightness to the default value.
  */
  void    on(void);
  /* Turns the display ON
  * Identical to setting the brightness to zero and clearing the display.
  */
  void    off(void);

  // SevenSegmentTM1637 METHODS ///////////////////////////////////////////////
  /* Blink the last printed text
  *
  @param [in] blinkDelay    optional: blink delay in ms
  @param [in] repeats       optional: number of blink repeats
  */
  void    blink(uint8_t blinkDelay = TM1637_DEFAULT_BLINK_DELAY, uint8_t repeats = TM1637_DEFAULT_BLINK_REPEAT, uint8_t maxBacklight=100, uint8_t minBacklight=0);

  // getters and setters ///////////////////////////////////////////////////////
  /* Turn the colon on or off
  * When turing the colon on, the next displayed text/numbers will have a colon
  @param [in] setToOn       sets the colon to on or off
  */
  void    setColonOn(bool setToOn);
  /* Get the currrent colon setting
  */
  bool    getColonOn(void);
  /* Sets the delay for scrolling text
  * When printing more than four characters/ the display will scroll, this setting determines the scrolling speed in ms
  @param [in] printDelay    the print delay in ms
  */
  void    setPrintDelay(uint16_t printDelay);

  // helpers //////////////////////////////////////////////////////////////////
  /* Encodes a character to sevensegment binairy
  *
  @param [in] c             a character to encode
  */
  uint8_t encode(char c);
  /* Encodes a single digit to sevensegment binairy
  *
  @param [in] d             a digit to encode
  */
  uint8_t encode(int16_t d);
  /* Encodes a null terminated c string (char array) to sevensegment binairy
  *
  @param [out] buffer       holds the encodes char array
  @param [in] str           the c string to encode
  @param [in] bufferSize    the size/length of the buffer
  */
  size_t  encode(uint8_t* buffer, const char* str, size_t bufferSize);
  /* Encodes a byte array to sevensegment binairy
  *
  @param [out] buffer       holds the encodes char array
  @param [in] byteArr       the byte array to encode
  @param [in] bufferSize    the size/length of the buffer
  */
  size_t  encode(uint8_t* buffer, const uint8_t* byteArr, size_t arrSize);
  /* Shift an array one position to the left
  @param [out] buffer       the buffer to be shifted
  @param [in] length        the length to the buffer
  */
  void    shiftLeft(uint8_t* buffer, size_t length);

  // SevenSegmentTM1637 low level methods (use when you know what you're doing)
  /* Prints raw (encoded) bytes to the display
  *         A
  *       ___
  *  * F |   | B
  * X     -G-
  *  * E |   | C
  *       ___
  *        D
  * Bit:      76543210
  * Segment:  XGFEDCBA
  *
  * For example to print an H, you would set bits BCEFG, this gives B01110110 in binary or 118 in decimal or 0x76 in HEX.
  * Bit 7 (X) only applies to the second digit and sets the colon
  */
  /* Print raw (binary encodes) bytes to the display
  @param [in] rawBytes      Array of raw bytes
  @param [in] length        optional: length to print to display
  @param [in] position      optional: Start position
  */
  void    printRaw(const uint8_t* rawBytes, size_t length = 4, uint8_t position = 0);
  /* Print raw (binary encodes) bytes to the display
  @param [in] rawByte       Raw byte
  @param [in] position      optional: Start position
  */
  void    printRaw(uint8_t rawByte, uint8_t position);
  /* Write command to IC TM1637
  @param [in] cmd         command to send
  @return acknowledged?   command was (successful) acknowledged
  */
  bool    command(uint8_t cmd) const;
  bool    command(const uint8_t* command, uint8_t length) const;
  /* Read bytes from IC TM1637
  * The IC also can read the state of a keypad? TODO untested
  */
  uint8_t comReadByte(void) const;
  /* Write a single command to the display
  @param [in] cmd         command to send
  */
  void    comWriteByte(uint8_t command) const;
  /* Send start signal
  * Send the start signal for serial communication
  */
  void    comStart(void) const;
  /* Send stop signal
  * Send the stop signal for serial communication
  */
  void    comStop(void) const;
  /* Get command acknowledged
  * Get acknowledge signal (command was succesful received)
  */
  bool    comAck(void) const;

  /* Static version of low level function
  * If using more than one display, this saves some space since these methods will be shared among all instances/objects of the class
  */
  static bool    command(uint8_t pinClk, uint8_t pinDIO, uint8_t cmd);
  static bool    command(uint8_t pinClk, uint8_t pinDIO, const uint8_t* command, uint8_t length);
  static void    comStart(uint8_t pinClk, uint8_t pinDIO);
  static void    comWriteByte(uint8_t pinClk, uint8_t pinDIO, uint8_t command);
  static bool    comAck(uint8_t pinClk, uint8_t pinDIO);
  static void    comStop(uint8_t pinClk, uint8_t pinDIO);
protected:
  const uint8_t   _pinClk;            // clock pin
  const uint8_t   _pinDIO;            // digital out pin
  uint8_t         _numCols;           // number of columns
  uint8_t         _numRows;           // number of rows

  uint8_t   _backLightValue;          // brightness of the display (0..100)
  uint8_t   _cursorPos;               // current cursor position
  uint16_t  _printDelay;              // print delay in ms (multiple chars)
  uint8_t   _colonOn;                 // colon bit if set
  uint8_t   _rawBuffer[TM1637_MAX_COLOM];// hold the last chars printed to display
};


#define TM1637_COLON_BIT        B10000000

// debug macros for debugging
#if TM1637_DEBUG
    #define TM1637_DEBUG_BEGIN(x)      Serial.begin(x)
    #define TM1637_DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
    #define TM1637_DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)
    #define TM1637_DEBUG_WRITE(x)      Serial.write(x)
    #define TM1637_DEBUG_MESSAGE(...)    \
      Serial.print(millis());   \
      Serial.print(F("\t"));    \
      Serial.print(__VA_ARGS__);
    #define TM1637_DEBUG_MESSAGELN(...)  \
      TM1637_DEBUG_MESSAGE(__VA_ARGS__)  \
      Serial.println();
#else
    #define TM1637_DEBUG_BEGIN(x)
    #define TM1637_DEBUG_PRINT(...)
    #define TM1637_DEBUG_PRINTLN(...)
    #define TM1637_DEBUG_WRITE(x)
    #define TM1637_DEBUG_MESSAGE(x)
    #define TM1637_DEBUG_MESSAGELN(x)
#endif

// arduino:standard variant direct port access macros for more speed ( communication is ~us)
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
  #define portOfPin(P) \
    ( ((P) >= 0 && (P) < 8)? &PORTD:( ((P) > 7 && (P) < 14) ? &PORTB: &PORTC ) )
  #define ddrOfPin(P) \
    ( ((P) >= 0 && (P) < 8)? &DDRD: ( ((P) > 7 && (P) < 14) ? &DDRB: &DDRC ) )
  #define pinOfPin(P) \
    ( ((P) >= 0 && (P) < 8)? &PIND: ( ((P) > 7 && (P) < 14) ? &PINB: &PINC ) )

  #define pinIndex(P)         ( (uint8_t)( P > 13 ? P-14: P&7 ) )
  #define pinMask(P)          ( (uint8_t)( 1 << pinIndex(P) ) )

  #define pinAsInput(P)       *(ddrOfPin(P) )     &= ~pinMask(P)
  #define pinAsInputPullUp(P) *(ddrOfPin(P) )     &= ~pinMask(P);digitalHigh(P)
  #define pinAsOutput(P)      *(ddrOfPin(P) )     |= pinMask(P)
  #define digitalLow(P)       *(portOfPin(P))     &= ~pinMask(P)
  #define digitalHigh(P)      *(portOfPin(P))     |= pinMask(P)
  #define isHigh(P)           ( ( *( pinOfPin(P) )  & pinMask(P) ) >  0 )
  #define isLow(P)            ( ( *( pinOfPin(P) )  & pinMask(P) ) == 0 )
  #define digitalState(P)     ((uint8_t)isHigh(P))
#else
  #define pinAsOutput(P)      pinMode(P, OUTPUT)
  #define pinAsInput(P)       pinMode(P, INPUT)
  #define pinAsInputPullUp(P) pinMode(P, INPUT_PULLUP)
  #define digitalLow(P)       digitalWrite(P, LOW)
  #define digitalHigh(P)      digitalWrite(P, HIGH)
  #define isHigh(P)           (digitalRead(P) == 1)
  #define isLow(P)            (digitalRead(P) == 0)
  #define digitalState(P)     digitalRead(P)
#endif

#endif
