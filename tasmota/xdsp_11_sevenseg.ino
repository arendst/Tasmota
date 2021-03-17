/*
  xdsp_11_sevenseg.ino - Display seven segment support for Tasmota

  Copyright (C) 2021  Theo Arends and Adafruit

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

#ifdef USE_I2C
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_SEVENSEG

#define XDSP_11                    11
#define XI2C_47                    47  // See I2CDEVICES.md

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>  // Seven segment LED

Adafruit_7segment *sevenseg[8];
uint8_t sevensegs = 0;
uint8_t sevenseg_state = 0;

/*********************************************************************************************/

#ifdef USE_DISPLAY_SEVENSEG_COMMON_ANODE
void bufferStuffer(uint32_t i) {
  uint8_t outArray[8] = {0,0,0,0,0,0,0,0};
  uint8_t v;

  for (int j = 0; j < 8; j++) {
    for (int k = 0; k < 8; k++) {
      v = ((sevenseg[i]->displaybuffer[j] >> k) & 1);
      outArray[k] |= (v << j);
    }
  }

  for (int j = 0; j < 8; j++) {
    sevenseg[i]->displaybuffer[j] = outArray[j];
  }
}
#endif

void SevensegWrite(void)
{
  for (uint32_t i = 0; i < sevensegs; i++) {
#ifdef USE_DISPLAY_SEVENSEG_COMMON_ANODE
    bufferStuffer(i);
#endif
    sevenseg[i]->writeDisplay();
  }
}

void SevensegLog(void)
{
  // get sensor data
  ResponseClear();
  ResponseAppendTime();
  XsnsCall(FUNC_JSON_APPEND);
  ResponseJsonEnd();

  // display nth sensor value on nth display
  // code adapted from xdrv_13_display.ino, DisplayAnalyzeJson()
  uint8_t unit = 0;
  int16_t valueDecimal = 0;
  double valueFloat = 0;
  uint8 fDigits = 0;
  String jsonStr = TasmotaGlobal.mqtt_data;  // Move from stack to heap to fix watchdogs (20180626)
  JsonParser parser((char*)jsonStr.c_str());
  JsonParserObject object_root = parser.getRootObject();
  if (object_root) {
    for (auto key_level1 : object_root) {
      JsonParserToken token_level1 = key_level1.getValue();
      if (token_level1.isObject()) {
        JsonParserObject object_level1 = token_level1.getObject();
        for (auto key_level2 : object_level1) {
          const char* value_level2 = key_level2.getValue().getStr(nullptr);
          if (value_level2 != nullptr) {
            if ((unit < sevensegs) && (sevenseg[unit] != nullptr)) {
              if (strchr( value_level2, '.') == NULL) {
                sevenseg[unit]->print(atoi(value_level2), DEC);
              } else {
                sevenseg[unit]->printFloat(atof(value_level2), 1, DEC);
              }
              sevenseg[unit]->writeDisplay();
              unit++;
            }
          }
        }
      }
    }
  }
}

void SevensegDim(void)
{
  for (uint32_t i = 0; i < sevensegs; i++) {
    sevenseg[i]->setBrightness(Settings.display_dimmer);
  }
}

void SevensegBlinkrate( void)
{
  for (uint32_t i = 0; i < sevensegs; i++) {
    sevenseg[i]->blinkRate(XdrvMailbox.payload);
  }
}

void SevensegClear(void)
{
  for (uint32_t i = 0; i < sevensegs; i++) {
    sevenseg[i]->clear();
  }
  SevensegWrite();
}

/*********************************************************************************************/

void SevensegInitMode(void)
{
  for (uint32_t i = 0; i < sevensegs; i++) {
    sevenseg[i]->setBrightness(Settings.display_dimmer);
    sevenseg[i]->blinkRate(0);
  }
  SevensegClear();
}

void SevensegInit(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      SevensegInitMode();
      break;
  }
}

