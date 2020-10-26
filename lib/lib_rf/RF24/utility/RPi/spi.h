/*
 * TMRh20 2015
 * SPI layer for RF24 <-> BCM2835
 */
/**
 * @file spi.h
 * \cond HIDDEN_SYMBOLS
 * Class declaration for SPI helper files
 */
#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <stdio.h>
#include "bcm2835.h"
#include "interrupt.h"

#define SPI_HAS_TRANSACTION
#define MSBFIRST BCM2835_SPI_BIT_ORDER_MSBFIRST
#define SPI_MODE0 BCM2835_SPI_MODE0
#define RF24_SPI_SPEED BCM2835_SPI_SPEED_8MHZ
    
class SPISettings {
public:
	SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
        init(clock,bitOrder,dataMode);
	}
    SPISettings() { init(RF24_SPI_SPEED, MSBFIRST, SPI_MODE0); }

    uint32_t clck;
    uint8_t border;
    uint8_t dmode;
private:

	void init(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
                clck = clock;
                border = bitOrder;
                dmode = dataMode;
	}
	friend class SPIClass;
};


class SPI {
public:

  SPI();
  virtual ~SPI();
  
  inline static uint8_t transfer(uint8_t _data);
  inline static void transfernb(char* tbuf, char* rbuf, uint32_t len);
  inline static void transfern(char* buf, uint32_t len);  

  static void begin(int busNo);
  static void end();

  static void setBitOrder(uint8_t bit_order);
  static void setDataMode(uint8_t data_mode);
  static void setClockDivider(uint16_t spi_speed);
  static void chipSelect(int csn_pin);
  
  static void beginTransaction(SPISettings settings);
  static void endTransaction();
  
  
};


uint8_t SPI::transfer(uint8_t _data) {
    uint8_t data = bcm2835_spi_transfer(_data);
    return data;
}

void SPI::transfernb(char* tbuf, char* rbuf, uint32_t len){
   bcm2835_spi_transfernb( tbuf, rbuf, len);
}

void SPI::transfern(char* buf, uint32_t len)
{
    transfernb(buf, buf, len);
}
/**
 * \endcond
 */
#endif
