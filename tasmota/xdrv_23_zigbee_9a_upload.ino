/*
  xdrv_23_zigbee_9_serial.ino - zigbee: serial communication with MCU

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

#ifdef USE_ZIGBEE_EZSP
/*********************************************************************************************\
 * MCU EFR32 firmware upload using xmodem
 *
 * Step 1 - Upload MCU firmware in ESP8266 flash free space (current size is about 200k)
 * Step 2 - Upload MCU firmware from ESP8266 flash to MCU EFR32 using XMODEM protocol
 * Step 3 - Restart
\*********************************************************************************************/

#define XM_SOH     0x01
#define XM_STX     0x02
#define XM_EOT     0x04
#define XM_ENQ     0x05
#define XM_ACK     0x06
#define XM_LF      0x0a
#define XM_CR      0x0d
#define XM_DLE     0x10
#define XM_XON     0x11
#define XM_XOFF    0x13
#define XM_NAK     0x15
#define XM_CAN     0x18
#define XM_EOF     0x1a

enum ZbUploadSteps { ZBU_IDLE, ZBU_INIT, ZBU_PROMPT, ZBU_SYNC, ZBU_UPLOAD, ZBU_DONE, ZBU_COMPLETE };

const uint8_t PIN_ZIGBEE_BOOTLOADER = 5;

struct ZBUPLOAD {
  uint32_t ota_size = 0;
  uint32_t sector_cursor = 0;
  uint32_t sector_counter = 0;
  uint32_t byte_counter = 0;
  char *buffer;
  uint8_t ota_step = ZBU_IDLE;
} ZbUpload;

/*********************************************************************************************\
 * Flash
\*********************************************************************************************/

uint32_t ZigbeeUploadFlashStart(void) {
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;
}

uint32_t ZigbeeUploadAvailable(void) {
  int available = ZbUpload.ota_size - ZbUpload.byte_counter;
  if (available < 0) { available = 0; }
  return available;
}

char ZigbeeUploadFlashRead(void) {
  if (0 == ZbUpload.byte_counter) {
    if (!(ZbUpload.buffer = (char *)malloc(SPI_FLASH_SEC_SIZE))) {
      return (-1);  // Not enough (memory) space
    }
    ZbUpload.sector_counter = ZigbeeUploadFlashStart();
  }

  uint32_t index = ZbUpload.byte_counter % SPI_FLASH_SEC_SIZE;
  if (0 == index) {
    ESP.flashRead(ZbUpload.sector_counter * SPI_FLASH_SEC_SIZE, (uint32_t*)ZbUpload.buffer, SPI_FLASH_SEC_SIZE);
    ZbUpload.sector_counter++;
  }

  char data = ZbUpload.buffer[index];
  ZbUpload.byte_counter++;

  if (ZbUpload.byte_counter > ZbUpload.ota_size) {
    data = XM_EOF;
//    if (ZbUpload.buffer) { free(ZbUpload.buffer); }  // Don't in case of retries
  }
  return data;
}

/*********************************************************************************************\
 * XModem protocol
\*********************************************************************************************/

// Number of seconds until giving up hope of receiving sync packets from host.
const uint8_t XMODEM_SYNC_TIMEOUT = 30;
// Number of times we try to send a packet to the host until we give up sending..
const uint8_t XMODEM_MAX_RETRY = 30;
// Packet size
const uint8_t XMODEM_PACKET_SIZE = 128;

struct XMODEM {
  uint32_t timeout = 0;
  uint32_t filepos = 0;
  int crcBuf = 0;
  uint8_t packetNo = 1;
  uint8_t checksumBuf = 0;
  bool oldChecksum;
} XModem;

// Send out a byte of payload data, includes checksumming
void XModemOutputByte(uint8_t out_char) {
  XModem.checksumBuf += out_char;

  XModem.crcBuf = XModem.crcBuf ^ (int) out_char << 8;
  for (uint32_t i = 0; i < 8; i++) {
    if (XModem.crcBuf & 0x8000) {
      XModem.crcBuf = XModem.crcBuf << 1 ^ 0x1021;
    } else {
      XModem.crcBuf = XModem.crcBuf << 1;
    }
  }

  ZigbeeSerial->write(out_char);
}

// Wait for the remote to acknowledge or cancel.
// Returns the received char if no timeout occured or a CAN was received. In this cases, it returns -1.
char XModemWaitACK(void)
{
  char in_char;
  do {
    uint8_t i = 0;
    while (!ZigbeeSerial->available()) {
      delayMicroseconds(100);
      i++;
      if (i > 200) { return -1; }
    }
    in_char = ZigbeeSerial->read();
    if (in_char == XM_CAN) { return XM_CAN; }
  } while ((in_char != XM_NAK) && (in_char != XM_ACK) && (in_char != 'C'));
  return in_char;
}

