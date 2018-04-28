/*
  xdrv_06_display.ino - display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
/*********************************************************************************************\
 * Display          Libraries needed
 * ---------------  ----------------------------------------
 * I2C LCD          LiquidCrystal_I2C
 * I2C SSD 1306     Adafruit_SSD1306 and Adafruit_GFX
 * I2C 8x8 Matrix   Adafruit_LED_Backpack and Adafruit_GFX
 * SPI TFT ILI9341  TasmotaTFT and Adafruit_GFX
 * SPI TFT ILI9481  TasmotaTFT and Adafruit_GFX
\*********************************************************************************************/

#define LCD_ADDRESS1           0x27         // LCD I2C address option 1
#define LCD_ADDRESS2           0x3F         // LCD I2C address option 2

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define DISPLAY_BUFFER_COLS    40           // Max number of columns in log buffer and display shadow buffer (needed for LCD and Oled)
#define DISPLAY_BUFFER_ROWS    8            // Max number of lines in display shadow buffer (needed for LCD and Oled)

#define DISPLAY_LOG_ROWS       32           // Number of lines in display log buffer

enum display_types { DISP_NONE, DISP_LCD, DISP_OLED, DISP_MATRIX, DISP_TFT, DISP_TFT4, DISP_MAX };

enum DisplayCommands { CMND_DISP_MODEL, CMND_DISP_MODE, CMND_DISP_REFRESH, CMND_DISP_DIMMER, CMND_DISP_COLS, CMND_DISP_ROWS,
  CMND_DISP_SIZE, CMND_DISP_TEXT, CMND_DISP_ADDRESS };
const char kDisplayCommands[] PROGMEM =
  D_CMND_DISP_MODEL "|" D_CMND_DISP_MODE "|" D_CMND_DISP_REFRESH "|" D_CMND_DISP_DIMMER "|" D_CMND_DISP_COLS "|" D_CMND_DISP_ROWS "|"
  D_CMND_DISP_SIZE "|" D_CMND_DISP_TEXT "|" D_CMND_DISP_ADDRESS ;

const char S_JSON_DISPLAY_COMMAND_VALUE[] PROGMEM =        "{\"" D_CMND_DISPLAY "%s\":\"%s\"}";
const char S_JSON_DISPLAY_COMMAND_NVALUE[] PROGMEM =       "{\"" D_CMND_DISPLAY "%s\":%d}";
const char S_JSON_DISPLAY_COMMAND_INDEX_NVALUE[] PROGMEM = "{\"" D_CMND_DISPLAY "%s%d\":%d}";

#define DISPLAY_LOG_COLS       DISPLAY_BUFFER_COLS +1  // Number of characters in display log buffer line +1

#ifdef USE_I2C  // ---------------------------------------------------------------------------
#include <Adafruit_LEDBackpack.h>  // 8x8 Matrix
#include <LiquidCrystal_I2C.h>     // LCD
#include <Adafruit_SSD1306.h>      // Oled

LiquidCrystal_I2C *lcd;
Adafruit_SSD1306 *oled;

//char disp_screen_buffer[Settings.display_rows][Settings.display_cols[0] +1];
char disp_screen_buffer[DISPLAY_BUFFER_ROWS][DISPLAY_BUFFER_COLS +1];

Adafruit_8x8matrix *matrix[8];
uint8_t mtx_matrices = 0;
uint8_t mtx_state = 0;
uint8_t mtx_counter = 0;
int16_t mtx_x = 0;
int16_t mtx_y = 0;

#endif  // USE_I2C ---------------------------------------------------------------------------

#ifdef USE_SPI  // ---------------------------------------------------------------------------
#define TFT_TOP                16
#define TFT_BOTTOM             16
#define TFT_FONT_WIDTH         6
#define TFT_FONT_HEIGTH        8     // Adafruit minimal font heigth pixels

#include <TasmotaTFT.h>                // TFT 320x240 and 480x320
TasmotaTFT *tft;

//#include <Adafruit_ILI9341.h>
//Adafruit_ILI9341 *tft;

