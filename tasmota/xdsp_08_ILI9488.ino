/*
  xdsp_08_ILI9488.ino - Display ILI9488 support for Tasmota

  Copyright (C) 2020  Theo Arends, Gerhard Mutz

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
#ifdef USE_DISPLAY_ILI9488

#define XDSP_08                8
#define XI2C_38                38  // See I2CDEVICES.md

#define COLORED                1
#define UNCOLORED              0

// touch panel controller
#define FT6236_address 0x38

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT


#include <ILI9488.h>
#include <FT6236.h>

TouchLocation ili9488_pLoc;
uint8_t ili9488_ctouch_counter = 0;

// currently fixed
#define BACKPLANE_PIN 2

extern uint8_t *buffer;
extern uint8_t color_type;
ILI9488 *ili9488;

#ifdef USE_TOUCH_BUTTONS
extern VButton *buttons[];
#endif

extern const uint16_t picture[];
uint8_t FT6236_found;

/*********************************************************************************************/

void ILI9488_InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_08;
  }

  if (XDSP_08 == Settings.display_model) {

    if (Settings.display_width != ILI9488_TFTWIDTH) {
      Settings.display_width = ILI9488_TFTWIDTH;
    }
    if (Settings.display_height != ILI9488_TFTHEIGHT) {
      Settings.display_height = ILI9488_TFTHEIGHT;
    }

    // disable screen buffer
    buffer=NULL;

    // default colors
    fg_color = ILI9488_WHITE;
    bg_color = ILI9488_BLACK;

    uint8_t bppin=BACKPLANE_PIN;
    if  (PinUsed(GPIO_BACKLIGHT)) {
      bppin=Pin(GPIO_BACKLIGHT);
    }

    // init renderer
    if (PinUsed(GPIO_SSPI_CS) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK)) {
      ili9488  = new ILI9488(Pin(GPIO_SSPI_CS),Pin(GPIO_SSPI_MOSI),Pin(GPIO_SSPI_SCLK),bppin);
    } else {
      if (PinUsed(GPIO_SPI_CS) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CLK)) {
        ili9488  = new ILI9488(Pin(GPIO_SPI_CS),Pin(GPIO_SPI_MOSI),Pin(GPIO_SPI_CLK),bppin);
      } else {
        return;
      }
    }

    SPI.begin();
    ili9488->begin();
    renderer = ili9488;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(ILI9488_WHITE,ILI9488_BLACK);
    renderer->DrawStringAt(50, 50, "ILI9488 TFT Display!", ILI9488_WHITE,0);
    delay(1000);

    //renderer->drawRGBBitmap(100,100, picture,51,34);
#endif

    color_type = COLOR_COLOR;
    // start digitizer with fixed adress

    if (I2cEnabled(XI2C_38) && I2cSetDevice(FT6236_address)) {
      FT6236begin(FT6236_address);
      FT6236_found=1;
      I2cSetActiveFound(FT6236_address, "FT6236");
    } else {
      FT6236_found=0;
    }

  }
}

#ifdef USE_TOUCH_BUTTONS
void ILI9488_MQTT(uint8_t count,const char *cp) {
  ResponseTime_P(PSTR(",\"RA8876\":{\"%s%d\":\"%d\"}}"), cp,count+1,(buttons[count]->vpower&0x80)>>7);
  MqttPublishTeleSensor();
}

void ILI9488_RDW_BUTT(uint32_t count,uint32_t pwr) {
  buttons[count]->xdrawButton(pwr);
  if (pwr) buttons[count]->vpower|=0x80;
  else buttons[count]->vpower&=0x7f;
}
// check digitizer hit
void FT6236Check() {
uint16_t temp;
uint8_t rbutt=0,vbutt=0;
ili9488_ctouch_counter++;
if (2 == ili9488_ctouch_counter) {
  // every 100 ms should be enough
  ili9488_ctouch_counter=0;
  if (FT6236readTouchLocation(&ili9488_pLoc,1)) {
    // did find a hit
    if (renderer) {
      uint8_t rot=renderer->getRotation();
      switch (rot) {
        case 0:
          temp=ili9488_pLoc.y;
          ili9488_pLoc.y=renderer->height()-ili9488_pLoc.x;
          ili9488_pLoc.x=temp;
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          temp=ili9488_pLoc.y;
          ili9488_pLoc.y=ili9488_pLoc.x;
          ili9488_pLoc.x=renderer->width()-temp;
          break;
      }
      // now must compare with defined buttons
      for (uint8_t count=0; count<MAXBUTTONS; count++) {
        if (buttons[count]) {
            uint8_t bflags=buttons[count]->vpower&0x7f;
            if (buttons[count]->contains(ili9488_pLoc.x,ili9488_pLoc.y)) {
                // did hit
                buttons[count]->press(true);
                if (buttons[count]->justPressed()) {
                  if (!bflags) {
                    uint8_t pwr=bitRead(power,rbutt);
                    if (!SendKey(KEY_BUTTON, rbutt+1, POWER_TOGGLE)) {
                      ExecuteCommandPower(rbutt+1, POWER_TOGGLE, SRC_BUTTON);
                      ILI9488_RDW_BUTT(count,!pwr);
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
                    ILI9488_MQTT(count,cp);
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
              ILI9488_MQTT(count,"PBT");
            }
            buttons[count]->xdrawButton(buttons[count]->vpower&0x80);
          }
        }
        if (!bflags) {
          // check if power button stage changed
          uint8_t pwr=bitRead(power,rbutt);
          uint8_t vpwr=(buttons[count]->vpower&0x80)>>7;
          if (pwr!=vpwr) {
            ILI9488_RDW_BUTT(count,pwr);
          }
          rbutt++;
        }
      }
    }
    ili9488_pLoc.x=0;
    ili9488_pLoc.y=0;
  }
}
}
#endif // USE_TOUCH_BUTTONS
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
    else if (XDSP_08 == Settings.display_model) {

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
#ifdef USE_TOUCH_BUTTONS
          if (FT6236_found) FT6236Check();
#endif
          break;
      }
    }
  //}
  return result;
}

#endif  // USE_DISPLAY_ILI9488
#endif  // USE_DISPLAY
#endif  // USE_SPI
