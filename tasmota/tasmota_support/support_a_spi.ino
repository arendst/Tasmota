/*
  support_a_spi.ino - SPI support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
/*********************************************************************************************\
 * Basic SPI routines supporting two busses
\*********************************************************************************************/

SPIClass *SpiBegin(uint32 bus = 1);
SPIClass *SpiBegin(uint32 bus) {
  SPIClass *spi;
  if (1 == bus) {                // SPI bus 1
    if (TasmotaGlobal.spi_enabled) {
      spi = &SPI;                // Uses VSPI on ESP32 or FSPI on all other ESP32 derivatives (See SPI.cpp)
#ifdef ESP8266
      spi->begin();
#endif // ESP8266
#ifdef ESP32      
      spi->begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif  // ESP32
      return spi;
    }
  }
#ifdef ESP32
  if (2 == bus) {                // SPI bus 2
    if (TasmotaGlobal.spi_enabled2) {
      spi = new SPIClass(HSPI);  // See framework-arduinoespressif32 esp32-hal-spi.h
      spi->begin(Pin(GPIO_SPI_CLK, 1), Pin(GPIO_SPI_MISO, 1), Pin(GPIO_SPI_MOSI, 1), -1);
      return spi;
    }
  }
#endif  // ESP32
  return nullptr;
}

#endif  // USE_SPI

/********************************************************************************************/

void AddLogSpi(uint32_t hardware, int clk, int mosi, int miso) {
#ifndef FIRMWARE_MINIMAL
  uint32_t enabled = TasmotaGlobal.soft_spi_enabled;
  char hwswbus[8];
  if (hardware) {
#ifdef ESP8266
    strcpy_P(hwswbus, PSTR("Hard"));
    enabled = TasmotaGlobal.spi_enabled;
#endif      
#ifdef ESP32
    strcpy_P(hwswbus, PSTR("Bus0"));
    hwswbus[3] += (char)hardware;
    enabled = (1 == hardware) ? TasmotaGlobal.spi_enabled : TasmotaGlobal.spi_enabled2;
#endif
  } else {
    strcpy_P(hwswbus, PSTR("Soft"));
  }
  switch(enabled) {
    case SPI_MOSI:
      AddLog(LOG_LEVEL_INFO, PSTR("SPI: %s using GPIO%02d(CLK) and GPIO%02d(MOSI)"),
        hwswbus, clk, mosi);
      break;
    case SPI_MISO:
      AddLog(LOG_LEVEL_INFO, PSTR("SPI: %s using GPIO%02d(CLK) and GPIO%02d(MISO)"),
        hwswbus, clk, miso);
      break;
    case SPI_MOSI_MISO:
      AddLog(LOG_LEVEL_INFO, PSTR("SPI: %s using GPIO%02d(CLK), GPIO%02d(MOSI) and GPIO%02d(MISO)"),
        hwswbus, clk, mosi, miso);
      break;
  }
#endif // FIRMWARE_MINIMAL
}
