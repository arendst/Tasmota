/*
  xsns_102_ld2410s.ino - HLK-LD2410S 24GHz smart wave motion sensor support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends, 2024 md5sum-as (https://github.com/md5sum-as)

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_LD2410S

/*********************************************************************************************\
 * HLK-LD2410S 24GHz smart wave motion sensor
 * 
 * Attention!
 * This module works with HLK-LD2410S devices. 
 * The module does not support another HLK-LD2410 devices.
 * 
 * Available commands:
 * LD2410S_Parameters                          - showing previously received parameters
 * LD2410S_ReRead                              - reread common, trigger and hold parameters from device
 * LD2410S_SetCommon 0-16,1-16,10-120,5-80,5-80,5/10 - set common: near door, far door, hold_time, status_freq, distance_freq, response_speed
 * LD2410S_SetTrigger n,n1..n16                - set trigger values (16)
 * LD2410S_SetHold n,n1..n16                   - set hold values (16)
 * LD2410S_Out_Mode 0/1                        - set device output mode 0-short (only distance and 0/1 - no people, 2/3 detect people), 1-normal mode (add energy values per door)
 * LD2410S_AutoUpdate 2,1,60                   - start autoupdate trigger and hold thresholds/ Params: trigger_scale,retension_factor,scan_time
 * LD2410S_Follow 0/1                          - if 1 then start reports every seconds
 *
\*********************************************************************************************/

#define XSNS_102                         102

#undef TM_SERIAL_BUFFER_SIZE
#define TM_SERIAL_BUFFER_SIZE            128
#define LD2410S_BUFFER_SIZE               TM_SERIAL_BUFFER_SIZE  // 128

#define LD2410S_NUM_GATES                16

#define LD2410S_CMND_START_CONFIGURATION  0xFF
#define LD2410S_CMND_END_CONFIGURATION    0xFE
#define LD2410S_CMND_SET_COMMON           0x70
#define LD2410S_CMND_READ_COMMON          0x71
#define LD2410S_CMND_AUTO_THRESHOLD       0x09
#define LD2410S_CMND_WRITE_TRIGGER        0x72
#define LD2410S_CMND_READ_TRIGGER         0x73
#define LD2410S_CMND_WRITE_HOLD           0x76
#define LD2410S_CMND_READ_HOLD            0x77
#define LD2410S_CMND_OUTPUT_MODE          0x7A

#define CMD_LD2410S_Read_Parametrs  40
#define CMD_LD2410S_Write_Common  50
#define CMD_LD2410S_Out_Mode  60
#define CMD_LD2410S_Auto_Update 100
#define CMD_LD2410S_Write_Trigger 70
#define CMD_LD2410S_Write_Hold 65

const uint8_t LD2410_config_header[4] = {0xFD, 0xFC, 0xFB, 0xFA};
const uint8_t LD2410_config_footer[4] = {0x04, 0x03, 0x02, 0x01};
const uint8_t LD2410_target_header[4] = {0xF4, 0xF3, 0xF2, 0xF1};
const uint8_t LD2410_target_footer[4] = {0xF8, 0xF7, 0xF6, 0xF5};

#include <TasmotaSerial.h>
TasmotaSerial *LD2410Serial = nullptr;

struct {
  uint8_t *buffer;
// Common params
  uint8_t far_end;
  uint8_t near_end;
  uint8_t hold_duration;
  uint8_t status_report_f;
  uint8_t distance_report_f;
  uint8_t response_speed;
// gates param
  uint8_t trigger_energy[LD2410S_NUM_GATES]; // not 16
  uint8_t hold_energy[LD2410S_NUM_GATES]; // not 16
// Report values
  uint16_t detect_distance;
  uint8_t energy[LD2410S_NUM_GATES]; // not 16
  uint8_t human;
  uint8_t human_last;
  
//  uint8_t state;
  uint8_t step;
  uint8_t retry;
  uint8_t next_step;
  uint8_t byte_counter;
  uint8_t ack;
  uint8_t out_mode;
  uint8_t report_type;
  uint8_t follow;
// autoupdate
  uint8_t auto_upd__scale;
  uint8_t auto_upd_retension;
  uint8_t auto_upd_time;
} LD2410S;

