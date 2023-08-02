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
 * I2C Address: None and ruins I2C bus hence reinit I2C after each call
\*********************************************************************************************/

#define XSNS_07             7
#define XI2C_08             8  // See I2CDEVICES.md

enum {
  SHT1X_CMD_MEASURE_TEMP  = B00000011,
  SHT1X_CMD_MEASURE_RH    = B00000101,
  SHT1X_CMD_SOFT_RESET    = B00011110
};

struct {
  float temperature = 0;
  float humidity = 0;
  int8_t sda_pin;
  int8_t scl_pin;
  uint8_t type = 0;
  uint8_t valid = 0;
  char types[6] = "SHT1X";
} Sht1x;

bool ShtReset(void) {
  pinMode(Sht1x.sda_pin, INPUT_PULLUP);
  pinMode(Sht1x.scl_pin, OUTPUT);
  delay(11);
  for (uint32_t i = 0; i < 9; i++) {
    digitalWrite(Sht1x.scl_pin, HIGH);
    digitalWrite(Sht1x.scl_pin, LOW);
  }
  bool success = ShtSendCommand(SHT1X_CMD_SOFT_RESET);
  delay(11);
  return success;
}

bool ShtSendCommand(const uint8_t cmd) {
  pinMode(Sht1x.sda_pin, OUTPUT);
  // Transmission Start sequence
  digitalWrite(Sht1x.sda_pin, HIGH);
  digitalWrite(Sht1x.scl_pin, HIGH);
  digitalWrite(Sht1x.sda_pin, LOW);
  digitalWrite(Sht1x.scl_pin, LOW);
  digitalWrite(Sht1x.scl_pin, HIGH);
  digitalWrite(Sht1x.sda_pin, HIGH);
  digitalWrite(Sht1x.scl_pin, LOW);
  // Send the command (address must be 000b)
  TasShiftOut(Sht1x.sda_pin, Sht1x.scl_pin, MSBFIRST, cmd);
  // Wait for ACK
  bool ackerror = false;
  digitalWrite(Sht1x.scl_pin, HIGH);
  pinMode(Sht1x.sda_pin, INPUT_PULLUP);
  if (digitalRead(Sht1x.sda_pin) != LOW) {
    ackerror = true;
  }
  digitalWrite(Sht1x.scl_pin, LOW);
  delayMicroseconds(1);  // Give the sensor time to release the data line
  if (digitalRead(Sht1x.sda_pin) != HIGH) {
    ackerror = true;
  }
  if (ackerror) {
//    Sht1x.type = 0;
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SHT1 D_SENSOR_DID_NOT_ACK_COMMAND));
  }
  return (!ackerror);
}

bool ShtAwaitResult(void) {
  // Maximum 320ms for 14 bit measurement
  for (uint32_t i = 0; i < 16; i++) {
    if (LOW == digitalRead(Sht1x.sda_pin)) {
      return true;
    }
    delay(20);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SHT1 D_SENSOR_BUSY));
//  Sht1x.type = 0;
  return false;
}

int ShtReadData(void) {
  int val = 0;

  // Read most significant byte
  val = TasShiftIn(Sht1x.sda_pin, Sht1x.scl_pin, 8);
  val <<= 8;
  // Send ACK
  pinMode(Sht1x.sda_pin, OUTPUT);
  digitalWrite(Sht1x.sda_pin, LOW);
  digitalWrite(Sht1x.scl_pin, HIGH);
  digitalWrite(Sht1x.scl_pin, LOW);
  pinMode(Sht1x.sda_pin, INPUT_PULLUP);
  // Read least significant byte
  val |= TasShiftIn(Sht1x.sda_pin, Sht1x.scl_pin, 8);
  // Keep DATA pin high to skip CRC
  digitalWrite(Sht1x.scl_pin, HIGH);
  digitalWrite(Sht1x.scl_pin, LOW);
  return val;
}

bool ShtRead(void) {
  if (Sht1x.valid) { Sht1x.valid--; }
  if (!ShtReset()) { return false; }
  if (!ShtSendCommand(SHT1X_CMD_MEASURE_TEMP)) { return false; }
  if (!ShtAwaitResult()) { return false; }
  float tempRaw = ShtReadData();
  if (!ShtSendCommand(SHT1X_CMD_MEASURE_RH)) { return false; }
  if (!ShtAwaitResult()) { return false; }
  float humRaw = ShtReadData();

  // Temperature conversion coefficients from SHT1X datasheet for version 4
  const float d1 = -39.7f;  // 3.5V
  const float d2 = 0.01f;   // 14-bit
  Sht1x.temperature = d1 + (tempRaw * d2);
  const float c1 = -2.0468f;
  const float c2 = 0.0367f;
  const float c3 = -1.5955E-6f;
  const float t1 = 0.01f;
  const float t2 = 0.00008f;
  float rhLinear = c1 + c2 * humRaw + c3 * humRaw * humRaw;
  Sht1x.humidity = (Sht1x.temperature - 25) * (t1 + t2 * humRaw) + rhLinear;
  Sht1x.temperature = ConvertTemp(Sht1x.temperature);
  Sht1x.humidity = ConvertHumidity(Sht1x.humidity);

  Sht1x.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void ShtDetect(void) {
  Sht1x.sda_pin = Pin(GPIO_I2C_SDA);
  Sht1x.scl_pin = Pin(GPIO_I2C_SCL);
  if (ShtRead()) {
    Sht1x.type = 1;
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SHT1X_FOUND));
  }
  I2cBegin(Sht1x.sda_pin, Sht1x.scl_pin);    // Reinit I2C bus
}

void ShtEverySecond(void) {
  if (!(TasmotaGlobal.uptime %4)) {          // Every 4 seconds
    // 344mS
    if (!ShtRead()) {
      AddLogMissed(Sht1x.types, Sht1x.valid);
    }
    I2cBegin(Sht1x.sda_pin, Sht1x.scl_pin);  // Reinit I2C bus
  }
}

void ShtShow(bool json) {
  if (Sht1x.valid) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), Sht1x.types, Sht1x.temperature, Sht1x.humidity);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns07(uint32_t function) {
  if (!I2cEnabled(XI2C_08)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    ShtDetect();
  }
  else if (Sht1x.type) {
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
