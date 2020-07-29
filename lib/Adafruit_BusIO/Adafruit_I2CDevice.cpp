#include <Adafruit_I2CDevice.h>
#include <Arduino.h>

//#define DEBUG_SERIAL Serial

/*!
 *    @brief  Create an I2C device at a given address
 *    @param  addr The 7-bit I2C address for the device
 *    @param  theWire The I2C bus to use, defaults to &Wire
 */
Adafruit_I2CDevice::Adafruit_I2CDevice(uint8_t addr, TwoWire *theWire) {
  _addr = addr;
  _wire = theWire;
  _begun = false;
#ifdef ARDUINO_ARCH_SAMD
  _maxBufferSize = 250; // as defined in Wire.h's RingBuffer
#else
  _maxBufferSize = 32;
#endif
}

/*!
 *    @brief  Initializes and does basic address detection
 *    @param  addr_detect Whether we should attempt to detect the I2C address with a scan.
 *            99% of sensors/devices don't mind but once in a while, they spaz on a scan!
 *    @return True if I2C initialized and a device with the addr found
 */
bool Adafruit_I2CDevice::begin(bool addr_detect) {
  _wire->begin();
  _begun = true;
  
  if (addr_detect) {
    return detected();
  }
  return true;
}


/*!
 *    @brief  Scans I2C for the address - note will give a false-positive
 *    if there's no pullups on I2C
 *    @return True if I2C initialized and a device with the addr found
 */
bool Adafruit_I2CDevice::detected(void) {
  // Init I2C if not done yet
  if (!_begun && !begin()) {
    return false;
  }

  // A basic scanner, see if it ACK's
  _wire->beginTransmission(_addr);
  if (_wire->endTransmission () == 0) {
    return true;
  }
  return false;
}

/*!
 *    @brief  Write a buffer or two to the I2C device. Cannot be more than maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to write
 *    @param  len Number of bytes from buffer to write
 *    @param  prefix_buffer Pointer to optional array of data to write before buffer. 
 *    Cannot be more than maxBufferSize() bytes.
 *    @param  prefix_len Number of bytes from prefix buffer to write
 *    @param  stop Whether to send an I2C STOP signal on write
 *    @return True if write was successful, otherwise false.
 */
bool Adafruit_I2CDevice::write(uint8_t *buffer, size_t len, bool stop, uint8_t *prefix_buffer, size_t prefix_len) {
  if ((len+prefix_len) > maxBufferSize()) {
    // currently not guaranteed to work if more than 32 bytes!
    // we will need to find out if some platforms have larger
    // I2C buffer sizes :/
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println(F("\tI2CDevice could not write such a large buffer"));
#endif
    return false;
  }

  _wire->beginTransmission(_addr);

  // Write the prefix data (usually an address)
  if ((prefix_len != 0) && (prefix_buffer != NULL)) {
    if (_wire->write(prefix_buffer, prefix_len) != prefix_len) {
#ifdef DEBUG_SERIAL
      DEBUG_SERIAL.println(F("\tI2CDevice failed to write"));
#endif
      return false;
    }
  }

  // Write the data itself
  if (_wire->write(buffer, len) != len) {
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println(F("\tI2CDevice failed to write"));
#endif
    return false;
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tI2CDevice Wrote: "));
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

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print("Stop: "); DEBUG_SERIAL.println(stop);
#endif

  if (_wire->endTransmission(stop) == 0) {
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println("Sent!");
#endif
    return true;
  } else {
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println("Failed to send!");
#endif
    return false;
  }
}


/*!
 *    @brief  Read from I2C into a buffer from the I2C device. 
 *    Cannot be more than maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal on read
 *    @return True if read was successful, otherwise false.
 */
bool Adafruit_I2CDevice::read(uint8_t *buffer, size_t len, bool stop) {
  if (len > maxBufferSize()) {
    // currently not guaranteed to work if more than 32 bytes!
    // we will need to find out if some platforms have larger
    // I2C buffer sizes :/
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println(F("\tI2CDevice could not read such a large buffer"));
#endif
    return false;
  }

  size_t recv = _wire->requestFrom((uint8_t)_addr, (uint8_t)len, (uint8_t)stop);
  if (recv != len) {
    // Not enough data available to fulfill our obligation!
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.print(F("\tI2CDevice did not receive enough data: "));
    DEBUG_SERIAL.println(recv);
#endif
    return false;
  }

  for (uint16_t i=0; i<len; i++) {
    buffer[i] = _wire->read();
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tI2CDevice Read: "));
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
 *    @brief  Write some data, then read some data from I2C into another buffer. 
 *    Cannot be more than maxBufferSize() bytes. The buffers can point to 
 *    same/overlapping locations.
 *    @param  write_buffer Pointer to buffer of data to write from
 *    @param  write_len Number of bytes from buffer to write.
 *    @param  read_buffer Pointer to buffer of data to read into.
 *    @param  read_len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal between the write and read
 *    @return True if write & read was successful, otherwise false.
 */
bool Adafruit_I2CDevice::write_then_read(uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len, bool stop) {
  if (! write(write_buffer, write_len, stop)) {
    return false;
  }
  
  return read(read_buffer, read_len);
}


/*!
 *    @brief  Returns the 7-bit address of this device
 *    @return The 7-bit address of this device
 */
uint8_t Adafruit_I2CDevice::address(void) {
  return _addr;
}
