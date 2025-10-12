#ifndef UDISPLAY_SPI_LOWLEVEL_H
#define UDISPLAY_SPI_LOWLEVEL_H

#include "uDisplay_config.h"

// ===== SPI Platform-Specific Includes =====
#ifdef ESP32
#include "soc/spi_reg.h"
#include "soc/spi_struct.h"
#include "esp32-hal-spi.h"
#include "esp32-hal.h"
#include "soc/spi_struct.h"
#endif

// ===== GPIO Control Macros =====

#ifdef ESP8266
#define PIN_OUT_SET 0x60000304
#define PIN_OUT_CLEAR 0x60000308
#define GPIO_SET(A) WRITE_PERI_REG( PIN_OUT_SET, 1 << A)
#define GPIO_CLR(A) WRITE_PERI_REG( PIN_OUT_CLEAR, 1 << A)
#define GPIO_CLR_SLOW(A) digitalWrite(A, LOW)
#define GPIO_SET_SLOW(A) digitalWrite(A, HIGH)
#else // ESP32
#undef GPIO_SET
#undef GPIO_CLR
#undef GPIO_SET_SLOW
#undef GPIO_CLR_SLOW

#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32P4
#define GPIO_CLR(A) GPIO.out_w1tc.val = (1 << A)
#define GPIO_SET(A) GPIO.out_w1ts.val = (1 << A)
#else // plain ESP32
#define GPIO_CLR(A) GPIO.out_w1tc = (1 << A)
#define GPIO_SET(A) GPIO.out_w1ts = (1 << A)
#endif

#define GPIO_CLR_SLOW(A) digitalWrite(A, LOW)
#define GPIO_SET_SLOW(A) digitalWrite(A, HIGH)
#endif

// ===== SPI Transaction Control Macros =====
#define SPI_BEGIN_TRANSACTION if (spi_nr <= 2) beginTransaction(spiSettings);
#define SPI_END_TRANSACTION if (spi_nr <= 2) endTransaction();

#define SPI_CS_LOW if (spi_cs >= 0) GPIO_CLR_SLOW(spi_cs);
#define SPI_CS_HIGH if (spi_cs >= 0) GPIO_SET_SLOW(spi_cs);
#define SPI_DC_LOW if (spi_dc >= 0) GPIO_CLR_SLOW(spi_dc);
#define SPI_DC_HIGH if (spi_dc >= 0) GPIO_SET_SLOW(spi_dc);

// ===== Function Declarations =====
// These would typically be in the class declaration in uDisplay.h
// but we list them here for reference:

/*
// Low-Level SPI Write Functions
void hw_write9(uint8_t val, uint8_t dc);
void write8(uint8_t val);
void write8_slow(uint8_t val);
void write9(uint8_t val, uint8_t dc);
void write9_slow(uint8_t val, uint8_t dc);
void write16(uint16_t val);
void write32(uint32_t val);

// RA8876 Specific Functions  
uint8_t writeReg16(uint8_t reg, uint16_t wval);
uint8_t readData(void);
uint8_t readStatus(void);
*/

#endif // UDISPLAY_SPI_LOWLEVEL_H