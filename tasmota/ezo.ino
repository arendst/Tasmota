/*
  ezo.ino - EZO modules base class

  Copyright (C) 2020  Christopher Tremblay

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
#if defined(USE_EZOPH) || defined(USE_EZOORP) || defined(USE_EZORTD)

#define D_EZO_DELAY   300   // Minimum delay for any instruction
#define D_EZO_MAX_BUF 40    // Maximum response



// This baseclass provides common functionality for all EZO devices
struct EZOStruct {
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

  void HandleCommand(uint32_t index) const
  {
    char *at      = XdrvMailbox.data;
    uint32_t len  = XdrvMailbox.data_len;

    // Figure out if we're trying to address a specific device
    // PS: This should ideally be done through the Tasmota mailbox
    if (at[0] == '-') {
      uint32_t idx = atoi(&at[1]) - 1;
      at = strchr(at, ' ');

      if (!at++) {
        return;
      }

      len -= (at - XdrvMailbox.data);

      if (idx != index) {
        return;
      }
    }

    // Transmit our command verbatim
    Wire.beginTransmission(addr);
    Wire.write(at, len);
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

public:
  uint8_t   valid     = 0;
  uint8_t   addr; 

private:
  uint32_t  lastRead  = -2000;
};



#endif  // USE_EZO*
#endif  // USE_I2C
