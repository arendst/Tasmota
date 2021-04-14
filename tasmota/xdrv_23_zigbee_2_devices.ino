/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

#ifndef ZIGBEE_SAVE_DELAY_SECONDS
#define ZIGBEE_SAVE_DELAY_SECONDS 2               // wait for 2s before saving Zigbee info
#endif
const uint16_t kZigbeeSaveDelaySeconds = ZIGBEE_SAVE_DELAY_SECONDS;    // wait for x seconds

enum class Z_Data_Type : uint8_t {
  Z_Unknown = 0x00,
  Z_Light = 1,              // Lights 1-5 channels
  Z_Plug = 2,               // Plug power consumption
  Z_PIR = 3,
  Z_Alarm = 4,
  Z_Thermo = 5,             // Thermostat and sensor for home environment (temp, himudity, pressure)
  Z_OnOff = 6,              // OnOff, Buttons and Relays (always complements Lights and Plugs)
  Z_Mode = 0xE,             // Encode special modes for communication, like Tuya Zigbee protocol
  Z_Ext = 0xF,              // extended for other values
  Z_Device = 0xFF           // special value when parsing Device level attributes
};

const uint8_t Z_Data_Type_char[] PROGMEM = {
  '?',      // 0x00 Z_Data_Type::Z_Unknown
  'L',      // 0x01 Z_Data_Type::Z_Light
  'P',      // 0x02 Z_Data_Type::Z_Plug
  'I',      // 0x03 Z_Data_Type::Z_PIR
  'A',      // 0x04 Z_Data_Type::Z_Alarm
  'T',      // 0x05 Z_Data_Type::Z_Thermo
  'O',      // 0x06 Z_Data_Type::Z_OnOff
  '\0',     // 0x07
  '\0',     // 0x08
  '\0',     // 0x09
  '\0',     // 0x0A
  '\0',     // 0x0B
  '\0',     // 0x0C
  '\0',     // 0x0D
  '~',      // 0x0E
  'E',      // 0x0F Z_Data_Type::Z_Ext
  // '_' maps to 0xFF Z_Data_Type::Z_Device
};

class Z_Data_Set;
/*********************************************************************************************\
 * Device specific data, sensors...
\*********************************************************************************************/
class Z_Data {
public:
  Z_Data(Z_Data_Type type = Z_Data_Type::Z_Unknown, uint8_t endpoint = 0) : _type(type), _endpoint(endpoint), _config(0xF), _align_1(0), _reserved(0) {}
  inline Z_Data_Type getType(void) const { return _type; }
  inline int8_t getConfig(void) const { return _config; }
  inline bool validConfig(void) const { return _config != 0xF; }
  inline void setConfig(int8_t config) { _config = config; }
  uint8_t getConfigByte(void) const { return ( ((uint8_t)_type) << 4) | ((_config & 0xF) & 0x0F); }
  uint8_t getReserved(void) const { return _reserved; }

  inline uint8_t getEndpoint(void) const { return _endpoint; }

  void toAttributes(Z_attribute_list & attr_list) const;

  // update internal structures after an attribut update
  // True if a configuration was changed
  inline bool update(void) { return false; }

  static const Z_Data_Type type = Z_Data_Type::Z_Unknown;
  static size_t DataTypeToLength(Z_Data_Type t);
  static bool ConfigToZData(const char * config_str, Z_Data_Type * type, uint8_t * ep, uint8_t * config);

  static Z_Data_Type CharToDataType(char c);
  static char DataTypeToChar(Z_Data_Type t);

  friend class Z_Data_Set;
protected:
  Z_Data_Type _type;        // encoded on 4 bits, type of the device
  uint8_t _endpoint;        // source endpoint, or 0x00 if any endpoint
  uint8_t _config : 4;      // encoded on 4 bits, customize behavior
  uint8_t _align_1 : 4;     // force aligned to bytes, and fill with zero
  uint8_t _reserved;           // power state if the type supports it
};

Z_Data_Type Z_Data::CharToDataType(char c) {
  if (c) {
    if (c == '_') {
      return Z_Data_Type::Z_Device;
    } else {
      for (uint32_t i=0; i<nitems(Z_Data_Type_char); i++) {
        if (pgm_read_byte(&Z_Data_Type_char[i]) == c) {
          return (Z_Data_Type) i;
        }
      }
    }
  }
  return Z_Data_Type::Z_Unknown;
}

char Z_Data::DataTypeToChar(Z_Data_Type t) {
  if (t == Z_Data_Type::Z_Device) {
    return '_';
  } else {
    uint8_t tt = (uint8_t) t;
    if (tt < nitems(Z_Data_Type_char)) {
      return pgm_read_byte(&Z_Data_Type_char[tt]);
    }
  }
  return '\0';
}

// Parse configuration
// Either '_'
// or 'T01' or 'L01.2'
// result: true if parsing ok
bool Z_Data::ConfigToZData(const char * config_str, Z_Data_Type * type, uint8_t * ep, uint8_t * config) {
  if (config_str == nullptr) { return false; }

  Z_Data_Type data_type = Z_Data::CharToDataType(config_str[0]);
  if (data_type == Z_Data_Type::Z_Unknown) {
    return false;
  }

  if (type != nullptr) {
    *type = data_type;
  }
  if (ep != nullptr) {
    *ep = strtoul(&config_str[1], nullptr, 16);   // hex base 16
  }
  if ((config != nullptr) && (config_str[3] == '.')) {
    // we have a config attribute
    *config = strtoul(&config_str[4], nullptr, 16) & 0x0F;   // hex base 16
  }
  return true;
}

