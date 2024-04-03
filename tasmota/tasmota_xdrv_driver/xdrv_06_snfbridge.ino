/*
  xdrv_06_snfbridge.ino - sonoff RF bridge 433 support for Tasmota

  Copyright (C) 2021  Theo Arends and Erik Andrén Zachrisson (fw update)

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

#ifdef USE_SONOFF_RF
/*********************************************************************************************\
  Sonoff RF Bridge 433
\*********************************************************************************************/

#define XDRV_06                   6

const uint32_t SFB_TIME_AVOID_DUPLICATE = 2000;  // Milliseconds

enum SonoffBridgeCommands {
  CMND_RFSYNC, CMND_RFLOW, CMND_RFHIGH, CMND_RFHOST, CMND_RFCODE };

const char kSonoffBridgeCommands[] PROGMEM = "|"  // No prefix
  D_CMND_RFSYNC "|" D_CMND_RFLOW "|" D_CMND_RFHIGH "|" D_CMND_RFHOST "|" D_CMND_RFCODE "|" D_CMND_RFKEY "|" D_CMND_RFRAW;

void (* const SonoffBridgeCommand[])(void) PROGMEM = {
  &CmndRfBridge, &CmndRfBridge, &CmndRfBridge, &CmndRfBridge, &CmndRfBridge, &CmndRfKey, &CmndRfRaw };

struct SONOFFBRIDGE {
  uint32_t last_received_id = 0;
  uint32_t last_send_code = 0;
  uint32_t last_time = 0;
  uint32_t last_learn_time = 0;
  uint8_t receive_flag = 0;
  uint8_t receive_raw_flag = 0;
  uint8_t learn_key = 1;
  uint8_t learn_active = 0;
  uint8_t expected_bytes = 0;
} SnfBridge;

#ifdef USE_RF_FLASH
/*********************************************************************************************\
 * EFM8BB1 RF microcontroller in-situ firmware update
 *
 * Enables upload of EFM8BB1 firmware provided by https://github.com/Portisch/RF-Bridge-EFM8BB1 using the web gui.
 * Based on source by Erik Andrén Zachrisson (https://github.com/arendst/Tasmota/pull/2886)
\*********************************************************************************************/

#include "ihx.h"
#include "c2.h"

uint32_t rf_decode_and_write(uint8_t *record, size_t size) {
  uint8_t err = ihx_decode(record, size);
  if (err != IHX_SUCCESS) { return 13; }  // Failed to decode RF firmware

  ihx_t *h = (ihx_t *) record;
  if (h->record_type == IHX_RT_DATA) {
    int retries = 5;
    uint16_t address = h->address_high * 0x100 + h->address_low;

    do {
      err = c2_programming_init(C2_DEVID_EFM8BB1);
      err = c2_block_write(address, h->data, h->len);
    } while (err != C2_SUCCESS && retries--);
  } else if (h->record_type == IHX_RT_END_OF_FILE) {
    // RF firmware upgrade done, restarting RF chip
    err = c2_reset();
  }

  if (err != C2_SUCCESS) { return 12; }  // Failed to write to RF chip

  return 0;
}

uint32_t rf_search_and_write(uint8_t *data, size_t size) {
  // Binary contains a set of commands, decode and program each one
  uint8_t buf[64];
  uint8_t* p_data = data;
  uint32_t addr = 0;
  uint32_t rec_end;
  uint32_t rec_start;
  uint32_t rec_size;
  uint32_t err;

  while (addr < size) {
    memcpy(buf, p_data, sizeof(buf));  // Must load flash using memcpy on 4-byte boundary

    // Find starts and ends of commands
    for (rec_start = 0; rec_start < 8; rec_start++) {
      if (':' == buf[rec_start]) { break; }
    }
    if (rec_start > 7) { return 8; }  // File invalid - RF Remnant data did not start with a start token
    for (rec_end = rec_start; rec_end < sizeof(buf); rec_end++) {
      if ('\n' == buf[rec_end]) { break; }
    }
    if (rec_end == sizeof(buf)) { return 9; }  // File too large - Failed to decode RF firmware
    rec_size = rec_end - rec_start;

//    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: %*_H"), rec_size, (uint8_t*)&buf + rec_start);

    err = rf_decode_and_write(buf + rec_start, rec_size);
    if (err != 0) { return err; }

    addr += rec_size +1;
    p_data += (rec_end & 0xFFFC);  // Stay on 4-byte boundary
    delay(0);
  }
  // Buffer was perfectly aligned, start and end found without any remaining trailing characters
  return 0;
}

