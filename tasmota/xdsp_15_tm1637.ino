/*
  xdsp_15_tm1637.ino - Support for TM1637 seven-segment display for Tasmota

  Copyright (C) 2020  Ajith Vasudevan

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

  This driver enables the display of numbers (both integers and floats) and basic text 
  on the inexpensive TM1637-based seven-segment module. Raw segments can also be displayed.
  In addition, it is also possible to clear the display, set brightness (8 levels), display
  a rudimentary bar graph, and a Clock. 
  
  To use, compile Tasmota with USE_DISPLAY and USE_DISPLAY_TM1637, or build the tasmota-display env.

  The pins to use are "SSPI MOSI" and "SSPI SCLK".

  Connect the TM1637 display module's DIO and CLK pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "SSPI MOSI"
  CLK hardware pin --> "SSPI SCLK"

  Once the device restarts the following "Display" commands become available:

  DisplayClear
                               Blanks the display, command: "DisplayClear"

  DisplayNumber         num [,leading_zeros {0|1} [,length {1-4} [,position {0-3} [,dot {0-4} ]]]]
                               Clears and then displays integer number  (-999 to 9999). command e.g., "DisplayNumber 1234"
                               Control 'leading zeros', 'length' and 'position' with  "DisplayNumber 1234, <leadingZeros>, <length>, <position>, <dot>"
                               'leading zeros' can be 1 or 0 (default), 'length' can be 1 to 4 (default), 'position' can be 0 (left-most) to 3 (right-most),
                               'dot' can be 0,(no dot), 1 (left-most) to 4 (right-most). See function description below for more details.

  DisplayNumberNC       num [,leading_zeros {0|1} [,length {1-4} [,position {0-3} [,dot {0-4} ]]]]
                               Display integer number as above, but without clearing first. e.g., "DisplayNumberNC 1234". Usage is same as above.
                        
  DisplayFloat          num [,precision {0-4} [,length {1-4} [,position {0-3} ]]]
                               Clears and then displays float (with decimal point)  (0.001 to 9999.) command e.g., "DisplayFloat 12.34"
                               See function description below for more details.

  DisplayFloatNC        num [,precision {0-4} [,length {1-4} [,position {0-3} ]]]
                               Displays float (with decimal point) as above, but without clearing first. command e.g., "DisplayFloatNC 12.34"
                               See function description below for more details.

  DisplayBrightness     num {0-7}
                               Set brightness (1 to 7) command e.g., "DisplayBrightness 2"  Note: Brightness takes effect only after a new display command is sent.

  DisplayRaw            num1, num2, num3, num4 [,length {1-4} [,position {0-3}] ]
                               Takes 4 comma-separated integers (0-255) and displays raw segments 
                               Each 7-segment display unit is represented by an 8-bit(8th bit for decimap point) number.
                               For example, the command "DisplayRaw 255, 255, 255, 255" would display "[8.8.8.8.]"

  DisplayLevel          num {0-100}
                               Display a horizontal bar graph (0-100) command e.g., "DisplayLevel 50" will display [||||    ]  

  DisplayText           text [, length {1-4} [, position {0-3}]]
                               Clears and then displays basic text (scrolls if > 4 characters)  command e.g., "DisplayText ajith vasudevan" 
                               Control 'length' and 'position' with  "DisplayText <text>, <length>, <position>"
                               'length' can be 1 to 4 (default), 'position' can be 0 (left-most) to 3 (right-most)
                               Note: A caret sign '^' in the input text would be replaced by a "degrees" symbol. This is handy for displaying temperature!
                                     Other Characters whose ASCII > 127 or ASCII < 32 would simply be blank.

  DisplayTextNC text [, length {1-4} [, position {0-3}]]
                               Clear first, then display text. Usage is same as above.

  DisplayScrollDelay delay_in_milliseconds    // default = 200
                               Sets the speed of text scroll. Takes effect only after a new TEXT command is sent with 4 chars or more.     

  DisplayClock  1|2|0          
                               Displays a clock. 
                               Commands "DisplayClock 1"     // 12 hr format
                                        "DisplayClock 2"     // 24 hr format
                                        "DisplayClock 0"     // turn off clock



*/



#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_TM1637
/*********************************************************************************************\
 * TM1637 support
\*********************************************************************************************/

#define XDSP_15                    15
#include <TM1637TinyDisplay.h>

