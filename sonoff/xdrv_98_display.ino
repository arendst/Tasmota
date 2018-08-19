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

#define NUM_GRAPHS 4

struct GRAPH {
  uint16_t xp;
  uint16_t yp;
  uint16_t xs;
  uint16_t ys;
  float ymin;
  float ymax;
  float range;
  uint32_t x_time;       // time per x slice in milliseconds
  uint32_t last_ms;
  int16_t decimation; // decimation or graph duration in minutes
  uint16_t dcnt;
  uint32_t summ;
  uint16_t xcnt;
  uint8_t *values;
  uint8_t xticks;
  uint8_t yticks;
  uint8_t last_val;
} graph[NUM_GRAPHS];

unsigned char display_ready;

void ClrGraph(uint16_t num) {
  uint16_t xticks=graph[num].xticks;
  uint16_t yticks=graph[num].yticks;
  uint16_t count;

  // clr inside
  Draw_FilledRectangle(graph[num].xp+1,graph[num].yp+1,graph[num].xs-2,graph[num].ys-2,0);

  if (xticks) {
    float cxp=graph[num].xp,xd=(float)graph[num].xs/(float)xticks;
    for (count=0; count<xticks; count++) {
      Draw_VLine(cxp,graph[num].yp+graph[num].ys-4,4);
      cxp+=xd;
    }
  }
  if (yticks) {
    if (graph[num].ymin<0 || graph[num].ymax>0) {
      // draw zero seperator
      float cxp=0;
      float czp=graph[num].yp+(graph[num].ymax/graph[num].range);
      while (cxp<graph[num].xs) {
        Draw_HLine(graph[num].xp+cxp,czp,2);
        cxp+=6.0;
      }
      // align ticks to zero line
      float cyp=0,yd=graph[num].ys/yticks;
      for (count=0; count<yticks; count++) {
        if ((czp-cyp)>graph[num].yp) {
          Draw_HLine(graph[num].xp,czp-cyp,4);
        }
        if ((czp+cyp)<(graph[num].yp+graph[num].ys)) {
          Draw_HLine(graph[num].xp,czp+cyp,4);
        }
        cyp+=yd;
      }

    } else {
      float cyp=graph[num].yp,yd=graph[num].ys/yticks;
      for (count=0; count<yticks; count++) {
        Draw_HLine(graph[num].xp,cyp,4);
        cyp+=yd;
      }
    }
  }
}

// define a graph
void DefineGraph(uint16_t num,uint16_t xp,uint16_t yp,uint16_t xs,uint16_t ys,int16_t dec,float ymin, float ymax) {

  uint16_t count;
  uint16_t index=num%NUM_GRAPHS;
  graph[index].xticks=(num>>2)&0x3f;
  graph[index].yticks=(num>>8)&0x3f;
  graph[index].xp=xp;
  graph[index].yp=yp;
  graph[index].xs=xs;
  graph[index].ys=ys;
  if (!dec) dec=1;
  graph[index].decimation=dec;
  if (dec<0) {
    // is minutes per sweep prepare timing paramters
    graph[index].x_time=(-dec*60000)/(float)xs;
    graph[index].last_ms=millis()+graph[index].x_time;
  }
  graph[index].ymin=ymin;
  graph[index].ymax=ymax;
  graph[index].range=(ymax-ymin)/ys;
  graph[index].xcnt=0;
  graph[index].dcnt=0;
  graph[index].summ=0;
  if (graph[index].values) free(graph[index].values);
  graph[index].values=(uint8_t*) calloc(1,xs+2);
  // start from zero
  graph[index].values[0]=0;
  // draw rectangle
  Draw_Rectangle(xp,yp,xs,ys);
  // clr inside
  ClrGraph(index);

}

// check if to advance GRAPH
void DisplayCheckGraph() {
  int16_t count;
  for (count=0;count<NUM_GRAPHS;count++) {
    if (graph[count].decimation<0) {
      // if time over add value
      while (millis()>graph[count].last_ms) {
        graph[count].last_ms+=graph[count].x_time;
        uint8_t val;
        if (graph[count].dcnt) {
          val=graph[count].summ/graph[count].dcnt;
          graph[count].dcnt=0;
          graph[count].summ=0;
          graph[count].last_val=val;
        } else {
          val=graph[count].last_val;
        }
        AddGraph(count,val);
      }
    }
  }
}

