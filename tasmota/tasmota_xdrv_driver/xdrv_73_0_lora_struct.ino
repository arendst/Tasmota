/*
  xdrv_73_0_lora_struct.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA

#define LORA_MAX_PACKET_LENGTH  252  // Max packet length allowed (defined by RadioLib driver)

#endif  // USE_SPI_LORA
#endif  // USE_SPI
