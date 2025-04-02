/*
  xsns_102_ld2402.ino - HLK-LD2402 24GHz smart wave motion sensor support for Tasmota

  Copyright (C) 2025  Anthony Sepa

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

  Based in part on xsns_102_ld2410s.ino by:
  SPDX-FileCopyrightText: 2022 Theo Arends, 2024 md5sum-as (https://github.com/md5sum-as)

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_LD2402
/*********************************************************************************************\
 * HLK-LD2402 24GHz smart wave motion sensor
 * 
 * Attention!
 * This module works with HLK-LD2402 devices. 
 * 
 * Available commands:
 * LD2402_Status 0|1|2 - showing parameters|sensor readings|firmware info
 * LD2402_ReRead - reread common, motion and micro threshold parameters from device
 * LD2402_SetCommon 7-100,0-65535 - set common: maximum distance, disappearance delay
 * LD2402_SetMotion n,n1..n16 - set motion threshold values (16)
 * LD2402_SetMicro n,n1..n16 - set micro threshold values (16)
 * LD2402_Mode 0/1 - set device output mode 0-normal, 1-engineering
 * LD2402_Save - save the internal device motion/micro thresholds (fw 3.3.2+)
 * LD2402_AutoUpdate 3.0,2.0,3.0 - start autoupdate trigger,keep,micro magnification factor
 * LD2402_AutoGain - start autogain process (fw 3.3.5+)
 * LD2402_Follow 0/n - reports every n seconds
 *
\*********************************************************************************************/

#define XSNS_102                        102

// Max line length is in engineering mode at 141 characters
#define LD2402_BUFFER_SIZE              141

#define LD2402_NUM_GATES                0x10

// control commands
#define LD2402_CMND_NOCMND              0x02

// use command values << 4 (to allow delay space) as steps
#define LD2402_CMND_READ_VERSION        0x00F
#define LD2402_CMND_READ_SERIAL         0x11F
#define LD2402_CMND_WRITE_PARAM         0x07F
#define LD2402_CMND_READ_PARAM          0x08F
#define LD2402_CMND_AUTO_THRESHOLD      0x09F
#define LD2402_CMND_AUTO_PROGRESS       0x0AF
#define LD2402_CMND_MODE                0x12F
#define LD2402_CMND_AUTO_INTERFERENCE   0x14F
#define LD2402_CMND_AUTO_GAIN           0xEEF
#define LD2402_CMND_AUTO_GAIN_COMPLETE  0xF0F
#define LD2402_CMND_SAVE_PARAM          0xFDF
#define LD2402_CMND_END_CONFIGURATION   0xFEF
#define LD2402_CMND_START_CONFIGURATION 0xFFF

#define CMD_LD2402_TIMEOUT_DELAY        20  // 20 x 250ms ticks or 5 seconds
#define CMD_LD2402_Wait_Timeout         (0xFFF+CMD_LD2402_TIMEOUT_DELAY+2)

#define CMD_LD2402_END_DELAY            2
#define CMD_LD2402_PROGRESS_DELAY       7

#define LD2402_CMND_PARAM_MAX_DISTANCE  0x01
#define LD2402_CMND_PARAM_DISP_DELAY    0x04
#define LD2402_CMND_PARAM_PWR_INTERFER  0x05
#define LD2402_CMND_PARAM_MOTION_START  0x10
#define LD2402_CMND_PARAM_MOTION_END    LD2402_CMND_PARAM_MOTION_START + LD2402_NUM_GATES - 1
#define LD2402_CMND_PARAM_MICRO_START   0x30
#define LD2402_CMND_PARAM_MICRO_END     LD2402_CMND_PARAM_MICRO_START + LD2402_NUM_GATES - 1

#define LD2402_target_Error             0x4572726f
#define LD2402_target_OFF               0x4F46460D
#define LD2402_target_distance          0x64697374
#define LD2402_config_header            0xFDFCFBFA
#define LD2402_config_header_a          {0xFD, 0xFC, 0xFB, 0xFA}
#define LD2402_engineering_header       0xF4F3F2F1
#define LD2402_config_footer            0x04030201
const uint8_t LD2402_config_footer_a[4] {0x04, 0x03, 0x02, 0x01};
#define LD2402_engineering_footer       0xF8F7F6F5

#include <TasmotaSerial.h>
TasmotaSerial *LD2402Serial = nullptr;

union uint32_union {
  uint8_t buffer[4];
  uint32_t buffer_32;
};

