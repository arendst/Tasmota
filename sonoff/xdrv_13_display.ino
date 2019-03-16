/*
  xdrv_13_display.ino - Display support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY

#define XDRV_13                13

#define DISPLAY_MAX_DRIVERS    16           // Max number of display drivers/models supported by xdsp_interface.ino
#define DISPLAY_MAX_COLS       44           // Max number of columns allowed with command DisplayCols
#define DISPLAY_MAX_ROWS       32           // Max number of lines allowed with command DisplayRows

#define DISPLAY_LOG_ROWS       32           // Number of lines in display log buffer

#define D_CMND_DISPLAY "Display"
#define D_CMND_DISP_ADDRESS "Address"
#define D_CMND_DISP_COLS "Cols"
#define D_CMND_DISP_DIMMER "Dimmer"
#define D_CMND_DISP_MODE "Mode"
#define D_CMND_DISP_MODEL "Model"
#define D_CMND_DISP_REFRESH "Refresh"
#define D_CMND_DISP_ROWS "Rows"
#define D_CMND_DISP_SIZE "Size"
#define D_CMND_DISP_FONT "Font"
#define D_CMND_DISP_ROTATE "Rotate"
#define D_CMND_DISP_TEXT "Text"

enum XdspFunctions { FUNC_DISPLAY_INIT_DRIVER, FUNC_DISPLAY_INIT, FUNC_DISPLAY_EVERY_50_MSECOND, FUNC_DISPLAY_EVERY_SECOND,
                     FUNC_DISPLAY_MODEL, FUNC_DISPLAY_MODE, FUNC_DISPLAY_POWER,
                     FUNC_DISPLAY_CLEAR, FUNC_DISPLAY_DRAW_FRAME,
                     FUNC_DISPLAY_DRAW_HLINE, FUNC_DISPLAY_DRAW_VLINE, FUNC_DISPLAY_DRAW_LINE,
                     FUNC_DISPLAY_DRAW_CIRCLE, FUNC_DISPLAY_FILL_CIRCLE,
                     FUNC_DISPLAY_DRAW_RECTANGLE, FUNC_DISPLAY_FILL_RECTANGLE,
                     FUNC_DISPLAY_TEXT_SIZE, FUNC_DISPLAY_FONT_SIZE, FUNC_DISPLAY_ROTATION, FUNC_DISPLAY_DRAW_STRING, FUNC_DISPLAY_ONOFF };

enum DisplayInitModes { DISPLAY_INIT_MODE, DISPLAY_INIT_PARTIAL, DISPLAY_INIT_FULL };

enum DisplayCommands { CMND_DISPLAY, CMND_DISP_MODEL, CMND_DISP_MODE, CMND_DISP_REFRESH, CMND_DISP_DIMMER, CMND_DISP_COLS, CMND_DISP_ROWS,
  CMND_DISP_SIZE, CMND_DISP_FONT, CMND_DISP_ROTATE, CMND_DISP_TEXT, CMND_DISP_ADDRESS };
const char kDisplayCommands[] PROGMEM =
  "|" D_CMND_DISP_MODEL "|" D_CMND_DISP_MODE "|" D_CMND_DISP_REFRESH "|" D_CMND_DISP_DIMMER "|" D_CMND_DISP_COLS "|" D_CMND_DISP_ROWS "|"
  D_CMND_DISP_SIZE "|" D_CMND_DISP_FONT "|" D_CMND_DISP_ROTATE "|" D_CMND_DISP_TEXT "|" D_CMND_DISP_ADDRESS ;

const char S_JSON_DISPLAY_COMMAND_VALUE[] PROGMEM =        "{\"" D_CMND_DISPLAY "%s\":\"%s\"}";
const char S_JSON_DISPLAY_COMMAND_NVALUE[] PROGMEM =       "{\"" D_CMND_DISPLAY "%s\":%d}";
const char S_JSON_DISPLAY_COMMAND_INDEX_NVALUE[] PROGMEM = "{\"" D_CMND_DISPLAY "%s%d\":%d}";

uint8_t disp_power = 0;
uint8_t disp_device = 0;
uint8_t disp_refresh = 1;

int16_t disp_xpos = 0;
int16_t disp_ypos = 0;
uint8_t disp_autodraw = 1;

uint8_t dsp_init;
uint8_t dsp_font;
uint8_t dsp_flag;
uint8_t dsp_on;
uint16_t dsp_x;
uint16_t dsp_y;
uint16_t dsp_x2;
uint16_t dsp_y2;
uint16_t dsp_rad;
uint16_t dsp_color;
int16_t dsp_len;
char *dsp_str;

#ifdef USE_DISPLAY_MODES1TO5

char disp_temp[2];    // C or F
uint8_t disp_subscribed = 0;

char **disp_log_buffer;
uint8_t disp_log_buffer_cols = 0;
uint8_t disp_log_buffer_idx = 0;
uint8_t disp_log_buffer_ptr = 0;

char **disp_screen_buffer;
uint8_t disp_screen_buffer_cols = 0;
uint8_t disp_screen_buffer_rows = 0;

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************/

