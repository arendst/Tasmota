/*
  xdrv_04_snfbridge.ino - sonoff RF bridge 433 support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

/*********************************************************************************************\
  Sonoff RF Bridge 433
\*********************************************************************************************/

#define SFB_TIME_AVOID_DUPLICATE  2000  // Milliseconds

enum SonoffBridgeCommands {
    CMND_RFSYNC,      CMND_RFLOW,      CMND_RFHIGH,      CMND_RFHOST,      CMND_RFCODE,      CMND_RFKEY };
const char kSonoffBridgeCommands[] PROGMEM =
  D_CMND_RFSYNC "|" D_CMND_RFLOW "|" D_CMND_RFHIGH "|" D_CMND_RFHOST "|" D_CMND_RFCODE "|" D_CMND_RFKEY ;

uint8_t sonoff_bridge_receive_flag = 0;
uint8_t sonoff_bridge_learn_key = 1;
uint8_t sonoff_bridge_learn_active = 0;
uint8_t sonoff_bridge_expected_bytes = 0;
uint32_t sonoff_bridge_last_received_id = 0;
uint32_t sonoff_bridge_last_send_code = 0;
unsigned long sonoff_bridge_last_time = 0;
unsigned long sonoff_bridge_last_learn_time = 0;

void SonoffBridgeLearnFailed()
{
  sonoff_bridge_learn_active = 0;
  snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_RFKEY, sonoff_bridge_learn_key, D_JSON_LEARN_FAILED);
  MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_RFKEY));
}

void SonoffBridgeReceived()
{
  uint16_t sync_time = 0;
  uint16_t low_time = 0;
  uint16_t high_time = 0;
  uint32_t received_id = 0;
  char rfkey[8];

  AddLogSerial(LOG_LEVEL_DEBUG);

  if (0xA2 == serial_in_buffer[0]) {       // Learn timeout
    SonoffBridgeLearnFailed();
  }
  else if (0xA3 == serial_in_buffer[0]) {  // Learned A3 20 F8 01 18 03 3E 2E 1A 22 55
    sonoff_bridge_learn_active = 0;
    low_time = serial_in_buffer[3] << 8 | serial_in_buffer[4];   // Low time in uSec
    high_time = serial_in_buffer[5] << 8 | serial_in_buffer[6];  // High time in uSec
    if (low_time && high_time) {
      for (byte i = 0; i < 9; i++) {
        Settings.rf_code[sonoff_bridge_learn_key][i] = serial_in_buffer[i +1];
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_RFKEY, sonoff_bridge_learn_key, D_JSON_LEARNED);
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_RFKEY));
    } else {
      SonoffBridgeLearnFailed();
    }
  }
  else if (0xA4 == serial_in_buffer[0]) {  // Received RF data A4 20 EE 01 18 03 3E 2E 1A 22 55
    if (sonoff_bridge_learn_active) {
      SonoffBridgeLearnFailed();
    } else {
      sync_time = serial_in_buffer[1] << 8 | serial_in_buffer[2];  // Sync time in uSec
      low_time = serial_in_buffer[3] << 8 | serial_in_buffer[4];   // Low time in uSec
      high_time = serial_in_buffer[5] << 8 | serial_in_buffer[6];  // High time in uSec
      received_id = serial_in_buffer[7] << 16 | serial_in_buffer[8] << 8 | serial_in_buffer[9];

      unsigned long now = millis();
      if (!((received_id == sonoff_bridge_last_received_id) && (now - sonoff_bridge_last_time < SFB_TIME_AVOID_DUPLICATE))) {
        sonoff_bridge_last_received_id = received_id;
        sonoff_bridge_last_time = now;
        strncpy_P(rfkey, PSTR("\"" D_JSON_NONE "\""), sizeof(rfkey));
        for (byte i = 1; i <= 16; i++) {
          if (Settings.rf_code[i][0]) {
            uint32_t send_id = Settings.rf_code[i][6] << 16 | Settings.rf_code[i][7] << 8 | Settings.rf_code[i][8];
            if (send_id == received_id) {
              snprintf_P(rfkey, sizeof(rfkey), PSTR("%d"), i);
              break;
            }
          }
        }
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_RFRECEIVED "\":{\"" D_JSON_SYNC "\":%d,\"" D_JSON_LOW "\":%d,\"" D_JSON_HIGH "\":%d,\"" D_JSON_DATA "\":\"%06X\",\"" D_CMND_RFKEY "\":%s}}"),
          sync_time, low_time, high_time, received_id, rfkey);
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_RFRECEIVED));
  #ifdef USE_DOMOTICZ
        DomoticzSensor(DZ_COUNT, received_id);  // Send rid as Domoticz Counter value
  #endif  // USE_DOMOTICZ
      }
    }
  }
}

boolean SonoffBridgeSerialInput()
{
  // iTead Rf Universal Transceiver Module Serial Protocol Version 1.0 (20170420)
  if (sonoff_bridge_receive_flag) {
    if (!((serial_in_byte_counter == 0) && (serial_in_byte == 0))) {  // Skip leading 0
      if (!serial_in_byte_counter) {
        sonoff_bridge_expected_bytes = 2;     // 0xA0, 0xA1, 0xA2
        if (serial_in_byte >= 0xA3) {
          sonoff_bridge_expected_bytes = 11;  // 0xA3, 0xA4, 0xA5
        }
      }
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
      if  ((sonoff_bridge_expected_bytes == serial_in_byte_counter) && (0x55 == serial_in_byte)) {  // 0x55 - End of text
        SonoffBridgeReceived();
        sonoff_bridge_receive_flag = 0;
        return 1;
      }
    }
    serial_in_byte = 0;
  }
  if (0xAA == serial_in_byte) {               // 0xAA - Start of text
    serial_in_byte_counter = 0;
    serial_in_byte = 0;
    sonoff_bridge_receive_flag = 1;
  }
  return 0;
}

