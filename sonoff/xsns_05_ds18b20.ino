/*
  xsns_05_ds18b20.ino - DS18B20 temperature sensor support for Sonoff-Tasmota

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

#ifdef USE_DS18B20
/*********************************************************************************************\
 * DS18B20 - Temperature - Single sensor
\*********************************************************************************************/

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE

float ds18b20_last_temperature = 0;
uint16_t ds18b20_last_result = 0;
uint8_t ds18x20_pin = 0;

/*********************************************************************************************\
 * Embedded stripped and tuned OneWire library
\*********************************************************************************************/

uint8_t OneWireReset()
{
  uint8_t retries = 125;

  //noInterrupts();
  pinMode(ds18x20_pin, INPUT);
  do {
    if (--retries == 0) {
      return 0;
    }
    delayMicroseconds(2);
  } while (!digitalRead(ds18x20_pin));
  pinMode(ds18x20_pin, OUTPUT);
  digitalWrite(ds18x20_pin, LOW);
  delayMicroseconds(480);
  pinMode(ds18x20_pin, INPUT);
  delayMicroseconds(70);
  uint8_t r = !digitalRead(ds18x20_pin);
  //interrupts();
  delayMicroseconds(410);
  return r;
}

void OneWireWriteBit(uint8_t v)
{
  static const uint8_t delay_low[2] = { 65, 10 };
  static const uint8_t delay_high[2] = { 5, 55 };

  v &= 1;
  //noInterrupts();
  digitalWrite(ds18x20_pin, LOW);
  pinMode(ds18x20_pin, OUTPUT);
  delayMicroseconds(delay_low[v]);
  digitalWrite(ds18x20_pin, HIGH);
  //interrupts();
  delayMicroseconds(delay_high[v]);
}

uint8_t OneWireReadBit()
{
  //noInterrupts();
  pinMode(ds18x20_pin, OUTPUT);
  digitalWrite(ds18x20_pin, LOW);
  delayMicroseconds(3);
  pinMode(ds18x20_pin, INPUT);
  delayMicroseconds(10);
  uint8_t r = digitalRead(ds18x20_pin);
  //interrupts();
  delayMicroseconds(53);
  return r;
}

void OneWireWrite(uint8_t v)
{
  for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
    OneWireWriteBit((bit_mask & v) ? 1 : 0);
  }
}

uint8_t OneWireRead()
{
  uint8_t r = 0;

  for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
    if (OneWireReadBit()) {
      r |= bit_mask;
    }
  }
  return r;
}

boolean OneWireCrc8(uint8_t *addr)
{
  uint8_t crc = 0;
  uint8_t len = 8;

  while (len--) {
    uint8_t inbyte = *addr++;          // from 0 to 7
    for (uint8_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) {
        crc ^= 0x8C;
      }
      inbyte >>= 1;
    }
  }
  return (crc == *addr);               // addr 8
}

/********************************************************************************************/

void Ds18x20Init()
{
  ds18x20_pin = pin[GPIO_DSB];
}

void Ds18x20Convert()
{
  OneWireReset();
  OneWireWrite(W1_SKIP_ROM);           // Address all Sensors on Bus
  OneWireWrite(W1_CONVERT_TEMP);       // start conversion, no parasite power on at the end
//  delay(750);                          // 750ms should be enough for 12bit conv
}

boolean Ds18b20Read(float &t)
{
  uint8_t data[9];
  int8_t sign = 1;

  if (!ds18b20_last_temperature) {
    t = NAN;
  } else {
    ds18b20_last_result++;
    if (ds18b20_last_result > 4) {     // Reset after 4 misses
      ds18b20_last_temperature = NAN;
    }
    t = ds18b20_last_temperature;
  }

/*
  if (!OneWireReadBit()) {             //check measurement end
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_BUSY));
    return !isnan(t);
  }
*/
  for (uint8_t retry = 0; retry < 3; retry++) {
    OneWireReset();
    OneWireWrite(W1_SKIP_ROM);
    OneWireWrite(W1_READ_SCRATCHPAD);
    for (uint8_t i = 0; i < 9; i++) {
      data[i] = OneWireRead();
    }
    if (OneWireCrc8(data)) {
      uint16_t temp12 = (data[1] << 8) + data[0];
      if (temp12 > 2047) {
        temp12 = (~temp12) +1;
        sign = -1;
      }
      t = ConvertTemp(sign * temp12 * 0.0625);
      ds18b20_last_result = 0;
    }
    if (!isnan(t)) {
      ds18b20_last_temperature = t;
      return true;
    }
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return !isnan(t);
}

void Ds18b20Show(boolean json)
{
  float t;

  if (Ds18b20Read(t)) {                // Check if read failed
    char temperature[10];

    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"DS18B20\":{\"" D_JSON_TEMPERATURE "\":%s}"), mqtt_data, temperature);
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_TEMP, temperature);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "DS18B20", temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_05

boolean Xsns05(byte function)
{
  boolean result = false;

  if (pin[GPIO_DSB] < 99) {
    switch (function) {
      case FUNC_INIT:
        Ds18x20Init();
        break;
      case FUNC_PREP_BEFORE_TELEPERIOD:
        Ds18x20Convert();   // Start conversion, takes up to one second
        break;
      case FUNC_JSON_APPEND:
        Ds18b20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Ds18b20Show(0);
        Ds18x20Convert();   // Start conversion, takes up to one second
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18B20
