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
#ifdef USE_DISPLAY_TM1637
/*********************************************************************************************\
  This driver enables the display of numbers (both integers and floats) and basic text
  on the inexpensive TM1637- and TM1638-based seven-segment modules.
  
  Raw segments can also be displayed.
  
  In addition, it is also possible to set brightness (8 levels), clear the display, scroll text,  
  display a rudimentary bar graph, and a Clock (12 hr and 24 hr).

  To use, compile Tasmota with USE_DISPLAY and USE_DISPLAY_TM1637, or build the tasmota-display env.

  Connect the TM1637 or TM1638 display module's pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "SSPI MOSI"
  CLK hardware pin --> "SSPI SCLK"
  STB hardware pin --> "SSPI MISO" (Only for TM1638)


  Once the device restarts the following "Display" commands should become available:
  
  In case you get a stat/<TOPIC>/RESULT = {"Command":"Unknown"} result for any of these commands,
  please ensure that your DisplayModel is set to 15 using the command "DisplayModel 15"


  DisplayType           type {0|1|2}

                               Sets the display type. 0 => TM1637, 4 digit
                                                      1 => TM1637, 6 digit
                                                      2 => TM1638, 8 digit
                               Command e.g., "DisplayType 1"   // to enable TM1637 6-digit variant
  


  DisplayClear

                               Clears the display, command: "DisplayClear"



  DisplayNumber         num [,position {0-(NUM_DIGITS-1))} [,leading_zeros {0|1} [,length {1 to NUM_DIGITS}]]]

                               Clears and then displays number without decimal. command e.g., "DisplayNumber 1234"
                               Control 'leading zeros', 'length' and 'position' with  "DisplayNumber 1234, <position>, <leadingZeros>, <length>"
                               'leading zeros' can be 1 or 0 (default), 'length' can be 1 to NUM_DIGITS, 'position' can be 0 (left-most) to NUM_DIGITS (right-most).
                               See function description below for more details.



  DisplayNumberNC       num [,position {0-(NUM_DIGITS-1))} [,leading_zeros {0|1} [,length {1 to NUM_DIGITS}]]]

                               Display integer number as above, but without clearing first. e.g., "DisplayNumberNC 1234". Usage is same as above.



  DisplayFloat          num [,position {0-(NUM_DIGITS-1)} [,precision {0-NUM_DIGITS} [,length {1 to NUM_DIGITS}]]]

                               Clears and then displays float (with decimal point)  command e.g., "DisplayFloat 12.34"
                               See function description below for more details.



  DisplayFloatNC        num [,position {0-(NUM_DIGITS-1)} [,precision {0-NUM_DIGITS} [,length {1 to NUM_DIGITS}]]]

                               Displays float (with decimal point) as above, but without clearing first. command e.g., "DisplayFloatNC 12.34"
                               See function description below for more details.



  DisplayBrightness     num {0-8}

                               Set brightness (0 (off) to 8) command e.g., "DisplayBrightness 2"



  DisplayRaw            position {0-(NUM_DIGITS-1)},length {1 to NUM_DIGITS}, num1 [, num2[, num3[, num4[, ...upto NUM_DIGITS numbers]]]]]

                               Takes upto NUM_DIGITS comma-separated integers (0-255) and displays raw segments. Each number represents a
                               7-segment digit. Each 8-bit number represents individual segments of a digit.
                               For example, the command "DisplayRaw 0, 4, 255, 255, 255, 255" would display "[8.8.8.8.]"



  DisplayText           text [, position {0-(NUM_DIGITS-1)} [,length {1 to NUM_DIGITS}]]

                               Clears and then displays basic text.  command e.g., "DisplayText ajith vasudevan"
                               Control 'length' and 'position' with  "DisplayText <text>, <position>, <length>"
                               'length' can be 1 to NUM_DIGITS, 'position' can be 0 (left-most) to NUM_DIGITS-1 (right-most)

                               A caret(^) symbol in the text input is dispayed as the degrees(°) symbol. This is useful for displaying Temperature!
                               For example, the command "DisplayText 22.5^" will display "22.5°".



  DisplayTextNC         text [, position {0-NUM_DIGITS-1} [,length {1 to NUM_DIGITS}]]

                               Clears first, then displays text. Usage is same as above.



  DisplayScrollText     text

                              Displays scrolling text.



  DisplayScrollDelay delay {0-15}   // default = 4

                               Sets the speed of text scroll. Smaller delay = faster scrolling.



  DisplayLevel          num {0-100}

                               Display a horizontal bar graph (0-100) command e.g., "DisplayLevel 50" will display [||||    ]



  DisplayClock  1|2|0

                               Displays a clock.
                               Commands "DisplayClock 1"     // 12 hr format
                                        "DisplayClock 2"     // 24 hr format
                                        "DisplayClock 0"     // turn off clock



Commands specific to TM1638
====================================

  DisplaySetLEDs        bit_array {0-255} 

                               Sets the 8 LEDs (not the digits!) on the TM1638 module to the binary number represented by the input bit_array.
                               For example, "DisplaySetLEDs 3" would light up the first and second LED (from left), because 3 => 00000011



  DisplaySetLED         position {0-7}, value {0|1}

                                Sets a specified LED to either ON or OFF.  e.g., "DisplaySetLED 2, 1" would light up the 3rd LED (2 => 3rd position)



  DisplayButtons

                                Causes the current state of the buttons to be returned as a "STAT" message of the form stat/TM1638/RESULT = {"DisplayButtons": <buttonvalue>}
                                The button value is the decimal representation of the bit-array that constitutes the button states. For example, if the 5th button is pressed and the 
                                DisplayButtons command is issued, the response will be stat/TM1638/RESULT = {"DisplayButtons": 16}  because 16 => 2^(5-1)
                                if the 2nd and 3rd buttons are pressed together and the DisplayButtons command is issued, the response will be 
                                stat/TM1638/RESULT = {"DisplayButtons": 6}  because 6 => 2^(2-1) + 2^(3-1)



  Button Functionality (TM1638 only):
======================================
  When this driver is initialized with "DisplayType 2" (TM1638), the buttons on the TM1638 module can be used 
  to toggle the corresponding LEDs.

  In addition, if SwitchTopic is set to some value, then for each button press, a TOGGLE for that switch number is sent.

  For example, if Topic = "TM1638" and SwitchTopic = "TEST_TM1638", and if S3 is pressed, then,

      1) a STAT message is sent  : stat/TM1638/RESULT = {"TM1638_BUTTONS":4}    // (4 = 2^(3-1))
  and 2) a TOGGLE command is sent: cmnd/TEST/POWER3 = TOGGLE


\*********************************************************************************************/

