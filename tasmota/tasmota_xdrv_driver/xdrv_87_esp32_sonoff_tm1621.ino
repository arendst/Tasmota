/*
  xdrv_87_esp32_sonoff_tm1621.ino - Sonoff POWR3xxD and THR3xxD display support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef USE_DISPLAY_TM1621_SONOFF
/*********************************************************************************************\
 * Sonoff POWR3xxD, POWCT and THR3xxD LCD support
 *
 * {"NAME":"Sonoff POWR316D","GPIO":[32,0,0,0,0,576,0,0,0,224,9280,0,3104,0,320,0,0,0,0,0,0,9184,9248,9216,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff POWR320D","GPIO":[32,0,9313,0,9312,576,0,0,0,0,9280,0,3104,0,320,0,0,0,0,0,0,9184,9248,9216,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff THR316D","GPIO":[32,0,0,0,225,9280,0,0,0,321,0,576,320,9184,9216,0,0,224,0,9248,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff THR316D GPIO26","GPIO":[32,0,0,0,225,9280,0,0,0,321,0,576,320,9184,9216,0,0,224,0,9248,0,1,1,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff THR320D","GPIO":[32,0,0,0,226,9280,0,0,0,321,0,576,320,9184,9216,9312,0,0,9313,9248,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff THR320D GPIO26","GPIO":[32,0,0,0,226,9280,0,0,0,321,0,576,320,9184,9216,9312,0,0,9313,9248,0,1,1,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff POWCT","GPIO":[32,0,0,0,0,9280,0,0,0,320,0,576,0,9184,9216,0,0,224,0,9248,0,7329,7296,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 *
 * DspSpeed 2..127 = Display rotation speed in seconds if more than one value is requested
 * DspLine<1|2> <index>,<unit>,<index>,<unit>,... = Display specific JSON value and rotate between them
 *   unit 0 = None
 *        1 = Temperature (Line 1 only)
 *        2 = %RH (Line 2 only)
 *        3 = Both V (Line 1 only) / A (Line 2 only)
 *        4 = Both kWh (Line 1 only) / W (Line 2 only)
 * DspLine1 0 and DspLine2 0 = Default of temperature/humidity
 * DspLine9 = Show sensor data
 *
 * Example: {"SCD30":{"CarbonDioxide":746,"eCO2":727,"Temperature":30.6,"Humidity":43.6,"DewPoint":16.8}}
 *   index:   1        2                   3          4                  5               6
 *    unit:   0        0 (ppm)             0 (ppm)    1 (C or F)         2 (%RH)         1 (C or F)
 *
 *   DspLine1 4,1,3,0 = Temperature and eCO2
 *   DspLine2 2,0,5,2 = CarbonDioxide and humidity
\*********************************************************************************************/

#define XDRV_87              87

#define TM1621_ROTATE        5     // Seconds display rotation speed
#define TM1621_MAX_VALUES    8     // Default 8 x two different lines

//#define TM1621_DEBUG

#define TM1621_PULSE_WIDTH   10    // microseconds (Sonoff = 100)

#define TM1621_SYS_EN        0x01  // 0b00000001
#define TM1621_LCD_ON        0x03  // 0b00000011
#define TM1621_TIMER_DIS     0x04  // 0b00000100
#define TM1621_WDT_DIS       0x05  // 0b00000101
#define TM1621_TONE_OFF      0x08  // 0b00001000
#define TM1621_BIAS          0x29  // 0b00101001 = LCD 1/3 bias 4 commons option
#define TM1621_IRQ_DIS       0x80  // 0b100x0xxx

enum Tm1621Device { TM1621_USER, TM1621_POWR316D, TM1621_THR316D, TM1621_POWCT };
const char tm1621_device PROGMEM[] = "User|PowR3|THR3|PowCT";

enum Tm1621Units  { TM1621_NONE, TM1621_TEMPERATURE, TM1621_HUMIDITY, TM1621_VOLTAGE_CURRENT, TM1621_ENERGY_POWER };

const uint8_t tm1621_commands[] = { TM1621_SYS_EN, TM1621_LCD_ON, TM1621_BIAS, TM1621_TIMER_DIS, TM1621_WDT_DIS, TM1621_TONE_OFF, TM1621_IRQ_DIS };

