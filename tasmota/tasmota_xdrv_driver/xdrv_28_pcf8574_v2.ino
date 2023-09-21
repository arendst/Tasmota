/*
  xdrv_28_pcf8574.ino - PCF8574 I2C support for Tasmota

  Copyright (C) 2021  Stefan Bode and Theo Arends

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
#ifdef USE_PCF8574
/*********************************************************************************************\
 * PCF8574 - I2C IO Expander
 *
 * I2C Address: PCF8574  = 0x20 .. 0x27 (0x27 is not supported),
 *              PCF8574A = 0x39 .. 0x3F (0x38 is not supported)
 *
 * Start address and count can be overriden in user_config_override.h to allow better
 * sharing of the I2C address space. Still the covered range must remains valid.
 * A count of 0 can be used totaly disable any of the 2 ranges.
 * By default, the following addresses are explicitly excluded (as per the docs) :
 *  - 0x27 and 0x37 are reserved for USE_DISPLAY_LCD in xdsp_01_lcd.ino
 *  - 0X38 is reserved for other sensors
 * If the respective drivers are not used, overrides allows to recover those addresses
 * If defined, USE_MCP230xx_ADDR is also always excluded
 *
 * Mode 1:
 * See documentation.
 *
 * Mode 2:
 * Allows easy configuration by using a sequential list of pins configured as Tasmota
 * template and handle any input and output as configured GPIOs.
 *
 * Restrictions:
 * - Uses incremental I2C addresses until template pin count reached
 * - Max support for 28 switches (input), 32 buttons (input), 32 relays (output)
 *
 * Supported template fields:
 * NAME  - Template name
 * BASE  - Optional. 0 = use relative buttons and switches (default), 1 = use absolute buttons and switches
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
 * - a rule like: rule3 on file#pcf8574.dat do {"NAME":"PCF8574 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]} endon
 * - a script like: >y{"NAME":"PCF8574 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}#
 * - file called pcf8574.dat with contents: {"NAME":"PCF8574 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}
 *
 * Inverted relays and buttons               Ri8 Ri7 Ri6 Ri5 Ri4 Ri3 Ri2 Ri1 B1 B2 B3 B4 B5 B6 B7 B8
 * {"NAME":"PCF8574 A=Ri8-1, B=B1-8","GPIO":[263,262,261,260,259,258,257,256,32,33,34,35,36,37,38,39]}
 *                                                      B1 B2 B3 B4 Ri4 Ri3 Ri2 Ri1 B5 B6 B7 B8 Ri8 Ri7 Ri6 Ri5
 * {"NAME":"PCF8574 A=B1-4,Ri4-1, B=B5-8,Ri8-5","GPIO":[32,33,34,35,259,258,257,256,36,37,38,39,263,262,261,260]}
\*********************************************************************************************/

#define XDRV_28              28
#define XI2C_02              2      // See I2CDEVICES.md

// PCF8574 address range from 0x20 to 0x26
#ifndef PCF8574_ADDR1
#define PCF8574_ADDR1        0x20   // PCF8574
#endif
#ifndef PCF8574_ADDR1_COUNT
#define PCF8574_ADDR1_COUNT  7
#endif
// PCF8574A address range from 0x39 to 0x3E
#ifndef PCF8574_ADDR2
#define PCF8574_ADDR2        0x39   // PCF8574A
#endif
#ifndef PCF8574_ADDR2_COUNT
#define PCF8574_ADDR2_COUNT  6
#endif

//#define USE_PCF8574_MODE2         // Enable Mode2 virtual relays/buttons/switches (+2k3 code)
//#define USE_PCF8574_SENSOR        // Enable Mode1 inputs and outputs in SENSOR message (+0k2 code)
//#define USE_PCF8574_DISPLAYINPUT  // Enable Mode1 inputs display in Web page (+0k2 code)
//#define USE_PCF8574_MQTTINPUT     // Enable Mode1 MQTT message & rule process on input change detection : stat/%topic%/PCF8574_INP = {"Time":"2021-03-07T16:19:23+01:00","PCF8574-1_INP":{"D1":1}} (+0k5 code)

/*********************************************************************************************\
 * PCF8574 support
\*********************************************************************************************/

