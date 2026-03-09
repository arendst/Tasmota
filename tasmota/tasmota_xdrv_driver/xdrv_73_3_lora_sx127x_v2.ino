/*
  xdrv_73_3_lora_sx127x_v2.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only                                                                                 
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
#ifdef USE_LORA_SX127X
/*********************************************************************************************\
 * Legacy Semtech SX1276/77/78/79 Long Range (LoRa)
 * - HopeRF RFM95W, RFM96W and RFM98W
 * - LilyGo TTGO T3 LoRa32 868MHz ESP32 (uses SX1276)
 * - LilyGo TTGO T-Higrow 868MHz (uses SX1276)
 * - DFRobot FireBeetle Covers LoRa Radio 868MHz (uses SX1278)
 * - M5Stack LoRa868 (uses AI-01 which uses SX1276)
 * - Modtronix
 * 
 * Used GPIO's:
 * - SPI_CLK
 * - SPI_MISO 
 * - SPI_MOSI
 * - LoRa_CS
 * - LoRa_Rst
 * - Lora_DIO0
 * - Lora_DIO1 (busy)
\*********************************************************************************************/

#include <RadioLib.h>
SX1276 LoRaRadio7x = nullptr;              // Select LoRa support

void IRAM_ATTR LoraSx127xV2OnReceive(void);
void LoraSx127xV2OnReceive(void) {
  if (!Lora->send_flag && !Lora->received_flag) {
    Lora->receive_time = millis();
  }
  Lora->received_flag = true;              // we got a packet, set the flag
}

bool LoraSx127xV2Available(void) {
  if (Lora->send_flag) {
    Lora->received_flag = false;           // Reset receive flag as it was caused by send interrupt

    uint32_t time = millis();
    int state = LoRaRadio7x.startReceive();  // Put module back to listen mode
    Lora->send_flag = false;
    if (state != RADIOLIB_ERR_NONE) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("S6X: Rcvd (%d) restarted (%d)"), time, state);
    }
  }
  return Lora->received_flag;              // Check if the receive flag is set
}

int LoraSx127xV2Receive(char* data) {
  Lora->received_flag = false;             // Reset flag
  int packet_size = LoRaRadio7x.getPacketLength();
  int state = LoRaRadio7x.readData((uint8_t*)data, TAS_LORA_MAX_PACKET_LENGTH -1);
  // LoRaWan downlink frames are sent without CRC, which will raise error on SX127x. We can ignore that error
  if (RADIOLIB_ERR_CRC_MISMATCH == state) {
    state = RADIOLIB_ERR_NONE;
    AddLog(LOG_LEVEL_DEBUG, PSTR("S7X: Ignoring CRC error"));
  }
  if (RADIOLIB_ERR_NONE == state) { 
    Lora->rssi = LoRaRadio7x.getRSSI();
    Lora->snr = LoRaRadio7x.getSNR();
  } else {
    packet_size = 0;                       // Some other error occurred
    AddLog(LOG_LEVEL_DEBUG, PSTR("S7X: Receive error %d"), state);
  }
  return packet_size;
}

bool LoraSx127xV2Send(uint8_t* data, uint32_t len, bool invert) {
  int state1 = RADIOLIB_ERR_NONE;
  int state2 = RADIOLIB_ERR_NONE;
  if (invert) {
    LoRaRadio7x.standby();
    state1 = LoRaRadio7x.invertIQ(true);
  }
  int state = LoRaRadio7x.transmit(data, len);
  Lora->send_flag = true;                  // Use this flag as LoRaRadio.transmit enable send interrupt
  if (invert) {
    state2 = LoRaRadio7x.invertIQ(false);
    LoRaRadio7x.standby();
  }
  if (state != RADIOLIB_ERR_NONE || state1 != RADIOLIB_ERR_NONE || state2 != RADIOLIB_ERR_NONE) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("S7X: Send error %d %d %d"), state1, state, state2);
  }
  return (RADIOLIB_ERR_NONE == state);
}

bool LoraSx127xV2Config(bool full) {
  LoRaRadio7x.setSpreadingFactor(Lora->settings.spreading_factor);
  LoRaRadio7x.setBandwidth(Lora->settings.bandwidth);
  LoRaRadio7x.setFrequency(Lora->settings.frequency);
  if (full) {
    LoRaRadio7x.setCodingRate(Lora->settings.coding_rate);
    LoRaRadio7x.setSyncWord(Lora->settings.sync_word);
    LoRaRadio7x.setPreambleLength(Lora->settings.preamble_length);
    LoRaRadio7x.setCurrentLimit(Lora->settings.current_limit);
    LoRaRadio7x.setCRC(Lora->settings.crc_bytes);
    LoRaRadio7x.setOutputPower(Lora->settings.output_power);
    if (Lora->settings.implicit_header) { 
      LoRaRadio7x.implicitHeader(Lora->settings.implicit_header);
    } else { 
      LoRaRadio7x.explicitHeader();
    }
    LoRaRadio7x.invertIQ(false);
  }
  return true;
}

bool LoraSx127xV2Init(void) {
  LoRaRadio7x = new Module(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_DI0), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_DI1));
  if (RADIOLIB_ERR_NONE == LoRaRadio7x.begin(Lora->settings.frequency)) {
    LoraSx127xV2Config(true);
    LoRaRadio7x.setPacketReceivedAction(LoraSx127xV2OnReceive);
    if (RADIOLIB_ERR_NONE == LoRaRadio7x.startReceive()) {
      return true;
    }
  }
  return false;
}

#endif  // USE_LORA_SX127X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
