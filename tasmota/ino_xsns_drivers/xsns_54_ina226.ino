/*
  xsns_54_ina226.ino - INA226 Current Sensor support for Tasmota

  Copyright (C) 2021  Stephen Rodgers and Theo Arends

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
#ifdef USE_INA226
/*
* Setup a single INA226 device at address 0x40:
*
* 1. Select a module type with free I2C pins.
* 2. Configure the module to use I2C on the correct pins.
* 3. Connect your ina226 module(s) to the I2C pins.
* 4. Use the i2cscan console command to probe the modules and check they are present.
* 5. Enable the first device at I2C address 0x40 using the following console commands:
*    a. Sensor54 11 [shunt resistance in ohms] e.g. Sensor54 11 0.1
*    b. Sensor54 12 [full scale current in amperes] e.g. Sensor54 12 3.0
*    c. Sensor54 2 saves the settings and restarts Tasmota. The device should show up after the system boots again.
*
*
* This driver will not probe I2C bus for INA226 devices unless the full scale current is set for a device number.
* It will map device numbers as follows:
*
* Device number to I2C address mapping
*
* 1 - 0x40
* 2 - 0x41
* 3 - 0x44
* 4 - 0x45
*
* To set shunt resistance and full scale current, use the Sensor54 command interface as follows:
*
* Sensor54 10                        Return channel 1 shunt resistance and full scale current
* Sensor54 11 [shunt_resistance]     Set INA226 channel 1 shunt resistance in ohms, floating point
* Sensor54 12 [full_scale_current]   Set INA226 channel 1 full scale current in amperes, floating point
* Sensor54 20                        Return channel 2 shunt resistance and full scale current
* Sensor54 21 [shunt_resistance]     Set INA226 channel 2 shunt resistance in ohms, floating point
* Sensor54 22 [full_scale_current]   Set INA226 channel 2 full scale current in amperes, floating point
* Sensor54 30                        Return channel 3 shunt resistance and full scale current
* Sensor54 31 [shunt_resistance]     Set INA226 channel 3 shunt resistance in ohms, floating point
* Sensor54 32 [full_scale_current]   Set INA226 channel 3 full scale current in amperes, floating point
* Sensor54 40                        Return channel 4 shunt resistance and full scale current
* Sensor54 41 [shunt_resistance]     Set INA226 channel 4 shunt resistance in ohms, floating point
* Sensor54 42 [full_scale_current]   Set INA226 channel 4 full scale current in amperes, floating point
*
* Other commands
*
* Sensor54 1 Rescan for devices and return the number of INA226 found.
* Sensor54 2 Save the configuration and restart
*
*
*/

// Define driver ID

#define XSNS_54                                 54
#define XI2C_35                                 35  // See I2CDEVICES.md

#define INA226_MAX_ADDRESSES                    4
#define INA226_ADDRESS1                         (0x40)    // 1000000 (A0+A1=GND)
#define INA226_ADDRESS2                         (0x41)    // 1000000 (A0=Vcc, A1=GND)
#define INA226_ADDRESS3                         (0x44)    // 1000000 (A0=GND, A1=Vcc)
#define INA226_ADDRESS4                         (0x45)    // 1000000 (A0+A1=Vcc)

#define INA226_REG_CONFIG                       (0x00)    // Config register
#define INA226_RES_CONFIG                       (0x4127)  // Config register at reset
#define INA226_DEF_CONFIG                       (0x42FF)  // Our default configuration
#define INA226_CONFIG_RESET                     (0x8000)  // Config register reset bit

#define INA226_REG_SHUNTVOLTAGE                 (0x01)
#define INA226_REG_BUSVOLTAGE                   (0x02)
#define INA226_REG_POWER                        (0x03)
#define INA226_REG_CURRENT                      (0x04)
#define INA226_REG_CALIBRATION                  (0x05)


typedef struct Ina226Info_tag {
  uint8_t address;
  uint16_t calibrationValue;
  uint16_t config;
  uint8_t present : 1;
  float i_lsb;
} Ina226Info_t;

/*
* Program memory constants
*/

static const uint8_t PROGMEM probeAddresses[INA226_MAX_ADDRESSES] = {INA226_ADDRESS1, INA226_ADDRESS2, INA226_ADDRESS3, INA226_ADDRESS4};

/*
* Global Variables
*/


static char Ina226Str[] = "INA226";
static uint8_t Ina226sFound = 0;
static uint8_t schedule_reinit = 0;
static Ina226Info_t Ina226Info[4] = {0};
//static uint16_t reinit_count[4];
static float voltages[4];
static float currents[4];
static float powers[4];


/*
* Log single floating point Number
*/

static void _debug_fval(const char *str, float fval, uint8_t prec = 4 )
{
  char fstr[32];
  dtostrfd(fval, prec, fstr);
  AddLog( LOG_LEVEL_DEBUG, PSTR("%s: %s"), str, fstr );
}


