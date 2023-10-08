/*
  xdrv_61_ds3502.ino - DS3502 digital potentiometer support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends and f-reiling

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_DS3502
/*********************************************************************************************\
 * DS3502 - digital potentiometer (https://datasheets.maximintegrated.com/en/ds/DS3502.pdf)
 *
 * I2C Address: 0x28 .. 0x2B
\*********************************************************************************************/

#define XDRV_61                  61
#define XI2C_67                  67  // See I2CDEVICES.md

#define USE_DS3502_ADDR          0x28

#define DS3502_REG_MODE          0x02
#define DS3502_REG_WIPER         0x00

struct DS3502 {
  uint8_t address[MAX_DS3502];
  uint8_t max_devices;
} Ds3502;

void Ds3502SetWiper(uint32_t idx) {
  I2cWrite8(Ds3502.address[idx], DS3502_REG_WIPER, Settings->ds3502_state[idx] & 0x7F);
}

void Ds3502Detect(void) {
  Ds3502.max_devices = 0;
  uint8_t ds3502_address = USE_DS3502_ADDR;
  while (ds3502_address < USE_DS3502_ADDR + MAX_DS3502) {
    if (I2cSetDevice(ds3502_address)) {
      I2cSetActiveFound(ds3502_address, "DS3502");
      Ds3502.address[Ds3502.max_devices] = ds3502_address;
      Ds3502.max_devices++;
    }
    ds3502_address++;
  }
  for (uint32_t idx = 0; idx < Ds3502.max_devices; idx++) {
    I2cWrite8(Ds3502.address[idx], DS3502_REG_MODE, 0x80);  // Mode 1 - Write to non-volatile storage only
    Ds3502SetWiper(idx);                                    // Restore state
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kDS3502Commands[] PROGMEM = "|"  // No Prefix
  "Wiper" ;

void (* const DS3502Command[])(void) PROGMEM = {
  &CmndWiper };

void CmndWiper(void) {
  // Wiper<x> 0..127
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Ds3502.max_devices)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 127)) {
      Settings->ds3502_state[XdrvMailbox.index -1] = XdrvMailbox.payload;
      Ds3502SetWiper(XdrvMailbox.index -1);                 // Set wiper
    }
    ResponseCmndIdxNumber(Settings->ds3502_state[XdrvMailbox.index -1]);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv61(uint32_t function) {
  if (!I2cEnabled(XI2C_67)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Ds3502Detect();
  }
  else if (Ds3502.max_devices) {
    switch (function) {
      case FUNC_COMMAND:
        result = DecodeCommand(kDS3502Commands, DS3502Command);
        break;
    }
  }
  return result;
}

#endif // USE_DS3502
#endif // USE_IC2