/*********************************************************************************************\
 * Device specific: On/Off, power up to 8 relays
\*********************************************************************************************/

// _config contains the number of valid OnOff relays: 0..7
// _power contains a bitfield of relays values
class Z_Data_OnOff : public Z_Data {
public:
  Z_Data_OnOff(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_OnOff, endpoint),
    power(0xFF)
    {}

  inline bool validPower(void) const { return 0xFF != power; }      // power is declared
  inline bool getPower(void)   const { return validPower() ? (power != 0) : false; }   // default to false if undefined
  inline void setPower(bool val)     { power = val ? 1 : 0; }

  static const Z_Data_Type type = Z_Data_Type::Z_OnOff;

  // 1 byte
  uint8_t           power;
};

/*********************************************************************************************\
 * Device specific: Plug device
\*********************************************************************************************/
class Z_Data_Plug : public Z_Data {
public:
  Z_Data_Plug(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_Plug, endpoint),
    mains_voltage(0xFFFF),
    mains_power(-0x8000)
    {}

  inline bool validMainsVoltage(void)   const { return 0xFFFF != mains_voltage; }
  inline bool validMainsPower(void)     const { return -0x8000 != mains_power; }

  inline uint16_t getMainsVoltage(void) const { return mains_voltage; }
  inline int16_t  getMainsPower(void)   const { return mains_power; }

  inline void setMainsVoltage(uint16_t _mains_voltage)  { mains_voltage = _mains_voltage; }
  inline void setMainsPower(int16_t _mains_power)       { mains_power = _mains_power; }

  static const Z_Data_Type type = Z_Data_Type::Z_Plug;
  // 4 bytes
  uint16_t              mains_voltage;  // AC voltage
  int16_t               mains_power;    // Active power
};

/*********************************************************************************************\
 * Device specific: Light device
\*********************************************************************************************/
class Z_Data_Light : public Z_Data {
public:
  Z_Data_Light(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_Light, endpoint),
    colormode(0xFF),
    dimmer(0xFF),
    sat(0xFF),
    hue(0xFF),
    ct(0xFFFF),
    x(0xFFFF),
    y(0xFFFF)
    {}

  inline bool validColormode(void)      const { return 0xFF != colormode; }
  inline bool validDimmer(void)         const { return 0xFF != dimmer; }
  inline bool validSat(void)            const { return 0xFF != sat; }
  inline bool validHue(void)            const { return 0xFF != hue; }
  inline bool validCT(void)             const { return 0xFFFF != ct; }
  inline bool validX(void)              const { return 0xFFFF != x; }
  inline bool validY(void)              const { return 0xFFFF != y; }

  inline uint8_t  getColorMode(void)    const { return colormode; }
  inline uint8_t  getDimmer(void)       const { return dimmer; }
  inline uint8_t  getSat(void)          const { return sat; }
  inline uint16_t getHue(void)          const { return changeUIntScale(hue, 0, 254, 0, 360); }
  inline uint16_t getCT(void)           const { return ct; }
  inline uint16_t getX(void)            const { return x; }
  inline uint16_t getY(void)            const { return y; }

  inline void setColorMode(uint8_t _colormode)  { colormode = _colormode; }
  inline void setDimmer(uint8_t _dimmer)        { dimmer = _dimmer; }
  inline void setSat(uint8_t _sat)              { sat = _sat; }
  inline void setHue(uint16_t _hue)             { hue = changeUIntScale(_hue, 0, 360, 0, 254);; }
  inline void setCT(uint16_t _ct)               { ct = _ct; }
  inline void setX(uint16_t _x)                 { x = _x; }
  inline void setY(uint16_t _y)                 { y = _y; }

  void toRGBAttributes(Z_attribute_list & attr_list) const ;
  static void toRGBAttributesHSB(Z_attribute_list & attr_list, uint16_t hue, uint8_t sat, uint8_t brightness);
  static void toRGBAttributesXYB(Z_attribute_list & attr_list, uint16_t x, uint16_t y, uint8_t brightness);
  static void toRGBAttributesRGBb(Z_attribute_list & attr_list, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);

  static const Z_Data_Type type = Z_Data_Type::Z_Light;
  // 12 bytes
  uint8_t               colormode;      // 0x00: Hue/Sat, 0x01: XY, 0x02: CT | 0xFF not set, default 0x01
  uint8_t               dimmer;         // last Dimmer value: 0-254 | 0xFF not set, default 0x00
  uint8_t               sat;            // last Sat: 0..254 | 0xFF not set, default 0x00
  uint8_t               hue;            // last Hue: 0..359 | 0xFFFF not set, default 0
  uint16_t              ct;             // last CT: 153-500 | 0xFFFF not set, default 200
  uint16_t              x, y;           // last color [x,y] | 0xFFFF not set, default 0
};

