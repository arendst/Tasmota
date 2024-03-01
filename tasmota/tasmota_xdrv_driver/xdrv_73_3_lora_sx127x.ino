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

//#define USE_LORA_SX127X_CAD

#include <LoRa.h>                          // extern LoRaClass LoRa;

#ifdef USE_LORA_SX127X_CAD
void LoraOnCadDoneSx127x(boolean signalDetected) {
  if (signalDetected) {                    // detect preamble
#ifdef USE_LORA_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Signal detected"));
#endif    
    LoRa.receive();                        // put the radio into continuous receive mode
  } else {
    LoRa.channelActivityDetection();       // try next activity dectection
  }
}
#endif  // USE_LORA_SX127X_CAD

// this function is called when a complete packet is received by the module
void LoraOnReceiveSx127x(int packet_size) {
#ifdef USE_LORA_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("LOR: Packet size %d"), packet_size);
#endif    
  if (0 == packet_size) { return; }        // if there's no packet, return
  if (!Lora.enableInterrupt) { return; }   // check if the interrupt is enabled
  Lora.packet_size = packet_size;          // we got a packet, set the flag
}

bool LoraAvailableSx127x(void) {
  return (Lora.packet_size > 0);           // check if the flag is set
}

int LoraReceiveSx127x(char* data) {
  Lora.enableInterrupt = false;            // disable the interrupt service routine while processing the data

  int packet_size = 0;
  while (LoRa.available()) {               // read packet up to LORA_MAX_PACKET_LENGTH
    char sdata = LoRa.read();
    if (packet_size < LORA_MAX_PACKET_LENGTH -1) {
      data[packet_size++] = sdata;
    }
  }
//  if (Lora.sendFlag) { packet_size = 0; }
//  Lora.sendFlag = false;

  Lora.packet_size = 0;                    // reset flag
  Lora.enableInterrupt = true;             // we're ready to receive more packets, enable interrupt service routine

  Lora.rssi = LoRa.packetRssi();
  Lora.snr = LoRa.packetSnr();
#ifdef USE_LORA_SX127X_CAD
  LoRa.channelActivityDetection();         // put the radio into CAD mode
#endif  // USE_LORA_SX127X_CAD
  return packet_size;
}

bool LoraSendSx127x(uint8_t* data, uint32_t len) {
//  Lora.sendFlag = true;
  LoRa.beginPacket(Lora.implicit_header);  // start packet
  LoRa.write(data, len);         // send message
  LoRa.endPacket();                        // finish packet and send it
  LoRa.receive();                          // go back into receive mode
  return true;
}

bool LoraConfigSx127x(void) {
  LoRa.setFrequency(Lora.frequency * 1000 * 1000);
  LoRa.setSignalBandwidth(Lora.bandwidth * 1000);
  LoRa.setSpreadingFactor(Lora.spreading_factor);
  LoRa.setCodingRate4(Lora.coding_rate);
  LoRa.setSyncWord(Lora.sync_word);
  LoRa.setTxPower(Lora.output_power);
  LoRa.setPreambleLength(Lora.preamble_length);
  LoRa.setOCP(Lora.current_limit);
  if (Lora.crc_bytes) {
    LoRa.enableCrc();
  } else {
    LoRa.disableCrc();
  }
/*    
  if (Lora.implicit_header) { 
    LoRa.implicitHeaderMode();
  } else { 
    LoRa.explicitHeaderMode();
  }
*/
  return true;
}

bool LoraInitSx127x(void) {
  LoRa.setPins(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_DI0));
  if (LoRa.begin(Lora.frequency * 1000 * 1000)) {
    LoraConfigSx127x();
    LoRa.onReceive(LoraOnReceiveSx127x);
#ifdef USE_LORA_SX127X_CAD
    LoRa.onCadDone(LoraOnCadDoneSx127x);   // register the channel activity dectection callback
    LoRa.channelActivityDetection();
#else
    LoRa.receive();
#endif  // USE_LORA_SX127X_CAD
    return true;
  }
  return false;
}

#endif  // USE_LORA_SX127X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
