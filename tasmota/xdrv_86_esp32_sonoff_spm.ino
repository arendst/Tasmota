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
#ifdef USE_SONOFF_SPM
/*********************************************************************************************\
 * Sonoff Stackable Power Manager (Current state: PROOF OF CONCEPT)
 *
 * Initial POC template:
 * {"NAME":"Sonoff SPM (POC1)","GPIO":[1,1,1,1,3200,1,1,1,1,1,1,1,3232,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,544,1,1,32,1,0,0,1],"FLAG":0,"BASE":1}
 * Add ethernet support:
 * {"NAME":"Sonoff SPM (POC2)","GPIO":[1,0,1,0,3200,5536,0,0,1,1,1,0,3232,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,544,1,1,32,1,0,0,1],"FLAG":0,"BASE":1}
 * Remove all user selectable GPIOs:
 * {"NAME":"Sonoff SPM (POC2)","GPIO":[0,0,0,0,3200,5536,0,0,0,0,0,0,3232,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,544,0,0,32,0,0,0,0],"FLAG":0,"BASE":1}
 *
 * Things to know:
 * Bulk of the action is handled by ARM processors present in every unit communicating over modbus RS-485.
 * Each SPM-4Relay has 4 bistable relays with their own CSE7761 energy monitoring device handled by an ARM processor.
 * Green led is controlled by ARM processor indicating SD-Card access.
 * ESP32 is used as interface between eWelink and ARM processor in SPM-Main unit communicating over proprietary serial protocol.
 * Power on sequence for two SPM-4Relay modules is 00-00-15-10-(0F)-(13)-(13)-(19)-0C-09-04-09-04-0B-0B
 *
 * Tasmota POC1:
 * Up to 7 SPM-4Relay units supporting up to 28 relays.
 * Gui rotating energy display for 4 relays at a time.
 * Button on SPM-Main initiates re-scan of SPM-4Relay units.
 * Blue led equals Tasmota WiFi status.
 *
 * Tasmota POC2:
 * Ethernet support.
 * Gui optimized for energy display.
 * Yellow led lights if no ARM connection can be made.
 * Yellow led blinks 2 seconds if an ARM-ESP comms CRC error is detected.
 * Supported commands:
 *   SspmDisplay 0|1     - Select alternative GUI rotating display either all or powered on only
 *   SspmIAmHere<relay>  - Blink ERROR in SPM-4Relay where relay resides
 *   SspmScan            - Rescan ARM modbus taking around 20 seconds
 *   SspmReset 1         - Reset ARM and restart ESP
 *
 * Todo:
 * Gui for Overload Protection entry (is handled by ARM processor).
 * Gui for Scheduling entry (is handled by ARM processor).
 * SPI master to ARM (ARM firmware upload from ESP using EasyFlash).
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
 * GPIO12 - SPI MOSI ARM output (pin36 - PB15) - ESP input
 * GPIO13 - SPI MISO ESP output - ARM input (pin35 - PB14)
 * GPIO14 - SPI CLK ESP input (ARM pin34 - PB13)
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

#define SSPM_JSON_ENERGY_TODAY               // Show JSON energy today
#define SSPM_JSON_ENERGY_YESTERDAY           // Show JSON energy yesterday

/*********************************************************************************************\
 * Fixed defines - Do not change
\*********************************************************************************************/

#define XDRV_86                      86

#define SSPM_MAX_MODULES             7       // Currently supports up to 7 SPM-4RELAY units for a total of 28 relays restricted by power_t size
#define SSPM_SERIAL_BUFFER_SIZE      512     // Needs to accomodate Energy total history for six months (408 bytes)

