#include <Adafruit_SPIDevice.h>
#include <Arduino.h>

//#define DEBUG_SERIAL Serial

/*!
 *    @brief  Create an SPI device with the given CS pin and settins
 *    @param  cspin The arduino pin number to use for chip select
 *    @param  freq The SPI clock frequency to use, defaults to 1MHz
 *    @param  dataOrder The SPI data order to use for bits within each byte, defaults to SPI_BITORDER_MSBFIRST
 *    @param  dataMode The SPI mode to use, defaults to SPI_MODE0
 *    @param  theSPI The SPI bus to use, defaults to &theSPI
 */
Adafruit_SPIDevice::Adafruit_SPIDevice(int8_t cspin, uint32_t freq, BitOrder dataOrder, uint8_t dataMode, SPIClass *theSPI) {
  _cs = cspin;
  _sck = _mosi = _miso = -1;
  _spi = theSPI;
  _begun = false;
  _spiSetting = new SPISettings(freq, dataOrder, dataMode);
  _freq = freq;
  _dataOrder = dataOrder;
  _dataMode = dataMode;
}

/*!
 *    @brief  Create an SPI device with the given CS pin and settins
 *    @param  cspin The arduino pin number to use for chip select
 *    @param  sckpin The arduino pin number to use for SCK
 *    @param  misopin The arduino pin number to use for MISO, set to -1 if not used
 *    @param  mosipin The arduino pin number to use for MOSI, set to -1 if not used
 *    @param  freq The SPI clock frequency to use, defaults to 1MHz
 *    @param  dataOrder The SPI data order to use for bits within each byte, defaults to SPI_BITORDER_MSBFIRST
 *    @param  dataMode The SPI mode to use, defaults to SPI_MODE0
 */
Adafruit_SPIDevice::Adafruit_SPIDevice(int8_t cspin, int8_t sckpin, int8_t misopin, int8_t mosipin, 
				       uint32_t freq, BitOrder dataOrder, uint8_t dataMode) {
  _cs = cspin;
  _sck = sckpin;
  _miso = misopin;
  _mosi = mosipin;
  _freq = freq;
  _dataOrder = dataOrder;
  _dataMode = dataMode;
  _begun = false;
  _spiSetting = new SPISettings(freq, dataOrder, dataMode);
  _spi = NULL;
}


/*!
 *    @brief  Initializes SPI bus and sets CS pin high
 *    @return Always returns true because there's no way to test success of SPI init
 */
bool Adafruit_SPIDevice::begin(void) {
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);

  if (_spi) { // hardware SPI
    _spi->begin();
  } else {
    pinMode(_sck, OUTPUT);

    if (_dataMode==SPI_MODE0) {
      digitalWrite(_sck, HIGH);
    } else {
      digitalWrite(_sck, LOW);
    }
    if (_mosi != -1) {
      pinMode(_mosi, OUTPUT);
      digitalWrite(_mosi, HIGH);
    }
    if (_miso != -1) {
      pinMode(_miso, INPUT);
    }
  }
    
  _begun = true;
  return true;
}


/*!
 *    @brief  Transfer (send/receive) one byte over hard/soft SPI
 *    @param  buffer The buffer to send and receive at the same time
 *    @param  len    The number of bytes to transfer
 */
void Adafruit_SPIDevice::transfer(uint8_t *buffer, size_t len) {
  if (_spi) {
    // hardware SPI is easy
    _spi->transfer(buffer, len);
    return;
  }

  // for softSPI we'll do it by hand
  for (size_t i=0; i<len; i++) {
    // software SPI
    uint8_t reply = 0;
    uint8_t send = buffer[i];

    if (_dataOrder == SPI_BITORDER_LSBFIRST) {
      // LSB is rare, if it happens we'll just flip the bits around for them
      uint8_t temp = 0;
      for (uint8_t b=0; b<8; b++) {
	temp |= ((send >> b) & 0x1) << (7-b);
      }
      send = temp;
    }
    for (int b=7; b>=0; b--) {
      reply <<= 1;
      if (_dataMode == SPI_MODE0) {
	digitalWrite(_sck, LOW);
	digitalWrite(_mosi, send & (1<<b));
	digitalWrite(_sck, HIGH);
	if ((_miso != -1) && digitalRead(_miso)) {
	  reply |= 1;
	}
      }
      if (_dataMode == SPI_MODE1) {
	digitalWrite(_sck, HIGH);
	digitalWrite(_mosi, send & (1<<b));
	digitalWrite(_sck, LOW);
	if ((_miso != -1) && digitalRead(_miso)) {
	  reply |= 1;
	}
      }
    }

    if (_dataOrder == SPI_BITORDER_LSBFIRST) {
      // LSB is rare, if it happens we'll just flip the bits around for them
      uint8_t temp = 0;
      for (uint8_t b=0; b<8; b++) {
	temp |= ((reply >> b) & 0x1) << (7-b);
      }
      reply = temp;
    }

    buffer[i] = reply;
  }
  return;
}