/********************************************************************************************/

void Ld1410HandleTargetData(void) {
  if (LD2410S.step > 150) {LD2410S.step = 20;} //Stop boot delay on receive valid data
/* 
F4F3F2F1  - 0..3
4600      - 4,5 length 70
01        - 6 type
02        - 7 people
9100      - 8,9 distance
4600      - 10,11 - old dist???
00000000  - 12..15 0..3
543D0000  - 16..19 4..7
1D160000  - 20..23 8..11
2E070000 D8020000 B0040000 DF000000 FD010000 74040000 B4070000 72090000 F0040000 C8000000 F0040000 A4060000 CE050000 F8F7F6F5
*/
  if ((LD2410S.buffer[6] == 1) && (LD2410S.buffer[4] == 70)) {
    if (LD2410S.report_type == 3) {
      Ld2410ExecCommand(CMD_LD2410S_Read_Parametrs);
    }
    LD2410S.report_type = 1;
    LD2410S.detect_distance = LD2410S.buffer[9] << 8 | LD2410S.buffer[8];
    LD2410S.human = LD2410S.buffer[7];
    for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
      LD2410S.energy[i] = /*LD2410S.buffer[i * 4 + 13] << 8 |*/ LD2410S.buffer[i * 4 + 12];
    }
  }
  if ((LD2410S.buffer[6]) == 3) {
    /* F4F3F2F1 0300 03    6400 F8F7F6F5 
               len  type  percent
    */
    LD2410S.report_type = 3;
    LD2410S.detect_distance = LD2410S.buffer[7];
  }
}



void Ld1410HandleConfigData(void) {
  LD2410S.ack = 0;
  if ((LD2410S.buffer[8]==0) && (LD2410S.buffer[7]==1)) {
    LD2410S.ack = LD2410S.buffer[6];
  }
  if (LD2410S_CMND_READ_COMMON == LD2410S.buffer[6]) {
    // FDFCFBFA - header 0,1,2,3
    // 1C00 - datalen 4,5
    // 7101 - command 6,7
    // 0000 - ACK 8,9
    // 00000000 - Near 10,11,12,13
    // 0C000000 - Far 14,15,16,17
    // 0A000000 - Hold 18,19,20,21
    // 28000000 - StatusF 22,23,24,25
    // 05000000 - DistanceF 26,27,28,29
    // 05000000 - RespSpeed 30,31,32,33
    // 04030201
    LD2410S.near_end = LD2410S.buffer[10];
    LD2410S.far_end = LD2410S.buffer[14];
    LD2410S.hold_duration = LD2410S.buffer[18];
    LD2410S.status_report_f = LD2410S.buffer[22];
    LD2410S.distance_report_f = LD2410S.buffer[26];
    LD2410S.response_speed = LD2410S.buffer[30];
  } else if (LD2410S_CMND_READ_TRIGGER == LD2410S.buffer[6]) {
    /*FDFCFBFA - 0..3
    4400 - 4,5
    7301 - 6,7
    0000 - 8,9
    30000000 2A000000 24000000 22000000 20000000  1F000000 1F000000 1F000000 1F000000 1F000000 1F000000 1F000000 1F000000 1F000000 1F000000 1F000000 04030201
    10       14       20   
    */
    for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
      LD2410S.trigger_energy[i] = /*LD2410S.buffer[i * 4 + 11] << 8 |*/ LD2410S.buffer[i * 4 + 10];
    }  
  } else if (LD2410S_CMND_READ_HOLD == LD2410S.buffer[6]) {
    for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
      LD2410S.hold_energy[i] = /*LD2410S.buffer[i * 4 + 11] << 8 |*/ LD2410S.buffer[i * 4 + 10];
    }  
  }
}