// Consistency tests - Checked across the complete range for the PCF8574/PCF8574A to allow override
#if (PCF8574_ADDR1 < 0x20) || ((PCF8574_ADDR1 + PCF8574_ADDR1_COUNT - 1) > 0x27)
#error PCF8574_ADDR1 and/or PCF8574_ADDR1_COUNT badly overriden. Fix your user_config_override
#endif
#if (PCF8574_ADDR2 < 0x38) || ((PCF8574_ADDR2 + PCF8574_ADDR2_COUNT - 1) > 0x3F)
#error PCF8574_ADDR2 and/or PCF8574_ADDR2_COUNT badly overriden. Fix your user_config_override.
#endif

struct PCF8574 {
  uint32_t relay_inverted;
  uint32_t button_inverted;
  uint8_t address[MAX_PCF8574];
  uint8_t pin_mask[MAX_PCF8574] = { 0 };
#ifdef USE_PCF8574_MQTTINPUT
  uint8_t last_input[MAX_PCF8574] = { 0 };
#endif
  uint8_t max_connected_ports = 0;        // Max numbers of devices comming from PCF8574 modules
  uint8_t max_devices = 0;                // Max numbers of PCF8574 modules
  uint8_t mode;
  uint8_t relay_max;
  uint8_t relay_offset;
  uint8_t button_max;
  uint8_t switch_max;
  int8_t button_offset;
  int8_t switch_offset;
  bool base;
  bool interrupt;
} Pcf8574;

uint16_t *Pcf8574_pin = nullptr;

/*********************************************************************************************\
 * PCF8574 - I2C
\*********************************************************************************************/

uint8_t Pcf8574Read(uint8_t idx) {
  Wire.requestFrom(Pcf8574.address[idx], (uint8_t)1);
  return Wire.read();
}

void Pcf8574Write(uint8_t idx) {
  Wire.beginTransmission(Pcf8574.address[idx]);
  Wire.write(Pcf8574.pin_mask[idx]);
  Wire.endTransmission();
}

#ifdef USE_PCF8574_MODE2

bool Pcf8574DigitalRead(uint8_t pin) {
  // pin 0 - 63
  uint32_t chip = pin / 8;
  uint32_t bit = pin % 8;
  uint32_t value = Pcf8574Read(chip);
  return value & (1 << bit);
}

void Pcf8574DigitalWrite(uint8_t pin, bool pin_value) {
  // pin 0 - 63
  // INPUT or INPUT_PULLUP = Pcf8574DigitalWrite(pin, 1);
  // OUTPUT = Pcf8574DigitalWrite(pin, 0); or Pcf8574DigitalWrite(pin, 1);
  uint32_t chip = pin / 8;
  uint32_t bit = pin % 8;
  uint32_t value = Pcf8574Read(chip);
  value |= Pcf8574.pin_mask[chip];                     // Restore inputs
  if (pin_value) {
    value |= 1 << bit;
  } else {
    value &= ~(1 << bit);
  }
  Wire.beginTransmission(Pcf8574.address[chip]);
  Wire.write(value);
  Wire.endTransmission();
}

void Pcf8574DigitalWriteConfig(uint8_t pin, bool pin_value) {
  if (pin_value) {
    bitSet(Pcf8574.pin_mask[pin / 8], pin % 8);        // Save inputs
  }
  Pcf8574DigitalWrite(pin, pin_value);
}

/*********************************************************************************************\
 * PCF8574 Mode 2 - Theo Arends
\*********************************************************************************************/

int Pcf8574Pin(uint32_t gpio, uint32_t index = 0);
int Pcf8574Pin(uint32_t gpio, uint32_t index) {
  uint16_t real_gpio = gpio << 5;
  uint16_t mask = 0xFFE0;
  if (index < GPIO_ANY) {
    real_gpio += index;
    mask = 0xFFFF;
  }
  for (uint32_t i = 0; i < Pcf8574.max_connected_ports; i++) {
    if ((Pcf8574_pin[i] & mask) == real_gpio) {
      return i;                                        // Pin number configured for gpio
    }
  }
  return -1;                                           // No pin used for gpio
}

bool Pcf8574PinUsed(uint32_t gpio, uint32_t index = 0);
bool Pcf8574PinUsed(uint32_t gpio, uint32_t index) {
  return (Pcf8574Pin(gpio, index) >= 0);
}

uint32_t Pcf8574GetPin(uint32_t lpin) {
  if (lpin < Pcf8574.max_connected_ports) {
    return Pcf8574_pin[lpin];
  } else {
    return GPIO_NONE;
  }
}

/*********************************************************************************************/

