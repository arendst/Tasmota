/*
  xsns_40_pn532.ino - Support for PN532 (I2C) NFC Tag Reader

  Copyright (C) 2019  Andre Thomas and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_PN532_I2C

/*********************************************************************************************\
 * PN532 - Near Field Communication (NFC) controller
 *
 * Datasheet at https://www.nxp.com/docs/en/nxp/data-sheets/PN532_C1.pdf
 *
 * I2C Address: 0x24
\*********************************************************************************************/

#define XSNS_40                           40

#define PN532_I2C_ADDRESS                 0x24

#define PN532_COMMAND_GETFIRMWAREVERSION  0x02
#define PN532_COMMAND_SAMCONFIGURATION    0x14
#define PN532_COMMAND_INDATAEXCHANGE      0x40
#define PN532_COMMAND_INLISTPASSIVETARGET 0x4A

#define MIFARE_CMD_READ                   0x30
#define MIFARE_CMD_AUTH_A                 0x60
#define MIFARE_CMD_AUTH_B                 0x61
#define MIFARE_CMD_WRITE                  0xA0

#define PN532_PREAMBLE                    0x00
#define PN532_STARTCODE1                  0x00
#define PN532_STARTCODE2                  0xFF
#define PN532_POSTAMBLE                   0x00

#define PN532_HOSTTOPN532                 0xD4
#define PN532_PN532TOHOST                 0xD5

#define PN532_INVALID_ACK                 -1
#define PN532_TIMEOUT                     -2
#define PN532_INVALID_FRAME               -3
#define PN532_NO_SPACE                    -4

#define PN532_MIFARE_ISO14443A            0x00

uint8_t pn532_i2c_detected = 0;
uint8_t pn532_i2c_packetbuffer[64];
uint8_t pn532_i2c_scan_defer_report = 0;         // If a valid card was found we will not scan for one again in the same two seconds so we set this to 19 if a card was found
uint8_t pn532_i2c_command = 0;

uint8_t pn532_i2c_disable = 0;

#ifdef USE_PN532_DATA_FUNCTION
uint8_t pn532_i2c_function = 0;
uint8_t pn532_i2c_newdata[16];
#endif // USE_PN532_DATA_FUNCTION

const uint8_t PROGMEM pn532_global_timeout = 10;

int16_t PN532_getResponseLength(uint8_t buf[], uint8_t len) {
  const uint8_t PN532_NACK[] = {0, 0, 0xFF, 0xFF, 0, 0};
  uint8_t time = 0;
  do {
    if (Wire.requestFrom(PN532_I2C_ADDRESS, 6)) {
      if (Wire.read() & 1) {  // check first byte --- status
        break;         // PN532 is ready
      }
    }
    delay(1);
    time++;
    if (time > pn532_global_timeout) {
      return -1;
    }
  } while (1);

  if ((0x00 != Wire.read()) || (0x00 != Wire.read()) || (0xFF != Wire.read())) { // PREAMBLE || STARTCODE1 || STARTCODE2
    return PN532_INVALID_FRAME;
  }

  uint8_t length = Wire.read();
  
  // request for last respond msg again
  Wire.beginTransmission(PN532_I2C_ADDRESS);
  for (uint16_t i = 0;i < sizeof(PN532_NACK); ++i) {
    Wire.write(PN532_NACK[i]);
  }
  Wire.endTransmission();
  return length;
}


