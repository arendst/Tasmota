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

const uint8_t PIN_ZIGBEE_BOOTLOADER = 5;

struct ZBUPLOAD {
  uint32_t ota_size = 0;
  uint32_t sector_cursor = 0;
  uint32_t sector_counter = 0;
  bool ota_ready = false;
} ZbUpload;

bool ZigbeeUploadOtaReady(void) {
  return ZbUpload.ota_ready;
}

uint32_t ZigbeeUploadFlashStart(void) {
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;
}

uint8_t ZigbeeUploadInit(void) {
  if (!PinUsed(GPIO_ZIGBEE_RST) && (ZigbeeSerial == nullptr)) { return 1; }  // Wrong pin configuration - No file selected

  ZbUpload.sector_counter = ZigbeeUploadFlashStart();
  ZbUpload.sector_cursor = 0;
  ZbUpload.ota_size = 0;
  ZbUpload.ota_ready = false;
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
  ZbUpload.ota_ready = true;
}

void ZigbeeUploadSetBootloader(uint8_t state) {
  pinMode(PIN_ZIGBEE_BOOTLOADER, OUTPUT);
  digitalWrite(PIN_ZIGBEE_BOOTLOADER, state);  // Toggle Gecko bootloader
  digitalWrite(Pin(GPIO_ZIGBEE_RST), 0);
  delay(100);                                  // Need to experiment to find a value as low as possible
  digitalWrite(Pin(GPIO_ZIGBEE_RST), 1);       // Reboot MCU EFR32
}

void ZigbeeUploadXmodem(void) {
  if (!ZbUpload.ota_ready) { return; }

  // Copy uploaded OTA file from ESP8266 flash to MCU EFR32 using xmodem
  uint32_t sector_counter = ZigbeeUploadFlashStart() * SPI_FLASH_SEC_SIZE;

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZIG: Upload start 0x%08X, size 0x%08X"), sector_counter, ZbUpload.ota_size);

  // TODO - implement XMODEM upload

//  ZigbeeUploadSetBootloader(1);                // Reboot MCU EFR32 which returns below text
  // Gecko Bootloader v1.A.3
  // 1. upload gbl
  // 2. run
  // 3. ebl info
  // BL >
  // Use ZigbeeInputLoop to flush received data

  // Send option 1 to prepare for xmodem upload
//  ZigbeeSerial->write('1');
//  ZigbeeSerial->write(0x0d);
//  ZigbeeSerial->write(0x0a);

  // Start xmodem upload

  ZbUpload.ota_ready = false;
//  ZigbeeUploadSetBootloader(0);               // Disable bootloader and reset MCU
  restart_flag = 2;  // Restart to disable bootloader and use new firmware
}

#endif // USE_ZIGBEE_EZSP

#endif // USE_ZIGBEE
