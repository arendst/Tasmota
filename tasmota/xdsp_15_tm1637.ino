/*
  xdsp_15_tm1637.ino - Support for TM1637 seven-segment display (upto 6 digits) for Tasmota

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
  on the inexpensive TM1637-based seven-segment modules (tested on both 4- and 6-digit variants). 
  Raw segments can also be displayed.
  In addition, it is also possible to set brightness (8 levels), clear the display, scroll text,  display
  a rudimentary bar graph, and a Clock (12 hr and 24 hr). 
  
  To use, compile Tasmota with USE_DISPLAY and USE_DISPLAY_TM1637, or build the tasmota-display env.

  The pins to use are "SSPI MOSI" and "SSPI SCLK".

  Connect the TM1637 display module's DIO and CLK pins to any free GPIOs of the ESP8266 module
  and assign the pins as follows from Tasmota's GUI:

  DIO hardware pin --> "SSPI MOSI"
  CLK hardware pin --> "SSPI SCLK"

  Once the device restarts the following "Display" commands become available:



  DisplaySize           size {1-6}

                               Sets the number of digits to use. This is typically set to the actual number of digits available
                               in the display module. command e.g., "DisplaySize 6" 


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



*/



#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_TM1637
/*********************************************************************************************\
 * TM1637 support
\*********************************************************************************************/

#define XDSP_15                    15
#include "SevenSegmentTM1637.h"

SevenSegmentTM1637 *display;
bool showClock = false;
bool clock24 = false;
char tm[5];
char msg[60];
uint32_t NUM_DIGITS = 4;
uint32_t prev_num_digits = 4;
bool scroll = false;
uint32_t scrolldelay = 4;
uint32_t scrollindex = 0;
uint32_t iteration = 0;

#define BRIGHTNESS_MIN    0   // Display OFF
#define BRIGHTNESS_MAX    8
#define CMD_MAX_LEN   55
#define LEVEL_MIN    0
#define LEVEL_MAX    100
#define SCROLL_MAX_LEN   50

char scrolltext[CMD_MAX_LEN];

/*********************************************************************************************\
* Init function
\*********************************************************************************************/
bool TM1637Init(void) {
  display = new SevenSegmentTM1637(Pin(GPIO_SSPI_SCLK), Pin(GPIO_SSPI_MOSI) );
  NUM_DIGITS = Settings.display_size ?  Settings.display_size : 4;
  display->begin(NUM_DIGITS, 1);
  display->setBacklight(40);
  clearDisplay();
  if (!Settings.display_model) {
    Settings.display_model = XDSP_15;
  }  
  AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1637 display driver initialized"));
  return true;
}


/*********************************************************************************************\
* Displays number without decimal, with/without leading zeros, specifying start-position
* and length, optionally skipping clearing display before displaying the number.
* commands: DisplayNumber   num [,position {0-(NUM_DIGITS-1)} [,leading_zeros {0|1} [,length {1 to NUM_DIGITS}]]] 
*           DisplayNumberNC num [,position {0-(NUM_DIGITS-1)} [,leading_zeros {0|1} [,length {1 to NUM_DIGITS}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Number(bool clear) {
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

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: num=%d"), num);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: position=%d"), position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: leadingzeros=%d"), leadingzeros);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);

  if(clear) clearDisplay(); 

  char txt[30];
  snprintf_P(txt, sizeof(txt), PSTR("%d"), num); 
  if(!length) length = strlen(txt);
  if((length < 0) || (length > NUM_DIGITS)) length = NUM_DIGITS;

  char pad = (leadingzeros ? '0': ' ');
  uint32_t i = position;
  uint8_t rawBytes[1];
  rawBytes[0] = display->encode(pad);
  for(; i<position + (length - strlen(txt)); i++) {
    if(i>NUM_DIGITS) break;
    display->printRaw(rawBytes, 1, i);
  }
  
  for(uint32_t j = 0; i< position + length; i++, j++) {
    if(txt[j] == 0) break;
    rawBytes[0] = display->encode(txt[j]);
    if(i>NUM_DIGITS) break;
    display->printRaw(rawBytes, 1, i);
  }

  return true;
}



/*********************************************************************************************\
* Displays number with decimal, specifying position, precision and length,
* optionally skipping clearing display before displaying the number.
* commands: DisplayFloat   num [,position {0-(NUM_DIGITS-1)} [,precision {0-NUM_DIGITS} [,length {1 to NUM_DIGITS}]]] 
*           DisplayFloatNC num [,position {0-(NUM_DIGITS-1)} [,precision {0-NUM_DIGITS} [,length {1 to NUM_DIGITS}]]]    // "NC" --> "No Clear"
\*********************************************************************************************/
bool CmndTM1637Float(bool clear) {

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
  ext_snprintf_P(s, sizeof(s), PSTR("LOG: TM1637: num=%*_f"), 4, &fnum); 
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s"), s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: precision=%d"), precision);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  uint8_t rawBytes[1];
  for(uint32_t i=0, j=0; i<length; i++, j++) {
    if(txt[i] == 0) break;
    rawBytes[0] = display->encode(txt[i]);      
    if(txt[i+1] == '.') { 
      rawBytes[0] = rawBytes[0] | 128;
      i++;
      length++;
    } 
    if((j+position) > NUM_DIGITS) break;
    display->printRaw(rawBytes, 1, j+position);
   }

  return true;
}


// /*********************************************************************************************\
// * Clears the display
// * Command:  DisplayClear
// \*********************************************************************************************/
bool CmndTM1637Clear(void) {
  clearDisplay();
  sprintf(msg, PSTR("Cleared"));
  XdrvMailbox.data = msg;  
  return true;
}