uint8_t rf_erase_flash(void) {
  uint8_t err;

  for (uint32_t i = 0; i < 4; i++) {  // HACK: Try multiple times as the command sometimes fails (unclear why)
    err = c2_programming_init(C2_DEVID_EFM8BB1);
    if (err != C2_SUCCESS) {
      return 10;                 // Failed to init RF chip
    }
    err = c2_device_erase();
    if (err != C2_SUCCESS) {

//      AddLog(LOG_LEVEL_DEBUG, PSTR("RFB: Device erase error %d"), err);

      if (i < 3) {
        c2_reset();              // Reset RF chip and try again
      } else {
        return 11;               // Failed to erase RF chip
      }
    } else {
      break;
    }
  }
  return 0;
}

uint32_t SnfBrUpdateFirmware(uint8_t* data, uint32_t size) {
  pinMode(PIN_C2CK, OUTPUT);
  pinMode(PIN_C2D, INPUT);

  uint32_t error = rf_erase_flash();  // 10, 11
  if (error) { return error; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("RFB: Erased"));

  return rf_search_and_write(data, size);
}
#endif  // USE_RF_FLASH

/********************************************************************************************/

void SonoffBridgeReceivedRaw(void)
{
  // Decoding according to https://github.com/Portisch/RF-Bridge-EFM8BB1
  uint8_t buckets = 0;

  if (0xB1 == TasmotaGlobal.serial_in_buffer[1]) { buckets = TasmotaGlobal.serial_in_buffer[2] << 1; }  // Bucket sniffing

  ResponseTime_P(PSTR(",\"" D_CMND_RFRAW "\":{\"" D_JSON_DATA "\":\""));
  for (uint32_t i = 0; i < TasmotaGlobal.serial_in_byte_counter; i++) {
    ResponseAppend_P(PSTR("%02X"), TasmotaGlobal.serial_in_buffer[i]);
    if (0xB1 == TasmotaGlobal.serial_in_buffer[1]) {
      if ((i > 3) && buckets) { buckets--; }
      if ((i < 3) || (buckets % 2) || (i == TasmotaGlobal.serial_in_byte_counter -2)) {
        ResponseAppend_P(PSTR(" "));
      }
    }
  }
  ResponseAppend_P(PSTR("\"}}"));
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_CMND_RFRAW));
}

/********************************************************************************************/

void SonoffBridgeLearnFailed(void)
{
  SnfBridge.learn_active = 0;
  Response_P(S_JSON_COMMAND_INDEX_SVALUE, D_CMND_RFKEY, SnfBridge.learn_key, D_JSON_LEARN_FAILED);
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_RFKEY));
}

