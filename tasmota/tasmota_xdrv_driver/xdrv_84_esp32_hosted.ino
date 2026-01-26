/*
  xdrv_84_esp32_hosted.ino - ESP32 Hosted MCU support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef CONFIG_ESP_WIFI_REMOTE_ENABLED
#ifndef FIRMWARE_SAFEBOOT
/*********************************************************************************************\
 * Support for Hosted MCU to be used on ESP32-H2 and ESP32-P4
\*********************************************************************************************/

#define XDRV_84               84

#include "esp_hosted.h"

extern "C" {
#include "esp_hosted_transport_config.h"
}

#include "port/esp/freertos/include/port_esp_hosted_host_config.h"

#if ESP_HOSTED_VERSION_MAJOR_1 >= 2 && ESP_HOSTED_VERSION_MINOR_1 >= 6
#  define ESP_HOSTED_NEW_OTA
#endif

enum EspHostTypes { ESP_HOST, ESP_HOSTED };

struct Hosted_t {
  char *ota_url;                     // Hosted MCU OTA URL
  int ota_http_state_flag;           // Hosted MCU OTA HTTP initiated flag
  int ota_file_state_flag;           // Hosted MCU OTA File initiated flag
} Hosted;

/*********************************************************************************************/

uint32_t GetHostFwVersion(void) {
  uint32_t host_version = (ESP_HOSTED_VERSION_MAJOR_1 << 16) | (ESP_HOSTED_VERSION_MINOR_1 << 8) | (ESP_HOSTED_VERSION_PATCH_1);
  return host_version;
}

int GetHostedMCUFwVersion(void) {
  static int hosted_version = -1;

  if (!esp_hosted_is_config_valid()) {
    return 0;
  }
  if (-1 == hosted_version) {
    hosted_version = 6;   // v0.0.6
    esp_hosted_coprocessor_fwver_t ver_info;
    esp_err_t err = esp_hosted_get_coprocessor_fwversion(&ver_info);  // This takes almost 4 seconds on <v0.0.6
    if (err == ESP_OK) {
      hosted_version = ver_info.major1 << 16 | ver_info.minor1 << 8 | ver_info.patch1;
    } else {
      // We can not know exactly, as API was added after 0.0.6
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Error %d, hosted version 0.0.6 or older"), err);
    }  
  }
  return hosted_version;
}

String GetHostedFwVersion(uint32_t device) {
  int version = (device) ? GetHostedMCUFwVersion() : GetHostFwVersion();

  uint16_t major1 = version >> 16;
  uint8_t minor1 = version >> 8;
  uint8_t patch1 = version;
  char data[40];
  snprintf_P(data, sizeof(data), PSTR("%d.%d.%d"), major1, minor1, patch1);
  return String(data);
}

String GetHostedMCU(void) {
  // Function is not yet implemented in Arduino Core so emulate it here
  if (0 == strcasecmp_P(CONFIG_ESP_HOSTED_IDF_SLAVE_TARGET, PSTR("esp32c6"))) {
    return String("ESP32-C6");
  }
  return String("Unknown");
}

void HostedMCUStatus(void) {
  // Execute after HostedMCU is init by WiFi.mode()
  static bool once_shown = false;

  if (once_shown) { return; }
  if (esp_hosted_is_config_valid()) {
    once_shown = true;
    char config[128] = { 0 };
    struct esp_hosted_transport_config *pconfig;
    if (ESP_TRANSPORT_OK == esp_hosted_transport_get_config(&pconfig)) {
      if (pconfig->transport_in_use == H_TRANSPORT_SDIO) {
        struct esp_hosted_sdio_config *psdio_config;
        if (ESP_TRANSPORT_OK == esp_hosted_sdio_get_config(&psdio_config)) {
          snprintf_P(config, sizeof(config), PSTR(" using GPIO%02d(CLK), GPIO%02d(CMD), GPIO%02d(D0), GPIO%02d(D1), GPIO%02d(D2), GPIO%02d(D3) and GPIO%02d(RST)"), 
            psdio_config->pin_clk.pin, psdio_config->pin_cmd.pin, psdio_config->pin_d0.pin, psdio_config->pin_d1.pin, psdio_config->pin_d2.pin, psdio_config->pin_d3.pin, psdio_config->pin_reset.pin);
        }
      }
    }
    AddLog(LOG_LEVEL_INFO, PSTR("HST: Hosted MCU %s v%s%s"),
      GetHostedMCU().c_str(), GetHostedFwVersion(ESP_HOSTED).c_str(), config);
  }
}

