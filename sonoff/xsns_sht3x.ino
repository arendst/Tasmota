/*
  xsns_sht3x.ino - SHT1x temperature and sensor support for Sonoff-Tasmota

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

#ifdef USE_I2C
#ifdef USE_SHT3X
#include "Wire.h"
/*********************************************************************************************\
 * SHT3x - Temperature and Humidy
 *
 * Reading temperature and humidity takes about TBD milliseconds!
 * Source: Wemos https://github.com/wemos/WEMOS_SHT3x_Arduino_Library/blob/master/src/WEMOS_SHT3X.cpp
\*********************************************************************************************/

enum {
  SHT3X_CMD_MEASURE_TEMP  = B00000011,
  SHT3X_CMD_MEASURE_RH    = B00000101,
  SHT3X_CMD_SOFT_RESET    = B00011110
};

uint8_t sht3x_sda_pin;
uint8_t sht3x_scl_pin;
uint8_t sht3x_type = 0;

boolean sht3x_reset()
{
  pinMode(sht3x_sda_pin, INPUT_PULLUP);
  pinMode(sht3x_scl_pin, OUTPUT);
  delay(11);
  for (byte i = 0; i < 9; i++) {
    digitalWrite(sht3x_scl_pin, HIGH);
    digitalWrite(sht3x_scl_pin, LOW);
  }
  boolean success = sht3x_sendCommand(SHT3X_CMD_SOFT_RESET);
  delay(11);
  return success;
}

boolean sht3x_sendCommand(const byte cmd)
{
  pinMode(sht3x_sda_pin, OUTPUT);
  // Transmission Start sequence
  digitalWrite(sht3x_sda_pin, HIGH);
  digitalWrite(sht3x_scl_pin, HIGH);
  digitalWrite(sht3x_sda_pin, LOW);
  digitalWrite(sht3x_scl_pin, LOW);
  digitalWrite(sht3x_scl_pin, HIGH);
  digitalWrite(sht3x_sda_pin, HIGH);
  digitalWrite(sht3x_scl_pin, LOW);
  // Send the command (address must be 000b)
  shiftOut(sht3x_sda_pin, sht1x_scl_pin, MSBFIRST, cmd);
  // Wait for ACK
  boolean ackerror = false;
  digitalWrite(sht3x_scl_pin, HIGH);
  pinMode(sht3x_sda_pin, INPUT_PULLUP);
  if (digitalRead(sht3x_sda_pin) != LOW) {
    ackerror = true;
  }
  digitalWrite(sht3x_scl_pin, LOW);
  delayMicroseconds(1);  // Give the sensor time to release the data line
  if (digitalRead(sht3x_sda_pin) != HIGH) {
    ackerror = true;
  }
  if (ackerror) {
    sht3x_type = 0;
    addLog_P(LOG_LEVEL_DEBUG, PSTR("SHT3X: Sensor did not ACK command"));
  }
  return (!ackerror);
}

boolean sht3x_awaitResult()
{
  // Maximum 320ms for 14 bit measurement
  for (byte i = 0; i < 16; i++) {
    if (LOW == digitalRead(sht3x_sda_pin)) {
      return true;
    }
    delay(20);
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("SHT3X: Data not ready"));
  sht3x_type = 0;
  return false;
}

int sht3x_readData()
{
  int val = 0;

  // Read most significant byte
  val = shiftIn(sht3x_sda_pin, sht1x_scl_pin, 8);
  val <<= 8;
  // Send ACK
  pinMode(sht3x_sda_pin, OUTPUT);
  digitalWrite(sht3x_sda_pin, LOW);
  digitalWrite(sht3x_scl_pin, HIGH);
  digitalWrite(sht3x_scl_pin, LOW);
  pinMode(sht3x_sda_pin, INPUT_PULLUP);
  // Read least significant byte
  val |= shiftIn(sht3x_sda_pin, sht1x_scl_pin, 8);
  // Keep DATA pin high to skip CRC
  digitalWrite(sht3x_scl_pin, HIGH);
  digitalWrite(sht3x_scl_pin, LOW);
  return val;
}

boolean sht3x_readTempHum(float &t, float &h)
{
  float tempRaw;
  float humRaw;
  float rhLinear;

  t = NAN;
  h = NAN;

  if (!sht3x_reset()) {
    return false;
  }
  if (!sht3x_sendCommand(SHT3X_CMD_MEASURE_TEMP)) {
    return false;
  }
  if (!sht3x_awaitResult()) {
    return false;
  }
  tempRaw = sht3x_readData();
  // Temperature conversion coefficients from SHT3X datasheet for version 4
  const float d1 = -39.7;  // 3.5V
  const float d2 = 0.01;   // 14-bit
  t = d1 + (tempRaw * d2);
  if (!sht3x_sendCommand(SHT3X_CMD_MEASURE_RH)) {
    return false;
  }
  if (!sht3x_awaitResult()) {
    return false;
  }
  humRaw = sht3x_readData();
  // Temperature conversion coefficients from SHT3X datasheet for version 4
  const float c1 = -2.0468;
  const float c2 = 0.0367;
  const float c3 = -1.5955E-6;
  const float t1 = 0.01;
  const float t2 = 0.00008;
  rhLinear = c1 + c2 * humRaw + c3 * humRaw * humRaw;
  h = (t - 25) * (t1 + t2 * humRaw) + rhLinear;
  t = convertTemp(t);
  return (!isnan(t) && !isnan(h));
}

boolean sht3x_readCharTempHum(char* temp, char* hum)
{
  float t;
  float h;

  boolean success = sht3x_readTempHum(t, h);
  dtostrf(t, 1, sysCfg.flag.temperature_resolution, temp);
  dtostrf(h, 1, sysCfg.flag.humidity_resolution, hum);
  return success;
}

boolean sht3x_detect()
{
  if (sht3x_type) {
    return true;
  }

  float t;
  float h;
  
  sht3x_sda_pin = pin[GPIO_I2C_SDA];
  sht3x_scl_pin = pin[GPIO_I2C_SCL];
  if (sht3x_readTempHum(t, h)) {
    sht3x_type = 1;
    addLog_P(LOG_LEVEL_DEBUG, PSTR("I2C: SHT3X found"));
  } else {
    Wire.begin(sht3x_sda_pin, sht1x_scl_pin);
    sht3x_type = 0;
  }
  return sht3x_type;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void sht3x_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!sht3x_type) {
    return;
  }

  char stemp[10];
  char shum[10];
  
  if (sht3x_readCharTempHum(stemp, shum)) {
    snprintf_P(svalue, ssvalue, JSON_SNS_TEMPHUM, svalue, "SHT3X", stemp, shum);
    *djson = 1;
#ifdef USE_DOMOTICZ
    domoticz_sensor2(stemp, shum);
#endif  // USE_DOMOTICZ
  }
}

#ifdef USE_WEBSERVER
String sht3x_webPresent()
{
  String page = "";
  if (sht3x_type) {
    char stemp[10];
    char shum[10];
    
    if (sht3x_readCharTempHum(stemp, shum)) {
      char sensor[80];
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, "SHT3X", stemp, tempUnit());
      page += sensor;
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, "SHT3X", shum);
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_SHT3X
#endif  // USE_I2C

