/*
  xdrv_69_pca9557.ino - PCA9557 GPIO Expander support for Tasmota

  SPDX-FileCopyrightText: 2023 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_PCA9557
/*********************************************************************************************\
 * 8-bit PCA9557 I2C GPIO Expander to be used as virtual relay
 *
 * Docs at https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/8-bit-ic-bus-and-smbus-i-o-port-with-reset:PCA9557
 *
 * I2C Addresses: 0x18 - 0x1F
 *
 * The goal of the driver is to provide a sequential list of pins configured as Tasmota template
 * and handle any input and output as configured GPIOs.
 *
 * Restrictions:
 * - Uses incremental I2C addresses until template pin count reached
 * - Max support for 32  buttons (input) / 28 switches (input) / 32 relays (output)
 *
 * Supported template fields:
 * NAME  - Template name
 * BASE  - Optional. 0 = use relative relays (default), 1 = use absolute relays
 * GPIO  - Sequential list of pin 1 and up with configured GPIO function
 *         Function             Code      Description
 *         -------------------  --------  ----------------------------------------
 *         None                 0         Not used
 *         Button_n1..32   Bn   64..95    Button to Gnd without internal pullup
 *         Button_in1..32  Bin  128..159  Button inverted to Vcc without internal pullup
 *         Switch_n1..28   Sn   192..219  Switch to Gnd without internal pullup
 *         Relay1..32      R    224..255  Relay
 *         Relay_i1..32    Ri   256..287  Relay inverted
 *         Output_Hi       Oh   3840      Fixed output high
 *         Output_lo       Ol   3872      Fixed output low
 *
 * Prepare a template to be loaded either by:
 * - a rule like: rule3 on file#pca9557.dat do {"NAME":"PCA9557","GPIO":[224,225,226,227,228,229,230,231]} endon
 * - a script like: -y{"NAME":"PCA9557","GPIO":[224,225,226,227,228,229,230,231]}
 * - file called pca9557.dat with contents: {"NAME":"PCA9557","GPIO":[224,225,226,227,228,229,230,231]}
 *
 * Inverted relays           Ri1 Ri2 Ri3 Ri4 Ri5 Ri6 Ri7 Ri8
 * {"NAME":"PCA9557","GPIO":[256,257,258,259,260,261,262,263]}
 * 
 * Inverted relays and buttons               Ri8 Ri7 Ri6 Ri5 Ri4 Ri3 Ri2 Ri1 B1 B2 B3 B4 B5 B6 B7 B8
 * {"NAME":"PCA9557 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}
 *
 * Buttons and relays                       B1 B2 B3 B4 R1  R2  R3  R4
 * {"NAME":"PCA9557 A=B1-8, B=R1-8","GPIO":[32,33,34,35,224,225,226,227]}
 * 
 * 16 relays                 R1  R2  R3  R4  R5  R6  R7  R8  R9  R10 R11 R12 R13 R14 R15 R16
 * {"NAME":"PCA9557","GPIO":[224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239]}
 *
 *
\*********************************************************************************************/

#define XDRV_69                  69
#define XI2C_81                  81       // See I2CDEVICES.md

#define PCA9557_ADDR_START       0x18     // 24
#define PCA9557_ADDR_END         0x20     // 32 (not included)

#define PCA9557_MAX_DEVICES      8        


/*********************************************************************************************\
 * PCA9557 support
\*********************************************************************************************/

enum PCA9557Registers {
  PCA9557_R0 = 0x00, // (R/ ) Register 0 - Input port register
  PCA9557_R1 = 0x01, // (R/W) Register 1 - Output port register
  PCA9557_R2 = 0x02, // (R/W) Register 2 - Polarity inversion register
  PCA9557_R3 = 0x03, // (R/W) Register 3 - Configuration register
};

typedef struct {
  uint8_t r1;
  uint8_t address;
  uint8_t pins;                                        // 8 (PCA9557)
} tPca9557Device;

struct PCA9557 {
  tPca9557Device device[PCA9557_MAX_DEVICES];
  uint32_t relay_inverted;
  uint32_t button_inverted;
  uint8_t chip;
  uint8_t max_devices;
  uint8_t max_pins;
  uint8_t relay_max;
  uint8_t relay_offset;
  uint8_t button_max;
  uint8_t switch_max;
  int8_t button_offset;
  int8_t switch_offset;
  bool base;
} Pca9557;

uint16_t *Pca9557_gpio_pin = nullptr;