/*********************************************************************************************\
 * Every second
\*********************************************************************************************/

void HostedMCUEverySecond(void) {
  if (!CommandsReady()) { return; }

#ifdef ESP_HOSTED_NEW_OTA
  if (Hosted.ota_file_state_flag) {
    Hosted.ota_file_state_flag--;
    if (Hosted.ota_file_state_flag <= 0) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("HST: File update %s"), Hosted.ota_url);
      int ret = -1;
      // Blocking

      if (!ffsp) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: No file system"));
      } else {
        File file = ffsp->open(Hosted.ota_url, "r");
        if (!file) {
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: File not found"));
        } else {
          // Start load on coprocessor
          if ((ret = esp_hosted_slave_ota_begin()) != ESP_OK) {
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: ota_begin failed %d"), ret);
            file.close();
          } else {
            const size_t bufSize = 1024;
            uint8_t *buf = (uint8_t*)malloc(bufSize);
            if (buf) {
              int read;
              bool write_ok = true;
              int total_size = 0;
              while ((read = file.read(buf, bufSize)) > 0) {
                total_size += read;
                if ((ret = esp_hosted_slave_ota_write(buf, (uint32_t)read)) != ESP_OK) {
                  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: ota_write failed %d"), ret);
                  write_ok = false;
                  break;
                }
                if (total_size % 102400 == 0) {
                  AddLog(LOG_LEVEL_DEBUG, PSTR("HST: Progress %d KB"), total_size / 1024);
                }
                delay(1);
                OsWatchLoop();
              }
              free(buf);
              file.close();
              if (write_ok) {
                AddLog(LOG_LEVEL_DEBUG, PSTR("HST: Successful %d bytes"), total_size);
                if ((ret = esp_hosted_slave_ota_end()) != ESP_OK) {
                  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: ota_end failed %d"), ret);
                } else {
                  // Activate will likely reboot the slave
                  ret = esp_hosted_slave_ota_activate();
                }
              }
            }
          }
        }
      }
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Done with result %d"), ret);
      free(Hosted.ota_url);
      Hosted.ota_url = nullptr;
      Response_P(PSTR("{\"" D_CMND_HOSTEDLOAD "\":\""));
      if (ret == ESP_OK) {
        // next lines are questionable, because currently the system will reboot immediately on succesful upgrade
        ResponseAppend_P(PSTR(D_JSON_SUCCESSFUL ". " D_JSON_RESTARTING));
        TasmotaGlobal.restart_flag = 5;                 // Allow time for webserver to update console
      } else {
        ResponseAppend_P(PSTR(D_JSON_FAILED " %d\"}"), ret);
      }
      ResponseAppend_P(PSTR("\"}"));
      MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR(D_CMND_HOSTEDLOAD));
    }
  }
#endif  // ESP_HOSTED_NEW_OTA
  if (Hosted.ota_http_state_flag) {
    Hosted.ota_http_state_flag--;
/*
    if (2 == Hosted.ota_http_state_flag) {
      SettingsSave(0);
    }
*/
    if (Hosted.ota_http_state_flag <= 0) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("HST: OTA update %s"), Hosted.ota_url);
      int ret = -1;
      // Blocking