void DisplayInit(uint8_t mode)
{
  dsp_init = mode;
  XdspCall(FUNC_DISPLAY_INIT);
}

void DisplayClear(void)
{
  XdspCall(FUNC_DISPLAY_CLEAR);
}

void DisplayDrawHLine(uint16_t x, uint16_t y, int16_t len, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_len = len;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_HLINE);
}

void DisplayDrawVLine(uint16_t x, uint16_t y, int16_t len, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_len = len;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_VLINE);
}

void DisplayDrawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_x2 = x2;
  dsp_y2 = y2;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_LINE);
}

void DisplayDrawCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_rad = rad;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_CIRCLE);
}

void DisplayDrawFilledCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_rad = rad;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_FILL_CIRCLE);
}

void DisplayDrawRectangle(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_x2 = x2;
  dsp_y2 = y2;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_RECTANGLE);
}

void DisplayDrawFilledRectangle(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_x2 = x2;
  dsp_y2 = y2;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_FILL_RECTANGLE);
}

void DisplayDrawFrame(void)
{
  XdspCall(FUNC_DISPLAY_DRAW_FRAME);
}

void DisplaySetSize(uint8_t size)
{
  Settings.display_size = size &3;
  XdspCall(FUNC_DISPLAY_TEXT_SIZE);
}

void DisplaySetFont(uint8_t font)
{
  Settings.display_font = font &3;
  XdspCall(FUNC_DISPLAY_FONT_SIZE);
}

void DisplaySetRotation(uint8_t rotation)
{
  Settings.display_rotate = rotation &3;
  XdspCall(FUNC_DISPLAY_ROTATION);
}

void DisplayDrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  dsp_x = x;
  dsp_y = y;
  dsp_str = str;
  dsp_color = color;
  dsp_flag = flag;
  XdspCall(FUNC_DISPLAY_DRAW_STRING);
}

void DisplayOnOff(uint8_t on)
{
  dsp_on = on;
  XdspCall(FUNC_DISPLAY_ONOFF);
}

/*-------------------------------------------------------------------------------------------*/

