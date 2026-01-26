/*
  xsns_102_ld2410.ino - HLK-LD2410 24GHz smart wave motion sensor support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends, 2024 md5sum-as (https://github.com/md5sum-as)

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_LD2410
/*********************************************************************************************\
 * HLK-LD2410 24GHz smart wave motion sensor
 * 
 * Attention!
 * This module works with HLK-LD2410, HLK-LD2410B (md5sum-as tested), HLK-LD2410C (md5sum-as tested) devices. 
 * The module does not support HLK-LD2410S (md5sum-as tested) and is not guaranteed to work with other devices.
 * 
 * 
 * LD2410Duration 0                            - Set factory default settings
 * LD2410Duration 1..65535                     - Set no-one duration in seconds (default 5)
 * LD2410MovingSens 50,50,40,30,20,15,15,15,15 - Set moving distance sensitivity for up to 9 gates (at 0.75 meter interval)
 * LD2410StaticSens 0,0,40,40,30,30,20,20,20   - Set static distance sensitivity for up to 9 gates (at 0.75 meter interval)
 *
 * LD2410Get                                   - Read last sensors
 * LD2410EngineeringStart                      - Start engineering mode
 * LD2410EngineeringEnd                        - End engineering mode
 *
 * Inspiration:
 * https://community.home-assistant.io/t/mmwave-wars-one-sensor-module-to-rule-them-all/453260/2
 * Resources:
 * https://drive.google.com/drive/folders/1p4dhbEJA3YubyIjIIC7wwVsSo8x29Fq-?spm=a2g0o.detail.1000023.17.93465697yFwVxH
 *
 * Internal info:
 * - After a LD2410 serial command a response takes about 10mS
 * - After a LD2410 restart it takes at least 1000mS before commands are allowed
\*********************************************************************************************/

#define XSNS_102                         102

#define LD2410_BUFFER_SIZE               TM_SERIAL_BUFFER_SIZE  // 64
#define LD2410_MAX_GATES                 8       // 0 to 8 (= 9) - DO NOT CHANGE

#define LD2410_CMND_START_CONFIGURATION  0xFF
#define LD2410_CMND_END_CONFIGURATION    0xFE
#define LD2410_CMND_SET_DISTANCE         0x60
#define LD2410_CMND_READ_PARAMETERS      0x61
#define LD2410_CMND_START_ENGINEERING    0x62
#define LD2410_CMND_END_ENGINEERING      0x63
#define LD2410_CMND_SET_SENSITIVITY      0x64
#define LD2410_CMND_GET_FIRMWARE         0xA0
#define LD2410_CMND_SET_BAUDRATE         0xA1
#define LD2410_CMND_FACTORY_RESET        0xA2
#define LD2410_CMND_REBOOT               0xA3
#define LD2410_CMND_SET_BLUETOOTH        0xA4
#define LD2410_CMND_GET_BLUETOOTH_MAC    0xA5

const uint8_t LD2410_config_header[4] = {0xFD, 0xFC, 0xFB, 0xFA};
const uint8_t LD2410_config_footer[4] = {0x04, 0x03, 0x02, 0x01};
const uint8_t LD2410_target_header[4] = {0xF4, 0xF3, 0xF2, 0xF1};
const uint8_t LD2410_target_footer[4] = {0xF8, 0xF7, 0xF6, 0xF5};

#include <TasmotaSerial.h>
TasmotaSerial *LD2410Serial = nullptr;

struct {
  uint8_t *buffer;
  uint16_t moving_distance;
  uint16_t static_distance;
  uint16_t detect_distance;
  uint16_t no_one_duration;
  uint8_t moving_sensitivity[LD2410_MAX_GATES +1];
  uint8_t static_sensitivity[LD2410_MAX_GATES +1];
  uint8_t max_moving_distance_gate;
  uint8_t max_static_distance_gate;
  uint8_t moving_energy;
  uint8_t static_energy;
  uint8_t step;
  uint8_t retry;
  uint8_t settings;
  uint8_t byte_counter;
  bool valid_response;
  uint8_t set_engin_mode;
  uint8_t web_engin_mode;
  struct {
    uint8_t moving_gate_energy[LD2410_MAX_GATES +1];
    uint8_t static_gate_energy[LD2410_MAX_GATES +1];
    uint8_t light;
    uint8_t out_pin;
  } engineering;
} LD2410;

