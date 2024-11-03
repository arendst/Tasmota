/*
  support_a_i2c.ino - I2C support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
/*********************************************************************************************\
 * Basic I2C routines supporting two busses
\*********************************************************************************************/

#ifdef ESP8266
#ifdef USE_I2C_BUS2
#define USE_I2C_BUS2_ESP8266
#endif  // USE_I2C_BUS2_ESP8266
#endif  // ESP8266

const uint8_t I2C_RETRY_COUNTER = 3;

struct I2Ct {
  uint32_t buffer;
  uint32_t frequency[2];
#ifdef USE_I2C_BUS2
  uint32_t active[2][4];
#else
  uint32_t active[1][4];
#endif  // No USE_I2C_BUS2
#ifdef USE_I2C_BUS2_ESP8266
  int8_t sda[2];
  int8_t scl[2];
  int8_t active_bus = -1;
#endif  // USE_I2C_BUS2_ESP8266
} I2C;

#ifdef USE_I2C_BUS2_ESP8266
TwoWire Wire1 = Wire;                                     // Not really backward compatible with ESP32

void I2cSetBus(uint32_t bus = 0);
void I2cSetBus(uint32_t bus) {
  if (I2C.active_bus != bus) {
    I2C.active_bus = bus;
    Wire.begin(I2C.sda[bus], I2C.scl[bus]);
    Wire.setClock(I2C.frequency[bus]);
  }
}
#endif  // USE_I2C_BUS2_ESP8266

bool I2cBegin(int sda, int scl, uint32_t bus = 0, uint32_t frequency = 100000);
bool I2cBegin(int sda, int scl, uint32_t bus, uint32_t frequency) {
  I2C.frequency[bus] = frequency;
  bool result = true;
#ifdef ESP8266
#ifdef USE_I2C_BUS2_ESP8266
  I2C.sda[bus] = sda;
  I2C.scl[bus] = scl;
  I2cSetBus();
#else
  if (bus > 0) { return false; }
  Wire.begin(sda, scl);
  Wire.setClock(frequency);
#endif  // USE_I2C_BUS2_ESP8266
#endif  // ESP8266
#ifdef ESP32
#ifdef USE_I2C_BUS2
  TwoWire& myWire = (0 == bus) ? Wire : Wire1;
#else
  if (bus > 0) { return false; }
  TwoWire& myWire = Wire;
#endif
  static bool reinit = false;
  if (reinit) { myWire.end(); }
  result = myWire.begin(sda, scl, frequency);
  reinit = result;
#endif  // ESP32
//  AddLog(LOG_LEVEL_DEBUG, PSTR("I2C: Bus%d %d"), bus +1, result);
  return result;
}

TwoWire * I2CSerialGetWire(TwoWire * orig_wire, uint8_t bus);

TwoWire& I2cGetWire(uint8_t bus = 0) {
  if ((0 == bus) && TasmotaGlobal.i2c_enabled[0]) {
#ifdef USE_I2C_BUS2_ESP8266
    I2cSetBus(bus);
#endif
#ifdef USE_I2C_SERIAL
    return I2CSerialGetWire(Wire, bus);
#else
    return Wire;
#endif // USE_I2C_SERIAL
  }
#ifdef USE_I2C_BUS2
  else if ((1 == bus) && TasmotaGlobal.i2c_enabled[1]) {
#ifdef USE_I2C_BUS2_ESP8266
    I2cSetBus(bus);
#endif
#ifdef USE_I2C_SERIAL
    return I2CSerialGetWire(Wire1, bus);
#else
    return Wire1;
#endif // USE_I2C_SERIAL
  }
#endif  // USE_I2C_BUS2
  else {
//    AddLog(LOG_LEVEL_ERROR, PSTR("I2C: bus%d not initialized"), bus +1);
    return *(TwoWire*)nullptr;
  }
}