// get asci number until delimiter and return asci number lenght and value
uint8_t atoiv(char *cp, int16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// get asci number until delimiter and return asci number lenght and value
uint8_t atoiV(char *cp, uint16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if (*cp>='0' && *cp<='9') {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

/*-------------------------------------------------------------------------------------------*/

#define DISPLAY_BUFFER_COLS    128          // Max number of characters in linebuf

void DisplayText(void)
{
  uint8_t lpos;
  uint8_t escape = 0;
  uint8_t var;
  uint8_t font_x = 6;
  uint8_t font_y = 8;
  uint8_t fontnumber = 1;
  int16_t lin = 0;
  int16_t col = 0;
  int16_t fill = 0;
  int16_t temp;
  int16_t temp1;
  uint16_t color = 0;

  char linebuf[DISPLAY_BUFFER_COLS];
  char *dp = linebuf;
  char *cp = XdrvMailbox.data;

  memset(linebuf, ' ', sizeof(linebuf));
  linebuf[sizeof(linebuf)-1] = 0;
  *dp = 0;

  while (*cp) {
    if (!escape) {
      // check for escape
      if (*cp == '[') {
        escape = 1;
        cp++;
        // if string in buffer print it
        if ((uint32_t)dp - (uint32_t)linebuf) {
          if (!fill) { *dp = 0; }
          if (col > 0 && lin > 0) {
            // use col and lin
            DisplayDrawStringAt(col, lin, linebuf, color, 1);
          } else {
            // use disp_xpos, disp_ypos
            DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, color, 0);
          }
          memset(linebuf, ' ', sizeof(linebuf));
          linebuf[sizeof(linebuf)-1] = 0;
          dp = linebuf;
        }
      } else {
        // copy chars
        if (dp < (linebuf + DISPLAY_BUFFER_COLS)) { *dp++ = *cp++; }
      }
    } else {
      // check escapes
      if (*cp == ']') {
        escape = 0;
        cp++;
      } else {
        // analyze escapes
        switch (*cp++) {
          case 'z':
            // clear display
            DisplayClear();
            disp_xpos = 0;
            disp_ypos = 0;
            col = 0;
            lin = 0;
            break;
          case 'i':
            // init display with partial update
            DisplayInit(DISPLAY_INIT_PARTIAL);
            break;
          case 'I':
            // init display with full refresh
            DisplayInit(DISPLAY_INIT_FULL);
            break;
          case 'o':
            DisplayOnOff(0);
            break;
          case 'O':
            DisplayOnOff(1);
            break;
          case 'x':
            // set disp_xpos
            var = atoiv(cp, &disp_xpos);
            cp += var;
            break;
          case 'y':
            // set disp_ypos
            var = atoiv(cp, &disp_ypos);
            cp += var;
            break;
          case 'l':
            // text line lxx
            var = atoiv(cp, &lin);
            cp += var;
            //display.setCursor(display.getCursorX(),(lin-1)*font_y*txtsize);
            break;
          case 'c':
            // text column cxx
            var = atoiv(cp, &col);
            cp += var;
            //display.setCursor((col-1)*font_x*txtsize,display.getCursorY());
            break;
          case 'C':
            // text color cxx
            var = atoiV(cp, &color);
            cp += var;
            break;
          case 'p':
            // pad field with spaces fxx
            var = atoiv(cp, &fill);
            cp += var;
            linebuf[fill] = 0;
            break;
          case 'h':
            // hor line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              DisplayDrawHLine(disp_xpos + temp, disp_ypos, -temp, color);
            } else {
              DisplayDrawHLine(disp_xpos, disp_ypos, temp, color);
            }
            disp_xpos += temp;
            break;
          case 'v':
            // vert line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              DisplayDrawVLine(disp_xpos, disp_ypos + temp, -temp, color);
            } else {
              DisplayDrawVLine(disp_xpos, disp_ypos, temp, color);
            }
            disp_ypos += temp;
            break;
          case 'L':
            // any line to
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            DisplayDrawLine(disp_xpos, disp_ypos, temp, temp1, color);
            disp_xpos += temp;
            disp_ypos += temp1;
            break;
          case 'k':
            // circle
            var = atoiv(cp, &temp);
            cp += var;
            DisplayDrawCircle(disp_xpos, disp_ypos, temp, color);
            break;
          case 'K':
            // filled circle
            var = atoiv(cp, &temp);
            cp += var;
            DisplayDrawFilledCircle(disp_xpos, disp_ypos, temp, color);
            break;
          case 'r':
            // rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            DisplayDrawRectangle(disp_xpos, disp_ypos, temp, temp1, color);
            break;
          case 'R':
            // filled rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, color);
            break;
          case 't':
            if (dp < (linebuf + DISPLAY_BUFFER_COLS) -5) {
              snprintf_P(dp, 6, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute);
              dp += 5;
            }
            break;
          case 'T':
            if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
              snprintf_P(dp, 9, PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year%2000);
              dp += 8;
            }
            break;
          case 'd':
            // force draw grafics buffer
            DisplayDrawFrame();
            break;
          case 'D':
            // set auto draw mode
            disp_autodraw = *cp&1;
            cp += 1;
            break;
          case 's':
            // size sx
            DisplaySetSize(*cp&3);
            cp += 1;
            break;
          case 'f':
            // font sx
            DisplaySetFont(*cp&3);
            cp += 1;
            break;
          case 'a':
            // rotation angle
            DisplaySetRotation(*cp&3);
            cp+=1;
            break;
          default:
            // unknown escape
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("Unknown Escape"));
            goto exit;
            break;
        }
      }
    }
  }
  exit:
  // now draw buffer
  if ((uint32_t)dp - (uint32_t)linebuf) {
    if (!fill) { *dp = 0; }
    if (col > 0 && lin > 0) {
      // use col and lin
      DisplayDrawStringAt(col, lin, linebuf, color, 1);
    } else {
      // use disp_xpos, disp_ypos
      DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, color, 0);
    }
  }
  // draw buffer
  if (disp_autodraw) { DisplayDrawFrame(); }
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void DisplayClearScreenBuffer(void)
{
  if (disp_screen_buffer_cols) {
    for (uint8_t i = 0; i < disp_screen_buffer_rows; i++) {
      memset(disp_screen_buffer[i], 0, disp_screen_buffer_cols);
    }
  }
}

void DisplayFreeScreenBuffer(void)
{
  if (disp_screen_buffer != NULL) {
    for (uint8_t i = 0; i < disp_screen_buffer_rows; i++) {
      if (disp_screen_buffer[i] != NULL) { free(disp_screen_buffer[i]); }
    }
    free(disp_screen_buffer);
    disp_screen_buffer_cols = 0;
    disp_screen_buffer_rows = 0;
  }
}