void SonoffBridgeReceived(void)
{
  uint16_t sync_time = 0;
  uint16_t low_time = 0;
  uint16_t high_time = 0;
  uint32_t received_id = 0;
  char rfkey[8];
  char stemp[16];

  AddLogSerial();

  if (0xA2 == TasmotaGlobal.serial_in_buffer[0]) {       // Learn timeout
    SonoffBridgeLearnFailed();
  }
  else if (0xA3 == TasmotaGlobal.serial_in_buffer[0]) {  // Learned A3 20 F8 01 18 03 3E 2E 1A 22 55
    SnfBridge.learn_active = 0;
    low_time = TasmotaGlobal.serial_in_buffer[3] << 8 | TasmotaGlobal.serial_in_buffer[4];   // Low time in uSec
    high_time = TasmotaGlobal.serial_in_buffer[5] << 8 | TasmotaGlobal.serial_in_buffer[6];  // High time in uSec
    if (low_time && high_time) {
      for (uint32_t i = 0; i < 9; i++) {
        Settings->rf_code[SnfBridge.learn_key][i] = TasmotaGlobal.serial_in_buffer[i +1];
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, D_CMND_RFKEY, SnfBridge.learn_key, D_JSON_LEARNED);
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_RFKEY));
    } else {
      SonoffBridgeLearnFailed();
    }
  }
  else if (0xA4 == TasmotaGlobal.serial_in_buffer[0]) {  // Received RF data A4 20 EE 01 18 03 3E 2E 1A 22 55
    if (SnfBridge.learn_active) {
      SonoffBridgeLearnFailed();
    } else {
      sync_time = TasmotaGlobal.serial_in_buffer[1] << 8 | TasmotaGlobal.serial_in_buffer[2];  // Sync time in uSec
      low_time = TasmotaGlobal.serial_in_buffer[3] << 8 | TasmotaGlobal.serial_in_buffer[4];   // Low time in uSec
      high_time = TasmotaGlobal.serial_in_buffer[5] << 8 | TasmotaGlobal.serial_in_buffer[6];  // High time in uSec
      received_id = TasmotaGlobal.serial_in_buffer[7] << 16 | TasmotaGlobal.serial_in_buffer[8] << 8 | TasmotaGlobal.serial_in_buffer[9];

      unsigned long now = millis();
      if (!((received_id == SnfBridge.last_received_id) && (now - SnfBridge.last_time < SFB_TIME_AVOID_DUPLICATE))) {
        SnfBridge.last_received_id = received_id;
        SnfBridge.last_time = now;
        strncpy_P(rfkey, PSTR("\"" D_JSON_NONE "\""), sizeof(rfkey));
        for (uint32_t i = 1; i <= 16; i++) {
          if (Settings->rf_code[i][0]) {
            uint32_t send_id = Settings->rf_code[i][6] << 16 | Settings->rf_code[i][7] << 8 | Settings->rf_code[i][8];
            if (send_id == received_id) {
              snprintf_P(rfkey, sizeof(rfkey), PSTR("%d"), i);
              break;
            }
          }
        }
        if (Settings->flag.rf_receive_decimal) {  // SetOption28 - RF receive data format
          snprintf_P(stemp, sizeof(stemp), PSTR("%u"), received_id);
        } else {
          snprintf_P(stemp, sizeof(stemp), PSTR("\"%06X\""), received_id);
        }
        ResponseTime_P(PSTR(",\"" D_JSON_RFRECEIVED "\":{\"" D_JSON_SYNC "\":%d,\"" D_JSON_LOW "\":%d,\"" D_JSON_HIGH "\":%d,\"" D_JSON_DATA "\":%s,\"" D_CMND_RFKEY "\":%s}}"),
          sync_time, low_time, high_time, stemp, rfkey);
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_RFRECEIVED));
  #ifdef USE_DOMOTICZ
        DomoticzSensor(DZ_COUNT, received_id);  // Send rid as Domoticz Counter value
  #endif  // USE_DOMOTICZ
      }
    }
  }
}