#define XDSP_15                    15

#include "SevenSegmentTM1637.h"
#include <TM1638plus.h>

SevenSegmentTM1637 *disp37;
TM1638plus *disp38;
bool driverinited = false;
bool showClock = false;
bool clock24 = false;
char tm[5];
char msg[60];
uint8_t buttons;
uint8_t prevbuttons;
uint32_t NUM_DIGITS = 4;
uint32_t prev_num_digits = 4;
bool scroll = false;
uint32_t scrolldelay = 4;
uint32_t scrollindex = 0;
uint32_t iteration = 0;
uint32_t brightness = 5;
char modelname[8];
enum displaytypes { TM1637, TM1638 };
uint8_t displaytype = TM1637;
bool modechanged = false;

#define BRIGHTNESS_MIN    0   // Display OFF
#define BRIGHTNESS_MAX    8
#define CMD_MAX_LEN   55
#define LEVEL_MIN    0
#define LEVEL_MAX    100
#define SCROLL_MAX_LEN   50
#define POSITION_MIN    0
#define POSITION_MAX    8
#define LED_MIN      0
#define LED_MAX      255
char scrolltext[CMD_MAX_LEN];

/*********************************************************************************************\
* Init function
\*********************************************************************************************/
bool DriverInit(void) {
  if(Settings.display_model == XDSP_15) {
    if(driverinited) return true;

    if(Settings.display_type == 2)      { NUM_DIGITS = 8; displaytype = TM1638; }
    else if(Settings.display_type == 1) { NUM_DIGITS = 6; displaytype = TM1637; }
    else                                { Settings.display_type = 0; NUM_DIGITS = 4; displaytype = TM1637; }

    if(displaytype == TM1637) {
      strcpy(modelname, "TM1637");
      disp37 = new SevenSegmentTM1637(Pin(GPIO_SSPI_SCLK), Pin(GPIO_SSPI_MOSI) );
      disp37->begin(NUM_DIGITS, 1);
    } else if(displaytype == TM1638) {
      strcpy(modelname, "TM1638");
      disp38 = new TM1638plus(Pin(GPIO_SSPI_MISO), Pin(GPIO_SSPI_SCLK), Pin(GPIO_SSPI_MOSI), true );
      NUM_DIGITS = 8;
      disp38->displayBegin();
    }
    Settings.display_size = NUM_DIGITS;   // Can use to check current display size
    clearDisplay();
    brightness = (Settings.display_dimmer ? Settings.display_dimmer : brightness);
    setBrightness(brightness);
    driverinited = true;
    modechanged = false;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: %s display driver initialized with %d digits (DisplayType %d)"), modelname, NUM_DIGITS, Settings.display_type);    
  }

  return true;
}