void DisplayAllocScreenBuffer(void)
{
  if (!disp_screen_buffer_cols) {
    disp_screen_buffer_rows = Settings.display_rows;
    disp_screen_buffer = (char**)malloc(sizeof(*disp_screen_buffer) * disp_screen_buffer_rows);
    if (disp_screen_buffer != NULL) {
      for (uint8_t i = 0; i < disp_screen_buffer_rows; i++) {
        disp_screen_buffer[i] = (char*)malloc(sizeof(*disp_screen_buffer[i]) * (Settings.display_cols[0] +1));
        if (disp_screen_buffer[i] == NULL) {
          DisplayFreeScreenBuffer();
          break;
        }
      }
    }
    if (disp_screen_buffer != NULL) {
      disp_screen_buffer_cols = Settings.display_cols[0] +1;
      DisplayClearScreenBuffer();
    }
  }
}

void DisplayReAllocScreenBuffer(void)
{
  DisplayFreeScreenBuffer();
  DisplayAllocScreenBuffer();
}

void DisplayFillScreen(uint8_t line)
{
  uint8_t len = disp_screen_buffer_cols - strlen(disp_screen_buffer[line]);
  if (len) {
    memset(disp_screen_buffer[line] + strlen(disp_screen_buffer[line]), 0x20, len);
    disp_screen_buffer[line][disp_screen_buffer_cols -1] = 0;
  }
}

/*-------------------------------------------------------------------------------------------*/

void DisplayClearLogBuffer(void)
{
  if (disp_log_buffer_cols) {
    for (uint8_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      memset(disp_log_buffer[i], 0, disp_log_buffer_cols);
    }
  }
}

void DisplayFreeLogBuffer(void)
{
  if (disp_log_buffer != NULL) {
    for (uint8_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      if (disp_log_buffer[i] != NULL) { free(disp_log_buffer[i]); }
    }
    free(disp_log_buffer);
    disp_log_buffer_cols = 0;
  }
}

void DisplayAllocLogBuffer(void)
{
  if (!disp_log_buffer_cols) {
    disp_log_buffer = (char**)malloc(sizeof(*disp_log_buffer) * DISPLAY_LOG_ROWS);
    if (disp_log_buffer != NULL) {
      for (uint8_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
        disp_log_buffer[i] = (char*)malloc(sizeof(*disp_log_buffer[i]) * (Settings.display_cols[0] +1));
        if (disp_log_buffer[i] == NULL) {
          DisplayFreeLogBuffer();
          break;
        }
      }
    }
    if (disp_log_buffer != NULL) {
      disp_log_buffer_cols = Settings.display_cols[0] +1;
      DisplayClearLogBuffer();
    }
  }
}

void DisplayReAllocLogBuffer(void)
{
  DisplayFreeLogBuffer();
  DisplayAllocLogBuffer();
}

void DisplayLogBufferAdd(char* txt)
{
  if (disp_log_buffer_cols) {
    strlcpy(disp_log_buffer[disp_log_buffer_idx], txt, disp_log_buffer_cols);  // This preserves the % sign where printf won't
    disp_log_buffer_idx++;
    if (DISPLAY_LOG_ROWS == disp_log_buffer_idx) { disp_log_buffer_idx = 0; }
  }
}

char* DisplayLogBuffer(char temp_code)
{
  char* result = NULL;
  if (disp_log_buffer_cols) {
    if (disp_log_buffer_idx != disp_log_buffer_ptr) {
      result = disp_log_buffer[disp_log_buffer_ptr];
      disp_log_buffer_ptr++;
      if (DISPLAY_LOG_ROWS == disp_log_buffer_ptr) { disp_log_buffer_ptr = 0; }

      char *pch = strchr(result, '~');  // = 0x7E (~) Replace degrees character (276 octal)
      if (pch != NULL) { result[pch - result] = temp_code; }
    }
  }
  return result;
}

void DisplayLogBufferInit(void)
{
  if (Settings.display_mode) {
    disp_log_buffer_idx = 0;
    disp_log_buffer_ptr = 0;
    disp_refresh = Settings.display_refresh;

    snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%c"), TempUnit());

    DisplayReAllocLogBuffer();

    char buffer[40];
    snprintf_P(buffer, sizeof(buffer), PSTR(D_VERSION " %s%s"), my_version, my_image);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR("Display mode %d"), Settings.display_mode);
    DisplayLogBufferAdd(buffer);

    snprintf_P(buffer, sizeof(buffer), PSTR(D_CMND_HOSTNAME " %s"), my_hostname);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_SSID " %s"), Settings.sta_ssid[Settings.sta_active]);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_MAC " %s"), WiFi.macAddress().c_str());
    DisplayLogBufferAdd(buffer);
    if (!global_state.wifi_down) {
      snprintf_P(buffer, sizeof(buffer), PSTR("IP %s"), WiFi.localIP().toString().c_str());
      DisplayLogBufferAdd(buffer);
      snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_RSSI " %d%%"), WifiGetRssiAsQuality(WiFi.RSSI()));
      DisplayLogBufferAdd(buffer);
    }
  }
}

