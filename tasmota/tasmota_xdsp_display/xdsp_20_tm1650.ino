/*
  xdsp_20_tm1650.ino - TM1650 four-digit seven-segment LED display controller support for Tasmota

  Copyright (C) 2021  Stefan Oskamp, Theo Arends, Anatoli Arkhipenko
  Copyright (C) 2023  Gabriele Lauricella

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


/*********************************************************************************************\
  This driver enables the display of numbers (both integers and floats) and basic text
  on the inexpensive TM1650 seven-segment modules.

  Raw segments can also be displayed.

  In addition, it is also possible to set brightness (8 levels), clear the display, scroll text,
  display a rudimentary bar graph, and a Clock (12 hr and 24 hr).

  To use, compile Tasmota with USE_I2C, USE_DISPLAY and USE_DISPLAY_TM1650, or build the tasmota-display env.

  For TM1650:
  Connect the TM1650 display module's pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "I2C SCA"
  CLK hardware pin --> "I2C SCL"

  Once the GPIO configuration is saved and the ESP8266/ESP32 module restarts,
  set the Display Model to 20 and Display Mode to 0 
  using the command "Backlog DisplayModel 20 ; DisplayMode 0;"
  Before using it set the Display Type, if you have an XY-Clock set it to 0 otherwise if you 
  have a 303WifiL01 set it to 2 using the "DisplayType 0" command.


  After the ESP8266/ESP32 module restarts again, turn ON the display with the command "Power 1"

  Now, the following "Display" commands can be used:


  DisplayClear

                               Clears the display, command: "DisplayClear"


  DisplayNumber         num [,position {0-(Settings->display_width-1))} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]

                               Clears and then displays number without decimal. command e.g., "DisplayNumber 1234"
                               Control 'leading zeros', 'length' and 'position' with  "DisplayNumber 1234, <position>, <leadingZeros>, <length>"
                               'leading zeros' can be 1 or 0 (default), 'length' can be 1 to Settings->display_width, 'position' can be 0 (left-most) to Settings->display_width (right-most).
                               See function description below for more details.

  DisplayNumberNC       num [,position {0-(Settings->display_width-1))} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]

                               Display integer number as above, but without clearing first. e.g., "DisplayNumberNC 1234". Usage is same as above.



  DisplayFloat          num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]

                               Clears and then displays float (with decimal point)  command e.g., "DisplayFloat 12.34"
                               See function description below for more details.



  DisplayFloatNC        num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]

                               Displays float (with decimal point) as above, but without clearing first. command e.g., "DisplayFloatNC 12.34"
                               See function description below for more details.



  DisplayRaw            position {0-(Settings->display_width-1)},length {1 to Settings->display_width}, num1 [, num2[, num3[, num4[, ...upto Settings->display_width numbers]]]]]

                               Takes upto Settings->display_width comma-separated integers (0-255) and displays raw segments. Each number represents a
                               7-segment digit. Each 8-bit number represents individual segments of a digit.
                               For example, the command "DisplayRaw 0, 4, 255, 255, 255, 255" would display "[8.8.8.8.]"



  DisplayText           text [, position {0-(Settings->display_width-1)} [,length {1 to Settings->display_width}]]

                               Clears and then displays basic text.  command e.g., "DisplayText ajith vasudevan"
                               Control 'length' and 'position' with  "DisplayText <text>, <position>, <length>"
                               'length' can be 1 to Settings->display_width, 'position' can be 0 (left-most) to Settings->display_width-1 (right-most)
                               A caret(^) or backtick(`) symbol in the text input is dispayed as the degrees(°) symbol. This is useful for 
			       displaying Temperature! For example, the command "DisplayText 22.5^" will display "22.5°".


  DisplayTextNC         text [, position {0-Settings->display_width-1} [,length {1 to Settings->display_width}]]

                               Clears first, then displays text. Usage is same as above.



  DisplayScrollText     text [, num_loops]

                              Displays scrolling text indefinitely, until another Display- command (other than DisplayScrollText 
                              or DisplayScrollDelay is issued). Optionally, stop scrolling after num_loops iterations.



  DisplayScrollDelay delay {0-15}   // default = 4

                               Sets the speed of text scroll. Smaller delay = faster scrolling.



  DisplayLevel          num {0-100}

                               Display a horizontal bar graph (0-100) command e.g., "DisplayLevel 50" will display [||||    ]



  DisplayClock  1|2|0

                               Displays a clock.
                               Commands "DisplayClock 1"     // 12 hr format
                                        "DisplayClock 2"     // 24 hr format
                                        "DisplayClock 0"     // turn off clock


In addition, if you compile using USE_DISPLAY_MODES1TO5, setting DisplayMode to 1 shows the time,
setting it to 2 shows the date and setting it to 3 alternates between time and date (using "DisplayRefresh [1..7]" 
for the time and seconds you want to show the time before displaying the date)

\*********************************************************************************************/


