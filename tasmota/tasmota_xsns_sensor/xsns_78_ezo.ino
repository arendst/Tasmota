/*
  xsns_78_ezo.ino - EZO modules base class

  Copyright (C) 2021  Christopher Tremblay

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
#ifdef USE_I2C

#if defined(USE_EZOPH) || defined(USE_EZOORP) || defined(USE_EZORTD) || defined(USE_EZOHUM) || defined(USE_EZOEC) || defined(USE_EZOCO2) || defined(USE_EZOO2) || defined(USE_EZOPRS) || defined(USE_EZOFLO) || defined(USE_EZODO) || defined(USE_EZORGB) || defined(USE_EZOPMP)
  #define USE_EZO
#endif
#if defined(USE_EZO)

#define D_EZO_DELAY   300   // Minimum delay for any instruction
#define D_EZO_MAX_BUF 52    // Maximum response

const char D_EZO_NAME[] PROGMEM = "EZO";



// This baseclass provides common functionality for all EZO devices
struct EZOStruct {
  EZOStruct(uint32_t addr) : valid(0), addr(addr), lastRead(-2000) {}

  void MeasureRequest(void)
  {
    const uint8_t EZOMeasureCmd[2] = {'R', 0};

    if (valid) {
      valid--;
    }

    Wire.beginTransmission(addr);
    Wire.write(EZOMeasureCmd, sizeof(EZOMeasureCmd));
    Wire.endTransmission();

    lastRead = millis();
  }

  void HandleCommand(char *cmd, uint32_t len) const
  {
    // Transmit our command verbatim
    Wire.beginTransmission(addr);
    Wire.write((uint8_t*)cmd, len);
    if (Wire.endTransmission() != 0) {
      return;
    }

    // Attempt to read the results
    char data[D_EZO_MAX_BUF];
    for (uint32_t code = 254; code == 254; code = Wire.read()) {
      delay(D_EZO_DELAY);
      Wire.requestFrom(addr, sizeof(data));
    }

    for (uint32_t i = 0; Wire.available() > 0; i++) {
      data[i] = Wire.read();
    }

    ResponseCmndChar((char *)data);
  }

  bool isValid(void)
  {
    return valid;
  }

  virtual void ProcessMeasurement(void);
  virtual void Show(bool json, const char *name);

  static const char id[] PROGMEM;

protected:
  void ProcessMeasurement(char *const data, const uint32_t len, const uint32_t latency)
  {
    // Wait for the data to arrive first
    const int32_t dur = lastRead + latency - millis();

    // Wait for the last readout to complete (only when commands are issued)
    if (dur > 0) {
      delay(dur);
    }

    Wire.requestFrom(addr, len);
    const char code = Wire.read();

    if (code == 1) {
      for (uint32_t i = 0; (Wire.available() > 0) && (i < len); i++) {
        data[i] = Wire.read();
      }

      valid = SENSOR_MAX_MISS;
    }
  }

  uint8_t   valid;
  uint8_t   addr;
  uint32_t  lastRead;
};

const char EZOStruct::id[] PROGMEM = "";

#endif  // USE_EZO
#endif  // USE_I2C
