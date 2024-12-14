/*
  xdrv_67_mcp23xxx.ino - MCP23008/MCP23017/MCP23S17 GPIO Expander support for Tasmota

  SPDX-FileCopyrightText: 2023 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_MCP23XXX_DRV
/*********************************************************************************************\
 * MCP23008/17 (I2C) and MCP23S17 (SPI) GPIO Expander to be used as virtual button/switch/relay only
 *
 * Docs at https://www.microchip.com/wwwproducts/en/MCP23008
 *         https://www.microchip.com/wwwproducts/en/MCP23017
 *
 * I2C Address: 0x20 - 0x26 (0x27 is not supported)
 *
 * The goal of the driver is to provide a sequential list of pins configured as Tasmota template
 * and handle any input and output as configured GPIOs.
 *
 * Restrictions:
 * - Uses incremental I2C addresses / SPI Chip select until template pin count reached
 * - Max support for 28 switches (input), 32 buttons (input), 32 relays (output)
 *
 * Supported template fields:
 * NAME  - Template name
 * BASE  - Optional. 0 = use relative buttons and switches (default), 1 = use absolute buttons and switches
 * IOCON - Optional. IOCON I/O Expander configuration register (bitmap: 0 MIRROR 0 DISSLW HAEN ODR INTPOL 0. Default 0b01011000 = 0x58)
 * GPIO  - Sequential list of pin 1 and up with configured GPIO function
 *         Function             Code      Description
 *         -------------------  --------  ----------------------------------------
 *         None                 0         Not used
 *         Button1..32     B    32..63    Button to Gnd with internal pullup
 *         Button_n1..32   Bn   64..95    Button to Gnd without internal pullup
 *         Button_i1..32   Bi   96..127   Button inverted to Vcc with internal pullup
 *         Button_in1..32  Bin  128..159  Button inverted to Vcc without internal pullup
 *         Switch1..28     S    160..187  Switch to Gnd with internal pullup
 *         Switch_n1..28   Sn   192..219  Switch to Gnd without internal pullup
 *         Relay1..32      R    224..255  Relay
 *         Relay_i1..32    Ri   256..287  Relay inverted
 *         Output_Hi       Oh   3840      Fixed output high
 *         Output_lo       Ol   3872      Fixed output low
 *
 * Prepare a template to be loaded either by:
 * - a rule like: rule3 on file#mcp23x.dat do {"NAME":"MCP23017 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]} endon
 * - a script like: -y{"NAME":"MCP23017 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}
 * - file called mcp23x.dat with contents: {"NAME":"MCP23017 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}
 *
 *                                           S3  S2  B2 B3 Oh   B1 S1    R1        R4  R2  R3  S4
 * {"NAME":"MCP23S17 Shelly Pro 4PM","GPIO":[194,193,65,66,3840,64,192,0,224,0,0,0,227,225,226,195]}
 *
 * Inverted relays and buttons                Ri8 Ri7 Ri6 Ri5 Ri4 Ri3 Ri2 Ri1 B1 B2 B3 B4 B5 B6 B7 B8
 * {"NAME":"MCP23017 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}
 *
 * Inverted relays and buttons                Ri1 Ri2 Ri3 Ri4 Ri5 Ri6 Ri7 Ri8 B1 B2 B3 B4 B5 B6 B7 B8
 * {"NAME":"MCP23017 A=Ri1-8, B=B1-8","GPIO":[256,257,258,259,260,261,262,263,32,33,34,35,36,37,38,39]}
 *
 * Relays and buttons                        R1  R2  R3  R4  R5  R6  R7  R8  B1 B2 B3 B4 B5 B6 B7 B8
 * {"NAME":"MCP23017 A=R1-8, B=B1-8","GPIO":[224,225,226,227,228,229,230,231,32,33,34,35,36,37,38,39]}
 *
 * Buttons and relays                        B1 B2 B3 B4 B5 B6 B7 B8 R1  R2  R3  R4  R5  R6  R7  R8
 * {"NAME":"MCP23017 A=B1-8, B=R1-8","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231]}
 * 
 * Buttons and relays with open-drain INT    B1 B2 B3 B4 B5 B6 B7 B8 R1  R2  R3  R4  R5  R6  R7  R8
 * {"NAME":"MCP23017 A=B1-8, B=R1-8","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231],"IOCON":0x5C}
 *
 * Buttons, relays, buttons and relays                         B1 B2 B3 B4 B5 B6 B7 B8 R1  R2  R3  R4  R5  R6  R7  R8  B9 B10B11B12B13B14B15B16R9  R10 R11 R12 R13 R14 R15 R16
 * {"NAME":"MCP23017 A=B1-8, B=R1-8, C=B9-16, D=R9-16","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231,40,41,42,43,44,45,46,47,232,233,234,235,236,237,238,239]}
 *
 * {"NAME":"MCP23017 A=R1-8, B=B1-8, C=R9-16, D=B9-16","GPIO":[224,225,226,227,228,229,230,231,32,33,34,35,36,37,38,39,232,233,234,235,236,237,238,239,40,41,42,43,44,45,46,47]}
 *
 * 32 relays                                                          R1  R2  R3  R4  R5  R6  R7  R8  R9  R10 R11 R12 R13 R14 R15 R16 R17 R18 R19 R20 R21 R22 R23 R24 R25 R26 R27 R28 R29 R30 R31 R32
 * {"NAME":"MCP23017 A=Ri1-8, B=Ri9-16, C=Ri17-24, D=Ri25-32","GPIO":[256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287]}
 *     {"NAME":"MCP23017 A=R1-8, B=R9-16, C=R17-24, D=R25-32","GPIO":[224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255]}
 *
\*********************************************************************************************/

