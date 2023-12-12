/*
  xsns_08_htu21.ino - HTU21 temperature and humidity sensor support for Tasmota

  Copyright (C) 2021  Heiko Krupp and Theo Arends

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
#ifdef USE_HTU
/*********************************************************************************************\
 * HTU21 - Temperature and Humidity
 *
 * Source: Heiko Krupp
 *
 * I2C Address: 0x40
\*********************************************************************************************/

#define XSNS_08             8
#define XI2C_09             9       // See I2CDEVICES.md

#define HTU21_ADDR          0x40

#define SI7013_CHIPID       0x0D
#define SI7020_CHIPID       0x14
#define SI7021_CHIPID       0x15
#define HTU21_CHIPID        0x32

#define HTU21_READTEMP      0xE3
#define HTU21_READHUM       0xE5
#define HTU21_WRITEREG      0xE6
#define HTU21_READREG       0xE7
#define HTU21_RESET         0xFE
#define HTU21_HEATER_WRITE  0x51
#define HTU21_HEATER_READ   0x11
#define HTU21_SERIAL2_READ1 0xFC    /* Read 3rd two Serial bytes */
#define HTU21_SERIAL2_READ2 0xC9    /* Read 4th two Serial bytes */

#define HTU21_HEATER_ON     0x04
#define HTU21_HEATER_OFF    0xFB

#define HTU21_RES_RH12_T14  0x00  // Default
#define HTU21_RES_RH8_T12   0x01
#define HTU21_RES_RH10_T13  0x80
#define HTU21_RES_RH11_T11  0x81

#define HTU21_CRC8_POLYNOM  0x13100

const char kHtuTypes[] PROGMEM = "HTU21|SI7013|SI7020|SI7021|T/RH?";

struct {
  float temperature = 0;
  float humidity = 0;
  uint8_t address;
  uint8_t bus;
  uint8_t type = 0;
  uint8_t delay_temp;
  uint8_t delay_humidity = 50;
  uint8_t valid = 0;
  char types[7];
} Htu;

/*********************************************************************************************/

uint8_t HtuCheckCrc8(uint16_t data) {
  for (uint32_t bit = 0; bit < 16; bit++) {
    if (data & 0x8000) {
      data =  (data << 1) ^ HTU21_CRC8_POLYNOM;
    } else {
      data <<= 1;
    }
  }
  return data >>= 8;
}

bool HtuReset(void) {
  TwoWire& myWire = I2cGetWire(Htu.bus);
  if (&myWire == nullptr) { return false; }  // No valid I2c bus

  myWire.beginTransmission(HTU21_ADDR);
  myWire.write(HTU21_RESET);
  myWire.endTransmission();
  delay(15);                // Reset takes 15ms
  return true;
}

uint8_t HtuReadDeviceId(void) {
  if (!HtuReset()) { return 0; };  // Fixes ESP32 sensor loss at restart

  uint16_t deviceID = 0;
  uint8_t checksum = 0;

  TwoWire& myWire = I2cGetWire(Htu.bus);
  myWire.beginTransmission(HTU21_ADDR);
  myWire.write(HTU21_SERIAL2_READ1);
  myWire.write(HTU21_SERIAL2_READ2);
  myWire.endTransmission();

  myWire.requestFrom(HTU21_ADDR, 3);
  deviceID  = myWire.read() << 8;
  deviceID |= myWire.read();
  checksum  = myWire.read();
  if (HtuCheckCrc8(deviceID) == checksum) {
    deviceID = deviceID >> 8;
  } else {
    deviceID = 0;
  }
  return (uint8_t)deviceID;
}

void HtuSetResolution(uint8_t resolution) {
  uint8_t current = I2cRead8(HTU21_ADDR, HTU21_READREG, Htu.bus);
  current &= 0x7E;          // Replace current resolution bits with 0
  current |= resolution;    // Add new resolution bits to register
  I2cWrite8(HTU21_ADDR, HTU21_WRITEREG, current, Htu.bus);
}

void HtuHeater(uint8_t heater) {
  uint8_t current = I2cRead8(HTU21_ADDR, HTU21_READREG, Htu.bus);

  switch(heater) {
    case HTU21_HEATER_ON  : current |= heater;
                            break;
    case HTU21_HEATER_OFF : current &= heater;
                            break;
    default               : current &= heater;
                            break;
  }
  I2cWrite8(HTU21_ADDR, HTU21_WRITEREG, current, Htu.bus);
}

