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

#include <vector>

#ifndef ZIGBEE_SAVE_DELAY_SECONDS
#define ZIGBEE_SAVE_DELAY_SECONDS 2               // wait for 2s before saving Zigbee info
#endif
const uint16_t kZigbeeSaveDelaySeconds = ZIGBEE_SAVE_DELAY_SECONDS;    // wait for x seconds

/*********************************************************************************************\
 * Structures for Rules variables related to the last received message
\*********************************************************************************************/

typedef struct Z_LastMessageVars {
  uint16_t    device;               // device short address
  uint16_t    groupaddr;            // group address
  uint16_t    cluster;              // cluster id
  uint8_t     endpoint;             // source endpoint
} Z_LastMessageVars;

Z_LastMessageVars gZbLastMessage;

uint16_t Z_GetLastDevice(void) { return gZbLastMessage.device; }
uint16_t Z_GetLastGroup(void) { return gZbLastMessage.groupaddr; }
uint16_t Z_GetLastCluster(void) { return gZbLastMessage.cluster; }
uint8_t  Z_GetLastEndpoint(void) { return gZbLastMessage.endpoint; }

/*********************************************************************************************\
 * Structures for device configuration
\*********************************************************************************************/

const size_t endpoints_max = 8;         // we limit to 8 endpoints

class Z_Device {
public:

  uint64_t              longaddr;       // 0x00 means unspecified
  char *                manufacturerId;
  char *                modelId;
  char *                friendlyName;
  uint8_t               endpoints[endpoints_max];   // static array to limit memory consumption, list of endpoints until 0x00 or end of array
  // json buffer used for attribute reporting
  DynamicJsonBuffer    *json_buffer;
  JsonObject           *json;
  // sequence number for Zigbee frames
  uint16_t              shortaddr;      // unique key if not null, or unspecified if null
  uint8_t               seqNumber;
  // Light information for Hue integration integration, last known values
  uint8_t               zb_profile;     // profile of the device
    // high 4 bits is device type:
    //   0x0. = bulb
    //   0x1. = switch
    //   0x2. = motion sensor
    //   0x3. = other alarms
    //   0xE. = reserved for extension
    //   0xF. = unknown
    // For Bulb (0x0.)
    //   0x0N = number of channel for the bulb: 0-5
    //   0x08 = the device is hidden from Alexa
  // other status
  uint8_t               power;          // power state (boolean), MSB (0x80) stands for reachable
  uint8_t               colormode;      // 0x00: Hue/Sat, 0x01: XY, 0x02: CT | 0xFF not set, default 0x01
  uint8_t               dimmer;         // last Dimmer value: 0-254 | 0xFF not set, default 0x00
  uint8_t               sat;            // last Sat: 0..254 | 0xFF not set, default 0x00
  uint16_t              ct;             // last CT: 153-500 | 0xFFFF not set, default 200
  uint16_t              hue;            // last Hue: 0..359 | 0xFFFF not set, default 0
  uint16_t              x, y;           // last color [x,y] | 0xFFFF not set, default 0
  uint8_t               lqi;            // lqi from last message, 0xFF means unknown
  uint8_t               batterypercent; // battery percentage (0..100), 0xFF means unknwon
  // sensor data
  int16_t               temperature;    // temperature in 1/10th of Celsius, 0x8000 if unknown
  uint16_t              pressure;       // air pressure in hPa, 0xFFFF if unknown
  uint8_t               humidity;       // humidity in percent, 0..100, 0xFF if unknown

  // Constructor with all defaults
  Z_Device(uint16_t _shortaddr, uint64_t _longaddr = 0x00):
    longaddr(_longaddr),
    manufacturerId(nullptr),
    modelId(nullptr),
    friendlyName(nullptr),
    endpoints{ 0, 0, 0, 0, 0, 0, 0, 0 },
    json_buffer(nullptr),
    json(nullptr),
    shortaddr(_shortaddr),
    seqNumber(0),
    // Hue support
    zb_profile(0xFF),  // no profile
    power(0x02),       // 0x80 = reachable, 0x01 = power on, 0x02 = power unknown
    colormode(0xFF),
    dimmer(0xFF),
    sat(0xFF),
    ct(0xFFFF),
    hue(0xFFFF),
    x(0xFFFF),
    y(0xFFFF),
    lqi(0xFF),
    batterypercent(0xFF),
    temperature(-0x8000),
    pressure(0xFFFF),
    humidity(0xFF)
    { };

  inline bool valid(void)               const { return BAD_SHORTADDR != shortaddr; }    // is the device known, valid and found?

  inline bool validLongaddr(void)       const { return 0x0000 != longaddr; }
  inline bool validManufacturerId(void) const { return nullptr != manufacturerId; }
  inline bool validModelId(void)        const { return nullptr != modelId; }
  inline bool validFriendlyName(void)   const { return nullptr != friendlyName; }

  inline bool validPower(void)          const { return 0x00 == (power & 0x02); }
  inline bool validColormode(void)      const { return 0xFF != colormode; }
  inline bool validDimmer(void)         const { return 0xFF != dimmer; }
  inline bool validSat(void)            const { return 0xFF != sat; }
  inline bool validCT(void)             const { return 0xFFFF != ct; }
  inline bool validHue(void)            const { return 0xFFFF != hue; }
  inline bool validX(void)              const { return 0xFFFF != x; }
  inline bool validY(void)              const { return 0xFFFF != y; }

  inline bool validLqi(void)            const { return 0xFF != lqi; }
  inline bool validBatteryPercent(void) const { return 0xFF != batterypercent; }

  inline bool validTemperature(void)    const { return -0x8000 != temperature; }
  inline bool validPressure(void)       const { return 0xFFFF != pressure; }
  inline bool validHumidity(void)       const { return 0xFF != humidity; }

