/*
  xdrv_86_esp32_sonoff_spm.ino - Sonoff SPM support for Tasmota

  Copyright (C) 2021  Theo Arends

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

//#define USE_SONOFF_SPM

#ifdef ESP32
#ifdef USE_ENERGY_SENSOR
#ifdef USE_SONOFF_SPM
/*********************************************************************************************\
 * Sonoff Stackable Power Manager
 *
 * {"NAME":"Sonoff SPM","GPIO":[0,0,0,0,3200,5536,0,0,672,704,736,0,3232,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,544,0,0,32,0,0,0,0],"FLAG":0,"BASE":1}
 *
 * Things to know:
 * Bulk of the action is handled by ARM processors present in every unit communicating over modbus RS-485.
 * Each SPM-4Relay has 4 bistable relays with their own CSE7761 energy monitoring device handled by an ARM processor.
 * Green led is controlled by ARM processor indicating SD-Card access.
 * ESP32 is used as interface between eWelink and ARM processor in SPM-Main unit communicating over proprietary serial protocol.
 * Power on sequence for two SPM-4Relay modules is 00-00-15-10-(0F)-(13)-(13)-(19)-0C-09-04-[25]-09-04-[25]-0B-0B
 * Up to 180 days of daily energy are stored on the SD-Card. Previous data is lost.
 * Tasmota support is based on Sonoff SPM v1.0.0 ARM firmware.
 * Energy history cannot be guaranteed using either SD-Card or internal flash. As a solution Tasmota stores the total energy and yesterday energy just after midnight.
 *
 * Tasmota features:
 * - Up to 8 SPM-4Relay units supporting a total of 32 relays.
 * - Button on SPM-Main initiates re-scan of SPM-4Relay units.
 * - SPI master to ARM (ARM firmware upload from ESP using EasyFlash not supported).
 * - Ethernet support.
 * - Gui rotating energy display for 4 relays at a time.
 * - Gui optimized for energy display.
 * - Blue led equals Tasmota WiFi status.
 * - Yellow led lights if no ARM connection can be made.
 * - Yellow led blinks 2 seconds if an ARM-ESP comms CRC error is detected.
 * - Persistence for module mapping, total energy and energy yesterday
 * - Supported commands:
 *   SspmDisplay 0|1|2          - Select alternative GUI rotating display either all (0), powered on only (1) or user selected (2)
 *   SspmDump 0|1               - Select shortenend (0) or full (1) serial receive buffer dumps
 *   SspmEnergyTotal<relay>     - (p)reset total energy without today's energy
 *   SspmEnergyYesterday<relay> - (p)reset energy yesterday
 *   SspmHistory<relay>         - Retrieve daily energy of last six month (as defined by ARM firmware)
 *   SspmIAmHere<relay>         - Blink ERROR in SPM-4Relay where relay resides
 *   SspmLog<relay> [x]         - Retrieve relay power state change and cause logging
 *   SspmMap 0                  - Start a scan to fill default mapping
 *   SspmMap 2,3,1,..           - Map scanned module number to physical module number using positional numbering
 *   SspmOverload<relay> 0      - Set default overload detection parameters as read from module during initial scan
 *   SspmOverload<relay> <delay>,<min_power>,<max_power>,<min_voltage>,<max_voltage,<max_current>
 *   SspmOverload<relay> 0,0.10,4400.00,0.10,240.00,20.00 - Set default values using comma separated options
 *   SspmOverload<relay> 0 0.10 4400.00 0.10 240.00 20.00 - Set default values using space separated options
 *   SspmOverload<relay> 10,12.3                          - Enable overload detection after 10 seconds for MinPower
 *   SspmOverload<relay> 10,0,22.2                        - Enable overload detection after 10 seconds for MaxPower
 *   SspmOverload<relay> 10,0,0,0,235.2                   - Enable overload detection after 10 seconds for MaxVoltage
 *   SspmScan                   - Rescan ARM modbus taking around 20 seconds
 *   SspmReset 1                - Reset ARM and restart ESP
 *
 * Todo:
 * Gui for Overload Protection entry (is handled by ARM processor).
 * Gui for Scheduling entry (is handled by ARM processor).
 *
 * Nice to have:
 * Support for all 32 SPM-4Relay units equals 128 relays (restricted due to internal Tasmota register use)
 *
 * GPIO's used:
 * GPIO00 - Bootmode / serial flash
 * GPIO01 - Serial console TX (921600bps8N1 originally)
 * GPIO03 - Serial console RX
 * GPIO04 - ARM processor TX (115200bps8N1)
 * GPIO05 - ETH POWER
 * GPIO12 - SPI MISO to MOSI ARM output (pin36 - PB15)
 * GPIO13 - SPI MOSI to MISO ARM input (pin35 - PB14)
 * GPIO14 - SPI SCLK to ARM input (ARM pin34 - PB13)
 * GPIO15 - ARM reset (output) - 18ms low active 125ms after restart esp32
 * GPIO16 - ARM processor RX
 * GPIO17 - EMAC_CLK_OUT_180
 * GPIO18 - ETH MDIO
 * GPIO19 - EMAC_TXD0(RMII)
 * GPIO21 - EMAC_TX_EN(RMII)
 * GPIO22 - EMAC_TXD1(RMII)
 * GPIO23 - ETH MDC
 * GPIO25 - EMAC_RXD0(RMII)
 * GPIO26 - EMAC_RXD1(RMII)
 * GPIO27 - EMAC_RX_CRS_DV
 * GPIO32 - Blue status led2
 * GPIO33 - Yellow error led3
 * GPIO35 - Button
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO17_OUT
 * #define ETH_ADDRESS       0
 *
 * Variables used:
 * module  = 0 to 31                   SPM-4Relays
 * channel = 0 to 3 or 01, 02, 04, 08  Bitmask of four relays in module
 * relay   = 0 to 127                  Relays
\*********************************************************************************************/

#ifndef SSPM_JSON_ENERGY_TODAY
#define SSPM_JSON_ENERGY_TODAY               // Show JSON energy today
#endif
#ifndef SSPM_JSON_ENERGY_YESTERDAY
#define SSPM_JSON_ENERGY_YESTERDAY           // Show JSON energy yesterday
#endif

/*********************************************************************************************\
 * Fixed defines - Do not change
\*********************************************************************************************/

#define XDRV_86                      86

#define SSPM_MAX_MODULES             8       // Currently supports up to 8 SPM-4RELAY units for a total of 32 relays restricted by 32-bit power_t size
#define SSPM_SERIAL_BUFFER_SIZE      548     // Needs to accomodate firmware upload data blocks (546 bytes)

//#define SSPM_SIMULATE                        // Simulate additional 4Relay modules based on first detected 4Relay module (debugging purposes only!!)

// From ESP to ARM
#define SSPM_FUNC_FIND               0       // 0x00
#define SSPM_FUNC_SET_OPS            3       // 0x03 - Overload Protection (OPS)
#define SSPM_FUNC_GET_OPS            4       // 0x04
#define SSPM_FUNC_SET_RELAY          8       // 0x08
#define SSPM_FUNC_GET_MODULE_STATE   9       // 0x09 - State of four channels
#define SSPM_FUNC_SET_SCHEME         10      // 0x0A
#define SSPM_FUNC_GET_SCHEME         11      // 0x0B
#define SSPM_FUNC_SET_TIME           12      // 0x0C
#define SSPM_FUNC_IAMHERE            13      // 0x0D
#define SSPM_FUNC_INIT_SCAN          16      // 0x10
#define SSPM_FUNC_UPLOAD_HEADER      20      // 0x14 - SPI Upload header
#define SSPM_FUNC_GET_MAIN_VERSION   21      // 0x15 - Read main ARM firmware version
#define SSPM_FUNC_GET_ENERGY_TOTAL   22      // 0x16
#define SSPM_FUNC_GET_ENERGY         24      // 0x18
#define SSPM_FUNC_GET_LOG            26      // 0x1A
#define SSPM_FUNC_ENERGY_PERIOD      27      // 0x1B
#define SSPM_FUNC_RESET              28      // 0x1C - Remove device from eWelink and factory reset
#define SSPM_FUNC_UPLOAD_DATA        31      // 0x1F - SPI Upload incremental data blocks of max 512 bytes to ARM
#define SSPM_FUNC_UPLOAD_DONE        33      // 0x21 - SPI Finish upload
#define SSPM_FUNC_34                 34      // 0x22 - v1.2.0
#define SSPM_FUNC_GET_OPS_DEFAULTS   35      // 0x23 - v1.2.0 - Get Overload protection defaults
#define SSPM_FUNC_SET_POS            36      // 0x24 - v1.2.0 - Save power on relay state (POS)
#define SSPM_FUNC_GET_POS            37      // 0x25 - v1.2.0 - Read power on relay state (POS)

// From ARM to ESP
#define SSPM_FUNC_ENERGY_RESULT      6       // 0x06
#define SSPM_FUNC_KEY_PRESS          7       // 0x07
#define SSPM_FUNC_SCAN_START         15      // 0x0F
#define SSPM_FUNC_SCAN_RESULT        19      // 0x13 - Provide 4relay ARM firmware version, module type and OPS limits
#define SSPM_FUNC_SCAN_DONE          25      // 0x19
#define SSPM_FUNC_UPLOAD_DONE_ACK    30      // 0x1E - Restart ARM

// Unknown
#define SSPM_FUNC_01                 1       // 0x01
#define SSPM_FUNC_02                 2       // 0x02
#define SSPM_FUNC_05                 5       // 0x05
#define SSPM_FUNC_14                 14      // 0x0E
#define SSPM_FUNC_17                 17      // 0x11
#define SSPM_FUNC_18                 18      // 0x12
#define SSPM_FUNC_23                 23      // 0x17
#define SSPM_FUNC_29                 29      // 0x1D
#define SSPM_FUNC_32                 32      // 0x20

#define SSPM_GPIO_ARM_RESET          15
#define SSPM_GPIO_LED_ERROR          33

#define SSPM_MODULE_NAME_SIZE        12

#define SSPM_VERSION_1_0_0           0x00010000
#define SSPM_VERSION_1_2_0           0x00010200
#define SSPM_VERSION_1_4_0           0x00010400

/*********************************************************************************************/

#define SSPM_TOTAL_MODULES           32                  // Max number of SPM-4RELAY units for a total of 128 relays

enum SspmMachineStates { SPM_NONE,                       // Do nothing
                         SPM_WAIT,                       // Wait 100ms
                         SPM_RESET,                      // Toggle ARM reset pin
                         SPM_POLL_ARM,                   // Wait for first acknowledge from ARM after reset
// Removed to accomodate v1.2.0 too
//                         SPM_POLL_ARM_SPI,               // Wait for first acknowledge from ARM SPI after reset
//                         SPM_POLL_ARM_2,                 // Wait for second acknowledge from ARM after reset
//                         SPM_POLL_ARM_3,                 // Wait for second acknowledge from ARM after reset
                         SPM_SEND_FUNC_GET_MAIN_VERSION, // Get main ARM firmware version
                         SPM_START_SCAN,                 // Start module scan sequence
                         SPM_WAIT_FOR_SCAN,              // Wait for scan sequence to complete
                         SPM_SCAN_COMPLETE,              // Scan complete
                         SPM_STALL_MIDNIGHT,             // Stall energy totals around midnight
                         SPM_GET_ENERGY_TOTALS,          // Init available Energy totals registers
                         SPM_UPDATE_CHANNELS             // Update Energy for powered on channels
                         };

enum SspmDisplayModes { SPM_DISPLAY_ROTATE, SPM_DISPLAY_ROTATE_POWERED_ON, SPM_DISPLAY_TABS, SPM_DISPLAY_MAX_OPTION };

const char kSSPMTriggers[] PROGMEM = "Tasmota|Device|Overload|Overtemp";
const char kSSPMOverload[] PROGMEM = "Tbd1|Voltage|Current|Power|Surge|Tbd6|Tbd7";

#include <TasmotaSerial.h>
TasmotaSerial *SspmSerial;

typedef union {
  uint16_t data;
  struct {
    uint16_t dump : 1;           // bit 0   (v10.1.0.6) - SSPMDump    - Short receive dump (0) or full receive dump (1)
    uint16_t display : 2;        // bit 1,2 (v10.1.0.6) - SSPMDisplay - GUI display all relays (0), only powered on relays (1) or user selected relays (2)
    uint16_t spare03 : 1;        // bit 3
    uint16_t spare04 : 1;        // bit 4
    uint16_t spare05 : 1;        // bit 5
    uint16_t spare06 : 1;        // bit 6
    uint16_t spare07 : 1;        // bit 7
    uint16_t spare08 : 1;        // bit 8  (This bit is default 1 due to legacy use)
    uint16_t spare09 : 1;        // bit 9
    uint16_t spare10 : 1;        // bit 10
    uint16_t spare11 : 1;        // bit 11
    uint16_t spare12 : 1;        // bit 12
    uint16_t spare13 : 1;        // bit 13
    uint16_t spare14 : 1;        // bit 14
    uint16_t spare15 : 1;        // bit 15
  };
} SSPMSOBitfield;

typedef struct {
  uint32_t crc32;                                 // To detect file changes
  uint16_t version;                               // To detect driver function changes
  SSPMSOBitfield flag;
  uint16_t module_map[SSPM_TOTAL_MODULES];        // Max possible SPM relay modules
  float energy_total[SSPM_TOTAL_MODULES][4];      // Total energy in kWh - float allows up to 262143.99 kWh
  float energy_yesterday[SSPM_TOTAL_MODULES][4];  // Energy yesterday in kWh - float allows up to 262143.99 kWh
  uint32_t simulate_count;
} tSspmSettings;