bool Ld2410Match(const uint8_t *header, uint32_t offset) {
  for (uint32_t i = 0; i < 4; i++) {
    if (LD2410S.buffer[offset +i] != header[i]) { return false; }
  }
  return true;
}

bool Ld2410MatchShort(uint32_t offset) {
  if (LD2410S.buffer[offset] != 0x6e) { return false; }
  if (LD2410S.buffer[offset+1] > 0x03) { return false; }
  if (LD2410S.buffer[offset+4] != 0x62) { return false; }
  LD2410S.detect_distance=LD2410S.buffer[offset+3] << 8 | LD2410S.buffer[offset+2];
  LD2410S.human = LD2410S.buffer[offset+1];
  if (LD2410S.report_type == 3) {
    Ld2410ExecCommand(CMD_LD2410S_Read_Parametrs);
  }
  LD2410S.report_type = 0;
  if (LD2410S.step > 150) {LD2410S.step = 20;}  //Stop boot delay on receive valid data
  return true;
}

void Ld2410Input(void) {
  while (LD2410Serial->available()) {
    yield();                                                    // Fix watchdogs

    LD2410S.buffer[LD2410S.byte_counter++] = LD2410Serial->read();
//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), LD2410.byte_counter, LD2410.buffer);

    if (LD2410S.byte_counter < 5) { continue; }                  // Need first four header bytes

    uint32_t header_start = LD2410S.byte_counter -5;             // Fix interrupted header transmits
    bool short_report  = (Ld2410MatchShort(header_start));
    if (short_report) {
      LD2410S.byte_counter = 0;                                    // Finished
      break;                                                      // Exit loop to satisfy yields
    }
    bool target_header = (Ld2410Match(LD2410_target_header, header_start));  // F4F3F2F1
    bool config_header = (Ld2410Match(LD2410_config_header, header_start));  // FDFCFBFA
    if ((target_header || config_header) && (header_start != 0)) {
      memmove(LD2410S.buffer, LD2410S.buffer + header_start, 5);  // Sync buffer with header
      LD2410S.byte_counter = 5;
    }
    if (LD2410S.byte_counter < 6) { continue; }                  // Need packet size bytes

    target_header = (Ld2410Match(LD2410_target_header, 0));     // F4F3F2F1
    config_header = (Ld2410Match(LD2410_config_header, 0));     // FDFCFBFA
    if (target_header || config_header) {
      uint32_t len = LD2410S.buffer[4] +10;                      // Total packet size
      if (len > LD2410S_BUFFER_SIZE) {
        LD2410S.byte_counter = 0;                                // Invalid data
        break;                                                  // Exit loop to satisfy yields
      }
      if (LD2410S.byte_counter < len) { continue; }              // Need complete packet
      if (target_header) {                                      // F4F3F2F1
        if (Ld2410Match(LD2410_target_footer, len -4)) {        // F8F7F6F5
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), len, LD2410S.buffer);
          Ld1410HandleTargetData();
        }
      }
      else if (config_header) {                                 // FDFCFBFA
        if (Ld2410Match(LD2410_config_footer, len -4)) {        // 04030201
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), len, LD2410S.buffer);
          Ld1410HandleConfigData();
          LD2410Serial->setReadChunkMode(0);                    // Disable chunk mode fixing Hardware Watchdogs
        }
      }
    }
    LD2410S.byte_counter = 0;                                    // Finished or bad received footer
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
  Ld2410SendCommand(LD2410S_CMND_START_CONFIGURATION, value, sizeof(value));
}

void Ld2410SetOutputMode(void) {
  uint8_t value[6] = { 0,0,0,0,0,0 };
  value[2] = LD2410S.out_mode?1:0;
  Ld2410SendCommand(LD2410S_CMND_OUTPUT_MODE, value, sizeof(value));
}