  inline void setReachable(bool reachable)    { bitWrite(power, 7, reachable); }
  inline bool getReachable(void)        const { return bitRead(power, 7); }
  inline void setPower(bool power_on)         { bitWrite(power, 0, power_on); bitWrite(power, 1, false); }
  inline bool getPower(void)            const { return bitRead(power, 0); }

  // If light, returns the number of channels, or 0xFF if unknown
  uint8_t getLightChannels(void)        const {
    if ((zb_profile & 0xF0) == 0x00) {
      return zb_profile & 0x07;
    }
    return 0xFF;
  }
};

/*********************************************************************************************\
 * Structures for deferred callbacks
\*********************************************************************************************/

typedef int32_t (*Z_DeviceTimer)(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value);

// Category for Deferred actions, this allows to selectively remove active deferred or update them
typedef enum Z_Def_Category {
  Z_CAT_NONE = 0,             // no category, it will happen anyways
  Z_CAT_READ_ATTR,            // Attribute reporting, either READ_ATTRIBUTE or REPORT_ATTRIBUTE, we coalesce all attributes reported if we can
  Z_CAT_VIRTUAL_OCCUPANCY,    // Creation of a virtual attribute, typically after a time-out. Ex: Aqara presence sensor
  Z_CAT_REACHABILITY,         // timer set to measure reachability of device, i.e. if we don't get an answer after 1s, it is marked as unreachable (for Alexa)
  Z_CAT_READ_0006,            // Read 0x0006 cluster
  Z_CAT_READ_0008,            // Read 0x0008 cluster
  Z_CAT_READ_0102,            // Read 0x0300 cluster
  Z_CAT_READ_0300,            // Read 0x0300 cluster
} Z_Def_Category;

const uint32_t Z_CAT_REACHABILITY_TIMEOUT = 1000;     // 1000 ms or 1s

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
  Z_Devices() {};

  // Probe the existence of device keys
  // Results:
  // - 0x0000 = not found
  // - BAD_SHORTADDR = bad parameter
  // - 0x<shortaddr> = the device's short address
  uint16_t isKnownLongAddr(uint64_t  longaddr) const;
  uint16_t isKnownIndex(uint32_t index) const;
  uint16_t isKnownFriendlyName(const char * name) const;
  
  const Z_Device & findShortAddr(uint16_t shortaddr) const;
  Z_Device & getShortAddr(uint16_t shortaddr);   // find Device from shortAddr, creates it if does not exist
  const Z_Device & getShortAddrConst(uint16_t shortaddr) const ;   // find Device from shortAddr, creates it if does not exist
  Z_Device & getLongAddr(uint64_t longaddr);     // find Device from shortAddr, creates it if does not exist

  int32_t findLongAddr(uint64_t longaddr) const;
  int32_t findFriendlyName(const char * name) const;
  uint64_t getDeviceLongAddr(uint16_t shortaddr) const;

  uint8_t findFirstEndpoint(uint16_t shortaddr) const;

  // Add new device, provide ShortAddr and optional longAddr
  // If it is already registered, update information, otherwise create the entry
  void updateDevice(uint16_t shortaddr, uint64_t longaddr = 0);

  // Add an endpoint to a device
  void addEndpoint(uint16_t shortaddr, uint8_t endpoint);
  void clearEndpoints(uint16_t shortaddr);
  uint32_t countEndpoints(uint16_t shortaddr) const;    // return the number of known endpoints (0 if unknown)

  void setManufId(uint16_t shortaddr, const char * str);
  void setModelId(uint16_t shortaddr, const char * str);
  void setFriendlyName(uint16_t shortaddr, const char * str);
  inline const char * getFriendlyName(uint16_t shortaddr) const {
    return findShortAddr(shortaddr).friendlyName;
  }
  inline const char * getModelId(uint16_t shortaddr) const {
    return findShortAddr(shortaddr).modelId;
  }
  inline const char * getManufacturerId(uint16_t shortaddr) const{
    return findShortAddr(shortaddr).manufacturerId;
  }

  void setReachable(uint16_t shortaddr, bool reachable);
  void setLQI(uint16_t shortaddr, uint8_t lqi);
  // uint8_t getLQI(uint16_t shortaddr) const;
  void setBatteryPercent(uint16_t shortaddr, uint8_t bp);
  uint8_t getBatteryPercent(uint16_t shortaddr) const;

  // get next sequence number for (increment at each all)
  uint8_t getNextSeqNumber(uint16_t shortaddr);

  // Dump json
  String dumpLightState(uint16_t shortaddr) const;
  String dump(uint32_t dump_mode, uint16_t status_shortaddr = 0) const;
  int32_t deviceRestore(const JsonObject &json);

  // General Zigbee device profile support
  void setZbProfile(uint16_t shortaddr, uint8_t zb_profile);
  uint8_t getZbProfile(uint16_t shortaddr) const ;

  // Hue support
  void setHueBulbtype(uint16_t shortaddr, int8_t bulbtype);
  int8_t getHueBulbtype(uint16_t shortaddr) const ;
  void hideHueBulb(uint16_t shortaddr, bool hidden);
  bool isHueBulbHidden(uint16_t shortaddr) const ;

  // Timers
  void resetTimersForDevice(uint16_t shortaddr, uint16_t groupaddr, uint8_t category);
  void setTimer(uint16_t shortaddr, uint16_t groupaddr, uint32_t wait_ms, uint16_t cluster, uint8_t endpoint, uint8_t category, uint32_t value, Z_DeviceTimer func);
  void runTimer(void);

  // Append or clear attributes Json structure
  void jsonClear(uint16_t shortaddr);
  void jsonAppend(uint16_t shortaddr, const JsonObject &values);
  const JsonObject *jsonGet(uint16_t shortaddr);
  void jsonPublishFlush(uint16_t shortaddr);    // publish the json message and clear buffer
  bool jsonIsConflict(uint16_t shortaddr, const JsonObject &values);
  void jsonPublishNow(uint16_t shortaddr, JsonObject &values);

  // Iterator
  size_t devicesSize(void) const {
    return _devices.size();
  }
  const Z_Device &devicesAt(size_t i) const {
    return *(_devices.at(i));
  }

  // Remove device from list
  bool removeDevice(uint16_t shortaddr);

  // Mark data as 'dirty' and requiring to save in Flash
  void dirty(void);
  void clean(void);   // avoid writing to flash the last changes
  void shrinkToFit(uint16_t shortaddr);

  // Find device by name, can be short_addr, long_addr, number_in_array or name
  uint16_t parseDeviceParam(const char * param, bool short_must_be_known = false) const;

