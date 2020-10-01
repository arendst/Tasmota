/*
  xsns_40_pn532.ino - Support for PN532 (HSU) NFC Tag Reader on Tasmota

  Copyright (C) 2020  Andre Thomas and Theo Arends

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

#ifdef USE_PN532_HSU

#define XSNS_40                                     40

#include <TasmotaSerial.h>

TasmotaSerial *PN532_Serial;

#define PN532_INVALID_ACK                           -1
#define PN532_TIMEOUT                               -2
#define PN532_INVALID_FRAME                         -3
#define PN532_NO_SPACE                              -4

#define PN532_PREAMBLE                              0x00
#define PN532_STARTCODE1                            0x00
#define PN532_STARTCODE2                            0xFF
#define PN532_POSTAMBLE                             0x00

#define PN532_HOSTTOPN532                           0xD4
#define PN532_PN532TOHOST                           0xD5

#define PN532_ACK_WAIT_TIME                         0x0A

#define PN532_COMMAND_GETFIRMWAREVERSION            0x02
#define PN532_COMMAND_SAMCONFIGURATION              0x14
#define PN532_COMMAND_RFCONFIGURATION               0x32
#define PN532_COMMAND_INDATAEXCHANGE                0x40
#define PN532_COMMAND_INLISTPASSIVETARGET           0x4A

#define PN532_MIFARE_ISO14443A                      0x00
#define MIFARE_CMD_READ                             0x30
#define MIFARE_CMD_AUTH_A                           0x60
#define MIFARE_CMD_AUTH_B                           0x61
#define MIFARE_CMD_WRITE                            0xA0

uint8_t pn532_model = 0;          // Used to maintain detection flag
uint8_t pn532_command = 0;        // Used to carry command code between functions
uint8_t pn532_scantimer = 0;      // Used to prevent multiple successful reads within 2 second window

uint8_t pn532_packetbuffer[64];   // Global buffer used to store packet

#ifdef USE_PN532_DATA_FUNCTION
uint8_t pn532_function = 0;
uint8_t pn532_newdata[16];
uint8_t pn532_newdata_len = 0;
#endif // USE_PN532_DATA_FUNCTION

void PN532_Init(void)
{
  if (PinUsed(GPIO_PN532_RXD) && PinUsed(GPIO_PN532_TXD)) {
    PN532_Serial = new TasmotaSerial(Pin(GPIO_PN532_RXD), Pin(GPIO_PN532_TXD), 1);
    if (PN532_Serial->begin(115200)) {
      if (PN532_Serial->hardwareSerial()) { ClaimSerial(); }
      PN532_wakeup();
      uint32_t ver = PN532_getFirmwareVersion();
      if (ver) {
        PN532_setPassiveActivationRetries(0xFF);
        PN532_SAMConfig();
        pn532_model = 1;
        AddLog_P2(LOG_LEVEL_INFO,"NFC: PN532 NFC Reader detected (V%u.%u)",(ver>>16) & 0xFF, (ver>>8) & 0xFF);
      }
    }
  }
}

int8_t PN532_receive(uint8_t *buf, int len, uint16_t timeout)
{
  int read_bytes = 0;
  int ret;
  unsigned long start_millis;
  while (read_bytes < len) {
    start_millis = millis();
    do {
      ret = PN532_Serial->read();
      if (ret >= 0) {
        break;
      }
    } while((timeout == 0) || ((millis()- start_millis ) < timeout));

    if (ret < 0) {
      if (read_bytes) {
        return read_bytes;
      } else {
        return PN532_TIMEOUT;
      }
    }
    buf[read_bytes] = (uint8_t)ret;
    read_bytes++;
  }
  return read_bytes;
}

int8_t PN532_readAckFrame(void)
{
  const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
  uint8_t ackBuf[sizeof(PN532_ACK)];

  if (PN532_receive(ackBuf, sizeof(PN532_ACK), PN532_ACK_WAIT_TIME) <= 0) {
    return PN532_TIMEOUT;
  }

  if (memcmp(&ackBuf, &PN532_ACK, sizeof(PN532_ACK))) {
    return PN532_INVALID_ACK;
  }
  return 0;
}

int8_t PN532_writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0)
{
  // Clear the serial buffer just in case
  PN532_Serial->flush();

  pn532_command = header[0];
  PN532_Serial->write((uint8_t)PN532_PREAMBLE);
  PN532_Serial->write((uint8_t)PN532_STARTCODE1);
  PN532_Serial->write(PN532_STARTCODE2);

  uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
  PN532_Serial->write(length);
  PN532_Serial->write(~length + 1);         // checksum of length

  PN532_Serial->write(PN532_HOSTTOPN532);
  uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA

  PN532_Serial->write(header, hlen);
  for (uint32_t i = 0; i < hlen; i++) {
    sum += header[i];
  }

  PN532_Serial->write(body, blen);
  for (uint32_t i = 0; i < blen; i++) {
    sum += body[i];
  }

  uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
  PN532_Serial->write(checksum);
  PN532_Serial->write((uint8_t)PN532_POSTAMBLE);

  return PN532_readAckFrame();
}

int16_t PN532_readResponse(uint8_t buf[], uint8_t len, uint16_t timeout = 50)
{
  uint8_t tmp[3];

  // Read preamble and start code
  if (PN532_receive(tmp, 3, timeout)<=0) {
    return PN532_TIMEOUT;
  }
  if (0 != tmp[0] || 0!= tmp[1] || 0xFF != tmp[2]) {
    return PN532_INVALID_FRAME;
  }

  // Get length of data to be received
  uint8_t length[2];
  if (PN532_receive(length, 2, timeout) <= 0) {
    return PN532_TIMEOUT;
  }
  // Validate that frame is valid
  if (0 != (uint8_t)(length[0] + length[1])) {
    return PN532_INVALID_FRAME;
  }
  length[0] -= 2;
  if (length[0] > len) {   // If this happens, then pn532_packetbuffer is not large enough
    return PN532_NO_SPACE;
  }

  // Get the command byte
  uint8_t cmd = pn532_command + 1;
  if (PN532_receive(tmp, 2, timeout) <= 0) { // Time out while receiving
    return PN532_TIMEOUT;
  }
  if (PN532_PN532TOHOST != tmp[0] || cmd != tmp[1]) { // Invalid frame received
    return PN532_INVALID_FRAME;
  }

  if (PN532_receive(buf, length[0], timeout) != length[0]) { // Timed out
    return PN532_TIMEOUT;
  }

  uint8_t sum = PN532_PN532TOHOST + cmd;
  for (uint32_t i=0; i<length[0]; i++) {
    sum += buf[i];
  }

  // Checksum & postamble
  if (PN532_receive(tmp, 2, timeout) <= 0) {
    return PN532_TIMEOUT;
  }
  if (0 != (uint8_t)(sum + tmp[0]) || 0 != tmp[1]) { // Checksum fail, so frame must be invalid
    return PN532_INVALID_FRAME;
  }

  return length[0];
}

uint32_t PN532_getFirmwareVersion(void)
{
  uint32_t response;

  pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

  if (PN532_writeCommand(pn532_packetbuffer, 1)) {
    return 0;
  }

  // Read data packet
  int16_t status = PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));
  if (0 > status) {
    return 0;
  }

  response = pn532_packetbuffer[0];
  response <<= 8;
  response |= pn532_packetbuffer[1];
  response <<= 8;
  response |= pn532_packetbuffer[2];
  response <<= 8;
  response |= pn532_packetbuffer[3];

  return response;
}

void PN532_wakeup(void)
{
  uint8_t wakeup[5] = {0x55, 0x55, 0, 0, 0 };
  PN532_Serial->write(wakeup,sizeof(wakeup));

  // Flush the serial buffer just in case there's garbage in there
  PN532_Serial->flush();
}

bool PN532_readPassiveTargetID(uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength, uint16_t timeout = 50)
{
  pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
  pn532_packetbuffer[2] = cardbaudrate;
  if (PN532_writeCommand(pn532_packetbuffer, 3)) {
    return 0x0;  // command failed
  }
  // read data packet
  if (PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer), timeout) < 0) {
    return 0x0;
  }

  /* Check some basic stuff
     b0              Tags Found
     b1              Tag Number (only one used in this example)
     b2..3           SENS_RES
     b4              SEL_RES
     b5              NFCID Length
     b6..NFCIDLen    NFCID
    */

  if (pn532_packetbuffer[0] != 1) {
    return 0;
  }

  uint16_t sens_res = pn532_packetbuffer[2];
  sens_res <<= 8;
  sens_res |= pn532_packetbuffer[3];

  /* Card appears to be Mifare Classic */
  *uidLength = pn532_packetbuffer[5];

  for (uint32_t i = 0; i < pn532_packetbuffer[5]; i++) {
    uid[i] = pn532_packetbuffer[6 + i];
  }

  return 1;
}

