/*
  xdrv_75_dali.ino - DALI support for Tasmota

  Copyright (C) 2022  Andrei Kazmirtsuk aka eeak and Theo Arends

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

#ifdef USE_DALI
/*********************************************************************************************\
 * DALI support for Tasmota
 * 
 * Available commands:
 *   <byte1>..<byte4> = 0..255 or 0x00..0xFF     - Both decimal and hexadecimal is supported
 *   <broadcast> = 0                             - DALI default
 *   <device> = 1..64                            - DALI short address + 1
 *   <group> = 101..116                          - DALI group + 101
 *   <dt> = 6, 8                                 - DALI Device Type specific extended command
 * DaliSend <byte1>,<byte2>                      - Execute DALI code and do not expect a DALI backward frame
 * DaliSend <dt>,<byte1>,<byte2>                 - Execute DALI extended code for DT and do not expect a DALI backward frame
 * DaliSend <0xA3>,<byte2>,<byte3>,<byte4>       - Set DALI parameter using DTR0 and do not expect a DALI backward frame
 * DaliQuery <byte1>,<byte2>                     - Execute DALI code and report result (DALI backward frame)
 * DaliQuery <dt>,<byte1>,<byte2>                - Execute DALI extended code for DT and report result (DALI backward frame)
 * DaliScan 1|2[,<max_count>]                    - Reset (0) or (1)/and commission device short addresses up to optional <max_count> - default 64
 * DaliGear 1..64                                - Set max short address to speed up scanning - default 64
 * DaliGroup<1..16> [+]|-<device>,<device>...    - Add(+) or Remove(-) devices to/from group
 * DaliPower<broadcast>|<device>|<group> 0..254  - Control power (0 = Off, 1 = Last dimmer, 2 = Toggle, 3..254 = absolute light brightness)
 * DaliDimmer<broadcast>|<device>|<group> 0..100 - Control dimmer (0 = Off, 1..100 = precentage of brightness)
 * DaliLight 0|1                                 - Enable Tasmota light control for DaliTarget device - default 1
 * DaliTarget <broadcast>|<device>|<group>       - Set Tasmota light control device (0, 1..64, 101..116) - default 0
 * DaliChannels 1..5                             - Set Tasmota light type (1 = R/C = DT6, 2 = RG/CW, 3 = RGB, 4 = RGBW, 5 = RGBWC) for DaliTarget
 * 
 * DALI background information
 * Address type        Address byte
 * ------------------  --------------------
 * Broadcast address   1111111S
 * 64 short address    0AAAAAAS
 * 16 group address    100AAAAS
 * Special command     101CCCC1 to 110CCCC1
 * A = Address bit, S = 0 Direct Arc Power control, S = 1 Command, C = Special command
 * 
 * Shelly DALI Dimmer Gen3 (ESP32C3-8M) - GPIO3 controls DALI power. In following template it is always ON. Max output is 16V/10mA (= 5 DALI gear)
 * - Template {"NAME":"Shelly DALI Dimmer Gen3","GPIO":[34,4736,0,3840,11360,11392,128,129,0,1,576,0,0,0,0,0,0,0,0,1,1,1],"FLAG":0,"BASE":1}
 * - AdcGpio1 10000,10000,4000        <- Temperature parameters
 * - Backlog ButtonTopic 0; SetOption1 1; SetOption11 0; SetOption32 20; DimmerStep 5; LedTable 0
 * - rule1 on button1#state=2 do dimmer + endon on button2#state=2 do dimmer - endon on button1#state=3 do power 2 endon on button2#state=3 do power 2 endon
 *
 * DALI RGBWAF color support tested with MiBoxer DALI 5 in 1 LED Controller (DT8) and 12V RGB led strip:
 * - Reset device (long press M button 10 seconds until digital display shows AES).
 * - Set fixed unused short address (Scanning/Commissioning doesn't work for this device) ie. 005.
 * - Use command `DaliTarget 6` to link Tasmota color control to this device ie. 005 +1 = 6.
 * - Use command `DaliChannels 3` to set the amount of color hardware channels connected ie. RGB ledstrip is 3.
 * - Use command `DaliLight 1` to enable Tasmota color control.
  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  1.4.1.0 20251130  update    - Add options to `DaliGear` and DaliGroup` to toggle specific outputs
                              - Make max number of devices persistent to speed up scan response
  1.4.0.0 20251126  update    - Change to TasmotaDali library
  1.3.0.4 20251123  update    - Add send retry on collision detection
                              - Prep DALI-2 24-bit transceive
  1.3.0.3 20251122  update    - Remove sleep dependency from frame handling
                              - Change receive timeout from 50 ms to 20 ms (DALI protocol is 9.2 ms)
                              - Add DALI DT8 RGBWAF Control Gear (receive) for Tasmota color light control
  1.3.0.2 20251121  update    - Revert timing from 10 to 14ms as changed due to bad dali PS (underrated Shelly DALI Dimmer Gen3)
                              - Add optional power off without fading (reduces DT8 dali commands)
                              - Remove not performing logging from interrupt routine
  1.3.0.1 20251120  update    - Reduce send-twice timing from 14 to 10ms fixing MiBoxer DT8
  1.3.0.0 20251119  update    - Add DALI DT8 RGBWAF Control Device (send) using Tasmota color light control
                              - Add persistent command `DaliChannels` to select Tasmota color type
  1.2.0.0 20251116  update    - Add persistence for `DaliTarget` if filesystem is present
  1.1.0.4 20251115  fix       - Tasmota light control using non-broadcast address
  1.1.0.3 20251112  remove    - Remove optional repeat for commands `DaliSend` and `DaliQuery`
                                Send twice is now based on DALI defined command type
  1.1.0.2 20251109  update    - Add optional extended commands prefix for commands `DaliSend` and `DaliQuery`
  1.1.0.1 20241101  update    - Enable DALI if another light is already claimed
  1.1.0.0 20241031  update    - Add GUI sliders with feedback when `DaliLight 0`
                              - Add command `DaliGroupSliders 0..16` to show GUI sliders
  1.0.0.2 20241025  update    - Fix GPIO detection
                              - Fix ESP32(C3) transmit stability by disabling interrupts
  1.0.0.1 20241024  update    - Change from signal invert defines to GPIO config DALI RX_i/DALI TX_i
                              - Fix inverted DALI signal support
                              - Experimental support for Shelly DALI Dimmer Gen3
  1.0.0.0 20241022  update    - Refactor commission
                              - Add receive collision detection
  0.1.0.8 20241019  update    - Rename command `DaliCommission` to `DaliScan`
                              - Rename command `DaliWeb` to `DaliLight`
                              - Add command `DaliGear` to set max found gear to speed up scan response
  0.1.0.7 20241017  update    - Add command `DaliCommission 1|2` assigning short addresses
                              - Add command `DaliTarget 0, 1..64, 101..116` to select light control address
                              - Add command `DaliGroup1..16 +<devices>|-<devices>` to add/remove devices from group
                              - Extend commands `DaliPower` and `DaliDimmer` with index to control short address or group
                              - Remove non-functional MQTT interface
  0.1.0.6 20241014  update    - Fix received light command loop
                              - Add send collision detection
  0.1.0.5 20241014  update    - Add command `DaliSend [repeat]<address>,<command>`
                              - Add command `DaliQuery [repeat]<address>,<command>`
                              - Send frame twice (repeat) for DALI defined commands
                              - Add support for receiving backward frame
  0.1.0.4 20241013  update    - Fix intermittent bad send timing
  0.1.0.3 20241010  update    - Change DaliDimmer range from 0..254 to 0..100
                              - Add command `DaliWeb 0|1` to enable persistent Web light controls
  0.1.0.2 20241008  update    - Better receive error detection
  0.1.0.1 20241007  update    - To stablizie communication send DALI datagram twice like Busch-Jaeger does 
                              - Change DaliPower 0..2 to act like Tasmota Power (Off, On, Toggle)
                              - Keep last Dimmer value as default power on
  0.1.0.0 20241006  rewrite   - Add support for ESP8266 by Theo Arends
                              - Fix decoding of received DALI data
                              - Refactor command `DaliPower 0..254` controlling Broadcast devices
                              - Add command `DaliDimmer 0..254` controlling Broadcast devices
  0.0.0.1 20221027  publish   - Initial version by Andrei Kazmirtsuk aka eeak
\*********************************************************************************************/

#define XDRV_75                    75

#ifndef DALI_INIT_STATE
#define DALI_INIT_STATE            50          // DALI init dimmer state 50/254
#endif
#ifndef DALI_INIT_FADE 
#define DALI_INIT_FADE             1           // Fade between light states in number of seconds
#endif
#ifndef DALI_TIMEOUT
#define DALI_TIMEOUT               20          // DALI backward frame receive timeout (ms) - Protocol = >7Te and <22Te (22 * 417us)
#endif

//#define DALI_LIGHT_COLOR_SUPPORT               // Support DALI DT8 RGBWAF
//#define DALI_LIGHT_NO_READ_AFTER_WRITE         // Use no DTR read-after-write for smooth color transitions (saves 55ms / channel)

