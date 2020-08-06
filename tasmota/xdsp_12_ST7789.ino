/*
  xdsp_12_ST7789.ino - Display ST7789 support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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
#ifdef USE_DISPLAY_ST7789

#define XDSP_12                12
#define XI2C_38                38  // See I2CDEVICES.md

#undef COLORED
#define COLORED                1
#undef UNCOLORED
#define UNCOLORED              0

// touch panel controller
#undef FT5206_address
#define FT5206_address 0x38

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#undef USE_TINY_FONT
#define USE_TINY_FONT


#include <Arduino_ST7789.h>
#include <FT5206.h>

// currently fixed
#define BACKPLANE_PIN 2

extern uint8_t *buffer;
extern uint8_t color_type;
Arduino_ST7789 *st7789;

#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS
extern VButton *buttons[];
#endif
FT5206_Class *touchp;
uint8_t FT5206_found;
TP_Point st7789_pLoc;
uint8_t st7789_ctouch_counter = 0;
#endif // USE_FT5206

/*********************************************************************************************/

void ST7789_InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_12;
  }

  if (XDSP_12 == Settings.display_model) {

    if (Settings.display_width != ST7789_TFTWIDTH) {
      Settings.display_width = ST7789_TFTWIDTH;
    }
    if (Settings.display_height != ST7789_TFTHEIGHT) {
      Settings.display_height = ST7789_TFTHEIGHT;
    }

    // disable screen buffer
    buffer=NULL;

    // default colors
    fg_color = ST7789_WHITE;
    bg_color = ST7789_BLACK;

    uint8_t bppin=BACKPLANE_PIN;
    if  (PinUsed(GPIO_BACKLIGHT)) {
      bppin=Pin(GPIO_BACKLIGHT);
    }

    uint8_t reset = -1;
    if  (PinUsed(GPIO_OLED_RESET)) {
      reset=Pin(GPIO_OLED_RESET);
    }

    uint8_t cs = -1;
    if  (PinUsed(GPIO_SSPI_CS)) {
      cs=Pin(GPIO_SSPI_CS);
    } else if (PinUsed(GPIO_SPI_CS)) {
      cs=Pin(GPIO_SPI_CS);
    }

#ifdef ESP32
#undef HW_SPI_MOSI
#define HW_SPI_MOSI 23
#undef HW_SPI_CLK
#define HW_SPI_CLK 18
#else
#undef HW_SPI_MOSI
#define HW_SPI_MOSI 13
#undef HW_SPI_CLK
#define HW_SPI_CLK 14
#endif

    // init renderer, may use hardware spi
    if (PinUsed(GPIO_SPI_CS) && (Pin(GPIO_SPI_MOSI)==HW_SPI_MOSI) && (Pin(GPIO_SPI_CLK)==HW_SPI_CLK) && PinUsed(GPIO_SPI_DC)) {
        st7789  = new Arduino_ST7789(Pin(GPIO_SPI_DC), reset, Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_CLK), cs, bppin);
    } else {
      if ((PinUsed(GPIO_SSPI_CS) || PinUsed(GPIO_OLED_RESET)) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK) && PinUsed(GPIO_SSPI_DC)) {
        st7789  = new Arduino_ST7789(Pin(GPIO_SSPI_DC), reset, Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_SCLK), cs, bppin);
      } else {
        return;
      }
    }
    st7789->init(Settings.display_width,Settings.display_height);
    renderer = st7789;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(ST7789_WHITE,ST7789_BLACK);
    renderer->DrawStringAt(30, 100, "ST7789 TFT!", ST7789_WHITE,0);
    delay(1000);
#endif

    color_type = COLOR_COLOR;

#ifdef ESP32
#ifdef USE_FT5206
    // start digitizer with fixed adress and pins for esp32
    #define SDA_2 23
    #define SCL_2 32
    Wire1.begin(SDA_2, SCL_2, 400000);
    touchp = new FT5206_Class();
    if (touchp->begin(Wire1, FT5206_address)) {
      FT5206_found=1;
      //I2cSetDevice(FT5206_address);
      I2cSetActiveFound(FT5206_address, "FT5206");
    } else {
      FT5206_found=0;
    }
#endif // USE_FT5206
#endif // ESP32

  }
}