/*********************************************************************************************\
 * PCA9557 - I2C
\*********************************************************************************************/

void PCA9557DumpRegs(void) {
  uint8_t data[4];
  for (Pca9557.chip = 0; Pca9557.chip < Pca9557.max_devices; Pca9557.chip++) {
    uint32_t data_size = sizeof(data);
    I2cReadBuffer(Pca9557.device[Pca9557.chip].address, 0, data, data_size);
    AddLog(LOG_LEVEL_DEBUG, PSTR("PCA: Intf %d, Address %02X, Regs %*_H"), Pca9557.device[Pca9557.chip].address, data_size, data);
  }
}

uint32_t PCA9557Read(uint8_t reg) {
  uint32_t value = 0;
  value = I2cRead8(Pca9557.device[Pca9557.chip].address, reg);
  return value;
}

bool PCA9557ValidRead(uint8_t reg, uint8_t *data) {
  return I2cValidRead8(data, Pca9557.device[Pca9557.chip].address, reg);
  return false;
}

void PCA9557Write(uint8_t reg, uint8_t value) {
  I2cWrite8(Pca9557.device[Pca9557.chip].address, reg, value);
}

/*********************************************************************************************/

void PCA9557Update(uint8_t pin, bool pin_value, uint8_t reg_addr) {
  // pin = 0 - 7
  uint8_t bit = pin % 8;
  uint8_t reg_value = 0;
  if (reg_addr == PCA9557_R1) {
    reg_value = Pca9557.device[Pca9557.chip].r1;
  } else {
    reg_value = PCA9557Read(reg_addr);
  }
  if (pin_value) {
    reg_value |= 1 << bit;
  } else {
    reg_value &= ~(1 << bit);
  }
  PCA9557Write(reg_addr, reg_value);
  if (reg_addr == PCA9557_R1) {
    Pca9557.device[Pca9557.chip].r1 = reg_value;
  }
}

/*********************************************************************************************/

uint32_t PCA9557SetChip(uint8_t pin) {
  // Calculate chip based on number of pins per chip. 8 for PCA9557
  // pin 0 - 63
  for (Pca9557.chip = 0; Pca9557.chip < Pca9557.max_devices; Pca9557.chip++) {
    if (Pca9557.device[Pca9557.chip].pins > pin) { break; }
    pin -= Pca9557.device[Pca9557.chip].pins;
  }
  return pin;                                          // relative pin number within chip (0 ... 7)
}