#define XDSP_20                   20
#define XI2C_74                   74   // See I2CDEVICES.md

#define CMD_MAX_LEN               55
#define LEVEL_MIN                 0
#define LEVEL_MAX                 100
#define SCROLL_MAX_LEN            50

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

#include <Wire.h>

static unsigned char TM1650Control[TM1650_DIGITS] = {0, 0, 0, 0};
static unsigned char TM1650Display[TM1650_DIGITS] = {0, 0, 0, 0};

static const byte TM1650Font[128] {
//0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10
//      !     "                             '     (     )                       -
  0x00, 0x82, 0x21, 0x00, 0x00, 0x00, 0x00, 0x02, 0x39, 0x0F, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, // 0x20
//0     1     2     3     4     5     6     7     8     9                       =           ?
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x6f, 0x00, 0x00, 0x00, 0x48, 0x00, 0x53, // 0x30
//      A     B     C     D     E     F     G     H     I     J           L           N     O
  0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x06, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x3F, // 0x40
//P     Q     R     S     T     U                       Y           [           ]     ^=°   _
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x00, 0x0F, 0x63, 0x08, // 0x50 
//`=°   a     b     c     d     e     f     g     h     i     j           l           n     o
  0x63, 0x5F, 0x7C, 0x58, 0x5E, 0x7B, 0x71, 0x6F, 0x74, 0x02, 0x1E, 0x00, 0x06, 0x00, 0x54, 0x5C, // 0x60
//p     q     r     s     t     u                       y           {     |     }
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x30, 0x0F, 0x00, 0x00  // 0x70
};

enum display_options_types
{
  T_XY_CLOCK, // XY-Clock - white PCB with a perpendicular small daughter board
  NOTUSED,
  T_303WIFILC01 // 303WifiLC01 - green clock
};

struct
{
  char scroll_text[CMD_MAX_LEN];
  char msg[60];
  char model_name[8];
  uint8_t scroll_delay = 4;
  uint8_t scroll_index = 0;
  uint8_t iteration = 0;
  uint8_t scroll_counter = 0;
  uint8_t scroll_counter_max = 3;
  uint8_t display_type = XDSP_20; 

  bool init_driver_done = false;
  bool scroll = false;
  bool show_clock = false;
  bool clock_24 = false;
  bool clock_blynk_dots = true;
} TM1650Data;

/*********************************************************************************************\
* Init function
\*********************************************************************************************/
void TM1650Init(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      TM1650Dim();
      TM1650ClearDisplay();
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      TM1650Dim();
      TM1650ClearDisplay();
      break;
  }
}

/*********************************************************************************************\
* Init Drive function
\*********************************************************************************************/
void TM1650InitDriver(void)
{
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
    Settings->display_rows = 1;
    Settings->display_width = Settings->display_cols[0];
    Settings->display_height = Settings->display_rows;

    if (T_XY_CLOCK == Settings->display_options.type)
    {
      strcpy_P(TM1650Data.model_name, PSTR("XY-Clock"));
    }
    else if (NOTUSED == Settings->display_options.type)
    {
      strcpy_P(TM1650Data.model_name, PSTR("NOTUSED"));
    }
    else if (T_303WIFILC01 == Settings->display_options.type)
    {
      strcpy_P(TM1650Data.model_name, PSTR("303WiFiLC01"));
    }

    TM1650Dim();
    TM1650ClearDisplay();

    AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1650 \"%s\" with %d digits (type %d)"), TM1650Data.model_name, Settings->display_width, Settings->display_options.type);
    TM1650Data.init_driver_done = true;
  }
}

void TM1650DisplayOn ()
{
  for (int i = 0; i < TM1650_DIGITS; i++) {
  	TM1650Control[i] |= _BV(TM1650_CONTROL_ON);
    Wire.beginTransmission(TM1650_CONTROL_BASE + i);
    Wire.write(TM1650Control[i]);
    Wire.endTransmission();
  }
}

void TM1650DisplayOff ()
{
  for (int i = 0; i < TM1650_DIGITS; i++) {
	  TM1650Control[i] &= ~_BV(TM1650_CONTROL_ON);
    Wire.beginTransmission(TM1650_CONTROL_BASE + i);
    Wire.write(TM1650Control[i]);
    Wire.endTransmission();
  }
}

