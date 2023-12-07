/*
  xdrv_16_tuyamcu.ino - Tuya MCU support for Tasmota

  Copyright (C) 2021  Federico Leoni, digiblur, Joel Stein and Theo Arends

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
#ifndef _TUYA_MCU_UPGRADE_H_
#define _TUYA_MCU_UPGRADE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_LIGHT
#ifdef USE_TUYA_MCU
#ifdef USE_TUYA_MCU_UPGRADE

class TuyaUpgBuffer final {
public:
  TuyaUpgBuffer (void);
  ~TuyaUpgBuffer (void);
  bool init (uint32_t len = 0, char* checksum = nullptr, char* filename = NULL);
  void reset (void);
  uint32_t writeToFlashOrFile(Stream &data);
  bool isChecksumOk (void);
  void resetPosition (void);
  bool hasMoreData (void);
  uint32_t bytesRead (void);
  uint32_t getPackageOffset (void);
  void setPacketSize (uint16_t value);
  bool readNextPacket (void);
  void getCurrentPacket (uint8_t *toBuffer);
  uint16_t getCurrentPacketSize (void);

private:
  uint32_t remaining (void);
  void abort (void);

#ifdef ESP8266
  bool eraseSector (void);
#endif
  bool writeBuffer (void);
  bool readToBuffer (void);

  static const uint8_t MAX_RETRIES = 3;
  uint32_t _start = 0;
  uint32_t _end = 0;
  uint32_t _num_sectors = 0;
  uint32_t _current_address = 0;
  uint32_t _len = 0;
  uint32_t _buffer_size = 0;
  uint32_t _buffer_pos = 0;
  uint8_t *_buffer = nullptr;
  br_md5_context *_md5_context = nullptr;
  char* _checksum = nullptr;
  uint16_t _packet_size = 0;
  uint8_t *_current_packet = nullptr;
  uint16_t _current_packet_size = 0;
#ifdef ESP8266
  static const uint32_t _flash_sector_size = FLASH_SECTOR_SIZE;
#elif defined ESP32
  static const uint32_t _flash_sector_size = 4096;
  char *_fname = nullptr;
#endif
};

// parameters used for MCU upgrade
struct MCU_UPGRADE_DATA {
  WiFiClient *wifi_client = nullptr;
  HTTPClient *http_client = nullptr;
  class TuyaUpgBuffer flash_buffer;
  
  bool next_send_req = false;
  
  uint32_t response_timeout = 0;          // Time after packages need to be resent.
  uint8_t retry_cnt = 0;                  // retry counter if MCU doesn't respond on a message
  uint32_t binary_len = 0;                // size fo the binary
  std::unique_ptr<char[]> new_version;    // expected version string
  bool version_req_sent = false;          // To decide if mcu_upg_response_timeout need to be evaluated
  bool version_req_trigger =  false;
};

#ifdef __cplusplus
}
#endif

#endif  // USE_TUYA_MCU_UPGRADE
#endif  // USE_TUYA_MCU
#endif  // USE_LIGHT
#endif  // _TUYA_MCU_UPGRADE_H_