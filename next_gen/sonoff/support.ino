/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*********************************************************************************************\
 * Config - Flash or Spiffs
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}

#define SPIFFS_START        ((uint32_t)&_SPIFFS_start - 0x40200000) / SPI_FLASH_SEC_SIZE
#define SPIFFS_END          ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE

// Version 2.x config
#define SPIFFS_CONFIG2      "/config.ini"
#define CFG_LOCATION2       SPIFFS_END - 2

// Version 3.x config
#define SPIFFS_CONFIG       "/cfg.ini"
#define CFG_LOCATION        SPIFFS_END - 4

uint32_t _cfgHash = 0;
int spiffsflag = 0;

boolean spiffsPresent()
{
  return (SPIFFS_END - SPIFFS_START);
}

uint32_t getHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&sysCfg;

  for (uint16_t i = 0; i < sizeof(SYSCFG); i++) hash += bytes[i]*(i+1);
  return hash;
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash or Spiffs ONLY if any parameter has changed
\*********************************************************************************************/
void CFG_Save()
{
  char log[LOGSZ];

  if ((getHash() != _cfgHash) && (spiffsPresent())) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      sysCfg.saveFlag++;
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg;
        for (int i = 0; i < sizeof(SYSCFG); i++) f.write(bytes[i]);
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration (%d bytes) to spiffs count %d"), sizeof(SYSCFG), sysCfg.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else {
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Saving configuration failed"));
      }
    } else {
#endif  // USE_SPIFFS
      noInterrupts();
      if (sysCfg.saveFlag == 0) {  // Handle default and rollover
        spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
        spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      }
      sysCfg.saveFlag++;
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration (%d bytes) to flash at %X and count %d"), sizeof(SYSCFG), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
    _cfgHash = getHash();
  }
}

void CFG_Load()
{
  char log[LOGSZ];

  if (spiffsPresent()) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg;
        for (int i = 0; i < sizeof(SYSCFG); i++) bytes[i] = f.read();
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from spiffs count %d"), sysCfg.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else {
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Loading configuration failed"));
      }
    } else {
#endif  // USE_SPIFFS
      struct SYSCFGH {
        unsigned long cfg_holder;
        unsigned long saveFlag;
      } _sysCfgH;

      noInterrupts();
      spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
      if (sysCfg.saveFlag < _sysCfgH.saveFlag)
        spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
//  snprintf_P(log, sizeof(log), PSTR("Config: Check 1 for migration (%08X)"), sysCfg.version);
//  addLog(LOG_LEVEL_NONE, log);
  if (sysCfg.cfg_holder != CFG_HOLDER) {
    if ((sysCfg.version < 0x03000000) || (sysCfg.version > 0x73000000)) {
      CFG_Migrate();  // Config may be present with versions below 3.0.0
    } else {
      CFG_Default();
    }
  }
  _cfgHash = getHash();
}

