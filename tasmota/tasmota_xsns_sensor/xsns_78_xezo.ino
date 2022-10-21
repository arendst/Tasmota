/*
  xsns_78_xezo.ino - EZO family I2C driver support for Tasmota

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
#if defined(USE_EZO)

#define XSNS_78 78
#define XI2C_55 55        // See I2CDEVICES.md

#define EZO_ADDR_0  0x61  // First EZO address
#define EZO_ADDR_n  16    // Number of ports for use with EZO devices


// List of known EZO devices and their default address
enum {
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



// The order of the EZO devices must map with the enum declared above
const char *const EZOSupport[EZO_ADDR_n] PROGMEM = {
#ifdef USE_EZODO
  EZODO::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOORP
  EZOORP::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOPH
  EZOPH::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOEC
  EZOEC::id,
#else
  EZOStruct::id,
#endif
  EZOStruct::id,  // <unnamed>
#ifdef USE_EZORTD
  EZORTD::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOPMP
  EZOPMP::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOFLO
  EZOFLO::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOCO2
  EZOCO2::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZOPRS
  EZOPRS::id,
#else
  EZOStruct::id,
#endif
  EZOStruct::id,  // <unnamed>
#ifdef USE_EZOO2
  EZOO2::id,
#else
  EZOStruct::id,
#endif
  EZOStruct::id,  // <unnamed>
  EZOStruct::id,  // <unnamed>
#ifdef USE_EZOHUM
  EZOHUM::id,
#else
  EZOStruct::id,
#endif
#ifdef USE_EZORGB
  EZORGB::id,
#else
  EZOStruct::id,
#endif
};

#define CREATE_EZO_CLASS(CLASS)             \
  case EZO_ ## CLASS:                       \
    sensor[count] = new EZO ## CLASS(addr); \
    break;


struct EZOManager {
  void Command()
  {
    char *at      = XdrvMailbox.data;
    uint32_t len  = XdrvMailbox.data_len;

    // Figure out if we're trying to address a specific device
    // PS: This should ideally be done through the Tasmota mailbox
    if (at[0] == '-') {
      int32_t idx = atoi(&at[1]) - 1;
      at = strchr(at, ' ');

      if (!at++) {
        return;
      }

      len -= (at - XdrvMailbox.data);

      if ((idx >= 0) && (idx < count)) {
        sensor[idx]->ProcessMeasurement();
        sensor[idx]->HandleCommand(at, len);
      }
    } else {
      for (uint32_t i = 0; i < count; i++) {
        sensor[i]->ProcessMeasurement();
        sensor[i]->HandleCommand(at, len);
      }
    }
  }

  void EverySecond()
  {
    // Do we have to deal with the 2 stage booting process?
    if (count < 0) {
      // EZO devices take 2s to boot
      if (TasmotaGlobal.uptime >= next) {
        count++;

        if (count == -1) {
          DetectRequest();
          next = TasmotaGlobal.uptime + 1;
        } else if (count == 0) {
          ProcessDetection();
        }
      }
    }

    for (int32_t i = 0; i < count; i++) {
      sensor[i]->ProcessMeasurement();
      sensor[i]->MeasureRequest();
    }
  }

  void Show(bool json)
  {
    for (int32_t i = 0; i < count; i++) {
      if (sensor[i]->isValid()) {
        char name[7];
        snprintf_P(name, sizeof(name), PSTR("%s%c%X"), D_EZO_NAME, IndexSeparator(), i + 1);

        if (count == 1) {
          name[sizeof(D_EZO_NAME) - 1] = 0;
        }

        sensor[i]->Show(json, name);
      }
    }
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
    // Check every address that we sent a request to
    for (uint8_t addr = EZO_ADDR_0; addr < EZO_ADDR_0 + EZO_ADDR_n; addr++) {
      if (alive & 1) {
        char data[D_EZO_MAX_BUF];
        Wire.requestFrom(addr, sizeof(data));
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
                I2cSetActiveFound(addr, data);

                // We use switch intead of virtual function to save RAM
                switch (j + EZO_ADDR_0) {
#ifdef USE_EZODO
                  CREATE_EZO_CLASS(DO)
#endif
#ifdef USE_EZOORP
                  CREATE_EZO_CLASS(ORP)
#endif
#ifdef USE_EZOPH
                  CREATE_EZO_CLASS(PH)
#endif
#ifdef USE_EZOEC
                  CREATE_EZO_CLASS(EC)
#endif
#ifdef USE_EZORTD
                  CREATE_EZO_CLASS(RTD)
#endif
#ifdef USE_EZOPMP
                  CREATE_EZO_CLASS(PMP)
#endif
#ifdef USE_EZOFLO
                  CREATE_EZO_CLASS(FLO)
#endif
#ifdef USE_EZOCO2
                  CREATE_EZO_CLASS(CO2)
#endif
#ifdef USE_EZOPRS
                  CREATE_EZO_CLASS(PRS)
#endif
#ifdef USE_EZOO2
                  CREATE_EZO_CLASS(O2)
#endif
#ifdef USE_EZOHUM
                  CREATE_EZO_CLASS(HUM)
#endif
#ifdef USE_EZORGB
                  CREATE_EZO_CLASS(RGB)
#endif
                }
                count++;
              }
            }
          }
        }
      }

      alive >>= 1;
    }
  }

  uint32_t  next  = 2;
  int8_t    count = -2;

  // Following variables are harcoded to allow a maximum of 16 entries
  uint16_t    alive;
  EZOStruct  *sensor[EZO_ADDR_n];
} EZOManager;



// The main driver is the same for all devices.
// What changes is the implementation of the class itself
bool Xsns78(uint8_t function)
{
  if (!I2cEnabled(XI2C_55)) {
    return false;
  }

  switch (function) {
    case FUNC_COMMAND_SENSOR:
      EZOManager.Command();
      break;

    case FUNC_EVERY_SECOND:
      EZOManager.EverySecond();
      break;

    case FUNC_JSON_APPEND:
      EZOManager.Show(1);
      break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      EZOManager.Show(0);
      break;
#endif
  }

  return false;
}

#endif  // USE_EZO
#endif  // USE_I2C