struct {
  uint8_t *buffer;
  uint8_t cmnd_param[8] = {};
  uint32_t step;
  uint32_t next_step;
  uint32_t sent_cmnd = LD2402_CMND_NOCMND;
// Static params
  char *version = NULL;
  char *serial_number = NULL;
// Detection Parameters
  uint32_t max_distance;
  uint32_t disp_delay;
  uint8_t motion_threshold[LD2402_NUM_GATES * 4];
  uint8_t micro_threshold[LD2402_NUM_GATES * 4];
// Results
  uint32_t pwr_interf;
  char gates[20] = "FFFF FFFF FFFF FFFF";
// Report values
  uint32_t person;
  uint32_t detect_distance;
  uint8_t motion_energy[LD2402_NUM_GATES * 4];
  uint8_t micro_energy[LD2402_NUM_GATES * 4];
// program flow
  uint32_t follow;
  uint32_t timer;
  enum {
    DISCONNECTED,
    ERROR,
    AUTOUPDATE,
    UNKNOWN,
    NORMAL,
    ENGINEERING,
    CONFIGURATION
  } state;
// autoupdate
  uint32_t auto_upd_progress;
// endianess
  uint32_t is_big;
} LD2402;

#define D_LD2402        "LD2402"
#define D_LD2402_LOG_PREFIX  "LD2: "
#define D_BUSY_MSG      "{\"" D_LD2402 "\":\"Busy, wait till finished current command.\"}"
#define D_DISCONNECTED_MSG "{\"" D_LD2402 "\":\"Disconnected, reconnect device.\"}"
#define D_COMMAND_PREFIX_JSON      "{\"" D_LD2402 "_%s\":"
#define D_COMMAND_HELP_MSG         D_COMMAND_PREFIX_JSON "\"%s\"}"
/********************************************************************************************/

void Ld2402HandleEngineeringData(void) {
  /* 
  01        - 0 type
  9100      - 1,2 distance
  F6110000 6C0A0000 3D020000 A3020000 20030000 50060000 57030000 48010000 F3010000 3B010000 07010000 00010000 D2000000 23010000 F3000000 F4000000 - 3+ motion gates
  B1270300 F30B0100 703E0000 8E120000 C5080000 3F100000 25030000 7A060000 7F080000 7E070000 FB050000 64040000 F3040000 2D040000 F9030000 43040000 - 67micro gates
  */
  LD2402.detect_distance = LD2402.buffer[2] << 8 | LD2402.buffer[1];
  LD2402.person = LD2402.buffer[0];
  memcpy(LD2402.motion_energy, LD2402.buffer + 3, LD2402_NUM_GATES * 4);
  memcpy(LD2402.micro_energy, LD2402.buffer + 67, LD2402_NUM_GATES * 4);
}

void Ld2402ToBinary(char *buffer, uint32_t byte_h, uint32_t byte_l) {
  uint32_t mask_01 = 0x01, mask_10 = 0x10;
  for (uint32_t i = 4; i; i--) {
    buffer[i-1] = (byte_h & mask_10) ? '1' : '0';
    buffer[i+4] = (byte_h & mask_01) ? '1' : '0';
    buffer[i+9] = (byte_l & mask_10) ? '1' : '0';
    buffer[i+14] = (byte_h & mask_01) ? '1' : '0';
    mask_10<<=1;
    mask_01<<=1;
  }
}