typedef struct {
  tSspmSettings Settings;
  float voltage[SSPM_MAX_MODULES][4];             // 123.12 V
  float current[SSPM_MAX_MODULES][4];             // 123.12 A
  float active_power[SSPM_MAX_MODULES][4];        // 123.12 W
  float apparent_power[SSPM_MAX_MODULES][4];      // 123.12 VA
  float reactive_power[SSPM_MAX_MODULES][4];      // 123.12 VAr
  float power_factor[SSPM_MAX_MODULES][4];        // 0.12
  float energy_today[SSPM_MAX_MODULES][4];        // 12345 kWh
  float energy_total[SSPM_MAX_MODULES][4];        // 12345 kWh total energy since last 6 month!!!

  float min_power;
  float max_power;
  float min_voltage;
  float max_voltage;
  float min_current;
  float max_current;

  float overload_min_power;
  float overload_max_power;
  float overload_min_voltage;
  float overload_max_voltage;
  float overload_max_current;

  uint32_t timeout;
  uint32_t main_version;
  uint32_t relay_version;
  power_t old_power;
  power_t power_on_state;
  uint16_t last_totals;
  uint16_t serial_in_byte_counter;
  uint16_t expected_bytes;
  uint8_t module[SSPM_MAX_MODULES][SSPM_MODULE_NAME_SIZE];
  uint8_t history_day[SSPM_MAX_MODULES][4];
  uint8_t poweron_state[SSPM_MAX_MODULES][4];

#ifdef SSPM_SIMULATE
  uint8_t simulate;
  uint8_t simulated_module;
#endif  // SSPM_SIMULATE
  uint8_t allow_updates;
  uint8_t get_energy_relay;
  int8_t get_energy_relay_focus;
  uint8_t get_totals;
  uint8_t rotate;
  uint8_t module_max;
  uint8_t module_selected;
  uint8_t no_send_key;
  uint8_t counter;
  uint8_t command_sequence;
  uint8_t mstate;
  uint8_t last_button;
  uint8_t error_led_blinks;
  uint8_t overload_relay;
  uint8_t overload_delay;
  uint8_t overload_enable;
  uint8_t history_relay;
  uint8_t log_relay;
  bool map_change;
  bool discovery_triggered;
} TSspm;

uint8_t *SspmBuffer = nullptr;
TSspm *Sspm = nullptr;

/*********************************************************************************************\
 * Driver Settings load and save using filesystem
\*********************************************************************************************/

const uint16_t XDRV_86_VERSION = 0x0104;              // Latest driver version (See settings deltas below)

void Xdrv86SettingsLoad(bool erase) {
  // *** Start init default values in case file is not found ***
  memset(&Sspm->Settings, 0x00, sizeof(tSspmSettings));
  Sspm->Settings.version = XDRV_86_VERSION;
  // Init any other parameter in struct
  Sspm->Settings.flag.display = SPM_DISPLAY_TABS;

  // *** End Init default values ***

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV86 Use defaults as file system not enabled"));
#else
  // Try to load file /.drvset086
  char filename[20];
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_86);
  if (erase) {
    TfsDeleteFile(filename);  // Use defaults
  }
  else if (TfsLoadFile(filename, (uint8_t*)&Sspm->Settings, sizeof(tSspmSettings))) {
    if (Sspm->Settings.version != XDRV_86_VERSION) {      // Fix version dependent changes

      // *** Start fix possible setting deltas ***
      if (Sspm->Settings.version < 0x0104) {
        Sspm->Settings.flag.display = Settings->sbflag1.sspm_display;
      }

      // *** End setting deltas ***

      // Set current version and save settings
      Sspm->Settings.version = XDRV_86_VERSION;
      Xdrv86SettingsSave();
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CFG: XDRV86 loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV86 Use defaults as file system not ready or file not found"));
  }
#endif  // USE_UFILESYS
}

void Xdrv86SettingsSave(void) {
#ifdef USE_UFILESYS
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&Sspm->Settings +4, sizeof(tSspmSettings) -4);  // Skip crc32
  if (crc32 != Sspm->Settings.crc32) {
    // Try to save file /.drvset086
    Sspm->Settings.crc32 = crc32;

    char filename[20];
    // Use for drivers:
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_86);
    if (TfsSaveFile(filename, (const uint8_t*)&Sspm->Settings, sizeof(tSspmSettings))) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV86 saved to file"));
    } else {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: XDRV86 ERROR File system not ready or unable to save file"));
    }
  }
#endif  // USE_UFILESYS
}

bool Xdrv86SettingsRestore(void) {
  XdrvMailbox.data = (char*)&Sspm->Settings;
  XdrvMailbox.index = sizeof(tSspmSettings);
  return true;
}

/*********************************************************************************************/

uint32_t SSMPGetModuleId(uint32_t module) {
  // Return short module id
  // input  number
  // output two_byte value
  uint32_t module_id = 0;
  if (module < Sspm->module_max) {
    module_id = Sspm->module[module][0] << 8 | Sspm->module[module][1];
  }
  return module_id;                 // 0 if not found, else first two bytes of module_id
}

int SSPMGetMappedModuleIdIfFound(uint32_t module) {
  // Return mapped module number
  // input  number
  // output number
  for (uint32_t module_nr = 0; module_nr < Sspm->module_max; module_nr++) {
    if (Sspm->Settings.module_map[module] == SSMPGetModuleId(module_nr)) {
      return module_nr;             // 0, 1, ..
    }
  }
  return -1;                        // -1 if not found
}

uint32_t SSPMGetMappedModuleId(uint32_t module) {
  // Return mapped module number
  // input  number
  // output number
  int module_nr = SSPMGetMappedModuleIdIfFound(module);
  if (-1 == module_nr) {
    module_nr = module;             // input module number if not found used as fallback
  }
#ifdef SSPM_SIMULATE
  if (Sspm->Settings.simulate_count) {
    Sspm->simulated_module = module_nr;
    if (Sspm->simulate && (module_nr > 0) && (module_nr <= Sspm->simulate)) {
      module_nr = 0;                // Emulate modules by 0
    }
  }
#endif  // SSPM_SIMULATE
  return (uint32_t)module_nr;       // 0, 1, ...
}

int SSPMGetModuleNumberFromMapIfFound(uint32_t id) {
  // Return module number based on first two bytes of module id
  // input  two-byte value
  // output number
  for (uint32_t module_nr = 0; module_nr < SSPM_MAX_MODULES; module_nr++) {
    if (id == Sspm->Settings.module_map[module_nr]) {
      return module_nr;             // 0, 1, ...
    }
  }
  return -1;                        // -1 if not found
}

uint32_t SSPMGetModuleNumberFromMap(uint32_t id) {
  // Return module number based on first two bytes of module id
  // input  two-byte value
  // output number
  int module_nr = SSPMGetModuleNumberFromMapIfFound(id);
  if (-1 == module_nr) {
    module_nr = 0;                  // 0 if not found used as fallback
  }
  return (uint32_t)module_nr;                 // 0, 1, ...
}

/*********************************************************************************************/

void SSPMSetLock(uint32_t seconds) {
  Sspm->timeout = seconds * 10;     // Decremented every 100mSec
  Sspm->allow_updates = 0;          // Disable requests from 100mSec loop
}

uint16_t SSPMCalculateCRC(uint8_t *frame, uint32_t num) {
  // CRC-16/ARC (polynomial 0x8005 reflected as 0xA001)
  uint16_t crc = 0;
  for (uint32_t i = 2; i < num; i++) {
    crc ^= frame[i];
    for (uint32_t i = 0; i < 8; i++) {
      crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
    }
  }
  return crc ^ 0;
}

float SSPMGetValue(uint8_t *buffer) {
  // return float from three bytes in buffer
  float value = (buffer[0] << 8) + buffer[1] + (float)buffer[2] / 100;
  return value;
}

void SSPMSetValue(uint8_t *buffer, float value) {
  // Store float in three bytes
  uint32_t integer = value;
  buffer[0] = integer >> 8;
  buffer[1] = integer;
  buffer[2] = (value * 100) - (integer * 100);  // Fraction
}

/*********************************************************************************************/

void SSPMSend(uint32_t size) {
  uint16_t crc = SSPMCalculateCRC(SspmBuffer, size -2);
  SspmBuffer[size -2] = (uint8_t)(crc >> 8);
  SspmBuffer[size -1] = (uint8_t)crc;

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SPM: ESP %*_H"), size, SspmBuffer);

  SspmSerial->write(SspmBuffer, size);
}

void SSPMSendAck(uint32_t command_sequence) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 0f 00 01 00 01 3d e6
  Marker  |Module id                          |Ac|Cm|Size |Pl|Ix|Chksm|
  */
  SspmBuffer[15] = 0x80;
  SspmBuffer[17] = 0x00;
  SspmBuffer[18] = 0x01;
  SspmBuffer[19] = 0x00;
  SspmBuffer[20] = command_sequence;

  SSPMSend(23);
}

void SSPMInitSend(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  Marker  |Module id                          |Ac|Cm|Size |
  */
  memset(SspmBuffer, 0, 19);
  SspmBuffer[0] = 0xAA;
  SspmBuffer[1] = 0x55;
  SspmBuffer[2] = 0x01;
}

void SSPMSendCmnd(uint32_t command) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FC 51
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  SSPMInitSend();
  SspmBuffer[16] = command;
  if (0 == command) {
    Sspm->command_sequence = 0;
  } else {
    Sspm->command_sequence++;
  }
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

/*********************************************************************************************/

void SSPMSendFindAck(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 01 00 00 FC 73
  Marker  |Module id                          |Ac|Cm|Size |  |Ix|Chksm|
  */
  SSPMInitSend();
  SspmBuffer[15] = 0x80;  // Ack
//  SspmBuffer[16] = SSPM_FUNC_FIND;  // 0x00
  SspmBuffer[18] = 1;
  SspmBuffer[19] = 0;
  SSPMSend(23);
}

void SSPMSendOPS(uint32_t relay) {
  /*
  Overload Protection


   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 03 00 12 04 00 11 30 00 00 00 0a 00 f0 00 00 00 0a 00 14 00 00 fb a6 f8  = Default settings
  Marker  |Module id                          |Ac|Cm|Size |Ch|Ra|Max P   |Min P   |Max U   |Min U   |Max I   |De|Ix|Chksm|
                                                          |  |  |   4400W|    0.1W|    240V|    0.1V|     20A|  |
  Ch - Bitmask channel 01 = 1, 02 = 2, 04 = 3, 08 = 4
  Ra - Bitmask enabled features xxxxxxx1 Enable Max current
  Ra - Bitmask enabled features xxxxxx1x Enable Min voltage
  Ra - Bitmask enabled features xxxxx1xx Enable Max voltage
  Ra - Bitmask enabled features xxxx1xxx Enable Min power
  Ra - Bitmask enabled features xxx1xxxx Enable Max power
  De - 0 to 255 seconds Overload detection delay
  Values are XX XX    - number
                   XX - decimals

  aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 03 00 12 02 10 00 14 00 00 00 0a 00 f0 00 00 00 0a 00 14 00 14 46 7b 80  - L2, 20 seconds, MaxPower 20W

  Acknowledge:
  aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 03 00 01 00 46 75 1d
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 80 03 00 01 00 14 08 bc
                                              |Ac|Cm|Size |Rt|Ix|Chksm|
  Ac - Acknowledge or error number
  Rt - Return code
  */
  uint8_t module = relay >> 2;

  if (module >= Sspm->module_max) { return; }

  uint8_t channel = 1 << (relay & 0x03);  // Channel relays are bit masked
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_SET_OPS;  // 0x03
  SspmBuffer[18] = 0x12;
  SspmBuffer[19] = channel;
  SspmBuffer[20] = Sspm->overload_enable;
  SSPMSetValue(&SspmBuffer[21], Sspm->overload_max_power);
  SSPMSetValue(&SspmBuffer[24], Sspm->overload_min_power);
  SSPMSetValue(&SspmBuffer[27], Sspm->overload_max_voltage);
  SSPMSetValue(&SspmBuffer[30], Sspm->overload_min_voltage);
  SSPMSetValue(&SspmBuffer[33], Sspm->overload_max_current);
  SspmBuffer[36] = Sspm->overload_delay;
  Sspm->command_sequence++;
  SspmBuffer[37] = Sspm->command_sequence;

  SSPMSend(40);
}

void SSPMSendGetOps(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  aa 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 04 00 00 08 c0 0a
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_OPS;  // 0x04
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendSetRelay(uint32_t relay, uint32_t state) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 08 00 01 44 08 c0 34
  Marker  |Module id                          |Ac|Cm|Size |Pl|Ix|Chksm|
  */
  uint8_t channel = 1 << (relay & 0x03);  // Channel relays are bit masked
  if (state) {
    channel |= (channel << 4);
  }
  uint8_t module = relay >> 2;

  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_SET_RELAY;  // 0x08
  SspmBuffer[18] = 0x01;
  SspmBuffer[19] = channel;
  Sspm->command_sequence++;
  SspmBuffer[20] = Sspm->command_sequence;

  SSPMSend(23);
}

void SSPMSendGetModuleState(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 09 00 01 0f 05 b5 de
  Marker  |Module id                          |Ac|Cm|Size |Pl|Ix|Chksm|
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_MODULE_STATE;  // 0x09
  SspmBuffer[18] = 0x01;
  SspmBuffer[19] = 0x0F;   // State of all four relays
  Sspm->command_sequence++;
  SspmBuffer[20] = Sspm->command_sequence;

  SSPMSend(23);
}

void SSPMSendScheme(uint32_t relay) {
  /*
  Time scheme
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51
  One time
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 1e 01 01 01 07 e5 0b 0e 0b 38 08 00 6b 01 00 ea 60 20 23 1b 04 fd 7a 83 05 63 ee dd a9 b9 3a 7e 14 95
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 1e 01 01 01 07 e5 0b 0e 0c 04 35 00 55 01 02 46 76 0e 0c 20 e1 22 7c 67 ab 9c 66 73 6d bd e8 7f 50 d4
  Marker  |Module id                          |Ac|Cm|Size |No|  |Mo| YYYY|MM|DD|HH|MM         |St|Re|Scheme id                                      |
  No - Number of schemes defined
  Mo - Scheme type (1 = temporarly, 2 = scheduled)
  Re - Relay 0 to 3
  St - State (0 = off, 1 = On)

  Scheduled On
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 18 01 01 02 15 0c 0c 01 03 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24 80 5b 48
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 18 01 01 02 15 0c 0c 01 03 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24 82 9a c9
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 18 01 01 02 53 0c 0c 01 03 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24 83 44 aa
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 18 01 01 02 53 0d 0b 00 02 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24 84 e0 22
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 18 01 01 02 0e 0d 3b 01 03 84 fb ea 35 ca 16 51 b5 b8 10 a1 1c d0 1a 3f 7a 86 e3 fa

  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 2f 02 01 02 0e 0d 3b 01 03 84 fb ea 35 ca 16 51 b5 b8 10 a1 1c d0 1a 3f 7a
                                                              01 02 53 0d 0b 00 02 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24 87 e8 02

  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 2f 02 01 02 53 0d 0b 00 02 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24
                                                              01 02 0e 0d 3b 01 03 84 fb ea 35 ca 16 51 b5 b8 10 a1 1c d0 1a 3f 7a 89 6e e6

  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 4c 03 01 01 07 e5 0b 0e 0e 0e 26 00 e7 01 00 e6 b2 48 8e ef be ce 78 3e 5d a8 3a c0 c5 6f 5e  = One time
                                                              01 02 53 0d 0b 00 02 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24                    = 14:11 OFF CH3 SuMoThSa
                                                              01 02 0e 0d 3b 01 03 84 fb ea 35 ca 16 51 b5 b8 10 a1 1c d0 1a 3f 7a 8a 2f f8           = 14:59 ON  CH4 MoTuWe

  Marker  |Module id                          |Ac|Cm|Size |No|  |Mo|Dy|HH|MM|St|Re|Scheme id                                      |
  Dy - Bitmask days xxxxxxx1 sunday
                    xxxxxx1x monday
                    xxxxx1xx tuesday
                    xxxx1xxx wednesday
                    xxx1xxxx thursday
                    xx1xxxxx friday
                    x1xxxxxx saturday

  Scheduled Off
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 01 00 81 26 9f
  Schedule 2 off
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0a 00 18 01 01 02 53 0d 0b 00 02 99 65 93 dc f8 d0 b0 29 a8 66 ba 8f 41 66 29 24 88 e5 22
  Marker  |Module id                          |Ac|Cm|Size |
  */

  SspmBuffer[16] = SSPM_FUNC_SET_SCHEME;  // 0x0A

}