uint16_t tft_scroll;

#endif  // USE_SPI ---------------------------------------------------------------------------

char disp_log_buffer[DISPLAY_LOG_ROWS][DISPLAY_LOG_COLS];
char disp_temp[2];    // C or F
char disp_time[9];    // 13:45:43
uint8_t disp_log_buffer_idx = 0;
uint8_t disp_log_buffer_ptr = 0;
bool disp_log_buffer_active = false;
uint8_t disp_model = DISP_NONE;
uint8_t disp_refresh = 1;
uint8_t disp_second = 0;
uint8_t disp_mode = 1;
uint8_t disp_power = 0;
uint8_t disp_device = 0;
uint8_t disp_subscribed = 0;

void DisplayLogBufferIdxInc()
{
  char *pch = strchr(disp_log_buffer[disp_log_buffer_idx],'~');                         // = 0x7E (~) Replace degrees character (276 octal)
  if (pch != NULL) {
    switch (disp_model) {
      case DISP_LCD:
        disp_log_buffer[disp_log_buffer_idx][pch - disp_log_buffer[disp_log_buffer_idx]] = '\337';    // = 0xDF
        break;
      case DISP_OLED:
      case DISP_MATRIX:
      case DISP_TFT:
      case DISP_TFT4:
        disp_log_buffer[disp_log_buffer_idx][pch - disp_log_buffer[disp_log_buffer_idx]] = '\370';  // = 0xF8
        break;
    }
  }

  disp_log_buffer_idx++;
  if (DISPLAY_LOG_ROWS == disp_log_buffer_idx) {
    disp_log_buffer_idx = 0;
  }
}

void DisplayLogBufferPtrInc()
{
  disp_log_buffer_ptr++;
  if (DISPLAY_LOG_ROWS == disp_log_buffer_ptr) {
    disp_log_buffer_ptr = 0;
  }
}

#ifdef USE_I2C  // ---------------------------------------------------------------------------
void DisplayScreenBuffer()
{
  uint8_t last_row = Settings.display_rows -1;

  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    disp_log_buffer_active = (disp_log_buffer_idx != disp_log_buffer_ptr);
    if (disp_log_buffer_active) {
      if (DISP_OLED == disp_model) {
        oled->clearDisplay();
        oled->setTextSize(Settings.display_size);
        oled->setCursor(0,0);
      }
      for (byte i = 0; i < last_row; i++) {
        strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], sizeof(disp_screen_buffer[i]));
        if (DISP_LCD == disp_model) {
          lcd->setCursor(0, i);            // Col 0, Row i
          lcd->print(disp_screen_buffer[i +1]);
        }
        else if (DISP_OLED == disp_model) {
          oled->println(disp_screen_buffer[i]);
        }
      }
      strlcpy(disp_screen_buffer[last_row], disp_log_buffer[disp_log_buffer_ptr], sizeof(disp_screen_buffer[last_row]));

      // Fill with spaces
      byte len = sizeof(disp_screen_buffer[last_row]) - strlen(disp_screen_buffer[last_row]);
      if (len) {
        memset(disp_screen_buffer[last_row] + strlen(disp_screen_buffer[last_row]), 0x20, len);
        disp_screen_buffer[last_row][sizeof(disp_screen_buffer[last_row])-1] = 0;
      }

      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);
      AddLog(LOG_LEVEL_DEBUG);

      if (DISP_LCD == disp_model) {
        lcd->setCursor(0, last_row);
        lcd->print(disp_screen_buffer[last_row]);
      }
      else if (DISP_OLED == disp_model) {
        oled->println(disp_screen_buffer[last_row]);
        oled->display();
      }
      DisplayLogBufferPtrInc();
    }
  }
}

// -------------------------------------------------------------------------------------------

