/*
  support_filesystem.ino - Filesystem support for Tasmota

  Copyright (C) 2021  Theo Arends

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
 * ESP32 Filesystem Support
\*********************************************************************************************/

#ifdef ESP32

#ifdef USE_TFS

//#define USE_LITTLEFS            // LittleFS not tested yet as currently ESP8266 only
//#define USE_FFAT                // FFat minimal 983k partition (4096 sector size) - tested
#define USE_SPIFFS              // SPIFFS - tested

#ifdef USE_LITTLEFS
  #include <LittleFS.h>
  #define TASMOTA_FS LittleFS
#endif
#ifdef USE_FFAT
  #include <FFat.h>
  #define TASMOTA_FS FFat
#endif
#ifdef USE_SPIFFS
  #include <SPIFFS.h>
  #define TASMOTA_FS SPIFFS
#endif

bool TfsInit(void) {
  static uint8_t FsMounted = 0;

  if (FsMounted) { return FsMounted -1; }

  AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Mounting..."));
  if (!TASMOTA_FS.begin()) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Formatting..."));
    TASMOTA_FS.format();
    if (!TASMOTA_FS.begin()) {
      AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Failed"));
      FsMounted = 1;  // false
      return false;
    }
  }
  AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Mounted"));
  FsMounted = 2;      // true
  return true;
}

bool TfsFileExists(const char *fname){
  if (!TfsInit()) { return false; }

  bool yes = false;
  File file = TASMOTA_FS.open(fname, "r");
  if (!file.isDirectory()) {
    yes = true;
  } else {
    AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: File not found"));
  }
  file.close();
  return yes;
}

bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len) {
  if (!TfsInit()) { return false; }

  File file = TASMOTA_FS.open(fname, "w");
  if (!file) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Save failed"));
    return false;
  }

  file.write(buf, len);
  file.close();
  return true;
}

bool TfsEraseFile(const char *fname, uint32_t len) {
  if (!TfsInit()) { return false; }

  File file = TASMOTA_FS.open(fname, "w");
  if (!file) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Erase failed"));
    return false;
  }

  uint8_t init_value = 0xff;
  for (uint32_t i = 0; i < len; i++) {
    file.write(&init_value, 1);
  }
  file.close();
  return true;
}

bool TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len) {
  if (!TfsInit()) { return false; }
  if (!TfsFileExists(fname)) { return false; }

  File file = TASMOTA_FS.open(fname, "r");
  if (!file) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: File not found"));
    return false;
  }

  file.read(buf, len);
  file.close();
  return true;
}

void TfsInfo(void) {
  if (!TfsInit()) { return; }

  uint32_t total_bytes = TASMOTA_FS.totalBytes();
#ifdef USE_SPIFFS
  uint32_t used_bytes = TASMOTA_FS.usedBytes();
#endif  // USE_SPIFFS
#ifdef USE_FFAT
  uint32_t used_bytes = total_bytes - TASMOTA_FS.freeBytes();
#endif  // USE_FFAT
  AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: Used %d/%d bytes"), used_bytes, total_bytes);

  File root = TASMOTA_FS.open("/");
  File file = root.openNextFile();
  while (file) {
    String filename = file.name();
    size_t filesize = file.size();
    AddLog_P(LOG_LEVEL_INFO, PSTR("TFS: File %s, size %d"), filename.c_str(), filesize);
    file = root.openNextFile();
  }
}

#endif  // USE_TFS
#endif  // ESP32