/*********************************************************************************************\
 * Sensors
\*********************************************************************************************/

enum SensorQuantity {
  JSON_TEMPERATURE,
  JSON_HUMIDITY, JSON_LIGHT, JSON_NOISE, JSON_AIRQUALITY,
  JSON_PRESSURE, JSON_PRESSUREATSEALEVEL,
  JSON_ILLUMINANCE,
  JSON_GAS,
  JSON_YESTERDAY, JSON_TOTAL, JSON_TODAY,
  JSON_PERIOD,
  JSON_POWERFACTOR, JSON_COUNTER, JSON_ANALOG_INPUT, JSON_UV_LEVEL,
  JSON_CURRENT,
  JSON_VOLTAGE,
  JSON_POWERUSAGE,
  JSON_CO2,
  JSON_FREQUENCY };
const char kSensorQuantity[] PROGMEM =
  D_JSON_TEMPERATURE "|"                                                        // degrees
  D_JSON_HUMIDITY "|" D_JSON_LIGHT "|" D_JSON_NOISE "|" D_JSON_AIRQUALITY "|"   // percentage
  D_JSON_PRESSURE "|" D_JSON_PRESSUREATSEALEVEL "|"                             // hPa
  D_JSON_ILLUMINANCE "|"                                                        // lx
  D_JSON_GAS "|"                                                                // kOhm
  D_JSON_YESTERDAY "|" D_JSON_TOTAL "|" D_JSON_TODAY "|"                        // kWh
  D_JSON_PERIOD "|"                                                             // Wh
  D_JSON_POWERFACTOR "|" D_JSON_COUNTER "|" D_JSON_ANALOG_INPUT "|" D_JSON_UV_LEVEL "|"                 // No unit
  D_JSON_CURRENT "|"                                                            // Ampere
  D_JSON_VOLTAGE "|"                                                            // Volt
  D_JSON_POWERUSAGE "|"                                                         // Watt
  D_JSON_CO2 "|"                                                                // ppm
  D_JSON_FREQUENCY ;                                                            // Hz

void DisplayJsonValue(const char* topic, const char* device, const char* mkey, const char* value)
{
  char quantity[TOPSZ];
  char buffer[Settings.display_cols[0] +1];
  char spaces[Settings.display_cols[0]];
  char source[Settings.display_cols[0] - Settings.display_cols[1]];
  char svalue[Settings.display_cols[1] +1];

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("DisplayJsonValue"));
#endif

  memset(spaces, 0x20, sizeof(spaces));
  spaces[sizeof(spaces) -1] = '\0';
  snprintf_P(source, sizeof(source), PSTR("%s%s%s%s"), topic, (strlen(topic))?"/":"", mkey, spaces);  // pow1/Voltage or Voltage if topic is empty (local sensor)

  int quantity_code = GetCommandCode(quantity, sizeof(quantity), mkey, kSensorQuantity);
  if ((-1 == quantity_code) || !strcmp_P(mkey, S_RSLT_POWER)) {  // Ok: Power, Not ok: POWER
    return;
  }
  if (JSON_TEMPERATURE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s~%s"), value, disp_temp);
  }
  else if ((quantity_code >= JSON_HUMIDITY) && (quantity_code <= JSON_AIRQUALITY)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%%"), value);
  }
  else if ((quantity_code >= JSON_PRESSURE) && (quantity_code <= JSON_PRESSUREATSEALEVEL)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_PRESSURE), value);
  }
  else if (JSON_ILLUMINANCE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_LUX), value);
  }
  else if (JSON_GAS == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_KILOOHM), value);
  }
  else if ((quantity_code >= JSON_YESTERDAY) && (quantity_code <= JSON_TODAY)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_KILOWATTHOUR), value);
  }
  else if (JSON_PERIOD == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_WATTHOUR), value);
  }
  else if ((quantity_code >= JSON_POWERFACTOR) && (quantity_code <= JSON_UV_LEVEL)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s"), value);
  }
  else if (JSON_CURRENT == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_AMPERE), value);
  }
  else if (JSON_VOLTAGE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_VOLT), value);
  }
  else if (JSON_POWERUSAGE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_WATT), value);
  }
  else if (JSON_CO2 == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_PARTS_PER_MILLION), value);
  }
  else if (JSON_FREQUENCY == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_HERTZ), value);
  }
  snprintf_P(buffer, sizeof(buffer), PSTR("%s %s"), source, svalue);

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "mkey [%s], source [%s], value [%s], quantity_code %d, log_buffer [%s]"), mkey, source, value, quantity_code, buffer);

  DisplayLogBufferAdd(buffer);
}