#define XDRV_67                  67
#define XI2C_77                  77       // See I2CDEVICES.md

#ifndef MCP23XXX_ADDR_START
#define MCP23XXX_ADDR_START      0x20     // 32
#endif
#ifndef MCP23XXX_ADDR_END
#define MCP23XXX_ADDR_END        0x26     // 38
#endif

#define MCP23XXX_MAX_DEVICES     6

#define MCP23XXX_SPI_CLOCK       1000000  // SPI clock speed set to 1MHz in case of signal interference at higher speed (Max is 10MHz)

/*********************************************************************************************\
 * MCP23017 support
\*********************************************************************************************/

#define D_JSON_IOCON "IOCON"

enum MCP23S08GPIORegisters {
  MCP23X08_IODIR = 0x00,
  MCP23X08_IPOL = 0x01,
  MCP23X08_GPINTEN = 0x02,
  MCP23X08_DEFVAL = 0x03,
  MCP23X08_INTCON = 0x04,
  MCP23X08_IOCON = 0x05,
  MCP23X08_GPPU = 0x06,
  MCP23X08_INTF = 0x07,
  MCP23X08_INTCAP = 0x08,
  MCP23X08_GPIO = 0x09,
  MCP23X08_OLAT = 0x0A,
};

enum MCP23X17GPIORegisters {
  // A side
  MCP23X17_IODIRA = 0x00,
  MCP23X17_IPOLA = 0x02,
  MCP23X17_GPINTENA = 0x04,
  MCP23X17_DEFVALA = 0x06,
  MCP23X17_INTCONA = 0x08,
  MCP23X17_IOCONA = 0x0A,
  MCP23X17_GPPUA = 0x0C,
  MCP23X17_INTFA = 0x0E,
  MCP23X17_INTCAPA = 0x10,
  MCP23X17_GPIOA = 0x12,
  MCP23X17_OLATA = 0x14,
  // B side
  MCP23X17_IODIRB = 0x01,
  MCP23X17_IPOLB = 0x03,
  MCP23X17_GPINTENB = 0x05,
  MCP23X17_DEFVALB = 0x07,
  MCP23X17_INTCONB = 0x09,
  MCP23X17_IOCONB = 0x0B,
  MCP23X17_GPPUB = 0x0D,
  MCP23X17_INTFB = 0x0F,
  MCP23X17_INTCAPB = 0x11,
  MCP23X17_GPIOB = 0x13,
  MCP23X17_OLATB = 0x15,
};

enum MCP23XInterruptMode { MCP23XXX_NO_INTERRUPT, MCP23XXX_CHANGE, MCP23XXX_RISING, MCP23XXX_FALLING };

enum MCP23XInterfaces { MCP23X_I2C, MCP23X_SPI };

typedef struct {
  uint8_t olata;
  uint8_t olatb;
  uint8_t address;
  uint8_t interface;
  uint8_t pins;                           // 8 (MCP23x08) or 16 (MCP23x17)
  int8_t pin_cs;
  int8_t pin_int;
} tMcp23xDevice;

typedef union {                           // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t reg;                            // Allow bit manipulation using template IOCON
  struct {
    uint8_t spare0 : 1;                   // 0   Unimplemented
    uint8_t INTPOL : 1;                   // (0) INT pin active-low. (1) active-high
    uint8_t ODR : 1;                      // (0) INT pin active driver output. (1) Open-drain output (overrides INTPOL)
    uint8_t HAEN : 1;                     // (1) Hardware Address enabled (MCS23S17 only)
    uint8_t DISSLW : 1;                   // (0) SDA output slew rate disabled
    uint8_t SEQOP : 1;                    // 0   Sequential operation enabled, address pointer increments
    uint8_t MIRROR : 1;                   // (1) INT pins are internally connected
    uint8_t BANK : 1;                     // 0   Registers are in the same bank (addresses are sequential) (MCS23x17 only)
  };
} tIOCON;

struct MCP230 {
  tMcp23xDevice device[MCP23XXX_MAX_DEVICES];
  uint32_t relay_inverted;
  uint32_t button_inverted;
  uint8_t chip;
  uint8_t max_devices;
  uint8_t max_pins;
  uint8_t relay_max;
  uint8_t relay_offset;
  uint8_t button_max;
  uint8_t switch_max;
  tIOCON iocon;
  int8_t button_offset;
  int8_t switch_offset;
  bool base;
  bool interrupt;
} Mcp23x;

uint16_t *Mcp23x_gpio_pin = nullptr;

/*********************************************************************************************\
 * MCP23x17 - SPI and I2C
\*********************************************************************************************/

