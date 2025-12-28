/*
  xsns_81_seesaw - Adafruit Seesaw family base class

  Copyright (C) 2021  Wayne Ross, Theo Arends, Peter Franck, Allen Schober

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

#if defined(USE_SEESAW_SOIL) || defined(USE_SEESAW_ENCODER)
  #define USE_SEESAW
#endif

#if defined(USE_SEESAW)

/*********************************************************************************************\
 * Seesaw - Base class for Adafruit seesaw devices
 *
 * This driver provides a unified interface for multiple seesaw device types:
 *   - STEMMA Soil Sensor - I2C Capacitive Moisture Sensor (USE_SEESAW_SOIL)
 *   - I2C QT Rotary Encoder (USE_SEESAW_ENCODER)
 *
 * Address ranges:
 *   - Soil sensors: 0x36 - 0x39
 *   - Encoders: 0x36 - 0x3D
 *
 * The driver detects which type of module is at each address by reading the
 * hardware ID, version, and options register.
\*********************************************************************************************/

#define XSNS_81              81
#define XI2C_56              56                 // See I2CDEVICES.md

#include "Adafruit_seesaw.h"                    // we only use definitions, no code

#define SEESAW_ADDR_MIN     0x36                // First seesaw address
#define SEESAW_ADDR_MAX     0x39                // Last seesaw address (limited to 4 devices)
#define SEESAW_MAX_SENSORS  4                   // Maximum supported devices

// I2C delays
#define SEESAW_DELAY_DETECT      10             // ms delay before reading ID
#define SEESAW_DELAY_RESET       100            // ms delay after slave reset

// Supported module types
enum SeesawDeviceType {
  SEESAW_TYPE_UNKNOWN = 0,
  SEESAW_TYPE_SOIL,
  SEESAW_TYPE_ENCODER
};

// Base struct for all seesaw devices
struct SeesawDevice {
  SeesawDevice(uint8_t addr) : address(addr), type(SEESAW_TYPE_UNKNOWN), valid(false), device_index(0) {}

  virtual ~SeesawDevice() {}

  virtual void Init() = 0;
  virtual void Read() = 0;
  virtual void Show(bool json, const char *name) = 0;
  virtual void Handler() {}  // Optional handler for devices that need periodic processing
  virtual bool HandleCommand(const char* cmd, uint32_t len) { return false; }  // Optional command handler

  bool IsValid() const { return valid; }
  uint8_t GetAddress() const { return address; }
  SeesawDeviceType GetType() const { return type; }
  uint8_t GetDeviceIndex() const { return device_index; }
  void SetDeviceIndex(uint8_t idx) { device_index = idx; }

  static const char id[] PROGMEM;

protected:
  uint8_t address;
  SeesawDeviceType type;
  bool valid;
  uint8_t device_index;  // Index in manager's device array (0-based)
  char device_name[16];  // Stores formatted name from Show() for Handler/debug use
};

const char SeesawDevice::id[] PROGMEM = "";

// Device type names for identification
#ifdef USE_SEESAW_SOIL
const char SEESAW_SOIL_ID[] PROGMEM = "SOIL";
#endif
#ifdef USE_SEESAW_ENCODER
const char SEESAW_ENCODER_ID[] PROGMEM = "ENCODER";
#endif

// Common seesaw I2C helper functions
namespace Seesaw {
  bool Write8(uint8_t addr, uint8_t regHigh, uint8_t regLow, uint8_t value) {
    Wire.beginTransmission(addr);
    Wire.write(regHigh);
    Wire.write(regLow);
    Wire.write(value);
    return (Wire.endTransmission() == 0);
  }

  bool Write(uint8_t addr, uint8_t regHigh, uint8_t regLow, const uint8_t *buf, uint8_t num) {
    Wire.beginTransmission(addr);
    Wire.write(regHigh);
    Wire.write(regLow);
    for (uint8_t i = 0; i < num; i++) {
      Wire.write(buf[i]);
    }
    return (Wire.endTransmission() == 0);
  }

  bool Read(uint8_t addr, uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num) {
    Wire.beginTransmission(addr);
    Wire.write(regHigh);
    Wire.write(regLow);
    if (Wire.endTransmission() != 0) { return false; }

    delay(1);  // Small delay for register read

    if (num != Wire.requestFrom(addr, num)) { return false; }
    for (uint8_t i = 0; i < num; i++) {
      buf[i] = Wire.read();
    }
    return true;
  }
}

// Manager class to handle all seesaw devices
struct SeesawManager {
  void Init() {
    // Send reset to all potential addresses
    for (uint8_t addr = SEESAW_ADDR_MIN; addr <= SEESAW_ADDR_MAX; addr++) {
      if (!I2cSetDevice(addr)) { continue; }
      Wire.beginTransmission(addr);
      Wire.write(SEESAW_STATUS_BASE);
      Wire.write(SEESAW_STATUS_SWRST);
      Wire.write(0xFF);
      Wire.endTransmission();
    }

    state = STATE_RESET;
    state_time = millis();
  }

