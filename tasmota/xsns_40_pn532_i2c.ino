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

// fast i2c
#define FAST_I2C Wire.setClock(400000L);
#define SLOW_I2C Wire.setClock(100000L);

// use real i2c eeprom
#define USE_24C256

// EEPROM MACROS
#ifdef USE_24C256
// i2c eeprom
#undef MAX_TAGS
#define MAX_TAGS 1000
#include <Eeprom24C128_256.h>
#define EEPROM_ADDRESS 0x50
static Eeprom24C128_256 eeprom(EEPROM_ADDRESS);
// eeprom.writeBytes(address, length, buffer);
#define EEP_WBYTES(A,B) eeprom.writeBytes(A*sizeof(struct RFID),sizeof(struct RFID),(byte*)B);
// eeprom.readBytes(address, length, buffer);
#define EEP_RBYTES(A,B) eeprom.readBytes(A*sizeof(struct RFID),sizeof(struct RFID),(byte*)B);
#define EEP_INIT(A) eeprom.initialize();
#define EEP_SAVE()
#else
// esp eeprom simulator
#undef MAX_TAGS
#define MAX_TAGS 12
#define EEP_WBYTES(A,B) TAG2EEPROM(A,B);
#define EEP_RBYTES(A,B) EEPROM2TAG(A,B);
#define EEP_INIT(A) EepromBegin(A);
#define EEP_SAVE() EepromCommit();
#endif

struct RFID {
  uint8_t uid[7]; // 4 or 7 bytes UID
  uint8_t bits;   // all bits used
  uint8_t master; // currently 2 bits used
};


// may block config by switch
//#define BLOCK_SWITCH

#define NR_SHORTSECTOR          (32)    // Number of short sectors on Mifare 1K/4K
#define NR_LONGSECTOR           (8)     // Number of long sectors on Mifare 4K
#define NR_BLOCK_OF_SHORTSECTOR (4)     // Number of blocks in a short sector
#define NR_BLOCK_OF_LONGSECTOR  (16)    // Number of blocks in a long sector

// Determine the sector trailer block based on sector number
#define BLOCK_NUMBER_OF_SECTOR_TRAILER(sector) (((sector)<NR_SHORTSECTOR)? \
  ((sector)*NR_BLOCK_OF_SHORTSECTOR + NR_BLOCK_OF_SHORTSECTOR-1):\
  (NR_SHORTSECTOR*NR_BLOCK_OF_SHORTSECTOR + (sector-NR_SHORTSECTOR)*NR_BLOCK_OF_LONGSECTOR + NR_BLOCK_OF_LONGSECTOR-1))

// Determine the sector's first block based on the sector number
#define BLOCK_NUMBER_OF_SECTOR_1ST_BLOCK(sector) (((sector)<NR_SHORTSECTOR)? \
  ((sector)*NR_BLOCK_OF_SHORTSECTOR):\
  (NR_SHORTSECTOR*NR_BLOCK_OF_SHORTSECTOR + (sector-NR_SHORTSECTOR)*NR_BLOCK_OF_LONGSECTOR))


#ifdef LEARN_TAGS


const char S_JSON_PN532_SHOWENTRY[] PROGMEM = "{\"" D_CMND_SENSOR "%d\":%s:%d:%d:%d:%s}";
const char S_JSON_PN532_DEL[] PROGMEM = "{\"" D_CMND_SENSOR "%d\":%s:%d}";

// 2 bits for master mode
#define MASTER_MASK 0x03

// 2 bits length 0 = not used entry
#define LENGTH_MASK 0x03
// 3 bits relays
#define RELAY_MASK  0x1C
#define RELAY_BPOS 2
// 2 bits mode
#define MODE_MASK   0x60
#define MODE_BPOS 5
// 1 bits pulse time info, 2 steps
#define PT_MASK      0x80
#define PT_BPOS 7

#define BITL4 1
#define BITL7 2

// mask UID of learned tags
#define MASK_UID

#define LCMD_LEARN 1
#define LCMD_DELETE 2
#define LCMD_NDF_FORMAT 3
#define LCMD_READ_BLOCK 4


uint8_t learn=0;
uint8_t mtag_active=0;
int16_t pn532_tag_relay;
int16_t pn532_tag_mode;
int16_t pn532_tag_time;