void CFG_Migrate()
{
  char log[LOGSZ];

  if (spiffsPresent()) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      File f = SPIFFS.open(SPIFFS_CONFIG2, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg2;
        for (int i = 0; i < sizeof(SYSCFG2); i++) bytes[i] = f.read();
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Loaded previous configuration from spiffs count %d"), sysCfg2.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else {
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Loading previous configuration failed"));
      }
    } else {
#endif  // USE_SPIFFS
      struct SYSCFGH {
        unsigned long cfg_holder;
        unsigned long saveFlag;
      } _sysCfgH;

      noInterrupts();
      spi_flash_read((CFG_LOCATION2) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg2, sizeof(SYSCFG2));
      spi_flash_read((CFG_LOCATION2 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
      if (sysCfg2.saveFlag < _sysCfgH.saveFlag)
        spi_flash_read((CFG_LOCATION2 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg2, sizeof(SYSCFG2));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Loaded previous configuration from flash at %X and count %d"), CFG_LOCATION2 + (sysCfg2.saveFlag &1), sysCfg2.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
//  snprintf_P(log, sizeof(log), PSTR("Config: Check 2 for migration (%08X)"), sysCfg2.version);
//  addLog(LOG_LEVEL_NONE, log);
  if ((sysCfg2.version > 0x01000000) && (sysCfg2.version < 0x03000000)) {
    CFG_Migrate_Part2();  // Config is present between version 1.0.0 and 3.0.0
  } else {
    CFG_Default();
  }
  _cfgHash = getHash();
}

void CFG_Erase()
{
  char log[LOGSZ];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  boolean _serialoutput = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);

  snprintf_P(log, sizeof(log), PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (_serialoutput) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (result == SPI_FLASH_RESULT_OK) {
        Serial.println(F(" OK"));
      } else {
        Serial.println(F(" Error"));
      }
      delay(10);
    }
  }
}

void CFG_Dump()
{
  #define CFG_COLS 16
  char log[LOGSZ];
  uint8_t buffer[((sizeof(SYSCFG)+CFG_COLS)/CFG_COLS)*CFG_COLS];
  uint16_t idx, row, col;

  if (spiffsPresent()) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&buffer;
        for (int i = 0; i < sizeof(buffer); i++) bytes[i] = f.read();
        f.close();
        addLog(LOG_LEVEL_INFO, PSTR("Config: Loaded buffer from spiffs"));
      } else {
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Loading buffer failed"));
      }
    } else {
#endif  // USE_SPIFFS
      noInterrupts();
      spi_flash_read((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&buffer, sizeof(buffer));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Loaded buffer from flash at %X"), CFG_LOCATION + (sysCfg.saveFlag &1));
      addLog(LOG_LEVEL_INFO, log);
    }
    for (row = 0; row < sizeof(buffer)/CFG_COLS; row++) {
      idx = row * CFG_COLS;
      snprintf_P(log, sizeof(log), PSTR("%04X:"), idx);
      for (col = 0; col < CFG_COLS; col++) {
        if (!(col%4)) snprintf_P(log, sizeof(log), PSTR("%s "), log);
        snprintf_P(log, sizeof(log), PSTR("%s %02X"), log, buffer[idx + col]);
      }
      snprintf_P(log, sizeof(log), PSTR("%s |"), log);
      for (col = 0; col < CFG_COLS; col++) {
//        if (!(col%4)) snprintf_P(log, sizeof(log), PSTR("%s "), log);
        snprintf_P(log, sizeof(log), PSTR("%s%c"), log, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
      }
      snprintf_P(log, sizeof(log), PSTR("%s|"), log);
      addLog(LOG_LEVEL_INFO, log);
    }
  } else {
    addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - No SPIFFS present"));
  }
}

#ifdef USE_SPIFFS
void initSpiffs()
{
  spiffsflag = 0;
  if (!spiffsPresent()) {
    spiffsflag = 1;
  } else {
    if (!SPIFFS.begin()) {
      addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: WARNING - Failed to mount file system. Will use flash"));
      spiffsflag = 2;
    } else {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Mount successful"));
      File f = SPIFFS.open(SPIFFS_CONFIG, "r");
      if (!f) {
        addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Formatting..."));
        SPIFFS.format();
        addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Formatted"));
        File f = SPIFFS.open(SPIFFS_CONFIG, "w");
        if (f) {
          for (int i = 0; i < sizeof(SYSCFG); i++) f.write(0);
          f.close();
        } else {
          addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: WARNING - Failed to init config file. Will use flash"));
          spiffsflag = 3;
        }
      }
    }
  }
}
#endif  // USE_SPIFFS

/*
void setFlashChipMode(byte mode)
{
  char log[LOGSZ];
  uint32_t data;
  
  uint8_t * bytes = (uint8_t *) &data;
  // read first 4 byte (magic byte + flash config)
  if (spi_flash_read(0x0000, &data, 4) == SPI_FLASH_RESULT_OK) {

    snprintf_P(log, sizeof(log), PSTR("FLSH: Magic byte and flash config %08X"), data);
    addLog(LOG_LEVEL_DEBUG, log);
    
    if (bytes[2] != mode) {
      bytes[2] = mode &3;
//      spi_flash_write(0x0000, &data, 4);
    }
  }
}
*/
/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#define WIFI_CONFIG_SEC   60   // seconds before restart
#define WIFI_MANAGER_SEC  120  // seconds before restart
#define WIFI_CHECK_SEC    20   // seconds
#define WIFI_RETRY_SEC    30   // seconds

uint8_t _wificounter, _wifiretry, _wifistatus, _wpsresult, _wificonfigflag = 0, _wifiConfigCounter = 0;