void HtuInit(void) {
  HtuReset();
  HtuHeater(HTU21_HEATER_OFF);
  HtuSetResolution(HTU21_RES_RH12_T14);
}

bool HtuRead(void) {
  uint8_t  checksum = 0;
  uint16_t sensorval = 0;

  if (Htu.valid) { Htu.valid--; }

  TwoWire& myWire = I2cGetWire(Htu.bus);
  myWire.beginTransmission(HTU21_ADDR);
  myWire.write(HTU21_READTEMP);
  if (myWire.endTransmission() != 0) { return false; }           // In case of error
  delay(Htu.delay_temp);                                       // Sensor time at max resolution

  myWire.requestFrom(HTU21_ADDR, 3);
  if (3 == myWire.available()) {
    sensorval = myWire.read() << 8;                              // MSB
    sensorval |= myWire.read();                                  // LSB
    checksum = myWire.read();
  }
  if (HtuCheckCrc8(sensorval) != checksum) { return false; }   // Checksum mismatch

  Htu.temperature = ConvertTemp(0.002681f * (float)sensorval - 46.85f);

  myWire.beginTransmission(HTU21_ADDR);
  myWire.write(HTU21_READHUM);
  if (myWire.endTransmission() != 0) { return false; }           // In case of error
  delay(Htu.delay_humidity);                                   // Sensor time at max resolution

  myWire.requestFrom(HTU21_ADDR, 3);
  if (3 <= myWire.available()) {
    sensorval = myWire.read() << 8;                              // MSB
    sensorval |= myWire.read();                                  // LSB
    checksum = myWire.read();
  }
  if (HtuCheckCrc8(sensorval) != checksum) { return false; }   // Checksum mismatch

  sensorval ^= 0x02;                                           // clear status bits
  Htu.humidity = 0.001907f * (float)sensorval - 6;
  if (Htu.humidity > 100) { Htu.humidity = 100.0f; }
  if (Htu.humidity < 0) { Htu.humidity = 0.01f; }

  if ((0.00f == Htu.humidity) && (0.00f == Htu.temperature)) {
    Htu.humidity = 0.0f;
  }
  if ((Htu.temperature > 0.00f) && (Htu.temperature < 80.00f)) {
    Htu.humidity = (-0.15f) * (25 - Htu.temperature) + Htu.humidity;
  }
  Htu.humidity = ConvertHumidity(Htu.humidity);

  Htu.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void HtuDetect(void) {
  Htu.address = HTU21_ADDR;
  for (Htu.bus = 0; Htu.bus < 2; Htu.bus++) {
    if (!I2cSetDevice(Htu.address, Htu.bus)) { continue; }

    Htu.type = HtuReadDeviceId();
    if (Htu.type) {
      uint8_t index = 0;
      HtuInit();
      switch (Htu.type) {
        case HTU21_CHIPID:
          Htu.delay_temp = 50;
          Htu.delay_humidity = 16;
          break;
        case SI7021_CHIPID:
          index++;  // 3
        case SI7020_CHIPID:
          index++;  // 2
        case SI7013_CHIPID:
          index++;  // 1
          Htu.delay_temp = 12;
          Htu.delay_humidity = 23;
          break;
        default:
          index = 4;
          Htu.delay_temp = 50;
          Htu.delay_humidity = 23;
      }
      GetTextIndexed(Htu.types, sizeof(Htu.types), index, kHtuTypes);
      I2cSetActiveFound(Htu.address, Htu.types, Htu.bus);
      break;
    }
  }
}

void HtuEverySecond(void) {
  if (TasmotaGlobal.uptime &1) {  // Every 2 seconds
    // HTU21: 68mS, SI70xx: 37mS
    if (!HtuRead()) {
      AddLogMissed(Htu.types, Htu.valid);
    }
  }
}

void HtuShow(bool json) {
  if (Htu.valid) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), Htu.types, Htu.temperature, Htu.humidity);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns08(uint32_t function)
{
  if (!I2cEnabled(XI2C_09)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    HtuDetect();
  }
  else if (Htu.type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        HtuEverySecond();
        break;
      case FUNC_JSON_APPEND:
        HtuShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HtuShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HTU
#endif  // USE_I2C

