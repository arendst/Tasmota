/*
  xdrv_98_display.ino - EXPERIMENTAL display support for Sonoff-Tasmota

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
#ifdef USE_DISPLAY

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

enum DisplayCommands { CMND_DISP_TEXT };
const char kDisplayCommands[] PROGMEM = D_CMND_DISP_TEXT ;
#define D_CMND_DISPLAY "Display"
const char S_JSON_DISPLAY_COMMAND_VALUE[] PROGMEM =        "{\"" D_CMND_DISPLAY "%s\":\"%s\"}";
int16_t xpos,ypos;


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
                      DrawStringAt(col,lin,linebuf,1);
                      //paint.DrawStringAt((col-1)*selected_font->Width,(lin-1)*selected_font->Height,linebuf,selected_font, COLORED);
                    } else {
                      // use xpos, ypos
                      DrawStringAt(xpos,ypos,linebuf,0);
                      //paint.DrawStringAt(xpos,ypos,linebuf,selected_font, COLORED);
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
                  case 'o':
                    DisplayOnOff(0);
                    break;
                  case 'O':
                    DisplayOnOff(1);
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
                    SetFontorSize(*cp&3);
                    cp+=1;
                    break;
                  case 'a':
                    // rotation angle
                    SetRotation(*cp&3);
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
              DrawStringAt(col,lin,linebuf,1);
              //paint.DrawStringAt((col-1)*selected_font->Width,(lin-1)*selected_font->Height,linebuf,selected_font, COLORED);
            } else {
              // use xpos, ypos
              DrawStringAt(xpos,ypos,linebuf,0);
              //paint.DrawStringAt(xpos,ypos,linebuf,selected_font, COLORED);
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

#define XDRV_98

boolean Xdrv98(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_PRE_INIT:
    #ifndef USE_EPAPER
          if (i2c_flg || spi_flg) {
            DisplayInit();
          }
    #else
          // epaper uses soft spi
          DisplayInit();
    #endif
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
