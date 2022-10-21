/*
  support_eeprom.ino - eeprom support for Sonoff-Tasmota

  Copyright (C) 2021  Theo Arends & Gerhard Mutz

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


  supports hardware i2c eeprom 24c256 and flash eeprom simulation on ESP8266 and ESP32
  all ESP8266 linker files contain 4k eeprom partition
  ESP32 requires esp32_partition_app1984k_spiffs60k.csv for 4k eeprom
*/


#ifdef USE_EEPROM

#ifdef USE_24C256
  // i2c eeprom
#include <Eeprom24C128_256.h>
#define EEPROM_ADDRESS 0x50
static Eeprom24C128_256 m_eeprom(EEPROM_ADDRESS);

void eeprom_writeBytes(uint32_t addr, uint32_t len, uint8_t *buff) {
  m_eeprom.writeBytes(addr,len,(uint8_t*)buff);
}
void eeprom_readBytes(uint32_t addr, uint32_t len, uint8_t *buff) {
  m_eeprom.readBytes(addr,len,(uint8_t*)buff);
}

uint32_t eeprom_init(uint32_t size) {
  if (TasmotaGlobal.i2c_enabled) {
    if (I2cActive(EEPROM_ADDRESS) || I2cSetDevice(EEPROM_ADDRESS)) {
      // eeprom is present
      I2cSetActiveFound(EEPROM_ADDRESS, "24C256");
      return 1;
    }
  }
  return 0;
}

#else // USE_24C256

#ifdef ESP8266
// esp8266 uses eeprom section
uint32_t eeprom_init(uint32_t size) {
  EEPROM.begin(size);
  return 1;
}
void eeprom_writeBytes(uint32_t adr, uint32_t len, uint8_t *buf) {
  for (uint32_t cnt=0; cnt<len; cnt++) {
    EEPROM.write(adr++, *buf++);
  }
  EEPROM.commit();
}

void eeprom_readBytes(uint32_t adr, uint32_t len, uint8_t *buf) {
  for (uint32_t cnt=0; cnt<len; cnt++) {
    *buf++ = EEPROM.read(adr++);
  }
}
#endif  // ESP8266
#ifdef ESP32
// esp32 uses eeprom section
uint32_t eeprom_init(uint32_t size) {
  return EEPROM.begin(size);
}
void eeprom_writeBytes(uint32_t addr, uint32_t len, uint8_t *buff) {
  EEPROM.writeBytes(addr, buff, len);
  EEPROM.commit();
}
void eeprom_readBytes(uint32_t addr, uint32_t len, uint8_t *buff) {
  EEPROM.readBytes(addr, buff, len);
}
#endif  // ESP32

#endif  // USE_24C256
#endif  // USE_EEPROM
