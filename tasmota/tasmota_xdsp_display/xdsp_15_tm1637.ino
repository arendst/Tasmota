/*
  xdsp_15_tm1637.ino - Support for TM1637- and TM1638-based seven-segment displays for Tasmota

  Copyright (C) 2021  Ajith Vasudevan

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

#ifdef USE_DISPLAY
#if defined(USE_DISPLAY_TM1637) || defined(USE_DISPLAY_MAX7219)
/*********************************************************************************************\
  This driver enables the display of numbers (both integers and floats) and basic text
  on the inexpensive TM1637-, TM1638- and MAX7219-based seven-segment modules.

  Raw segments can also be displayed.

  In addition, it is also possible to set brightness (8 levels), clear the display, scroll text,
  display a rudimentary bar graph, and a Clock (12 hr and 24 hr).

  To use, compile Tasmota with USE_DISPLAY and USE_DISPLAY_TM1637, or build the tasmota-display env.

  For TM1637:
  Connect the TM1637 display module's pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "TM1637 DIO"
  CLK hardware pin --> "TM1637 CLK"

  For TM1638:
  Connect the TM1638 display module's pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "TM1638 DIO"
  CLK hardware pin --> "TM1638 CLK"
  STB hardware pin --> "TM1638 STB"

  For MAX7219:
  Connect the MAX7219 display module's pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIN hardware pin --> "MAX7219 DIN"
  CS hardware pin --> "MAX7219 CS"
  CLK hardware pin --> "MAX7219 CLK"

  Once the GPIO configuration is saved and the ESP8266/ESP32 module restarts,
  set the Display Model to 15 and Display Mode to 0
  using the command "Backlog DisplayModel 15 ; DisplayMode 0"

  If your display is a TM1637 with 6 digits, set Display Width to the number of digits your
  display has, using the command "DisplayWidth 6".

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



  DisplayFloat          num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width} [,alignment {0=left 1=right}]]]]

                               Clears and then displays float (with decimal point)  command e.g., "DisplayFloat 12.34"
                               See function description below for more details.



  DisplayFloatNC        num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width} [,alignment {0=left 1=right}]]]]

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
                               A caret(^) symbol in the text input is dispayed as the degrees(°) symbol. This is useful for displaying Temperature!
                               For example, the command "DisplayText 22.5^" will display "22.5°".


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


In addition, setting DisplayMode to 1 shows the time, setting it to 2 shows the date
and setting it to 3 alternates between time and date.



\*********************************************************************************************/

#define XDSP_15 15

#define CMD_MAX_LEN 55
#define LEVEL_MIN 0
#define LEVEL_MAX 100
#define SCROLL_MAX_LEN 50
#define POSITION_MIN 0
#define POSITION_MAX 8
#define LED_MIN 0
#define LED_MAX 255
#define MAX7219_ADDR 0


#include "SevenSegmentTM1637.h"
#include <TM1638plus.h>
#include <LedControl.h>

SevenSegmentTM1637 *tm1637display;
TM1638plus *tm1638display;
LedControl *max7219display;

enum display_types
{
  TM1637,
  TM1638,
  MAX7219
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
  uint8_t display_type = TM1637;
  uint8_t digit_order[6] = { 0, 1, 2, 3, 4, 5 };

  bool init_done = false;
  bool scroll = false;
  bool show_clock = false;
  bool clock_24 = false;
} TM1637Data;

/*********************************************************************************************\
* Init function
\*********************************************************************************************/