TM1637TinyDisplay *display;
bool showClock = false;
bool clock24 = false;
char tm[5];
char msg[60];


#define BRIGHTNESS_MIN    0
#define BRIGHTNESS_MAX    7
#define CMD_MAX_LEN   55
#define LEVEL_MIN    0
#define LEVEL_MAX    100



/*********************************************************************************************\
* Init function
\*********************************************************************************************/
bool TM1637Init(void) {
  display = new TM1637TinyDisplay(Pin(GPIO_SSPI_SCLK), Pin(GPIO_SSPI_MOSI) );
  display->setBrightness(BRIGHT_4);
  display->clear();
  if (!Settings.display_model) {
    Settings.display_model = XDSP_15;
  }  
  AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1637 display driver initialized"));
  return true;
}


/*********************************************************************************************\
* Displays number with/without decimal, with/without leading zeros, specifying length, start-position
* and dot-position, optionally skipping clearing display before displaying the number.
* commands: DisplayNumber   num [,leading_zeros {0|1} [,length {1-4} [,position {0-3} [,dot {0-4} ]]]] 
*           DisplayNumberNC num [,leading_zeros {0|1} [,length {1-4} [,position {0-3} [,dot {0-4} ]]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Number(bool clear) {
  char sNum[CMD_MAX_LEN]; 
  char sLeadingZero[CMD_MAX_LEN]; 
  char sLength[CMD_MAX_LEN]; 
  char sPos[CMD_MAX_LEN]; 
  char sDot[CMD_MAX_LEN];
  uint8_t dot = 0;
  uint8_t length = 4;
  uint8_t position = 0;
  bool leadingzeros = false;
  uint32_t num;

  switch (ArgC()) 
  {
    case 5 :
      subStr(sDot, XdrvMailbox.data, ",", 5);
      dot = atoi(sDot);
    case 4 :
      subStr(sPos, XdrvMailbox.data, ",", 4);
      position = atoi(sPos);
    case 3 :
      subStr(sLength, XdrvMailbox.data, ",", 3);
      length = atoi(sLength);
    case 2 :
      subStr(sLeadingZero, XdrvMailbox.data, ",", 2);
      leadingzeros = atoi(sLeadingZero);
    case 1 :
      subStr(sNum, XdrvMailbox.data, ",", 1);
      num = atoi(Trim(sNum));
  }
  
  if(position > 3) position = 3;
  if(position < 0) position = 0;    
  if((length <= 0) || (length > 4)) length = 4;
  if((dot < 0) || (dot > 4)) dot = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: num=%d"), num);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: leadingzeros=%d"), leadingzeros);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: position=%d"), position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: dot=%d"), dot);

  if(clear) display->clear(); 
  if(!dot) display->showNumber(num, leadingzeros, length, position);
  else display->showNumberDec(num, (1 << ( 8 - dot )), leadingzeros, length, position);
  return true;
}



/*********************************************************************************************\
* Displays number with decimal, specifying precision and length,
* optionally skipping clearing display before displaying the number.
* commands: DisplayFloat   num [,precision {0-4} [,length {1-4} [,position {0-3} ]]] 
*           DisplayFloatNC num [,precision {0-4} [,length {1-4} [,position {0-3} ]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Float(bool clear) {

  char sNum[CMD_MAX_LEN]; 
  char sPrecision[CMD_MAX_LEN]; 
  char sLength[CMD_MAX_LEN];
  uint8_t length = 4;
  uint8_t precision = 4;

  float fnum = 0.0f;

  switch (ArgC()) 
  {
     case 3 :
      subStr(sLength, XdrvMailbox.data, ",", 3);
      length = atoi(sLength);
    case 2 :
      subStr(sPrecision, XdrvMailbox.data, ",", 2);
      precision = atoi(sPrecision);
    case 1 :
      subStr(sNum, XdrvMailbox.data, ",", 1);
      fnum = atof(sNum);
  }
  
  if((precision < 0) || (precision > 4)) precision = 4;
  if((length <= 0) || (length > 4)) length = 4;

  char s[30];
  ext_snprintf_P(s, sizeof(s), PSTR("LOG: TM1637: num=%*_f"), 4, &fnum); 
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s"), s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: precision=%d"), precision);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);

  if(clear) display->clear(); 
  display->showNumber(fnum, precision, length, 0);
  return true;
}

/*********************************************************************************************\
* Clears the display
* Command:  DisplayClear
\*********************************************************************************************/
bool CmndTM1637Clear(void) {
  display->clear();
  sprintf(msg, PSTR("Cleared"));
  XdrvMailbox.data = msg;  
  return true;
}