  void Every50ms() {
    uint32_t time_diff = millis() - state_time;

    switch (state) {
      case STATE_RESET:
        state = STATE_INIT;
        break;

      case STATE_INIT:
        if (time_diff < SEESAW_DELAY_RESET) { return; }
        // Send hardware ID read command to all potential addresses
        for (uint8_t addr = SEESAW_ADDR_MIN; addr <= SEESAW_ADDR_MAX; addr++) {
          if (!I2cSetDevice(addr)) { continue; }
          Wire.beginTransmission(addr);
          Wire.write(SEESAW_STATUS_BASE);
          Wire.write(SEESAW_STATUS_HW_ID);
          Wire.endTransmission();
        }
        state = STATE_DETECT;
        break;

      case STATE_DETECT:
        if (time_diff < SEESAW_DELAY_DETECT) { return; }
        Detect();
        state = STATE_READ;
        break;

      case STATE_READ:
        // Read all sensors
        for (uint8_t i = 0; i < count; i++) {
          if (devices[i]) {
            devices[i]->Read();
            devices[i]->Handler();
          }
        }
        break;
    }
    state_time = millis();
  }

  void Show(bool json) {
    for (uint8_t i = 0; i < count; i++) {
      if (devices[i] && devices[i]->IsValid()) {
        char name[12];
        GetDeviceName(i, name, sizeof(name));
        devices[i]->Show(json, name);
      }
    }
  }

  bool HandleCommand(uint32_t index, const char* cmd, uint32_t len) {
    if (index == 0 || index > count) { return false; }
    return devices[index - 1]->HandleCommand(cmd, len);
  }

  uint8_t GetCount() const { return count; }

  uint8_t GetTypeCount(SeesawDeviceType type) const {
    uint8_t type_count = 0;
    for (uint8_t i = 0; i < count; i++) {
      if (devices[i] && devices[i]->GetType() == type) {
        type_count++;
      }
    }
    return type_count;
  }

  SeesawDevice* GetDevice(uint8_t index) {
    if (index >= count) { return nullptr; }
    return devices[index];
  }

private:
  void Detect() {
    count = 0;

    for (uint8_t addr = SEESAW_ADDR_MIN; addr <= SEESAW_ADDR_MAX && count < SEESAW_MAX_SENSORS; addr++) {
      if (!I2cSetDevice(addr)) { continue; }

      // Check for valid hardware ID
      if (1 != Wire.requestFrom(addr, (uint8_t)1)) { 
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: No response at ADDR=0x%02X, skipping device."), addr);
        continue;
      }
      uint8_t hw_id = Wire.read();
      bool valid_hw_id = (hw_id == SEESAW_HW_ID_CODE_SAMD09 ||    // Soil sensor, encoder
                          hw_id == SEESAW_HW_ID_CODE_TINY806 ||
                          hw_id == SEESAW_HW_ID_CODE_TINY807 ||
                          hw_id == SEESAW_HW_ID_CODE_TINY816 ||
                          hw_id == SEESAW_HW_ID_CODE_TINY817 ||
                          hw_id == SEESAW_HW_ID_CODE_TINY1616 ||
                          hw_id == SEESAW_HW_ID_CODE_TINY1617);
      if (!valid_hw_id) {
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: Unknown HW ID 0x%02X at ADDR=0x%02X, skipping device."), hw_id, addr);
        continue;
      }

      uint8_t version_buf[4];
      if (!Seesaw::Read(addr, SEESAW_STATUS_BASE, SEESAW_STATUS_VERSION, version_buf, 4)) {
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: Failed to read VERSION at ADDR=0x%02X, skipping device."), addr);
        continue;
      }
      uint32_t version = ((uint32_t)version_buf[0] << 24) | ((uint32_t)version_buf[1] << 16) |
                  ((uint32_t)version_buf[2] << 8) | (uint32_t)version_buf[3];
      AddLog(LOG_LEVEL_INFO, PSTR("SEE: Seesaw module at ADDR=0x%02X with firmware 0x%08X"), addr, version);

      // Determine device type by reading module options register
      // The SEESAW_STATUS_OPTIONS register returns a 32-bit bitmask where each bit
      // corresponds to a module base address (e.g., bit 0x11 = SEESAW_ENCODER_BASE)
      SeesawDevice* device = nullptr;
      SeesawDeviceType detected_type = SEESAW_TYPE_UNKNOWN;

      uint8_t options_buf[4];
      if (!Seesaw::Read(addr, SEESAW_STATUS_BASE, SEESAW_STATUS_OPTIONS, options_buf, 4)) {
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: Failed to read OPTIONS register at ADDR=0x%02X, skipping device."), addr);
        continue;
      }
      uint32_t options = ((uint32_t)options_buf[0] << 24) | ((uint32_t)options_buf[1] << 16) |
                         ((uint32_t)options_buf[2] << 8) | (uint32_t)options_buf[3];

#ifdef USE_SEESAW_ENCODER
      // Check for encoder module (bit 0x11 = SEESAW_ENCODER_BASE)
      if (!device && (options & (1UL << SEESAW_ENCODER_BASE))) {
        device = CreateEncoderDevice(addr);
        detected_type = SEESAW_TYPE_ENCODER;
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: Detected Seesaw encoder at 0x%02X"), addr);
      }
#endif

#ifdef USE_SEESAW_SOIL
      // Check for capacitive module (bit 0x0F = SEESAW_TOUCH_BASE)
      if (!device && (options & (1UL << SEESAW_TOUCH_BASE))) {
        device = CreateSoilDevice(addr);
        detected_type = SEESAW_TYPE_SOIL;
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: Detected Seesaw soil sensor at 0x%02X"), addr);
      }
#endif

      if(!device) {
        AddLog(LOG_LEVEL_INFO, PSTR("SEE: No known modules found at ADDR=0x%02X with OPTIONS=0x%08X, skipping device."), addr, options);
        continue;
      } else {
        devices[count] = device;

        // Set the type-specific index based on the number of same-type devices detected so far
        uint8_t type_index = GetTypeCount(detected_type);
        device->SetDeviceIndex(type_index);
        device->Init();

        char name[12];
        GetDeviceName(count, name, sizeof(name));
        I2cSetActiveFound(addr, name);

        count++;
      }
    }
  }

