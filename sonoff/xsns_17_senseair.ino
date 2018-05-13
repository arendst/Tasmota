/*
  xsns_17_senseair.ino - SenseAir CO2 sensor support for Sonoff-Tasmota

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

#ifdef USE_SENSEAIR
/*********************************************************************************************\
 * SenseAir K30, K70 and S8 - CO2 sensor
 *
 * Adapted from EspEasy plugin P052 by Mikael Trieb (mikael__AT__triebconsulting.se)
 *
 * Hardware Serial will be selected if GPIO1 = [SAir Rx] and GPIO3 = [SAir Tx]
\*********************************************************************************************/

#include <TasmotaSerial.h>

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

TasmotaSerial *SensairSerial;

const char kSenseairTypes[] PROGMEM = "Kx0|S8";

uint8_t senseair_type = 1;
char senseair_types[7];

uint16_t senseair_co2 = 0;
float senseair_temperature = 0;
float senseair_humidity = 0;

uint8_t senseair_state = 0;

/*********************************************************************************************/

void ModbusSend(uint8_t function_code, uint16_t start_address, uint16_t register_count)
{
  uint8_t frame[8];

  frame[0] = 0xFE;  // Any Address
  frame[1] = function_code;
  frame[2] = (uint8_t)(start_address >> 8);
  frame[3] = (uint8_t)(start_address);
  frame[4] = (uint8_t)(register_count >> 8);
  frame[5] = (uint8_t)(register_count);
  uint16_t crc = 0xFFFF;
  for (uint8_t pos = 0; pos < sizeof(frame) -2; pos++) {
    crc ^= (uint16_t)frame[pos];        // XOR byte into least sig. byte of crc
    for (uint8_t i = 8; i != 0; i--) {  // Loop over each bit
      if ((crc & 0x0001) != 0) {        // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else {                            // Else LSB is not set
        crc >>= 1;                      // Just shift right
      }
    }
  }
  frame[7] = (uint8_t)((crc >> 8) & 0xFF);
  frame[6] = (uint8_t)(crc & 0xFF);

  SensairSerial->flush();
  SensairSerial->write(frame, sizeof(frame));
}

bool ModbusReceiveReady()
{
  return (SensairSerial->available() >= 5);  // 5 - Error frame, 7 - Ok frame
}

uint8_t ModbusReceive(uint16_t *value)
{
  uint8_t buffer[7];

  uint8_t len = 0;
  while (SensairSerial->available() > 0) {
    buffer[len++] = (uint8_t)SensairSerial->read();
    if (3 == len) {
      if (buffer[1] & 0x80) {  // fe 84 02 f2 f1
        return buffer[2];      // 1 = Illegal Function, 2 = Illegal Data Address, 3 = Illegal Data Value
      }
    }
  }

  AddLogSerial(LOG_LEVEL_DEBUG_MORE, buffer, len);

  if (len != sizeof(buffer)) {
    return 9;                  // 9 = Unexpected result
  }
  *value = (buffer[3] << 8) | buffer[4];
  return 0;                    // 0 = No error
}

/*********************************************************************************************/

const uint8_t start_addresses[] { 0x1A, 0x00, 0x03, 0x04, 0x05, 0x1C, 0x0A };

uint8_t senseair_read_state = 0;
uint8_t senseair_send_retry = 0;

void Senseair50ms()              // Every 50 mSec
{
  senseair_state++;
  if (6 == senseair_state) {     // Every 300 mSec
    senseair_state = 0;

    uint16_t value = 0;
    bool data_ready = ModbusReceiveReady();

    if (data_ready) {
      uint8_t error = ModbusReceive(&value);
      if (error) {
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir response error %d"), error);
        AddLog(LOG_LEVEL_DEBUG);
      } else {
        switch(senseair_read_state) {
          case 0:                // 0x1A (26) READ_TYPE_LOW - S8: fe 04 02 01 77 ec 92
            senseair_type = 2;
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir type id low %04X"), value);
            AddLog(LOG_LEVEL_DEBUG);
            break;
          case 1:                // 0x00 (0) READ_ERRORLOG - fe 04 02 00 00 ad 24
            if (value) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir error %04X"), value);
              AddLog(LOG_LEVEL_DEBUG);
            }
            break;
          case 2:                // 0x03 (3) READ_CO2 - fe 04 02 06 2c af 59
            senseair_co2 = value;
            LightSetSignal(CO2_LOW, CO2_HIGH, senseair_co2);
            break;
          case 3:                // 0x04 (4) READ_TEMPERATURE - S8: fe 84 02 f2 f1 - Illegal Data Address
            senseair_temperature = ConvertTemp((float)value / 100);
            break;
          case 4:                // 0x05 (5) READ_HUMIDITY - S8: fe 84 02 f2 f1 - Illegal Data Address
            senseair_humidity = (float)value / 100;
            break;
          case 5:                // 0x1C (28) READ_RELAY_STATE - S8: fe 04 02 01 54 ad 4b - firmware version
          {
            bool relay_state = value >> 8 & 1;
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir relay state %d"), relay_state);
            AddLog(LOG_LEVEL_DEBUG);
            break;
          }
          case 6:                // 0x0A (10) READ_TEMP_ADJUSTMENT - S8: fe 84 02 f2 f1 - Illegal Data Address
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir temp adjustment %d"), value);
            AddLog(LOG_LEVEL_DEBUG);
            break;
        }
      }
      senseair_read_state++;
      if (2 == senseair_type) {  // S8
        if (3 == senseair_read_state) {
          senseair_read_state = 1;
        }
      } else {                   // K30, K70
        if (sizeof(start_addresses) == senseair_read_state) {
          senseair_read_state = 1;
        }
      }
    }

    if (0 == senseair_send_retry || data_ready) {
      senseair_send_retry = 5;
      ModbusSend(0x04, (uint16_t)start_addresses[senseair_read_state], 1);
    } else {
      senseair_send_retry--;
    }

  }
}

/*********************************************************************************************/

void SenseairInit()
{
  senseair_type = 0;
  if ((pin[GPIO_SAIR_RX] < 99) && (pin[GPIO_SAIR_TX] < 99)) {
    SensairSerial = new TasmotaSerial(pin[GPIO_SAIR_RX], pin[GPIO_SAIR_TX], 1);
    if (SensairSerial->begin(9600)) {
      if (SensairSerial->hardwareSerial()) { ClaimSerial(); }
      senseair_type = 1;
    }
  }
}

void SenseairShow(boolean json)
{
  char temperature[10];
  char humidity[10];
  dtostrfd(senseair_temperature, Settings.flag2.temperature_resolution, temperature);
  dtostrfd(senseair_humidity, Settings.flag2.temperature_resolution, humidity);
  GetTextIndexed(senseair_types, sizeof(senseair_types), senseair_type -1, kSenseairTypes);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_CO2 "\":%d"), mqtt_data, senseair_types, senseair_co2);
    if (senseair_type != 2) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s"), mqtt_data, temperature, humidity);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
#ifdef USE_DOMOTICZ
    if (0 == tele_period) DomoticzSensor(DZ_AIRQUALITY, senseair_co2);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_CO2, mqtt_data, senseair_types, senseair_co2);
    if (senseair_type != 2) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, senseair_types, temperature, TempUnit());
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, senseair_types, humidity);
    }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_17

boolean Xsns17(byte function)
{
  boolean result = false;

  if (senseair_type) {
    switch (function) {
      case FUNC_INIT:
        SenseairInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        Senseair50ms();
        break;
      case FUNC_JSON_APPEND:
        SenseairShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SenseairShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SENSEAIR
