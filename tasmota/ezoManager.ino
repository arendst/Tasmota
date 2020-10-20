/*
  ezoManager.ino - EZO device manager

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

#define XI2C_55     55    // See I2CDEVICES.md

#define EZO_ADDR_0  0x61  // First EZO address
#define EZO_ADDR_n  16    // Number of ports for use with EZO devices


// List of known EZO devices and their default address
enum EZOType {
  EZO_DO  = 0x61, // D.O.
  EZO_ORP = 0x62, // ORP
  EZO_PH  = 0x63, // pH
  EZO_EC  = 0x64, // EC

  EZO_RTD = 0x66, // RTD
  EZO_PMP = 0x67, // PMP
  EZO_FLO = 0x68, // FLO
  EZO_CO2 = 0x69, // CO2
  EZO_PRS = 0x6A, // PRS

  EZO_O2  = 0x6C, // O2


  EZO_HUM = 0x6F, // HUM
  EZO_RGB = 0x70, // RGB
};

const char EZO_EMPTY[]    PROGMEM = "";
//const char EZO_DO_NAME[]  PROGMEM = "DO";
#ifdef USE_EZOORP
const char EZO_ORP_NAME[] PROGMEM = "ORP";
#endif
#ifdef USE_EZOPH
const char EZO_PH_NAME[]  PROGMEM = "pH";
#endif
//const char EZO_EC_NAME[]  PROGMEM = "EC";
#ifdef USE_EZORTD
const char EZO_RTD_NAME[] PROGMEM = "RTD";
#endif
//const char EZO_PMP_NAME[] PROGMEM = "PMP";
//const char EZO_FLO_NAME[] PROGMEM = "FLO";
//const char EZO_CO2_NAME[] PROGMEM = "CO2";
//const char EZO_PRS_NAME[] PROGMEM = "PRS";
//const char EZO_O2_NAME[]  PROGMEM = "O2";
//const char EZO_HUM_NAME[] PROGMEM = "HUM";
//const char EZO_RGB_NAME[] PROGMEM = "RGB";

const char *const EZOSupport[EZO_ADDR_n] PROGMEM = {
  EZO_EMPTY,

#ifdef USE_EZOORP
  EZO_ORP_NAME,
#else
  EZO_EMPTY,
#endif

#ifdef USE_EZOPH
  EZO_PH_NAME,
#else
  EZO_EMPTY,
#endif

  EZO_EMPTY,
  EZO_EMPTY,

#ifdef USE_EZORTD
  EZO_RTD_NAME,
#else
  EZO_EMPTY,
#endif

  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
  EZO_EMPTY,
};



struct EZOManager {
  // Returns the count of devices of the specified type or -1 if the driver isn't ready yet
  // list must be a client-allocated array of atleast 16 elements
  int getDevice(const EZOType type, uint32_t *list)
  {
    // EZO devices take 2s to boot
    if (uptime >= next) {
      if (stage == 0) {
        DetectRequest();
        next = uptime + 1;
      } else if (stage == 1) {
        ProcessDetection();
      }

      stage++;
    }

    if (stage >= 2) {
      int count = 0;

      for (uint32_t i = 0; i < EZO_ADDR_n; i++) {
        if ((alive & (1 << i)) && (((devices[i >> 3] >> ((i & 7) << 2)) & 0xF) == (type - EZO_ADDR_0))) {
          list[count++] = i + EZO_ADDR_0;
        }
      }

      return count;
    }

    return -1;
  }

private:
  void DetectRequest(void)
  {
    const uint8_t EZOInfoCmd[2] = {'i', 0};
    alive = 0;

    // Scan the address range
    uint16_t shift = 1;
    for (uint8_t i = EZO_ADDR_0; shift; i++) {
      if (!I2cActive(i)) {
        // Request the device to identify itself
        Wire.beginTransmission(i);
        Wire.write(EZOInfoCmd, sizeof(EZOInfoCmd));

        int c = Wire.endTransmission();

        if (c == 0) {
          alive |= shift;
        }
      }
      shift <<= 1;
    }
  }

  void ProcessDetection(void)
  {
    uint32_t mask = alive;

    devices[0] = devices[1] = 0;

    // Check every address that we sent a request to
    for (uint8_t addr = 0; addr < EZO_ADDR_n; addr++) {
      if (mask & 1) {
        char data[D_EZO_MAX_BUF];
        Wire.requestFrom(addr + EZO_ADDR_0, sizeof(data));
        char code = Wire.read();

        if (code == 1) {
          uint32_t i;

          for (i = 0; Wire.available() > 0; i++) {
            char c  = Wire.read();

            // Helps us strcmp
            data[i] = (c == ',') ? 0 : c;
          }

          // Technically the response starts with "?I," but we'll skip testing it to save space
          if (i >= 3) {
            for (uint32_t j = 0; j < EZO_ADDR_n; j++) {
              if (strcasecmp_P(&data[3], EZOSupport[j]) == 0) {
                data[0] = 'E';
                data[1] = 'Z';
                data[2] = 'O';
                I2cSetActiveFound(addr + EZO_ADDR_0, data);
                devices[addr >> 3] |= j << ((addr & 7) * 4);
              }
            }
          }
        }
      }

      mask >>= 1;
    }
  }

  uint32_t  next  = 2;
  uint8_t   stage = 0;

// Following 2 members are harcoded to allow a maximum of 16 entries
  uint16_t  alive;
  uint32_t  devices[2];
} EZOManager;



// The main driver is the same for all devices.
// What changes is the implementation of the class itself
template <class T, EZOType type> bool XsnsEZO(uint8_t function)
{
  if (!I2cEnabled(XI2C_55)) {
    return false;
  }

  // Initialization: Gather the list of devices for this class
  if ((T::count < 0) && (function == FUNC_EVERY_SECOND)) {
    uint32_t addr[EZO_ADDR_n];
    T::count = EZOManager.getDevice(type, &addr[0]);

    if (T::count > 0) {
      T::list  = new T[T::count];

      for (uint32_t i = 0; i < T::count; i++) {
        T::list[i].addr = addr[i];
      }
    }
  }

  // Process the function on each of them
  T *cur = &T::list[0];
  for (int32_t i = 0; i < T::count; i++) {
    switch (function) {
      case FUNC_COMMAND_SENSOR:
        cur->ProcessMeasurement();
        cur->HandleCommand(i);
        break;

      case FUNC_EVERY_SECOND:
        if (uptime & 1) {
          cur->ProcessMeasurement();
          cur->MeasureRequest();
        }
        break;

      case FUNC_JSON_APPEND:
        cur->Show(1, i);
        break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        cur->Show(0, i);
        break;
    }
#endif  // USE_WEBSERVER
    cur++;
  }

  return false;
}

#endif  // USE_EZO*
#endif  // USE_I2C