bool I2cSetClock(uint32_t frequency = 0, uint32_t bus = 0);
bool I2cSetClock(uint32_t frequency, uint32_t bus) {
  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return false; }               // No valid I2c bus

  if (0 == frequency) {
    if (0 == I2C.frequency[bus]) {
      I2C.frequency[bus] = 100000;                        // Tasmota default I2C bus speed
    }
  } else {
    I2C.frequency[bus] = frequency;
  }
  if (frequency != I2C.frequency[bus]) {
    myWire.setClock(I2C.frequency[bus]);
  }
  return true;
}

/*-------------------------------------------------------------------------------------------*\
 * Return code: 0 = Error, 1 = OK
\*-------------------------------------------------------------------------------------------*/

bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size, uint8_t bus = 0, bool sendStop = false) {
  I2C.buffer = 0;

  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return false; }               // No valid I2c bus

  uint8_t retry = I2C_RETRY_COUNTER;
  bool status = false;
  while (!status && retry) {
    myWire.beginTransmission(addr);                       // start transmission to device
    myWire.write(reg);                                    // sends register address to read from
    if (0 == myWire.endTransmission(sendStop)) {          // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      myWire.requestFrom((int)addr, (int)size);           // send data n-bytes read
      if (myWire.available() == size) {
        for (uint32_t i = 0; i < size; i++) {
          I2C.buffer = I2C.buffer << 8 | myWire.read();   // receive DATA
        }
        status = true;                                    // 1 = OK
      }
    }
    retry--;
  }
  if (!retry) myWire.endTransmission();
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  return status;                                          // 0 = Error, 1 = OK
}

bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  bool status = I2cValidRead(addr, reg, 1, bus);
  *data = (uint8_t)I2C.buffer;
  return status;                                          // 0 = Error, 1 = OK
}

bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  bool status = I2cValidRead(addr, reg, 2, bus);
  *data = (uint16_t)I2C.buffer;
  return status;                                          // 0 = Error, 1 = OK
}

bool I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  bool status = I2cValidRead(addr, reg, 2, bus);
  *data = (int16_t)I2C.buffer;
  return status;                                          // 0 = Error, 1 = OK
}

bool I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  uint16_t ldata;
  bool status = I2cValidRead16(&ldata, addr, reg, bus);
  *data = (ldata >> 8) | (ldata << 8);
  return status;                                          // 0 = Error, 1 = OK
}

bool I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  uint16_t ldata;
  bool status = I2cValidRead16LE(&ldata, addr, reg, bus);
  *data = (int16_t)ldata;
  return status;                                          // 0 = Error, 1 = OK
}

bool I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  bool status = I2cValidRead(addr, reg, 3, bus);
  *data = I2C.buffer;
  return status;                                          // 0 = Error, 1 = OK
}

uint8_t I2cRead8(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  I2cValidRead(addr, reg, 1, bus);
  return (uint8_t)I2C.buffer;
}

uint16_t I2cRead16(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  I2cValidRead(addr, reg, 2, bus);
  return (uint16_t)I2C.buffer;
}

int16_t I2cReadS16(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  I2cValidRead(addr, reg, 2, bus);
  return (int16_t)I2C.buffer;
}

uint16_t I2cRead16LE(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  I2cValidRead(addr, reg, 2, bus);
  uint16_t temp = (uint16_t)I2C.buffer;
  return (temp >> 8) | (temp << 8);
}

int16_t I2cReadS16_LE(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  return (int16_t)I2cRead16LE(addr, reg, bus);
}

int32_t I2cRead24(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
  I2cValidRead(addr, reg, 3, bus);
  return I2C.buffer;
}

/*-------------------------------------------------------------------------------------------*/

