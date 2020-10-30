#include <Adafruit_BusIO_Register.h>

/*!
 *    @brief  Create a register we access over an I2C Device (which defines the bus and address)
 *    @param  i2cdevice The I2CDevice to use for underlying I2C access
 *    @param  reg_addr The address pointer value for the I2C/SMBus register, can be 8 or 16 bits
 *    @param  width    The width of the register data itself, defaults to 1 byte
 *    @param  bitorder The bit order of the register (used when width is > 1), defaults to LSBFIRST
 *    @param  address_width The width of the register address itself, defaults to 1 byte
 */
Adafruit_BusIO_Register::Adafruit_BusIO_Register(Adafruit_I2CDevice *i2cdevice, uint16_t reg_addr, 
						 uint8_t width, uint8_t bitorder, uint8_t address_width) {
  _i2cdevice = i2cdevice;
  _spidevice = NULL;
  _addrwidth = address_width;
  _address = reg_addr;
  _bitorder = bitorder;
  _width = width;
}

/*!
 *    @brief  Create a register we access over an SPI Device (which defines the bus and CS pin)
 *    @param  spidevice The SPIDevice to use for underlying I2C access
 *    @param  reg_addr The address pointer value for the I2C/SMBus register, can be 8 or 16 bits
 *    @param  type     The method we use to read/write data to SPI (which is not as well defined as I2C)
 *    @param  width    The width of the register data itself, defaults to 1 byte
 *    @param  bitorder The bit order of the register (used when width is > 1), defaults to LSBFIRST
 *    @param  address_width The width of the register address itself, defaults to 1 byte
 */
Adafruit_BusIO_Register::Adafruit_BusIO_Register(Adafruit_SPIDevice *spidevice, uint16_t reg_addr, 
						 Adafruit_BusIO_SPIRegType type,
						 uint8_t width, uint8_t bitorder, uint8_t address_width) {
  _spidevice = spidevice;
  _spiregtype = type;
  _i2cdevice = NULL;
  _addrwidth = address_width;
  _address = reg_addr;
  _bitorder = bitorder;
  _width = width;
}

/*!
 *    @brief  Create a register we access over an I2C or SPI Device. This is a handy function because we
 *            can pass in NULL for the unused interface, allowing libraries to mass-define all the registers
 *    @param  i2cdevice The I2CDevice to use for underlying I2C access, if NULL we use SPI
 *    @param  spidevice The SPIDevice to use for underlying I2C access, if NULL we use I2C
 *    @param  reg_addr The address pointer value for the I2C/SMBus register, can be 8 or 16 bits
 *    @param  type     The method we use to read/write data to SPI (which is not as well defined as I2C)
 *    @param  width    The width of the register data itself, defaults to 1 byte
 *    @param  bitorder The bit order of the register (used when width is > 1), defaults to LSBFIRST
 *    @param  address_width The width of the register address itself, defaults to 1 byte
 */
Adafruit_BusIO_Register::Adafruit_BusIO_Register(Adafruit_I2CDevice *i2cdevice, Adafruit_SPIDevice *spidevice, 
						  Adafruit_BusIO_SPIRegType type, uint16_t reg_addr,
						 uint8_t width, uint8_t bitorder, uint8_t address_width) {
  _spidevice = spidevice;
  _i2cdevice = i2cdevice;
  _spiregtype = type;
  _addrwidth = address_width;
  _address = reg_addr;
  _bitorder = bitorder;
  _width = width;
}


/*!
 *    @brief  Write a buffer of data to the register location
 *    @param  buffer Pointer to data to write
 *    @param  len Number of bytes to write
 *    @return True on successful write (only really useful for I2C as SPI is uncheckable)
 */
bool Adafruit_BusIO_Register::write(uint8_t *buffer, uint8_t len) {

  uint8_t addrbuffer[2] = {(uint8_t)(_address & 0xFF), (uint8_t)(_address>>8)};

  if (_i2cdevice) {
    return _i2cdevice->write(buffer, len, true, addrbuffer, _addrwidth);
  }
  if (_spidevice) {
    if (_spiregtype == ADDRBIT8_HIGH_TOREAD) {
      addrbuffer[0] &= ~0x80;
    }
    return _spidevice->write( buffer, len, addrbuffer, _addrwidth);
  }
  return false;
}

/*!
 *    @brief  Write up to 4 bytes of data to the register location
 *    @param  value Data to write
 *    @param  numbytes How many bytes from 'value' to write
 *    @return True on successful write (only really useful for I2C as SPI is uncheckable)
 */