/********************************************************************************************/

uint32_t ToBcd(uint32_t value) {
  return ((value >> 4) * 10) + (value & 0xF);
}

/********************************************************************************************/

void Ld1410HandleTargetData(void) {
  uint8_t i;

  if (((0x0D == LD2410.buffer[4]) && (0x55 == LD2410.buffer[17]) && (0x02 == LD2410.buffer[6]))
      or ((0x23 == LD2410.buffer[4]) && (0x55 == LD2410.buffer[39]) && (0x01 == LD2410.buffer[6]))) {  // Add bad reception detection
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
    // F4 F3 F2 F1 0D 00 02 AA 00 00 00 00 00 00 37 00 00 55 00 F8 F7 F6 F5 - No target
    // F4 F3 F2 F1 0D 00 02 AA 00 45 00 3E 00 00 3A 00 00 55 00 F8 F7 F6 F5 - No target
    // F4 F3 F2 F1 0D 00 02 AA 03 46 00 34 00 00 3C 00 00 55 00 F8 F7 F6 F5 - Movement and Stationary target
    // F4 F3 F2 F1 0D 00 02 AA 02 54 00 00 00 00 64 00 00 55 00 F8 F7 F6 F5 - Stationary target
    // F4 F3 F2 F1 0D 00 02 AA 02 96 00 00 00 00 36 00 00 55 00 F8 F7 F6 F5 - Stationary target
    // F4 F3 F2 F1 0D 00 02 AA 03 2A 00 64 00 00 64 00 00 55 00 F8 F7 F6 F5 - Movement and Stationary target
    //
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44
    // F4 F3 F2 F1 23 00 01 AA 00 1E 00 00 1E 00 0D 00 00 08 08 13 0E 07 02 05 07 03 04 05 00 00 0D 06 05 05 05 05 05 62 00 55 00 F8 F7 F6 F5
    //
    // F4 F3 F2 F1 23 00 01 AA 02 20 01 00 37 01 64 26 01                     
    //  08 08 - max moving and static dist (17,18)
    //  12 05 04 09 0C 0D 0F 04 01 - Movement energy (19-27)
    //  00 00 1F 64 64 64 64 31 1A - Static energy (28-36)
    //  8C - Photo sens (37)
    //  01 - Out pin (38)
    //                                                    55 00 F8 F7 F6 F5
    // header     |len  |dt|hd|st|movin|me|stati|se|detec|tr|ck|trailer

    LD2410.moving_distance = 0;
    LD2410.moving_energy = 0;
    LD2410.static_distance = 0;
    LD2410.static_energy = 0;
    LD2410.detect_distance = 0;

    if (LD2410.buffer[8] != 0x00) {                               // Movement and/or Stationary target
      LD2410.moving_distance = LD2410.buffer[10] << 8 | LD2410.buffer[9];
      LD2410.moving_energy = LD2410.buffer[11];
      LD2410.static_distance = LD2410.buffer[13] << 8 | LD2410.buffer[12];
      LD2410.static_energy = LD2410.buffer[14];
      LD2410.detect_distance = LD2410.buffer[16] << 8 | LD2410.buffer[15];

    }
    LD2410.web_engin_mode = LD2410.buffer[6]==1?1:0;
    if (0x01 == LD2410.buffer[6]) { /* Engineering mode*/
      if (LD2410.buffer[17] < 9) {
        for (i=0; i<= LD2410.buffer[17]; i++) {
          LD2410.engineering.moving_gate_energy[i] = LD2410.buffer[i+19];
        }
      }
      if (LD2410.buffer[18] < 9) {
        for (i=0; i<= LD2410.buffer[18]; i++) {
          LD2410.engineering.static_gate_energy[i] = LD2410.buffer[i+28];
        }
      }
      LD2410.engineering.light=LD2410.buffer[37];
      LD2410.engineering.out_pin=LD2410.buffer[38];
      // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2 Eng: mov: %d %d %d %d %d %d %d %d %d, st: %d %d %d %d %d %d %d %d %d, light: %d, out: %d"), 
      //     LD2410.engineering.moving_gate_energy[0],LD2410.engineering.moving_gate_energy[1],LD2410.engineering.moving_gate_energy[2],
      //     LD2410.engineering.moving_gate_energy[3],LD2410.engineering.moving_gate_energy[4],LD2410.engineering.moving_gate_energy[5],
      //     LD2410.engineering.moving_gate_energy[6],LD2410.engineering.moving_gate_energy[7],LD2410.engineering.moving_gate_energy[8],
      //     LD2410.engineering.static_gate_energy[0],LD2410.engineering.static_gate_energy[1],LD2410.engineering.static_gate_energy[2],
      //     LD2410.engineering.static_gate_energy[3],LD2410.engineering.static_gate_energy[4],LD2410.engineering.static_gate_energy[5],
      //     LD2410.engineering.static_gate_energy[6],LD2410.engineering.static_gate_energy[7],LD2410.engineering.static_gate_energy[8],
      //     LD2410.engineering.light,LD2410.engineering.out_pin);
    }
  }
}

