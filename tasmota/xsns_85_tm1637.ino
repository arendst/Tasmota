/*
  xsns_85_tm1637.ino - Support for TM1637 seven-segment display for Tasmota

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
  
  To use, compile Tasmota with USE_DISPLAY_SEVENSEG and USE_TM1637 
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



#ifdef USE_DISPLAY_SEVENSEG
#ifdef USE_TM1637
/*********************************************************************************************\
 * TM1637 support
\*********************************************************************************************/

#define XSNS_85                    85
#include <TM1637TinyDisplay.h>

TM1637TinyDisplay *display;
bool defaultResponse = true;
bool isString = false;
char command[CMDSZ];


#define BRIGHTNESS_MIN    0
#define BRIGHTNESS_MAX    7
#define NUMBER_MIN    -999
#define NUMBER_MAX    9999
#define FLOAT_MIN    -999.0
#define FLOAT_MAX    9999.0
#define CMD_MAX_LEN   55
#define LEVEL_MIN    0
#define LEVEL_MAX    100


#define D_CMND_TM1637 "TM1637"

const char S_TM1637_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_TM1637 "%s\":%s}";
const char S_TM1637_COMMAND_SVALUE[] PROGMEM = "{\"" D_CMND_TM1637 "%s\":\"%s\"}";
const char S_TM1637_RAWCOMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_TM1637 "%s\":\"%d,%d,%d,%d\"}";
const char S_TM1637_MESSAGE_SVALUE[] PROGMEM = "{\"" "\"%s\":\"%s\"}";
const char kTM1637_Commands[] PROGMEM   = "Clear|Number|NumberC|Float|Brightness|Raw|Level|Text|TextC|ScrollDelay";
uint32_t len = strlen(D_CMND_TM1637);


enum TM1637_Commands {        // commands useable in console/MQTT or rules

  CMND_TM1637_CLEAR,          // Blanks the display, command: "TM1637Clear"

  CMND_TM1637_NUMBER,         // Display integer number  (-999 to 9999). command e.g., "TM1637Number 1234"
                              // Control 'leading zeros', 'length' and 'position' with  "TM1637Number 1234, <leadingZeros>, <length>, <position>"
                              // 'leading zeros' can be 1 or 0 (default), 'length' can be 1 to 4 (default), 'position' can be 0 (left-most) to 3 (right-most)

  CMND_TM1637_NUMBERC,        // Clear first, then display integer number  (-999 to 9999). Usage is same as above.
                        
  CMND_TM1637_FLOAT,          // Display float (with decimal point)  (0.001 to 9999.) command e.g., "TM1637Float 12.34"  

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

  CMND_TM1637_SCROLLDELAY     // Sets the speed of text scroll. Takes effect only after a new TEXT command is sent with 4 chars or more.     
};                                  

bool TM1637Init(void) {
  display = new TM1637TinyDisplay(Pin(GPIO_TM1637_CLK), Pin(GPIO_TM1637_DIO));
  display->setBrightness(BRIGHT_5);
  display->clear();
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: initialized"));
  return true;
}


bool CmndTM1637Number(bool clear) {

  char sNum[5]; char sLeadingZero[5]; char sLength[5]; char sPos[5];
  subStr(sNum, XdrvMailbox.data, ",", 1);
  subStr(sLeadingZero, XdrvMailbox.data, ",", 2);
  subStr(sLength, XdrvMailbox.data, ",", 3);
  subStr(sPos, XdrvMailbox.data, ",", 4);

  uint32_t num = atoi(sNum);
  bool leadingZero = atoi(sLeadingZero);
  uint32_t length = atoi(sLength);
  uint32_t pos = atoi(sPos);

  if((length <= 0) || (length > 4)) length = 4;
  if(pos > 4) pos = 4;
  if(pos < 0) pos = 0;
     
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: num=%d"), num);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: leadingZero=%d"), leadingZero);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: pos=%d"), pos);

  if(clear) display->clear(); 
  display->showNumber(num, leadingZero, length, pos);
  return true;
}

bool CmndTM1637Float(void) {
  float val = atof(XdrvMailbox.data); 
  char msg[50]; 
  if((val < FLOAT_MIN) || (val > FLOAT_MAX)) {
    defaultResponse = false;
    float fmin = FLOAT_MIN;
    float fmax = FLOAT_MAX;
    char msg[50];
    ext_snprintf_P(msg, sizeof(msg), PSTR("Float should be in the range [%1_f, %1_f]"), &fmin, &fmax);
    Response_P(S_TM1637_MESSAGE_SVALUE, "Error", msg);        
    return true;     
  } 
  display->showNumber(val); 
  return true;
}

bool CmndTM1637Clear(void) {
  display->clear();
  return true;
}

bool CmndTM1637Level(void) {
  uint16_t val = XdrvMailbox.payload;
  if((val < LEVEL_MIN) || (val > LEVEL_MAX)) {
    defaultResponse = false;
    char msg[45]; 
    sprintf(msg, PSTR("Level should be a number in the range [%d, %d]"), LEVEL_MIN, LEVEL_MAX);
    Response_P(S_TM1637_MESSAGE_SVALUE, "Error", msg);    
    return true;     
  } 
  display->showLevel(val, false);
  return true;
}