int WIFI_getRSSIasQuality(int RSSI)
{
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

boolean WIFI_configCounter()
{
  if (_wifiConfigCounter) _wifiConfigCounter = WIFI_MANAGER_SEC;
  return (_wifiConfigCounter);
}

extern "C" {
#include "user_interface.h"
}

void WIFI_wps_status_cb(wps_cb_status status);

void WIFI_wps_status_cb(wps_cb_status status)
{
  char log[LOGSZ];

/* from user_interface.h:
  enum wps_cb_status {
    WPS_CB_ST_SUCCESS = 0,
    WPS_CB_ST_FAILED,
    WPS_CB_ST_TIMEOUT,
    WPS_CB_ST_WEP,      // WPS failed because that WEP is not supported
    WPS_CB_ST_SCAN_ERR, // can not find the target WPS AP
  };
*/

  _wpsresult = status;
  if (_wpsresult == WPS_CB_ST_SUCCESS) {
    wifi_wps_disable();
  } else {
    snprintf_P(log, sizeof(log), PSTR("WPSconfig: FAILED with status %d"), _wpsresult);
    addLog(LOG_LEVEL_DEBUG, log);
    _wifiConfigCounter = 2;
  }
}

boolean WIFI_WPSConfigDone(void)
{
  return (!_wpsresult);
}

boolean WIFI_beginWPSConfig(void)
{
  _wpsresult = 99;
  if (!wifi_wps_disable()) return false;
  if (!wifi_wps_enable(WPS_TYPE_PBC)) return false;  // so far only WPS_TYPE_PBC is supported (SDK 2.0.0)
  if (!wifi_set_wps_cb((wps_st_cb_t) &WIFI_wps_status_cb)) return false;
  if (!wifi_wps_start()) return false;
  return true;
}

void WIFI_config(uint8_t type)
{
  if (!_wificonfigflag) {
    if (type == WIFI_RETRY) return;
#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
    UDP_Disconnect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
    WiFi.disconnect();        // Solve possible Wifi hangs
    _wificonfigflag = type;
    _wifiConfigCounter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    _wificounter = _wifiConfigCounter +5;
    blinks = 1999;
    if (_wificonfigflag == WIFI_RESTART) {
      restartflag = 2;
    }
    else if (_wificonfigflag == WIFI_SMARTCONFIG) {
      addLog_P(LOG_LEVEL_INFO, PSTR("Smartconfig: Active for 1 minute"));
      WiFi.beginSmartConfig();
    }
    else if (_wificonfigflag == WIFI_WPSCONFIG) {
      if (WIFI_beginWPSConfig()) {
        addLog_P(LOG_LEVEL_INFO, PSTR("WPSconfig: Active for 1 minute"));
      } else {
        addLog_P(LOG_LEVEL_INFO, PSTR("WPSconfig: Failed to start"));
        _wifiConfigCounter = 3;
      }
    }
#ifdef USE_WEBSERVER
    else if (_wificonfigflag == WIFI_MANAGER) {
      addLog_P(LOG_LEVEL_INFO, PSTR("Wifimanager: Active for 1 minute"));
      beginWifiManager();
    }
#endif  // USE_WEBSERVER
  }
}

void WIFI_begin(uint8_t flag)
{
  const char PhyMode[] = " BGN";
  char log[LOGSZ];

#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
  UDP_Disconnect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
  if (!strncmp(ESP.getSdkVersion(),"1.5.3",5)) {
    addLog_P(LOG_LEVEL_DEBUG, "Wifi: Patch issue 2186");
    WiFi.mode(WIFI_OFF);    // See https://github.com/esp8266/Arduino/issues/2186
  }
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);      // Disable AP mode
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  if (!WiFi.getAutoConnect()) WiFi.setAutoConnect(true);
//  WiFi.setAutoReconnect(true);
  switch (flag) {
  case 0:  // AP1
  case 1:  // AP2
    sysCfg.sta_active = flag;
    break;
  case 2:  // Toggle
    sysCfg.sta_active ^= 1;
  }        // 3: Current AP
  if (strlen(sysCfg.sta_ssid[1]) == 0) sysCfg.sta_active = 0;
  WiFi.begin(sysCfg.sta_ssid[sysCfg.sta_active], sysCfg.sta_pwd[sysCfg.sta_active]);
  snprintf_P(log, sizeof(log), PSTR("Wifi: Connecting to AP%d %s in mode 11%c as %s..."),
    sysCfg.sta_active +1, sysCfg.sta_ssid[sysCfg.sta_active], PhyMode[WiFi.getPhyMode() & 0x3], Hostname);
  addLog(LOG_LEVEL_INFO, log);
}