int16_t PN532_readResponse(uint8_t buf[], uint8_t len)
{
  uint8_t time = 0;
  uint8_t length;

  length = PN532_getResponseLength(buf, len);

  // [RDY] 00 00 FF LEN LCS (TFI PD0 ... PDn) DCS 00

  do {
    if (Wire.requestFrom(PN532_I2C_ADDRESS, 6 + length + 2)) {
      if (Wire.read() & 1) {  // check first byte --- status
        break;                // PN532 is ready
      }
    }
    delay(1);
    time++;
    if (time > pn532_global_timeout) {
      return -1;
    }
  } while (1);

  if ((0x00 != Wire.read()) || (0x00 != Wire.read()) || (0xFF != Wire.read())) { // PREAMBLE || STARTCODE1 || STARTCODE2
    return PN532_INVALID_FRAME;
  }
  
  length = Wire.read();
  
  if (0 != (uint8_t)(length + Wire.read())) {   // checksum of length
    return PN532_INVALID_FRAME;
  }

  uint8_t cmd = pn532_i2c_command + 1;               // response command
  if ((PN532_PN532TOHOST != Wire.read()) || ((cmd) != Wire.read())) {
    return PN532_INVALID_FRAME;
  }
  length -= 2;
  if (length > len) {
    return PN532_NO_SPACE;  // not enough space
  }
  uint8_t sum = PN532_PN532TOHOST + cmd;
  for (uint8_t i = 0; i < length; i++) {
    buf[i] = Wire.read();
    sum += buf[i];
  }
  uint8_t checksum = Wire.read();
  if (0 != (uint8_t)(sum + checksum)) {
    return PN532_INVALID_FRAME;
  }
  Wire.read();         // POSTAMBLE
  return length;
}


int8_t PN532_readAckFrame(void)
{
  const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
  uint8_t ackBuf[sizeof(PN532_ACK)];
  
  uint8_t time = 0;
  
  do {
    if (Wire.requestFrom(PN532_I2C_ADDRESS,  sizeof(PN532_ACK) + 1)) {
      if (Wire.read() & 1) {  // check first byte --- status
        break;         // PN532 is ready
      }
    }
    delay(1);
    time++;
        if (time > pn532_global_timeout) { // We time out after 10ms
            return PN532_TIMEOUT;
        }
  } while (1);

  for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
    ackBuf[i] = Wire.read();
  }

  if (memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK))) {
    return PN532_INVALID_ACK;
  }

  return 0;
}

int8_t PN532_writeCommand(const uint8_t *header, uint8_t hlen)
{
  pn532_i2c_command = header[0];
  Wire.beginTransmission(PN532_I2C_ADDRESS);
  Wire.write(PN532_PREAMBLE);
  Wire.write(PN532_STARTCODE1);
  Wire.write(PN532_STARTCODE2);
  uint8_t length = hlen + 1;          // TFI + DATA
  Wire.write(length);
  Wire.write(~length + 1);            // checksum of length
  Wire.write(PN532_HOSTTOPN532);
  uint8_t sum = PN532_HOSTTOPN532;    // Sum of TFI + DATA
  for (uint8_t i = 0; i < hlen; i++) {
    if (Wire.write(header[i])) {
      sum += header[i];
    } else {
      return PN532_INVALID_FRAME;
    }
  }
  uint8_t checksum = ~sum + 1;        // Checksum of TFI + DATA
  Wire.write(checksum);
  Wire.write(PN532_POSTAMBLE);
  Wire.endTransmission();
  return PN532_readAckFrame();
}

uint32_t PN532_getFirmwareVersion(void)
{
    uint32_t response;
    pn532_i2c_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;
    if (PN532_writeCommand(pn532_i2c_packetbuffer, 1)) {
        return 0;
    }
    int16_t status = PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer));
    if (0 > status) {
        return 0;
    }
    response = pn532_i2c_packetbuffer[0];
    response <<= 8;
    response |= pn532_i2c_packetbuffer[1];
    response <<= 8;
    response |= pn532_i2c_packetbuffer[2];
    response <<= 8;
    response |= pn532_i2c_packetbuffer[3];
    return response;
}

bool PN532_SAMConfig(void)
{
    pn532_i2c_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
    pn532_i2c_packetbuffer[1] = 0x01; // normal mode;
    pn532_i2c_packetbuffer[2] = 0x01; // timeout 50ms * 1 = 50ms
    pn532_i2c_packetbuffer[3] = 0x00; // Disable IRQ pin

    if (PN532_writeCommand(pn532_i2c_packetbuffer, 4))
        return false;

    return (0 < PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer)));
}