void Ld2402HandleConfigData(void) {
  uint32_t rcvd_cmnd = LD2402.buffer[0];
  if (rcvd_cmnd != LD2402.sent_cmnd) {
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Command mismatch sent %01x"), LD2402.sent_cmnd);
    return;
  }

  // if this is a response check the ACK
  if ((1 == LD2402.buffer[1]) && (LD2402.buffer[2] || LD2402.buffer[3])) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Command 0x%02X received ACK: 0x%02X%02X"), rcvd_cmnd, LD2402.buffer[3], LD2402.buffer[2]);
    // for non end configuration errors try to end configuration
    // for end configuration errors mark as disconnected
    if (LD2402_CMND_END_CONFIGURATION>>4 != rcvd_cmnd) {
      LD2402.step = LD2402_CMND_END_CONFIGURATION;
    } else {
      LD2402.state = LD2402.DISCONNECTED;
    }
    return;
  }

  switch (rcvd_cmnd) {
  case LD2402_CMND_START_CONFIGURATION>>4:  // run before configuration commands
    LD2402.state = LD2402.CONFIGURATION;
    LD2402.step = LD2402.next_step;
    break;

  case LD2402_CMND_END_CONFIGURATION>>4:  // run after configuration commands
    LD2402.state = LD2402.UNKNOWN;
    LD2402.step = 0;
    return;
  
  case LD2402_CMND_SAVE_PARAM>>4:  // run from terminal command
  case LD2402_CMND_AUTO_GAIN_COMPLETE>>4:  // faked after terminal command
  case LD2402_CMND_MODE>>4:  // run from terminal command
    LD2402.step = LD2402_CMND_END_CONFIGURATION;
    break;

  case LD2402_CMND_READ_VERSION>>4: { // run on initialization and reconnect
    // 00010000060076332E332E32
    // 0   2   4   6
    uint32_t length = LD2402.buffer[4];
    free(LD2402.version);
    LD2402.version = (char*)malloc(length+1);
    memcpy(LD2402.version, LD2402.buffer + 6, length);
    LD2402.version[length] = 0;
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Rcvd %s"), LD2402.version);
    LD2402.step = LD2402_CMND_READ_SERIAL;  // process read serial
    }break;
  
  case LD2402_CMND_READ_SERIAL>>4: { // run after read version
    // 1101000008004646464646464646
    // 0   2   4   6
    uint32_t length = LD2402.buffer[4];
    free(LD2402.serial_number);
    LD2402.serial_number = (char*)malloc(length+1);
    memcpy(LD2402.serial_number, LD2402.buffer + 6, length);
    LD2402.serial_number[length] = 0;
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Rcvd %s"), LD2402.serial_number);
    LD2402.cmnd_param[0] = LD2402_CMND_PARAM_MAX_DISTANCE;
    LD2402.step = LD2402_CMND_READ_PARAM;  // do initial read all params
    }break;

  case LD2402_CMND_READ_PARAM>>4:  // run from terminal command and read serial
    switch (LD2402.cmnd_param[0]) {
    // common params
    case LD2402_CMND_PARAM_MAX_DISTANCE:
      LD2402.max_distance = LD2402.buffer[4];
      LD2402.cmnd_param[0] = LD2402_CMND_PARAM_DISP_DELAY;
      LD2402.step = LD2402_CMND_READ_PARAM;
      break;
    case LD2402_CMND_PARAM_DISP_DELAY:
      LD2402.disp_delay = LD2402.buffer[5] << 8 | LD2402.buffer[4];
      LD2402.cmnd_param[0] = LD2402_CMND_PARAM_PWR_INTERFER;
      LD2402.step = LD2402_CMND_READ_PARAM;
      break;
    case LD2402_CMND_PARAM_PWR_INTERFER:
      LD2402.pwr_interf = LD2402.buffer[4];
      LD2402.cmnd_param[0] = LD2402_CMND_PARAM_MOTION_START;
      if (2 == LD2402.pwr_interf) {
        // sidetrack through interference details
        LD2402.step = LD2402_CMND_AUTO_INTERFERENCE;
      } else {
        LD2402.step = LD2402_CMND_READ_PARAM;
      }
      break;
    case LD2402_CMND_PARAM_MOTION_END:
      memcpy(LD2402.motion_threshold + (LD2402.cmnd_param[0] & 0x0F) * 4, LD2402.buffer + 4, 4);
      LD2402.cmnd_param[0] = LD2402_CMND_PARAM_MICRO_START;
      LD2402.step = LD2402_CMND_READ_PARAM;
      break;
    case LD2402_CMND_PARAM_MICRO_END:
      memcpy(LD2402.micro_threshold + (LD2402.cmnd_param[0] & 0x0F) * 4, LD2402.buffer + 4, 4);
      LD2402.step = LD2402_CMND_END_CONFIGURATION;
      break;
    default:
      switch (LD2402.cmnd_param[0] & 0xF0) {
      // gate specific params
      case LD2402_CMND_PARAM_MOTION_START:
        memcpy(LD2402.motion_threshold + (LD2402.cmnd_param[0] & 0x0F) * 4, LD2402.buffer + 4, 4);
        break;
      case LD2402_CMND_PARAM_MICRO_START:
        memcpy(LD2402.micro_threshold + (LD2402.cmnd_param[0] & 0x0F) * 4, LD2402.buffer + 4, 4);
        break;
      default:
        AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Unknown read parameter type 0x%02x"), LD2402.cmnd_param[0]);
        LD2402.step = LD2402_CMND_END_CONFIGURATION;
        return;
      }
      LD2402.cmnd_param[0]++;
      LD2402.step = LD2402_CMND_READ_PARAM;
      break;
    }
    break;

  case LD2402_CMND_AUTO_INTERFERENCE>>4:  // sidetracked here through read param go back to it
    // 140100000000
    // 0   2   4
    Ld2402ToBinary(LD2402.gates, LD2402.buffer[5], LD2402.buffer[4]);
    LD2402.step = LD2402_CMND_READ_PARAM;
    break;
  
  case LD2402_CMND_AUTO_THRESHOLD>>4:  // run from terminal command
    LD2402.state = LD2402.AUTOUPDATE;
    LD2402.step = LD2402_CMND_AUTO_PROGRESS;
    break;

  case LD2402_CMND_AUTO_PROGRESS>>4:  // run after auto threshold
    // 0A0100003C00
    // 0   2   4
    LD2402.auto_upd_progress = LD2402.buffer[4];
    if (100 > LD2402.auto_upd_progress) {
      LD2402.step = LD2402_CMND_AUTO_PROGRESS;
      return;  // no point running command right away
    }
    LD2402.state = LD2402.CONFIGURATION;
    LD2402.step = LD2402_CMND_READ_PARAM;
    break;

  case LD2402_CMND_AUTO_GAIN>>4:  // run from terminal command
    // auto gain complete is just received no command is sent
    // so pretend as if we sent a command and wait for it
    LD2402.sent_cmnd = LD2402_CMND_AUTO_GAIN_COMPLETE>>4;
    LD2402.step = CMD_LD2402_Wait_Timeout;
    break;

  case LD2402_CMND_WRITE_PARAM>>4:  // run from terminal command
    switch (LD2402.cmnd_param[0]) {
    case LD2402_CMND_PARAM_MAX_DISTANCE:
      Ld2402LoadLastParam(LD2402_CMND_PARAM_DISP_DELAY);
      LD2402.step = LD2402_CMND_WRITE_PARAM;
      break;
    case LD2402_CMND_PARAM_DISP_DELAY:
    case LD2402_CMND_PARAM_MOTION_END:
    case LD2402_CMND_PARAM_MICRO_END:
      LD2402.step = LD2402_CMND_END_CONFIGURATION;
      break;
    default:
      switch (LD2402.cmnd_param[0] & 0xF0) {
      // gate specific params
      case LD2402_CMND_PARAM_MICRO_START:
      case LD2402_CMND_PARAM_MOTION_START:
        Ld2402LoadLastParam(++LD2402.cmnd_param[0]);
        LD2402.step = LD2402_CMND_WRITE_PARAM;
        break;
      default:
        AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Unknown write parameter type 0x%02x"), LD2402.cmnd_param[0]);
        LD2402.step = LD2402_CMND_END_CONFIGURATION;
        return;
      }
    }
    break;

  default:
    AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Unknown command response recevied 0x%02x"), rcvd_cmnd);
    return;
  }
  Ld2402EveryXMSecond();  // immediately send the next command queued
}