const char tm1621_kchar[] PROGMEM = " |0|1|2|3|4|5|6|7|8|9|-|E";
//    b0 ---
//   b1 /  / b4
//      --- b5
//  b2 /  / b6
// b3  ---                        off   0     1     2     3     4     5     6     7     8     9     -     E
const uint8_t tm1621_digit[] = { 0x00, 0x5F, 0x50, 0x3D, 0x79, 0x72, 0x6B, 0x6F, 0x51, 0x7F, 0x7B, 0x20, 0x2F };

struct Tm1621 {
  uint8_t buffer[8];
  char row[2][12];
  uint8_t pin_da;
  uint8_t pin_cs;
  uint8_t pin_rd;
  uint8_t pin_wr;
  uint8_t state;
  uint8_t device;
  uint8_t display_rotate;
  uint8_t temp_sensors;
  uint8_t temp_sensors_rotate;
  bool celsius;
  bool fahrenheit;
  bool humidity;
  bool voltage;
  bool kwh;
  bool present;
} Tm1621;

/*********************************************************************************************\
 * Driver Settings load and save using filesystem
\*********************************************************************************************/

const uint16_t XDRV_87_VERSION = 0x0104;          // Latest driver version (See settings deltas below)

typedef struct {
  uint32_t crc32;                                 // To detect file changes
  uint16_t version;                               // To detect driver function changes
  uint8_t rotate;
  uint8_t spare;
  uint8_t line[2][TM1621_MAX_VALUES];
  uint8_t unit[2][TM1621_MAX_VALUES];
} tXdrv87Settings;
tXdrv87Settings Xdrv87Settings;

/*********************************************************************************************/

void Xdrv87SettingsLoad(bool erase) {
  // *** Start init default values in case file is not found ***
  memset(&Xdrv87Settings, 0x00, sizeof(tXdrv87Settings));
  Xdrv87Settings.version = XDRV_87_VERSION;
  // Init any other parameter in struct
  Xdrv87Settings.rotate = TM1621_ROTATE;

  // *** End Init default values ***

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV87 Use defaults as file system not enabled"));
#else
  // Try to load file /.drvset087
  char filename[20];
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_87);
  if (erase) {
    TfsDeleteFile(filename);  // Use defaults
  }
  else if (TfsLoadFile(filename, (uint8_t*)&Xdrv87Settings, sizeof(tXdrv87Settings))) {
    if (Xdrv87Settings.version != XDRV_87_VERSION) {      // Fix version dependent changes

      // *** Start fix possible setting deltas ***
//      if (Xdrv87Settings.version < 0x0105) {
//        Xdrv87Settings.spare = test;
//      }

      // *** End setting deltas ***

      // Set current version and save settings
      Xdrv87Settings.version = XDRV_87_VERSION;
      Xdrv87SettingsSave();
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CFG: XDRV87 loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV87 Use defaults as file system not ready or file not found"));
  }
#endif  // USE_UFILESYS
}

void Xdrv87SettingsSave(void) {
#ifdef USE_UFILESYS
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&Xdrv87Settings +4, sizeof(tXdrv87Settings) -4);  // Skip crc32
  if (crc32 != Xdrv87Settings.crc32) {
    // Try to save file /.drvset087
    Xdrv87Settings.crc32 = crc32;

    char filename[20];
    // Use for drivers:
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_87);
    if (TfsSaveFile(filename, (const uint8_t*)&Xdrv87Settings, sizeof(tXdrv87Settings))) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV87 saved to file"));
    } else {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV87 ERROR File system not ready or unable to save file"));
    }
  }
#endif  // USE_UFILESYS
}

bool Xdrv87SettingsRestore(void) {
  XdrvMailbox.data = (char*)&Xdrv87Settings;
  XdrvMailbox.index = sizeof(tXdrv87Settings);
  return true;
}

/*********************************************************************************************/

void TM1621StopSequence(void) {
  digitalWrite(Tm1621.pin_cs, 1);      // Stop command sequence
  delayMicroseconds(TM1621_PULSE_WIDTH / 2);
  digitalWrite(Tm1621.pin_da, 1);      // Reset data
}