/*********************************************************************************************\
 * Device specific: PIR
 *
  // List of occupancy time-outs:
  // 0xF = default (90 s)
  // 0x0 = no time-out
  // 0x1 = 15 s
  // 0x2 = 30 s
  // 0x3 = 45 s
  // 0x4 = 60 s
  // 0x5 = 75 s
  // 0x6 = 90 s -- default
  // 0x7 = 105 s
  // 0x8 = 120 s
\*********************************************************************************************/
class Z_Data_PIR : public Z_Data {
public:
  Z_Data_PIR(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_PIR, endpoint),
    occupancy(0xFF),
    illuminance(0xFFFF)
    {}

  inline bool validOccupancy(void)      const { return 0xFF != occupancy; }
  inline bool validIlluminance(void)    const { return 0xFFFF != illuminance; }

  inline uint8_t  getOccupancy(void)    const { return occupancy; }
  inline uint16_t getIlluminance(void)  const { return illuminance; }

  inline void setOccupancy(uint8_t _occupancy)          { occupancy = _occupancy; }
  inline void setIlluminance(uint16_t _illuminance)     { illuminance = _illuminance; }

  uint32_t getTimeoutSeconds(void) const;
  void setTimeoutSeconds(int32_t value);

  static const Z_Data_Type type = Z_Data_Type::Z_PIR;
  // PIR
  uint8_t               occupancy;      // map8
  uint16_t              illuminance;    // illuminance
};

uint32_t Z_Data_PIR::getTimeoutSeconds(void) const {
  if (_config != 0xF) {
    return _config * 15;
  } else {
    return 90;
  }
}

void Z_Data_PIR::setTimeoutSeconds(int32_t value) {
  if (value < 0) {
    _config = 0xF;
  } else {
    uint32_t val_15 = (value + 14)/ 15;   // always round up
    if (val_15 > 8) { val_15 = 8; }
    _config = val_15;
  }
}

/*********************************************************************************************\
 * Device specific: Sensors: temp, humidity, pressure...
\*********************************************************************************************/
enum Z_Alarm_Type {
  ZA_CIE =        0x0,
  ZA_PIR =        0x1,
  ZA_Contact =    0x2,
  ZA_Fire =       0x3,
  ZA_Water =      0x4,
  ZA_CO =         0x5,
  ZA_Personal =   0x6,
  ZA_Movement =   0x7,
  ZA_Panic =      0x8,
  ZA_GlassBreak = 0x9,
};

class Z_Data_Thermo : public Z_Data {
public:
  Z_Data_Thermo(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_Thermo, endpoint),
    temperature(-0x8000),
    pressure(-0x8000),
    humidity(0xFFFF),
    th_setpoint(0xFF),
    temperature_target(-0x8000)
    {}

  inline bool validTemperature(void)    const { return -0x8000 != temperature; }
  inline bool validPressure(void)       const { return -0x8000 != pressure; }
  inline bool validHumidity(void)       const { return 0xFFFF != humidity; }
  inline bool validThSetpoint(void)     const { return 0xFF != th_setpoint; }
  inline bool validTempTarget(void)     const { return -0x8000 != temperature_target; }

  inline int16_t  getTemperature(void)  const { return temperature; }
  inline int16_t getPressure(void)      const { return pressure; }
  inline uint16_t getHumidity(void)     const { return humidity; }
  inline uint8_t  getThSetpoint(void)   const { return th_setpoint; }
  inline int16_t  getTempTarget(void)   const { return temperature_target; }

  inline void setTemperature(int16_t _temperature)      { temperature = _temperature; }
  inline void setPressure(int16_t _pressure)            { pressure = _pressure; }
  inline void setHumidity(uint16_t _humidity)           { humidity = _humidity; }
  inline void setThSetpoint(uint8_t _th_setpoint)       { th_setpoint = _th_setpoint; }
  inline void setTempTarget(int16_t _temperature_target){ temperature_target = _temperature_target; }

  static const Z_Data_Type type = Z_Data_Type::Z_Thermo;
  // 8 bytes
  // sensor data
  int16_t               temperature;    // temperature in 1/10th of Celsius, 0x8000 if unknown
  int16_t               pressure;       // air pressure in hPa, 0xFFFF if unknown
  uint16_t              humidity;       // humidity in percent, 0..100, 0xFF if unknown
  // thermostat
  uint8_t               th_setpoint;    // percentage of heat/cool in percent
  int16_t               temperature_target; // settings for the temparature
};

/*********************************************************************************************\
 * Device specific: Alarm
\*********************************************************************************************/
// We're lucky that alarm type fits in 12 bits, so we can have a total entry of 16 bits
typedef union Z_Alarm_Types_t {
  struct {
    uint16_t    zcl_type : 12;
    uint8_t     config : 4;
  } t;
  uint16_t i;
} Z_Alarm_Types_t;

static const Z_Alarm_Types_t Z_Alarm_Types[] PROGMEM = {
  { .t = { 0x000, ZA_CIE }},        // 0x0 : Standard CIE
  { .t = { 0x00d, ZA_PIR }},        // 0x1 : PIR
  { .t = { 0x015, ZA_Contact }},    // 0x2 : Contact
  { .t = { 0x028, ZA_Fire }},       // 0x3 : Fire
  { .t = { 0x02a, ZA_Water }},       // 0x4 : Leak
  { .t = { 0x02b, ZA_CO }},         // 0x5 : CO
  { .t = { 0x02c, ZA_Personal }},   // 0x6 : Personal
  { .t = { 0x02d, ZA_Movement }},   // 0x7 : Movement
  { .t = { 0x10f, ZA_Panic }},      // 0x8 : Panic
  { .t = { 0x115, ZA_Panic }},      // 0x8 : Panic
  { .t = { 0x21d, ZA_Panic }},      // 0x8 : Panic
  { .t = { 0x226, ZA_GlassBreak }}, // 0x9 : Glass break
};