/*********************************************************************************************\
* Displays number without decimal, with/without leading zeros, specifying start-position
* and length, optionally skipping clearing display before displaying the number.
* commands: DisplayNumber   num [,position {0-(NUM_DIGITS-1)} [,leading_zeros {0|1} [,length {1 to NUM_DIGITS}]]]
*           DisplayNumberNC num [,position {0-(NUM_DIGITS-1)} [,leading_zeros {0|1} [,length {1 to NUM_DIGITS}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndNumber(bool clear) {
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
    case 4 :
      subStr(sLength, XdrvMailbox.data, ",", 4);
      length = atoi(sLength);
    case 3 :
      subStr(sLeadingzeros, XdrvMailbox.data, ",", 3);
      leadingzeros = atoi(sLeadingzeros);
    case 2 :
      subStr(sPosition, XdrvMailbox.data, ",", 2);
      position = atoi(sPosition);
    case 1 :
      subStr(sNum, XdrvMailbox.data, ",", 1);
      num = atof(sNum);
  }


  if((position < 0) || (position > (NUM_DIGITS-1))) position = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: num=%d"), modelname, num);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: position=%d"), modelname, position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: leadingzeros=%d"), modelname, leadingzeros);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: length=%d"), modelname, length);

  if(clear) clearDisplay();

  char txt[30];
  snprintf_P(txt, sizeof(txt), PSTR("%d"), num);
  if(!length) length = strlen(txt);
  if((length < 0) || (length > NUM_DIGITS)) length = NUM_DIGITS;

  char pad = (leadingzeros ? '0': ' ');
  uint32_t i = position;
  uint8_t rawBytes[1];

  for(; i<position + (length - strlen(txt)); i++) {
    if(i>NUM_DIGITS) break;
    if(displaytype == TM1637) { rawBytes[0] = disp37->encode(pad); disp37->printRaw(rawBytes, 1, i); }
    else if(displaytype == TM1638) disp38->displayASCII(i, pad);
  }

  for(uint32_t j = 0; i< position + length; i++, j++) {
    if(i>NUM_DIGITS) break;
    if(txt[j] == 0) break;
    if(displaytype == TM1637) { rawBytes[0] = disp37->encode(txt[j]);  disp37->printRaw(rawBytes, 1, i); }
    else if(displaytype == TM1638) disp38->displayASCII(i, txt[j]);
  }

  return true;
}



/*********************************************************************************************\
* Displays number with decimal, specifying position, precision and length,
* optionally skipping clearing display before displaying the number.
* commands: DisplayFloat   num [,position {0-(NUM_DIGITS-1)} [,precision {0-NUM_DIGITS} [,length {1 to NUM_DIGITS}]]]
*           DisplayFloatNC num [,position {0-(NUM_DIGITS-1)} [,precision {0-NUM_DIGITS} [,length {1 to NUM_DIGITS}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndFloat(bool clear) {

  char sNum[CMD_MAX_LEN];
  char sPrecision[CMD_MAX_LEN];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  uint8_t length = 0;
  uint8_t precision = NUM_DIGITS;
  uint8_t position = 0;

  float fnum = 0.0f;

  switch (ArgC())
  {
    case 4 :
      subStr(sLength, XdrvMailbox.data, ",", 4);
      length = atoi(sLength);
    case 3 :
      subStr(sPrecision, XdrvMailbox.data, ",", 3);
      precision = atoi(sPrecision);
    case 2 :
      subStr(sPosition, XdrvMailbox.data, ",", 2);
      position = atoi(sPosition);
    case 1 :
      subStr(sNum, XdrvMailbox.data, ",", 1);
      fnum = atof(sNum);
  }


  if((position < 0) || (position > (NUM_DIGITS-1))) position = 0;
  if((precision < 0) || (precision > NUM_DIGITS)) precision = NUM_DIGITS;

  if(clear) clearDisplay();

  char txt[30];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), precision, &fnum);

  if(!length) length = strlen(txt);
  if((length <= 0) || (length > NUM_DIGITS)) length = NUM_DIGITS;

  char s[30];
  ext_snprintf_P(s, sizeof(s), PSTR("LOG: %s: num=%4_f"), modelname, &fnum);
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s"), s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: precision=%d"), modelname, precision);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: length=%d"), modelname, length);

  if(displaytype == TM1637) {
    uint8_t rawBytes[1];
    for(uint32_t i=0, j=0; i<length; i++, j++) {
      if(txt[i] == 0) break;
      rawBytes[0] = disp37->encode(txt[i]);
      if(txt[i+1] == '.') {
        rawBytes[0] = rawBytes[0] | 128;
        i++;
        length++;
      }
      if((j+position) > NUM_DIGITS) break;
      disp37->printRaw(rawBytes, 1, j+position);
    }
  } else if(displaytype == TM1638) {
    for(uint32_t i=0, j=0; i<length; i++, j++) {
      if((j+position) > 7) break;
      if(txt[i] == 0) break;
      if(txt[i+1] == '.') { 
        disp38->displayASCIIwDot(j+position, txt[i]);
        i++;
        length++;
      }
      else disp38->displayASCII(j+position, txt[i]);
    }
  }

  return true;
}


// /*********************************************************************************************\
// * Clears the display
// * Command:  DisplayClear
// \*********************************************************************************************/
bool CmndClear(void) {
  clearDisplay();
  sprintf(msg, PSTR("Cleared"));
  XdrvMailbox.data = msg;
  return true;
}