void WIFI_check_ip()
{
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    _wificounter = WIFI_CHECK_SEC;
    _wifiretry = WIFI_RETRY_SEC;
    addLog_P((_wifistatus != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Connected"));
    _wifistatus = WL_CONNECTED;
  } else {
    _wifistatus = WiFi.status();
    switch (_wifistatus) {
      case WL_CONNECTED:
        addLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Connect failed as no IP address received"));
        _wifistatus = 0;
        _wifiretry = WIFI_RETRY_SEC;
        break;
      case WL_NO_SSID_AVAIL:
        addLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Connect failed as AP cannot be reached"));
        if (_wifiretry > (WIFI_RETRY_SEC / 2)) _wifiretry = WIFI_RETRY_SEC / 2;
        else if (_wifiretry) _wifiretry = 0;
        break;
      case WL_CONNECT_FAILED:
        addLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Connect failed with AP incorrect password"));
        if (_wifiretry > (WIFI_RETRY_SEC / 2)) _wifiretry = WIFI_RETRY_SEC / 2;
        else if (_wifiretry) _wifiretry = 0;
        break;
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        if (!_wifiretry || (_wifiretry == (WIFI_RETRY_SEC / 2))) {
          addLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Connect failed with AP timeout"));
        } else {
          addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: Attempting connection..."));
        }
    }
    if (_wifiretry) {
      if (_wifiretry == WIFI_RETRY_SEC) WIFI_begin(3);        // Select default SSID
      if (_wifiretry == (WIFI_RETRY_SEC / 2)) WIFI_begin(2);  // Select alternate SSID
      _wificounter = 1;
      _wifiretry--;
    } else {
      WIFI_config(sysCfg.sta_config);
      _wificounter = 1;
      _wifiretry = WIFI_RETRY_SEC;
    }
  }
}

void WIFI_Check(uint8_t param)
{
  char log[LOGSZ];

  _wificounter--;
  switch (param) {
  case WIFI_SMARTCONFIG:
  case WIFI_MANAGER:
  case WIFI_WPSCONFIG:
    WIFI_config(param);
    break;
  default:
    if (_wifiConfigCounter) {
      _wifiConfigCounter--;
      _wificounter = _wifiConfigCounter +5;
      if (_wifiConfigCounter) {
        if ((_wificonfigflag == WIFI_SMARTCONFIG) && WiFi.smartConfigDone()) _wifiConfigCounter = 0;
        if ((_wificonfigflag == WIFI_WPSCONFIG) && WIFI_WPSConfigDone()) _wifiConfigCounter = 0;
        if (!_wifiConfigCounter) {
          if (strlen(WiFi.SSID().c_str())) strlcpy(sysCfg.sta_ssid[0], WiFi.SSID().c_str(), sizeof(sysCfg.sta_ssid[0]));
          if (strlen(WiFi.psk().c_str())) strlcpy(sysCfg.sta_pwd[0], WiFi.psk().c_str(), sizeof(sysCfg.sta_pwd[0]));
          sysCfg.sta_active = 0;
          snprintf_P(log, sizeof(log), PSTR("Wificonfig: SSID1 %s and Password1 %s"), sysCfg.sta_ssid[0], sysCfg.sta_pwd[0]);
          addLog(LOG_LEVEL_INFO, log);
        }
      }
      if (!_wifiConfigCounter) {
        if (_wificonfigflag == WIFI_SMARTCONFIG) WiFi.stopSmartConfig();
        restartflag = 2;
      }
    } else {
      if (_wificounter <= 0) {
        addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Checking connection..."));
        _wificounter = WIFI_CHECK_SEC;
        WIFI_check_ip();
      }
      if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !_wificonfigflag) {
#ifdef USE_DISCOVERY
        if (!mDNSbegun) {
          mDNSbegun = MDNS.begin(Hostname);
          snprintf_P(log, sizeof(log), PSTR("mDNS: %s"), (mDNSbegun)?"Initialized":"Failed");
          addLog(LOG_LEVEL_INFO, log);
        }
#endif  // USE_DISCOVERY
#ifdef USE_WEBSERVER
        if (sysCfg.webserver) {
          startWebserver(sysCfg.webserver, WiFi.localIP());
#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
          MDNS.addService("http", "tcp", 80);
#endif  // WEBSERVER_ADVERTISE          
#endif  // USE_DISCOVERY
        } else {
          stopWebserver();
        }
#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
        UDP_Connect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
#endif  // USE_WEBSERVER
      } else {
#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
        UDP_Disconnect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
        mDNSbegun = false;
      }
    }
  }
}

