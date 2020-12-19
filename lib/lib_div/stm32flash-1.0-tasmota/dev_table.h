/*
  stm32flash - Open Source ST STM32 flash program for Arduino
  Copyright (C) 2010 Geoffrey McRae <geoff@spacevs.com>
  Copyright (C) 2014-2015 Antonio Borneo <borneo.antonio@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#define SZ_128	0x00000080
#define SZ_256	0x00000100
#define SZ_1K	0x00000400
#define SZ_2K	0x00000800
#define SZ_16K	0x00004000
#define SZ_32K	0x00008000
#define SZ_64K	0x00010000
#define SZ_128K	0x00020000
#define SZ_256K	0x00040000

/*
 * Page-size for page-by-page flash erase.
 * Arrays are zero terminated; last non-zero value is automatically repeated
 */

/* fixed size pages */
static uint32_t p_128[] = { SZ_128, 0 };
static uint32_t p_256[] = { SZ_256, 0 };
static uint32_t p_1k[]  = { SZ_1K,  0 };
static uint32_t p_2k[]  = { SZ_2K,  0 };
/* F2 and F4 page size */
static uint32_t f2f4[]  = { SZ_16K, SZ_16K, SZ_16K, SZ_16K, SZ_64K, SZ_128K, 0 };
/* F4 dual bank page size */
static uint32_t f4db[]  = {
	SZ_16K, SZ_16K, SZ_16K, SZ_16K, SZ_64K, SZ_128K, SZ_128K, SZ_128K,
	SZ_16K, SZ_16K, SZ_16K, SZ_16K, SZ_64K, SZ_128K, 0
};
/* F7 page size */
static uint32_t f7[]    = { SZ_32K, SZ_32K, SZ_32K, SZ_32K, SZ_128K, SZ_256K, 0 };

/*
 * Device table, corresponds to the "Bootloader device-dependant parameters"
 * table in ST document AN2606.
 * Note that the option bytes upper range is inclusive!
 */
const stm32_dev_t devices[] = {
	/* ID   SRAM-address-range      FLASH-address-range    PPS  PSize   Option-byte-addr-range  System-mem-addr-range   Flags */
	/* F0 */
  // "STM32F030x8/F05xxx" = Shelly Dimmer 2
	{0x440, 0x20000800, 0x20002000, 0x08000000, 0x08010000,  4, p_1k  , 0x1FFFF800, 0x1FFFF80F, 0x1FFFEC00, 0x1FFFF800, 0},
  // "STM32F03xx4/6" = Shelly Dimmer 1
	{0x444, 0x20000800, 0x20001000, 0x08000000, 0x08008000,  4, p_1k  , 0x1FFFF800, 0x1FFFF80F, 0x1FFFEC00, 0x1FFFF800, 0},
	{0x0}
};