void Ld2402Input(void) {
  uint32_t avail;
  while ((avail = LD2402Serial->available()) && avail) {
    static uint32_t byte_counter = 0;
    static uint32_union header;
    uint32_t data_type;
    if (0 == byte_counter) {
      while (LD2402Serial->available() < 4) {
        yield();
      }
      for (uint32_t i = 3; i; i--) {
        header.buffer_32<<=8;
        header.buffer[LD2402.is_big*3] = LD2402Serial->read();
      }
    }
    header.buffer_32<<=8;
    header.buffer[LD2402.is_big*3] = LD2402Serial->read();
    byte_counter = 4;

    // I will get out of this mess with either:
    //  - Config/Engineering header match, set data_type/state (most likely/quickest further processing)
    //  - Known text match, process line, set state, clear byte count for next line (less likely but no further processing)
    //  - No header matches, continue, shift in the next character if/when available
    data_type = (LD2402_engineering_header == header.buffer_32) * 2;
    if (!data_type) { // not engineering line
      data_type = (LD2402_config_header == header.buffer_32) * 3;
      if (!data_type) { // not command return
        if (LD2402_target_Error != header.buffer_32) { // not an error line
          if (LD2402_engineering_footer != header.buffer_32) { // not a missed engineering line
            if (LD2402_target_OFF != header.buffer_32) { // not an OFF line
              if (LD2402_target_distance != header.buffer_32) { // not a Distance line
                DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Head %*_H"), 4, header.buffer);
                continue;
              } else { // Distance header
                byte_counter = LD2402Serial->readBytesUntil(0x0A, LD2402.buffer, LD2402_BUFFER_SIZE);
                LD2402.buffer[byte_counter] = 0;
                DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Dist%s"), LD2402.buffer);
                // ance:105\r\n
                LD2402.detect_distance = atoi((char*) LD2402.buffer + 5);
                LD2402.person = 2;
              }
            } else { // OFF header
              LD2402.detect_distance = 0;
              LD2402.person = 0;
              LD2402Serial->read();
              DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "OFF"));
            }
            // only allow OFF and Distance lines to get here
            LD2402.state = LD2402.NORMAL;
          } else { // engineering footer
            DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Found engineering footer but have no header!"));
            LD2402.state = LD2402.ENGINEERING;
          }
        } else { // error header
          byte_counter = LD2402Serial->readBytesUntil(0x0A, LD2402.buffer, LD2402_BUFFER_SIZE);
          LD2402.buffer[byte_counter] = 0;
          AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Erro%s"), LD2402.buffer);
          LD2402.state = LD2402.ERROR;
        }
        byte_counter = 0;
        break;
      }
    } else { // engineering header
      LD2402.state = LD2402.ENGINEERING;
    }
    byte_counter = 0;

    while (LD2402Serial->available() < 2) { yield(); }
    uint32_t length = LD2402Serial->read();
    LD2402Serial->read();
    if (length > LD2402_BUFFER_SIZE) {
      break;
    }

    if (LD2402Serial->readBytes(LD2402.buffer, length) < length) {
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Wasn't able to get whole line!"));
      break;
    }

    uint32_union footer;
    while (LD2402Serial->available() < 4) { yield(); }
    for (uint32_t i = 4; i; i--) {
      footer.buffer_32<<=8;
      footer.buffer[LD2402.is_big*3] = LD2402Serial->read();
    }

    if (2 == data_type) {
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Eng %*_H"), length, LD2402.buffer);
      if (LD2402_engineering_footer == footer.buffer_32) {
        Ld2402HandleEngineeringData();
      } else {
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Head %*_H"), 4, header.buffer);
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "avail: %d, length %d"), avail, length);
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Foot %*_H"), 4, footer.buffer);
      }
    } else {
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Cmd %*_H"), length, LD2402.buffer);
      if (LD2402_config_footer == footer.buffer_32) {
        Ld2402HandleConfigData();
      } else {
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Head %*_H"), 4, header.buffer);
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "avail: %d, length %d"), avail, length);
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Foot %*_H"), 4, footer.buffer);
      }
    }
    break;
  }
  // If here then LD2402.byte_counter could still be partial correct for next loop
}

