/*
  xdrv_60_shift595.ino - 74x595 shift register family support for Tasmota

  Copyright (C) 2021  Jacek Ziółkowski

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

#ifdef USE_SHIFT595

#define XDRV_60           60

const char kShift595Commands[] PROGMEM = "|" D_CMND_SHIFT595_DEVICE_COUNT ;
void (* const Shift595Command[])(void) PROGMEM = { &CmndShift595Devices };

struct Shift595 {
  uint8_t pinSRCLK;
  uint8_t pinRCLK;
  uint8_t pinSER;
  uint8_t pinOE;
  uint8_t outputs;
  uint8_t first;
} *Shift595 = nullptr;

/*********************************************************************************************\
 * Low level Shift 74x595
\*********************************************************************************************/

void Shift595ConfigurePin(uint8_t pin, uint8_t value = 0) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

void Shift595LatchPin(uint8_t pin) {
  digitalWrite(pin, 1);
  digitalWrite(pin, 0);
}

/*********************************************************************************************\
 * FUNC_SETUP_RING2 at T +1
 * Claim devices_present
\*********************************************************************************************/

void Shift595ModuleInit(void) {
  if (PinUsed(GPIO_SHIFT595_SRCLK) && PinUsed(GPIO_SHIFT595_RCLK) && PinUsed(GPIO_SHIFT595_SER)) {
    Shift595 = (struct Shift595*)calloc(1, sizeof(struct Shift595));
    if (Shift595) {
      Shift595->pinSRCLK = Pin(GPIO_SHIFT595_SRCLK);
      Shift595->pinRCLK = Pin(GPIO_SHIFT595_RCLK);
      Shift595->pinSER = Pin(GPIO_SHIFT595_SER);

      Shift595ConfigurePin(Shift595->pinSRCLK);
      Shift595ConfigurePin(Shift595->pinRCLK);
      Shift595ConfigurePin(Shift595->pinSER);
#ifdef ESP32
      // Release hold on clocks (if set before restart)
      gpio_hold_dis((gpio_num_t)Shift595->pinSRCLK);
      gpio_hold_dis((gpio_num_t)Shift595->pinRCLK);
#endif

      if (PinUsed(GPIO_SHIFT595_OE)) {
        Shift595->pinOE = Pin(GPIO_SHIFT595_OE);
        // Fix relay toggle at restart
        // On power ON set all outputs to 3-state (3-state converted to OFF by ULN2803 relay drivers)
        Shift595ConfigurePin(Shift595->pinOE, ResetReasonPowerOn());
      }

      Shift595->first = TasmotaGlobal.devices_present; // devices_present offset
      Shift595->outputs = Settings->shift595_device_count * 8;  // Max number of outputs present
      UpdateDevicesPresent(Shift595->outputs);
    }
  }
}

/*********************************************************************************************\
 * FUNC_SET_POWER at T +2
 * Reduce devices_present with display and/or lights not known before
 * Add offset for previous defined relays
\*********************************************************************************************/

void Shift595SwitchRelay(void) {
  // XdrvMailbox.index = 32-bit rpower bit mask
  // Use relative and sequential relay indexes
  power_t rpower = XdrvMailbox.index;
  uint32_t relay_max = Shift595->outputs;              // Total number of outputs
  DevicesPresentNonDisplayOrLight(relay_max);          // Skip display and/or light(s)
  uint32_t relay_offset = Shift595->outputs - relay_max + Shift595->first;

  static bool first = false;
  if (!first) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("595: Output 1 to %d use POWER%d to POWER%d"), Shift595->outputs - relay_offset, Shift595->first + 1, relay_max);
    first = true;
  }

  uint32_t power_bit = relay_max -1;                   // Start at highest non display and/or light power bit
  for (uint32_t i = 0; i < Shift595->outputs; i++) {   // We need to set all shift outputs even if not used
    uint32_t relay_state = 0;                          // Unused state
    if (i >= relay_offset) {
      relay_state = bitRead(rpower, power_bit);        // Shift-in from high to low
      power_bit--;
    }
    digitalWrite(Shift595->pinSER, Settings->flag5.shift595_invert_outputs ? !relay_state : relay_state);  // SetOption133 - (Shift595) Invert outputs of 74x595 shift registers
    Shift595LatchPin(Shift595->pinSRCLK);
  }

  Shift595LatchPin(Shift595->pinRCLK);

  if (PinUsed(GPIO_SHIFT595_OE)) {
    digitalWrite(Shift595->pinOE, 0);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndShift595Devices(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload * 8 <= MAX_RELAYS_SET - Shift595->first)) {
    Settings->shift595_device_count = (1 == XdrvMailbox.payload) ? SHIFT595_DEVICE_COUNT : XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->shift595_device_count);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv60(uint32_t function) {
  bool result = false;

  if (FUNC_SETUP_RING2 == function) {
    Shift595ModuleInit();
  } else if (Shift595) {
    switch (function) {
      case FUNC_SET_POWER:
        Shift595SwitchRelay();
        break;
#ifdef ESP32
      case FUNC_SAVE_BEFORE_RESTART:
        // Set hold on clocks to disable relay click on restart
        gpio_hold_en((gpio_num_t)Shift595->pinSRCLK);
        gpio_hold_en((gpio_num_t)Shift595->pinRCLK);
        break;
#endif  // ESP32
      case FUNC_COMMAND:
        result = DecodeCommand(kShift595Commands, Shift595Command);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SHIFT595