bool I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size, uint8_t bus = 0) {
  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return false; }               // No valid I2c bus

  uint8_t x = I2C_RETRY_COUNTER;
  do {
    myWire.beginTransmission((uint8_t)addr);              // start transmission to device
    myWire.write(reg);                                    // sends register address to write to
    uint8_t bytes = size;
    while (bytes--) {
      myWire.write((val >> (8 * bytes)) & 0xFF);          // write data
    }
    x--;
  } while (myWire.endTransmission(true) != 0 && x != 0);  // end transmission
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  return (x);                                             // 0 = Error, 1 = OK
}

bool I2cWrite0(uint8_t addr, uint8_t reg, uint8_t bus = 0) {
   return I2cWrite(addr, reg, 0, 0, bus);                 // 0 = Error, 1 = OK
}

bool I2cWrite8(uint8_t addr, uint8_t reg, uint32_t val, uint8_t bus = 0) {
   return I2cWrite(addr, reg, val, 1, bus);               // 0 = Error, 1 = OK
}

bool I2cWrite16(uint8_t addr, uint8_t reg, uint32_t val, uint8_t bus = 0) {
   return I2cWrite(addr, reg, val, 2, bus);               // 0 = Error, 1 = OK
}

/*-------------------------------------------------------------------------------------------*\
 * Return code: 0 = OK, 1 = Error
\*-------------------------------------------------------------------------------------------*/

bool I2cReadBuffer0(uint8_t addr, uint8_t *reg_data, uint16_t len, uint8_t bus = 0) {
  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return true; }                // No valid I2c bus

  myWire.requestFrom((uint8_t)addr, (uint8_t)len);
  if (myWire.available() != len) {
#ifdef USE_I2C_BUS2_ESP8266
    I2cSetBus();
#endif
    return true;                                          // 1 = Error
  }
  while (len--) {
    *reg_data = (uint8_t)myWire.read();
    reg_data++;
  }
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  return false;                                           // 0 = OK
}

bool I2cReadBuffer(uint8_t addr, int reg, uint8_t *reg_data, uint16_t len, uint8_t bus = 0) {
  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return true; }                // No valid I2c bus

  myWire.beginTransmission((uint8_t)addr);
  if (reg > -1) { 
    myWire.write((uint8_t)reg);
    if (reg > 255) {
      myWire.write((uint8_t)(reg >> 8));
    }
    myWire.endTransmission();
  }
  if (len != myWire.requestFrom((uint8_t)addr, (uint8_t)len)) {
#ifdef USE_I2C_BUS2_ESP8266
    I2cSetBus();
#endif
    return true;                                          // 1 = Error
  }
  while (len--) {
    *reg_data = (uint8_t)myWire.read();
    reg_data++;
  }
  if (reg < 0) { 
    myWire.endTransmission();
  }
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  return false;                                           // 0 = OK
}

bool I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len, uint8_t bus = 0) {
  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return true; }                // 1 = Error, No valid I2c bus

  myWire.beginTransmission((uint8_t)addr);
  myWire.write((uint8_t)reg);
  while (len--) {
    myWire.write(*reg_data);
    reg_data++;
  }
  myWire.endTransmission();
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  return false;                                           // 0 = OK
}

/*-------------------------------------------------------------------------------------------*/

void I2cScan(uint8_t bus = 0) {
  // Return error codes defined in twi.h and core_esp8266_si2c.c
  // I2C_OK                      0
  // I2C_SCL_HELD_LOW            1 = SCL held low by another device, no procedure available to recover
  // I2C_SCL_HELD_LOW_AFTER_READ 2 = I2C bus error. SCL held low beyond client clock stretch time
  // I2C_SDA_HELD_LOW            3 = I2C bus error. SDA line held low by client/another_master after n bits
  // I2C_SDA_HELD_LOW_AFTER_INIT 4 = line busy. SDA again held low by another device. 2nd master?
  //                             5 = bus busy. Timeout
  // https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
  // 0: success
  // 1: data too long to fit in transmit buffer
  // 2: received NACK on transmit of address
  // 3: received NACK on transmit of data
  // 4: other error
  // 5: timeout
  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return; }  // No valid I2c bus
  Response_P(PSTR("{\"" D_CMND_I2CSCAN "\":\"Device(s) found "));
