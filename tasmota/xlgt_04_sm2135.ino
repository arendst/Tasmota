/*
  xlgt_04_sm2135.ino - sm2135 five channel led support for Tasmota

  Copyright (C) 2021  Theo Arends and CrudelisPL

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

#ifdef USE_LIGHT
#ifdef USE_SM2135
/*********************************************************************************************\
 * SM2135 RGBCW Led bulbs like some Action LSC SmartLed or Polux
 *
 * Action LSC SmartLed (GreenRedBlue)
 * {"NAME":"LSC RGBCW LED","GPIO":[0,0,0,0,0,0,0,0,181,0,180,0,0],"FLAG":0,"BASE":18}
 * {"NAME":"LSC RGBCW LED","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
 * Polux E14 (BlueGreenRed) - Notice GPIO00 = 9 (Switch1)
 * {"NAME":"Polux RGBCW E14","GPIO":[9,0,0,0,0,0,0,0,181,0,180,0,0],"FLAG":0,"BASE":18}
 * Polux E14 (BlueGreenRed)
 * {"NAME":"Polux RGBCW E14","GPIO":[0,0,0,0,0,0,0,0,4065,0,4032,0,0,0],"FLAG":0,"BASE":18}
 * LE LampUX 907001-US
 * {"NAME":"LE LampUX 907001-US","GPIO":[0,0,0,0,0,0,0,0,4066,0,4032,0,0,0],"FLAG":0,"BASE":18}
\*********************************************************************************************/

#define XLGT_04             4

#define SM2135_ADDR_MC      0xC0  // Max current register
#define SM2135_ADDR_CH      0xC1  // RGB or CW channel select register
#define SM2135_ADDR_R       0xC2  // Red color
#define SM2135_ADDR_G       0xC3  // Green color
#define SM2135_ADDR_B       0xC4  // Blue color
#define SM2135_ADDR_C       0xC5  // Cold
#define SM2135_ADDR_W       0xC6  // Warm

#define SM2135_RGB          0x00  // RGB channel
#define SM2135_CW           0x80  // CW channel (Chip default)

#define SM2135_10MA         0x00
#define SM2135_15MA         0x01
#define SM2135_20MA         0x02  // RGB max current (Chip default)
#define SM2135_25MA         0x03
#define SM2135_30MA         0x04  // CW max current (Chip default)
#define SM2135_35MA         0x05
#define SM2135_40MA         0x06
#define SM2135_45MA         0x07  // Max value for RGB
#define SM2135_50MA         0x08
#define SM2135_55MA         0x09
#define SM2135_60MA         0x0A

enum Sm2135Color { SM2135_WCGRB, SM2135_WCBGR, SM2135_WCGRBHI, SM2135_WCBGRHI, SM2135_WCGRB15W, SM2135_WCBGR15W };

struct SM2135 {
  uint8_t clk = 0;
  uint8_t data = 0;
  uint8_t current;
  uint8_t model = SM2135_WCGRB;
} Sm2135;

/*********************************************************************************************\
 * SM2135 code
\*********************************************************************************************/

const uint8_t SM2135_DELAY = 4;

void Sm2135SetLow(uint8_t pin) {
  noInterrupts();
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  interrupts();
}

void Sm2135SetHigh(uint8_t pin) {
  noInterrupts();
  pinMode(pin, INPUT_PULLUP);
  interrupts();
}

bool Sm2135Init(void) {
  digitalWrite(Sm2135.data, LOW);
  digitalWrite(Sm2135.clk, LOW);
  Sm2135SetHigh(Sm2135.data);
  Sm2135SetHigh(Sm2135.clk);
  return (!((digitalRead(Sm2135.data) == LOW || digitalRead(Sm2135.clk) == LOW)));
}

bool Sm2135Write(uint8_t value) {
  for (uint8_t curr = 0X80; curr != 0; curr >>= 1) {
    if (curr & value) {
      Sm2135SetHigh(Sm2135.data);
    } else {
      Sm2135SetLow(Sm2135.data);
    }
    Sm2135SetHigh(Sm2135.clk);
    delayMicroseconds(SM2135_DELAY);
    Sm2135SetLow(Sm2135.clk);
  }
  // get Ack or Nak
  Sm2135SetHigh(Sm2135.data);
  Sm2135SetHigh(Sm2135.clk);
  delayMicroseconds(SM2135_DELAY / 2);
  uint8_t ack = digitalRead(Sm2135.data);
  Sm2135SetLow(Sm2135.clk);
  delayMicroseconds(SM2135_DELAY / 2);
  Sm2135SetLow(Sm2135.data);
  return (0 == ack);
}

