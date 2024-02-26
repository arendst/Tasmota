/*
  xdrv_73_0_lora_struct.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA

#define LORA_MAX_PACKET_LENGTH  252    // Max packet length allowed (defined by RadioLib driver)

struct {
  bool (* Config)(void);
  bool (* Available)(void);
  int (* Receive)(char*);
  bool (* Send)(char*, uint32_t);
  float rssi;
  float snr;
  int packet_size;
  float frequency;                     // 868.0 MHz
  float bandwidth;                     // 125.0 kHz
  int spreading_factor;                // 9
  int coding_rate;                     // 7
  int sync_word;                       // 0x12
  int output_power;                    // 10 dBm
  long preamble_length;                // 8 symbols
  float current_limit;                 // 60.0 mA (Overcurrent Protection (OCP))
  int implicit_header;                 // 0
  bool crc_bytes;                      // 2 bytes
  uint8_t gain;
  volatile bool receivedFlag;          // flag to indicate that a packet was received
  volatile bool enableInterrupt;       // disable interrupt when it's not needed
  bool sendFlag;
  bool raw;
  bool present;
} Lora;

#endif  // USE_SPI_LORA
#endif  // USE_SPI