// /*********************************************************************************************\
// * Clears the display
// \*********************************************************************************************/
void clearDisplay (void) {
  if(displaytype == TM1637) {
    unsigned char arr[] =  {0};
    for(int i=0; i<NUM_DIGITS; i++) disp37->printRaw(arr, 1, i);
  } else if(displaytype == TM1638) {
    for(int i=0; i<NUM_DIGITS; i++) disp38->display7Seg(i, 0);
  }
}


/*********************************************************************************************\
* Display scrolling text
* Command:   DisplayScrollText text
\*********************************************************************************************/
bool CmndScrollText(void) {

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: text=%s"), modelname, XdrvMailbox.data);

  if(XdrvMailbox.data_len > SCROLL_MAX_LEN) {
    snprintf(msg, sizeof(msg), PSTR("Text too long. Length should be less than %d"), SCROLL_MAX_LEN);
    XdrvMailbox.data = msg;
    return false;
  } else {
    snprintf(scrolltext, sizeof(scrolltext), PSTR("%s"), XdrvMailbox.data);
    scrolltext[XdrvMailbox.data_len] = 0;
    scrollindex = 0;
    scroll = true;
    return true;
  }

}



/*********************************************************************************************\
* Sets the scroll delay for scrolling text.
* Command:  DisplayScrollDelay delay {0-15}    // default = 4
\*********************************************************************************************/
bool CmndScrollDelay(void) {
  if(scrolldelay<0) scrolldelay=0;
  scrolldelay = XdrvMailbox.payload;
  return true;
}



/*********************************************************************************************\
* Scrolls a given string. Called every 50ms
\*********************************************************************************************/
void scrollText(void) {
  iteration++;
  if(scrolldelay) iteration = iteration %  scrolldelay;
  else iteration = 0;
  if(iteration) return;

  if(scrollindex > strlen(scrolltext)) {
    scroll = false;
    scrollindex = 0;
    return;
  }
  bool clr = false;
  uint8_t rawBytes[1];
  for(uint32_t i=0, j=scrollindex; i< strlen(scrolltext); i++, j++) {
    if(i > (NUM_DIGITS-1)) break;
    if(scrolltext[j] == 0) {clr = true;};
    if(displaytype == TM1637) {
      char charToDisp = (clr ? ' ' : scrolltext[j]);
      rawBytes[0] = disp37->encode(charToDisp);
      disp37->printRaw(rawBytes, 1, i);
    } else if(displaytype == TM1638) {
      disp38->displayASCII(i, (clr ? ' ' : scrolltext[j]));
    }

  }
  scrollindex++;
}