/*!
 *    @brief  Transfer (send/receive) one byte over hard/soft SPI
 *    @param  send The byte to send
 *    @return The byte received while transmitting
 */
uint8_t Adafruit_SPIDevice::transfer(uint8_t send) {
  uint8_t data = send;
  transfer(&data, 1);
  return data;
}


/*!
 *    @brief  Write a buffer or two to the SPI device.
 *    @param  buffer Pointer to buffer of data to write
 *    @param  len Number of bytes from buffer to write
 *    @param  prefix_buffer Pointer to optional array of data to write before buffer.
 *    @param  prefix_len Number of bytes from prefix buffer to write
 *    @return Always returns true because there's no way to test success of SPI writes
 */
bool Adafruit_SPIDevice::write(uint8_t *buffer, size_t len, uint8_t *prefix_buffer, size_t prefix_len) {
  if (_spi) {
    _spi->beginTransaction(*_spiSetting);
  }

  digitalWrite(_cs, LOW);
  // do the writing
  for (size_t i=0; i<prefix_len; i++) {
    transfer(prefix_buffer[i]);
  }
  for (size_t i=0; i<len; i++) {
    transfer(buffer[i]);
  }
  digitalWrite(_cs, HIGH);

  if (_spi) {
    _spi->endTransaction();
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tSPIDevice Wrote: "));
  if ((prefix_len != 0) && (prefix_buffer != NULL)) {
    for (uint16_t i=0; i<prefix_len; i++) {
      DEBUG_SERIAL.print(F("0x")); 
      DEBUG_SERIAL.print(prefix_buffer[i], HEX); 
      DEBUG_SERIAL.print(F(", "));
    }
  }
  for (uint16_t i=0; i<len; i++) {
    DEBUG_SERIAL.print(F("0x")); 
    DEBUG_SERIAL.print(buffer[i], HEX); 
    DEBUG_SERIAL.print(F(", "));
    if (len % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }
  DEBUG_SERIAL.println();
#endif

  return true;
}

/*!
 *    @brief  Read from SPI into a buffer from the SPI device.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  sendvalue The 8-bits of data to write when doing the data read, defaults to 0xFF
 *    @return Always returns true because there's no way to test success of SPI writes
 */
bool Adafruit_SPIDevice::read(uint8_t *buffer, size_t len, uint8_t sendvalue) {
  memset(buffer, sendvalue, len);  // clear out existing buffer
  if (_spi) {
    _spi->beginTransaction(*_spiSetting);
  }
  digitalWrite(_cs, LOW);
  transfer(buffer, len); 
  digitalWrite(_cs, HIGH);

  if (_spi) {
    _spi->endTransaction();
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tSPIDevice Read: "));
  for (uint16_t i=0; i<len; i++) {
    DEBUG_SERIAL.print(F("0x")); 
    DEBUG_SERIAL.print(buffer[i], HEX); 
    DEBUG_SERIAL.print(F(", "));
    if (len % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }
  DEBUG_SERIAL.println();
#endif

  return true;
}


/*!
 *    @brief  Write some data, then read some data from SPI into another buffer. The buffers can point to same/overlapping locations. This does not transmit-receive at the same time!
 *    @param  write_buffer Pointer to buffer of data to write from
 *    @param  write_len Number of bytes from buffer to write.
 *    @param  read_buffer Pointer to buffer of data to read into.
 *    @param  read_len Number of bytes from buffer to read.
 *    @param  sendvalue The 8-bits of data to write when doing the data read, defaults to 0xFF
 *    @return Always returns true because there's no way to test success of SPI writes
 */
bool Adafruit_SPIDevice::write_then_read(uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len, uint8_t sendvalue) {
  if (_spi) {
    _spi->beginTransaction(*_spiSetting);
  }

  digitalWrite(_cs, LOW);
  // do the writing
  for (size_t i=0; i<write_len; i++) {
    transfer(write_buffer[i]);
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tSPIDevice Wrote: "));
  for (uint16_t i=0; i<write_len; i++) {
    DEBUG_SERIAL.print(F("0x")); 
    DEBUG_SERIAL.print(write_buffer[i], HEX); 
    DEBUG_SERIAL.print(F(", "));
    if (write_len % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }
  DEBUG_SERIAL.println();
#endif

  // do the reading
  for (size_t i=0; i<read_len; i++) {
    read_buffer[i] = transfer(sendvalue);
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tSPIDevice Read: "));
  for (uint16_t i=0; i<read_len; i++) {
    DEBUG_SERIAL.print(F("0x")); 
    DEBUG_SERIAL.print(read_buffer[i], HEX); 
    DEBUG_SERIAL.print(F(", "));
    if (read_len % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }
  DEBUG_SERIAL.println();
#endif

  digitalWrite(_cs, HIGH);

  if (_spi) {
    _spi->endTransaction();
  }

  return true;
}