String Pcf8574TemplateLoadFile(void) {
  String pcftmplt = "";
#ifdef USE_UFILESYS
  pcftmplt = TfsLoadString("/pcf8574.dat");
#endif  // USE_UFILESYS
#ifdef USE_RULES
  if (!pcftmplt.length()) {
    pcftmplt = RuleLoadFile("PCF8574.DAT");
  }
#endif  // USE_RULES
#ifdef USE_SCRIPT
  if (!pcftmplt.length()) {
    pcftmplt = ScriptLoadSection(">y");
  }
#endif  // USE_SCRIPT
  return pcftmplt;
}

bool Pcf8574LoadTemplate(void) {
  String pcftmplt = Pcf8574TemplateLoadFile();
  uint32_t len = pcftmplt.length() +1;
  if (len < 7) { return false; }                       // No PcfTmplt found

  JsonParser parser((char*)pcftmplt.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  // rule3 on file#pcf8574.dat do {"NAME":"PCF8574 A=B12345678,B=Ri87654321","GPIO":[32,33,34,35,36,37,38,39,263,262,261,260,259,258,257,256]} endon
  // rule3 on file#pcf8574.dat do {"NAME":"PCF8574 A=B1234,Ri4321,B=B5678,Ri8765","GPIO":[32,33,34,35,259,258,257,256,36,37,38,39,263,262,261,260]} endon
  // rule3 on file#pcf8574.dat do {"NAME":"PCF8574 A=B3456,Ri6543,B=B78910,Ri10987","BASE":1,"GPIO":[34,35,36,37,261,260,259,258,38,39,40,41,265,264,263,262]} endon
  JsonParserToken val = root[PSTR(D_JSON_BASE)];
  if (val) {
    Pcf8574.base = (val.getUInt()) ? true : false;
  }
  val = root[PSTR(D_JSON_NAME)];
  if (val) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: Base %d, Template '%s'"), Pcf8574.base, val.getStr());
  }
  JsonParserArray arr = root[PSTR(D_JSON_GPIO)];
  if (arr) {
    uint32_t pin = 0;
    for (pin; pin < Pcf8574.max_connected_ports; pin++) {  // Max number of detected chip pins
//      if (0 == (pin % 8)) { Pcf8574.pin_mask[pin / 8] = 0; }  // Reset config for next 8-pins
      JsonParserToken val = arr[pin];
      if (!val) { break; }
      uint16_t mpin = val.getUInt();
      if (mpin) {                                      // Above GPIO_NONE
        if ((mpin >= AGPIO(GPIO_SWT1)) && (mpin < (AGPIO(GPIO_SWT1) + MAX_SWITCHES_SET))) {
          Pcf8574.switch_max++;
          Pcf8574DigitalWriteConfig(pin, 1);           // INPUT_PULLUP
        }
        else if ((mpin >= AGPIO(GPIO_SWT1_NP)) && (mpin < (AGPIO(GPIO_SWT1_NP) + MAX_SWITCHES_SET))) {
          mpin -= (AGPIO(GPIO_SWT1_NP) - AGPIO(GPIO_SWT1));
          Pcf8574.switch_max++;
          Pcf8574DigitalWriteConfig(pin, 1);           // INPUT
        }
        else if ((mpin >= AGPIO(GPIO_KEY1)) && (mpin < (AGPIO(GPIO_KEY1) + MAX_KEYS_SET))) {
          Pcf8574.button_max++;
          Pcf8574DigitalWriteConfig(pin, 1);           // INPUT_PULLUP
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_NP)) && (mpin < (AGPIO(GPIO_KEY1_NP) + MAX_KEYS_SET))) {
          mpin -= (AGPIO(GPIO_KEY1_NP) - AGPIO(GPIO_KEY1));
          Pcf8574.button_max++;
          Pcf8574DigitalWriteConfig(pin, 1);           // INPUT
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_INV)) && (mpin < (AGPIO(GPIO_KEY1_INV) + MAX_KEYS_SET))) {
          bitSet(Pcf8574.button_inverted, mpin - AGPIO(GPIO_KEY1_INV));
          mpin -= (AGPIO(GPIO_KEY1_INV) - AGPIO(GPIO_KEY1));
          Pcf8574.button_max++;
          Pcf8574DigitalWriteConfig(pin, 1);           // INPUT_PULLUP
        }
        else if ((mpin >= AGPIO(GPIO_KEY1_INV_NP)) && (mpin < (AGPIO(GPIO_KEY1_INV_NP) + MAX_KEYS_SET))) {
          bitSet(Pcf8574.button_inverted, mpin - AGPIO(GPIO_KEY1_INV_NP));
          mpin -= (AGPIO(GPIO_KEY1_INV_NP) - AGPIO(GPIO_KEY1));
          Pcf8574.button_max++;
          Pcf8574DigitalWriteConfig(pin, 1);           // INPUT
        }
        else if ((mpin >= AGPIO(GPIO_REL1)) && (mpin < (AGPIO(GPIO_REL1) + MAX_RELAYS_SET))) {
          Pcf8574.relay_max++;
//          Pcf8574DigitalWriteConfig(pin, 1);           // OUTPUT - Leave unchanged to fix restart and power on spikes (default is 1)
        }
        else if ((mpin >= AGPIO(GPIO_REL1_INV)) && (mpin < (AGPIO(GPIO_REL1_INV) + MAX_RELAYS_SET))) {
          bitSet(Pcf8574.relay_inverted, mpin - AGPIO(GPIO_REL1_INV));
          mpin -= (AGPIO(GPIO_REL1_INV) - AGPIO(GPIO_REL1));
          Pcf8574.relay_max++;
//          Pcf8574DigitalWriteConfig(pin, 1);           // OUTPUT - Leave unchanged to fix restart and power on spikes (default is 1)
        }
        else if (mpin == AGPIO(GPIO_OUTPUT_HI)) {
          Pcf8574DigitalWriteConfig(pin, 1);           // OUTPUT
        }
        else if (mpin == AGPIO(GPIO_OUTPUT_LO)) {
          Pcf8574DigitalWriteConfig(pin, 0);           // OUTPUT
        }
        else { mpin = 0; }
        Pcf8574_pin[pin] = mpin;
      }
      if ((Pcf8574.switch_max >= MAX_SWITCHES_SET) ||
          (Pcf8574.button_max >= MAX_KEYS_SET) ||
          (Pcf8574.relay_max >= MAX_RELAYS_SET)) {
        AddLog(LOG_LEVEL_INFO, PSTR("PCF: Max reached (S%d/B%d/R%d)"), Pcf8574.switch_max, Pcf8574.button_max, Pcf8574.relay_max);
        break;
      }
    }
    Pcf8574.max_connected_ports = pin;                 // Max number of configured pins
  }
  return true;
}

