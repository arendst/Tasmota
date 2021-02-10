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
  In addition, it is also possible to clear the display, set brightness (7 levels) and display
  a rudimentary bar graph. 
  
  To use, compile Tasmota with USE_SPI, USE_DISPLAY and USE_DISPLAY_TM1637 
  This adds the following
  Pins:
    * TM 1637 DIO
    * TM 1637 CLK
  
  Connect the TM1637 display module's DIO and CLK pins to any free GPIOs of the ESP8266 module
  and assign the above pins accordingly, from Tasmota's GUI.
  Once the device restarts the following commands become available:
    * TM1637Clear
    * TM1637Number
    * TM1637Float
    * TM1637Brightness
    * TM1637Raw
    * TM1637Level
    * TM1637Text

  The usage of these commands are explained in code comments below. 

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
uint8_t length = 4;
uint8_t position = 0;
bool leadingzeros = false;
uint8_t length2 = 4;
uint8_t precision = 4;

char msg[60];


#define BRIGHTNESS_MIN    0
#define BRIGHTNESS_MAX    7
#define CMD_MAX_LEN   55
#define LEVEL_MIN    0
#define LEVEL_MAX    100


#define D_CMND_TM1637 "TM1637"

const char S_TM1637_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_TM1637 "%s\":%s}";
const char S_TM1637_COMMAND_SVALUE[] PROGMEM = "{\"" D_CMND_TM1637 "%s\":\"%s\"}";
const char S_TM1637_RAWCOMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_TM1637 "%s\":\"%d,%d,%d,%d\"}";
const char S_TM1637_MESSAGE_SVALUE[] PROGMEM = "{\"" "\"%s\":\"%s\"}";
const char kTM1637_Commands[] PROGMEM   = "Clear|Number|NumberC|Float|FloatC|Brightness|Raw|Level|Text|TextC|ScrollDelay|Clock";

enum TM1637_Commands {        // commands useable in console/MQTT or rules

  CMND_TM1637_CLEAR,          // Blanks the display, command: "TM1637Clear"

  CMND_TM1637_NUMBER,         // Display integer number  (-999 to 9999). command e.g., "TM1637Number 1234"
                              // Control 'leading zeros', 'length' and 'position' with  "TM1637Number 1234, <leadingZeros>, <length>, <position>"
                              // 'leading zeros' can be 1 or 0 (default), 'length' can be 1 to 4 (default), 'position' can be 0 (left-most) to 3 (right-most)

  CMND_TM1637_NUMBERC,        // Clear first, then display integer number  (-999 to 9999). e.g., "TM1637NumberC 1234". Usage is same as above.
                        
  CMND_TM1637_FLOAT,          // Display float (with decimal point)  (0.001 to 9999.) command e.g., "TM1637Float 12.34"

  CMND_TM1637_FLOATC,         // Clear first, then display float (with decimal point)  (0.001 to 9999.) command e.g., "TM1637FloatC 12.34"  

  CMND_TM1637_BRIGHTNESS,     // Set brightness (1 to 7) command e.g., "TM1637Brightness 2"  Note: Brightness takes effect only after a new display command is sent.

  CMND_TM1637_RAW,            // Takes 4 comma-separated integers (0-255) and displays raw segments 
                              // Each 7-segment display unit is represented by an 8-bit(8th bit for decimap point) number.
                              // For example, the command "TM1637Raw 255, 255, 255, 255" would display "[8.8.8.8.]"

  CMND_TM1637_LEVEL,          // Display a horizontal bar graph (0-100) command e.g., "TM1637Level 50" will display [||||    ]  

  CMND_TM1637_TEXT,           // Display basic text (scrolls if > 4 characters)  command e.g., "TM1637Text ajith vasudevan" 
                              // Control 'length' and 'position' with  "TM1637Text abcd, <length>, <position>"
                              // 'length' can be 1 to 4 (default), 'position' can be 0 (left-most) to 3 (right-most)
                              // Note: A caret sign '^' in the input text would be replaced by a "degrees" symbol. This is handy for displaying temperature!
                              //       Other Characters whose ASCII > 127 or ASCII < 32 would simply be blank.

  CMND_TM1637_TEXTC,          // Clear first, then display text. Usage is same as above.

  CMND_TM1637_SCROLLDELAY,     // Sets the speed of text scroll. Takes effect only after a new TEXT command is sent with 4 chars or more.     

  CMND_TM1637_CLOCK        // Displays a clock. To start clock, command "TM1637Clock 1". To turn offclock, "TM1637Clock 0" 
};                                  

/*********************************************************************************************\
* Init function
\*********************************************************************************************/
bool TM1637Init(void) {
  display = new TM1637TinyDisplay(Pin(GPIO_TM1637_CLK), Pin(GPIO_TM1637_DIO));
  display->setBrightness(BRIGHT_5);
  display->clear();
  if (!Settings.display_model) {
    Settings.display_model = XDSP_15;
  }  
  AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1637 display driver initialized"));
  return true;
}


