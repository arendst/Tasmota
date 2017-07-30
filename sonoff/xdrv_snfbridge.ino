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

uint8_t sfb_rcvflg = 0;                // Sonoff RF Bridge communication
uint8_t sfb_learnKey = 1;
uint8_t sfb_learnFlg = 0;

void sb_received()
{
  char svalue[60];
  char log[LOGSZ];

  svalue[0] = '\0';
  for (byte i = 0; i < SerialInByteCounter; i++) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%02X "), svalue, serialInBuf[i]);
  }
  snprintf_P(log, sizeof(log), PSTR("BRDG: Received %s"), svalue);
  addLog(LOG_LEVEL_DEBUG, log);

  if (0xA2 == serialInBuf[0]) {  // Learn failed
    sfb_learnFlg = 0;
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"RfKey%d\":\"Learn failed\"}"), sfb_learnKey);
    mqtt_publish_topic_P(5, PSTR("RFKEY"), svalue);
  }
  if (0xA3 == serialInBuf[0]) {  // Learn
    sfb_learnFlg = 0;
    for (uint8_t i = 0; i < 9; i++) {
      sysCfg.sfb_code[sfb_learnKey][i] = serialInBuf[i +1];
    }
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"RfKey%d\":\"Learned\"}"), sfb_learnKey);
    mqtt_publish_topic_P(5, PSTR("RFKEY"), svalue);
  }
}

boolean sb_serial()
{
  if (sfb_rcvflg) {
    if (SerialInByte > 0) {
      serialInBuf[SerialInByteCounter++] = SerialInByte;
      if (0x55 == SerialInByte) {
//        serialInBuf[SerialInByteCounter] = 0x55;
        sb_received();
        sfb_rcvflg = 0;                     // 0x55 - End of text
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

boolean sb_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  char *p;

  if (!strcmp_P(type, PSTR("RFDEFAULT"))) {
    if (4 == data_len) {
      uint16_t hexcode = strtol(dataBuf, &p, 16);
      uint8_t msb = hexcode >> 8;
      uint8_t lsb = hexcode & 0xFF;
      if ((hexcode > 0) && (hexcode < 0x7FFF) && (msb != 0x55) && (lsb != 0x55)) {
        sysCfg.sfb_code[0][6] = msb;
        sysCfg.sfb_code[0][7] = lsb;
      }
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"RfDefault\":\"%0X%0X\"}"), sysCfg.sfb_code[0][6], sysCfg.sfb_code[0][7]);
  }
  else if (!strcmp_P(type, PSTR("RFKEY")) && (index > 0) && (index <= 16)) {
    if (!sfb_learnFlg) {
      if (2 == payload) {
        sb_learn(index);
        snprintf_P(svalue, ssvalue, PSTR("{\"RfKey%d\":\"Start learning\"}"), index);
      }
      else if (3 == payload) {
        sysCfg.sfb_code[index][0] = 0;
        snprintf_P(svalue, ssvalue, PSTR("{\"RfKey%d\":\"Set to default\"}"), index);
      } else {
        if ((1 == payload) || (0 == sysCfg.sfb_code[index][0])) {
          sb_send(0, index); 
          snprintf_P(svalue, ssvalue, PSTR("{\"RfKey%d\":\"Default sent\"}"), index);
        } else {
          sb_send(index, 0);
          snprintf_P(svalue, ssvalue, PSTR("{\"RfKey%d\":\"Learned sent\"}"), index);
        }
      }
    } else {
      snprintf_P(svalue, ssvalue, PSTR("{\"RfKey%d\":\"Learning active\"}"), sfb_learnKey);
    }
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}