void Ld1410HandleConfigData(void) {
  if (LD2410_CMND_READ_PARAMETERS == LD2410.buffer[6]) {           // 0x61
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
    // FD FC FB FA 1C 00 61 01 00 00 AA 08 08 08 32 32 28 1E 14 0F 0F 0F 0F 00 00 28 28 1E 1E 14 14 14 05 00 04 03 02 01 - Default
    // header     |len  |cw cv|ack  |hd|dd|md|sd|moving sensitivity 0..8   |static sensitivity 0..8   |timed|trailer
    //            |   28|     |    0|  | 8| 8| 8|50 50 40 30 20 15 15 15 15| 0  0 40 40 30 30 20 20 20|    5|
    LD2410.max_moving_distance_gate = LD2410.buffer[12];
    LD2410.max_static_distance_gate = LD2410.buffer[13];
    for (uint32_t i = 0; i <= LD2410_MAX_GATES; i++) {
      LD2410.moving_sensitivity[i] = LD2410.buffer[14 +i];
      LD2410.static_sensitivity[i] = LD2410.buffer[23 +i];
    }
    LD2410.no_one_duration = LD2410.buffer[33] << 8 | LD2410.buffer[32];
  }
  else if (LD2410_CMND_START_CONFIGURATION == LD2410.buffer[6]) {  // 0xFF
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
    // FD FC FB FA 08 00 FF 01 00 00 01 00 40 00 04 03 02 01
    // header     |len  |ty   |ack  |protv|bsize|trailer
    //            |    8|     |    0|    1|   64|
    LD2410.valid_response = true;
  }
  else if (LD2410_CMND_GET_FIRMWARE == LD2410.buffer[6]) {         // 0xA0
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
    // FD FC FB FA 0C 00 A0 01 00 00 00 01 07 01 16 15 09 22 04 03 02 01
    // header     |len  |ty|hd|ack  |ftype|major|minor      |trailer
    //            |   12|  | 1|    0|  256|  1.7|   22091516|
    AddLog(LOG_LEVEL_INFO, PSTR("LD2: Firmware version V%d.%02d.%02d%02d%02d%02d"),  // Firmware version V1.07.22091516
      ToBcd(LD2410.buffer[13]), ToBcd(LD2410.buffer[12]),
      ToBcd(LD2410.buffer[17]), ToBcd(LD2410.buffer[16]), ToBcd(LD2410.buffer[15]), ToBcd(LD2410.buffer[14]));
  }
}