void DisplayAnalyzeJson(char *topic, char *json)
{
// //tele/pow2/STATE    {"Time":"2017-09-20T11:53:03", "Uptime":10, "Vcc":3.123, "POWER":"ON", "Wifi":{"AP":2, "SSId":"indebuurt2", "RSSI":68, "APMac":"00:22:6B:FE:8E:20"}}
// //tele/pow2/ENERGY   {"Time":"2017-09-20T11:53:03", "Total":6.522, "Yesterday":0.150, "Today":0.073, "Period":0.5, "Power":12.1, "Factor":0.56, "Voltage":210.1, "Current":0.102}

// tele/pow1/SENSOR = {"Time":"2018-01-02T17:13:17","ENERGY":{"Total":13.091,"Yesterday":0.060,"Today":0.046,"Period":0.2,"Power":9.8,"Factor":0.49,"Voltage":206.8,"Current":0.096}}
// tele/dual/STATE    {"Time":"2017-09-20T11:53:03","Uptime":25,"Vcc":3.178,"POWER1":"OFF","POWER2":"OFF","Wifi":{"AP":2,"SSId":"indebuurt2","RSSI":100,"APMac":"00:22:6B:FE:8E:20"}}
// tele/sc/SENSOR     {"Time":"2017-09-20T11:53:09","Temperature":24.0,"Humidity":16.0,"Light":30,"Noise":20,"AirQuality":100,"TempUnit":"C"}
// tele/rf1/SENSOR    {"Time":"2017-09-20T11:53:23","BH1750":{"Illuminance":57}}
// tele/wemos5/SENSOR {"Time":"2017-09-20T11:53:53","SHT1X":{"Temperature":20.1,"Humidity":58.9},"HTU21":{"Temperature":20.7,"Humidity":58.5},"BMP280":{"Temperature":21.6,"Pressure":1020.3},"TempUnit":"C"}
// tele/th1/SENSOR    {"Time":"2017-09-20T11:54:48","DS18B20":{"Temperature":49.7},"TempUnit":"C"}

  const char *tempunit;

//  char jsonStr[MESSZ];
//  strlcpy(jsonStr, json, sizeof(jsonStr));  // Save original before destruction by JsonObject
  String jsonStr = json;  // Move from stack to heap to fix watchdogs (20180626)

  StaticJsonBuffer<1024> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(jsonStr);
  if (root.success()) {

    tempunit = root[D_JSON_TEMPERATURE_UNIT];
    if (tempunit) {
      snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%s"), tempunit);
//      AddLog_P2(LOG_LEVEL_DEBUG, disp_temp);
    }

    for (JsonObject::iterator it = root.begin(); it != root.end(); ++it) {
      JsonVariant value = it->value;
      if (value.is<JsonObject>()) {
        JsonObject& Object2 = value;
        for (JsonObject::iterator it2 = Object2.begin(); it2 != Object2.end(); ++it2) {
          JsonVariant value2 = it2->value;
          if (value2.is<JsonObject>()) {
            JsonObject& Object3 = value2;
            for (JsonObject::iterator it3 = Object3.begin(); it3 != Object3.end(); ++it3) {
              const char* value = it3->value;
              if (value != nullptr) {  // "DHT11":{"Temperature":null,"Humidity":null} - ignore null as it will raise exception 28
                DisplayJsonValue(topic, it->key, it3->key, value);  // Sensor 56%
              }
            }
          } else {
            const char* value = it2->value;
            if (value != nullptr) {
              DisplayJsonValue(topic, it->key, it2->key, value);  // Sensor  56%
            }
          }
        }
      } else {
        const char* value = it->value;
        if (value != nullptr) {
          DisplayJsonValue(topic, it->key, it->key, value);  // Topic  56%
        }
      }
    }
  }
}

void DisplayMqttSubscribe(void)
{
/* Subscribe to tele messages only
 * Supports the following FullTopic formats
 * - %prefix%/%topic%
 * - home/%prefix%/%topic%
 * - home/level2/%prefix%/%topic% etc.
 */
//  if (Settings.display_mode &0x04) {
  if (Settings.display_model) {

    char stopic[TOPSZ];
    char ntopic[TOPSZ];

    ntopic[0] = '\0';
    strlcpy(stopic, Settings.mqtt_fulltopic, sizeof(stopic));
    char *tp = strtok(stopic, "/");
    while (tp != NULL) {
      if (!strcmp_P(tp, PSTR(MQTT_TOKEN_PREFIX))) {
        break;
      }
      strncat_P(ntopic, PSTR("+/"), sizeof(ntopic) - strlen(ntopic) -1);           // Add single-level wildcards
      tp = strtok(NULL, "/");
    }
    strncat(ntopic, Settings.mqtt_prefix[2], sizeof(ntopic) - strlen(ntopic) -1);  // Subscribe to tele messages
    strncat_P(ntopic, PSTR("/#"), sizeof(ntopic) - strlen(ntopic) -1);             // Add multi-level wildcard
    MqttSubscribe(ntopic);
    disp_subscribed = 1;
  } else {
    disp_subscribed = 0;
  }
}