#ifdef ESP_HOSTED_NEW_OTA      
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Using streaming OTA API"));
      HTTPClientLight http;
      if (!http.begin(Hosted.ota_url)) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: HTTP begin failed"));
        ret = -1;
      } else {
        http.setTimeout(15000);
        int httpCode = http.GET();
        if (httpCode != 200) {
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: HTTP GET failed %d"), httpCode);
          http.end();
          ret = -1;
        } else {
          // Start OTA on coprocessor
          if ((ret = esp_hosted_slave_ota_begin()) != ESP_OK) {
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: ota_begin failed %d"), ret);
            http.end();
          } else {
            const size_t bufSize = 1024;
            uint8_t *buf = (uint8_t*)malloc(bufSize);
            if (!buf) {
              http.end();
              ret = -1;
            } else {
              // Stream response in blocks
              WiFiClient& stream = http.getStream();
              int read;
              bool write_ok = true;
              int total_size = 0;
              while ((read = stream.readBytes((char*)buf, bufSize)) > 0) {
                total_size += read;
                if ((ret = esp_hosted_slave_ota_write(buf, (uint32_t)read)) != ESP_OK) {
                  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: ota_write failed %d"), ret);
                  write_ok = false;
                  break;
                }
                if (total_size % 102400 == 0) {
                  AddLog(LOG_LEVEL_DEBUG, PSTR("HST: Progress %d KB"), total_size / 1024);
                }
                delay(1);
                OsWatchLoop();
              }
              free(buf);
              http.end();
              if (write_ok) {
                AddLog(LOG_LEVEL_DEBUG, PSTR("HST: Successful %d bytes"), total_size);
                if ((ret = esp_hosted_slave_ota_end()) != ESP_OK) {
                  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: ota_end failed %d"), ret);
                } else {
                  if (GetHostedMCUFwVersion() >= 0x00020600) { 
                    // Activate will likely reboot the slave
                    ret = esp_hosted_slave_ota_activate();
                  }
                }
              }
            }
          }
        }
      }
#else   // OLD_OTA
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Using legacy OTA API"));
      ret = esp_hosted_slave_ota(Hosted.ota_url);
#endif  // ESP_HOSTED_NEW_OTA

      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Done with result %d"), ret);
      free(Hosted.ota_url);
      Hosted.ota_url = nullptr;
      Response_P(PSTR("{\"" D_CMND_HOSTEDOTA "\":\""));
      if (ret == ESP_OK) {
        // next lines are questionable, because currently the system will reboot immediately on succesful upgrade
        ResponseAppend_P(PSTR(D_JSON_SUCCESSFUL ". " D_JSON_RESTARTING));
        TasmotaGlobal.restart_flag = 5;                 // Allow time for webserver to update console
      } else {
        ResponseAppend_P(PSTR(D_JSON_FAILED " %d\"}"), ret);
      }
      ResponseAppend_P(PSTR("\"}"));
      MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR(D_CMND_HOSTEDOTA));
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kHostedCommands[] PROGMEM = "Hosted|"  // Prefix
  "|"
#ifdef ESP_HOSTED_NEW_OTA      
  "Load|"
#endif  // ESP_HOSTED_NEW_OTA      
  "Ota";

void (* const HostedCommand[])(void) PROGMEM = {
  &CmndHosted,
#ifdef ESP_HOSTED_NEW_OTA      
  &CmndHostedLoad,
#endif  // ESP_HOSTED_NEW_OTA      
  &CmndHostedOta };

void CmndHosted(void) {
  Response_P(PSTR("{\"Hosted\":{\"Host\":\"%s\",\"Hosted\":\"%s\",\"MCU\":\"%s\"}}"),
    GetHostedFwVersion(ESP_HOST).c_str(), GetHostedFwVersion(ESP_HOSTED).c_str(), GetHostedMCU().c_str()
  );
}