void Ld2402SendCommand(uint8_t command, uint32_t val_len = 0);
void Ld2402SendCommand(uint8_t command, uint32_t val_len) {
  uint8_t buffer[20] = LD2402_config_header_a;

  if (val_len) {
    memcpy(buffer+8,LD2402.cmnd_param,val_len);
  } else if (LD2402_CMND_START_CONFIGURATION == command) {
    const uint8_t start_cmnd[2] = {0x01, 0x00};
    memcpy(buffer+8, start_cmnd, 2);
    val_len = 2;
  }

  buffer[4] = val_len + 2;
  buffer[6] = command;
  memcpy(buffer+8+val_len, LD2402_config_footer_a, sizeof(LD2402_config_footer_a));

  DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Send %*_H"), val_len + 12, buffer);
  LD2402.sent_cmnd = command;
  LD2402Serial->flush();
  LD2402Serial->write(buffer, val_len + 12);
}

void Ld2402ExecConfigCmnd(uint32_t cmnd) {
  LD2402.next_step = cmnd;
  LD2402.step = LD2402_CMND_START_CONFIGURATION;
}

void Ld2402LoadLastParam(uint32_t curr_param) {
  memset(LD2402.cmnd_param, 0x00, 6);
  LD2402.cmnd_param[0] = curr_param;
  switch (curr_param) {
  case LD2402_CMND_PARAM_MAX_DISTANCE:
    LD2402.cmnd_param[2] = LD2402.max_distance;
    break;
  
  case LD2402_CMND_PARAM_DISP_DELAY:
    LD2402.cmnd_param[2] = LD2402.disp_delay & 0x00FF;
    LD2402.cmnd_param[3] = LD2402.disp_delay >> 8;
    break;
  
  default:
    switch (curr_param & 0xF0) {
    case LD2402_CMND_PARAM_MOTION_START:
      memcpy(LD2402.cmnd_param + 2, LD2402.motion_threshold + (curr_param & 0x0F) * 4, 4);
      break;
    
    case LD2402_CMND_PARAM_MICRO_START:
      memcpy(LD2402.cmnd_param + 2, LD2402.micro_threshold + (curr_param & 0x0F) * 4, 4);
      break;
    
    default:
      break;
    }
    break;
  }
}

void Ld2402WriteThresholds(uint8_t *thresholds, uint32_t cmnd_param) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    ResponseAppend_P(PSTR("\" D_BUSY_MSG \"}"));
    return;
  }
  if (ArgC() != 16) {
    ResponseAppend_P(PSTR("\"Requires 1,2,..16 (values are dB <= 95.00\"}"));
    return;
  }
  char strbuf[24];
  float param;
  uint32_t i = 0, val;
  for (uint32_t j = 0; j < LD2402_NUM_GATES; j++) {
    ArgV(strbuf, j+1);
    param = CharToFloat(strbuf) / 10.0f;
    val = exp10(param > 9.5f ? 9.5f : param);
    thresholds[i++] = val & 0x000000FF;
    thresholds[i++] = val >> 8 & 0x000000FF;
    thresholds[i++] = val >> 16 & 0x000000FF;
    thresholds[i++] = val >> 24;
  }
  Ld2402LoadLastParam(cmnd_param);
  Ld2402ExecConfigCmnd(LD2402_CMND_WRITE_PARAM);
  ResponseAppend_P(PSTR("\"Accepted... Allow 2 seconds to write to device.\"}"));
}

void Ld2402ResponseAppendGates(uint8_t *energies) {
  const float multiplier = 10.0f / 2.302585f;
  uint32_t i = 0;
  float val;
  while (i < LD2402_NUM_GATES * 4) {
    val = energies[i++] | energies[i++] << 8 | energies[i++] << 16 | energies[i++] << 24;
    val = (val ? multiplier * logf(val) : 0);
    if (4 != i) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("%2_f"), &val);
  }
}

