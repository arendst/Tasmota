/*
  xsns_07_sht1x.ino - SHT1x temperature and sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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
 * SHT1x - Temperature and Humidity
 *
 * Reading temperature and humidity takes about 320 milliseconds!
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy
 *
 * I2C Address: None
\*********************************************************************************************/

#define XSNS_07             7
#define XI2C_08             8  // See I2CDEVICES.md

enum {
  SHT1X_CMD_MEASURE_TEMP  = B00000011,
  SHT1X_CMD_MEASURE_RH    = B00000101,
  SHT1X_CMD_SOFT_RESET    = B00011110
};

uint8_t sht_sda_pin;
uint8_t sht_scl_pin;
uint8_t sht_type = 0;
char sht_types[] = "SHT1X";
uint8_t sht_valid = 0;
float sht_temperature = 0;
float sht_humidity = 0;

bool ShtReset(void)
{
  pinMode(sht_sda_pin, INPUT_PULLUP);
  pinMode(sht_scl_pin, OUTPUT);
  delay(11);
  for (uint32_t i = 0; i < 9; i++) {
    digitalWrite(sht_scl_pin, HIGH);
    digitalWrite(sht_scl_pin, LOW);
  }
  bool success = ShtSendCommand(SHT1X_CMD_SOFT_RESET);
  delay(11);
  return success;
}

bool ShtSendCommand(const uint8_t cmd)
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
  bool ackerror = false;
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
//    sht_type = 0;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_SHT1 D_SENSOR_DID_NOT_ACK_COMMAND));
  }
  return (!ackerror);
}

bool ShtAwaitResult(void)
{
  // Maximum 320ms for 14 bit measurement
  for (uint32_t i = 0; i < 16; i++) {
    if (LOW == digitalRead(sht_sda_pin)) {
      return true;
    }
    delay(20);
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_SHT1 D_SENSOR_BUSY));
//  sht_type = 0;
  return false;
}

int ShtReadData(void)
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

bool ShtRead(void)
{
  if (sht_valid) { sht_valid--; }
  if (!ShtReset()) { return false; }
  if (!ShtSendCommand(SHT1X_CMD_MEASURE_TEMP)) { return false; }
  if (!ShtAwaitResult()) { return false; }
  float tempRaw = ShtReadData();
  if (!ShtSendCommand(SHT1X_CMD_MEASURE_RH)) { return false; }
  if (!ShtAwaitResult()) { return false; }
  float humRaw = ShtReadData();

  // Temperature conversion coefficients from SHT1X datasheet for version 4
  const float d1 = -39.7;  // 3.5V
  const float d2 = 0.01;   // 14-bit
  sht_temperature = d1 + (tempRaw * d2);
  const float c1 = -2.0468;
  const float c2 = 0.0367;
  const float c3 = -1.5955E-6;
  const float t1 = 0.01;
  const float t2 = 0.00008;
  float rhLinear = c1 + c2 * humRaw + c3 * humRaw * humRaw;
  sht_humidity = (sht_temperature - 25) * (t1 + t2 * humRaw) + rhLinear;
  sht_temperature = ConvertTemp(sht_temperature);
  sht_humidity = ConvertHumidity(sht_humidity);

  sht_valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void ShtDetect(void)
{
  sht_sda_pin = Pin(GPIO_I2C_SDA);
  sht_scl_pin = Pin(GPIO_I2C_SCL);
  if (ShtRead()) {
    sht_type = 1;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SHT1X_FOUND));
  } else {
    Wire.begin(sht_sda_pin, sht_scl_pin);
    sht_type = 0;
  }
}

void ShtEverySecond(void)
{
  if (!(TasmotaGlobal.uptime %4)) {  // Every 4 seconds
    // 344mS
    if (!ShtRead()) {
      AddLogMissed(sht_types, sht_valid);
    }
  }
}

void ShtShow(bool json)
{
  if (sht_valid) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), sht_types, sht_temperature, sht_humidity);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns07(uint8_t function)
{
  if (!I2cEnabled(XI2C_08)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    ShtDetect();
  }
  else if (sht_type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        ShtEverySecond();
        break;
      case FUNC_JSON_APPEND:
        ShtShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ShtShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT
#endif  // USE_I2C
