/*
  xdrv_80_esp32_lora.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef USE_SPI
#ifdef USE_SPI_LORA
#ifdef USE_LORA_SX1262
/*********************************************************************************************\
 * Demo of LoRa using LilyGo T3S3 using SX1262 on 868MHz
 * 
 * Tasmota currently does not support user config of GPIO33 and GPIO34 on ESP32S3
\*********************************************************************************************/

#define XDRV_80              80

#include <RadioLib.h>
SX1262 LoRa = nullptr;

struct {
  // flag to indicate that a packet was received
  volatile bool receivedFlag;
  // flag to indicate that a packet was send
  volatile bool sendFlag;
  // disable interrupt when it's not needed
  volatile bool enableInterrupt;

  bool present;
} Lora;

/*********************************************************************************************/

// this function is called when a complete packet is received by the module
// IMPORTANT: this function MUST be 'void' type and MUST NOT have any arguments!
void LoraSetFlag(void) {
  // check if the interrupt is enabled
  if (!Lora.enableInterrupt) { return; }
  // we got a packet, set the flag
  Lora.receivedFlag = true;
}

/*********************************************************************************************/

void LoraInput(void) {
  // check if the flag is set
  if (!Lora.receivedFlag) { return; }

  // disable the interrupt service routine while
  // processing the data
  Lora.enableInterrupt = false;

  // reset flag
  Lora.receivedFlag = false;

  // you can read received data as an Arduino String
  String str;
  int state = LoRa.readData(str);

  // you can also read received data as byte array
  /*
  byte byteArr[8];
  int state = Lora.readData(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) { 
    if (!Lora.sendFlag) {
      float rssi = LoRa.getRSSI();
      float snr = LoRa.getSNR();
      AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Data '%s', RSSI %1_f dBm, SNR %1_f dB"), str.c_str(), &rssi, &snr);
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

  Lora.sendFlag = false;
  // we're ready to receive more packets,
  // enable interrupt service routine
  Lora.enableInterrupt = true;
}

void LoraInit(void) {
  if ((SPI_MOSI_MISO == TasmotaGlobal.spi_enabled) &&
      (PinUsed(GPIO_LORA_CS)) && (PinUsed(GPIO_LORA_RST))) {
    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);

//    LoRa = new Module(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_DI1), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_BUSY));
    LoRa = new Module(Pin(GPIO_LORA_CS), 33, Pin(GPIO_LORA_RST), 34);
    if (RADIOLIB_ERR_NONE == LoRa.begin(868.0)) {
      Lora.enableInterrupt = true;
      LoRa.setDio1Action(LoraSetFlag);
      if (RADIOLIB_ERR_NONE == LoRa.startReceive()) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Initialized"));
        Lora.present = true;
      }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_LORASEND   "Send"

const char kLoraCommands[] PROGMEM = "LoRa|"  // Prefix
  D_CMND_LORASEND;

void (* const LoraCommand[])(void) PROGMEM = {
  &CmndLoraSend };

void CmndLoraSend(void) {
  if (XdrvMailbox.data_len > 0) {
    Lora.sendFlag = true;
    LoRa.startTransmit(XdrvMailbox.data);
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv80(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    LoraInit();
  }
  else if (Lora.present) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        LoraInput();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kLoraCommands, LoraCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_LORA_SX1262
#endif  // USE_SPI_LORA
#endif  // USE_SPI
#endif  // ESP32