void TM1637Init(void)
{
  if (PinUsed(GPIO_TM1638CLK) && PinUsed(GPIO_TM1638DIO) && PinUsed(GPIO_TM1638STB))
  {
    TM1637Data.display_type = TM1638;
    Settings->display_width = 8;
  }
  else if (PinUsed(GPIO_TM1637CLK) && PinUsed(GPIO_TM1637DIO))
  {
    TM1637Data.display_type = TM1637;
    if ((!Settings->display_width || Settings->display_width > 6))
    {
      Settings->display_width = 4;
      Settings->display_options.type = 0;
    }
    TM1637SetDigitOrder();
  }
  else if (PinUsed(GPIO_MAX7219DIN) && PinUsed(GPIO_MAX7219CLK) && PinUsed(GPIO_MAX7219CS))
  {
    TM1637Data.display_type = MAX7219;
    if (Settings->display_width)
    {
      Settings->display_width = (Settings->display_width / 8) * 8;
    }
    else
    {
      Settings->display_width = 8;
    }
  }
  else
  {
    return;
  }

  Settings->display_model = XDSP_15;
  Settings->display_cols[0] = Settings->display_width;
  Settings->display_height = 1;
  Settings->display_rows = Settings->display_height;
  uint8_t dimmer16 = GetDisplayDimmer16();
  if(!dimmer16 || dimmer16 < 2 || dimmer16 > 15) SetDisplayDimmer(50);

  if (TM1637 == TM1637Data.display_type)
  {
    strcpy_P(TM1637Data.model_name, PSTR("TM1637"));
    tm1637display = new SevenSegmentTM1637(Pin(GPIO_TM1637CLK), Pin(GPIO_TM1637DIO));
    tm1637display->begin(Settings->display_width, 1);
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    strcpy_P(TM1637Data.model_name, PSTR("TM1638"));
    tm1638display = new TM1638plus(Pin(GPIO_TM1638STB), Pin(GPIO_TM1638CLK), Pin(GPIO_TM1638DIO), true);
    tm1638display->displayBegin();
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    strcpy_P(TM1637Data.model_name, PSTR("MAX7219"));
    max7219display = new LedControl(Pin(GPIO_MAX7219DIN), Pin(GPIO_MAX7219CLK), Pin(GPIO_MAX7219CS), Settings->display_width / 8);
    for (uint8_t dev_addr = 0; dev_addr < Settings->display_width / 8; dev_addr++)
    {
      max7219display->shutdown(MAX7219_ADDR + dev_addr, false);
    }
  }
  TM1637ClearDisplay();
  TM1637Dim();
  TM1637Data.init_done = true;
  AddLog(LOG_LEVEL_INFO, PSTR("DSP: %s with %d digits (type %d)"), TM1637Data.model_name, Settings->display_width, Settings->display_options.type);
}

// Function to display specified ascii char at specified position for MAX7219
void displayMAX7219ASCII(uint8_t pos, char c)
{
  max7219display->setChar(MAX7219_ADDR + (pos / 8), 7 - (pos % 8), c, false);
}

// Function to display specified ascii char with dot at specified position for MAX7219
void displayMAX7219ASCIIwDot(uint8_t pos, char c)
{
  max7219display->setChar(MAX7219_ADDR + (pos / 8), 7 - (pos % 8), c, true);
}

// Function to display raw segments at specified position for MAX7219
void displayMAX72197Seg(uint8_t pos, uint8_t seg)
{
  bool dec_bit = seg & 128;
  seg = seg << 1;
  seg = seg | dec_bit;
  uint8_t NO_OF_BITS = 8;
  uint8_t reverse_num = 0;
  for (uint8_t i = 0; i < NO_OF_BITS; i++)
  {
    if ((seg & (1 << i)))
      reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
  }
  seg = reverse_num;

  max7219display->setRow(MAX7219_ADDR + (pos / 8), 7 - (pos % 8), seg);
}

// Function to fix order of hardware digits for different TM1637 variants
void TM1637SetDigitOrder(void) {
  if (0 == Settings->display_options.type) {
    for (uint32_t i = 0; i < 6; i++) {
      TM1637Data.digit_order[i] = i;
    }
  }
  else if (1 == Settings->display_options.type) {
    TM1637Data.digit_order[0] = 2;
    TM1637Data.digit_order[1] = 1;
    TM1637Data.digit_order[2] = 0;
    TM1637Data.digit_order[3] = 5;
    TM1637Data.digit_order[4] = 4;
    TM1637Data.digit_order[5] = 3;
  }
}

