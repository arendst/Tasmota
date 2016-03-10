extern "C" {
#include "spi_flash.h"
}

extern "C" uint32_t _SPIFFS_end;
#define CFG_LOCATION (((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2  // End of SPIFFS area

SYSCFG myCfg;

void CFG_Save()
{
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
    DEBUG_MSG("APP: Saved configuration to flash at %X and count %d\n", CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
    myCfg = sysCfg;
  }
}

void CFG_Default()
{
  DEBUG_MSG("APP: Use default configuration\n");
  os_memset(&sysCfg, 0x00, sizeof(SYSCFG));
  os_memset(&myCfg, 0x00, sizeof(SYSCFG));
  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  strcpy(sysCfg.sta_ssid, STA_SSID);
  strcpy(sysCfg.sta_pwd, STA_PASS);
  strcpy(sysCfg.mqtt_host, MQTT_HOST);
  strcpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC);
  strcpy(sysCfg.mqtt_topic, MQTT_TOPIC);
  strcpy(sysCfg.otaUrl, OTA_URL);
  strcpy(sysCfg.mqtt_subtopic, MQTT_SUBTOPIC);
  sysCfg.timezone = APP_TIMEZONE;
  sysCfg.power = APP_POWER;
  CFG_Save();
}

void CFG_Load()
{
  noInterrupts();
  spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
  spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32 *)&myCfg, sizeof(SYSCFG));
  interrupts();
  if (sysCfg.saveFlag < myCfg.saveFlag) sysCfg = myCfg;
  DEBUG_MSG("APP: Loaded configuration from flash at %X and count %d\n", CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  myCfg = sysCfg;
}