bool Ld2410Match(const uint8_t *header, uint32_t offset) {
  for (uint32_t i = 0; i < 4; i++) {
    if (LD2410.buffer[offset +i] != header[i]) { return false; }
  }
  return true;
}

void Ld2410Input(void) {
/*
  // Works with TasmotaSerial as SoftwareSerial but fails with HardwareSerial
  uint32_t size = LD2410Serial->read(LD2410.buffer, LD2410_BUFFER_SIZE);
  if (size) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), size, LD2410.buffer);

    bool target_header = (Ld2410Match(LD2410_target_header, 0));  // F4F3F2F1
    bool config_header = (Ld2410Match(LD2410_config_header, 0));  // FDFCFBFA
    if (target_header || config_header) {
      uint32_t len = LD2410.buffer[4] +10;                        // Total packet size
      if (size >= len) {                                          // Handle only the first entry (if there are more)
        if (target_header) {                                      // F4F3F2F1
          if (Ld2410Match(LD2410_target_footer, len -4)) {        // F8F7F6F5
            Ld1410HandleTargetData();
          }
        }
        else if (config_header) {                                 // FDFCFBFA
          if (Ld2410Match(LD2410_config_footer, len -4)) {        // 04030201
            Ld1410HandleConfigData();
          }
        }
      }
    }
  }
*/
  // Works with TasmotaSerial and HardwareSerial
  while (LD2410Serial->available()) {
    yield();                                                    // Fix watchdogs

    LD2410.buffer[LD2410.byte_counter++] = LD2410Serial->read();
    if (LD2410.byte_counter < 4) { continue; }                  // Need first four header bytes

    uint32_t header_start = LD2410.byte_counter -4;             // Fix interrupted header transmits
    bool target_header = (Ld2410Match(LD2410_target_header, header_start));  // F4F3F2F1
    bool config_header = (Ld2410Match(LD2410_config_header, header_start));  // FDFCFBFA
    if ((target_header || config_header) && (header_start != 0)) {
      memmove(LD2410.buffer, LD2410.buffer + header_start, 4);  // Sync buffer with header
      LD2410.byte_counter = 4;
    }
    if (LD2410.byte_counter < 6) { continue; }                  // Need packet size bytes

    target_header = (Ld2410Match(LD2410_target_header, 0));     // F4F3F2F1
    config_header = (Ld2410Match(LD2410_config_header, 0));     // FDFCFBFA
    if (target_header || config_header) {
      uint32_t len = LD2410.buffer[4] +10;                      // Total packet size
      if (len > LD2410_BUFFER_SIZE) {
        LD2410.byte_counter = 0;                                // Invalid data
        break;                                                  // Exit loop to satisfy yields
      }
      if (LD2410.byte_counter < len) { continue; }              // Need complete packet

      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), len, LD2410.buffer);

      if (target_header) {                                      // F4F3F2F1

//        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), len, LD2410.buffer);

        if (Ld2410Match(LD2410_target_footer, len -4)) {        // F8F7F6F5
          Ld1410HandleTargetData();
        }
      }
      else if (config_header) {                                 // FDFCFBFA

//        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), len, LD2410.buffer);

        if (Ld2410Match(LD2410_config_footer, len -4)) {        // 04030201
          Ld1410HandleConfigData();
          LD2410Serial->setReadChunkMode(0);                    // Disable chunk mode fixing Hardware Watchdogs
        }
      }
    }
    LD2410.byte_counter = 0;                                    // Finished or bad received footer
    break;                                                      // Exit loop to satisfy yields
  }
  // If here then LD2410.byte_counter could still be partial correct for next loop
}