//#define DALI_POWER_OFF_NO_FADE                 // Power off immediatly without fading

//#define DALI_DEBUG

/*********************************************************************************************/

#include <TasmotaDali.h>

#define DALI_MAX_STORED            17          // Store broadcast and group states

#define DALI_TOPIC "DALI"
#define D_PRFX_DALI "Dali"

/*********************************************************************************************/

typedef struct DliSettings_t {
  uint32_t crc32;                              // To detect file changes
  uint8_t target;
  uint8_t light_type;
  uint8_t max_gear;
} DliSettings_t;

struct DALI {
  DliSettings_t Settings;                      // Persistent settings
  TasmotaDali *dali;
  uint8_t address;
  uint8_t command;
  uint8_t last_dimmer;
  uint8_t dimmer[DALI_MAX_STORED];
  uint8_t web_dimmer[DALI_MAX_STORED];
  uint8_t color[5];
  uint8_t target_rgbwaf;
  uint8_t device_type;
  uint8_t dtr[3];
  uint8_t probe;
#ifdef DALI_DEBUG
  uint8_t log_level;
#endif  // DALI_DEBUG
  bool allow_light;
  bool last_power;
  bool power[DALI_MAX_STORED];
  bool light_sync;
} *Dali = nullptr;

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

#ifdef USE_UFILESYS
#define XDRV_75_KEY           "drvset75"

bool DaliLoadData(void) {
  char key[] = XDRV_75_KEY;
  String json = UfsJsonSettingsRead(key);
  if (json.length() == 0) { return false; }

  // {"Crc":1882268982,"Target":0,"LightType":3}
  JsonParser parser((char*)json.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  Dali->Settings.crc32 = root.getUInt(PSTR("Crc"), Dali->Settings.crc32);
  Dali->Settings.target = root.getUInt(PSTR("Target"), Dali->Settings.target);
  Dali->Settings.light_type = root.getUInt(PSTR("LightType"), Dali->Settings.light_type);
  Dali->Settings.max_gear = root.getUInt(PSTR("MaxGear"), Dali->Settings.max_gear);

  return true;
}

bool DaliSaveData(void) {
  Response_P(PSTR("{\"" XDRV_75_KEY "\":{\"Crc\":%u,\"Target\":%u,\"LightType\":%u,\"MaxGear\":%u}}"),
                   Dali->Settings.crc32,
                   Dali->Settings.target,
                   Dali->Settings.light_type,
                   Dali->Settings.max_gear);

  return UfsJsonSettingsWrite(ResponseData());
}

void DaliDeleteData(void) {
  char key[] = XDRV_75_KEY;
  UfsJsonSettingsDelete(key);                  // Use defaults
}
#endif  // USE_UFILESYS

/*********************************************************************************************/

void DaliSettingsLoad(bool erase) {
  // Called from FUNC_MODULE_INIT/FUNC_PRE_INIT (erase = 0) once at restart
  // Called from FUNC_RESET_SETTINGS (erase = 1) after command reset 4, 5, or 6

  // *** Start init default values in case key is not found ***
  memset(&Dali->Settings, 0x00, sizeof(DliSettings_t));
  Dali->Settings.light_type = LT_RGB;          // Default RGB channel
  Dali->Settings.max_gear = 64;                // Default max supported short address
  // *** End Init default values ***

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Use default cfg as file system not enabled"));
#else
  // Try to load key
  if (erase) {
    DaliDeleteData();
  }
  else if (DaliLoadData()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Cfg loaded from file"));
  }
  else {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Use default cfg as file system not ready or key not found"));
  }
#endif  // USE_UFILESYS
}

void DaliSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
#ifdef USE_UFILESYS
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&Dali->Settings +4, sizeof(DliSettings_t) -4);  // Skip crc32
  if (crc32 != Dali->Settings.crc32) {
    Dali->Settings.crc32 = crc32;
    if (DaliSaveData()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Cfg saved to file"));
    } else {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: ERROR File system not ready or unable to save file"));
    }
  }
#endif  // USE_UFILESYS
}

/*********************************************************************************************\
 * DALI low level
\*********************************************************************************************/

uint32_t DaliKelvinToMirek(uint32_t kelvin) {
  if (kelvin < 16) {
    return 65535;
  }
  if (kelvin > 1000000) {
    return 0;
  }
  return 1000000 / kelvin;
}

uint32_t DaliMirekToKelvin(uint32_t mirek) {
  if (mirek <= 0) {
    return 1000001;
  }
  if (mirek >= 65535) {
    return 0;
  }
  return 1000000 / mirek;
}

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliTarget2Address(uint32_t target) {
  // 1..64    = Short address
  // 101..116 = Group address
  // Others   = Broadcast
  if ((target >= 1) && (target <= 64)) {       // 1 .. 64
    return (target -1) << 1;                   // Short address: 0b00000000 .. 0b01111110
  }
  else if ((target >= 101) && (target <= 116)) {  // 101 .. 116
    return ((target -101) << 1) | 0x80;        // Group address: 0b10000000 .. 0b10011110
  }
  return DALI_BROADCAST_DP;                    // Broadcast address: 0b11111110
}
/*
uint32_t DaliAddress2Target(uint32_t adr) {
  if (adr >= 254) {                            // 0b1111111S
    return 0;                                  // Broadcast address (0)
  }
  else if ((adr >= 128) && (adr <= 159)) {     // 0b1000000S .. 0b1001111S
    return (adr >> 1) +101;                    // Group address (101 .. 116)
  }
  return (adr >> 1) +1;                        // 0b0000000S .. 0b0111111S Short address (1 .. 64)
}
*/
/*-------------------------------------------------------------------------------------------*/

uint32_t DaliSaveState(uint32_t adr, uint32_t cmd) {
  if (adr & DALI_SELECTOR_BIT) { return 0; }   // No address
  int index = -1;
  if (DALI_BROADCAST_DP == adr) {              // Broadcast address
    index = 0;
  }
  adr >>= 1;
  if ((adr >= 0x40) && (adr <= 0x4F)) {        // Group address 0 to 15
    index = adr -0x3F;
  }
  if (index >= 0) {
    Dali->last_power = Dali->power[index];
    Dali->power[index] = (cmd);                // State
    if (Dali->power[index]) {
      Dali->last_dimmer = Dali->dimmer[index];
      Dali->dimmer[index] = cmd;               // Value
    }
    if ((0 == index) && !Dali->power[0]) {     // Only on Broadcast change to power Off
      for (uint32_t i = 0; i < DALI_MAX_STORED; i++) {
        Dali->power[i] = false;                // Log all group power as Off when Broadcast is Off
      }
    }
  } else {
    index = 0;                                 // Use broadcast
  }
  return index;
}

/*-------------------------------------------------------------------------------------------*/

void DaliSendData(uint32_t adr, uint32_t cmd) {
  DaliFrame frame;
  if (adr & TM_DALI_EVENT_FRAME) {             // 24-bit event frame
    frame.data = cmd;
    frame.meta = adr & TM_DALI_BIT_COUNT_MASK;
  } else {                                     // 16-bit command frame
    adr &= 0xFF;
    cmd &= 0xFF;

    Dali->address = adr;
    Dali->command = cmd;
    DaliSaveState(adr, cmd);

    frame.data = adr << 8 | cmd;
    frame.meta = 16;

    if (adr & DALI_SELECTOR_BIT) {               // Selector bit (command) or special command
      if (DALI_102_ENABLE_DEVICE_TYPE_X == adr) {
        Dali->device_type = cmd;                 // Next command will be an extended command
      }
      uint32_t send_twice_extended_start;
      uint32_t send_twice_extended_end;
      if (DALI_205_DEVICE_TYPE == Dali->device_type) {
        send_twice_extended_start = DALI_205_REFERENCE_SYSTEM_POWER;
        send_twice_extended_end = DALI_205_RESERVED237;
      }
      else if (DALI_207_DEVICE_TYPE == Dali->device_type) {
        send_twice_extended_start = DALI_207_REFERENCE_SYSTEM_POWER;
        send_twice_extended_end = DALI_207_RESERVED236;
      }
      else if (DALI_209_DEVICE_TYPE == Dali->device_type) {
        send_twice_extended_start = DALI_209_SET_TEMPORARY_X_COORDINATE;
        send_twice_extended_end = DALI_209_START_AUTO_CALIBRATION;
      }
      if ((DALI_102_INITIALISE == adr) ||
          (DALI_102_RANDOMISE == adr) ||
          ((cmd >= DALI_102_RESET) && (cmd <= DALI_102_RESERVED143)) ||  // Configuration commands
          (Dali->device_type && (cmd >= send_twice_extended_start) && (cmd <= send_twice_extended_end))) {
        frame.meta |= TM_DALI_SEND_TWICE;
      }
    }
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Tx 0x%08X %2d DT%d%s"),
    frame.data, frame.meta & TM_DALI_BIT_COUNT_MASK, 
    Dali->device_type, (frame.meta & TM_DALI_SEND_TWICE)?" twice":"");

  Dali->dali->write(frame);                    // Takes 14.7 ms

  if (Dali->device_type && (cmd >= 224) && (cmd <= 255)) {  // Extended commands
    Dali->device_type = 0;                     // Reset after execution of extended command
  }
}

