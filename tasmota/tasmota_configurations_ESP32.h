/*
  tasmota_configurations_ESP32.h - ESP32 only Configurations for Tasmota

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

#ifndef _TASMOTA_CONFIGURATIONS_ESP32_H_
#define _TASMOTA_CONFIGURATIONS_ESP32_H_

#ifdef ESP32

/*********************************************************************************************\
 * [tasmota32-webcam.bin]
 * Provide an image with useful supported sensors enabled
\*********************************************************************************************/

#ifdef FIRMWARE_WEBCAM

#undef CODE_IMAGE_STR
#define CODE_IMAGE_STR "webcam"

#define USE_WEBCAM
#endif  // FIRMWARE_WEBCAM

#endif  // ESP32

#endif  // _TASMOTA_CONFIGURATIONS_ESP32_H_