/*********************************************************************************************\
* Displays a horizontal bar graph. Takes a percentage number (0-100) as input
* Command:   DisplayLevel level {0-100}
\*********************************************************************************************/
bool CmndTM1637Level(void) {
  uint16_t val = XdrvMailbox.payload;
  if((val < LEVEL_MIN) || (val > LEVEL_MAX)) {
    sprintf(msg, PSTR("Level should be a number in the range [%d, %d]"), LEVEL_MIN, LEVEL_MAX);
    XdrvMailbox.data = msg;    
    return false;     
  } 
  display->showLevel(val, false);
  return true;
}


/*********************************************************************************************\
* Display arbitrary data on the display module
* Command:   DisplayRaw a, b, c, d [, length {1-4} [, position {0-3}] ]
* where a,b,c,d are 4 numbers in the range 0-255, representing a byte array,
* each byte corresponding to a single digit. Within each byte, bit 0 is segment A, 
* bit 1 is segment B etc. The function may either set the entire display 
* or any desirable part using the length and position parameters.
\*********************************************************************************************/
bool CmndTM1637Raw(void) {
  uint8_t DATA[4] = { 0, 0, 0, 0 };

  char as[CMD_MAX_LEN]; 
  char bs[CMD_MAX_LEN]; 
  char cs[CMD_MAX_LEN]; 
  char ds[CMD_MAX_LEN]; 
  char sLength[CMD_MAX_LEN]; 
  char sPos[CMD_MAX_LEN]; 

  uint8_t a = 0;
  uint8_t b = 0;
  uint8_t c = 0;
  uint8_t d = 0;
  uint32_t position = 0;
  uint32_t length = 4;

  switch (ArgC()) 
  {
    case 6 :
      subStr(sPos, XdrvMailbox.data, ",", 6);
      position = atoi(sPos);
    case 5 :
      subStr(sLength, XdrvMailbox.data, ",", 5);
      length = atoi(sLength);
    case 4 :
      subStr(ds, XdrvMailbox.data, ",", 4);
      d = atoi(ds);
    case 3 :
      subStr(cs, XdrvMailbox.data, ",", 3);
      c = atoi(cs);
    case 2 :
      subStr(bs, XdrvMailbox.data, ",", 2);
      b = atoi(bs);
    case 1 :
      subStr(as, XdrvMailbox.data, ",", 1);
      a = atoi(as);
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: a=%d"), a);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: b=%d"), b);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: c=%d"), c);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: d=%d"), d);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: position=%d"), position);

  DATA[0] = a;
  DATA[1] = b;
  DATA[2] = c;
  DATA[3] = d;
  display->setSegments(DATA, length, position);
  return true;
}


/*********************************************************************************************\
* Display a given string. If more than 4 characters, will scroll message on display
* Text can be placed at arbitrary location on the display using the length and 
* position parameters without affecting the rest of the display.
* Command:   DisplayText text [, length {1-4} [, position {0-3}]]
\*********************************************************************************************/
bool CmndTM1637Text(bool clear) {

  char sString[CMD_MAX_LEN + 1]; 
  char sLength[CMD_MAX_LEN]; 
  char sPos[CMD_MAX_LEN];
  uint32_t position = 0;
  uint32_t length = 4;

  switch (ArgC()) 
  {
     case 3 :
      subStr(sPos, XdrvMailbox.data, ",", 3);
      position = atoi(sPos);
    case 2 :
      subStr(sLength, XdrvMailbox.data, ",", 2);
      length = atoi(sLength);
    case 1 :
      subStr(sString, XdrvMailbox.data, ",", 1);
  }

 
  if((length <= 0) || (length > 4)) length = 4;
  if(position > 3) position = 3;
  if(position < 0) position = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: string=%s"), sString);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: pos=%d"), position);

  for(uint32_t i=0; i<strlen(sString); i++) {    // replacing caret '^' with ASCII value for degrees symbol
    if(sString[i] == '^') {
      AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: Found Caret at %d"), i);
      sString[i] = 176;
    }
  }

  if(clear) display->clear();
  display->showString_P(sString, length, position);
  return true;
}