void Pcf8574ServiceInput(void) {
  Pcf8574.interrupt = false;
  // This works with no interrupt too
  uint32_t pin_offset = 0;
  for (uint32_t chip = 0; chip < Pcf8574.max_devices; chip++) {
    uint32_t gpio = Pcf8574Read(chip);                 // Reset interrupt
    uint32_t mask = 1;
    for (uint32_t pin = 0; pin < 8; pin++) {
      uint32_t state = ((gpio & mask) != 0);
      uint32_t lpin = Pcf8574GetPin(pin_offset + pin);  // 0 for None, 32 for KEY1, 160 for SWT1, 224 for REL1
      uint32_t index = lpin & 0x001F;                  // Max 32 buttons or switches
      lpin = BGPIO(lpin);                              // UserSelectablePins number
      if (GPIO_KEY1 == lpin) {
        ButtonSetVirtualPinState(Pcf8574.button_offset + index, (state != bitRead(Pcf8574.button_inverted, index)));
//        Pcf8574DigitalWrite(pin_offset + pin, 1);      // INPUT and reset interrupt
      }
      else if (GPIO_SWT1 == lpin) {
        SwitchSetVirtualPinState(Pcf8574.switch_offset + index, state);
//        Pcf8574DigitalWrite(pin_offset + pin, 1);      // INPUT and reset interrupt
      }
      mask <<= 1;
    }
    pin_offset += 8;
  }
}

static void IRAM_ATTR Pcf8574InputIsr(void) {
  Pcf8574.interrupt = true;
}

void Pcf8574Init(void) {
  if (Pcf8574.button_max || Pcf8574.switch_max) {
    if (PinUsed(GPIO_PCF8574_INT)) {
      pinMode(Pin(GPIO_PCF8574_INT), INPUT_PULLUP);
      attachInterrupt(Pin(GPIO_PCF8574_INT), Pcf8574InputIsr, CHANGE);
    }
  }
}