void Ld2410SendCommand(uint32_t command, uint8_t *val = nullptr, uint32_t val_len = 0);
void Ld2410SendCommand(uint32_t command, uint8_t *val, uint32_t val_len) {
  uint32_t len = val_len +12;
  uint8_t buffer[len];
  buffer[0] = 0xFD;
  buffer[1] = 0xFC;
  buffer[2] = 0xFB;
  buffer[3] = 0xFA;
  buffer[4] = val_len +2;
  buffer[5] = 0x00;
  buffer[6] = command;
  buffer[7] = 0x00;
  if (val) {
    for (uint32_t i = 0; i < val_len; i++) {
      buffer[8 +i] = val[i];
    }
  }
  buffer[8 +val_len] = 0x04;
  buffer[9 +val_len] = 0x03;
  buffer[10 +val_len] = 0x02;
  buffer[11 +val_len] = 0x01;

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Send %*_H"), len, buffer);

  LD2410Serial->setReadChunkMode(1);                            // Enable chunk mode introducing possible Hardware Watchdogs
  LD2410Serial->flush();
  LD2410Serial->write(buffer, len);
}

void Ld2410SetConfigMode(void) {                                // 0xFF
  uint8_t value[2] = { 0x01, 0x00 };
  Ld2410SendCommand(LD2410_CMND_START_CONFIGURATION, value, sizeof(value));
}

void Ld2410SetMaxDistancesAndNoneDuration(uint32_t max_moving_distance_range, uint32_t max_static_distance_range, uint32_t no_one_duration) {  // 0x60
  // Distance range value can be set from 1 to 8 (distance gates of 0.75 meter)
  // No-one duration value can be set from 1 to 65535 (seconds)
  // 00 00 08 00 00 00 01 00 08 00 00 00 02 00 05 00 00 00
  // motio|          8|stati|          8|durat|seconds
  uint8_t lsb_nd = no_one_duration & 0xFF;
  uint8_t msb_nd = (no_one_duration >> 8) & 0xFF;
  uint8_t value[18] = { 0x00, 0x00, (uint8_t)max_moving_distance_range, 0x00, 0x00, 0x00, 0x01, 0x00, (uint8_t)max_static_distance_range, 0x00, 0x00, 0x00, 0x02, 0x00, lsb_nd, msb_nd, 0x00, 0x00 };
  Ld2410SendCommand(LD2410_CMND_SET_DISTANCE, value, sizeof(value));
}

void Ld2410SetGateSensitivity(uint32_t gate, uint32_t moving_sensitivity, uint32_t static_sensitivity) {  // 0x64
  // Sensitivity value can be set from 0 to 100 (%) for gates 0 to 8
  // 00 00 03 00 00 00 01 00 28 00 00 00 02 00 28 00 00 00
  // gate |          3|motio|         40|stati|         40
  uint8_t value[18] = { 0x00, 0x00, (uint8_t)gate, 0x00, 0x00, 0x00, 0x01, 0x00, (uint8_t)moving_sensitivity, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)static_sensitivity, 0x00, 0x00, 0x00 };
  Ld2410SendCommand(LD2410_CMND_SET_SENSITIVITY, value, sizeof(value));
}

void Ld2410SetAllSensitivity(uint32_t sensitivity) {            // 0x64
  // Sensitivity value can be set from 0 to 100
  // 00 00 FF FF 00 00 01 00 28 00 00 00 02 00 28 00 00 00
  // gate |all gates  |motio|         40|stati|         40
  uint8_t value[18] = { 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, (uint8_t)sensitivity, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)sensitivity, 0x00, 0x00, 0x00 };
  Ld2410SendCommand(LD2410_CMND_SET_SENSITIVITY, value, sizeof(value));
}

void Ld2410SetBaudrate(uint32_t index) {                        // 0xA1
  uint8_t value[2] = { (uint8_t)index, 0x00 };
  Ld2410SendCommand(LD2410_CMND_SET_BAUDRATE, value, sizeof(value));
}

/********************************************************************************************/