bool SonoffBridgeSerialInput(void)
{
  // iTead Rf Universal Transceiver Module Serial Protocol Version 1.0 (20170420)
  static int8_t receive_len = 0;

  if (SnfBridge.receive_flag) {
    if (SnfBridge.receive_raw_flag) {
      if (!TasmotaGlobal.serial_in_byte_counter) {
        TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = 0xAA;
      }
      TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = TasmotaGlobal.serial_in_byte;
      if (TasmotaGlobal.serial_in_byte_counter == 3) {
        if ((0xA6 == TasmotaGlobal.serial_in_buffer[1]) || (0xAB == TasmotaGlobal.serial_in_buffer[1])) {  // AA A6 06 023908010155 55 - 06 is receive_len
          receive_len = TasmotaGlobal.serial_in_buffer[2] + 4;  // Get at least receive_len bytes
        }
      }
      if ((!receive_len && (0x55 == TasmotaGlobal.serial_in_byte)) || (receive_len && (TasmotaGlobal.serial_in_byte_counter == receive_len))) {  // 0x55 - End of text
        SonoffBridgeReceivedRaw();
        SnfBridge.receive_flag = 0;
        return 1;
      }
    }
    else if (!((0 == TasmotaGlobal.serial_in_byte_counter) && (0 == TasmotaGlobal.serial_in_byte))) {  // Skip leading 0
      if (0 == TasmotaGlobal.serial_in_byte_counter) {
        SnfBridge.expected_bytes = 2;     // 0xA0, 0xA1, 0xA2
        if (TasmotaGlobal.serial_in_byte >= 0xA3) {
          SnfBridge.expected_bytes = 11;  // 0xA3, 0xA4, 0xA5
        }
        if (TasmotaGlobal.serial_in_byte == 0xA6) {
          SnfBridge.expected_bytes = 0;   // 0xA6 and up supported by Portisch firmware only
          TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = 0xAA;
          SnfBridge.receive_raw_flag = 1;
        }
      }
      TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = TasmotaGlobal.serial_in_byte;
      if ((SnfBridge.expected_bytes == TasmotaGlobal.serial_in_byte_counter) && (0x55 == TasmotaGlobal.serial_in_byte)) {  // 0x55 - End of text
        SonoffBridgeReceived();
        SnfBridge.receive_flag = 0;
        return 1;
      }
    }
    TasmotaGlobal.serial_in_byte = 0;
  }
  if (0xAA == TasmotaGlobal.serial_in_byte) {               // 0xAA - Start of text
    TasmotaGlobal.serial_in_byte_counter = 0;
    TasmotaGlobal.serial_in_byte = 0;
    SnfBridge.receive_flag = 1;
    receive_len = 0;
  }
  return 0;
}

void SonoffBridgeSendCommand(uint8_t code)
{
  Serial.write(0xAA);  // Start of Text
  Serial.write(code);  // Command or Acknowledge
  Serial.write(0x55);  // End of Text
}

void SonoffBridgeSendAck(void)
{
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA0);  // Acknowledge
  Serial.write(0x55);  // End of Text
}

void SonoffBridgeSendCode(uint32_t code)
{
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA5);  // Send following code
  for (uint32_t i = 0; i < 6; i++) {
    Serial.write(Settings->rf_code[0][i]);
  }
  Serial.write((code >> 16) & 0xff);
  Serial.write((code >> 8) & 0xff);
  Serial.write(code & 0xff);
  Serial.write(0x55);  // End of Text
  Serial.flush();
}

void SonoffBridgeSend(uint8_t idx, uint8_t key)
{
  uint8_t code;

  key--;               // Support 1 to 16
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA5);  // Send following code
  for (uint32_t i = 0; i < 8; i++) {
    Serial.write(Settings->rf_code[idx][i]);
  }
  if (0 == idx) {
    code = (0x10 << (key >> 2)) | (1 << (key & 3));  // 11,12,14,18,21,22,24,28,41,42,44,48,81,82,84,88
  } else {
    code = Settings->rf_code[idx][8];
  }
  Serial.write(code);
  Serial.write(0x55);  // End of Text
  Serial.flush();
#ifdef USE_DOMOTICZ
//  uint32_t rid = Settings->rf_code[idx][6] << 16 | Settings->rf_code[idx][7] << 8 | code;
//  DomoticzSensor(DZ_COUNT, rid);  // Send rid as Domoticz Counter value
#endif  // USE_DOMOTICZ
}