void Pcf8574Power(void) {
  // XdrvMailbox.index = 32-bit rpower bit mask
  // Use absolute relay indexes unique with main template
  power_t rpower = XdrvMailbox.index;
  uint32_t relay_max = TasmotaGlobal.devices_present;
  if (!Pcf8574.base) {
    // Use relative and sequential relay indexes
    rpower >>= Pcf8574.relay_offset;
    relay_max = Pcf8574.relay_max;
  }
  for (uint32_t index = 0; index < relay_max; index++) {
    power_t state = rpower &1;
    if (Pcf8574PinUsed(GPIO_REL1, index)) {
      uint32_t pin = Pcf8574Pin(GPIO_REL1, index) & 0x3F;   // Fix possible overflow over 63 gpios
      Pcf8574DigitalWrite(pin, bitRead(Pcf8574.relay_inverted, index) ? !state : state);
    }
    rpower >>= 1;                                      // Select next power
  }
}

bool Pcf8574AddButton(void) {
  // XdrvMailbox.index = button/switch index
  uint32_t index = XdrvMailbox.index;
  if (!Pcf8574.base) {
    // Use relative and sequential button indexes
    if (Pcf8574.button_offset < 0) { Pcf8574.button_offset = index; }
    index -= Pcf8574.button_offset;
    if (index >= Pcf8574.button_max) { return false; }
  } else {
    // Use absolute button indexes unique with main template
    if (!Pcf8574PinUsed(GPIO_KEY1, index)) { return false; }
    Pcf8574.button_offset = 0;
  }
  XdrvMailbox.index = (Pcf8574DigitalRead(Pcf8574Pin(GPIO_KEY1, index)) != bitRead(Pcf8574.button_inverted, index));
  return true;
}

bool Pcf8574AddSwitch(void) {
  // XdrvMailbox.index = button/switch index
  uint32_t index = XdrvMailbox.index;
  if (!Pcf8574.base) {
    // Use relative and sequential switch indexes
    if (Pcf8574.switch_offset < 0) { Pcf8574.switch_offset = index; }
    index -= Pcf8574.switch_offset;
    if (index >= Pcf8574.switch_max) { return false; }
  } else {
    // Use absolute switch indexes unique with main template
    if (!Pcf8574PinUsed(GPIO_SWT1, index)) { return false; }
    Pcf8574.switch_offset = 0;
  }
  XdrvMailbox.index = Pcf8574DigitalRead(Pcf8574Pin(GPIO_SWT1, index));
  return true;
}

#endif  // USE_PCF8574_MODE2

/*********************************************************************************************\
 * PCF8574 Mode 1 - Stefan Bode
\*********************************************************************************************/

void Pcf8574SwitchRelay(void) {
  uint32_t devices_present = TasmotaGlobal.devices_present - Pcf8574.relay_offset;
  for (uint32_t i = 0; i < devices_present; i++) {
    uint8_t relay_state = bitRead(XdrvMailbox.index, Pcf8574.relay_offset + i);

    if (Pcf8574.max_devices > 0 && Pcf8574_pin[i] < 99) {
      uint8_t board = Pcf8574_pin[i]>>3;
      uint8_t pin = Pcf8574_pin[i]&0x7;
      uint8_t oldpinmask = Pcf8574.pin_mask[board];
      uint8_t _val = bitRead(TasmotaGlobal.rel_inverted, Pcf8574.relay_offset + i) ? !relay_state : relay_state;

      //AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: SwitchRelay %d=%d => PCF-%d.D%d=%d"), i, relay_state, board +1, pin, _val);
      bitWrite(Pcf8574.pin_mask[board], pin, _val);
      if (oldpinmask != Pcf8574.pin_mask[board]) {
        Pcf8574Write(board);
      }
      //else AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: SwitchRelay skipped"));
    }
  }
}

#ifdef USE_PCF8574_MQTTINPUT
void Pcf8574CheckForInputChange(void) {
  for (uint32_t idx = 0; idx < Pcf8574.max_devices; idx++) {
    uint8_t input_mask = ~Settings->pcf8574_config[idx];  // Invert to 1 = input
    uint8_t input = Pcf8574Read(idx) & input_mask;
    uint8_t last_input = Pcf8574.last_input[idx];
    if (input != last_input) {                         // Don't scan bits if no change (EVERY_50_MS !)
      for (uint32_t pin = 0 ; pin < 8 ; ++pin) {
        if (bitRead(input_mask,pin) && bitRead(input,pin) != bitRead(last_input,pin)) {
          ResponseTime_P(PSTR(",\"PCF8574%c%d_INP\":{\"D%i\":%i}}"), IndexSeparator(), idx +1, pin, bitRead(input,pin));
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("PCF8574_INP"));
          if (Settings->flag3.hass_tele_on_power) {    // SetOption59 - Send tele/%topic%/SENSOR in addition to stat/%topic%/RESULT
            MqttPublishSensor();
          }
        }
        Pcf8574.last_input[idx] =  input;
      }
    }
  }
}
#endif  // USE_PCF8574_MQTTINPUT


