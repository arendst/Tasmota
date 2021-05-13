/*
  xdrv_02_mqtt_1_file.ino - mqtt file support for Tasmota

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

#define USE_MQTT_FILE

#ifdef USE_MQTT_FILE
/*********************************************************************************************\
 * MQTT file transfer
 *
 * Supports base64 encoded binary data transfer
\*********************************************************************************************/

#include <base64.hpp>

struct FMQTT {
  uint32_t file_pos = 0;                 // MQTT file position during upload/download
  uint32_t file_size = 0;                // MQTT total file size
  uint32_t file_type = 0;                // MQTT File type (See UploadTypes)
  uint8_t* file_buffer = nullptr;        // MQTT file buffer
  const char* file_password = nullptr;   // MQTT password
  MD5Builder md5;                        // MQTT md5
  String file_md5;                       // MQTT received file md5 (32 chars)
  uint16_t topic_size;                   // MQTT topic length with terminating <null>
  uint8_t file_id = 0;                   // MQTT unique file id during upload/download
} FMqtt;

void MqttTopicSize(uint32_t topic_size) {
  FMqtt.topic_size = topic_size +1;
}

/*
  The download chunk size is the data size before it is encoded to base64.
  It is smaller than the upload chunksize as it is bound by MESSZ
  The download buffer with length MESSZ (1042) contains
    - Payload ({"Id":117,"Data":"<base64 encoded mqtt_file_chuck_size>"}<null>)
*/
const uint32_t FileTransferHeaderSize = 21;       // {"Id":116,"Data":""}<null>
const uint32_t mqtt_file_chuck_size = (((MESSZ - FileTransferHeaderSize) / 4) * 3) -2;

uint32_t FileUploadChunckSize(void) {
/*
  The upload chunk size is the data size before it is encoded to base64.
  It can be larger than the download chunksize which is bound by MESSZ
  The PubSubClient upload buffer with length MQTT_MAX_PACKET_SIZE (1200) contains
    - Header of 5 bytes (MQTT_MAX_HEADER_SIZE)
    - Topic string terminated with a zero (stat/demo/FILEUPLOAD<null>)
    - Payload ({"Id":116,"Data":"<base64 encoded FileUploadChunckSize>"}<null>)
*/
  const uint32_t PubSubClientHeaderSize = 5;   // MQTT_MAX_HEADER_SIZE
  return MQTT_MAX_PACKET_SIZE - PubSubClientHeaderSize - FMqtt.topic_size - FileTransferHeaderSize;
}

uint32_t MqttFileUploadValidate(uint32_t rcv_id) {
  if ((0 == FMqtt.file_id) && (rcv_id > 0) && (FMqtt.file_size > 0) && (FMqtt.file_type > 0)) {
    FMqtt.file_buffer = nullptr;                             // Init upload buffer

    if (!FMqtt.file_password || (strcmp(FMqtt.file_password, SettingsText(SET_MQTT_PWD)) != 0)) {
      return 1;                                              // Invalid password
    }

    if (UPL_SETTINGS != FMqtt.file_type) {                   // Check enough flash space for intermediate upload
      uint32_t head_room = (FlashWriteMaxSector() - FlashWriteStartSector()) * SPI_FLASH_SEC_SIZE;
      uint32_t rounded_size = (FMqtt.file_size + SPI_FLASH_SEC_SIZE -1) & (~(SPI_FLASH_SEC_SIZE - 1));
      if (rounded_size > head_room) {
        return 2;                                            // Not enough space
      }
    }

    if (UPL_TASMOTA == FMqtt.file_type) {
      if (Update.begin(FMqtt.file_size)) {
        FMqtt.file_buffer = &FMqtt.file_id;                  // Dummy buffer
        SettingsSave(1);                                     // Free flash for OTA update
      }
    }
    else if (UPL_SETTINGS == FMqtt.file_type) {
      if (SettingsConfigBackup()) {
        FMqtt.file_buffer = settings_buffer;
      }
    }
    else {
      return 3;                                              // Invalid file type
    }

    FMqtt.file_id = rcv_id;
    FMqtt.file_pos = 0;

    FMqtt.md5 = MD5Builder();
    FMqtt.md5.begin();

    ResponseCmndChar(PSTR(D_JSON_STARTED));
    MqttPublishPrefixTopic_P(STAT, XdrvMailbox.command);     // Enforce stat/wemos10/FILEUPLOAD
  }
  else if ((FMqtt.file_id > 0) && (FMqtt.file_id != rcv_id)) {
    // Error receiving data

    if (UPL_TASMOTA == FMqtt.file_type) {
      Update.end(true);
    }
    else if (UPL_SETTINGS == FMqtt.file_type) {
      SettingsBufferFree();
    }
    return 4;                                                // Upload aborted
  }
  return 0;                                                  // No error
}