private:
  std::vector<Z_Device*>    _devices = {};
  std::vector<Z_Deferred>   _deferred = {};   // list of deferred calls
  uint32_t                  _saveTimer = 0;
  uint8_t                   _seqNumber = 0;     // global seqNumber if device is unknown

  // Following device is used represent the unknown device, with all defaults
  // Any find() function will not return Null, instead it will return this instance
  const Z_Device device_unk = Z_Device(BAD_SHORTADDR);

  template < typename T>
  static bool findInVector(const std::vector<T>  & vecOfElements, const T  & element);

  int32_t findShortAddrIdx(uint16_t shortaddr) const;
  // Create a new entry in the devices list - must be called if it is sure it does not already exist
  Z_Device & createDeviceEntry(uint16_t shortaddr, uint64_t longaddr = 0);
  void freeDeviceEntry(Z_Device *device);

  void setStringAttribute(char*& attr, const char * str);

  void updateZbProfile(uint16_t shortaddr);
};

/*********************************************************************************************\
 * Singleton variable
\*********************************************************************************************/
Z_Devices zigbee_devices = Z_Devices();

// Local coordinator information
uint64_t localIEEEAddr = 0;
uint16_t localShortAddr = 0;

/*********************************************************************************************\
 * Implementation
\*********************************************************************************************/

// https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/
template < typename T>
bool Z_Devices::findInVector(const std::vector<T>  & vecOfElements, const T  & element) {
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end()) {
		return true;
	} else {
		return false;
	}
}

//
// Create a new Z_Device entry in _devices. Only to be called if you are sure that no
// entry with same shortaddr or longaddr exists.
//
Z_Device & Z_Devices::createDeviceEntry(uint16_t shortaddr, uint64_t longaddr) {
  if ((BAD_SHORTADDR == shortaddr) && !longaddr) { return (Z_Device&) device_unk; }      // it is not legal to create this entry
  Z_Device * device_alloc = new Z_Device(shortaddr, longaddr);

  device_alloc->json_buffer = new DynamicJsonBuffer(16);
  _devices.push_back(device_alloc);
  dirty();
  return *(_devices.back());
}

void Z_Devices::freeDeviceEntry(Z_Device *device) {
  if (device->manufacturerId) { free(device->manufacturerId); }
  if (device->modelId) { free(device->modelId); }
  if (device->friendlyName) { free(device->friendlyName); }
  free(device);
}

//
// Scan all devices to find a corresponding shortaddr
// Looks info device.shortaddr entry
// In:
//    shortaddr (not BAD_SHORTADDR)
// Out:
//    index in _devices of entry, -1 if not found
//
int32_t Z_Devices::findShortAddrIdx(uint16_t shortaddr) const {
  if (BAD_SHORTADDR == shortaddr) { return -1; }              // does not make sense to look for BAD_SHORTADDR shortaddr (broadcast)
  int32_t found = 0;
  for (auto &elem : _devices) {
    if (elem->shortaddr == shortaddr) { return found; }
    found++;
  }
  return -1;
}
const Z_Device & Z_Devices::findShortAddr(uint16_t shortaddr) const {
  for (auto &elem : _devices) {
    if (elem->shortaddr == shortaddr) { return *elem; }
  }
  return device_unk;
}
//
// Scan all devices to find a corresponding longaddr
// Looks info device.longaddr entry
// In:
//    longaddr (non null)
// Out:
//    index in _devices of entry, -1 if not found
//
int32_t Z_Devices::findLongAddr(uint64_t longaddr) const {
  if (!longaddr) { return -1; }
  int32_t found = 0;
  for (auto &elem : _devices) {
    if (elem->longaddr == longaddr) { return found; }
    found++;
  }
  return -1;
}
//
// Scan all devices to find a corresponding friendlyNme
// Looks info device.friendlyName entry
// In:
//    friendlyName (null terminated, should not be empty)
// Out:
//    index in _devices of entry, -1 if not found
//
int32_t Z_Devices::findFriendlyName(const char * name) const {
  if (!name) { return -1; }              // if pointer is null
  size_t name_len = strlen(name);
  int32_t found = 0;
  if (name_len) {
    for (auto &elem : _devices) {
      if (elem->friendlyName) {
        if (strcasecmp(elem->friendlyName, name) == 0) { return found; }
      }
      found++;
    }
  }
  return -1;
}

uint16_t Z_Devices::isKnownLongAddr(uint64_t longaddr) const {
  int32_t found = findLongAddr(longaddr);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    return device.shortaddr;    // can be zero, if not yet registered
  } else {
    return BAD_SHORTADDR;
  }
}

uint16_t Z_Devices::isKnownIndex(uint32_t index) const {
  if (index < devicesSize()) {
    const Z_Device & device = devicesAt(index);
    return device.shortaddr;
  } else {
    return BAD_SHORTADDR;
  }
}

uint16_t Z_Devices::isKnownFriendlyName(const char * name) const {
  if ((!name) || (0 == strlen(name))) { return BAD_SHORTADDR; }         // Error
  int32_t found = findFriendlyName(name);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    return device.shortaddr;    // can be zero, if not yet registered
  } else {
    return BAD_SHORTADDR;
  }
}