const uint8_t defaultkey[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t keya[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
const uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };

#endif

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
#define PN532_COMMAND_INDATAEXCHANGE      0x40

#define MIFARE_CMD_AUTH_A                 0x60
#define MIFARE_CMD_AUTH_B                 0x61
#define MIFARE_CMD_WRITE                  0xA0
#define MIFARE_CMD_READ                   0x30

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
uint8_t pn532_i2c_command;
uint8_t pn532_i2c_disable = 0;

int16_t PN532_getResponseLength(uint8_t buf[], uint8_t len, uint16_t timeout) {
  const uint8_t PN532_NACK[] = {0, 0, 0xFF, 0xFF, 0, 0};
  uint16_t time = 0;
  do {
    if (Wire.requestFrom(PN532_I2C_ADDRESS, 6)) {
      if (Wire.read() & 1) {  // check first byte --- status
        break;         // PN532 is ready
      }
    }
    delay(1);
    time++;
    if ((0 != timeout) && (time > timeout)) {
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
  uint16_t time = 0;
  uint8_t length;
  uint8_t timeout = 10;

  length = PN532_getResponseLength(buf, len, timeout);

  // [RDY] 00 00 FF LEN LCS (TFI PD0 ... PDn) DCS 00

  do {
    if (Wire.requestFrom(PN532_I2C_ADDRESS, 6 + length + 2)) {
      if (Wire.read() & 1) {  // check first byte --- status
        break;                // PN532 is ready
      }
    }
    delay(1);
    time++;
    if ((0 != timeout) && (time > timeout)) {
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

  uint16_t time = 0;

  do {
    if (Wire.requestFrom(PN532_I2C_ADDRESS,  sizeof(PN532_ACK) + 1)) {
      if (Wire.read() & 1) {  // check first byte --- status
        break;         // PN532 is ready
      }
    }
    delay(1);
    time++;
        if (time > 10) { // We time out after 10ms
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

  Wire.setClockStretchLimit(1000);

  uint32_t ver = PN532_getFirmwareVersion();
  if (ver) {
    pn532_i2c_detected = 1;
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "PN532 NFC Reader (V%u.%u)", PN532_I2C_ADDRESS);
    snprintf_P(log_data, sizeof(log_data), log_data, (ver>>16) & 0xFF, (ver>>8) & 0xFF);
    AddLog(LOG_LEVEL_DEBUG);
    PN532_SAMConfig();

#ifdef LEARN_TAGS
#ifdef USE_24C256
  if (I2cDevice(EEPROM_ADDRESS)) {
    snprintf_P(log_data, sizeof(log_data), "I2C EEPROM not found!");
    AddLog(LOG_LEVEL_DEBUG);
  }
#endif

  struct RFID tag;
    EEP_INIT(sizeof(struct RFID)*MAX_TAGS);
    EEP_RBYTES(0,&tag);
    uint64_t *lptr=(uint64_t *)&tag.uid;
    if (*lptr==0xffffffffffffffff) {
      // assume eeprom not erased to zero
      ClearEEPROM();
    }
#endif
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


uint8_t last_uid[7];  // Buffer to store the returned UID
uint8_t last_uidLength=0;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

void PN532_ScanForTag(void)
{

  if (pn532_i2c_disable) { return; }

  FAST_I2C

  uint16_t found;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uid_len = 0;
  if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
    if (pn532_i2c_scan_defer_report > 0) {
      pn532_i2c_scan_defer_report--;
    } else {
      char uids[15];

#ifdef LEARN_TAGS
      // check for master tags
      found=compare_tag(uid,uid_len);
      if (found&0x8000) {
        if (!mtag_active) {
          snprintf_P(log_data, sizeof(log_data),F("mastertag found => wait for tag"));
          AddLog(LOG_LEVEL_INFO);
          mtag_active=1;
        }
        goto exit;
      }

      if (learn) {

        // in learn mode, store or delete next tag
        int16_t result=learn_tag(uid,uid_len,learn);
        // should report sucess here
        const char *cp;
        if (result==0) {
          if (learn==LCMD_LEARN) cp="tag %s added";
          else cp="tag %s deleted";
        } else {
          cp="tag list is full";
        }
        array_to_hstring(uid,uid_len,uids);
        snprintf_P(log_data, sizeof(log_data),cp,uids);
        AddLog(LOG_LEVEL_INFO);
        learn=0;
        mtag_active=0;
        goto exit;
      }

      // check if known tag
      found=compare_tag(uid,uid_len);
      if (found) {

#ifdef MASK_UID
        // learned, set dummy uid with index
        memset(uid,0,uid_len);
        uid[uid_len-1]=found;
#endif

        // and execute with parameters
        pn532_execute(found);
      }

#endif

      last_uidLength=uid_len;
      memmove(last_uid,uid,uid_len);
      array_to_hstring(uid,uid_len,uids);

      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"PN532\":{\"UID\":\"%s\"}}"), mqtt_data, uids);
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
      char command[27];
      sprintf(command,"event PN532=%s",uids);
      ExecuteCommand(command, SRC_RULE);
      pn532_i2c_scan_defer_report = 7;
    }
  } else {
    if (pn532_i2c_scan_defer_report > 0) { pn532_i2c_scan_defer_report--; }
  }
exit:
  SLOW_I2C
}

#ifdef LEARN_TAGS

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
    memcpy (data, pn532_i2c_packetbuffer + 1, 16);

    return 1;
}


uint8_t mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data)
{
    /* Prepare the first command */
    pn532_i2c_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_i2c_packetbuffer[1] = 1;                      /* Card number */
    pn532_i2c_packetbuffer[2] = MIFARE_CMD_WRITE;       /* Mifare Write command = 0xA0 */
    pn532_i2c_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */
    memcpy (pn532_i2c_packetbuffer + 4, data, 16);        /* Data Payload */

    /* Send the command */
    if (PN532_writeCommand(pn532_i2c_packetbuffer, 20)) {
        return 0;
    }

    /* Read the response packet */
    return (0 < PN532_readResponse(pn532_i2c_packetbuffer, sizeof(pn532_i2c_packetbuffer)));
}

uint8_t mifareclassic_AuthenticateBlock (uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, const uint8_t *keyData)
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
    memcpy (pn532_i2c_packetbuffer + 4, _key, 6);
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

// format mifare to ndef
uint8_t mifareclassic_FormatNDEF (uint8_t *uid, uint8_t uid_len) {
uint8_t success;
  success = mifareclassic_AuthenticateBlock(uid, uid_len, 0, 0,defaultkey);
  if (!success) {
    // ok, format
    uint8_t sectorbuffer1[16] = {0x14, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};
    uint8_t sectorbuffer2[16] = {0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};
    uint8_t sectorbuffer3[16] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77, 0x88, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    // Note 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 must be used for key A
    // for the MAD sector in NDEF records (sector 0)

    // Write block 1 and 2 to the card
    if (!(mifareclassic_WriteDataBlock (1, sectorbuffer1)))
        return 2;
    if (!(mifareclassic_WriteDataBlock (2, sectorbuffer2)))
        return 3;
    // Write key A and access rights card
    if (!(mifareclassic_WriteDataBlock (3, sectorbuffer3)))
        return 4;

    // Seems that everything was OK (?!)
    return 0;
  }
  return 1;
}

// reformat ndef back to mifare
uint8_t NDEF_Reformat_mifareclassic (uint8_t *uid, uint8_t uid_len) {
uint8_t numOfSector = 16,success;
uint8_t blockBuffer[16];
uint8_t blankAccessBits[3] = { 0xff, 0x07, 0x80 };
  // Now run through the card sector by sector
  for (uint8_t idx = 0; idx < numOfSector; idx++) {
    // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    success = mifareclassic_AuthenticateBlock (uid, uid_len, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1, (uint8_t *)defaultkey);
    if (!success) {
      return 2;
    }
    // Step 2: Write to the other blocks
    if (idx == 16)  {
      memset(blockBuffer, 0, sizeof(blockBuffer));
      if (!(mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
        return 3;
      }
    }
    if ((idx == 0) || (idx == 16)) {
      memset(blockBuffer, 0, sizeof(blockBuffer));
      if (!(mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
        return 4;
      }
    }
    else  {
      memset(blockBuffer, 0, sizeof(blockBuffer));
      if (!(mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
        return 5;
      }
      if (!(mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
        return 6;
      }
    }
    memset(blockBuffer, 0, sizeof(blockBuffer));
    if (!(mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer))) {
      return 7;
    }

    // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    memcpy(blockBuffer, defaultkey, sizeof(defaultkey));
    memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
    blockBuffer[9] = 0x69;
    memcpy(blockBuffer + 10, defaultkey, sizeof(defaultkey));

    // Step 4: Write the trailer block
    if (!(mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)), blockBuffer))) {
      return 8;
    }
  }
  return 0;
}

uint8_t mifareclassic_WriteNDEFURI (uint8_t sectorNumber, uint8_t uriIdentifier, const char *url)
{
    // Figure out how long the string is
    uint8_t len = strlen(url);

    // Make sure we're within a 1K limit for the sector number
    if ((sectorNumber < 1) || (sectorNumber > 15))
        return 0;

    // Make sure the URI payload is between 1 and 38 chars
    if ((len < 1) || (len > 38))
        return 0;

    // Note 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 must be used for key A
    // in NDEF records

    // Setup the sector buffer (w/pre-formatted TLV wrapper and NDEF message)
    uint8_t sectorbuffer1[16] = {0x00, 0x00, 0x03, len + 5, 0xD1, 0x01, len + 1, 0x55, uriIdentifier, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t sectorbuffer2[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t sectorbuffer3[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t sectorbuffer4[16] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    if (len <= 6) {
        // Unlikely we'll get a url this short, but why not ...
        memcpy (sectorbuffer1 + 9, url, len);
        sectorbuffer1[len + 9] = 0xFE;
    } else if (len == 7) {
        // 0xFE needs to be wrapped around to next block
        memcpy (sectorbuffer1 + 9, url, len);
        sectorbuffer2[0] = 0xFE;
    } else if ((len > 7) && (len <= 22)) {
        // Url fits in two blocks
        memcpy (sectorbuffer1 + 9, url, 7);
        memcpy (sectorbuffer2, url + 7, len - 7);
        sectorbuffer2[len - 7] = 0xFE;
    } else if (len == 23) {
        // 0xFE needs to be wrapped around to final block
        memcpy (sectorbuffer1 + 9, url, 7);
        memcpy (sectorbuffer2, url + 7, len - 7);
        sectorbuffer3[0] = 0xFE;
    } else {
        // Url fits in three blocks
        memcpy (sectorbuffer1 + 9, url, 7);
        memcpy (sectorbuffer2, url + 7, 16);
        memcpy (sectorbuffer3, url + 23, len - 23);
        sectorbuffer3[len - 23] = 0xFE;
    }

    // Now write all three blocks back to the card
    if (!(mifareclassic_WriteDataBlock (sectorNumber * 4, sectorbuffer1)))
        return 0;
    if (!(mifareclassic_WriteDataBlock ((sectorNumber * 4) + 1, sectorbuffer2)))
        return 0;
    if (!(mifareclassic_WriteDataBlock ((sectorNumber * 4) + 2, sectorbuffer3)))
        return 0;
    if (!(mifareclassic_WriteDataBlock ((sectorNumber * 4) + 3, sectorbuffer4)))
        return 0;

    // Seems that everything was OK (?!)
    return 1;
}

// execute tag cmd
void pn532_execute(uint8_t index) {
  uint8_t mode,relay,time;
  struct RFID tag;
  index--;
  EEP_RBYTES(index,&tag);
  //relay=(PN532_Settings.tags[index].bits&RELAY_MASK)>>RELAY_BPOS;
  relay=(tag.bits&RELAY_MASK)>>RELAY_BPOS;
  relay++;
  //mode=(PN532_Settings.tags[index].bits&MODE_MASK)>>MODE_BPOS;
  mode=(tag.bits&MODE_MASK)>>MODE_BPOS;
  //time=(PN532_Settings.tags[index].bits&PT_MASK)>>PT_BPOS;
  time=(tag.bits&PT_MASK)>>PT_BPOS;
  switch (mode&3) {
    case 0:
      // relay off
      ExecuteCommandPower(relay, POWER_OFF, SRC_BUTTON);
      break;
    case 1:
      // relay on
      ExecuteCommandPower(relay, POWER_ON, SRC_BUTTON);
      break;
    case 2:
      // toggle relay
      ExecuteCommandPower(relay, POWER_TOGGLE, SRC_BUTTON);
      break;
    case 3:
      // pulse relay 500 or 1000 ms
      ExecuteCommandPower(relay, POWER_ON, SRC_BUTTON);
      delay((time+1)*500);
      ExecuteCommandPower(relay, POWER_OFF, SRC_BUTTON);
      break;
  }
}

// returns entry number from 1-N or zero
uint16_t compare_tag(uint8_t *uid,uint8_t uidLength) {
uint16_t count,len;
uint16_t result=0;
struct RFID tag;
#ifdef USE_24C256
//FAST_I2C
#endif
  for (count=0; count<MAX_TAGS; count++) {
    EEP_RBYTES(count,&tag);
    len=tag.bits&LENGTH_MASK;
    if (len) {
      if (!memcmp(uid,tag.uid,uidLength)) {
        // did match
        if (tag.master&MASTER_MASK) {
          // setup master params
          learn=tag.master&MASTER_MASK;
          pn532_tag_relay=(tag.bits&RELAY_MASK)>>RELAY_BPOS;
          pn532_tag_relay++;
          pn532_tag_mode=(tag.bits&MODE_MASK)>>MODE_BPOS;
          pn532_tag_time=(tag.bits&PT_MASK)>>PT_BPOS;
          pn532_tag_time++;
          result=(count+1)|0x8000;
          goto exit;
        }
        else  {
          result=(count+1);
          goto exit;
        }
      }
    }
  }

exit:
#ifdef USE_24C256
//SLOW_I2C
#endif
  return result;
}

// learn or delete tag mode=1 => learn, mode=2 => delete
int16_t learn_tag(uint8_t *uid,uint8_t uidLength,uint8_t mode) {
uint16_t count,len;
uint8_t bits;
int16_t result=0;
struct RFID tag;
uint8_t mmode=mode>>4;

#ifdef USE_24C256
  //FAST_I2C
#endif

  mode&=3;
  // compare if already there
  uint8_t found=0;
  for (count=0; count<MAX_TAGS; count++) {
    EEP_RBYTES(count,&tag);
    len=tag.bits&LENGTH_MASK;
    if (len) {
      if (!memcmp(uid,tag.uid,uidLength)) {
        found=count+1;
        break;
      }
    }
  }
  if (found) {
    if (mode==1) {
      // replace old entry
      memmove(tag.uid,uid,uidLength);
      if (uidLength==4) bits=BITL4;
      else bits=BITL7;
      bits|=(pn532_tag_relay-1)<<RELAY_BPOS;
      bits|=(pn532_tag_mode)<<MODE_BPOS;
      bits|=(pn532_tag_time-1)<<PT_BPOS;
      tag.bits=bits;
      if (mmode) tag.master=mmode;
      else tag.master=0;

      EEP_WBYTES(found-1,&tag);
    } else {
      // delete old entry
      tag.bits=0;
      tag.master=0;
      EEP_WBYTES(found-1,&tag);
    }
  } else {
    // look for free entry
    found=0;
    for (count=0; count<MAX_TAGS; count++) {
      EEP_RBYTES(count,&tag);
      if (!tag.bits) {
        // copy new entry
        memmove(tag.uid,uid,uidLength);
        if (uidLength==4) bits=BITL4;
        else bits=BITL7;
        bits|=(pn532_tag_relay-1)<<RELAY_BPOS;
        bits|=pn532_tag_mode<<MODE_BPOS;
        bits|=(pn532_tag_time-1)<<PT_BPOS;
        tag.bits=bits;
        if (mmode) tag.master=mmode;
        else tag.master=0;
        EEP_WBYTES(count,&tag);
        found=1;
        break;
      }
    }
    if (!found) {
      // list is full
      result=-1;
      goto exit;
    }
  }
  EEP_SAVE();
exit:
#ifdef USE_24C256
  //SLOW_I2C
#endif
  return result;
}

#ifndef USE_24C256

// read a tag entry
void EEPROM2TAG(uint8_t index,struct RFID *rfid) {
  uint16_t offset=index*sizeof(struct RFID);
  uint8_t *mptr=(uint8_t*)rfid;
  for (uint16_t count=0; count<sizeof(struct RFID); count++) {
    *mptr++=EepromRead(offset+count);
  }
}

// write a tag entry
void TAG2EEPROM(uint8_t index,struct RFID *rfid) {
  uint16_t offset=index*sizeof(struct RFID);
  uint8_t *mptr=(uint8_t*)rfid;
  for (uint16_t count=0; count<sizeof(struct RFID); count++) {
    EepromWrite(offset+count, *mptr++);
  }
}
#endif

// clear eeprom to zero
void ClearEEPROM(void) {
  struct RFID tag;
#ifdef USE_24C256
  FAST_I2C
#endif
  memset(&tag,0,sizeof(struct RFID));
  for (uint16_t count=0; count<MAX_TAGS; count++) {
    EEP_WBYTES(count,&tag);
  }
  EEP_SAVE();
#ifdef USE_24C256
  SLOW_I2C
#endif
}

// get asci number until delimiter and return asci number lenght and value
uint8_t pn532_atoiv(char *cp, int16_t *res)
{
  uint8_t index = 0;
  // skip leading spaces
  while (*cp==' ') {
    cp++;
    index++;
  }

  *res = atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// execute sensor40 cmds
bool PN532_cmd(void)
{
  boolean serviced = true;
  uint8_t var;
  int16_t wvar;
  char uid_str[40];
  struct RFID tag;
  uint8_t uid[7],uid_len=0;

#ifdef BLOCK_SWITCH
  // block cmds with switch 1
  if (pin[GPIO_SWT1]<99) {
      if (digitalRead(pin[GPIO_SWT1])) {
        return false;
      }
  }
#endif

  if (XdrvMailbox.data_len > 0) {
    char *cp=XdrvMailbox.data;
    if (*cp=='a' || *cp=='A') {
      uint8_t amode=0;
      // add tag  => a relaynr mode time and wait for tag
      // add tag with UID  => A(M) UID relaynr mode time
      // mode 0 = off, 1 = on, 2 = toggle, 3 = pulse with time in ms, bit 7 => report as index UID
      if (*cp=='A') {
        // provide UID
        cp++;
        if (*cp=='M' || *cp=='D') {
          if (*cp=='M') amode=1; //  master learn
          else amode=2; // master delete
          cp++;
        } else {
          amode=3;
        }

        while (*cp==' ') cp++;
        for (uint8_t count=0;count<sizeof(uid_str);count++) {
          if (*cp!=0 && *cp!=' ') {
            uid_str[count]=*cp++;
          } else {
            uid_len=count;
            break;
          }
        }

        if (uid_len==8 || uid_len==14) {
          // convert to uint8 array
          uid_len/=2;
          hstring_to_array(uid,uid_len,uid_str);
        }
        else {
          // ill UID format
          return false;
        }
      } else {
        cp++;
      }

      // relay
      if (*cp) {
        var = pn532_atoiv(cp, &pn532_tag_relay);
        cp += var;
      }
      if (pn532_tag_relay<1 || pn532_tag_relay>8) pn532_tag_relay=1;

      // mode
      if (*cp) {
        var = pn532_atoiv(cp, &pn532_tag_mode);
        cp += var;
      }
      pn532_tag_mode&=3;

      // time
      if (*cp) {
        var = pn532_atoiv(cp, &pn532_tag_time);
        cp += var;
      }
      if (pn532_tag_time<1 || pn532_tag_time>2) pn532_tag_time=1;

      if (!amode) {
        // learn mode
        learn=LCMD_LEARN;
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_SHOWENTRY, XSNS_40,"wait for tag",pn532_tag_relay,pn532_tag_mode,pn532_tag_time,"T");
      } else {
        // immediate mode
        uint8_t mode=1;
        if (amode==1 || amode==2) mode|=(amode<<4);
        int16_t result=learn_tag(uid,uid_len,mode);
        // show result
        const char *msg;
        if (result==0) {
          msg="tag added";
        } else {
          msg="tag list full";
        }
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_SHOWENTRY, XSNS_40,msg,pn532_tag_relay,pn532_tag_mode,pn532_tag_time,"T");
      }
    } else if (*cp=='d') {
      // delete entry num  => d num
      cp++;
      if (*cp) {
        var = pn532_atoiv(cp, &wvar);
        cp += var;
        if (wvar>=1 && wvar<=MAX_TAGS) {
          EEP_RBYTES(wvar-1,&tag);
          //PN532_Settings.tags[wvar-1].bits=0;
          tag.bits=0;
          EEP_WBYTES(wvar-1,&tag);
          EEP_SAVE();
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"delete tag index",wvar);
    } else if (*cp=='D') {
      // delete tag => D and wait for tag
      cp++;
      learn=LCMD_DELETE;
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"wait for tag to delete",0);
    }
    else if (*cp=='s') {
      // show entry num => s index
      cp++;
      if (*cp) {
        var = pn532_atoiv(cp, &wvar);
        cp += var;
        if (wvar>=1 && wvar<=MAX_TAGS) {
          wvar--;
          uint8_t len;
          EEP_RBYTES(wvar,&tag);
          if ((tag.bits&LENGTH_MASK)==BITL4) len=4;
          else len=7;

          if (!tag.bits) strcpy(uid_str,(const char*)"--------");
          else array_to_hstring(tag.uid,len,uid_str);
          const char *mm;
          if ((tag.master&MASTER_MASK)==1) mm="M";
          else if ((tag.master&MASTER_MASK)==2) mm="D";
          else mm="T";
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_SHOWENTRY, XSNS_40,uid_str,(((tag.bits&RELAY_MASK)>>RELAY_BPOS)&7)+1,((tag.bits&MODE_MASK)>>MODE_BPOS)&3,((tag.bits&PT_MASK)>>PT_BPOS)+1,mm);
        }
      }
    }
    else if (*cp=='e') {
      // erase list
      ClearEEPROM();
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"tag list erased",0);
    }
    else if (*cp=='f') {
      cp++;
      if (*cp=='m') {
        // reformat to mifare
        uint8_t error=0;
        if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
          error = NDEF_Reformat_mifareclassic(uid, uid_len);
          if (error) goto fmerror;
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"tag format to MIFARE OK",0);
        } else {
          error=1;
          fmerror:
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"tag format failed: ",error);
        }
      } else {
        // format card to NDEF
        uint8_t error;
        if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
          error = mifareclassic_FormatNDEF(uid, uid_len);
          if (error) goto ferror;
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"tag format to NDEF OK",0);
        } else {
          // error
          error=1;
          ferror:
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"tag format failed: ",error);
        }
      }
    }
    else if (*cp=='r') {
        // read block
        cp++;
        if (*cp) {
          var = pn532_atoiv(cp, &wvar);
          cp += var;
          if (wvar>=0 && wvar<64) {
            uint8_t error;
            if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
              uint8_t result = mifareclassic_AuthenticateBlock(uid, uid_len, wvar, 0, defaultkey);
              if (!result) {
                // use keyb
                result = mifareclassic_AuthenticateBlock(uid, uid_len, wvar, 0, keyb);
                if (!result) {
                  error=2;
                  goto rerror;
                }
              }
              uint8_t data[16];
              result = mifareclassic_ReadDataBlock(wvar, data);
              if (!result) {
                error=3;
                goto rerror;
              }
              array_to_hstring(data,sizeof(data),uid_str);
              snprintf_P(mqtt_data, sizeof(mqtt_data), "{\"" D_CMND_SENSOR "%d\":%s: %d - %s}", XSNS_40,"tag block read",wvar,uid_str);
            } else {
              // error
              error=1;
              rerror:
              snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"read block failed: ",error);
            }
          }
        }
    }
    else if (*cp=='w') {
        // write block
        cp++;
        if (*cp) {
          var = pn532_atoiv(cp, &wvar);
          cp += var;
          if (wvar<0 || wvar>63) wvar=0;
          // get block data as hex
          while (*cp==' ') cp++;
          for (uint8_t count=0;count<32;count++) {
            if (*cp!=0 && *cp!=' ') {
              uid_str[count]=*cp++;
            } else {
              uid_len=count;
              break;
            }
          }
          uid_len/=2;
          if (uid_len>16) uid_len=16;
          uint8_t data[18];
          memset(data,0,sizeof(data));
          hstring_to_array(data,uid_len,uid_str);
          uint8_t error;
          if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
            uint8_t result = mifareclassic_AuthenticateBlock(uid, uid_len, wvar, 0, defaultkey);
            if (!result) {
              error=2;
              goto werror;
            }
            result = mifareclassic_WriteDataBlock(wvar, data);
            if (!result) {
              error=3;
              goto werror;
            }
            // reread block
            result = mifareclassic_ReadDataBlock(wvar, data);
            if (!result) {
              error=4;
              goto werror;
            }
            array_to_hstring(data,sizeof(data),uid_str);
            snprintf_P(mqtt_data, sizeof(mqtt_data), "{\"" D_CMND_SENSOR "%d\":%s: %d - %s}", XSNS_40,"tag block written",wvar,uid_str);
          } else {
            // error
            error=1;
            werror:
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"write block failed: ",error);
          }
        }
    } else if (*cp=='n') {
        // write ndef entry n sector uri_id url
        int16_t uri_id=0,keyf=0;
        cp++;
        if (*cp=='u') {
          keyf=1;
          cp++;
        }
        if (*cp) {
          var = pn532_atoiv(cp, &wvar);
          cp += var;
          if (wvar<1 || wvar>15) wvar=1;

          var = pn532_atoiv(cp, &uri_id);
          cp += var;

          // copy asci data
          while (*cp==' ') cp++;
          for (uint8_t count=0;count<32;count++) {
            if (*cp!=0) {
              uid_str[count]=*cp++;
            } else {
              uid_str[count]=0;
              uid_len=count;
              break;
            }
          }
          uint8_t error;
          uint8_t result;
          if (PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_len)) {
            if (keyf) result = mifareclassic_AuthenticateBlock(uid, uid_len, 4, 0, keyb);
            else result = mifareclassic_AuthenticateBlock(uid, uid_len, 4, 0, defaultkey);
            if (!result) {
              error=2;
              goto nerror;
            }
            result = mifareclassic_WriteNDEFURI (wvar,uri_id,uid_str);
            if (!result) {
              error=3;
              goto nerror;
            }
            snprintf_P(mqtt_data, sizeof(mqtt_data), "{\"" D_CMND_SENSOR "%d\":%s: %d : %d - %s}", XSNS_40,"NDEF data written",wvar,uri_id,uid_str);
          } else {
            // error
            error=1;
            nerror:
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_PN532_DEL, XSNS_40,"NDEF data write failed: ",error);
          }
        }
    }
    else   {
      // other option
    }
  }

  return serviced;
}
#endif