void SevensegInitDriver(void) {
  if (!TasmotaGlobal.i2c_enabled) { return; }

  if (!Settings.display_model) {
    if (I2cSetDevice(Settings.display_address[0])) {
      Settings.display_model = XDSP_11;
    }
  }

  if (XDSP_11 == Settings.display_model) {
    sevenseg_state = 1;
    for (sevensegs = 0; sevensegs < 8; sevensegs++) {
        if (Settings.display_address[sevensegs]) {
          I2cSetActiveFound(Settings.display_address[sevensegs], "SevenSeg");
          sevenseg[sevensegs] = new Adafruit_7segment();
          sevenseg[sevensegs]->begin(Settings.display_address[sevensegs]);
        } else {
            break;
        }
    }

    Settings.display_width = 4;
    Settings.display_height = sevensegs;

    SevensegInitMode();
  }
}

void SevensegOnOff(void)
{
  if (!disp_power) { SevensegClear(); }
}

void SevensegDrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  enum OutNumType {DECIMAL, HEXADECIMAL, FLOAT, SEGMENTS};
  int16_t number = 0;
  double numberf = 0;
  boolean hasnumber= false;
  uint8_t dots= 0;
  OutNumType outnumtype= DECIMAL;
  uint8 fds = 0; // number of fractional digits for fp number
  boolean done= false;
  boolean s= false;
  uint8_t unit= y;
  char *buf;

  if ((unit>=sevensegs) || (unit<0)) {
    unit=0;
  }

  for (int i=0; (str[i]!='\0') && (!done); i++) {
    // [optional prefix(es) chars]digits
    // Some combinations won't make sense.
    // Reference: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-led-backpack.pdf
    // This code has been tested on 1.2" and 0.56" 7-Segment LED displays, but should mostly work for others.
    //
    // Prefixes:
    // x  upcoming decimal integer number displayed as hex
    // :  turn on middle colon
    // ^  turn on top left dot
    // v  turn on bottom left dot
    // .  turn on AM/PM/Degree dot
    // s  upcoming number is seconds, print as HH:MM or MM:SS
    // z  clear this display
    // f  upcoming number is floating point
    // r  raw segment based on bitmap of upcoming integer number (see reference document above)
    //
    // Some sample valid combinations:
    //  787     -> 787
    //  x47     -> 2F
    //  s:241   -> 04:01
    //  s241    -> 4 01
    //  s1241   -> 20:41
    //  z       ->
    //  x88     -> 58
    //  f8.5    -> 8.5
    //  f-9.34  -> -9.34
    //  f:-9.34 -> -9.:34
    //  r255    -> 8. (all 8 segments on)

    switch (str[i]) {
      case 'x': // print given dec value as hex
        // hex = true;
        outnumtype = HEXADECIMAL;
        break;
      case 'f': // given number is floating point number
        // fp = true;
        outnumtype = FLOAT;
        break;
      case ':': // print colon
        dots |= 0x02;
        break;
      case '^': // print top_left_dot
        dots |= 0x08;
        break;
      case 'v': // print bottom_left_dot
        dots |= 0x04;
        break;
      case '.': // print ampm
        dots |= 0x10;
        break;
      case 's': // duration in seconds
        s = true;
        break;
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        hasnumber= true;
        if (outnumtype == FLOAT) {
          // Floating point number is given
          numberf = atof(str+i);
          // Find number of fractional digits
          buf= str+i;
          char *cp= strchr(buf, '.');
          if (cp == NULL) {
            fds= 0;
          } else {
            fds= buf+strlen(buf) - 1 - cp;
          }
        } else {
          // Integer is given
          number = atoi(str+i);
        }
        done = true;
        break;
      case 'z': // Clear this display
        hasnumber=false;
        dots=0;
        s=false;
        sevenseg[unit]->clear();
        break;
      case 'r': // Raw segment
        outnumtype= SEGMENTS;
        break;
      default: // unknown format, ignore
        break;
    }
  }


  if (hasnumber) {
    if (s) {
      // number is duration in seconds
      int hour = number/60/60;
      int minute = (number/60)%60;

      if (hour) {
        // HH:MM
        number = hour*100 + minute;
      } else {
        // MM:SS
        number = minute*100 + number%60;
      }
    }

    if (outnumtype == HEXADECIMAL) {
      // Hex
      sevenseg[unit]->print(number, HEX);
    } else if (outnumtype == FLOAT) {
      // Floating point
      sevenseg[unit]->printFloat(numberf, fds, 10);
    } else if (outnumtype == SEGMENTS) {
      // Raw segments
      sevenseg[unit]->writeDigitRaw(x, number);
    } else {
      // Decimal
      sevenseg[unit]->print(number, DEC);
    }
  }

  if (dots) {
    sevenseg[unit]->writeDigitRaw(2, dots);
  }

