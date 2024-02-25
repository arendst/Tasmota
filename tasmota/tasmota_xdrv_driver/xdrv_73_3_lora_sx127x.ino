/*
  xdrv_73_3_lora_sx127x.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA
#ifdef USE_LORA_SX127X
/*********************************************************************************************\
 * SX127x and RFM95W
\*********************************************************************************************/

#include <LoRa.h>

/*********************************************************************************************/

// this function is called when a complete packet is received by the module
void LoraOnReceiveSx127x(int packet_size) {
  if (0 == packet_size) { return; }       // if there's no packet, return
  if (!Lora.enableInterrupt) { return; }  // check if the interrupt is enabled
  Lora.packet_size = packet_size;         // we got a packet, set the flag
}

bool LoraAvailableSx127x(void) {
  return (Lora.packet_size > 0);          // check if the flag is set
}

int LoraReceiveSx127x(char* data) {
  Lora.enableInterrupt = false;           // disable the interrupt service routine while processing the data

  int packet_size = 0;
  while (LoRa.available()) {              // read packet up to LORA_MAX_PACKET_LENGTH
    char sdata = LoRa.read();
    if (packet_size < LORA_MAX_PACKET_LENGTH -1) {
      data[packet_size++] = sdata;
    }
  }
  packet_size = (Lora.sendFlag) ? 0 : +1;

  Lora.sendFlag = false;
  Lora.packet_size = 0;                   // reset flag
  Lora.enableInterrupt = true;            // we're ready to receive more packets, enable interrupt service routine

  Lora.rssi = LoRa.packetRssi();
  Lora.snr = LoRa.packetSnr();
  return packet_size;
}

bool LoraSendSx127x(char* data, uint32_t len) {
  Lora.sendFlag = true;
  LoRa.beginPacket();                     // start packet
  LoRa.write((uint8_t*)data, len);        // send message
  LoRa.endPacket();                       // finish packet and send it
  LoRa.receive();                         // go back into receive mode
  return true;
}

bool LoraInitSx127x(void) {
  LoRa.setPins(Pin(GPIO_LORA_CS), Pin(GPIO_LORA_RST), Pin(GPIO_LORA_DI0));
  if (LoRa.begin(868E6)) {
//    LoRa.setSyncWord(0x12);
    LoRa.onReceive(LoraOnReceiveSx127x);
    LoRa.receive();
    return true;
  }
  return false;
}

#endif  // USE_LORA_SX127X
#endif  // USE_SPI_LORA
#endif  // USE_SPI