void TM1621SendCmnd(uint16_t command) {
  uint16_t full_command = (0x0400 | command) << 5;  // 0b100cccccccc00000
  digitalWrite(Tm1621.pin_cs, 0);      // Start command sequence
  delayMicroseconds(TM1621_PULSE_WIDTH / 2);
  for (uint32_t i = 0; i < 12; i++) {
    digitalWrite(Tm1621.pin_wr, 0);    // Start write sequence
    if (full_command & 0x8000) {
      digitalWrite(Tm1621.pin_da, 1);  // Set data
    } else {
      digitalWrite(Tm1621.pin_da, 0);  // Set data
    }
    delayMicroseconds(TM1621_PULSE_WIDTH);
    digitalWrite(Tm1621.pin_wr, 1);    // Read data
    delayMicroseconds(TM1621_PULSE_WIDTH);
    full_command <<= 1;
  }
  TM1621StopSequence();
}

void TM1621SendAddress(uint16_t address) {
  uint16_t full_address = (address | 0x0140) << 7;  // 0b101aaaaaa0000000
  digitalWrite(Tm1621.pin_cs, 0);      // Start command sequence
  delayMicroseconds(TM1621_PULSE_WIDTH / 2);
  for (uint32_t i = 0; i < 9; i++) {
    digitalWrite(Tm1621.pin_wr, 0);    // Start write sequence
    if (full_address & 0x8000) {
      digitalWrite(Tm1621.pin_da, 1);  // Set data
    } else {
      digitalWrite(Tm1621.pin_da, 0);  // Set data
    }
    delayMicroseconds(TM1621_PULSE_WIDTH);
    digitalWrite(Tm1621.pin_wr, 1);    // Read data
    delayMicroseconds(TM1621_PULSE_WIDTH);
    full_address <<= 1;
  }
}

void TM1621SendCommon(uint8_t common) {
  for (uint32_t i = 0; i < 8; i++) {
    digitalWrite(Tm1621.pin_wr, 0);    // Start write sequence
    if (common & 1) {
      digitalWrite(Tm1621.pin_da, 1);  // Set data
    } else {
      digitalWrite(Tm1621.pin_da, 0);  // Set data
    }
    delayMicroseconds(TM1621_PULSE_WIDTH);
    digitalWrite(Tm1621.pin_wr, 1);    // Read data
    delayMicroseconds(TM1621_PULSE_WIDTH);
    common >>= 1;
  }
}

uint32_t TM1621Row2(uint32_t row1) {
  uint32_t row2 = 0;
  bitWrite(row2, 0, bitRead(row1, 6));
  bitWrite(row2, 1, bitRead(row1, 5));
  bitWrite(row2, 2, bitRead(row1, 4));
  bitWrite(row2, 3, bitRead(row1, 7));
  bitWrite(row2, 4, bitRead(row1, 3));
  bitWrite(row2, 5, bitRead(row1, 2));
  bitWrite(row2, 6, bitRead(row1, 1));
  bitWrite(row2, 7, bitRead(row1, 0));
  return row2;  
}

