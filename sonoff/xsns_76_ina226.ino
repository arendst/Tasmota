// Conditional compilation of driver
#ifdef USE_INA226

// Define driver ID
#define XSNS_76  76

#define INA226_MAX_ADDRESSES                    4
#define INA226_ADDRESS1                         (0x40)    // 1000000 (A0+A1=GND)
#define INA226_ADDRESS2                         (0x41)    // 1000000 (A0=Vcc, A1=GND)
#define INA226_ADDRESS3                         (0x44)    // 1000000 (A0=GND, A1=Vcc)
#define INA226_ADDRESS4                         (0x45)    // 1000000 (A0+A1=Vcc)

#define INA226_REG_CONFIG                       (0x00)    // Config register
#define INA226_RES_CONFIG                       (0x4127)  // Config register at reset
#define INA226_DEF_CONFIG                       (0x4527)  // Our default configuration
#define INA226_CONFIG_RESET                     (0x8000)  // Config register reset bit

#define INA226_REG_SHUNTVOLTAGE                 (0x01)
#define INA226_REG_BUSVOLTAGE                   (0x02)
#define INA226_REG_POWER                        (0x03)
#define INA226_REG_CURRENT                      (0x04)
#define INA226_REG_CALIBRATION                  (0x05)


typedef struct Ina226SlaveInfo_tag {
  uint8_t address;
  uint8_t missedCount;
  uint16_t calibrationValue;
  uint16_t config;
  uint8_t present : 1;
  float i_lsb;
} Ina226SlaveInfo_t;

/*
* Program memory connstants
*/

static const uint8_t PROGMEM probeAddresses[INA226_MAX_ADDRESSES] = {INA226_ADDRESS1, INA226_ADDRESS2, INA226_ADDRESS3, INA226_ADDRESS4};

/*
* Global Variables
*/


static char Ina226Str[] = "INA226";
static uint8_t slavesFound = 0;
static uint8_t schedule_reinit = 0;
static Ina226SlaveInfo_t slaveInfo[4] = {0};
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
  AddLog_P2( LOG_LEVEL_NONE, PSTR("%s: %s"), str, fstr );
}

/*
* Set calibration value for Ina226
*/

void Ina226SetCalibration(uint8_t slaveIndex)
{

Ina226SlaveInfo_t *si = slaveInfo + slaveIndex;

I2cWrite16( si->address, INA226_REG_CALIBRATION, si->calibrationValue);

}


/*
* Test for presence of an Ina226
*/

bool Ina226TestPresence(uint8_t device)
{

  // Read config

  uint16_t config  = I2cRead16( slaveInfo[device].address, INA226_REG_CONFIG );
  //AddLog_P2( LOG_LEVEL_NONE, PSTR("Config register %04x" ), config);

  if (config != slaveInfo[device].config)
    return false;

  return true;

}


/*
* Initialize INA226 devices
*/