/*
* Convert 16 bit repesentation of shunt resisance to 32 bit micro ohms by looking at the msb range bit.
* If the msb is 1, the LSB's define the number of milli ohms. (Maximum shunt resistor value 32.767 ohms)
* If the msb is 0, the LSB's define the number of micro ohms. (Maximum shunt resistor value 0.032767 ohms)
*/

static uint32_t _expand_r_shunt(uint16_t compact_r_shunt)
{

  uint32_t r_shunt_uohms = (compact_r_shunt & 0x8000) ?
  (((uint32_t)(compact_r_shunt & 0x7FFF)) * 1000ul) :
  (compact_r_shunt & 0x7FFF);
  return r_shunt_uohms;
}

/*
* Set calibration value for Ina226
*/

void Ina226SetCalibration(uint8_t Ina226Index)
{

Ina226Info_t *si = Ina226Info + Ina226Index;

I2cWrite16( si->address, INA226_REG_CALIBRATION, si->calibrationValue);

}


/*
* Test for presence of an Ina226
*/

bool Ina226TestPresence(uint8_t device)
{

  // Read config

  uint16_t config  = I2cRead16( Ina226Info[device].address, INA226_REG_CONFIG );
  //AddLog( LOG_LEVEL_NONE, PSTR("Config register %04x" ), config);

  if (config != Ina226Info[device].config)
    return false;

  return true;

}

void Ina226ResetActive(void)
{
  Ina226Info_t *p = Ina226Info;

  for (uint32_t i = 0; i < INA226_MAX_ADDRESSES; i++) {
    p = &Ina226Info[i];
    // Address
    uint8_t addr = p->address;
    if (addr) {
      I2cResetActive(addr);
    }
  }
}

/*
* Initialize INA226 devices
*/

void Ina226Init()
{
  uint32_t i;

  Ina226sFound = 0;

  Ina226Info_t *p = Ina226Info;

  //AddLog( LOG_LEVEL_NONE, "Ina226Init");
//  AddLog( LOG_LEVEL_NONE, "Size of Settings: %d bytes", sizeof(TSettings));

//  if (!TasmotaGlobal.i2c_enabled)
//    AddLog(LOG_LEVEL_DEBUG, "INA226: Initialization failed: No I2C support");


  // Clear Ina226 info data

  for (i = 0; i < 4; i++){
    *p = {0};
  }

  //AddLog( LOG_LEVEL_NONE, PSTR("Sizeof Ina226Cfg: %d" ), sizeof(Ina226Cfg));

  // Detect devices

  for (i = 0; i < INA226_MAX_ADDRESSES; i++){
    uint8_t addr = pgm_read_byte(probeAddresses + i);

    if (!I2cSetDevice(addr)) { continue; }

    // Skip device probing if the full scale current is zero

    //AddLog( LOG_LEVEL_NONE, "fs_i[%d]: %d", i, Settings->ina226_i_fs[i]);
    if (!Settings->ina226_i_fs[i])
      continue;


    //AddLog( LOG_LEVEL_NONE, PSTR("INA226 trying address %02x" ), addr );

    // Try Resetting the device

    if (!I2cWrite16( addr, INA226_REG_CONFIG, INA226_CONFIG_RESET)){

      AddLog( LOG_LEVEL_DEBUG, "No INA226 at address: %02X", addr);
      continue; // No device
    }

    // Read config

    uint16_t config  = I2cRead16( addr, INA226_REG_CONFIG );
    //AddLog( LOG_LEVEL_NONE, PSTR("INA226 Config register %04x" ), config);

    if (INA226_RES_CONFIG != config)
      continue;


    config = INA226_DEF_CONFIG; // Fixme

    // Set the default configuration
    if (!I2cWrite16( addr, INA226_REG_CONFIG, config))
        continue; // No device

    // store data in info struct.
    p = &Ina226Info[i];
    // Address
    p->address = addr;
    // Configuration
    p->config = config;
    // Full scale current in tenths of an amp
    //AddLog( LOG_LEVEL_NONE, "Full Scale I in tenths of an amp: %u", Settings->ina226_i_fs[i]);
    p->i_lsb = (((float) Settings->ina226_i_fs[i])/10.0f)/32768.0f;
    //_debug_fval("i_lsb: %s", p->i_lsb, 7);

    // Get shunt resistor value in micro ohms
    uint32_t r_shunt_uohms = _expand_r_shunt(Settings->ina226_r_shunt[i]);
    //AddLog( LOG_LEVEL_NONE, "Shunt R in micro-ohms: %u", r_shunt_uohms);


    p->calibrationValue = ((uint16_t) (0.00512/(p->i_lsb * r_shunt_uohms/1000000.0f)));
    // Device present
    p->present = true;
    //AddLog( LOG_LEVEL_NONE, "INA226 Device %d calibration value: %04X", i, p->calibrationValue);

    Ina226SetCalibration(i);

    I2cSetActiveFound(addr, Ina226Str);

    Ina226sFound++;
  }
}