void TM1621SendRows(void) {
  // Tm1621.row[x] = "text", "----", "    " or a number with one decimal like "0.4", "237.5", "123456.7"
  // "123456.7" will be shown as "12E4" being a four digit overflow

//  AddLog(LOG_LEVEL_DEBUG, PSTR("TM1: Row1 '%s', Row2 '%s'"), Tm1621.row[0], Tm1621.row[1]);

  uint8_t buffer[8] = { 0 };  // TM1621 16-segment 4-bit common buffer
  char row[4];
  for (uint32_t j = 0; j < 2; j++) {
    // 0.4V => "  04", 0.0A => "  ", 1234.5V => "1234", 12345.6V => "12E3"
    uint32_t len = strlen(Tm1621.row[j]);
    char *dp = nullptr;           // Expect number larger than "123"
    int row_idx = len -3;         // "1234.5"
    if (len <= 5) {               // "----", "    ", "0.4", "237.5"
      dp = strchr(Tm1621.row[j], '.');
      row_idx = len -1;
    }
    else if (len > 6) {           // "1234567890.3" = "12E8"
      Tm1621.row[j][2] = 'E';
      Tm1621.row[j][3] = '0' + len -4;
      row_idx = 3;
    }
    row[3] = (row_idx >= 0) ? Tm1621.row[j][row_idx--] : ' ';
    if ((row_idx >= 0) && dp) { row_idx--; }
    row[2] = (row_idx >= 0) ? Tm1621.row[j][row_idx--] : ' ';
    row[1] = (row_idx >= 0) ? Tm1621.row[j][row_idx--] : ' ';
    row[0] = (row_idx >= 0) ? Tm1621.row[j][row_idx--] : ' ';

//    AddLog(LOG_LEVEL_DEBUG, PSTR("TM1: Dump%d %4_H"), j +1, row);

    char command[10];
    char needle[2] = { 0 };
    for (uint32_t i = 0; i < 4; i++) {
      needle[0] = row[i];
      int index = GetCommandCode(command, sizeof(command), (const char*)needle, tm1621_kchar);
      if (-1 == index) { index = 0; }         // Off
      uint32_t digit = tm1621_digit[index];
      if ((2 == i) && dp) { digit |= 0x80; }  // Decimal point
      uint32_t bidx = (!j) ? i : 7 -i;
      buffer[bidx] = (j) ? TM1621Row2(digit) : digit;
    }
  }

  if (Tm1621.fahrenheit) { buffer[1] |= 0x80; }
  if (Tm1621.celsius) { buffer[3] |= 0x80; }
  if (Tm1621.kwh) { buffer[4] |= 0x08; }
  if (Tm1621.humidity) { buffer[6] |= 0x08; }
  if (Tm1621.voltage) { buffer[7] |= 0x08; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("TM1: Dump3 %8_H"), buffer);

  TM1621SendAddress(0x10);  // Sonoff only uses the upper 16 Segments
  for (uint32_t i = 0; i < 8; i++) {
    TM1621SendCommon(buffer[i]);
  }
  TM1621StopSequence();
}

void TM1621PreInit(void) {
  if (!PinUsed(GPIO_TM1621_CS) || !PinUsed(GPIO_TM1621_WR) || !PinUsed(GPIO_TM1621_RD) || !PinUsed(GPIO_TM1621_DAT)) { return; }

  Tm1621.device = TM1621_USER;
  uint32_t pin_tm1621_dat = Pin(GPIO_TM1621_DAT);
  if (5 == Pin(GPIO_TM1621_DAT)) {
    if (25 == Pin(GPIO_CSE7761_RX, 1)) {
      Tm1621.device = TM1621_POWCT;
    } else {
      Tm1621.device = TM1621_THR316D;
    }
  }
  else if (14 == Pin(GPIO_TM1621_DAT)) {
    Tm1621.device = TM1621_POWR316D;
  }
  Tm1621.present = true;
  Tm1621.pin_da = Pin(GPIO_TM1621_DAT);
  Tm1621.pin_cs = Pin(GPIO_TM1621_CS);
  Tm1621.pin_rd = Pin(GPIO_TM1621_RD);
  Tm1621.pin_wr = Pin(GPIO_TM1621_WR);
  pinMode(Tm1621.pin_da, OUTPUT);
  digitalWrite(Tm1621.pin_da, 1);
  pinMode(Tm1621.pin_cs, OUTPUT);
  digitalWrite(Tm1621.pin_cs, 1);
  pinMode(Tm1621.pin_rd, OUTPUT);
  digitalWrite(Tm1621.pin_rd, 1);
  pinMode(Tm1621.pin_wr, OUTPUT);
  digitalWrite(Tm1621.pin_wr, 1);

  Xdrv87SettingsLoad(0);

  Tm1621.state = 200;

  char stemp[8];
  AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1621 for %s"), 
    GetTextIndexed(stemp, sizeof(stemp), Tm1621.device, tm1621_device));
}

void TM1621Init(void) {
  digitalWrite(Tm1621.pin_cs, 0);
  delayMicroseconds(80);
  digitalWrite(Tm1621.pin_rd, 0);
  delayMicroseconds(15);
  digitalWrite(Tm1621.pin_wr, 0);
  delayMicroseconds(25);
  digitalWrite(Tm1621.pin_da, 0);
  delayMicroseconds(TM1621_PULSE_WIDTH);
  digitalWrite(Tm1621.pin_da, 1);

  for (uint32_t command = 0; command < sizeof(tm1621_commands); command++) {
    TM1621SendCmnd(tm1621_commands[command]);
  }

  TM1621SendAddress(0x00);
  for (uint32_t segment = 0; segment < 16; segment++) {
    TM1621SendCommon(0);
  }
  TM1621StopSequence();

  snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("----"));
  snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("----"));
  TM1621SendRows();
}