void Ld2410Every100MSecond(void) {
  if (LD2410.step) {
    LD2410.step--;
    switch (LD2410.step) {
      // case 60: Set default settings
      case 59:
        Ld2410SetConfigMode();                                  // Stop running mode
        break;
      case 57:
        Ld2410SendCommand(LD2410_CMND_FACTORY_RESET);
        break;
      case 56:
        Ld2410SendCommand(LD2410_CMND_REBOOT);                  // Wait at least 1 second
        break;
      case 51:
        LD2410.step = 12;
        AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Settings factory reset"));
        break;

      // case 40: Save settings
      case 39:
        Ld2410SetConfigMode();                                  // Stop running mode
        break;
      case 37:
        Ld2410SetMaxDistancesAndNoneDuration(8, 8, LD2410.no_one_duration);
        break;
      case 28 ... 36: {
          uint32_t index = LD2410.step -28;
          Ld2410SetGateSensitivity(index, LD2410.moving_sensitivity[index], LD2410.static_sensitivity[index]);
        }
        break;
      case 27:
        LD2410.step = 3;
        AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Settings saved"));
        break;
/*
      // case 24: pre-POC using 57600 bps instead of default 256000 bps
      case 23:
        AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Switch to 57600 bps"));
        LD2410Serial->flush();
        LD2410Serial->begin(256000);
        break;
      case 21:
        Ld2410SetConfigMode();                                  // Stop running mode
        break;
      case 19:
        Ld2410SetBaudrate(4);                                   // 57600 bps
        break;
      case 18:
        Ld2410SendCommand(LD2410_CMND_REBOOT);                  // Wait at least 1 second
        LD2410Serial->flush();
        LD2410Serial->begin(57600);
      break;
*/
      case 17:
        Ld2410SetConfigMode();                                  // Stop running mode
        break;
      case 14:
        if (0 == LD2410.set_engin_mode) {
          Ld2410SendCommand(LD2410_CMND_END_ENGINEERING);
        } else {
          Ld2410SendCommand(LD2410_CMND_START_ENGINEERING);
        }
        LD2410.step = 2;
        break;

      // case 12: Init
      case 5:
        Ld2410SetConfigMode();                                  // Stop running mode
        break;
      case 3:
        if (!LD2410.valid_response && LD2410.retry) {
          LD2410.retry--;
          if (LD2410.retry) {
//            LD2410.step = 24;                                   // Change baudrate
            LD2410.step = 7;                                    // Retry
          } else {
            LD2410.step = 0;
            AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Not detected"));
          }
        } else {
          Ld2410SendCommand(LD2410_CMND_GET_FIRMWARE);
        }
        break;
      case 2:
        Ld2410SendCommand(LD2410_CMND_READ_PARAMETERS);
        break;
      case 1:
        Ld2410SendCommand(LD2410_CMND_END_CONFIGURATION);
        break;
    }
  } else {
    if (1 == LD2410.settings) {
      LD2410.settings = 0;
      LD2410.step = 40;
    }
    else if (2 == LD2410.settings) {
      LD2410.settings = 0;
      LD2410.step = 60;
    }
  }
}

void Ld2410EverySecond(void) {
  if (LD2410.moving_energy and (!Settings->flag6.ld2410_use_pin)) {

    // Send state change to be captured by rules
    // {"Time":"2022-11-26T10:48:16","Switch1":"ON","LD2410":{"Distance":[125.0,0.0,0.0],"Energy":[0,100]}}
    MqttPublishSensor();
  }
}

