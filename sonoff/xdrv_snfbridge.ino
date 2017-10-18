/*
  xdrv_snfbridge.ino - sonoff RF bridge 433 support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

uint8_t sonoff_bridge_receive_flag = 0;
uint8_t sonoff_bridge_learn_key = 1;
uint8_t sonoff_bridge_learn_active = 0;
uint32_t sonoff_bridge_last_received_id = 0;
unsigned long sonoff_bridge_last_time = 0;

void SonoffBridgeReceived()
{
  uint16_t sync_time = 0;
  uint16_t low_time = 0;
  uint16_t high_time = 0;
  uint32_t received_id = 0;
  char svalue[90];
  char rfkey[8];

  svalue[0] = '\0';
  for (byte i = 0; i < serial_in_byte_counter; i++) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%02X "), svalue, serial_in_buffer[i]);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_BRIDGE D_RECEIVED " %s"), svalue);
  AddLog(LOG_LEVEL_DEBUG);

  if (0xA2 == serial_in_buffer[0]) {       // Learn timeout
    sonoff_bridge_learn_active = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARN_FAILED "\"}"), sonoff_bridge_learn_key);
    MqttPublishPrefixTopic_P(5, PSTR(D_CMND_RFKEY));
  }
  else if (0xA3 == serial_in_buffer[0]) {  // Learned A3 20 F8 01 18 03 3E 2E 1A 22 55
    sonoff_bridge_learn_active = 0;
    low_time = serial_in_buffer[3] << 8 | serial_in_buffer[4];  // Low time in uSec
    high_time = serial_in_buffer[5] << 8 | serial_in_buffer[6];  // High time in uSec
    if (low_time && high_time) {
      for (byte i = 0; i < 9; i++) {
        Settings.rf_code[sonoff_bridge_learn_key][i] = serial_in_buffer[i +1];
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARNED "\"}"), sonoff_bridge_learn_key);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARN_FAILED "\"}"), sonoff_bridge_learn_key);
    }
    MqttPublishPrefixTopic_P(5, PSTR(D_CMND_RFKEY));
  }
  else if (0xA4 == serial_in_buffer[0]) {  // Received RF data A4 20 EE 01 18 03 3E 2E 1A 22 55
    sync_time = serial_in_buffer[1] << 8 | serial_in_buffer[2];  // Sync time in uSec
    low_time = serial_in_buffer[3] << 8 | serial_in_buffer[4];  // Low time in uSec
    high_time = serial_in_buffer[5] << 8 | serial_in_buffer[6];  // High time in uSec
    received_id = serial_in_buffer[7] << 16 | serial_in_buffer[8] << 8 | serial_in_buffer[9];

    unsigned long now = millis();
    if (!((received_id == sonoff_bridge_last_received_id) && (now - sonoff_bridge_last_time < SFB_TIME_AVOID_DUPLICATE))) {
      sonoff_bridge_last_received_id = received_id;
      sonoff_bridge_last_time = now;
      strncpy_P(rfkey, PSTR("\"" D_NONE "\""), sizeof(rfkey));
      for (byte i = 1; i <= 16; i++) {
        if (Settings.rf_code[i][0]) {
          uint32_t send_id = Settings.rf_code[i][6] << 16 | Settings.rf_code[i][7] << 8 | Settings.rf_code[i][8];
          if (send_id == received_id) {
            snprintf_P(rfkey, sizeof(rfkey), PSTR("%d"), i);
            break;
          }
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_RFRECEIVED "\":{\"" D_SYNC "\":%d, \"" D_LOW "\":%d, \"" D_HIGH "\":%d, \"" D_DATA "\":\"%06X\", \"" D_CMND_RFKEY "\":%s}}"),
        sync_time, low_time, high_time, received_id, rfkey);
      MqttPublishPrefixTopic_P(6, PSTR(D_RFRECEIVED));
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_COUNT, received_id);  // Send rid as Domoticz Counter value
#endif  // USE_DOMOTICZ
    }
  }
}

boolean SonoffBridgeSerialInput()
{
  if (sonoff_bridge_receive_flag) {
    if (!((serial_in_byte_counter == 0) && (serial_in_byte == 0))) {  // Skip leading 0
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
      if (0x55 == serial_in_byte) {          // 0x55 - End of text
        SonoffBridgeReceived();
        sonoff_bridge_receive_flag = 0;
        return 1;
      }
    }
    serial_in_byte = 0;
  }
  if (0xAA == serial_in_byte) {              // 0xAA - Start of text
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

void SonoffBridgeSend(uint8_t idx, uint8_t key)
{
  uint8_t code;

  key--;               // Support 1 to 16
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA5);  // Send following code
  for (uint8_t i = 0; i < 8; i++) {
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
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA1);  // Start learning
  Serial.write(0x55);  // End of Text
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean SonoffBridgeCommand(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  boolean serviced = true;
  char *p;

  if (!strcasecmp_P(type, PSTR(D_CMND_RFDEFAULT))) {
    if (4 == data_len) {
      uint16_t hexcode = strtol(dataBuf, &p, 16);
      uint8_t msb = hexcode >> 8;
      uint8_t lsb = hexcode & 0xFF;
      if ((hexcode > 0) && (hexcode < 0x7FFF) && (msb != 0x55) && (lsb != 0x55)) {
        Settings.rf_code[0][6] = msb;
        Settings.rf_code[0][7] = lsb;
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFDEFAULT "\":\"%0X%0X\"}"), Settings.rf_code[0][6], Settings.rf_code[0][7]);
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_RFKEY)) && (index > 0) && (index <= 16)) {
    if (!sonoff_bridge_learn_active) {
      if (2 == payload) {
        SonoffBridgeLearn(index);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_START_LEARNING "\"}"), index);
      }
      else if (3 == payload) {
        Settings.rf_code[index][0] = 0;
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_SET_TO_DEFAULT "\"}"), index);
      } else {
        if ((1 == payload) || (0 == Settings.rf_code[index][0])) {
          SonoffBridgeSend(0, index);
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_DEFAULT_SENT "\"}"), index);
        } else {
          SonoffBridgeSend(index, 0);
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARNED_SENT "\"}"), index);
        }
      }
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARNING_ACTIVE "\"}"), sonoff_bridge_learn_key);
    }
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}
