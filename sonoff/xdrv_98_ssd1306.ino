/*
  xdrv_98_sd1306.ino - EXPERIMENTAL display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz and Adafruit

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

  about 9.1k
*/

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY
#ifdef USE_SSD1306

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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0,0);
  display.clearDisplay();
}


boolean DisplayCommand() {
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
        char *cp=XdrvMailbox.data;
        uint8_t lpos,escape=0,var;
        int16_t lin=0,col=0,fill=0,temp,temp1;
        uint8_t font_x=6,font_y=8,txtsize=1;
        char linebuf[80],*dp=linebuf;
        memset(linebuf,' ',sizeof(linebuf));
        linebuf[sizeof(linebuf)-1]=0;

        while (*cp) {
            if (!escape) {
              // character copy
              if (*cp=='[') {
                escape=1;
                cp++;
                if ((uint32_t)dp-(uint32_t)linebuf) {
                  if (!fill) *dp=0;
                  if (col==0 && lin==0) {
                    // use xpos,ypos
                    display.setCursor(xpos,ypos);
                  }
                  display.println(linebuf);
                  display.display();
                  memset(linebuf,' ',sizeof(linebuf));
                  linebuf[sizeof(linebuf)-1]=0;
                  dp=linebuf;
                }
              } else {
                // copy chars
                *dp++=*cp++;
              }
            } else {
              // check escapes
              if (*cp==']') {
                escape=0;
                cp++;
              } else {
                // analyze escapes
                switch (*cp++) {
                  case 'z':
                    // clear display
                    display.clearDisplay();
                    display.display();
                    display.setCursor(0,0);
                    break;
                  case 'i':
                  case 'I':
                    break;
                  case 'x':
                    // set xpos
                    var=atoiv(cp,&xpos);
                    cp+=var;
                    break;
                  case 'y':
                    // set ypos
                    var=atoiv(cp,&ypos);
                    cp+=var;
                    break;
                  case 'l':
                    // text line lxx
                    var=atoiv(cp,&lin);
                    cp+=var;
                    display.setCursor(display.getCursorX(),(lin-1)*font_y*txtsize);
                    break;
                  case 'c':
                    // text column cxx
                    var=atoiv(cp,&col);
                    cp+=var;
                    display.setCursor((col-1)*font_x*txtsize,display.getCursorY());
                    break;
                  case 'p':
                    // pad field with spaces fxx
                    var=atoiv(cp,&fill);
                    cp+=var;
                    linebuf[fill]=0;
                    break;
                  case 'h':
                      // hor line to
                    var=atoiv(cp,&temp);
                    cp+=var;
                    if (temp<0) {
                      display.writeFastHLine(xpos+temp,ypos,-temp,WHITE);
                    } else {
                      display.writeFastHLine(xpos,ypos,temp,WHITE);
                    }
                    xpos+=temp;
                    break;
                  case 'v':
                    // vert line to
                    var=atoiv(cp,&temp);
                    cp+=var;
                    if (temp<0) {
                      display.writeFastVLine(xpos,ypos+temp,-temp,WHITE);
                    } else {
                      display.writeFastVLine(xpos,ypos,temp,WHITE);
                    }
                    ypos+=temp;
                    break;
                  case 'k':
                    // circle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    display.drawCircle(xpos,ypos,temp,WHITE);
                    break;
                  case 'K':
                    // filled circle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    display.fillCircle(xpos,ypos,temp,WHITE);
                    break;
                  case 'r':
                    // rectangle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    var=atoiv(cp,&temp1);
                    cp+=var;
                    display.drawRect(xpos,ypos,temp,temp1,WHITE);
                    break;
                  case 'R':
                    // filled rectangle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    var=atoiv(cp,&temp1);
                    cp+=var;
                    display.fillRect(xpos,ypos,temp,temp1,WHITE);
                    break;
                  case 't':
                    sprintf(dp,"%02d:%02d",RtcTime.hour,RtcTime.minute);
                    dp+=5;
                    break;
                  case 's':
                  case 'f':
                    // size sx
                    txtsize=*cp&7;
                    cp+=1;
                    display.setTextSize(txtsize);
                    break;
                  default:
                    // unknown escape
                    snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("unknown escape"));
                    goto exit;
                    break;
                }
              }
            }
        }
        exit:
        // now draw buffer
        if ((uint32_t)dp-(uint32_t)linebuf) {
          if (!fill) *dp=0;
          if (col==0 && lin==0) {
            // use xpos,ypos
            display.setCursor(xpos,ypos);
          }
          display.println(linebuf);
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


#endif  // USE_SSD1306
#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