class Z_Data_Alarm : public Z_Data {
public:
  Z_Data_Alarm(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_Alarm, endpoint),
    zone_type(0xFFFF)
    {}

  static const Z_Data_Type type = Z_Data_Type::Z_Alarm;

  inline bool validZoneType(void)   const { return 0xFFFF != zone_type; }

  inline uint16_t getZoneType(void) const { return zone_type; }
  inline bool isPIR(void)             const { return ZA_PIR         == _config; }
  inline bool isContact(void)         const { return ZA_Contact     == _config; }
  inline bool isFire(void)            const { return ZA_Fire        == _config; }
  inline bool isWater(void)            const { return ZA_Water        == _config; }
  inline bool isCO(void)              const { return ZA_CO          == _config; }
  inline bool isPersonalAlarm(void)   const { return ZA_Personal    == _config; }
  inline bool isMovement(void)        const { return ZA_Movement    == _config; }
  inline bool isPanic(void)           const { return ZA_Panic       == _config; }
  inline bool isGlassBreak(void)      const { return ZA_GlassBreak  == _config; }

  inline void setZoneType(uint16_t _zone_type)  { zone_type = _zone_type; }

  bool update(void) {
    for (uint32_t i=0; i<nitems(Z_Alarm_Types); i++) {
      Z_Alarm_Types_t conv_type;
      conv_type.i = pgm_read_word(&Z_Alarm_Types[i].i);
      if (zone_type == conv_type.t.zcl_type) {
        if (_config == conv_type.t.config) {
          return false;     // no change
        } else {
          _config = conv_type.t.config;
          return true;
        }
      }
    }
    return false;
  }

  void convertZoneStatus(Z_attribute_list & attr_list, uint16_t val) const;

  // 4 bytes
  uint16_t              zone_status;      // last known state for sensor 1 & 2
  uint16_t              zone_type;        // mapped to the Zigbee standard
    // 0x0000  Standard CIE
    // 0x000d  Motion sensor
    // 0x0015  Contact switch
    // 0x0028  Fire sensor
    // 0x002a  Water sensor
    // 0x002b  Carbon Monoxide (CO) sensor
    // 0x002c  Personal emergency device
    // 0x002d  Vibration/Movement sensor
    // 0x010f  Remote Control
    // 0x0115  Key fob
    // 0x021d  Keypad
    // 0x0225  Standard Warning Device (see [N1] part 4)
    // 0x0226  Glass break sensor
    // 0x0229  Security repeater*
};

void Z_Data_Alarm::convertZoneStatus(Z_attribute_list & attr_list, uint16_t val) const {
  if (validConfig()) {
    // indicator #1, published for false and true
    if (isPIR()) {                  // set Occupancy
      attr_list.addAttribute(0x0406, 0x0000).setUInt(val & 0x01 ? 1 : 0);
    } else {                              // all other cases
      attr_list.addAttribute(0x0500, 0xFFF0 + getConfig()).setUInt(val & 0x01 ? 1 : 0);
    }
    // indicator #2 published only if true
    if (val & 0x02) {
      if (isPIR()) {                  // set Occupancy
        attr_list.addAttribute(0x0406, 0x0000, 2).setUInt(val & 0x02 ? 1 : 0);
      } else {                              // all other cases
        attr_list.addAttribute(0x0500, 0xFFF0 + getConfig(), 2).setUInt(val & 0x02 ? 1 : 0);
      }
    }
    // Tamper
    if (val & 0x04) {
      attr_list.addAttributePMEM(PSTR("Tamper")).setUInt(1);
    }
    // BatteryLow
    if (val & 0x08) {
      attr_list.addAttributePMEM(PSTR("BatteryLow")).setUInt(1);
    }
    // // SupervisionReports
    // if (val & 0x10) {
    //   attr_list.addAttributePMEM(PSTR("SupervisionReports")).setUInt(1);
    // }
    // // RestoreReports
    // if (val & 0x20) {
    //   attr_list.addAttributePMEM(PSTR("RestoreReports")).setUInt(1);
    // }
    // Failure
    if (val & 0x40) {
      attr_list.addAttributePMEM(PSTR("Failure")).setUInt(1);
    }
    // MainsFault
    if (val & 0x80) {
      attr_list.addAttributePMEM(PSTR("MainsFault")).setUInt(1);
    }
    // Test
    if (val & 0x100) {
      attr_list.addAttributePMEM(PSTR("Test")).setUInt(1);
    }
    // BatteryDefect
    if (val & 0x200) {
      attr_list.addAttributePMEM(PSTR("BatteryDefect")).setUInt(1);
    }
  }
}


/*********************************************************************************************\
 * Mode
 *
  // List of modes
  // 0x1 = Tuya Zigbee mode
  // 0xF (default) = ZCL standard mode
\*********************************************************************************************/
enum Z_Mode_Type {
  ZM_Tuya =       0x1,
};