void TM1650DisplayOnOff()
{
  if (disp_power) {
    TM1650DisplayOn();
  }
  else {
    TM1650DisplayOff();
  }
}

void TM1650SetBrightness (unsigned int level)
{
  if (level > 0b111) level = 0b111;

  for (int i = 0; i < TM1650_DIGITS; i++) {
	  TM1650Control[i] = TM1650Control[i] & ~(0b111 << TM1650_CONTROL_BRIGHTNESS) | level << TM1650_CONTROL_BRIGHTNESS;
    Wire.beginTransmission(TM1650_CONTROL_BASE + i);
    Wire.write(TM1650Control[i]);
    Wire.endTransmission();
  }
}


void TM1650DisplayText (char *text)  // Text shall match regex (([^.]?\.?){0,4}\0), e.g., 123.4 or 8.8.8.8 for full digit
{
  //AddLog(LOG_LEVEL_DEBUG, PSTR("TM1650DisplayText(\"%s\")"), text);
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

      if (T_303WIFILC01 == Settings->display_options.type) {
        TM1650Display[i] = swapbits(TM1650Display[i]); // 303WIFILC01 board has special wiring
      }
    }
    else {
      TM1650Display[i] = 0;  // Clear digits after the text.
    }
    Wire.beginTransmission(TM1650_DISPLAY_BASE + i);
    Wire.write(TM1650Display[i]);
    Wire.endTransmission();
  }
}

uint8_t swapbits(uint8_t n)
{
    // For model 303WiFiLC01 board has different segments wiring  
    // PGFEDCBA -> BFAEDCGP
 
            //seg EDC    //seg A             //seg B
     return (n & 0x1C) | ((n & 0x01) << 5) | ((n & 0x02) << 6)
     | ((n & 0x20) << 1) | ((n & 0x40) >> 5) | ((n & 0x80) >> 7); 
       //seg F             //seg G             //seg P
}  

void TM1650DisplayRaw (char *text)  // Text shall match regex (([^.]?\.?){0,4}\0), e.g., 123.4
{
  for (int i = 0; i < TM1650_DIGITS; i++) {
    char c = *text++;
    TM1650Display[i] = c;
    //AddLog(LOG_LEVEL_DEBUG, PSTR("Raw Digit(%d - %d - %c)"), i, TM1650Display[i], TM1650Display[i]);
    Wire.beginTransmission(TM1650_DISPLAY_BASE + i);
    Wire.write(TM1650Display[i]);
    Wire.endTransmission();
  }
}