void PN532_Detect(void)
{
  if ((pn532_i2c_detected) || (pn532_i2c_disable)) { return; }
  
  Wire.setClockStretchLimit(1000); // Enable 1ms clock stretch as per datasheet Table 12.25 (Timing for the I2C interface)

  uint32_t ver = PN532_getFirmwareVersion();
  if (ver) {
    pn532_i2c_detected = 1;
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "PN532 NFC Reader (V%u.%u)", PN532_I2C_ADDRESS);
    snprintf_P(log_data, sizeof(log_data), log_data, (ver>>16) & 0xFF, (ver>>8) & 0xFF);
    AddLog(LOG_LEVEL_DEBUG);
    PN532_SAMConfig();
  }
}

boolean PN532_readPassiveTargetID(uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength)
{
  pn532_i2c_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
  pn532_i2c_packetbuffer[1] = 1;
  pn532_i2c_packetbuffer[2] = cardbaudrate;

  if (PN532_writeCommand(pn532_i2c_packetbuffer, 3)) {
    return false;  // command failed
  }
  
  if (PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer)) < 0) { // No data packet so no tag was found
    Wire.beginTransmission(PN532_I2C_ADDRESS);
    Wire.endTransmission();
    return false;
  }

  if (pn532_i2c_packetbuffer[0] != 1) { return false; } // Not a valid tag

  *uidLength = pn532_i2c_packetbuffer[5];
  for (uint8_t i = 0;i < pn532_i2c_packetbuffer[5]; i++) {
    uid[i] = pn532_i2c_packetbuffer[6 + i];
  }
  return true;
}

#ifdef USE_PN532_DATA_FUNCTION

uint8_t mifareclassic_AuthenticateBlock (uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData)
{
    uint8_t i;
    uint8_t _key[6];
    uint8_t _uid[7];
    uint8_t _uidLen;

    // Hang on to the key and uid data
    memcpy (_key, keyData, 6);
    memcpy (_uid, uid, uidLen);
    _uidLen = uidLen;

    // Prepare the authentication command //
    pn532_i2c_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;   /* Data Exchange Header */
    pn532_i2c_packetbuffer[1] = 1;                              /* Max card numbers */
    pn532_i2c_packetbuffer[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
    pn532_i2c_packetbuffer[3] = blockNumber;                    /* Block Number (1K = 0..63, 4K = 0..255 */
    memcpy (&pn532_i2c_packetbuffer[4], _key, 6);
    for (i = 0; i < _uidLen; i++) {
        pn532_i2c_packetbuffer[10 + i] = _uid[i];              /* 4 bytes card ID */
    }

    if (PN532_writeCommand(pn532_i2c_packetbuffer, 10 + _uidLen))
        return 0;

    // Read the response packet
    PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer));

    // Check if the response is valid and we are authenticated???
    // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
    // Mifare auth error is technically byte 7: 0x14 but anything other and 0x00 is not good
    if (pn532_i2c_packetbuffer[0] != 0x00) {
        // Authentification failed
        return 0;
    }

    return 1;
}

uint8_t mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t *data)
{
    /* Prepare the command */
    pn532_i2c_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_i2c_packetbuffer[1] = 1;                      /* Card number */
    pn532_i2c_packetbuffer[2] = MIFARE_CMD_READ;        /* Mifare Read command = 0x30 */
    pn532_i2c_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */

    /* Send the command */
    if (PN532_writeCommand(pn532_i2c_packetbuffer, 4)) {
        return 0;
    }

    /* Read the response packet */
    PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer));

    /* If byte 8 isn't 0x00 we probably have an error */
    if (pn532_i2c_packetbuffer[0] != 0x00) {
        return 0;
    }

    /* Copy the 16 data bytes to the output buffer        */
    /* Block content starts at byte 9 of a valid response */
    memcpy (data, &pn532_i2c_packetbuffer[1], 16);

    return 1;
}