class Z_Data_Mode : public Z_Data {
public:
  Z_Data_Mode(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_Mode, endpoint)
    {}

  inline bool isTuyaProtocol(void) const { return _config == 1; }

  static const Z_Data_Type type = Z_Data_Type::Z_Mode;
};

/*********************************************************************************************\
 *
\*********************************************************************************************/
const uint8_t Z_Data_Type_len[] PROGMEM = {
  0,                        // 0x00 Z_Data_Type::Z_Unknown
  sizeof(Z_Data_Light),     // 0x01 Z_Data_Type::Z_Light
  sizeof(Z_Data_Plug),      // 0x02 Z_Data_Type::Z_Plug
  sizeof(Z_Data_PIR),       // 0x03 Z_Data_Type::Z_PIR
  sizeof(Z_Data_Alarm),     // 0x04 Z_Data_Type::Z_Alarm
  sizeof(Z_Data_Thermo),    // 0x05 Z_Data_Type::Z_Thermo
  sizeof(Z_Data_OnOff),     // 0x06 Z_Data_Type::Z_OnOff
  0,     // 0x07
  0,     // 0x08
  0,     // 0x09
  0,     // 0x0A
  0,     // 0x0B
  0,     // 0x0C
  0,     // 0x0D
  sizeof(Z_Data_Mode),      // 0x0E
  0,      // 0x0F Z_Data_Type::Z_Ext
  // '_' maps to 0xFF Z_Data_Type::Z_Device
};

size_t Z_Data::DataTypeToLength(Z_Data_Type t) {
  uint32_t tt = (uint32_t) t;
  if (tt < nitems(Z_Data_Type_len)) {
    return pgm_read_byte(&Z_Data_Type_len[tt]);
  }
  return 0;
}


/*********************************************************************************************\
 *
 * Device specific Linked List
 *
\*********************************************************************************************/
class Z_Data_Set : public LList<Z_Data> {
public:
  // List<Z_Data>() : List<Z_Data>() {}
  Z_Data & getByType(Z_Data_Type type, uint8_t ep = 0);     // creates if non-existent
  const Z_Data & find(Z_Data_Type type, uint8_t ep = 0) const;

  // getX() always returns a valid object, and creates the object if there is none
  // find() does not create an object if it does not exist, and returns *(X*)nullptr

  // Emulate a virtuel update method for Z_Data
  static bool updateData(Z_Data & elt);

  template <class M>
  M & get(uint8_t ep = 0);
  template <class M>
  const M & getConst(uint8_t ep = 0) const;

  template <class M>
  const M & find(uint8_t ep = 0) const;

  // create a new data object from a 4 bytes buffer
  Z_Data & createFromBuffer(const SBuffer & buf, uint32_t start, uint32_t len);

  // check if the pointer is null, if so create a new object with the right sub-class
  template <class M>
  M & addIfNull(M & cur, uint8_t ep = 0);
};

bool Z_Data_Set::updateData(Z_Data & elt) {
  switch (elt._type) {
    case Z_Data_Type::Z_Light:  return ((Z_Data_Light&) elt).update();       break;
    case Z_Data_Type::Z_Plug:   return ((Z_Data_Plug&) elt).update();        break;
    case Z_Data_Type::Z_Alarm:  return ((Z_Data_Alarm&) elt).update();       break;
    case Z_Data_Type::Z_Thermo: return ((Z_Data_Thermo&) elt).update();      break;
    case Z_Data_Type::Z_OnOff:  return ((Z_Data_OnOff&) elt).update();       break;
    case Z_Data_Type::Z_PIR:    return ((Z_Data_PIR&) elt).update();         break;
    case Z_Data_Type::Z_Mode:   return ((Z_Data_Mode&) elt).update();        break;
    default: return false;
  }
}

Z_Data & Z_Data_Set::getByType(Z_Data_Type type, uint8_t ep) {
  switch (type) {
    case Z_Data_Type::Z_Light:
      return get<Z_Data_Light>(ep);
    case Z_Data_Type::Z_Plug:
      return get<Z_Data_Plug>(ep);
    case Z_Data_Type::Z_Alarm:
      return get<Z_Data_Alarm>(ep);
    case Z_Data_Type::Z_Thermo:
      return get<Z_Data_Thermo>(ep);
    case Z_Data_Type::Z_OnOff:
      return get<Z_Data_OnOff>(ep);
    case Z_Data_Type::Z_PIR:
      return get<Z_Data_PIR>(ep);
    case Z_Data_Type::Z_Mode:
      return get<Z_Data_Mode>(ep);
    default:
      return *(Z_Data*)nullptr;
  }
}

// Instanciate with either:
// (04)04010100          - without data except minimal Z_Data
// (08)04010100.B06DFFFF - with complete data - in this case must not exceed the structure len
//
// Byte 0: type
// Byte 1: endpoint
// Byte 2: config
// Byte 3: Power
Z_Data & Z_Data_Set::createFromBuffer(const SBuffer & buf, uint32_t start, uint32_t len) {
  if (len < sizeof(Z_Data)) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Invalid len (<4) %d"), len);
    return *(Z_Data*)nullptr;
  }

  Z_Data_Type data_type = (Z_Data_Type) buf.get8(start);
  uint8_t expected_len = Z_Data::DataTypeToLength(data_type);
  uint8_t endpoint = buf.get8(start + 1);

  Z_Data & elt = getByType(data_type, endpoint);
  if (&elt == nullptr) { return *(Z_Data*)nullptr; }
  if (len <= expected_len) {
    memcpy(&elt, buf.buf(start), len);
  } else {
    memcpy(&elt, buf.buf(start), sizeof(Z_Data));
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "buffer len overflow %d > %d"), len, expected_len);
  }
  return elt;
}

