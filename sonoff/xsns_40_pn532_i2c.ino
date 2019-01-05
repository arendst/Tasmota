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
#define PN532_COMMAND_INLISTPASSIVETARGET 0x4A

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
    pn532_i2c_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_i2c_packetbuffer[3] = 0x01; // use IRQ pin!

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

void PN532_ScanForTag(void)
{
  if (pn532_i2c_disable) { return; }
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uid_len = 0;
  if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
    if (pn532_i2c_scan_defer_report > 0) {
      pn532_i2c_scan_defer_report--;
    } else {
      char uids[15];
      sprintf(uids,"");
      for (uint8_t i = 0;i < uid_len;i++) {
        sprintf(uids,"%s%X",uids,uid[i]);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"PN532\":{\"UID\":\"%s\"}}"), mqtt_data, uids);
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
      char command[27];
      sprintf(command,"event PN532=%s",uids);
      ExecuteCommand(command, SRC_RULE);
      pn532_i2c_scan_defer_report = 7; // Ignore tags found for two seconds
    }
  } else {
    if (pn532_i2c_scan_defer_report > 0) { pn532_i2c_scan_defer_report--; }
  }
}

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
