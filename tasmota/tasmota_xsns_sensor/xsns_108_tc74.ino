/*
  xsns_108_tc74.ino - TC74 I2C temperature sensor support for Tasmota

  Copyright (C) 2023  Michael Loftis

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
#ifdef USE_TC74

/*********************************************************************************************\
 * TC74 - Temperature Sensor
 *
 * TC74 I2C Address: 0x4D
 * Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/21462D.pdf
 *
 * Eight I2C address variant parts are available.  The part numbers are TC74An where N is
 * one of the eight addresses noted in the (unused) #defines below.  Be sure to check your
 * part number for the correct address before building your firmware image!  Due to the
 * limited RAM in ESP8266's the default configuration does not try to work with all eight.
 *
 * For I2C 0x4D, TC74A5 part, is the standard address, other addresses are available as a
 * custom order, eight total addresses.  This driver thus supports up to
 * eight sensors, sensors beyond/other than the default 0x4D require setting
 * of defines, there is also no good way to ID these sensors as there's no MSR
 * and nothing unique about their information.
 *
 * These sensors only have a nominal 1C resolution.  The git devices are sold
 * calibrated to a specific supply voltage but will work from 2.7V-5.5V, but
 * will suffer 1C per Volt deviation from the rating of accuracy loss.  They're
 * only accurate to about +/-2C to begin with.
 \*********************************************************************************************/

#define XSNS_108                108
#define XI2C_80                 80

//#define TC74_MAX_SENSORS 8            // Support non-default/multiple I2C addresses
//#define TC74_I2C_PROBE_ADDRESSES { 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F }  // Addresses to probe/support
//#define TC74_MAX_FAILCOUNT 8          // maximum failed polls before it's marked inactive until reprobing later
//#define TC74_EXTRA_DEBUG              // Compile in some extra debug logging statements around failures and measurements

#define TC74_ADDR0              0x48
#define TC74_ADDR1              0x49
#define TC74_ADDR2              0x4A
#define TC74_ADDR3              0x4B
#define TC74_ADDR4              0x4C
#define TC74_ADDR5              0x4D
#define TC74_ADDR6              0x4E
#define TC74_ADDR7              0x4F
#define TC74_ADDR_BASE          TC74_ADDR0
#define TC74_ADDR_DEFAULT       TC74_ADDR5

#ifndef TC74_MAX_SENSORS
#define TC74_MAX_SENSORS        1
uint8_t tc74_probes[TC74_MAX_SENSORS] PROGMEM = { TC74_ADDR_DEFAULT };
#else
uint8_t tc74_probes[TC74_MAX_SENSORS] PROGMEM = TC74_I2C_PROBE_ADDRESSES;
#endif

#ifndef TC74_MAX_FAILCOUNT
#define TC74_MAX_FAILCOUNT      8
#endif

const uint8_t TC74_CMD_RTR =           0x00;    // Read Temperature
const uint8_t TC74_CMD_RWCR =          0x01;    // Read/Write CONFIG

/* CONFIG register
 * D7 = STANDBY (R/W)
 * D6 = READY (R/O)
 * D5-D0 = Reserved - always zero
 * POR state is all zeroes.
 */
const uint8_t TC74_CONFIG_DRDY  = 0b00000010;

// only used once...
#define TC74_CONFIG_MASK          0b11111100

struct {
  uint8_t found_count = 0;
  uint8_t tcnt = 0;
} tc74_status;

struct {
  // Last read temperature
  float   temperature = NAN;
  // flag if we could/couldn't read in most recent poll
  bool is_active;
  // failure count, if this goes above N we set active to false and
  // the sensor will be retried later
  uint8_t failed_count;
  uint8_t address;
} tc74_sensors[TC74_MAX_SENSORS];


void TC74InitState() {
#ifdef TC74_EXTRA_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: InitState"));
#endif
  for (uint32_t i = 0; i < TC74_MAX_SENSORS; i++) {
    tc74_sensors[i].is_active = false;
    tc74_sensors[i].failed_count = 0;
    tc74_sensors[i].address = pgm_read_byte(tc74_probes + i);
  }
}