void SSPMSendGetScheme(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 0b 00 00 09 14 c8
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_SCHEME;  // 0x0B
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendSetTime(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0c 00 0b 07 e5 0b 06 0c 39 01 00 00 02 00 04 8a 37
  Marker  |Module id                          |Ac|Cm|Size |YY YY MM DD HH MM SS|Ln|St|Tzone|Ix|Chksm|
  UTC time
  Tzone = Time zone, [-12,+14], can be a decimal, such as 7.5
  */
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_SET_TIME;  // 0x0C
  SspmBuffer[18] = 0x0B;
  TIME_T time;
  BreakTime(Rtc.utc_time, time);
  uint16_t year = time.year + 1970;
  SspmBuffer[19] = year >> 8;
  SspmBuffer[20] = year;
  SspmBuffer[21] = time.month;
  SspmBuffer[22] = time.day_of_month;
  SspmBuffer[23] = time.hour;
  SspmBuffer[24] = time.minute;
  SspmBuffer[25] = time.second;
  SspmBuffer[26] = 0;
  SspmBuffer[27] = 0;
  SspmBuffer[28] = Rtc.time_timezone / 60;
//  SspmBuffer[28] = (Rtc.time_timezone / 60) +1;  // possibly itead bug
  SspmBuffer[29] = abs(Rtc.time_timezone % 60);
  Sspm->command_sequence++;
  SspmBuffer[30] = Sspm->command_sequence;

  SSPMSend(33);
}

void SSPMSendIAmHere(uint32_t relay) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 0d 00 00 17 35 b6
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|

  Response is blink green COMM led on SPM-4Relay
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 0d 00 01 00 17 48 b5
  Marker  |Module id                          |Ac|Cm|Size |Rs|Ix|Chksm|
  Rs = Return state
  */
  uint8_t module = relay >> 2;

  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_IAMHERE;  // 0x0D
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendInitScan(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  AA 55 01 ff ff ff ff ff ff ff ff ff ff ff ff 00 10 00 00 02 cd f0
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|

  Acknowledge:
  AA 55 01 ff ff ff ff ff ff ff ff ff ff ff ff 80 10 00 01 00 02 e5 03
                                              |Ac|Cm|Size |Rt|Ix|Chksm|
  */
  SSPMSetLock(30);           // Disable requests from 100mSec loop

  memset(SspmBuffer, 0xFF, 15);
  SspmBuffer[0] = 0xAA;
  SspmBuffer[1] = 0x55;
  SspmBuffer[2] = 0x01;

  SspmBuffer[15] = 0;
  SspmBuffer[16] = SSPM_FUNC_INIT_SCAN;  // 0x10
  SspmBuffer[17] = 0;
  SspmBuffer[18] = 0;
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);

  AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Start relay scan..."));
}

void SSPMSendGetEnergyTotal(uint32_t relay) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 16 00 0d 6b 7e 32 37 39 37 34 13 4b 35 36 37 01 14 e6 93
  Marker  |                                   |  |Cm|Size |Module id                          |Ch|Ix|Chksm|
  */
  uint8_t module = relay >> 2;

  if (module >= Sspm->module_max) { return; }

  uint8_t channel = relay & 0x03;  // Channel relays are NOT bit masked this time
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_ENERGY_TOTAL;  // 0x16
  SspmBuffer[18] = 0x0D;
  memcpy(SspmBuffer +19, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[31] = channel;
  Sspm->command_sequence++;
  SspmBuffer[32] = Sspm->command_sequence;

  SSPMSend(35);
}