#ifdef ESP_HOSTED_NEW_OTA      
void CmndHostedLoad(void) {
  /*
  Expect files in folder /coprocessor/v2.0.17/network_adapter_esp32c6.bin
  As an option allow user to enter file like:
   HostedLoad /coprocessor/network_adapter_esp32c6.bin
  Or allow user to enter required version like:
   HostedLoad v2.0.17
  */
//  if (GetHostedMCUFwVersion() < 0x00020600) { return; }

  Hosted.ota_url = (char*)calloc(200, sizeof(char));
  if (!Hosted.ota_url) { return; }                 // Unable to allocate memory
  if (XdrvMailbox.data_len > 15) {
    strlcpy(Hosted.ota_url, XdrvMailbox.data, 200);
  } else {
    char version[16] = { 0 };
    if (XdrvMailbox.data_len) {
      snprintf_P(version, sizeof(version), PSTR("/%s"), XdrvMailbox.data);
    }
    snprintf_P(Hosted.ota_url, 200, PSTR("/coprocessor%s/network_adapter_" CONFIG_ESP_HOSTED_IDF_SLAVE_TARGET ".bin"),
      version);
  }
  Hosted.ota_file_state_flag = 1;
  Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), 
    XdrvMailbox.command, GetHostedFwVersion(ESP_HOSTED).c_str(), Hosted.ota_url);
}
#endif  // ESP_HOSTED_NEW_OTA      

void CmndHostedOta(void) {
  /*
  If OtaUrl = "https://ota.tasmota.com/tasmota32/tasmota32p4.bin"
   Then use "https://ota.tasmota.com/tasmota32/coprocessor/network_adapter_" CONFIG_ESP_HOSTED_IDF_SLAVE_TARGET ".bin"
  As an option allow user to enter URL like:
   HostedOta https://ota.tasmota.com/tasmota32/coprocessor/network_adapter_esp32c6.bin
   HostedOta https://ota.tasmota.com/tasmota32/coprocessor/v2.0.14/network_adapter_esp32c6.bin
  Or allow user to enter required version like:
   HostedOta v2.0.17
  */
  Hosted.ota_url = (char*)calloc(200, sizeof(char));
  if (!Hosted.ota_url) { return; }                 // Unable to allocate memory
  if (XdrvMailbox.data_len > 15) {
    strlcpy(Hosted.ota_url, XdrvMailbox.data, 200);
  } else {
    // Replace https://ota.tasmota.com/tasmota32/tasmota32p4.bin  with https://ota.tasmota.com/tasmota32/coprocessor/network_adapter_esp32c6.bin
    char ota_url[TOPSZ];
    strlcpy(Hosted.ota_url, GetOtaUrl(ota_url, sizeof(ota_url)), 200);
    char *bch = strrchr(Hosted.ota_url, '/');      // Only consider filename after last backslash
    if (bch == nullptr) { bch = Hosted.ota_url; }  // No path found so use filename only
    *bch = '\0';                                   // full_ota_url = https://ota.tasmota.com/tasmota32
    char version[16] = { 0 };
    if (XdrvMailbox.data_len) {
      snprintf_P(version, sizeof(version), PSTR("/%s"), XdrvMailbox.data);
    }
    snprintf_P(Hosted.ota_url, 200, PSTR("%s/coprocessor%s/network_adapter_" CONFIG_ESP_HOSTED_IDF_SLAVE_TARGET ".bin"), 
      Hosted.ota_url, version);
  }
  Hosted.ota_http_state_flag = 1;
  Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), 
    XdrvMailbox.command, GetHostedFwVersion(ESP_HOSTED).c_str(), Hosted.ota_url);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv84(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      HostedMCUEverySecond();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kHostedCommands, HostedCommand);
      break;
  }
  return result;
}

#endif  // FIRMWARE_SAFEBOOT
#endif  // CONFIG_ESP_WIFI_REMOTE_ENABLED
#endif  // ESP32