/*********************************************************************************************\
* Displays number without decimal, with/without leading zeros, specifying start-position
* and length, optionally skipping clearing display before displaying the number.
* commands: DisplayNumber   num [,position {0-(Settings->display_width-1)} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]
*           DisplayNumberNC num [,position {0-(Settings->display_width-1)} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/

bool CmndTM1650Number(bool clear)
{
  char sNum[CMD_MAX_LEN];
  char sLeadingzeros[CMD_MAX_LEN];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  uint8_t length = 0;
  bool leadingzeros = false;
  uint8_t position = 0;

  uint32_t num = 0;

  switch (ArgC())
  {
  case 4:
    subStr(sLength, XdrvMailbox.data, ",", 4);
    length = atoi(sLength);
  case 3:
    subStr(sLeadingzeros, XdrvMailbox.data, ",", 3);
    leadingzeros = atoi(sLeadingzeros);
  case 2:
    subStr(sPosition, XdrvMailbox.data, ",", 2);
    position = atoi(sPosition);
  case 1:
    subStr(sNum, XdrvMailbox.data, ",", 1);
    num = TextToInt(sNum);
  }

  if ((position < 0) || (position > (Settings->display_width - 1)))
    position = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: num %d, pos %d, lead %d, len %d"), num, position, leadingzeros, length);

  if (clear)
    TM1650ClearDisplay();

  char txt[30];
  snprintf_P(txt, sizeof(txt), PSTR("%d"), num);
  if (!length)
    length = strlen(txt);
  if ((length < 0) || (length > Settings->display_width))
    length = Settings->display_width;

  char pad = (leadingzeros ? '0' : ' ');
  uint32_t i = position;

  char text[TM1650_DIGITS + 1];

  //write empty pos
  for (uint32_t j=0; j < position; j++)
  {
    if (j >= Settings->display_width)
      break;
    text[j] = ' ';
  }

  //write pads
  for (; i < position + (length - strlen(txt)); i++)
  {
    if (i >= Settings->display_width)
      break;
    text[i] = pad;
  }

  //write digits
  for (uint32_t j = 0; i < position + length; i++, j++)
  {
    if (i >= Settings->display_width)
      break;
    if (txt[j] == 0)
      break;
    if (txt[j] == '.') {
      i--;
      continue;
    }
    text[i] = txt[j];
  }

  text[i] = 0; //string termination

  TM1650DisplayText(text);
  return true;
}


/*********************************************************************************************\
* Displays number with decimal, specifying position, precision and length,
* optionally skipping clearing display before displaying the number.
* commands: DisplayFloat   num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]
*           DisplayFloatNC num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/

bool CmndTM1650Float(bool clear)
{

  char sNum[CMD_MAX_LEN];
  char sPrecision[CMD_MAX_LEN];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  uint8_t length = 0;
  uint8_t precision = Settings->display_width-1;
  uint8_t position = 0;

  float fnum = 0.0f;

  switch (ArgC())
  {
  case 4:
    subStr(sLength, XdrvMailbox.data, ",", 4);
    length = atoi(sLength);
  case 3:
    subStr(sPrecision, XdrvMailbox.data, ",", 3);
    precision = atoi(sPrecision);
  case 2:
    subStr(sPosition, XdrvMailbox.data, ",", 2);
    position = atoi(sPosition);
  case 1:
    subStr(sNum, XdrvMailbox.data, ",", 1);
    fnum = CharToFloat(sNum);
  }

  if ((position < 0) || (position > (Settings->display_width)))
    position = 0;
  if ((precision < 0) || (precision >= Settings->display_width))
    precision = Settings->display_width-1;

  if (clear)
    TM1650ClearDisplay();

  char txt[30];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), precision, &fnum);

  if (!length)
    length = strlen(txt);
  if ((length <= 0) || (length > Settings->display_width))
    length = Settings->display_width;

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: num %4_f, pos %d, prec %d, len %d, txt %s"), &fnum, position, precision, length, txt);

  uint32_t i = position;
  uint32_t dots = 0;
  char text[TM1650_DIGITS + 1 + 1];

  //write empty pos
  for (uint32_t j=0; j < position; j++)
  {
    if (j >= Settings->display_width)
      break;
    text[j] = ' ';
  }

  if (T_XY_CLOCK == Settings->display_options.type) {

    for (uint32_t j = 0; i < position + length + dots; i++, j++)
    {
      if (txt[j] == '.') dots++;
      if (i >= Settings->display_width + dots)
        break;
      if (txt[j] == 0)
        break;
      if (txt[j] == '.') {
        if(i==2) {
          //2nd dot ok
        }
        else if(i==3){
          //3rd dot but move do 1st 
          text[i] = text[i-1];
          text[i-1] = text[i-2];
          text[i-2] = txt[j];
          continue;
        }
        else {
          //dot on 1st or 4th
          AddLog(LOG_LEVEL_INFO, PSTR("TM5: Can't display this float"));
          return false;
        }
      }
      text[i] = txt[j];
    }

  }
  else if (T_303WIFILC01 == Settings->display_options.type) {
     //todo
     AddLog(LOG_LEVEL_INFO, PSTR("TM5: DisplayFloat not implemented yet for display type: 303WifiClock"));
     return false;
  }

  text[i] = 0; //string termination

  TM1650DisplayText(text);
  return true;
}

// /*********************************************************************************************\
// * Clears the display
// * Command:  DisplayClear
// \*********************************************************************************************/
bool CmndTM1650Clear(void)
{
  TM1650ClearDisplay();
  sprintf(TM1650Data.msg, PSTR("Cleared"));
  XdrvMailbox.data = TM1650Data.msg;
  return true;
}

// /*********************************************************************************************\
// * Clears the display
// \*********************************************************************************************/
void TM1650ClearDisplay (void)
{
  for (int i = 0; i < TM1650_DIGITS; i++) {
	  TM1650Display[i] = 0;
    Wire.beginTransmission(TM1650_DISPLAY_BASE + i);
    Wire.write(TM1650Display[i]);
    Wire.endTransmission();
  }
}

