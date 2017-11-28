/*
  xsns_05_ds18x20.ino - DS18x20 temperature sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature - Multiple sensors
\*********************************************************************************************/

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_WRITE_EEPROM      0x48
#define W1_WRITE_SCRATCHPAD  0x4E
#define W1_READ_SCRATCHPAD   0xBE

#define DS18X20_MAX_SENSORS  8

const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";

uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };
uint8_t ds18x20_address[DS18X20_MAX_SENSORS][8];
uint8_t ds18x20_index[DS18X20_MAX_SENSORS] = { 0 };
uint8_t ds18x20_sensors = 0;
uint8_t ds18x20_pin = 0;
char ds18x20_types[9];

/*********************************************************************************************\
 * Embedded tuned OneWire library
\*********************************************************************************************/

#define W1_MATCH_ROM         0x55
#define W1_SEARCH_ROM        0xF0

uint8_t onewire_last_discrepancy = 0;
uint8_t onewire_last_family_discrepancy = 0;
bool onewire_last_device_flag = false;
unsigned char onewire_rom_id[8] = { 0 };

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

void OneWireSelect(const uint8_t rom[8])
{
  OneWireWrite(W1_MATCH_ROM);
  for (uint8_t i = 0; i < 8; i++) {
    OneWireWrite(rom[i]);
  }
}

void OneWireResetSearch()
{
  onewire_last_discrepancy = 0;
  onewire_last_device_flag = false;
  onewire_last_family_discrepancy = 0;
  for (uint8_t i = 0; i < 8; i++) {
    onewire_rom_id[i] = 0;
  }
}

