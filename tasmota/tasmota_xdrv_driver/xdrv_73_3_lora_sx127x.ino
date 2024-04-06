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

void LoraSx127xOnReceive(int packet_size) {
  // This function is called when a complete packet is received by the module
#ifdef USE_LORA_DEBUG
//  AddLog(LOG_LEVEL_DEBUG, PSTR("S7X: Packet size %d"), packet_size);
#endif    
  if (0 == packet_size) { return; }        // if there's no packet, return
  if (!Lora->receive_time) {
    Lora->receive_time = millis();
  }
  Lora->received_flag = true;              // we got a packet, set the flag
}

bool LoraSx127xAvailable(void) {
  return Lora->received_flag;              // check if the flag is set
}

int LoraSx127xReceive(char* data) {
  Lora->received_flag = false;             // reset flag
  int packet_size = 0;
  int sdata = LoRa.read();
  while ((sdata > -1) && (packet_size < TAS_LORA_MAX_PACKET_LENGTH -1)) {  // Read packet up to LORA_MAX_PACKET_LENGTH
    data[packet_size++] = (char)sdata;
    sdata = LoRa.read();
  }
  Lora->rssi = LoRa.packetRssi();
  Lora->snr = LoRa.packetSnr();
  return packet_size;
}

bool LoraSx127xSend(uint8_t* data, uint32_t len, bool invert) {
  if (invert) {
    LoRa.enableInvertIQ();                 // active invert I and Q signals
  }
  LoRa.beginPacket(Lora->settings.implicit_header);  // start packet
  LoRa.write(data, len);                   // send message
  LoRa.endPacket();                        // finish packet and send it
  if (invert) {
    LoRa.disableInvertIQ();                // normal mode
  }
  LoRa.receive();                          // go back into receive mode
  return true;
}

bool LoraSx127xConfig(void) {
  LoRa.setFrequency(Lora->settings.frequency * 1000 * 1000);
  LoRa.setSignalBandwidth(Lora->settings.bandwidth * 1000);
  LoRa.setSpreadingFactor(Lora->settings.spreading_factor);
  LoRa.setCodingRate4(Lora->settings.coding_rate);
  LoRa.setSyncWord(Lora->settings.sync_word);
  LoRa.setTxPower(Lora->settings.output_power);
  LoRa.setPreambleLength(Lora->settings.preamble_length);
  LoRa.setOCP(Lora->settings.current_limit);
  if (Lora->settings.crc_bytes) {
    LoRa.enableCrc();
  } else {
    LoRa.disableCrc();
  }
/*    
  if (Lora->settings.implicit_header) { 
    LoRa.implicitHeaderMode();
  } else { 
    LoRa.explicitHeaderMode();
  }
*/
  LoRa.disableInvertIQ();                  // normal mode
  return true;
}

bool LoraSx127xInit(void) {
  LoRa.setPins(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_DI0));
  if (LoRa.begin(Lora->settings.frequency * 1000 * 1000)) {
    LoraSx127xConfig();
    LoRa.onReceive(LoraSx127xOnReceive);
    LoRa.receive();
    return true;
  }
  return false;
}

#endif  // USE_LORA_SX127X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