bool DisplayMqttData(void)
{
  if (disp_subscribed) {
    char stopic[TOPSZ];

    snprintf_P(stopic, sizeof(stopic) , PSTR("%s/"), Settings.mqtt_prefix[2]);  // tele/
    char *tp = strstr(XdrvMailbox.topic, stopic);
    if (tp) {                                                // tele/sonoff/SENSOR
      if (Settings.display_mode &0x04) {
        tp = tp + strlen(stopic);                              // sonoff/SENSOR
        char *topic = strtok(tp, "/");                         // sonoff
        DisplayAnalyzeJson(topic, XdrvMailbox.data);
      }
      return true;
    }
  }
  return false;
}

void DisplayLocalSensor(void)
{
  if ((Settings.display_mode &0x02) && (0 == tele_period)) {
    char no_topic[1] = { 0 };
//    DisplayAnalyzeJson(mqtt_topic, mqtt_data);  // Add local topic
    DisplayAnalyzeJson(no_topic, mqtt_data);    // Discard any topic
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void DisplayInitDriver(void)
{
  XdspCall(FUNC_DISPLAY_INIT_DRIVER);

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Display model %d"), Settings.display_model);

  if (Settings.display_model) {
    devices_present++;
    disp_device = devices_present;

#ifndef USE_DISPLAY_MODES1TO5
    Settings.display_mode = 0;
#else
    DisplayLogBufferInit();
#endif  // USE_DISPLAY_MODES1TO5
  }
}

void DisplaySetPower(void)
{
  disp_power = bitRead(XdrvMailbox.index, disp_device -1);
  if (Settings.display_model) {
    XdspCall(FUNC_DISPLAY_POWER);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool DisplayCommand(void)
{
  char command [CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_DISPLAY);  // Prep for string length change

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_DISPLAY), disp_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic +disp_len, kDisplayCommands);
    if (-1 == command_code) {
      serviced = false;  // Unknown command
    }
    else if (CMND_DISPLAY == command_code) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_DISPLAY "\":{\"" D_CMND_DISP_MODEL "\":%d,\"" D_CMND_DISP_MODE "\":%d,\"" D_CMND_DISP_DIMMER "\":%d,\""
         D_CMND_DISP_SIZE "\":%d,\"" D_CMND_DISP_FONT "\":%d,\"" D_CMND_DISP_ROTATE "\":%d,\"" D_CMND_DISP_REFRESH "\":%d,\"" D_CMND_DISP_COLS "\":[%d,%d],\"" D_CMND_DISP_ROWS "\":%d}}"),
        Settings.display_model, Settings.display_mode, Settings.display_dimmer, Settings.display_size, Settings.display_font, Settings.display_rotate, Settings.display_refresh,
        Settings.display_cols[0], Settings.display_cols[1], Settings.display_rows);
    }
    else if (CMND_DISP_MODEL == command_code) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < DISPLAY_MAX_DRIVERS)) {
        uint8_t last_display_model = Settings.display_model;
        Settings.display_model = XdrvMailbox.payload;
        if (XdspCall(FUNC_DISPLAY_MODEL)) {
          restart_flag = 2;  // Restart to re-init interface and add/Remove MQTT subscribe
        } else {
          Settings.display_model = last_display_model;
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_model);
    }
    else if (CMND_DISP_MODE == command_code) {
#ifdef USE_DISPLAY_MODES1TO5
/*     Matrix               LCD / Oled                           TFT
 * 1 = Text up and time     Time
 * 2 = Date                 Local sensors                        Local sensors
 * 3 = Day                  Local sensors and time               Local sensors and time
 * 4 = Mqtt left and time   Mqtt (incl local) sensors            Mqtt (incl local) sensors
 * 5 = Mqtt up and time     Mqtt (incl local) sensors and time   Mqtt (incl local) sensors and time
*/
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
        uint8_t last_display_mode = Settings.display_mode;
        Settings.display_mode = XdrvMailbox.payload;
        if (!disp_subscribed) {
          restart_flag = 2;  // Restart to Add/Remove MQTT subscribe
        } else {
          if (last_display_mode && !Settings.display_mode) {  // Switch to mode 0
            DisplayInit(DISPLAY_INIT_MODE);
            DisplayClear();
          } else {
//          if (!last_display_mode && Settings.display_mode) {  // Switch to non mode 0
            DisplayLogBufferInit();
            DisplayInit(DISPLAY_INIT_MODE);
          }
        }
      }