void PCA9557PinMode(uint8_t pin, uint8_t flags) {
  // pin 0 - 63
  pin = PCA9557SetChip(pin);
  uint8_t iodir = PCA9557_R3;
  switch (flags) {
    case INPUT:
      PCA9557Update(pin, true, iodir);
      break;
    case OUTPUT:
      PCA9557Update(pin, false, iodir);
      break;
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: PCA9557PinMode chip %d, pin %d, flags %d, regs %d"), Pca9557.chip, pin, flags, iodir);
}


void PCA9557SetPinModes(uint8_t pin, uint8_t flags) {
  // pin 0 - 63
  PCA9557PinMode(pin, flags);
}

bool PCA9557DigitalRead(uint8_t pin) {
  // pin 0 - 63
  pin = PCA9557SetChip(pin);
  uint8_t bit = pin % 8;
  uint8_t reg_addr = PCA9557_R0;
  uint8_t value = PCA9557Read(reg_addr);
  return value & (1 << bit);
}

void PCA9557DigitalWrite(uint8_t pin, bool value) {
  // pin 0 - 63
  pin = PCA9557SetChip(pin);
  uint8_t reg_addr = PCA9557_R1;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: PCA9557DigitalWrite chip %d, pin %d, state %d, reg %d"), Pca9557.chip, pin, value, reg_addr);

  PCA9557Update(pin, value, reg_addr);
}

/*********************************************************************************************\
 * Tasmota
\*********************************************************************************************/

int PCA9557Pin(uint32_t gpio, uint32_t index = 0);
int PCA9557Pin(uint32_t gpio, uint32_t index) {
  uint16_t real_gpio = gpio << 5;
  uint16_t mask = 0xFFE0;
  if (index < GPIO_ANY) {
    real_gpio += index;
    mask = 0xFFFF;
  }
  for (uint32_t i = 0; i < Pca9557.max_pins; i++) {
    if ((Pca9557_gpio_pin[i] & mask) == real_gpio) {
      return i;                                        // Pin number configured for gpio
    }
  }
  return -1;                                           // No pin used for gpio
}

bool PCA9557PinUsed(uint32_t gpio, uint32_t index = 0);
bool PCA9557PinUsed(uint32_t gpio, uint32_t index) {
  return (PCA9557Pin(gpio, index) >= 0);
}

uint32_t PCA9557GetPin(uint32_t lpin) {
  if (lpin < Pca9557.max_pins) {
    return Pca9557_gpio_pin[lpin];
  } else {
    return GPIO_NONE;
  }
}

/*********************************************************************************************/

String PCA9557TemplateLoadFile(void) {
  String pcatmplt = "";
#ifdef USE_UFILESYS
  pcatmplt = TfsLoadString("/pca9557.dat");
#endif  // USE_UFILESYS
#ifdef USE_RULES
  if (!pcatmplt.length()) {
    pcatmplt = RuleLoadFile("PCA9557.DAT");
  }
#endif  // USE_RULES
#ifdef USE_SCRIPT
  if (!pcatmplt.length()) {
    pcatmplt = ScriptLoadSection(">y");
  }
#endif  // USE_SCRIPT
  return pcatmplt;
}

bool PCA9557LoadTemplate(void) {
  String pcatmplt = PCA9557TemplateLoadFile();
  uint32_t len = pcatmplt.length() +1;
  if (len < 7) { return false; }                       // No PcaTmplt found

  JsonParser parser((char*)pcatmplt.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  // rule3 on file#pca9557.dat do {"NAME":"PCA9557","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231]} endon
  // rule3 on file#pca9557.dat do {"NAME":"PCA9557","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]} endon
  // rule3 on file#pca9557.dat do {"NAME":"PCA9557 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]} endon
  // rule3 on file#pca9557.dat do {"NAME":"PCA9557 A=Ri8-1, B=B1-8, C=Ri16-9, D=B9-16","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39,271,270,269,268,267,266,265,264,40,41,42,43,44,45,46,47]} endon

  // {"NAME":"PCA9557","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231]}
  // {"NAME":"PCA9557","GPIO":[32,33,34,35,36,37,38,39,224,225,226,227,228,229,230,231,40,41,42,43,44,45,46,47,232,233,234,235,236,237,238,239]}
  JsonParserToken val = root[PSTR(D_JSON_BASE)];
  if (val) {
    Pca9557.base = (val.getUInt()) ? true : false;
  }
  val = root[PSTR(D_JSON_NAME)];
  if (val) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("PCA: Base %d, Template '%s'"), Pca9557.base, val.getStr());
  }
  JsonParserArray arr = root[PSTR(D_JSON_GPIO)];
  if (arr) {
    uint32_t pin = 0;
    for (pin; pin < Pca9557.max_pins; pin++) {          // Max number of detected chip pins
      JsonParserToken val = arr[pin];
      if (!val) { break; }
      uint16_t mpin = val.getUInt();
      if (mpin) {                                      // Above GPIO_NONE
        if ((mpin >= AGPIO(GPIO_SWT1_NP)) && (mpin < (AGPIO(GPIO_SWT1_NP) + MAX_SWITCHES_SET))) {
          mpin -= (AGPIO(GPIO_SWT1_NP) - AGPIO(GPIO_SWT1));
          Pca9557.switch_max++;
          PCA9557SetPinModes(pin, INPUT);
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_NP)) && (mpin < (AGPIO(GPIO_KEY1_NP) + MAX_KEYS_SET))) {
          mpin -= (AGPIO(GPIO_KEY1_NP) - AGPIO(GPIO_KEY1));
          Pca9557.button_max++;
          PCA9557SetPinModes(pin, INPUT);
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_INV_NP)) && (mpin < (AGPIO(GPIO_KEY1_INV_NP) + MAX_KEYS_SET))) {
          bitSet(Pca9557.button_inverted, mpin - AGPIO(GPIO_KEY1_INV_NP));
          mpin -= (AGPIO(GPIO_KEY1_INV_NP) - AGPIO(GPIO_KEY1));
          Pca9557.button_max++;
          PCA9557SetPinModes(pin, INPUT);
        }
        else if ((mpin >= AGPIO(GPIO_REL1)) && (mpin < (AGPIO(GPIO_REL1) + MAX_RELAYS_SET))) {
          Pca9557.relay_max++;
          PCA9557PinMode(pin, OUTPUT);
        }
        else if ((mpin >= AGPIO(GPIO_REL1_INV)) && (mpin < (AGPIO(GPIO_REL1_INV) + MAX_RELAYS_SET))) {
          bitSet(Pca9557.relay_inverted, mpin - AGPIO(GPIO_REL1_INV));
          mpin -= (AGPIO(GPIO_REL1_INV) - AGPIO(GPIO_REL1));
          Pca9557.relay_max++;
          PCA9557PinMode(pin, OUTPUT);
        }
        else if (mpin == AGPIO(GPIO_OUTPUT_HI)) {
          PCA9557PinMode(pin, OUTPUT);
          PCA9557DigitalWrite(pin, 1);
        }
        else if (mpin == AGPIO(GPIO_OUTPUT_LO)) {
          PCA9557PinMode(pin, OUTPUT);
          PCA9557DigitalWrite(pin, 0);
        }
        else { mpin = 0; }
        Pca9557_gpio_pin[pin] = mpin;
      }
      if ((Pca9557.switch_max >= MAX_SWITCHES_SET) ||
          (Pca9557.button_max >= MAX_KEYS_SET) ||
          (Pca9557.relay_max >= MAX_RELAYS_SET)) {
        AddLog(LOG_LEVEL_INFO, PSTR("PCA: Max reached (S%d/B%d/R%d)"), Pca9557.switch_max, Pca9557.button_max, Pca9557.relay_max);
        break;
      }
    }
    Pca9557.max_pins = pin;                             // Max number of configured pins
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("PCA: Pins %d, Pca9557_gpio_pin %*_V"), Pca9557.max_pins, Pca9557.max_pins, (uint8_t*)Pca9557_gpio_pin);

  return true;
}