void Ld2410ReadCommonParameters(void) {
  /*
  Detection of the closest distance door  0x0A  0~16
  Detection of the farthest distance door 0x05  1~16
  No delay time                           0x06  10 ~ 120s
  Status reporting frequency              0x02  0.5 ~ 8（0.5SteppingHz)
  Distance reporting frequency            0x0C  0.5 ~ 8（0.5SteppingHz)
  Response speed                          0x0B  5(normal)/10(fast)-
*/
  uint8_t value[12] = { 0x0A, 0x00, 0x05, 0x00, 0x06, 0x00, 0x02, 0x00, 0x0C, 0x00, 0x0B, 0x00};
  Ld2410SendCommand(LD2410S_CMND_READ_COMMON, value, sizeof(value));
}

void Ld2410SetCommonParametrs(void) {
  uint8_t value[36] = { 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x0B, 0x00, 0x00, 0x00, 0x00, 0x00 };
value[2] = LD2410S.near_end;
value[8] = LD2410S.far_end;
value[14] = LD2410S.hold_duration;
value[20] = LD2410S.status_report_f;
value[26] = LD2410S.distance_report_f;
value[32] = LD2410S.response_speed;
Ld2410SendCommand(LD2410S_CMND_SET_COMMON, value, sizeof(value));
}

#define Ld2410ReadTrigger() Ld2410ReadTriggerHold(LD2410S_CMND_READ_TRIGGER)
#define Ld2410ReadHold() Ld2410ReadTriggerHold(LD2410S_CMND_READ_HOLD)
void Ld2410ReadTriggerHold(uint8_t cmdn) {
  uint8_t value[32] = {0,0, 1,0, 2,0, 3,0, 4,0, 5,0, 6,0, 7,0, 8,0, 9,0, 10,0, 11,0, 12,0, 13,0, 14,0, 15,0};
  Ld2410SendCommand(cmdn, value, sizeof(value));
}

void Ld2410AutoUpdate(void) {
  uint8_t value[6] = { 0 };
  value[0] = LD2410S.auto_upd__scale;
  value[2] = LD2410S.auto_upd_retension;
  value[4] = LD2410S.auto_upd_time;
  Ld2410SendCommand(LD2410S_CMND_AUTO_THRESHOLD, value, sizeof(value));
}

#define Ld2410WriteTrigger() Ld2410WriteTriggerHold(LD2410S_CMND_WRITE_TRIGGER)
#define Ld2410WriteHold() Ld2410WriteTriggerHold(LD2410S_CMND_WRITE_HOLD)
void Ld2410WriteTriggerHold(uint8_t cmnd) {
  uint8_t value[96] = { 0 };
  for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
    value[i*6]=i;
    if (cmnd == LD2410S_CMND_WRITE_TRIGGER) {
      value[i*6+2]=LD2410S.trigger_energy[i];
    }else{
      value[i*6+2]=LD2410S.hold_energy[i];      
    }
  }
  Ld2410SendCommand(cmnd, value, sizeof(value));
}

void Ld2410ExecCommand(uint8_t cmnd) {
    LD2410S.step = 15;
    LD2410S.next_step = cmnd;
}

/********************************************************************************************/