#ifdef USE_I2C_BUS2
  ResponseAppend_P(PSTR("on bus%d "), bus +1);
#endif
  ResponseAppend_P(PSTR("at"));

  uint8_t error = 0;
  uint8_t address = 0;
  uint8_t any = 0;
  for (address = 1; address <= 127; address++) {
    myWire.beginTransmission(address);
    error = myWire.endTransmission();
    if (0 == error) {
      any = 1;
      ResponseAppend_P(PSTR(" 0x%02x"), address);
    }
    else if (error != 2) {  // Seems to happen anyway using this scan
      any = 2;
      Response_P(PSTR("{\"" D_CMND_I2CSCAN "\":\"Error %d at 0x%02x"), error, address);
#ifdef USE_I2C_BUS2
      ResponseAppend_P(PSTR(" (bus%d)"), bus +1);
#endif  // USE_I2C_BUS2
      break;
    }
  }
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  if (any) {
    ResponseAppend_P(PSTR("\"}"));
  } else {
    Response_P(PSTR("{\"" D_CMND_I2CSCAN "\":\"No devices found\"}"));
  }
}

void I2cResetActive(uint32_t addr, uint8_t bus = 0) {
  addr &= 0x7F;         // Max I2C address is 127
  I2C.active[bus][addr / 32] &= ~(1 << (addr % 32));

//  AddLog(LOG_LEVEL_DEBUG, PSTR("I2C: I2cResetActive bus0 %08X-%08X-%08X-%08X, bus1 %08X-%08X-%08X-%08X"),
//    I2C.active[0][0], I2C.active[0][1], I2C.active[0][2], I2C.active[0][3],
//    I2C.active[1][0], I2C.active[1][1], I2C.active[1][2], I2C.active[1][3]);
}

void I2cSetActive(uint32_t addr, uint8_t bus = 0) {
  addr &= 0x7F;         // Max I2C address is 127
  I2C.active[bus][addr / 32] |= (1 << (addr % 32));

//  AddLog(LOG_LEVEL_DEBUG, PSTR("I2C: I2cSetActive addr %02X, bus%d, bus0 %08X-%08X-%08X-%08X, bus1 %08X-%08X-%08X-%08X"),
//    addr, bus,
//    I2C.active[0][0], I2C.active[0][1], I2C.active[0][2], I2C.active[0][3],
//    I2C.active[1][0], I2C.active[1][1], I2C.active[1][2], I2C.active[1][3]);
}

void I2cSetActiveFound(uint32_t addr, const char *types, uint8_t bus = 0) {
  I2cSetActive(addr, bus);
  AddLog(LOG_LEVEL_INFO, PSTR("I2C: %s found at 0x%02x%s"), types, addr, (bus)?" (bus2)":"");
}

bool I2cActive(uint32_t addr, uint8_t bus = 0) {
  addr &= 0x7F;         // Max I2C address is 127
  return (I2C.active[bus][addr / 32] & (1 << (addr % 32)));
}

bool I2cSetDevice(uint32_t addr, uint8_t bus = 0) {
  addr &= 0x7F;         // Max I2C address is 127
  if (I2cActive(addr, bus)) {
    return false;       // If already active report as not present;
  }

  TwoWire& myWire = I2cGetWire(bus);
  if (&myWire == nullptr) { return false; }  // No valid I2c bus
  myWire.beginTransmission((uint8_t)addr);
//  return (0 == myWire.endTransmission());
  uint32_t err = myWire.endTransmission();
#ifdef USE_I2C_BUS2_ESP8266
  I2cSetBus();
#endif
  if (err && (err != 2)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("I2C: Error %d at 0x%02x%s"), err, addr, (bus)?" (bus2)":"");
  }
  return (0 == err);
}
#endif  // USE_I2C