/*-------------------------------------------------------------------------------------------*/

int DaliSendWaitResponse(uint32_t adr, uint32_t cmd, uint32_t timeout = DALI_TIMEOUT);
int DaliSendWaitResponse(uint32_t adr, uint32_t cmd, uint32_t timeout) {
  Dali->dali->flush();
  DaliSendData(adr, cmd);
  while (!Dali->dali->available() && timeout--) {  // Expect backward frame within DALI_TIMEOUT ms (>7Te and <22Te)
    delay(1);
  };
  int result = -1;                             // DALI NO or no response
  DaliFrame frame;
  frame.data = 0;
  frame.meta = 0;
  if (Dali->dali->available()) {
    frame = Dali->dali->read();
    if ((frame.meta & TM_DALI_COLLISION) ||
        (frame.meta != 8)) {
      result = -2;                             // Collision
    }
    else {                         
      result = (frame.data &0xFF);             // Backward frame
    }
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Rx 0x%08X %2d response"), result, frame.meta);

  return result;
}

/*********************************************************************************************\
 * DALI tools
 * 
 * Courtesy of https://github.com/qqqlab/DALI-Lighting-Interface
\*********************************************************************************************/

bool DaliSetDTR(uint32_t dtr, uint32_t adr, uint32_t value) {
  uint8_t dtr_set[3] = { DALI_102_SET_DTR0, DALI_102_SET_DTR1, DALI_102_SET_DTR2 };
  uint8_t dtr_query[3] = { DALI_102_QUERY_CONTENT_DTR0, DALI_102_QUERY_CONTENT_DTR1, DALI_102_QUERY_CONTENT_DTR2 };

  if (dtr > 2) { dtr = 0; }
  uint32_t retry = 3;
  while (retry--) {
    DaliSendData(dtr_set[dtr], value);         // Store value in DTR
    int result = DaliSendWaitResponse(adr | DALI_SELECTOR_BIT, dtr_query[dtr]);  // Get DTR value
    if (result == value) { return true; }
  }
  return false;
}

bool DaliSetValue(uint32_t adr, uint32_t getcmd, uint32_t setcmd, uint32_t v) {
  // Set a parameter value, returns true on success
  adr |= DALI_SELECTOR_BIT;                    // Enable Selector bit
  int current_v = DaliSendWaitResponse(adr, getcmd);  // Get current parameter value
  if (current_v == v) { return true; }         // Already set
  if (!DaliSetDTR(0, adr, v)) { return false; }
  DaliSendData(adr, setcmd);                   // Set parameter value = DTR
  current_v = DaliSendWaitResponse(adr, getcmd);  // Get current parameter value
  if (current_v != v) { return false; }        // Set failed
  return true;
}

bool DaliSetOperatingMode(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_102_QUERY_OPERATING_MODE, DALI_102_SET_OPERATING_MODE, v);
}

bool DaliSetMaxLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_102_QUERY_MAX_LEVEL, DALI_102_SET_MAX_LEVEL, v);
}

bool DaliSetMinLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_102_QUERY_MIN_LEVEL, DALI_102_SET_MIN_LEVEL, v);
}

bool DaliSetSystemFailureLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_102_QUERY_SYSTEM_FAILURE_LEVEL, DALI_102_SET_SYSTEM_FAILURE_LEVEL, v);
}

bool DaliSetPowerOnLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_102_QUERY_POWER_ON_LEVEL, DALI_102_SET_POWER_ON_LEVEL, v);
}

int DaliQueryExtendedVersionNumber(uint32_t adr, uint32_t device_type) {
  DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, device_type);  // Enable Extended command
  return DaliSendWaitResponse(adr | DALI_SELECTOR_BIT, 255); // DALI_xxx_QUERY_EXTENDED_VERSION_NUMBER
}

#ifdef DALI_LIGHT_COLOR_SUPPORT
uint32_t DaliQueryRGBWAF(uint32_t adr) {
  // https://www.dali-alliance.org/tech-notes/device-type-discovery.html
  uint32_t rgbwaf_channels = 0;

  adr |= DALI_SELECTOR_BIT;                    // Enable Selector bit
  int dt = DaliSendWaitResponse(adr, DALI_102_QUERY_DEVICE_TYPE);
  // If the device does not implement any part 2xx device type then the response will be 254;
  // If the device implements one part 2xx device type then the response will be the device type number;
  // If the device implements multiple part 2xx device types then the response will be MASK (0xff).
  // In all other cases returns NO (no response).
  if (255 == dt) {                             // DALI-2
    int next_dt;
    do {
      next_dt = DaliSendWaitResponse(adr, DALI_102_QUERY_NEXT_DEVICE_TYPE);
      // DALI2: If directly preceded by DALI_102_QUERY_DEVICE_TYPE and more than one device type is supported, returns the first and lowest device type number.
      // DALI2: If directly preceded by DALI_102_QUERY_NEXT_DEVICE_TYPE and not all device types have been reported, returns the next lowest device type number.
      // DALI2: If directly preceded by DALI_102_QUERY_NEXT_DEVICE_TYPE and all device types have been reported, returns 254.
      // In all other cases returns NO (no response).
      if (DALI_209_DEVICE_TYPE == next_dt) {
        dt = DALI_209_DEVICE_TYPE;
      }
    } while ((next_dt >= 0) && (next_dt != 254));
  }
  if (dt < 0) {                                // DALI version-1
    if (DaliQueryExtendedVersionNumber(adr, DALI_209_DEVICE_TYPE) >= 0) {  // Color device
      dt = DALI_209_DEVICE_TYPE;
    }
  }
  if (DALI_209_DEVICE_TYPE == dt) {
    uint32_t retry = 3;
    while (retry--) {
      DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
      int colour_type = DaliSendWaitResponse(adr, DALI_209_QUERY_COLOUR_TYPE_FEATURES);
      if (colour_type >= 0) { 
        rgbwaf_channels = (colour_type >> 5) & 0x07;  // RGBWAF channels in bits 5..7
        break;
      }
    }
  }
  return rgbwaf_channels;
}
#endif  // DALI_LIGHT_COLOR_SUPPORT

/*-------------------------------------------------------------------------------------------*/

void DaliInitLight(void) {
  // Taken from Shelly Dali Dimmer ;-)
  uint32_t adr = DALI_BROADCAST_DP | DALI_SELECTOR_BIT;
  DaliSendData(DALI_102_SET_DTR0, DALI_INIT_FADE);  // Fade x second
  DaliSendData(adr, DALI_102_SET_FADE_TIME);
  DaliSendData(DALI_102_SET_DTR0, 0);          // Power off after gear power restore
  DaliSendData(adr, DALI_102_SET_POWER_ON_LEVEL);
  DaliSendData(DALI_102_SET_DTR0, 0xFE);       // Reset all but short circuit
  DaliSendData(adr, DALI_102_SET_SYSTEM_FAILURE_LEVEL);
}

/*********************************************************************************************\
 * DALI commissioning short addresses
 * 
 * Courtesy of https://github.com/qqqlab/DALI-Lighting-Interface
\*********************************************************************************************/

void DaliSetSearchAddress(uint32_t adr) {
  // Set search address
  DaliSendData(DALI_102_SEARCHADDRH, adr>>16);
  DaliSendData(DALI_102_SEARCHADDRM, adr>>8);
  DaliSendData(DALI_102_SEARCHADDRL, adr);
}

/*-------------------------------------------------------------------------------------------*/

void DaliSetSearchAddressDifference(uint32_t adr_new, uint32_t adr_current) {
  // Set search address, but set only changed bytes (takes less time)
  if ( (uint8_t)(adr_new>>16) !=  (uint8_t)(adr_current>>16) ) DaliSendData(DALI_102_SEARCHADDRH, adr_new>>16);
  if ( (uint8_t)(adr_new>>8)  !=  (uint8_t)(adr_current>>8)  ) DaliSendData(DALI_102_SEARCHADDRM, adr_new>>8);
  if ( (uint8_t)(adr_new)     !=  (uint8_t)(adr_current)     ) DaliSendData(DALI_102_SEARCHADDRL, adr_new);
}

/*-------------------------------------------------------------------------------------------*/

bool DaliCompare() {
  // Is the random address smaller or equal to the search address?
  // As more than one device can reply, the reply gets garbled
  uint8_t retry = 2;
  while (retry > 0) {
    // Compare is true if we received any activity on the bus as reply.
    // Sometimes the reply is not registered... so only accept retry times 'no reply' as a real false compare
    int rv = DaliSendWaitResponse(DALI_102_COMPARE, 0x00);
    if (rv == 0xFF) return true;               // Yes reply
    if (rv == -2) return true;                 // Reply but collision
    retry--;
  }
  return false;
}

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliFindAddress(void) {
  // Find addr with binary search
  uint32_t adr = 0x800000;
  uint32_t addsub = 0x400000;
  uint32_t adr_last = adr;
  DaliSetSearchAddress(adr);
  
  while (addsub) {
    DaliSetSearchAddressDifference(adr, adr_last);
    adr_last = adr;
    if (DaliCompare()) {                       // Returns true if searchadr > adr
      adr -= addsub;
    } else {
      adr += addsub;
    }
    addsub >>= 1;
  }
  DaliSetSearchAddressDifference(adr, adr_last);
  adr_last = adr;
  if (!DaliCompare()) {
    adr++;
    DaliSetSearchAddressDifference(adr, adr_last);
  }
  return adr;
}