uint64_t Z_Devices::getDeviceLongAddr(uint16_t shortaddr) const {
  return getShortAddrConst(shortaddr).longaddr;     // if unknown, it reverts to the Unknown device and longaddr is 0x00
}

//
// We have a seen a shortaddr on the network, get the corresponding device object
//
Z_Device & Z_Devices::getShortAddr(uint16_t shortaddr) {
  if (BAD_SHORTADDR == shortaddr) { return (Z_Device&) device_unk; }   // this is not legal
  int32_t found = findShortAddrIdx(shortaddr);
  if (found >= 0) {
    return *(_devices[found]);
  }
  //Serial.printf("Device entry created for shortaddr = 0x%02X, found = %d\n", shortaddr, found);
  return createDeviceEntry(shortaddr, 0);
}
// Same version but Const
const Z_Device & Z_Devices::getShortAddrConst(uint16_t shortaddr) const {
  int32_t found = findShortAddrIdx(shortaddr);
  if (found >= 0) {
    return *(_devices[found]);
  }
  return device_unk;
}

// find the Device object by its longaddr (unique key if not null)
Z_Device & Z_Devices::getLongAddr(uint64_t longaddr) {
  if (!longaddr) { return (Z_Device&) device_unk; }
  int32_t found = findLongAddr(longaddr);
  if (found > 0) {
    return *(_devices[found]);
  }
  return createDeviceEntry(0, longaddr);
}

// Remove device from list, return true if it was known, false if it was not recorded
bool Z_Devices::removeDevice(uint16_t shortaddr) {
  int32_t found = findShortAddrIdx(shortaddr);
  if (found >= 0) {
    freeDeviceEntry(_devices.at(found));
    _devices.erase(_devices.begin() + found);
    dirty();
    return true;
  }
  return false;
}

//
// We have just seen a device on the network, update the info based on short/long addr
// In:
//    shortaddr
//    longaddr (both can't be null at the same time)
void Z_Devices::updateDevice(uint16_t shortaddr, uint64_t longaddr) {
  int32_t s_found = findShortAddrIdx(shortaddr);       // is there already a shortaddr entry
  int32_t l_found = findLongAddr(longaddr);         // is there already a longaddr entry

  if ((s_found >= 0) && (l_found >= 0)) {           // both shortaddr and longaddr are already registered
    if (s_found == l_found) {
    } else {                                        // they don't match
      // the device with longaddr got a new shortaddr
      _devices[l_found]->shortaddr = shortaddr;      // update the shortaddr corresponding to the longaddr
      // erase the previous shortaddr
      freeDeviceEntry(_devices.at(s_found));
      _devices.erase(_devices.begin() + s_found);
      dirty();
    }
  } else if (s_found >= 0) {
    // shortaddr already exists but longaddr not
    // add the longaddr to the entry
    _devices[s_found]->longaddr = longaddr;
    dirty();
  } else if (l_found >= 0) {
    // longaddr entry exists, update shortaddr
    _devices[l_found]->shortaddr = shortaddr;
    dirty();
  } else {
    // neither short/lonf addr are found.
    if ((BAD_SHORTADDR != shortaddr) || longaddr) {
      createDeviceEntry(shortaddr, longaddr);
    }
  }
}

//
// Clear all endpoints
//
void Z_Devices::clearEndpoints(uint16_t shortaddr) {
  Z_Device &device = getShortAddr(shortaddr);
  for (uint32_t i = 0; i < endpoints_max; i++) {
    device.endpoints[i] = 0;
    // no dirty here because it doesn't make sense to store it, does it?
  }
}

//
// Add an endpoint to a shortaddr
//
void Z_Devices::addEndpoint(uint16_t shortaddr, uint8_t endpoint) {
  if (0x00 == endpoint) { return; }
  Z_Device &device = getShortAddr(shortaddr);

  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (endpoint == device.endpoints[i]) {
      return;     // endpoint already there
    }
    if (0 == device.endpoints[i]) {
      device.endpoints[i] = endpoint;
      dirty();
      return;
    }
  }
}

//
// Count the number of known endpoints
//
uint32_t Z_Devices::countEndpoints(uint16_t shortaddr) const {
  uint32_t count_ep = 0;
  int32_t found = findShortAddrIdx(shortaddr);
  if (found < 0)  return 0;     // avoid creating an entry if the device was never seen
  const Z_Device &device = devicesAt(found);

  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (0 != device.endpoints[i]) {
      count_ep++;
    }
  }
  return count_ep;
}

// Find the first endpoint of the device
uint8_t Z_Devices::findFirstEndpoint(uint16_t shortaddr) const {
  // When in router of end-device mode, the coordinator was not probed, in this case always talk to endpoint 1
  if (0x0000 == shortaddr) { return 1; }
  return findShortAddr(shortaddr).endpoints[0];   // returns 0x00 if no endpoint
}

void Z_Devices::setStringAttribute(char*& attr, const char * str) {
  if (nullptr == str)  { return; }                    // ignore a null parameter
  size_t str_len = strlen(str);

  if ((nullptr == attr) && (0 == str_len)) { return; } // if both empty, don't do anything
  if (attr) {
    // we already have a value
    if (strcmp(attr, str) != 0) {
      // new value
      free(attr);      // free previous value
      attr = nullptr;
    } else {
      return;        // same value, don't change anything
    }
  }
  if (str_len) {
    attr = (char*) malloc(str_len + 1);
    strlcpy(attr, str, str_len + 1);
  }
  dirty();
}

//
// Sets the ManufId for a device.
// No action taken if the device does not exist.
// Inputs:
// - shortaddr: 16-bits short address of the device. No action taken if the device is unknown
// - str:       string pointer, if nullptr it is considered as empty string
// Impact:
// - Any actual change in ManufId (i.e. setting a different value) triggers a `dirty()` and saving to Flash
//
void Z_Devices::setManufId(uint16_t shortaddr, const char * str) {
  setStringAttribute(getShortAddr(shortaddr).manufacturerId, str);
}