void Ld2402ResponseAppendReport() {
  if (LD2402.AUTOUPDATE == LD2402.state) {
    ResponseAppend_P(PSTR("\"AutoUpdate\":\"%1d%%\""), LD2402.auto_upd_progress);
  } else if (LD2402.DISCONNECTED == LD2402.state) {
    ResponseAppend_P(PSTR("\"Error\":\"Disconnected\""));
  } else {
    ResponseAppend_P(PSTR("\"" D_JSON_DISTANCE "\":%d"), LD2402.detect_distance);
    if (LD2402.ENGINEERING == LD2402.state) {
      ResponseAppend_P(PSTR(",\"Motion\":%s"), (1 == LD2402.person ? "true" : "false"));
      ResponseAppend_P(PSTR(",\"Occupancy\":%s"), (LD2402.person ? "true" : "false"));
      ResponseAppend_P(PSTR(",\"PowerInterference\":%d"), LD2402.pwr_interf);
      if (2 == LD2402.pwr_interf) { ResponseAppend_P(PSTR(",\"InterferenceGates\":\"%s\""), LD2402.gates); }
      ResponseAppend_P(PSTR(",\"MotionEnergies\":["));
      Ld2402ResponseAppendGates(LD2402.motion_energy);
      ResponseAppend_P(PSTR("],\"MicroEnergies\":["));
      Ld2402ResponseAppendGates(LD2402.micro_energy);
      ResponseAppend_P(PSTR("]"));
    }
  }
}

/********************************************************************************************/

void Ld2402EveryXMSecond(void) {
  if (LD2402.step) {
    uint32_t command = LD2402.step>>4;
    switch (LD2402.step--) {
    // special start configuration command
    case LD2402_CMND_START_CONFIGURATION:
      Ld2402SendCommand(command);
      break;
    
    // process commands with 2 byte argument
    case LD2402_CMND_READ_PARAM:
      Ld2402SendCommand(command, 2);
      break;

    // process commands with no argument
    case LD2402_CMND_AUTO_PROGRESS-CMD_LD2402_PROGRESS_DELAY:
    case LD2402_CMND_AUTO_INTERFERENCE:
    case LD2402_CMND_READ_VERSION:
    case LD2402_CMND_READ_SERIAL:
    case LD2402_CMND_AUTO_GAIN:
    case LD2402_CMND_SAVE_PARAM:
    case LD2402_CMND_END_CONFIGURATION:
      Ld2402SendCommand(command);
      break;
    
    // process commands with 6 byte argument
    case LD2402_CMND_AUTO_THRESHOLD:
    case LD2402_CMND_WRITE_PARAM:
    case LD2402_CMND_MODE:
      Ld2402SendCommand(command, 6);
      break;

    case CMD_LD2402_Wait_Timeout-CMD_LD2402_TIMEOUT_DELAY:
      AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Device disconnected"));
      LD2402.state = LD2402.DISCONNECTED;
      return;

    default:
      return;
    }
    LD2402.step = CMD_LD2402_Wait_Timeout;
  }
}

void Ld2402EverySecond(void) {
  static uint32_t person = 4;
  if (!LD2402.state) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LD2402_LOG_PREFIX "Trying to connect."));
    Ld2402ExecConfigCmnd(LD2402_CMND_READ_VERSION);
    return;
  }

  // if person has not changed and timer is not set, return
  if (!LD2402.timer && (LD2402.person == person)) { return; }
  // if timer is set, reduce timer
  if (LD2402.timer) { --LD2402.timer; }
  // if person has not changed and timer is still set, return
  if (LD2402.timer && (LD2402.person == person)) { return; }
  // if person has changed, save new person
  if (LD2402.person != person) { person = LD2402.person; }
  // if timer is not set, set to follow
  if (!LD2402.timer) { LD2402.timer = LD2402.follow; }

  MqttPublishSensor();
}

void Ld2402Detect(void) {
  if (PinUsed(GPIO_LD2402_RX) && PinUsed(GPIO_LD2402_TX)) {
    LD2402.buffer = (uint8_t*)malloc(LD2402_BUFFER_SIZE);
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LD2402_LOG_PREFIX "Buff size %d"), LD2402_BUFFER_SIZE);
    if (!LD2402.buffer) { AddLog(LOG_LEVEL_DEBUG, PSTR(D_LD2402_LOG_PREFIX "No buff")); return; }
    LD2402Serial = new TasmotaSerial(Pin(GPIO_LD2402_RX), Pin(GPIO_LD2402_TX), 2, 0, LD2402_BUFFER_SIZE);
    if (LD2402Serial->begin(115200)) {
      if (LD2402Serial->hardwareSerial()) { ClaimSerial(); }
      LD2402Serial->setTimeout(200);
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LD2402_LOG_PREFIX "Serial UART%d"), LD2402Serial->getUart());
#endif
      volatile uint32_t i=0x01234567;
      LD2402.is_big = (*((uint8_t*)(&i))) != 0x67;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMD_MODE                  "Mode"
#define D_CMD_AUTOUPDATE            "AutoUpdate"
#define D_CMD_AUTOGAIN              "AutoGain"
#define D_CMD_STATUS                "Status"
#define D_CMD_SETCOMMON             "SetCommon"
#define D_CMD_SETMOTION             "SetMotion"
#define D_CMD_SETMICRO              "SetMicro"
#define D_CMD_SAVE                  "Save"
#define D_CMD_HELP                  "Help"
#define D_CMD_REREAD                "ReRead"
#define D_CMD_FOLLOW                "Follow"

