extern "C" {
#include "spi_flash.h"
}

extern "C" uint32_t _SPIFFS_end;
#define CFG_LOCATION (((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2  // End of SPIFFS area

SYSCFG myCfg;

void CFG_Save()
{
  char log[LOGSZ];
  
  if (memcmp(&myCfg, &sysCfg, sizeof(SYSCFG))) {
    noInterrupts();
    if (sysCfg.saveFlag == 0) {  // Handle default and rollover
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
    }
    sysCfg.saveFlag++;
    spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
    spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
    interrupts();
    snprintf_P(log, LOGSZ, PSTR("Config: Saved configuration to flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
    addLog(LOG_LEVEL_DEBUG, log);
    myCfg = sysCfg;
  }
}

void CFG_Default()
{
  addLog(LOG_LEVEL_INFO, "Config: Use default configuration");
  memset(&sysCfg, 0x00, sizeof(SYSCFG));
  memset(&myCfg, 0x00, sizeof(SYSCFG));
  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.seriallog_level = SERIAL_LOG_LEVEL;
  sysCfg.syslog_level = SYS_LOG_LEVEL;
  strncpy(sysCfg.syslog_host, SYS_LOG_HOST, 32);
  strncpy(sysCfg.sta_ssid, STA_SSID, 32);
  strncpy(sysCfg.sta_pwd, STA_PASS, 64);
  strncpy(sysCfg.otaUrl, OTA_URL, 80);
  strncpy(sysCfg.mqtt_host, MQTT_HOST, 32);
  strncpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC, 32);
  strncpy(sysCfg.mqtt_topic, MQTT_TOPIC, 32);
  strncpy(sysCfg.mqtt_topic2, "0", 32);
  strncpy(sysCfg.mqtt_subtopic, MQTT_SUBTOPIC, 32);
  sysCfg.timezone = APP_TIMEZONE;
  sysCfg.power = APP_POWER;
  sysCfg.ledstate = APP_LEDSTATE;
  CFG_Save();
}

void CFG_Load()
{
  char log[LOGSZ];

  noInterrupts();
  spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
  spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32 *)&myCfg, sizeof(SYSCFG));
  interrupts();
  if (sysCfg.saveFlag < myCfg.saveFlag) sysCfg = myCfg;
  snprintf_P(log, LOGSZ, PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
  addLog(LOG_LEVEL_DEBUG, log);
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  myCfg = sysCfg;
}

void CFG_Erase()
{
  char log[LOGSZ];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  byte seriallog_level = sysCfg.seriallog_level;

  snprintf_P(log, LOGSZ, PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (LOG_LEVEL_DEBUG_MORE <= seriallog_level) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (result == SPI_FLASH_RESULT_OK)
        Serial.println(F(" OK"));
      else
        Serial.println(F(" Error"));
      delay(10);
    }
  }
}