void CmndFileUpload(void) {
/*
  Upload (binary) max 700 bytes chunks of data base64 encoded with MD5 hash over base64 decoded data
  FileUpload 0  - Abort current upload
  FileUpload {"File":"Config_wemos10_9.4.0.3.dmp","Id":116,"Type":2,"Size":4096}
  FileUpload {"Id":116,"Data":"CRJcTQ9fYGF ... OT1BRUlNUVVZXWFk="}
  FileUpload {"Id":116,"Data":" ... "}
  FileUpload {"Id":116,"Md5":"496fcbb433bbca89833063174d2c5747"}
*/
  if (XdrvMailbox.grpflg) { return; }

  const char* base64_data = nullptr;
  uint32_t rcv_id = 0;
  char* dataBuf = (char*)XdrvMailbox.data;

  bool binary_data = false;
  if (XdrvMailbox.index > 199) {                             // Check for raw data
    XdrvMailbox.index -= 200;
    binary_data = true;
  }

  if (!binary_data) {
    if (strlen(dataBuf) > 8) {                               // Workaround exception if empty JSON like {} - Needs checks
      JsonParser parser((char*) dataBuf);
      JsonParserObject root = parser.getRootObject();
      if (root) {
        JsonParserToken val = root[PSTR("ID")];
        if (val) { rcv_id = val.getUInt(); }
        val = root[PSTR("TYPE")];
        if (val) { FMqtt.file_type = val.getUInt(); }
        val = root[PSTR("SIZE")];
        if (val) { FMqtt.file_size = val.getUInt(); }
        val = root[PSTR("MD5")];
        if (val) { FMqtt.file_md5 = val.getStr(); }
        val = root[PSTR("DATA")];
        if (val) { base64_data = val.getStr(); }
        val = root[PSTR("PASS")];
        if (val) { FMqtt.file_password = val.getStr(); }
      }
    }
  } else {
    rcv_id = FMqtt.file_id;
  }

  uint32_t error = MqttFileUploadValidate(rcv_id);
  if (error) {
    FMqtt.file_buffer = nullptr;

    TasmotaGlobal.masterlog_level = LOG_LEVEL_NONE;          // Enable logging

    char error_txt[TOPSZ];
    snprintf_P(error_txt, sizeof(error_txt), PSTR(D_JSON_ERROR " %d"), error);
    ResponseCmndChar(error_txt);
  }

  if (FMqtt.file_buffer) {
    if ((FMqtt.file_pos < FMqtt.file_size) && (binary_data || base64_data)) {
      // Save upload into buffer - Handle possible buffer overflows

      unsigned char* raw_data = (unsigned char*)XdrvMailbox.data;
      uint32_t rcvd_bytes = XdrvMailbox.data_len;
      if (!binary_data) {
        raw_data = (unsigned char*)malloc(XdrvMailbox.data_len);
        if (raw_data) {
          rcvd_bytes = decode_base64((unsigned char*)base64_data, (unsigned char*)raw_data);
        }
      }

      if (raw_data) {
        uint32_t bytes_left = FMqtt.file_size - FMqtt.file_pos;
        uint32_t read_bytes = (bytes_left < rcvd_bytes) ? bytes_left : rcvd_bytes;
        FMqtt.md5.add(raw_data, read_bytes);

        if (UPL_TASMOTA == FMqtt.file_type) {
          Update.write(raw_data, read_bytes);
        } else {
          uint8_t* buffer = FMqtt.file_buffer + FMqtt.file_pos;
          memcpy(buffer, raw_data, read_bytes);
        }

        if (!binary_data) {
          free(raw_data);
        }

        FMqtt.file_pos += read_bytes;
      }

      if ((FMqtt.file_pos > rcvd_bytes) && ((FMqtt.file_pos % 102400) <= rcvd_bytes)) {
        TasmotaGlobal.masterlog_level = LOG_LEVEL_NONE;      // Enable logging
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Progress %d kB"), FMqtt.file_pos / 1024);
        TasmotaGlobal.masterlog_level = LOG_LEVEL_DEBUG_MORE;  // Hide upload data logging
      }
    }

    if ((FMqtt.file_pos < FMqtt.file_size) || (FMqtt.file_md5.length() != 32))   {
      TasmotaGlobal.masterlog_level = LOG_LEVEL_DEBUG_MORE;  // Hide upload data logging

      uint32_t chunk_size = FileUploadChunckSize();
      if (!binary_data) {
        chunk_size = ((chunk_size / 4) * 3) -2;              // Calculate base64 chunk size
      }
      // {"Id":116,"MaxSize":"765"}
      Response_P(PSTR("{\"Id\":%d,\"MaxSize\":%d}"), FMqtt.file_id, chunk_size);
    } else {
      FMqtt.md5.calculate();
      if (strcasecmp(FMqtt.file_md5.c_str(), FMqtt.md5.toString().c_str())) {
        ResponseCmndChar(PSTR(D_JSON_MD5_MISMATCH));
      } else {
        // Process upload data en free buffer
        ResponseCmndDone();

        if (UPL_TASMOTA == FMqtt.file_type) {
          if (!Update.end(true)) {
            ResponseCmndFailed();
          } else {
            TasmotaGlobal.restart_flag = 2;                  // Always restart to re-enable disabled features during update
          }
        }
        else if (UPL_SETTINGS == FMqtt.file_type) {
          if (!SettingsConfigRestore()) {
            ResponseCmndFailed();
          } else {
            TasmotaGlobal.restart_flag = 2;                  // Always restart to re-enable disabled features during update
          }
        }

      }
      FMqtt.file_buffer = nullptr;
    }
  }

  if (!FMqtt.file_buffer) {
    TasmotaGlobal.masterlog_level = LOG_LEVEL_NONE;          // Enable logging
    FMqtt.file_id = 0;
    FMqtt.file_size = 0;
    FMqtt.file_type = 0;
    FMqtt.file_md5 = (const char*) nullptr;                  // Force deallocation of the String internal memory
    FMqtt.file_password = nullptr;
  }
  MqttPublishPrefixTopic_P(STAT, XdrvMailbox.command);       // Enforce stat/wemos10/FILEUPLOAD
  ResponseClear();
}