void DisplayMatrixInit()
{
  mtx_state = 1;
  for (mtx_matrices = 0; mtx_matrices < 8; mtx_matrices++) {
    if (Settings.display_address[mtx_matrices]) {
      matrix[mtx_matrices] = new Adafruit_8x8matrix();
      matrix[mtx_matrices]->begin(Settings.display_address[mtx_matrices]);
      matrix[mtx_matrices]->setRotation(1);
      matrix[mtx_matrices]->setBrightness(Settings.display_dimmer);
      matrix[mtx_matrices]->blinkRate(0);               // 0 - 3
      matrix[mtx_matrices]->setTextWrap(false);         // Allow text to run off edges
//      matrix[mtx_matrices]->setTextSize(Settings.display_size);
//      matrix[mtx_matrices]->setTextColor(LED_RED);
      matrix[mtx_matrices]->cp437(true);
    } else {
      break;
    }
  }
  DisplayMatrixClear();
}

void DisplayMatrixWrite()
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->writeDisplay();
  }
}

void DisplayMatrixClear()
{
  for (byte i = 0; i < mtx_matrices; i++) {
     matrix[i]->clear();
  }
  DisplayMatrixWrite();
}
/*
void DisplayMatrixAll()  // On based on Text value (1 - 6)
{
  int value = atoi(Settings.text);
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
    if (i < value) {
      matrix[i]->fillRect(0,0, 8,8, LED_ON);
    }
    matrix[i]->setBrightness(Settings.display_dimmer);
  }
  DisplayMatrixWrite();
}

void DisplayMatrixAllOn()
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
    matrix[i]->fillRect(0,0, 8,8, LED_ON);
    matrix[i]->setBrightness(Settings.display_dimmer);
  }
  DisplayMatrixWrite();
}
*/
void DisplayMatrixFixed(char* txt)
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
    matrix[i]->setCursor(-i *8, 0);
    matrix[i]->print(txt);
    matrix[i]->setBrightness(Settings.display_dimmer);
  }
  DisplayMatrixWrite();
}

void DisplayMatrixCenter(char* txt)
{
  int offset;

  int len = strlen(txt);
  offset = (len < 8) ? offset = ((mtx_matrices *8) - (len *6)) / 2 : 0;
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
    matrix[i]->setCursor(-(i *8)+offset, 0);
    matrix[i]->print(txt);
    matrix[i]->setBrightness(Settings.display_dimmer);
  }
  DisplayMatrixWrite();
}

void DisplayMatrixScrollLeft(char* txt, int loop)
{
  switch (mtx_state) {
  case 1:
    mtx_state = 2;
    // Horiz. position of text -- starts off right edge
    mtx_x = 8 * mtx_matrices;

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "[%s]"), txt);
    AddLog(LOG_LEVEL_DEBUG);

    disp_refresh = Settings.display_refresh;
  case 2:
  disp_refresh--;
    if (!disp_refresh) {
      disp_refresh = Settings.display_refresh;
      for (byte i = 0; i < mtx_matrices; i++) {
        matrix[i]->clear();
        matrix[i]->setCursor(mtx_x - i *8, 0);
        matrix[i]->print(txt);
        matrix[i]->setBrightness(Settings.display_dimmer);
      }
      DisplayMatrixWrite();
      // Move text position left by 1 pixel.
      mtx_x--;
      int16_t len = strlen(txt);
      if (mtx_x < -(len *6)) {
        mtx_state = loop;
      }
    }
    break;
  }
}

