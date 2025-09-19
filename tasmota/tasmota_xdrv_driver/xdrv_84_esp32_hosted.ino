/*
  xdrv_84_esp32_hosted.ino - ESP32 Hosted MCU support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef CONFIG_ESP_WIFI_REMOTE_ENABLED
/*********************************************************************************************\
 * Support for Hosted MCU to be used on ESP32-H2 and ESP32-P4
\*********************************************************************************************/

#define XDRV_84               84

#include "esp_hosted.h"

extern "C" {
#include "esp_hosted_transport_config.h"
}

#include "port/esp/freertos/include/port_esp_hosted_host_config.h"

struct Hosted_t {
  char *hosted_ota_url;                     // Hosted MCU OTA URL
  int hosted_ota_state_flag;                // Hosted MCU OTA initiated flag
} Hosted;

/*********************************************************************************************/

String GetHostedMCU(void) {
  // Function is not yet implemented in Arduino Core so emulate it here
  if (0 == strcasecmp_P(CONFIG_ESP_HOSTED_IDF_SLAVE_TARGET, PSTR("esp32c6"))) {
    return String("ESP32-C6");
  }
  return String("Unknown");
}

int GetFwVersionNumber(void) {
  // Function is not yet implemented in Arduino Core so emulate it here
  return 0x0200000E;   // v2.0.14
}

int GetHostedMCUFwVersionNumber(void) {
  static int version = -1;

  if (!esp_hosted_is_config_valid()) {
    return 0;
  }
  if (-1 == version) {
    version = 6;   // v0.0.6
    esp_hosted_coprocessor_fwver_t ver_info;
    esp_err_t err = esp_hosted_get_coprocessor_fwversion(&ver_info);  // This takes almost 4 seconds on <v0.0.6
    if (err == ESP_OK) {
      version = ver_info.major1 << 24 | ver_info.minor1 << 16 | ver_info.patch1;
    } else {
      // We can not know exactly, as API was added after 0.0.6
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Error %d, hosted version 0.0.6 or older"), err);
    }  
  }
  return version;
}

String GetHostedMCUFwVersion(void) {
  int version = GetHostedMCUFwVersionNumber();

  if (0 == version) {
    return String("");
  }
  uint8_t major1 = version >> 24;
  uint8_t minor1 = version >> 16;
  uint16_t patch1 = version;
  char data[40];
  snprintf_P(data, sizeof(data), PSTR("%d.%d.%d"), major1, minor1, patch1);
  return String(data);
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
      GetHostedMCU().c_str(), GetHostedMCUFwVersion().c_str(), config);
  }
}

/*********************************************************************************************\
 * Every second
\*********************************************************************************************/

void HostedMCUEverySecond(void) {
  if (Hosted.hosted_ota_state_flag && CommandsReady()) {
    Hosted.hosted_ota_state_flag--;
/*
    if (2 == Hosted.hosted_ota_state_flag) {
      SettingsSave(0);
    }
*/
    if (Hosted.hosted_ota_state_flag <= 0) {
      // Blocking
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: About to OTA update with %s"), Hosted.hosted_ota_url);
      int ret = esp_hosted_slave_ota(Hosted.hosted_ota_url);
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Done with result %d"), ret);
      free(Hosted.hosted_ota_url);
      Hosted.hosted_ota_url = nullptr;
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
  "Ota";

void (* const HostedCommand[])(void) PROGMEM = {
  &CmndHostedOta };

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
  Hosted.hosted_ota_url = (char*)calloc(200, sizeof(char));
  if (!Hosted.hosted_ota_url) { return; }                 // Unable to allocate memory
  if (XdrvMailbox.data_len > 15) {
    strlcpy(Hosted.hosted_ota_url, XdrvMailbox.data, 200);
  } else {
    // Replace https://ota.tasmota.com/tasmota32/tasmota32p4.bin  with https://ota.tasmota.com/tasmota32/coprocessor/network_adapter_esp32c6.bin
    char ota_url[TOPSZ];
    strlcpy(Hosted.hosted_ota_url, GetOtaUrl(ota_url, sizeof(ota_url)), 200);
    char *bch = strrchr(Hosted.hosted_ota_url, '/');      // Only consider filename after last backslash
    if (bch == nullptr) { bch = Hosted.hosted_ota_url; }  // No path found so use filename only
    *bch = '\0';                                                 // full_ota_url = https://ota.tasmota.com/tasmota32
    char version[16] = { 0 };
    if (XdrvMailbox.data_len) {
      snprintf_P(version, sizeof(version), PSTR("/%s"), XdrvMailbox.data);
    }
    snprintf_P(Hosted.hosted_ota_url, 200, PSTR("%s/coprocessor%s/network_adapter_" CONFIG_ESP_HOSTED_IDF_SLAVE_TARGET ".bin"), 
      Hosted.hosted_ota_url, version);
  }
  Hosted.hosted_ota_state_flag = 1;
  Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), 
    XdrvMailbox.command, GetHostedMCUFwVersion().c_str(), Hosted.hosted_ota_url);
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

#endif  // CONFIG_ESP_WIFI_REMOTE_ENABLED
#endif  // ESP32