void clearDisplay (void) {
  unsigned char arr[] =  {0};
  AddLog(LOG_LEVEL_DEBUG, PSTR("Clearing digit %d"), NUM_DIGITS);
  for(int i=0; i<NUM_DIGITS; i++) display->printRaw(arr, 1, i);
}


/*********************************************************************************************\
* Display scrolling text
* Command:   DisplayScrollText text 
\*********************************************************************************************/
bool CmndTM1637ScrollText(void) {

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: text=%s"), XdrvMailbox.data);

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
bool CmndTM1637ScrollDelay(void) {
  if(scrolldelay<0) scrolldelay=0;
  scrolldelay = XdrvMailbox.payload; 
  return true;
}



/*********************************************************************************************\
* Scrolls a given string. Called every 50ms
\*********************************************************************************************/
void scrollText(void) {
  if(scroll) {
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
      char charToDisp = (clr ? ' ' : scrolltext[j]);
      rawBytes[0] = display->encode(charToDisp);
      display->printRaw(rawBytes, 1, i);
    }
    scrollindex++;
  }
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

  uint8_t totalBars = 2*NUM_DIGITS;
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: CmndTM1637Level totalBars=%d"), totalBars);
  float barsToDisplay = totalBars * val / 100.0f;
  char txt[5];
  ext_snprintf_P(txt, sizeof(txt), PSTR("%*_f"), 1, &barsToDisplay); 
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: CmndTM1637Level barsToDisplay=%s"), txt);
  char s[4];
  ext_snprintf_P(s, sizeof(s), PSTR("%*_f"), 0, &barsToDisplay); 
  uint8_t numBars = atoi(s);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: CmndTM1637Level numBars=%d"), numBars);

  clearDisplay();
  uint8_t rawBytes[1];
  for(int i=1; i<=numBars; i++) {
    uint8_t digit = (i-1) / 2;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: CmndTM1637Level digit=%d"), digit);
    uint8_t value = (((i%2) == 0) ? 54 : 48);
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: CmndTM1637Level value=%d"), value);
    rawBytes[0] = value;
    display->printRaw(rawBytes, 1, digit);
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
bool CmndTM1637Raw(void) {
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


  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: a=%d"), DATA[0]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: b=%d"), DATA[1]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: c=%d"), DATA[2]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: d=%d"), DATA[3]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: e=%d"), DATA[4]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: f=%d"), DATA[5]);

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: position=%d"), position);

  uint8_t rawBytes[1];
  for(uint32_t i=position; i<position+length; i++ ) {
    if(i>(NUM_DIGITS-1)) break;
    rawBytes[0] = DATA[i-position];      
    display->printRaw(rawBytes, 1, i);
  }

  return true;
}



/*********************************************************************************************\
* Display a given string. 
* Text can be placed at arbitrary location on the display using the length and 
* position parameters without affecting the rest of the display.
* Command:   DisplayText text [, position {0-(NUM_DIGITS-1)} [,length {1 to NUM_DIGITS}]]
\*********************************************************************************************/
bool CmndTM1637Text(bool clear) {
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

  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: sString=%s"), sString);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: position=%d"), position);
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: length=%d"), length);

  if(clear) clearDisplay(); 

  if(!length) length = strlen(sString);
  if((length < 0) || (length > NUM_DIGITS)) length = NUM_DIGITS;

  uint32_t i = position;
  uint8_t rawBytes[1];  
  for(uint32_t j = 0; i< position + length; i++, j++) {
    if(i > (NUM_DIGITS-1)) break;
    if(sString[j] == 0) break;
    rawBytes[0] = display->encode(sString[j]);      
    if(i>(NUM_DIGITS-1)) break;
    display->printRaw(rawBytes, 1, i);
  }

  return true;
}


/*********************************************************************************************\
* Sets brightness of the display. 
* Command:  DisplayBrightness {0-8}    // 0 => off
\*********************************************************************************************/
bool CmndTM1637Brightness(void) {

  uint16_t val = XdrvMailbox.payload; 

  if((val < BRIGHTNESS_MIN) || (val > BRIGHTNESS_MAX)) {
    sprintf(msg, PSTR("Brightness should be a number in the range [%d, %d]"), BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    XdrvMailbox.data = msg;
    return false;     
  } 
  display->setBacklight(val*10);
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
  uint8_t rawBytes[1];  
  for(uint32_t i = 0; i< 4; i++) {
    rawBytes[0] = display->encode(tm[i]);      
    if((millis() % 1000) > 500 && (i == 1)) rawBytes[0] = rawBytes[0] | 128;
    display->printRaw(rawBytes, 1, i);
  }
}

/*********************************************************************************************\
* This function is called for all TM1637 Display functions.
\*********************************************************************************************/
bool TM1637Cmd(uint8_t fn) {
  bool result = false;
  NUM_DIGITS = Settings.display_size;
  if(prev_num_digits != NUM_DIGITS) {    // Cleck for change of display size, and re-init the library
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: Size changed. Re-initializing library..."));
    display = new SevenSegmentTM1637(Pin(GPIO_SSPI_SCLK), Pin(GPIO_SSPI_MOSI) );
    display->begin(NUM_DIGITS, 1);
    display->setBacklight(40);
    clearDisplay();
    prev_num_digits = NUM_DIGITS;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOG: TM1637: Re-initialized library"));
  }

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
      case FUNC_DISPLAY_CLOCK:
        result = TM1637Cmd(function);
        break;
      case FUNC_DISPLAY_EVERY_50_MSECOND:
        scrollText();
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