/*-------------------------------------------------------------------------------------------*/

void DaliProgramShortAddress(uint8_t shortadr) {
  // The slave shall store the received 6-bit address (AAAAAA) as a short address if it is selected.
  DaliSendData(DALI_102_PROGRAM_SHORT_ADDRESS, (shortadr << 1) | DALI_SELECTOR_BIT);

  AddLog(LOG_LEVEL_INFO, PSTR("DLI: Set short address %d"), shortadr +1);
}

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliCommission(uint32_t init_arg, uint32_t max_count) {
  // Based on Shelly DALI Dimmer Gen3 received frames
  // init_arg=11111111 : all without short address
  // init_arg=00000000 : all 
  // init_arg=0AAAAAA1 : only for this shortadr
  // returns number of new short addresses assigned
  DaliSendData(DALI_BROADCAST_DP | DALI_SELECTOR_BIT, DALI_102_RESET); // Turns ON all lights
  uint8_t arr[64];
  uint32_t sa;
  for (sa = 0; sa < 64; sa++) {
    arr[sa] = 0;
  }
  delay(450);                                  // It is not guaranteed that any commands will be received properly within the next 300ms
  DaliSendData(DALI_102_SET_DTR0, 0xFF);
  DaliSendData(DALI_BROADCAST_DP | DALI_SELECTOR_BIT, DALI_102_SET_SHORT_ADDRESS);
  DaliSendData(DALI_102_TERMINATE, 0x00);      // Terminate the DALI_102_INITIALISE command
  delay(15);
  // Start commissioning
  DaliSendData(DALI_102_INITIALISE, init_arg);
  DaliSendData(DALI_102_RANDOMISE, 0x00);
  delay(100);                                  // The new random address shall be available within a time period of 100ms.

  uint32_t cnt = 0;
  while (true) {                               // Find random addresses and assign unused short addresses
    uint32_t adr = DaliFindAddress();
    if (adr > 0xffffff) { break; }             // No more random addresses found -> exit
    for (sa = 0; sa < 64; sa++) {              // Find first unused short address
      if (0 == arr[sa]) { break; }
    }
    if (sa >= 64) { break; }                   // All 64 short addresses assigned -> exit

    arr[sa] = 1;                               // Mark short address as used
    cnt++;
    DaliProgramShortAddress(sa);               // Assign short address
    DaliSendData(DALI_102_WITHDRAW, 0x00);     // Remove the device from the search
    delay(100);
    OsWatchLoop();                             // Feed blocked-loop watchdog
    DaliSendData((sa << 1) | DALI_SELECTOR_BIT, DALI_102_OFF); // Turns OFF latest short address light

    if (cnt >= max_count) { break; }
  }

  delay(100);
  DaliSendData(DALI_102_TERMINATE, 0x00);      // Terminate the DALI_102_INITIALISE command

  uint32_t address = DALI_BROADCAST_DP;
#ifdef USE_LIGHT
  DaliInitLight();
  if (Settings->sbflag1.dali_light) {          // DaliLight 1
    address = DaliTarget2Address(Dali->Settings.target);
  }
#endif  // USE_LIGHT
  DaliSendData(address, Dali->power[0]);       // Restore lights
  return cnt;
}

/*-------------------------------------------------------------------------------------------*/

void DaliToggle(uint32_t adr, uint32_t *count) {
  static uint32_t interval = 0;

  if (*count && TimeReached(interval)) {
    SetNextTimeInterval(interval, 600);
    (*count)--;
    DaliSendData(adr, (*count &1) ? 128 : 0);  // Power toggle
  }
  delay(1);
}

/*********************************************************************************************\
 * DALI Control Gear - Ballast or Sensor / Receiver
 *
 * Implemented servicing of POWER, DIMMER and Color Control as send by Tasmota
\*********************************************************************************************/

void ResponseAppendDali(uint32_t index) {
  char number[12];
  uint8_t dimmer = changeUIntScale(Dali->dimmer[index], 0, 254, 0, 100);
  ResponseAppend_P(PSTR("\"DALI\":{\"Power%s\":\"%s\",\"Dimmer%s\":%d,\"Address\":%d,\"Command\":%d}"), 
    (0==index)?"":itoa(index+100, number, 10), GetStateText(Dali->power[index]),
    (0==index)?"":itoa(index+100, number, 10), dimmer,
    Dali->address, Dali->command);
}

void ResponseDali(uint32_t index) {
  Response_P(PSTR("{"));
  ResponseAppendDali(index);
  ResponseJsonEnd();
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void DaliLoop(void) {
  while (Dali->dali->available()) { 
    uint32_t queue = Dali->dali->available();

    DaliFrame frame = Dali->dali->read();

    uint32_t bit_count = frame.meta & TM_DALI_BIT_COUNT_MASK;
    bool collision = frame.meta & TM_DALI_COLLISION;
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Rx 0x%08X %2d queue %d%s%s"),
      frame.data, bit_count, queue, (8 == bit_count)?" backward":"", (collision)?" collision":"");

    if ((frame.meta != 16) ||                    // Skip backward frames
        (1 == Dali->probe)) {                    // Probe only
      continue;
    }

    Dali->address = (frame.data >> 8) &0xFF;
    Dali->command = frame.data &0xFF;

  #ifdef USE_LIGHT
  #ifdef DALI_LIGHT_COLOR_SUPPORT
    if (DALI_102_SET_DTR0 == Dali->address) { Dali->dtr[0] = Dali->command; }  // Might be Red / White
    else if (DALI_102_SET_DTR1 == Dali->address) { Dali->dtr[1] = Dali->command; }  // Might be Green / Amber
    else if (DALI_102_SET_DTR2 == Dali->address) { Dali->dtr[2] = Dali->command; }  // Might be Blue
    else if (DALI_209_SET_TEMPORARY_RGB_DIMLEVEL == Dali->command) { 
      Dali->color[0] = Dali->dtr[0];             // Red
      Dali->color[1] = Dali->dtr[1];             // Green
      Dali->color[2] = Dali->dtr[2];             // Blue
    }
    else if (DALI_209_SET_TEMPORARY_RGB_DIMLEVEL == Dali->command) { 
      Dali->color[3] = Dali->dtr[0];             // Cold White
      Dali->color[4] = Dali->dtr[1];             // Warm White (Amber)
    }
    else if (DALI_209_ACTIVATE == Dali->command) {
      uint32_t channels = Dali->Settings.light_type -8;
      if ((Dali->target_rgbwaf > 0) && (channels > 0)) {  // Color control
        Dali->address &= 0xFE;                   // Reset DALI_SELECTOR_BIT set
        if (Dali->allow_light && (DaliTarget2Address(Dali->Settings.target) == Dali->address)) {
          if (Settings->sbflag1.dali_light) {    // DaliLight 1
            uint32_t any_color = 0;
            char scolors[20];
            scolors[0] = 0;
            for (uint32_t i = 0; i < channels; i++) {
              any_color += Dali->color[i];
              snprintf_P(scolors, sizeof(scolors), PSTR("%s%02X"), scolors, Dali->color[i]);
            }
            Dali->light_sync = true;             // Block local loop
            if (any_color) {
              if (Settings->save_data) {
                // Postpone save_data during fast color changes which results in exception 0 on ESP8266
                TasmotaGlobal.save_data_counter = 4;
              }
              char scmnd[20];
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLOR " %s"), scolors);
              ExecuteCommand(scmnd, SRC_SWITCH);
            } else {
              ExecuteCommandPower(LightDevice(), 0, SRC_SWITCH);
            }
          }
        }
      }
    } else
  #endif  // DALI_LIGHT_COLOR_SUPPORT
  #endif  // USE_LIGHT
    if (!(Dali->address & DALI_SELECTOR_BIT)) {  // Address
      uint32_t index = DaliSaveState(Dali->address, Dali->command);  // Update dimmer and power
      bool show_response = true;
  #ifdef USE_LIGHT
      if (Dali->allow_light && (DaliTarget2Address(Dali->Settings.target) == Dali->address)) {
        if (Settings->sbflag1.dali_light) {      // DaliLight 1
          // Sync local light settings with DALI bus data
          uint8_t dim_old = changeUIntScale(Dali->last_dimmer, 0, 254, 0, 100);
          uint8_t dim_new = changeUIntScale(Dali->dimmer[index], 0, 254, 0, 100);
          if (Dali->last_power != Dali->power[index]) {
            Dali->light_sync = true;             // Block local loop
            ExecuteCommandPower(LightDevice(), Dali->power[index], SRC_SWITCH);
          }
          else if (dim_old != dim_new) {
            char scmnd[20];
            snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), dim_new);
            Dali->light_sync = true;             // Block local loop
            ExecuteCommand(scmnd, SRC_SWITCH);
          }
          show_response = false;
        }
      }
  #endif  // USE_LIGHT
      if (show_response) {
        ResponseDali(index);
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_PRFX_DALI));
      }
    }
    yield();
  }
}