/*********************************************************************************************\
* Display scrolling text
* Command:   DisplayScrollText text
\*********************************************************************************************/
bool CmndTM1650ScrollText(void)
{

  char sString[SCROLL_MAX_LEN + 1];
  char sMaxLoopCount[CMD_MAX_LEN];
  uint8_t maxLoopCount = 0;

  switch (ArgC())
  {
  case 2:
    subStr(sMaxLoopCount, XdrvMailbox.data, ",", 2);
    maxLoopCount = atoi(sMaxLoopCount);
  case 1:
    subStr(sString, XdrvMailbox.data, ",", 1);
  }

  if (maxLoopCount < 0)
    maxLoopCount = 0;

  //AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: sString %s, maxLoopCount %d"), sString, maxLoopCount);

  TM1650Data.scroll_counter_max = maxLoopCount;

  if (strlen(sString) > SCROLL_MAX_LEN)
  {
    snprintf(TM1650Data.msg, sizeof(TM1650Data.msg), PSTR("Text too long. Length should be less than %d"), SCROLL_MAX_LEN);
    XdrvMailbox.data = TM1650Data.msg;
    return false;
  }
  else
  {
    snprintf(TM1650Data.scroll_text, sizeof(TM1650Data.scroll_text), PSTR("                                                               "));
    snprintf(TM1650Data.scroll_text, Settings->display_width + sizeof(TM1650Data.scroll_text), PSTR("    %s"), &sString);
    TM1650Data.scroll_text[strlen(sString) + Settings->display_width] = 0;
    TM1650Data.scroll_index = 0;
    TM1650Data.scroll = true;
    TM1650Data.scroll_counter = 0;
    return true;
  }
}

/*********************************************************************************************\
* Sets the scroll delay for scrolling text.
* Command:  DisplayScrollDelay delay {0-15}    // default = 4
\*********************************************************************************************/
bool CmndTM1650ScrollDelay(void)
{
  if (ArgC() == 0)
  {
    XdrvMailbox.payload = TM1650Data.scroll_delay;
    return true;
  }
  if (TM1650Data.scroll_delay < 0)
    TM1650Data.scroll_delay = 0;
  TM1650Data.scroll_delay = XdrvMailbox.payload;
  return true;
}

/*********************************************************************************************\
* Scrolls a given string. Called every 50ms
\*********************************************************************************************/
void TM1650ScrollText(void)
{
  if(!TM1650Data.scroll) return;
  TM1650Data.iteration++;
  if (TM1650Data.scroll_delay)
    TM1650Data.iteration = TM1650Data.iteration % TM1650Data.scroll_delay;
  else
    TM1650Data.iteration = 0;
  if (TM1650Data.iteration)
    return;

  if (TM1650Data.scroll_index > strlen(TM1650Data.scroll_text))
  {
    TM1650Data.scroll_index = 0;
    TM1650Data.scroll_counter++;
    if(TM1650Data.scroll_counter_max != 0 && (TM1650Data.scroll_counter >= TM1650Data.scroll_counter_max)) {
      TM1650Data.scroll = false;
      return;
    }    
  }
 
  char text[CMD_MAX_LEN + 2 + 1];
  uint32_t i;
  uint32_t j;
  for (i = 0, j = TM1650Data.scroll_index; i < 1 + strlen(TM1650Data.scroll_text); i++, j++)
  {
    if (i > (Settings->display_width - 1))
    {
      break;
    }
     text[i] = TM1650Data.scroll_text[j];
  }

  text[i] = 0; //string termination

  TM1650DisplayText(text);
  TM1650Data.scroll_index++;
}

/*********************************************************************************************\
* Displays a horizontal bar graph. Takes a percentage number (0-100) as input
* Command:   DisplayLevel level {0-100}
\*********************************************************************************************/
bool CmndTM1650Level(void)
{
  uint16_t val = XdrvMailbox.payload;
  if ((val < LEVEL_MIN) || (val > LEVEL_MAX))
  {
    Response_P(PSTR("{\"Error\":\"Level should be a number in the range [%d, %d]\"}"), LEVEL_MIN, LEVEL_MAX);
    return false;
  }

  char text[TM1650_DIGITS + 1];
  for (uint32_t i = 0; i < TM1650_DIGITS; i++)
  {
     text[i]=0;
  }

  uint8_t totalBars = 2 * Settings->display_width;
  AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: CmndTM1650Level totalBars=%d"), totalBars);
  float barsToDisplay = totalBars * val / 100.0f;
  char txt[5];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), 1, &barsToDisplay);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: CmndTM1650Level barsToDisplay=%s"), txt);
  char s[4];
  ext_snprintf_P(s, sizeof(s), PSTR("%0_f"), &barsToDisplay);
  uint8_t numBars = atoi(s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: CmndTM1650Level numBars %d"), numBars);

  TM1650ClearDisplay();

  uint8_t digit = numBars / 2;
  uint8_t remainder = numBars % 2;

  for (uint32_t i = 0; i < TM1650_DIGITS; i++)
  {
    if (i >= Settings->display_width)
      break;
    if(i<digit) {
      text[i] = 0x36;
    }
    else if(i == digit && remainder == 1) {
       text[i] = 0x30;
    }
    else {
      text[i] = 0;
    }
  }

  TM1650DisplayRaw(text);

  return true;
}