void Z_Devices::setModelId(uint16_t shortaddr, const char * str) {
  setStringAttribute(getShortAddr(shortaddr).modelId, str);
}

void Z_Devices::setFriendlyName(uint16_t shortaddr, const char * str) {
  setStringAttribute(getShortAddr(shortaddr).friendlyName, str);
}


void Z_Devices::setReachable(uint16_t shortaddr, bool reachable) {
  getShortAddr(shortaddr).setReachable(reachable);
}

void Z_Devices::setLQI(uint16_t shortaddr, uint8_t lqi) {
  if (shortaddr == localShortAddr) { return; }
  getShortAddr(shortaddr).lqi = lqi;
}

void Z_Devices::setBatteryPercent(uint16_t shortaddr, uint8_t bp) {
  getShortAddr(shortaddr).batterypercent = bp;
}

// get the next sequance number for the device, or use the global seq number if device is unknown
uint8_t Z_Devices::getNextSeqNumber(uint16_t shortaddr) {
  int32_t short_found = findShortAddrIdx(shortaddr);
  if (short_found >= 0) {
    Z_Device &device = getShortAddr(shortaddr);
    device.seqNumber += 1;
    return device.seqNumber;
  } else {
    _seqNumber += 1;
    return _seqNumber;
  }
}

// General Zigbee device profile support
void Z_Devices::setZbProfile(uint16_t shortaddr, uint8_t zb_profile) {
  Z_Device &device = getShortAddr(shortaddr);
  if (zb_profile != device.zb_profile) {
    device.zb_profile = zb_profile;
    updateZbProfile(shortaddr);
    dirty();
  }
}

// Do all the required action when a profile is changed
void Z_Devices::updateZbProfile(uint16_t shortaddr) {
  Z_Device &device = getShortAddr(shortaddr);
  uint8_t zb_profile = device.zb_profile;
  if (0xFF == zb_profile) { return; }

  switch (zb_profile & 0xF0) {
  case 0x00:      // bulb profile
    {
      uint32_t channels = zb_profile & 0x07;
      // depending on the bulb type, the default parameters from unknown to credible defaults
      // if (!device.validPower()) { device.setPower(false); }
      // if (1 <= channels) {
      //   if (0xFF == device.dimmer) { device.dimmer = 0; }
      // }
      // if (3 <= channels) {
      //   if (0xFF == device.sat) { device.sat = 0; }
      //   if (0xFFFF == device.hue) { device.hue = 0; }
      //   if (0xFFFF == device.x) { device.x = 0; }
      //   if (0xFFFF == device.y) { device.y = 0; }
      //   if (0xFF == device.colormode) { device.colormode = 0; }   // HueSat mode
      // }
      // if ((2 == channels) || (5 == channels)) {
      //   if (0xFFFF == device.ct) { device.ct = 200; }
      //   if (0xFF == device.colormode) { device.colormode = 2; }   // CT mode
      // }
    }
    break;
  }
}

// Returns the device profile or 0xFF if the device or profile is unknown
uint8_t Z_Devices::getZbProfile(uint16_t shortaddr) const {
  return findShortAddr(shortaddr).zb_profile;
}

// Hue support
void Z_Devices::setHueBulbtype(uint16_t shortaddr, int8_t bulbtype) {
  uint8_t zb_profile = (0 > bulbtype) ? 0xFF : (bulbtype & 0x07);
  setZbProfile(shortaddr, zb_profile);
}

int8_t Z_Devices::getHueBulbtype(uint16_t shortaddr) const {
  uint8_t zb_profile = getZbProfile(shortaddr);
  if (0x00 == (zb_profile & 0xF0)) {
    return (zb_profile & 0x07);
  } else {
    // not a bulb
    return -1;
  }
}

void Z_Devices::hideHueBulb(uint16_t shortaddr, bool hidden) {
  uint8_t hue_hidden_flag = hidden ? 0x08 : 0x00;
  
  Z_Device &device = getShortAddr(shortaddr);
  if (0x00 == (device.zb_profile & 0xF0)) {
    // bulb type
    // set bit 3 accordingly
    if (hue_hidden_flag != (device.zb_profile & 0x08)) {
      device.zb_profile = (device.zb_profile & 0xF7) | hue_hidden_flag;
      dirty();
    }
  }
}
// true if device is not knwon or not a bulb - it wouldn't make sense to publish a non-bulb
bool Z_Devices::isHueBulbHidden(uint16_t shortaddr) const {
  int32_t found = findShortAddrIdx(shortaddr);
  if (found >= 0) {
    uint8_t zb_profile = _devices[found]->zb_profile;
    if (0x00 == (zb_profile & 0xF0)) {
      // bulb type
      return (zb_profile & 0x08) ? true : false;
    }
  }
  return true;      // Fallback - Device is considered as hidden
}

// Deferred actions
// Parse for a specific category, of all deferred for a device if category == 0xFF
void Z_Devices::resetTimersForDevice(uint16_t shortaddr, uint16_t groupaddr, uint8_t category) {
  // iterate the list of deferred, and remove any linked to the shortaddr
  for (auto it = _deferred.begin(); it != _deferred.end(); it++) {
    // Notice that the iterator is decremented after it is passed
		// to erase() but before erase() is executed
    // see https://www.techiedelight.com/remove-elements-vector-inside-loop-cpp/
    if ((it->shortaddr == shortaddr) && (it->groupaddr == groupaddr)) {
      if ((0xFF == category) || (it->category == category)) {
        _deferred.erase(it--);
      }
    }
  }
}

