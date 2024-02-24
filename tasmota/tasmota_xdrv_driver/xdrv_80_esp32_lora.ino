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

#define XDRV_80                 80

#define LORA_MAX_PACKET_LENGTH  252  // Max packet length allowed (defined by RadioLib driver)

#include <RadioLib.h>
SX1262 LoRa = nullptr;

struct {
  // flag to indicate that a packet was received
  volatile bool receivedFlag;
  // disable interrupt when it's not needed
  volatile bool enableInterrupt;

  bool sendFlag;
  bool raw;
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

  // disable the interrupt service routine while processing the data
  Lora.enableInterrupt = false;

  // reset flag
  Lora.receivedFlag = false;

//  String str;
//  int state = LoRa.readData(str);
  char data[LORA_MAX_PACKET_LENGTH] = { 0 };
  int state = LoRa.readData((uint8_t*)data, LORA_MAX_PACKET_LENGTH -1);
  if (state == RADIOLIB_ERR_NONE) { 
    if (!Lora.sendFlag) {
      // Find end of raw data being non-zero (No way to know raw data length)
      uint32_t len = LORA_MAX_PACKET_LENGTH;
      while (len-- && (0 == data[len]));
      if (len) {
        len++;
        bool raw = Lora.raw;
        // Set raw mode if zeroes within data
        for (uint32_t i = 0; i < len; i++) {
          if (0 == data[i]) {
            raw = true;
            break;
          }
        }
        bool assume_json = (!raw && (data[0] == '{'));
        Response_P(PSTR("{\"LoRaReceived\":"));
        if (assume_json) {
          ResponseAppend_P(data);
        } else {
          ResponseAppend_P(PSTR("\""));
          if (raw) {
            ResponseAppend_P(PSTR("%*_H"), len, data);
          } else {
            ResponseAppend_P(EscapeJSONString(data).c_str());
          }
          ResponseAppend_P(PSTR("\""));
        }
        float rssi = LoRa.getRSSI();
        float snr = LoRa.getSNR();
        ResponseAppend_P(PSTR(",\"RSSI\":%1_f,\"SNR\":%1_f}"), &rssi, &snr);
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR("LoRaReceived"));
      }
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
  // LoRaSend "Hello Tiger"     - Send "Hello Tiger\n"
  // LoRaSend1 "Hello Tiger"    - Send "Hello Tiger\n"
  // LoRaSend2 "Hello Tiger"    - Send "Hello Tiger"
  // LoRaSend3 "Hello Tiger"    - Send "Hello Tiger\f"
  // LoRaSend4 = LoraSend2
  // LoRaSend5 "AA004566"       - Send "AA004566" as hex values
  // LoRaSend6 "72,101,108,108" - Send decimals as hex values
//  if (XdrvMailbox.index > 9) { XdrvMailbox.index -= 10; }                   // Allows leading spaces (not supported - See support_command/CommandHandler)
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    Lora.raw = (XdrvMailbox.index > 3);                                     // Global flag set even without data
    if (XdrvMailbox.data_len > 0) {
      uint8_t data[LORA_MAX_PACKET_LENGTH];
      uint32_t len = (XdrvMailbox.data_len < LORA_MAX_PACKET_LENGTH -1) ? XdrvMailbox.data_len : LORA_MAX_PACKET_LENGTH -2;
      if (1 == XdrvMailbox.index) {                                         // "Hello Tiger\n"
        memcpy(data, XdrvMailbox.data, len);
        data[len++] = (uint8_t)'\n';
      }
      else if ((2 == XdrvMailbox.index) || (4 == XdrvMailbox.index)) {      // "Hello Tiger" or "A0"
        memcpy(data, XdrvMailbox.data, len);
      }
      else if (3 == XdrvMailbox.index) {                                    // "Hello\f"
        Unescape(XdrvMailbox.data, &len);
        memcpy(data, XdrvMailbox.data, len);
      }
      else if (5 == XdrvMailbox.index) {                                    // "AA004566" as hex values
        char *p;
        char stemp[3];

        char *codes = RemoveSpace(XdrvMailbox.data);
        int size = strlen(XdrvMailbox.data);

        len = 0;
        while (size > 1) {
          strlcpy(stemp, codes, sizeof(stemp));
          data[len++] = strtol(stemp, &p, 16);
          if (len > LORA_MAX_PACKET_LENGTH -2) { break; }
          size -= 2;
          codes += 2;
        }
      }
      else if (6 == XdrvMailbox.index) {                                    // "72,101,108,108"
        char *p;
        uint8_t code;
        char *values = XdrvMailbox.data;
        len = 0;
        for (char* str = strtok_r(values, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
          data[len++] = (uint8_t)atoi(str);
          if (len > LORA_MAX_PACKET_LENGTH -2) { break; }
        }
      }
      else {
        len = 0;
      }
      if (len) {
        Lora.sendFlag = true;
        LoRa.startTransmit(data, len);
      }
      ResponseCmndDone();
    }
  }
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