/*********************************************************************************************\
* Display arbitrary data on the display module
* Command:   DisplayRaw position {0-(Settings->display_width-1)},length {1 to Settings->display_width}, a [, b[, c[, d[...upto Settings->display_width]]]]
* where a,b,c,d... are upto Settings->display_width numbers in the range 0-255, each number (byte)
* corresponding to a single 7-segment digit. Within each byte, bit 0 is segment A,
* bit 1 is segment B etc. The function may either set the entire display
* or any desired part using the length and position parameters.
\*********************************************************************************************/
bool CmndTM1650Raw(void)
{
  char text[TM1650_DIGITS + 1];
  for (uint32_t i = 0; i < TM1650_DIGITS; i++)
  {
     text[i]=0;
  }

  uint8_t DATA[4] = {0, 0, 0, 0};

  char as[CMD_MAX_LEN];
  char bs[CMD_MAX_LEN];
  char cs[CMD_MAX_LEN];
  char ds[CMD_MAX_LEN];


  char sLength[CMD_MAX_LEN];
  char sPos[CMD_MAX_LEN];

  uint32_t position = 0;
  uint32_t length = 0;

  switch (ArgC())
  {
  case 6:
    subStr(ds, XdrvMailbox.data, ",", 6);
    DATA[3] = atoi(ds);
  case 5:
    subStr(cs, XdrvMailbox.data, ",", 5);
    DATA[2] = atoi(cs);
  case 4:
    subStr(bs, XdrvMailbox.data, ",", 4);
    DATA[1] = atoi(bs);
  case 3:
    subStr(as, XdrvMailbox.data, ",", 3);
    DATA[0] = atoi(as);
  case 2:
    subStr(sLength, XdrvMailbox.data, ",", 2);
    length = atoi(sLength);
  case 1:
    subStr(sPos, XdrvMailbox.data, ",", 1);
    position = atoi(sPos);
  }

  if (!length)
    length = ArgC() - 2;
  if (length < 0 || length > Settings->display_width)
    length = Settings->display_width;
  if (position < 0 || position > (Settings->display_width - 1))
    position = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM5: a %d, b %d, c %d, d %d, len %d, pos %d"),
         DATA[0], DATA[1], DATA[2], DATA[3], length, position);

  for (uint32_t i = position; i < position + length; i++)
  {
    if (i >= Settings->display_width)
      break;
    text[i] = DATA[i - position];
  }

  TM1650DisplayRaw(text);

  return true;
}

/*********************************************************************************************\
* Display a given string.
* Text can be placed at arbitrary location on the display using the length and
* position parameters without affecting the rest of the display.
* Command:   DisplayText text [, position {0-(Settings->display_width-1)} [,length {1 to Settings->display_width}]]
\*********************************************************************************************/
bool CmndTM1650Text(bool clear)
{
  char text[CMD_MAX_LEN + 2 + 1];
  char sString[CMD_MAX_LEN + 2 + 1];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  uint8_t length = 0;
  uint8_t position = 0;
  uint8_t dots = 0;
  uint8_t strLen = 0;

  switch (ArgC())
  {
  case 3:
    subStr(sLength, XdrvMailbox.data, ",", 3);
    length = atoi(sLength);
  case 2:
    subStr(sPosition, XdrvMailbox.data, ",", 2);
    position = atoi(sPosition);
  case 1:
    subStr(sString, XdrvMailbox.data, ",", 1);
  }

  if ((position < 0) || (position > (Settings->display_width - 1)))
    position = 0;
  
  strLen = strlen(sString);

  if (!length)
    length = Settings->display_width;
  if ((length < 0) || (length > Settings->display_width))
    length = Settings->display_width;

  if (clear)
    TM1650ClearDisplay();

  uint32_t s = 0;
  uint32_t i = 0;
  
  for (i = 0; i < (strLen + position); i++)
  {
    if ((i >= (length + dots + position)) || dots > 4) {
	    break;
    }
	  
	  if(i<position) {
      text[i] = ' '; 
      continue;
	  }
	  
	  if (sString[s] == '.' && dots <= 4) {
      dots++;
    }
	
    text[i] = sString[s];
    s++;
  }

  if(i<strLen && sString[s] == '.') {
      text[i] = '.';
      i++;
  }

  text[i] = 0; //terminate string

  TM1650DisplayText(text);
  return true;
}


/*********************************************************************************************\
* Displays a clock.
* Command: DisplayClock 1   // 12-hour format
*          DisplayClock 2   // 24-hour format
*          DisplayClock 0   // turn off clock and clear
\*********************************************************************************************/