// Set timer for a specific device
void Z_Devices::setTimer(uint16_t shortaddr, uint16_t groupaddr, uint32_t wait_ms, uint16_t cluster, uint8_t endpoint, uint8_t category, uint32_t value, Z_DeviceTimer func) {
  // First we remove any existing timer for same device in same category, except for category=0x00 (they need to happen anyway)
  if (category) {     // if category == 0, we leave all previous
    resetTimersForDevice(shortaddr, groupaddr, category);    // remove any cluster
  }

  // Now create the new timer
  Z_Deferred deferred = { wait_ms + millis(),   // timer
                          shortaddr,
                          groupaddr,
                          cluster,
                          endpoint,
                          category,
                          value,
                          func };
  _deferred.push_back(deferred);
}

// Run timer at each tick
// WARNING: don't set a new timer within a running timer, this causes memory corruption
void Z_Devices::runTimer(void) {
  // visit all timers
  for (auto it = _deferred.begin(); it != _deferred.end(); it++) {
    Z_Deferred &defer = *it;

    uint32_t timer = defer.timer;
    if (TimeReached(timer)) {
      (*defer.func)(defer.shortaddr, defer.groupaddr, defer.cluster, defer.endpoint, defer.value);
      _deferred.erase(it--);    // remove from list
    }
  }

  // check if we need to save to Flash
  if ((_saveTimer) && TimeReached(_saveTimer)) {
    saveZigbeeDevices();
    _saveTimer = 0;
  }
}

// Clear the JSON buffer for coalesced and deferred attributes
void Z_Devices::jsonClear(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  device.json = nullptr;
  device.json_buffer->clear();
}

// Copy JSON from one object to another, this helps preserving the order of attributes
void CopyJsonVariant(JsonObject &to, const String &key, const JsonVariant &val) {
  // first remove the potentially existing key in the target JSON, so new adds will be at the end of the list
  to.remove(key);    // force remove to have metadata like LinkQuality at the end

  if (val.is<char*>()) {
    const char * sval = val.as<char*>();    // using char* forces a copy, and also captures 'null' values
    to.set(key, (char*) sval);
  } else if (val.is<JsonArray>()) {
    JsonArray &nested_arr = to.createNestedArray(key);
    CopyJsonArray(nested_arr, val.as<JsonArray>());   // deep copy
  } else if (val.is<JsonObject>()) {
    JsonObject &nested_obj = to.createNestedObject(key);
    CopyJsonObject(nested_obj, val.as<JsonObject>()); // deep copy
  } else {
    to.set(key, val);                     // general case for non array, object or string
  }
}

// Shallow copy of array, we skip any sub-array or sub-object. It may be added in the future
void CopyJsonArray(JsonArray &to, const JsonArray &arr) {
  for (auto v : arr) {
    if (v.is<char*>()) {
      String sval = v.as<String>();       // force a copy of the String value
      to.add(sval);
    } else if (v.is<JsonArray>()) {
    } else if (v.is<JsonObject>()) {
    } else {
      to.add(v);
    }
  }
}

// Deep copy of object
void CopyJsonObject(JsonObject &to, const JsonObject &from) {
  for (auto kv : from) {
    String key_string = kv.key;
    JsonVariant &val = kv.value;

    CopyJsonVariant(to, key_string, val);
  }
}

// does the new payload conflicts with the existing payload, i.e. values would be overwritten
// true - one attribute (except LinkQuality) woudl be lost, there is conflict
// false - new attributes can be safely added
bool Z_Devices::jsonIsConflict(uint16_t shortaddr, const JsonObject &values) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&values == nullptr) { return false; }

  if (nullptr == device.json) {
    return false;                                           // if no previous value, no conflict
  }

  // compare groups
  // Special case for group addresses. Group attribute is only present if the target
  // address is a group address, so just comparing attributes will not work.
  // Eg: if the first packet has no group attribute, and the second does, conflict would not be detected
  // Here we explicitly compute the group address of both messages, and compare them. No group means group=0x0000
  // (we use the property of an missing attribute returning 0)
  // (note: we use .get() here which is case-sensitive. We know however that the attribute was set with the exact syntax D_CMND_ZIGBEE_GROUP, so we don't need a case-insensitive get())
  uint16_t group1 = device.json->get<unsigned int>(D_CMND_ZIGBEE_GROUP);
  uint16_t group2 = values.get<unsigned int>(D_CMND_ZIGBEE_GROUP);
  if (group1 != group2) {
    return true;      // if group addresses differ, then conflict
  }

  // parse all other parameters
  for (auto kv : values) {
    String key_string = kv.key;

    if (0 == strcasecmp_P(kv.key, PSTR(D_CMND_ZIGBEE_GROUP))) {
      // ignore group, it was handled already
    } else if (0 == strcasecmp_P(kv.key, PSTR(D_CMND_ZIGBEE_ENDPOINT))) {
      // attribute "Endpoint" or "Group"
      if (device.json->containsKey(kv.key)) {
        if (kv.value.as<unsigned int>() != device.json->get<unsigned int>(kv.key)) {
          return true;
        }
      }
    } else if (strcasecmp_P(kv.key, PSTR(D_CMND_ZIGBEE_LINKQUALITY))) {  // exception = ignore duplicates for LinkQuality
      if (device.json->containsKey(kv.key)) {
        return true;          // conflict!
      }
    }
  }
  return false;
}

void Z_Devices::jsonAppend(uint16_t shortaddr, const JsonObject &values) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&values == nullptr) { return; }

  if (nullptr == device.json) {
    device.json = &(device.json_buffer->createObject());
  }
  // Prepend Device, will be removed later if redundant
  char sa[8];
  snprintf_P(sa, sizeof(sa), PSTR("0x%04X"), shortaddr);
  device.json->set(F(D_JSON_ZIGBEE_DEVICE), sa);
  // Prepend Friendly Name if it has one
  const char * fname = zigbee_devices.getFriendlyName(shortaddr);
  if (fname) {
    device.json->set(F(D_JSON_ZIGBEE_NAME), (char*) fname);   // (char*) forces ArduinoJson to make a copy of the cstring
  }

  // copy all values from 'values' to 'json'
  CopyJsonObject(*device.json, values);
}