/*********************************************************************************************\
* Displays a horizontal bar graph. Takes a percentage number (0-100) as input
* Command:   DisplayLevel level {0-100}
\*********************************************************************************************/
bool CmndLevel(void) {
  uint16_t val = XdrvMailbox.payload;
  if((val < LEVEL_MIN) || (val > LEVEL_MAX)) {
    sprintf(msg, PSTR("Level should be a number in the range [%d, %d]"), LEVEL_MIN, LEVEL_MAX);
    XdrvMailbox.data = msg;
    return false;
  }

  uint8_t totalBars = 2*NUM_DIGITS;
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: CmndLevel totalBars=%d"), modelname, totalBars);
  float barsToDisplay = totalBars * val / 100.0f;
  char txt[5];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), 1, &barsToDisplay);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: CmndLevel barsToDisplay=%s"), modelname, txt);
  char s[4];
  ext_snprintf_P(s, sizeof(s), PSTR("%*_f"), 0, &barsToDisplay);
  uint8_t numBars = atoi(s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: CmndLevel numBars=%d"), modelname, numBars);

  clearDisplay();
  uint8_t rawBytes[1];
  for(int i=1; i<=numBars; i++) {
    uint8_t digit = (i-1) / 2;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: CmndLevel digit=%d"), modelname, digit);
    uint8_t value = (((i%2) == 0) ? 54 : 48);
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: CmndLevel value=%d"), modelname, value);

    if(displaytype == TM1637) {
      rawBytes[0] = value;
      disp37->printRaw(rawBytes, 1, digit);
    } else if(displaytype == TM1638) {
      disp38->display7Seg(digit, value);
    }

  }
  return true;
}


/*********************************************************************************************\
* Display arbitrary data on the display module
* Command:   DisplayRaw position {0-(NUM_DIGITS-1)},length {1 to NUM_DIGITS}, a [, b[, c[, d[...upto NUM_DIGITS]]]]
* where a,b,c,d... are upto NUM_DIGITS numbers in the range 0-255, each number (byte)
* corresponding to a single 7-segment digit. Within each byte, bit 0 is segment A,
* bit 1 is segment B etc. The function may either set the entire display
* or any desired part using the length and position parameters.
\*********************************************************************************************/
bool CmndRaw(void) {
  uint8_t DATA[6] = { 0, 0, 0, 0, 0, 0 };

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
    case 8 :
      subStr(fs, XdrvMailbox.data, ",", 8);
      DATA[5] = atoi(fs);
    case 7 :
      subStr(es, XdrvMailbox.data, ",", 7);
      DATA[4] = atoi(es);
    case 6 :
      subStr(ds, XdrvMailbox.data, ",", 6);
      DATA[3] = atoi(ds);
    case 5 :
      subStr(cs, XdrvMailbox.data, ",", 5);
      DATA[2] = atoi(cs);
    case 4 :
      subStr(bs, XdrvMailbox.data, ",", 4);
      DATA[1] = atoi(bs);
    case 3 :
      subStr(as, XdrvMailbox.data, ",", 3);
      DATA[0] = atoi(as);
    case 2 :
      subStr(sLength, XdrvMailbox.data, ",", 2);
      length = atoi(sLength);
    case 1 :
      subStr(sPos, XdrvMailbox.data, ",", 1);
      position = atoi(sPos);
  }

  if(!length) length = ArgC() - 2;
  if(length < 0 || length > NUM_DIGITS) length = NUM_DIGITS;
  if(position < 0 || position > (NUM_DIGITS-1)) position = 0;


  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: a=%d"), modelname, DATA[0]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: b=%d"), modelname, DATA[1]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: c=%d"), modelname, DATA[2]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: d=%d"), modelname, DATA[3]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: e=%d"), modelname, DATA[4]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: f=%d"), modelname, DATA[5]);

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: length=%d"), modelname, length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: position=%d"), modelname, position);


  if(displaytype == TM1637) {
    uint8_t rawBytes[1];
    for(uint32_t i=position; i<position+length; i++ ) {
      if(i>(NUM_DIGITS-1)) break;
      rawBytes[0] = DATA[i-position];
      disp37->printRaw(rawBytes, 1, i);
    }    
  } else if(displaytype == TM1638) {
    for(uint32_t i=position; i<position+length; i++ ) {
      if(i>7) break;
      disp38->display7Seg(i, DATA[i-position]);
    }
  }


  return true;
}



