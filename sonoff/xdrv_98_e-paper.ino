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
}

void Clr_screen() {
  paint.Clear(UNCOLORED);
}

void Init_Partial() {
  epd.Init(lut_partial_update);
  paint.Clear(UNCOLORED);
  epd.DisplayFrame();
  delay(500);
}

void Init_Full() {
  epd.Init(lut_full_update);
  paint.Clear(UNCOLORED);
  epd.DisplayFrame();
  delay(500);
}

void Draw_HLine(uint16_t x,uint16_t y,uint16_t len) {
  paint.DrawHorizontalLine(x,y,len,COLORED);
}

void Draw_VLine(uint16_t x,uint16_t y,uint16_t len) {
  paint.DrawVerticalLine(x,y,len,COLORED);
}

void Draw_Circle(uint16_t x,uint16_t y,uint16_t rad) {
  paint.DrawCircle(x,y,rad,COLORED);
}

void Draw_FilledCircle(uint16_t x,uint16_t y,uint16_t rad) {
  paint.DrawFilledCircle(x,y,rad,COLORED);
}

void Draw_Rectangle(uint16_t x,uint16_t y,uint16_t x2,uint16_t y2) {
  paint.DrawRectangle(x,y,x2,y2,COLORED);
}

void Draw_FilledRectangle(uint16_t x,uint16_t y,uint16_t x2,uint16_t y2) {
  paint.DrawFilledRectangle(x,y,x2,y2,COLORED);
}

void Draw_Frame() {
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
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
                    Clr_screen();
                    break;
                  case 'i':
                    // init display with partial update
                    Init_Partial();
                    break;
                  case 'I':
                    // init display with full refresh
                    Init_Full();
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
                      Draw_HLine(xpos+temp,ypos,-temp);
                    } else {
                      Draw_HLine(xpos,ypos,temp);
                    }
                    xpos+=temp;
                    break;
                  case 'v':
                    // vert line to
                    var=atoiv(cp,&temp);
                    cp+=var;
                    if (temp<0) {
                      Draw_VLine(xpos,ypos+temp,-temp);
                    } else {
                      Draw_VLine(xpos,ypos,temp);
                    }
                    ypos+=temp;
                    break;
                  case 'k':
                    // circle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    Draw_Circle(xpos,ypos,temp);
                    break;
                  case 'K':
                    // filled circle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    Draw_FilledCircle(xpos,ypos,temp);
                    break;
                  case 'r':
                    // rectangle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    cp++;
                    var=atoiv(cp,&temp1);
                    cp+=var;
                    Draw_Rectangle(xpos,ypos,temp,temp1);
                    break;
                  case 'R':
                    // filled rectangle
                    var=atoiv(cp,&temp);
                    cp+=var;
                    cp++;
                    var=atoiv(cp,&temp1);
                    cp+=var;
                    Draw_FilledRectangle(xpos,ypos,temp,temp1);
                    break;
                  case 't':
                    sprintf(dp,"%02d:%02d",RtcTime.hour,RtcTime.minute);
                    dp+=5;
                    break;
                  case 'd':
                    // force draw grafics buffer
                    Draw_Frame();
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
        Draw_Frame();

      } else {
        snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("no Text"));
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DISPLAY_COMMAND_VALUE, command, XdrvMailbox.data);
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}


#endif  // USE_EPAPER
#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
