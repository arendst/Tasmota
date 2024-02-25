/*
  xdrv_73_3_lora_sx1262.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
#ifdef USE_LORA_SX1262
/*********************************************************************************************\
 * Demo of LoRa using LilyGo T3S3 using SX1262 on 868MHz
 * 
 * Tasmota currently does not support user config of GPIO33 and GPIO34 on ESP32S3
\*********************************************************************************************/

#include <RadioLib.h>
SX1262 LoRa = nullptr;

struct {
  // flag to indicate that a packet was received
  volatile bool receivedFlag;
  // disable interrupt when it's not needed
  volatile bool enableInterrupt;

  bool sendFlag;
} Sx1262;

/*********************************************************************************************/

// this function is called when a complete packet is received by the module
// IMPORTANT: this function MUST be 'void' type and MUST NOT have any arguments!
void LoraSetFlagSx1262(void) {
  // check if the interrupt is enabled
  if (!Sx1262.enableInterrupt) { return; }
  // we got a packet, set the flag
  Sx1262.receivedFlag = true;
}

bool LoraAvailableSx1262(void) {
  // check if the flag is set
  return (Sx1262.receivedFlag);
}

int LoraInputSx1262(char* data) {
  int packet_size = 0;

  // disable the interrupt service routine while processing the data
  Sx1262.enableInterrupt = false;

  // reset flag
  Sx1262.receivedFlag = false;

//  String str;
//  int state = LoRa.readData(str);
  int state = LoRa.readData((uint8_t*)data, LORA_MAX_PACKET_LENGTH -1);
  if (state == RADIOLIB_ERR_NONE) { 
    if (!Sx1262.sendFlag) {
      // Find end of raw data being non-zero (No way to know raw data length)
      packet_size = LORA_MAX_PACKET_LENGTH;
      while (packet_size-- && (0 == data[packet_size]));
      if (0 != data[packet_size]) { packet_size++; }
    }
  }
  else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: CRC error"));
  }
  else {
    // some other error occurred
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Receive error %d"), state);
  }

  // put module back to listen mode
  LoRa.startReceive();

  Sx1262.sendFlag = false;
  // we're ready to receive more packets, enable interrupt service routine
  Sx1262.enableInterrupt = true;

  return packet_size;
}

bool LoraInitSx1262(void) {
//    LoRa = new Module(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_DI1), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_BUSY));
  LoRa = new Module(Pin(GPIO_LORA_CS), 33, Pin(GPIO_LORA_RST), 34);
  if (RADIOLIB_ERR_NONE == LoRa.begin(868.0)) {
    Sx1262.enableInterrupt = true;
    LoRa.setDio1Action(LoraSetFlagSx1262);
    if (RADIOLIB_ERR_NONE == LoRa.startReceive()) {
      return true;
    }
  }
  return false;
}

void LoraSendSx1262(char* data, uint32_t len) {
  Sx1262.sendFlag = true;
  LoRa.startTransmit(data, len);
}

#endif  // USE_LORA_SX1262
#endif  // USE_SPI_LORA
#endif  // USE_SPI

