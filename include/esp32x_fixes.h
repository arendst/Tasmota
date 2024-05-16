/*
  esp32x_fixes.h - fix esp32x toolchain

  Copyright (C) 2021  Theo Arends

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
*/

/*
 * Xtensa toolchain declares `int32_t` as `int` but RISC-V toolchain
 * declares `int32_t` as `long int` which causes compilation errors.
 *
 * See:
 *   https://github.com/espressif/esp-idf/issues/6906
 *   https://github.com/espressif/arduino-esp32/issues/5086
 *
 * You need to add the following lines in `build_flags`:
 *                            -I$PROJECT_DIR/include
 *                            -include "esp32x_fixes.h"
 */
#ifdef __riscv

#undef __INT32_TYPE__
#define __INT32_TYPE__      int

#undef __UINT32_TYPE__
#define __UINT32_TYPE__     unsigned int

#endif // __riscv

//alias, deprecated for the chips after esp32s2
#ifdef CONFIG_IDF_TARGET_ESP32
#define SPI_HOST    SPI1_HOST
#define HSPI_HOST   SPI2_HOST
#define VSPI_HOST   SPI3_HOST

#elif CONFIG_IDF_TARGET_ESP32S2
// SPI_HOST (SPI1_HOST) is not supported by the SPI Master and SPI Slave driver on ESP32-S2 and later
#define SPI_HOST    SPI1_HOST
#define FSPI_HOST   SPI2_HOST
#define HSPI_HOST   SPI3_HOST
#define VSPI_HOST   SPI3_HOST

#elif CONFIG_IDF_TARGET_ESP32S3
// SPI_HOST (SPI1_HOST) is not supported by the SPI Master and SPI Slave driver on ESP32-S2 and later
#define SPI_HOST    SPI1_HOST
#define FSPI_HOST   SPI2_HOST
#define HSPI_HOST   SPI3_HOST
#define VSPI_HOST   SPI3_HOST
// SPI_MOSI_DLEN_REG is not defined anymore in esp32s3
#define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)

#elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
#define SPI_HOST    SPI1_HOST
#define HSPI_HOST   SPI2_HOST
#define VSPI_HOST   SPI2_HOST  /* No SPI3_host on C2/C6 */
#define VSPI        SPI
// SPI_MOSI_DLEN_REG is not defined anymore
#define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)

#endif // TARGET

// This trick makes sure that 'lto' optimizer does not inline `delay()
// so we can override it with `-Wl,--wrap=delay` linker directive
#ifdef __cplusplus
extern "C"
#endif // _cplusplus
void  delay(__UINT32_TYPE__ ms) __attribute__((noinline)) __attribute__ ((noclone));