/*********************************************************************************************\
* Displays number without decimal, with/without leading zeros, specifying start-position
* and length, optionally skipping clearing display before displaying the number.
* commands: DisplayNumber   num [,position {0-(Settings->display_width-1)} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]
*           DisplayNumberNC num [,position {0-(Settings->display_width-1)} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Number(bool clear)
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
    num = CharToFloat(sNum);
  }

  if ((position < 0) || (position > (Settings->display_width - 1)))
    position = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: num %d, pos %d, lead %d, len %d"), num, position, leadingzeros, length);

  if (clear)
    TM1637ClearDisplay();

  char txt[30];
  snprintf_P(txt, sizeof(txt), PSTR("%d"), num);
  if (!length)
    length = strlen(txt);
  if ((length < 0) || (length > Settings->display_width))
    length = Settings->display_width;

  char pad = (leadingzeros ? '0' : ' ');
  uint32_t i = position;
  uint8_t rawBytes[1];

  for (; i < position + (length - strlen(txt)); i++)
  {
    if (i > Settings->display_width)
      break;
    if (TM1637 == TM1637Data.display_type)
    {
      rawBytes[0] = tm1637display->encode(pad);
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
    else if (TM1638 == TM1637Data.display_type)
      tm1638display->displayASCII(i, pad);
    else if (MAX7219 == TM1637Data.display_type)
    {
      if (i > Settings->display_width - 1)
        break;
      displayMAX7219ASCII(i, pad);
    }
  }

  for (uint32_t j = 0; i < position + length; i++, j++)
  {
    if (i > Settings->display_width)
      break;
    if (txt[j] == 0)
      break;
    if (TM1637 == TM1637Data.display_type)
    {
      rawBytes[0] = tm1637display->encode(txt[j]);
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
    else if (TM1638 == TM1637Data.display_type)
      tm1638display->displayASCII(i, txt[j]);
    else if (MAX7219 == TM1637Data.display_type)
    {
      if (i > Settings->display_width - 1)
        break;
      if (txt[j] == 0)
        break;
      displayMAX7219ASCII(i, txt[j]);
    }
  }

  return true;
}

/*********************************************************************************************\
* Displays number with decimal, specifying position, precision and length,
* optionally skipping clearing display before displaying the number.
* commands: DisplayFloat   num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]
*           DisplayFloatNC num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Float(bool clear)
{

  char sNum[CMD_MAX_LEN];
  char sPrecision[CMD_MAX_LEN];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  char sAlignment[CMD_MAX_LEN];
  uint8_t length = 0;
  uint8_t precision = Settings->display_width;
  uint8_t position = 0;
  uint8_t alignment = 0;
  float fnum = 0.0f;

  switch (ArgC())
  {
  case 5:
    subStr(sAlignment, XdrvMailbox.data, ",", 5);
    alignment = atoi(sAlignment);
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

  if ((position < 0) || (position > (Settings->display_width - 1)))
    position = 0;
  if ((precision < 0) || (precision > Settings->display_width))
    precision = Settings->display_width;

  if (clear)
    TM1637ClearDisplay();

  char txt[30];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), precision, &fnum);

  if (!length)
    length = strlen(txt);
  if ((length <= 0) || (length > Settings->display_width))
    length = Settings->display_width;

  // Add leading spaces before value if txt is shorter than length
  if ((alignment == 1) && (strlen(txt) < length + 1))
  {
    char tmptxt[30];
    ext_snprintf_P(tmptxt, sizeof(tmptxt), "%*s%s", strlen(txt)-(length+1), "", txt);
    strcpy (txt, tmptxt);
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: num %4_f, prec %d, len %d"), &fnum, precision, length);

  if (TM1637 == TM1637Data.display_type)
  {
    uint8_t rawBytes[1];
    for (uint32_t i = 0, j = 0; i < length; i++, j++)
    {
      if (txt[i] == 0)
        break;
      rawBytes[0] = tm1637display->encode(txt[i]);
      if (txt[i + 1] == '.')
      {
        rawBytes[0] = rawBytes[0] | 128;
        i++;
        length++;
      }
      if ((j + position) > Settings->display_width)
        break;
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[j + position]);
    }
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    for (uint32_t i = 0, j = 0; i < length; i++, j++)
    {
      if ((j + position) > 7)
        break;
      if (txt[i] == 0)
        break;
      if (txt[i + 1] == '.')
      {
        tm1638display->displayASCIIwDot(j + position, txt[i]);
        i++;
        length++;
      }
      else
        tm1638display->displayASCII(j + position, txt[i]);
    }
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    for (uint32_t i = 0, j = 0; i < length; i++, j++)
    {
      if ((j + position) > Settings->display_width - 1)
        break;
      if (txt[i] == 0)
        break;
      if (txt[i + 1] == '.')
      {
        displayMAX7219ASCIIwDot(j + position, txt[i]);
        i++;
        length++;
      }
      else
        displayMAX7219ASCII(j + position, txt[i]);
    }
  }

  return true;
}

// /*********************************************************************************************\
// * Clears the display
// * Command:  DisplayClear
// \*********************************************************************************************/
bool CmndTM1637Clear(void)
{
  TM1637ClearDisplay();
  sprintf(TM1637Data.msg, PSTR("Cleared"));
  XdrvMailbox.data = TM1637Data.msg;
  return true;
}

