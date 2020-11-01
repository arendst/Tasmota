/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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
  'O',      // 0x05 Z_Data_Type::Z_OnOff
  '\0',     // 0x06
  '\0',     // 0x07
  '\0',     // 0x08
  '\0',     // 0x09
  '\0',     // 0x0A
  '\0',     // 0x0B
  '\0',     // 0x0C
  '\0',     // 0x0D
  '\0',     // 0x0E
  'E',      // 0x05 Z_Data_Type::Z_Ext
  // '_' maps to 0xFF Z_Data_Type::Z_Device
};

class Z_Data_Set;
/*********************************************************************************************\
 * Device specific data, sensors...
\*********************************************************************************************/
class Z_Data {
public:
  Z_Data(Z_Data_Type type = Z_Data_Type::Z_Unknown, uint8_t endpoint = 0) : _type(type), _endpoint(endpoint), _config(0xF), _power(0) {}
  inline Z_Data_Type getType(void) const { return _type; }
  inline int8_t getConfig(void) const { return _config; }
  inline bool validConfig(void) const { return _config != 0xF; }
  inline void setConfig(int8_t config) { _config = config; }
  uint8_t getConfigByte(void) const { return ( ((uint8_t)_type) << 4) | ((_config & 0xF) & 0x0F); }

  inline uint8_t getEndpoint(void) const { return _endpoint; }

  void toAttributes(Z_attribute_list & attr_list, Z_Data_Type type) const;

  static const Z_Data_Type type = Z_Data_Type::Z_Unknown;
  static bool ConfigToZData(const char * config_str, Z_Data_Type * type, uint8_t * ep, uint8_t * config);

  static Z_Data_Type CharToDataType(char c);
  static char DataTypeToChar(Z_Data_Type t);

  friend class Z_Data_Set;
protected:
  Z_Data_Type _type;        // encoded on 4 bits, type of the device
  uint8_t _endpoint;    // source endpoint, or 0x00 if any endpoint
  uint8_t _config : 4;      // encoded on 4 bits, customize behavior
  uint8_t _power;       // power state if the type supports it
};

Z_Data_Type Z_Data::CharToDataType(char c) {
  if (c) {
    if (c == '_') {
      return Z_Data_Type::Z_Device;
    } else {
      for (uint32_t i=0; i<ARRAY_SIZE(Z_Data_Type_char); i++) {
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
    if (tt < ARRAY_SIZE(Z_Data_Type_char)) {
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
    Z_Data(Z_Data_Type::Z_OnOff, endpoint)
    {
      _config = 1;         // at least 1 OnOff
    }

  inline bool validPower(uint32_t relay = 0) const { return (_config > relay); }      // power is declared
  inline bool getPower(uint32_t relay = 0)  const { return bitRead(_power, relay); }
         void setPower(bool val, uint32_t relay = 0);

  void toAttributes(Z_attribute_list & attr_list, Z_Data_Type type) const;

  static const Z_Data_Type type = Z_Data_Type::Z_OnOff;
};


void Z_Data_OnOff::setPower(bool val, uint32_t relay) {
  if (relay < 8) {
    if (_config < relay) { _config = relay; }     // we update the number of valid relays
    bitWrite(_power, relay, val);
  }
}

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
  inline bool validHue(void)            const { return 0xFFFF != hue; }
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
class Z_Data_Alarm : public Z_Data {
public:
  Z_Data_Alarm(uint8_t endpoint = 0) :
    Z_Data(Z_Data_Type::Z_Alarm, endpoint),
    zone_type(0xFFFF)
    {}

  static const Z_Data_Type type = Z_Data_Type::Z_Alarm;

  inline bool validZoneType(void)   const { return 0xFFFF != zone_type; }

  inline uint16_t getZoneType(void) const { return zone_type; }

  inline void setZoneType(uint16_t _zone_type)  { zone_type = _zone_type; }

  // 4 bytes
  uint16_t               zone_type;       // mapped to the Zigbee standard
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


  template <class M>
  M & get(uint8_t ep = 0);

  template <class M>
  const M & find(uint8_t ep = 0) const;

  // check if the point is null, if so create a new object with the right sub-class
  template <class M>
  M & addIfNull(M & cur, uint8_t ep = 0);
};

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
    default:
      return *(Z_Data*)nullptr;
  }
}

template <class M>
M & Z_Data_Set::get(uint8_t ep) {
  M & m = (M&) find(M::type, ep);
  return addIfNull<M>(m, ep);
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

void Z_Data_OnOff::toAttributes(Z_attribute_list & attr_list, Z_Data_Type type) const {
  if (validPower())             { attr_list.addAttribute(PSTR("Power")).setUInt(getPower() ? 1 : 0); }
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
  bool                  hidden;
  bool                  reachable;
  // Light information for Hue integration integration, last known values

  // New version of device data handling
  Z_Data_Set            data;            // Linkedlist of device data per endpoint
  // other status
  uint8_t               lqi;            // lqi from last message, 0xFF means unknown
  uint8_t               batterypercent; // battery percentage (0..100), 0xFF means unknwon
  // power plug data-
  uint32_t              last_seen;      // Last seen time (epoch)

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
    hidden(false),
    reachable(false),
    data(),
    lqi(0xFF),
    batterypercent(0xFF),
    last_seen(0)
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
  static String dumpLightState(const Z_Device & device);
  String dumpDevice(uint32_t dump_mode, const Z_Device & device) const;
  static String dumpSingleDevice(uint32_t dump_mode, const Z_Device & device);
  int32_t deviceRestore(JsonParserObject json);

  // Hue support
  int8_t getHueBulbtype(uint16_t shortaddr) const ;
  void hideHueBulb(uint16_t shortaddr, bool hidden);
  bool isHueBulbHidden(uint16_t shortaddr) const ;
  Z_Data_Light & getLight(uint16_t shortaddr);

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
  size_t devicesSize(void) const {
    return _devices.length();
  }
  Z_Device & devicesAt(size_t i) const;

  // Remove device from list
  bool removeDevice(uint16_t shortaddr);

  // Mark data as 'dirty' and requiring to save in Flash
  void dirty(void);
  void clean(void);   // avoid writing to flash the last changes

  // Find device by name, can be short_addr, long_addr, number_in_array or name
  Z_Device & parseDeviceFromName(const char * param, bool short_must_be_known = false);


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
