/*
  xsns_05_ds18b20.ino - DS18B20 temperature sensor support for Sonoff-Tasmota

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

#ifdef USE_DS18B20
/*********************************************************************************************\
 * DS18B20 - Temperature - Single sensor
\*********************************************************************************************/

#define XSNS_05              5

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE

float ds18b20_temperature = 0;
uint8_t ds18b20_valid = 0;
uint8_t ds18x20_pin = 0;
char ds18b20_types[] = "DS18B20";

/*********************************************************************************************\
 * Embedded stripped and tuned OneWire library
\*********************************************************************************************/

uint8_t OneWireReset(void)
{
  uint8_t retries = 125;

  //noInterrupts();
#ifdef DS18B20_INTERNAL_PULLUP
  pinMode(ds18x20_pin, INPUT_PULLUP);
#else
  pinMode(ds18x20_pin, INPUT);
#endif
  do {
    if (--retries == 0) {
      return 0;
    }
    delayMicroseconds(2);
  } while (!digitalRead(ds18x20_pin));
  pinMode(ds18x20_pin, OUTPUT);
  digitalWrite(ds18x20_pin, LOW);
  delayMicroseconds(480);
#ifdef DS18B20_INTERNAL_PULLUP
  pinMode(ds18x20_pin, INPUT_PULLUP);
#else
  pinMode(ds18x20_pin, INPUT);
#endif
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

uint8_t OneWireReadBit(void)
{
  //noInterrupts();
  pinMode(ds18x20_pin, OUTPUT);
  digitalWrite(ds18x20_pin, LOW);
  delayMicroseconds(3);
#ifdef DS18B20_INTERNAL_PULLUP
  pinMode(ds18x20_pin, INPUT_PULLUP);
#else
  pinMode(ds18x20_pin, INPUT);
#endif
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

uint8_t OneWireRead(void)
{
  uint8_t r = 0;

  for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
    if (OneWireReadBit()) {
      r |= bit_mask;
    }
  }
  return r;
}

bool OneWireCrc8(uint8_t *addr)
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

void Ds18b20Convert(void)
{
  OneWireReset();
  OneWireWrite(W1_SKIP_ROM);           // Address all Sensors on Bus
  OneWireWrite(W1_CONVERT_TEMP);       // start conversion, no parasite power on at the end
//  delay(750);                          // 750ms should be enough for 12bit conv
}

bool Ds18b20Read(void)
{
  uint8_t data[9];
  int8_t sign = 1;

  if (ds18b20_valid) { ds18b20_valid--; }
/*
  if (!OneWireReadBit()) {     // Check end of measurement
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_BUSY));
    return;
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
      ds18b20_temperature = ConvertTemp(sign * temp12 * 0.0625);
      ds18b20_valid = SENSOR_MAX_MISS;
      return true;
    }
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

/********************************************************************************************/

void Ds18b20EverySecond(void)
{
  ds18x20_pin = pin[GPIO_DSB];
  if (uptime &1) {
    // 2mS
    Ds18b20Convert();          // Start conversion, takes up to one second
  } else {
    // 12mS
    if (!Ds18b20Read()) {      // Read temperature
      AddLogMissed(ds18b20_types, ds18b20_valid);
    }
  }
}

void Ds18b20Show(bool json)
{
  if (ds18b20_valid) {        // Check for valid temperature
    char temperature[33];
    dtostrfd(ds18b20_temperature, Settings.flag2.temperature_resolution, temperature);
    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMP, mqtt_data, ds18b20_types, temperature);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_TEMP, temperature);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, ds18b20_temperature);
      }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, ds18b20_types, temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns05(uint8_t function)
{
  bool result = false;

  if (pin[GPIO_DSB] < 99) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Ds18b20EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ds18b20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Ds18b20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18B20
