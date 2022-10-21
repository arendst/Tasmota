/*
  xsns_70_veml6075.ino - VEML6075 UVA/UVB/UVINDEX Sensor support for Tasmota

  Copyright (C) 2021  Martin Wagner

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
#ifdef USE_VEML6075
/*********************************************************************************************\
 * VEML6075 UVA/UVB/UVINDEX Sensor
 *
 * I2C Address: 0x10
\*********************************************************************************************/

#define XSNS_70             70
#define XI2C_49             49  // See I2CDEVICES.md


#define VEML6075_ADDR         0x10 // I2C address
#define VEML6075_CHIP_ID      0x26 // Manufacture ID

// I2C register
#define VEML6075_REG_CONF     0x00 // Configuration register
#define VEML6075_REG_UVA      0x07 // UVA band raw measurement
#define VEML6075_REG_DARK     0x08 // Dark current (?) measurement
#define VEML6075_REG_UVB      0x09 // UVB band raw measurement
#define VEML6075_REG_UVCOMP1  0x0A // UV1 compensation value
#define VEML6075_REG_UVCOMP2  0x0B // UV2 compensation value
#define VEML6075_REG_ID       0x0C // ID Register

// global constants for Calc
#define VEML6075_DEFAULT_UVA_A_COEFF 2.22     // Default for no coverglass
#define VEML6075_DEFAULT_UVA_B_COEFF 1.33     // Default for no coverglass
#define VEML6075_DEFAULT_UVB_C_COEFF 2.95     // Default for no coverglass
#define VEML6075_DEFAULT_UVB_D_COEFF 1.74     // Default for no coverglass
#define UVA_RESPONSIVITY_100MS_UNCOVERED 0.001461  // Default for no coverglass
#define UVB_RESPONSIVITY_100MS_UNCOVERED 0.002591   // Default for no coverglass

const float UVA_RESPONSIVITY[] PROGMEM =
{
    UVA_RESPONSIVITY_100MS_UNCOVERED / 0.5016286645, // 50ms
    UVA_RESPONSIVITY_100MS_UNCOVERED,                // 100ms
    UVA_RESPONSIVITY_100MS_UNCOVERED / 2.039087948,  // 200ms
    UVA_RESPONSIVITY_100MS_UNCOVERED / 3.781758958,  // 400ms
    UVA_RESPONSIVITY_100MS_UNCOVERED / 7.371335505   // 800ms
};

const float UVB_RESPONSIVITY[] PROGMEM =
{
    UVB_RESPONSIVITY_100MS_UNCOVERED / 0.5016286645, // 50ms
    UVB_RESPONSIVITY_100MS_UNCOVERED,                // 100ms
    UVB_RESPONSIVITY_100MS_UNCOVERED / 2.039087948,  // 200ms
    UVB_RESPONSIVITY_100MS_UNCOVERED / 3.781758958,  // 400ms
    UVB_RESPONSIVITY_100MS_UNCOVERED / 7.371335505   // 800ms
};

// http and json defines
#define D_NAME_VEML6075 "VEML6075"
#define D_UVA_INTENSITY "UVA intensity"
#define D_UVB_INTENSITY "UVB intensity"

const char HTTP_SNS_UVA[] PROGMEM = "{s}%s " D_UVA_INTENSITY "{m}%d " D_UNIT_WATT_METER_QUADRAT "{e}";
const char HTTP_SNS_UVB[] PROGMEM = "{s}%s " D_UVB_INTENSITY "{m}%d " D_UNIT_WATT_METER_QUADRAT "{e}";
const char HTTP_SNS_UVINDEX[] PROGMEM = "{s}%s " D_UV_INDEX "{m}%s {e}";
const char JSON_SNS_VEML6075[] PROGMEM = ",\"%s\":{\"" D_JSON_UVA_INTENSITY "\":%d,\"" D_JSON_UVB_INTENSITY "\":%d,\"" D_JSON_UV_INDEX "\":%s}";
const char S_JSON_VEML6075_COMMAND_NVALUE[] PROGMEM = "{\"" D_NAME_VEML6075 "\":{\"%s\":%d}}";

