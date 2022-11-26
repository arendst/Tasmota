/*
  xsns_102_ld2410.ino - HLK-LD2410 24GHz smart wave motion sensor support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_LD2410
/*********************************************************************************************\
 * HLK-LD2410 24GHz smart wave motion sensor
 *
 * Inspiration:
 * https://community.home-assistant.io/t/mmwave-wars-one-sensor-module-to-rule-them-all/453260/2
 * Resources:
 * https://drive.google.com/drive/folders/1p4dhbEJA3YubyIjIIC7wwVsSo8x29Fq-?spm=a2g0o.detail.1000023.17.93465697yFwVxH
 *
 * Basic comms using Serial Bridge:
 *   sbaudrate 256000
 *   serialdelimiter 254
 *   sserialsend5 FDFCFBFA0400FF00010004030201 -> {"SSerialReceived":"FDFCFBFA0800FF0100000100400004030201"}
 *   sserialsend5 FDFCFBFA0200610004030201 -> {"SSerialReceived":"FDFCFBFA1C0061010000AA0808083232281E140F0F0F0F000028281E1E141414050004030201"}
 *   sserialsend5 FDFCFBFA0200A00004030201 -> {"SSerialReceived":"FDFCFBFA0C00A0010000000107011615092204030201"}
\*********************************************************************************************/

#define XSNS_102                         102

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
  uint8_t moving_energy;
  uint8_t static_energy;
  uint8_t init_step;
  uint8_t init_retry;
  uint8_t byte_counter;
  bool valid_response;
} LD2410;

/********************************************************************************************/

uint32_t ToBcd(uint32_t value) {
  return ((value >> 4) * 10) + (value & 0xF);
}

/********************************************************************************************/

void Ld1410HandleTargetData(void) {
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
  // F4 F3 F2 F1 0D 00 02 AA 00 00 00 00 00 00 37 00 00 55 00 F8 F7 F6 F5 - No target
  // F4 F3 F2 F1 0D 00 02 AA 00 45 00 3E 00 00 3A 00 00 55 00 F8 F7 F6 F5 - No target
  // F4 F3 F2 F1 0D 00 02 AA 03 46 00 34 00 00 3C 00 00 55 00 F8 F7 F6 F5 - Movement and Stationary target
  // F4 F3 F2 F1 0D 00 02 AA 02 54 00 00 00 00 64 00 00 55 00 F8 F7 F6 F5 - Stationary target
  // F4 F3 F2 F1 0D 00 02 AA 02 96 00 00 00 00 36 00 00 55 00 F8 F7 F6 F5 - Stationary target
  // F4 F3 F2 F1 0D 00 02 AA 03 2A 00 64 00 00 64 00 00 55 00 F8 F7 F6 F5 - Movement and Stationary target
  // header     |len  |dt|hd|st|movin|me|stati|se|detec|tr|ck|trailer
  if (LD2410.buffer[8] != 0x00) {                               // Movement and/or Stationary target
    LD2410.moving_distance = LD2410.buffer[10] << 8 | LD2410.buffer[9];
    LD2410.moving_energy = LD2410.buffer[11];
    LD2410.static_distance = LD2410.buffer[13] << 8 | LD2410.buffer[12];
    LD2410.static_energy = LD2410.buffer[14];
    LD2410.detect_distance = LD2410.buffer[16] << 8 | LD2410.buffer[15];

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Type %d, State %d, Moving %d/%d%%, Static %d/%d%%, Detect %d"),
      LD2410.buffer[6], LD2410.buffer[8],
      LD2410.moving_distance, LD2410.moving_energy,
      LD2410.static_distance, LD2410.static_energy,
      LD2410.detect_distance);

    if (0x01 == LD2410.buffer[6]) {                             // Engineering mode data
      // Adds 22 extra bytes of data

    }
  } else {
    LD2410.moving_distance = 0;
    LD2410.moving_energy = 0;
    LD2410.static_distance = 0;
    LD2410.static_energy = 0;
    LD2410.detect_distance = 0;
  }
}

void Ld1410HandleConfigData(void) {
  if (LD2410_CMND_READ_PARAMETERS == LD2410.buffer[6]) {           // 0x61
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37
    // FD FC FB FA 1C 00 61 01 00 00 AA 08 08 08 32 32 28 1E 14 0F 0F 0F 0F 00 00 28 28 1E 1E 14 14 14 05 00 04 03 02 01 - Default
    // header     |len  |cw cv|ack  |hd|dd|md|sd|moving sensitivity 0..8   |static sensitivity 0..8   |timed|trailer
    //            |   28|     |    0|  | 8| 8| 8|50 50 40 30 20 15 15 15 15| 0  0 40 40 30 30 20 20 20|    5|


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
    AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Firmware version V%d.%02d.%02d%02d%02d%02d"),  // Firmware version V1.07.22091516
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
      if (len > TM_SERIAL_BUFFER_SIZE) { len = TM_SERIAL_BUFFER_SIZE; }
      if (LD2410.byte_counter < len) { continue; }              // Need complete packet

      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LD2: Rcvd %*_H"), len, LD2410.buffer);

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
    LD2410.byte_counter = 0;
  }
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

  LD2410Serial->flush();
  LD2410Serial->write(buffer, len);
//  LD2410Serial->flush();
}