void SSPMSendGetEnergy(uint32_t relay) {
  /*
  relay_num = 1..8
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 18 00 10 6b 7e 32 37 39 37 34 13 4b 35 36 37 01 01 00 3c 2a db d1
  Marker  |                                   |  |Cm|Size |Module id                          |  |Ch|     |Ix|Chksm|
  */
  uint8_t module = relay >> 2;

  if (module >= Sspm->module_max) { return; }

  uint8_t channel = 1 << (relay & 0x03);  // Channel relays are bit masked
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_ENERGY;  // 0x18
  SspmBuffer[18] = 0x10;
  memcpy(SspmBuffer +19, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[31] = 0x01;
  SspmBuffer[32] = channel;
  SspmBuffer[33] = 0;
  SspmBuffer[34] = 0x3C;
  Sspm->command_sequence++;
  SspmBuffer[35] = Sspm->command_sequence;

  SSPMSend(38);
}

void SSPMSendGetLog(uint32_t relay, uint32_t entries) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1a 00 10 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 00 00 1d 09 8c cd  -- Logs 1 to 29
  aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1a 00 10 8b 34 32 37 39 37 34 13 4b 35 36 37 00 1d 00 3a 14 b8 ee  -- Logs 30 to 58
  Marker  |                                   |  |Cm|Size |Module id                          |Start|End  |Ix|Chksm|
  Start = newest log start number  (Latest is 0)
  End   = older log end number     (End - Start >= 29 (0x1d))
  */
  uint8_t module = relay >> 2;

  if (module >= Sspm->module_max) { return; }

  uint32_t startlog = (entries >= 29) ? entries -29 : 0;
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_LOG;  // 0x1A
  SspmBuffer[18] = 0x10;
  memcpy(SspmBuffer +19, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[31] = startlog >> 8;    // MSB start log
  SspmBuffer[32] = startlog;         // LSB start log
  SspmBuffer[33] = entries >> 8;     // MSB end log
  SspmBuffer[34] = entries;          // LSB end log - Number of logs
  Sspm->command_sequence++;
  SspmBuffer[35] = Sspm->command_sequence;

  SSPMSend(38);
}

void SSPMSendGetEnergyPeriod(uint32_t relay) {
  /*
  Start
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44
  aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1b 00 17 8b 34 32 37 39 37 34 13 4b 35 36 37 03 00 07 e6 01 1c 0d 08 33 00 81 1a 1c 41   - L4 Start 2022-01-28T14:08:51
  Marker  |                                   |  |Cm|Size |Module id                          |Ch|St|Year |Mo|Da|HH|MM|SS|Milli|Ix|Chksm|
                                                                                              |L4|--| 2022| 1|28|13|08|51|  129|
                                                                                                 |Start

  Immediate request for refresh
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
  aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1b 00 0e 8b 34 32 37 39 37 34 13 4b 35 36 37 03 02 1b 5a 30  - L4 Refresh
  Marker  |                                   |  |Cm|Size |Module id                          |Ch|--|Ix|Chksm|
                                                                                              |L4|Refresh

  Following requests
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
  aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1b 00 0e 8b 34 32 37 39 37 34 13 4b 35 36 37 03 02 1c 98 71  - L4 Refresh
  Marker  |                                   |  |Cm|Size |Module id                          |Ch|--|Ix|Chksm|
                                                                                              |L4|Refresh

  Stop
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44
  aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1b 00 17 8b 34 32 37 39 37 34 13 4b 35 36 37 03 01 07 e6 01 1c 0d 28 1f 03 23 21 04 04  - L4 Stop 2022-01-28T14:40:32
  Marker  |                                   |  |Cm|Size |Module id                          |Ch|St|Year |Mo|Da|HH|MM|SS|Milli|Ix|Chksm|
                                                                                              |L4|--| 2022| 1|28|13|40|31|  803|
                                                                                                 |Stop
  */

  SspmBuffer[16] = SSPM_FUNC_ENERGY_PERIOD;  // 0x1B

}

void SSPMSendFunc34(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 22 00 00 f2 6a 7f
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_34;  // 0x22
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendGetOPSDefaults(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 23 00 00 f4 94 fe
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_OPS_DEFAULTS;  // 0x23
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendSetPowerOnState(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
  aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 00 24 00 05 0f 00 01 02 01 00 f3 c2
  Marker  |Module id                          |Ac|Cm|Size |??|P1|P2|P3|P4|Ix|Chksm|
  P1 - Relay1 power on state (0 = On, 1 = Off, 2 = Laststate)
  P2 - Relay2 power on state (0 = On, 1 = Off, 2 = Laststate)
  P3 - Relay3 power on state (0 = On, 1 = Off, 2 = Laststate)
  P4 - Relay4 power on state (0 = On, 1 = Off, 2 = Laststate)
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_SET_POS;  // 0x24
  SspmBuffer[18] = 0x05;
  SspmBuffer[19] = 0x0F;
  SspmBuffer[20] = Sspm->poweron_state[module][0];
  SspmBuffer[21] = Sspm->poweron_state[module][1];
  SspmBuffer[22] = Sspm->poweron_state[module][2];
  SspmBuffer[23] = Sspm->poweron_state[module][3];
  Sspm->command_sequence++;
  SspmBuffer[24] = Sspm->command_sequence;

  SSPMSend(27);
}

void SSPMSendGetPowerOnState(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  aa 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 25 00 00 08 c0 0a
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  if (module >= Sspm->module_max) { return; }

  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[SSPMGetMappedModuleId(module)], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_POS;  // 0x25
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

/*********************************************************************************************/

void SSPMAddModule(void) {
  if (Sspm->module_max < SSPM_MAX_MODULES) {
    memcpy(Sspm->module[Sspm->module_max], SspmBuffer + 19, SSPM_MODULE_NAME_SIZE);
    if (0 == Sspm->max_power) {
      Sspm->max_current = SspmBuffer[39] + (float)SspmBuffer[40] / 100;  // x.xxA
      Sspm->min_current = SspmBuffer[41] + (float)SspmBuffer[42] / 100;  // x.xxA
      Sspm->max_power = SSPMGetValue(&SspmBuffer[49]);                   // x.xxVA
      Sspm->min_power = SSPMGetValue(&SspmBuffer[52]);                   // x.xxVA
      Sspm->max_voltage = SSPMGetValue(&SspmBuffer[43]);                 // x.xxV
      Sspm->min_voltage = SSPMGetValue(&SspmBuffer[46]);                 // x.xxV
    }
    uint32_t module_id = SspmBuffer[19] << 8 | SspmBuffer[20];
    int mapped = SSPMGetModuleNumberFromMapIfFound(module_id);
    if (-1 == mapped) {
      // Scanned module not in mapped list. Append if possible
      for (uint32_t module = Sspm->module_max; module < SSPM_MAX_MODULES; module++) {
        if (0 == Sspm->Settings.module_map[module]) {
          Sspm->Settings.module_map[module] = module_id;
          mapped = module;
          break;
        }
      }
      Sspm->map_change = true;
    }

    uint32_t relay_version = SspmBuffer[36] << 16 | SspmBuffer[37] << 8 | SspmBuffer[38];  // 0x00010000 or 0x00010200
    if (relay_version < Sspm->relay_version) {
      Sspm->relay_version = relay_version;      // Lowest version will be supported
    }
    mapped++;
    AddLog(LOG_LEVEL_INFO, PSTR("SPM: 4Relay %d (mapped to %d) type %d version %d.%d.%d found with id %12_H"),
      Sspm->module_max +1, mapped, SspmBuffer[35], SspmBuffer[36], SspmBuffer[37], SspmBuffer[38], Sspm->module[Sspm->module_max]);

    Sspm->module_max++;

    if (Settings->save_data) {
      TasmotaGlobal.save_data_counter = Settings->save_data +2;            // Postpone flash write until all modules are scanned
    }
  }
}

/*********************************************************************************************/

void SSPMLogResult(uint32_t command, uint32_t status) {
  if (1 == status) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Command %d not supported"), command);
  } else if (2 == status) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Command %d timeout"), command);
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Command %d result %d"), command, status);
  }
}

void SSPMHandleReceivedData(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 04 00 02 00 00 06 98 06
  Marker  |Module id                          |Ac|Cm|Size |     |Ix|Chksm|
  */
  bool ack = (0x80 == SspmBuffer[15]);                               // Ac
  uint32_t command = SspmBuffer[16];                                 // Cm
  uint32_t expected_bytes = (SspmBuffer[17] << 8) + SspmBuffer[18];  // Size
  // 0 - OK
  // 1 - Not supported
  // 2 - Timeout
  // 3 - Log empty
  // 4 -
  // 5 - Out of command sync
  uint32_t status = SspmBuffer[19];                                  // Status id expected_bytes is 1
  uint32_t command_sequence = SspmBuffer[19 + expected_bytes];       // Ix

//  AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Rcvd ack %d, cmnd %d, seq %d, size %d"),
//    ack, command, command_sequence, expected_bytes);

  if (ack) {
    // Responses from ARM (Acked)
    if (status > 0) {
      SSPMLogResult(command, status);
    }
    switch(command) {
      case SSPM_FUNC_FIND:
        /* 0x00
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 01 00 00 fc 73
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00 00 22 50     -- SPI response
                                                    |Er|Cm|Size |St|Ix|Chksm|
        */
        if (!status) {
          Sspm->mstate++;   // Cycle to
        }
        break;
      case SSPM_FUNC_SET_OPS:
        /* 0x03
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        aa 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 80 03 00 01 00 fb 84 fd
        Marker  |Module id                          |Ac|Cm|Size |St|Ix|Chksm|
        */
        if (!status && (Sspm->overload_relay < 255)) {
          SSPMSendGetOps(Sspm->overload_relay >> 2);
        }
        break;
      case SSPM_FUNC_GET_OPS:
        /* 0x04 - Overload Protection
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 04 00 02 00 00 06 98 06
        Marker  |Module id                          |Ac|Cm|Size |St|  |Ix|Chksm|

         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
        AA 55 01 6B 7E 32 37 39 37 34 13 4B 35 36 37 80 04 00 35 00 07 00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00
                                                                       38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54
                                                                       00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00
                                                                       55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74
                                                                       00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00 07 8A 86
        Marker  |Module id                          |Ac|Cm|Size |St|Ch|Ra|Max P   |Min P   |Max U   |Min U   |Max I   |De|Ix|Chksm|
                                                                   |  |  |   4400W|    0.1W|    240V|    0.1V|     20A|  |
        Ch - Bitmask channel 01 = 1, 02 = 2, 04 = 3, 08 = 4
        Ra - Bitmask enabled features xxxxxxx1 Enable Max current
        Ra - Bitmask enabled features xxxxxx1x Enable Min voltage
        Ra - Bitmask enabled features xxxxx1xx Enable Max voltage
        Ra - Bitmask enabled features xxxx1xxx Enable Min power
        Ra - Bitmask enabled features xxx1xxxx Enable Max power
        De - 0 to 255 seconds Overload detection delay
        */
        if (!status && (Sspm->overload_relay < 255)) {
          Response_P(PSTR("{\"SSPMOverload%d\":"), Sspm->overload_relay +1);
          if (expected_bytes < 19) {
            ResponseAppend_P(PSTR("\"None\"}"));
          } else {
            uint32_t module = SSPMGetModuleNumberFromMap(SspmBuffer[3] << 8 | SspmBuffer[4]);
            uint32_t channels = SspmBuffer[20];
            uint32_t offset = 21;
            for (uint32_t i = 0; i < 4; i++) {
              if (channels & 1) {
                uint32_t relay = (module * 4) +i;
                if (Sspm->overload_relay == relay) {
                  Sspm->overload_enable = SspmBuffer[offset];
                  uint32_t enabled = Sspm->overload_enable;
                  char bitmask[] = "00000";
                  for (uint32_t j = 0; j < 5; j++) {
                    if (enabled & 1) {
                      if (0 == j) { bitmask[4] = '1'; }  // MaxCurrent
                      if (1 == j) { bitmask[2] = '1'; }  // MinVoltage
                      if (2 == j) { bitmask[3] = '1'; }  // MaxVoltage
                      if (3 == j) { bitmask[0] = '1'; }  // MinPower
                      if (4 == j) { bitmask[1] = '1'; }  // MaxPower
                    }
                    enabled >>= 1;
                  }
                  Sspm->overload_max_power = SSPMGetValue(&SspmBuffer[offset +1]);     // x.xxVA
                  Sspm->overload_min_power = SSPMGetValue(&SspmBuffer[offset +4]);     // x.xxVA
                  Sspm->overload_max_voltage = SSPMGetValue(&SspmBuffer[offset +7]);   // x.xxV
                  Sspm->overload_min_voltage = SSPMGetValue(&SspmBuffer[offset +10]);  // x.xxV
                  Sspm->overload_max_current = SSPMGetValue(&SspmBuffer[offset +13]);  // x.xxA
                  Sspm->overload_delay = SspmBuffer[offset +16];
                  ResponseAppend_P(PSTR("{\"Delay\":%d,\"Set\":%s,\"MinPower\":%2_f,\"MaxPower\":%2_f,\"MinVoltage\":%2_f,\"MaxVoltage\":%2_f,\"MaxCurrent\":%2_f}}"),
                    Sspm->overload_delay, bitmask,
                    &Sspm->overload_min_power, &Sspm->overload_max_power, &Sspm->overload_min_voltage, &Sspm->overload_max_voltage, &Sspm->overload_max_current);
                  break;
                }
                offset += 17;
              }
              channels >>= 1;
            }
            if (!channels) {
              ResponseAppend_P(PSTR("\"None\"}"));
            }
          }
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("SSPMOverload"));
          Sspm->overload_relay = 255;
        } else {
          if (Sspm->main_version > SSPM_VERSION_1_0_0) {
            SSPMSendGetPowerOnState(Sspm->module_selected -1);
          } else {
            Sspm->module_selected--;
            if (Sspm->module_selected > 0) {
              SSPMSendGetModuleState(Sspm->module_selected -1);
            } else {
              SSPMSendGetScheme(Sspm->module_selected);
            }
          }
        }
        break;
      case SSPM_FUNC_GET_MODULE_STATE:
        /* 0x09
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 09 00 06 00 0f 01 01 01 01 05 fe 35
                                                                   |OS|4RelayMasks|
        */
        if (!status && (0x06 == expected_bytes)) {
          // SspmBuffer[20] & 0x0F                      // Relays operational
          uint32_t module = SSPMGetModuleNumberFromMap(SspmBuffer[3] << 8 | SspmBuffer[4]);
#ifdef SSPM_SIMULATE
          if (Sspm->Settings.simulate_count) { module = Sspm->simulated_module; }
#endif  // SSPM_SIMULATE
          power_t current_state = (SspmBuffer[20] >> 4) << (module * 4);  // Relays state
          power_t mask = 0x0000000F << (module * 4);
          TasmotaGlobal.power &= (POWER_MASK ^ mask);
          TasmotaGlobal.power |= current_state;

          Sspm->old_power = TasmotaGlobal.power;
          UpdateDevicesPresent(4);
        }
        SSPMSendGetOps(Sspm->module_selected -1);
        break;
      case SSPM_FUNC_GET_SCHEME:
        /* 0x0B
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
        AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 80 0b 00 02 00 00 09 bb c7
                                                                |St|??|
        */
        if (0x02 == expected_bytes) {

        }
        Sspm->module_selected++;
        if (Sspm->module_selected < Sspm->module_max) {
          SSPMSendGetScheme(Sspm->module_selected);
        } else {
          AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Relay scan done"));
          Sspm->mstate = SPM_SCAN_COMPLETE;
        }
        break;
      case SSPM_FUNC_SET_TIME:
        /* 0x0C
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 0c 00 01 00 04 3e 62
        v1.2.0: adds response from each 4-relay module
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 0c 00 01 00 19 4c 09
        */
        if (0 == SspmBuffer[3]) {
          // Discard v1.2.0 additions
          TasmotaGlobal.devices_present = 0;
          SSPMSendGetModuleState(Sspm->module_selected -1);
        }
        break;
      case SSPM_FUNC_INIT_SCAN:
        /* 0x10
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 ff ff ff ff ff ff ff ff ff ff ff ff 80 10 00 01 00 02 e5 03
        */
        break;
      case SSPM_FUNC_GET_MAIN_VERSION:
        /* 0x15
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 15 00 04 00 01 00 00 01 81 b1
                                                                |St|FwVersio|
                                                                |  |   1.0.0|
        v1.2.0:
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 15 00 04 00 01 02 00 01 41 10
                                                                |  |   1.2.0|
        */
        Sspm->main_version = SspmBuffer[20] << 16 | SspmBuffer[21] << 8 | SspmBuffer[22];  // 0x00010000 or 0x00010200
        AddLog(LOG_LEVEL_INFO, PSTR("SPM: Main version %d.%d.%d found"), SspmBuffer[20], SspmBuffer[21], SspmBuffer[22]);

        Sspm->mstate = SPM_START_SCAN;
        break;
      case SSPM_FUNC_GET_ENERGY_TOTAL:
        /* 0x16
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 16 01 7e 00 8b 34 32 37 39 37 34 13 4b 35 36 37
        03           <- L4
        07 e5 0b 0d  <- End date (Today) 2021 nov 13
        07 e5 05 11  <- Start date       2021 may 17
        00 05        <- 0.05kWh  (13/11 Today)
        00 00        <- 0        (12/11 Yesterday)
        00 04        <- 0.04kWh  (11/11 etc)
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        42 67 46

         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 16 00 0D 6B 7E 32 37 39 37 34 13 4B 35 36 37 00 B4 0E 92  - L1 Response after midnight (out of sequence)
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 16 00 0D 8B 34 32 37 39 37 34 13 4B 35 36 37 00 B5 24 54  - L5 Response after midnight (out of sequence)
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 16 00 0D 8B 34 32 37 39 37 34 13 4B 35 36 37 01 B6 B5 15  - L6 Response after midnight (out of sequence)
        */
        if (expected_bytes > 24) {
          uint32_t entries = (expected_bytes - 22) / 2;
          // Find last valid (= non-zero) entry in 6 month fifo buffer
          uint16_t energy = 0;
          while (!energy && --entries) {
            energy = SspmBuffer[41 + (entries *2)] + SspmBuffer[42 + (entries *2)];
            if (0x701E == energy) { energy = 0; }  // Unknown why sometimes 0x701E (=112.30kWh) pops up
          }

          uint32_t channel = SspmBuffer[32];
          uint32_t module = SSPMGetModuleNumberFromMap(SspmBuffer[20] << 8 | SspmBuffer[21]);
#ifdef SSPM_SIMULATE
          if (Sspm->Settings.simulate_count) { module = Sspm->simulated_module; }
#endif  // SSPM_SIMULATE
          if (Sspm->history_relay < 255) {
            uint32_t history_module = Sspm->history_relay >> 2;
            uint32_t history_channel = Sspm->history_relay & 0x03;  // Channel relays are NOT bit masked this time
            if ((history_channel == channel) && (history_module == module)) {
              Response_P(PSTR("{\"SSPMHistory%d\":["), Sspm->history_relay +1);
            } else {
              Sspm->history_relay = 255;
            }
          }

          float last_energy_today = Sspm->energy_today[module][channel];
          float energy_yesterday = 0;
          float energy_total = 0;
          for (uint32_t i = 0; i <= entries; i++) {
            float energy = SspmBuffer[41 + (i*2)] + (float)SspmBuffer[42 + (i*2)] / 100;   // x.xxkWh
            if (112.30 == energy) { energy = 0; }  // Unknown why sometimes 0x701E (=112.30kWh) pops up

            if (Sspm->history_relay < 255) {
              ResponseAppend_P(PSTR("%s%*_f"), (i)?",":"", Settings->flag2.energy_resolution, &energy);
            }

            if (0 == i) {
              Sspm->energy_today[module][channel] = energy;
            } else {
              if (1 == i) { energy_yesterday = energy; }
              energy_total += energy;
            }
          }

          uint8_t history_day = SspmBuffer[36];                                    // Date of last entry
          if (0 == Sspm->history_day[module][channel]) {                           // Initial setting
            Sspm->history_day[module][channel] = history_day;
          }
          if ((0 == Sspm->Settings.energy_total[module][channel]) && energy_total) {
            Sspm->Settings.energy_yesterday[module][channel] = energy_yesterday;   // Initial setting
            Sspm->Settings.energy_total[module][channel] = energy_total;           // Initial setting
            if (Settings->save_data) {
              TasmotaGlobal.save_data_counter = Settings->save_data +2;            // Postpone flash write until all relays are updated
            }
          }
          // If received daily energy date is changed then update total energy
          // This happens around midnight in normal situations
          else if (Sspm->history_day[module][channel] != history_day) {
            Sspm->history_day[module][channel] = history_day;
            Sspm->Settings.energy_yesterday[module][channel] = last_energy_today;  // Daily save
            Sspm->Settings.energy_total[module][channel] += last_energy_today;     // Daily incremental save
            if (Settings->save_data) {
              TasmotaGlobal.save_data_counter = Settings->save_data +2;            // Postpone flash write until all relays are updated
            }
          }
          Sspm->energy_total[module][channel] = Sspm->Settings.energy_total[module][channel] + Sspm->energy_today[module][channel];

          if (Sspm->history_relay < 255) {
            ResponseAppend_P(PSTR("]}"));
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("SSPMHistory"));
            Sspm->history_relay = 255;  // Disable display energy history
          }

          Sspm->allow_updates = 1;
        }
        break;
      case SSPM_FUNC_GET_ENERGY:
        /* 0x18
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 18 00 01 00 15 31 92
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 18 00 01 05 85 0D 91  -- Error after midnight
        Marker  |Module id                          |Ac|Cm|Size |St|Ix|Chksm|
        */
        break;
      case SSPM_FUNC_GET_LOG:
        /* 0x1A - Module logging
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1a 01 3a 00 6b 7e 32 37 39 37 34 13 4b 35 36 37
        1e                            Number of log entries (1e = 30)
        07 e5 0b 06 0f 25 19 02 01 00 10 byte log entry
                                  |-- trigger 00 = App, 01 = Device, 02 = Overload, 03 = Overtemp
                               |----- state   00 = Off, 01 = On
                            |-------- Channel 00 to 03
                         |----------- Second  = 25
                      |-------------- Minute  = 37
                   |----------------- Hour    = 15
                |-------------------- Day     =  6
             |----------------------- Month   = 11 = November
        ----------------------------- Year    07 e5 = 2021
        07 e5 0b 06 0f 1f 08 00 00 01
        07 e5 0b 06 0f 1e 1e 01 01 01
        07 e5 0b 06 0e 37 36 03 00 00
        07 e5 0b 06 0e 37 36 01 00 00
        07 e5 0b 06 0e 36 37 01 01 00
        ...
        07 e5 0b 06 0d 30 2d 03 00 01 09 89 fe

        Error:
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1A 00 01 03 E5 45 EB  -- Log empty
                                                                |  |
        */
        if ((!status || (3 ==  status)) && (Sspm->log_relay < 255)) {
          uint32_t module = Sspm->log_relay >> 2;
          uint32_t channel = Sspm->log_relay & 0x03;  // Channel relays are NOT bit masked this time
          Response_P(PSTR("{\"SSPMLog%d\":"), Sspm->log_relay +1);
          if (module != SSPMGetModuleNumberFromMap(SspmBuffer[20] << 8 | SspmBuffer[21])) {
            ResponseAppend_P(PSTR("\"Wrong module\"}"));
          }
          else if (3 == status) {
            ResponseAppend_P(PSTR("\"Empty\"}"));  // Module log empty
          }
          else {
            uint32_t entries = SspmBuffer[32];
            uint32_t offset = 33;
            bool more = false;
            for (uint32_t i = 0; i < entries; i++) {
              if (SspmBuffer[offset +7] == channel) {
                uint32_t year = SspmBuffer[offset] << 8 | SspmBuffer[offset +1];
                char stemp[10];  // One of "App|Device|Overload|Overtemp"
                ResponseAppend_P(PSTR("%s{\"Time\":\"%d-%02d-%02dT%02d:%02d:%02d\",\"Trigger\":\"%s\",\"State\":\"%s\"}"),
                  (more)?",":"[", year, SspmBuffer[offset +2], SspmBuffer[offset +3],
                  SspmBuffer[offset +4], SspmBuffer[offset +5], SspmBuffer[offset +6],
                  GetTextIndexed(stemp, sizeof(stemp), SspmBuffer[offset +9] & 0x03, kSSPMTriggers),
                  GetStateText(SspmBuffer[offset +8]));
                more = true;
              }
              offset += 10;
            }
            if (more) {
              ResponseAppend_P(PSTR("]}"));
            }else {
              ResponseAppend_P(PSTR("\"None\"}"));  // Module log contains no logging for requested relay
            }
          }
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("SSPMLog"));
        }
        Sspm->log_relay = 255;  // Disable display energy history
        break;
      case SSPM_FUNC_ENERGY_PERIOD:
        /* 0x1B
        Response after start energy period
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 0e 00 8b 34 32 37 39 37 34 13 4b 35 36 37 03 1a fc 7c  - L4
        Marker  |Module id                          |Ac|Cm|Size |St|Module Id                          |Ch|Ix|Chksm|
                                                                |OK|                                   |L4|

        Response after first auto-refresh AND following refreshes AND Stop
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 11 00 8b 34 32 37 39 37 34 13 4b 35 36 37 03 00 00 00 1b 1d 54
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 11 00 8b 34 32 37 39 37 34 13 4b 35 36 37 03 00 00 01 20 5e 14
        Marker  |Module id                          |Ac|Cm|Size |St|Module Id                          |Ch|Energy  |Ix|Chksm|
                                                                |OK|                                   |L4| 0.01kWh|
        */
        break;
      case SSPM_FUNC_RESET:
        /* 0x1C
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1c 00 01 00 0b f9 e3
        Marker  |Module id                          |Ac|Cm|Size |St|Ix|Chksm|
        */
//        TasmotaGlobal.restart_flag = 2;
        break;
      case SSPM_FUNC_34:
        /* 0x22 v1.2.0
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
        aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 22 00 02 00 00 f2 19 00
        Marker  |Module id                          |Ac|Cm|Size |St|  |Ix|Chksm|
        */
        break;
      case SSPM_FUNC_GET_OPS_DEFAULTS:
        /* 0x23 v1.2.0
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38
        aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 23 00 11 00 14 00 00 0a 01 08 00 00 5a 00 12 c0 00 00 00 0a f4 7f 4d
        Marker  |Module id                          |Ac|Cm|Size |St|Max I|Min I|Max U   |Min U   |Max P   |Min P   |Ix|Chksm|
                                                                |OK|20.0A|0.10A| 240.00V|   0.10V|4400.00W|   0.10W|
        */
        break;
      case SSPM_FUNC_SET_POS:
        /* 0x24 v1.2.0
        aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 24 00 01 00 00 80 a8
        Marker  |Module id                          |Ac|Cm|Size |St|Ix|Chksm|
        */
        break;
      case SSPM_FUNC_GET_POS:
        /* 0x25 v1.2.0 - Get Power On State
        Response v1.0.0
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 25 00 01 01 06 98 06
        Marker  |Module id                          |Ac|Cm|Size |St|Ix|Chksm|

        Response v1.2.0
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
        aa 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 25 00 05 00 00 01 02 01 07 b6 89
        Marker  |Module id                          |Ac|Cm|Size |St|P1|P2|P3|P4|Ix|Chksm|
        P1 - Relay1 power on state (0 = On, 1 = Off, 2 = Laststate)
        P2 - Relay2 power on state (0 = On, 1 = Off, 2 = Laststate)
        P3 - Relay3 power on state (0 = On, 1 = Off, 2 = Laststate)
        P4 - Relay4 power on state (0 = On, 1 = Off, 2 = Laststate)
        */
        Sspm->module_selected--;
        if (!status && (expected_bytes >= 0x05)) {
          for (uint32_t i = 0; i < 4; i++) {
            Sspm->poweron_state[Sspm->module_selected][i] = SspmBuffer[20 +i];
          }
        }
        if (Sspm->module_selected > 0) {
          SSPMSendGetModuleState(Sspm->module_selected -1);
        } else {
          SSPMSendGetScheme(Sspm->module_selected);
        }
        break;
    }
  } else {
    // Initiated by ARM
    switch(command) {
      case SSPM_FUNC_ENERGY_RESULT:
        /* 0x06
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 06 00 1c 6b 7e 32 37 39 37 34 13 4b 35 36 37 01 00 00 00 e3 5b 00 00 00 00 00 00 00 00 00 6b 1f 95 1e
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 06 00 1C 8B 34 32 37 39 37 34 13 4B 35 36 37 01 00 0B 00 E4 37 00 19 0E 00 00 02 00 19 09 4B 28 1D 71
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 06 00 1C 8B 34 32 37 39 37 34 13 4B 35 36 37 08 00 0A 00 E3 61 00 18 2E 00 00 00 00 18 33 4B 27 D3 0D
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 06 00 1C 8B 34 32 37 39 37 34 13 4B 35 36 37 08 02 04 00 DC 14 01 C1 3D 00 10 19 01 C2 29 4B 37 6B 26
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 06 00 1c 8b 34 32 37 39 37 34 13 4b 35 36 37 08 00 44 00 e1 35 00 9a 3e 00 01 45 00 9a 38 00 08 8b ae
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 06 00 1c 8b 34 32 37 39 37 34 13 4b 35 36 37 08 00 4a 00 e1 22 00 61 4d 00 2c 38 00 a8 28 20 26 21 70
                                                                                                    |Ch|Curre|Voltage |ActivePo|Reactive|Apparent|5m|
        Values are XX XX    - number
                         XX - decimals
        5m - 5 minutes Power Consumption (Ws)
        */
        {
          uint32_t channel = 0;
          for (channel = 0; channel < 4; channel++) {
            if (SspmBuffer[31] & 1) { break; }
            SspmBuffer[31] >>= 1;
          }
          uint32_t module = SSPMGetModuleNumberFromMap(SspmBuffer[19] << 8 | SspmBuffer[20]);
#ifdef SSPM_SIMULATE
          if (Sspm->Settings.simulate_count) { module = Sspm->simulated_module; }
#endif  // SSPM_SIMULATE
          Sspm->current[module][channel] = SspmBuffer[32] + (float)SspmBuffer[33] / 100;                                 // x.xxA
          Sspm->voltage[module][channel] = SSPMGetValue(&SspmBuffer[34]);         // x.xxV
          Sspm->active_power[module][channel] = SSPMGetValue(&SspmBuffer[37]);    // x.xxW
          Sspm->reactive_power[module][channel] = SSPMGetValue(&SspmBuffer[40]);  // x.xxVAr
          Sspm->apparent_power[module][channel] = SSPMGetValue(&SspmBuffer[43]);  // x.xxVA
          float power_factor = (Sspm->active_power[module][channel] && Sspm->apparent_power[module][channel]) ? Sspm->active_power[module][channel] / Sspm->apparent_power[module][channel] : 0;
          if (power_factor > 1) { power_factor = 1; }
          Sspm->power_factor[module][channel] = power_factor;
          SSPMSendAck(command_sequence);
          Sspm->allow_updates = 1;
        }
        break;
      case SSPM_FUNC_KEY_PRESS:
        /* 0x07
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 07 00 0d 6b 7e 32 37 39 37 34 13 4b 35 36 37 11 04 bf c3
                                                                                                    |AS|
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 07 00 0D 6B 7E 32 37 39 37 34 13 4B 35 36 37 01 22 A5 4F  - L1 Overload triggered power off

        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 07 00 0d 8b 34 32 37 39 37 34 13 4b 35 36 37 02 2d bb 08  - L2 Overload triggered power off
        */
        if (!Sspm->no_send_key) {
          uint32_t module = SSPMGetModuleNumberFromMap(SspmBuffer[19] << 8 | SspmBuffer[20]);
#ifdef SSPM_SIMULATE
//          if (Sspm->Settings.simulate_count) { module = Sspm->simulated_module; }  // Won't work as this is initiated from device
#endif  // SSPM_SIMULATE
          power_t relay = (SspmBuffer[31] & 0x0F) << (module * 4);      // Relays active
          power_t relay_state = (SspmBuffer[31] >> 4) << (module * 4);  // Relays state
          for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {
            if (relay &1) {
              ExecuteCommandPower(i, relay_state &1, SRC_BUTTON);
            }
            relay >>= 1;
            relay_state >>= 1;
          }
          Sspm->old_power = TasmotaGlobal.power;
        }
        SSPMSendAck(command_sequence);
        break;
      case SSPM_FUNC_SCAN_START:
        /* 0x0F
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 01 02 01 9d f8  - Response after normal scan start
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 01 02 01 9D F8  - Response after midnight - notice reset of sequence number
        Marker  |                                   |Ac|Cm|Size |St|Ix|Chksm|

         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 14 00 6B 7E 32 37 39 37 34 13 4B 35 36 37 00 00 00 01 00 00 00 23 9D EF  - Response after L1 max_power overload powered off
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 14 00 8B 34 32 37 39 37 34 13 4B 35 36 37 00 00 02 00 00 00 00 98 AC 8B  - Response after L6 max_current overload powered off
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 14 00 8B 34 32 37 39 37 34 13 4B 35 36 37 00 02 00 00 00 00 00 94 8B A9  - Response after L6 max_voltage overload powered off
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 14 00 8B 34 32 37 39 37 34 13 4B 35 36 37 00 20 00 00 00 00 00 81 46 6A  - Response after L6 min_voltage overload powered off
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 14 00 8b 34 32 37 39 37 34 13 4b 35 36 37 00 00 00 00 00 00 00 2f 38 ca  - After power on and overload was disabled
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 14 00 8b 34 32 37 39 37 34 13 4b 35 36 37 80 00 00 00 00 00 00 04 47 82  - At 02:50:24
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 14 00 8b 34 32 37 39 37 34 13 4b 35 36 37 00 00 00 00 00 00 00 05 e7 4b  - At 02:50:30
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 14 00 8b 34 32 37 39 37 34 13 4b 35 36 37 20 00 00 00 00 00 00 06 fe 09  - At 08:40:52
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 14 00 8b 34 32 37 39 37 34 13 4b 35 36 37 00 00 00 00 00 00 00 07 26 ca  - At 08:40:58
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0F 00 14 00 25 6C 47 31 36 39 37 09 54 39 30 30 00 00 00 00 10 00 00 14 9A 66  - Response after L1 current surge overload powered off
        Marker  |                                   |Ac|Cm|Size |St|Module id                          |  |Vo|Cu|Po|Su|  |  |Ix|Chksm|
        32..38 - Bitmask channel 01 = 1, 02 = 2, 04 = 3, 08 = 4 (Max border)
        32..38 - Bitmask channel 10 = 1, 20 = 2, 40 = 3, 80 = 4 (Min border)
        Cu - Current
        Su - Current Surge or Inrush (See https://github.com/arendst/Tasmota/discussions/15695#discussioncomment-3747975)
        Vo - Voltage
        Po - Power
        Ot - Overtemp
        */
        if (status > 0) {
          SSPMLogResult(command, status);
        }
        else if (0x14 == expected_bytes) {                // Overload/Overtemp triggered
          uint32_t any_bit_set = 0;
          for (uint32_t i = 0; i < 7; i++) {
            any_bit_set += SspmBuffer[32 +i];             // Overload triggered channel bits
          }
          if (any_bit_set) {                              // Signals all is OK again if NOT set
            uint32_t module = SSPMGetModuleNumberFromMap(SspmBuffer[20] << 8 | SspmBuffer[21]);
            bool more = false;
            char border[2][4] = { "Max","Min" };
            char stemp[10];   // "Tbd1|Voltage|Current|Power|Surge|Tbd6|Tbd7"
            Response_P(PSTR("{\"SSPMOverload\":"));
            for (uint32_t i = 0; i < 7; i++) {
              uint32_t channel = SspmBuffer[32 +i];
              for (uint32_t j = 0; j < 8; j++) {
                if (channel &1) {
                  uint32_t relay = (module << 2) +(j & 3);
                  uint32_t idx = (j >> 2) & 1;
                  ResponseAppend_P(PSTR("%s{\"L%d\":\"%s%s\"}"), (more)?",":"[", relay +1, (4==i)?"":border[idx], GetTextIndexed(stemp, sizeof(stemp), i, kSSPMOverload));
                  more = true;
                }
                channel >>= 1;
              }
            }
            ResponseAppend_P(PSTR("]}"));
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("SSPMOverload"));
          }
        }
        SSPMSendAck(command_sequence);
        break;
      case SSPM_FUNC_SCAN_RESULT:
        /* 0x13
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
        Marker  |                                   |Ac|Cm|Size |Module id                          |Ch|        |Ty|FwVersio|Max I|Min I|Max U   |Min U   |Max P   |Min P   |Ix|Chksm|
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 6b 7e 32 37 39 37 34 13 4b 35 36 37 04 00 00 00 82 01 00 00 14 00 00 0a 00 f0 00 00 00 0a 11 30 00 00 00 0a 02 8f cd - v1.0.0
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 8b 34 32 37 39 37 34 13 4b 35 36 37 04 00 00 00 82 01 00 00 14 00 00 0a 00 f0 00 00 00 0a 11 30 00 00 00 0a 02 a0 6f - v1.0.0
                                                                                                                |130|  1.0.0|20.0A|0.10A| 240.00V|   0.10V|4400.00W|   0.10W|
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 8B 34 32 37 39 37 34 13 4B 35 36 37 04 00 00 00 82 01 02 00 14 00 00 0A 01 08 00 00 5A 00 12 C0 00 00 00 0A 02 6B 93 - v1.2.0
                                                                                                                |130|  1.2.0|20.0A|0.10A| 264.00V|     90V|4800.00W|   0.10W|
        Ty = Type of sub-device. 130: Four-channel sub-device
        */
        if (0x24 == expected_bytes) {
          SSPMAddModule();
#ifdef SSPM_SIMULATE
          if (Sspm->Settings.simulate_count && (0 == Sspm->simulate)) {
            uint8_t current_idh = SspmBuffer[19];
            uint8_t current_idl = SspmBuffer[20];
            for (Sspm->simulate = 0; Sspm->simulate < Sspm->Settings.simulate_count; Sspm->simulate++) {
              SspmBuffer[19] = Sspm->simulate +1;
              SspmBuffer[20] = Sspm->simulate +1;
              SSPMAddModule();
            }
            SspmBuffer[19] = current_idh;
            SspmBuffer[20] = current_idl;
          }
#endif  // SSPM_SIMULATE
        }
        SSPMSendAck(command_sequence);
        break;
      case SSPM_FUNC_SCAN_DONE:
        /* 0x19
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 19 00 00 03 a1 16
        */
        SSPMSendAck(command_sequence);

        if (Sspm->module_max) {
          // Warn for mapping change when removal or addition of 4Relay modules
          for (uint32_t module = Sspm->module_max; module < SSPM_MAX_MODULES; module++) {
            // Clear obsolete mapping slots
            Sspm->Settings.module_map[module] = 0;
          }
          for (uint32_t module = 0; module < Sspm->module_max; module++) {
            // Remove not scanned module (probably physical removed) from mapping
            if (-1 == SSPMGetMappedModuleIdIfFound(module)) {
              // Clear mapping slot
              Sspm->Settings.module_map[module] = 0;
              Sspm->map_change = true;
            }
          }
          for (uint32_t module = 0; module < Sspm->module_max; module++) {
            // Add scanned module to mapping
            uint32_t module_id = SSMPGetModuleId(module);
            if (-1 == SSPMGetModuleNumberFromMapIfFound(module_id)) {
              // Scanned module not in mapping list (probably due to physical install)
              for (uint32_t i = 0; i < Sspm->module_max; i++) {
                // Find empty slot in mapping and insert
                if (0 == Sspm->Settings.module_map[i]) {
                  Sspm->Settings.module_map[i] = module_id;
                  Sspm->map_change = true;
                  break;
                }
              }
            }
          }
          if (Sspm->map_change) {
            Sspm->map_change = false;
            AddLog(LOG_LEVEL_INFO, PSTR("SPM: WARNING 4Relay mapping possibly changed"));
          }

          Sspm->module_selected = Sspm->module_max;
        } else {
          for (uint32_t module = 0; module < SSPM_MAX_MODULES; module++) {
            // Clear mapping slots
            Sspm->Settings.module_map[module] = 0;
            memset(Sspm->module[module], 0, SSPM_MODULE_NAME_SIZE);
          }
          AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Relay scan done - none found"));

          Sspm->mstate = SPM_NONE;
          Sspm->error_led_blinks = 255;
        }

        SSPMSendSetTime();
        break;
      case SSPM_FUNC_UPLOAD_DONE_ACK:
        /* 0x1E
        0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        aa 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1e 00 01 00 01 fe 05
        Marker  |                                   |  |Cm|Size |  |Ix|Chksm|
        */
        SSPMSendFindAck();
        break;
    }
  }
}