bool Adafruit_BusIO_Register::write(uint32_t value, uint8_t numbytes) {
  if (numbytes == 0) {
    numbytes = _width;
  }
  if (numbytes > 4) {
    return false;
  }

  for (int i=0; i<numbytes; i++) {
    if (_bitorder == LSBFIRST) {
      _buffer[i] = value & 0xFF;
    } else {
      _buffer[numbytes-i-1] = value & 0xFF;
    }
    value >>= 8;
  }
  return write(_buffer, numbytes);
}

/*!
 *    @brief  Read data from the register location. This does not do any error checking! 
 *    @return Returns 0xFFFFFFFF on failure, value otherwise
 */
uint32_t Adafruit_BusIO_Register::read(void) {
  if (! read(_buffer, _width)) {
    return -1;
  }

  uint32_t value = 0;

   for (int i=0; i < _width; i++) {
     value <<= 8;
     if (_bitorder == LSBFIRST) {
       value |= _buffer[_width-i-1];
     } else {
       value |= _buffer[i];
     }
   }

   return value;
}


/*!
 *    @brief  Read a buffer of data from the register location
 *    @param  buffer Pointer to data to read into
 *    @param  len Number of bytes to read
 *    @return True on successful write (only really useful for I2C as SPI is uncheckable)
 */
bool Adafruit_BusIO_Register::read(uint8_t *buffer, uint8_t len) {
  uint8_t addrbuffer[2] = {(uint8_t)(_address & 0xFF), (uint8_t)(_address>>8)};

  if (_i2cdevice) {
    return _i2cdevice->write_then_read(addrbuffer, _addrwidth, buffer, len);
  }
  if (_spidevice) {
    if (_spiregtype == ADDRBIT8_HIGH_TOREAD) {
      addrbuffer[0] |= 0x80;
    }
    return _spidevice->write_then_read(addrbuffer, _addrwidth, buffer, len);
  }
  return false;
}

/*!
 *    @brief  Read 2 bytes of data from the register location
 *    @param  value Pointer to uint16_t variable to read into
 *    @return True on successful write (only really useful for I2C as SPI is uncheckable)
 */
bool Adafruit_BusIO_Register::read(uint16_t *value) {
  if (! read(_buffer, 2)) {
    return false;
  }

  if (_bitorder == LSBFIRST) {
    *value = _buffer[1];
    *value <<= 8;
    *value |= _buffer[0];
  } else {
    *value = _buffer[0];
    *value <<= 8;
    *value |= _buffer[1];
  }
  return true;
}

/*!
 *    @brief  Read 1 byte of data from the register location
 *    @param  value Pointer to uint8_t variable to read into
 *    @return True on successful write (only really useful for I2C as SPI is uncheckable)
 */
bool Adafruit_BusIO_Register::read(uint8_t *value) {
  if (! read(_buffer, 1)) {
    return false;
  }

  *value = _buffer[0];
  return true;
}

/*!
 *    @brief  Pretty printer for this register
 *    @param  s The Stream to print to, defaults to &Serial
 */
void Adafruit_BusIO_Register::print(Stream *s) {
  uint32_t val = read();
  s->print("0x"); s->print(val, HEX);
}

/*!
 *    @brief  Pretty printer for this register
 *    @param  s The Stream to print to, defaults to &Serial
 */
void Adafruit_BusIO_Register::println(Stream *s) {
  print(s);
  s->println();
}


/*!
 *    @brief  Create a slice of the register that we can address without touching other bits
 *    @param  reg The Adafruit_BusIO_Register which defines the bus/register
 *    @param  bits The number of bits wide we are slicing
 *    @param  shift The number of bits that our bit-slice is shifted from LSB
 */
Adafruit_BusIO_RegisterBits::Adafruit_BusIO_RegisterBits(Adafruit_BusIO_Register *reg, uint8_t bits, uint8_t shift) {
  _register = reg;
  _bits = bits;
  _shift = shift;
}

/*!
 *    @brief  Read 4 bytes of data from the register
 *    @return  data The 4 bytes to read
 */
uint32_t Adafruit_BusIO_RegisterBits::read(void) {
  uint32_t val = _register->read();
  val >>= _shift;
  return val & ((1 << (_bits)) - 1);
}


/*!
 *    @brief  Write 4 bytes of data to the register
 *    @param  data The 4 bytes to write
 */
void Adafruit_BusIO_RegisterBits::write(uint32_t data) {
  uint32_t val = _register->read();

  // mask off the data before writing
  uint32_t mask = (1 << (_bits)) - 1;
  data &= mask;

  mask <<= _shift;
  val &= ~mask;      // remove the current data at that spot
  val |= data << _shift; // and add in the new data
  
  _register->write(val, _register->width());
}

/*!
 *    @brief  The width of the register data, helpful for doing calculations
 *    @returns The data width used when initializing the register
 */
uint8_t Adafruit_BusIO_Register::width(void) { return _width; }