void Ld2410Every100MSecond(void) {
  if (LD2410S.step) {
    LD2410S.step--;
    switch (LD2410S.step) {
      // boot module delay
      case 200:
        LD2410S.step = 15;
        break;
      // 100 auto update
      case 99:
        Ld2410AutoUpdate();
        LD2410S.step = 5;
        break;

      // 70 - write trigger
      case 69:
        Ld2410WriteTrigger();
        LD2410S.retry = 2;
        break;
      case 67:
        if (LD2410S.ack != LD2410S_CMND_WRITE_TRIGGER) {
          if (LD2410S.retry--) {
            LD2410S.step=70;
            break;
          }
        }
        LD2410S.step = 40; // reread params
        break;

      // 65 - write hold
      case 64:
        Ld2410WriteHold();
        LD2410S.retry = 2;
        break;
      case 62:
        if (LD2410S.ack != LD2410S_CMND_WRITE_HOLD) {
          if (LD2410S.retry--) {
            LD2410S.step=65;
            break;
          }
        }
        LD2410S.step = 40; // reread params
        break;

      // 60 - Out mode
      case 59:
        Ld2410SetOutputMode();
        LD2410S.retry = 2;
        break;
      case 57:  
        if (LD2410S.ack != LD2410S_CMND_OUTPUT_MODE) {
          if (LD2410S.retry--) {
            LD2410S.step=60;
            break;
          }
        }
        LD2410S.step = 5; // End command
        break;

      // 50 - write common
      case 49:
        Ld2410SetCommonParametrs();
        LD2410S.retry = 2;
        break;
      case 47:
        if (LD2410S.ack != LD2410S_CMND_SET_COMMON) {
          if (LD2410S.retry--) {
            LD2410S.step=50;
            break;
          }
        }
        LD2410S.step = 40; // read params
        break;

// 40 - read params
      case 39:
        Ld2410ReadCommonParameters();
        LD2410S.retry = 4;
        break;
      case 37:
        if (LD2410S.ack != LD2410S_CMND_READ_COMMON) {
          if (LD2410S.retry--) {
            LD2410S.step=40;
            break;
          }
        }
        break;
// 35 - read trigger
      case 34:
        Ld2410ReadTrigger();
        LD2410S.retry = 2;
        break;
      case 32:
        if (LD2410S.ack != LD2410S_CMND_READ_TRIGGER) {
          if (LD2410S.retry--) {
            LD2410S.step=35;
            break;
          }
        }
        break;
// 30 - read hold
      case 29:
        Ld2410ReadHold();
        LD2410S.retry = 2;
        break;
      case 27:
        if (LD2410S.ack != LD2410S_CMND_READ_HOLD) {
          if (LD2410S.retry--) {
            LD2410S.step=30;
            break;
          }
        }
        LD2410S.step=5; // End command
        break;

// 20 - loop
// 15 - Config mode
      case 14:
        Ld2410SetConfigMode();                                  // Stop running mode
        break;
      case 10:
        if (LD2410S.ack != LD2410S_CMND_START_CONFIGURATION) {
          if (LD2410S.retry--) {
            LD2410S.step = 20;                                    // Retry
          } else {
            LD2410S.step = 0;
            AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Not detected"));
          }
        } else {
          LD2410S.step = LD2410S.next_step;
        }
        break;
      case 1:
        Ld2410SendCommand(LD2410S_CMND_END_CONFIGURATION);
        break;
      default:
        break;
    }
  }
}

void Ld2410EverySecond(void) {
  if (LD2410S.human != LD2410S.human_last) {
    LD2410S.human_last = LD2410S.human;
    MqttPublishSensor();
  }else if ((LD2410S.report_type ==3 ) || (LD2410S.follow)) {
    MqttPublishSensor();
  }
}