void SSPMSerialInput(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 19 00 00 03 a1 16
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 01 00 00 fc 73
  Marker  |Module id                          |Ac|Cm|Size |Pl|Ix|Chksm|
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 80 09 00 06 00 0f 01 01 01 01 05 f9 9d
  Marker  |Module id                          |Ac|Cm|Size |Payload          |Ix|Chksm|
                                               00                                        Request
                                               80                                        Response (Ack)
  */
  while (SspmSerial->available()) {
    yield();
    uint8_t serial_in_byte = SspmSerial->read();

    if ((0x01 == serial_in_byte) && (0x55 == SspmBuffer[Sspm->serial_in_byte_counter -1]) && (0xAA == SspmBuffer[Sspm->serial_in_byte_counter -2])) {

      if (Sspm->serial_in_byte_counter > 2) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SPM: ARM out of sync %*_H"), Sspm->serial_in_byte_counter, SspmBuffer);
      }

      // Start of message
      Sspm->expected_bytes = 0;
      SspmBuffer[0] = 0xAA;
      SspmBuffer[1] = 0x55;
      Sspm->serial_in_byte_counter = 2;
    }
    if (Sspm->serial_in_byte_counter < SSPM_SERIAL_BUFFER_SIZE -1) {
      SspmBuffer[Sspm->serial_in_byte_counter++] = serial_in_byte;
      if ((19 == Sspm->serial_in_byte_counter) && (0xAA == SspmBuffer[0]) && (0x55 == SspmBuffer[1]) && (0x01 == SspmBuffer[2])) {
        // Message size known
        Sspm->expected_bytes = 22 + (SspmBuffer[17] << 8) + SspmBuffer[18];
      }
      if (Sspm->serial_in_byte_counter == Sspm->expected_bytes) {
        // Complete message received
        bool more = (!Sspm->Settings.flag.dump && (Sspm->serial_in_byte_counter > 58));  // Skip long dumps as they overwrite log buffer
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SPM: ARM %*_H%s"), (more) ? 58 : Sspm->serial_in_byte_counter, SspmBuffer, (more) ? "..." : "");

        uint16_t crc_rcvd = (SspmBuffer[Sspm->serial_in_byte_counter -2] << 8) + SspmBuffer[Sspm->serial_in_byte_counter -1];
        uint16_t crc_calc = SSPMCalculateCRC(SspmBuffer, Sspm->serial_in_byte_counter -2);
        if (crc_rcvd == crc_calc) {
          SSPMHandleReceivedData();
        } else {
          Sspm->error_led_blinks = 20;
          AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: CRC error"));
        }
        Sspm->serial_in_byte_counter = 0;
        Sspm->expected_bytes = 0;
      }
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Serial input buffer overflow"));
      Sspm->serial_in_byte_counter = 0;
      Sspm->expected_bytes = 0;
    }
  }
}

