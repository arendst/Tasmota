#include <Wire.h>

#ifndef Adafruit_I2CDevice_h
#define Adafruit_I2CDevice_h

///< The class which defines how we will talk to this device over I2C
class Adafruit_I2CDevice {
 public:
  Adafruit_I2CDevice(uint8_t addr, TwoWire *theWire=&Wire);
  uint8_t address(void);
  bool begin(bool addr_detect=true);
  bool detected(void);

  bool read(uint8_t *buffer, size_t len, bool stop=true);
  bool write(uint8_t *buffer, size_t len, bool stop=true, uint8_t *prefix_buffer=NULL, size_t prefix_len=0);
  bool write_then_read(uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len, bool stop=false);

  /*!   @brief  How many bytes we can read in a transaction
   *    @return The size of the Wire receive/transmit buffer */  
  uint16_t maxBufferSize() { return _maxBufferSize; }

 private:
  uint8_t _addr;
  TwoWire *_wire;
  bool _begun;
  uint16_t _maxBufferSize;
};

#endif // Adafruit_I2CDevice_h