void Ina226Init()
{


  uint32_t i;

  slavesFound = 0;

  Ina226SlaveInfo_t *p = slaveInfo;


  //AddLog_P2( LOG_LEVEL_NONE, "Ina226Init");
  //AddLog_P2( LOG_LEVEL_NONE, "Size of Settings: %d bytes", sizeof(Settings));

  if (!i2c_flg)
    AddLog_P2(LOG_LEVEL_DEBUG, "INA226: Initialization failed: No I2C support");


  // Clear slave info data

  for (i = 0; i < 4; i++){
    *p = {0};
  }

  //AddLog_P2( LOG_LEVEL_NONE, PSTR("Sizeof Ina226Cfg: %d" ), sizeof(Ina226Cfg));

  // Detect devices

  for (i = 0; (i < INA226_MAX_ADDRESSES); i++){
    uint8_t addr = pgm_read_byte(probeAddresses + i);

    // Skip device probing if the full scale current is zero
    if (!Settings.ina226_config[i].i_fs)
      continue;


    //AddLog_P2( LOG_LEVEL_NONE, PSTR("INA226 trying slave address %02x" ), addr );

    // Try Resetting the device

    if (!I2cWrite16( addr, INA226_REG_CONFIG, INA226_CONFIG_RESET)){

      AddLog_P2( LOG_LEVEL_DEBUG, "No INA226 at address: %02X", addr);
      continue; // No device
    }

    // Read config

    uint16_t config  = I2cRead16( addr, INA226_REG_CONFIG );
    //AddLog_P2( LOG_LEVEL_NONE, PSTR("INA226 Config register %04x" ), config);

    if (INA226_RES_CONFIG != config)
      continue;


    config = INA226_DEF_CONFIG; // Fixme

    // Set the default configuration
    if (!I2cWrite16( addr, INA226_REG_CONFIG, config))
        continue; // No device

    // store data in slave info struct.

    p = &slaveInfo[i];
    // Address
    p->address = addr;
    // Configuration
    p->config = config;
    // Full scale current in tenths of an amp
    //AddLog_P2( LOG_LEVEL_NONE, "Full Scale I in tenths of an amp: %u", Settings.ina226_config[i].i_fs );
    p->i_lsb = (((float) Settings.ina226_config[i].i_fs)/10.0f)/32768.0f;
    //_debug_fval("i_lsb: %s", p->i_lsb, 7);
    // Shunt resistance in microohm increments
    //AddLog_P2( LOG_LEVEL_NONE, "Shunt R in micro-ohms: %u", Settings.ina226_config[i].r_shunt );
    p->calibrationValue = ((uint16_t) (0.00512/(p->i_lsb * ((float) Settings.ina226_config[i].r_shunt)/1000000.0f)));
    // Device present
    p->present = true;
    //AddLog_P2( LOG_LEVEL_NONE, "INA226 Device %d calibration value: %04X", i, p->calibrationValue);

    Ina226SetCalibration(i);

    //AddLog_P2( LOG_LEVEL_NONE, S_LOG_I2C_FOUND_AT, Ina226Str, addr );
    slavesFound++;

  }
}

/*
* Read the bus voltage, and return it as a float
*/

float Ina226ReadBus_v(uint8_t device)
{
  uint8_t addr = slaveInfo[device].address;
  int16_t reg_bus_v = I2cReadS16( addr, INA226_REG_BUSVOLTAGE);

  float result = ((float) reg_bus_v) * 0.00125f;

  return result;

}

/*
* Read the shunt current, and return it as a float
*/

float Ina226ReadShunt_i(uint8_t device)
{
  uint8_t addr = slaveInfo[device].address;
  int16_t reg_shunt_i = I2cReadS16( addr, INA226_REG_CURRENT);

  float result = ((float) reg_shunt_i) * slaveInfo[device].i_lsb;

  return result;
}

/*
* Read the calculated power
*/

float Ina226ReadPower_w(uint8_t device)
{
  uint8_t addr = slaveInfo[device].address;
  int16_t reg_shunt_i = I2cReadS16( addr, INA226_REG_POWER);

  float result = ((float) reg_shunt_i) * (slaveInfo[device].i_lsb * 25.0);

  return result;
}


/*
* Read voltage, shunt voltage, current, and power registerd for a given device
*/

void Ina226Read(uint8_t device)
{
  //AddLog_P2( LOG_LEVEL_NONE, "Ina226Read");
  voltages[device] = Ina226ReadBus_v(device);
  currents[device] = Ina226ReadShunt_i(device);
  powers[device] = Ina226ReadPower_w(device);
  //AddLog_P2( LOG_LEVEL_NONE, "INA226 Device %d", device );
  //_debug_fval("Voltage", voltages[device]);
  //_debug_fval("Current", currents[device]);
  //_debug_fval("Power", powers[device]);
}

/*
* Poll sensors, and chack for sensor presence
*/

