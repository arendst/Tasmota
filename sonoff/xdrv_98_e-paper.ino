/*
  xdrv_98_sd1306.ino - EXPERIMENTAL display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz and Waveshare

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

  about 12,5k
*/

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY
#ifdef USE_EPAPER

#include <epd2in9.h>
#include <epdpaint.h>


unsigned char image[(EPD_HEIGHT*EPD_WIDTH)/8];
Paint paint(image,EPD_WIDTH,EPD_HEIGHT);    // width should be the multiple of 8
Epd epd;
sFONT *selected_font;

void DisplayInit(void) {
  // init paper display
  #define COLORED     0
  #define UNCOLORED   1

  if (pin[GPIO_SSPI_CS]<99) {
    epd.cs_pin=pin[GPIO_SSPI_CS];
  } else {
    return;
  }
  if (pin[GPIO_SSPI_MOSI]<99) {
    epd.mosi_pin=pin[GPIO_SSPI_MOSI];
  } else {
    return;
  }
  if (pin[GPIO_SSPI_SCLK]<99) {
    epd.sclk_pin=pin[GPIO_SSPI_SCLK];
  } else {
    return;
  }

  // whiten display with full update
  epd.Init(lut_full_update);
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(3000);

  // switch to partial update
  epd.Init(lut_partial_update);

  // Clear image memory
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(500);

  selected_font=&Font12;

  // Welcome text
  paint.SetRotate(ROTATE_90);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(50, 50, "Waveshare E-Paper Display!", selected_font, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(1000);

  paint.Clear(UNCOLORED);

/*
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  Serial.println("Displayed image data");
  delay(3000);
*/

  // Black screen
  //epd.ClearFrameMemory(0);
  //epd.DisplayFrame();
  //Serial.println("Displayed black screen");
  //delay(3000);

  //epd.Sleep();

}


enum DisplayCommands { CMND_DISP_TEXT };
const char kDisplayCommands[] PROGMEM = D_CMND_DISP_TEXT ;
#define D_CMND_DISPLAY "Display"
const char S_JSON_DISPLAY_COMMAND_VALUE[] PROGMEM =        "{\"" D_CMND_DISPLAY "%s\":\"%s\"}";
int16_t xpos,ypos;

// get asci number until delimiter and return asci number lenght and value
uint8_t atoiv(char *cp,int16_t *res) {
  uint8_t index=0;
  *res=atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
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

        // Here display command manipulation could take place like textsize, color, position etc. using intext parameters
        // Currently just adds to the logbuffer

        //strlcpy(disp_log_buffer[disp_log_buffer_idx], XdrvMailbox.data, sizeof(disp_log_buffer[disp_log_buffer_idx]));
        // xx:y:text
        char *cp=XdrvMailbox.data;
        uint8_t lpos,escape=0,var;
        int16_t lin=0,col=0,fill=0,temp,temp1;
        uint8_t font_x=6,font_y=8,fontnumber=1;
        char linebuf[80],*dp=linebuf;
        memset(linebuf,' ',sizeof(linebuf));
        linebuf[sizeof(linebuf)-1]=0;
        *dp=0;

        while (*cp) {
            if (!escape) {
              // check for escape
              if (*cp=='[') {
                escape=1;
                cp++;
                // if string in buffer print it
                if ((uint32_t)dp-(uint32_t)linebuf) {
                    if (!fill) *dp=0;
                    if (col>0 && lin>0) {
                      // use col and lin
                      paint.DrawStringAt((col-1)*selected_font->Width,(lin-1)*selected_font->Height,linebuf,selected_font, COLORED);
                    } else {
                      // use xpos, ypos
                      paint.DrawStringAt(xpos,ypos,linebuf,selected_font, COLORED);
                    }
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
                    paint.Clear(UNCOLORED);
                    break;
                  case 'i':
                    // init display with partial update
                    epd.Init(lut_partial_update);
                    paint.Clear(UNCOLORED);
                    epd.DisplayFrame();
                    delay(500);
                    break;
                  case 'I':
                    // init display with full refresh
                    epd.Init(lut_full_update);
                    paint.Clear(UNCOLORED);
                    epd.ClearFrameMemory(0xFF);
                    epd.DisplayFrame();
                    delay(3000);
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
                    //display.setCursor(display.getCursorX(),(lin-1)*font_y*txtsize);
                    break;
                  case 'c':
                    // text column cxx
                    var=atoiv(cp,&col);
                    cp+=var;
                    //display.setCursor((col-1)*font_x*txtsize,display.getCursorY());
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
                      paint.DrawHorizontalLine(xpos+temp,ypos,-temp,COLORED);
                    } else {
                      paint.DrawHorizontalLine(xpos,ypos,temp,COLORED);
                    }
                    xpos+=temp;
                    break;
                  case 'v':
                    // vert line to
                    var=atoiv(cp,&temp);
                    cp+=var;
                    if (temp<0) {
                      paint.DrawVerticalLine(xpos,ypos+temp,-temp,COLORED);
                    } else {
                      paint.DrawVerticalLine(xpos,ypos,temp,COLORED);
                    }
                    ypos+=temp;
                    break;
                  case 'k':
                    // circle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    paint.DrawCircle(xpos,ypos,temp,COLORED);
                    break;
                  case 'K':
                    // filled circle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    paint.DrawFilledCircle(xpos,ypos,temp,COLORED);
                    break;
                  case 'r':
                    // rectangle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    cp++;
                    var=atoiv(cp,&temp1);
                    cp+=var;
                    paint.DrawRectangle(xpos,ypos,temp,temp1,COLORED);
                    break;
                  case 'R':
                    // filled rectangle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    cp++;
                    var=atoiv(cp,&temp1);
                    cp+=var;
                    paint.DrawFilledRectangle(xpos,ypos,temp,temp1,COLORED);
                    break;
                  case 'd':
                    // force draw grafics buffer
                    epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
                    epd.DisplayFrame();
                    break;
                  case 's':
                  case 'f':
                    // size or font sx
                    fontnumber=*cp&7;
                    if (fontnumber==1) {
                      selected_font=&Font12;
                    } else {
                      selected_font=&Font24;
                    }
                    cp+=1;
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
            if (col>0 && lin>0) {
              // use col and lin
              paint.DrawStringAt((col-1)*selected_font->Width,(lin-1)*selected_font->Height,linebuf,selected_font, COLORED);
            } else {
              // use xpos, ypos
              paint.DrawStringAt(xpos,ypos,linebuf,selected_font, COLORED);
            }
        }
        // draw buffer
        epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
        epd.DisplayFrame();

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
      //if (i2c_flg || spi_flg) {
        DisplayInit();
      //}
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

#endif  // USE_EPAPER
#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