const JsonObject *Z_Devices::jsonGet(uint16_t shortaddr) {
  return getShortAddr(shortaddr).json;
}

void Z_Devices::jsonPublishFlush(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (!device.valid()) { return; }                 // safeguard
  JsonObject & json = *device.json;
  if (&json == nullptr) { return; }                // abort if nothing in buffer

  const char * fname = zigbee_devices.getFriendlyName(shortaddr);
  bool use_fname = (Settings.flag4.zigbee_use_names) && (fname);    // should we replace shortaddr with friendlyname?

  // save parameters is global variables to be used by Rules
  gZbLastMessage.device = shortaddr;                // %zbdevice%
  gZbLastMessage.groupaddr = json[F(D_CMND_ZIGBEE_GROUP)];      // %zbgroup%
  gZbLastMessage.cluster = json[F(D_CMND_ZIGBEE_CLUSTER)];      // %zbcluster%
  gZbLastMessage.endpoint = json[F(D_CMND_ZIGBEE_ENDPOINT)];    // %zbendpoint%

  // dump json in string
  String msg = "";
  json.printTo(msg);
  zigbee_devices.jsonClear(shortaddr);

  if (use_fname) {
    if (Settings.flag4.remove_zbreceived) {
      Response_P(PSTR("{\"%s\":%s}"), fname, msg.c_str());
    } else {
      Response_P(PSTR("{\"" D_JSON_ZIGBEE_RECEIVED "\":{\"%s\":%s}}"), fname, msg.c_str());
    }
  } else {
    if (Settings.flag4.remove_zbreceived) {
      Response_P(PSTR("{\"0x%04X\":%s}"), shortaddr, msg.c_str());
    } else {
      Response_P(PSTR("{\"" D_JSON_ZIGBEE_RECEIVED "\":{\"0x%04X\":%s}}"), shortaddr, msg.c_str());
    }
  }
  if (Settings.flag4.zigbee_distinct_topics) {
    char subtopic[16];
    snprintf_P(subtopic, sizeof(subtopic), PSTR("%04X/" D_RSLT_SENSOR), shortaddr);
    MqttPublishPrefixTopic_P(TELE, subtopic, Settings.flag.mqtt_sensor_retain);
  } else {
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  }
  XdrvRulesProcess();     // apply rules
}

void Z_Devices::jsonPublishNow(uint16_t shortaddr, JsonObject & values) {
  jsonPublishFlush(shortaddr);    // flush any previous buffer
  jsonAppend(shortaddr, values);
  jsonPublishFlush(shortaddr);    // publish now
}

void Z_Devices::dirty(void) {
  _saveTimer = kZigbeeSaveDelaySeconds * 1000 + millis();
}
void Z_Devices::clean(void) {
  _saveTimer = 0;
}

// Parse the command parameters for either:
// - a short address starting with "0x", example: 0x1234
// - a long address starting with "0x", example: 0x7CB03EBB0A0292DD
// - a number 0..99, the index number in ZigbeeStatus
// - a friendly name, between quotes, example: "Room_Temp"
uint16_t Z_Devices::parseDeviceParam(const char * param, bool short_must_be_known) const {
  if (nullptr == param) { return 0; }
  size_t param_len = strlen(param);
  char dataBuf[param_len + 1];
  strcpy(dataBuf, param);
  RemoveSpace(dataBuf);
  uint16_t shortaddr = BAD_SHORTADDR;    // start with unknown

  if (strlen(dataBuf) < 4) {
    // simple number 0..99
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 99)) {
      shortaddr = zigbee_devices.isKnownIndex(XdrvMailbox.payload - 1);
    }
  } else if ((dataBuf[0] == '0') && ((dataBuf[1] == 'x') || (dataBuf[1] == 'X'))) {
    // starts with 0x
    if (strlen(dataBuf) < 18) {
      // expect a short address
      shortaddr = strtoull(dataBuf, nullptr, 0);
      if (short_must_be_known) {
        shortaddr = zigbee_devices.findShortAddr(shortaddr).shortaddr;   // if not found, it reverts to the unknown_device with address BAD_SHORTADDR
      }
      // else we don't check if it's already registered to force unregistered devices
    } else {
      // expect a long address
      uint64_t longaddr = strtoull(dataBuf, nullptr, 0);
      shortaddr = zigbee_devices.isKnownLongAddr(longaddr);
    }
  } else {
    // expect a Friendly Name
    shortaddr = zigbee_devices.isKnownFriendlyName(dataBuf);
  }

  return shortaddr;
}

// Display the tracked status for a light
String Z_Devices::dumpLightState(uint16_t shortaddr) const {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  char hex[8];

  int32_t found = findShortAddrIdx(shortaddr);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    const char * fname = getFriendlyName(shortaddr);

    bool use_fname = (Settings.flag4.zigbee_use_names) && (fname);    // should we replace shortaddr with friendlyname?

    snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), shortaddr);

    JsonObject& dev = use_fname ? json.createNestedObject((char*) fname)   // casting (char*) forces a copy
                                : json.createNestedObject(hex);
    if (use_fname) {
      dev[F(D_JSON_ZIGBEE_DEVICE)] = hex;
    } else if (fname) {
      dev[F(D_JSON_ZIGBEE_NAME)] = (char*) fname;
    }

    // expose the last known status of the bulb, for Hue integration
    dev[F(D_JSON_ZIGBEE_LIGHT)] = getHueBulbtype(shortaddr);   // sign extend, 0xFF changed as -1
    // dump all known values
    dev[F("Reachable")] = device.getReachable();   // TODO TODO
    if (device.validPower())        { dev[F("Power")]     = device.getPower(); }
    if (device.validDimmer())       { dev[F("Dimmer")]    = device.dimmer; }
    if (device.validColormode())    { dev[F("Colormode")] = device.colormode; }
    if (device.validCT())           { dev[F("CT")]        = device.ct; }
    if (device.validSat())          { dev[F("Sat")]       = device.sat; }
    if (device.validHue())          { dev[F("Hue")]       = device.hue; }
    if (device.validX())            { dev[F("X")]         = device.x; }
    if (device.validY())            { dev[F("Y")]         = device.y; }
  }

  String payload = "";
  payload.reserve(200);
  json.printTo(payload);
  return payload;
}