bool Sm2135Start(uint8_t addr) {
  Sm2135SetLow(Sm2135.data);
  delayMicroseconds(SM2135_DELAY);
  Sm2135SetLow(Sm2135.clk);
  return Sm2135Write(addr);
}

void Sm2135Stop(void) {
  Sm2135SetLow(Sm2135.data);
  delayMicroseconds(SM2135_DELAY);
  Sm2135SetHigh(Sm2135.clk);
  delayMicroseconds(SM2135_DELAY);
  Sm2135SetHigh(Sm2135.data);
  delayMicroseconds(SM2135_DELAY);
}

/********************************************************************************************/

bool Sm2135SetChannels(void) {
  uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;
  uint8_t data[6];

  uint32_t light_type = 3;      // RGB and CW
  if (Sm2135.model < 2) {       // Only allow one of two options due to power supply
    if ((0 == cur_col[0]) && (0 == cur_col[1]) && (0 == cur_col[2])) {
      light_type = 1;           // CW only
    } else {
      light_type = 2;           // RGB only
    }
  }
  if (light_type &2) {          // Set RGB
    Sm2135Start(SM2135_ADDR_MC);
    Sm2135Write(Sm2135.current);
    Sm2135Write(SM2135_RGB);
    if (Sm2135.model &1) {      // SM2135_WCBGR
      Sm2135Write(cur_col[2]);  // Blue
      Sm2135Write(cur_col[1]);  // Green
      Sm2135Write(cur_col[0]);  // Red
    } else {                    // SM2135_WCGRB
      Sm2135Write(cur_col[1]);  // Green
      Sm2135Write(cur_col[0]);  // Red
      Sm2135Write(cur_col[2]);  // Blue
    }
    Sm2135Stop();
  }
  if (light_type &1) {          // Set CW
    Sm2135Start(SM2135_ADDR_MC);
    Sm2135Write(Sm2135.current);
    Sm2135Write(SM2135_CW);
    Sm2135Stop();
    delay(1);
    Sm2135Start(SM2135_ADDR_C);
    Sm2135Write(cur_col[4]);    // Warm
    Sm2135Write(cur_col[3]);    // Cold
    Sm2135Stop();
  }

  return true;
}

void Sm2135ModuleSelected(void)
{
  if (PinUsed(GPIO_SM2135_CLK) && PinUsed(GPIO_SM2135_DAT, GPIO_ANY)) {
    Sm2135.clk = Pin(GPIO_SM2135_CLK);
    Sm2135.data = Pin(GPIO_SM2135_DAT, GPIO_ANY);

    // See #define MAX_SM2135_DAT 6 in tasmota_template.h
    Sm2135.model = GetPin(Sm2135.data) - AGPIO(GPIO_SM2135_DAT);  // 0 .. 5

    // Legacy support of model selection
    if (PinUsed(GPIO_SWT1)) {
      Sm2135.model = SM2135_WCBGR;
      pinMode(Pin(GPIO_SWT1), INPUT);             // Discard GPIO_SWT functionality
      SetPin(Pin(GPIO_SWT1), AGPIO(GPIO_NONE));
    }

    // SM2135 Dat 1/2
    //                RGB current         CW current
    Sm2135.current = (SM2135_20MA << 4) | SM2135_15MA;  // See https://github.com/arendst/Tasmota/issues/6495#issuecomment-549121683
    switch (Sm2135.model) {
      case SM2135_WCGRBHI:      // SM2135 Dat 3
      case SM2135_WCBGRHI:      // SM2135 Dat 4
        Sm2135.current = (SM2135_20MA << 4) | SM2135_30MA;
        break;
      case SM2135_WCGRB15W:     // SM2135 Dat 5
      case SM2135_WCBGR15W:     // SM2135 Dat 6
        Sm2135.current = (SM2135_45MA << 4) | SM2135_60MA;
        break;
    }

    Sm2135Init();

    TasmotaGlobal.light_type = LT_RGBWC;
    TasmotaGlobal.light_driver = XLGT_04;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: SM2135 %s Found"), (SM2135_WCBGR == (Sm2135.model &1)) ? PSTR("BGR") : PSTR("GRB"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt04(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = Sm2135SetChannels();
      break;
    case FUNC_MODULE_INIT:
      Sm2135ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_SM2135
#endif  // USE_LIGHT