void SonoffBridgeSendAck()
{
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA0);  // Acknowledge
  Serial.write(0x55);  // End of Text
}

void SonoffBridgeSendCode(uint32_t code)
{
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA5);  // Send following code
  for (byte i = 0; i < 6; i++) {
    Serial.write(Settings.rf_code[0][i]);
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
  for (byte i = 0; i < 8; i++) {
    Serial.write(Settings.rf_code[idx][i]);
  }
  if (0 == idx) {
    code = (0x10 << (key >> 2)) | (1 << (key & 3));  // 11,12,14,18,21,22,24,28,41,42,44,48,81,82,84,88
  } else {
    code = Settings.rf_code[idx][8];
  }
  Serial.write(code);
  Serial.write(0x55);  // End of Text
  Serial.flush();
#ifdef USE_DOMOTICZ
//  uint32_t rid = Settings.rf_code[idx][6] << 16 | Settings.rf_code[idx][7] << 8 | code;
//  DomoticzSensor(DZ_COUNT, rid);  // Send rid as Domoticz Counter value
#endif  // USE_DOMOTICZ
}

void SonoffBridgeLearn(uint8_t key)
{
  sonoff_bridge_learn_key = key;
  sonoff_bridge_learn_active = 1;
  sonoff_bridge_last_learn_time = millis();
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA1);  // Start learning
  Serial.write(0x55);  // End of Text
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean SonoffBridgeCommand()
{
  char command [CMDSZ];
  boolean serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kSonoffBridgeCommands);
  if ((command_code >= CMND_RFSYNC) && (command_code <= CMND_RFCODE)) {  // RfSync, RfLow, RfHigh, RfHost and RfCode
    char *p;
    char stemp [10];
    uint32_t code = 0;
    uint8_t radix = 10;

    uint8_t set_index = command_code *2;

    if (XdrvMailbox.data[0] == '#') {
      XdrvMailbox.data++;
      XdrvMailbox.data_len--;
      radix = 16;
    }

    if (XdrvMailbox.data_len) {
      code = strtol(XdrvMailbox.data, &p, radix);
      if (code) {
        if (CMND_RFCODE == command_code) {
          sonoff_bridge_last_send_code = code;
          SonoffBridgeSendCode(code);
        } else {
          if (1 == XdrvMailbox.payload) {
            code = pgm_read_byte(kDefaultRfCode + set_index) << 8 | pgm_read_byte(kDefaultRfCode + set_index +1);
          }
          uint8_t msb = code >> 8;
          uint8_t lsb = code & 0xFF;
          if ((code > 0) && (code < 0x7FFF) && (msb != 0x55) && (lsb != 0x55)) {  // Check for End of Text codes
            Settings.rf_code[0][set_index] = msb;
            Settings.rf_code[0][set_index +1] = lsb;
          }
        }
      }
    }
    if (CMND_RFCODE == command_code) {
      code = sonoff_bridge_last_send_code;
    } else {
      code = Settings.rf_code[0][set_index] << 8 | Settings.rf_code[0][set_index +1];
    }
    if (10 == radix) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%d"), code);
    } else {
      snprintf_P(stemp, sizeof(stemp), PSTR("\"#%X\""), code);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_XVALUE, command, stemp);
  }
  else if ((CMND_RFKEY == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 16)) {
    unsigned long now = millis();
    if ((!sonoff_bridge_learn_active) || (now - sonoff_bridge_last_learn_time > 60100)) {
      sonoff_bridge_learn_active = 0;
      if (2 == XdrvMailbox.payload) {              // Learn RF data
        SonoffBridgeLearn(XdrvMailbox.index);
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, D_JSON_START_LEARNING);
      }
      else if (3 == XdrvMailbox.payload) {         // Unlearn RF data
        Settings.rf_code[XdrvMailbox.index][0] = 0;
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, D_JSON_SET_TO_DEFAULT);
      }
      else if (4 == XdrvMailbox.payload) {         // Save RF data provided by RFSync, RfLow, RfHigh and last RfCode
        for (byte i = 0; i < 6; i++) {
          Settings.rf_code[XdrvMailbox.index][i] = Settings.rf_code[0][i];
        }
        Settings.rf_code[XdrvMailbox.index][6] = (sonoff_bridge_last_send_code >> 16) & 0xff;
        Settings.rf_code[XdrvMailbox.index][7] = (sonoff_bridge_last_send_code >> 8) & 0xff;
        Settings.rf_code[XdrvMailbox.index][8] = sonoff_bridge_last_send_code & 0xff;
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, D_JSON_SAVED);
      } else {
        if ((1 == XdrvMailbox.payload) || (0 == Settings.rf_code[XdrvMailbox.index][0])) {
          SonoffBridgeSend(0, XdrvMailbox.index);  // Send default RF data
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, D_JSON_DEFAULT_SENT);
        } else {
          SonoffBridgeSend(XdrvMailbox.index, 0);  // Send learned RF data
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, D_JSON_LEARNED_SENT);
        }
      }
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, sonoff_bridge_learn_key, D_JSON_LEARNING_ACTIVE);
    }
  } else serviced = false;
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_04

boolean Xdrv04(byte function)
{
  boolean result = false;

  if (SONOFF_BRIDGE == Settings.module) {
    switch (function) {
      case FUNC_COMMAND:
        result = SonoffBridgeCommand();
        break;
    }
  }
  return result;
}