void DisplayMatrixScrollUp(char* txt, int loop)
{
  int wordcounter = 0;
  char tmpbuf[200];
  char *words[100];
//  char separators[] = " ,.;:!?";
//  char separators[] = " ";
//  char separators[] = " /|";
  char separators[] = " /";

  switch (mtx_state) {
  case 1:
    mtx_state = 2;
    // Vertical position of text -- starts off left bottom edge
    mtx_y = 8;
    mtx_counter = 0;
    disp_refresh = Settings.display_refresh;
  case 2:
    disp_refresh--;
    if (!disp_refresh) {
      disp_refresh = Settings.display_refresh;
      strlcpy(tmpbuf, txt, sizeof(tmpbuf));
      char *p = strtok(tmpbuf, separators);
      while (p != NULL && wordcounter < 40) {
        words[wordcounter++] = p;
        p = strtok(NULL, separators);
      }
      for (byte i = 0; i < mtx_matrices; i++) {
        matrix[i]->clear();
        for (byte j = 0; j < wordcounter; j++) {
          matrix[i]->setCursor(-i *8, mtx_y + (j *8));
          matrix[i]->println(words[j]);
        }
        matrix[i]->setBrightness(Settings.display_dimmer);
      }
      DisplayMatrixWrite();
      if (((mtx_y %8) == 0) && mtx_counter) {
        mtx_counter--;
      } else {
        mtx_y--;                   // Move text position up by 1 pixel.
        mtx_counter = STATES * 1;  // Hold text for 1 seconds
      }
      if (mtx_y < -(wordcounter *8)) {
        mtx_state = loop;
      }
    }
    break;
  }
}

void DisplayMatrixBufferScroll(uint8_t direction)
{
  if (disp_log_buffer_idx != disp_log_buffer_ptr) {
    if (!mtx_state) {
      mtx_state = 1;
    }
    if (direction) {
      DisplayMatrixScrollUp(disp_log_buffer[disp_log_buffer_ptr], 0);
    } else {
      DisplayMatrixScrollLeft(disp_log_buffer[disp_log_buffer_ptr], 0);
    }
    if (!mtx_state) {
      DisplayLogBufferPtrInc();
    }
  } else {
    DisplayMatrixFixed(disp_time);
  }
}

// -------------------------------------------------------------------------------------------

void DisplayLcdInit()
{
  lcd = new LiquidCrystal_I2C(Settings.display_address[0], Settings.display_cols[0], Settings.display_rows);
  lcd->init();
  lcd->clear();
  memset(disp_screen_buffer[Settings.display_rows -1], 0x20, Settings.display_cols[0]);
  disp_screen_buffer[Settings.display_rows -1][Settings.display_cols[0]] = 0;
}

void DisplayLcdCenter(byte row, char* txt)
{
  int offset;
  int len;
  char line[Settings.display_cols[0] +2];

  memset(line, 0x20, Settings.display_cols[0]);
  line[Settings.display_cols[0]] = 0;
  len = strlen(txt);
  offset = (len < Settings.display_cols[0]) ? offset = (Settings.display_cols[0] - len) / 2 : 0;
  strncpy(line +offset, txt, len);
  lcd->setCursor(0, row);
  lcd->print(line);
}

