/*
  xdrv_98_sd1306.ino - EXPERIMENTAL display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

  about 9.6k
*/

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void DisplayInit(void) {


  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();

  // text display tests
#if 0
display.setFont(&FreeMono9pt7b);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("In: 33400 kWh");
display.println("Out: 12400 kWh");
display.println("IO: 3400 W");
display.display();
#else
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0,0);
  display.clearDisplay();
  /*
  display.println("Power in: 33400 kWh");
  display.println("");
  display.println("Power out: 12400 kWh");
  display.println("");
  display.println("Current io: 3400 W");
  display.display();
  */
#endif

}

enum DisplayCommands { CMND_DISP_TEXT };
const char kDisplayCommands[] PROGMEM = D_CMND_DISP_TEXT ;
#define D_CMND_DISPLAY "Display"
const char S_JSON_DISPLAY_COMMAND_VALUE[] PROGMEM =        "{\"" D_CMND_DISPLAY "%s\":\"%s\"}";

boolean DisplayCommand()
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t disp_len = strlen(D_CMND_DISPLAY);  // Prep for string length change

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_DISPLAY), disp_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic +disp_len, kDisplayCommands);
    if (-1 == command_code) {
      serviced = false;  // Unknown command
    }
    else if (CMND_DISP_TEXT == command_code) {
      if (XdrvMailbox.data_len > 0) {

        // Here display command manipulation could take place like textsize, color, position etc. using intext parameters
        // Currently just adds to the logbuffer

        //strlcpy(disp_log_buffer[disp_log_buffer_idx], XdrvMailbox.data, sizeof(disp_log_buffer[disp_log_buffer_idx]));
        // xx:y:text
        char *cp=XdrvMailbox.data;
        char ypos=0,xpos=0;
        char fillflg=0;
        if (*cp=='c') {
          cp++;
          if (*cp=='s') {
            //clear screen
            display.clearDisplay();
            display.display();
            display.setCursor(0,0);
          } else {
            if (*cp=='f') {
              fillflg=1;
            } else {
              // clr line
              ypos=(*cp&7)*8;
              display.setCursor(0,ypos);
              // 21 chars
              display.println("                     ");
            }
          }
          cp++;
          if (*cp==':') {
              cp++;
              goto getpars;
          }
        } else {
          getpars:
          xpos=atoi(cp)*6;
          cp+=3;
          ypos=(*cp&7)*8;
          cp+=2;
          display.setCursor(xpos,ypos);
          if (fillflg) {
            char linebuf[22];
            memset(linebuf,' ',sizeof(linebuf));
            strcpy(linebuf,cp);
            cp=linebuf;
          }
          display.println(cp);
          display.display();
        }

      } else {
        snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("no Text"));
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_VALUE, command, XdrvMailbox.data);
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_98

boolean Xdrv98(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      if (i2c_flg || spi_flg) {
        DisplayInit();
      }
      break;
    case FUNC_EVERY_50_MSECOND:
      //DisplayRefresh();
      break;
    case FUNC_COMMAND:
      result = DisplayCommand();
      break;
    case FUNC_MQTT_SUBSCRIBE:
      //DisplayMqttSubscribe();
      break;
    case FUNC_MQTT_DATA:
      //result = DisplayMqttData();
      break;
    case FUNC_SET_POWER:
      //DisplaySetPower();
      break;
    case FUNC_SHOW_SENSOR:
      //DisplayLocalSensor();
      break;
  }
  return result;
}

#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
