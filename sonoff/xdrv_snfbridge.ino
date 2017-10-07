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

uint8_t sfb_rcvflg = 0;
uint8_t sfb_learnKey = 1;
uint8_t sfb_learnFlg = 0;
uint32_t sfb_lastrid = 0;
unsigned long sfb_lasttime = 0;

void sb_received()
{
  uint8_t i = 0;
  uint32_t sid = 0;
  uint32_t rid = 0;
  uint16_t rsy = 0;
  uint16_t rlo = 0;
  uint16_t rhi = 0;
  char svalue[90];
  char rfkey[8];

  svalue[0] = '\0';
  for (i = 0; i < SerialInByteCounter; i++) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%02X "), svalue, serialInBuf[i]);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_BRIDGE D_RECEIVED " %s"), svalue);
  addLog(LOG_LEVEL_DEBUG);

  if (0xA2 == serialInBuf[0]) {       // Learn timeout
    sfb_learnFlg = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARN_FAILED "\"}"), sfb_learnKey);
    mqtt_publish_topic_P(5, PSTR(D_CMND_RFKEY));
  }
  else if (0xA3 == serialInBuf[0]) {  // Learned A3 20 F8 01 18 03 3E 2E 1A 22 55
    sfb_learnFlg = 0;
    rlo = serialInBuf[3] << 8 | serialInBuf[4];  // Low time in uSec
    rhi = serialInBuf[5] << 8 | serialInBuf[6];  // High time in uSec
    if (rlo && rhi) {
      for (i = 0; i < 9; i++) {
        sysCfg.sfb_code[sfb_learnKey][i] = serialInBuf[i +1];
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARNED "\"}"), sfb_learnKey);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARN_FAILED "\"}"), sfb_learnKey);
    }
    mqtt_publish_topic_P(5, PSTR(D_CMND_RFKEY));
  }
  else if (0xA4 == serialInBuf[0]) {  // Received RF data A4 20 EE 01 18 03 3E 2E 1A 22 55
    rsy = serialInBuf[1] << 8 | serialInBuf[2];  // Sync time in uSec
    rlo = serialInBuf[3] << 8 | serialInBuf[4];  // Low time in uSec
    rhi = serialInBuf[5] << 8 | serialInBuf[6];  // High time in uSec
    rid = serialInBuf[7] << 16 | serialInBuf[8] << 8 | serialInBuf[9];

    unsigned long now = millis();
    if (!((rid == sfb_lastrid) && (now - sfb_lasttime < SFB_TIME_AVOID_DUPLICATE))) {
      sfb_lastrid = rid;
      sfb_lasttime = now;
      strncpy_P(rfkey, PSTR("\"" D_NONE "\""), sizeof(rfkey));
      for (i = 1; i <= 16; i++) {
        if (sysCfg.sfb_code[i][0]) {
          sid = sysCfg.sfb_code[i][6] << 16 | sysCfg.sfb_code[i][7] << 8 | sysCfg.sfb_code[i][8];
          if (sid == rid) {
            snprintf_P(rfkey, sizeof(rfkey), PSTR("%d"), i);
            break;
          }
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_RFRECEIVED "\":{\"" D_SYNC "\":%d, \"" D_LOW "\":%d, \"" D_HIGH "\":%d, \"" D_DATA "\":\"%06X\", \"" D_CMND_RFKEY "\":%s}}"),
        rsy, rlo, rhi, rid, rfkey);
      mqtt_publish_topic_P(6, PSTR(D_RFRECEIVED));
    }
  }
}

boolean sb_serial()
{
  if (sfb_rcvflg) {
    if (!((SerialInByteCounter == 0) && (SerialInByte == 0))) {  // Skip leading 0
      serialInBuf[SerialInByteCounter++] = SerialInByte;
      if (0x55 == SerialInByte) {          // 0x55 - End of text
        sb_received();
        sfb_rcvflg = 0;
        return 1;
      }
    }
    SerialInByte = 0;
  }
  if (0xAA == SerialInByte) {              // 0xAA - Start of text
    SerialInByteCounter = 0;
    SerialInByte = 0;
    sfb_rcvflg = 1;
  }
  return 0;
}

void sb_sendAck()
{
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA0);  // Acknowledge
  Serial.write(0x55);  // End of Text
}

void sb_send(uint8_t idx, uint8_t key)
{
  uint8_t code;

  key--;               // Support 1 to 16
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA5);  // Send following code
  for (uint8_t i = 0; i < 8; i++) {
    Serial.write(sysCfg.sfb_code[idx][i]);
  }
  if (0 == idx) {
    code = (0x10 << (key >> 2)) | (0x01 << (key & 3));  // 11,12,14,18,21,22,24,28,41,42,44,48,81,82,84,88
  } else {
    code = sysCfg.sfb_code[idx][8];
  }
  Serial.write(code);
  Serial.write(0x55);  // End of Text
  Serial.flush();
}

void sb_learn(uint8_t key)
{
  sfb_learnKey = key;
  sfb_learnFlg = 1;
  Serial.write(0xAA);  // Start of Text
  Serial.write(0xA1);  // Start learning
  Serial.write(0x55);  // End of Text
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean sb_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  boolean serviced = true;
  char *p;

  if (!strcasecmp_P(type, PSTR(D_CMND_RFDEFAULT))) {
    if (4 == data_len) {
      uint16_t hexcode = strtol(dataBuf, &p, 16);
      uint8_t msb = hexcode >> 8;
      uint8_t lsb = hexcode & 0xFF;
      if ((hexcode > 0) && (hexcode < 0x7FFF) && (msb != 0x55) && (lsb != 0x55)) {
        sysCfg.sfb_code[0][6] = msb;
        sysCfg.sfb_code[0][7] = lsb;
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFDEFAULT "\":\"%0X%0X\"}"), sysCfg.sfb_code[0][6], sysCfg.sfb_code[0][7]);
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_RFKEY)) && (index > 0) && (index <= 16)) {
    if (!sfb_learnFlg) {
      if (2 == payload) {
        sb_learn(index);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_START_LEARNING "\"}"), index);
      }
      else if (3 == payload) {
        sysCfg.sfb_code[index][0] = 0;
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_SET_TO_DEFAULT "\"}"), index);
      } else {
        if ((1 == payload) || (0 == sysCfg.sfb_code[index][0])) {
          sb_send(0, index);
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_DEFAULT_SENT "\"}"), index);
        } else {
          sb_send(index, 0);
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARNED_SENT "\"}"), index);
        }
      }
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFKEY "%d\":\"" D_LEARNING_ACTIVE "\"}"), sfb_learnKey);
    }
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}