void DisplayLcdTime()
{
  char line[Settings.display_cols[0] +1];

  snprintf_P(line, sizeof(line), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
  DisplayLcdCenter(0, line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);
  DisplayLcdCenter(1, line);
}

void DisplayLcdBufferOrTime()
{
  DisplayScreenBuffer();
  if (!disp_log_buffer_active) {
    DisplayLcdTime();
  }
}

// -------------------------------------------------------------------------------------------

void DisplayOledInit()
{
  oled = new Adafruit_SSD1306();
  oled->begin(SSD1306_SWITCHCAPVCC, Settings.display_address[0]);
  oled->invertDisplay(false);
  oled->clearDisplay();
  oled->setTextWrap(false);         // Allow text to run off edges
  oled->cp437(true);

  oled->setTextSize(Settings.display_size);
  oled->setTextColor(WHITE);
  oled->setCursor(0,0);
  oled->display();

//  memset(disp_screen_buffer[Settings.display_rows -1], 0x20, Settings.display_cols);
//  disp_screen_buffer[Settings.display_rows -1][Settings.display_cols[0]] = 0;
}

void DisplayOledDisplayOnOff(byte state)
{
  if (state) {
    oled->ssd1306_command(SSD1306_DISPLAYON);
  } else {
    oled->ssd1306_command(SSD1306_DISPLAYOFF);
  }
  oled->display();
}

void DisplayOledTime()
{
  char line[12];

  oled->clearDisplay();
  oled->setTextSize(2);
  oled->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  oled->println(line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  oled->println(line);
  oled->display();
}

#endif  // USE_I2C ---------------------------------------------------------------------------
#ifdef USE_SPI  // ---------------------------------------------------------------------------
void DisplayTftInit()
{
  if (DISP_TFT == disp_model) {
    tft = new TasmotaTFT(ILI9341, pin[GPIO_SPI_CS], pin[GPIO_SPI_DC]);
//    tft = new Adafruit_ILI9341(pin[GPIO_SPI_CS], pin[GPIO_SPI_DC]);
  }
  else if (DISP_TFT4 == disp_model) {
    tft = new TasmotaTFT(ILI9481, pin[GPIO_SPI_CS], pin[GPIO_SPI_DC]);
  }
  tft->begin();
  tft->setRotation(0);
  tft->invertDisplay(0);
  tft->fillScreen(TFT_BLACK);
  tft->setTextWrap(false);         // Allow text to run off edges
  tft->cp437(true);

  tft->setScrollMargins(TFT_TOP, TFT_BOTTOM);
  tft->setCursor(0, 0);
  tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  tft->setTextSize(2);
  tft->println("HEADER");

  tft_scroll = TFT_TOP;
}

void DisplayTftDisplayOnOff(byte state)
{
//  tft->showDisplay(state);
//  tft->invertDisplay(state);
  if (pin[GPIO_BACKLIGHT] < 99) {
    pinMode(pin[GPIO_BACKLIGHT], OUTPUT);
    digitalWrite(pin[GPIO_BACKLIGHT], state);
  }
}

void DisplayTftPrint(byte size, char *txt)
{
  uint16_t theight;

  tft->setCursor(0, tft_scroll);
  tft->setTextSize(size);
  theight = size * TFT_FONT_HEIGTH;
  tft->fillRect(0, tft_scroll, tft->width(), theight, TFT_BLACK);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION "[%s]"), txt);
  AddLog(LOG_LEVEL_DEBUG);

  tft->print(txt);
  tft_scroll += theight;
  if (tft_scroll >= (tft->height() - TFT_BOTTOM)) {
    tft_scroll = TFT_TOP;
  }
  tft->setScrollStart(tft_scroll);
}

void DisplayTftBuffer()
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    disp_log_buffer_active = (disp_log_buffer_idx != disp_log_buffer_ptr);
    if (disp_log_buffer_active) {
      DisplayTftPrint(Settings.display_size, disp_log_buffer[disp_log_buffer_ptr]);
      DisplayLogBufferPtrInc();
    }
  }
}
#endif  // USE_SPI ---------------------------------------------------------------------------

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
  JSON_CO2 };
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
  D_JSON_CO2 ;                                                                  // ppm

void DisplayJsonValue(const char *topic, const char* device, const char* mkey, const char* value)
{
  char quantity[TOPSZ];
  char spaces[Settings.display_cols[0]];
  char source[Settings.display_cols[0] - Settings.display_cols[1]];
  char svalue[Settings.display_cols[1] +1];

  memset(spaces, 0x20, sizeof(spaces));
  spaces[sizeof(spaces) -1] = '\0';
  snprintf_P(source, sizeof(source), PSTR("%s/%s%s"), topic, mkey, (DISP_MATRIX == Settings.display_model) ? "" : spaces);  // pow1/Voltage

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
  snprintf_P(disp_log_buffer[disp_log_buffer_idx], sizeof(disp_log_buffer[disp_log_buffer_idx]), PSTR("%s %s"), source, svalue);

//  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "mkey [%s], source [%s], value [%s], quantity_code %d, log_buffer [%s]"), mkey, source, value, quantity_code, disp_log_buffer[disp_log_buffer_idx]);
//  AddLog(LOG_LEVEL_DEBUG);

  DisplayLogBufferIdxInc();
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

  char jsonStr[MESSZ];
  const char *tempunit;

  strlcpy(jsonStr, json, sizeof(jsonStr));  // Save original before destruction by JsonObject
  StaticJsonBuffer<400> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(jsonStr);
  if (root.success()) {

    tempunit = root[D_JSON_TEMPERATURE_UNIT];
    if (tempunit) {
      snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%s"), tempunit);
//      snprintf_P(log_data, sizeof(log_data), disp_temp);
//      AddLog(LOG_LEVEL_DEBUG);
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
              DisplayJsonValue(topic, it->key, it3->key, it3->value.as<const char*>());  // Sensor 56%
            }
          } else {
            DisplayJsonValue(topic, it->key, it2->key, it2->value.as<const char*>());  // Sensor  56%
          }
        }
      } else {
        DisplayJsonValue(topic, it->key, it->key, it->value.as<const char*>());  // Topic  56%
      }
    }
  }
}