void Ld2410SetConfigMode(bool state) {
  if (state) {
    uint8_t value[2] = { 0x01, 0x00 };
    Ld2410SendCommand(LD2410_CMND_START_CONFIGURATION, value, sizeof(value));
  } else {
    Ld2410SendCommand(LD2410_CMND_END_CONFIGURATION, nullptr, 0);
  }
}

void Ld2410SetBaudrate(uint32_t index) {
  uint8_t value[2] = { (uint8_t)index, 0x00 };
  Ld2410SendCommand(LD2410_CMND_SET_BAUDRATE, value, sizeof(value));
}

void Ld2410SetAllSensitivity(uint32_t sensitivity) {
  // 00 00 FF FF 00 00 01 00 28 00 00 00 02 00 28 00 00 00
  // gates|all gates  |motio|sensitivity|stati|sensitivity
  uint8_t lsb = sensitivity & 0xFF;
  uint8_t msb = (sensitivity >> 8) & 0xFF;
  uint8_t value[18] = { 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, lsb, msb, 0x00, 0x00, 0x02, 0x00, lsb, msb, 0x00, 0x00 };
  Ld2410SendCommand(LD2410_CMND_SET_SENSITIVITY, value, sizeof(value));
}

/********************************************************************************************/

void Ld2410Every250MSecond(void) {
  if (LD2410.init_step) {
    LD2410.init_step--;
    switch (LD2410.init_step) {
/*
      // POC using 57600 bps instead of default 256000 bps
      case 20:
        AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Switch to 57600 bps"));
        LD2410Serial->flush();
        LD2410Serial->begin(256000);
        break;
      case 19:
        Ld2410SetConfigMode(1);                                 // Stop running mode
        break;
      case 16:
        Ld2410SetBaudrate(4);                                   // 57600 bps
        break;
      case 14:
        Ld2410SendCommand(LD2410_CMND_REBOOT, nullptr, 0);
        LD2410Serial->flush();
        LD2410Serial->begin(57600);
      break;
*/
      case 10:
        Ld2410SetConfigMode(1);                                 // Stop running mode
        break;
      case 7:
        if (!LD2410.valid_response && LD2410.init_retry) {
          LD2410.init_retry--;
          if (LD2410.init_retry) {
//            LD2410.init_step = 21;                              // Change baudrate
            LD2410.init_step = 13;                              // Retry
          } else {
            LD2410.init_step = 0;
            AddLog(LOG_LEVEL_DEBUG, PSTR("LD2: Not detected"));
          }
        } else {
          Ld2410SendCommand(LD2410_CMND_GET_FIRMWARE);
        }
        break;
      case 4:
        Ld2410SendCommand(LD2410_CMND_READ_PARAMETERS);
        break;
      case 1:
        Ld2410SetConfigMode(0);
        break;
    }
  } else {
/*
    // Four times a second might be too much to handle by rules
    if (LD2410.moving_energy) {
      // Send state change to be captured by rules
      // {"Time":"2022-11-26T10:48:16","Switch1":"ON","LD2410":{"Distance":[125.0,0.0,0.0],"Energy":[0,100]}}
      MqttPublishSensor();
    }
*/
  }
}

void Ld2410EverySecond(void) {
  if (LD2410.moving_energy) {
    // Send state change to be captured by rules
    // {"Time":"2022-11-26T10:48:16","Switch1":"ON","LD2410":{"Distance":[125.0,0.0,0.0],"Energy":[0,100]}}
    MqttPublishSensor();
  }
}

void Ld2410Detect(void) {
  if (PinUsed(GPIO_LD2410_RX) && PinUsed(GPIO_LD2410_TX)) {
    LD2410.buffer = (uint8_t*)malloc(TM_SERIAL_BUFFER_SIZE);
    if (!LD2410.buffer) { return; }
    LD2410Serial = new TasmotaSerial(Pin(GPIO_LD2410_RX), Pin(GPIO_LD2410_TX), 2);  // Default TM_SERIAL_BUFFER_SIZE (=64) size
    if (LD2410Serial->begin(256000)) {
      if (LD2410Serial->hardwareSerial()) { ClaimSerial(); }

      LD2410.init_retry = 5;
      LD2410.init_step = 13;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/




/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_LD2410_CM[] PROGMEM =
  "{s}LD2410 " D_MOVING_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}"
  "{s}LD2410 " D_STATIC_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}"
  "{s}LD2410 " D_DETECT_DISTANCE "{m}%1_f " D_UNIT_CENTIMETER "{e}";
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
      case FUNC_EVERY_250_MSECOND:
        Ld2410Every250MSecond();
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
    }
  }
  return result;
}

#endif  // USE_LD2410