template <class M>
M & Z_Data_Set::get(uint8_t ep) {
  M & m = (M&) find(M::type, ep);
  return addIfNull<M>(m, ep);
}
template <class M>
const M & Z_Data_Set::getConst(uint8_t ep) const {
  const M & m = (M&) find(M::type, ep);
  return m;
}

template <class M>
const M & Z_Data_Set::find(uint8_t ep) const {
 return (M&) find(M::type, ep);
}

// Input: reference to object
// Output: if reference is null, instantiate object and add it at the end of the list
template <class M>
M & Z_Data_Set::addIfNull(M & cur, uint8_t ep) {
  if (nullptr == &cur) {
    LList_elt<M> * elt = new LList_elt<M>();
    elt->val()._endpoint = ep;
    this->addToLast((LList_elt<Z_Data>*)elt);
    return elt->val();
  } else {
    if (cur._endpoint == 0) { cur._endpoint = ep; }     // be more specific on endpoint
    return cur;
  }
}

const Z_Data & Z_Data_Set::find(Z_Data_Type type, uint8_t ep) const {
  for (auto & elt : *this) {
    if (elt._type == type) {
      // type matches, check if ep matches.
      // 0 matches all endpoints
      if ((ep == 0) || (elt._endpoint == 0) || (ep == elt._endpoint)) {
        return elt;
      }
    }
  }
  return *(Z_Data*)nullptr;
}

/*********************************************************************************************\
 * Structures for Rules variables related to the last received message
\*********************************************************************************************/
const size_t endpoints_max = 8;         // we limit to 8 endpoints

class Z_Device {
public:

  uint64_t              longaddr;       // 0x00 means unspecified
  char *                manufacturerId;
  char *                modelId;
  char *                friendlyName;
  // _defer_last_time : what was the last time an outgoing message is scheduled
  // this is designed for flow control and avoid messages to be lost or unanswered
  uint32_t              defer_last_message_sent;

  uint8_t               endpoints[endpoints_max];   // static array to limit memory consumption, list of endpoints until 0x00 or end of array
  // Used for attribute reporting
  Z_attribute_list      attr_list;
  // sequence number for Zigbee frames
  uint16_t              shortaddr;      // unique key if not null, or unspecified if null
  uint8_t               seqNumber;
  bool                  is_router;         // flag used by ZbMap to distibguish routers from end-devices
  bool                  hidden;
  bool                  reachable;
  // Light information for Hue integration integration, last known values

  // New version of device data handling
  Z_Data_Set            data;            // Linkedlist of device data per endpoint
  // other status - device wide data is 8 bytes
  // START OF DEVICE WIDE DATA
  uint32_t              last_seen;      // Last seen time (epoch)
  uint8_t               lqi;            // lqi from last message, 0xFF means unknown
  uint8_t               batterypercent; // battery percentage (0..100), 0xFF means unknwon
  uint16_t              reserved_for_alignment;
  // Debounce informmation when receiving commands
  // If we receive the same ZCL transaction number from the same device and the same endpoint within 300ms
  // then discard the second packet
  uint8_t               debounce_endpoint;  // endpoint of the last received packet, or 0 if not active (expired)
  uint8_t               debounce_transact;  // ZCL transaction number of the last packet
  // END OF DEVICE WIDE DATA

  // Constructor with all defaults
  Z_Device(uint16_t _shortaddr = BAD_SHORTADDR, uint64_t _longaddr = 0x00):
    longaddr(_longaddr),
    manufacturerId(nullptr),
    modelId(nullptr),
    friendlyName(nullptr),
    defer_last_message_sent(0),
    endpoints{ 0, 0, 0, 0, 0, 0, 0, 0 },
    attr_list(),
    shortaddr(_shortaddr),
    seqNumber(0),
    is_router(false),
    hidden(false),
    reachable(false),
    data(),
    last_seen(0),
    lqi(0xFF),
    batterypercent(0xFF),
    reserved_for_alignment(0xFFFF),
    debounce_endpoint(0),
    debounce_transact(0)
    { };

  inline bool valid(void)               const { return BAD_SHORTADDR != shortaddr; }    // is the device known, valid and found?

  inline bool validLongaddr(void)       const { return 0x0000 != longaddr; }
  inline bool validManufacturerId(void) const { return nullptr != manufacturerId; }
  inline bool validModelId(void)        const { return nullptr != modelId; }
  inline bool validFriendlyName(void)   const { return nullptr != friendlyName; }

  inline bool validPower(uint8_t ep =0)          const;

  inline bool validLqi(void)            const { return 0xFF != lqi; }
  inline bool validBatteryPercent(void) const { return 0xFF != batterypercent; }
  inline bool validLastSeen(void)       const { return 0x0 != last_seen; }

  inline void setReachable(bool _reachable)   { reachable = _reachable; }
  inline bool getReachable(void)        const { return reachable; }
  inline bool getPower(uint8_t ep =0)   const;