const char kLd2402Commands[] PROGMEM = D_LD2402 "_|"  // Prefix
  D_CMD_MODE "|" D_CMD_AUTOUPDATE "|" D_CMD_AUTOGAIN "|" D_CMD_STATUS "|" D_CMD_SETCOMMON "|"
  D_CMD_SETMOTION "|" D_CMD_SETMICRO "|" D_CMD_SAVE "|" D_CMD_HELP "|" D_CMD_REREAD "|"
  D_CMD_FOLLOW;

void (* const Ld2402Command[])(void) PROGMEM = {
  &CmndLd2402Mode, &CmndLd2402AutoUpdate, &CmndLd2402AutoGain, &CmndLd2402Status, &CmndLd2402Common,
  &CmndLd2402Motion, &CmndLd2402Micro, &CmndLd2402Save, &CmndLd2402Help, &CmndLd2402ReRead,
  &CmndLd2402Follow };

void CmndLd2402Help(void) {
  Response_P(PSTR(D_COMMAND_PREFIX_JSON"\"%s"), D_CMD_HELP, "Available commands: ");
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_MODE", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_AUTOUPDATE", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_AUTOGAIN" (fw 3.3.5+), "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_STATUS", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SETCOMMON", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SETMOTION", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SETMICRO", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SAVE" (fw 3.3.2+), "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_HELP", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_REREAD", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_FOLLOW"\"}"));
}

void CmndLd2402Follow(void) {
  if (ArgC() != 1) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_FOLLOW, "Args: 0|n (MQTT update normal|MQTT update every n second(s))");
    return;
  }
  char Argument[XdrvMailbox.data_len];
  ArgV(Argument,1);
  LD2402.timer = atoi(Argument);
  LD2402.follow = LD2402.timer;
  Response_P(PSTR(D_COMMAND_PREFIX_JSON"%d}"), D_CMD_FOLLOW, LD2402.follow);
}

void CmndLd2402Status(void) {
  if (LD2402.DISCONNECTED == LD2402.state) {
    Response_P(PSTR(D_DISCONNECTED_MSG));
    return;
  }
  if (ArgC() != 1) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_STATUS, "Args: 0|1|2 (parameters|sensor|firmware)");
    return;
  }
  uint32_t status_type = 0;
  if (ArgC() == 1) {
    char Argument[XdrvMailbox.data_len];
    ArgV(Argument,1);
    status_type = atoi(Argument);
  }
  
  Response_P(PSTR("{\"" D_LD2402));
  if (1 == status_type) {
    ResponseAppend_P(PSTR("SNS\":{"));
    Ld2402ResponseAppendReport();
    ResponseJsonEnd();
  } else if (2 == status_type) {
    ResponseAppend_P(PSTR("FWR\":{\"Version\":\"%s\","),LD2402.version);  
    ResponseAppend_P(PSTR("\"SerialNumber\":\"%s\"}"),LD2402.serial_number);  
  } else {
    ResponseAppend_P(PSTR("\":{\"MaximumDistance\":%d,"), LD2402.max_distance);
    ResponseAppend_P(PSTR("\"DisappearenceDelay\":%d,"), LD2402.disp_delay);
    ResponseAppend_P(PSTR("\"MotionThresholds\":["));
    Ld2402ResponseAppendGates(LD2402.motion_threshold);
    ResponseAppend_P(PSTR("],\"MicroThresholds\":["));
    Ld2402ResponseAppendGates(LD2402.micro_threshold);
    ResponseAppend_P(PSTR("]}"));
  }
  ResponseJsonEnd();
}

void CmndLd2402AutoGain(void) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    Response_P(PSTR(D_BUSY_MSG));
    return;
  }
  if (ArgC()) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOGAIN, "No Args: Starts the modules's automatic gain adjustment");
    return;
  }
  Ld2402ExecConfigCmnd(LD2402_CMND_AUTO_GAIN);
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOGAIN, "Automatic gain ...");
}

void CmndLd2402ReRead(void) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    Response_P(PSTR(D_BUSY_MSG));
    return;
  }
  if (ArgC()) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_REREAD, "No Args: Retrieves common and threshold data from device");
    return;
  }
  LD2402.cmnd_param[0] = LD2402_CMND_PARAM_MAX_DISTANCE;
  Ld2402ExecConfigCmnd(LD2402_CMND_READ_PARAM);
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_REREAD, "Retrieving ... wait six seconds for it to complete");
}

void CmndLd2402Common(void) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    Response_P(PSTR(D_BUSY_MSG));
    return;
  }
  if (ArgC() != 2) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SETCOMMON, "Args: MaximumDistance,DisappearanceDelay (decimeter,seconds)");
    return;
  }
  uint32_t param[2] = { 0 };
  ParseParameters(2, param);
  if (param[0]>100) {param[0] = 100;}
  if (param[0]<7) {param[0] = 7;}
  if (param[1]<0) {param[1] = 0;}
  if (param[1]>65535) {param[1] = 65535;}
  LD2402.max_distance = param[0];
  LD2402.disp_delay = param[1];
  Ld2402LoadLastParam(LD2402_CMND_PARAM_MAX_DISTANCE);
  Ld2402ExecConfigCmnd(LD2402_CMND_WRITE_PARAM);
  CmndLd2402Status();
}