/*********************************************************************************************/

bool SSPMSendSPI(uint32_t size) {
  uint16_t crc = SSPMCalculateCRC(SspmBuffer, size -2);
  SspmBuffer[size -2] = (uint8_t)(crc >> 8);
  SspmBuffer[size -1] = (uint8_t)crc;

  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));      // Set up SPI at 10MHz, MSB first, Capture at rising edge

  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  // AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FC 51
  // Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  SPI.writeBytes(SspmBuffer, size);                                      // Send data

  delayMicroseconds(600);                                                // Wait for receipt delay deduced from initial SPM SPI comms

  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  // AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00 00 22 50
  // Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  bool crc_ok = false;

  uint32_t expected_bytes = 20;                                          // Location of expected bytes (Size) - This works fine
  SPI.transferBytes(nullptr, SspmBuffer, expected_bytes);                // First receive data up to known location of expected bytes
  if ((0xAA == SspmBuffer[0]) && (0x55 == SspmBuffer[1]) && (0x01 == SspmBuffer[2])) {
    // Message size known
    expected_bytes = 2 + (SspmBuffer[17] << 8) + SspmBuffer[18];
    if (expected_bytes < SSPM_SERIAL_BUFFER_SIZE - 20) {
      SPI.transferBytes(nullptr, SspmBuffer +20, expected_bytes);        // Then receive the expected bytes
      // Complete message received
      expected_bytes += 20;

      bool more = (!Sspm->Settings.flag.dump && (expected_bytes > 58));  // Skip long dumps as they overwrite log buffer
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SPM: SPI %*_H%s"), (more) ? 58 : expected_bytes, SspmBuffer, (more) ? "..." : "");

      uint16_t crc_rcvd = (SspmBuffer[expected_bytes -2] << 8) + SspmBuffer[expected_bytes -1];
      uint16_t crc_calc = SSPMCalculateCRC(SspmBuffer, expected_bytes -2);
      crc_ok = (crc_rcvd == crc_calc);
      if (crc_ok) {
//        SSPMHandleReceivedData();
      } else {
        Sspm->error_led_blinks = 20;
        AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: SPI CRC error"));
      }
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: SPI buffer overflow"));
    }
  }
  SPI.endTransaction();                                                  // Stop SPI transaction

  return crc_ok;
}

bool SSPMSendSPIFind(void) {
  // Send
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  // AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FC 51 every 600uSecs
  // Wait for
  // AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 01 00 00 FC 73 from ARM
  // or
  // AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00 00 22 50 from ARM (No flash)
  for (uint32_t i = 0; i < 40; i++) {
    SSPMInitSend();
    SspmBuffer[19] = SSPM_FUNC_FIND;
    if (SSPMSendSPI(22)) { return true; }
  }

  return false;
}

void SSPMSendSPIUploadHeader(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 14 00 0b 09 09 00 1b e5 a4 c7 00 02 88 74 00 6d df
  Marker  |                                   |  |Cm|Size |        |Checksum   |UploadSize |Ix|Chksm|
  */
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_UPLOAD_HEADER;  // 0x14
  SspmBuffer[18] = 0x0B;


  SspmBuffer[30] = 0;
  SSPMSendSPI(33);
}

void SSPMSendSPIUpload(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38    539 540 541 542 543 544 545
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1f 02 0c 00 00 00 00 00 00 02 00 a2 99 c3 22 00 00 01 20 cd 95 01 08 ... 04  48  af  f3  01  xx  yy
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1f 02 0c 00 00 02 00 00 00 02 00 27 f7 24 87 00 80 01 23 23 70 10 bd ... 21  fa  04  f3  02  xx  yy
  ...
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1f 02 0c 00 02 86 00 00 00 02 00 f8 f5 25 6d f1 61 00 08 02 01 ff 00 ...                 44  xx  yy
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1f 00 80 00 02 88 00 00 00 00 74 95 4e 01 c1 c5 e5 02 08 c5 e5 02 08 ...                 45  xx  yy
  Marker  |                                   |  |Cm|Size |Address    |UploadSize |Checksum   |512 data bytes                            |Ix |Chksm  |
  */
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_UPLOAD_DATA;  // 0x1F



  Sspm->command_sequence++;
  SspmBuffer[543] = Sspm->command_sequence;
  SSPMSendSPI(546);
}

void SSPMSendSPIUploadDone(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 21 00 00 46 32 da
  Marker  |                                   |  |Cm|Size |Ix|Chksm|
  */
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_UPLOAD_DONE;  // 0x21
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;
  SSPMSendSPI(22);
}

/*********************************************************************************************/

void SSPMInit(void) {
  if (!ValidTemplate(PSTR("Sonoff SPM")) || !PinUsed(GPIO_RXD) || !PinUsed(GPIO_TXD)) { return; }

  Sspm = (TSspm*)calloc(sizeof(TSspm), 1);    // Need calloc to reset registers to 0/false
  if (!Sspm) { return; }
  SspmBuffer = (uint8_t*)malloc(SSPM_SERIAL_BUFFER_SIZE);
  if (!SspmBuffer) {
    free(Sspm);
    return;
  }
  SspmSerial = new TasmotaSerial(Pin(GPIO_RXD), Pin(GPIO_TXD), 1, 0, SSPM_SERIAL_BUFFER_SIZE);
  if (!SspmSerial->begin(115200)) {
    free(SspmBuffer);
    free(Sspm);
    return;
  }

  Xdrv86SettingsLoad(0);

  pinMode(SSPM_GPIO_ARM_RESET, OUTPUT);
  digitalWrite(SSPM_GPIO_ARM_RESET, 1);

  pinMode(SSPM_GPIO_LED_ERROR, OUTPUT);
  digitalWrite(SSPM_GPIO_LED_ERROR, 0);

  if (TasmotaGlobal.spi_enabled) {
    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
  } else {
    SPI.begin(14, 12, 13, -1);
  }

  if (0 == Settings->flag2.voltage_resolution) {
    Settings->flag2.voltage_resolution = 1;   // SPM has 2 decimals but this keeps the gui clean
    Settings->flag2.current_resolution = 2;   // SPM has 2 decimals
    Settings->flag2.wattage_resolution = 1;   // SPM has 2 decimals but this keeps the gui clean
    Settings->flag2.energy_resolution = 1;    // SPM has 2 decimals but this keeps the gui clean
  }

#if CONFIG_IDF_TARGET_ESP32
#ifdef USE_ETHERNET
  Settings->eth_address = 0;                      // EthAddress
  Settings->eth_type = ETH_PHY_LAN8720;           // EthType
  Settings->eth_clk_mode = ETH_CLOCK_GPIO17_OUT;  // EthClockMode
#endif
#endif

  for (uint32_t module = 0; module < Sspm->module_max; module++) {
    for (uint32_t relay = 0; relay < 4; relay++) {
      Sspm->poweron_state[module][relay] = 1;  // Set default power on state to Off ( = Sonoff 1)
    }
  }
  Sspm->relay_version = 0xFFFFFFFF;           // Find lowest supported relay version
  Sspm->overload_relay = 255;                 // Disable display overload settings
  Sspm->history_relay = 255;                  // Disable display energy history
  Sspm->log_relay = 255;                      // Disable display logging
  Sspm->old_power = TasmotaGlobal.power;
  Sspm->power_on_state = TasmotaGlobal.power;
  Sspm->mstate = SPM_WAIT;                    // Start init sequence
}

/*********************************************************************************************/