// Dump the internal memory of Zigbee devices
// Mode = 1: simple dump of devices addresses
// Mode = 2: simple dump of devices addresses and names
// Mode = 3: Mode 2 + also dump the endpoints, profiles and clusters
String Z_Devices::dump(uint32_t dump_mode, uint16_t status_shortaddr) const {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.createArray();
  JsonArray& devices = json;

  for (std::vector<Z_Device*>::const_iterator it = _devices.begin(); it != _devices.end(); ++it) {
    const Z_Device &device = **it;
    uint16_t shortaddr = device.shortaddr;
    char hex[22];

    // ignore non-current device, if device specified
    if ((BAD_SHORTADDR != status_shortaddr) && (status_shortaddr != shortaddr)) { continue; }

    JsonObject& dev = devices.createNestedObject();

    snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), shortaddr);
    dev[F(D_JSON_ZIGBEE_DEVICE)] = hex;

    if (device.friendlyName > 0) {
      dev[F(D_JSON_ZIGBEE_NAME)] = (char*) device.friendlyName;
    }

    if (2 <= dump_mode) {
      hex[0] = '0';   // prefix with '0x'
      hex[1] = 'x';
      Uint64toHex(device.longaddr, &hex[2], 64);
      dev[F("IEEEAddr")] = hex;
      if (device.modelId) {
        dev[F(D_JSON_MODEL D_JSON_ID)] = device.modelId;
      }
      int8_t bulbtype = getHueBulbtype(shortaddr);
      if (bulbtype >= 0) {
        dev[F(D_JSON_ZIGBEE_LIGHT)] = bulbtype;   // sign extend, 0xFF changed as -1
      }
      if (device.manufacturerId) {
        dev[F("Manufacturer")] = device.manufacturerId;
      }
      JsonArray& dev_endpoints = dev.createNestedArray(F("Endpoints"));
      for (uint32_t i = 0; i < endpoints_max; i++) {
        uint8_t endpoint = device.endpoints[i];
        if (0x00 == endpoint) { break; }

        snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
        dev_endpoints.add(hex);
      }
    }
  }
  String payload = "";
  payload.reserve(200);
  json.printTo(payload);
  return payload;
}

// Restore a single device configuration based on json export
// Input: json element as expported by `ZbStatus2``
// Mandatory attribue: `Device`
//
// Returns:
//  0 : Ok
// <0 : Error
//
// Ex: {"Device":"0x5ADF","Name":"IKEA_Light","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}
int32_t Z_Devices::deviceRestore(const JsonObject &json) {

  // params
  uint16_t device = 0x0000;                 // 0x0000 is coordinator so considered invalid
  uint64_t ieeeaddr = 0x0000000000000000LL; // 0 means unknown
  const char * modelid = nullptr;
  const char * manufid = nullptr;
  const char * friendlyname = nullptr;
  int8_t   bulbtype = 0xFF;
  size_t   endpoints_len = 0;

  // read mandatory "Device"
  const JsonVariant &val_device = GetCaseInsensitive(json, PSTR("Device"));
  if (nullptr != &val_device) {
    device = strToUInt(val_device);
  } else {
    return -1;        // missing "Device" attribute
  }

  // read "IEEEAddr" 64 bits in format "0x0000000000000000"
  const JsonVariant &val_ieeeaddr = GetCaseInsensitive(json, PSTR("IEEEAddr"));
  if (nullptr != &val_ieeeaddr) {
    ieeeaddr = strtoull(val_ieeeaddr.as<const char*>(), nullptr, 0);
  }

  // read "Name"
  friendlyname = getCaseInsensitiveConstCharNull(json, PSTR("Name"));

  // read "ModelId"
  modelid = getCaseInsensitiveConstCharNull(json, PSTR("ModelId"));

  // read "Manufacturer"
  manufid = getCaseInsensitiveConstCharNull(json, PSTR("Manufacturer"));

  // read "Light"
  const JsonVariant &val_bulbtype = GetCaseInsensitive(json, PSTR(D_JSON_ZIGBEE_LIGHT));
  if (nullptr != &val_bulbtype) { bulbtype = strToUInt(val_bulbtype);; }

  // update internal device information
  updateDevice(device, ieeeaddr);
  if (modelid) { setModelId(device, modelid); }
  if (manufid) { setManufId(device, manufid); }
  if (friendlyname) { setFriendlyName(device, friendlyname); }
  if (&val_bulbtype) { setHueBulbtype(device, bulbtype); }

  // read "Endpoints"
  const JsonVariant &val_endpoints = GetCaseInsensitive(json, PSTR("Endpoints"));
  if ((nullptr != &val_endpoints) && (val_endpoints.is<JsonArray>())) {
    const JsonArray &arr_ep = val_endpoints.as<const JsonArray&>();
    endpoints_len = arr_ep.size();
    clearEndpoints(device);     // clear even if array is empty
    if (endpoints_len) {
      for (auto ep_elt : arr_ep) {
        uint8_t ep = strToUInt(ep_elt);
        if (ep) {
          addEndpoint(device, ep);
        }
      }
    }
  }

  return 0;
}

#endif // USE_ZIGBEE
