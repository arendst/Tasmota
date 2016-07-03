extern "C" {
#include "spi_flash.h"
}

extern "C" uint32_t _SPIFFS_end;
#define CFG_LOCATION (((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2  // End of SPIFFS area

SYSCFG myCfg;

void CFG_Save()
{
  char log[80];
  
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
    sprintf_P(log, PSTR("Config: Saved configuration to flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
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
  strcpy(sysCfg.syslog_host, SYS_LOG_HOST);
  strcpy(sysCfg.sta_ssid, STA_SSID);
  strcpy(sysCfg.sta_pwd, STA_PASS);
  strcpy(sysCfg.otaUrl, OTA_URL);
  strcpy(sysCfg.mqtt_host, MQTT_HOST);
  strcpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC);
  strcpy(sysCfg.mqtt_topic, MQTT_TOPIC);
  strcpy(sysCfg.mqtt_topic2, "0");
  strcpy(sysCfg.mqtt_subtopic, MQTT_SUBTOPIC);
  sysCfg.timezone = APP_TIMEZONE;
  sysCfg.power = APP_POWER;
  sysCfg.ledstate = APP_LEDSTATE;
  CFG_Save();
}

void CFG_Load()
{
  char log[80];

  noInterrupts();
  spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
  spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32 *)&myCfg, sizeof(SYSCFG));
  interrupts();
  if (sysCfg.saveFlag < myCfg.saveFlag) sysCfg = myCfg;
  sprintf_P(log, PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
  addLog(LOG_LEVEL_DEBUG, log);
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  myCfg = sysCfg;
}

void CFG_Erase()
{
  char log[80];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  byte seriallog_level = sysCfg.seriallog_level;

  sprintf_P(log, PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
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
