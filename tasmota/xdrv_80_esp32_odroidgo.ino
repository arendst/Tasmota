/*
  xdrv_81_esp32_odroidgo.ino - ESP32 odroid go support for Tasmota

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

#ifdef ESP32
#ifdef USE_ODROID_GO
/*********************************************************************************************\
 * Odroid Go
 *
 * Clock frequency 160MHz (board_build.f_cpu       = 160000000L)
 * SPI Flash Size = 16MB (board_build.partitions  = esp32_partition_app1984k_spiffs12M.csv)
 *
 * To be done:
 * - Audio on GPIO25/26
 *
/*********************************************************************************************/

#endif  // USE_ODROID_GO
#endif  // ESP32