  void GetDeviceName(uint8_t index, char* name, size_t len) {
    if (index >= count || !devices[index]) {
      snprintf_P(name, len, PSTR("Seesaw"));
      return;
    }

    const char* type_prefix = "Seesaw";
    bool use_address = false;
    SeesawDeviceType device_type = devices[index]->GetType();

    switch (device_type) {
#ifdef USE_SEESAW_SOIL
      case SEESAW_TYPE_SOIL:
        type_prefix = "SeeSoil";
#ifdef SEESAW_SOIL_PERSISTENT_NAMING
        use_address = true;
#endif
        break;
#endif
#ifdef USE_SEESAW_ENCODER
      case SEESAW_TYPE_ENCODER:
        type_prefix = "SeeEnc";
#ifdef SEESAW_ENCODER_PERSISTENT_NAMING
        use_address = true;
#endif
        break;
#endif
      default:
        break;
    }

    if (use_address) {
      // Address-based naming: always include address, even for single device (e.g. "SeeSoil-36", "SeeEnc-38")
      snprintf_P(name, len, PSTR("%s%c%02X"), type_prefix, IndexSeparator(), devices[index]->GetAddress());
    } else {
      // Index-based naming: only add index if multiple devices of same type
      uint8_t type_count = GetTypeCount(device_type);

      if (type_count > 1) {
        // Multiple devices: "SeeSoil-1", "SeeEnc-1" using a type-specific device_index
        snprintf_P(name, len, PSTR("%s%c%u"), type_prefix, IndexSeparator(), devices[index]->GetDeviceIndex() + 1);
      } else {
        // Single device of this type: just "SeeSoil" or "SeeEnc"
        strlcpy(name, type_prefix, len);
      }
    }
  }

  SeesawDevice* CreateSoilDevice(uint8_t addr);
  SeesawDevice* CreateEncoderDevice(uint8_t addr);

  enum State {
    STATE_RESET,
    STATE_INIT,
    STATE_DETECT,
    STATE_READ
  };

  State state = STATE_RESET;
  uint32_t state_time = 0;
  uint8_t count = 0;
  SeesawDevice* devices[SEESAW_MAX_SENSORS] = {nullptr};
} SeesawMgr;

/*********************************************************************************************\
 * Stub implementations for factory functions when device types are not compiled in
\*********************************************************************************************/
#ifndef USE_SEESAW_SOIL
SeesawDevice* SeesawManager::CreateSoilDevice(uint8_t addr) {
  return nullptr;
}
#endif

#ifndef USE_SEESAW_ENCODER
SeesawDevice* SeesawManager::CreateEncoderDevice(uint8_t addr) {
  return nullptr;
}
#endif

/*********************************************************************************************\
 * Forward declarations for Encoder commands (defined in xsns_81_seesaw_encoder.ino)
\*********************************************************************************************/
#ifdef USE_SEESAW_ENCODER
extern const char kSeeEncCommands[];
extern void (* const SeeEncCommand[])(void);
#endif

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns81(uint32_t function)
{
  if (!I2cEnabled(XI2C_56)) { return false; }
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      SeesawMgr.Init();
      break;

    case FUNC_EVERY_50_MSECOND:
      SeesawMgr.Every50ms();
      break;

    case FUNC_JSON_APPEND:
      SeesawMgr.Show(true);
      break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      SeesawMgr.Show(false);
      break;
#endif

#ifdef USE_SEESAW_ENCODER
    case FUNC_COMMAND:
      result = DecodeCommand(kSeeEncCommands, SeeEncCommand);
      break;
#endif
  }

  return result;
}

#endif  // USE_SEESAW
#endif  // USE_I2C