void Ld2410Detect(void) {
  if (PinUsed(GPIO_LD2410S_RX) && PinUsed(GPIO_LD2410S_TX)) {
    LD2410S.buffer = (uint8_t*)malloc(LD2410S_BUFFER_SIZE);    // Default 64
    AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Buff size %d"), LD2410S_BUFFER_SIZE);
    if (!LD2410S.buffer) { AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: No buff")); return; }
    LD2410Serial = new TasmotaSerial(Pin(GPIO_LD2410S_RX), Pin(GPIO_LD2410S_TX), 2);
    if (LD2410Serial->begin(115200)) {
      if (LD2410Serial->hardwareSerial()) { ClaimSerial(); }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Serial UART%d"), LD2410Serial->getUart());
#endif
    LD2410S.retry = 4;
    LD2410S.step = 250;
    LD2410S.next_step = CMD_LD2410S_Read_Parametrs;

    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kLd2410Commands[] PROGMEM = "LD2410S_|"  // Prefix
  "SetCommon|Out_Mode|AutoUpdate|Parameters|SetTrigger|SetHold|Help|ReRead|Follow";

void (* const Ld2410Command[])(void) PROGMEM = {
  &CmndLd2410Common, &CmndLd2410OutMode, &CmndLd2410AutoUpdate, &CmndLd2410Response, &CmndLd2410Trigger, &CmndLd2410Hold, &CmndLd2410Help, &CmndLd2410ReRead, &CmndLd2410Follow};

void CmndLd2410Help(void) {
  Response_P(PSTR("Available commands: LD2410S_Parameters (display parameters), LD2410S_ReRead (read param from module), LD2410S_SetCommon, LD2410S_SetTrigger, LD2410S_SetHold, LD2410S_Out_Mode (0-short, 1-normal), LD2410S_AutoUpdate, LD2410S_Follow (0/1 every 1 sec.)"));
}

void CmndLd2410Response(void) {
  Response_P(PSTR("{\"LD2410S_Common\":{\"Near Door\": %d,\"Far Door\":%d,\"Hold Time\":%d,\"Status freq\":%d,\"Distance freq\":%d,\"Response speed\":%d}"),
              LD2410S.near_end, LD2410S.far_end, LD2410S.hold_duration, LD2410S.status_report_f, LD2410S.distance_report_f,LD2410S.response_speed);
  ResponseAppend_P(PSTR(",\"Trigger values\":["));
  for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
    ResponseAppend_P(PSTR("%d"), LD2410S.trigger_energy[i]);
    if (i < (LD2410S_NUM_GATES-1)) {
      ResponseAppend_P(PSTR(","));
    } else {
      ResponseAppend_P(PSTR("]"));
    }
  }
  ResponseAppend_P(PSTR(",\"Hold values\":["));
  for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
    ResponseAppend_P(PSTR("%d"), LD2410S.hold_energy[i]);
    if (i < (LD2410S_NUM_GATES-1)) {
      ResponseAppend_P(PSTR(","));
    } else {
      ResponseAppend_P(PSTR("]"));
    }
  }
  ResponseJsonEnd();
}

void CmndLd2410ReRead(void) {
  Ld2410ExecCommand(CMD_LD2410S_Read_Parametrs);
  Response_P(PSTR("Accepted... Use LD2410S_Parameters after 1 second."));
}

void CmndLd2410Common(void) {
  if (ArgC() == 6) {
    uint32_t param[6] = { 0 };
    ParseParameters(6, param);
    param[3]=(param[3]/5)*5;
    param[3]=(param[4]/5)*5;
    if (param[0]>16) {param[0] = 16;}
    if (param[1]>16) {param[1] = 16;}
    if (param[1]<1) {param[1] = 1;}
    if (param[2]>120) {param[2] = 120;}
    if (param[2]<10) {param[2] = 10;}
    if (param[3]>80) {param[3] = 80;}
    if (param[3]<5) {param[3] = 5;}
    if (param[4]>80) {param[4] = 80;}
    if (param[4]<5) {param[4] = 5;}
    LD2410S.near_end = (uint8_t)param[0];
    LD2410S.far_end = (uint8_t)param[1];
    LD2410S.hold_duration = (uint8_t)param[2];
    LD2410S.status_report_f = (uint8_t)param[3];
    LD2410S.distance_report_f = (uint8_t)param[4];
    LD2410S.response_speed = (param[5]>5?10:5);

    Ld2410ExecCommand(CMD_LD2410S_Write_Common);
//    Response_P(PSTR("Accepted."));
    CmndLd2410Response();
  } else {
    Response_P(PSTR("Use LD2410S_SetCommon near_door,far_door,hold_time,status_freq,distance_freq,response_speed"));
  }
}

void CmndLd2410OutMode(void) {
  char Argument[XdrvMailbox.data_len];
  ArgV(Argument,1);
  LD2410S.out_mode = atoi(Argument);
  Response_P(PSTR("{\"LD2410S_out_mode\":%d}"),LD2410S.out_mode);
  Ld2410ExecCommand(CMD_LD2410S_Out_Mode);
}

void CmndLd2410Follow(void) {
  char Argument[XdrvMailbox.data_len];
  ArgV(Argument,1);
  LD2410S.follow = atoi(Argument);
  Response_P(PSTR("{\"LD2410S_Follow\":%d}"),LD2410S.follow);
}

void CmndLd2410AutoUpdate(void) {
  if (ArgC() != 3) {
    Response_P(PSTR("Use LS2410S_AutoUpdate trigger_scale,retension_factor,scan_time"));
    return;
  }
  uint32_t param[3] = {0};
  ParseParameters(3, param);
  LD2410S.auto_upd__scale = param[0];
  LD2410S.auto_upd_retension = param[1];
  LD2410S.auto_upd_time = param[2];
  Ld2410ExecCommand(CMD_LD2410S_Auto_Update);
  Response_P(PSTR("LD2410S Auto Update started..."));
}

void CmndLd2410Trigger(void) {
    if (ArgC() != 16) {
    Response_P(PSTR("Use LS2410S_SetTrigger 1,2,..16"));
    return;
  }
  uint32_t param[16] = { 0 };
  ParseParameters(16, param);
  for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
    LD2410S.trigger_energy[i]=param[i];
  }
  Ld2410ExecCommand(CMD_LD2410S_Write_Trigger);
//  Response_P(PSTR("Accepted."));
    CmndLd2410Response();
}

void CmndLd2410Hold(void) {
    if (ArgC() != 16) {
    Response_P(PSTR("Use LS2410S_SetHold 1,2,..16"));
    return;
  }
  uint32_t param[16] = { 0 };
  ParseParameters(16, param);
  for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
    LD2410S.hold_energy[i]=param[i];
  }
  Ld2410ExecCommand(CMD_LD2410S_Write_Hold);
//  Response_P(PSTR("Accepted."));
    CmndLd2410Response();
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_LD2410_CM[] PROGMEM =
  "{s}LD2410S " D_DETECT_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}";