int WIFI_State()
{
  int state;

  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) state = WIFI_RESTART;
  if (_wificonfigflag) state = _wificonfigflag;
  return state;
}

void WIFI_Connect(char *Hostname)
{
  WiFi.persistent(false);   // Solve possible wifi init errors
  WiFi.hostname(Hostname);
  _wifistatus = 0;
  _wifiretry = WIFI_RETRY_SEC;
  _wificounter = 1;
}

#ifdef USE_DISCOVERY
/*********************************************************************************************\
 * mDNS
\*********************************************************************************************/

#ifdef MQTT_HOST_DISCOVERY
boolean mdns_discoverMQTTServer()
{
  char log[LOGSZ], ip_str[20];
  int n;

  if (!mDNSbegun) return false;

  n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  snprintf_P(log, sizeof(log), PSTR("mDNS: Query done with %d mqtt services found"), n);
  addLog(LOG_LEVEL_INFO, log);

  if (n > 0) {
    // Note: current strategy is to get the first MQTT service (even when many are found)
    IPtoCharArray(MDNS.IP(0), ip_str, 20);
    
    snprintf_P(log, sizeof(log), PSTR("mDNS: Service found on %s ip %s port %d"),
      MDNS.hostname(0).c_str(), ip_str, MDNS.port(0));
    addLog(LOG_LEVEL_INFO, log);

    strlcpy(sysCfg.mqtt_host, ip_str, sizeof(sysCfg.mqtt_host));
    sysCfg.mqtt_port = MDNS.port(0);
  }

  return n > 0;
}
#endif  // MQTT_HOST_DISCOVERY

void IPtoCharArray(IPAddress address, char *ip_str, size_t size)
{
    String str = String(address[0]);
    str += ".";
    str += String(address[1]);
    str += ".";
    str += String(address[2]);
    str += ".";
    str += String(address[3]);
    str.toCharArray(ip_str, size);
}
#endif  // USE_DISCOVERY

/*********************************************************************************************\
 * Basic I2C routines
\*********************************************************************************************/

#ifdef USE_I2C
#define I2C_RETRY_COUNTER 3

int32_t i2c_read(uint8_t addr, uint8_t reg, uint8_t size)
{
  char log[LOGSZ];
  byte x = 0;
  int32_t data = 0;

  do {
    Wire.beginTransmission(addr);             // start transmission to device
    Wire.write(reg);                          // sends register address to read from
    if (Wire.endTransmission(false) == 0) {   // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      Wire.requestFrom((int)addr, (int)size); // send data n-bytes read
      if (Wire.available() == size)
        for(byte i = 0; i < size; i++) {
          data <<= 8;
          data |= Wire.read();                // receive DATA
        }
    }
    x++;
  } while (Wire.endTransmission(true) != 0 && x <= I2C_RETRY_COUNTER); // end transmission

//  snprintf_P(log, sizeof(log), PSTR("I2C: received %X, retries %d"), data, x -1);
//  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return data;
}

uint8_t i2c_read8(uint8_t addr, uint8_t reg)
{
  return i2c_read(addr, reg, 1);
}

uint16_t i2c_read16(uint8_t addr, uint8_t reg)
{
  return i2c_read(addr, reg, 2);
}

int16_t i2c_readS16(uint8_t addr, uint8_t reg)
{
  return (int16_t)i2c_read(addr, reg, 2);
}

uint16_t i2c_read16_LE(uint8_t addr, uint8_t reg)
{
  uint16_t temp = i2c_read(addr, reg, 2);
  return (temp >> 8) | (temp << 8);
}

int16_t i2c_readS16_LE(uint8_t addr, uint8_t reg)
{
  return (int16_t)i2c_read16_LE(addr, reg);
}

int32_t i2c_read24(uint8_t addr, uint8_t reg)
{
  return i2c_read(addr, reg, 3);
}

void i2c_write8(uint8_t addr, uint8_t reg, uint8_t val)
{
  byte x = I2C_RETRY_COUNTER;

  do {
    Wire.beginTransmission((uint8_t)addr);  // start transmission to device
    Wire.write(reg);                         // sends register address to read from
    Wire.write(val);                         // write data
    x--;
  } while (Wire.endTransmission(true) != 0 && x != 0); // end transmission
}

