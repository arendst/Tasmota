// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include <SPI.h>

#if defined(ARDUINO_SAMD_MKRWAN1300)
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 200000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      -1
#elif defined(ARDUINO_SAMD_MKRWAN1310)
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 200000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      LORA_IRQ
#else
#define LORA_DEFAULT_SPI           SPI
#define LORA_DEFAULT_SPI_FREQUENCY 8E6 
#define LORA_DEFAULT_SS_PIN        10
#define LORA_DEFAULT_RESET_PIN     9
#define LORA_DEFAULT_DIO0_PIN      2
#endif

#define PA_OUTPUT_RFO_PIN          0
#define PA_OUTPUT_PA_BOOST_PIN     1

class LoRaClass : public Stream {
public:
  LoRaClass();

  int begin(long frequency);
  void end();

  int beginPacket(int implicitHeader = false);
  int endPacket(bool async = false);

  int parsePacket(int size = 0);
  int packetRssi();
  float packetSnr();
  long packetFrequencyError();

  int rssi();

  // from Print
  virtual size_t write(uint8_t byte);
  virtual size_t write(const uint8_t *buffer, size_t size);

  // from Stream
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();

#ifndef ARDUINO_SAMD_MKRWAN1300
  void onReceive(void(*callback)(int));
  void onCadDone(void(*callback)(boolean));
  void onTxDone(void(*callback)());

  void receive(int size = 0);
  void channelActivityDetection(void);
#endif
  void idle();
  void sleep();

  void setTxPower(int level, int outputPin = PA_OUTPUT_PA_BOOST_PIN);
  void setFrequency(long frequency);
  void setSpreadingFactor(int sf);
  void setSignalBandwidth(long sbw);
  void setCodingRate4(int denominator);
  void setPreambleLength(long length);
  void setSyncWord(int sw);
  void enableCrc();
  void disableCrc();
  void enableInvertIQ();
  void disableInvertIQ();
  void enableLowDataRateOptimize();
  void disableLowDataRateOptimize();
  
  void setOCP(uint8_t mA); // Over Current Protection control
  
  void setGain(uint8_t gain); // Set LNA gain

  // deprecated
  void crc() { enableCrc(); }
  void noCrc() { disableCrc(); }

  byte random();

  void setPins(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN);
  void setSPI(SPIClass& spi);
  void setSPIFrequency(uint32_t frequency);

  void dumpRegisters(Stream& out);

private:
  void explicitHeaderMode();
  void implicitHeaderMode();

  void handleDio0Rise();
  bool isTransmitting();

  int getSpreadingFactor();
  long getSignalBandwidth();

  void setLdoFlag();
  void setLdoFlagForced(const boolean);

  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
  uint8_t singleTransfer(uint8_t address, uint8_t value);

  static void onDio0Rise();

private:
  SPISettings _spiSettings;
  SPIClass* _spi;
  int _ss;
  int _reset;
  int _dio0;
  long _frequency;
  int _packetIndex;
  int _implicitHeaderMode;
  void (*_onReceive)(int);
  void (*_onCadDone)(boolean);
  void (*_onTxDone)();
};

extern LoRaClass LoRa;

#endif