uint32_t PCA9557TemplateGpio(void) {
  String pcatmplt = PCA9557TemplateLoadFile();
  uint32_t len = pcatmplt.length() +1;
  if (len < 7) { return 0; }                           // No PcaTmplt found

  JsonParser parser((char*)pcatmplt.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return 0; }

  JsonParserArray arr = root[PSTR(D_JSON_GPIO)];
  if (arr.isArray()) {
    return arr.size();                                // Number of requested pins
  }
  return 0;
}

void PCA9557ModuleInit(void) {
  int32_t pins_needed = PCA9557TemplateGpio();
  if (!pins_needed) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("PCA: Invalid template"));
    return;
  }


   uint8_t pca9557_address = PCA9557_ADDR_START;
    while ((Pca9557.max_devices < PCA9557_MAX_DEVICES) && (pca9557_address < PCA9557_ADDR_END)) {
      Pca9557.chip = Pca9557.max_devices;
      if (I2cSetDevice(pca9557_address)) {
        Pca9557.device[Pca9557.chip].address = pca9557_address;

        uint8_t buffer;
        if (PCA9557ValidRead(PCA9557_R2, &buffer)) {
          I2cSetActiveFound(pca9557_address, "PCA9557");
          Pca9557.device[Pca9557.chip].pins = 8;
          PCA9557Write(PCA9557_R2, 0b00000000);     // disable polarity inversion
          Pca9557.max_devices++;

          Pca9557.max_pins += Pca9557.device[Pca9557.chip].pins;
          pins_needed -= Pca9557.device[Pca9557.chip].pins;
        }
      }
      if (pins_needed) {
        pca9557_address++;
      } else {
        pca9557_address = PCA9557_ADDR_END;
      }
    }


  if (!Pca9557.max_devices) { return; }

  Pca9557_gpio_pin = (uint16_t*)calloc(Pca9557.max_pins, 2);
  if (!Pca9557_gpio_pin) { return; }

  if (!PCA9557LoadTemplate()) {
    AddLog(LOG_LEVEL_INFO, PSTR("PCA: No valid template found"));  // Too many GPIO's
    Pca9557.max_devices = 0;
    return;
  }

  Pca9557.relay_offset = TasmotaGlobal.devices_present;
  Pca9557.relay_max -= UpdateDevicesPresent(Pca9557.relay_max);

  Pca9557.button_offset = -1;
  Pca9557.switch_offset = -1;
}