// Send
#define SSPM_FUNC_FIND               0       // 0x00
#define SSPM_FUNC_SET_OPS            3       // 0x03 - Overload Protection
#define SSPM_FUNC_GET_OPS            4       // 0x04
#define SSPM_FUNC_SET_RELAY          8       // 0x08
#define SSPM_FUNC_GET_MODULE_STATE   9       // 0x09 - State of four channels
#define SSPM_FUNC_SET_SCHEME         10      // 0x0A
#define SSPM_FUNC_GET_SCHEME         11      // 0x0B
#define SSPM_FUNC_SET_TIME           12      // 0x0C
#define SSPM_FUNC_IAMHERE            13      // 0x0D
#define SSPM_FUNC_INIT_SCAN          16      // 0x10
#define SSPM_FUNC_UNITS              21      // 0x15
#define SSPM_FUNC_GET_ENERGY_TOTAL   22      // 0x16
#define SSPM_FUNC_GET_ENERGY         24      // 0x18
#define SSPM_FUNC_GET_LOG            26      // 0x1A
#define SSPM_FUNC_ENERGY_PERIOD      27      // 0x1B
#define SSPM_FUNC_RESET              28      // 0x1C - Remove device from eWelink and factory reset

// Receive
#define SSPM_FUNC_ENERGY_RESULT      6       // 0x06
#define SSPM_FUNC_KEY_PRESS          7       // 0x07
#define SSPM_FUNC_SCAN_START         15      // 0x0F
#define SSPM_FUNC_SCAN_RESULT        19      // 0x13
#define SSPM_FUNC_SCAN_DONE          25      // 0x19

#define SSPM_GPIO_ARM_RESET          15
#define SSPM_GPIO_LED_ERROR          33

#define SSPM_MODULE_NAME_SIZE        12

/*********************************************************************************************/

enum SspmMachineStates { SPM_NONE,                 // Do nothing
                         SPM_WAIT,                 // Wait 100ms
                         SPM_RESET,                // Toggle ARM reset pin
                         SPM_POLL_ARM,             // Wait for first acknowledge from ARM after reset
                         SPM_POLL_ARM_2,           // Wait for second acknowledge from ARM after reset
                         SPM_SEND_FUNC_UNITS,      // Get number of units
                         SPM_START_SCAN,           // Start module scan sequence
                         SPM_WAIT_FOR_SCAN,        // Wait for scan sequence to complete
                         SPM_SCAN_COMPLETE,        // Scan complete
                         SPM_GET_ENERGY_TOTALS,    // Init available Energy totals registers
                         SPM_UPDATE_CHANNELS       // Update Energy for powered on channels
                         };

#include <TasmotaSerial.h>
TasmotaSerial *SspmSerial;

typedef struct {
  float voltage[SSPM_MAX_MODULES][4];             // 123.12 V
  float current[SSPM_MAX_MODULES][4];             // 123.12 A
  float active_power[SSPM_MAX_MODULES][4];        // 123.12 W
  float apparent_power[SSPM_MAX_MODULES][4];      // 123.12 VA
  float reactive_power[SSPM_MAX_MODULES][4];      // 123.12 VAr
  float power_factor[SSPM_MAX_MODULES][4];        // 0.12
  float energy_today[SSPM_MAX_MODULES][4];        // 12345 kWh
  float energy_yesterday[SSPM_MAX_MODULES][4];    // 12345 kWh
  float energy_total[SSPM_MAX_MODULES][4];        // 12345 kWh total energy since last 6 month!!!

  uint32_t timeout;
  power_t old_power;
  uint16_t last_totals;
  uint16_t serial_in_byte_counter;
  uint16_t expected_bytes;
  uint8_t module[SSPM_MAX_MODULES][SSPM_MODULE_NAME_SIZE];

  uint8_t allow_updates;
  uint8_t get_energy_relay;
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
  bool discovery_triggered;
} TSspm;

uint8_t *SspmBuffer = nullptr;
TSspm *Sspm = nullptr;

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

void SSPMSendOPS(uint32_t relay_num) {
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

  Acknowledge:
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 80 03 00 01 00 14 08 bc
                                              |Ac|Cm|Size |Rt|Ix|Chksm|
  Ac - Acknowledge or error number
  Rt - Return code
  */

  SspmBuffer[16] = SSPM_FUNC_SET_OPS;  // 0x03

}

