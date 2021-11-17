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
 * {"NAME":"Sonoff SPM (POC1)","GPIO":[1,1,1,1,3200,1,1,1,1,1,1,1,3232,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,544,1,1,32,1,0,0,1],"FLAG":0,"BASE":1}
 *
 * Things to know:
 * Bulk of the action is handled by ARM processors present in every unit communicating over modbus RS-485.
 * Each SPM-4Relay has 4 bistable relays with their own CSE7761 energy monitoring device handled by an ARM processor.
 * Green led is controlled by ARM processor indicating SD-Card access.
 * ESP32 is used as interface between Welink and ARM processor in SPM-Main unit communicating over proprietary serial protocol.
 * Inductive/Capacitive loads are not reported correctly.
 * Power on sequence for two SPM-4Relay modules is 00-00-15-10-(0F)-(13)-(13)-(19)-0C-09-04-09-04-0B-0B
 *
 * Tasmota POC1:
 * Up to 7 SPM-4Relay units supporting up to 28 relays.
 * Gui rotating energy display for 4 relays at a time.
 * Button on SPM-Main initiates re-scan of SPM-4Relay units.
 * Blue led equals Tasmota WiFi status.
 *
 * Todo:
 * Ethernet support (Find correct MDIO, MDC, POWER GPIO's and ETH_ parameters).
 * Gui optimization for energy display.
 * Gui for Overload Protection entry (is handled by ARM processor).
 * Gui for Scheduling entry (is handled by ARM processor).
 * Yellow led functionality.
 * Interpretation of reset sequence on GPIO's 12-14.
 *
 * Nice to have:
 * Support for all 32 SPM4Relay units equals 128 relays
 *
 * GPIO's used:
 * GPIO00 - Bootmode / serial flash
 * GPIO01 - Serial console TX (921600bps8N1 originally)
 * GPIO03 - Serial console RX
 * GPIO04 - ARM processor TX (115200bps8N1)
 * GPIO12 - SPI MISO ARM pulsetrain code (input?)
 * GPIO13 - SPI CLK
 * GPIO14 - SPI CS ARM pulsetrain eoc (input?)
 * GPIO15 - ARM reset (output) - 18ms low active 125ms after restart esp32
 * GPIO16 - ARM processor RX
 * GPIO17 - EMAC_CLK_OUT_180
 * GPIO18 - ??ETH MDIO
 * GPIO19 - EMAC_TXD0(RMII)
 * GPIO21 - EMAC_TX_EN(RMII)
 * GPIO22 - EMAC_TXD1(RMII)
 * GPIO23 - ??ETH MDC
 * GPIO25 - EMAC_RXD0(RMII)
 * GPIO26 - EMAC_RXD1(RMII)
 * GPIO27 - EMAC_RX_CRS_DV
 * GPIO?? - ??ETH POWER
 * GPIO32 - Blue status led2
 * GPIO33 - Yellow error led3
 * GPIO35 - Button
 * #define ETH_TYPE          ETH_PHY_LAN8720
 * #define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN
 * #define ETH_ADDRESS       0
 *
 * Variables used:
 * module  = 0 to 31                   SPM-4Relays
 * channel = 0 to 3 or 01, 02, 04, 08  Bitmask of four relays in module
 * relay   = 0 to 127                  Relays
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
#define SSPM_FUNC_INIT_SCAN          16      // 0x10
#define SSPM_FUNC_UNITS              21      // 0x15
#define SSPM_FUNC_GET_ENERGY_TOTAL   22      // 0x16
#define SSPM_FUNC_GET_ENERGY         24      // 0x18
#define SSPM_FUNC_GET_LOG            26      // 0x1A

#define SSPM_FUNC_ENERGY_PERIOD      27      // 0x1B

// Receive
#define SSPM_FUNC_ENERGY_RESULT      6       // 0x06
#define SSPM_FUNC_KEY_PRESS          7       // 0x07
#define SSPM_FUNC_SCAN_START         15      // 0x0F
#define SSPM_FUNC_SCAN_RESULT        19      // 0x13
#define SSPM_FUNC_SCAN_DONE          25      // 0x19

#define SSPM_GPIO_ARM_TX             4
#define SSPM_GPIO_ARM_RX             16
#define SSPM_GPIO_ARM_RESET          15
#define SSPM_GPIO_PULSE_OUT          13
#define SSPM_GPIO_PULSE_IN1          12
#define SSPM_GPIO_PULSE_IN2          14
#define SSPM_GPIO_LED_STATUS         32
#define SSPM_GPIO_LED_ERROR          33

#define SSPM_MODULE_NAME_SIZE        12

enum SspmInitSteps { SPM_NONE, SPM_WAIT, SPM_RESET, SPM_POLL, SPM_POLL_ACK, SPM_SEND_FUNC_UNITS, SPM_STEP_WAIT2,
                     SPM_DEVICES_FOUND,
                     SPM_GET_ENERGY_TOTALS,
                     SPM_ALLOW_LOOP};

#include <TasmotaSerial.h>
TasmotaSerial *SspmSerial;

typedef struct {
  float voltage[SSPM_MAX_MODULES][4];             // 123.12 V
  float current[SSPM_MAX_MODULES][4];             // 123.12 A
  float active_power[SSPM_MAX_MODULES][4];        // 123.12 W
  float apparent_power[SSPM_MAX_MODULES][4];      // 123.12 VA
  float reactive_power[SSPM_MAX_MODULES][4];      // 123.12 VAr
  float power_factor[SSPM_MAX_MODULES][4];        // 0.12
  float total[SSPM_MAX_MODULES][4];               // 12345 kWh total energy

  uint32_t timeout;
  power_t old_power;
  uint16_t serial_in_byte_counter;
  uint16_t expected_bytes;
  uint8_t module[SSPM_MAX_MODULES][SSPM_MODULE_NAME_SIZE];

  uint8_t allow_updates;
  uint8_t get_energy_relay;
  uint8_t rotate;
  uint8_t module_max;
  uint8_t module_selected;
  uint8_t no_send_key;
  uint8_t counter;
  uint8_t command_sequence;
  uint8_t loop_step;
  uint8_t init_step;
  uint8_t last_button;
  bool discovery_triggered;
} TSspm;

uint8_t *SspmBuffer = nullptr;
TSspm *Sspm = nullptr;

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

void SSPMTime(uint8_t *frame) {
  /*
   0  1  2  3  4  5  6
  YY YY MM DD HH MM SS
  07 e5 0b 06 0c 39 01
  */
  TIME_T time;
  BreakTime(Rtc.utc_time, time);
  uint16_t year = time.year + 1970;
  frame[0] = year >> 8;
  frame[1] = year;
  frame[2] = time.month;
  frame[3] = time.day_of_month;
  frame[4] = time.hour;
  frame[5] = time.minute;
  frame[6] = time.second;
}

void SSPMSend(uint32_t size) {
  uint16_t crc = SSPMCalculateCRC(SspmBuffer, size -2);
  SspmBuffer[size -2] = (uint8_t)(crc >> 8);
  SspmBuffer[size -1] = (uint8_t)crc;

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SPM: ESP %*_H"), size, SspmBuffer);

  SspmSerial->write(SspmBuffer, size);
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

void SSPMSendSetTime(void) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 0c 00 0b 07 e5 0b 06 0c 39 01 00 00 02 00 04 8a 37
  Marker  |Module id                          |Ac|Cm|Size |YY YY MM DD HH MM SS|Ln|St|Tr|  |Ix|Chksm|
  */
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_SET_TIME;
  SspmBuffer[18] = 0x0B;
  SSPMTime(SspmBuffer + 19);
  SspmBuffer[26] = 0x00;
  SspmBuffer[27] = 0x00;
  SspmBuffer[28] = 0x02;
  SspmBuffer[29] = 0x00;
  Sspm->command_sequence++;
  SspmBuffer[30] = Sspm->command_sequence;

  SSPMSend(33);
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
  SspmBuffer[16] = SSPM_FUNC_SET_RELAY;
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
  SspmBuffer[16] = SSPM_FUNC_GET_MODULE_STATE;
  SspmBuffer[18] = 0x01;
  SspmBuffer[19] = 0x0F;   // State of all four relays
  Sspm->command_sequence++;
  SspmBuffer[20] = Sspm->command_sequence;

  SSPMSend(23);
}

void SSPMSendGetOps(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  aa 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 04 00 00 08 c0 0a
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_OPS;
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendGetScheme(uint32_t module) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  AA 55 01 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 0b 00 00 09 14 c8
  Marker  |Module id                          |Ac|Cm|Size |Ix|Chksm|
  */
  SSPMInitSend();
  memcpy(SspmBuffer +3, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[16] = SSPM_FUNC_GET_SCHEME;
  Sspm->command_sequence++;
  SspmBuffer[19] = Sspm->command_sequence;

  SSPMSend(22);
}

void SSPMSendGetEnergy(uint32_t relay) {
  /*
  relay_num = 1..8
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 18 00 10 6b 7e 32 37 39 37 34 13 4b 35 36 37 01 01 00 3c 2a db d1
  */
  uint8_t module = relay >> 2;
  uint8_t channel = 1 << (relay & 0x03);  // Channel relays are bit masked
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_ENERGY;
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

void SSPMSendGetEnergyTotal(uint32_t relay) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 16 00 0d 6b 7e 32 37 39 37 34 13 4b 35 36 37 01 14 e6 93
  */
  uint8_t module = relay >> 2;
  uint8_t channel = relay & 0x03;  // Channel relays are NOT bit masked this time
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_ENERGY_TOTAL;
  SspmBuffer[18] = 0x0D;
  memcpy(SspmBuffer +19, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[31] = channel;
  Sspm->command_sequence++;
  SspmBuffer[32] = Sspm->command_sequence;

  SSPMSend(35);
}

void SSPMSendGetLog(uint32_t relay, uint32_t entries) {
  /*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
  AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 1a 00 10 6b 7e 32 37 39 37 34 13 4b 35 36 37 00 00 00 1d 09 8c cd
  */
  uint8_t module = relay >> 2;
  SSPMInitSend();
  SspmBuffer[16] = SSPM_FUNC_GET_LOG;
  SspmBuffer[18] = 0x10;
  memcpy(SspmBuffer +19, Sspm->module[module], SSPM_MODULE_NAME_SIZE);
  SspmBuffer[31] = 0;
  SspmBuffer[32] = 0;
  SspmBuffer[33] = 0;
  SspmBuffer[34] = entries;   // Number of logs
  Sspm->command_sequence++;
  SspmBuffer[35] = Sspm->command_sequence;

  SSPMSend(38);
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
          Sspm->init_step++;
        }
        break;
      case SSPM_FUNC_GET_OPS:
        /* 0x04 - Overload Protection
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
        AA 55 01 8b 34 32 37 39 37 34 13 4b 35 36 37 80 04 00 02 00 00 06 98 06
        Marker  |Module id                          |Ac|Cm|Size |  |Ch|Ra|Max P   |Min P   |Max U   |Min U   |Max I   |De|Ix|Chksm|
                                                                   |  |  |   4400W|    0.1W|    240V|    0.1V|     20A|  |
        AA 55 01 6B 7E 32 37 39 37 34 13 4B 35 36 37 80 04 00 35 00 07 00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00
                                                                       00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00
                                                                       00 11 30 00 00 00 0A 00 F0 00 00 00 0A 00 14 00 00 07 8A 86
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

          Sspm->init_step = SPM_DEVICES_FOUND;
//          Sspm->get_energy_relay = 1;
//          Sspm->allow_updates = 1;      // Enable requests from 100mSec loop
        }
        break;
      case SSPM_FUNC_SET_TIME:
        /* 0x0C
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 0c 00 01 00 04 3e 62
        */
        SSPMSendGetModuleState(Sspm->module_selected -1);
        break;
      case SSPM_FUNC_INIT_SCAN:
        /* 0x10
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
        AA 55 01 ff ff ff ff ff ff ff ff ff ff ff ff 80 10 00 01 00 02 e5 03
        */
        Sspm->module_max = 0;
        break;
      case SSPM_FUNC_UNITS:
        /* 0x15
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 15 00 04 00 01 00 00 01 81 b1
                                                                |?? ?? ?? ??|
        */
        SSPMSendInitScan();
        break;
      case SSPM_FUNC_GET_ENERGY_TOTAL:
        /* 0x16
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 16 01 7e 00 8b 34 32 37 39 37 34 13 4b 35 36 37
        03           <- L4
        07 e5 0b 0d  <- End date (Today) 2021 nov 13
        07 e5 05 11  <- Start date       2021 may 17
        00 05        <- 5kWh  (13/11 Today)
        00 00        <- 0     (12/11 Yesterday)
        00 04        <- 4kWh  (11/11 etc)
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        42 67 46
        */
        {
          uint32_t total_energy = 0;
          uint32_t entries = (Sspm->expected_bytes - 22) / 2;
          for (uint32_t i = 0; i < entries; i++) {
            total_energy += (SspmBuffer[41 + (i*2)] << 8) + SspmBuffer[42 + (i*2)];
          }
          uint32_t channel = SspmBuffer[32];
          for (uint32_t module = 0; module < Sspm->module_max; module++) {
            if ((SspmBuffer[20] == Sspm->module[module][0]) && (SspmBuffer[21] == Sspm->module[module][1])) {
              Sspm->total[module][channel] = total_energy;  // xkWh
              break;
            }
          }
          Sspm->allow_updates = 1;
        }
        break;
      case SSPM_FUNC_ENERGY_PERIOD:
        /* 0x1B
        Response after start energy period
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 0e [00] 8b 34 32 37 39 37 34 13 4b 35 36 37 [03] f7 b1 bc              L4
        Response after refresh or stop energy period
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 80 1b 00 11 [00] 8b 34 32 37 39 37 34 13 4b 35 36 37 [03] [00 00 00] f8 94 15   L4, kWh start period (0)
        */

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
                                                                                                    |Ch|Curre|Voltage |ActivePo|Reactive|Apparent|??|
        Values are XX XX    - number
                         XX - decimals



                                                                         Curr Voltag Active Reacti Appare
         0 1 2 3 4 5 6 7 8 910111213141516171819202122232425262728293031 3233 343536 373839 404142 434445 46 47 4849
        AA55010000000000000000000000000006001C8B343237393734134B35363708 000A 00E05B 001817 00013B 001825 4B BC 3DDA   0.100A 224.91V 24.23W  <-- 25W bulb
        AA55010000000000000000000000000006001C8B343237393734134B35363708 000A 00E115 00181A 00013D 001823 4B BE 6209
        AA55010000000000000000000000000006001C8B343237393734134B35363708 0044 00E260 009C1C 000000 009C1B 00 36 FD69   0.680A 226.96V 156.28W  <-- 150W bulb

        AA55010000000000000000000000000006001C8B343237393734134B35363708 0054 00E134 007525 00220A 00BD5D 20 34 55D6
        AA55010000000000000000000000000006001C8B343237393734134B35363708 0054 00E10A 007519 002126 00BD27 20 36 77EA
        AA55010000000000000000000000000006001C8B343237393734134B35363708 0053 00DE40 00731F 001604 00B952 4B 12 9255

        AA55010000000000000000000000000006001C8B343237393734134B35363708 075B 00D502 06940F 001863 069830 4B 1C E0DE

        AA55010000000000000000000000000006001c8b343237393734134b35363708 0044 00e025 009920 00010f 00993b 00 b3 07 a2  0.68A 223.25V 152.66W 0.54 Rea 152.5 Schijn


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
//        Sspm->module_max = 0;
        SSPMSendAck(command_sequence);
        break;
      case SSPM_FUNC_SCAN_RESULT:
        /* 0x13
         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 6b 7e 32 37 39 37 34 13 4b 35 36 37 04 00 00 00 82 01 00 00 14 00 00 0a 00 f0 00 00 00 0a 11 30 00 00 00 0a 02 8f cd
        AA 55 01 00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 24 8b 34 32 37 39 37 34 13 4b 35 36 37 04 00 00 00 82 01 00 00 14 00 00 0a 00 f0 00 00 00 0a 11 30 00 00 00 0a 02 a0 6f
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

void SSPMInit(void) {
  if (!ValidTemplate(PSTR("Sonoff SPM (POC1)"))) { return; }
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

  if (0 == Settings->flag2.voltage_resolution) {
    Settings->flag2.voltage_resolution = 1;   // SPM has only 2 decimals
    Settings->flag2.current_resolution = 2;   // SPM has only 2 decimals
    Settings->flag2.wattage_resolution = 2;   // SPM has only 2 decimals
    Settings->flag2.energy_resolution = 0;    // SPM has no decimals on total energy
  }

  Sspm->old_power = TasmotaGlobal.power;
  Sspm->init_step = SPM_WAIT;  // Start init sequence
}

void SSPMEvery100ms(void) {
  if (Sspm->no_send_key) { Sspm->no_send_key--; }

  if (Sspm->timeout) {
    Sspm->timeout--;
    if (!Sspm->timeout) {
      Sspm->allow_updates = 1;
    }
  }

  // Fix race condition if the ARM doesn't respond
  if ((Sspm->init_step > SPM_NONE) && (Sspm->init_step < SPM_SEND_FUNC_UNITS)) {
    Sspm->counter++;
    if (Sspm->counter > 20) {
      Sspm->init_step = SPM_NONE;
    }
  }
  switch (Sspm->init_step) {
    case SPM_NONE:
      return;
    case SPM_WAIT:
      Sspm->init_step++;
      break;
    case SPM_RESET:
      // Reset ARM
      digitalWrite(SSPM_GPIO_ARM_RESET, 0);
      delay(18);
      digitalWrite(SSPM_GPIO_ARM_RESET, 1);
      delay(18);
      Sspm->init_step++;
    case SPM_POLL:
      SSPMSendCmnd(SSPM_FUNC_FIND);
      break;
    case SPM_POLL_ACK:
      SSPMSendCmnd(SSPM_FUNC_FIND);
      break;
    case SPM_SEND_FUNC_UNITS:
      Sspm->init_step++;
      SSPMSendCmnd(SSPM_FUNC_UNITS);
      break;
    case SPM_DEVICES_FOUND:
      TasmotaGlobal.discovery_counter = 1;      // force TasDiscovery()
      Sspm->get_energy_relay = 1;
      Sspm->allow_updates = 1;      // Enable requests from 100mSec loop
      Sspm->init_step++;
      break;
    case SPM_GET_ENERGY_TOTALS:
      // Retrieve Energy total status from up to 128 relays
      if (Sspm->allow_updates && (Sspm->get_energy_relay > 0)) {
        SSPMSetLock(4);
        SSPMSendGetEnergyTotal(Sspm->get_energy_relay -1);
        Sspm->get_energy_relay++;
        if (Sspm->get_energy_relay > TasmotaGlobal.devices_present) {
          Sspm->get_energy_relay = 1;
          Sspm->init_step++;
        }
      }
      break;
    case SPM_ALLOW_LOOP:
      // Retrieve Energy status from up to 128 relays
      if (Sspm->allow_updates && (Sspm->get_energy_relay > 0)) {
        power_t powered_on = TasmotaGlobal.power >> (Sspm->get_energy_relay -1);
        if (0 == Sspm->loop_step) {
          // Get energy total only once in any 256 requests to safe comms
          if (powered_on &1) {
            SSPMSetLock(4);
            SSPMSendGetEnergyTotal(Sspm->get_energy_relay -1);
          }
          Sspm->get_energy_relay++;
          if (Sspm->get_energy_relay > TasmotaGlobal.devices_present) {
            Sspm->get_energy_relay = 1;
            Sspm->loop_step++;
          }
        } else {
          if (powered_on &1) {
            SSPMSetLock(4);
            SSPMSendGetEnergy(Sspm->get_energy_relay -1);
          } else {
            uint32_t relay_set = (Sspm->get_energy_relay -1) >> 2;
            uint32_t relay_num = (Sspm->get_energy_relay -1) &3;
            if (Sspm->voltage[relay_set][relay_num]) {
              Sspm->voltage[relay_set][relay_num] = 0;
              Sspm->current[relay_set][relay_num] = 0;
              Sspm->active_power[relay_set][relay_num] = 0;
              Sspm->apparent_power[relay_set][relay_num] = 0;
              Sspm->reactive_power[relay_set][relay_num] = 0;
              Sspm->power_factor[relay_set][relay_num] = 0;
            }
          }
          Sspm->loop_step++;     // Rolls over after 256 so allows for scanning at least all relays twice
          Sspm->get_energy_relay++;
          if ((Sspm->get_energy_relay > TasmotaGlobal.devices_present) || !Sspm->loop_step) {
            Sspm->get_energy_relay = 1;
          }
        }
      }
      break;
  }
}

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

bool SSPMButton(void) {
  bool result = false;
  uint32_t button = XdrvMailbox.payload;
  if ((PRESSED == button) && (NOT_PRESSED == Sspm->last_button)) {  // Button pressed
    SSPMSendInitScan();
    result = true;                 // Disable further button processing
  }
  Sspm->last_button = button;
  return result;
}

const char kSSPMEnergyPhases[] PROGMEM = "%*_f / %*_f / %*_f / %*_f|[%*_f,%*_f,%*_f,%*_f]";

char* SSPMEnergyFormat(char* result, float* input, uint32_t resolution, bool json) {
  char layout[32];
  GetTextIndexed(layout, sizeof(layout), json, kSSPMEnergyPhases);
  ext_snprintf_P(result, FLOATSZ * 4, layout, resolution, &input[0], resolution, &input[1], resolution, &input[2], resolution, &input[3]);
  return result;
}

const char HTTP_SSPMENERGY_SNS[] PROGMEM =
  "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
  "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

void SSPMEnergyShow(bool json) {
  if (!TasmotaGlobal.devices_present) { return; }  // Not ready yet

  if (json) {
    ResponseAppend_P(PSTR(",\"SPM\":{\"" D_JSON_TOTAL "\":["));
    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      ResponseAppend_P(PSTR("%s%*_f"), (i>0)?",":"", 0, &Sspm->total[i >>2][i &3]);
    }
    ResponseAppend_P(PSTR("],\"" D_JSON_POWERUSAGE "\":["));
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
    Sspm->rotate++;
    if ((Sspm->rotate >> 2) == Sspm->module_max) {
      Sspm->rotate = 0;
    }

    uint32_t module = Sspm->rotate >> 2;
    uint32_t relay_base = module * 4;
    WSContentSend_P(PSTR("{s}" D_SENSOR_RELAY "{m}L%02d / L%02d / L%02d / L%02d{e}"), relay_base +1, relay_base +2, relay_base +3, relay_base +4);
    char value_chr[FLOATSZ * 4];
    WSContentSend_PD(HTTP_SNS_VOLTAGE,    SSPMEnergyFormat(value_chr, Sspm->voltage[module], Settings->flag2.voltage_resolution, json));
    WSContentSend_PD(HTTP_SNS_CURRENT,    SSPMEnergyFormat(value_chr, Sspm->current[module], Settings->flag2.current_resolution, json));
    WSContentSend_PD(HTTP_SNS_POWER,      SSPMEnergyFormat(value_chr, Sspm->active_power[module], Settings->flag2.wattage_resolution, json));
    char valu2_chr[FLOATSZ * 4];
    char valu3_chr[FLOATSZ * 4];
    WSContentSend_PD(HTTP_SSPMENERGY_SNS, SSPMEnergyFormat(value_chr, Sspm->apparent_power[module], Settings->flag2.wattage_resolution, json),
                                          SSPMEnergyFormat(valu2_chr, Sspm->reactive_power[module], Settings->flag2.wattage_resolution, json),
                                          SSPMEnergyFormat(valu3_chr, Sspm->total[module], Settings->flag2.energy_resolution, json));
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kSSPMCommands[] PROGMEM = "SSPM|"  // Prefix
  "Log|Energy|History|Scan" ;

void (* const SSPMCommand[])(void) PROGMEM = {
  &CmndSSPMLog, &CmndSSPMEnergy, &CmndSSPMEnergyHistory, &CmndSSPMScan };

void CmndSSPMLog(void) {
  if ((XdrvMailbox.index < 1) || (XdrvMailbox.index > TasmotaGlobal.devices_present)) { XdrvMailbox.index = 1; }
  XdrvMailbox.payload &= 0x1F;  // Max 32 entries
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
  SSPMSendInitScan();

  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv86(uint8_t function)
{
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
      case FUNC_EVERY_SECOND:
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