void SSPMEvery100ms(void) {
  Sspm->last_totals++;

  if (Sspm->no_send_key) { Sspm->no_send_key--; }

  if (Sspm->timeout) {
    Sspm->timeout--;
    if (!Sspm->timeout) {
      Sspm->allow_updates = 1;
    }
  }

  if (Sspm->error_led_blinks) {
    uint32_t state = 1;                        // Stay lit
    if (Sspm->error_led_blinks < 255) {
      Sspm->error_led_blinks--;
      state = Sspm->error_led_blinks >> 1 &1;  // Blink every 0.4s
    }
    digitalWrite(SSPM_GPIO_LED_ERROR, state);
  }

  // Fix race condition if the ARM doesn't respond
  if ((Sspm->mstate > SPM_NONE) && (Sspm->mstate < SPM_SEND_FUNC_GET_MAIN_VERSION)) {
    Sspm->counter++;
    if (Sspm->counter > 30) {
      Sspm->mstate = SPM_NONE;
      Sspm->error_led_blinks = 255;
    }
  }
  switch (Sspm->mstate) {
    case SPM_NONE:
      return;
    case SPM_WAIT:
      // 100ms wait
      Sspm->mstate = SPM_RESET;
      break;
    case SPM_RESET:
      // Reset ARM
      digitalWrite(SSPM_GPIO_ARM_RESET, 0);
      delay(18);
      digitalWrite(SSPM_GPIO_ARM_RESET, 1);
      delay(18);
      Sspm->mstate = SPM_POLL_ARM;
    case SPM_POLL_ARM:
      // Wait for first acknowledge from ARM after reset
      SSPMSendCmnd(SSPM_FUNC_FIND);
      break;
/*
    // Removed to accomodate v1.2.0 too
    case SPM_POLL_ARM_SPI:
      SSPMSendSPIFind();
      Sspm->mstate = SPM_POLL_ARM_2;
      break;
    case SPM_POLL_ARM_2:
      SSPMSendCmnd(SSPM_FUNC_FIND);
      Sspm->mstate = SPM_POLL_ARM_3;
      break;
    case SPM_POLL_ARM_3:
      // Wait for second acknowledge from ARM after reset
      break;
*/
    case SPM_SEND_FUNC_GET_MAIN_VERSION:
      // Get main version number
      SSPMSendCmnd(SSPM_FUNC_GET_MAIN_VERSION);
      break;
    case SPM_START_SCAN:
      // Start scan module sequence
      Sspm->error_led_blinks = 0;              // Reset error light
      Sspm->overload_relay = 255;              // Disable display overload settings
      Sspm->history_relay = 255;               // Disable display energy history
      Sspm->log_relay = 255;                   // Disable display logging
      Sspm->module_max = 0;
      SSPMSendInitScan();
      Sspm->mstate = SPM_WAIT_FOR_SCAN;
      Sspm->last_totals = 0;
      break;
    case SPM_WAIT_FOR_SCAN:
      // Wait for scan sequence to complete within 60 seconds
      if (Sspm->last_totals > 600) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Relay scan timeout"));
        Sspm->mstate = SPM_NONE;
        Sspm->error_led_blinks = 255;
      }
      break;
    case SPM_SCAN_COMPLETE:
      // Scan sequence finished
#ifdef SSPM_SIMULATE
      if (!Sspm->Settings.simulate_count) {
#endif  // SSPM_SIMULATE
        if (Sspm->relay_version < SSPM_VERSION_1_2_0) {
          // Set relay power on state based on Tasmota global setting
          if (Sspm->power_on_state) {
            TasmotaGlobal.power = Sspm->power_on_state;
            Sspm->power_on_state = 0;            // Reset power on state solving re-scan
            SetPowerOnState();                   // Set power on state now that all relays have been detected
          }
        }
#ifdef SSPM_SIMULATE
      }
#endif  // SSPM_SIMULATE
      TasmotaGlobal.discovery_counter = 1;     // Force TasDiscovery()
      Sspm->allow_updates = 1;                 // Enable requests from 100mSec loop
      Sspm->get_energy_relay = 0;
      Sspm->mstate = SPM_GET_ENERGY_TOTALS;
      break;
    case SPM_STALL_MIDNIGHT:
      // Get totals for ALL relays after midnight updating Tasmotas total and yesterday energy
      if (Sspm->last_totals > 600) {           // Continue after 60 seconds
        Sspm->get_energy_relay = 0;
        Sspm->mstate = SPM_GET_ENERGY_TOTALS;
      }
      break;
    case SPM_GET_ENERGY_TOTALS:
      // Retrieve Energy total status from up to 128 relays
      if (Sspm->allow_updates) {
        SSPMSetLock(4);
        SSPMSendGetEnergyTotal(Sspm->get_energy_relay);
        Sspm->get_energy_relay++;
        if (Sspm->get_energy_relay >= TasmotaGlobal.devices_present) {
          Sspm->get_energy_relay = TasmotaGlobal.devices_present;
          Sspm->get_energy_relay_focus = -1;
          Sspm->mstate = SPM_UPDATE_CHANNELS;
        }
      }
      break;
    case SPM_UPDATE_CHANNELS:
      // Retrieve Energy status from up to 128 powered on relays (takes 128 * 2s!!)
      if (Sspm->allow_updates) {
        int32_t time = (RtcTime.hour *3600) + (RtcTime.minute *60) + RtcTime.second;
        if (time > 86370) {                    // Stall updates after 23:59:31 to satisfy ARM firmware
          Sspm->last_totals = 0;
          Sspm->mstate = SPM_STALL_MIDNIGHT;
        } else {
          Sspm->get_energy_relay++;
          if (Sspm->get_energy_relay >= TasmotaGlobal.devices_present) {
            Sspm->get_energy_relay = 0;
            if (Sspm->last_totals > 1200) {    // Get totals every 2 minutes (takes 128 * 0.2s)
              Sspm->last_totals = 0;
              Sspm->get_totals = 1;
            } else {
              Sspm->get_totals = 0;
            }
          }

          // If focused on one module increase focused relay polling
          if ((SPM_DISPLAY_TABS == Sspm->Settings.flag.display) && (TasmotaGlobal.devices_present > 8) && (0 == Sspm->get_totals)) {
            if ((Sspm->get_energy_relay % 4) == 0) {  // Every fourth relay
              uint32_t next = Sspm->rotate +4;
              if (next >= TasmotaGlobal.devices_present) { next = 0; }
              if ((Sspm->get_energy_relay_focus > -1) || !((Sspm->get_energy_relay == Sspm->rotate) || (Sspm->get_energy_relay == next)))  {
                if (-1 == Sspm->get_energy_relay_focus) {
                  Sspm->get_energy_relay_focus = Sspm->get_energy_relay;
                  Sspm->get_energy_relay = Sspm->rotate;
                } else {
                  Sspm->get_energy_relay = Sspm->get_energy_relay_focus;
                  Sspm->get_energy_relay_focus = -1;
                }
              }
            }
          }
//          AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Totals %d, Relay %d"), Sspm->get_totals, Sspm->get_energy_relay);

          power_t powered_on = TasmotaGlobal.power >> Sspm->get_energy_relay;
          if (powered_on &1) {
            SSPMSetLock(4);
            if (Sspm->get_totals) {
              SSPMSendGetEnergyTotal(Sspm->get_energy_relay);
            } else {
              SSPMSendGetEnergy(Sspm->get_energy_relay);
            }
          } else {
            uint32_t module = Sspm->get_energy_relay >> 2;
            uint32_t channel = Sspm->get_energy_relay &3;
            if (Sspm->voltage[module][channel]) {
              Sspm->voltage[module][channel] = 0;
              Sspm->current[module][channel] = 0;
              Sspm->active_power[module][channel] = 0;
              Sspm->apparent_power[module][channel] = 0;
              Sspm->reactive_power[module][channel] = 0;
              Sspm->power_factor[module][channel] = 0;
            }
          }
        }
      }
      break;
  }
}

/*********************************************************************************************/

bool SSPMSetDevicePower(void) {
  power_t new_power = XdrvMailbox.index;
  if (new_power != Sspm->old_power) {
    uint32_t relay_count = 0;
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      uint32_t new_state = (new_power >> i) &1;
      if (new_state != ((Sspm->old_power >> i) &1)) {
        SSPMSendSetRelay(i, new_state);
        relay_count++;
      }
    }
    Sspm->old_power = new_power;
    if (relay_count) {
      Sspm->no_send_key = relay_count *10;  // Disable button response for relay_count * 10 * 0.1 second
    }
  }
  return true;
}

/*********************************************************************************************/

bool SSPMButton(void) {
  bool result = false;
  uint32_t button = XdrvMailbox.payload;
  if ((PRESSED == button) && (NOT_PRESSED == Sspm->last_button)) {  // Button pressed
    Sspm->mstate = SPM_START_SCAN;
    result = true;                 // Disable further button processing
  }
  Sspm->last_button = button;
  return result;
}

/*********************************************************************************************/

const uint16_t SSPM_SIZE = 300;

char* SSPMEnergyFormat(char* result, float* input, uint32_t resolution, uint8_t* indirect, uint8_t offset, uint32_t count) {
  ext_snprintf_P(result, SSPM_SIZE, PSTR("</td>"));       // Skip first column
  // </td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td>
  // </td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td>
  // </td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td>
  // </td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td style='text-align:right'>1.23</td><td>&nbsp;</td><td>
  for (uint32_t i = 0; i < count; i++) {
    ext_snprintf_P(result, SSPM_SIZE, PSTR("%s<td style='text-align:%s'>%*_f</td><td>&nbsp;</td>"),
      result, (Settings->flag5.gui_table_align)?PSTR("right"):PSTR("left"), resolution, &input[indirect[offset +i]]);
  }
  ext_snprintf_P(result, SSPM_SIZE, PSTR("%s<td>"), result);
  return result;
}