void CmndLd2402Mode(void) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    Response_P(PSTR(D_BUSY_MSG));
    return;
  }
  if (ArgC() != 1) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_MODE, "Args: 0|1 (normal|engineering)");
    return;
  }
  char Argument[XdrvMailbox.data_len];
  ArgV(Argument,1);
  memset(LD2402.cmnd_param, 0x00, 6);
  LD2402.cmnd_param[2] = atoi(Argument) ? 0x04 : 0x64;
  Response_P(PSTR(D_COMMAND_PREFIX_JSON "\"%s\"}"), D_CMD_MODE, (0x04 == LD2402.cmnd_param[2] ? "Engineering" : "Normal"));
  Ld2402ExecConfigCmnd(LD2402_CMND_MODE);
}

void CmndLd2402AutoUpdate(void) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    Response_P(PSTR(D_BUSY_MSG));
    return;
  }
  if (ArgC() != 3) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOUPDATE, "Args: trigger,keep,micro (magnification factor, 1.0 to 20.0)");
    return;
  }

  char strbuf[24];
  for (uint32_t i = 3; i; i--) {
    ArgV(strbuf, i);
    float param = CharToFloat(strbuf);
    LD2402.cmnd_param[(i-1)*2] = (param < 1.0f) ? 10.0f : (param > 20.0f ? 200.0f : param * 10.0f);
  }
  Ld2402ExecConfigCmnd(LD2402_CMND_AUTO_THRESHOLD);
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOUPDATE, "Updating ...");
}

void CmndLd2402Save(void) {
  if (LD2402.CONFIGURATION == LD2402.state) {
    Response_P(PSTR(D_BUSY_MSG));
    return;
  }
  if (ArgC()) {
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SAVE, "No Args: Saves configuration parameters in case of power failure (v3.3.2 and above)");
    return;
  }
  Ld2402ExecConfigCmnd(LD2402_CMND_SAVE_PARAM);
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SAVE, "Saving ...");
}

void CmndLd2402Motion(void) {
  Response_P(PSTR(D_COMMAND_PREFIX_JSON), D_CMD_SETMOTION);
  Ld2402WriteThresholds(LD2402.motion_threshold, LD2402_CMND_PARAM_MOTION_START);
}

void CmndLd2402Micro(void) {
  Response_P(PSTR(D_COMMAND_PREFIX_JSON), D_CMD_SETMICRO);
  Ld2402WriteThresholds(LD2402.micro_threshold, LD2402_CMND_PARAM_MICRO_START);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_LD2402_DIS[] PROGMEM =
  "{s}" D_LD2402 " " D_DISCONNECTED "{e}";
const char HTTP_SNS_LD2402_ERR[] PROGMEM =
  "{s}" D_LD2402 " " D_ERROR "{e}";
const char HTTP_SNS_LD2402_INI[] PROGMEM =
  "{s}" D_LD2402 " " D_INITIALIZED "{e}";
const char HTTP_SNS_LD2402_CM[] PROGMEM =
  "{s}" D_LD2402 " " D_DISTANCE "{m}%d " D_UNIT_CENTIMETER "{e}";
const char HTTP_SNS_LD2402_MV[] PROGMEM =
  "{s}" D_LD2402 " " D_DISTANCE " (!){m}%d " D_UNIT_CENTIMETER "{e}";
const char HTTP_SNS_LD2402_UPD[] PROGMEM =
  "{s}" D_LD2402 " " D_AUTO_UPDATE "{m}%d " D_UNIT_PERCENT "{e}";

void Ld2402Web(void) {
  if (LD2402.AUTOUPDATE == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_UPD, LD2402.auto_upd_progress);
  } else if (LD2402.NORMAL == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_CM, LD2402.detect_distance);
  } else if (LD2402.ENGINEERING == LD2402.state) {
    WSContentSend_PD((1 == LD2402.person ? HTTP_SNS_LD2402_MV : HTTP_SNS_LD2402_CM), LD2402.detect_distance);
  } else if (LD2402.ERROR == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_ERR);
  } else if (LD2402.DISCONNECTED == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_DIS);
  } else {
    WSContentSend_PD(HTTP_SNS_LD2402_INI);
  }
}
#endif

void Ld2402Show(void) {
  ResponseAppend_P(PSTR(",\"" D_LD2402 "\":{"));
  Ld2402ResponseAppendReport();
  ResponseJsonEnd();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns102(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    Ld2402Detect();
  }
  else if (LD2402Serial) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        Ld2402Input();
        break;
      case FUNC_EVERY_250_MSECOND:
        Ld2402EveryXMSecond();
        break;
      case FUNC_EVERY_SECOND:
        Ld2402EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ld2402Show();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ld2402Web();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kLd2402Commands, Ld2402Command);
        break;
    }
  }
  return result;
}

#endif  // USE_LD2402
