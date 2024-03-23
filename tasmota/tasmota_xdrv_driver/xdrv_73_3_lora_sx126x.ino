/*
  xdrv_73_3_lora_sx126x.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
#ifdef USE_LORA_SX126X
/*********************************************************************************************\
 * Latest Semtech SX1261/62 Long Range (LoRa)
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
 * - Lora_Busy
 * - Lora_DI1
\*********************************************************************************************/

#include <RadioLib.h>
SX1262 LoRaRadio = nullptr;               // Select LoRa support

void LoraOnReceiveSx126x(void) {
  // This is called after EVERY type of enabled interrupt so chk for valid receivedFlag in LoraAvailableSx126x()
  if (!Lora.sendFlag && !Lora.receivedFlag && !Lora.receive_time) {
    Lora.receive_time = millis();
  }
  Lora.receivedFlag = true;               // we got a packet, set the flag
}

bool LoraAvailableSx126x(void) {
  if (Lora.receivedFlag && Lora.sendFlag) {
    Lora.receivedFlag = false;            // Reset receive flag as it was caused by send interrupt
    Lora.sendFlag = false;
    LoRaRadio.startReceive();             // Put module back to listen mode
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("S6X: Receive restarted"));
  }
  return (Lora.receivedFlag);             // Check if the receive flag is set
}

int LoraReceiveSx126x(char* data) {
  Lora.receivedFlag = false;              // Reset flag
  int packet_size = LoRaRadio.getPacketLength();
  int state = LoRaRadio.readData((uint8_t*)data, TAS_LORA_MAX_PACKET_LENGTH -1);
  // LoRaWan downlink frames are sent without CRC, which will raise error on SX126x. We can ignore that error
  if (RADIOLIB_ERR_CRC_MISMATCH == state) {
    state = RADIOLIB_ERR_NONE;
    AddLog(LOG_LEVEL_DEBUG, PSTR("S6X: Ignoring CRC error"));
  }
  if (RADIOLIB_ERR_NONE == state) { 
    Lora.rssi = LoRaRadio.getRSSI();
    Lora.snr = LoRaRadio.getSNR();
  } else {
    packet_size = 0;                      // Some other error occurred
    AddLog(LOG_LEVEL_DEBUG, PSTR("S6X: Receive error %d"), state);
  }
  return packet_size;
}

bool LoraSendSx126x(uint8_t* data, uint32_t len, bool invert) {
  Lora.sendFlag = true;                   // Use this flag as LoRaRadio.transmit enable send interrupt
  if (invert) {
    LoRaRadio.invertIQ(true);
  }
  int state = LoRaRadio.transmit(data, len);
  if (invert) {
    LoRaRadio.invertIQ(false);
  }
  return (RADIOLIB_ERR_NONE == state);
}

bool LoraConfigSx126x(void) {
  LoRaRadio.setCodingRate(LoraSettings.coding_rate);
  LoRaRadio.setSyncWord(LoraSettings.sync_word);
  LoRaRadio.setPreambleLength(LoraSettings.preamble_length);
  LoRaRadio.setCurrentLimit(LoraSettings.current_limit);
  LoRaRadio.setCRC(LoraSettings.crc_bytes);
  LoRaRadio.setSpreadingFactor(LoraSettings.spreading_factor);
  LoRaRadio.setBandwidth(LoraSettings.bandwidth);
  LoRaRadio.setFrequency(LoraSettings.frequency);
  LoRaRadio.setOutputPower(LoraSettings.output_power);
  if (LoraSettings.implicit_header) { 
    LoRaRadio.implicitHeader(LoraSettings.implicit_header);
  } else { 
    LoRaRadio.explicitHeader();
  }
  LoRaRadio.invertIQ(false);
  return true;
}

bool LoraInitSx126x(void) {
  LoRaRadio = new Module(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_DI1), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_BUSY));
  if (RADIOLIB_ERR_NONE == LoRaRadio.begin(LoraSettings.frequency)) {
    LoraConfigSx126x();
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