uint32_t TM1621GetSensors(bool refresh) {
  if (refresh) {
    ResponseClear();
    XsnsXdrvCall(FUNC_JSON_APPEND);
    ResponseJsonStart();  // Overwrite first comma
    ResponseJsonEnd();    // Append }
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TM1: Sensors %s"), ResponseData());
  }
  return ResponseLength();
}

float TM1621GetValues(uint32_t index, bool refresh) {
  float value = NAN;
  if (TM1621GetSensors(refresh)) {
    uint32_t idx = 0;
    char *data = ResponseData();  // {"HTU21":{"Temperature":30.7,"Humidity":39.0,"DewPoint":15.2},"BME680":{"Temperature":30.0,"Humidity":50.4,"DewPoint":18.5,"Pressure":1009.6,"Gas":1660.52},"ESP32":{"Temperature":53.3}}
    while (data) {
      data = strstr_P(data, PSTR("\":"));
      if (data) {
        idx++;
        data += 2;
        if (idx == index) {
          value = CharToFloat(data);
          break;
        }
      }
    }
  }
  return value;
}

float TM1621GetTemperatureValues(uint32_t index) {
  float value = NAN;
  if (TM1621GetSensors(1)) {
    uint32_t idx = 0;
    char *data = ResponseData();  // {"HTU21":{"Temperature":30.7,"Humidity":39.0,"DewPoint":15.2},"BME680":{"Temperature":30.0,"Humidity":50.4,"DewPoint":18.5,"Pressure":1009.6,"Gas":1660.52},"ESP32":{"Temperature":53.3}}
    while (data) {
      data = strstr_P(data, PSTR(D_JSON_TEMPERATURE));
      if (data) {
        idx++;
        data += 13;      // strlen("Temperature") + 2;
        if (idx == index) {
          value = CharToFloat(data);
          if (Tm1621.temp_sensors) {
            break;
          }
        }
      }
    }
    if (0 == Tm1621.temp_sensors) {
      Tm1621.temp_sensors = idx;
    }
  }
  return value;
}

void TM1621Show(void) {
  Tm1621.celsius = false;
  Tm1621.fahrenheit = false;
  Tm1621.humidity = false;
  Tm1621.voltage = false;
  Tm1621.kwh = false;
  snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("    "));
  snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("    "));

  if ((Xdrv87Settings.line[0][0] > 0) || (Xdrv87Settings.line[1][0] > 0)) {
    float value = TM1621GetValues(Xdrv87Settings.line[0][Tm1621.display_rotate], 1);
    if (!isnan(value)) {
      ext_snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("%1_f"), &value);
      if (TM1621_TEMPERATURE == Xdrv87Settings.unit[0][Tm1621.display_rotate]) {
        if (Settings->flag.temperature_conversion) {  // SetOption8 - Switch between Celsius or Fahrenheit
          Tm1621.fahrenheit = true;
        } else {
          Tm1621.celsius = true;
        }
      }
      Tm1621.voltage = (TM1621_VOLTAGE_CURRENT == Xdrv87Settings.unit[0][Tm1621.display_rotate]);
      Tm1621.kwh = (4 == Xdrv87Settings.unit[0][Tm1621.display_rotate]);
    }
    value = TM1621GetValues(Xdrv87Settings.line[1][Tm1621.display_rotate], 0);
    if (!isnan(value)) {
      ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &value);
      Tm1621.humidity = (TM1621_HUMIDITY == Xdrv87Settings.unit[1][Tm1621.display_rotate]);
      Tm1621.voltage = (TM1621_VOLTAGE_CURRENT == Xdrv87Settings.unit[1][Tm1621.display_rotate]);
      Tm1621.kwh = (TM1621_ENERGY_POWER == Xdrv87Settings.unit[1][Tm1621.display_rotate]);
    }
    uint32_t max = 0;
    while ((max < TM1621_MAX_VALUES) && ((Xdrv87Settings.line[0][max] > 0) || (Xdrv87Settings.line[1][max] > 0))) { max++; }
    Tm1621.display_rotate++;
    if (Tm1621.display_rotate >= max) {
      Tm1621.display_rotate = 0;
    }
    TM1621SendRows();
    return;
  }