void Pcf8574ModuleInitMode1(void) {
  for (uint32_t i = 0; i < Pcf8574.max_connected_ports; i++) {
    Pcf8574_pin[i] = 99;
  }
  Pcf8574.max_connected_ports = 0;                     // Reset no of devices

  for (uint32_t idx = 0; idx < Pcf8574.max_devices; idx++) { // suport up to 8 boards PCF8574
    uint8_t gpio = Pcf8574Read(idx);
    // Insure the input pins are actually writen a 1 for proper input operation
    Pcf8574.pin_mask[idx] = gpio | ~Settings->pcf8574_config[idx];
    Pcf8574Write(idx);                                 // Write back to the register
#ifdef USE_PCF8574_MQTTINPUT
    Pcf8574.last_input[idx] = gpio & ~Settings->pcf8574_config[idx];
#endif  // USE_PCF8574_MQTTINPUT
    //AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: PCF-%d config=0x%02x, gpio=0x%02X"), idx +1, Settings->pcf8574_config[idx], gpio);

    for (uint32_t i = 0; i < 8; i++, gpio>>=1) {
      uint8_t _result = Settings->pcf8574_config[idx] >> i &1;
      uint32_t devices_present = TasmotaGlobal.devices_present - Pcf8574.relay_offset;
      //AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: I2C shift i %d: %d. Powerstate: %d, devices_present: %d"), i,_result, Settings->power>>i&1, devices_present);
      if (_result > 0) {
        Pcf8574_pin[devices_present] = i + 8 * idx;
        bitWrite(TasmotaGlobal.rel_inverted, TasmotaGlobal.devices_present, Settings->flag3.pcf8574_ports_inverted);  // SetOption81 - Invert all ports on PCF8574 devices
        if (!Settings->flag.save_state && !Settings->flag3.no_power_feedback) {  // SetOption63 - Don't scan relay power state at restart - #5594 and #5663
          //AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: Set power from from chip state"));
          uint8_t power_state = Settings->flag3.pcf8574_ports_inverted ? 1 & ~gpio : 1 & gpio;
          bitWrite(TasmotaGlobal.power, TasmotaGlobal.devices_present, power_state);
          bitWrite(Settings->power, TasmotaGlobal.devices_present, power_state);
        }
        //else AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: DON'T set power from chip state"));
        UpdateDevicesPresent(1);
        Pcf8574.max_connected_ports++;
      }
    }
  }
  //AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: Settings->power=0x%08X, TasmotaGlobal.power=0x%08X"), Settings->power, TasmotaGlobal.power);
  AddLog(LOG_LEVEL_INFO, PSTR("PCF: Total devices %d, PCF8574 output ports %d"), Pcf8574.max_devices, Pcf8574.max_connected_ports);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
#define WEB_HANDLE_PCF8574 "pcf"

const char HTTP_BTN_MENU_PCF8574[] PROGMEM =
  "<p><form action='" WEB_HANDLE_PCF8574 "' method='get'><button>" D_CONFIGURE_PCF8574 "</button></form></p>";

const char HTTP_FORM_I2C_PCF8574_1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_PCF8574_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='" WEB_HANDLE_PCF8574 "'>"
  "<p><label><input id='b1' name='b1' type='checkbox'%s><b>" D_INVERT_PORTS "</b></label></p><hr/>";

const char HTTP_FORM_I2C_PCF8574_2[] PROGMEM =
  "<tr><td><b>" D_DEVICE " %d " D_PORT " %d</b></td><td style='width:100px'><select id='i2cs%d' name='i2cs%d'>"
  "<option%s value='0'>" D_DEVICE_INPUT "</option>"
  "<option%s value='1'>" D_DEVICE_OUTPUT "</option>"
  "</select></td></tr>";

const char HTTP_SNS_PCF8574_GPIO[] PROGMEM = "{s}PCF8574%c%d D%d{m}%d{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

void HandlePcf8574(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP  D_CONFIGURE_PCF8574));

  if (Webserver->hasArg("save")) {
    Pcf8574SaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(D_CONFIGURE_PCF8574);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_I2C_PCF8574_1, (Settings->flag3.pcf8574_ports_inverted) ? PSTR(" checked") : "");  // SetOption81 - Invert all ports on PCF8574 devices
  WSContentSend_P(HTTP_TABLE100);
  for (uint32_t idx = 0; idx < Pcf8574.max_devices; idx++) {
    for (uint32_t idx2 = 0; idx2 < 8; idx2++) {        // 8 ports on PCF8574
      uint8_t helper = 1 << idx2;
      WSContentSend_P(HTTP_FORM_I2C_PCF8574_2,
        idx +1, idx2,
        idx2 + 8*idx,
        idx2 + 8*idx,
        ((helper & Settings->pcf8574_config[idx]) >> idx2 == 0) ? PSTR(" selected ") : " ",
        ((helper & Settings->pcf8574_config[idx]) >> idx2 == 1) ? PSTR(" selected ") : " "
      );
    }
  }
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void Pcf8574SaveSettings(void) {
  char stemp[7];
  char tmp[100];

  //AddLog(LOG_LEVEL_DEBUG, PSTR("PCF: Start working on Save arguements: inverted:%d")), Webserver->hasArg("b1");

  Settings->flag3.pcf8574_ports_inverted = Webserver->hasArg("b1");  // SetOption81 - Invert all ports on PCF8574 devices
  for (byte idx = 0; idx < Pcf8574.max_devices; idx++) {
    byte count=0;
    byte n = Settings->pcf8574_config[idx];
    while(n!=0) {
      n = n&(n-1);
      count++;
    }
    uint32_t devices_present = TasmotaGlobal.devices_present - Pcf8574.relay_offset;
    if (count <= devices_present) {
      UpdateDevicesPresent(-count);
    }
    for (byte i = 0; i < 8; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("i2cs%d"), i+8*idx);
      WebGetArg(stemp, tmp, sizeof(tmp));
      byte _value = (!strlen(tmp)) ?  0 : atoi(tmp);
      if (_value) {
        Settings->pcf8574_config[idx] = Settings->pcf8574_config[idx] | 1 << i;
        UpdateDevicesPresent(1);
        Pcf8574.max_connected_ports++;
      } else {
        Settings->pcf8574_config[idx] = Settings->pcf8574_config[idx] & ~(1 << i );
      }
    }
    //Settings->pcf8574_config[0] = (!strlen(webServer->arg("i2cs0").c_str())) ?  0 : atoi(webServer->arg("i2cs0").c_str());
    //AddLog(LOG_LEVEL_INFO, PSTR("PCF: I2C Board: %d, Config: %2x")),  idx, Settings->pcf8574_config[idx];
  }
}