void SonoffBridgeLearn(uint8_t key)
{
  SnfBridge.learn_key = key;
  SnfBridge.learn_active = 1;
  SnfBridge.last_learn_time = millis();
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA1);  // Start learning
  Serial.write(0x55);  // End of Text
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndRfBridge(void)  // RfSync, RfLow, RfHigh, RfHost and RfCode
{
  char *p;
  char stemp [10];
  uint32_t code = 0;
  uint8_t radix = 10;

  uint32_t set_index = XdrvMailbox.command_code *2;

  if (XdrvMailbox.data[0] == '#') {
    XdrvMailbox.data++;
    XdrvMailbox.data_len--;
    radix = 16;
  }

  if (XdrvMailbox.data_len) {
    code = strtol(XdrvMailbox.data, &p, radix);
    if (code) {
      if (CMND_RFCODE == XdrvMailbox.command_code) {
        SnfBridge.last_send_code = code;
        SonoffBridgeSendCode(code);
      } else {
        if (1 == XdrvMailbox.payload) {
          code = pgm_read_byte(kDefaultRfCode + set_index) << 8 | pgm_read_byte(kDefaultRfCode + set_index +1);
        }
        uint8_t msb = code >> 8;
        uint8_t lsb = code & 0xFF;
        if ((code > 0) && (code < 0x7FFF) && (msb != 0x55) && (lsb != 0x55)) {  // Check for End of Text codes
          Settings->rf_code[0][set_index] = msb;
          Settings->rf_code[0][set_index +1] = lsb;
        }
      }
    }
  }
  if (CMND_RFCODE == XdrvMailbox.command_code) {
    code = SnfBridge.last_send_code;
  } else {
    code = Settings->rf_code[0][set_index] << 8 | Settings->rf_code[0][set_index +1];
  }
  if (10 == radix) {
    snprintf_P(stemp, sizeof(stemp), PSTR("%d"), code);
  } else {
    snprintf_P(stemp, sizeof(stemp), PSTR("\"#%06X\""), code);
  }
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, stemp);
}

void CmndRfKey(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 16)) {
    unsigned long now = millis();
    if ((!SnfBridge.learn_active) || (now - SnfBridge.last_learn_time > 60100)) {
      SnfBridge.learn_active = 0;
      if (2 == XdrvMailbox.payload) {              // Learn RF data
        SonoffBridgeLearn(XdrvMailbox.index);
        ResponseCmndIdxChar(PSTR(D_JSON_START_LEARNING));
      }
      else if (3 == XdrvMailbox.payload) {         // Unlearn RF data
        Settings->rf_code[XdrvMailbox.index][0] = 0;  // Reset sync_time MSB
        ResponseCmndIdxChar(PSTR(D_JSON_SET_TO_DEFAULT));
      }
      else if (4 == XdrvMailbox.payload) {         // Save RF data provided by RFSync, RfLow, RfHigh and last RfCode
        for (uint32_t i = 0; i < 6; i++) {
          Settings->rf_code[XdrvMailbox.index][i] = Settings->rf_code[0][i];
        }
        Settings->rf_code[XdrvMailbox.index][6] = (SnfBridge.last_send_code >> 16) & 0xff;
        Settings->rf_code[XdrvMailbox.index][7] = (SnfBridge.last_send_code >> 8) & 0xff;
        Settings->rf_code[XdrvMailbox.index][8] = SnfBridge.last_send_code & 0xff;
        ResponseCmndIdxChar(PSTR(D_JSON_SAVED));
      } else if (5 == XdrvMailbox.payload) {      // Show default or learned RF data
        uint8_t key = XdrvMailbox.index;
        uint8_t index = (0 == Settings->rf_code[key][0]) ? 0 : key;  // Use default if sync_time MSB = 0
        uint16_t sync_time = (Settings->rf_code[index][0] << 8) | Settings->rf_code[index][1];
        uint16_t low_time = (Settings->rf_code[index][2] << 8) | Settings->rf_code[index][3];
        uint16_t high_time = (Settings->rf_code[index][4] << 8) | Settings->rf_code[index][5];
        uint32_t code = (Settings->rf_code[index][6] << 16) | (Settings->rf_code[index][7] << 8);
        if (0 == index) {
          key--;
          code |= (uint8_t)((0x10 << (key >> 2)) | (1 << (key & 3)));
        } else {
          code |= Settings->rf_code[index][8];
        }
        Response_P(PSTR("{\"%s%d\":{\"" D_JSON_SYNC "\":%d,\"" D_JSON_LOW "\":%d,\"" D_JSON_HIGH "\":%d,\"" D_JSON_DATA "\":\"%06X\"}}"),
                   XdrvMailbox.command, XdrvMailbox.index, sync_time, low_time, high_time, code);
      } else {
        if ((1 == XdrvMailbox.payload) || (0 == Settings->rf_code[XdrvMailbox.index][0])) {  // Test sync_time MSB
          SonoffBridgeSend(0, XdrvMailbox.index);  // Send default RF data
          ResponseCmndIdxChar(PSTR(D_JSON_DEFAULT_SENT));
        } else {
          SonoffBridgeSend(XdrvMailbox.index, 0);  // Send learned RF data
          ResponseCmndIdxChar(PSTR(D_JSON_LEARNED_SENT));
        }
      }
    } else {
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, XdrvMailbox.command, SnfBridge.learn_key, D_JSON_LEARNING_ACTIVE);
    }
  }
}