uint8_t mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data)
{
    /* Prepare the first command */
    pn532_i2c_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_i2c_packetbuffer[1] = 1;                      /* Card number */
    pn532_i2c_packetbuffer[2] = MIFARE_CMD_WRITE;       /* Mifare Write command = 0xA0 */
    pn532_i2c_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */
    memcpy(&pn532_i2c_packetbuffer[4], data, 16);       /* Data Payload */

    /* Send the command */
    if (PN532_writeCommand(pn532_i2c_packetbuffer, 20)) {
        return 0;
    }

    /* Read the response packet */
    return (0 < PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer)));
}

#endif // USE_PN532_DATA_FUNCTION

void PN532_ScanForTag(void)
{
  if (pn532_i2c_disable) { return; }
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uid_len = 0;
  uint8_t card_data[16];
  boolean erase_success = false;
  boolean set_success = false;
  if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
    if (pn532_i2c_scan_defer_report > 0) {
      pn532_i2c_scan_defer_report--;
    } else {
      char uids[15];
      
#ifdef USE_PN532_DATA_FUNCTION      
      char card_datas[34];
#endif // USE_PN532_DATA_FUNCTION      

      sprintf(uids,"");
      for (uint8_t i = 0;i < uid_len;i++) {
        sprintf(uids,"%s%02X",uids,uid[i]);
      }
      
#ifdef USE_PN532_DATA_FUNCTION      
      if (uid_len == 4) { // Lets try to read block 0 of the mifare classic card for more information
        uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        if (mifareclassic_AuthenticateBlock (uid, uid_len, 1, 1, keyuniversal)) {
          if (mifareclassic_ReadDataBlock(1, card_data)) {
            for (uint8_t i = 0;i < sizeof(card_data);i++) {
              if ((isalpha(card_data[i])) || ((isdigit(card_data[i])))) {
                card_datas[i] = char(card_data[i]);
              } else {
                card_datas[i] = '\0';
              }
            }
          }
          if (pn532_i2c_function == 1) { // erase block 1 of card
            for (uint8_t i = 0;i<16;i++) {
              card_data[i] = 0x00;
            }
            if (mifareclassic_WriteDataBlock(1, card_data)) {
              erase_success = true;
              snprintf_P(log_data, sizeof(log_data),"I2C: PN532 NFC - Erase success");
              AddLog(LOG_LEVEL_INFO);
              memcpy(&card_datas,&card_data,sizeof(card_data)); // Cast block 1 to a string
            }
          }
          if (pn532_i2c_function == 2) {
            memcpy(&card_data,&pn532_i2c_newdata,sizeof(card_data));
            if (mifareclassic_WriteDataBlock(1, card_data)) {
              set_success = true;
              snprintf_P(log_data, sizeof(log_data),"I2C: PN532 NFC - Data write successful");
              AddLog(LOG_LEVEL_INFO);
              memcpy(&card_datas,&card_data,sizeof(card_data)); // Cast block 1 to a string
            }
          }
        } else {
          sprintf(card_datas,"AUTHFAIL");
        }
      }
      switch (pn532_i2c_function) {
        case 0x01:
          if (!erase_success) {
            snprintf_P(log_data, sizeof(log_data),"I2C: PN532 NFC - Erase fail - exiting erase mode");
            AddLog(LOG_LEVEL_INFO);
          }
          break;
        case 0x02:
          if (!set_success) {
            snprintf_P(log_data, sizeof(log_data),"I2C: PN532 NFC - Write failed - exiting set mode");
            AddLog(LOG_LEVEL_INFO);
          }
        default:
          break;
      }
      pn532_i2c_function = 0;
#endif // USE_PN532_DATA_FUNCTION      
      
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());

#ifdef USE_PN532_DATA_FUNCTION
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"PN532\":{\"UID\":\"%s\", \"DATA\":\"%s\"}}"), mqtt_data, uids, card_datas);
#else
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"PN532\":{\"UID\":\"%s\"}}"), mqtt_data, uids);
#endif // USE_PN532_DATA_FUNCTION      

      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);

