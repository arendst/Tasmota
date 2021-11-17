/*
  xdsp_08_ILI9488.ino - Display ILI9488 support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz

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

#ifdef USE_SPI
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_ILI9488_UD
#ifdef USE_UNIVERSAL_DISPLAY

#define XDSP_08                8

bool ili9488_init_done = false;

Renderer *Init_uDisplay(const char *desc, int8_t cs);
void udisp_CheckTouch(void);

/*********************************************************************************************/

const char ILI9488_DESC[] PROGMEM =
":H,ILI9488,480,320,16,SPI,1,*,*,*,*,*,*,*,10\n"
":S,2,1,1,0,40,20\n"
":I\n"
"E0,0F,00,03,09,08,16,0A,3F,78,4C,09,0A,08,16,1A,0F\n"
"E1,0F,00,16,19,03,0F,05,32,45,46,04,0E,0D,35,37,0F\n"
"C0,2,17,15\n"
"C1,1,41\n"
"C5,3,00,12,80\n"
"36,1,48\n"
"3A,1,66\n"
"B0,1,80\n"
"B1,1,A0\n"
"B4,1,02\n"
"B6,2,02,02\n"
"E9,1,00\n"
"F7,4,A9,51,2C,82\n"
"11,80\n"
"29,0\n"
":o,28\n"
":O,29\n"
":A,2A,2B,2C,16\n"
":R,36\n"
";:0,48,00,00,00\n"
":0,28,00,00,01\n"
":1,28,00,00,00\n"
":2,E8,00,00,03\n"
":3,88,00,00,02\n"
":P,18\n"
":i,20,21\n"
":TI1,38,*,*\n"
"#\n";

void ILI9488_InitDriver(void) {
  if (PinUsed(GPIO_ILI9488_CS) && (TasmotaGlobal.spi_enabled & SPI_MOSI)) {

    renderer = Init_uDisplay(ILI9488_DESC, Pin(GPIO_ILI9488_CS));

    if (!renderer) return;

    Settings->display_model = XDSP_08;

    ili9488_init_done = true;
  }
}

/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp08(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    ILI9488_InitDriver();
  }
  else if (ili9488_init_done && (XDSP_08 == Settings->display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_EVERY_50_MSECOND:
#ifdef USE_FT5206
        if (FT5206_found) {
          udisp_CheckTouch();
        }
#endif
        break;
    }
  }
  return result;
}

#endif // USE_UNIVERSAL_DISPLAY
#endif  // USE_DISPLAY_ILI9488
#endif  // USE_DISPLAY
#endif  // USE_SPI
