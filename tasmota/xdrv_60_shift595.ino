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
void (* const Shit595Command[])(void) PROGMEM = { &CmndShift595Devices };

struct Shift595 {
  int8_t pinSRCLK;
  int8_t pinRCLK;
  int8_t pinSER;
  int8_t pinOE;
  bool connected = false;
  int8_t outputs;
  int8_t first = TasmotaGlobal.devices_present;
} Shift595;

void Shift595Init(void)
{
  if (PinUsed(GPIO_SHIFT595_SRCLK) && PinUsed(GPIO_SHIFT595_RCLK) && PinUsed(GPIO_SHIFT595_SER)) {
    Shift595.pinSRCLK = Pin(GPIO_SHIFT595_SRCLK);
    pinMode(Shift595.pinSRCLK, OUTPUT);
    digitalWrite(Shift595.pinSRCLK, 0);

    Shift595.pinRCLK = Pin(GPIO_SHIFT595_RCLK);
    pinMode(Shift595.pinRCLK, OUTPUT);
    digitalWrite(Shift595.pinRCLK, 0);
    
    Shift595.pinSER = Pin(GPIO_SHIFT595_SER);
    pinMode(Shift595.pinSER, OUTPUT);
    digitalWrite(Shift595.pinSER, 0);
    
    if (PinUsed(GPIO_SHIFT595_OE)) {
      Shift595.pinOE = Pin(GPIO_SHIFT595_OE);
      pinMode(Shift595.pinOE, OUTPUT);
      digitalWrite(Shift595.pinOE, 1);     
    }
    Shift595.outputs = Settings->shift595_device_count * 8;
    TasmotaGlobal.devices_present += Shift595.outputs;
    Shift595.connected = true;
    AddLog(LOG_LEVEL_DEBUG, PSTR("595: Controlling relays POWER%d to POWER%d"), Shift595.first + 1, Shift595.outputs);
  }
}

void Shift595LatchPin(uint8 pin) {
  digitalWrite(pin, 1);
  digitalWrite(pin, 0);
}

void Shift595SwitchRelay(void)
{
  if (Shift595.connected == true) {
    for (uint32_t i = 0; i < Shift595.outputs; i++) {
      uint8_t relay_state = bitRead(XdrvMailbox.index, Shift595.first + Shift595.outputs -1 -i);
      // digitalWrite(Shift595.pinSER, Settings->flag5.shift595_invert_outputs ? !relay_state : relay_state);
      digitalWrite(Shift595.pinSER, relay_state);
      Shift595LatchPin(Shift595.pinSRCLK);
    }

    Shift595LatchPin(Shift595.pinRCLK);

    if (PinUsed(GPIO_SHIFT595_OE)) {
        digitalWrite(Shift595.pinOE, 0);     
      }
    }
}

void CmndShift595Devices(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
    Settings->shift595_device_count = (1 == XdrvMailbox.payload) ? SHIFT595_DEVICE_COUNT : XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->shift595_device_count);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv60(uint8_t function)
{
  bool result = false;
  switch (function) {
      case FUNC_PRE_INIT:
        Shift595Init();
        break;
      case FUNC_SET_POWER:
        Shift595SwitchRelay();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kShift595Commands, Shift595Command);
        break;
  return result;
}

#endif  // USE_SHIFT595
