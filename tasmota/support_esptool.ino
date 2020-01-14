/*
  support_esptool.ino - esptool support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#define USE_ESPTOOL
#ifdef USE_ESPTOOL
/*********************************************************************************************\
 * EspTool Erase function based on Version 2.8
 *
 * Source: https://github.com/espressif/esptool/blob/master/flasher_stub
\*********************************************************************************************/

// *** flasher_stub/include/soc_support.h
#define READ_REG(REG) (*((volatile uint32_t *)(REG)))
#define WRITE_REG(REG, VAL) *((volatile uint32_t *)(REG)) = (VAL)
#define REG_SET_MASK(reg, mask) WRITE_REG((reg), (READ_REG(reg)|(mask)))

#define SPI_BASE_REG       0x60000200             // SPI peripheral 0

#define SPI_CMD_REG       (SPI_BASE_REG + 0x00)
#define SPI_FLASH_RDSR    (1<<27)
#define SPI_FLASH_SE      (1<<24)
#define SPI_FLASH_BE      (1<<23)
#define SPI_FLASH_WREN    (1<<30)

#define SPI_ADDR_REG      (SPI_BASE_REG + 0x04)
#define SPI_CTRL_REG      (SPI_BASE_REG + 0x08)
#define SPI_RD_STATUS_REG (SPI_BASE_REG + 0x10)
#define SPI_W0_REG        (SPI_BASE_REG + 0x40)
#define SPI_EXT2_REG      (SPI_BASE_REG + 0xF8)

#define SPI_ST 0x7                                // Done state value

// *** flasher_stub/include/stub_flasher.h
#define SECTORS_PER_BLOCK (FLASH_BLOCK_SIZE / SPI_FLASH_SEC_SIZE)

// *** flasher_stub/stub_write_flash.c
static const uint32_t STATUS_WIP_BIT = (1 << 0);  // SPI status bits

// Wait for the SPI state machine to be ready, ie no command in progress in the internal host.
inline static void spi_wait_ready(void)
{
  while((READ_REG(SPI_EXT2_REG) & SPI_ST)) { }    // Wait for SPI state machine ready
}

// Returns true if the spiflash is ready for its next write operation.
// Doesn't block, except for the SPI state machine to finish any previous SPI host operation.
static bool spiflash_is_ready(void)
{
  spi_wait_ready();
  WRITE_REG(SPI_RD_STATUS_REG, 0);
  WRITE_REG(SPI_CMD_REG, SPI_FLASH_RDSR);         // Issue read status command
  while(READ_REG(SPI_CMD_REG) != 0) { }
  uint32_t status_value = READ_REG(SPI_RD_STATUS_REG);
  return (status_value & STATUS_WIP_BIT) == 0;
}

static void spi_write_enable(void)
{
  while(!spiflash_is_ready()) { }
  WRITE_REG(SPI_CMD_REG, SPI_FLASH_WREN);
  while(READ_REG(SPI_CMD_REG) != 0) { }
}

bool EsptoolEraseSector(uint32_t sector)
{
  spi_write_enable();
  spi_wait_ready();

  WRITE_REG(SPI_ADDR_REG, (sector * SPI_FLASH_SEC_SIZE) & 0xffffff);
  WRITE_REG(SPI_CMD_REG, SPI_FLASH_SE);           // Sector erase, 4KB
  while(READ_REG(SPI_CMD_REG) != 0) { }
  while(!spiflash_is_ready()) { }

  return true;
}

void EsptoolErase(uint32_t start_sector, uint32_t end_sector)
{
  int next_erase_sector = start_sector;
  int remaining_erase_sector = end_sector - start_sector;

  while (remaining_erase_sector > 0) {
    spi_write_enable();

    uint32_t command = SPI_FLASH_SE;              // Sector erase, 4kB
    uint32_t sectors_to_erase = 1;
    if (remaining_erase_sector >= SECTORS_PER_BLOCK &&
        next_erase_sector % SECTORS_PER_BLOCK == 0) {
      command = SPI_FLASH_BE;                     // Block erase 64kB if we have space for it
      sectors_to_erase = SECTORS_PER_BLOCK;
    }
    uint32_t addr = next_erase_sector * SPI_FLASH_SEC_SIZE;

    spi_wait_ready();
    WRITE_REG(SPI_ADDR_REG, addr & 0xffffff);
    WRITE_REG(SPI_CMD_REG, command);              // Perform erase, 4kB or 65kB
    while(READ_REG(SPI_CMD_REG) != 0) { }
    remaining_erase_sector -= sectors_to_erase;
    next_erase_sector += sectors_to_erase;

    while (!spiflash_is_ready()) { }
    yield();
    OsWatchLoop();
  }
}

#endif  // USE_ESPTOOL