#ifdef USE_PN532_CAUSE_EVENTS

      char command[71];
#ifdef USE_PN532_DATA_FUNCTION
      sprintf(command,"backlog event PN532_UID=%s;event PN532_DATA=%s",uids,card_datas);
#else
      sprintf(command,"event PN532_UID=%s",uids);
#endif // USE_PN532_DATA_FUNCTION      
      ExecuteCommand(command, SRC_RULE);
#endif // USE_PN532_CAUSE_EVENTS
      
      pn532_i2c_scan_defer_report = 7; // Ignore tags found for two seconds
    }
  } else {
    if (pn532_i2c_scan_defer_report > 0) { pn532_i2c_scan_defer_report--; }
  }
}

#ifdef USE_PN532_DATA_FUNCTION

boolean PN532_Command(void)
{
  boolean serviced = true;
  uint8_t paramcount = 0;
  if (XdrvMailbox.data_len > 0) {
    paramcount=1;
  } else {
    serviced = false;
    return serviced;
  }
  char sub_string[XdrvMailbox.data_len];
  char sub_string_tmp[XdrvMailbox.data_len];
  for (uint8_t ca=0;ca<XdrvMailbox.data_len;ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    if (',' == XdrvMailbox.data[ca]) { paramcount++; }
  }
  UpperCase(XdrvMailbox.data,XdrvMailbox.data);
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"E")) {
    pn532_i2c_function = 1; // Block 0 of next card/tag will be reset to 0x00...
    snprintf_P(log_data, sizeof(log_data),"I2C: PN532 NFC - Next scanned tag data block 1 will be erased");
    AddLog(LOG_LEVEL_INFO);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"PN532\":{\"COMMAND\":\"E\"\"}}"), mqtt_data);
    return serviced;
  }
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"S")) {
    if (paramcount > 1) {
      if (XdrvMailbox.data[XdrvMailbox.data_len-1] == ',') {
        serviced = false;
        return serviced;
      }
      sprintf(sub_string_tmp,subStr(sub_string, XdrvMailbox.data, ",", 2));
      uint8_t dlen = strlen(sub_string_tmp);
      if (dlen > 15) { dlen = 15; }
      memcpy(&pn532_i2c_newdata,&sub_string_tmp,dlen);
      pn532_i2c_newdata[dlen] = 0x00; // Null terminate the string
      pn532_i2c_function = 2;
      snprintf_P(log_data, sizeof(log_data),"I2C: PN532 NFC - Next scanned tag data block 1 will be set to '%s'",pn532_i2c_newdata);
      AddLog(LOG_LEVEL_INFO);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"PN532\":{\"COMMAND\":\"S\"\"}}"), mqtt_data);
      return serviced;
    }
  }
}

#endif // USE_PN532_DATA_FUNCTION

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns40(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        if (pn532_i2c_detected) {
          PN532_ScanForTag();
        }
        break;
      case FUNC_EVERY_SECOND:
        PN532_Detect();
        break;

#ifdef USE_PN532_DATA_FUNCTION        
      case FUNC_COMMAND:
        if (XSNS_40 == XdrvMailbox.index) {
          result = PN532_Command();
        }
        break;
#endif // USE_PN532_DATA_FUNCTION        

      case FUNC_SAVE_BEFORE_RESTART:
        if (!pn532_i2c_disable) {
          pn532_i2c_disable = 1;
          snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "PN532 NFC Reader - Disabling for reboot", PN532_I2C_ADDRESS);
          AddLog(LOG_LEVEL_DEBUG);
        }
        break;
      default:
        break;
    }
  }
  return result;
}

#endif  // USE_PN532_I2C
#endif  // USE_I2C