bool PN532_setPassiveActivationRetries(uint8_t maxRetries)
{
  pn532_packetbuffer[0] = PN532_COMMAND_RFCONFIGURATION;
  pn532_packetbuffer[1] = 5;    // Config item 5 (MaxRetries)
  pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
  pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
  pn532_packetbuffer[4] = maxRetries;
  if (PN532_writeCommand(pn532_packetbuffer, 5)) {
    return 0; // no ACK
  }
  return (0 < PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

bool PN532_SAMConfig(void)
{
  pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
  pn532_packetbuffer[1] = 0x01; // normal mode
  pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
  pn532_packetbuffer[3] = 0x00; // we don't need the external IRQ pin
  if (PN532_writeCommand(pn532_packetbuffer, 4)) {
    return false;
  }
  return (0 < PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

#ifdef USE_PN532_DATA_FUNCTION

uint8_t mifareclassic_AuthenticateBlock (uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData)
{
    uint8_t i;
    uint8_t _key[6];
    uint8_t _uid[7];
    uint8_t _uidLen;

    // Hang on to the key and uid data
    memcpy(&_key, keyData, 6);
    memcpy(&_uid, uid, uidLen);
    _uidLen = uidLen;

    // Prepare the authentication command //
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;   /* Data Exchange Header */
    pn532_packetbuffer[1] = 1;                              /* Max card numbers */
    pn532_packetbuffer[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
    pn532_packetbuffer[3] = blockNumber;                    /* Block Number (1K = 0..63, 4K = 0..255 */
    memcpy(&pn532_packetbuffer[4], &_key, 6);
    for (i = 0; i < _uidLen; i++) {
        pn532_packetbuffer[10 + i] = _uid[i];              /* 4 bytes card ID */
    }

    if (PN532_writeCommand(pn532_packetbuffer, 10 + _uidLen)) { return 0; }

    // Read the response packet
    PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    // Check if the response is valid and we are authenticated???
    // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
    // Mifare auth error is technically byte 7: 0x14 but anything other and 0x00 is not good
    if (pn532_packetbuffer[0] != 0x00) {
        // Authentification failed
        return 0;
    }

    return 1;
}

uint8_t mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t *data)
{
    /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1;                      /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_READ;        /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */

    /* Send the command */
    if (PN532_writeCommand(pn532_packetbuffer, 4)) {
        return 0;
    }

    /* Read the response packet */
    PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    /* If byte 8 isn't 0x00 we probably have an error */
    if (pn532_packetbuffer[0] != 0x00) {
        return 0;
    }

    /* Copy the 16 data bytes to the output buffer        */
    /* Block content starts at byte 9 of a valid response */
    memcpy (data, &pn532_packetbuffer[1], 16);

    return 1;
}

uint8_t mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data)
{
    /* Prepare the first command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1;                      /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_WRITE;       /* Mifare Write command = 0xA0 */
    pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */
    memcpy(&pn532_packetbuffer[4], data, 16);       /* Data Payload */

    /* Send the command */
    if (PN532_writeCommand(pn532_packetbuffer, 20)) {
        return 0;
    }

    /* Read the response packet */
    return (0 < PN532_readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

#endif // USE_PN532_DATA_FUNCTION

void PN532_ScanForTag(void)
{
  if (!pn532_model) { return; }
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uid_len = 0;
  uint8_t card_data[16];
  bool erase_success = false;
  bool set_success = false;
  if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
      char uids[15];

#ifdef USE_PN532_DATA_FUNCTION
      char card_datas[34];
#endif // USE_PN532_DATA_FUNCTION

      ToHex_P((unsigned char*)uid, uid_len, uids, sizeof(uids));

#ifdef USE_PN532_DATA_FUNCTION
      if (uid_len == 4) { // Lets try to read block 1 of the mifare classic card for more information
        uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        if (mifareclassic_AuthenticateBlock (uid, uid_len, 1, 1, keyuniversal)) {
          if (mifareclassic_ReadDataBlock(1, card_data)) {
#ifdef USE_PN532_DATA_RAW
            memcpy(&card_datas,&card_data,sizeof(card_data));
#else
            for (uint32_t i = 0;i < sizeof(card_data);i++) {
              if ((isalpha(card_data[i])) || ((isdigit(card_data[i])))) {
                card_datas[i] = char(card_data[i]);
              } else {
                card_datas[i] = '\0';
              }
            }
#endif // USE_PN532_DATA_RAW
          }
          if (pn532_function == 1) { // erase block 1 of card
            for (uint32_t i = 0;i<16;i++) {
              card_data[i] = 0x00;
            }
            if (mifareclassic_WriteDataBlock(1, card_data)) {
              erase_success = true;
              AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Erase success"));
              memcpy(&card_datas,&card_data,sizeof(card_data)); // Cast block 1 to a string
            }
          }
          if (pn532_function == 2) {
#ifdef USE_PN532_DATA_RAW
            memcpy(&card_data,&pn532_newdata,sizeof(card_data));
            if (mifareclassic_WriteDataBlock(1, card_data)) {
              set_success = true;
              AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Data write successful"));
              memcpy(&card_datas,&card_data,sizeof(card_data)); // Cast block 1 to a string
            }
#else
            bool IsAlphaNumeric = true;
            for (uint32_t i = 0;i < pn532_newdata_len;i++) {
              if ((!isalpha(pn532_newdata[i])) && (!isdigit(pn532_newdata[i]))) {
                IsAlphaNumeric = false;
              }
            }
            if (IsAlphaNumeric) {
              memcpy(&card_data,&pn532_newdata,pn532_newdata_len);
              card_data[pn532_newdata_len] = '\0'; // Enforce null termination
              if (mifareclassic_WriteDataBlock(1, card_data)) {
                set_success = true;
                AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Data write successful"));
                memcpy(&card_datas,&card_data,sizeof(card_data)); // Cast block 1 to a string
              }
            } else {
              AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Data must be alphanumeric"));
            }
#endif // USE_PN532_DATA_RAW
          }
        } else {
          sprintf(card_datas,"AUTHFAIL");
        }
      }
      switch (pn532_function) {
        case 0x01:
          if (!erase_success) {
            AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Erase fail - exiting erase mode"));
          }
          break;
        case 0x02:
          if (!set_success) {
            AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Write failed - exiting set mode"));
          }
        default:
          break;
      }
      pn532_function = 0;
#endif // USE_PN532_DATA_FUNCTION

#ifdef USE_PN532_DATA_FUNCTION
      ResponseTime_P(PSTR(",\"PN532\":{\"UID\":\"%s\", \"" D_JSON_DATA "\":\"%s\"}}"), uids, card_datas);
#else
      ResponseTime_P(PSTR(",\"PN532\":{\"UID\":\"%s\"}}"), uids);
#endif // USE_PN532_DATA_FUNCTION

      MqttPublishTeleSensor();

#ifdef USE_PN532_CAUSE_EVENTS

      char command[71];
#ifdef USE_PN532_DATA_FUNCTION
      sprintf(command,"backlog event PN532_UID=%s;event PN532_DATA=%s",uids,card_datas);
#else
      sprintf(command,"event PN532_UID=%s",uids);
#endif // USE_PN532_DATA_FUNCTION
      ExecuteCommand(command, SRC_RULE);
#endif // USE_PN532_CAUSE_EVENTS

      pn532_scantimer = 7; // Ignore tags found for two seconds
  }
}

#ifdef USE_PN532_DATA_FUNCTION

bool PN532_Command(void)
{
  bool serviced = true;
  uint8_t paramcount = 0;
  if (XdrvMailbox.data_len > 0) {
    paramcount=1;
  } else {
    serviced = false;
    return serviced;
  }
  char sub_string[XdrvMailbox.data_len];
  char sub_string_tmp[XdrvMailbox.data_len];
  for (uint32_t ca=0;ca<XdrvMailbox.data_len;ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    if (',' == XdrvMailbox.data[ca]) { paramcount++; }
  }
  UpperCase(XdrvMailbox.data,XdrvMailbox.data);
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"E")) {
    pn532_function = 1; // Block 1 of next card/tag will be reset to 0x00...
    AddLog_P(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Next scanned tag data block 1 will be erased"));
    ResponseTime_P(PSTR(",\"PN532\":{\"COMMAND\":\"E\"}}"));
    return serviced;
  }
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"S")) {
    if (paramcount > 1) {
      if (XdrvMailbox.data[XdrvMailbox.data_len-1] == ',') {
        serviced = false;
        return serviced;
      }
      sprintf(sub_string_tmp,subStr(sub_string, XdrvMailbox.data, ",", 2));
      pn532_newdata_len = strlen(sub_string_tmp);
      if (pn532_newdata_len > 15) { pn532_newdata_len = 15; }
      memcpy(&pn532_newdata,&sub_string_tmp,pn532_newdata_len);
      pn532_newdata[pn532_newdata_len] = 0x00; // Null terminate the string
      pn532_function = 2;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("NFC: PN532 NFC - Next scanned tag data block 1 will be set to '%s'"), pn532_newdata);
      ResponseTime_P(PSTR(",\"PN532\":{\"COMMAND\":\"S\"}}"));
      return serviced;
    }
  }
  return false;
}

#endif // USE_PN532_DATA_FUNCTION

bool Xsns40(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      PN532_Init();
      result = true;
      break;
    case FUNC_EVERY_50_MSECOND:
      break;
    case FUNC_EVERY_100_MSECOND:
      break;
    case FUNC_EVERY_250_MSECOND:
      if (pn532_scantimer > 0) {
        pn532_scantimer--;
      } else {
        PN532_ScanForTag();
      }
      break;
    case FUNC_EVERY_SECOND:
      break;
#ifdef USE_PN532_DATA_FUNCTION
    case FUNC_COMMAND_SENSOR:
      if (XSNS_40 == XdrvMailbox.index) {
        result = PN532_Command();
      }
      break;
#endif
  }
  return result;
}

#endif // USE_PN532_HSU