void i2c_scan(char *devs, unsigned int devs_len)
{
  byte error, address, any = 0;
  char tstr[10];

  snprintf_P(devs, devs_len, PSTR("{\"I2Cscan\":\"Device(s) found at"));
  for (address = 1; address <= 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      snprintf_P(tstr, sizeof(tstr), PSTR(" 0x%2x"), address);
      strncat(devs, tstr, devs_len);
      any = 1;
    }
    else if (error == 4) snprintf_P(devs, devs_len, PSTR("{\"I2Cscan\":\"Unknow error at 0x%2x\"}"), address);
  }
  if (any) {
    strncat(devs, "\"}", devs_len);
  } else {
    snprintf_P(devs, devs_len, PSTR("{\"I2Cscan\":\"No devices found\"}"));
  }
}
#endif  // USE_I2C

/*********************************************************************************************\
 * Real Time Clock
 *
 * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
 *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
\*********************************************************************************************/

extern "C" {
#include "sntp.h"
}

#define SECS_PER_MIN  ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY  ((uint32_t)(SECS_PER_HOUR * 24UL))
#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

Ticker tickerRTC;

static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char monthNames[37] = { "JanFebMrtAprMayJunJulAugSepOctNovDec" };

uint32_t utctime = 0, loctime = 0, dsttime = 0, stdtime = 0, ntptime = 0, midnight = 1451602800;

rtcCallback rtcCb = NULL;

void breakTime(uint32_t timeInput, TIME_T &tm)
{
// break the given timeInput into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year, month, monthLength;
  uint32_t time;
  unsigned long days;

  time = timeInput;
  tm.Second = time % 60;
  time /= 60;                // now it is minutes
  tm.Minute = time % 60;
  time /= 60;                // now it is hours
  tm.Hour = time % 24;
  time /= 24;                // now it is days
  tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.Year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.DayOfYear = time;

  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = monthDays[month];
    }

    if (time >= monthLength) {
      time -= monthLength;
    } else {
      break;
    }
  }
  strlcpy(tm.MonthName, monthNames + (month *3), 4);
  tm.Month = month + 1;      // jan is month 1
  tm.Day = time + 1;         // day of month
  tm.Valid = (timeInput > 1451602800);  // 2016-01-01
}