#ifdef USE_SPI
void MCP23xEnable(void) {
  SPI.beginTransaction(SPISettings(MCP23XXX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
  digitalWrite(Mcp23x.device[Mcp23x.chip].pin_cs, 0);
}

void MCP23xDisable(void) {
  SPI.endTransaction();
  digitalWrite(Mcp23x.device[Mcp23x.chip].pin_cs, 1);
}
#endif

void MCP23xDumpRegs(void) {
  uint8_t data[22];
  for (Mcp23x.chip = 0; Mcp23x.chip < Mcp23x.max_devices; Mcp23x.chip++) {
    uint32_t data_size = sizeof(data);
    if (8 == Mcp23x.device[Mcp23x.chip].pins) { data_size /= 2; }
#ifdef USE_SPI
    if (MCP23X_SPI == Mcp23x.device[Mcp23x.chip].interface) {
      MCP23xEnable();
      SPI.transfer(Mcp23x.device[Mcp23x.chip].address | 1);
      SPI.transfer(0);
      for (uint32_t i = 0; i < data_size; i++) {
        data[i] = SPI.transfer(0xFF);
      }
      MCP23xDisable();
    }
#endif
#ifdef USE_I2C
    if (MCP23X_I2C == Mcp23x.device[Mcp23x.chip].interface) {
      I2cReadBuffer(Mcp23x.device[Mcp23x.chip].address, 0, data, data_size);
    }
#endif
    AddLog(LOG_LEVEL_DEBUG, PSTR("MCP: Intf %d, Address %02X, Regs %*_H"), Mcp23x.device[Mcp23x.chip].interface, Mcp23x.device[Mcp23x.chip].address, data_size, data);
  }
}

uint32_t MCP23xRead16(uint8_t reg) {
  // Read 16-bit registers: (regb << 8) | rega
  uint32_t value = 0;
#ifdef USE_SPI
  if (MCP23X_SPI == Mcp23x.device[Mcp23x.chip].interface) {
    MCP23xEnable();
    SPI.transfer(Mcp23x.device[Mcp23x.chip].address | 1);
    SPI.transfer(reg);
    value = SPI.transfer(0xFF);  // RegA
    value |= (SPI.transfer(0xFF) << 8);   // RegB
    MCP23xDisable();
  }
#endif
#ifdef USE_I2C
  if (MCP23X_I2C == Mcp23x.device[Mcp23x.chip].interface) {
    value = I2cRead16LE(Mcp23x.device[Mcp23x.chip].address, reg);
  }
#endif
  return value;
}

uint32_t MCP23xRead(uint8_t reg) {
  uint32_t value = 0;
#ifdef USE_SPI
  if (MCP23X_SPI == Mcp23x.device[Mcp23x.chip].interface) {
    MCP23xEnable();
    SPI.transfer(Mcp23x.device[Mcp23x.chip].address | 1);
    SPI.transfer(reg);
    value = SPI.transfer(0xFF);
    MCP23xDisable();
  }
#endif
#ifdef USE_I2C
  if (MCP23X_I2C == Mcp23x.device[Mcp23x.chip].interface) {
    value = I2cRead8(Mcp23x.device[Mcp23x.chip].address, reg);
  }
#endif
  return value;
}

bool MCP23xValidRead(uint8_t reg, uint8_t *data) {
#ifdef USE_SPI
  if (MCP23X_SPI == Mcp23x.device[Mcp23x.chip].interface) {
    MCP23xEnable();
    SPI.transfer(Mcp23x.device[Mcp23x.chip].address | 1);
    SPI.transfer(reg);
    *data = SPI.transfer(0xFF);
    MCP23xDisable();
    return true;
  }
#endif
#ifdef USE_I2C
  if (MCP23X_I2C == Mcp23x.device[Mcp23x.chip].interface) {
    return I2cValidRead8(data, Mcp23x.device[Mcp23x.chip].address, reg);
  }
  return false;
#endif
}

void MCP23xWrite(uint8_t reg, uint8_t value) {
#ifdef USE_SPI
  if (MCP23X_SPI == Mcp23x.device[Mcp23x.chip].interface) {
    MCP23xEnable();
    SPI.transfer(Mcp23x.device[Mcp23x.chip].address);
    SPI.transfer(reg);
    SPI.transfer(value);
    MCP23xDisable();
  }
#endif
#ifdef USE_I2C
  if (MCP23X_I2C == Mcp23x.device[Mcp23x.chip].interface) {
    I2cWrite8(Mcp23x.device[Mcp23x.chip].address, reg, value);
  }
#endif
}

/*********************************************************************************************/

void MCP23xUpdate(uint8_t pin, bool pin_value, uint8_t reg_addr) {
  // pin = 0 - 15
  uint8_t bit = pin % 8;
  uint8_t reg_value = 0;
  if (reg_addr == MCP23X17_OLATA) {
    reg_value = Mcp23x.device[Mcp23x.chip].olata;
    if (8 == Mcp23x.device[Mcp23x.chip].pins) {
      reg_addr = MCP23X08_OLAT;
    }
  } else if (reg_addr == MCP23X17_OLATB) {
    reg_value = Mcp23x.device[Mcp23x.chip].olatb;
  } else {
    reg_value = MCP23xRead(reg_addr);
  }
  if (pin_value) {
    reg_value |= 1 << bit;
  } else {
    reg_value &= ~(1 << bit);
  }
  MCP23xWrite(reg_addr, reg_value);
  if ((8 == Mcp23x.device[Mcp23x.chip].pins) && (reg_addr == MCP23X08_OLAT)) {
    reg_addr = MCP23X17_OLATA;
  }
  if (reg_addr == MCP23X17_OLATA) {
    Mcp23x.device[Mcp23x.chip].olata = reg_value;
  } else if (reg_addr == MCP23X17_OLATB) {
    Mcp23x.device[Mcp23x.chip].olatb = reg_value;
  }
}

/*********************************************************************************************/

uint32_t MCP23xSetChip(uint8_t pin) {
  // Calculate chip based on number of pins per chip. 8 for MCP23x08, 16 for MCP23x17
  // pin 0 - 63
  for (Mcp23x.chip = 0; Mcp23x.chip < Mcp23x.max_devices; Mcp23x.chip++) {
    if (Mcp23x.device[Mcp23x.chip].pins > pin) { break; }
    pin -= Mcp23x.device[Mcp23x.chip].pins;
  }
  return pin;                                          // relative pin number within chip (0 ... 7 or 0 ... 15)
}

void MCP23xPinMode(uint8_t pin, uint8_t flags) {
  // pin 0 - 63
  pin = MCP23xSetChip(pin);
  uint8_t iodir;
  uint8_t gppu;
  if (8 == Mcp23x.device[Mcp23x.chip].pins) {
    iodir = MCP23X08_IODIR;
    gppu = MCP23X08_GPPU;
  } else {
    iodir = pin < 8 ? MCP23X17_IODIRA : MCP23X17_IODIRB;
    gppu = pin < 8 ? MCP23X17_GPPUA : MCP23X17_GPPUB;
  }
  switch (flags) {
    case INPUT:
      MCP23xUpdate(pin, true, iodir);                  // Pin is configured as an input
      MCP23xUpdate(pin, false, gppu);                  // Pull-up disabled
      break;
    case INPUT_PULLUP:
      MCP23xUpdate(pin, true, iodir);                  // Pin is configured as an input
      MCP23xUpdate(pin, true, gppu);                   // Pull-up enabled
      break;
    case OUTPUT:
      MCP23xUpdate(pin, false, iodir);                 // Pin is configured as an output
      break;
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: MCP23xPinMode chip %d, pin %d, flags %d, regs %d,%d"), Mcp23x.chip, pin, flags, iodir, gppu);
}

void MCP23xPinInterruptMode(uint8_t pin, uint8_t interrupt_mode) {
  // pin 0 - 63
  pin = MCP23xSetChip(pin);
  uint8_t gpinten;
  uint8_t intcon;
  uint8_t defval;
  if (8 == Mcp23x.device[Mcp23x.chip].pins) {
    gpinten = MCP23X08_GPINTEN;
    intcon = MCP23X08_INTCON;
    defval = MCP23X08_DEFVAL;
  } else {
    gpinten = pin < 8 ? MCP23X17_GPINTENA : MCP23X17_GPINTENB;
    intcon = pin < 8 ? MCP23X17_INTCONA : MCP23X17_INTCONB;
    defval = pin < 8 ? MCP23X17_DEFVALA : MCP23X17_DEFVALB;
  }
  switch (interrupt_mode) {
    case MCP23XXX_CHANGE:
      MCP23xUpdate(pin, true, gpinten);                // Enable GPIO input pin for interrupt-on-change event
      MCP23xUpdate(pin, false, intcon);                // Pin value is compared against the previous pin value
      break;
    case MCP23XXX_RISING:
      MCP23xUpdate(pin, true, gpinten);                // Enable GPIO input pin for interrupt-on-change event
      MCP23xUpdate(pin, true, intcon);                 // Controls how the associated pin value is compared for interrupt-on-change
      MCP23xUpdate(pin, false, defval);                // If the associated pin level is the opposite from the register bit, an interrupt occurs.
      break;
    case MCP23XXX_FALLING:
      MCP23xUpdate(pin, true, gpinten);                // Enable GPIO input pin for interrupt-on-change event
      MCP23xUpdate(pin, true, intcon);                 // Controls how the associated pin value is compared for interrupt-on-change
      MCP23xUpdate(pin, true, defval);                 // If the associated pin level is the opposite from the register bit, an interrupt occurs.
      break;
    case MCP23XXX_NO_INTERRUPT:
      MCP23xUpdate(pin, false, gpinten);               // Disable GPIO input pin for interrupt-on-change event
      break;
  }
}

void MCP23xSetPinModes(uint8_t pin, uint8_t flags) {
  // pin 0 - 63
  MCP23xPinMode(pin, flags);
  if (Mcp23x.device[Mcp23x.chip].pin_int > -1) {       // Mcp23x.chip is updated by call to MCP23xPinMode
    MCP23xPinInterruptMode(pin, (Mcp23x.iocon.ODR) ? MCP23XXX_FALLING : MCP23XXX_CHANGE);
  }
}

bool MCP23xDigitalRead(uint8_t pin) {
  // pin 0 - 63
  pin = MCP23xSetChip(pin);
  uint8_t bit = pin % 8;
  uint8_t reg_addr;
  if (8 == Mcp23x.device[Mcp23x.chip].pins) {
    reg_addr = MCP23X08_GPIO;
  } else {
    reg_addr = pin < 8 ? MCP23X17_GPIOA : MCP23X17_GPIOB;
  }
  uint8_t value = MCP23xRead(reg_addr);
  return value & (1 << bit);
}

void MCP23xDigitalWrite(uint8_t pin, bool value) {
  // pin 0 - 63
  pin = MCP23xSetChip(pin);
  uint8_t reg_addr = pin < 8 ? MCP23X17_OLATA : MCP23X17_OLATB;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: MCP23xDigitalWrite chip %d, pin %d, state %d, reg %d"), Mcp23x.chip, pin, value, reg_addr);

  MCP23xUpdate(pin, value, reg_addr);
}

/*********************************************************************************************\
 * Tasmota
\*********************************************************************************************/

int MCP23xPin(uint32_t gpio, uint32_t index = 0);
int MCP23xPin(uint32_t gpio, uint32_t index) {
  uint16_t real_gpio = gpio << 5;
  uint16_t mask = 0xFFE0;
  if (index < GPIO_ANY) {
    real_gpio += index;
    mask = 0xFFFF;
  }
  for (uint32_t i = 0; i <= Mcp23x.max_pins; i++) {
    if ((Mcp23x_gpio_pin[i] & mask) == real_gpio) {
      return i;                                        // Pin number configured for gpio
    }
  }
  return -1;                                           // No pin used for gpio
}

bool MCP23xPinUsed(uint32_t gpio, uint32_t index = 0);
bool MCP23xPinUsed(uint32_t gpio, uint32_t index) {
  return (MCP23xPin(gpio, index) >= 0);
}

uint32_t MCP23xGetPin(uint32_t lpin) {
  if (lpin <= Mcp23x.max_pins) {
    return Mcp23x_gpio_pin[lpin];
  } else {
    return GPIO_NONE;
  }
}

/*********************************************************************************************/

bool MCP23xAddItem(uint8_t &item) {
  if (item >= MAX_RELAYS_SET) {                        // MAX_RELAYS_SET = MAX_SWITCHES_SET = MAX_KEYS_SET = 32
    AddLog(LOG_LEVEL_INFO, PSTR("MCP: Max reached"));
    return false;
  }
  item++;
  return true;
}

String MCP23xTemplateLoadFile(void) {
  String mcptmplt = "";
#ifdef USE_UFILESYS
  mcptmplt = TfsLoadString("/mcp23x.dat");
#endif  // USE_UFILESYS
#ifdef USE_RULES
  if (!mcptmplt.length()) {
    mcptmplt = RuleLoadFile("MCP23X.DAT");
  }
#endif  // USE_RULES
#ifdef USE_SCRIPT
  if (!mcptmplt.length()) {
    mcptmplt = ScriptLoadSection(">y");
  }
#endif  // USE_SCRIPT
  return mcptmplt;
}

bool MCP23xLoadTemplate(void) {
  String mcptmplt = MCP23xTemplateLoadFile();
  uint32_t len = mcptmplt.length() +1;
  if (len < 7) { return false; }                       // No McpTmplt found

  JsonParser parser((char*)mcptmplt.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  // rule3 on file#mcp23x.dat do {"NAME":"MCP23017","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231]} endon
  // rule3 on file#mcp23x.dat do {"NAME":"MCP23017","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]} endon
  // rule3 on file#mcp23x.dat do {"NAME":"MCP23017 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]} endon
  // rule3 on file#mcp23x.dat do {"NAME":"MCP23017 A=Ri8-1, B=B1-8, C=Ri16-9, D=B9-16","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39,271,270,269,268,267,266,265,264,40,41,42,43,44,45,46,47]} endon

  // {"NAME":"MCP23017","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231]}
  // {"NAME":"MCP23017","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231,40,41,42,43,44,45,46,47,232,233,234,235,236,237,238,239]}
  JsonParserToken val = root[PSTR(D_JSON_BASE)];
  if (val) {
    Mcp23x.base = (val.getUInt()) ? true : false;
  }
  val = root[PSTR(D_JSON_NAME)];
  if (val) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MCP: IOCON 0x%02X, Base %d, Template '%s'"), Mcp23x.iocon, Mcp23x.base, val.getStr());
  }
  JsonParserArray arr = root[PSTR(D_JSON_GPIO)];
  if (arr) {
    uint32_t pin = 0;
    for (pin; pin < Mcp23x.max_pins; pin++) {          // Max number of detected chip pins
      JsonParserToken val = arr[pin];
      if (!val) { break; }
      uint16_t mpin = val.getUInt();
      if (mpin) {                                      // Above GPIO_NONE
        if ((mpin >= AGPIO(GPIO_SWT1)) && (mpin < (AGPIO(GPIO_SWT1) + MAX_SWITCHES_SET)) && MCP23xAddItem(Mcp23x.switch_max)) {
          MCP23xSetPinModes(pin, INPUT_PULLUP);
        }
        else if ((mpin >= AGPIO(GPIO_SWT1_NP)) && (mpin < (AGPIO(GPIO_SWT1_NP) + MAX_SWITCHES_SET)) && MCP23xAddItem(Mcp23x.switch_max)) {
          mpin -= (AGPIO(GPIO_SWT1_NP) - AGPIO(GPIO_SWT1));
          MCP23xSetPinModes(pin, INPUT);
        }
        else if ((mpin >= AGPIO(GPIO_KEY1)) && (mpin < (AGPIO(GPIO_KEY1) + MAX_KEYS_SET)) && MCP23xAddItem(Mcp23x.button_max)) {
          MCP23xSetPinModes(pin, INPUT_PULLUP);
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_NP)) && (mpin < (AGPIO(GPIO_KEY1_NP) + MAX_KEYS_SET)) && MCP23xAddItem(Mcp23x.button_max)) {
          mpin -= (AGPIO(GPIO_KEY1_NP) - AGPIO(GPIO_KEY1));
          MCP23xSetPinModes(pin, INPUT);
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_INV)) && (mpin < (AGPIO(GPIO_KEY1_INV) + MAX_KEYS_SET)) && MCP23xAddItem(Mcp23x.button_max)) {
          bitSet(Mcp23x.button_inverted, mpin - AGPIO(GPIO_KEY1_INV));
          mpin -= (AGPIO(GPIO_KEY1_INV) - AGPIO(GPIO_KEY1));
          MCP23xSetPinModes(pin, INPUT_PULLUP);
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_INV_NP)) && (mpin < (AGPIO(GPIO_KEY1_INV_NP) + MAX_KEYS_SET)) && MCP23xAddItem(Mcp23x.button_max)) {
          bitSet(Mcp23x.button_inverted, mpin - AGPIO(GPIO_KEY1_INV_NP));
          mpin -= (AGPIO(GPIO_KEY1_INV_NP) - AGPIO(GPIO_KEY1));
          MCP23xSetPinModes(pin, INPUT);
        }
        else if ((mpin >= AGPIO(GPIO_REL1)) && (mpin < (AGPIO(GPIO_REL1) + MAX_RELAYS_SET)) && MCP23xAddItem(Mcp23x.relay_max)) {
          MCP23xPinMode(pin, OUTPUT);
        }
        else if ((mpin >= AGPIO(GPIO_REL1_INV)) && (mpin < (AGPIO(GPIO_REL1_INV) + MAX_RELAYS_SET)) && MCP23xAddItem(Mcp23x.relay_max)) {
          bitSet(Mcp23x.relay_inverted, mpin - AGPIO(GPIO_REL1_INV));
          mpin -= (AGPIO(GPIO_REL1_INV) - AGPIO(GPIO_REL1));
          MCP23xPinMode(pin, OUTPUT);
        }
        else if (mpin == AGPIO(GPIO_OUTPUT_HI)) {
          MCP23xPinMode(pin, OUTPUT);
          MCP23xDigitalWrite(pin, 1);
        }
        else if (mpin == AGPIO(GPIO_OUTPUT_LO)) {
          MCP23xPinMode(pin, OUTPUT);
          MCP23xDigitalWrite(pin, 0);
        }
        else { mpin = 0; }
        Mcp23x_gpio_pin[pin] = mpin;
      }
    }
    Mcp23x.max_pins = pin;                             // Max number of configured pins
    AddLog(LOG_LEVEL_INFO, PSTR("MCP: Pins %d (S%d/B%d/R%d)"), Mcp23x.max_pins, Mcp23x.switch_max, Mcp23x.button_max, Mcp23x.relay_max);
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("MCP: Pins %d, Mcp23x_gpio_pin %*_V"), Mcp23x.max_pins, Mcp23x.max_pins, (uint8_t*)Mcp23x_gpio_pin);

  return true;
}