// /*********************************************************************************************\
// * Clears the display
// \*********************************************************************************************/
void TM1637ClearDisplay(void)
{
  if (TM1637 == TM1637Data.display_type)
  {
    unsigned char arr[] = {0};
    for (int i = 0; i < Settings->display_width; i++)
      tm1637display->printRaw(arr, 1, i);
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    for (int i = 0; i < Settings->display_width; i++)
      tm1638display->display7Seg(i, 0);
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    for (uint8_t dev_addr = 0; dev_addr < Settings->display_width / 8; dev_addr++)
    {
      max7219display->clearDisplay(MAX7219_ADDR + dev_addr);
    }
  }
}

/*********************************************************************************************\
* Display scrolling text
* Command:   DisplayTM1637Data.scroll_text text
\*********************************************************************************************/
bool CmndTM1637ScrollText(void)
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

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: sString %s, maxLoopCount %d"), sString, maxLoopCount);

  TM1637Data.scroll_counter_max = maxLoopCount;

  if (strlen(sString) > SCROLL_MAX_LEN)
  {
    snprintf(TM1637Data.msg, sizeof(TM1637Data.msg), PSTR("Text too long. Length should be less than %d"), SCROLL_MAX_LEN);
    XdrvMailbox.data = TM1637Data.msg;
    return false;
  }
  else
  {
    snprintf(TM1637Data.scroll_text, sizeof(TM1637Data.scroll_text), PSTR("                                                               "));
    snprintf(TM1637Data.scroll_text, Settings->display_width + sizeof(TM1637Data.scroll_text), PSTR("        %s"), &sString);
    TM1637Data.scroll_text[strlen(sString) + Settings->display_width] = 0;
    TM1637Data.scroll_index = 0;
    TM1637Data.scroll = true;
    TM1637Data.scroll_counter = 0;
    return true;
  }
}

/*********************************************************************************************\
* Sets the scroll delay for scrolling text.
* Command:  DisplayTM1637Data.scroll_delay delay {0-15}    // default = 4
\*********************************************************************************************/
bool CmndTM1637ScrollDelay(void)
{
  if (ArgC() == 0)
  {
    XdrvMailbox.payload = TM1637Data.scroll_delay;
    return true;
  }
  if (TM1637Data.scroll_delay < 0)
    TM1637Data.scroll_delay = 0;
  TM1637Data.scroll_delay = XdrvMailbox.payload;
  return true;
}