uint32_t makeTime(TIME_T &tm)
{
// assemble time elements into time_t
// note year argument is offset from 1970

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.Year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < tm.Month; i++) {
    if ((i == 2) && LEAP_YEAR(tm.Year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.Day - 1) * SECS_PER_DAY;
  seconds+= tm.Hour * SECS_PER_HOUR;
  seconds+= tm.Minute * SECS_PER_MIN;
  seconds+= tm.Second;
  return seconds;
}

uint32_t toTime_t(TimeChangeRule r, int yr)
{
    TIME_T tm;
    uint32_t t;
    uint8_t m, w;            // temp copies of r.month and r.week

    m = r.month;
    w = r.week;
    if (w == 0) {            // Last week = 0
        if (++m > 12) {      // for "Last", go to the next month
            m = 1;
            yr++;
        }
        w = 1;               // and treat as first week of next month, subtract 7 days later
    }

    tm.Hour = r.hour;
    tm.Minute = 0;
    tm.Second = 0;
    tm.Day = 1;
    tm.Month = m;
    tm.Year = yr - 1970;
    t = makeTime(tm);        // First day of the month, or first day of next month for "Last" rules
    breakTime(t, tm);
    t += (7 * (w - 1) + (r.dow - tm.Wday + 7) % 7) * SECS_PER_DAY;
    if (r.week == 0) t -= 7 * SECS_PER_DAY;    //back up a week if this is a "Last" rule
    return t;
}

String rtc_time(int type)
{
  char stime[25];   // Skip newline

  uint32_t time = utctime;
  if (type == 1) time = loctime;
  if (type == 2) time = dsttime;
  if (type == 3) time = stdtime;
  snprintf_P(stime, sizeof(stime), PSTR("%s"), sntp_get_real_time(time));
  return String(stime);
}

uint32_t rtc_loctime()
{
  return loctime;
}

uint32_t rtc_midnight()
{
  return midnight;
}

void rtc_second()
{
  char log[LOGSZ];
  byte ntpsync;
  uint32_t stdoffset, dstoffset;
  TIME_T tmpTime;

  ntpsync = 0;
  if (rtcTime.Year < 2016) {
    if (WiFi.status() == WL_CONNECTED) {
      ntpsync = 1;  // Initial NTP sync
    }
  } else {
    if ((rtcTime.Minute == 1) && (rtcTime.Second == 1)) {
      ntpsync = 1;  // Hourly NTP sync at xx:01:01
    }
  }
  if (ntpsync) {
    ntptime = sntp_get_current_timestamp();
    if (ntptime) {
      utctime = ntptime;
      breakTime(utctime, tmpTime);
      rtcTime.Year = tmpTime.Year + 1970;
      dsttime = toTime_t(myDST, rtcTime.Year);
      stdtime = toTime_t(mySTD, rtcTime.Year);
      snprintf_P(log, sizeof(log), PSTR("RTC: (UTC) %s"), rtc_time(0).c_str());
      addLog(LOG_LEVEL_DEBUG, log);
      snprintf_P(log, sizeof(log), PSTR("RTC: (DST) %s"), rtc_time(2).c_str());
      addLog(LOG_LEVEL_DEBUG, log);
      snprintf_P(log, sizeof(log), PSTR("RTC: (STD) %s"), rtc_time(3).c_str());
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
  utctime++;
  loctime = utctime;
  if (loctime > 1451602800) {  // 2016-01-01
    if (sysCfg.timezone == 99) {
      dstoffset = myDST.offset * SECS_PER_MIN;
      stdoffset = mySTD.offset * SECS_PER_MIN;
      if ((utctime >= (dsttime - stdoffset)) && (utctime < (stdtime - dstoffset))) {
        loctime += dstoffset;  // Daylight Saving Time
      } else {
        loctime += stdoffset;  // Standard Time
      }
    } else {
      loctime += sysCfg.timezone * SECS_PER_HOUR;
    }
  }
  breakTime(loctime, rtcTime);
  if (!rtcTime.Hour && !rtcTime.Minute && !rtcTime.Second && rtcTime.Valid) {
    midnight = loctime;
  }
  rtcTime.Year += 1970;
  if (rtcCb) rtcCb();
}

void rtc_init(rtcCallback cb)
{
  rtcCb = cb;
  sntp_setservername(0, (char*)NTP_SERVER1);
  sntp_setservername(1, (char*)NTP_SERVER2);
  sntp_setservername(2, (char*)NTP_SERVER3);
  sntp_stop();
  sntp_set_timezone(0);      // UTC time
  sntp_init();
  utctime = 0;
  breakTime(utctime, rtcTime);
  tickerRTC.attach(1, rtc_second);
}

/*********************************************************************************************\
 * Syslog
\*********************************************************************************************/

void syslog(const char *message)
{
  char str[TOPSZ+MESSZ];

  if (portUDP.beginPacket(sysCfg.syslog_host, sysCfg.syslog_port)) {
    snprintf_P(str, sizeof(str), PSTR("%s ESP-%s"), Hostname, message);
    portUDP.write(str);
    portUDP.endPacket();
  } else {
    syslog_level = 0;
    syslog_timer = SYSLOG_TIMER;
    snprintf_P(str, sizeof(str), PSTR("SYSL: Syslog Host not found so logging disabled for %d seconds. Consider syslog 0"), SYSLOG_TIMER);
    addLog(LOG_LEVEL_INFO, str);
  }
}

void addLog(byte loglevel, const char *line)
{
  char mxtime[9];

  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d"), rtcTime.Hour, rtcTime.Minute, rtcTime.Second);

#ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.printf("%s %s\n", mxtime, line);
#endif  // DEBUG_ESP_PORT
  if (loglevel <= seriallog_level) Serial.printf("%s %s\n", mxtime, line);
#ifdef USE_WEBSERVER
  if (loglevel <= sysCfg.weblog_level) {
    Log[logidx] = String(mxtime) + " " + String(line);
    logidx++;
    if (logidx > MAX_LOG_LINES -1) logidx = 0;
  }
#endif  // USE_WEBSERVER
  if ((WiFi.status() == WL_CONNECTED) && (loglevel <= syslog_level)) syslog(line);
}

void addLog_P(byte loglevel, const char *formatP)
{
  char mess[MESSZ];

  snprintf_P(mess, sizeof(mess), formatP);
  addLog(loglevel, mess);
}

/*********************************************************************************************\
 *
\*********************************************************************************************/