/*-------------------------------------------------------------------------------------------*/

void DaliEverySecond(void) {
  if (5 == TasmotaGlobal.uptime) {
    DaliInitLight();
  }
}

/*********************************************************************************************\
 * DALI Control Device - Controller  / Transmitter
 *
 * Implements Tasmota light POWER, DIMMER and Color Control if `DaliLight 1`
\*********************************************************************************************/

#ifdef USE_LIGHT
bool DaliSetChannels(void) {
  if (Settings->sbflag1.dali_light) {          // DaliLight 1
    Settings->light_fade = 0;                  // Use Dali fading
    Settings->light_correction = 0;            // Use Dali light correction
    if (Dali->light_sync) {                    // Block local loop
      Dali->light_sync = false;
    } else {
      uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;
      // cur_col[0] = Red, cur_col[1] = Green, cur_col[2] = Blue, cur_col[3] = Cold = White, cur_col[4] = Warm = Amber
      for (uint32_t i = 0; i < 5; i++) {
        if (255 == cur_col[i]) { cur_col[i] = 254; }  // Max Dali value
      }
      uint32_t adr = DaliTarget2Address(Dali->Settings.target);
#ifdef DALI_LIGHT_COLOR_SUPPORT
      uint32_t channels = Dali->Settings.light_type -8;
      if ((Dali->target_rgbwaf > 0) && (channels > 0)) {  // Color control
        adr |= DALI_SELECTOR_BIT;              // Enable Selector bit

#ifdef DALI_POWER_OFF_NO_FADE
        uint32_t power_on = 0;
        for (uint32_t i = 0; i < channels; i++) {
          power_on += cur_col[i];
        }
        if (!power_on) {
          DaliSendData(adr, DALI_102_OFF);     // Power off without fade
          return true;
        }
#endif  // DALI_POWER_OFF_NO_FADE

#ifdef DALI_LIGHT_NO_READ_AFTER_WRITE
        // This takes 310ms for 3 channels but might send bad data as no DTR read-after-write
        DaliSendData(DALI_102_SET_DTR0, 0x7F);           // Linked Channel control
        DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
        DaliSendData(adr, DALI_209_SET_TEMPORARY_RGBWAF_CONTROL);

        DaliSendData(DALI_102_SET_DTR0, cur_col[0]);     // DALI Red
        DaliSendData(DALI_102_SET_DTR1, (channels > 1) ? cur_col[1] : 255);  // DALI Green
        DaliSendData(DALI_102_SET_DTR2, (channels > 2) ? cur_col[2] : 255);  // DALI Blue
        DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
        DaliSendData(adr, DALI_209_SET_TEMPORARY_RGB_DIMLEVEL);

        if (channels > 3) {
          DaliSendData(DALI_102_SET_DTR0, cur_col[3]);   // DALI White
          DaliSendData(DALI_102_SET_DTR1, (channels > 4) ?  cur_col[4] : 255); // DALI Amber
          DaliSendData(DALI_102_SET_DTR2, 255);          // DALI Freecolour - no change
          DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
          DaliSendData(adr, DALI_209_SET_TEMPORARY_WAF_DIMLEVEL);
        }
#else
        // This takes 480ms for 3 channels but might send nothing if DTR read-after-write fails
        if (!DaliSetDTR(0, adr, 0x7F)) { return true; }  // Linked Channel control
        DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
        DaliSendData(adr, DALI_209_SET_TEMPORARY_RGBWAF_CONTROL);

        if (!DaliSetDTR(0, adr, cur_col[0])) { return true; }  // DALI Red
        if (!DaliSetDTR(1, adr, (channels > 1) ? cur_col[1] : 255)) { return true; }  // DALI Green
        if (!DaliSetDTR(2, adr, (channels > 2) ? cur_col[2] : 255)) { return true; }  // DALI Blue
        DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
        DaliSendData(adr, DALI_209_SET_TEMPORARY_RGB_DIMLEVEL);

        if (channels > 3) {
          if (!DaliSetDTR(0, adr, cur_col[3])) { return true; }  // DALI While
          if (!DaliSetDTR(1, adr, (channels > 4) ? cur_col[4] : 255)) { return true; }  // DALI Amber
          if (!DaliSetDTR(2, adr, 255)) { return true; }         // DALI Freecolour - no change
          DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
          DaliSendData(adr, DALI_209_SET_TEMPORARY_WAF_DIMLEVEL);
        }

#endif  // DALI_LIGHT_NO_READ_AFTER_WRITE
        DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
        DaliSendData(adr, DALI_209_ACTIVATE);
        return true;
      }
#endif  // DALI_LIGHT_COLOR_SUPPORT

#ifdef DALI_POWER_OFF_NO_FADE
      if (!cur_col[0]) {
        DaliSendData(adr | DALI_SELECTOR_BIT, DALI_102_OFF);  // Power off without fade
        return true;
      }
#endif  // DALI_POWER_OFF_NO_FADE
      DaliSendData(adr, cur_col[0]);           // DAPC command - dim level
    }
  }
  return true;
}
#endif  // USE_LIGHT

/*********************************************************************************************\
 * DALI Tasmota init
\*********************************************************************************************/