void CmndFileDownload(void) {
/*
  Download (binary) max 700 bytes chunks of data base64 encoded with MD5 hash over base64 decoded data
  Currently supports Settings (file type 2)
  Filedownload 0  - Abort current download
  FileDownload 2  - Start download of settings file
  FileDownload    - Continue downloading data until reception of MD5 hash
*/
  if (XdrvMailbox.grpflg) { return; }

  if (FMqtt.file_id && FMqtt.file_buffer) {
    bool finished = false;

    if (0 == XdrvMailbox.payload) {   // Abort file download
      ResponseCmndChar(PSTR(D_JSON_ABORTED));
      finished = true;
    }
    else if (FMqtt.file_pos < FMqtt.file_size) {
      uint32_t bytes_left = FMqtt.file_size - FMqtt.file_pos;
      uint32_t write_bytes = (bytes_left < mqtt_file_chuck_size) ? bytes_left : mqtt_file_chuck_size;

      uint8_t* buffer = FMqtt.file_buffer + FMqtt.file_pos;
      FMqtt.md5.add(buffer, write_bytes);

      // {"Id":117,"Data":"CRJcTQ9fYGF ... OT1BRUlNUVVZXWFk="}
      Response_P(PSTR("{\"Id\":%d,\"Data\":\""), FMqtt.file_id);  // FileTransferHeaderSize
      char base64_data[encode_base64_length(write_bytes)];
      encode_base64((unsigned char*)buffer, write_bytes, (unsigned char*)base64_data);
      ResponseAppend_P(base64_data);
      ResponseAppend_P("\"}");

      FMqtt.file_pos += write_bytes;
    } else {
      FMqtt.md5.calculate();

      // {"Id":117,"Md5":"496fcbb433bbca89833063174d2c5747"}
      Response_P(PSTR("{\"Id\":%d,\"Md5\":\"%s\"}"), FMqtt.file_id, FMqtt.md5.toString().c_str());
      finished = true;
    }

    if (finished) {
      if (UPL_SETTINGS == FMqtt.file_type) {
        SettingsBufferFree();
      }

      FMqtt.file_id = 0;
    }
  }
  else if (XdrvMailbox.data_len) {
    FMqtt.file_buffer = nullptr;
    FMqtt.file_id = (UtcTime() & 0xFE) +1;  // Odd id between 1 and 255

    if (UPL_SETTINGS == XdrvMailbox.payload) {
      uint32_t len = SettingsConfigBackup();
      if (len) {
        FMqtt.file_type = UPL_SETTINGS;
        FMqtt.file_buffer = settings_buffer;
        FMqtt.file_size = len;

        // {"File":"Config_wemos10_9.4.0.3.dmp","Id":117,"Type":2,"Size":4096}
        Response_P(PSTR("{\"File\":\"%s\",\"Id\":%d,\"Type\":%d,\"Size\":%d}"),
          SettingsConfigFilename().c_str(), FMqtt.file_id, FMqtt.file_type, len);
      }
    }

    if (FMqtt.file_buffer) {
      FMqtt.file_pos = 0;

      FMqtt.md5 = MD5Builder();
      FMqtt.md5.begin();
    } else {
      FMqtt.file_id = 0;
      ResponseCmndFailed();
    }
  }
  MqttPublishPrefixTopic_P(STAT, XdrvMailbox.command);
  ResponseClear();
}

#endif  // USE_MQTT_FILE