void Ld2410Detect(void) {
  if (PinUsed(GPIO_LD2410_RX) && PinUsed(GPIO_LD2410_TX)) {
    LD2410.buffer = (uint8_t*)malloc(LD2410_BUFFER_SIZE);    // Default 64
    if (!LD2410.buffer) { return; }
    LD2410Serial = new TasmotaSerial(Pin(GPIO_LD2410_RX), Pin(GPIO_LD2410_TX), 2);
    if (LD2410Serial->begin(256000)) {
      if (LD2410Serial->hardwareSerial()) { ClaimSerial(); }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Serial UART%d"), LD2410Serial->getUart());
#endif

      LD2410.retry = 4;
      LD2410.step = 12;
    }
    LD2410.set_engin_mode = 0;
    memset(&LD2410.engineering,0,sizeof(LD2410.engineering));
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kLd2410Commands[] PROGMEM = "LD2410|"  // Prefix
  "Duration|MovingSens|StaticSens|Get|EngineeringEnd|EngineeringStart";

void (* const Ld2410Command[])(void) PROGMEM = {
  &CmndLd2410Duration, &CmndLd2410MovingSensitivity, &CmndLd2410StaticSensitivity, &CmndLd2410last, &CmndLd2410EngineeringEnd, &CmndLd2410EngineeringStart };

void Ld2410Response(void) {
  Response_P(PSTR("{\"LD2410\":{\"Duration\":%d,\"Moving\":{\"Gates\":%d,\"Sensitivity\":["),
    LD2410.no_one_duration, LD2410.max_moving_distance_gate);
  for (uint32_t i = 0; i <= LD2410_MAX_GATES; i++) {
    ResponseAppend_P(PSTR("%s%d"), (i==0)?"":",", LD2410.moving_sensitivity[i]);
  }
  ResponseAppend_P(PSTR("]},\"Static\":{\"Gates\":%d,\"Sensitivity\":["), LD2410.max_static_distance_gate);
  for (uint32_t i = 0; i <= LD2410_MAX_GATES; i++) {
    ResponseAppend_P(PSTR("%s%d"), (i==0)?"":",", LD2410.static_sensitivity[i]);
  }
  ResponseAppend_P(PSTR("]}}}"));
}

void CmndLd2410Duration(void) {
  // LD2410Duration 0  - Set default settings
  if (0 == XdrvMailbox.payload) {
    LD2410.settings = 2;
  }
  // LD2410Duration 5
  else if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 65535)) {
    LD2410.no_one_duration = XdrvMailbox.payload;
    LD2410.settings = 1;
  }
  Ld2410Response();
}

void CmndLd2410MovingSensitivity(void) {
  // LD2410MovingSens 50,50,40,30,20,15,15,15,15
  uint32_t parm[LD2410_MAX_GATES +1] = { 0 };
  uint32_t count = ParseParameters(LD2410_MAX_GATES +1, parm);
  if (count) {
    for (uint32_t i = 0; i < count; i++) {
      if ((parm[i] >= 0) && (parm[i] <= 100)) {
        LD2410.moving_sensitivity[i] = parm[i];
      }
    }
    LD2410.settings = 1;
  }
  Ld2410Response();
}

void CmndLd2410StaticSensitivity(void) {
  // LD2410StaticSens 0,0,40,40,30,30,20,20,20
  uint32_t parm[LD2410_MAX_GATES +1] = { 0 };
  uint32_t count = ParseParameters(LD2410_MAX_GATES +1, parm);
  if (count) {
    for (uint32_t i = 0; i < count; i++) {
      if ((parm[i] >= 0) && (parm[i] <= 100)) {
        LD2410.static_sensitivity[i] = parm[i];
      }
    }
    LD2410.settings = 1;
  }
  Ld2410Response();
}

void CmndLd2410last(void) {
  Response_P(PSTR("{\"LD2410\":{\"Moving energy\":[%d,%d,%d,%d,%d,%d,%d,%d,%d],\"Static energy\":[%d,%d,%d,%d,%d,%d,%d,%d,%d],\"Light\":%d,\"Out_pin\":%d}}"),
          LD2410.engineering.moving_gate_energy[0],LD2410.engineering.moving_gate_energy[1],LD2410.engineering.moving_gate_energy[2],
          LD2410.engineering.moving_gate_energy[3],LD2410.engineering.moving_gate_energy[4],LD2410.engineering.moving_gate_energy[5],
          LD2410.engineering.moving_gate_energy[6],LD2410.engineering.moving_gate_energy[7],LD2410.engineering.moving_gate_energy[8],
          LD2410.engineering.static_gate_energy[0],LD2410.engineering.static_gate_energy[1],LD2410.engineering.static_gate_energy[2],
          LD2410.engineering.static_gate_energy[3],LD2410.engineering.static_gate_energy[4],LD2410.engineering.static_gate_energy[5],
          LD2410.engineering.static_gate_energy[6],LD2410.engineering.static_gate_energy[7],LD2410.engineering.static_gate_energy[8],
          LD2410.engineering.light,LD2410.engineering.out_pin);
}