bool DaliInit(uint32_t function) {
  int pin_tx = -1;
  bool invert_tx = false;
  if (PinUsed(GPIO_DALI_TX)) {
    pin_tx = Pin(GPIO_DALI_TX);
  }
  else if (PinUsed(GPIO_DALI_TX_INV)) {
    pin_tx = Pin(GPIO_DALI_TX_INV);
    invert_tx = true;
  }
  int pin_rx = -1;
  bool invert_rx = false;
  if (PinUsed(GPIO_DALI_RX)) {
    pin_rx = Pin(GPIO_DALI_RX);
  }
  else if (PinUsed(GPIO_DALI_RX_INV)) {
    pin_rx = Pin(GPIO_DALI_RX_INV);
    invert_rx = true;
  }
  if ((-1 == pin_tx) || (-1 == pin_rx)) { return false; }

  Dali = (DALI*)calloc(sizeof(DALI), 1);    // Need calloc to reset registers to 0/false
  if (!Dali) { return false; }
  DaliSettingsLoad(0);

  for (uint32_t i = 0; i < DALI_MAX_STORED; i++) {
    Dali->dimmer[i] = DALI_INIT_STATE;
  }

  Dali->dali = new TasmotaDali(pin_rx, pin_tx, invert_rx, invert_tx);
  if (!Dali->dali->begin()) { return false; }

  Dali->allow_light = (FUNC_MODULE_INIT == function);  // Light control is possible

  AddLog(LOG_LEVEL_INFO, PSTR("DLI: GPIO%d(RX%s) and GPIO%d(TX%s)%s"),
    pin_rx, (invert_rx)?"i":"", pin_tx, (invert_tx)?"i":"", (Dali->allow_light)?" as light":"");

#ifdef DALI_DEBUG
  Dali->log_level = LOG_LEVEL_DEBUG;
#endif  // DALI_DEBUG

  if (!Dali->allow_light) {
    Settings->sbflag1.dali_light = false;      // No light control possible
  }
#ifdef USE_LIGHT
  if (!Settings->sbflag1.dali_light) {         // DaliLight 0
    return false;
  }

  Settings->light_fade = 0;                    // Use Dali fading instead
  Settings->light_correction = 0;              // Use Dali light correction
  UpdateDevicesPresent(1);

  TasmotaGlobal.light_type = LT_W;             // Single channel
#ifdef DALI_LIGHT_COLOR_SUPPORT
  Dali->target_rgbwaf = DaliQueryRGBWAF(DaliTarget2Address(Dali->Settings.target));
  if (Dali->target_rgbwaf > 1) {
    TasmotaGlobal.light_type = Dali->Settings.light_type;
    if (!Settings->flag3.pwm_multi_channels &&   // SetOption68 0 - Enable multi-channels PWM instead of Color PWM
       (TasmotaGlobal.light_type >= LT_RGBW) &&  // RGBW or RGBCW
       (Settings->param[P_RGB_REMAP] & 128)) {   // SetOption37 128
      UpdateDevicesPresent(1);                   // We manage RGB and W separately, hence adding a device
    }
  }
#endif  // DALI_LIGHT_COLOR_SUPPORT

  return true;
#else
  return false;
#endif  // USE_LIGHT
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kDALICommands[] PROGMEM = D_PRFX_DALI "|"  // Prefix
  "|" D_CMND_POWER "|" D_CMND_DIMMER "|Target"
#ifdef USE_LIGHT
  "|Light|Channels"
#endif  // USE_LIGHT
  "|Send|Query|Scan|Group|GroupSliders|Gear";

void (* const DALICommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliPower, &CmndDaliDimmer, &CmndDaliTarget,
#ifdef USE_LIGHT
  &CmndDaliLight, &CmndDaliChannels,
#endif  // USE_LIGHT
  &CmndDaliSend, &CmndDaliQuery, &CmndDaliScan, &CmndDaliGroup, &CmndDaliGroupSliders, &CmndDaliGear };

bool DaliJsonParse(void) {
  // {"addr":254,"cmd":100}
  // {"addr":2}
  // {"dim":3}

  bool served = false;
  JsonParser parser((char *)XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (root) {
    int DALIindex = 0;
    int ADRindex = 0;
    int8_t DALIdim = -1;
    uint32_t DALIaddr = DALI_BROADCAST_DP;

    JsonParserToken val = root[PSTR("cmd")];
    if (val) {
      uint32_t cmd = val.getUInt();
      val = root[PSTR("addr")];
      if (val) {
        uint32_t addr = val.getUInt();
        AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: cmd = %d, addr = %d"), cmd, addr);
        DaliSendData(addr, cmd);
        return true;
      } else {
        return false;
      }
    }
    val = root[PSTR("addr")];
    if (val) {
      uint32_t addr = val.getUInt();
      if ((addr >= 0) && (addr < 64)) {
        DALIaddr = addr  << 1;
      }
    }
    val = root[PSTR("dim")];
    if (val) {
      uint8_t dim = val.getUInt();
      if (dim < 255) {
        DALIdim = dim;
      }
    }
    DaliSendData(DALIaddr, DALIdim);
    served = true;
  }
  return served;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void CmndDali(void) {
  // Dali {"addr":254,"cmd":100} - Any address and/or command
  // Dali 0|1                    - Enable DALI receive probe
  // Dali 2                      - Disable debug log output
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      Dali->probe = XdrvMailbox.payload;
#ifdef DALI_DEBUG
      if (0 == Dali->probe) {
        Dali->log_level = LOG_LEVEL_DEBUG;
      }
      else if (2 == Dali->probe) {
        Dali->log_level = LOG_LEVEL_DEBUG_MORE +1;
      }
#endif  // DALI_DEBUG
      ResponseCmndNumber(Dali->probe);
      return;
    }
    DaliJsonParse();
  }
  ResponseDali(0);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliTarget(void) {
  // DaliTarget          - Set transmit target
  // DaliTarget 0        - Set target to broadcast address
  // DaliTarget 1..64    - Set target to short address
  // DaliTarget 101..116 - Set target to group address
  if (((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 64)) ||
      ((XdrvMailbox.payload >= 101) && (XdrvMailbox.payload <= 116)) ||
      (XdrvMailbox.payload == 0)) {
    Dali->Settings.target = XdrvMailbox.payload;
  }
#ifdef DALI_LIGHT_COLOR_SUPPORT
  Dali->target_rgbwaf = DaliQueryRGBWAF(DaliTarget2Address(Dali->Settings.target));
#endif  // DALI_LIGHT_COLOR_SUPPORT
  ResponseCmndNumber(Dali->Settings.target);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliPower(void) {
  // DaliPower 0       - Broadcast power off
  // DaliPower 1       - Broadcast power on to last dimmer state
  // DaliPower 2       - Broadcast toggle power off or last dimmer state
  // DaliPower 3..254  - Broadcast equals DaliDimmer command
  // DaliPower 0..254  - Broadcast control
  // DaliPower0 0..254 - Broadcast control (= DaliPower)
  // DaliPower1 0..254 - Short address 0 control
  // DaliPower3 0..254 - Short address 2 control
  uint32_t index = 0;                          // Broadcast
  if ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116)) {
    index = XdrvMailbox.index - 100;           // Group1 to 16
  }
  if (((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= 64)) ||
      ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116))) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 254)) {
      if (XdrvMailbox.payload <= 2) {
        if (2 == XdrvMailbox.payload) {
          XdrvMailbox.payload = (Dali->power[index]) ? 0 : 1;
        }
        if (1 == XdrvMailbox.payload) {
          XdrvMailbox.payload = Dali->dimmer[index];
        }
      }
      uint32_t adr = DaliTarget2Address(XdrvMailbox.index);
#ifdef DALI_POWER_OFF_NO_FADE
      if (!XdrvMailbox.payload) {
        DaliSendData(adr | DALI_SELECTOR_BIT, DALI_102_OFF);  // Power off without fade
      } else
#endif  // DALI_POWER_OFF_NO_FADE
      DaliSendData(adr, XdrvMailbox.payload);  // DAPC command - dim level
    }
  }
  ResponseDali(index);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliDimmer(void) {
  // DaliDimmer 0..100  - Broadcast set power off or dimmer state
  // DaliDimmer0 0..100 - Broadcast set power off or dimmer state
  // DaliDimmer1 0..100 - Short address 0 set power off or dimmer state
  // DaliDimmer3 0..100 - Short address 2 set power off or dimmer state
  uint32_t index = 0;                          // Broadcast
  if ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116)) {
    index = XdrvMailbox.index - 100;           // Group1 to 16
  }
  if (((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= 64)) ||
      ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116))) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      uint8_t dimmer = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 254);
      uint32_t adr = DaliTarget2Address(XdrvMailbox.index);
#ifdef DALI_POWER_OFF_NO_FADE
      if (!dimmer) {
        DaliSendData(adr | DALI_SELECTOR_BIT, DALI_102_OFF);  // Power off without fade
      } else