/*********************************************************************************************\
* Scrolls a given string. Called every 50ms
\*********************************************************************************************/
void TM1637ScrollText(void)
{
  if(!TM1637Data.scroll) return;
  TM1637Data.iteration++;
  if (TM1637Data.scroll_delay)
    TM1637Data.iteration = TM1637Data.iteration % TM1637Data.scroll_delay;
  else
    TM1637Data.iteration = 0;
  if (TM1637Data.iteration)
    return;

  if (TM1637Data.scroll_index > strlen(TM1637Data.scroll_text))
  {
    TM1637Data.scroll_index = 0;
    TM1637Data.scroll_counter++;
    if(TM1637Data.scroll_counter_max != 0 && (TM1637Data.scroll_counter >= TM1637Data.scroll_counter_max)) {
      TM1637Data.scroll = false;
      return;
    }    
  }
  uint8_t rawBytes[1];
  for (uint32_t i = 0, j = TM1637Data.scroll_index; i < 1 + strlen(TM1637Data.scroll_text); i++, j++)
  {
    if (i > (Settings->display_width - 1))
    {
      break;
    }
    rawBytes[0] = tm1637display->encode(TM1637Data.scroll_text[j]);
    bool dotSkipped = false;
    if (TM1637Data.scroll_text[j + 1] == '.')
    {
      dotSkipped = true;
      rawBytes[0] = rawBytes[0] | 128;
      j++;
    }
    else if (TM1637Data.scroll_text[j] == '^')
    {
      rawBytes[0] = 1 | 2 | 32 | 64;
    }
    if (!dotSkipped && TM1637Data.scroll_text[j] == '.')
    {
      j++;
      TM1637Data.scroll_index++;
      rawBytes[0] = tm1637display->encode(TM1637Data.scroll_text[j]);
    }
    if (TM1637Data.scroll_text[j + 1] == '.')
    {
      rawBytes[0] = rawBytes[0] | 128;
    }
    if (TM1637 == TM1637Data.display_type)
    {
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
    else if (TM1638 == TM1637Data.display_type)
    {
      tm1638display->display7Seg(i, rawBytes[0]);
    }
    else if (MAX7219 == TM1637Data.display_type)
    {
      displayMAX72197Seg(i, rawBytes[0]);
    }
  }
  TM1637Data.scroll_index++;
}

/*********************************************************************************************\
* Displays a horizontal bar graph. Takes a percentage number (0-100) as input
* Command:   DisplayLevel level {0-100}
\*********************************************************************************************/
bool CmndTM1637Level(void)
{
  uint16_t val = XdrvMailbox.payload;
  if ((val < LEVEL_MIN) || (val > LEVEL_MAX))
  {
    Response_P(PSTR("{\"Error\":\"Level should be a number in the range [%d, %d]\"}"), LEVEL_MIN, LEVEL_MAX);
    return false;
  }

  uint8_t totalBars = 2 * Settings->display_width;
  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: TM1637Data.model_name %s CmndTM1637Level totalBars=%d"), TM1637Data.model_name, totalBars);
  float barsToDisplay = totalBars * val / 100.0f;
  char txt[5];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), 1, &barsToDisplay);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: TM1637Data.model_name %s CmndTM1637Level barsToDisplay=%s"), TM1637Data.model_name, txt);
  char s[4];
  ext_snprintf_P(s, sizeof(s), PSTR("%0_f"), &barsToDisplay);
  uint8_t numBars = atoi(s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: CmndTM1637Level numBars %d"), numBars);

  TM1637ClearDisplay();
  uint8_t rawBytes[1];
  for (int i = 1; i <= numBars; i++)
  {
    uint8_t digit = (i - 1) / 2;
    uint8_t value = (((i % 2) == 0) ? 54 : 48);
    if (TM1637 == TM1637Data.display_type)
    {
      rawBytes[0] = value;
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[digit]);
    }
    else if (TM1638 == TM1637Data.display_type)
    {
      tm1638display->display7Seg(digit, value);
    }
    else if (MAX7219 == TM1637Data.display_type)
    {
      displayMAX72197Seg(digit, value);
    }
  }
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
bool CmndTM1637Raw(void)
{
  uint8_t DATA[6] = {0, 0, 0, 0, 0, 0};

  char as[CMD_MAX_LEN];
  char bs[CMD_MAX_LEN];
  char cs[CMD_MAX_LEN];
  char ds[CMD_MAX_LEN];
  char es[CMD_MAX_LEN];
  char fs[CMD_MAX_LEN];

  char sLength[CMD_MAX_LEN];
  char sPos[CMD_MAX_LEN];

  uint32_t position = 0;
  uint32_t length = 0;

  switch (ArgC())
  {
  case 8:
    subStr(fs, XdrvMailbox.data, ",", 8);
    DATA[5] = atoi(fs);
  case 7:
    subStr(es, XdrvMailbox.data, ",", 7);
    DATA[4] = atoi(es);
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

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: a %d, b %d, c %d, d %d, e %d, f %d, len %d, pos %d"),
         DATA[0], DATA[1], DATA[2], DATA[3], DATA[4], DATA[5], length, position);

  if (TM1637 == TM1637Data.display_type)
  {
    uint8_t rawBytes[1];
    for (uint32_t i = position; i < position + length; i++)
    {
      if (i > (Settings->display_width - 1))
        break;
      rawBytes[0] = DATA[i - position];
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    for (uint32_t i = position; i < position + length; i++)
    {
      if (i > 7)
        break;
      tm1638display->display7Seg(i, DATA[i - position]);
    }
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    for (uint32_t i = position; i < position + length; i++)
    {
      if (i > Settings->display_width - 1)
        break;
      displayMAX72197Seg(i, DATA[i - position]);
    }
  }
  return true;
}

/*********************************************************************************************\
* Display a given string.
* Text can be placed at arbitrary location on the display using the length and
* position parameters without affecting the rest of the display.
* Command:   DisplayText text [, position {0-(Settings->display_width-1)} [,length {1 to Settings->display_width}]]
\*********************************************************************************************/
bool CmndTM1637Text(bool clear)
{
  char sString[CMD_MAX_LEN + 1];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  uint8_t length = 0;
  uint8_t position = 0;

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

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: sString %s, pos %d, len %d"), sString, position, length);

  if (clear)
    TM1637ClearDisplay();

  if (!length)
    length = strlen(sString);
  if ((length < 0) || (length > Settings->display_width))
    length = Settings->display_width;

  uint32_t i = position;
  if (TM1637 == TM1637Data.display_type)
  {
    uint8_t rawBytes[1];
    for (uint32_t j = 0; i < position + length; i++, j++)
    {
      if (i > (Settings->display_width - 1))
        break;
      if (sString[j] == 0)
        break;
      rawBytes[0] = tm1637display->encode(sString[j]);
      bool dotSkipped = false;
      if (sString[j + 1] == '.')
      {
        dotSkipped = true;
        rawBytes[0] = rawBytes[0] | 128;
        j++;
      }
      else if (sString[j] == '^')
      {
        rawBytes[0] = 1 | 2 | 32 | 64;
      }
      if (!dotSkipped && sString[j] == '.')
        rawBytes[0] = 128;
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    for (uint32_t j = 0; i < position + length; i++, j++)
    {
      if (i > 7)
        break;
      if (sString[j] == 0)
        break;
      if (sString[j + 1] == '.')
      {
        tm1638display->displayASCIIwDot(i, sString[j]);
        j++;
      }
      else if (sString[j] == '^')
      {
        tm1638display->display7Seg(i, (1 | 2 | 32 | 64));
      }
      else
        tm1638display->displayASCII(i, sString[j]);
    }
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    uint8_t rawBytes[1];
    for (uint32_t j = 0; i < position + length; i++, j++)
    {
      if (i > Settings->display_width - 1)
        break;
      if (sString[j] == 0)
        break;
      rawBytes[0] = tm1637display->encode(sString[j]);
      bool dotSkipped = false;
      if (sString[j + 1] == '.')
      {
        dotSkipped = true;
        rawBytes[0] = rawBytes[0] | 128;
        j++;
      }
      else if (sString[j] == '^')
      {
        rawBytes[0] = 1 | 2 | 32 | 64;
      }
      if (!dotSkipped && sString[j] == '.')
        rawBytes[0] = 128;
      displayMAX72197Seg(i, rawBytes[0]);
    }
  }
  return true;
}

/*********************************************************************************************\
* Displays a clock.
* Command: DisplayClock 1   // 12-hour format
*          DisplayClock 2   // 24-hour format
*          DisplayClock 0   // turn off clock and clear
\*********************************************************************************************/
bool CmndTM1637Clock(void)
{

  TM1637Data.show_clock = XdrvMailbox.payload;

  if (ArgC() == 0)
    XdrvMailbox.payload = 1;
  if (XdrvMailbox.payload > 1) {
    TM1637Data.clock_24 = true;
    XdrvMailbox.payload = 2;
  } else {
    TM1637Data.clock_24 = false;
    XdrvMailbox.payload = 1;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("TM7: TM1637Data.show_clock %d, TM1637Data.clock_24 %d"), TM1637Data.show_clock, TM1637Data.clock_24);

  TM1637ClearDisplay();
  return true;
}

/*********************************************************************************************\
* refreshes the time if clock is displayed
\*********************************************************************************************/
void TM1637ShowTime()
{
  struct TIME_T t = RtcTime;
  BreakNanoTime(Rtc.local_time, Rtc.nanos + (millis() - Rtc.millis) * 1000000, t);
  uint8_t hr = t.hour;
  uint8_t mn = t.minute;
  uint8_t sc = t.second;
  uint16_t ms = t.nanos / 1000000;
  
  if (!TM1637Data.clock_24)
  {
    if (hr > 12)
      hr -= 12;
    if (hr == 0)
      hr = 12;
  }

  char tm[7];
  snprintf_P(tm, sizeof(tm), PSTR("%02d%02d%02d"), hr, mn, sc);

  if (!TM1637Data.clock_24 && tm[0] == '0')
  {
    tm[0] = ' ';
  }
  
  if (TM1637 == TM1637Data.display_type)
  {
    uint8_t colon = ms > 500? 0: 128;
    uint8_t rawBytes[1];
    uint8_t width = Settings->display_width >= 6? 6: 4;
    for (uint32_t i = 0; i < width; i++)
    {
      rawBytes[0] = tm1637display->encode(tm[i]);
      if (i == 1 || (i == 3 && width > 4))
        rawBytes[0] = rawBytes[0] | colon;
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    for (uint32_t i = 0; i < 4; i++)
    {
      if ((millis() % 1000) > 500 && (i == 1))
        tm1638display->displayASCIIwDot(i, tm[i]);
      else
        tm1638display->displayASCII(i, tm[i]);
    }
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    for (uint32_t i = 0; i < 6; i++)
    {
      //if ((millis() % 1000) > 500 && (i == 3))
      if ((i == 1) || (i == 3))
        displayMAX7219ASCIIwDot(i, tm[i]);
      else
        displayMAX7219ASCII(i, tm[i]);
    }
  }
}

/*********************************************************************************************\
* This function is called for all Display functions.
\*********************************************************************************************/
bool TM1637MainFunc(uint8_t fn)
{
  bool result = false;
  if(fn != FUNC_DISPLAY_SCROLLDELAY) TM1637Data.scroll = false;
  if (XdrvMailbox.data_len > CMD_MAX_LEN)
  {
    Response_P(PSTR("{\"Error\":\"Command text too long. Please limit it to %d characters\"}"), CMD_MAX_LEN);
    return false;
  }

  switch (fn)
  {
  case FUNC_DISPLAY_CLEAR:
    result = CmndTM1637Clear();
    break;
  case FUNC_DISPLAY_NUMBER:
    result = CmndTM1637Number(true);
    break;
  case FUNC_DISPLAY_NUMBERNC:
    result = CmndTM1637Number(false);
    break;
  case FUNC_DISPLAY_FLOAT:
    result = CmndTM1637Float(true);
    break;
  case FUNC_DISPLAY_FLOATNC:
    result = CmndTM1637Float(false);
    break;
  case FUNC_DISPLAY_RAW:
    result = CmndTM1637Raw();
    break;
  case FUNC_DISPLAY_SEVENSEG_TEXT:
    result = CmndTM1637Text(true);
    break;
  case FUNC_DISPLAY_SEVENSEG_TEXTNC:
    result = CmndTM1637Text(false);
    break;
  case FUNC_DISPLAY_LEVEL:
    result = CmndTM1637Level();
    break;
  case FUNC_DISPLAY_SCROLLTEXT:
    result = CmndTM1637ScrollText();
    break;
  case FUNC_DISPLAY_SCROLLDELAY:
    result = CmndTM1637ScrollDelay();
    break;
  case FUNC_DISPLAY_CLOCK:
    result = CmndTM1637Clock();
    break;
  }

  return result;
}

void TM1637Dim(void)
{
  // GetDisplayDimmer16() = 0 - 15
  uint8_t brightness = GetDisplayDimmer16() >> 1; // 0 - 7

  if (TM1637 == TM1637Data.display_type)
  {
    tm1637display->setBacklight(brightness * 12); // 0 - 84
  }
  else if (TM1638 == TM1637Data.display_type)
  {
    tm1638display->brightness(brightness); // 0 - 7
  }
  else if (MAX7219 == TM1637Data.display_type)
  {
    for (uint8_t dev_addr = 0; dev_addr < Settings->display_width / 8; dev_addr++)
    {
      max7219display->setIntensity(MAX7219_ADDR, brightness); // 0 - 7
    }
  }
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void TM1637Print(char *txt)
{
  for (uint32_t i = 0; i < Settings->display_cols[0]; i++)
  {
    if (TM1637 == TM1637Data.display_type)
    {
      uint8_t rawBytes[1];
      rawBytes[0] = tm1637display->encode(txt[i]);
      //      if ((millis() % 1000) > 500 && (i == 1)) { rawBytes[0] = rawBytes[0] | 128; }
      tm1637display->printRaw(rawBytes, 1, TM1637Data.digit_order[i]);
    }
    else if (TM1638 == TM1637Data.display_type)
    {
      //      if ((millis() % 1000) > 500 && (i == 1)) { tm1638display->displayASCIIwDot(i, txt[i]); }
      tm1638display->displayASCII(i, txt[i]);
    }
    else if (MAX7219 == TM1637Data.display_type)
    {
      //      if ((millis() % 1000) > 500 && (i == 1)) { tm1638display->displayASCIIwDot(i, txt[i]); }
      displayMAX7219ASCII(i, txt[i]);
    }

  }
}

void TM1637Center(char *txt)
{
  char line[Settings->display_cols[0] + 2];

  int len = strlen(txt);
  int offset = 0;
  if (len >= Settings->display_cols[0])
  {
    len = Settings->display_cols[0];
  }
  else
  {
    offset = (Settings->display_cols[0] - len) / 2;
  }
  memset(line, 0x20, Settings->display_cols[0]);
  line[Settings->display_cols[0]] = 0;
  for (uint32_t i = 0; i < len; i++)
  {
    line[offset + i] = txt[i];
  }
  TM1637Print(line);
}

/*
bool TM1637PrintLog(void) {
  bool result = false;

  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings->display_refresh;
    if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }

    char* txt = DisplayLogBuffer('\337');
    if (txt != nullptr) {
      uint8_t last_row = Settings->display_rows -1;

      strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
      DisplayFillScreen(last_row);

      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);

      TM1637Print(disp_screen_buffer[last_row]);

      result = true;
    }
  }
  return result;
}
*/

void TM1637Time(void)
{
  char line[Settings->display_cols[0] + 1];

  if (Settings->display_cols[0] >= 8)
  {
    snprintf_P(line, sizeof(line), PSTR("%02d %02d %02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
  }
  else if (Settings->display_cols[0] >= 6)
  {
    snprintf_P(line, sizeof(line), PSTR("%02d%02d%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
  }
  else
  {
    snprintf_P(line, sizeof(line), PSTR("%02d%02d"), RtcTime.hour, RtcTime.minute);
  }
  TM1637Center(line);
}

void TM1637Date(void)
{
  char line[Settings->display_cols[0] + 1];

  if (Settings->display_cols[0] >= 8)
  {
    snprintf_P(line, sizeof(line), PSTR("%02d-%02d-%02d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year - 2000);
  }
  else if (Settings->display_cols[0] >= 6)
  {
    snprintf_P(line, sizeof(line), PSTR("%02d%02d%02d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year - 2000);
  }
  else
  {
    snprintf_P(line, sizeof(line), PSTR("%02d%02d"), RtcTime.day_of_month, RtcTime.month);
  }
  TM1637Center(line);
}

void TM1637Refresh(void)
{ // Every second
  if (!disp_power || !Settings->display_mode)
  {
    return;
  } // Mode 0 is User text

  switch (Settings->display_mode)
  {
  case 1: // Time
    TM1637Time();
    break;
  case 2: // Date
    TM1637Date();
    break;
  case 3: // Time
    if (TasmotaGlobal.uptime % Settings->display_refresh)
    {
      TM1637Time();
    }
    else
    {
      TM1637Date();
    }
    break;
    /*
    case 4:  // Mqtt
      TM1637PrintLog();
      break;
    case 5: {  // Mqtt
      if (!TM1637PrintLog()) { TM1637Time(); }
      break;
    }
*/
  }
}

#endif // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp15(uint32_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function)
  {
    TM1637Init();
  }
  else if (TM1637Data.init_done && (XDSP_15 == Settings->display_model))
  {
    switch (function)
    {
    case FUNC_DISPLAY_EVERY_50_MSECOND:
      if (disp_power && !Settings->display_mode)
      {
        if (TM1637Data.scroll)
        {
          TM1637ScrollText();
        }
        if (TM1637Data.show_clock)
        {
          TM1637ShowTime();
        }
      }
      break;
#ifdef USE_DISPLAY_MODES1TO5
    case FUNC_DISPLAY_EVERY_SECOND:
      TM1637Refresh();
      break;
#endif // USE_DISPLAY_MODES1TO5
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
      if (disp_power && !Settings->display_mode)
      {
        TM1637Data.show_clock = false;
        result = TM1637MainFunc(function);
      }
      break;
    case FUNC_DISPLAY_DIM:
      TM1637Dim();
      break;
    case FUNC_DISPLAY_POWER:
      if (!disp_power)
      {
        TM1637ClearDisplay();
      }
      break;
    }
  }
  return result;
}

#endif // USE_DISPLAY_TM1637
#endif // USE_DISPLAY