void PCA9557ServiceInput(void) {
  // I found no reliable way to receive interrupts; noise received at undefined moments - unstable usage
  // Pca9557.interrupt = false;
  // This works with no interrupt
  uint32_t pin_offset = 0;
  uint32_t gpio;
  for (Pca9557.chip = 0; Pca9557.chip < Pca9557.max_devices; Pca9557.chip++) {
    gpio = PCA9557Read(PCA9557_R0);             // Read PCA9557 gpio

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PCA: Chip %d, State %02X"), Pca9557.chip, gpio);

    uint32_t mask = 1;
    for (uint32_t pin = 0; pin < Pca9557.device[Pca9557.chip].pins; pin++) {
      uint32_t state = ((gpio & mask) != 0);
      uint32_t lpin = PCA9557GetPin(pin_offset + pin);  // 0 for None, 32 for KEY1, 160 for SWT1, 224 for REL1
      uint32_t index = lpin & 0x001F;                  // Max 32 buttons or switches
      lpin = BGPIO(lpin);                              // UserSelectablePins number
      if (GPIO_KEY1 == lpin) {
        ButtonSetVirtualPinState(Pca9557.button_offset + index, (state != bitRead(Pca9557.button_inverted, index)));
      }
      else if (GPIO_SWT1 == lpin) {
        SwitchSetVirtualPinState(Pca9557.switch_offset + index, state);
      }
      mask <<= 1;
    }
    pin_offset += Pca9557.device[Pca9557.chip].pins;
  }
}

void PCA9557Init(void) {
  PCA9557Write(PCA9557_R2, 0b00000000);     // disable polarity inversion
}

void PCA9557Power(void) {
  // XdrvMailbox.index = 32-bit rpower bit mask
  // Use absolute relay indexes unique with main template
  power_t rpower = XdrvMailbox.index;
  uint32_t relay_max = TasmotaGlobal.devices_present;
  if (!Pca9557.base) {
    // Use relative and sequential relay indexes
    rpower >>= Pca9557.relay_offset;
    relay_max = Pca9557.relay_max;
  }
  for (uint32_t index = 0; index < relay_max; index++) {
    power_t state = rpower &1;
    if (PCA9557PinUsed(GPIO_REL1, index)) {
      uint32_t pin = PCA9557Pin(GPIO_REL1, index) & 0x3F;   // Fix possible overflow over 63 gpios
      PCA9557DigitalWrite(pin, bitRead(Pca9557.relay_inverted, index) ? !state : state);
    }
    rpower >>= 1;                                      // Select next power
  }
}


bool PCA9557AddButton(void) {
  // XdrvMailbox.index = button/switch index
  uint32_t index = XdrvMailbox.index;
  if (!Pca9557.base) {
    // Use relative and sequential button indexes
    if (Pca9557.button_offset < 0) { Pca9557.button_offset = index; }
    index -= Pca9557.button_offset;
    if (index >= Pca9557.button_max) { return false; }
  } else {
    // Use absolute button indexes unique with main template
    if (!PCA9557PinUsed(GPIO_KEY1, index)) { return false; }
    Pca9557.button_offset = 0;
  }
  XdrvMailbox.index = (PCA9557DigitalRead(PCA9557Pin(GPIO_KEY1, index)) != bitRead(Pca9557.button_inverted, index));
  return true;
}

bool PCA9557AddSwitch(void) {
  // XdrvMailbox.index = button/switch index
  uint32_t index = XdrvMailbox.index;
  if (!Pca9557.base) {
    // Use relative and sequential switch indexes
    if (Pca9557.switch_offset < 0) { Pca9557.switch_offset = index; }
    index -= Pca9557.switch_offset;
    if (index >= Pca9557.switch_max) { return false; }
  } else {
    // Use absolute switch indexes unique with main template
    if (!PCA9557PinUsed(GPIO_SWT1, index)) { return false; }
    Pca9557.switch_offset = 0;
  }
  XdrvMailbox.index = PCA9557DigitalRead(PCA9557Pin(GPIO_SWT1, index));
  return true;
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv69(uint32_t function) {
  bool i2c_enabled = false;
  i2c_enabled = I2cEnabled(XI2C_81);
  if (!i2c_enabled) { return false; }

  bool result = false;

  if (FUNC_SETUP_RING2 == function) {
    PCA9557ModuleInit();
  } else if (Pca9557.max_devices) {
    switch (function) {
      case FUNC_EVERY_100_MSECOND:
        if (Pca9557.button_max || Pca9557.switch_max) {
          PCA9557ServiceInput();
        }
        break;
      case FUNC_SET_POWER:
        PCA9557Power();
        break;
      case FUNC_INIT:
        PCA9557Init();
        break;
      case FUNC_ADD_BUTTON:
        result = PCA9557AddButton();
        break;
      case FUNC_ADD_SWITCH:
        result = PCA9557AddSwitch();
        break;
    }
  }
  return result;
}

#endif  // USE_PCA9557_DRV
#endif  // USE_I2C
