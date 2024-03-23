/*
  xdrv_73_3_lora_sx127x.ino - LoRa support for Tasmota

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
 * - LoRa_DIO0
\*********************************************************************************************/

#include <LoRa.h>                          // extern LoRaClass LoRa;

void LoraOnReceiveSx127x(int packet_size) {
  // This function is called when a complete packet is received by the module
#ifdef USE_LORA_DEBUG
//  AddLog(LOG_LEVEL_DEBUG, PSTR("S7X: Packet size %d"), packet_size);
#endif    
  if (0 == packet_size) { return; }        // if there's no packet, return
  if (!Lora.receive_time) {
    Lora.receive_time = millis();
  }
  Lora.packet_size = packet_size;          // we got a packet, set the flag
}

bool LoraAvailableSx127x(void) {
  return (Lora.packet_size > 0);           // check if the flag is set
}

int LoraReceiveSx127x(char* data) {
  int packet_size = 0;
  while (LoRa.available()) {               // read packet up to LORA_MAX_PACKET_LENGTH
    char sdata = LoRa.read();
    if (packet_size < TAS_LORA_MAX_PACKET_LENGTH -1) {
      data[packet_size++] = sdata;
    }
  }
  Lora.rssi = LoRa.packetRssi();
  Lora.snr = LoRa.packetSnr();
  Lora.packet_size = 0;                    // reset flag
  return packet_size;
}

bool LoraSendSx127x(uint8_t* data, uint32_t len, bool invert) {
  if (invert) {
    LoRa.enableInvertIQ();                 // active invert I and Q signals
  }
  LoRa.beginPacket(LoraSettings.implicit_header);  // start packet
  LoRa.write(data, len);                   // send message
  LoRa.endPacket();                        // finish packet and send it
  if (invert) {
    LoRa.disableInvertIQ();                // normal mode
  }
  LoRa.receive();                          // go back into receive mode
  return true;
}

bool LoraConfigSx127x(void) {
  LoRa.setFrequency(LoraSettings.frequency * 1000 * 1000);
  LoRa.setSignalBandwidth(LoraSettings.bandwidth * 1000);
  LoRa.setSpreadingFactor(LoraSettings.spreading_factor);
  LoRa.setCodingRate4(LoraSettings.coding_rate);
  LoRa.setSyncWord(LoraSettings.sync_word);
  LoRa.setTxPower(LoraSettings.output_power);
  LoRa.setPreambleLength(LoraSettings.preamble_length);
  LoRa.setOCP(LoraSettings.current_limit);
  if (LoraSettings.crc_bytes) {
    LoRa.enableCrc();
  } else {
    LoRa.disableCrc();
  }
/*    
  if (LoraSettings.implicit_header) { 
    LoRa.implicitHeaderMode();
  } else { 
    LoRa.explicitHeaderMode();
  }
*/
  LoRa.disableInvertIQ();                  // normal mode
  return true;
}

bool LoraInitSx127x(void) {
  LoRa.setPins(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_DI0));
  if (LoRa.begin(LoraSettings.frequency * 1000 * 1000)) {
    LoraConfigSx127x();
    LoRa.onReceive(LoraOnReceiveSx127x);
    LoRa.receive();
    return true;
  }
  return false;
}

#endif  // USE_LORA_SX127X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
