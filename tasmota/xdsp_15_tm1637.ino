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
  on the inexpensive TM1637- and TM1638-based seven-segment modules. Additionally, for the TM1638, 
  it allows the TM1638 LEDs to be toggled using its buttons. 
  Useful STAT messages are also sent when the TM1638 buttons are pressed.
  
  Raw segments can also be displayed.
  
  In addition, it is also possible to set brightness (8 levels), clear the display, scroll text,  
  display a rudimentary bar graph, and a Clock (12 hr and 24 hr).

  To use, compile Tasmota with USE_DISPLAY and USE_DISPLAY_TM1637, or build the tasmota-display env.

  Connect the TM1637 or TM1638 display module's pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "SSPI MOSI"
  CLK hardware pin --> "SSPI SCLK"
  STB hardware pin --> "SSPI MISO" (Only for TM1638)

  
  Once the GPIO configuration is saved and the ESP8266/ESP32 module restarts, set the Display Model to 15 
  using the command "DisplayModel 15"
  
  After this, depending upon your display variant, set your Display Type to 0, 1 or 2 using the command 
     "DisplayType 0"  // for the 4-digit TM1637
  or "DisplayType 1"  // for the 6-digit TM1637
  or "DisplayType 2"  // for the 8-digit TM1638
  
  After the ESP8266/ESP32 module restarts again, the following "Display" commands can be used:
  

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



  DisplayBrightness     num {1-8}

                               Set brightness (1 to 8) command e.g., "DisplayBrightness 2"



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

                                Causes the current state of the buttons/LEDs to be returned as a "STAT" message of the following form:
                                stat/TM1638/RESULT = {"DisplayButtons": {"S1":<s1>, "S2":<s2>, "S3":<s3>, "S4":<s4>, "S5":<s5>, "S6":<s6>, "S7":<s7>, "S8":<s8>, "Array": [<s1>,<s2>,<s3>,<s4>,<s5>,<s6>,<s7>,<s8>], "Byte": <byte>}}
                                where <s1>, <s2>, ... are the states of each of the buttons/LEDs, and <byte> is the decimal representation 
                                of the bit-array that constitutes these states. 

                                For example, if the 5th button is pressed to turn ON LED5, and the "DisplayButtons" command is issued, 
                                the response STAT message will be of the form:
                                stat/TM1638/RESULT = {"DisplayButtons": {"S1":0, "S2":0, "S3":0, "S4":0, "S5":1, "S6":0, "S7":0, "S8":0, "Array": [0,0,0,0,1,0,0,0], "Byte": 16}}
                                because 16 => 2^(5-1)



  Button Functionality (TM1638 only):
======================================
  When this driver is initialized with "DisplayType 2" (TM1638), the buttons on the TM1638 module can be used 
  to toggle the corresponding LEDs.

  In addition, for each button press, a STAT message of the following form is sent:

  stat/<Topic>/RESULT = {"TM1638_BUTTON<x>":<state>}

      where    <x> = button index (starting at 1)
           <state> = state of the corresponding LED. It can be "ON" or "OFF" 

  For example, if Button3 is pressed, LED3 would light up and a STAT message is sent as follows:
  
  stat/TM1638/RESULT = {"TM1638_BUTTON3":"ON"}

  Pressing Button3 again would turn OFF LED3 and a STAT message is sent as follows:

  stat/TM1638/RESULT = {"TM1638_BUTTON3":"OFF"}


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
bool LED[8] = {false, false, false, false, false, false, false, false};
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

