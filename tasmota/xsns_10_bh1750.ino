/*
  xsns_10_bh1750.ino - BH1750 ambient light sensor support for Tasmota

  Copyright (C) 2020  Theo Arends

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
#ifdef USE_BH1750
/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
 *
 * I2C Address: 0x23 or 0x5C
\*********************************************************************************************/

#define XSNS_10                          10
#define XI2C_11                          11    // See I2CDEVICES.md

#define BH1750_ADDR1                     0x23
#define BH1750_ADDR2                     0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE2 0x11  // Start measurement at 0.5 lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_HIGH_RES_MODE  0x10  // Start measurement at 1   lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_LOW_RES_MODE   0x13  // Start measurement at 4   lx resolution. Measurement time is approx 16ms.

#define BH1750_MEASUREMENT_TIME_HIGH     0x40  // Measurement Time register high 3 bits
#define BH1750_MEASUREMENT_TIME_LOW      0x60  // Measurement Time register low 5 bits

struct BH1750DATA {
  uint8_t address;
  uint8_t addresses[2] = { BH1750_ADDR1, BH1750_ADDR2 };
  uint8_t resolution[3] = { BH1750_CONTINUOUS_HIGH_RES_MODE, BH1750_CONTINUOUS_HIGH_RES_MODE2, BH1750_CONTINUOUS_LOW_RES_MODE };
  uint8_t type = 0;
  uint8_t valid = 0;
  uint8_t mtreg = 69;                          // Default Measurement Time
  uint16_t illuminance = 0;
  char types[7] = "BH1750";
} Bh1750;

/*********************************************************************************************/

bool Bh1750SetResolution(void)
{
  Wire.beginTransmission(Bh1750.address);
  Wire.write(Bh1750.resolution[Settings.SensorBits1.bh1750_resolution]);
  return (!Wire.endTransmission());
}

bool Bh1750SetMTreg(void)
{
  Wire.beginTransmission(Bh1750.address);
  uint8_t data = BH1750_MEASUREMENT_TIME_HIGH | ((Bh1750.mtreg >> 5) & 0x07);
  Wire.write(data);
  if (Wire.endTransmission()) { return false; }
  Wire.beginTransmission(Bh1750.address);
  data = BH1750_MEASUREMENT_TIME_LOW | (Bh1750.mtreg & 0x1F);
  Wire.write(data);
  if (Wire.endTransmission()) { return false; }
  return Bh1750SetResolution();
}

bool Bh1750Read(void)
{
  if (Bh1750.valid) { Bh1750.valid--; }

  if (2 != Wire.requestFrom(Bh1750.address, (uint8_t)2)) { return false; }
  float illuminance = (Wire.read() << 8) | Wire.read();
  illuminance /= (1.2 * (69 / (float)Bh1750.mtreg));
  if (1 == Settings.SensorBits1.bh1750_resolution) {
    illuminance /= 2;
  }
  Bh1750.illuminance = illuminance;

  Bh1750.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void Bh1750Detect(void)
{
  for (uint32_t i = 0; i < sizeof(Bh1750.addresses); i++) {
    Bh1750.address = Bh1750.addresses[i];
    if (I2cActive(Bh1750.address)) { continue; }

    if (Bh1750SetMTreg()) {
      I2cSetActiveFound(Bh1750.address, Bh1750.types);
      Bh1750.type = 1;
      break;
    }
  }
}

void Bh1750EverySecond(void)
{
  // 1mS
  if (!Bh1750Read()) {
    AddLogMissed(Bh1750.types, Bh1750.valid);
  }
}

/*********************************************************************************************\
 * Command Sensor10
 *
 * 0       - High resolution mode (default)
 * 1       - High resolution mode 2
 * 2       - Low resolution mode
 * 31..254 - Measurement Time value (not persistent, default is 69)
\*********************************************************************************************/

bool Bh1750CommandSensor(void)
{
  if (XdrvMailbox.data_len) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      Settings.SensorBits1.bh1750_resolution = XdrvMailbox.payload;
      Bh1750SetResolution();
    }
    else if ((XdrvMailbox.payload > 30) && (XdrvMailbox.payload < 255)) {
      Bh1750.mtreg = XdrvMailbox.payload;
      Bh1750SetMTreg();
    }
  }
  Response_P(PSTR("{\"" D_CMND_SENSOR "10\":{\"Resolution\":%d,\"MTime\":%d}}"), Settings.SensorBits1.bh1750_resolution, Bh1750.mtreg);

  return true;
}

void Bh1750Show(bool json)
{
  if (Bh1750.valid) {
    if (json) {
      ResponseAppend_P(JSON_SNS_ILLUMINANCE, Bh1750.types, Bh1750.illuminance);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_ILLUMINANCE, Bh1750.illuminance);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ILLUMINANCE, Bh1750.types, Bh1750.illuminance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns10(uint8_t function)
{
  if (!I2cEnabled(XI2C_11)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Bh1750Detect();
  }
  else if (Bh1750.type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Bh1750EverySecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_10 == XdrvMailbox.index) {
          result = Bh1750CommandSensor();
        }
        break;
      case FUNC_JSON_APPEND:
        Bh1750Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Bh1750Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_BH1750
#endif  // USE_I2C