// convert a byte array to a hex string
// sprintf hex formatter not supported in all versions of arduino ????
void array_to_hstring(uint8_t array[], uint8_t len, char buffer[])
{

#if 1
    for (uint8_t i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
#else
    char *cp=buffer;
    *cp=0;
    for (uint8_t i = 0;i < len;i++) {
      sprintf(cp,"%s%02X",cp,array[i]);
    }
#endif
}


uint8_t pn_hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
    if (chr >= 'a' && chr <= 'f') rVal = chr + 10 - 'a';
  }
  return rVal;
}

// convert hex string to int array
void hstring_to_array(uint8_t array[], uint8_t len, char buffer[])
{
  char *cp=buffer;
  for (uint8_t i = 0; i < len; i++) {
    uint8_t val = pn_hexnibble(*cp++) << 4;
    array[i]= val | pn_hexnibble(*cp++);
  }
}


#ifdef USE_WEBSERVER
const char HTTP_PN532[] PROGMEM =
 "{s}PN532 " "UID" "{m}%s" "{e}";

void PN532_Show(void) {
char uid_str[16];
char b_mqtt_data[256];
b_mqtt_data[0]=0;

	if (!last_uidLength) {
    // shows "----" if ic not detected
    if (pn532_i2c_detected) strcpy(uid_str,"????");
    else strcpy(uid_str,(const char*)"----");
  }
  else array_to_hstring(last_uid,last_uidLength,uid_str);

  WSContentSend_PD(HTTP_PN532,uid_str);

}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns40(byte function)
{
  bool result = false;

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
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        PN532_Show();
        break;
#endif  // USE_WEBSERVER

#ifdef LEARN_TAGS
      case FUNC_COMMAND_SENSOR:
        if (XSNS_40 == XdrvMailbox.index) {
          result = PN532_cmd();
        }
        break;
#endif

    // this does not work, because save cmd calls it
      case FUNC_SAVE_BEFORE_RESTART:
        if (!pn532_i2c_disable) {
          //pn532_i2c_disable = 1;
          snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "PN532 NFC Reader - Disabling for reboot", PN532_I2C_ADDRESS);
          AddLog(LOG_LEVEL_DEBUG);
        }
        break;
    }

  }
  return result;
}

#endif  // USE_PN532_I2C
#endif  // USE_I2C