bool XModemSendPacket(uint32_t packet_no) {
  XModem.filepos = ZbUpload.byte_counter;

  // Sending a packet will be retried
  uint32_t retries = 0;
  char in_char;
  do {
    // Seek to start of current data block,
    // will advance through the file as block will be acked..
    ZbUpload.byte_counter = XModem.filepos;

    // Reset checksum stuff
    XModem.checksumBuf = 0x00;
    XModem.crcBuf = 0x00;

    // Try to send packet, so header first
    ZigbeeSerial->write(XM_SOH);
    ZigbeeSerial->write(packet_no);
    ZigbeeSerial->write(~packet_no);
    for (uint32_t i = 0; i < XMODEM_PACKET_SIZE; i++) {
      in_char = ZigbeeUploadFlashRead();
      XModemOutputByte(in_char);
    }
    // Send out checksum, either CRC-16 CCITT or classical inverse of sum of bytes.
    // Depending on how the received introduced himself
    if (XModem.oldChecksum) {
      ZigbeeSerial->write((char)XModem.checksumBuf);
    } else {
      ZigbeeSerial->write((char)(XModem.crcBuf >> 8));
      ZigbeeSerial->write((char)(XModem.crcBuf & 0xFF));
    }
    in_char = XModemWaitACK();
    if (XM_CAN == in_char) { return false; }
    retries++;
    if (retries > XMODEM_MAX_RETRY) { return false; }
  } while (in_char != XM_ACK);
  return true;
}

bool XModemSendEOT(void) {
  // Send EOT and wait for ACK
  uint32_t retries = 0;
  char in_char;
  do {
    ZigbeeSerial->write(XM_EOT);
    in_char = XModemWaitACK();
    retries++;
    // When timed out, leave immediately
    if (retries == XMODEM_SYNC_TIMEOUT) { return false; }
  } while (in_char != XM_ACK);
  return true;
}

/*********************************************************************************************\
 * Step 2 - Upload MCU firmware from ESP8266 flash to MCU EFR32 using XMODEM protocol
 *
 * https://www.silabs.com/documents/public/application-notes/an760-using-legacy-standalone-bootloader.pdf
\*********************************************************************************************/

void ZigbeeUploadSetBootloader(uint8_t state) {
  pinMode(PIN_ZIGBEE_BOOTLOADER, OUTPUT);
  digitalWrite(PIN_ZIGBEE_BOOTLOADER, state);  // Toggle Gecko bootloader
  digitalWrite(Pin(GPIO_ZIGBEE_RST), 0);
  delay(100);                                  // Need to experiment to find a value as low as possible
  digitalWrite(Pin(GPIO_ZIGBEE_RST), 1);       // Reboot MCU EFR32
}

void ZigbeeUploadBootloaderDone(void) {
  ZbUpload.ota_step = ZBU_COMPLETE;  // Never return to zero without a restart to get a sane Zigbee environment
  ZigbeeUploadSetBootloader(0);  // Disable bootloader and reset MCU - should happen or restart
  restart_flag = 2;              // Restart to disable bootloader and use new firmware
}

bool ZigbeeUploadBootloaderPrompt(void) {
  // Scripts that interact with the bootloader should use only the “BL >” prompt to determine
  // when the bootloader is ready for input. While current menu options should remain functionally
  // unchanged, the menu title and options text is liable to change, and new options might be added.
  while (ZigbeeSerial->available()) {
    yield();
    char bootloader_byte = ZigbeeSerial->read();
    switch (ZbUpload.byte_counter) {
      case 0:
        if ('B' == bootloader_byte) { ZbUpload.byte_counter++; } break;
      case 1:
        if ('L' == bootloader_byte) { ZbUpload.byte_counter++; } break;
      case 2:
        if (' ' == bootloader_byte) { ZbUpload.byte_counter++; } break;
      case 3:
        if ('>' == bootloader_byte) { ZbUpload.byte_counter++; }
    }
  }
  return (4 == ZbUpload.byte_counter);
}