  inline bool isRouter(void)            const { return is_router; }
  inline bool isCoordinator(void)       const { return 0x0000 == shortaddr; }
  inline void setRouter(bool router)          { is_router = router; }

  inline void setLQI(uint8_t _lqi)            { lqi = _lqi; }
  inline void setBatteryPercent(uint8_t bp)   { batterypercent = bp; }

  // Add an endpoint to a device
  bool addEndpoint(uint8_t endpoint);
  void clearEndpoints(void);
  uint32_t countEndpoints(void) const;    // return the number of known endpoints (0 if unknown)

  void setManufId(const char * str);
  void setModelId(const char * str);
  void setFriendlyName(const char * str);

  void setLastSeenNow(void);

  // multiple function to dump part of the Device state into JSON
  void jsonAddDeviceNamme(Z_attribute_list & attr_list) const;
  void jsonAddIEEE(Z_attribute_list & attr_list) const;
  void jsonAddModelManuf(Z_attribute_list & attr_list) const;
  void jsonAddEndpoints(Z_attribute_list & attr_list) const;
  void jsonAddConfig(Z_attribute_list & attr_list) const;
  void jsonAddDataAttributes(Z_attribute_list & attr_list) const;
  void jsonAddDeviceAttributes(Z_attribute_list & attr_list) const;
  void jsonDumpSingleDevice(Z_attribute_list & attr_list, uint32_t dump_mode, bool add_name) const;
  void jsonPublishAttrList(const char * json_prefix, const Z_attribute_list &attr_list) const;
  void jsonLightState(Z_attribute_list & attr_list) const;

  // dump device attributes to ZbData
  void toAttributes(Z_attribute_list & attr_list) const;

  // Device data specific
  void setPower(bool power_on, uint8_t ep = 0);

  // If light, returns the number of channels, or 0xFF if unknown
  int8_t getLightChannels(void)        const {
    const Z_Data_Light & light = data.find<Z_Data_Light>(0);
    if (&light != nullptr) {
      return light.getConfig();
    } else {
      return -1;
    }
  }

  void setLightChannels(int8_t channels);

protected:

  static void setStringAttribute(char*& attr, const char * str);
};

/*********************************************************************************************\
 * Structures for deferred callbacks
\*********************************************************************************************/

typedef void (*Z_DeviceTimer)(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value);

// Category for Deferred actions, this allows to selectively remove active deferred or update them
typedef enum Z_Def_Category {
  Z_CAT_ALWAYS = 0,           // no category, it will happen whatever new timers
  // Below will clear any event in the same category for the same address (shortaddr / groupaddr)
  Z_CLEAR_DEVICE = 0x01,
  Z_CAT_READ_ATTR,            // Attribute reporting, either READ_ATTRIBUTE or REPORT_ATTRIBUTE, we coalesce all attributes reported if we can
  Z_CAT_VIRTUAL_OCCUPANCY,    // Creation of a virtual attribute, typically after a time-out. Ex: Aqara presence sensor
  Z_CAT_REACHABILITY,         // timer set to measure reachability of device, i.e. if we don't get an answer after 1s, it is marked as unreachable (for Alexa)
  Z_CAT_PERMIT_JOIN,          // timer to signal the end of the PermitJoin period
  // Below will clear based on device + cluster pair.
  Z_CLEAR_DEVICE_CLUSTER,
  Z_CAT_READ_CLUSTER,
  // Below will clear based on device + cluster + endpoint
  Z_CLEAR_DEVICE_CLUSTER_ENDPOINT,
  Z_CAT_EP_DESC,              // read endpoint descriptor to gather clusters
  Z_CAT_BIND,                 // send auto-binding to coordinator
  Z_CAT_CONFIG_ATTR,          // send a config attribute reporting request
  Z_CAT_READ_ATTRIBUTE,       // read a single attribute
  Z_CAT_DEBOUNCE_CMD,         // debounce incoming commands
  Z_CAT_CIE_ATTRIBUTE,        // write CIE address
  Z_CAT_CIE_ENROLL,           // enroll CIE zone
} Z_Def_Category;

const uint32_t Z_CAT_REACHABILITY_TIMEOUT = 2000;     // 1000 ms or 1s

typedef struct Z_Deferred {
  // below are per device timers, used for example to query the new state of the device
  uint32_t              timer;          // millis() when to fire the timer, 0 if no timer
  uint16_t              shortaddr;      // identifier of the device
  uint16_t              groupaddr;      // group address (if needed)
  uint16_t              cluster;        // cluster to use for the timer
  uint8_t               endpoint;       // endpoint to use for timer
  uint8_t               category;       // which category of deferred is it
  uint32_t              value;          // any raw value to use for the timer
  Z_DeviceTimer         func;           // function to call when timer occurs
} Z_Deferred;

/*********************************************************************************************\
 * Singleton for device configuration
\*********************************************************************************************/

// All devices are stored in a Vector
// Invariants:
// - shortaddr is unique if not null
// - longaddr is unique if not null
// - shortaddr and longaddr cannot be both null
class Z_Devices {
public:
  Z_Devices() : _deferred() {};