const char HTTP_SNS_LD2410_UPD[] PROGMEM =
  "{s}LD2410S Auto Update{m}%d " D_UNIT_PERCENT "{e}";

#endif

#ifndef D_JSON_PEOPLE
#define D_JSON_PEOPLE "People"
#endif

void Ld2410Show(bool json) {
  float detect_distance = LD2410S.detect_distance;

  if (json) {
    if (LD2410S.report_type != 3) {
      ResponseAppend_P(PSTR(",\"LD2410S\":{\"" D_JSON_DISTANCE "\":%1_f, \"" D_JSON_PEOPLE "\":%d"), &detect_distance, LD2410S.human);
      if (LD2410S.report_type == 1) {
        ResponseAppend_P(PSTR(", \"Energy\":["));
        for (uint32_t i = 0; i < LD2410S_NUM_GATES; i++) {
          ResponseAppend_P(PSTR("%d"), LD2410S.energy[i]);
          if (i < (LD2410S_NUM_GATES-1)) {
            ResponseAppend_P(PSTR(","));
          } else {
            ResponseAppend_P(PSTR("]"));
          }
        }
      }
      ResponseJsonEnd();
    }else{
      ResponseAppend_P(PSTR(",\"LD2410S\":{\"Update threshold\":\"%1d%%\"}"), LD2410S.detect_distance);
    }
#ifdef USE_WEBSERVER
  } else {
    if (LD2410S.report_type != 3) {
     WSContentSend_PD(HTTP_SNS_LD2410_CM, &detect_distance);
    }else{
      WSContentSend_PD(HTTP_SNS_LD2410_UPD, LD2410S.detect_distance);
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

#endif  // USE_LD2410S