uint8_t OneWireSearch(uint8_t *newAddr)
{
  uint8_t id_bit_number = 1;
  uint8_t last_zero = 0;
  uint8_t rom_byte_number = 0;
  uint8_t search_result = 0;
  uint8_t id_bit;
  uint8_t cmp_id_bit;
  unsigned char rom_byte_mask = 1;
  unsigned char search_direction;

  if (!onewire_last_device_flag) {
    if (!OneWireReset()) {
      onewire_last_discrepancy = 0;
      onewire_last_device_flag = false;
      onewire_last_family_discrepancy = 0;
      return false;
    }
    OneWireWrite(W1_SEARCH_ROM);
    do {
      id_bit     = OneWireReadBit();
      cmp_id_bit = OneWireReadBit();

      if ((id_bit == 1) && (cmp_id_bit == 1)) {
        break;
      } else {
        if (id_bit != cmp_id_bit) {
          search_direction = id_bit;
        } else {
          if (id_bit_number < onewire_last_discrepancy) {
            search_direction = ((onewire_rom_id[rom_byte_number] & rom_byte_mask) > 0);
          } else {
            search_direction = (id_bit_number == onewire_last_discrepancy);
          }
          if (search_direction == 0) {
            last_zero = id_bit_number;
            if (last_zero < 9) {
              onewire_last_family_discrepancy = last_zero;
            }
          }
        }
        if (search_direction == 1) {
          onewire_rom_id[rom_byte_number] |= rom_byte_mask;
        } else {
          onewire_rom_id[rom_byte_number] &= ~rom_byte_mask;
        }
        OneWireWriteBit(search_direction);
        id_bit_number++;
        rom_byte_mask <<= 1;
        if (rom_byte_mask == 0) {
          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < 8);
    if (!(id_bit_number < 65)) {
      onewire_last_discrepancy = last_zero;
      if (onewire_last_discrepancy == 0) {
        onewire_last_device_flag = true;
      }
      search_result = true;
    }
  }
  if (!search_result || !onewire_rom_id[0]) {
    onewire_last_discrepancy = 0;
    onewire_last_device_flag = false;
    onewire_last_family_discrepancy = 0;
    search_result = false;
  }
  for (uint8_t i = 0; i < 8; i++) {
    newAddr[i] = onewire_rom_id[i];
  }
  return search_result;
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
  uint64_t ids[DS18X20_MAX_SENSORS];

  ds18x20_pin = pin[GPIO_DSB];
  OneWireResetSearch();
  for (ds18x20_sensors = 0; ds18x20_sensors < DS18X20_MAX_SENSORS; ds18x20_sensors) {
    if (!OneWireSearch(ds18x20_address[ds18x20_sensors])) {
      break;
    }
    if (OneWireCrc8(ds18x20_address[ds18x20_sensors]) &&
       ((ds18x20_address[ds18x20_sensors][0] == DS18S20_CHIPID) ||
        (ds18x20_address[ds18x20_sensors][0] == DS1822_CHIPID) ||
        (ds18x20_address[ds18x20_sensors][0] == DS18B20_CHIPID) ||
        (ds18x20_address[ds18x20_sensors][0] == MAX31850_CHIPID))) {
      ds18x20_index[ds18x20_sensors] = ds18x20_sensors;
      ids[ds18x20_sensors] = ds18x20_address[ds18x20_sensors][0];  // Chip id
      for (uint8_t j = 6; j > 0; j--) {
        ids[ds18x20_sensors] = ids[ds18x20_sensors] << 8 | ds18x20_address[ds18x20_sensors][j];
      }
      ds18x20_sensors++;
    }
  }
  for (uint8_t i = 0; i < ds18x20_sensors; i++) {
    for (uint8_t j = i + 1; j < ds18x20_sensors; j++) {
      if (ids[ds18x20_index[i]] > ids[ds18x20_index[j]]) {  // Sort ascending
        std::swap(ds18x20_index[i], ds18x20_index[j]);
      }
    }
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), ds18x20_sensors);
  AddLog(LOG_LEVEL_DEBUG);
}

void Ds18x20Convert()
{
  OneWireReset();
  OneWireWrite(W1_SKIP_ROM);           // Address all Sensors on Bus
  OneWireWrite(W1_CONVERT_TEMP);       // start conversion, no parasite power on at the end
//  delay(750);                          // 750ms should be enough for 12bit conv
}

boolean Ds18x20Read(uint8_t sensor, float &t)
{
  uint8_t data[9];
  int8_t sign = 1;
  float temp9 = 0.0;

  t = NAN;

  for (uint8_t retry = 0; retry < 3; retry++) {
    OneWireReset();
    OneWireSelect(ds18x20_address[ds18x20_index[sensor]]);
    OneWireWrite(W1_READ_SCRATCHPAD);
    for (uint8_t i = 0; i < 9; i++) {
      data[i] = OneWireRead();
    }
    if (OneWireCrc8(data)) {
      switch(ds18x20_address[ds18x20_index[sensor]][0]) {
      case DS18S20_CHIPID:
        if (data[1] > 0x80) {
          data[0] = (~data[0]) +1;
          sign = -1;                     // App-Note fix possible sign error
        }
        if (data[0] & 1) {
          temp9 = ((data[0] >> 1) + 0.5) * sign;
        } else {
          temp9 = (data[0] >> 1) * sign;
        }
        t = ConvertTemp((temp9 - 0.25) + ((16.0 - data[6]) / 16.0));
        break;
      case DS1822_CHIPID:
      case DS18B20_CHIPID:
        if (data[4] != 0x7F) {
          data[4] = 0x7F;                // Set resolution to 12-bit
          OneWireReset();
          OneWireSelect(ds18x20_address[ds18x20_index[sensor]]);
          OneWireWrite(W1_WRITE_SCRATCHPAD);
          OneWireWrite(data[2]);              // Th Register
          OneWireWrite(data[3]);              // Tl Register
          OneWireWrite(data[4]);              // Configuration Register
          OneWireSelect(ds18x20_address[ds18x20_index[sensor]]);
          OneWireWrite(W1_WRITE_EEPROM);      // Save scratchpad to EEPROM
        }
      case MAX31850_CHIPID:
        uint16_t temp12 = (data[1] << 8) + data[0];
        if (temp12 > 2047) {
          temp12 = (~temp12) +1;
          sign = -1;
        }
        t = ConvertTemp(sign * temp12 * 0.0625);
        break;
      }
    }
    if (!isnan(t)) {
      return true;
    }
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void Ds18x20Show(boolean json)
{
  char temperature[10];
  char stemp[12];
  float t;
  bool domoticz_flag = true;

  for (uint8_t i = 0; i < ds18x20_sensors; i++) {
    if (Ds18x20Read(i, t)) {           // Check if read failed
      dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

      uint8_t index = sizeof(ds18x20_chipids);
      while (index) {
        if (ds18x20_address[ds18x20_index[i]][0] == ds18x20_chipids[index]) {
          break;
        }
        index--;
      }
      GetTextIndexed(ds18x20_types, sizeof(ds18x20_types), index, kDs18x20Types);

      snprintf_P(stemp, sizeof(stemp), PSTR("%s-%d"), ds18x20_types, i +1);
      if (json) {
        if (1 == ds18x20_sensors) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_TEMPERATURE "\":%s}"), mqtt_data, ds18x20_types, temperature);
        } else {
          char address[17];
          for (byte j = 0; j < 6; j++) {
            sprintf(address+2*j, "%02X", ds18x20_address[ds18x20_index[i]][6-j]);  // Skip sensor type and crc
          }
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_ID "\":\"%s\",\"" D_TEMPERATURE "\":%s}"), mqtt_data, stemp, address, temperature);
        }
#ifdef USE_DOMOTICZ
        if (domoticz_flag) {
          DomoticzSensor(DZ_TEMP, temperature);
          domoticz_flag = false;
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, (1 == ds18x20_sensors) ? ds18x20_types : stemp, temperature, TempUnit());
#endif  // USE_WEBSERVER
      }
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
      case FUNC_XSNS_INIT:
        Ds18x20Init();
        break;
      case FUNC_XSNS_PREP:
        Ds18x20Convert();   // Start conversion, takes up to one second
        break;
      case FUNC_XSNS_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        Ds18x20Show(0);
        Ds18x20Convert();   // Start conversion, takes up to one second
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20
