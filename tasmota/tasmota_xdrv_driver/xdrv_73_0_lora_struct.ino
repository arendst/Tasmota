/*
  xdrv_73_0_lora_struct.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA

#define LORA_MAX_PACKET_LENGTH  252    // Max packet length allowed (defined by RadioLib driver)

struct {
  bool (* Available)(void);
  int (* Receive)(char*);
  bool (* Send)(char*, uint32_t);
  float rssi;
  float snr;
  int packet_size;
  volatile bool receivedFlag;          // flag to indicate that a packet was received
  volatile bool enableInterrupt;       // disable interrupt when it's not needed
  bool sendFlag;
  bool raw;
  bool present;
} Lora;

#endif  // USE_SPI_LORA
#endif  // USE_SPI