bool ZigbeeUploadXmodem(void) {
  if (!ZbUpload.ota_step) { return false; }

  switch (ZbUpload.ota_step) {
    case ZBU_INIT: {  // Init ESF32 bootloader
      ZbUpload.ota_step = ZBU_PROMPT;

      uint32_t sector_counter = ZigbeeUploadFlashStart() * SPI_FLASH_SEC_SIZE;
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZIG: Upload 0x%08X, size 0x%08X"), sector_counter, ZbUpload.ota_size);

      ZigbeeUploadSetBootloader(1);                // Reboot MCU EFR32 which returns below text
      break;
    }
    case ZBU_PROMPT: {  // Wait for prompt and select option upload ebl
      if (!ZigbeeSerial->available()) {
        // The target device’s bootloader sends output over its serial port after it receives a carriage return
        // from the source device
        ZigbeeSerial->write(XM_CR);
        delay(1);
      } else {
        // After the bootloader receives a carriage return from the target device, it displays a menu
        // Gecko Bootloader v1.A.3
        // 1. upload gbl
        // 2. run
        // 3. ebl info
        // BL >
        if (ZigbeeUploadBootloaderPrompt()) {
          ZigbeeSerial->flush();
          ZigbeeSerial->write('1');  // upload ebl
          XModem.timeout = millis() + (XMODEM_SYNC_TIMEOUT * 1000);
          ZbUpload.ota_step = ZBU_SYNC;
        }
      }
      break;
    }
    case ZBU_SYNC: {  // Handle file upload using XModem - sync
      if (millis() > XModem.timeout) {
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("XMD: Initial sync failed"));
        ZigbeeUploadBootloaderDone();
        return true;
      }
      // Wait for either C or NACK as a sync packet.
      // Determines protocol details, checksum algorithm.
      if (ZigbeeSerial->available()) {
        char xmodem_sync = ZigbeeSerial->read();
        if ((xmodem_sync == 'C') || (xmodem_sync == XM_NAK)) {
          // Determine which checksum algorithm to use
          XModem.oldChecksum = (xmodem_sync == XM_NAK);
          XModem.packetNo = 1;
          ZbUpload.byte_counter = 0;
          ZbUpload.ota_step = ZBU_UPLOAD;
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("XMD: Sync received"));
        }
      }
      break;
    }
    case ZBU_UPLOAD: {  // Handle file upload using XModem - upload
      if (ZigbeeUploadAvailable()) {
        if (!XModemSendPacket(XModem.packetNo)) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("XMD: Packet send failed"));
          ZigbeeUploadBootloaderDone();
          return true;
        }
        XModem.packetNo++;
      } else {
        if (!XModemSendEOT()) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("XMD: EOT failed"));
          ZigbeeUploadBootloaderDone();
          return true;
        }
        ZbUpload.ota_step = ZBU_DONE;
      }
      break;
    }
    case ZBU_DONE: {  // Clean up and restart to disable bootloader and use new firmware
      ZigbeeUploadBootloaderDone();
    }
  }
  return true;
}

/*********************************************************************************************\
 * Step 1 - Upload MCU firmware in ESP8266 flash free space (current size is about 200k)
\*********************************************************************************************/

bool ZigbeeUploadOtaReady(void) {
  return (ZBU_INIT == ZbUpload.ota_step);
}

uint8_t ZigbeeUploadInit(void) {
  if (!PinUsed(GPIO_ZIGBEE_RST) && (ZigbeeSerial == nullptr)) { return 1; }  // Wrong pin configuration - No file selected

  ZbUpload.sector_counter = ZigbeeUploadFlashStart();
  ZbUpload.sector_cursor = 0;
  ZbUpload.ota_size = 0;
  ZbUpload.ota_step = ZBU_IDLE;
  return 0;
}

bool ZigbeeUploadWriteBuffer(uint8_t *buf, size_t size) {
  // Read complete file into ESP8266 flash
  // Current files are about 200k
  if (0 == ZbUpload.sector_cursor) {  // Starting a new sector write so we need to erase it first
    ESP.flashEraseSector(ZbUpload.sector_counter);
  }
  ZbUpload.sector_cursor++;
  ESP.flashWrite((ZbUpload.sector_counter * SPI_FLASH_SEC_SIZE) + ((ZbUpload.sector_cursor-1) * 2048), (uint32_t*)buf, size);
  ZbUpload.ota_size += size;
  if (2 == ZbUpload.sector_cursor) {  // The web upload sends 2048 bytes at a time so keep track of the cursor position to reset it for the next flash sector erase
    ZbUpload.sector_cursor = 0;
    ZbUpload.sector_counter++;
    if (ZbUpload.sector_counter > (SPIFFS_END + 12)) {
      return false;  // File too large - Not enough free space
    }
  }
  return true;
}

void ZigbeeUploadDone(void) {
  ZbUpload.ota_step = ZBU_INIT;
}

#endif // USE_ZIGBEE_EZSP

#endif // USE_ZIGBEE