uint32_t MCP23xTemplateGpio(void) {
  String mcptmplt = MCP23xTemplateLoadFile();
  uint32_t len = mcptmplt.length() +1;
  if (len < 7) { return 0; }                           // No McpTmplt found

  JsonParser parser((char*)mcptmplt.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return 0; }

  JsonParserToken val = root[PSTR(D_JSON_IOCON)];
  if (val) {
    Mcp23x.iocon.reg = val.getUInt() & 0x5E;          // Only allow 0 MIRROR 0 DISSLW HAEN ODR INTPOL 0
  }
  JsonParserArray arr = root[PSTR(D_JSON_GPIO)];
  if (arr.isArray()) {
    return arr.size();                                // Number of requested pins
  }
  return 0;
}

void MCP23xModuleInit(void) {
  Mcp23x.iocon.reg = 0b01011000;                      // Default 0x58 = Enable INT mirror, Disable Slew rate, HAEN pins for addressing
  int32_t pins_needed = MCP23xTemplateGpio();
  if (!pins_needed) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MCP: Invalid template"));
    return;
  }

#ifdef USE_SPI
  if ((SPI_MOSI_MISO == TasmotaGlobal.spi_enabled) && PinUsed(GPIO_MCP23SXX_CS, GPIO_ANY)) {
#ifdef ESP8266
    SPI.begin();
#endif
#ifdef ESP32
    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif
    while ((Mcp23x.max_devices < MCP23XXX_MAX_DEVICES) && PinUsed(GPIO_MCP23SXX_CS, Mcp23x.max_devices)) {
      Mcp23x.chip = Mcp23x.max_devices;
      uint32_t pin_int = (Mcp23x.iocon.ODR) ? 0 : Mcp23x.chip;  // INT ODR pins are open-drain outputs and supposedly connected together to one GPIO
      Mcp23x.device[Mcp23x.chip].pin_int = (PinUsed(GPIO_MCP23XXX_INT, pin_int)) ? Pin(GPIO_MCP23XXX_INT, pin_int) : -1;
      Mcp23x.device[Mcp23x.chip].pin_cs = Pin(GPIO_MCP23SXX_CS, Mcp23x.max_devices);
      digitalWrite(Mcp23x.device[Mcp23x.chip].pin_cs, 1);
      pinMode(Mcp23x.device[Mcp23x.chip].pin_cs, OUTPUT);
      Mcp23x.device[Mcp23x.chip].interface = MCP23X_SPI;
      Mcp23x.device[Mcp23x.chip].address = MCP23XXX_ADDR_START << 1;

      MCP23xWrite(MCP23X08_IOCON, 0x80);               // Attempt to set bank mode - this will only work on MCP23x17, so its the best way to detect the different chips 23x08 vs 23x17
      uint8_t buffer;
      if (MCP23xValidRead(MCP23X08_IOCON, &buffer)) {
        if (0x00 == buffer) { // MCP23S08
          AddLog(LOG_LEVEL_INFO, PSTR("SPI: MCP23S08 found at CS%d"), Mcp23x.chip +1);
          Mcp23x.device[Mcp23x.chip].pins = 8;
//          MCP23xWrite(MCP23X08_IOCON, 0b00011000);  // Slew rate disabled, HAEN pins for addressing
          MCP23xWrite(MCP23X08_IOCON, Mcp23x.iocon.reg & 0x3E);
          Mcp23x.device[Mcp23x.chip].olata = MCP23xRead(MCP23X08_OLAT);
        } else if (0x80 == buffer) { // MCP23S17
          AddLog(LOG_LEVEL_INFO, PSTR("SPI: MCP23S17 found at CS%d"), Mcp23x.chip +1);
          Mcp23x.device[Mcp23x.chip].pins = 16;
//          MCP23xWrite(MCP23X17_IOCONA, 0b01011000);    // Enable INT mirror, Slew rate disabled, HAEN pins for addressing
          MCP23xWrite(MCP23X17_IOCONA, Mcp23x.iocon.reg);
          Mcp23x.device[Mcp23x.chip].olata = MCP23xRead(MCP23X17_OLATA);
          Mcp23x.device[Mcp23x.chip].olatb = MCP23xRead(MCP23X17_OLATB);
        }
      }
      Mcp23x.max_devices++;

      Mcp23x.max_pins += Mcp23x.device[Mcp23x.chip].pins;
      pins_needed -= Mcp23x.device[Mcp23x.chip].pins;
      if (!pins_needed) { break; }
    }
  } else {
#endif  // USE_SPI
#ifdef USE_I2C
    uint8_t mcp23xxx_address = MCP23XXX_ADDR_START;
    while ((Mcp23x.max_devices < MCP23XXX_MAX_DEVICES) && (mcp23xxx_address < MCP23XXX_ADDR_END)) {
      Mcp23x.chip = Mcp23x.max_devices;
      uint32_t pin_int = (Mcp23x.iocon.ODR) ? 0 : Mcp23x.chip;  // INT pins are open-drain outputs and supposedly connected together to one GPIO
      if (I2cSetDevice(mcp23xxx_address)) {
        Mcp23x.device[Mcp23x.chip].pin_int = (PinUsed(GPIO_MCP23XXX_INT, pin_int)) ? Pin(GPIO_MCP23XXX_INT, pin_int) : -1;
        Mcp23x.device[Mcp23x.chip].interface = MCP23X_I2C;
        Mcp23x.device[Mcp23x.chip].address = mcp23xxx_address;

        MCP23xWrite(MCP23X08_IOCON, 0x80);               // Attempt to set bank mode - this will only work on MCP23x17, so its the best way to detect the different chips 23x08 vs 23x17
        uint8_t buffer;
        if (MCP23xValidRead(MCP23X08_IOCON, &buffer)) {
          if (0x00 == buffer) {
            I2cSetActiveFound(mcp23xxx_address, "MCP23008");
            Mcp23x.device[Mcp23x.chip].pins = 8;
//            MCP23xWrite(MCP23X08_IOCON, 0b00011000);   // Slew rate disabled, HAEN pins for addressing
            MCP23xWrite(MCP23X08_IOCON, Mcp23x.iocon.reg & 0x3E);
            Mcp23x.device[Mcp23x.chip].olata = MCP23xRead(MCP23X08_OLAT);
            Mcp23x.max_devices++;
          }
          else if (0x80 == buffer) {
            I2cSetActiveFound(mcp23xxx_address, "MCP23017");
            Mcp23x.device[Mcp23x.chip].pins = 16;
            MCP23xWrite(MCP23X08_IOCON, 0x00);           // Reset bank mode to 0 (MCP23X17_GPINTENB)
//            MCP23xWrite(MCP23X17_IOCONA, 0b01011000);  // Enable INT mirror, Slew rate disabled, HAEN pins for addressing
            MCP23xWrite(MCP23X17_IOCONA, Mcp23x.iocon.reg);
            Mcp23x.device[Mcp23x.chip].olata = MCP23xRead(MCP23X17_OLATA);
            Mcp23x.device[Mcp23x.chip].olatb = MCP23xRead(MCP23X17_OLATB);
            Mcp23x.max_devices++;
          }
          Mcp23x.max_pins += Mcp23x.device[Mcp23x.chip].pins;
          pins_needed -= Mcp23x.device[Mcp23x.chip].pins;
        }
      }
      if (pins_needed) {
        mcp23xxx_address++;
      } else {
        mcp23xxx_address = MCP23XXX_ADDR_END;
      }
    }
#endif  // USE_I2C
#ifdef USE_SPI
  }
