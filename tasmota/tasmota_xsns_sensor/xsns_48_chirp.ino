/*
  xsns_48_chirp.ino - soil moisture sensor support for Tasmota

  Copyright (C) 2021  Theo Arends & Christian Baars

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

  --------------------------------------------------------------------------------------------
  Version Date      Action    Description
  --------------------------------------------------------------------------------------------

  1.0.0.2 20200611  changed   - bugfix: decouple restart of the work loop from FUNC_JSON_APPEND callback
  ---
  1.0.0.1 20190917  changed   - rework of the inner loop to enable delays in the middle of I2C-reads
                    changed   - double send address change only for fw>0x25
                    changed   - use DEBUG_SENSOR_LOG, change ILLUMINANCE to DARKNESS
                    changed   - do not publish missing temperature reads, show fw-version as hex
                    added     - now really support the (slower) CHIRP!-Sensor
  ---
  1.0.0.0 20190608  started   - further development by Christian Baars  - https://github.com/Staars/Sonoff-Tasmota
                    forked    - from arendst/tasmota                    - https://github.com/arendst/Tasmota
                    base      - code base from arendst and              - https://github.com/Miceuz/i2c-moisture-sensor

*/

#ifdef USE_I2C
#ifdef USE_CHIRP

/*********************************************************************************************\
 * CHIRP - Chirp!-sensor and I2C-soil-moisture-sensor
 * !! The I2C-soil-moisture-sensor is the preferred one !!
 *
 * I2C Address: 0x20 - standard address, is changeable
 * - Uses I2C clock stretching
 * - Scans all I2C addresses
\*********************************************************************************************/

#define XSNS_48                       48
#define XI2C_33                       33  // See I2CDEVICES.md

#define CHIRP_MAX_SENSOR_COUNT        3            // 127 is expectectd to be the max number

#define CHIRP_ADDR_STANDARD           0x20         // standard address

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_CHIRP "CHIRP"

const char S_JSON_CHIRP_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_CHIRP "%s\":%d}";
const char S_JSON_CHIRP_COMMAND[] PROGMEM        = "{\"" D_CMND_CHIRP "%s\"}";
const char kCHIRP_Commands[] PROGMEM             = "Select|Set|Scan|Reset|Sleep|Wake";

const char kChirpTypes[] PROGMEM = "CHIRP";

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum CHIRP_Commands {                                 // commands useable in console or rules
  CMND_CHIRP_SELECT,                                  // select active sensor by I2C address, makes only sense for multiple sensors
  CMND_CHIRP_SET,                                     // set new I2C address for selected/active sensor, will reset
  CMND_CHIRP_SCAN,                                    // scan the I2C bus for one or more chirp sensors
  CMND_CHIRP_RESET,                                   // CHIRPReset, a fresh and default restart
  CMND_CHIRP_SLEEP,                                   // put sensor to sleep
  CMND_CHIRP_WAKE };                                  // wake sensor by reading firmware version


/*********************************************************************************************\
 * command defines
\*********************************************************************************************/

#define CHIRP_GET_CAPACITANCE       0x00          // 16 bit, read
#define CHIRP_SET_ADDRESS           0x01          // 8 bit,  write
#define CHIRP_GET_ADDRESS           0x02          // 8 bit,  read
#define CHIRP_MEASURE_LIGHT         0x03          // no value, write, -> initiate measurement, then wait at least 3 seconds
#define CHIRP_GET_LIGHT             0x04          // 16 bit, read, -> higher value means darker environment, noisy data, not calibrated
#define CHIRP_GET_TEMPERATURE       0x05          // 16 bit, read
#define CHIRP_RESET                 0x06          // no value, write
#define CHIRP_GET_VERSION           0x07          // 8 bit, read, -> 0x22 means 2.2
#define CHIRP_SLEEP                 0x08          // no value, write
#define CHIRP_GET_BUSY              0x09          // 8 bit, read, -> 1 = busy, 0 = otherwise

/*********************************************************************************************\
 * helper function
\*********************************************************************************************/

void ChirpWriteI2CRegister(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
} // now the original CHIRP needs 1100 ms delay