/*********************************************************************************************\
* Display a given string.
* Text can be placed at arbitrary location on the display using the length and
* position parameters without affecting the rest of the display.
* Command:   DisplayText text [, position {0-(NUM_DIGITS-1)} [,length {1 to NUM_DIGITS}]]
\*********************************************************************************************/
bool CmndText(bool clear) {
  char sString[CMD_MAX_LEN + 1];
  char sPosition[CMD_MAX_LEN];
  char sLength[CMD_MAX_LEN];
  uint8_t length = 0;
  uint8_t position = 0;

  switch (ArgC())
  {
    case 3 :
      subStr(sLength, XdrvMailbox.data, ",", 3);
      length = atoi(sLength);
    case 2 :
      subStr(sPosition, XdrvMailbox.data, ",", 2);
      position = atoi(sPosition);
    case 1 :
      subStr(sString, XdrvMailbox.data, ",", 1);
  }


  if((position < 0) || (position > (NUM_DIGITS-1))) position = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: sString=%s"), modelname, sString);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: position=%d"), modelname, position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: length=%d"), modelname, length);

  if(clear) clearDisplay();

  if(!length) length = strlen(sString);
  if((length < 0) || (length > NUM_DIGITS)) length = NUM_DIGITS;

  uint32_t i = position;
  if(displaytype == TM1637) {
    uint8_t rawBytes[1];
    for(uint32_t j = 0; i< position + length; i++, j++) {
      if(i > (NUM_DIGITS-1)) break;
      if(sString[j] == 0) break;
      rawBytes[0] = disp37->encode(sString[j]);
      if(sString[j+1] == '.') {
        rawBytes[0] = rawBytes[0] | 128;
        j++;
      } else if(sString[j] == '^') {
        rawBytes[0] = 1 | 2 | 32 | 64;
      }
      disp37->printRaw(rawBytes, 1, i);
    }
  } else if(displaytype == TM1638) {
    for(uint32_t j = 0; i< position + length; i++, j++) {
      if(i > 7) break;
      if(sString[j] == 0) break;
      if(sString[j+1] == '.') {
        disp38->displayASCIIwDot(i, sString[j]);
        j++;
      } else if(sString[j] == '^') {
        disp38->display7Seg(i, (1 | 2 | 32 | 64));
      } else disp38->displayASCII(i, sString[j]);
    }      
  }

  return true;
}


/*********************************************************************************************\
* Sets brightness of the display.
* Command:  DisplayBrightness {0-8}    // 0 => off
\*********************************************************************************************/
bool CmndBrightness(void) {

  uint16_t val = XdrvMailbox.payload;
  if(ArgC() == 0) {
    XdrvMailbox.payload = brightness;
    return true;
  }

  if((val < BRIGHTNESS_MIN) || (val > BRIGHTNESS_MAX)) {
    sprintf(msg, PSTR("Brightness should be a number in the range [%d, %d]"), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    XdrvMailbox.data = msg;
    return false;
  }
  brightness = val;
  setBrightness(brightness);
  return true;
}



void setBrightness(uint8_t val) {
  if((val < BRIGHTNESS_MIN) || (val > BRIGHTNESS_MAX)) val = 5;
  Settings.display_dimmer = val;
  if(displaytype == TM1637)  disp37->setBacklight(val*10);
  else if(displaytype == TM1638) disp38->brightness(val);  
}





/*********************************************************************************************\
* Displays a clock.
* Command: DisplayClock 1   // 12-hour format
*          DisplayClock 2   // 24-hour format
*          DisplayClock 0   // turn off clock and clear
\*********************************************************************************************/
bool CmndClock(void) {

  showClock = XdrvMailbox.payload;

  if(ArgC() == 0) XdrvMailbox.payload = 1;
  if(XdrvMailbox.payload > 1) clock24 = true;
  else if(XdrvMailbox.payload == 1) clock24 = false;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: showClock=%d"), modelname, showClock);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: clock24=%d"), modelname, clock24);

  if(!showClock) {
    clearDisplay();
  }
  return true;
}