bool CmndTM1650Clock(void)
{
  uint16_t val = XdrvMailbox.payload;

  if (ArgC() == 0)
    val = 0;

  if ((val < 0) || (val > 2))
    return false;

  if (val == 1) {
    TM1650Data.show_clock = true;
    TM1650Data.clock_24 = false;
  } 
  else if (val == 2) {
    TM1650Data.show_clock = true;
    TM1650Data.clock_24 = true;
  } else {
    TM1650Data.show_clock = false;
    TM1650Data.clock_24 = false;
  }

  TM1650ClearDisplay();
  return true;
}


/*********************************************************************************************\
* refreshes the time if clock is displayed
\*********************************************************************************************/
void TM1650ShowTime()
{
  TM1650Data.iteration++;
  if (20 != TM1650Data.iteration) {
    // every 20*50ms = 1000 ms should be enough
    return;
  }
  TM1650Data.iteration = 0;

  char text[TM1650_DIGITS + 2 + 1];
  int i = 0;
  uint8_t hour = RtcTime.hour;
  uint8_t min = RtcTime.minute;
  
  if (!TM1650Data.clock_24)
  {
    if (hour > 12)
      hour -= 12;
    if (hour == 0)
      hour = 12;
  }

  text[i++] = '0' + hour / 10;
  text[i++] = '0' + hour % 10;

  if (T_XY_CLOCK == Settings->display_options.type) {
    text[i++] = '0' + min / 10;
    if(TM1650Data.clock_blynk_dots)  text[i++] = '.';  // Lower half of the colon, depending on how the LEDs are connected to the TM1650 in the XY-Clock.
    text[i++] = '0' + min % 10;
    if(TM1650Data.clock_blynk_dots)  text[i++] = '.';  // Upper half of the colon.
  }
  else if (T_303WIFILC01 == Settings->display_options.type) {
    if(TM1650Data.clock_blynk_dots) text[i++] = '.';  // Colon for 303WIFILC01
    text[i++] = '0' + min / 10;
    text[i++] = '0' + min % 10;
  }

  text[i++] = 0;

  if (!TM1650Data.clock_24 && text[0] == '0')
  {
    text[0] = ' ';
  }
  
  TM1650Data.clock_blynk_dots = !TM1650Data.clock_blynk_dots;
  TM1650DisplayText(text);
}

/*********************************************************************************************\
* This function is called for all Display functions.
\*********************************************************************************************/
bool TM1650MainFunc(uint8_t fn)
{
  bool result = false;
  if(fn != FUNC_DISPLAY_SCROLLDELAY) TM1650Data.scroll = false;
  if (XdrvMailbox.data_len > CMD_MAX_LEN)
  {
    Response_P(PSTR("{\"Error\":\"Command text too long. Please limit it to %d characters\"}"), CMD_MAX_LEN);
    return false;
  }

  switch (fn)
  {
  case FUNC_DISPLAY_CLEAR:
    result = CmndTM1650Clear();
    break;
  case FUNC_DISPLAY_NUMBER:
    result = CmndTM1650Number(true);
    break;
  case FUNC_DISPLAY_NUMBERNC:
    result = CmndTM1650Number(false);
    break;
  case FUNC_DISPLAY_FLOAT:
    result = CmndTM1650Float(true);
    break;
  case FUNC_DISPLAY_FLOATNC:
    result = CmndTM1650Float(false);
    break;
  case FUNC_DISPLAY_RAW:
    result = CmndTM1650Raw();
    break;
  case FUNC_DISPLAY_SEVENSEG_TEXT:
    result = CmndTM1650Text(true);
    break;
  case FUNC_DISPLAY_SEVENSEG_TEXTNC:
    result = CmndTM1650Text(false);
    break;
  case FUNC_DISPLAY_LEVEL:
    result = CmndTM1650Level();
    break;
  case FUNC_DISPLAY_SCROLLTEXT:
    result = CmndTM1650ScrollText();
    break;
  case FUNC_DISPLAY_SCROLLDELAY:
    result = CmndTM1650ScrollDelay();
    break;
  case FUNC_DISPLAY_CLOCK:
    result = CmndTM1650Clock();
    break;
  }

  return result;
}


void TM1650Dim(void)
{
  int brightness = GetDisplayDimmer16();
  if (brightness < 2) {
    TM1650DisplayOff();
  }
  else if (brightness > 14) {
    TM1650DisplayOn();
    TM1650SetBrightness(0); // In TM1650, brightness 0 means max brightness (level 8).
    TM1650DisplayOn();
  }
  else {
    // Map 2...14 to 1...7:
    TM1650SetBrightness(brightness >> 1);
    TM1650DisplayOn();
  }
}


