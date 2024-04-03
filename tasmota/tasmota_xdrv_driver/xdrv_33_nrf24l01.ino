/*
  xdrv_33_nrf24l01.ino - nrf24l01 support for Tasmota

  Copyright (C) 2021  Christian Baars and Theo Arends

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


  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------

  0.9.0.1 20200624  changes - removed unused legacy code
  ---
  0.9.0.0 20191127  started - further development by Christian Baars
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/

#ifdef USE_SPI
#ifdef USE_NRF24

/*********************************************************************************************\
* NRF24l01(+)
*
* Usage: 5 SPI-data-wires plus VVC/ground, use hardware SPI, select GPIO_NRF24_CS/GPIO_NRF24_DC
\*********************************************************************************************/

#define XDRV_33             33

#include <RF24.h>

const char NRF24type[] PROGMEM = "NRF24";

struct {
  uint8_t chipType = 0; // NRF24l01 active: 32 - NRF24L01 , 43- NRF24L01+  ... we mis-use ascii-codes
} NRF24;

/********************************************************************************************/

RF24 NRF24radio;

bool NRF24initRadio() {
  NRF24radio.begin(Pin(GPIO_NRF24_CS), Pin(GPIO_NRF24_DC));
  NRF24radio.powerUp();

  if (NRF24radio.isChipConnected()) {
    DEBUG_DRIVER_LOG(PSTR("NRF: Chip connected"));
    return true;
  }
  DEBUG_DRIVER_LOG(PSTR("NRF: Chip NOT !!!! connected"));
  return false;
}

void NRF24Detect(void) {
  if (PinUsed(GPIO_NRF24_CS) && PinUsed(GPIO_NRF24_DC) && TasmotaGlobal.spi_enabled) {
    if (NRF24initRadio()) {
      NRF24.chipType = 32; // SPACE
      AddLog(LOG_LEVEL_INFO, PSTR("NRF: Model 24L01 initialized"));
      if (NRF24radio.isPVariant()) {
        NRF24.chipType = 43; // +
        AddLog(LOG_LEVEL_INFO, PSTR("NRF: Model 24L01+ detected"));
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv33(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    NRF24Detect();
  } else if (NRF24.chipType) {
    if (FUNC_ACTIVE == function) {
      result = true;
    }
  }
  return result;
}

#endif  // USE_NRF24
#endif  // USE_SPI