void CmndRfRaw(void)
{
  if (XdrvMailbox.data_len) {
    if (XdrvMailbox.data_len < 6) {  // On, Off
      switch (XdrvMailbox.payload) {
      case 0:    // Receive Raw Off
        SonoffBridgeSendCommand(0xA7);  // Stop reading RF signals enabling iTead default RF handling
      case 1:    // Receive Raw On
        SnfBridge.receive_raw_flag = XdrvMailbox.payload;
        break;
      case 166:  // 0xA6 - Start reading RF signals disabling iTead default RF handling
      case 167:  // 0xA7 - Stop reading RF signals enabling iTead default RF handling
      case 169:  // 0xA9 - Start learning predefined protocols
      case 176:  // 0xB0 - Stop sniffing
      case 177:  // 0xB1 - Start sniffing
      case 255:  // 0xFF - Show firmware version
        SonoffBridgeSendCommand(XdrvMailbox.payload);
        SnfBridge.receive_raw_flag = 1;
        break;
      case 192:  // 0xC0 - Beep
        char beep[] = "AAC000C055\0";
        SerialSendRaw(beep);
        break;
      }
    } else {
      SerialSendRaw(RemoveSpace(XdrvMailbox.data));
      SnfBridge.receive_raw_flag = 1;
    }
  }
  ResponseCmndStateText(SnfBridge.receive_raw_flag);
}

#ifdef USE_WEBSERVER

void SonoffBridgeAddButton(void) {
  WSContentSend_P(HTTP_TABLE100);
  WSContentSend_P(PSTR("<tr>"));
  char number[4];
  uint32_t idx = 0;
  for (uint32_t i = 0; i < 4; i++) {
    if (idx > 0) { WSContentSend_P(PSTR("</tr><tr>")); }
    for (uint32_t j = 0; j < 4; j++) {
      idx++;
      WSContentSend_P(PSTR("<td style='width:25%%'><button onclick='la(\"&k=%d\");'>%s</button></td>"), idx,  // &k is related to WebGetArg("k", tmp, sizeof(tmp));
        (strlen(GetWebButton(idx -1))) ? HtmlEscape(GetWebButton(idx -1)).c_str() : itoa(idx, number, 10));
    }
  }
  WSContentSend_P(PSTR("</tr></table>"));
}

void SonoffBridgeWebGetArg(void) {
  char tmp[8];                             // WebGetArg numbers only
  WebGetArg(PSTR("k"), tmp, sizeof(tmp));  // 1 - 16 Pre defined RF keys
  if (strlen(tmp)) {
    char command[20];
    snprintf_P(command, sizeof(command), PSTR(D_CMND_RFKEY "%s"), tmp);
    ExecuteWebCommand(command);
  }
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv06(uint32_t function)
{
  bool result = false;

#ifdef ESP8266
  if (SONOFF_BRIDGE == TasmotaGlobal.module_type) {
    switch (function) {
      case FUNC_SERIAL:
        result = SonoffBridgeSerialInput();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kSonoffBridgeCommands, SonoffBridgeCommand);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_MAIN_BUTTON:
        SonoffBridgeAddButton();
        break;
      case FUNC_WEB_GET_ARG:
        SonoffBridgeWebGetArg();
        break;
#endif  // USE_WEBSERVER
      case FUNC_INIT:
        SnfBridge.receive_raw_flag = 0;
        SonoffBridgeSendCommand(0xA7);  // Stop reading RF signals enabling iTead default RF handling
        break;
      case FUNC_PRE_INIT:
        SetSerial(19200, TS_SERIAL_8N1);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
#endif  // ESP8266
  return result;
}

#endif  // USE_SONOFF_RF