void SSPMEnergyShow(bool json) {
  if (!TasmotaGlobal.devices_present) { return; }  // Not ready yet

  if (json) {
    ResponseAppend_P(PSTR(",\"SPM\":{\"" D_JSON_ENERGY "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.energy_resolution, &Sspm->energy_total[i >>2][i &3]);
    }
#ifdef SSPM_JSON_ENERGY_YESTERDAY
    ResponseAppend_P(PSTR("],\"" D_JSON_YESTERDAY "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.energy_resolution, &Sspm->Settings.energy_yesterday[i >>2][i &3]);
    }
#endif
#ifdef SSPM_JSON_ENERGY_TODAY
    ResponseAppend_P(PSTR("],\"" D_JSON_TODAY "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.energy_resolution, &Sspm->energy_today[i >>2][i &3]);
    }
#endif
    ResponseAppend_P(PSTR("],\"" D_JSON_ACTIVE_POWERUSAGE "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.wattage_resolution, &Sspm->active_power[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("],\"" D_JSON_APPARENT_POWERUSAGE "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.wattage_resolution, &Sspm->apparent_power[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("],\"" D_JSON_REACTIVE_POWERUSAGE "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.wattage_resolution, &Sspm->reactive_power[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("],\"" D_JSON_POWERFACTOR "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", 2, &Sspm->power_factor[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("],\"" D_JSON_VOLTAGE "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.voltage_resolution, &Sspm->voltage[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("],\"" D_JSON_CURRENT "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.current_resolution, &Sspm->current[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("]}"));
#ifdef USE_WEBSERVER
  } else {
    uint8_t relays[SSPM_MAX_MODULES * 4];
    uint8_t indirect[SSPM_MAX_MODULES * 4];

    uint32_t relay_show = 0;
    power_t power = TasmotaGlobal.power;
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      if ((SPM_DISPLAY_ROTATE == Sspm->Settings.flag.display) ||
          ((SPM_DISPLAY_ROTATE_POWERED_ON == Sspm->Settings.flag.display) && (power >> i) &1) ||
          (SPM_DISPLAY_TABS == Sspm->Settings.flag.display)) {
        relays[relay_show] = i +1;
        indirect[relay_show] = i;
        relay_show++;
      }
    }

    if (relay_show) {
      if (Sspm->Settings.flag.display != SPM_DISPLAY_TABS) {
        if (relay_show > 4) {
          Sspm->rotate++;
        } else {
          Sspm->rotate = 0;
        }
      }
      if (Sspm->rotate > ((relay_show -1) | 0x3)) {  // Always test in case relay has changed due to use of SspmDisplay command
        Sspm->rotate = 0;
      }
      uint32_t offset = (Sspm->rotate >> 2) * 4;
      uint32_t count = relay_show - offset;
      if (count > 4) { count = 4; }
      WSContentSend_P(PSTR("</table><hr/>"));        // Close current table as we will use different column count
      if (SPM_DISPLAY_TABS == Sspm->Settings.flag.display) {
        uint32_t modules = relay_show / 4;
        if (modules > 1) {
          WSContentSend_P(PSTR("{t}<tr>"));          // {t} = <table style='width:100%'>
          uint32_t cols_width = 100 / modules;
          uint32_t current_module = Sspm->rotate >> 2;
          for (uint32_t idx = 0; idx < modules; idx++) {
            WSContentSend_P(PSTR("<td style='width:%d%%'><button style='border-radius:0;background:#%06X;font-weight:%s' onclick='la(\"&k86=%d\");'>L%d</button></td>"), // &k86 is related to WebGetArg("k", tmp, sizeof(tmp));
              cols_width, (current_module == idx) ? WebColor(COL_BACKGROUND) : WebColor(COL_FORM), (current_module == idx) ? "bold" : "normal", idx, (idx *4) +1);
          }
          WSContentSend_P(PSTR("</tr></table>"));    // Close current table as we will use different column count
        }
      }
      // {s}</th><th></th><th>Head1</th><th></th><td>{e}
      // {s}</th><th></th><th>Head1</th><th></th><th>Head2</th><th></th><td>{e}
      // {s}</th><th></th><th>Head1</th><th></th><th>Head2</th><th></th><th>Head3</th><th></th><td>{e}
      // {s}</th><th></th><th>Head1</th><th></th><th>Head2</th><th></th><th>Head3</th><th></th><th>Head4</th><th></th><td>{e}
      WSContentSend_P(PSTR("{t}{s}</th><th></th>")); // First column is empty ({t} = <table style='width:100%'>, {s} = <tr><th>)
      char value_chr[SSPM_SIZE];
      bool no_label = false;
      for (uint32_t i = 0; i < count; i++) {
        WSContentSend_P(PSTR("<th style='text-align:center'>%s%s<th></th>"), (no_label)?"":"L", (no_label)?"":itoa(relays[offset +i], value_chr, 10));
      }
      WSContentSend_P(PSTR("<td>{e}"));   // Last column is units ({e} = </td></tr>)
      WSContentSend_PD(HTTP_SNS_VOLTAGE, SSPMEnergyFormat(value_chr, Sspm->voltage[0], Settings->flag2.voltage_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_CURRENT, SSPMEnergyFormat(value_chr, Sspm->current[0], Settings->flag2.current_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_POWER,   SSPMEnergyFormat(value_chr, Sspm->active_power[0], Settings->flag2.wattage_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_POWERUSAGE_APPARENT, SSPMEnergyFormat(value_chr, Sspm->apparent_power[0], Settings->flag2.wattage_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_POWERUSAGE_REACTIVE, SSPMEnergyFormat(value_chr, Sspm->reactive_power[0], Settings->flag2.wattage_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_POWER_FACTOR, SSPMEnergyFormat(value_chr, Sspm->power_factor[0], 2, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_ENERGY_TODAY, SSPMEnergyFormat(value_chr, Sspm->energy_today[0], Settings->flag2.energy_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_ENERGY_YESTERDAY, SSPMEnergyFormat(value_chr, Sspm->Settings.energy_yesterday[0], Settings->flag2.energy_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SNS_ENERGY_TOTAL, SSPMEnergyFormat(value_chr, Sspm->energy_total[0], Settings->flag2.energy_resolution, indirect, offset, count));
      WSContentSend_P(PSTR("</table><hr/>{t}"));    // {t} = <table style='width:100%'> - Define for next FUNC_WEB_SENSOR
    }
#endif  // USE_WEBSERVER
  }
}

#ifdef USE_WEBSERVER

void SSPMWebGetArg(void) {
  char tmp[8];                               // WebGetArg numbers only
  WebGetArg(PSTR("k86"), tmp, sizeof(tmp));  // 1 - 8 relay modules
  if (strlen(tmp)) { Sspm->rotate = atoi(tmp) *4; }
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kSSPMCommands[] PROGMEM = "SSPM|"  // Prefix
  "Display|Dump|"                             // SetOptions
#ifdef SSPM_SIMULATE
  "Simulate|"
#endif  // SSPM_SIMULATE
  "Log|Energy|History|Scan|IamHere|"
  "Reset|Map|Overload|"
  D_CMND_ENERGYTOTAL "|" D_CMND_ENERGYYESTERDAY "|Send|" D_CMND_POWERONSTATE;

void (* const SSPMCommand[])(void) PROGMEM = {
  &CmndSSPMDisplay, &CmndSSPMDump,
#ifdef SSPM_SIMULATE
  &CmndSSPMSimulate,
#endif  // SSPM_SIMULATE
  &CmndSSPMLog, &CmndSSPMEnergy, &CmndSSPMHistory, &CmndSSPMScan, &CmndSSPMIamHere,
  &CmndSSPMReset, &CmndSSPMMap, &CmndSSPMOverload,
  &CmndSpmEnergyTotal, &CmndSpmEnergyYesterday, &CmndSSPMSend, &CmndSSPMPowerOnState };

void CmndSSPMDisplay(void) {
  // Select either all relays, only powered on relays or user selected relay module
  // SspmDisplay 0, SspmDisplay 1 or SspmDisplay 2
  if ((XdrvMailbox.payload >= SPM_DISPLAY_ROTATE) && (XdrvMailbox.payload < SPM_DISPLAY_MAX_OPTION)) {
    Sspm->Settings.flag.display = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Sspm->Settings.flag.display);
}

void CmndSSPMDump(void) {
  // Select either short or full serial dump controlling logging buffer space
  // SspmDump 0 or SspmDump 1
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Sspm->Settings.flag.dump = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Sspm->Settings.flag.dump);
}

#ifdef SSPM_SIMULATE
void CmndSSPMSimulate(void) {
  // Simulate 4Relay modules based on first detected 4Relay module
  // SspmSimulate 0 to turn simulation off
  // SspmSimulate 6 to add 6 simulated modules
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < SSPM_MAX_MODULES)) {
    Sspm->Settings.simulate_count = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Sspm->Settings.simulate_count);
}
#endif  // SSPM_SIMULATE

void CmndSpmEnergyTotal(void) {
  // Reset Energy Total
  // SspmEnergyTotal<relay> 0     - Set total energy from midnight with sum of last month history
  // SspmEnergyTotal<relay> 4.23  - Set total energy from midnight (without today's energy)
  if (Sspm->module_max) {
    uint32_t relay = XdrvMailbox.index -1;
    uint32_t module = relay >> 2;
    uint32_t channel = relay & 0x03;
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.devices_present) && XdrvMailbox.data_len) {
      Sspm->Settings.energy_total[module][channel] = CharToFloat(XdrvMailbox.data);
      Sspm->energy_total[module][channel] = Sspm->Settings.energy_total[module][channel] + Sspm->energy_today[module][channel];
    }
    ResponseCmndFloat(Sspm->energy_total[module][channel], Settings->flag2.energy_resolution);
  }
}

void CmndSpmEnergyYesterday(void) {
  // Reset Energy Yesterday
  // SspmEnergyTotal<relay> 0     - Set total energy from midnight with sum of last month history
  // SspmEnergyTotal<relay> 4.23  - Set total energy from midnight (without today's energy)
  if (Sspm->module_max) {
    uint32_t relay = XdrvMailbox.index -1;
    uint32_t module = relay >> 2;
    uint32_t channel = relay & 0x03;
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.devices_present) && XdrvMailbox.data_len) {
      Sspm->Settings.energy_yesterday[module][channel] = CharToFloat(XdrvMailbox.data);
    }
    ResponseCmndFloat(Sspm->Settings.energy_yesterday[module][channel], Settings->flag2.energy_resolution);
  }
}

void CmndSSPMOverload(void) {
  // Get / Set overload
  // SspmOverload<relay> 0                                  - Reset overload detection parameters
  // SspmOverload<relay> {"Delay":0,"Set":00000,"MinPower":0.10,"MaxPower":4400.00,"MinVoltage":0.10,"MaxVoltage":240.00,"MaxCurrent":20.00} - v1.0.0
  // SspmOverload<relay> {"Delay":0,"Set":00000,"MinPower":0.10,"MaxPower":4800.00,"MinVoltage":90,"MaxVoltage":264.00,"MaxCurrent":20.00}   - v1.2.0
  // SspmOverload<relay> <delay>,<min_power>,<max_power>,<min_voltage>,<max_voltage,<max_current>
  // SspmOverload<relay> 10,0.10,4400.00,0.10,240.00,20.00
  // SspmOverload<relay> 10 0.10 4400.00 0.10 240.00 20.00
  // SspmOverload<relay> 10,12.3                            - Enable overload detection after 10 seconds for MinPower
  // SspmOverload<relay> 10,0,22.2                          - Enable overload detection after 10 seconds for MaxPower
  // SspmOverload<relay> 10,0,0,0,235.2                     - Enable overload detection after 10 seconds for MaxVoltage
  if (Sspm->module_max) {
    if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
    Sspm->overload_relay = XdrvMailbox.index -1;
    bool set_overload = false;
    if (XdrvMailbox.data_len) {
      // Init defaults
      Sspm->overload_delay = 0;                        // No delay
      Sspm->overload_enable = 0x00;                    // Disable overload detection
      Sspm->overload_max_power = Sspm->max_power;      // x.xxVA
      Sspm->overload_min_power = Sspm->min_power;      // x.xxVA
      Sspm->overload_max_voltage = Sspm->max_voltage;  // x.xxV
      Sspm->overload_min_voltage = Sspm->min_voltage;  // x.xxV
      Sspm->overload_max_current = Sspm->max_current;  // x.xxA
      if ((1 == XdrvMailbox.data_len) && (0 == XdrvMailbox.payload)) {  // Set defaults
        // SspmOverload<relay> 0       - Reset overload detection parameters
        set_overload = true;
      }
      else if ('{' == XdrvMailbox.data[0]) {  // Process as JSON
        // SspmOverload<relay> {"Delay":0,"Set":00000,"MinPower":0.10,"MaxPower":4400.00,"MinVoltage":0.10,"MaxVoltage":240.00,"MaxCurrent":20.00}

  //      set_overload = true;
      }
      else if (strchr(XdrvMailbox.data, ',') != nullptr) {
        // SspmOverload<relay> 10,0.10,4400.00,0.10,240.00,20.00
        // SspmOverload<relay> 10 0.10 4400.00 0.10 240.00 20.00
        char *data;
        uint32_t i = 0;
        for (char *str = strtok_r(XdrvMailbox.data, ", ", &data); str && i < 6; str = strtok_r(nullptr, ", ", &data)) {
          float value = CharToFloat(str);
          if (value > 0) {                              // 0 = default and no overload detection
            switch (i++) {
              case 0:
                Sspm->overload_delay = (uint8_t)value;  // Overload detection reaction time in seconds
                break;
              case 1:
                Sspm->overload_enable = 0x08;           // Enable min power overload detection
                Sspm->overload_min_power = value;
                break;
              case 2:
                Sspm->overload_enable = 0x10;           // Enable max power overload detection
                Sspm->overload_max_power = value;
                break;
              case 3:
                Sspm->overload_enable = 0x02;           // Enable min voltage overload detection
                Sspm->overload_min_voltage = value;
                break;
              case 4:
                Sspm->overload_enable = 0x04;           // Enable max voltage overload detection
                Sspm->overload_max_voltage = value;
                break;
              case 5:
                Sspm->overload_enable = 0x01;           // Enable max current overload detection
                Sspm->overload_max_current = value;
                break;
            }
          }
          set_overload = true;
        }
      }
      if (set_overload) {
        SSPMSendOPS(Sspm->overload_relay);
        ResponseClear();
      }
    } else {
      SSPMSendGetOps(Sspm->overload_relay >> 2);
      ResponseClear();
    }
  }
}

void CmndSSPMLog(void) {
  // SspmLog<relay>     - Report from up to 29 latest log entries
  // SspmLog<relay> 10  - Report from up to 10 latest log entries
  // SspmLog<relay> 100 - Report from up to 29 log entries
  if (Sspm->module_max) {
    if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
    if ((XdrvMailbox.payload < 1) || (XdrvMailbox.payload > 65000)) { XdrvMailbox.payload = 28; }
    Sspm->log_relay = XdrvMailbox.index -1;
    SSPMSendGetLog(Sspm->log_relay, XdrvMailbox.payload +1);
    ResponseClear();
  }
}

void CmndSSPMEnergy(void) {
  if (Sspm->module_max) {
    if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
    SSPMSendGetEnergy(XdrvMailbox.index -1);
    ResponseCmndDone();
  }
}

void CmndSSPMHistory(void) {
  // Retreive daily history of one relay up to six month
  // SspmHistory<relay>
  if (Sspm->module_max) {
    if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
    Sspm->history_relay = XdrvMailbox.index -1;
    SSPMSendGetEnergyTotal(Sspm->history_relay);
    ResponseClear();
  }
}

void CmndSSPMScan(void) {
  // Start relay module scan taking up to 20 seconds
  // SspmScan
  Sspm->mstate = SPM_START_SCAN;
  ResponseCmndChar(PSTR(D_JSON_STARTED));
}

void CmndSSPMIamHere(void) {
  // Blink module ERROR led containing relay
  // SspmIamHere 6
  if (Sspm->module_max) {
    if ((XdrvMailbox.payload < 1) || (XdrvMailbox.payload > TasmotaGlobal.devices_present)) { XdrvMailbox.payload = 1; }
    SSPMSendIAmHere(XdrvMailbox.payload -1);
    ResponseCmndDone();
  }
}

void CmndSSPMReset(void) {
  // Reset ARM and restart
  // Reset 1
  switch (XdrvMailbox.payload) {
    case 1:
      TasmotaGlobal.restart_flag = 2;
    case 2:
      Sspm->mstate = SPM_NONE;
      SSPMSendCmnd(SSPM_FUNC_RESET);
      ResponseCmndChar(PSTR(D_JSON_RESET_AND_RESTARTING));
      break;
    default:
      ResponseCmndChar(PSTR(D_JSON_ONE_TO_RESET));
  }
}

void CmndSSPMMap(void) {
  // Map scanned module number to physical module number using positional numbering
  // SspmMap 0        - start a scan to fill default mapping
  // SspmMap 1,3,4,2  - map modules
  // TODO: Might need input checks on count and valid different numbers
  if (0 == XdrvMailbox.payload) {
    for (uint32_t module = 0; module < SSPM_MAX_MODULES; module++) {
      Sspm->Settings.module_map[module] = 0;               // Clear mapping slots
    }
    CmndSSPMScan();                                        // Start scan to fill default mapping
  }
  else if (Sspm->module_max) {                             // Valid after initial scan
    char *p;
    uint32_t i = 0;
    for (char* str = strtok_r(XdrvMailbox.data, ",", &p); str && i < Sspm->module_max; str = strtok_r(nullptr, ",", &p)) {
      uint32_t module = atoi(str);
      if ((module > 0) && (module <= Sspm->module_max)) {  // Only valid modules 1 to x
        Sspm->Settings.module_map[i] = SSMPGetModuleId(module -1);
      }
      i++;
    }
    Response_P(PSTR("{\"%s\":["), XdrvMailbox.command);
    for (uint32_t i = 0; i < Sspm->module_max; i++) {
      ResponseAppend_P(PSTR("%s%d"), (i)?",":"", SSPMGetModuleNumberFromMap(SSMPGetModuleId(i)) +1);
    }
    ResponseAppend_P(PSTR("]}"));
  }
}

void CmndSSPMSend(void) {
  // Want to send AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 21 00 00 ix ch ks
  // SspmSend 00 00 00 00 00 00 00 00 00 00 00 00 00 21 00 00
  char data[TOPSZ];
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(data))) {
    strlcpy(data, XdrvMailbox.data, sizeof(data));
    uint32_t len = (XdrvMailbox.data_len +1) / 3;
    char *p = data;
    SSPMInitSend();
    for (uint32_t i = 0; i < len; i++) {
      SspmBuffer[i +3] = strtol(p, &p, 16);
    }
    Sspm->command_sequence++;
    SspmBuffer[len +3] = Sspm->command_sequence;
    SSPMSend(len +6);

    ResponseCmndIdxChar(ToHex_P((unsigned char *)SspmBuffer, len +6, data, sizeof(data), ' '));
  }
}

void CmndSSPMPowerOnState(void) {
  // SspmPowerOnState2 0|1|2 - Set relay2 power on state (0 = Off, 1 = On, 2 = Saved)
  // Needs both main and 4relay at v1.2.0
  if (Sspm->main_version > SSPM_VERSION_1_0_0) {
    uint32_t max_index = Sspm->module_max *4;
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= max_index)) {
      uint32_t module = (XdrvMailbox.index -1) >>2;
      uint32_t relay = (XdrvMailbox.index -1) &3;
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
        if (XdrvMailbox.payload < 2) { XdrvMailbox.payload = !XdrvMailbox.payload; }  // Swap Tasmota power off (0) with Sonoff (1)
        Sspm->poweron_state[module][relay] = XdrvMailbox.payload;
        SSPMSendSetPowerOnState(module);
      }
      Response_P(PSTR("{\"%s\":["), XdrvMailbox.command);
      bool more = false;
      for (uint32_t module = 0; module < Sspm->module_max; module++) {
        for (uint32_t relay = 0; relay < 4; relay++) {
          uint32_t poweron_state = Sspm->poweron_state[module][relay];
          if (poweron_state < 2) { poweron_state = !poweron_state; }  // Swap Sonoff power off (1) with Tasmota (0)
          ResponseAppend_P(PSTR("%s%d"), (more)?",":"", poweron_state);
          more = true;
        }
      }
      ResponseAppend_P(PSTR("]}"));
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv86(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    SSPMInit();
  }
  else if (Sspm) {
    switch (function) {
      case FUNC_LOOP:
        if (SspmSerial) { SSPMSerialInput(); }
        break;
      case FUNC_EVERY_100_MSECOND:
        SSPMEvery100ms();
        break;
      case FUNC_RESET_SETTINGS:
        Xdrv86SettingsLoad(1);
        break;
      case FUNC_RESTORE_SETTINGS:
        result = Xdrv86SettingsRestore();
        break;
      case FUNC_SAVE_SETTINGS:
        Xdrv86SettingsSave();
        break;
      case FUNC_SET_DEVICE_POWER:
        result = SSPMSetDevicePower();
        break;
      case FUNC_JSON_APPEND:
        SSPMEnergyShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SSPMEnergyShow(false);
        break;
      case FUNC_WEB_GET_ARG:
        SSPMWebGetArg();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kSSPMCommands, SSPMCommand);
        break;
      case FUNC_BUTTON_PRESSED:
        result = SSPMButton();
        break;
    }
  }
  return result;
}

#endif  // USE_SONOFF_SPM
#endif  // USE_ENERGY_SENSOR
#endif  // ESP32
