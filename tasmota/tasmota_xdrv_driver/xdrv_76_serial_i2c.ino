/*
  xdrv_76_serial_i2c.ino - UART to I2C using SC18IM704 compatible protocol

  Copyright (C) 2024  Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_I2C_SERIAL

#define XDRV_76           76

class TwoWireSerial;

struct {
  bool active = false;
  uint8_t bus = 0;
  uint8_t tx = 0;
  uint8_t rx = 0;
  TwoWireSerial *WireSerial = nullptr;      // replacement object for TwoWire
} i2c_serial;

class TwoWireSerial : public TwoWire {
protected:
  uint8_t tx;
  uint8_t rx;
  TasmotaSerial *serial;                    // serial instance to communicate with SC18IM704
private:
public:
  TwoWireSerial(uint8_t bus_num) : tx(-1), rx(-1), serial(nullptr), TwoWire(bus_num) {
    AddLog(LOG_LEVEL_DEBUG_MORE, "ISR: TwoWireSerial(%i)", bus_num);
  };
  // ~TwoWireSerial() {};

  bool setPins(int _tx, int _rx) {
    if (_tx >= 0 && _rx >= 0) {
      tx = _tx;
      rx = _rx;
      return true;
    }
    return false;
  }


  // virtual bool begin() 
  // virtual bool begin(uint8_t address) override{ Serial.printf(">>>>>>> begin\n"); return true; };
  // virtual bool end() = 0;
  bool beginSerial() {
    AddLog(LOG_LEVEL_DEBUG_MORE, "ISR: beginSerial");
    if (tx >= 0 && rx >= 0) {
      serial = new TasmotaSerial(tx, rx);
      if (serial) {
        serial->begin(115200);
        return true;
      }
    }
    return false;
  }

  virtual bool setClock(uint32_t freq) override {
    AddLog(LOG_LEVEL_DEBUG_MORE, "ISR: setClock(%i) -- ignored", freq);
    return true;
  }

  virtual void beginTransmission(uint8_t address) override {
    AddLog(LOG_LEVEL_DEBUG_MORE, "ISR: beginTransmission(0x%02X)", address);
  };
  virtual uint8_t endTransmission(bool stopBit) override {
    AddLog(LOG_LEVEL_DEBUG_MORE, "ISR: endTransmission(%i)", stopBit);
    return 2;
  }
  virtual uint8_t endTransmission(void) override {
    AddLog(LOG_LEVEL_DEBUG_MORE, "ISR: endTransmission()");
    return 2;
  }

  // not used, but redefine to avoid any accidental call
  virtual size_t requestFrom(uint8_t address, size_t len, bool stopBit) override { return 0; };
  virtual size_t requestFrom(uint8_t address, size_t len) override { return 0; };
};

// return the original Wire object or the I2C Serial object
TwoWire & I2CSerialGetWire(TwoWire & orig_wire, uint8_t bus) {
  if (i2c_serial.active && i2c_serial.bus == bus) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("I2C: Bus%d %p"), bus +1, i2c_serial.WireSerial);
    return *i2c_serial.WireSerial;
  } else {
    return orig_wire;
  }
}

// Initialize I2C Serial
// - check if I2C serial is configured on some GPIOs
// - configure serial bus
// - register serial bus with Tasmota
void I2CSerialInit(void) {
  i2c_serial.active = false;
  // check if I2C serial is configured on some GPIOs
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    if (PinUsed(GPIO_I2C_SER_TX, bus) && PinUsed(GPIO_I2C_SER_RX, bus)) {
      if (i2c_serial.active) {
        // Error: I2C Serial was already configured on bus 0, we don't accept a second one
        AddLog(LOG_LEVEL_ERROR, "I2C: I2C serial can be configured only on 1 bus");
        continue;
      }
      if (TasmotaGlobal.i2c_enabled[bus]) {
        // Error: I2C was already configured with SDA/SCL on this bus
        AddLog(LOG_LEVEL_ERROR, "I2C: I2C serial failed on bus %i because SDA/SCL already configured", bus + 1);
      } else {
        // all good
        i2c_serial.bus = bus;
        i2c_serial.tx = Pin(GPIO_I2C_SER_TX, bus);
        i2c_serial.rx = Pin(GPIO_I2C_SER_RX, bus);
        i2c_serial.active = true;
      }
    }
  }
  // configure serial bus
  if (i2c_serial.active) {
    i2c_serial.WireSerial = new TwoWireSerial(1);   // TODO is it ok to use UART 1 ?
    i2c_serial.WireSerial->setPins(i2c_serial.tx, i2c_serial.rx);
    if (i2c_serial.WireSerial->beginSerial()) {
      TasmotaGlobal.i2c_enabled[i2c_serial.bus] = true;          // enable at Tasmota level
      AddLog(LOG_LEVEL_INFO, "I2C: I2C serial configured on GPIO TX %i / RX %i for bus %i", i2c_serial.tx, i2c_serial.rx, i2c_serial.bus + 1);
    } else {
      delete i2c_serial.WireSerial;
      i2c_serial.active = false;
    }
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, "I2C: I2C serial active %i, bus %i, tx %i / rx %i, wire %p", i2c_serial.active, i2c_serial.bus + 1, i2c_serial.tx, i2c_serial.rx, i2c_serial.WireSerial);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv76(uint32_t function) {
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    I2CSerialInit();
  } else if (i2c_serial.active) {
    switch (function) {
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_I2C_SERIAL