/*********************************************************************************************\
* refreshes the time if clock is displayed
\*********************************************************************************************/
void showTime() {
  uint8_t hr = RtcTime.hour;
  uint8_t mn = RtcTime.minute;
  // uint8_t hr = 1;
  // uint8_t mn = 0;
  char z = ' ';
  if(clock24) {
    z = '0';
  } else {
    if(hr > 12) hr -= 12;
    if(hr == 0) hr = 12;
  }

  if(hr < 10) {
    if(mn < 10) snprintf(tm, sizeof(tm), PSTR("%c%d0%d"), z, hr, mn);
    else snprintf(tm, sizeof(tm), PSTR("%c%d%d"), z, hr, mn);
  } else {
    if(mn < 10) snprintf(tm, sizeof(tm), PSTR("%d0%d"), hr, mn);
    else snprintf(tm, sizeof(tm), PSTR("%d%d"), hr, mn);
  }

  if(displaytype == TM1637) {
    uint8_t rawBytes[1];
    for(uint32_t i = 0; i< 4; i++) {
      rawBytes[0] = disp37->encode(tm[i]);
      if((millis() % 1000) > 500 && (i == 1)) rawBytes[0] = rawBytes[0] | 128;
      disp37->printRaw(rawBytes, 1, i);
    }
  } else if(displaytype == TM1638) {
    for(uint32_t i = 0; i< 4; i++) {
      if((millis() % 1000) > 500 && (i == 1)) disp38->displayASCIIwDot(i, tm[i]);
      else disp38->displayASCII(i, tm[i]);
    }
  }

}




/*********************************************************************************************\
* Sets all LEDs of the display. 
* Command:  DisplaySetLEDs {0-255}
\*********************************************************************************************/
bool CmndSetLEDs(void) {
  if(displaytype != TM1638) {
    sprintf(msg, PSTR("Command not valid for DisplayType %d"), displaytype); 
    XdrvMailbox.data = msg;
    return false;     
  }
  if(ArgC() == 0) XdrvMailbox.payload = 0;
  uint16_t val = XdrvMailbox.payload; 
  if((val < LED_MIN) || (val > LED_MAX)) {
    sprintf(msg, PSTR("Set LEDs value should be a number in the range [%d, %d]"), LED_MIN, LED_MAX);
    XdrvMailbox.data = msg;
    return false;     
  } 
  disp38->setLEDs(val << 8);
  return true;
}


/*********************************************************************************************\
* Sets an LED at specified position. 
* Command:  DisplaySetLED position {0-7}, value {0|1}
\*********************************************************************************************/
bool CmndSetLED(void) {
  if(displaytype != TM1638) {
    sprintf(msg, PSTR("Command not valid for DisplayType %d"), displaytype); 
    XdrvMailbox.data = msg;
    return false;     
  }   
  if(ArgC() < 2) {
    sprintf(msg, PSTR("Set LED requires two comma-separated numbers as arguments"));
    XdrvMailbox.data = msg;
    return false;     
  } 

  char sVal[CMD_MAX_LEN]; 
  char sPos[CMD_MAX_LEN];
  uint32_t position = 0;
  uint32_t value = 0;

  switch (ArgC()) 
  {
     case 2 :
      subStr(sVal, XdrvMailbox.data, ",", 2);
      value = atoi(sVal);
    case 1 :
      subStr(sPos, XdrvMailbox.data, ",", 1);
      position = atoi(sPos);
  }

 
  if((position < POSITION_MIN) || (position > POSITION_MAX)) {
    sprintf(msg, PSTR("First argument, position should be in the range [%d, %d]"), POSITION_MIN, POSITION_MAX);
    XdrvMailbox.data = msg;
    return false;        
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: position=%d"), position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: value=%d"), value);

  disp38->setLED(position, value);
  return true;
}


/*********************************************************************************************\
* Reads the button states. Called every 50ms
\*********************************************************************************************/
bool readButtons(void) {
  buttons = disp38->readButtons();
  if(prevbuttons != buttons) {
    prevbuttons = buttons;
    if(!buttons) return true;
    if(buttons) {
      Response_P(PSTR("{\"TM1638_BUTTONS\":%d}"), buttons);
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR("BUTTONS"));
      AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: buttons changed: %d"), buttons);
      for(int i=0; i<8; i++) {
        if(buttons & (1<<i)) { 
          if(SwitchGetVirtual(i)) SwitchSetVirtual(i, 0); 
          else SwitchSetVirtual(i, 1); 
          disp38->setLED(i, (1 + SwitchGetVirtual(i)) % 2 );
        }
      }
     
      SwitchHandler(1);
    }
  }
  return true;
}

