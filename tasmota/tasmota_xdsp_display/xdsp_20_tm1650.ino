/*
  xdsp_20_tm1650.ino - TM1650 four-digit seven-segment LED display controller support for Tasmota

  Copyright (C) 2021  Stefan Oskamp, Theo Arends, Anatoli Arkhipenko

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
  The TM1650 chip is a controller for a four digit seven-segment (plus dot) LED display.
  It uses eight I2C addresses; four to control the digits and four to display content.
  The addresses are not configurable: 0x24...0x27, 0x34...0x37
  There is a Chinese datasheet on the Internet and a machine translation to English.
  The Controller also supports reading a keyboard matrix of 4 x 8 keys. This is not 
  supported by this driver.
  The TM1650 is found in clocks with seven-segment LED displays, e.g., the "XY-Clock".
  In this clock, the dots of the display are connected to the controller in an unintuitive
  way. While digits are counted from left to right, the associated dots on the display
  are: 0 = rightmost dot, 1 = dot after third digit, 2 = upper half of the colon
  3 = lower half of the column. The dots after the first and second digits are not 
  connected. This wiring of the XY-Clock has been reflected in the time format. Other
  clocks using a TM1650 might be wired differently.
*/
#ifdef USE_I2C
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_TM1650

#include <Wire.h>

#define XDSP_20                   20
#define XI2C_74                   74   // See I2CDEVICES.md

#define TM1650_CONTROL_BASE     0x24   // I2C address to control left-most digit.
#define TM1650_DISPLAY_BASE     0x34   // I2C address to display something in the left-most digit.
#define TM1650_DIGITS              4   // One TM1650 can only control modules with up to four digits.

// TM1650 Display bits:
#define TM1650_DISPLAY_DOT         7

// TM1650 Control bits:
#define TM1650_CONTROL_ON          0
#define TM1650_CONTROL_RESERVED1   1
#define TM1650_CONTROL_RESERVED2   2
#define TM1650_CONTROL_DOT         3   // Dots can be switched on/off with both control and display.
#define TM1650_CONTROL_BRIGHTNESS  4   // Bits 4...6
#define TM1650_CONTROL_RESERVED3   7


static unsigned char TM1650Control[TM1650_DIGITS] = {0, 0, 0, 0};
static unsigned char TM1650Display[TM1650_DIGITS] = {0, 0, 0, 0};

static const byte TM1650Font[128] {
//0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10
  0x00, 0x82, 0x21, 0x00, 0x00, 0x00, 0x00, 0x02, 0x39, 0x0F, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, // 0x20
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x6f, 0x00, 0x00, 0x00, 0x48, 0x00, 0x53, // 0x30
  0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x06, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x3F, // 0x40
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x00, 0x0F, 0x00, 0x08, // 0x50 
  0x63, 0x5F, 0x7C, 0x58, 0x5E, 0x7B, 0x71, 0x6F, 0x74, 0x02, 0x1E, 0x00, 0x06, 0x00, 0x54, 0x5C, // 0x60
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x30, 0x0F, 0x00, 0x00  // 0x70
};


/*********************************************************************************************/

void TM1650InitMode(void)
{
  TM1650Dim();
  TM1650Clear();
} // void TM1650InitMode(void)

void TM1650Init(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      TM1650InitMode();
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      TM1650InitMode();
      break;
  }
} // void TM1650Init(uint8_t mode)

void TM1650InitDriver(void)
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("M1650InitDriver()"));
  if (!TasmotaGlobal.i2c_enabled) {
    return;
  }

  if (!Settings->display_model) {
    if (I2cSetDevice(TM1650_CONTROL_BASE)) {
      Settings->display_address[0] = TM1650_CONTROL_BASE;
      Settings->display_model = XDSP_20;
    }
  }

  if (XDSP_20 == Settings->display_model) {
    I2cSetActiveFound(Settings->display_address[0], "TM1650");

    Settings->display_cols[0] = 4;
    Settings->display_cols[0] = 1;
    Settings->display_width = Settings->display_cols[0];
    Settings->display_height = Settings->display_rows;

    TM1650InitMode();

    AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1650"));
  }
} // void TM1650InitDriver(void)

void TM1650DisplayOn ()
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650DisplayOn()"));
  for (int i = 0; i < TM1650_DIGITS; i++) {
  	TM1650Control[i] |= _BV(TM1650_CONTROL_ON);
    Wire.beginTransmission(TM1650_CONTROL_BASE + i);
    Wire.write(TM1650Control[i]);
    Wire.endTransmission();
  }
} // void TM1650DisplayOn ()

void TM1650DisplayOff ()
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650DisplayOff()"));
  for (int i = 0; i < TM1650_DIGITS; i++) {
	  TM1650Control[i] &= ~_BV(TM1650_CONTROL_ON);
    Wire.beginTransmission(TM1650_CONTROL_BASE + i);
    Wire.write(TM1650Control[i]);
    Wire.endTransmission();
  }
} // void TM1650DisplayOff ()

void TM1650DisplayOnOff()
{
  if (disp_power) {
    TM1650DisplayOn();
  }
  else {
    TM1650DisplayOff();
  }
} // void TM1650DisplayOnOff()