const char kVEML6075_Commands[] PROGMEM  = D_CMND_VEML6075_POWER "|" D_CMND_VEML6075_DYNAMIC "|" D_CMND_VEML6075_INTTIME;

enum VEML6075_Commands {         // commands for Console
  CMND_VEML6075_PWR,
  CMND_VEML6075_SET_HD,
  CMND_VEML6075_SET_UVIT,
  };

// global variables
struct VEML6075STRUCT
{
  char types[9]   = D_NAME_VEML6075;
  uint8_t address = VEML6075_ADDR;
  uint8_t inttime = 0;
  uint16_t uva = 0;
  uint16_t uvb = 0;
  uint16_t uva_raw = 0;
  uint16_t uvb_raw = 0;
  uint16_t comp1 = 0;
  uint16_t comp2 = 0;
  uint16_t conf = 0;
  float uvi = 0.0f;
} veml6075_sensor;

uint8_t veml6075_active = 0;

// typedef of config register
typedef union {
  struct {
    uint8_t pwr:1;            // Shut Down
    uint8_t forded_auto:1;    // Auto or forced
    uint8_t forced_trigger:1; // Trigger forced mode
    uint8_t hd:1;             // High dynamic
    uint8_t inttime:3;        // Integration Time
    uint8_t spare7:1;         // spare
  };
  uint16_t config;
} veml6075configRegister;

veml6075configRegister veml6075Config;

/********************************************************************************************/

uint16_t VEML6075read16 (uint8_t reg) {
  uint16_t swap = I2cRead16(VEML6075_ADDR, reg);
  uint16_t ret = ((swap & 0xFF) << 8) | (swap >> 8);
  return ret;
}

void VEML6075write16 (uint8_t reg, uint16_t val) {
  uint16_t swap = ((val & 0xFF) << 8) | (val >> 8);
  I2cWrite16(VEML6075_ADDR, reg, swap);
}

float VEML6075calcUVA (void) {
  float uva_calc = veml6075_sensor.uva_raw - (VEML6075_DEFAULT_UVA_A_COEFF * veml6075_sensor.comp1) - (VEML6075_DEFAULT_UVA_B_COEFF * veml6075_sensor.comp2);
  return uva_calc;
}

float VEML6075calcUVB (void) {
  float uvb_calc = veml6075_sensor.uvb_raw - (VEML6075_DEFAULT_UVB_C_COEFF * veml6075_sensor.comp1) - (VEML6075_DEFAULT_UVB_D_COEFF * veml6075_sensor.comp2);
  return uvb_calc;
}

float VEML6075calcUVI (void) {
  float uvi_calc = ((veml6075_sensor.uva * UVA_RESPONSIVITY[veml6075_sensor.inttime]) + (veml6075_sensor.uvb * UVB_RESPONSIVITY[veml6075_sensor.inttime])) / 2;
  return uvi_calc;
}

void VEML6075SetHD(uint8_t val){
  veml6075Config.hd = val;
  VEML6075write16 (VEML6075_REG_CONF, veml6075Config.config);
}

uint8_t VEML6075ReadHD(void){
  veml6075Config.config  = VEML6075read16 (VEML6075_REG_CONF);
  return veml6075Config.hd;
}

void VEML6075SetUvIt(uint8_t val){
  veml6075Config.inttime = val;
  VEML6075Pwr(1);
  VEML6075write16 (VEML6075_REG_CONF, veml6075Config.config);
  VEML6075Pwr(0);
}

uint8_t VEML6075GetUvIt(void){
  veml6075Config.config = VEML6075read16 (VEML6075_REG_CONF);
  return veml6075Config.inttime;
}

