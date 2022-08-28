/*
  xdrv_87_tm1621_sonoff.ino - Sonoff POWR3xxD and THR3xxD display support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_DISPLAY_TM1621_SONOFF
/*********************************************************************************************\
 * Sonoff POWR3xxD and THR3xxD LCD support
 *
 * {"NAME":"Sonoff POWR316D","GPIO":[32,0,0,0,0,576,0,0,0,224,9280,0,3104,0,320,0,0,0,0,0,0,9184,9248,9216,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff POWR320D","GPIO":[32,0,9313,0,9312,576,0,0,0,0,9280,0,3104,0,320,0,0,0,0,0,0,9184,9248,9216,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff THR316D","GPIO":[32,0,0,0,225,9280,0,0,0,321,0,576,320,9184,9216,0,0,224,0,9248,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Sonoff THR320D","GPIO":[32,0,0,0,226,9280,0,0,0,321,0,576,320,9184,9216,9312,0,0,9313,9248,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
\*********************************************************************************************/

#define XDRV_87              87

#define TM1621_PULSE_WIDTH   10    // microseconds (Sonoff = 100)

#define TM1621_SYS_EN        0x01  // 0b00000001
#define TM1621_LCD_ON        0x03  // 0b00000011
#define TM1621_TIMER_DIS     0x04  // 0b00000100
#define TM1621_WDT_DIS       0x05  // 0b00000101
#define TM1621_TONE_OFF      0x08  // 0b00001000
#define TM1621_BIAS          0x29  // 0b00101001 = LCD 1/3 bias 4 commons option
#define TM1621_IRQ_DIS       0x80  // 0b100x0xxx

enum Tm1621Device { TM1621_USER, TM1621_POWR316D, TM1621_THR316D };

const uint8_t tm1621_commands[] = { TM1621_SYS_EN, TM1621_LCD_ON, TM1621_BIAS, TM1621_TIMER_DIS, TM1621_WDT_DIS, TM1621_TONE_OFF, TM1621_IRQ_DIS };

const char tm1621_kchar[] PROGMEM = { "0|1|2|3|4|5|6|7|8|9|-| " };
//                                          0     1     2     3     4     5     6     7     8     9     -     off
const uint8_t tm1621_digit_row[2][12] = {{ 0x5F, 0x50, 0x3D, 0x79, 0x72, 0x6B, 0x6F, 0x51, 0x7F, 0x7B, 0x20, 0x00 },
                                         { 0xF5, 0x05, 0xB6, 0x97, 0x47, 0xD3, 0xF3, 0x85, 0xF7, 0xD7, 0x02, 0x00 }};

struct Tm1621 {
  uint8_t buffer[8];
  char row[2][12];
  uint8_t pin_da;
  uint8_t pin_cs;
  uint8_t pin_rd;
  uint8_t pin_wr;
  uint8_t state;
  uint8_t device;
  uint8_t temp_sensors;
  uint8_t temp_sensors_rotate;
  bool celsius;
  bool fahrenheit;
  bool humidity;
  bool voltage;
  bool kwh;
  bool present;
} Tm1621;

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

void TM1621SendRows(void) {
  // Tm1621.row[x] = "text", "----", "    " or a number with one decimal like "0.4", "237.5", "123456.7"
  // "123456.7" will be shown as "9999" being a four digit overflow

//  AddLog(LOG_LEVEL_DEBUG, PSTR("TM1: Row1 '%s', Row2 '%s'"), Tm1621.row[0], Tm1621.row[1]);

  uint8_t buffer[8] = { 0 };  // TM1621 16-segment 4-bit common buffer
  char row[4];
  for (uint32_t j = 0; j < 2; j++) {
    // 0.4V => "  04", 0.0A => "  ", 1234.5V => "1234"
    uint32_t len = strlen(Tm1621.row[j]);
    char *dp = nullptr;           // Expect number larger than "123"
    int row_idx = len -3;         // "1234.5"
    if (len <= 5) {               // "----", "    ", "0.4", "237.5"
      dp = strchr(Tm1621.row[j], '.');
      row_idx = len -1;
    }
    else if (len > 6) {           // "12345.6"
      snprintf_P(Tm1621.row[j], sizeof(Tm1621.row[j]), PSTR("9999"));
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
      if (-1 == index) { index = 11; }
      uint32_t bidx = (0 == j) ? i : 7 -i;
      buffer[bidx] = tm1621_digit_row[j][index];
    }
    if (dp) {
      if (0 == j) {
        buffer[2] |= 0x80;   // Row 1 decimal point
      } else {
        buffer[5] |= 0x08;   // Row 2 decimal point
      }
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

  Tm1621.device = (14 == Pin(GPIO_TM1621_DAT)) ? TM1621_POWR316D : (5 == Pin(GPIO_TM1621_DAT)) ? TM1621_THR316D : TM1621_USER;
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

  Tm1621.state = 100;

  AddLog(LOG_LEVEL_INFO, PSTR("DSP: TM1621"));
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

float TM1621GetTemperatureValues(uint32_t index) {
  char *start = ResponseData();
  int data_start = ResponseLength();

  XsnsCall(FUNC_JSON_APPEND);
  XdrvCall(FUNC_JSON_APPEND);

  if (data_start == ResponseLength()) { return NAN; }

  float value = NAN;
  uint32_t idx = 0;
  char *data = start;  // Invalid JSON ,"HTU21":{"Temperature":30.7,"Humidity":39.0,"DewPoint":15.2},"BME680":{"Temperature":30.0,"Humidity":50.4,"DewPoint":18.5,"Pressure":1009.6,"Gas":1660.52},"ESP32":{"Temperature":53.3}
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
  return value;
}

void TM1621Show(void) {
  if (TM1621_POWR316D == Tm1621.device) {
    static uint32_t display = 0;
    if (0 == display) {
      Tm1621.kwh = false;
      ext_snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("%1_f"), &Energy.voltage[0]);
      ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &Energy.current[0]);
      Tm1621.voltage = true;
      display = 1;
    } else {
      Tm1621.voltage = false;
      ext_snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("%1_f"), &Energy.total[0]);
      ext_snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("%1_f"), &Energy.active_power[0]);
      Tm1621.kwh = true;
      display = 0;
    }
    TM1621SendRows();
  }

  if (TM1621_THR316D == Tm1621.device) {
    Tm1621.celsius = false;
    Tm1621.fahrenheit = false;
    Tm1621.humidity = false;
    snprintf_P(Tm1621.row[0], sizeof(Tm1621.row[0]), PSTR("    "));
    snprintf_P(Tm1621.row[1], sizeof(Tm1621.row[1]), PSTR("    "));
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
    TM1621SendRows();
  }
}

void TM1621EverySecond(void) {
  Tm1621.state++;
  if (5 == Tm1621.state) {
    TM1621Show();
    Tm1621.state = 0;
  }
  if (102 == Tm1621.state) {
    TM1621Init();
    Tm1621.state = 0;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv87(uint8_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    TM1621PreInit();
  }
  else if (Tm1621.present) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        TM1621EverySecond();
        break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_TM1621_SONOFF
