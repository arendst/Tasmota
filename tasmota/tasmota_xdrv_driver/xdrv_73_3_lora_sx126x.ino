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
 * - Waveshare SX1262 Lora Node (HF) and (LF)
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

//#define USE_LORA_SX126X_DEBUG

#include <RadioLib.h>
SX1262 LoRaRadio = nullptr;                // Select LoRa support

bool LoraSx126xBusy(void) {
  // This is not consistently implemented in the used library
  uint32_t timeout;
  SetNextTimeInterval(timeout, 100);
  while ((1 == digitalRead(Pin(GPIO_LORA_BUSY))) && !TimeReached(timeout)) {
    delay(0);
  }
  return TimeReached(timeout);
}

/*********************************************************************************************/

void IRAM_ATTR LoraSx126xOnInterrupt(void);
void LoraSx126xOnInterrupt(void) {
  // This is called after EVERY type of enabled interrupt so chk for valid receivedFlag in LoraAvailableSx126x()
  if (!Lora->send_flag && !Lora->received_flag && !Lora->receive_time) {
    Lora->receive_time = millis();
  }
  Lora->received_flag = true;              // we got a packet, set the flag
}

bool LoraSx126xAvailable(void) {
  if (Lora->send_flag) {
    Lora->received_flag = false;           // Reset receive flag as it was caused by send interrupt

    uint32_t time = millis();
    int state = LoRaRadio.startReceive();  // Put module back to listen mode
    Lora->send_flag = false;
    if (state != RADIOLIB_ERR_NONE) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("S6X: Rcvd (%d) restarted (%d)"), time, state);
    }
  }
  else if (Lora->received_flag) {
    uint32_t irq_stat = LoRaRadio.getIrqStatus();

#ifdef USE_LORA_SX126X_DEBUG
    if (irq_stat != 0) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("S6X: Flag (%d) irq %04X"), millis(), irq_stat);
    }
#endif  // USE_LORA_SX126X_DEBUG

    if (0 == (irq_stat & RADIOLIB_SX126X_IRQ_RX_DONE)) {
      Lora->received_flag = false;         // Reset receive flag
    }
  }
  return (Lora->received_flag);            // Check if the receive flag is set
}

int LoraSx126xReceive(char* data) {
  Lora->received_flag = false;             // Reset flag
  int packet_size = LoRaRadio.getPacketLength();
  int state = LoRaRadio.readData((uint8_t*)data, TAS_LORA_MAX_PACKET_LENGTH -1);
  // LoRaWan downlink frames are sent without CRC, which will raise error on SX126x. We can ignore that error
  if (RADIOLIB_ERR_CRC_MISMATCH == state) {
    state = RADIOLIB_ERR_NONE;
    AddLog(LOG_LEVEL_DEBUG, PSTR("S6X: Ignoring CRC error"));
  }
  if (RADIOLIB_ERR_NONE == state) { 
    Lora->rssi = LoRaRadio.getRSSI();
    Lora->snr = LoRaRadio.getSNR();
  } else {
    packet_size = 0;                       // Some other error occurred
    AddLog(LOG_LEVEL_DEBUG, PSTR("S6X: Receive error %d"), state);
  }
  return packet_size;
}

bool LoraSx126xSend(uint8_t* data, uint32_t len, bool invert) {
  int state1 = RADIOLIB_ERR_NONE;
  int state2 = RADIOLIB_ERR_NONE;
  if (invert) {
    LoRaRadio.standby();
    state1 = LoRaRadio.invertIQ(true);
    LoraSx126xBusy();
  }
  int state = LoRaRadio.transmit(data, len);
  Lora->send_flag = true;                  // Use this flag as LoRaRadio.transmit enable send interrupt
  if (invert) {
    LoraSx126xBusy();
    state2 = LoRaRadio.invertIQ(false);
    LoRaRadio.standby();
  }
  if (state != RADIOLIB_ERR_NONE || state1 != RADIOLIB_ERR_NONE || state2 != RADIOLIB_ERR_NONE) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("S6X: Send error %d %d %d"), state1, state, state2);
  }
  return (RADIOLIB_ERR_NONE == state);
}

bool LoraSx126xConfig(void) {
  LoRaRadio.setCodingRate(Lora->settings.coding_rate);
  LoRaRadio.setSyncWord(Lora->settings.sync_word);
  LoRaRadio.setPreambleLength(Lora->settings.preamble_length);
  LoRaRadio.setCurrentLimit(Lora->settings.current_limit);
  LoRaRadio.setCRC(Lora->settings.crc_bytes);
  LoRaRadio.setSpreadingFactor(Lora->settings.spreading_factor);
  LoRaRadio.setBandwidth(Lora->settings.bandwidth);
  LoRaRadio.setFrequency(Lora->settings.frequency);
  LoRaRadio.setOutputPower(Lora->settings.output_power);
  if (Lora->settings.implicit_header) { 
    LoRaRadio.implicitHeader(Lora->settings.implicit_header);
  } else { 
    LoRaRadio.explicitHeader();
  }
  LoRaRadio.invertIQ(false);
  return true;
}

bool LoraSx126xInit(void) {
  LoRaRadio = new Module(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_DI1), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_BUSY));
  if (RADIOLIB_ERR_NONE == LoRaRadio.begin(Lora->settings.frequency)) {
    LoraSx126xConfig();
    LoRaRadio.setDio1Action(LoraSx126xOnInterrupt);
    if (RADIOLIB_ERR_NONE == LoRaRadio.startReceive()) {
      return true;
    }
  }
  return false;
}

#endif  // USE_LORA_SX126X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