void CmndLd2410EngineeringEnd(void) {
    LD2410.set_engin_mode = 0;
    LD2410.step = 18;
    Response_P(PSTR("LD2410: End engineering mode"));
}

void CmndLd2410EngineeringStart(void) {
    LD2410.set_engin_mode= 1;
    LD2410.step = 18;
    Response_P(PSTR("LD2410: Start engineering mode"));
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_LD2410_CM[] PROGMEM =
  "{s}LD2410 " D_MOVING_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}"
  "{s}LD2410 " D_STATIC_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}"
  "{s}LD2410 " D_DETECT_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}";
const char HTTP_SNS_LD2410_ENG[] PROGMEM =
  "{s}LD2410 " D_MOVING_ENERGY_T "{m}%d %d %d %d %d %d %d %d %d{e}"
  "{s}LD2410 " D_STATIC_ENERGY_T "{m}%d %d %d %d %d %d %d %d %d{e}"
  "{s}LD2410 " D_LD2410_LIGHT "{m}%d{e}"
  "{s}LD2410 " D_LD2410_PIN_STATE "{m}%d{e}";
#endif

void Ld2410Show(bool json) {
  float moving_distance = LD2410.moving_distance;
  float static_distance = LD2410.static_distance;
  float detect_distance = LD2410.detect_distance;
  if (json) {
    //                                                             cm   cm   cm                          %  %
    ResponseAppend_P(PSTR(",\"LD2410\":{\"" D_JSON_DISTANCE "\":[%1_f,%1_f,%1_f],\"" D_JSON_ENERGY "\":[%d,%d]}"),
      &moving_distance, &static_distance, &detect_distance, LD2410.moving_energy, LD2410.static_energy);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_LD2410_CM, &moving_distance, &static_distance, &detect_distance);
    if (LD2410.web_engin_mode == 1) {
      WSContentSend_PD(HTTP_SNS_LD2410_ENG, 
          LD2410.engineering.moving_gate_energy[0],LD2410.engineering.moving_gate_energy[1],LD2410.engineering.moving_gate_energy[2],
          LD2410.engineering.moving_gate_energy[3],LD2410.engineering.moving_gate_energy[4],LD2410.engineering.moving_gate_energy[5],
          LD2410.engineering.moving_gate_energy[6],LD2410.engineering.moving_gate_energy[7],LD2410.engineering.moving_gate_energy[8],
          LD2410.engineering.static_gate_energy[0],LD2410.engineering.static_gate_energy[1],LD2410.engineering.static_gate_energy[2],
          LD2410.engineering.static_gate_energy[3],LD2410.engineering.static_gate_energy[4],LD2410.engineering.static_gate_energy[5],
          LD2410.engineering.static_gate_energy[6],LD2410.engineering.static_gate_energy[7],LD2410.engineering.static_gate_energy[8],
          LD2410.engineering.light,LD2410.engineering.out_pin);
    }
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns102(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    Ld2410Detect();
  }
  else if (LD2410Serial) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        Ld2410Input();
        break;
      case FUNC_EVERY_100_MSECOND:
        Ld2410Every100MSecond();
        break;
      case FUNC_EVERY_SECOND:
        Ld2410EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ld2410Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ld2410Show(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kLd2410Commands, Ld2410Command);
        break;
    }
  }
  return result;
}

#endif  // USE_LD2410