/*********************************************************************************************\
* Returns the current state of the buttons as a decimal representation of the button states
* Command: DisplayButtons
\*********************************************************************************************/
bool CmndButtons(void) {
  if(displaytype != TM1638) {
    sprintf(msg, PSTR("Command not valid for DisplayType %d"), displaytype); 
    XdrvMailbox.data = msg;
    return false;     
  }   
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: buttons=%d"), buttons);
  XdrvMailbox.payload = buttons;
  return true;
}





/*********************************************************************************************\
* This function is called for all Display functions.
\*********************************************************************************************/
bool MainFunc(uint8_t fn) {
  bool result = false;

  if(XdrvMailbox.data_len > CMD_MAX_LEN) {
    sprintf(msg, PSTR("Command text too long. Please limit it to %d characters"), CMD_MAX_LEN);
    XdrvMailbox.data = msg;
    return result;
  }

  switch (fn) {
    case FUNC_DISPLAY_CLEAR:
      result = CmndClear();
      break;
    case FUNC_DISPLAY_NUMBER :
      result = CmndNumber(true);
      break;
    case FUNC_DISPLAY_NUMBERNC :
      result = CmndNumber(false);
      break;
    case FUNC_DISPLAY_FLOAT :
      result = CmndFloat(true);
      break;
    case FUNC_DISPLAY_FLOATNC :
      result = CmndFloat(false);
      break;
    case FUNC_DISPLAY_BRIGHTNESS:
      result = CmndBrightness();
      break;
    case FUNC_DISPLAY_RAW:
      result = CmndRaw();
      break;
    case FUNC_DISPLAY_SEVENSEG_TEXT:
      result = CmndText(true);
      break;
    case FUNC_DISPLAY_SEVENSEG_TEXTNC:
      result = CmndText(false);
      break;
    case FUNC_DISPLAY_LEVEL:
      result = CmndLevel();
      break;
    case FUNC_DISPLAY_SCROLLTEXT:
      result = CmndScrollText();
      break;
    case FUNC_DISPLAY_SCROLLDELAY:
      result = CmndScrollDelay();
      break;
    case FUNC_DISPLAY_SETLEDS:
      result = CmndSetLEDs();
      break;      
    case FUNC_DISPLAY_SETLED:
      result = CmndSetLED();
      break;
    case FUNC_DISPLAY_BUTTONS:
      result = CmndButtons();
      break;   
   case FUNC_DISPLAY_CLOCK:
      result = CmndClock();
      break;
  }

  return result;
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp15(uint8_t function)
{
  bool result = false;

  if(function == FUNC_DISPLAY_MODEL) {
    return true;
  }

  if (Settings.display_model == XDSP_15) {
    switch (function) {
      case FUNC_DISPLAY_INIT_DRIVER:
        result = DriverInit();              // init 
        break;
      case FUNC_DISPLAY_INIT:
        AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: FUNC_DISPLAY_INIT: Display depends on DisplayType, currently %d"), modelname, Settings.display_type);
        result = true;  
        break;
      case FUNC_DISPLAY_TYPE:
        AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: %s: FUNC_DISPLAY_TYPE: DisplayType set to %d, restarting to take effect ..."), modelname, Settings.display_type);
        TasmotaGlobal.restart_flag = 2;
        break;
      case FUNC_DISPLAY_SEVENSEG_TEXT:
      case FUNC_DISPLAY_CLEAR:
      case FUNC_DISPLAY_NUMBER:
      case FUNC_DISPLAY_FLOAT:
      case FUNC_DISPLAY_NUMBERNC:
      case FUNC_DISPLAY_FLOATNC:
      case FUNC_DISPLAY_BRIGHTNESS:
      case FUNC_DISPLAY_RAW:
      case FUNC_DISPLAY_LEVEL:
      case FUNC_DISPLAY_SEVENSEG_TEXTNC:
      case FUNC_DISPLAY_SCROLLTEXT:
      case FUNC_DISPLAY_SCROLLDELAY:
      case FUNC_DISPLAY_SETLEDS:
      case FUNC_DISPLAY_SETLED:
      case FUNC_DISPLAY_BUTTONS:      
      case FUNC_DISPLAY_CLOCK:
        result = MainFunc(function);
        break;
      case FUNC_DISPLAY_EVERY_50_MSECOND:
        if(scroll) scrollText();
        if(showClock) showTime();
        if(displaytype == TM1638) readButtons();        
        break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_TM1637
#endif  // USE_DISPLAY