#endif  // USE_SPI

  if (!Mcp23x.max_devices) { return; }

  Mcp23x_gpio_pin = (uint16_t*)calloc(Mcp23x.max_pins, 2);
  if (!Mcp23x_gpio_pin) { return; }

  if (!MCP23xLoadTemplate()) {
    AddLog(LOG_LEVEL_INFO, PSTR("MCP: No valid template found"));  // Too many GPIO's
    Mcp23x.max_devices = 0;
    return;
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MCP: INT open-drain %d"), Mcp23x.iocon.ODR);

  Mcp23x.relay_offset = TasmotaGlobal.devices_present;
  Mcp23x.relay_max -= UpdateDevicesPresent(Mcp23x.relay_max);

  Mcp23x.button_offset = -1;
  Mcp23x.switch_offset = -1;
}

void MCP23xServiceInput(void) {
  // I found no reliable way to receive interrupts; noise received at undefined moments - unstable usage
  Mcp23x.interrupt = false;
  // This works with no interrupt
  uint32_t pin_offset = 0;
  uint32_t gpio;
  for (Mcp23x.chip = 0; Mcp23x.chip < Mcp23x.max_devices; Mcp23x.chip++) {
    if (8 == Mcp23x.device[Mcp23x.chip].pins) {
      gpio = MCP23xRead(MCP23X08_GPIO);                // Read MCP23x08 gpio
    } else {
      gpio = MCP23xRead16(MCP23X17_GPIOA);             // Read MCP23x17 gpio
    }

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MCP: Chip %d, State %04X"), Mcp23x.chip, gpio);

    uint32_t mask = 1;
    for (uint32_t pin = 0; pin < Mcp23x.device[Mcp23x.chip].pins; pin++) {
      uint32_t state = ((gpio & mask) != 0);
      uint32_t lpin = MCP23xGetPin(pin_offset + pin);  // 0 for None, 32 for KEY1, 160 for SWT1, 224 for REL1
      uint32_t index = lpin & 0x001F;                  // Max 32 buttons or switches
      lpin = BGPIO(lpin);                              // UserSelectablePins number
      if (GPIO_KEY1 == lpin) {
        ButtonSetVirtualPinState(Mcp23x.button_offset + index, (state != bitRead(Mcp23x.button_inverted, index)));
      }
      else if (GPIO_SWT1 == lpin) {
        SwitchSetVirtualPinState(Mcp23x.switch_offset + index, state);
      }
      mask <<= 1;
    }
    pin_offset += Mcp23x.device[Mcp23x.chip].pins;
  }
}