bool CmndTM1637Raw(void) {
  uint8_t DATA[1][4] = {
    { 0x00, 0x00, 0x00, 0x00 }
  };

  char a[5]; char b[5]; char c[5]; char d[5];
  subStr(a, XdrvMailbox.data, ",", 1);
  subStr(b, XdrvMailbox.data, ",", 2);
  subStr(c, XdrvMailbox.data, ",", 3);
  subStr(d, XdrvMailbox.data, ",", 4);

  uint16_t a1 = atoi(a);
  uint16_t b1 = atoi(b);
  uint16_t c1 = atoi(c);
  uint16_t d1 = atoi(d);

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: a1=%d"), a1);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: b1=%d"), b1);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: c1=%d"), c1);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: d1=%d"), d1);

  DATA[0][0] = a1;
  DATA[0][1] = b1;
  DATA[0][2] = c1;
  DATA[0][3] = d1;
  defaultResponse = false;
  Response_P(S_TM1637_RAWCOMMAND_NVALUE, command, a1, b1, c1, d1);
  display->showAnimation_P(DATA, FRAMES(DATA), TIME_MS(150));
  return true;
}


bool CmndTM1637Text(bool clear) {
  char sString[CMD_MAX_LEN + 1]; char sLength[5]; char sPos[5];
  subStr(sString, XdrvMailbox.data, ",", 1);
  subStr(sLength, XdrvMailbox.data, ",", 2);
  subStr(sPos, XdrvMailbox.data, ",", 3);

  uint32_t length = atoi(sLength);
  uint32_t pos = atoi(sPos);
 
  if((length <= 0) || (length > 4)) length = 4;
  if(pos > 4) pos = 4;
  if(pos < 0) pos = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: string=%s"), sString);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: pos=%d"), pos);

  for(uint32_t i; i<strlen(sString); i++) {    // replacing caret '^' with ASCII value for degrees symbol
    if(sString[i] == '^') sString[i] = 176;
  }

  if(clear) display->clear();
  display->showString_P(sString, length, pos);
  return true;
}


bool CmndTM1637Brightness(void) {
  uint16_t val = XdrvMailbox.payload; 
  if((val < BRIGHTNESS_MIN) || (val > BRIGHTNESS_MAX)) {
    defaultResponse = false;
    char msg[45]; 
    sprintf(msg, PSTR("Brightness is a number in the range [%d, %d]"), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    Response_P(S_TM1637_MESSAGE_SVALUE, "Error", msg);    
    return true;     
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


bool TM1637Cmd(void) {
  bool result = false;
  uint32_t command_code = -1;
  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_TM1637), len)) {  
    command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + len, kTM1637_Commands);
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: command_code: %d   params: %s"), command_code, XdrvMailbox.data);
  }
  if(command_code < 0) {
    return result;
  }
  if(XdrvMailbox.data_len > CMD_MAX_LEN) {
    char msg[45]; 
    sprintf(msg, PSTR("Command too long. Should be less than %d characters long"), CMD_MAX_LEN);
    Response_P(S_TM1637_MESSAGE_SVALUE, "Error", msg);
    return result;    
  }

  switch (command_code) {
    case CMND_TM1637_CLEAR:
      defaultResponse = false;
      result = CmndTM1637Clear();
      Response_P(S_TM1637_MESSAGE_SVALUE, "Message", "Cleared");    
      break;
    case CMND_TM1637_NUMBER :
      result = CmndTM1637Number(false);                  
      break;
    case CMND_TM1637_NUMBERC :
      result = CmndTM1637Number(true);                  
      break;
    case CMND_TM1637_FLOAT :
      result = CmndTM1637Float();                  
      break;
    case CMND_TM1637_BRIGHTNESS:
      result = CmndTM1637Brightness();
      break;
    case CMND_TM1637_RAW:
      isString = true;
      result = CmndTM1637Raw();
      break;
    case CMND_TM1637_LEVEL:
      result = CmndTM1637Level();
      break;
    case CMND_TM1637_TEXT:
      isString = true;
      result = CmndTM1637Text(false);
      break;
    case CMND_TM1637_TEXTC:
      isString = true;
      result = CmndTM1637Text(true);
      break;
    case CMND_TM1637_SCROLLDELAY:
      isString = true;
      result = CmndTM1637ScrollDelay();
      break;
  }

  if(defaultResponse) {
    if(isString) Response_P(S_TM1637_COMMAND_SVALUE, command, XdrvMailbox.data);
    else Response_P(S_TM1637_COMMAND_NVALUE, command, XdrvMailbox.data);
  }
  return result;
} 


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns85(uint8_t function)
{
  bool result = false;
  if (PinUsed(GPIO_TM1637_CLK) && PinUsed(GPIO_TM1637_DIO)) {
    switch (function) {
      case FUNC_PRE_INIT:
        TM1637Init();              // init 
        break;
      case FUNC_COMMAND:
        defaultResponse = true;    // resetting value for new cmnd
        isString = false;          // resetting value for new cmmd
        result = TM1637Cmd();
        break;
    }
  }
  return result;
}

#endif  // USE_1637
#endif  // USE_DISPLAY_SEVENSEG