  // Probe the existence of device keys
  // Results:
  // - 0x0000 = not found
  // - BAD_SHORTADDR = bad parameter
  // - 0x<shortaddr> = the device's short address
  Z_Device & isKnownLongAddrDevice(uint64_t  longaddr) const;
  Z_Device & isKnownIndexDevice(uint32_t index) const;
  Z_Device & isKnownFriendlyNameDevice(const char * name) const;

  Z_Device & findShortAddr(uint16_t shortaddr);
  const Z_Device & findShortAddr(uint16_t shortaddr) const;
  Z_Device & findLongAddr(uint64_t longaddr);
  const Z_Device & findLongAddr(uint64_t longaddr) const;
  Z_Device & getShortAddr(uint16_t shortaddr);   // find Device from shortAddr, creates it if does not exist
  Z_Device & getLongAddr(uint64_t longaddr);     // find Device from shortAddr, creates it if does not exist
  // check if a device was found or if it's the fallback device
  inline bool foundDevice(const Z_Device & device) const { return device.valid(); }

  int32_t findFriendlyName(const char * name) const;
  uint64_t getDeviceLongAddr(uint16_t shortaddr) const;

  uint8_t findFirstEndpoint(uint16_t shortaddr) const;

  // Add new device, provide ShortAddr and optional longAddr
  // If it is already registered, update information, otherwise create the entry
  Z_Device &  updateDevice(uint16_t shortaddr, uint64_t longaddr = 0);

  inline const char * getFriendlyName(uint16_t shortaddr) const {
    return findShortAddr(shortaddr).friendlyName;
  }
  inline const char * getModelId(uint16_t shortaddr) const {
    return findShortAddr(shortaddr).modelId;
  }
  inline const char * getManufacturerId(uint16_t shortaddr) const{
    return findShortAddr(shortaddr).manufacturerId;
  }

  // get next sequence number for (increment at each all)
  uint8_t getNextSeqNumber(uint16_t shortaddr);

  // Dump json
  String dumpDevice(uint32_t dump_mode, const Z_Device & device) const;
  String dumpCoordinator(void) const;
  int32_t deviceRestore(JsonParserObject json);

  // Hue support
  int8_t getHueBulbtype(uint16_t shortaddr) const ;
  void hideHueBulb(uint16_t shortaddr, bool hidden);
  bool isHueBulbHidden(uint16_t shortaddr) const ;
  Z_Data_Light & getLight(uint16_t shortaddr);

  // device is reachable
  void deviceWasReached(uint16_t shortaddr);

  // Timers
  void resetTimersForDevice(uint16_t shortaddr, uint16_t groupaddr, uint8_t category, uint16_t cluster = 0xFFFF, uint8_t endpoint = 0xFF);
  void setTimer(uint16_t shortaddr, uint16_t groupaddr, uint32_t wait_ms, uint16_t cluster, uint8_t endpoint, uint8_t category, uint32_t value, Z_DeviceTimer func);
  void queueTimer(uint16_t shortaddr, uint16_t groupaddr, uint32_t wait_ms, uint16_t cluster, uint8_t endpoint, uint8_t category, uint32_t value, Z_DeviceTimer func);
  void runTimer(void);

  // Append or clear attributes Json structure
  void jsonAppend(uint16_t shortaddr, const Z_attribute_list &attr_list);
  void jsonPublishFlush(uint16_t shortaddr);    // publish the json message and clear buffer
  bool jsonIsConflict(uint16_t shortaddr, const Z_attribute_list &attr_list) const;
  void jsonPublishNow(uint16_t shortaddr, Z_attribute_list &attr_list);

  // Iterator
  inline const LList<Z_Device> & getDevices(void) const { return _devices; }
  size_t devicesSize(void) const {
    return _devices.length();
  }
  Z_Device & devicesAt(size_t i) const;

  // Remove device from list
  void clearDeviceRouterInfo(void);           // reset all router flags, done just before ZbMap
  bool removeDevice(uint16_t shortaddr);

  // Mark data as 'dirty' and requiring to save in Flash
  void dirty(void);
  void clean(void);   // avoid writing to flash the last changes

  // Find device by name, can be short_addr, long_addr, number_in_array or name
  Z_Device & parseDeviceFromName(const char * param, uint16_t * parsed_shortaddr = nullptr);

  bool isTuyaProtocol(uint16_t shortaddr, uint8_t ep = 0) const;

private:
  LList<Z_Device>           _devices;     // list of devices
  LList<Z_Deferred>         _deferred;    // list of deferred calls
  uint32_t                  _saveTimer = 0;
  uint8_t                   _seqNumber = 0;     // global seqNumber if device is unknown

  //int32_t findShortAddrIdx(uint16_t shortaddr) const;
  // Create a new entry in the devices list - must be called if it is sure it does not already exist
  Z_Device & createDeviceEntry(uint16_t shortaddr, uint64_t longaddr = 0);
  void freeDeviceEntry(Z_Device *device);
};

/*********************************************************************************************\
 * Singleton variable
\*********************************************************************************************/
Z_Devices zigbee_devices = Z_Devices();

// Following device is used represent the unknown device, with all defaults
// Any find() function will not return Null, instead it will return this instance
Z_Device device_unk = Z_Device(BAD_SHORTADDR);

// Local coordinator information
uint64_t localIEEEAddr = 0;
uint16_t localShortAddr = 0;

#endif // USE_ZIGBEE