static void IRAM_ATTR MCP23xInputIsr(void) {
  Mcp23x.interrupt = true;
}

void MCP23xInit(void) {
  if (Mcp23x.button_max || Mcp23x.switch_max) {
    uint32_t gpio;
    for (Mcp23x.chip = 0; Mcp23x.chip < Mcp23x.max_devices; Mcp23x.chip++) {
      if (Mcp23x.device[Mcp23x.chip].pin_int > -1) {
        if (8 == Mcp23x.device[Mcp23x.chip].pins) {
          gpio = MCP23xRead(MCP23X08_GPIO);            // Clear MCP23x08 interrupt
        } else {
          gpio = MCP23xRead16(MCP23X17_GPIOA);         // Clear MCP23x17 interrupt
        }
        if (Mcp23x.iocon.ODR && Mcp23x.chip) { continue; }
//        pinMode(Mcp23x.device[Mcp23x.chip].pin_int, (Mcp23x.iocon.ODR) ? INPUT_PULLUP : INPUT);
        pinMode(Mcp23x.device[Mcp23x.chip].pin_int, INPUT_PULLUP);
        attachInterrupt(Mcp23x.device[Mcp23x.chip].pin_int, MCP23xInputIsr, (Mcp23x.iocon.ODR) ? FALLING : CHANGE);
      }
    }
  }
}