#endif  // USE_DISPLAY_MODES1TO5
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_mode);
    }
    else if (CMND_DISP_DIMMER == command_code) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
        Settings.display_dimmer = ((XdrvMailbox.payload +1) * 100) / 666;  // Correction for Domoticz (0 - 15)
        if (Settings.display_dimmer && !(disp_power)) {
          ExecuteCommandPower(disp_device, POWER_ON, SRC_DISPLAY);
        }
        else if (!Settings.display_dimmer && disp_power) {
          ExecuteCommandPower(disp_device, POWER_OFF, SRC_DISPLAY);
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_dimmer);
    }
    else if (CMND_DISP_SIZE == command_code) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
        Settings.display_size = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_size);
    }
    else if (CMND_DISP_FONT == command_code) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
        Settings.display_font = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_font);
    }
    else if (CMND_DISP_ROTATE == command_code) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 4)) {
        if (Settings.display_rotate != XdrvMailbox.payload) {
/*
          // Needs font info regarding height and width
          if ((Settings.display_rotate &1) != (XdrvMailbox.payload &1)) {
            uint8_t temp_rows = Settings.display_rows;
            Settings.display_rows = Settings.display_cols[0];
            Settings.display_cols[0] = temp_rows;
#ifdef USE_DISPLAY_MODES1TO5
            DisplayReAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
          }
*/
          Settings.display_rotate = XdrvMailbox.payload;
          DisplayInit(DISPLAY_INIT_MODE);
#ifdef USE_DISPLAY_MODES1TO5
          DisplayLogBufferInit();
#endif  // USE_DISPLAY_MODES1TO5
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_rotate);
    }
    else if (CMND_DISP_TEXT == command_code) {
      mqtt_data[0] = '\0';
      if (disp_device && XdrvMailbox.data_len > 0) {
#ifndef USE_DISPLAY_MODES1TO5
        DisplayText();
#else
        if (!Settings.display_mode) {
          DisplayText();
        } else {
          DisplayLogBufferAdd(XdrvMailbox.data);
        }
#endif  // USE_DISPLAY_MODES1TO5
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("No Text"));
      }
      if (mqtt_data[0] == '\0') {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_VALUE, command, XdrvMailbox.data);
      }
    }
    else if ((CMND_DISP_ADDRESS == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
        Settings.display_address[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.display_address[XdrvMailbox.index -1]);
    }
    else if (CMND_DISP_REFRESH == command_code) {
      if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 7)) {
        Settings.display_refresh = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_refresh);
    }
    else if ((CMND_DISP_COLS == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_COLS)) {
        Settings.display_cols[XdrvMailbox.index -1] = XdrvMailbox.payload;
#ifdef USE_DISPLAY_MODES1TO5
        if (1 == XdrvMailbox.index) {
          DisplayLogBufferInit();
          DisplayReAllocScreenBuffer();
        }
#endif  // USE_DISPLAY_MODES1TO5
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.display_cols[XdrvMailbox.index -1]);
    }
    else if (CMND_DISP_ROWS == command_code) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_ROWS)) {
        Settings.display_rows = XdrvMailbox.payload;
#ifdef USE_DISPLAY_MODES1TO5
        DisplayLogBufferInit();
        DisplayReAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_rows);
    }
    else serviced = false;  // Unknown command
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv13(uint8_t function)
{
  bool result = false;

  if ((i2c_flg || spi_flg || soft_spi_flg) && XdspPresent()) {
    switch (function) {
      case FUNC_PRE_INIT:
        DisplayInitDriver();
        break;
      case FUNC_EVERY_50_MSECOND:
        if (Settings.display_model) { XdspCall(FUNC_DISPLAY_EVERY_50_MSECOND); }
        break;
      case FUNC_COMMAND:
        result = DisplayCommand();
        break;
      case FUNC_SET_POWER:
        DisplaySetPower();
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_EVERY_SECOND:
        if (Settings.display_model && Settings.display_mode) { XdspCall(FUNC_DISPLAY_EVERY_SECOND); }
        break;
      case FUNC_MQTT_SUBSCRIBE:
        DisplayMqttSubscribe();
        break;
      case FUNC_MQTT_DATA:
        result = DisplayMqttData();
        break;
      case FUNC_SHOW_SENSOR:
        DisplayLocalSensor();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