void VEML6075Pwr(uint8_t val){
  veml6075Config.pwr = val;
  VEML6075write16 (VEML6075_REG_CONF, veml6075Config.config);
}

uint8_t VEML6075GetPwr(void){
  veml6075Config.config = VEML6075read16 (VEML6075_REG_CONF);
  return veml6075Config.pwr;
}

void VEML6075ReadData(void)
{
  veml6075_sensor.uva_raw = VEML6075read16 (VEML6075_REG_UVA);
  veml6075_sensor.uvb_raw = VEML6075read16 (VEML6075_REG_UVB);
  veml6075_sensor.comp1 = VEML6075read16 (VEML6075_REG_UVCOMP1);
  veml6075_sensor.comp2 = VEML6075read16 (VEML6075_REG_UVCOMP2);
  veml6075_sensor.inttime = VEML6075GetUvIt();
  veml6075_sensor.uva = VEML6075calcUVA();
  veml6075_sensor.uvb = VEML6075calcUVB();
  veml6075_sensor.uvi = VEML6075calcUVI();
}

bool VEML6075init(void)
{
  uint8_t id = VEML6075read16 (VEML6075_REG_ID);
  if(id == VEML6075_CHIP_ID) // Sensor id
    return true;
  return false;
}

void VEML6075Detect(void) {
  if (!I2cSetDevice(veml6075_sensor.address)) return;

  if (VEML6075init()) {
    I2cSetActiveFound(veml6075_sensor.address, veml6075_sensor.types);
    VEML6075write16 (VEML6075_REG_CONF, 0x10); // set default
    veml6075_active = 1;
  }
}

void VEML6075EverySecond(void) {
  VEML6075ReadData();
}

bool VEML6075Cmd(void) {
  char command[CMDSZ];
  uint8_t name_len = strlen(D_NAME_VEML6075);
  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_NAME_VEML6075), name_len)) {
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + name_len, kVEML6075_Commands);
    switch (command_code) {
      case CMND_VEML6075_PWR:
        if (XdrvMailbox.data_len) {
          if (2 >= XdrvMailbox.payload) {
            VEML6075Pwr(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_VEML6075_COMMAND_NVALUE, command, VEML6075GetPwr());
       break;
      case CMND_VEML6075_SET_HD:
        if (XdrvMailbox.data_len) {
          if (2 >= XdrvMailbox.payload) {
            VEML6075SetHD(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_VEML6075_COMMAND_NVALUE, command, VEML6075ReadHD());
        break;
       case CMND_VEML6075_SET_UVIT:
        if (XdrvMailbox.data_len) {
          if (4 >= XdrvMailbox.payload) {
            VEML6075SetUvIt(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_VEML6075_COMMAND_NVALUE, command, VEML6075GetUvIt());
        break;
    default:
      return false;
    }
    return true;
  } else {
    return false;
  }
}

void VEML6075Show(bool json)
{
  char s_uvindex[FLOATSZ];
  dtostrfd(veml6075_sensor.uvi,1, s_uvindex);

  if (json) {
    ResponseAppend_P(JSON_SNS_VEML6075, D_NAME_VEML6075, veml6075_sensor.uva, veml6075_sensor.uvb, s_uvindex);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_UVA, D_NAME_VEML6075, veml6075_sensor.uva);
    WSContentSend_PD(HTTP_SNS_UVB, D_NAME_VEML6075, veml6075_sensor.uvb);
    WSContentSend_PD(HTTP_SNS_UVINDEX, D_NAME_VEML6075 ,s_uvindex);
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns70(uint8_t function)
{
  if (!I2cEnabled(XI2C_49)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    VEML6075Detect();
  }
  else if (veml6075_active) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        VEML6075EverySecond();
        break;
      case FUNC_COMMAND:
        result = VEML6075Cmd();
        break;
      case FUNC_JSON_APPEND:
        VEML6075Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        VEML6075Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VEML6075
#endif  // USE_I2C