void TC74Detect(bool forced) {
  for (uint32_t i = 0; i < TC74_MAX_SENSORS; i++) {
    uint8_t config_reg;
    uint8_t addr = tc74_sensors[i].address;
#ifdef TC74_EXTRA_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: Addr %X probing"), addr);
#endif

    // if we failed more than N times, unless we're being forced to, skip it.
    if (!forced && tc74_sensors[i].failed_count >= TC74_MAX_FAILCOUNT ) {
      tc74_sensors[i].is_active = false;
      continue;
    }

    // I2cSetDevice ALWAYs returns false if a device is marked active already...
    // So if we have the is_active flag for this device SKIP this check
    if (!tc74_sensors[i].is_active && !I2cSetDevice(addr)) {
      if (tc74_sensors[i].failed_count < 253) { tc74_sensors[i].failed_count++; }
#ifdef TC74_EXTRA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: Addr %X failed I2cSetDevice"), addr);
#endif
      continue;
    }

    // Pull CONFIG and check it, best we can do to keep away from other I2C devices
    if (!I2cValidRead8(&config_reg, addr, TC74_CMD_RWCR)) {
      tc74_sensors[i].is_active = false;
      tc74_sensors[i].failed_count++;
      if(I2cActive(addr)) { I2cResetActive(addr); }
#ifdef TC74_EXTRA_DEBUG
      AddLog(LOG_LEVEL_ERROR, PSTR("TC7: Addr %X failed CONFIG read, deactivated"), addr);
#endif
      continue;
    }

    // if any reserved bits are set, not our device
    if (config_reg & TC74_CONFIG_MASK != 0x00 ) {
      tc74_sensors[i].is_active = false;
      tc74_sensors[i].failed_count++;
      if(I2cActive(addr)) { I2cResetActive(addr); }
#ifdef TC74_EXTRA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: Addr %X found reserved bits set [%x]"), addr, config_reg);
#endif

      continue;
    }

    // Make sure STANDBY is not set, POR should be clear, but, if another I2C driver toggled it....
    I2cWrite8(addr, TC74_CMD_RWCR, 0x0);

    if (!tc74_sensors[i].is_active) {
      tc74_sensors[i].is_active = true;
#ifdef TC74_EXTRA_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: Addr %X set active"), addr);
#endif
      I2cSetActiveFound(addr, "TC74");
    }
    tc74_sensors[i].failed_count = 0;
  } // for sensors...
}

void TC74PollActive() {
  for (uint8_t i = 0; i < TC74_MAX_SENSORS; i++) {
    uint8_t addr = tc74_sensors[i].address;
    uint8_t config_reg;
    int8_t temperature_register;

    if (!tc74_sensors[i].is_active) { continue; }

    // if timing were a problem we could read both bytes from the RTR, then
    // do mask/shift to recover the parts we want.
    if (!I2cValidRead8(&config_reg, addr, TC74_CMD_RWCR)) {
      tc74_sensors[i].failed_count++;
      continue;
    }

    // check if a measurement is ready
    if (config_reg & TC74_CONFIG_DRDY != 0b00000010) { continue; }

    // grab most recent reading
    if (!I2cValidRead8((uint8_t*)&temperature_register, addr, TC74_CMD_RTR)) {
      tc74_sensors[i].failed_count++;
      continue;
    }

    tc74_sensors[i].temperature = ConvertTemp(temperature_register);
    tc74_sensors[i].failed_count = 0;
#ifdef TC74_EXTRA_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: Addr %X register temperature was %hhi or [%i]"), addr, temperature_register, temperature_register);
    AddLog(LOG_LEVEL_DEBUG, PSTR("TC7: Addr %X stored temperature %*_f"), addr, Settings->flag2.temperature_resolution, &tc74_sensors[i].temperature);
#endif

  } // for sensors...
}

void TC74Show(bool json) {
  bool once = true;
  // loop over sensors, only output if active
  for (uint8_t i = 0; i < TC74_MAX_SENSORS; i++) {
    if (tc74_sensors[i].is_active) {
      char sname[10];
      snprintf_P(sname, sizeof(sname), PSTR("TC74%c%02X"), IndexSeparator(), tc74_sensors[i].address);
      if (json) {
        ResponseAppend_P(JSON_SNS_F_TEMP, sname, Settings->flag2.temperature_resolution, &tc74_sensors[i].temperature);
        // also send KNX and Domoticz if enabled...and first sensor reporting
        // might beed guarded by some sort of enable (compile time or otherwise)
        if ((0 == TasmotaGlobal.tele_period) && once) {
#ifdef USE_DOMOTICZ
          DomoticzFloatSensor(DZ_TEMP, tc74_sensors[i].temperature);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
          KnxSensor(KNX_TEMPERATURE, tc74_sensors[i].temperature);
#endif  // USE_KNX
          once = false;
        } // tele_period, first reporter
#ifdef USE_WEBSERVER
      } else {
         WSContentSend_Temp(sname, tc74_sensors[i].temperature);
#endif  // USE_WEBSERVER
      } // if(json)
    } // if(is_active)
  } // for sensors...
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns108(uint32_t function) {
  if (!I2cEnabled(XI2C_80)) { return false; }

  bool result = false;

  // Unlike some other drivers we DO support sensors disappearing and reappearing later
  // or simply not available during initial startup

  switch (function) {
    case FUNC_INIT:
        TC74InitState();
        TC74Detect(false);
        break;
    case FUNC_HOTPLUG_SCAN:
        TC74Detect(true);
        tc74_status.tcnt = 1;
        break;
    case FUNC_EVERY_SECOND:
        /* if(tc74_status.tcnt == 0 || tc74_status.tcnt == 60) {
            TC74Detect(true);
        } */
        tc74_status.tcnt++;
        if(tc74_status.tcnt > 60) {
            tc74_status.tcnt = 1;
        }
        TC74PollActive();
        break;
    case FUNC_JSON_APPEND:
        TC74Show(true);
        break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
        TC74Show(false);
        break;
#endif
  } // switch(function)
  return result;
}

#endif // USE_TC74
#endif // USE_I2C