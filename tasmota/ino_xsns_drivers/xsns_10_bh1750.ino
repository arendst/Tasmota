/*
  xsns_10_bh1750.ino - BH1750 ambient light sensor support for Tasmota

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
#ifdef USE_BH1750
/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
 *
 * Bh1750Resolution1   0..2    - Set BH1750 1 resolution mode
 * Bh1750Resolution2   0..2    - Set BH1750 2 resolution mode
 * Bh1750MTime1       30..255  - Set BH1750 1 MT register
 * Bh1750MTime2       30..255  - Set BH1750 2 MT register
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

#define D_PRFX_BH1750 "Bh1750"
#define D_CMND_RESOLUTION "Resolution"
#define D_CMND_MTREG "MTime"

const char kBh1750Commands[] PROGMEM = D_PRFX_BH1750 "|"  // Prefix
  D_CMND_RESOLUTION "|" D_CMND_MTREG ;

void (* const Bh1750Command[])(void) PROGMEM = {
  &CmndBh1750Resolution, &CmndBh1750MTime };

struct {
  uint8_t addresses[2] = { BH1750_ADDR1, BH1750_ADDR2 };
  uint8_t resolution[3] = { BH1750_CONTINUOUS_HIGH_RES_MODE, BH1750_CONTINUOUS_HIGH_RES_MODE2, BH1750_CONTINUOUS_LOW_RES_MODE };
  uint8_t count = 0;
  char types[7] = "BH1750";
} Bh1750;

struct {
  uint8_t address;
  uint8_t valid = 0;
  uint8_t mtreg = 69;                          // Default Measurement Time
  uint16_t illuminance = 0;
} Bh1750_sensors[2];

/*********************************************************************************************/

uint8_t Bh1750Resolution(uint32_t sensor_index) {
  uint8_t settings_resolution = Settings->SensorBits1.bh1750_1_resolution;
  if (1 == sensor_index) {
    settings_resolution = Settings->SensorBits1.bh1750_2_resolution;
  }
  return settings_resolution;
}

bool Bh1750SetResolution(uint32_t sensor_index) {
  Wire.beginTransmission(Bh1750_sensors[sensor_index].address);
  Wire.write(Bh1750.resolution[Bh1750Resolution(sensor_index)]);
  return (!Wire.endTransmission());
}

bool Bh1750SetMTreg(uint32_t sensor_index) {
  Wire.beginTransmission(Bh1750_sensors[sensor_index].address);
  uint8_t data = BH1750_MEASUREMENT_TIME_HIGH | ((Bh1750_sensors[sensor_index].mtreg >> 5) & 0x07);
  Wire.write(data);
  if (Wire.endTransmission()) { return false; }
  Wire.beginTransmission(Bh1750_sensors[sensor_index].address);
  data = BH1750_MEASUREMENT_TIME_LOW | (Bh1750_sensors[sensor_index].mtreg & 0x1F);
  Wire.write(data);
  if (Wire.endTransmission()) { return false; }
  return Bh1750SetResolution(sensor_index);
}

bool Bh1750Read(uint32_t sensor_index) {
  if (Bh1750_sensors[sensor_index].valid) { Bh1750_sensors[sensor_index].valid--; }

  if (2 != Wire.requestFrom(Bh1750_sensors[sensor_index].address, (uint8_t)2)) { return false; }

  float illuminance = (Wire.read() << 8) | Wire.read();
  illuminance /= (1.2 * (69 / (float)Bh1750_sensors[sensor_index].mtreg));
  if (1 == Bh1750Resolution(sensor_index)) {
    illuminance /= 2;
  }
  Bh1750_sensors[sensor_index].illuminance = illuminance;

  Bh1750_sensors[sensor_index].valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void Bh1750Detect(void) {
  for (uint32_t i = 0; i < sizeof(Bh1750.addresses); i++) {
    if (!I2cSetDevice(Bh1750.addresses[i])) { continue; }

    Bh1750_sensors[Bh1750.count].address = Bh1750.addresses[i];
    if (Bh1750SetMTreg(Bh1750.count)) {
      I2cSetActiveFound(Bh1750_sensors[Bh1750.count].address, Bh1750.types);
      Bh1750.count++;
    }
  }
}

void Bh1750EverySecond(void) {
  for (uint32_t i = 0; i < Bh1750.count; i++) {
    // 1mS
    if (!Bh1750Read(i)) {
//      AddLogMissed(Bh1750.types, Bh1750.valid);
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndBh1750Resolution(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Bh1750.count)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      if (1 == XdrvMailbox.index) {
        Settings->SensorBits1.bh1750_1_resolution = XdrvMailbox.payload;
      } else {
        Settings->SensorBits1.bh1750_2_resolution = XdrvMailbox.payload;
      }
      Bh1750SetResolution(XdrvMailbox.index -1);
    }
    ResponseCmndIdxNumber(Bh1750Resolution(XdrvMailbox.index -1));
  }
}

void CmndBh1750MTime(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Bh1750.count)) {
    if ((XdrvMailbox.payload > 30) && (XdrvMailbox.payload < 255)) {
      Bh1750_sensors[XdrvMailbox.index -1].mtreg = XdrvMailbox.payload;
      Bh1750SetMTreg(XdrvMailbox.index -1);
    }
    ResponseCmndIdxNumber(Bh1750_sensors[XdrvMailbox.index -1].mtreg);
  }
}

/********************************************************************************************/

void Bh1750Show(bool json) {
  for (uint32_t sensor_index = 0; sensor_index < Bh1750.count; sensor_index++) {
    if (Bh1750_sensors[sensor_index].valid) {
      char sensor_name[10];
      strlcpy(sensor_name, Bh1750.types, sizeof(sensor_name));
      if (Bh1750.count > 1) {
        snprintf_P(sensor_name, sizeof(sensor_name), PSTR("%s%c%02X"), sensor_name, IndexSeparator(), Bh1750_sensors[sensor_index].address);  // BH1750-23
      }

      if (json) {
        ResponseAppend_P(JSON_SNS_ILLUMINANCE, sensor_name, Bh1750_sensors[sensor_index].illuminance);
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == sensor_index)) {
          DomoticzSensor(DZ_ILLUMINANCE, Bh1750_sensors[sensor_index].illuminance);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_ILLUMINANCE, sensor_name, Bh1750_sensors[sensor_index].illuminance);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns10(uint8_t function) {
  if (!I2cEnabled(XI2C_11)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Bh1750Detect();
  }
  else if (Bh1750.count) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Bh1750EverySecond();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kBh1750Commands, Bh1750Command);
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