#ifdef USE_DISPLAY_SEVENSEG_COMMON_ANODE
  bufferStuffer(unit);
#endif
  sevenseg[unit]->writeDisplay();
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5
void SevensegTime(boolean time_24)
{

  uint hours = RtcTime.hour;
  uint minutes = RtcTime.minute;
  uint second = RtcTime.second;
  uint16_t displayValue = hours * 100 + minutes;
  uint16_t dots = 0;

  // Do 24 hour to 12 hour format conversion when required.
  if (!time_24) {
    // Handle when hours are past 12 by subtracting 12 hours (1200 value).
    if (hours > 12) {
      displayValue -= 1200;
    }
    // Handle hour 0 (midnight) being shown as 12.
    else if (hours == 0) {
      displayValue += 1200;
    }
  }


  // Now print the time value to the display.
  sevenseg[0]->print(displayValue, DEC);

  // Add zero padding when in 24 hour mode and it's midnight.
  // In this case the print function above won't have leading 0's
  // which can look confusing.  Go in and explicitly add these zeros.
  if (time_24) {
    if (hours == 0) {
      // Pad hour 0.
      sevenseg[0]->writeDigitNum(1, 0);
      // Also pad when the 10's minute is 0 and should be padded.
      if (minutes < 10) {
        sevenseg[0]->writeDigitNum(3, 0);
      }
    }
    if (hours < 10) {
      // Always have 4 digits time
      sevenseg[0]->writeDigitNum(0, 0);
    }
  } else {
    // Identify and display AM/PM
    if (hours >= 12) {
        dots |= 0x10;
    }
  }

  sevenseg[0]->writeDigitRaw(2, dots |= ((second%2) << 1));
#ifdef USE_DISPLAY_SEVENSEG_COMMON_ANODE
  bufferStuffer(0);
#endif
  sevenseg[0]->writeDisplay();
}

void SevensegRefresh(void)  // Every second
{
  if (disp_power) {
    if (Settings.display_mode) {  // Mode 0 is User text
      switch (Settings.display_mode) {
        case 1:  // Time 12
          SevensegTime(false);
          break;
        case 2:  // Time 24
          SevensegTime(true);
          break;
        case 4:  // Mqtt
        case 3:  // Local
        case 5: {  // Mqtt
          SevensegLog();
          break;
        }
      }
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp11(uint8_t function)
{
  if (!I2cEnabled(XI2C_47)) { return false; }

  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    SevensegInitDriver();
  }
  else if (XDSP_11 == Settings.display_model) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_INIT:
        SevensegInit(dsp_init);
        break;
      case FUNC_DISPLAY_CLEAR:
        SevensegClear();
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        SevensegRefresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_POWER:
        SevensegOnOff();
        break;
      case FUNC_DISPLAY_DRAW_STRING:
        SevensegDrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
        break;
      case FUNC_DISPLAY_DIM:
        SevensegDim();
	break;
      case FUNC_DISPLAY_BLINKRATE:
        SevensegBlinkrate();
	break;

    }
  }
  return result;
}

#endif  // USE_DISPLAY_SEVENSEG
#endif  // USE_DISPLAY
#endif  // USE_I2C