void SSPMSendGetOps(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  aa 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 04 00 00 08 c0 0a
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  uint8_t channel = relay & 0x03;  // Channel relays are NOT bit masked this time
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_ENERGY_TOTAL;  // 0x16
  SspmBuffer[18] = 0x0D;
  memcpy(SspmBuffer +19, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  uint8_t channel = 1 << (relay & 0x03);  // Channel relays are bit masked
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_ENERGY;  // 0x18
  SspmBuffer[18] = 0x10;
  memcpy(SspmBuffer +19, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
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
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1a 00 10 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 00 00 1d 09 8c cd
  Marker  |                                   |  |Cm|Size |Module id                          |Start|End  |Ix|Chksm|
  Start = newest log start number  (Latest is 0)
  End   = older log end number     (End - Start >= 29 (0x1d))
  */
  uint8_t module = relay >> 2;
  uint32_t startlog = (entries >= 29) ? entries -29 : 0;
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_LOG;  // 0x1A
  SspmBuffer[18] = 0x10;
  memcpy(SspmBuffer +19, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[31] = startlog >> 8;    // MSB start log
  SspmBuffer[32] = startlog;         // LSB start log
  SspmBuffer[33] = entries >> 8;     // MSB end log
  SspmBuffer[34] = entries;          // LSB end log - Number of logs
  Sspm->command_sequence++;
  SspmBuffer[35] = Sspm->command_sequence;

  SSPMSend(38);
}

/*********************************************************************************************/

void SSPMHandleReceivedData(void) {
  uint8_t command = SspmBuffer[16];
  bool ack = (0x80 == SspmBuffer[15]);
  uint8_t command_sequence = SspmBuffer[19 + Sspm->expected_bytes];

//  AddLog(LOG_LEVEL_DEBUG, PSTR("SPM: Rcvd ack %d, cmnd %d, seq %d, size %d"),
//    ack, command, command_sequence, Sspm->expected_bytes);

  if (ack) {
    // Responses from ARM (Acked)
    switch(command) {
      case SSPM_FUNC_FIND:
        /* 0x00
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 01 00 00 fc 73
                                                    |Er|        |St|
        */
        if ((1 == Sspm->expected_bytes) && (0 == SspmBuffer[19])) {
          Sspm->mstate++;   // Cycle to
        }
        break;
      case SSPM_FUNC_GET_OPS:
        /* 0x04 - Overload Protection
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 04 00 02 00 00 06 98 06
        Marker  |Module id                          |Ac|Cm|Size |     |Ix|Chksm|

        AA 55 01 6B 7E 32 37 39 37 34 13 4B 35 36 37 80 04 00 35 00 07 00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00
                                                                       00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00
                                                                       00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00 07 8A 86
        Marker  |Module id                          |Ac|Cm|Size |  |Ch|Ra|Max P   |Min P   |Max U   |Min U   |Max I   |De|Ix|Chksm|
                                                                   |  |  |   4400W|    0.1W|    240V|    0.1V|     20A|  |
        */
        if (0x02 == Sspm->expected_bytes) {

        }

        Sspm->module_selected--;
        if (Sspm->module_selected > 0) {
          SSPMSendGetModuleState(Sspm->module_selected -1);
        } else {
          SSPMSendGetScheme(Sspm->module_selected);
        }
        break;
      case SSPM_FUNC_GET_MODULE_STATE:
        /* 0x09
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 09 00 06 00 0f 01 01 01 01 05 fe 35
                                                                   |OS|4RelayMasks|
        */
        if (0x06 == Sspm->expected_bytes) {
          // SspmBuffer[20] & 0x0F                      // Relays operational
          power_t current_state = SspmBuffer[20] >> 4;  // Relays state
          power_t mask = 0x0000000F;
          for (uint32_t i = 0; i < Sspm->module_max; i++) {
            if ((SspmBuffer[3] == Sspm->module[i][0]) && (SspmBuffer[4] == Sspm->module[i][1])) {
              current_state <<= (i * 4);
              mask <<= (i * 4);
              TasmotaGlobal.power &= (POWER_MASK ^ mask);
              TasmotaGlobal.power |= current_state;
              break;
            }
          }
          Sspm->old_power = TasmotaGlobal.power;
          TasmotaGlobal.devices_present += 4;
        }
        SSPMSendGetOps(Sspm->module_selected -1);
        break;
      case SSPM_FUNC_GET_SCHEME:
        /* 0x0B
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
        AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 80 0b 00 02 00 00 09 bb c7
                                                                |?? ??|
        */
        if (0x02 == Sspm->expected_bytes) {

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
        */
        TasmotaGlobal.devices_present = 0;
        SSPMSendGetModuleState(Sspm->module_selected -1);
        break;
      case SSPM_FUNC_INIT_SCAN:
        /* 0x10
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 ff ff ff ff ff ff ff ff ff ff ff ff 80 10 00 01 00 02 e5 03
        */
        break;
      case SSPM_FUNC_UNITS:
        /* 0x15
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 15 00 04 00 01 00 00 01 81 b1
                                                                |?? ?? ?? ??|
        */
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
        */
        {
          float energy_today = 0;
          float energy_yesterday = 0;
          float energy_total = 0;
          uint32_t entries = (Sspm->expected_bytes - 22) / 2;

          for (uint32_t i = 0; i < entries; i++) {
            float today_energy = SspmBuffer[41 + (i*2)] + (float)SspmBuffer[42 + (i*2)] / 100;   // x.xxkWh
            if (112.30 == today_energy) { today_energy = 0; }  // Unknown why sometimes 0x701E (=112.30kWh) pops up
            if (0 == i) { energy_today = today_energy; }
            if (1 == i) { energy_yesterday = today_energy; }
            energy_total += today_energy;
          }
          uint32_t channel = SspmBuffer[32];
          for (uint32_t module = 0; module < Sspm->module_max; module++) {
            if ((SspmBuffer[20] == Sspm->module[module][0]) && (SspmBuffer[21] == Sspm->module[module][1])) {
              Sspm->energy_today[module][channel] = energy_today;
              Sspm->energy_yesterday[module][channel] = energy_yesterday;
              Sspm->energy_total[module][channel] = energy_total;  // x.xxkWh
              break;
            }
          }
          Sspm->allow_updates = 1;
        }
        break;
      case SSPM_FUNC_GET_LOG:
        /* 0x1A
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
        07 e5 0b 06 0f 1f 04 02 00 01
        07 e5 0b 06 0f 1e 32 01 00 01
        07 e5 0b 06 0f 1e 1e 01 01 01
        07 e5 0b 06 0f 18 38 02 01 01
        07 e5 0b 06 0f 12 38 00 01 01
        07 e5 0b 06 0e 37 36 03 00 00
        07 e5 0b 06 0e 37 36 01 00 00
        07 e5 0b 06 0e 37 1e 03 01 00
        07 e5 0b 06 0e 36 37 01 01 00
        ...
        07 e5 0b 06 0d 30 2d 03 00 01 09 89 fe

        Error:
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1A 00 01 03 E5 45 EB
                                                                |  |
        */

        break;
      case SSPM_FUNC_ENERGY_PERIOD:
        /* 0x1B
        Response after start energy period
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 0e [00] 8b 34 32 37 39 37 34 13 4b 35 36 37 [03] f7 b1 bc              L4
        Response after refresh or stop energy period
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 11 [00] 8b 34 32 37 39 37 34 13 4b 35 36 37 [03] [00 00 00] f8 94 15   L4, kWh start period (0)
        */

        break;
      case SSPM_FUNC_RESET:
        /* 0x1C
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1c 00 01 00 0b f9 e3
        */
//        TasmotaGlobal.restart_flag = 2;
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
          for (uint32_t module = 0; module < Sspm->module_max; module++) {
            if ((SspmBuffer[19] == Sspm->module[module][0]) && (SspmBuffer[20] == Sspm->module[module][1])) {
              Sspm->current[module][channel] = SspmBuffer[32] + (float)SspmBuffer[33] / 100;                                 // x.xxA
              Sspm->voltage[module][channel] = (SspmBuffer[34] << 8) + SspmBuffer[35] + (float)SspmBuffer[36] / 100;         // x.xxV
              Sspm->active_power[module][channel] = (SspmBuffer[37] << 8) + SspmBuffer[38] + (float)SspmBuffer[39] / 100;    // x.xxW
              Sspm->reactive_power[module][channel] = (SspmBuffer[40] << 8) + SspmBuffer[41] + (float)SspmBuffer[42] / 100;  // x.xxVAr
              Sspm->apparent_power[module][channel] = (SspmBuffer[43] << 8) + SspmBuffer[44] + (float)SspmBuffer[45] / 100;  // x.xxVA
              float power_factor = (Sspm->active_power[module][channel] && Sspm->apparent_power[module][channel]) ? Sspm->active_power[module][channel] / Sspm->apparent_power[module][channel] : 0;
              if (power_factor > 1) { power_factor = 1; }
              Sspm->power_factor[module][channel] = power_factor;
              break;
            }
          }
          SSPMSendAck(command_sequence);
          Sspm->allow_updates = 1;
        }
        break;
      case SSPM_FUNC_KEY_PRESS:
        /* 0x07
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 07 00 0d 6b 7e 32 37 39 37 34 13 4b 35 36 37 11 04 bf c3
                                                                                                    |AS|
        */
        if (!Sspm->no_send_key) {
          power_t relay = SspmBuffer[31] & 0x0F;      // Relays active
          power_t relay_state = SspmBuffer[31] >> 4;  // Relays state
          for (uint32_t i = 0; i < Sspm->module_max; i++) {
            if ((SspmBuffer[19] == Sspm->module[i][0]) && (SspmBuffer[20] == Sspm->module[i][1])) {
              relay <<= (i * 4);
              relay_state <<= (i * 4);
              break;
            }
          }
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
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0f 00 01 02 01 9d f8
        */
        SSPMSendAck(command_sequence);
        break;
      case SSPM_FUNC_SCAN_RESULT:
        /* 0x13
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 6b 7e 32 37 39 37 34 13 4b 35 36 37 04 00 00 00 82 01 00 00 14 00 00 0a 00 f0 00 00 00 0a 11 30 00 00 00 0a 02 8f cd
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 8b 34 32 37 39 37 34 13 4b 35 36 37 04 00 00 00 82 01 00 00 14 00 00 0a 00 f0 00 00 00 0a 11 30 00 00 00 0a 02 a0 6f
        Marker  |                                   |Ac|Cm|Size |Module id                          |Ch|        |Ty|FwVersio|Max I|Min I|Max U   |Min U   |Max P   |Min P   |Ix|Chksm|
                                                                                                                |130|  1.0.0|  20A| 0.1A|    240V|    0.1V|   4400W|    0.1W|
        Ty = Type of sub-device. 130: Four-channel sub-device
        */
        if ((0x24 == Sspm->expected_bytes) && (Sspm->module_max < SSPM_MAX_MODULES)) {
          memcpy(Sspm->module[1], Sspm->module[0], (SSPM_MAX_MODULES -1) * SSPM_MODULE_NAME_SIZE);
          memcpy(Sspm->module[0], SspmBuffer + 19, SSPM_MODULE_NAME_SIZE);
          Sspm->module_max++;
        }
        SSPMSendAck(command_sequence);
        break;
      case SSPM_FUNC_SCAN_DONE:
        /* 0x19
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 19 00 00 03 a1 16
        */
        SSPMSendAck(command_sequence);
        Sspm->module_selected = Sspm->module_max;
        SSPMSendSetTime();
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

    if ((0x55 == serial_in_byte) && (0xAA == SspmBuffer[Sspm->serial_in_byte_counter -1])) {
      Sspm->expected_bytes = 0;
      SspmBuffer[0] = 0xAA;
      Sspm->serial_in_byte_counter = 1;
    }
    if (Sspm->serial_in_byte_counter < SSPM_SERIAL_BUFFER_SIZE -1) {
      SspmBuffer[Sspm->serial_in_byte_counter++] = serial_in_byte;
      if (19 == Sspm->serial_in_byte_counter) {
        Sspm->expected_bytes = (SspmBuffer[Sspm->serial_in_byte_counter -2] << 8) + SspmBuffer[Sspm->serial_in_byte_counter -1];
      }
      if (Sspm->serial_in_byte_counter == (22 + Sspm->expected_bytes)) {

        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SPM: ARM %*_H"), Sspm->serial_in_byte_counter, SspmBuffer);

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

void SSPMInit(void) {
  if (!ValidTemplate(PSTR("Sonoff SPM (POC1)")) &&
      !ValidTemplate(PSTR("Sonoff SPM (POC2)"))) { return; }
  if (!PinUsed(GPIO_RXD) || !PinUsed(GPIO_TXD)) { return; }

  Sspm = (TSspm*)calloc(sizeof(TSspm), 1);
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

  pinMode(SSPM_GPIO_ARM_RESET, OUTPUT);
  digitalWrite(SSPM_GPIO_ARM_RESET, 1);

  pinMode(SSPM_GPIO_LED_ERROR, OUTPUT);
  digitalWrite(SSPM_GPIO_LED_ERROR, 0);

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

  Sspm->old_power = TasmotaGlobal.power;
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
  if ((Sspm->mstate > SPM_NONE) && (Sspm->mstate < SPM_SEND_FUNC_UNITS)) {
    Sspm->counter++;
    if (Sspm->counter > 20) {
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
    case SPM_POLL_ARM_2:
      // Wait for second acknowledge from ARM after reset
      SSPMSendCmnd(SSPM_FUNC_FIND);
      break;
    case SPM_SEND_FUNC_UNITS:
      // Get number of units
      SSPMSendCmnd(SSPM_FUNC_UNITS);
      break;
    case SPM_START_SCAN:
      // Start scan module sequence
      Sspm->module_max = 0;
      SSPMSendInitScan();
      Sspm->mstate = SPM_WAIT_FOR_SCAN;
      break;
    case SPM_WAIT_FOR_SCAN:
      // Wait for scan sequence to complete
      break;
    case SPM_SCAN_COMPLETE:
      // Scan sequence finished
      TasmotaGlobal.discovery_counter = 1;      // Force TasDiscovery()
      Sspm->get_energy_relay = 0;
      Sspm->allow_updates = 1;                  // Enable requests from 100mSec loop
      Sspm->mstate = SPM_GET_ENERGY_TOTALS;
      break;
    case SPM_GET_ENERGY_TOTALS:
      // Retrieve Energy total status from up to 128 relays
      if (Sspm->allow_updates) {
        SSPMSetLock(4);
        SSPMSendGetEnergyTotal(Sspm->get_energy_relay);
        Sspm->get_energy_relay++;
        if (Sspm->get_energy_relay >= TasmotaGlobal.devices_present) {
          Sspm->get_energy_relay = TasmotaGlobal.devices_present;
          Sspm->mstate = SPM_UPDATE_CHANNELS;
        }
      }
      break;
    case SPM_UPDATE_CHANNELS:
      // Retrieve Energy status from up to 128 powered on relays (takes 128 * 2s!!)
      if (Sspm->allow_updates) {
        Sspm->get_energy_relay++;
        if (Sspm->get_energy_relay >= TasmotaGlobal.devices_present) {
          Sspm->get_energy_relay = 0;
          if (Sspm->last_totals > 1200) {  // Get totals every 2 minutes (takes 128 * 0.2s)
            Sspm->last_totals = 0;
            Sspm->get_totals = 1;
          } else {
            Sspm->get_totals = 0;
          }
        }
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
      break;

  }
}

/*********************************************************************************************/

bool SSPMSetDevicePower(void) {
  power_t new_power = XdrvMailbox.index;
  if (new_power != Sspm->old_power) {
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      uint8_t new_state = (new_power >> i) &1;
      if (new_state != ((Sspm->old_power >> i) &1)) {
        SSPMSendSetRelay(i, new_state);
        Sspm->no_send_key = 10;  // Disable buttons for 10 * 0.1 second
      }
    }
    Sspm->old_power = new_power;
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

const uint16_t SSPM_SIZE = 128;

char* SSPMEnergyFormat(char* result, float* input, uint32_t resolution, uint8_t* indirect, uint8_t offset, uint32_t count) {
  result[0] = '\0';
  for (uint32_t i = 0; i < count; i++) {
    ext_snprintf_P(result, SSPM_SIZE, PSTR("%s<td>%*_f</td>"), result, resolution, &input[indirect[offset +i]]);
  }
  ext_snprintf_P(result, SSPM_SIZE, PSTR("%s<td style='white-space:nowrap'>"), result);
  return result;
}

const char HTTP_SSPM_VOLTAGE[] PROGMEM =
  "{s}" D_VOLTAGE "</th>%s" D_UNIT_VOLT "{e}";  // {s} = <tr><th>, {m} = </th><td style='width:20px;white-space:nowrap'>, {e} = </td></tr>
const char HTTP_SSPM_CURRENT[] PROGMEM =
  "{s}" D_CURRENT "</th>%s" D_UNIT_AMPERE "{e}";
const char HTTP_SSPM_POWER[] PROGMEM =
  "{s}" D_POWERUSAGE_ACTIVE "</th>%s" D_UNIT_WATT "{e}";
const char HTTP_SSPM_POWER2[] PROGMEM =
  "{s}" D_POWERUSAGE_APPARENT "</th>%s" D_UNIT_VA "{e}"
  "{s}" D_POWERUSAGE_REACTIVE "</th>%s" D_UNIT_VAR "{e}"
  "{s}" D_POWER_FACTOR "</th>%s{e}";
const char HTTP_SSPM_ENERGY[] PROGMEM =
  "{s}" D_ENERGY_TODAY "</th>%s" D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "</th>%s" D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "</th>%s" D_UNIT_KILOWATTHOUR "{e}";

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
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", Settings->flag2.energy_resolution, &Sspm->energy_yesterday[i >>2][i &3]);
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
  } else {
    uint8_t relay[SSPM_MAX_MODULES * 4];
    uint8_t indirect[SSPM_MAX_MODULES * 4];

    uint32_t index = 0;
    power_t power = TasmotaGlobal.power;
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      if ((0 == Settings->sbflag1.sspm_display) ||
          ((1 == Settings->sbflag1.sspm_display) && (power >> i) &1)) {
        relay[index] = i +1;
        indirect[index] = i;
        index++;
      }
    }

    if (index) {
      uint32_t offset = 0;
      if (index > 4) {
        Sspm->rotate++;
        if (Sspm->rotate >= ((index -1) >> 2) << 3) {
          Sspm->rotate = 0;
        }
        offset = (Sspm->rotate >> 2) * 4;
      }
      uint32_t count = index - offset;
      if (count > 4) { count = 4; }
      WSContentSend_P(PSTR("</table>{t}{s}")); // First column is empty ({t} = <table style='width:100%'>, {s} = <tr><th>)
      for (uint32_t i = 0; i < count; i++) {
        WSContentSend_P(PSTR("</th><th style='width:60px;white-space:nowrap'>L%d"), relay[offset +i]);
      }
      WSContentSend_P(PSTR("</th><td>{e}"));   // Last column is units ({e} = </td></tr>)
      char value_chr[SSPM_SIZE];
      WSContentSend_PD(HTTP_SSPM_VOLTAGE, SSPMEnergyFormat(value_chr, Sspm->voltage[0], Settings->flag2.voltage_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SSPM_CURRENT, SSPMEnergyFormat(value_chr, Sspm->current[0], Settings->flag2.current_resolution, indirect, offset, count));
      WSContentSend_PD(HTTP_SSPM_POWER,   SSPMEnergyFormat(value_chr, Sspm->active_power[0], Settings->flag2.wattage_resolution, indirect, offset, count));
      char valu2_chr[SSPM_SIZE];
      char valu3_chr[SSPM_SIZE];
      WSContentSend_PD(HTTP_SSPM_POWER2,  SSPMEnergyFormat(value_chr, Sspm->apparent_power[0], Settings->flag2.wattage_resolution, indirect, offset, count),
                                          SSPMEnergyFormat(valu2_chr, Sspm->reactive_power[0], Settings->flag2.wattage_resolution, indirect, offset, count),
                                          SSPMEnergyFormat(valu3_chr, Sspm->power_factor[0], 2, indirect, offset, count));
      WSContentSend_PD(HTTP_SSPM_ENERGY,  SSPMEnergyFormat(value_chr, Sspm->energy_today[0], Settings->flag2.energy_resolution, indirect, offset, count),
                                          SSPMEnergyFormat(valu2_chr, Sspm->energy_yesterday[0], Settings->flag2.energy_resolution, indirect, offset, count),
                                          SSPMEnergyFormat(valu3_chr, Sspm->energy_total[0], Settings->flag2.energy_resolution, indirect, offset, count));
      WSContentSend_P(PSTR("</table>{t}"));    // {t} = <table style='width:100%'> - Define for next FUNC_WEB_SENSOR
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kSSPMCommands[] PROGMEM = "SSPM|"  // Prefix
  "Log|Energy|History|Scan|IamHere|Display|Reset" ;

void (* const SSPMCommand[])(void) PROGMEM = {
  &CmndSSPMLog, &CmndSSPMEnergy, &CmndSSPMEnergyHistory, &CmndSSPMScan, &CmndSSPMIamHere, &CmndSSPMDisplay, &CmndSSPMReset };

void CmndSSPMLog(void) {
  // Report 29 log entries
  if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
  XdrvMailbox.payload &= 0xFFFF;  // Max 65000 entries
  SSPMSendGetLog(XdrvMailbox.index -1, XdrvMailbox.payload +1);
  ResponseCmndDone();
}

void CmndSSPMEnergy(void) {
  if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
  SSPMSendGetEnergy(XdrvMailbox.index -1);
  ResponseCmndDone();
}

void CmndSSPMEnergyHistory(void) {
  if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
  SSPMSendGetEnergyTotal(XdrvMailbox.index -1);
  ResponseCmndDone();
}

void CmndSSPMScan(void) {
  // Start relay module scan taking up to 20 seconds
  Sspm->mstate = SPM_START_SCAN;
  ResponseCmndChar(PSTR(D_JSON_STARTED));
}

void CmndSSPMIamHere(void) {
  // Blink module ERROR led containing relay
  if ((XdrvMailbox.payload < 1) || (XdrvMailbox.payload > TasmotaGlobal.devices_present)) { XdrvMailbox.payload = 1; }
  SSPMSendIAmHere(XdrvMailbox.payload -1);
  ResponseCmndDone();
}

void CmndSSPMDisplay(void) {
  // Select either all relays or only powered on relays
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->sbflag1.sspm_display = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->sbflag1.sspm_display);
}

void CmndSSPMReset(void) {
  // Reset ARM and restart
  if (1 == XdrvMailbox.payload) {
    Sspm->mstate = SPM_NONE;
    SSPMSendCmnd(SSPM_FUNC_RESET);
    TasmotaGlobal.restart_flag = 3;
    ResponseCmndChar(PSTR(D_JSON_RESET_AND_RESTARTING));
  } else {
    ResponseCmndChar(PSTR(D_JSON_ONE_TO_RESET));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv86(uint8_t function) {
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
#endif  // ESP32