#ifdef USE_ENERGY_SENSOR
  if ((TM1621_POWR316D == Tm1621.device) ||
      (TM1621_POWCT == Tm1621.device)) {
    if (0 == Tm1621.display_rotate) {
      ext_snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("%1_f"), &Energy->voltage[0]);
      ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &Energy->current[0]);
      Tm1621.voltage = true;
      Tm1621.display_rotate = 1;
    } else {
      ext_snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("%1_f"), &Energy->total[0]);
      ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &Energy->active_power[0]);
      Tm1621.kwh = true;
      Tm1621.display_rotate = 0;
    }
    TM1621SendRows();
    return;
  }
#endif  // USE_ENERGY_SENSOR

  if (TM1621_THR316D == Tm1621.device) {
    if (!isnan(TasmotaGlobal.temperature_celsius)) {
      float temperature = ConvertTempToFahrenheit(TasmotaGlobal.temperature_celsius);
      if (TasmotaGlobal.humidity == 0.0f) {  // No humidity so check for more temperature sensors
        if (0 == Tm1621.temp_sensors) {
          TM1621GetTemperatureValues(100);   // Find max number of temperature sensors
        }
        if (Tm1621.temp_sensors > 1) {
          if (Tm1621.temp_sensors > 2) {
            Tm1621.temp_sensors_rotate++;
            if (Tm1621.temp_sensors_rotate > Tm1621.temp_sensors) {
              Tm1621.temp_sensors_rotate = 1;
            }
            temperature = TM1621GetTemperatureValues(Tm1621.temp_sensors_rotate);
            ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%d"), Tm1621.temp_sensors_rotate);
          } else {
            temperature = TM1621GetTemperatureValues(1);  // Fix in case GlobalTemp is set wrong (#17694)
            float temperature2 = TM1621GetTemperatureValues(2);
            ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &temperature2);
          }
        }
      }
      ext_snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("%1_f"), &temperature);
      if (Settings->flag.temperature_conversion) {  // SetOption8 - Switch between Celsius or Fahrenheit
        Tm1621.fahrenheit = true;
      } else {
        Tm1621.celsius = true;
      }
    }
    if (TasmotaGlobal.humidity > 0.0f) {
      Tm1621.humidity = true;
      ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &TasmotaGlobal.humidity);
    }
  }
  TM1621SendRows();
}

void TM1621EverySecond(void) {
  Tm1621.state++;
  if (Tm1621.state > 127) {
    if (202 == Tm1621.state) {
      TM1621Init();
      Tm1621.state = 0;
    }
  } else {
    if (Tm1621.state >= Xdrv87Settings.rotate) {
      TM1621Show();
      Tm1621.state = 0;
    }
  }
}

/*********************************************************************************************\
 * Command
\*********************************************************************************************/

const char kTm1621Commands[] PROGMEM = "Dsp|"  // No prefix
  "Line|"
#ifdef TM1621_DEBUG
  "Test|"
#endif  // TM1621_DEBUG
  "Speed";
void (*const kTm1621Command[])(void) PROGMEM = {
  &CmndDspLine,
#ifdef TM1621_DEBUG
  &CmndDspTest,
#endif  // TM1621_DEBUG
  &CmndDspSpeed };