/*
* Read the bus voltage, and return it as a float
*/

float Ina226ReadBus_v(uint8_t device)
{
  uint8_t addr = Ina226Info[device].address;
  int16_t reg_bus_v = I2cReadS16( addr, INA226_REG_BUSVOLTAGE);

  float result = ((float) reg_bus_v) * 0.00125f;

  return result;

}

/*
* Read the shunt current, and return it as a float
*/

float Ina226ReadShunt_i(uint8_t device)
{
  uint8_t addr = Ina226Info[device].address;
  int16_t reg_shunt_i = I2cReadS16( addr, INA226_REG_CURRENT);

  float result = ((float) reg_shunt_i) * Ina226Info[device].i_lsb;

  return result;
}

/*
* Read the calculated power
*/

float Ina226ReadPower_w(uint8_t device)
{
  uint8_t addr = Ina226Info[device].address;
  int16_t reg_shunt_i = I2cReadS16( addr, INA226_REG_POWER);

  float result = ((float) reg_shunt_i) * (Ina226Info[device].i_lsb * 25.0);

  return result;
}


/*
* Read voltage, shunt voltage, current, and power registerd for a given device
*/

void Ina226Read(uint8_t device)
{
  //AddLog( LOG_LEVEL_NONE, "Ina226Read");
  voltages[device] = Ina226ReadBus_v(device);
  currents[device] = Ina226ReadShunt_i(device);
  powers[device] = Ina226ReadPower_w(device);
  //AddLog( LOG_LEVEL_NONE, "INA226 Device %d", device );
  //_debug_fval("Voltage", voltages[device]);
  //_debug_fval("Current", currents[device]);
  //_debug_fval("Power", powers[device]);
}

/*
* Poll sensors, and chack for sensor presence
*/

void Ina226EverySecond()
{
  //AddLog( LOG_LEVEL_NONE, "Ina226EverySecond");
  for (uint8_t device = 0; device < INA226_MAX_ADDRESSES; device++){
    // If there are Ina226s, and the device was present, and the device still is present, read its registers
    if (Ina226sFound && Ina226Info[device].present && Ina226TestPresence(device)){
      Ina226Read(device);
    }
    else {
        powers[device] = currents[device] = voltages[device] = 0.0f;
        // If device was present, note that it dropped off here
        //if(Ina226Info[device].present){
          //reinit_count[device]++;
          //AddLog( LOG_LEVEL_DEBUG, "INA226 Device %d dropped off, count: %d", device, reinit_count[device]);
        //}
        // Device no longer present
        Ina226Info[device].present = false;
    }
  }
}

/*
* Decode a sensor command and act on it
*/