#endif  // DALI_POWER_OFF_NO_FADE
      DaliSendData(adr, dimmer);               // DAPC command - dim level
    }
  }
  ResponseDali(index);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliGroup(void) {
  // DaliGroup1 1,2   - Add device 1 and 2 to group 1
  // DaliGroup1 -1,2  - Remove device 1 and 2 to group 1
  // DaliGroup1 b     - Blink group devices twice
  if ((XdrvMailbox.index >= 1) && (XdrvMailbox.index <= 16)) {
    uint32_t group = XdrvMailbox.index -1;
    bool more = false;
    char temp[200] = { 0 };
    uint32_t tcount = 0;
    uint32_t adr = DaliTarget2Address(group + 101);
    if (XdrvMailbox.data_len) {
      if ('b' == XdrvMailbox.data[0]) {        // Blink devices
        tcount = 4;
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
      }
    }
    if (XdrvMailbox.data_len) {
      uint32_t command = DALI_102_ADD_TO_GROUP0;
      temp[0] = '+';
      if ('+' == XdrvMailbox.data[0]) {        // Add devices
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
      }
      else if ('-' == XdrvMailbox.data[0]) {   // Remove devices
        command = DALI_102_REMOVE_FROM_GROUP0;
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
        temp[0] = '-';
      }
      uint32_t argc = ArgC();                  // Number of devices
      if (argc) {
        command |= group;
        uint32_t sas[argc];
        ParseParameters(argc, sas);
        for (uint32_t arg = 0; arg < argc; arg++) {
          uint32_t sa = sas[arg] -1;
          if (sa < 64) {
            snprintf_P(temp, sizeof(temp), PSTR("%s%s%d"), temp, (more)?",":"", sa +1);
            more = true;
            DaliSendData((sa << 1) | DALI_SELECTOR_BIT, command);
          }
        }
        while (tcount) { DaliToggle(adr, &tcount); }
        ResponseCmndIdxChar(temp);
      }
    } else {
      uint32_t command = DALI_102_QUERY_GROUPS_0_7;
      uint32_t bitmask = 1 << group;
      if (group > 7) {
        command = DALI_102_QUERY_GROUPS_8_15;
        bitmask = 1 << group - 8;
      }
      for (uint32_t sa = 0; sa < Dali->Settings.max_gear; sa++) {   // Scanning 64 addresses takes about 2500 ms
        if (tcount) { DaliToggle(adr, &tcount); }
        int result = DaliSendWaitResponse((sa << 1) | DALI_SELECTOR_BIT, command, 20);
        if ((result >= 0) && (result & bitmask)) {
          snprintf_P(temp, sizeof(temp), PSTR("%s%s%d"), temp, (more)?",":"", sa +1);
          more = true;
        }
      }
      if (!strlen(temp)) {
        snprintf_P(temp, sizeof(temp), PSTR("None"));
      } else {
        while (tcount) { DaliToggle(adr, &tcount); }
      }
      ResponseCmndIdxChar(temp);
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliGear(void) {
  // DaliGear[2] [<max_address>|<address>] - Scan bus for up to <max_address> devices and toggle output twice
  // DaliGear                              - Scan bus for 64 devices taking around 2.5 sec
  // DaliGear 15                           - Scan bus for up to 15 devices
  // DaliGear2                             - Scan bus and toggle output twice
  // DaliGear2 4                           - Toggle output twice for device 4 only
  uint32_t toggle_count = 0;
  uint32_t start = 0;
  uint32_t end = Dali->Settings.max_gear;
  uint32_t payload = ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 64)) ? XdrvMailbox.payload : 0;
  if (1 == XdrvMailbox.index) {
    if (payload) {
      end = payload;
      Dali->Settings.max_gear = end;
    }
  }
  else if (2 == XdrvMailbox.index) {
    toggle_count = 4;
    if (payload) {
      start = payload -1;
      end = payload;
    }
  }
  char temp[200] = { 0 };
  uint32_t count = 0;
  for (uint32_t address = start; address < end; address++) {  // Scanning 64 addresses takes about 2500 ms
    uint32_t adr = address << 1;
    uint32_t tcount = toggle_count;
    if (DaliSendWaitResponse(adr | DALI_SELECTOR_BIT, DALI_102_QUERY_CONTROL_GEAR_PRESENT, 20) >= 0) {
      snprintf_P(temp, sizeof(temp), PSTR("%s%s%d"), temp, (count)?",":"", address +1);
      count++;
      AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Device %d at %d, short address %d"), count, address, adr);
      while (tcount) { DaliToggle(adr, &tcount); }
    }
  }
  ResponseCmnd();
  ResponseAppend_P(PSTR("%d,\"Present\":%d,\"Address\":[%s]}"), Dali->Settings.max_gear, count, temp);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliSend(void) {
  // Send command
  // DaliSend 0xa5,255           - DALI Initialise
  // DaliSend 6,3,0xe2           - DALI DT6 (6) for address 1 (3) extended command disable current protector (0xe2) 
  // DaliSend 0x01,0xa3,0x2d,254 - Set Power On level (0x2d) for address 0 (0x01) to 254 only if Read Power On level (0xa3) is different

  uint32_t values[5] = { 0 };
  uint32_t params = ParseParameters(5, values);

#ifdef DALI_DEBUG
  AddLog(Dali->log_level, PSTR("DLI: index %d, params %d, values %i,%i,%i,%i,%i"), XdrvMailbox.index, params, values[0], values[1], values[2], values[3], values[4]);
#endif  // DALI_DEBUG

  if (255 == XdrvMailbox.index) {                   // DaliSend255 <bitcount>,<value> - Dali-2 24-bit frame
    if (params >= 2) {
      DaliSendData(values[0] | TM_DALI_EVENT_FRAME, values[1]);
      ResponseCmndDone();
    }
    return;
  }
  if (DALI_207_DEVICE_TYPE == XdrvMailbox.index) {  // DaliSend6 - DT6 = 207 = Extended LED commands 224...236
    /*
    params    0                                               1                                2
    DaliSend6 <broadcast>|<device>|<group> |<special_command>,<command>|<special_command_data>,<dtr0_data>
              254         0..126   128..158 160..251          0..255    0..255                 0..255
    DaliSend6 <broadcast>|<device>|<group>,<command>
    */
    if ((params >= 2) && (values[1] >= 224) && (values[1] <= 255)) {  // DT6 extended command
      uint32_t adr = values[0] | DALI_SELECTOR_BIT;
      if ((DALI_207_SELECT_DIMMING_CURVE == values[1]) ||
          (DALI_207_SET_FAST_FADE_TIME == values[1])) {
        // DaliSend6 <broadcast>|<device>|<group>,<command>,<dtr0>
        if (!DaliSetDTR(0, adr, values[2])) { return; }
      }
      DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_207_DEVICE_TYPE);  // Enable Extended command
      DaliSendData(adr, values[1]);
      ResponseCmndDone();
      return;
    }
  }
  if (DALI_209_DEVICE_TYPE == XdrvMailbox.index) {  // DaliSend8 - DT8 = 209 = Extended colour commands 224...246
    /*
    params    0                                               1                                2                                     3                       4
    DaliSend8 <broadcast>|<device>|<group> |<special_command>,<command>|<special_command_data>,<dtr0_data>|<dtr0_1_data>|<dtr2_data>,<dtr1_data>|<dtr2_data>,<dtr2_data>
              254         0..126   128..158 160..251          0..255    0..255                 0..255      0..65535      0..255      0..255      0..255      0..255
    DaliSend8 <special_command>,<special_command_data>
    DaliSend8 <broadcast>|<device>|<group>,<command>
    */
    if ((params >= 2) && (values[1] >= 224) && (values[1] <= 255)) {  // DT8 extended command
      uint32_t adr = values[0] | DALI_SELECTOR_BIT;
      // params == 3
      if ((DALI_209_SET_TEMPORARY_RGBWAF_CONTROL == values[1]) ||
          (DALI_209_STORE_GEAR_FEATURES_STATUS == values[1]) ||
          (DALI_209_ASSIGN_COLOUR_TO_LINKED_COMMAND == values[1])) {
        // DaliSend8 <broadcast>|<device>|<group>,<command>,<dtr0_data>
        if (!DaliSetDTR(0, adr, values[2])) { return; }
      }
      else if ((DALI_209_SET_TEMPORARY_X_COORDINATE == values[1]) ||
               (DALI_209_SET_TEMPORARY_Y_COORDINATE == values[1]) ||
               (DALI_209_SET_TEMPORARY_COLOUR_TEMP_TC == values[1])) {
        // DaliSend8 <broadcast>|<device>|<group>,<command>,<dtr0_1_data>
        if (!DaliSetDTR(0, adr, values[2] % 256)) { return; }
        if (!DaliSetDTR(1, adr, values[2] / 256)) { return; }
      }
      else if (DALI_209_STORE_XY_COORDINATE_PRIMARY_N == values[1]) {
        // DaliSend8 <broadcast>|<device>|<group>,<command>,<dtr2_data>
        if (!DaliSetDTR(2, adr, values[2])) { return; }
      }
      // params == 4
      else if ((DALI_209_SET_TEMPORARY_PRIMARY_N_DIMLEVEL == values[1]) ||
               (DALI_209_STORE_TY_PRIMARY_N == values[1]) ||
               (DALI_209_STORE_COLOUR_TEMP_TC_LIMIT == values[1])) {
        // DaliSend8 <broadcast>|<device>|<group>,<command>,<dtr0_1_data>,<dtr2_data>
        if (!DaliSetDTR(0, adr, values[2] % 256)) { return; }
        if (!DaliSetDTR(1, adr, values[2] / 256)) { return; }
        if (!DaliSetDTR(2, adr, values[3])) { return; }
      }    
      // params == 5
      else if ((DALI_209_SET_TEMPORARY_RGB_DIMLEVEL == values[1]) ||
               (DALI_209_SET_TEMPORARY_WAF_DIMLEVEL == values[1])) {
        // DaliSend8 <broadcast>|<device>|<group>,<command>,<dtr0_data>,<dtr1_data>,<dtr2_data>
        if (!DaliSetDTR(0, adr, values[2])) { return; }
        if (!DaliSetDTR(1, adr, values[3])) { return; }
        if (!DaliSetDTR(2, adr, values[4])) { return; }
      }
      DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
      DaliSendData(adr, values[1]);
      ResponseCmndDone();
      return;
    }
  }
  /*
  params    0     1                                               2                                3             4
  DaliSend [<dt>,]<broadcast>|<device>|<group> |<special_command>,<command>|<special_command_data>,<dtr0_1_data>,<dtr2_data>
            6,8   254         0..126   128..158 160..251          0..255    0..255                 0..255/65535  0..255
  DaliSend <special_command>,<special_command_data>
  DaliSend <broadcast>|<device>|<group>,<command>
  DaliSend <broadcast>|<device>|<group>,<command>,<dtr0>
  */
  if (2 == params) {                           // Prepare for default Extended command DT6 - LEDs
    if ((values[1] >= 224) && (values[1] <= 255)) {  // Extended command
      values[2] = values[1];
      values[1] = values[0];
      values[0] = DALI_207_DEVICE_TYPE;        // Default to DT6 - LEDs
      params = 3;
    }
  }
  if (3 == params) {                           // Set extended command mode
    DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, values[0]);  // Enable Extended command
    values[0] = values[1];
    values[1] = values[2];
    params = 2;
  }
  uint32_t adr = values[0];
  if (XdrvMailbox.index > 1) { adr |= DALI_SELECTOR_BIT; }
  if (2 == params) {
    DaliSendData(adr, values[1]);
    ResponseCmndDone();
  }
  else if (4 == params) {
    if (DaliSetValue(adr, values[1], values[2], values[3])) {
      ResponseCmndDone();
    } else {
      ResponseCmndFailed();
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliQuery(void) {
  // Send command and return response or -1 (no response within DALI_TIMEOUT)
  // DaliQuery 0xff,0x90 - DALI Query status
  // DaliQuery 0xff,144  - DALI Query status
  // DaliQuery 6,7,237   - DALI DT6 (6) for address 3 (7) extended command query status (237)
  uint32_t values[3] = { 0 };
  uint32_t params = ParseParameters(3, values);

  if (DALI_207_DEVICE_TYPE == XdrvMailbox.index) {  // DaliQuery6 - DT6 = 207 = Extended LED commands 224...236
    if ((params >= 2) && (values[1] >= 224) && (values[1] <= 255)) {  // DT6 extended command
      uint32_t adr = values[0] | DALI_SELECTOR_BIT;
      DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_207_DEVICE_TYPE);  // Enable Extended command
      int result = DaliSendWaitResponse(adr, values[1]);
      ResponseCmndNumber(result);
      return;
    }
  }
  if (DALI_209_DEVICE_TYPE == XdrvMailbox.index) {  // DaliQuery8 - DT8 = 209 = Extended colour commands 224...246
    if ((params >= 2) && (values[1] >= 224) && (values[1] <= 255)) {  // DT8 extended command
      uint32_t adr = values[0] | DALI_SELECTOR_BIT;
      if (DALI_209_QUERY_COLOUR_VALUE == values[1]) {
        if (!DaliSetDTR(0, adr, values[2])) { return; }
      }
      DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, DALI_209_DEVICE_TYPE);  // Enable Extended command
      int result = DaliSendWaitResponse(adr, values[1]);
      if (DALI_209_QUERY_COLOUR_VALUE == values[1]) {
        if (result >= 0) {
          uint32_t result2 = result << 8;
          int result = DaliSendWaitResponse(adr, DALI_102_QUERY_CONTENT_DTR0);
          if (result >= 0) { 
            result = result2 | result;
          }
        }
      }
      ResponseCmndNumber(result);
      return;
    }
  }
  if (2 == params) {
    if ((values[1] >= 224) && (values[1] <= 255)) {  // Extended command
      values[2] = values[1];
      values[1] = values[0];
      values[0] = DALI_207_DEVICE_TYPE;        // Default to DT6 - LEDs
      params = 3;
    }
  }
  if (3 == params) {
    DaliSendData(DALI_102_ENABLE_DEVICE_TYPE_X, values[0]);  // Enable Extended command
    values[0] = values[1];
    values[1] = values[2];
    params = 2;
  }
  if (2 == params) {
    uint32_t adr = values[0] | DALI_SELECTOR_BIT;
    int result = DaliSendWaitResponse(adr, values[1]);
    ResponseCmndNumber(result);
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliScan(void) {
  // Scan short addresses
  // DaliScan 1     - Reset and commission short addresses
  // DaliScan 2     - Commission unassigned short addresses
  // DaliScan x,5   - Commission up to 5 short addresses
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  if ((values[0] >= 1) && (values[0] <= 2)) {
    uint32_t init_arg = 0x00;                  // Commission all
    if (2 == values[0]) {
      init_arg = 0xFF;                         // Commission all without short addresses
    }
    int result = DaliCommission(init_arg, (0 == values[1]) ? 64 : values[1]);
    ResponseCmndNumber(result);
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliGroupSliders(void) {
  // DaliGroupSliders 0..16  - Add group sliders
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 16)) {
    Settings->mbflag2.dali_group_sliders = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;            // Restart to update GUI
  }
  ResponseCmndNumber(Settings->mbflag2.dali_group_sliders);
}

#ifdef USE_LIGHT
/*-------------------------------------------------------------------------------------------*/

void CmndDaliLight(void) {
  // DaliLight 0  - Disable light controls
  // DaliLight 1  - Enable light controls
  if (Dali->allow_light && (XdrvMailbox.data_len > 0)) {
    Settings->sbflag1.dali_light = XdrvMailbox.payload &1;  // DaliLight 0/1
    TasmotaGlobal.restart_flag = 2;            // Restart to update GUI
  }
  ResponseCmndStateText(Settings->sbflag1.dali_light);  // DaliLight 0/1
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliChannels(void) {
  // DaliChannels    - Show amount of color channels
  // DaliChannels 1  - Set amount of color channels to R
  // DaliChannels 2  - Set amount of color channels to RG
  // DaliChannels 3  - Set amount of color channels to RGB
  // DaliChannels 4  - Set amount of color channels to RGBW
  // DaliChannels 5  - Set amount of color channels to RGBWC
  if (Dali->allow_light && ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 5))) {
    Dali->Settings.light_type = XdrvMailbox.payload + 8;
    TasmotaGlobal.restart_flag = 2;            // Restart to update GUI
  }
  ResponseCmndNumber(Dali->Settings.light_type -8);
}
#endif  // USE_LIGHT

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_MSG_SLIDER_DALI[] PROGMEM =
  "<tr>"
  "<td style='width:15%%'><button id='k75%d' style='background:#%06x;' onclick='la(\"&k75=%d\");'>%s%s</button></td>"
  "<td style='width:85%%'><div class='r' style='background-image:linear-gradient(to right,#000,#FFF);'>"
  "<input id='i75%d' type='range' min='1' max='100' value='%d' onchange='lc(\"i\",75%d,value)'></div></td>"
  "</tr>";

void DaliWebAddMainSlider(void) {
  WSContentSend_P(HTTP_TABLE100);
  char number[12];
  for (uint32_t i = Settings->sbflag1.dali_light; i <= Settings->mbflag2.dali_group_sliders; i++) {  // DaliLight 0/1, DaliGroupSliders
    Dali->web_dimmer[i] = Dali->dimmer[i];
    WSContentSend_P(HTTP_MSG_SLIDER_DALI,      // Brightness - Black to White
      i,                                       // k75<i>
      WebColor((Dali->power[i]) ? COL_BUTTON : COL_BUTTON_OFF),
      i,                                       // k75=<i>
      (0==i)?"B":"G",                          // B (Broadcast) or G1 to G16 (Group)
      (0==i)?"":itoa(i, number, 10),
      i,                                       // i75<i>
      changeUIntScale(Dali->web_dimmer[i], 0, 254, 0, 100),
      i                                        // i75<i>
    );
  }
  WSContentSend_P(PSTR("</table>"));
}

/*********************************************************************************************/

void DaliWebGetArg(void) {
  char tmp[8];                                 // WebGetArg numbers only
  char svalue[32];                             // Command and number parameter
  char webindex[8];                            // WebGetArg name

  uint32_t index;
  for (uint32_t i = Settings->sbflag1.dali_light; i <= Settings->mbflag2.dali_group_sliders; i++) {  // DaliLight 0/1, DaliGroupSliders
    snprintf_P(webindex, sizeof(webindex), PSTR("i75%d"), i);
    WebGetArg(webindex, tmp, sizeof(tmp));     // 0 - 100 percent
    if (strlen(tmp)) {
      index = i;
      if (index > 0) { index += 100; }         // Group
      snprintf_P(svalue, sizeof(svalue), PSTR("DaliDimmer%d %s"), index, tmp);
      ExecuteWebCommand(svalue);
    }
  }
  WebGetArg(PSTR("k75"), tmp, sizeof(tmp));
  if (strlen(tmp)) {
    index = atoi(tmp);
    if (index > 0) { index += 100; }           // Group
    snprintf_P(svalue, sizeof(svalue), PSTR("DaliPower%d 2"), index);
    ExecuteWebCommand(svalue);
  }
}

/*********************************************************************************************/

void DaliWebShow(void) {
  WSContentSend_P(PSTR("</table>"));         // Terminate current {t}
  WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);  // "<img style='display:none;' src onerror=\""
  for (uint32_t i = Settings->sbflag1.dali_light; i <= Settings->mbflag2.dali_group_sliders; i++) {  // DaliLight 0/1, DaliGroupSliders
    WSContentSend_P(PSTR("eb('k75%d').style='background:#%06x';"),
      i, WebColor((Dali->power[i]) ? COL_BUTTON : COL_BUTTON_OFF));
    if (Dali->dimmer[i] != Dali->web_dimmer[i]) {
      if (WebUpdateSliderTime()) {
        Dali->web_dimmer[i] = Dali->dimmer[i];
      }
      WSContentSend_P(PSTR("eb('i75%d').value='%d';"),
        i, changeUIntScale(Dali->dimmer[i], 0, 254, 0, 100));
    }
  }
  WSContentSend_P(PSTR("\">{t}"));           // Restart {t} = <table style='width:100%'>
  WSContentSeparator(3);                     // Don't print separator on next WSContentSeparator(1)
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv75(uint32_t function) {
  bool result = false;

  if (FUNC_MODULE_INIT == function) {          // Try to claim DALI as light
    result = DaliInit(function);
  }
  else if ((FUNC_PRE_INIT == function) && !Dali) {  // If claim failed then use DALI controls only
    DaliInit(function);
  }
  else if (Dali) {
    switch (function) {
      case FUNC_LOOP:
        DaliLoop();
        break;
      case FUNC_EVERY_SECOND:
        DaliEverySecond();
        break;
      case FUNC_RESET_SETTINGS:
        DaliSettingsLoad(1);
        break;
      case FUNC_SAVE_SETTINGS:
        DaliSettingsSave();
        break;
#ifdef USE_LIGHT
      case FUNC_SET_CHANNELS:
        result = DaliSetChannels();
        break;
#endif  // USE_LIGHT
      case FUNC_JSON_APPEND:
        ResponseAppend_P(PSTR(","));
        ResponseAppendDali(0);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        DaliWebShow();
        break;
      case FUNC_WEB_ADD_MAIN_BUTTON:
        DaliWebAddMainSlider();
        break;
      case FUNC_WEB_GET_ARG:
        DaliWebGetArg();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kDALICommands, DALICommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_DALI
