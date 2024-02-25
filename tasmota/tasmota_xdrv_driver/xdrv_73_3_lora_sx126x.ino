/*
  xdrv_73_3_lora_sx126x.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
#ifdef USE_LORA_SX126X
/*********************************************************************************************\
 * Semtech SX1261/62 Long Range (LoRa)
 * - LilyGo T3S3 LoRa32 868MHz ESP32S3 (uses SX1262)
 * - LilyGo TTGO T-Weigh ESP32 LoRa 868MHz HX711 (uses SX1262)
 * - Heltec (CubeCell) (uses SX1262)
 * - Waveshare
 * 
 * Used GPIO's:
 * - SPI_CLK
 * - SPI_MISO 
 * - SPI_MOSI
 * - LoRa_CS
 * - LoRa_Rst
 * - Lora_Busy (Tasmota currently does not support user config of GPIO34 on ESP32S3
 * - Lora_DI1  (Tasmota currently does not support user config of GPIO33 on ESP32S3
\*********************************************************************************************/

#include <RadioLib.h>
SX1262 LoRaRadio = nullptr;

/*********************************************************************************************/

void LoraOnReceiveSx126x(void) {
  if (!Lora.enableInterrupt) { return; }  // check if the interrupt is enabled
  Lora.receivedFlag = true;               // we got a packet, set the flag
}

bool LoraAvailableSx126x(void) {
  return (Lora.receivedFlag);             // check if the flag is set
}

int LoraReceiveSx126x(char* data) {
  Lora.enableInterrupt = false;           // disable the interrupt service routine while processing the data
  Lora.receivedFlag = false;              // reset flag

  int packet_size = 0;
  int state = LoRaRadio.readData((uint8_t*)data, LORA_MAX_PACKET_LENGTH -1);
  if (RADIOLIB_ERR_NONE == state) { 
    if (!Lora.sendFlag) {
      // Find end of raw data being non-zero (No way to know raw data length)
      packet_size = LORA_MAX_PACKET_LENGTH;
      while (packet_size-- && (0 == data[packet_size]));
      if (0 != data[packet_size]) { packet_size++; }
    }
  }
  else if (RADIOLIB_ERR_CRC_MISMATCH == state) {
    // packet was received, but is malformed
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: CRC error"));
  }
  else {
    // some other error occurred
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Receive error %d"), state);
  }

  LoRaRadio.startReceive();               // put module back to listen mode
  Lora.sendFlag = false;
  Lora.enableInterrupt = true;            // we're ready to receive more packets, enable interrupt service routine

  Lora.rssi = LoRaRadio.getRSSI();
  Lora.snr = LoRaRadio.getSNR();
  return packet_size;
}

bool LoraSendSx126x(char* data, uint32_t len) {
  Lora.sendFlag = true;
  int state = LoRaRadio.startTransmit(data, len);
  return (RADIOLIB_ERR_NONE == state);
}

bool LoraInitSx126x(void) {
//    LoRa = new Module(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_DI1), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_BUSY));
  LoRaRadio = new Module(Pin(GPIO_LORA_CS), 33, Pin(GPIO_LORA_RST), 34);
  if (RADIOLIB_ERR_NONE == LoRaRadio.begin(868.0)) {
    LoRaRadio.setDio1Action(LoraOnReceiveSx126x);
    if (RADIOLIB_ERR_NONE == LoRaRadio.startReceive()) {
      return true;
    }
  }
  return false;
}

#endif  // USE_LORA_SX126X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