bool Ina226CommandSensor()
{
  bool serviced = true;
  bool show_config = false;
  char param_str[64];
  char *cp, *params[4];
  uint8_t i, param_count, device, p1 = XdrvMailbox.payload;
  uint32_t r_shunt_uohms;
  uint16_t compact_r_shunt_uohms;
  //AddLog( LOG_LEVEL_NONE, "Command received: %d", XdrvMailbox.payload);
  //AddLog( LOG_LEVEL_NONE, "Command data received: %s", XdrvMailbox.data);

  // Make a copy of the data and add another terminator

  if (XdrvMailbox.data_len > 62){
    return false;
  }

  strncpy(param_str, XdrvMailbox.data, XdrvMailbox.data_len + 1);
  param_str[XdrvMailbox.data_len] = 0;

  // Build parameter substrings (this should really be a helper function in support_command.ino)
  for (cp = param_str, i = 0, param_count = 0; *cp && (i < XdrvMailbox.data_len + 1) && (param_count <= 3); i++)
    if (param_str[i] == ' ' || param_str[i] == ',' || param_str[i] == 0){
      param_str[i] = 0;
      params[param_count] = cp;
      //AddLog( LOG_LEVEL_NONE, "INA226 Command parameter: %d, value: %s", param_count, params[param_count]);
      param_count++;
      cp = param_str + i + 1;
    }


  if (p1 < 10 || p1 >= 50){
    // Device-less commands
    switch (p1){
      case 1: // Rerun init
        Ina226ResetActive();
        Ina226Init();
        Response_P(PSTR("{\"Sensor54-Command-Result\":{\"Ina226sFound\":%d}}"),Ina226sFound);
        break;

      case 2: // Save and restart
        TasmotaGlobal.restart_flag = 2;
        Response_P(PSTR("{\"Sensor54-Command-Result\":{\"Restart_flag\":%d}}"),TasmotaGlobal.restart_flag);
        break;

      default:
        serviced = false;
    }
  }
  else if (p1 < 50){
    // Commands 10-49 tied to a particular device
    device = (p1 / 10) - 1; // Leading Tens digit is device number {1-4}
    switch (p1 % 10){
      case 0:  // Show config
        show_config = true;
        break;

      case 1: // Set compacted shunt resistance from user input in ohms
        r_shunt_uohms = (uint32_t) ((CharToFloat(params[1])) * 1000000.0f);


        //AddLog( LOG_LEVEL_NONE, "r_shunt_uohms: %d", r_shunt_uohms);
        if (r_shunt_uohms > 32767){
          uint32_t r_shunt_mohms = r_shunt_uohms/1000UL;
          Settings->ina226_r_shunt[device] = (uint16_t) (r_shunt_mohms | 0x8000);
        }
        else
          Settings->ina226_r_shunt[device] = (uint16_t) r_shunt_uohms;

        //AddLog( LOG_LEVEL_NONE, "r_shunt_compacted: %04X", Settings->ina226_r_shunt[device]);
        show_config = true;
        break;

      case 2: // Set full scale current in tenths of amps from user input in Amps
        Settings->ina226_i_fs[device] = (uint16_t) ((CharToFloat(params[1])) * 10.0f);
        //AddLog( LOG_LEVEL_NONE, "i_fs: %d", Settings->ina226_i_fs[device]);
        show_config = true;
        break;


      default:
        serviced = false;
        break;
    }
  }
  else
    serviced = false;

  if (show_config) {
    char shunt_r_str[16];
    char fs_i_str[16];

    // Shunt resistance is stored in EEPROM in microohms. Convert to ohms
    r_shunt_uohms = _expand_r_shunt(Settings->ina226_r_shunt[device]);
    dtostrfd(((float)r_shunt_uohms)/1000000.0f, 6, shunt_r_str);
    // Full scale current is stored in EEPROM in tenths of an amp. Convert to amps.
    dtostrfd(((float)Settings->ina226_i_fs[device])/10.0f, 1, fs_i_str);
    // Send json response
    Response_P(PSTR("{\"Sensor54-device-settings-%d\":{\"SHUNT_R\":%s,\"FS_I\":%s}}"),
      device + 1, shunt_r_str, fs_i_str);
  }

  return serviced;
}

/*
* Show data gathered from INA226 devices
*/

#ifdef USE_WEBSERVER
const char HTTP_SNS_INA226_DATA[] PROGMEM =
  "{s}%s " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}%s " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}%s " D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";
#endif  // USE_WEBSERVER

void Ina226Show(bool json)
{
  int i, num_found;
  for (num_found = 0, i = 0; i < INA226_MAX_ADDRESSES; i++) {
    // Skip uninstalled sensors
    if (!Ina226Info[i].present)
      continue;

    num_found++;

    char voltage[16];
    dtostrfd(voltages[i], Settings->flag2.voltage_resolution, voltage);
    char current[16];
    dtostrfd(currents[i], Settings->flag2.current_resolution, current);
    char power[16];
    dtostrfd(powers[i], Settings->flag2.wattage_resolution, power);
    char name[16];
    snprintf_P(name, sizeof(name), PSTR("INA226%c%d"),IndexSeparator(), i + 1);


    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"Id\":%d,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s,\"" D_JSON_POWERUSAGE "\":%s}"),
                       name, i, voltage, current, power);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_VOLTAGE, voltage);
        DomoticzSensor(DZ_CURRENT, current);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_INA226_DATA, name, voltage, name, current, name, power);
#endif  // USE_WEBSERVER)
    }

  }

}


/**
 * The callback function Xsns_57() interfaces Tasmota with the sensor driver.
 *
 * It provides the Tasmota callback IDs.
 *
 * @param   byte    callback_id  Tasmota function ID.
 * @return  bool                 Return value.
 * @pre     None.
 * @post    None.
 *
 */
bool Xsns54(byte callback_id)
{
  if (!I2cEnabled(XI2C_35)) { return false; }

  // Set return value to `false`
  bool result = false;

  // Check which callback ID is called by Tasmota
  switch (callback_id) {
    case FUNC_EVERY_SECOND:
      Ina226EverySecond();
      break;
    case FUNC_JSON_APPEND:
      Ina226Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Ina226Show(0);
      break;
#endif // USE_WEBSERVER
    case FUNC_COMMAND_SENSOR:
      if (XSNS_54 == XdrvMailbox.index) {
        result = Ina226CommandSensor();
      }
      break;
    case FUNC_INIT:
      Ina226Init();
      break;
  }
  // Return boolean result
  return result;
}

#endif  // USE_INA226
#endif  // USE_I2C