void TM1650SetBrightness (unsigned int level)
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650SetBrightness()"));
  if (level > 0b111) level = 0b111;

  for (int i = 0; i < TM1650_DIGITS; i++) {
	  TM1650Control[i] = TM1650Control[i] & ~(0b111 << TM1650_CONTROL_BRIGHTNESS) | level << TM1650_CONTROL_BRIGHTNESS;
    Wire.beginTransmission(TM1650_CONTROL_BASE + i);
    Wire.write(TM1650Control[i]);
    Wire.endTransmission();
  }
} // void TM1650SetBrightness (unsigned int level)

void TM1650Dim(void)
{
  int b = GetDisplayDimmer16();
  if (b < 2) {
    TM1650DisplayOff();
  }
  else if (b > 14) {
    TM1650DisplayOn();
    TM1650SetBrightness(0); // In TM1650, brightness 0 means max brightness (level 8).
    TM1650DisplayOn();
  }
  else {
    // Map 2...14 to 1...7:
    TM1650SetBrightness(b >> 1);
    TM1650DisplayOn();
  }
} // void TM1650Dim(void)

void TM1650Clear (void)
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650Clear()"));
  for (int i = 0; i < TM1650_DIGITS; i++) {
	  TM1650Display[i] = 0;
    Wire.beginTransmission(TM1650_DISPLAY_BASE + i);
    Wire.write(TM1650Display[i]);
    Wire.endTransmission();
  }
} // void TM1650Clear (void)

void TM1650DisplayText (char *text)  // Text shall match regex (([^.]?\.?){0,4}\0), e.g., 123.4
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650DisplayText(\"%s\")"), text);
  for (int i = 0; i < TM1650_DIGITS; i++) {
    if (*text != 0) {
      if (*text == '.') { 
        TM1650Display[i] = 0; // Blank this digit, set the dot below.
      }
      else {  // Something to display.
        char c = *text++;
        TM1650Display[i] = TM1650Font[c & ~_BV(TM1650_DISPLAY_DOT)];
      }

      if (*text == '.') {
        char c = *text++;
        TM1650Display[i] |= _BV(TM1650_DISPLAY_DOT);
      }
    } // if (not at end of text)
    else { // No more text.
      TM1650Display[i] = 0;  // Clear digits after the text.
    }
    Wire.beginTransmission(TM1650_DISPLAY_BASE + i);
    Wire.write(TM1650Display[i]);
    Wire.endTransmission();
  } // for (all digits)
} // void TM1650DisplayText (char *text) 

void TM1650Time(void)
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650Time()"));
  char text[TM1650_DIGITS + 2 + 1];
  int i = 0;

  text[i++] = '0' + RtcTime.hour / 10;
  text[i++] = '0' + RtcTime.hour % 10;
  text[i++] = '0' + RtcTime.minute / 10;
  text[i++] = '.';  // Lower half of the colon, depending on how the LEDs are connected to the TM1650 in the XY-Clock.                    
  text[i++] = '0' + RtcTime.minute % 10;
  text[i++] = '.';  // Upper half of the colon.
  text[i++] = 0;

  TM1650DisplayText(text);
} // void TM1650Time(void)

void TM1650Refresh(void)  // Every second
{
  if (Settings->display_mode) {  // Mode 0 is User text
    switch (Settings->display_mode) {
      case 1:  // Time
        TM1650Time();
        break;
      case 2:  // Local
      case 4:  // Mqtt
        // TM1650PrintLog();
        break;
      case 3:  // Local
      case 5:
//        // Mqtt
//        if (!TM1650PrintLog()) {
//          TM1650Time();
//        }
        break;
    }
  }
} // void TM1650Refresh(void) 


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp20(uint32_t function)
{
  if (!I2cEnabled(XI2C_74)) { return false; }

  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    TM1650InitDriver();
  }
  else if (XDSP_20 == Settings->display_model) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_INIT:
        TM1650Init(dsp_init);
        break;
      case FUNC_DISPLAY_POWER:
        TM1650DisplayOnOff();
        break;
      case FUNC_DISPLAY_DIM:
        TM1650Dim();
	      break;    
      case FUNC_DISPLAY_CLEAR:
        TM1650Clear();
        break;
      case FUNC_DISPLAY_DRAW_STRING:
        TM1650DisplayText(dsp_str);
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        TM1650Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
//        case FUNC_DISPLAY_DRAW_HLINE:
//          break;
//        case FUNC_DISPLAY_DRAW_VLINE:
//          break;
//        case FUNC_DISPLAY_DRAW_CIRCLE:
//          break;
//        case FUNC_DISPLAY_FILL_CIRCLE:
//          break;
//        case FUNC_DISPLAY_DRAW_RECTANGLE:
//          break;
//        case FUNC_DISPLAY_FILL_RECTANGLE:
//          break;
//        case FUNC_DISPLAY_DRAW_FRAME:
//          break;
//        case FUNC_DISPLAY_TEXT_SIZE:
//          break;
//        case FUNC_DISPLAY_FONT_SIZE:
//          break;
//        case FUNC_DISPLAY_ROTATION:
//          break;
    } // switch (function)
  } // else if (display model matches)
  return result;
} // bool Xdsp20(uint32_t function)

#endif  // USE_DISPLAY_TM1650
#endif  // USE_DISPLAY
#endif  // USE_I2C