void MCP23xPower(void) {
  // XdrvMailbox.index = 32-bit rpower bit mask
  // Use absolute relay indexes unique with main template
  power_t rpower = XdrvMailbox.index;
  uint32_t relay_max = TasmotaGlobal.devices_present;
  if (!Mcp23x.base) {
    // Use relative and sequential relay indexes
    rpower >>= Mcp23x.relay_offset;
    relay_max = Mcp23x.relay_max;
  }
  DevicesPresentNonDisplayOrLight(relay_max);          // Skip display and/or light(s)
  for (uint32_t index = 0; index < relay_max; index++) {
    power_t state = rpower &1;
    if (MCP23xPinUsed(GPIO_REL1, index)) {
      uint32_t pin = MCP23xPin(GPIO_REL1, index) & 0x3F;   // Fix possible overflow over 63 gpios
      MCP23xDigitalWrite(pin, bitRead(Mcp23x.relay_inverted, index) ? !state : state);
    }
    rpower >>= 1;                                      // Select next power
  }
}

bool MCP23xAddButton(void) {
  // XdrvMailbox.index = button/switch index
  uint32_t index = XdrvMailbox.index;
  if (!Mcp23x.base) {
    // Use relative and sequential button indexes
    if (Mcp23x.button_offset < 0) { Mcp23x.button_offset = index; }
    index -= Mcp23x.button_offset;
    if (index >= Mcp23x.button_max) { return false; }
  } else {
    // Use absolute button indexes unique with main template
    if (!MCP23xPinUsed(GPIO_KEY1, index)) { return false; }
    Mcp23x.button_offset = 0;
  }
  XdrvMailbox.index = (MCP23xDigitalRead(MCP23xPin(GPIO_KEY1, index)) != bitRead(Mcp23x.button_inverted, index));
  return true;
}