#ifdef USE_PCF8574_DISPLAYINPUT
void Pcf8574ShowWeb(void) {
  for (uint32_t idx = 0 ; idx < Pcf8574.max_devices ; idx++) {
    uint8_t input_mask = ~Settings->pcf8574_config[idx];  // Invert to 1 = input
    uint8_t gpio = Pcf8574Read(idx);
    for (uint32_t pin = 0 ; pin < 8 ; ++pin, input_mask >>= 1, gpio >>= 1) {
      if (input_mask & 1) {
        WSContentSend_P(HTTP_SNS_PCF8574_GPIO, IndexSeparator(), idx +1, pin, gpio & 1);
      }
    }
  }
}
#endif // USE_PCF8574_DISPLAYINPUT
#endif // USE_WEBSERVER

#ifdef USE_PCF8574_SENSOR
void Pcf8574ShowJson(void) {
  for (uint32_t idx = 0 ; idx < Pcf8574.max_devices ; idx++) {
    uint8_t gpio = Pcf8574Read(idx);
    ResponseAppend_P(PSTR(",\"PCF8574%c%d\":{"), IndexSeparator(), idx +1);
    for (uint32_t pin = 0; pin < 8; ++pin, gpio >>= 1) {
      ResponseAppend_P(PSTR("%s\"D%d\":%i"), (0==pin)?"":",", pin, gpio & 1);
    }
    ResponseJsonEnd();
  }
}
#endif // #ifdef USE_PCF8574_SENSOR

/*********************************************************************************************\
 * PCF8574 Module Init
\*********************************************************************************************/