/*********************************************************************************************\
* Sets brightness of the display. The setting takes effect when a command is given 
* to change the data being displayed.
* Command:  DisplayBrightness {0-7}
\*********************************************************************************************/
bool CmndTM1637Brightness(void) {

  uint16_t val = XdrvMailbox.payload; 

  if((val < BRIGHTNESS_MIN) || (val > BRIGHTNESS_MAX)) {
    sprintf(msg, PSTR("Brightness should be a number in the range [%d, %d]"), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    XdrvMailbox.data = msg;
    return false;     
  } 
  display->setBrightness(val);
  return true;
}


/*********************************************************************************************\
* Sets the scroll frame delay when the display needs to scroll. The setting takes effect 
* when a text display command sends an argument with over four characters.
* Command:  DisplayScrollDelay delay_in_milliseconds    // default = 200
\*********************************************************************************************/
bool CmndTM1637ScrollDelay(void) {
  uint16_t val = XdrvMailbox.payload; 
  display->setScrolldelay(val);
  return true;
}


/*********************************************************************************************\
* Displays a clock.
* Command: DisplayClock 1   // 12-hour format
*          DisplayClock 2   // 24-hour format
*          DisplayClock 0   // turn off clock and clear
\*********************************************************************************************/
bool CmndTM1637Clock(void) {

  showClock = XdrvMailbox.payload;

  if(ArgC() == 0) XdrvMailbox.payload = 1;
  if(XdrvMailbox.payload > 1) clock24 = true;
  else if(XdrvMailbox.payload == 1) clock24 = false;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: showClock=%d"), showClock);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: clock24=%d"), clock24);

  if(!showClock) {
    display->clear();
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
  tm[4] = 0;
  if((millis() % 1000) > 500) display->showNumberDec(atoi(tm), 64, clock24, 4, 0);
  else display->showString_P(tm, 4, 0);
}

/*********************************************************************************************\
* This function is called for all TM1637 Display functions.
\*********************************************************************************************/
bool TM1637Cmd(uint8_t fn) {
  bool result = false;
  if(XdrvMailbox.data_len > CMD_MAX_LEN) {
    sprintf(msg, PSTR("Command text too long. Please limit it to %d characters"), CMD_MAX_LEN);
    XdrvMailbox.data = msg;    
    return result;      
  }

  switch (fn) {
    case FUNC_DISPLAY_CLEAR:
      result = CmndTM1637Clear();
      break;
    case FUNC_DISPLAY_NUMBER :
      result = CmndTM1637Number(true);                  
      break;
    case FUNC_DISPLAY_NUMBERNC :
      result = CmndTM1637Number(false);                  
      break;
    case FUNC_DISPLAY_FLOAT :
      result = CmndTM1637Float(true);                  
      break;
    case FUNC_DISPLAY_FLOATNC :
      result = CmndTM1637Float(false);                  
      break;
    case FUNC_DISPLAY_BRIGHTNESS:
      result = CmndTM1637Brightness();
      break;
    case FUNC_DISPLAY_RAW:
      result = CmndTM1637Raw();
      break;
    case FUNC_DISPLAY_LEVEL:
      result = CmndTM1637Level();
      break;
    case FUNC_DISPLAY_SEVENSEG_TEXT:
      result = CmndTM1637Text(true);
      break;
    case FUNC_DISPLAY_SEVENSEG_TEXTNC:
      result = CmndTM1637Text(false);
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


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp15(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    result = TM1637Init();              // init 
  }
  else if (XDSP_15 == Settings.display_model) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: FUNC_DISPLAY_MODEL"));
        result = true;
        break;
      case FUNC_DISPLAY_INIT:
        CmndTM1637Clear();
        AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: FUNC_DISPLAY_INIT"));
        break;
      // case FUNC_DISPLAY_DRAW_STRING:
      //   sprintf(msg, PSTR("For TM1637, please use DisplaySevenSegText instead of DisplayText"));
      //   XdrvMailbox.data = msg;
      //   result = true;
      //   break;
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
      case FUNC_DISPLAY_SCROLLDELAY:
      case FUNC_DISPLAY_CLOCK:
        result = TM1637Cmd(function);
        break;
      case FUNC_DISPLAY_EVERY_50_MSECOND:
        if(showClock) {
          showTime();
        }
        break;        
    }
  }
  return result;
}

#endif  // USE_DISPLAY_TM1637
#endif  // USE_DISPLAY