#define BRIGHTNESS_MIN    1
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
    Response_P(PSTR("{\"Error\":\"Text too long. Length should be less than %d\"}"), SCROLL_MAX_LEN);
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
    Response_P(PSTR("{\"Error\":\"Level should be a number in the range [%d, %d]\"}"), LEVEL_MIN, LEVEL_MAX);
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
    uint8_t value = (((i%2) == 0) ? 54 : 48);
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
* Command:  DisplayBrightness {1-8}
\*********************************************************************************************/
bool CmndBrightness(void) {

  uint16_t val = XdrvMailbox.payload;
  if(ArgC() == 0) {
    XdrvMailbox.payload = brightness;
    return true;
  }

  if((val < BRIGHTNESS_MIN) || (val > BRIGHTNESS_MAX)) {
    Response_P(PSTR("{\"Error\":\"Brightness should be a number in the range [%d, %d]\"}"), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
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
  else if(displaytype == TM1638) disp38->brightness(val-1);  
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
    Response_P(PSTR("{\"Error\":\"Command not valid for DisplayType %d\"}"), displaytype);
    return false;     
  }
  if(ArgC() == 0) XdrvMailbox.payload = 0;
  uint16_t val = XdrvMailbox.payload; 
  if((val < LED_MIN) || (val > LED_MAX)) {
    Response_P(PSTR("{\"Error\":\"Set LEDs value should be a number in the range [%d, %d]\"}"), LED_MIN, LED_MAX);
    return false;   
  }
  for(uint8_t i=0; i<8; i++) LED[i] = ((val & 2^i) >> i);
  disp38->setLEDs(val << 8);
  return true;
}


/*********************************************************************************************\
* Sets an LED at specified position. 
* Command:  DisplaySetLED position {0-7}, value {0|1}
\*********************************************************************************************/
bool CmndSetLED(void) {
  if(displaytype != TM1638) {
    Response_P(PSTR("{\"Error\":\"Command not valid for DisplayType %d\"}"), displaytype);
    return false;     
  }   
  if(ArgC() < 2) {
    Response_P(PSTR("{\"Error\":\"Set LED requires two comma-separated numbers as arguments\"}"));
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
    Response_P(PSTR("{\"Error\":\"First argument, position should be in the range [%d, %d]\"}"), POSITION_MIN, POSITION_MAX);
    return false;   
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: position=%d"), position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: value=%d"), value);

  LED[position] = value;
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
      for(int i=0; i<8; i++) {
        if(buttons & (1<<i)) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: button pressed: %d"), i+1);
          LED[i] = !LED[i];
          disp38->setLED(i, LED[i]);
          Response_P(PSTR("{\"TM1638_BUTTON%d\":\"%s\"}"), i+1, (LED[i]?PSTR("ON"):PSTR("OFF")));
          MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR("BUTTONS"));
        }
      }
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
    Response_P(PSTR("{\"Error\":\"Command not valid for DisplayType %d\"}"), displaytype); 
    return false;     
  }   
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1638: buttons=%d"), buttons);
  uint8_t byte = LED[0]<<0 | LED[1]<<1 | LED[2]<<2 | LED[3]<<3 | LED[4]<<4 | LED[5]<<5 | LED[6]<<6 | LED[7]<<7; 
  Response_P(PSTR("{\"DisplayButtons\": {\"S1\":%d, \"S2\":%d, \"S3\":%d, \"S4\":%d, \"S5\":%d, \"S6\":%d, \"S7\":%d, \"S8\":%d, \"Array\": [%d,%d,%d,%d,%d,%d,%d,%d], \"Byte\": %d}}"), 
              LED[0], LED[1], LED[2], LED[3], LED[4], LED[5], LED[6], LED[7], LED[0], LED[1], LED[2], LED[3], LED[4], LED[5], LED[6], LED[7], byte); 
  return true;
}





/*********************************************************************************************\
* This function is called for all Display functions.
\*********************************************************************************************/
bool MainFunc(uint8_t fn) {
  bool result = false;

  if(XdrvMailbox.data_len > CMD_MAX_LEN) {
    Response_P(PSTR("{\"Error\":\"Command text too long. Please limit it to %d characters\"}"), CMD_MAX_LEN); 
    return false;   
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
      case FUNC_DISPLAY_RAW:
      case FUNC_DISPLAY_LEVEL:
      case FUNC_DISPLAY_SEVENSEG_TEXTNC:
      case FUNC_DISPLAY_SCROLLTEXT:
      case FUNC_DISPLAY_SCROLLDELAY:
      case FUNC_DISPLAY_CLOCK:
        showClock = false;
      case FUNC_DISPLAY_BRIGHTNESS:
      case FUNC_DISPLAY_SETLEDS:
      case FUNC_DISPLAY_SETLED:
      case FUNC_DISPLAY_BUTTONS:
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