void Ina226EverySecond()
{
  //AddLog_P2( LOG_LEVEL_NONE, "Ina226EverySecond");
  for (uint8_t device = 0; device < INA226_MAX_ADDRESSES; device++){
    // If there are slaves, and the device was present, and the device still is present, read its registers
    if (slavesFound && slaveInfo[device].present && Ina226TestPresence(device)){
      Ina226Read(device);
    }
    else {
        powers[device] = currents[device] = voltages[device] = 0.0f;
        // If device was present, note that it dropped off here
        //if(slaveInfo[device].present){
          //reinit_count[device]++;
          //AddLog_P2( LOG_LEVEL_DEBUG, "INA226 Device %d dropped off, count: %d", device, reinit_count[device]);
        //}
        // Device no longer present
        slaveInfo[device].present = false;
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

  //AddLog_P2( LOG_LEVEL_NONE, "Command received: %d", XdrvMailbox.payload);
  //AddLog_P2( LOG_LEVEL_NONE, "Command data received: %s", XdrvMailbox.data);

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
      //AddLog_P2( LOG_LEVEL_NONE, "INA226 Command parameter: %d, value: %s", param_count, params[param_count]);
      param_count++;
      cp = param_str + i + 1;
    }


  if (p1 < 10 || p1 >= 50){
    // Device-less commands
    switch (p1){
      case 1: // Rerun init
        Ina226Init();
        Response_P(PSTR("{\"Sensor76-Command-Result\":{\"SlavesFound\":%d}}"),slavesFound);
        break;

      case 2: // Save and restart
        restart_flag = 2;
        Response_P(PSTR("{\"Sensor76-Command-Result\":{\"Restart_flag\":%d}}"),restart_flag);
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

      case 1: // Set shunt resistance in microohms from user input in ohms
        Settings.ina226_config[device].r_shunt = (uint32_t) ((CharToFloat(params[1])) * 1000000.0f);
        //AddLog_P2( LOG_LEVEL_NONE, "r_shunt: %d", Settings.ina226_config[device].r_shunt);
        show_config = true;
        break;

      case 2: // Set full scale current in tenths of amps from user input in Amps
        Settings.ina226_config[device].i_fs = (uint16_t) ((CharToFloat(params[1])) * 10.0f);
        //AddLog_P2( LOG_LEVEL_NONE, "i_fs: %d", Settings.ina226_config[device].i_fs);
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
    dtostrfd(((float)Settings.ina226_config[device].r_shunt)/1000000.0, 6, shunt_r_str);
    // Full scale current is stored in EEPROM in tenths of an amp. Convert to amps.
    dtostrfd(((float)Settings.ina226_config[device].i_fs)/10.0f, 1, fs_i_str);
    // Send json response
    Response_P(PSTR("{\"Sensor76-device-settings-%d\":{\"SHUNT_R\":%s,\"FS_I\":%s}}"),
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
    if (!slaveInfo[i].present)
      continue;

    num_found++;

    char voltage[16];
    dtostrfd(voltages[i], Settings.flag2.voltage_resolution, voltage);
    char current[16];
    dtostrfd(currents[i], Settings.flag2.current_resolution, current);
    char power[16];
    dtostrfd(powers[i], Settings.flag2.wattage_resolution, power);
    char name[16];
    snprintf_P(name, sizeof(name), PSTR("INA226%c%d"),IndexSeparator(), i + 1);


    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"Id\":%02x,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s,\"" D_JSON_POWERUSAGE "\":%s}"),
                       name, i, voltage, current, power);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
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
 * @return  boolean              Return value.
 * @pre     None.
 * @post    None.
 *
 */
boolean Xsns76(byte callback_id) {

  // Set return value to `false`
  boolean result = false;

  // Check if I2C interface mode
// if(i2c_flg) {

  // Check which callback ID is called by Tasmota
  switch (callback_id) {
    case FUNC_INIT:
      Ina226Init();
      break;
    case FUNC_EVERY_50_MSECOND:
      break;
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
    case FUNC_SAVE_BEFORE_RESTART:
      break;
    case FUNC_COMMAND_SENSOR:
      if (XSNS_76 == XdrvMailbox.index) {
        result = Ina226CommandSensor();
      }
      break;
  }
// } // if(i2c_flg)

  // Return boolean result
  return result;
}
#endif // USE_<driver_name>