bool MCP23xAddSwitch(void) {
  // XdrvMailbox.index = button/switch index
  uint32_t index = XdrvMailbox.index;
  if (!Mcp23x.base) {
    // Use relative and sequential switch indexes
    if (Mcp23x.switch_offset < 0) { Mcp23x.switch_offset = index; }
    index -= Mcp23x.switch_offset;
    if (index >= Mcp23x.switch_max) { return false; }
  } else {
    // Use absolute switch indexes unique with main template
    if (!MCP23xPinUsed(GPIO_SWT1, index)) { return false; }
    Mcp23x.switch_offset = 0;
  }
  XdrvMailbox.index = MCP23xDigitalRead(MCP23xPin(GPIO_SWT1, index));
  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv67(uint32_t function) {
  bool spi_enabled = false;
  bool i2c_enabled = false;
#ifdef USE_SPI
  spi_enabled = (SPI_MOSI_MISO == TasmotaGlobal.spi_enabled);
#endif  // USE_SPI
#ifdef USE_I2C
  i2c_enabled = I2cEnabled(XI2C_77);
#endif  // USE_I2C
  if (!spi_enabled && !i2c_enabled) { return false; }

  bool result = false;

  if (FUNC_SETUP_RING2 == function) {
    MCP23xModuleInit();
  } else if (Mcp23x.max_devices) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        if (!Mcp23x.interrupt) { return false; }
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MCP: Interrupt"));
        MCP23xServiceInput();
        break;
      case FUNC_EVERY_100_MSECOND:
        if (Mcp23x.button_max || Mcp23x.switch_max) {
          MCP23xServiceInput();
        }
        break;
      case FUNC_SET_POWER:
        MCP23xPower();
        break;
      case FUNC_INIT:
        MCP23xInit();
        break;
      case FUNC_ADD_BUTTON:
        result = MCP23xAddButton();
        break;
      case FUNC_ADD_SWITCH:
        result = MCP23xAddSwitch();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_MCP23XXX_DRV
#endif  // USE_I2C or USE_ESP_SPI
