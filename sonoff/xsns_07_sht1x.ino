/*
  xsns_07_sht1x.ino - SHT1x temperature and sensor support for Sonoff-Tasmota

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

#ifdef USE_I2C
#ifdef USE_SHT
/*********************************************************************************************\
 * SHT1x - Temperature and Humidy
 *
 * Reading temperature and humidity takes about 320 milliseconds!
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy
 *
 * I2C Address: None
\*********************************************************************************************/

enum {
  SHT1X_CMD_MEASURE_TEMP  = B00000011,
  SHT1X_CMD_MEASURE_RH    = B00000101,
  SHT1X_CMD_SOFT_RESET    = B00011110
};

uint8_t sht_sda_pin;
uint8_t sht_scl_pin;
uint8_t sht_type = 0;

boolean ShtReset()
{
  pinMode(sht_sda_pin, INPUT_PULLUP);
  pinMode(sht_scl_pin, OUTPUT);
  delay(11);
  for (byte i = 0; i < 9; i++) {
    digitalWrite(sht_scl_pin, HIGH);
    digitalWrite(sht_scl_pin, LOW);
  }
  boolean success = ShtSendCommand(SHT1X_CMD_SOFT_RESET);
  delay(11);
  return success;
}

boolean ShtSendCommand(const byte cmd)
{
  pinMode(sht_sda_pin, OUTPUT);
  // Transmission Start sequence
  digitalWrite(sht_sda_pin, HIGH);
  digitalWrite(sht_scl_pin, HIGH);
  digitalWrite(sht_sda_pin, LOW);
  digitalWrite(sht_scl_pin, LOW);
  digitalWrite(sht_scl_pin, HIGH);
  digitalWrite(sht_sda_pin, HIGH);
  digitalWrite(sht_scl_pin, LOW);
  // Send the command (address must be 000b)
  shiftOut(sht_sda_pin, sht_scl_pin, MSBFIRST, cmd);
  // Wait for ACK
  boolean ackerror = false;
  digitalWrite(sht_scl_pin, HIGH);
  pinMode(sht_sda_pin, INPUT_PULLUP);
  if (digitalRead(sht_sda_pin) != LOW) {
    ackerror = true;
  }
  digitalWrite(sht_scl_pin, LOW);
  delayMicroseconds(1);  // Give the sensor time to release the data line
  if (digitalRead(sht_sda_pin) != HIGH) {
    ackerror = true;
  }
  if (ackerror) {
    sht_type = 0;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_SHT1 D_SENSOR_DID_NOT_ACK_COMMAND));
  }
  return (!ackerror);
}

boolean ShtAwaitResult()
{
  // Maximum 320ms for 14 bit measurement
  for (byte i = 0; i < 16; i++) {
    if (LOW == digitalRead(sht_sda_pin)) {
      return true;
    }
    delay(20);
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_SHT1 D_SENSOR_BUSY));
  sht_type = 0;
  return false;
}

int ShtReadData()
{
  int val = 0;

  // Read most significant byte
  val = shiftIn(sht_sda_pin, sht_scl_pin, 8);
  val <<= 8;
  // Send ACK
  pinMode(sht_sda_pin, OUTPUT);
  digitalWrite(sht_sda_pin, LOW);
  digitalWrite(sht_scl_pin, HIGH);
  digitalWrite(sht_scl_pin, LOW);
  pinMode(sht_sda_pin, INPUT_PULLUP);
  // Read least significant byte
  val |= shiftIn(sht_sda_pin, sht_scl_pin, 8);
  // Keep DATA pin high to skip CRC
  digitalWrite(sht_scl_pin, HIGH);
  digitalWrite(sht_scl_pin, LOW);
  return val;
}

boolean ShtReadTempHum(float &t, float &h)
{
  float tempRaw;
  float humRaw;
  float rhLinear;

  t = NAN;
  h = NAN;

  if (!ShtReset()) {
    return false;
  }
  if (!ShtSendCommand(SHT1X_CMD_MEASURE_TEMP)) {
    return false;
  }
  if (!ShtAwaitResult()) {
    return false;
  }
  tempRaw = ShtReadData();
  // Temperature conversion coefficients from SHT1X datasheet for version 4
  const float d1 = -39.7;  // 3.5V
  const float d2 = 0.01;   // 14-bit
  t = d1 + (tempRaw * d2);
  if (!ShtSendCommand(SHT1X_CMD_MEASURE_RH)) {
    return false;
  }
  if (!ShtAwaitResult()) {
    return false;
  }
  humRaw = ShtReadData();
  // Temperature conversion coefficients from SHT1X datasheet for version 4
  const float c1 = -2.0468;
  const float c2 = 0.0367;
  const float c3 = -1.5955E-6;
  const float t1 = 0.01;
  const float t2 = 0.00008;
  rhLinear = c1 + c2 * humRaw + c3 * humRaw * humRaw;
  h = (t - 25) * (t1 + t2 * humRaw) + rhLinear;
  t = ConvertTemp(t);
  return (!isnan(t) && !isnan(h));
}

/********************************************************************************************/

void ShtDetect()
{
  if (sht_type) {
    return;
  }

  float t;
  float h;

  sht_sda_pin = pin[GPIO_I2C_SDA];
  sht_scl_pin = pin[GPIO_I2C_SCL];
  if (ShtReadTempHum(t, h)) {
    sht_type = 1;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SHT1X_FOUND));
  } else {
    Wire.begin(sht_sda_pin, sht_scl_pin);
    sht_type = 0;
  }
}

void ShtShow(boolean json)
{
  if (sht_type) {
    float t;
    float h;

    if (ShtReadTempHum(t, h)) {
      char temperature[10];
      char humidity[10];

      dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
      dtostrfd(h, Settings.flag2.humidity_resolution, humidity);

      if (json) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, "SHT1X", temperature, humidity);
#ifdef USE_DOMOTICZ
        DomoticzTempHumSensor(temperature, humidity);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "SHT1X", temperature, TempUnit());
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, "SHT1X", humidity);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_07

boolean Xsns07(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        ShtDetect();
        break;
      case FUNC_JSON_APPEND:
        ShtShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        ShtShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT
#endif  // USE_I2C