void CmndDspLine(void) {
  // DspLine1 <index>,<unit>,<index>,<unit>,... = Display specific JSON value and rotate between them
  //   unit 0 = None
  //        1 = temperature (Line 1 only)
  //        2 = %RH (Line 2 only)
  //        3 = Both V (Line 1 only) / A (Line 2 only)
  //        4 = Both kWh (Line 1 only) / W (Line 2 only)
  // DspLine1 0 and DspLine2 0 = Default of temperature/humidity
  //
  //      {"SCD30":{"CarbonDioxide":746,"eCO2":727,"Temperature":30.6,"Humidity":43.6,"DewPoint":16.8}}
  // index: 1        2                   3          4                  5               6
  //  unit: 0        0 (ppm)             0 (ppm)    1 (C or F)         2 (%RH)         1 (C or F)
  //
  //  DspLine1 4,1,3,0 = Temperature and eCO2
  //  DspLine2 2,0,5,2 = CarbonDioxide and humidity
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    if (XdrvMailbox.data_len > 0) {
      uint32_t parm[2 * TM1621_MAX_VALUES] = { 0 };
      ParseParameters(2 * TM1621_MAX_VALUES, parm);
      for (uint32_t i = 0; i < TM1621_MAX_VALUES; i++) {
        uint32_t j = i << 1;
        Xdrv87Settings.line[XdrvMailbox.index -1][i] = parm[j];
        Xdrv87Settings.unit[XdrvMailbox.index -1][i] = parm[j +1];
      }
    }
    char values[8 * TM1621_MAX_VALUES];
    values[0] = '\0';
    uint32_t i = 0;
    do {
      snprintf_P(values, sizeof(values), PSTR("%s%s%d,%d"),
        values, (i > 0) ? "," : "", Xdrv87Settings.line[XdrvMailbox.index -1][i], Xdrv87Settings.unit[XdrvMailbox.index -1][i]);
      i++;
    } while ((i < TM1621_MAX_VALUES) && (Xdrv87Settings.line[XdrvMailbox.index -1][i] > 0));
    ResponseCmndIdxChar(values);
  }
  if (9 == XdrvMailbox.index) {
    // DspLine9 = Show sensor JSON
    // Example 1: SCD30 I2C connected (GPIO25 = I2C SCL, GPIO26 = I2C SDA)
    //      {"SCD30":{"CarbonDioxide":746,"eCO2":727,"Temperature":30.6,"Humidity":43.6,"DewPoint":16.8}}
    // index: 1        2                   3          4                  5               6
    //  unit: 0        0 (ppm)             0 (ppm)    1 (C or F)         2 (%RH)         1 (C or F)
    // Example 2: PZEM016 serial connected (GPIO25 = PZEM0XX Tx, GPIO26 = PZEM016 Rx)
    //      {"ENERGY":{"TotalStartTime":"2022-07-05T16:01:39","Total":0.000,"Yesterday":0.000,"Today":0.000,"Power":0.00,"ApparentPower":0.00,"ReactivePower":0.00,"Factor":0.00,"Frequency":50,"Voltage":231.7,"Current":0.000}}
    // index: 1         2                                      3             4                 5             6            7                    8                    9             10             11              12
    //  unit: 0         0                                      4 (kWh)       4 (kWh)           4 (kWh)       4 (W)        0 (VA)               0 (var)              0             0              3 (V)           3 (A)
    TM1621GetSensors(1);
  }
}

void CmndDspSpeed(void) {
  // DspSpeed 2..127 = Display rotation speed in seconds if more than one value is requested
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload > 1) {  // We need at least 2 seconds to poll all sensors
      Xdrv87Settings.rotate = XdrvMailbox.payload &0x7F;  // Max 127 seconds
    }
  }
  ResponseCmndNumber(Xdrv87Settings.rotate);
}

#ifdef TM1621_DEBUG
void CmndDspTest(void) {
  // Only one decimal is supported !!!
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    if (XdrvMailbox.data_len > 0) {
      uint32_t line = XdrvMailbox.index -1;
      snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("----"));
      snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("----"));
      snprintf_P(Tm1621.row[line], sizeof(Tm1621.row[line]), PSTR("%s"), XdrvMailbox.data);
      TM1621SendRows();
    }
  }
  ResponseCmndDone();
}
#endif  // TM1621_DEBUG

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv87(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    TM1621PreInit();
  }
  else if (Tm1621.present) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        TM1621EverySecond();
        break;
      case FUNC_RESET_SETTINGS:
        Xdrv87SettingsLoad(1);
        break;
      case FUNC_RESTORE_SETTINGS:
        result = Xdrv87SettingsRestore();
        break;
      case FUNC_SAVE_SETTINGS:
        Xdrv87SettingsSave();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTm1621Commands, kTm1621Command);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_TM1621_SONOFF
#endif  // ESP32