/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void DisplayInit()
{
  if (!Settings.display_model) {
#ifdef USE_I2C
    if (i2c_flg) {
      if (I2cDevice(LCD_ADDRESS1)) {
        Settings.display_address[0] = LCD_ADDRESS1;
        Settings.display_model = DISP_LCD;
      }
      else if (I2cDevice(LCD_ADDRESS2)) {
        Settings.display_address[0] = LCD_ADDRESS2;
        Settings.display_model = DISP_LCD;
      }
      else if (I2cDevice(OLED_ADDRESS1)) {
        Settings.display_address[0] = OLED_ADDRESS1;
        Settings.display_model = DISP_OLED;
      }
      else if (I2cDevice(OLED_ADDRESS2)) {
        Settings.display_address[0] = OLED_ADDRESS2;
        Settings.display_model = DISP_OLED;
      }
      else if (I2cDevice(Settings.display_address[1])) {
        Settings.display_model = DISP_MATRIX;
      }
    }
#endif  // USE_I2C
#ifdef USE_SPI
    if (spi_flg) {
      Settings.display_model = DISP_TFT;
    }
#endif  // USE_SPI
  }
  disp_model = Settings.display_model;

//  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "Display model %d"), disp_model);
//  AddLog(LOG_LEVEL_DEBUG);

  if (disp_model) {
    devices_present++;
    disp_device = devices_present;
    snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%c"), TempUnit());
    disp_log_buffer_idx = 0;
    disp_log_buffer_ptr = 0;
    disp_log_buffer_active = false;
    disp_refresh = Settings.display_refresh;
    disp_mode = Settings.display_mode;

#ifdef USE_I2C
    if (DISP_LCD == disp_model) {
      DisplayLcdInit();
    }
    else if (DISP_OLED == disp_model) {
      DisplayOledInit();
    }
    else if (DISP_MATRIX == disp_model) {
      DisplayMatrixInit();
    }
#endif  // USE_I2C
#ifdef USE_SPI
    if ((DISP_TFT == disp_model) || (DISP_TFT4 == disp_model)) {
      DisplayTftInit();
    }
#endif
    snprintf_P(disp_log_buffer[disp_log_buffer_idx], sizeof(disp_log_buffer[disp_log_buffer_idx]), PSTR(D_VERSION " %s"), my_version);
    DisplayLogBufferIdxInc();
    snprintf_P(disp_log_buffer[disp_log_buffer_idx], sizeof(disp_log_buffer[disp_log_buffer_idx]), PSTR("Display mode %d"), disp_mode);
    DisplayLogBufferIdxInc();
  }
}

void DisplaySetPower()
{
//  disp_power = XdrvMailbox.index;
  disp_power = bitRead(XdrvMailbox.index, disp_device -1);
  if (disp_model) {
    if (disp_power) {
#ifdef USE_I2C
      if (DISP_LCD == disp_model) {
        lcd->backlight();
      }
      else if (DISP_OLED == disp_model) {
        DisplayOledDisplayOnOff(1);
      }
#endif
#ifdef USE_SPI
      if ((DISP_TFT == disp_model) || (DISP_TFT4 == disp_model)) {
        DisplayTftDisplayOnOff(1);
      }
#endif
    } else {
#ifdef USE_I2C
      if (DISP_LCD == disp_model) {
        lcd->noBacklight();
      }
      else if (DISP_OLED == disp_model) {
        DisplayOledDisplayOnOff(0);
      }
#endif
#ifdef USE_SPI
      if ((DISP_TFT == disp_model) || (DISP_TFT4 == disp_model)) {
        DisplayTftDisplayOnOff(0);
      }
#endif
    }
  }
}