/*********************************************************************************************\
* Displays number without decimal, with/without leading zeros, specifying length
* and position, optionally skipping clearing display before displaying the number.
* commands: DisplayNumber   num [,leading_zeros {0|1} [,length {1-4} [,position {0-3} ]]] 
*           DisplayNumberNC num [,leading_zeros {0|1} [,length {1-4} [,position {0-3} ]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Number(bool clear) {
  char sNum[15]; char sLeadingZero[5]; char sLength[5]; char sPos[5];
  uint32_t num;

  switch (ArgC()) 
  {
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

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: num=%d"), num);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: leadingzeros=%d"), leadingzeros);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: position=%d"), position);

  if(clear) display->clear(); 
  display->showNumber(num, leadingzeros, length, position);
  return true;
}



/*********************************************************************************************\
* Displays number with decimal, specifying precision and length,
* optionally skipping clearing display before displaying the number.
* commands: DisplayFloat   num [,precision {0-4} [,length {1-4} [,position {0-3} ]]] 
*           DisplayFloatNC num [,precision {0-4} [,length {1-4} [,position {0-3} ]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Float(bool clear) {

  char sNum[15]; char sPrecision[5]; char sLength[5];
  float fnum = 0.0f;

  switch (ArgC()) 
  {
     case 3 :
      subStr(sLength, XdrvMailbox.data, ",", 3);
      length2 = atoi(sLength);
    case 2 :
      subStr(sPrecision, XdrvMailbox.data, ",", 2);
      precision = atoi(sPrecision);
    case 1 :
      subStr(sNum, XdrvMailbox.data, ",", 1);
      fnum = atof(sNum);
  }
  
  if((precision < 0) || (precision > 4)) precision = 4;
  if((length2 <= 0) || (length2 > 4)) length2 = 4;

  char s[30];
  ext_snprintf_P(s, sizeof(s), PSTR("LOG: TM1637: num=%*_f"), 4, &fnum); 
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s"), s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: precision=%d"), precision);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length2=%d"), length2);

  if(clear) display->clear(); 
  display->showNumber(fnum, precision, length2, 0);
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

  char as[5]; char bs[5]; char cs[5]; char ds[5]; char sLength[5]; char sPos[5]; 
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
* Command:   DisplaySevensegText text [, length {1-4} [, position {0-3}]]
\*********************************************************************************************/
bool CmndTM1637Text(bool clear) {
  if(XdrvMailbox.data_len > CMD_MAX_LEN) {
    sprintf(msg, PSTR("Text too long. Please limit text command length to %d"), CMD_MAX_LEN);
    XdrvMailbox.data = msg;
    return true;    
  }
  char sString[CMD_MAX_LEN + 1]; char sLength[5]; char sPos[5];
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

  for(uint32_t i; i<strlen(sString); i++) {    // replacing caret '^' with ASCII value for degrees symbol
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
    sprintf(msg, PSTR("Brightness is a number in the range [%d, %d]"), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    XdrvMailbox.data = msg;
    return false;     
  } 
  display->setBrightness(val);
  return true;
}

bool CmndTM1637ScrollDelay(void) {
  uint16_t val = XdrvMailbox.payload; 
  if(val == 0) val = 200;
  display->setScrolldelay(val);
  return true;
}


/*********************************************************************************************\
* Displays a clock.
* Command: DisplayClock 1   // for 12-hour format
*          DisplayClock 2   // for 24-hour format
*          DisplayClock 0   // turn off clock
\*********************************************************************************************/
bool CmndTM1637Clock(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: CmndTM1637Clock %d"), XdrvMailbox.payload);

  showClock = XdrvMailbox.payload;
  if(XdrvMailbox.payload > 1) clock24 = true;
  else if(XdrvMailbox.payload == 1) clock24 = false;
  if(!showClock) {
    display->clear();
  }
  return true;
}


/*********************************************************************************************\
* refreshes the time on if clock is displayed
\*********************************************************************************************/
void showTime() {
  uint8_t hr = RtcTime.hour;
  uint8_t mn = RtcTime.minute;
  
  if(!clock24) {
    if(hr > 12) hr -= 12;
  }
  if((millis() % 1000) > 500) display->showNumber(1.0f * hr + ((float)RtcTime.minute)/100.0f, 2, 4, 0);
  else display->showNumber(100 * hr + RtcTime.minute);
}

/*********************************************************************************************\
* This function is called for all TM1637 Display functions.
\*********************************************************************************************/
bool TM1637Cmd(uint8_t fn) {
  bool result = false;
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
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: FUNC_DISPLAY_INIT_DRIVER"));
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
      case FUNC_DISPLAY_DRAW_STRING:
        sprintf(msg, PSTR("For TM1637, please use DisplaySevenSegText instead of DisplayText"));
        XdrvMailbox.data = msg;
        result = true;
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