uint16_t ChirpFinishReadI2CRegister16bit(uint8_t addr) {
  Wire.requestFrom(addr,(uint8_t)2);
  uint16_t t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

/********************************************************************************************/

// globals

uint8_t    chirp_current        = 0;    // current selected/active sensor
uint8_t    chirp_found_sensors  = 0;    // number of found sensors

char       chirp_name[7];
uint8_t    chirp_next_job       = 0;    //0=reset, 1=auto-wake, 2-13 = various measure steps; 14 = TELE done
uint32_t   chirp_timeout_count  = 0;    //is handled every second, so value is equal to seconds (it is a slow sensor)

#pragma pack(1)
struct ChirpSensor_t{
    uint16_t   moisture = 0;      // shall hold post-processed data, if implemented
    uint16_t   light = 0;         // light level, maybe already postprocessed depending on the firmware
    int16_t    temperature = 0;   // temperature in degrees CELSIUS * 10 , we will also store the I2C error code
    uint8_t    version = 0;       // firmware-version
    uint8_t    address:7;         // we need only 7bit so...
    uint8_t    explicitSleep:1;   // there is a free bit to play with ;)
};
#pragma pack()

ChirpSensor_t chirp_sensor[CHIRP_MAX_SENSOR_COUNT];       // should be 8 bytes per sensor slot

/********************************************************************************************/

void ChirpReset(uint8_t addr) {
    ChirpWriteI2CRegister(addr, CHIRP_RESET);
}

/********************************************************************************************/

void ChirpResetAll(void) {
    for (uint32_t i = 0; i < chirp_found_sensors; i++) {
      if (chirp_sensor[i].version) {
        ChirpReset(chirp_sensor[i].address);
        }
    }
}
/********************************************************************************************/

void ChirpClockSet() { // set I2C for this slow sensor
#ifdef ESP8266
    Wire.setClockStretchLimit(4000);
#endif  // ESP8266
    Wire.setClock(50000);
}

/********************************************************************************************/

void ChirpSleep(uint8_t addr) {
    ChirpWriteI2CRegister(addr, CHIRP_SLEEP);
}

/********************************************************************************************/

// void ChirpSleepAll(void) {
//     for (uint32_t i = 0; i < chirp_found_sensors; i++) {
//       if (chirp_sensor[i].version) {
//         ChirpSleep(chirp_sensor[i].address);
//         }
//     }
// }

// /********************************************************************************************/

// void ChirpAutoWakeAll(void) {
//     for (uint32_t i = 0; i < chirp_found_sensors; i++) {
//       if (chirp_sensor[i].version && !chirp_sensor[i].explicitSleep) {
//         ChirpReadVersion(chirp_sensor[i].address);
//         }
//     }
// }

/********************************************************************************************/

void ChirpSelect(uint8_t sensor) {
  if(sensor < chirp_found_sensors) { //TODO: show some infos
    chirp_current = sensor;
    DEBUG_SENSOR_LOG(PSTR("CHIRP: Sensor %u now active."), chirp_current);
  }
  if (sensor == 255) {
    DEBUG_SENSOR_LOG(PSTR("CHIRP: Sensor %u active at address 0x%x."), chirp_current, chirp_sensor[chirp_current].address);
  }
}

/******************************************************************************************************************/

uint8_t ChirpReadVersion(uint8_t addr) {
  return (I2cRead8(addr, CHIRP_GET_VERSION)); // the Chirp!-sensor does not provide fw-version and we will get 255
}

/******************************************************************************************************************/

bool ChirpSet(uint8_t addr) {
  if(addr < 128){
    if (I2cWrite8(chirp_sensor[chirp_current].address, CHIRP_SET_ADDRESS, addr)){
      if(chirp_sensor[chirp_current].version>0x25 && chirp_sensor[chirp_current].version != 255){
        delay(5);
        I2cWrite8(chirp_sensor[chirp_current].address, CHIRP_SET_ADDRESS, addr);
        // two calls are needed for sensor firmware version 2.6, but maybe dangerous before
      }
      DEBUG_SENSOR_LOG(PSTR("CHIRP: Wrote adress %u "), addr);
      ChirpReset(chirp_sensor[chirp_current].address);
      chirp_sensor[chirp_current].address = addr;
      chirp_timeout_count = 10;
      chirp_next_job = 0;
      if(chirp_sensor[chirp_current].version == 255){ // this should be Chirp! and it seems to need a power cycle (or RESET to GND)
          AddLog(LOG_LEVEL_INFO, PSTR("CHIRP: wrote new address %u, please power off device"), addr);
          chirp_sensor[chirp_current].version == 0; // make it "invisible"
      }
      return true;
    }
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CHIRP: address %u incorrect and not used"), addr);
  return false;
}

/********************************************************************************************/

bool ChirpScan()
{
  ChirpClockSet();
  chirp_found_sensors = 0;
  for (uint8_t address = 1; address <= 127; address++) {
    chirp_sensor[chirp_found_sensors].version = 0;
    chirp_sensor[chirp_found_sensors].version = ChirpReadVersion(address);
    delay(2);
    chirp_sensor[chirp_found_sensors].version = ChirpReadVersion(address);
    if (chirp_sensor[chirp_found_sensors].version > 0) {
      I2cSetActiveFound(address, "CHIRP");
      if (chirp_found_sensors<CHIRP_MAX_SENSOR_COUNT) {
        chirp_sensor[chirp_found_sensors].address = address; // push next sensor, as long as there is space in the array
        AddLog(LOG_LEVEL_DEBUG, PSTR("CHIRP: fw %x"), chirp_sensor[chirp_found_sensors].version);
      }
      chirp_found_sensors++;
    }
  }
  // chirp_timeout_count = 11; // wait a second to read the real fw-version in the next step
  AddLog(LOG_LEVEL_DEBUG, PSTR("Found %u CHIRP sensor(s)."), chirp_found_sensors);
  return (chirp_found_sensors > 0);
}

/********************************************************************************************/

void ChirpDetect(void)
{
  if (chirp_next_job > 0) { return; }

  DEBUG_SENSOR_LOG(PSTR("CHIRP: scan will start ..."));
  if (ChirpScan()) {
    uint8_t chirp_model = 0;  // TODO: ??
    GetTextIndexed(chirp_name, sizeof(chirp_name), chirp_model, kChirpTypes);
  }
}
/********************************************************************************************/

void ChirpServiceAllSensors(uint8_t job){
  for (uint32_t i = 0; i < chirp_found_sensors; i++) {
    if (chirp_sensor[i].version && !chirp_sensor[i].explicitSleep) {
      DEBUG_SENSOR_LOG(PSTR("CHIRP: prepare for sensor at address 0x%x"), chirp_sensor[i].address);
      switch(job){
        case 0:
        ChirpWriteI2CRegister(chirp_sensor[i].address, CHIRP_GET_CAPACITANCE);
        break;
        case 1:
        chirp_sensor[i].moisture = ChirpFinishReadI2CRegister16bit(chirp_sensor[i].address);
        break;
        case 2:
        ChirpWriteI2CRegister(chirp_sensor[i].address, CHIRP_GET_TEMPERATURE);
        break;
        case 3:
        chirp_sensor[i].temperature = ChirpFinishReadI2CRegister16bit(chirp_sensor[i].address);
        break;
        case 4:
        ChirpWriteI2CRegister(chirp_sensor[i].address, CHIRP_MEASURE_LIGHT);
        break;
        case 5:
        ChirpWriteI2CRegister(chirp_sensor[i].address, CHIRP_GET_LIGHT);
        break;
        case 6:
        chirp_sensor[i].light = ChirpFinishReadI2CRegister16bit(chirp_sensor[i].address);
        break;
        default:
        break;
      }
    }
  }
}

/********************************************************************************************/

void ChirpEvery100MSecond(void)
{
  // DEBUG_SENSOR_LOG(PSTR("CHIRP: every 100 mseconds, counter: %u, next job: %u"),chirp_timeout_count,chirp_next_job);
  if(chirp_timeout_count == 0) {    //countdown complete, now do something
      switch(chirp_next_job) {
          case 0:                   //this should only be called after driver initialization
          DEBUG_SENSOR_LOG(PSTR("CHIRP: reset all"));
          ChirpResetAll();
          chirp_timeout_count = 10; // wait a second
          chirp_next_job++;
          break;
          case 1:                   // auto-sleep-wake seems to expose a fundamental I2C-problem of the sensor and is deactivated
          // DEBUG_SENSOR_LOG(PSTR("CHIRP: auto-wake all"));
          // ChirpAutoWakeAll();       // this is only a wake-up call at the start of next read cycle
          chirp_next_job++;         // go on, next job should start in a second
          break;
          case 2:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: prepare moisture read"));
          ChirpServiceAllSensors(0);
          chirp_timeout_count = 11;  // wait 1.1 seconds,
          chirp_next_job++;
          break;
          case 3:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: finish moisture read"));
          ChirpServiceAllSensors(1);
          chirp_next_job++;
          break;
          case 4:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: prepare moisture read - 2nd"));
          ChirpServiceAllSensors(0);
          chirp_timeout_count = 11;  // wait 1.1 seconds,
          chirp_next_job++;
          break;
          case 5:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: finish moisture read - 2nd"));
          ChirpServiceAllSensors(1);
          chirp_next_job++;
          break;
          case 6:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: prepare temperature read"));
          ChirpServiceAllSensors(2);
          chirp_timeout_count = 11;  // wait 1.1 seconds,
          chirp_next_job++;
          break;
          case 7:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: finish temperature read"));
          ChirpServiceAllSensors(3);
          chirp_next_job++;
          break;
          case 8:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: prepare temperature read - 2nd"));
          ChirpServiceAllSensors(2);
          chirp_timeout_count = 11;  // wait 1.1 seconds,
          chirp_next_job++;
          break;
          case 9:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: finish temperature read - 2nd"));
          ChirpServiceAllSensors(3);
          chirp_next_job++;
          break;
          case 10:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: start light measure process"));
          ChirpServiceAllSensors(4);
          chirp_timeout_count = 90;  // wait 9 seconds,
          chirp_next_job++;
          break;
          case 11:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: prepare light read"));
          ChirpServiceAllSensors(5);
          chirp_timeout_count = 11;  // wait 1.1 seconds,
          chirp_next_job++;
          break;
          case 12:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: finish light read"));
          ChirpServiceAllSensors(6);
          chirp_next_job++;
          break;
          case 13:
          DEBUG_SENSOR_LOG(PSTR("CHIRP: paused, waiting for TELE"));
          chirp_next_job++;
          break;
          case 14:
          if (Settings->tele_period > 16){
              chirp_timeout_count = (Settings->tele_period - 16) * 10;  // sync it with the TELEPERIOD, we need about up to 16 seconds to measure
              DEBUG_SENSOR_LOG(PSTR("CHIRP: timeout 1/10 sec: %u, tele: %u"), chirp_timeout_count, Settings->tele_period);
            }
          else{
            AddLog(LOG_LEVEL_INFO, PSTR("CHIRP: TELEPERIOD must be > 16 seconds !"));
            // we could overwrite it to i.e. 20 seconds here
          }
          chirp_next_job = 1;                                 // back to step 1
          break;
      }
  }
  else {
      chirp_timeout_count--;         // count down
  }
}

/********************************************************************************************/

// normaly in i18n.h
#ifdef USE_WEBSERVER
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

const char HTTP_SNS_DARKNESS[] PROGMEM = "{s} " D_JSON_DARKNESS "{m}%s %%{e}";
const char HTTP_SNS_CHIRPVER[] PROGMEM = "{s} CHIRP-sensor %u at address{m}0x%x{e}"
                                         "{s} FW-version{m}%s {e}";                                                                                            ;
const char HTTP_SNS_CHIRPSLEEP[] PROGMEM = "{s} {m} is sleeping ...{e}";
#endif  // USE_WEBSERVER

/********************************************************************************************/

void ChirpShow(bool json)
{
  for (uint32_t i = 0; i < chirp_found_sensors; i++) {
    if (chirp_sensor[i].version) {
      float t_temperature = ConvertTemp(((float)chirp_sensor[i].temperature )/10.0);

      char str_light[33];
      dtostrfd(chirp_sensor[i].light, 0, str_light);
      char str_version[7];
      if(chirp_sensor[i].version == 0xff){
        strncpy_P(str_version, PSTR("Chirp!"), sizeof(str_version));
      }
      else{
        sprintf(str_version, "%x", chirp_sensor[i].version);
      }

      if (json) {
        if(!chirp_sensor[i].explicitSleep) {
          ResponseAppend_P(PSTR(",\"%s%u\":{\"" D_JSON_MOISTURE "\":%d"), chirp_name, i, chirp_sensor[i].moisture);
          if(chirp_sensor[i].temperature!=-1){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%*_f"), Settings->flag2.temperature_resolution, &t_temperature);
          }
          ResponseAppend_P(PSTR(",\"" D_JSON_DARKNESS "\":%s}"),str_light);
        }
        else {
          ResponseAppend_P(PSTR(",\"%s%u\":{\"sleeping\"}"),chirp_name, i);
        }
  #ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzTempHumPressureSensor(t_temperature, chirp_sensor[i].moisture);
        DomoticzSensor(DZ_ILLUMINANCE,chirp_sensor[i].light); // this is not LUX!!
      }
  #endif  // USE_DOMOTICZ
  #ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_CHIRPVER, i, chirp_sensor[i].address, str_version);
        if (chirp_sensor[i].explicitSleep){
          WSContentSend_PD(HTTP_SNS_CHIRPSLEEP);
        }
        else {
          WSContentSend_PD(HTTP_SNS_MOISTURE, "", chirp_sensor[i].moisture);
          WSContentSend_PD(HTTP_SNS_DARKNESS, str_light);
          if (chirp_sensor[i].temperature!=-1) { // this is the error code -> no temperature
            WSContentSend_Temp("", t_temperature);
          }
        }

  #endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * check the Chirp commands
\*********************************************************************************************/

bool ChirpCmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_CHIRP);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_CHIRP), disp_len)) {  // prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kCHIRP_Commands);

    switch (command_code) {
      case CMND_CHIRP_SELECT:
      case CMND_CHIRP_SET:
        if (XdrvMailbox.data_len > 0) {
          if (command_code == CMND_CHIRP_SELECT)  { ChirpSelect(XdrvMailbox.payload); }                       //select active sensor, i.e. for wake, sleep or reset
          if (command_code == CMND_CHIRP_SET)     { ChirpSet((uint8_t)XdrvMailbox.payload); }                 //set and change I2C-address of selected sensor
        Response_P(S_JSON_CHIRP_COMMAND_NVALUE, command, XdrvMailbox.payload);
        }
        else {
          if (command_code == CMND_CHIRP_SELECT)  { ChirpSelect(255); }                                       //show active sensor
        Response_P(S_JSON_CHIRP_COMMAND, command, XdrvMailbox.payload);
        }
        break;
      case CMND_CHIRP_SCAN:
      case CMND_CHIRP_SLEEP:
      case CMND_CHIRP_WAKE:
      case CMND_CHIRP_RESET:
        if (command_code == CMND_CHIRP_SCAN)     {  chirp_next_job = 0;
                                                    ChirpDetect(); }                                            // this will re-init the sensor array
        if (command_code == CMND_CHIRP_SLEEP)    {  chirp_sensor[chirp_current].explicitSleep = true;         // we do not touch this sensor in the read functions
                                                    ChirpSleep(chirp_sensor[chirp_current].address); }
        if (command_code == CMND_CHIRP_WAKE)     {  chirp_sensor[chirp_current].explicitSleep = false;        // back in action
                                                    ChirpReadVersion(chirp_sensor[chirp_current].address); }  // just use read version as wakeup call
        if (command_code == CMND_CHIRP_RESET)    { ChirpReset(chirp_sensor[chirp_current].address); }
        Response_P(S_JSON_CHIRP_COMMAND, command, XdrvMailbox.payload);
        break;
      default:
        // else for Unknown command
        serviced = false;
      break;
    }
  } else {
    serviced = false;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns48(uint32_t function)
{
  if (!I2cEnabled(XI2C_33)) { return false; }

  bool result = false;

  switch (function) {
    case FUNC_EVERY_100_MSECOND:
      if(chirp_found_sensors > 0){
        ChirpEvery100MSecond();
      }
      break;
    case FUNC_COMMAND:
      result = ChirpCmd();
      break;
    case FUNC_JSON_APPEND:
      ChirpShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      ChirpShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_INIT:
      ChirpDetect();         // We can call CHIRPSCAN later to re-detect
      break;
  }
  return result;
}

#endif  // USE_CHIRP
#endif  // USE_I2C
