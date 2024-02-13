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
  bool connected = false;
} *Shift595 = nullptr;

void Shift595ConfigurePin(uint8_t pin, uint8_t value = 0) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

void Shift595Init(void) {
  if (PinUsed(GPIO_SHIFT595_SRCLK) && PinUsed(GPIO_SHIFT595_RCLK) && PinUsed(GPIO_SHIFT595_SER)) {
    Shift595 = (struct Shift595*)calloc(1, sizeof(struct Shift595));
    if (Shift595) {
      Shift595->pinSRCLK = Pin(GPIO_SHIFT595_SRCLK);
      Shift595->pinRCLK = Pin(GPIO_SHIFT595_RCLK);
      Shift595->pinSER = Pin(GPIO_SHIFT595_SER);

      Shift595ConfigurePin(Shift595->pinSRCLK);
      Shift595ConfigurePin(Shift595->pinRCLK);
      Shift595ConfigurePin(Shift595->pinSER);

      if (PinUsed(GPIO_SHIFT595_OE)) {
        Shift595->pinOE = Pin(GPIO_SHIFT595_OE);
        Shift595ConfigurePin(Shift595->pinOE, 1);
      }

      Shift595->first = TasmotaGlobal.devices_present;
      Shift595->outputs = Settings->shift595_device_count * 8;
      UpdateDevicesPresent(Shift595->outputs);

      Shift595->connected = true;
      AddLog(LOG_LEVEL_DEBUG, PSTR("595: Controlling relays POWER%d to POWER%d"), Shift595->first + 1, Shift595->first + Shift595->outputs);
    }
  }
}

void Shift595LatchPin(uint8_t pin) {
  digitalWrite(pin, 1);
  digitalWrite(pin, 0);
}

void Shift595SwitchRelay(void) {
  if (Shift595 && Shift595->connected == true) {
    for (uint32_t i = 0; i < Shift595->outputs; i++) {
      uint8_t relay_state = bitRead(XdrvMailbox.index, Shift595->first + Shift595->outputs -1 -i);
      digitalWrite(Shift595->pinSER, Settings->flag5.shift595_invert_outputs ? !relay_state : relay_state);
      Shift595LatchPin(Shift595->pinSRCLK);
    }

    Shift595LatchPin(Shift595->pinRCLK);

    if (PinUsed(GPIO_SHIFT595_OE)) {
        digitalWrite(Shift595->pinOE, 0);
    }
  }
}

void CmndShift595Devices(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 3)) {
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

  if (FUNC_PRE_INIT == function) {
    Shift595Init();
  } else if (Shift595) {
    switch (function) {
      case FUNC_SET_POWER:
        Shift595SwitchRelay();
        break;
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