void Pcf8574ModuleInit(void) {
  uint8_t pcf8574_address = (PCF8574_ADDR1_COUNT > 0) ? PCF8574_ADDR1 : PCF8574_ADDR2;
  while ((Pcf8574.max_devices < MAX_PCF8574) && (pcf8574_address < PCF8574_ADDR2 +PCF8574_ADDR2_COUNT)) {

#if defined(USE_MCP230xx) && defined(USE_MCP230xx_ADDR)
    if (USE_MCP230xx_ADDR == pcf8574_address) {
      AddLog(LOG_LEVEL_INFO, PSTR("PCF: Address 0x%02x reserved for MCP230xx"), pcf8574_address);
    } else {
#endif

      if (I2cSetDevice(pcf8574_address)) {
        Pcf8574.mode = 1;

        Pcf8574.max_connected_ports += 8;
        Pcf8574.address[Pcf8574.max_devices] = pcf8574_address;
        Pcf8574.max_devices++;

        char stype[12];
        sprintf_P(stype, PSTR("PCF8574%s"), (pcf8574_address >= PCF8574_ADDR2) ? "A" : "");
        I2cSetActiveFound(pcf8574_address, stype);
      }

#if defined(USE_MCP230xx) && defined(USE_MCP230xx_ADDR)
    }
#endif

    pcf8574_address++;
    if ((PCF8574_ADDR1 +PCF8574_ADDR1_COUNT) == pcf8574_address) {  // Support I2C addresses 0x20 to 0x26 and 0x39 to 0x3F
      pcf8574_address = PCF8574_ADDR2;
    }
  }

  if (Pcf8574.mode) {
    Pcf8574_pin = (uint16_t*)malloc(Pcf8574.max_connected_ports * sizeof(uint16_t));
    if (Pcf8574_pin) {
      Pcf8574.relay_offset = TasmotaGlobal.devices_present;
#ifdef USE_PCF8574_MODE2
      if (Pcf8574LoadTemplate()) {
        Pcf8574.mode = 2;
        Pcf8574.button_offset = -1;
        Pcf8574.switch_offset = -1;
        Pcf8574.relay_max -= UpdateDevicesPresent(Pcf8574.relay_max);
      } else
#endif  // USE_PCF8574_MODE2
        Pcf8574ModuleInitMode1();
    } else {
      Pcf8574.mode = 0;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv28(uint32_t function) {
  if (!I2cEnabled(XI2C_02)) { return false; }

  bool result = false;

  if (FUNC_SETUP_RING2 == function) {
    Pcf8574ModuleInit();
  } else if (1 == Pcf8574.mode) {
    switch (function) {
#ifdef USE_PCF8574_MQTTINPUT
      case FUNC_EVERY_50_MSECOND:
        Pcf8574CheckForInputChange();
        break;
#endif  // USE_PCF8574_MQTTINPUT
      case FUNC_SET_POWER:
        Pcf8574SwitchRelay();
        break;
#ifdef USE_PCF8574_SENSOR
      case FUNC_JSON_APPEND:
        Pcf8574ShowJson();
        break;
#endif  // USE_PCF8574_SENSOR
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_PCF8574);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/" WEB_HANDLE_PCF8574), HandlePcf8574);
        break;
#ifdef USE_PCF8574_DISPLAYINPUT
      case FUNC_WEB_SENSOR:
        Pcf8574ShowWeb();
        break;
#endif  // USE_PCF8574_DISPLAYINPUT
#endif  // USE_WEBSERVER
    }
#ifdef USE_PCF8574_MODE2
  } else if (2 == Pcf8574.mode) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        if (!Pcf8574.interrupt) { return false; }
//        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PCF: Interrupt"));
        Pcf8574ServiceInput();
        break;
      case FUNC_EVERY_100_MSECOND:
        if (Pcf8574.button_max || Pcf8574.switch_max) {
          Pcf8574ServiceInput();
        }
        break;
      case FUNC_SET_POWER:
        Pcf8574Power();
        break;
      case FUNC_INIT:
        Pcf8574Init();
        break;
      case FUNC_ADD_BUTTON:
        result = Pcf8574AddButton();
        break;
      case FUNC_ADD_SWITCH:
        result = Pcf8574AddSwitch();
        break;
    }
#endif  // USE_PCF8574_MODE2
  }
  return result;
}

#endif  // USE_PCF8574
#endif  // USE_I2C