/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void TM1650Time(void)
{
  char text[TM1650_DIGITS + 2 + 1];
  int i = 0;

  text[i++] = '0' + RtcTime.hour / 10;
  text[i++] = '0' + RtcTime.hour % 10;

  if (T_XY_CLOCK == Settings->display_options.type) {
    text[i++] = '0' + RtcTime.minute / 10;
    if(TM1650Data.clock_blynk_dots)  text[i++] = '.';  // Lower half of the colon, depending on how the LEDs are connected to the TM1650 in the XY-Clock.
    text[i++] = '0' + RtcTime.minute % 10;
    if(TM1650Data.clock_blynk_dots)  text[i++] = '.';  // Upper half of the colon.
  }
  else if (T_303WIFILC01 == Settings->display_options.type) {
    if(TM1650Data.clock_blynk_dots) text[i++] = '.';  // Colon for 303WIFILC01
    text[i++] = '0' + RtcTime.minute / 10;
    text[i++] = '0' + RtcTime.minute % 10;
  }

  text[i++] = 0;
  
  TM1650Data.clock_blynk_dots = !TM1650Data.clock_blynk_dots;
  TM1650DisplayText(text);
}

void TM1650Date(void)
{
  char text[TM1650_DIGITS + 2 + 1];
  int i = 0;

  text[i++] = '0' + RtcTime.day_of_month / 10;
  text[i++] = '0' + RtcTime.day_of_month % 10;
  
  if (T_XY_CLOCK == Settings->display_options.type) {
    text[i++] = '0' + RtcTime.month / 10;
    text[i++] = '0' + RtcTime.month % 10;
    text[i++] = '.';  // Lower half of the colon, depending on how the LEDs are connected to the TM1650 in the XY-Clock.
  }
  else if (T_303WIFILC01 == Settings->display_options.type ) {
    text[i++] = '.';  // Colon for 303WIFILC01
    text[i++] = '0' + RtcTime.month / 10;
    text[i++] = '0' + RtcTime.month % 10;
  }

  text[i++] = 0;

  TM1650DisplayText(text);
}

void TM1650Refresh(void)  // Every second
{
  if (Settings->display_mode) {  // Mode 0 is User text
    switch (Settings->display_mode) {
      case 1:  // Time
        TM1650Time();
        break;
      case 2:  // Date
        TM1650Date();
        break;
      case 3: // Time/Date
        if (TasmotaGlobal.uptime % Settings->display_refresh)
        {
          TM1650Time();
        }
        else
        {
          TM1650Date();
        }
        break;
      case 4: 
      case 5:
        // not in use
        break;
    }
  }
}


#endif // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp20(uint32_t function)
{
  if (!I2cEnabled(XI2C_74)) { return false; }

  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function)
  {
    TM1650InitDriver();
  }
  else if ((TM1650Data.init_driver_done || FUNC_DISPLAY_MODEL == function) 
           && (XDSP_20 == Settings->display_model)) {
    switch (function) {
      case FUNC_DISPLAY_EVERY_50_MSECOND:
      if (disp_power && !Settings->display_mode)
      {
        if (TM1650Data.scroll)
        {
          TM1650ScrollText();
        }
        if (TM1650Data.show_clock)
        {
          TM1650ShowTime();
        }
      }
      break;
      case FUNC_DISPLAY_INIT:
        TM1650Init(dsp_init);
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        TM1650Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_SEVENSEG_TEXT:
      case FUNC_DISPLAY_CLEAR:
      case FUNC_DISPLAY_NUMBER:
      case FUNC_DISPLAY_FLOAT:
      case FUNC_DISPLAY_NUMBERNC:
      case FUNC_DISPLAY_FLOATNC:
      case FUNC_DISPLAY_RAW:
      case FUNC_DISPLAY_LEVEL:
      case FUNC_DISPLAY_SEVENSEG_TEXTNC:
      case FUNC_DISPLAY_SCROLLTEXT:
      case FUNC_DISPLAY_SCROLLDELAY:
      case FUNC_DISPLAY_CLOCK:
          result = TM1650MainFunc(function);
        break;
      case FUNC_DISPLAY_DIM:
        TM1650Dim();
	break;
      case FUNC_DISPLAY_POWER:
        TM1650DisplayOnOff();
        break;  

    }
  }
  return result;
}

#endif  // USE_DISPLAY_TM1650
#endif  // USE_DISPLAY
#endif  // USE_I2C