#ifdef ESP32
#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS
void ST7789_MQTT(uint8_t count,const char *cp) {
  ResponseTime_P(PSTR(",\"ST7789\":{\"%s%d\":\"%d\"}}"), cp,count+1,(buttons[count]->vpower&0x80)>>7);
  MqttPublishTeleSensor();
}

void ST7789_RDW_BUTT(uint32_t count,uint32_t pwr) {
  buttons[count]->xdrawButton(pwr);
  if (pwr) buttons[count]->vpower|=0x80;
  else buttons[count]->vpower&=0x7f;
}
// check digitizer hit
void FT5206Check() {
uint16_t temp;
uint8_t rbutt=0,vbutt=0;
st7789_ctouch_counter++;
if (2 == st7789_ctouch_counter) {
  // every 100 ms should be enough
  st7789_ctouch_counter=0;

  if (touchp->touched()) {
    // did find a hit
    st7789_pLoc = touchp->getPoint(0);
    if (renderer) {
      uint8_t rot=renderer->getRotation();
      switch (rot) {
        case 0:
          temp=st7789_pLoc.y;
          st7789_pLoc.y=renderer->height()-st7789_pLoc.x;
          st7789_pLoc.x=temp;
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          temp=st7789_pLoc.y;
          st7789_pLoc.y=st7789_pLoc.x;
          st7789_pLoc.x=renderer->width()-temp;
          break;
      }
      // now must compare with defined buttons
      for (uint8_t count=0; count<MAXBUTTONS; count++) {
        if (buttons[count]) {
            uint8_t bflags=buttons[count]->vpower&0x7f;
            if (buttons[count]->contains(st7789_pLoc.x,st7789_pLoc.y)) {
                // did hit
                buttons[count]->press(true);
                if (buttons[count]->justPressed()) {
                  if (!bflags) {
                    uint8_t pwr=bitRead(power,rbutt);
                    if (!SendKey(KEY_BUTTON, rbutt+1, POWER_TOGGLE)) {
                      ExecuteCommandPower(rbutt+1, POWER_TOGGLE, SRC_BUTTON);
                      ST7789_RDW_BUTT(count,!pwr);
                    }
                  } else {
                    // virtual button
                    const char *cp;
                    if (bflags==1) {
                      // toggle button
                      buttons[count]->vpower^=0x80;
                      cp="TBT";
                    } else {
                      // push button
                      buttons[count]->vpower|=0x80;
                      cp="PBT";
                    }
                    buttons[count]->xdrawButton(buttons[count]->vpower&0x80);
                    ST7789_MQTT(count,cp);
                  }
                }
            }
            if (!bflags) {
              rbutt++;
            } else {
              vbutt++;
            }
        }
      }
    }
  } else {
    // no hit
    for (uint8_t count=0; count<MAXBUTTONS; count++) {
      if (buttons[count]) {
        uint8_t bflags=buttons[count]->vpower&0x7f;
        buttons[count]->press(false);
        if (buttons[count]->justReleased()) {
          uint8_t bflags=buttons[count]->vpower&0x7f;
          if (bflags>0) {
            if (bflags>1) {
              // push button
              buttons[count]->vpower&=0x7f;
              ST7789_MQTT(count,"PBT");
            }
            buttons[count]->xdrawButton(buttons[count]->vpower&0x80);
          }
        }
        if (!bflags) {
          // check if power button stage changed
          uint8_t pwr=bitRead(power,rbutt);
          uint8_t vpwr=(buttons[count]->vpower&0x80)>>7;
          if (pwr!=vpwr) {
            ST7789_RDW_BUTT(count,pwr);
          }
          rbutt++;
        }
      }
    }
    st7789_pLoc.x=0;
    st7789_pLoc.y=0;
  }
}
}
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206
#endif // ESP32

/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp12(uint8_t function)
{
  bool result = false;

//AddLog_P2(LOG_LEVEL_INFO, PSTR("touch %d - %d"), FT5206_found, function);

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    ST7789_InitDriver();
  }
  else if (XDSP_12 == Settings.display_model) {
      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS
          if (FT5206_found) {
            FT5206Check();
          }
#endif
#endif // USE_FT5206
          break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_ST7789
#endif  // USE_DISPLAY
#endif  // USE_SPI
