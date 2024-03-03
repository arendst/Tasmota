/*
  xdrv_73_0_lora_struct.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
/*********************************************************************************************\
 * LoRa defines and global struct
\*********************************************************************************************/

//#define USE_LORA_DEBUG

#define LORA_MAX_PACKET_LENGTH     252    // Max packet length allowed (keeping room for control bytes)
#define TAS_LORA_REMOTE_COMMAND   0x17    // Header defining remote LoRaCommand

#ifndef TAS_LORA_FREQUENCY
#define TAS_LORA_FREQUENCY         868.0  // Allowed values range from 150.0 to 960.0 MHz
#endif
#ifndef TAS_LORA_BANDWIDTH
#define TAS_LORA_BANDWIDTH         125.0  // Allowed values are 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0 kHz
#endif
#ifndef TAS_LORA_SPREADING_FACTOR
#define TAS_LORA_SPREADING_FACTOR    9    // Allowed values range from 5 to 12
#endif
#ifndef TAS_LORA_CODING_RATE
#define TAS_LORA_CODING_RATE         7    // Allowed values range from 5 to 8
#endif
#ifndef TAS_LORA_SYNC_WORD
#define TAS_LORA_SYNC_WORD        0x12    // Allowed values range from 1 to 255
#endif
#ifndef TAS_LORA_OUTPUT_POWER
#define TAS_LORA_OUTPUT_POWER       10    // Allowed values range from 1 to 20
#endif
#ifndef TAS_LORA_PREAMBLE_LENGTH
#define TAS_LORA_PREAMBLE_LENGTH     8    // Allowed values range from 1 to 65535
#endif
#ifndef TAS_LORA_CURRENT_LIMIT
#define TAS_LORA_CURRENT_LIMIT      60.0  // Overcurrent Protection - OCP in mA
#endif
#ifndef TAS_LORA_HEADER
#define TAS_LORA_HEADER              0    // Explicit (0) or Implicit (1 to 4) Header
#endif
#ifndef TAS_LORA_CRC_BYTES
#define TAS_LORA_CRC_BYTES           2    // No (0) or Number (1 to 4) of CRC bytes
#endif

struct {
  bool (* Config)(void);
  bool (* Available)(void);
  int (* Receive)(char*);
  bool (* Send)(uint8_t*, uint32_t);
  float rssi;
  float snr;
  float frequency;                     // 868.0 MHz
  float bandwidth;                     // 125.0 kHz
  float current_limit;                 // 60.0 mA (Overcurrent Protection (OCP))
  uint16_t preamble_length;            // 8 symbols
  uint8_t sync_word;                   // 0x12
  uint8_t spreading_factor;            // 9
  uint8_t coding_rate;                 // 7
  uint8_t output_power;                // 10 dBm
  uint8_t implicit_header;             // 0
  uint8_t crc_bytes;                   // 2 bytes
  uint8_t packet_size;                 // Max is 255 (LORA_MAX_PACKET_LENGTH)
  volatile bool receivedFlag;          // flag to indicate that a packet was received
  volatile bool enableInterrupt;       // disable interrupt when it's not needed
  bool sendFlag;
  bool raw;
  bool present;
} Lora;

#endif  // USE_SPI_LORA
#endif  // USE_SPI