// add next value to graph
void AddGraph(uint8_t num,uint8_t val) {
  graph[num].xcnt++;
  if (graph[num].xcnt>graph[num].xs) {
    graph[num].xcnt=graph[num].xs;
    // clr area, shift and redraw graph
    // draw rectangle
    Draw_Rectangle(graph[num].xp,graph[num].yp,graph[num].xs,graph[num].ys);
    ClrGraph(num);
    int16_t count;
    for (count=0;count<graph[num].xs-1;count++) {
      graph[num].values[count]=graph[num].values[count+1];
    }
    graph[num].values[graph[num].xcnt-1]=val;
    for (count=0;count<graph[num].xs-1;count++) {
      DrawLine(graph[num].xp+count,graph[num].yp+graph[num].ys-graph[num].values[count]-1,graph[num].xp+count+1,graph[num].yp+graph[num].ys-graph[num].values[count+1]-1);
    }
  } else {
    // add value and draw a single line
    graph[num].values[graph[num].xcnt]=val;
    DrawLine(graph[num].xp+graph[num].xcnt-1,graph[num].yp+graph[num].ys-graph[num].values[graph[num].xcnt-1]-1,graph[num].xp+graph[num].xcnt,graph[num].yp+graph[num].ys-graph[num].values[graph[num].xcnt]-1);
  }
}

// add next value
void AddValue(uint8_t num,float fval) {
  // not yet defined ???
  num=num%NUM_GRAPHS;
  if (!graph[num].values) return;

  if (fval>graph[num].ymax) fval=graph[num].ymax;
  if (fval<graph[num].ymin) fval=graph[num].ymin;

  int16_t val;
  val=(fval-graph[num].ymin)/graph[num].range;

  if (val>graph[num].ys-1) val=graph[num].ys-1;
  if (val<0) val=0;

  // summ values
  graph[num].summ+=val;
  graph[num].dcnt++;

  if (graph[num].decimation>0) {
    if (graph[num].dcnt>=graph[num].decimation) {
      graph[num].dcnt=0;
      // calc average
      val=graph[num].summ/graph[num].decimation;
      graph[num].summ=0;
      // add to graph
      AddGraph(num,val);
    }
  }
}

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

// get asci float number
uint8_t fatoiv(char *cp,float *res) {
  uint8_t index=0;
  *res=CharToDouble(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-') || (*cp=='.')) {
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
  //display_ready=0;
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t disp_len = strlen(D_CMND_DISPLAY);  // Prep for string length change

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_DISPLAY), disp_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic +disp_len, kDisplayCommands);
    if (-1 == command_code) {
      serviced = false;  // Unknown command
    }
    else if (CMND_DISP_TEXT == command_code) {
      if (display_ready) {

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
                  case 'L':
                     // any line to
                     var=atoiv(cp,&temp);
                     cp+=var;
                     cp++;
                     var=atoiv(cp,&temp1);
                     cp+=var;
                     DrawLine(xpos,ypos,temp,temp1);
                     xpos+=temp;
                     ypos+=temp1;
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
                    Draw_FilledRectangle(xpos,ypos,temp,temp1,0);
                    break;
                  case 'G':
                    // define graph
                    { int16_t num,gxp,gyp,gxs,gys,dec;
                        float ymin,ymax;
                        var=atoiv(cp,&num);
                        cp+=var;
                        cp++;
                        var=atoiv(cp,&gxp);
                        cp+=var;
                        cp++;
                        var=atoiv(cp,&gyp);
                        cp+=var;
                        cp++;
                        var=atoiv(cp,&gxs);
                        cp+=var;
                        cp++;
                        var=atoiv(cp,&gys);
                        cp+=var;
                        cp++;
                        var=atoiv(cp,&dec);
                        cp+=var;
                        cp++;
                        var=fatoiv(cp,&ymin);
                        cp+=var;
                        cp++;
                        var=fatoiv(cp,&ymax);
                        cp+=var;
                        DefineGraph(num,gxp,gyp,gxs,gys,dec,ymin,ymax);
                    }
                    break;
                  case 'g':
                    { float temp;
                      int16_t num;
                      var=atoiv(cp,&num);
                      cp+=var;
                      cp++;
                      var=fatoiv(cp,&temp);
                      cp+=var;
                      AddValue(num,temp);
                    }
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
    case FUNC_EVERY_SECOND:
      DisplayCheckGraph();
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