void DisplayMqttSubscribe()
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
      strncat_P(ntopic, PSTR("+/"), sizeof(ntopic));           // Add single-level wildcards
      tp = strtok(NULL, "/");
    }
    strncat(ntopic, Settings.mqtt_prefix[2], sizeof(ntopic));  // Subscribe to tele messages
    strncat_P(ntopic, PSTR("/#"), sizeof(ntopic));             // Add multi-level wildcard
    MqttSubscribe(ntopic);
    disp_subscribed = 1;
  } else {
    disp_subscribed = 0;
  }
}

boolean DisplayMqttData()
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

void DisplayLocalSensor()
{
  if ((Settings.display_mode &0x02) && (0 == tele_period)) {
    DisplayAnalyzeJson(mqtt_topic, mqtt_data);
  }
}

void DisplayRefresh()  // Every 0.05 second
{
  snprintf_P(disp_time, sizeof(disp_time), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);

  disp_second++;
  if (disp_second >= STATES) {
    disp_second = 0;

#ifdef USE_I2C
    if (DISP_LCD == disp_model) {
      switch (disp_mode) {
        case 0:
          lcd->clear();
          break;
        case 1:
          DisplayLcdTime();
          break;
        case 2:
        case 4:
          DisplayScreenBuffer();
          break;
        case 3:
        case 5:
          DisplayLcdBufferOrTime();
          break;
      }
    }
    else if (DISP_OLED == disp_model) {
      switch (disp_mode) {
  //      case 0:
  //        oled->clearDisplay();
  //        oled->display();
  //        break;
        case 1:
          DisplayOledTime();
          break;
        case 0:  // Text only
        case 2:  // Local
        case 3:  // Local
        case 4:  // Mqtt
        case 5:  // Mqtt
          DisplayScreenBuffer();
          break;
      }
    }
#endif
#ifdef USE_SPI
    if (DISP_TFT == disp_model) {
      char tftdt[21];
      char disp_date4[11];  // 24-04-2017

      snprintf_P(disp_date4, sizeof(disp_date4), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);

      tft->setTextSize(2);
      tft->setTextColor(TFT_YELLOW, TFT_BLACK);   // Add background color to solve flicker
      tft->setCursor(0, 0);
      snprintf_P(tftdt, sizeof(tftdt), PSTR("%s  %s"), disp_date4, disp_time);
      tft->print(tftdt);
      switch (disp_mode) {
        case 0:  // Text
        case 2:  // Local
        case 3:  // Local
        case 4:  // Mqtt
        case 5:  // Mqtt
          tft->setTextColor(TFT_CYAN, TFT_BLACK);   // Add background color to solve flicker
          DisplayTftBuffer();
          break;
      }
    }
#endif
  }

#ifdef USE_I2C
  if (DISP_MATRIX == disp_model) {
    if (!disp_power) {
      DisplayMatrixClear();
    } else {
      char disp_date[9];    // 24-04-17
      char disp_day[10];    // Mon

      snprintf_P(disp_date, sizeof(disp_date), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year -2000);
      snprintf_P(disp_day, sizeof(disp_day), PSTR("%d %s"), RtcTime.day_of_month, RtcTime.name_of_month);

      switch (disp_mode) {
        case 0:
//          DisplayMatrixScrollLeft(Settings.text, Settings.loop);
        case 2:
          DisplayMatrixFixed(disp_date);
          break;
        case 3:
          DisplayMatrixCenter(disp_day);
          break;
        case 4:
          DisplayMatrixBufferScroll(0);
          break;
        case 1:  // Time and user text
        case 5:  // Time, user text and MQTT
          DisplayMatrixBufferScroll(1);
          break;
//        case 8:
//          DisplayMatrixAllOn();
//          break;
//        case 9:
//          DisplayMatrixAll();
//          break;
      }
    }
  }
#endif
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean DisplayCommand()
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t disp_len = strlen(D_CMND_DISPLAY);  // Prep for string length change

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_DISPLAY), disp_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic +disp_len, kDisplayCommands);
    if (CMND_DISP_MODEL == command_code) {
      if ((XdrvMailbox.payload >= DISP_NONE) && (XdrvMailbox.payload < DISP_MAX)) {
        Settings.display_model = XdrvMailbox.payload;
        restart_flag = 2;  // Restart to re-init interface and add/Remove MQTT subscribe
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_model);
    }
    else if (CMND_DISP_MODE == command_code) {
/*
 *     Matrix               LCD / Oled                           TFT
 * 0 =                      Clear display
 * 1 = Text up and time     Time
 * 2 = Date                 Local sensors                        Local sensors
 * 3 = Day                  Local sensors and time               Local sensors and time
 * 4 = Mqtt left and time   Mqtt (incl local) sensors            Mqtt (incl local) sensors
 * 5 = Mqtt up and time     Mqtt (incl local) sensors and time   Mqtt (incl local) sensors and time
*/
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
        Settings.display_mode = XdrvMailbox.payload;
//        if ((disp_mode &0x04) != (Settings.display_mode &0x04)) {
        if (!disp_subscribed) {
          restart_flag = 2;  // Restart to Add/Remove MQTT subscribe
        }
        disp_mode = Settings.display_mode;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_mode);
    }
    else if (CMND_DISP_REFRESH == command_code) {
      if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 7)) {
        Settings.display_refresh = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_refresh);
    }
    else if ((CMND_DISP_COLS == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_BUFFER_COLS)) {
        Settings.display_cols[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.display_cols[XdrvMailbox.index -1]);
    }
    else if (CMND_DISP_DIMMER == command_code) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
        Settings.display_dimmer = ((XdrvMailbox.payload +1) * 100) / 666;  // Correction for Domoticz (0 - 15)
        if (Settings.display_dimmer && !(disp_power)) {
          ExecuteCommandPower(disp_device, POWER_ON);
        }
        else if (!Settings.display_dimmer && disp_power) {
          ExecuteCommandPower(disp_device, POWER_OFF);
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_dimmer);
    }
    else if (CMND_DISP_ROWS == command_code) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_BUFFER_ROWS)) {
        Settings.display_rows = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_rows);
    }
    else if (CMND_DISP_SIZE == command_code) {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
        Settings.display_size = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_NVALUE, command, Settings.display_size);
    }
    else if (CMND_DISP_TEXT == command_code) {
      if (XdrvMailbox.data_len > 0) {

        // Here display command manipulation could take place like textsize, color, position etc. using intext parameters
        // Currently just adds to the logbuffer

        strlcpy(disp_log_buffer[disp_log_buffer_idx], XdrvMailbox.data, sizeof(disp_log_buffer[disp_log_buffer_idx]));
        DisplayLogBufferIdxInc();
      } else {
        snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("Text too long"));
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_VALUE, command, XdrvMailbox.data);
    }
    else if ((CMND_DISP_ADDRESS == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
        Settings.display_address[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.display_address[XdrvMailbox.index -1]);
    }
    else serviced = false;
  }
  else serviced = false;
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_06

boolean Xdrv06(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_INIT:
      if (i2c_flg || spi_flg) {
        DisplayInit();
      }
      break;
    case FUNC_EVERY_50_MSECOND:
      DisplayRefresh();
      break;
    case FUNC_COMMAND:
      result = DisplayCommand();
      break;
    case FUNC_MQTT_SUBSCRIBE:
      DisplayMqttSubscribe();
      break;
    case FUNC_MQTT_DATA:
      result = DisplayMqttData();
      break;
    case FUNC_SET_POWER:
      DisplaySetPower();
      break;
    case FUNC_SHOW_SENSOR:
      DisplayLocalSensor();
      break;
  }
  return result;
}

#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
