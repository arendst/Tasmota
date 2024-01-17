/*
  xsns_114_ams.ino - AMS5915, AMS6915 pressure and temperature sensor support for Tasmota

  Copyright (C) 2023 Bastian Urschel

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
#ifdef USE_AMSX915
/*********************************************************************************************\
 * AMS5915, AMS6915 - Pressure and Temperature
 *
 * Source: Bastian Urschel
 *
 * I2C Address: 0x28
\*********************************************************************************************/

#define XSNS_114            114
#define XI2C_86              86  // See I2CDEVICES.md

#ifndef AMSX915_ADDR
  #define AMSX915_ADDR         0x28
#endif

#define PMIN_DEFAULT      0
#define PMAX_DEFAULT      0

#ifndef USE_UFILESYS
#warning "AMSx915 pressure settings cannot be saved persistent due to missing filesystem"
#endif

/********************************************************************************************/

struct amsx915_data {
  float pressure;
  float temperature;
  bool present;
} amsx915data;

const uint16_t AMSX915_VERSION = 0x0100;       // Latest sensor version (See settings deltas below)

// Global structure containing driver saved variables
struct {
  uint32_t  crc32;    // To detect file changes
  uint16_t  version;  // To detect driver function changes
  int16_t   pmin;
  int16_t   pmax;
} amsx915Settings;

bool Amsx915Command() {
  int32_t vals[2];
  ParseParameters(2,(uint32_t *)vals);
  if(XdrvMailbox.data_len >= 3 && XdrvMailbox.data_len < 13) {
    if (vals[0] >= -32768 && vals[0] < 32768) {
      if (vals[1] >= -32768 && vals[1] < 32768) {
        amsx915Settings.pmin = (int16_t)vals[0]; // save pmin of sensor
        amsx915Settings.pmax = (int16_t)vals[1];  // same with pmax
        Amsx915SettingsSave();
        Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_114, "pressure range set");
        return true;
      }
    }
    Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_114, "invalid pressure range [-32768..32767]");
    return false;
  }
  return false;
}

void Amsx915ReadData(void) {
  // i2c frontend does not provide any commands/register to detect this sensor type -> request 4 bytes and check for 4 byte response is mandatory
  uint8_t buffer[4];
  Wire.requestFrom(AMSX915_ADDR, 4);
  if(Wire.available() != 4) {
    amsx915data.present = false;
    return;
  }
  buffer[0] = Wire.read();
  buffer[1] = Wire.read();
  buffer[2] = Wire.read();
  buffer[3] = Wire.read();

  amsx915data.pressure = ((256*(buffer[0]&0x3F)+buffer[1])-1638.0)*(amsx915Settings.pmax-amsx915Settings.pmin)/13107+amsx915Settings.pmin;
  amsx915data.temperature = (((256.0*buffer[2]+buffer[3])*200.0)/65536)-50;
  if(!amsx915data.present) {
    I2cSetActiveFound(AMSX915_ADDR, "AMSx915");
    amsx915data.present = true;
  }
}

void Amsx915Show(bool json) {
  if(amsx915data.present) {
    if (json) {
      ResponseAppend_P(PSTR(",\"AMSx915\":{\"" D_JSON_TEMPERATURE "\":%1_f,\"" D_JSON_PRESSURE "\":%2_f}"), &amsx915data.temperature, &amsx915data.pressure);
#ifdef USE_WEBSERVER
      } else {
        char str_pressure[9];
        dtostrfd(amsx915data.pressure, 2, str_pressure);
        WSContentSend_PD(HTTP_SNS_PRESSURE, "AMSx915", str_pressure, PressureUnit().c_str());
        WSContentSend_Temp("AMSx915", amsx915data.temperature);
#endif  // USE_WEBSERVER
      }
  }
}

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

void Amsx915SettingsLoad(bool erase) {
  // Called from FUNC_PRE_INIT (erase = 0) once at restart
  memset(&amsx915Settings, 0x00, sizeof(amsx915Settings));
  amsx915Settings.version = AMSX915_VERSION;
  amsx915Settings.pmax = PMAX_DEFAULT;
  amsx915Settings.pmin = PMIN_DEFAULT;

  // *** End Init default values ***

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_INFO, PSTR("CFG: AMS defaults as file system not enabled"));
#else
  // Try to load file /.drvset122
  char filename[20];
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_SENSOR), XSNS_114);

  if (erase) {
    TfsDeleteFile(filename);  // Use defaults
  }
  else if (TfsLoadFile(filename, (uint8_t*)&amsx915Settings, sizeof(amsx915Settings))) {
    if (amsx915Settings.version != AMSX915_VERSION) {      // Fix version dependent changes

      // *** Start fix possible setting deltas ***
      if (Settings->version < 0x01010100) {
        AddLog(LOG_LEVEL_INFO, PSTR("CFG: Update oldest version restore"));

      }
      if (Settings->version < 0x01010101) {
        AddLog(LOG_LEVEL_INFO, PSTR("CFG: Update old version restore"));

      }

      // *** End setting deltas ***

      // Set current version and save settings
      amsx915Settings.version = AMSX915_VERSION;
      Amsx915SettingsSave();
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: AMS config loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: AMS use defaults as file system not ready or file not found"));
  }
#endif  // USE_UFILESYS
}

void Amsx915SettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
#ifdef USE_UFILESYS
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&amsx915Settings +4, sizeof(amsx915Settings) -4);  // Skip crc32
  if (crc32 != amsx915Settings.crc32) {
    // Try to save file /.drvset122
    amsx915Settings.crc32 = crc32;

    char filename[20];
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_SENSOR), XSNS_114);

    if (TfsSaveFile(filename, (const uint8_t*)&amsx915Settings, sizeof(amsx915Settings))) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: AMS Settings saved to file"));
    } else {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: ERROR AMS file system not ready or unable to save file"));
    }
  }
#endif  // USE_UFILESYS
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns114(uint32_t function) {
  if (!I2cEnabled(XI2C_86)) { return false; }

  bool result = false;

  switch(function) {
    case FUNC_PRE_INIT:
      Amsx915SettingsLoad(0);
      break;
    case FUNC_INIT:
      Amsx915ReadData();
      break;
    case FUNC_EVERY_SECOND:
      Amsx915ReadData();
      break;
    case FUNC_SAVE_SETTINGS:
      Amsx915SettingsSave();
      break;
    case FUNC_COMMAND_SENSOR:
      if(XSNS_114 == XdrvMailbox.index) {
        result = Amsx915Command();
      }
      break;
    case FUNC_JSON_APPEND:
      Amsx915Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Amsx915Show(0);
      break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_AMSX915
#endif  // USE_I2C
