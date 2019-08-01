/*
  xdsp_08_ILI9488.ino - Display ILI9488 support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz

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

#define COLORED                1
#define UNCOLORED              0

// touch panel controller
#define FT6236_address 0x38

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT


#include <ILI9488.h>
#include <FT6236.h>

TouchLocation pLoc;
uint8_t ctouch_counter = 0;

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
    if  (pin[GPIO_BACKLIGHT]<99) {
      bppin=pin[GPIO_BACKLIGHT];
    }

    // init renderer
    if  ((pin[GPIO_SSPI_CS]<99) && (pin[GPIO_SSPI_MOSI]<99) && (pin[GPIO_SSPI_SCLK]<99)){
      ili9488  = new ILI9488(pin[GPIO_SSPI_CS],pin[GPIO_SSPI_MOSI],pin[GPIO_SSPI_SCLK],bppin);
    } else {
      if ((pin[GPIO_SPI_CS]<99) && (pin[GPIO_SPI_MOSI]<99) && (pin[GPIO_SPI_CLK]<99)) {
        ili9488  = new ILI9488(pin[GPIO_SPI_CS],pin[GPIO_SPI_MOSI],pin[GPIO_SPI_CLK],bppin);
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

    if (i2c_flg && I2cDevice(FT6236_address)) {
      FT6236begin(FT6236_address);
      FT6236_found=1;
    } else {
      FT6236_found=0;
    }

  }
}

void ILI9488_MQTT(uint8_t count,const char *cp) {
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"RA8876\":{\"%s%d\":\"%d\"}}"), mqtt_data,cp,count+1,(buttons[count]->vpower&0x80)>>7);
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
}

// check digitizer hit
void FT6236Check() {
uint16_t temp;
ctouch_counter++;
if (2 == ctouch_counter) {
  // every 100 ms should be enough
  ctouch_counter=0;
  if (FT6236readTouchLocation(&pLoc,1)) {
    // did find a hit
    if (renderer) {
      uint8_t rot=renderer->getRotation();
      switch (rot) {
        case 0:
          temp=pLoc.y;
          pLoc.y=renderer->height()-pLoc.x;
          pLoc.x=temp;
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          temp=pLoc.y;
          pLoc.y=pLoc.x;
          pLoc.x=renderer->width()-temp;
          break;
      }
      // now must compare with defined buttons
      for (uint8_t count=0; count<MAXBUTTONS; count++) {
        if (buttons[count]) {
            if (buttons[count]->contains(pLoc.x,pLoc.y)) {
                // did hit
                buttons[count]->press(true);
                if (buttons[count]->justPressed()) {
                  uint8_t bflags=buttons[count]->vpower&0x7f;
                  if (!bflags) {
                    // real button
                    if (!SendKey(0, count+1, POWER_TOGGLE)) {
                      ExecuteCommandPower(count+1, POWER_TOGGLE, SRC_BUTTON);
                    }
                    buttons[count]->xdrawButton(bitRead(power,count));
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
        }
      }
    }
  } else {
    // no hit
    for (uint8_t count=0; count<MAXBUTTONS; count++) {
      if (buttons[count]) {
        buttons[count]->press(false);
        if (buttons[count]->justReleased()) {
          uint8_t bflags=buttons[count]->vpower&0x7f;
          if (bflags>1) {
            // push button
            buttons[count]->vpower&=0x7f;
            ILI9488_MQTT(count,"PBT");
          }
          buttons[count]->xdrawButton(buttons[count]->vpower&0x80);
        }
      }
    }
    pLoc.x=0;
    pLoc.y=0;
  }
}
}
/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp08(byte function)
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
          if (FT6236_found) FT6236Check();
          break;
      }
    }
  //}
  return result;
}

#endif  // USE_DISPLAY_ILI9488
#endif  // USE_DISPLAY
#endif  // USE_SPI
