/*
  xdrv_13_display.ino - Display support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY

#define XDRV_13       13

#include <renderer.h>
#include <FT6236.h>

Renderer *renderer;

enum ColorType { COLOR_BW, COLOR_COLOR };

#ifndef MAXBUTTONS
#define MAXBUTTONS 16
#endif

#ifdef USE_TOUCH_BUTTONS
VButton *buttons[MAXBUTTONS];
#endif

// drawing color is WHITE
// on epaper the whole display buffer is transfered inverted this results in white paper
uint16_t fg_color = 1;
uint16_t bg_color = 0;
uint8_t color_type = COLOR_BW;
uint8_t auto_draw=1;

const uint8_t DISPLAY_MAX_DRIVERS = 16;        // Max number of display drivers/models supported by xdsp_interface.ino
const uint8_t DISPLAY_MAX_COLS = 44;           // Max number of columns allowed with command DisplayCols
const uint8_t DISPLAY_MAX_ROWS = 32;           // Max number of lines allowed with command DisplayRows

const uint8_t DISPLAY_LOG_ROWS = 32;           // Number of lines in display log buffer

#define D_PRFX_DISPLAY "Display"
#define D_CMND_DISP_ADDRESS "Address"
#define D_CMND_DISP_COLS "Cols"
#define D_CMND_DISP_DIMMER "Dimmer"
#define D_CMND_DISP_MODE "Mode"
#define D_CMND_DISP_MODEL "Model"
#define D_CMND_DISP_REFRESH "Refresh"
#define D_CMND_DISP_ROWS "Rows"
#define D_CMND_DISP_SIZE "Size"
#define D_CMND_DISP_FONT "Font"
#define D_CMND_DISP_ROTATE "Rotate"
#define D_CMND_DISP_TEXT "Text"
#define D_CMND_DISP_WIDTH "Width"
#define D_CMND_DISP_HEIGHT "Height"

enum XdspFunctions { FUNC_DISPLAY_INIT_DRIVER, FUNC_DISPLAY_INIT, FUNC_DISPLAY_EVERY_50_MSECOND, FUNC_DISPLAY_EVERY_SECOND,
                     FUNC_DISPLAY_MODEL, FUNC_DISPLAY_MODE, FUNC_DISPLAY_POWER,
                     FUNC_DISPLAY_CLEAR, FUNC_DISPLAY_DRAW_FRAME,
                     FUNC_DISPLAY_DRAW_HLINE, FUNC_DISPLAY_DRAW_VLINE, FUNC_DISPLAY_DRAW_LINE,
                     FUNC_DISPLAY_DRAW_CIRCLE, FUNC_DISPLAY_FILL_CIRCLE,
                     FUNC_DISPLAY_DRAW_RECTANGLE, FUNC_DISPLAY_FILL_RECTANGLE,
                     FUNC_DISPLAY_TEXT_SIZE, FUNC_DISPLAY_FONT_SIZE, FUNC_DISPLAY_ROTATION, FUNC_DISPLAY_DRAW_STRING, FUNC_DISPLAY_ONOFF };

enum DisplayInitModes { DISPLAY_INIT_MODE, DISPLAY_INIT_PARTIAL, DISPLAY_INIT_FULL };

const char kDisplayCommands[] PROGMEM = D_PRFX_DISPLAY "|"  // Prefix
  "|" D_CMND_DISP_MODEL "|" D_CMND_DISP_WIDTH "|" D_CMND_DISP_HEIGHT "|" D_CMND_DISP_MODE "|" D_CMND_DISP_REFRESH "|"
  D_CMND_DISP_DIMMER "|" D_CMND_DISP_COLS "|" D_CMND_DISP_ROWS "|" D_CMND_DISP_SIZE "|" D_CMND_DISP_FONT "|"
  D_CMND_DISP_ROTATE "|" D_CMND_DISP_TEXT "|" D_CMND_DISP_ADDRESS ;

void (* const DisplayCommand[])(void) PROGMEM = {
  &CmndDisplay, &CmndDisplayModel, &CmndDisplayWidth, &CmndDisplayHeight, &CmndDisplayMode, &CmndDisplayRefresh,
  &CmndDisplayDimmer, &CmndDisplayColumns, &CmndDisplayRows, &CmndDisplaySize, &CmndDisplayFont,
  &CmndDisplayRotate, &CmndDisplayText, &CmndDisplayAddress };

char *dsp_str;

uint16_t dsp_x;
uint16_t dsp_y;
uint16_t dsp_x2;
uint16_t dsp_y2;
uint16_t dsp_rad;
uint16_t dsp_color;
int16_t dsp_len;
int16_t disp_xpos = 0;
int16_t disp_ypos = 0;

uint8_t disp_power = 0;
uint8_t disp_device = 0;
uint8_t disp_refresh = 1;
uint8_t disp_autodraw = 1;
uint8_t dsp_init;
uint8_t dsp_font;
uint8_t dsp_flag;
uint8_t dsp_on;

#ifdef USE_DISPLAY_MODES1TO5

char **disp_log_buffer;
char **disp_screen_buffer;
char disp_temp[2];    // C or F
char disp_pres[5];   // hPa or mmHg

uint8_t disp_log_buffer_cols = 0;
uint8_t disp_log_buffer_idx = 0;
uint8_t disp_log_buffer_ptr = 0;
uint8_t disp_screen_buffer_cols = 0;
uint8_t disp_screen_buffer_rows = 0;
bool disp_subscribed = false;

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************/

void DisplayInit(uint8_t mode)
{
  if (renderer)  {
    renderer->DisplayInit(mode, Settings.display_size, Settings.display_rotate, Settings.display_font);
  }
  else {
    dsp_init = mode;
    XdspCall(FUNC_DISPLAY_INIT);
  }
}

void DisplayClear(void)
{
  XdspCall(FUNC_DISPLAY_CLEAR);
}

void DisplayDrawHLine(uint16_t x, uint16_t y, int16_t len, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_len = len;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_HLINE);
}

void DisplayDrawVLine(uint16_t x, uint16_t y, int16_t len, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_len = len;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_VLINE);
}

void DisplayDrawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_x2 = x2;
  dsp_y2 = y2;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_LINE);
}

void DisplayDrawCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_rad = rad;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_CIRCLE);
}

void DisplayDrawFilledCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_rad = rad;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_FILL_CIRCLE);
}

void DisplayDrawRectangle(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_x2 = x2;
  dsp_y2 = y2;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_DRAW_RECTANGLE);
}

void DisplayDrawFilledRectangle(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color)
{
  dsp_x = x;
  dsp_y = y;
  dsp_x2 = x2;
  dsp_y2 = y2;
  dsp_color = color;
  XdspCall(FUNC_DISPLAY_FILL_RECTANGLE);
}

void DisplayDrawFrame(void)
{
  XdspCall(FUNC_DISPLAY_DRAW_FRAME);
}

void DisplaySetSize(uint8_t size)
{
  Settings.display_size = size &3;
  XdspCall(FUNC_DISPLAY_TEXT_SIZE);
}

void DisplaySetFont(uint8_t font)
{
  Settings.display_font = font &3;
  XdspCall(FUNC_DISPLAY_FONT_SIZE);
}

void DisplaySetRotation(uint8_t rotation)
{
  Settings.display_rotate = rotation &3;
  XdspCall(FUNC_DISPLAY_ROTATION);
}

void DisplayDrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  dsp_x = x;
  dsp_y = y;
  dsp_str = str;
  dsp_color = color;
  dsp_flag = flag;
  XdspCall(FUNC_DISPLAY_DRAW_STRING);
}

void DisplayOnOff(uint8_t on)
{
  dsp_on = on;
  XdspCall(FUNC_DISPLAY_ONOFF);
}

/*-------------------------------------------------------------------------------------------*/

// get asci float number
uint8_t fatoiv(char *cp,float *res) {
  uint8_t index=0;
  *res=CharToFloat(cp);
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

// get asci number until delimiter and return asci number lenght and value
uint8_t atoiv(char *cp, int16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
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

// get asci number until delimiter and return asci number lenght and value
uint8_t atoiV(char *cp, uint16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if (*cp>='0' && *cp<='9') {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// right align string
void alignright(char *string) {
  uint16_t slen=strlen(string);
  uint16_t len=slen;
  while (len) {
    // count spaces to the right
    if (string[len-1]!=' ') {
      break;
    }
    len--;
  }
  uint16_t diff=slen-len;
  if (diff>0) {
    // move string
    memmove(&string[diff],string,len);
    memset(string,' ',diff);
  }
}

char *get_string(char *buff,uint8_t len,char *cp) {
uint8_t index=0;
  while (*cp!=':') {
    buff[index]=*cp++;
    index++;
    if (index>=len) break;
  }
  buff[index]=0;
  cp++;
  return cp;
}

#define ESCAPE_CHAR '~'

// decode text escapes, 1 hexbyte assumed
void decode_te(char *line) {
  char sbuf[3],*cp;
  while (*line) {
    if (*line==ESCAPE_CHAR) {
      cp=line+1;
      if (*cp!=0 && *cp==ESCAPE_CHAR) {
        // escape escape, discard one
        memmove(cp,cp+1,strlen(cp));
      } else {
        // escape HH
        if (strlen(cp)<2) {
          // illegal lenght, ignore
          return;
        }
        // take 2 hex chars
        sbuf[0]=*(cp);
        sbuf[1]=*(cp+1);
        sbuf[2]=0;
        *line=strtol(sbuf,0,16);
        // must shift string 2 bytes shift zero also
        memmove(cp,cp+2,strlen(cp)-1);
      }
    }
    line++;
  }
}

/*-------------------------------------------------------------------------------------------*/

#define DISPLAY_BUFFER_COLS    128          // Max number of characters in linebuf

void DisplayText(void)
{
  uint8_t lpos;
  uint8_t escape = 0;
  uint8_t var;
  int16_t lin = 0;
  int16_t col = 0;
  int16_t fill = 0;
  int16_t temp;
  int16_t temp1;
  float ftemp;

  char linebuf[DISPLAY_BUFFER_COLS];
  char *dp = linebuf;
  char *cp = XdrvMailbox.data;

  memset(linebuf, ' ', sizeof(linebuf));
  linebuf[sizeof(linebuf)-1] = 0;
  *dp = 0;

  while (*cp) {
    if (!escape) {
      // check for escape
      if (*cp == '[') {
        escape = 1;
        cp++;
        // if string in buffer print it
        if ((uint32_t)dp - (uint32_t)linebuf) {
          if (!fill) { *dp = 0; }
          if (col > 0 && lin > 0) {
            // use col and lin
            if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
            else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
          } else {
            // use disp_xpos, disp_ypos
            if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
            else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
          }
          memset(linebuf, ' ', sizeof(linebuf));
          linebuf[sizeof(linebuf)-1] = 0;
          dp = linebuf;
        }
      } else {
        // copy chars
        if (dp < (linebuf + DISPLAY_BUFFER_COLS)) { *dp++ = *cp++; }
      }
    } else {
      // check escapes
      if (*cp == ']') {
        escape = 0;
        cp++;
      } else {
        // analyze escapes
        switch (*cp++) {
          case 'z':
            // clear display
            if (!renderer) DisplayClear();
            else renderer->fillScreen(bg_color);
            disp_xpos = 0;
            disp_ypos = 0;
            col = 0;
            lin = 0;
            break;
          case 'i':
            // init display with partial update
            DisplayInit(DISPLAY_INIT_PARTIAL);
            break;
          case 'I':
            // init display with full refresh
            DisplayInit(DISPLAY_INIT_FULL);
            break;
          case 'o':
            if (!renderer) {
              DisplayOnOff(0);
            } else {
              renderer->DisplayOnff(0);
            }
            break;
          case 'O':
            if (!renderer) {
              DisplayOnOff(1);
            } else {
              renderer->DisplayOnff(1);
            }
            break;
          case 'x':
            // set disp_xpos
            var = atoiv(cp, &disp_xpos);
            cp += var;
            break;
          case 'y':
            // set disp_ypos
            var = atoiv(cp, &disp_ypos);
            cp += var;
            break;
          case 'l':
            // text line lxx
            var = atoiv(cp, &lin);
            cp += var;
            //display.setCursor(display.getCursorX(),(lin-1)*font_y*txtsize);
            break;
          case 'c':
            // text column cxx
            var = atoiv(cp, &col);
            cp += var;
            //display.setCursor((col-1)*font_x*txtsize,display.getCursorY());
            break;
          case 'C':
            // text color cxx
            if (*cp=='i') {
              // color index 0-18
              cp++;
              var = atoiv(cp, &temp);
              if (renderer) ftemp=renderer->GetColorFromIndex(temp);
            } else {
              // float because it must handle unsigned 16 bit
              var = fatoiv(cp,&ftemp);
            }
            fg_color=ftemp;
            cp += var;
            if (renderer) renderer->setTextColor(fg_color,bg_color);
            break;
          case 'B':
            // bg color Bxx
            if (*cp=='i') {
              // color index 0-18
              cp++;
              var = atoiv(cp, &temp);
              if (renderer) ftemp=renderer->GetColorFromIndex(temp);
            } else {
              var = fatoiv(cp,&ftemp);
            }
            bg_color=ftemp;
            cp += var;
            if (renderer) renderer->setTextColor(fg_color,bg_color);
            break;
          case 'p':
            // pad field with spaces fxx
            var = atoiv(cp, &fill);
            cp += var;
            linebuf[fill] = 0;
            break;
#if defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)
          case 'P':
            { char *ep=strchr(cp,':');
             if (ep) {
               *ep=0;
               ep++;
               Draw_RGB_Bitmap(cp,disp_xpos,disp_ypos);
               cp=ep;
             }
            }
            break;
#endif
          case 'h':
            // hor line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
              else DisplayDrawHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastHLine(disp_xpos, disp_ypos, temp, fg_color);
              else DisplayDrawHLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_xpos += temp;
            break;
          case 'v':
            // vert line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
              else DisplayDrawVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos, temp, fg_color);
              else DisplayDrawVLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_ypos += temp;
            break;
          case 'L':
            // any line to
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->writeLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            else DisplayDrawLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            disp_xpos += temp;
            disp_ypos += temp1;
            break;
          case 'k':
            // circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->drawCircle(disp_xpos, disp_ypos, temp, fg_color);
            else DisplayDrawCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'K':
            // filled circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->fillCircle(disp_xpos, disp_ypos, temp, fg_color);
            else DisplayDrawFilledCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'r':
            // rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->drawRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            else DisplayDrawRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'R':
            // filled rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->fillRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'u':
            // rounded rectangle
            { int16_t rad;
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            cp++;
            var = atoiv(cp, &rad);
            cp += var;
            if (renderer) renderer->drawRoundRect(disp_xpos, disp_ypos, temp, temp1, rad, fg_color);
              //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            }
            break;
          case 'U':
            // rounded rectangle
            { int16_t rad;
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            cp++;
            var = atoiv(cp, &rad);
            cp += var;
            if (renderer) renderer->fillRoundRect(disp_xpos, disp_ypos, temp, temp1, rad, fg_color);
                  //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            }
            break;

          case 't':
            if (*cp=='S') {
              cp++;
              if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
                snprintf_P(dp, 9, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
                dp += 8;
              }
            } else {
              if (dp < (linebuf + DISPLAY_BUFFER_COLS) -5) {
                snprintf_P(dp, 6, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute);
                dp += 5;
              }
            }
            break;
          case 'T':
            if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
              snprintf_P(dp, 9, PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year%2000);
              dp += 8;
            }
            break;
          case 'd':
            // force draw grafics buffer
            if (renderer) renderer->Updateframe();
            else DisplayDrawFrame();
            break;
          case 'D':
            // set auto draw mode
            auto_draw=*cp&3;
            if (renderer) renderer->setDrawMode(auto_draw>>1);
            cp += 1;
            break;
          case 's':
            // size sx
            if (renderer) renderer->setTextSize(*cp&7);
            else DisplaySetSize(*cp&3);
            cp += 1;
            break;
          case 'f':
            // font sx
            if (renderer) renderer->setTextFont(*cp&7);
            else DisplaySetFont(*cp&7);
            cp += 1;
            break;
          case 'a':
            // rotation angle
            if (renderer) renderer->setRotation(*cp&3);
            else DisplaySetRotation(*cp&3);
            cp+=1;
            break;

#ifdef USE_GRAPH
          case 'G':
            // define graph
            if (*cp=='d') {
              cp++;
              var=atoiv(cp,&temp);
              cp+=var;
              cp++;
              var=atoiv(cp,&temp1);
              cp+=var;
              RedrawGraph(temp,temp1);
              break;
            }
#if defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)
            if (*cp=='s') {
              cp++;
              var=atoiv(cp,&temp);
              cp+=var;
              cp++;
              // path
              char bbuff[128];
              cp=get_string(bbuff,sizeof(bbuff),cp);
              Save_graph(temp,bbuff);
              break;
            }
            if (*cp=='r') {
              cp++;
              var=atoiv(cp,&temp);
              cp+=var;
              cp++;
              // path
              char bbuff[128];
              cp=get_string(bbuff,sizeof(bbuff),cp);
              Restore_graph(temp,bbuff);
              break;
            }
#endif
            { int16_t num,gxp,gyp,gxs,gys,dec,icol;
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
              if (color_type==COLOR_COLOR) {
                // color graph requires channel color
                cp++;
                var=atoiv(cp,&icol);
                cp+=var;
              } else {
                icol=0;
              }
              DefineGraph(num,gxp,gyp,gxs,gys,dec,ymin,ymax,icol);
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
#endif

#ifdef USE_AWATCH
          case 'w':
              var = atoiv(cp, &temp);
              cp += var;
              DrawAClock(temp);
              break;
#endif

#ifdef USE_TOUCH_BUTTONS
          case 'b':
          { int16_t num,gxp,gyp,gxs,gys,outline,fill,textcolor,textsize;
            var=atoiv(cp,&num);
            cp+=var;
            cp++;
            uint8_t bflags=num>>8;
            num=num%MAXBUTTONS;
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
            var=atoiv(cp,&outline);
            cp+=var;
            cp++;
            var=atoiv(cp,&fill);
            cp+=var;
            cp++;
            var=atoiv(cp,&textcolor);
            cp+=var;
            cp++;
            var=atoiv(cp,&textsize);
            cp+=var;
            cp++;
            // text itself
            char bbuff[32];
            cp=get_string(bbuff,sizeof(bbuff),cp);

            if (buttons[num]) {
              delete buttons[num];
            }
            if (renderer) {
              buttons[num]= new VButton();
              if (buttons[num]) {
                buttons[num]->vpower=bflags;
                buttons[num]->initButtonUL(renderer,gxp,gyp,gxs,gys,renderer->GetColorFromIndex(outline),\
                renderer->GetColorFromIndex(fill),renderer->GetColorFromIndex(textcolor),bbuff,textsize);
                if (!bflags) {
                  // power button
                  buttons[num]->xdrawButton(bitRead(power,num));
                } else {
                  // virtual button
                  buttons[num]->vpower&=0x7f;
                  buttons[num]->xdrawButton(buttons[num]->vpower&0x80);
                }
              }
            }
          }
          break;
#endif
          default:
            // unknown escape
            Response_P(PSTR("Unknown Escape"));
            goto exit;
            break;
        }
      }
    }
  }
  exit:
  // now draw buffer
    decode_te(linebuf);
    if ((uint32_t)dp - (uint32_t)linebuf) {
      if (!fill) *dp = 0;
      else linebuf[abs(fill)] = 0;
      if (fill<0) {
        // right align
        alignright(linebuf);
      }
      if (col > 0 && lin > 0) {
        // use col and lin
        if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
        else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
      } else {
        // use disp_xpos, disp_ypos
        if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
        else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
      }
    }
    // draw buffer
    if (auto_draw&1) {
      if (renderer) renderer->Updateframe();
      else DisplayDrawFrame();
    }
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void DisplayClearScreenBuffer(void)
{
  if (disp_screen_buffer_cols) {
    for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
      memset(disp_screen_buffer[i], 0, disp_screen_buffer_cols);
    }
  }
}

void DisplayFreeScreenBuffer(void)
{
  if (disp_screen_buffer != nullptr) {
    for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
      if (disp_screen_buffer[i] != nullptr) { free(disp_screen_buffer[i]); }
    }
    free(disp_screen_buffer);
    disp_screen_buffer_cols = 0;
    disp_screen_buffer_rows = 0;
  }
}

void DisplayAllocScreenBuffer(void)
{
  if (!disp_screen_buffer_cols) {
    disp_screen_buffer_rows = Settings.display_rows;
    disp_screen_buffer = (char**)malloc(sizeof(*disp_screen_buffer) * disp_screen_buffer_rows);
    if (disp_screen_buffer != nullptr) {
      for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
        disp_screen_buffer[i] = (char*)malloc(sizeof(*disp_screen_buffer[i]) * (Settings.display_cols[0] +1));
        if (disp_screen_buffer[i] == nullptr) {
          DisplayFreeScreenBuffer();
          break;
        }
      }
    }
    if (disp_screen_buffer != nullptr) {
      disp_screen_buffer_cols = Settings.display_cols[0] +1;
      DisplayClearScreenBuffer();
    }
  }
}

void DisplayReAllocScreenBuffer(void)
{
  DisplayFreeScreenBuffer();
  DisplayAllocScreenBuffer();
}

void DisplayFillScreen(uint32_t line)
{
  uint32_t len = disp_screen_buffer_cols - strlen(disp_screen_buffer[line]);
  if (len) {
    memset(disp_screen_buffer[line] + strlen(disp_screen_buffer[line]), 0x20, len);
    disp_screen_buffer[line][disp_screen_buffer_cols -1] = 0;
  }
}

/*-------------------------------------------------------------------------------------------*/

void DisplayClearLogBuffer(void)
{
  if (disp_log_buffer_cols) {
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      memset(disp_log_buffer[i], 0, disp_log_buffer_cols);
    }
  }
}

void DisplayFreeLogBuffer(void)
{
  if (disp_log_buffer != nullptr) {
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      if (disp_log_buffer[i] != nullptr) { free(disp_log_buffer[i]); }
    }
    free(disp_log_buffer);
    disp_log_buffer_cols = 0;
  }
}

void DisplayAllocLogBuffer(void)
{
  if (!disp_log_buffer_cols) {
    disp_log_buffer = (char**)malloc(sizeof(*disp_log_buffer) * DISPLAY_LOG_ROWS);
    if (disp_log_buffer != nullptr) {
      for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
        disp_log_buffer[i] = (char*)malloc(sizeof(*disp_log_buffer[i]) * (Settings.display_cols[0] +1));
        if (disp_log_buffer[i] == nullptr) {
          DisplayFreeLogBuffer();
          break;
        }
      }
    }
    if (disp_log_buffer != nullptr) {
      disp_log_buffer_cols = Settings.display_cols[0] +1;
      DisplayClearLogBuffer();
    }
  }
}

void DisplayReAllocLogBuffer(void)
{
  DisplayFreeLogBuffer();
  DisplayAllocLogBuffer();
}

void DisplayLogBufferAdd(char* txt)
{
  if (disp_log_buffer_cols) {
    strlcpy(disp_log_buffer[disp_log_buffer_idx], txt, disp_log_buffer_cols);  // This preserves the % sign where printf won't
    disp_log_buffer_idx++;
    if (DISPLAY_LOG_ROWS == disp_log_buffer_idx) { disp_log_buffer_idx = 0; }
  }
}

char* DisplayLogBuffer(char temp_code)
{
  char* result = nullptr;
  if (disp_log_buffer_cols) {
    if (disp_log_buffer_idx != disp_log_buffer_ptr) {
      result = disp_log_buffer[disp_log_buffer_ptr];
      disp_log_buffer_ptr++;
      if (DISPLAY_LOG_ROWS == disp_log_buffer_ptr) { disp_log_buffer_ptr = 0; }

      char *pch = strchr(result, '~');  // = 0x7E (~) Replace degrees character (276 octal)
      if (pch != nullptr) { result[pch - result] = temp_code; }
    }
  }
  return result;
}

void DisplayLogBufferInit(void)
{
  if (Settings.display_mode) {
    disp_log_buffer_idx = 0;
    disp_log_buffer_ptr = 0;
    disp_refresh = Settings.display_refresh;

    snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%c"), TempUnit());
    snprintf_P(disp_pres, sizeof(disp_pres), PressureUnit().c_str());

    DisplayReAllocLogBuffer();

    char buffer[40];
    snprintf_P(buffer, sizeof(buffer), PSTR(D_VERSION " %s%s"), my_version, my_image);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR("Display mode %d"), Settings.display_mode);
    DisplayLogBufferAdd(buffer);

    snprintf_P(buffer, sizeof(buffer), PSTR(D_CMND_HOSTNAME " %s"), my_hostname);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_SSID " %s"), SettingsText(SET_STASSID1 + Settings.sta_active));
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_MAC " %s"), WiFi.macAddress().c_str());
    DisplayLogBufferAdd(buffer);
    if (!global_state.wifi_down) {
      snprintf_P(buffer, sizeof(buffer), PSTR("IP %s"), WiFi.localIP().toString().c_str());
      DisplayLogBufferAdd(buffer);
      snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_RSSI " %d%%"), WifiGetRssiAsQuality(WiFi.RSSI()));
      DisplayLogBufferAdd(buffer);
    }
  }
}

/*********************************************************************************************\
 * Sensors
\*********************************************************************************************/

enum SensorQuantity {
  JSON_TEMPERATURE,
  JSON_HUMIDITY, JSON_LIGHT, JSON_NOISE, JSON_AIRQUALITY,
  JSON_PRESSURE, JSON_PRESSUREATSEALEVEL,
  JSON_ILLUMINANCE,
  JSON_GAS,
  JSON_YESTERDAY, JSON_TOTAL, JSON_TODAY,
  JSON_PERIOD,
  JSON_POWERFACTOR, JSON_COUNTER, JSON_ANALOG_INPUT, JSON_UV_LEVEL,
  JSON_CURRENT,
  JSON_VOLTAGE,
  JSON_POWERUSAGE,
  JSON_CO2,
  JSON_FREQUENCY };
const char kSensorQuantity[] PROGMEM =
  D_JSON_TEMPERATURE "|"                                                        // degrees
  D_JSON_HUMIDITY "|" D_JSON_LIGHT "|" D_JSON_NOISE "|" D_JSON_AIRQUALITY "|"   // percentage
  D_JSON_PRESSURE "|" D_JSON_PRESSUREATSEALEVEL "|"                             // hPa
  D_JSON_ILLUMINANCE "|"                                                        // lx
  D_JSON_GAS "|"                                                                // kOhm
  D_JSON_YESTERDAY "|" D_JSON_TOTAL "|" D_JSON_TODAY "|"                        // kWh
  D_JSON_PERIOD "|"                                                             // Wh
  D_JSON_POWERFACTOR "|" D_JSON_COUNTER "|" D_JSON_ANALOG_INPUT "|" D_JSON_UV_LEVEL "|"                 // No unit
  D_JSON_CURRENT "|"                                                            // Ampere
  D_JSON_VOLTAGE "|"                                                            // Volt
  D_JSON_POWERUSAGE "|"                                                         // Watt
  D_JSON_CO2 "|"                                                                // ppm
  D_JSON_FREQUENCY ;                                                            // Hz

void DisplayJsonValue(const char* topic, const char* device, const char* mkey, const char* value)
{
  char quantity[TOPSZ];
  char buffer[Settings.display_cols[0] +1];
  char spaces[Settings.display_cols[0]];
  char source[Settings.display_cols[0] - Settings.display_cols[1]];
  char svalue[Settings.display_cols[1] +1];

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("DisplayJsonValue"));
#endif

  memset(spaces, 0x20, sizeof(spaces));
  spaces[sizeof(spaces) -1] = '\0';
  snprintf_P(source, sizeof(source), PSTR("%s%s%s%s"), topic, (strlen(topic))?"/":"", mkey, spaces);  // pow1/Voltage or Voltage if topic is empty (local sensor)

  int quantity_code = GetCommandCode(quantity, sizeof(quantity), mkey, kSensorQuantity);
  if ((-1 == quantity_code) || !strcmp_P(mkey, S_RSLT_POWER)) {  // Ok: Power, Not ok: POWER
    return;
  }
  if (JSON_TEMPERATURE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s~%s"), value, disp_temp);
  }
  else if ((quantity_code >= JSON_HUMIDITY) && (quantity_code <= JSON_AIRQUALITY)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%%"), value);
  }
  else if ((quantity_code >= JSON_PRESSURE) && (quantity_code <= JSON_PRESSUREATSEALEVEL)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), value, disp_pres);
  }
  else if (JSON_ILLUMINANCE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_LUX), value);
  }
  else if (JSON_GAS == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_KILOOHM), value);
  }
  else if ((quantity_code >= JSON_YESTERDAY) && (quantity_code <= JSON_TODAY)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_KILOWATTHOUR), value);
  }
  else if (JSON_PERIOD == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_WATTHOUR), value);
  }
  else if ((quantity_code >= JSON_POWERFACTOR) && (quantity_code <= JSON_UV_LEVEL)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s"), value);
  }
  else if (JSON_CURRENT == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_AMPERE), value);
  }
  else if (JSON_VOLTAGE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_VOLT), value);
  }
  else if (JSON_POWERUSAGE == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_WATT), value);
  }
  else if (JSON_CO2 == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_PARTS_PER_MILLION), value);
  }
  else if (JSON_FREQUENCY == quantity_code) {
    snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_HERTZ), value);
  }
  snprintf_P(buffer, sizeof(buffer), PSTR("%s %s"), source, svalue);

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "mkey [%s], source [%s], value [%s], quantity_code %d, log_buffer [%s]"), mkey, source, value, quantity_code, buffer);

  DisplayLogBufferAdd(buffer);
}

void DisplayAnalyzeJson(char *topic, char *json)
{
// //tele/pow2/STATE    {"Time":"2017-09-20T11:53:03", "Uptime":10, "Vcc":3.123, "POWER":"ON", "Wifi":{"AP":2, "SSId":"indebuurt2", "RSSI":68, "APMac":"00:22:6B:FE:8E:20"}}
// //tele/pow2/ENERGY   {"Time":"2017-09-20T11:53:03", "Total":6.522, "Yesterday":0.150, "Today":0.073, "Period":0.5, "Power":12.1, "Factor":0.56, "Voltage":210.1, "Current":0.102}

// tele/pow1/SENSOR = {"Time":"2018-01-02T17:13:17","ENERGY":{"Total":13.091,"Yesterday":0.060,"Today":0.046,"Period":0.2,"Power":9.8,"Factor":0.49,"Voltage":206.8,"Current":0.096}}
// tele/dual/STATE    {"Time":"2017-09-20T11:53:03","Uptime":25,"Vcc":3.178,"POWER1":"OFF","POWER2":"OFF","Wifi":{"AP":2,"SSId":"indebuurt2","RSSI":100,"APMac":"00:22:6B:FE:8E:20"}}
// tele/sc/SENSOR     {"Time":"2017-09-20T11:53:09","Temperature":24.0,"Humidity":16.0,"Light":30,"Noise":20,"AirQuality":100,"TempUnit":"C"}
// tele/rf1/SENSOR    {"Time":"2017-09-20T11:53:23","BH1750":{"Illuminance":57}}
// tele/wemos5/SENSOR {"Time":"2017-09-20T11:53:53","SHT1X":{"Temperature":20.1,"Humidity":58.9},"HTU21":{"Temperature":20.7,"Humidity":58.5},"BMP280":{"Temperature":21.6,"Pressure":1020.3},"TempUnit":"C"}
// tele/th1/SENSOR    {"Time":"2017-09-20T11:54:48","DS18B20":{"Temperature":49.7},"TempUnit":"C"}


//  char jsonStr[MESSZ];
//  strlcpy(jsonStr, json, sizeof(jsonStr));  // Save original before destruction by JsonObject
  String jsonStr = json;  // Move from stack to heap to fix watchdogs (20180626)

  StaticJsonBuffer<1024> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(jsonStr);
  if (root.success()) {

    const char *unit;
    unit = root[D_JSON_TEMPERATURE_UNIT];
    if (unit) {
      snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%s"), unit);  // C or F
    }
    unit = root[D_JSON_PRESSURE_UNIT];
    if (unit) {
      snprintf_P(disp_pres, sizeof(disp_pres), PSTR("%s"), unit);  // hPa or mmHg
    }

    for (JsonObject::iterator it = root.begin(); it != root.end(); ++it) {
      JsonVariant value = it->value;
      if (value.is<JsonObject>()) {
        JsonObject& Object2 = value;
        for (JsonObject::iterator it2 = Object2.begin(); it2 != Object2.end(); ++it2) {
          JsonVariant value2 = it2->value;
          if (value2.is<JsonObject>()) {
            JsonObject& Object3 = value2;
            for (JsonObject::iterator it3 = Object3.begin(); it3 != Object3.end(); ++it3) {
              const char* value = it3->value;
              if (value != nullptr) {  // "DHT11":{"Temperature":null,"Humidity":null} - ignore null as it will raise exception 28
                DisplayJsonValue(topic, it->key, it3->key, value);  // Sensor 56%
              }
            }
          } else {
            const char* value = it2->value;
            if (value != nullptr) {
              DisplayJsonValue(topic, it->key, it2->key, value);  // Sensor  56%
            }
          }
        }
      } else {
        const char* value = it->value;
        if (value != nullptr) {
          DisplayJsonValue(topic, it->key, it->key, value);  // Topic  56%
        }
      }
    }
  }
}

void DisplayMqttSubscribe(void)
{
/* Subscribe to tele messages only
 * Supports the following FullTopic formats
 * - %prefix%/%topic%
 * - home/%prefix%/%topic%
 * - home/level2/%prefix%/%topic% etc.
 */
  if (Settings.display_model && (Settings.display_mode &0x04)) {

    char stopic[TOPSZ];
    char ntopic[TOPSZ];

    ntopic[0] = '\0';
    strlcpy(stopic, SettingsText(SET_MQTT_FULLTOPIC), sizeof(stopic));
    char *tp = strtok(stopic, "/");
    while (tp != nullptr) {
      if (!strcmp_P(tp, MQTT_TOKEN_PREFIX)) {
        break;
      }
      strncat_P(ntopic, PSTR("+/"), sizeof(ntopic) - strlen(ntopic) -1);           // Add single-level wildcards
      tp = strtok(nullptr, "/");
    }
    strncat(ntopic, SettingsText(SET_MQTTPREFIX3), sizeof(ntopic) - strlen(ntopic) -1);  // Subscribe to tele messages
    strncat_P(ntopic, PSTR("/#"), sizeof(ntopic) - strlen(ntopic) -1);             // Add multi-level wildcard
    MqttSubscribe(ntopic);
    disp_subscribed = true;
  } else {
    disp_subscribed = false;
  }
}

bool DisplayMqttData(void)
{
  if (disp_subscribed) {
    char stopic[TOPSZ];

    snprintf_P(stopic, sizeof(stopic) , PSTR("%s/"), SettingsText(SET_MQTTPREFIX3));  // tele/
    char *tp = strstr(XdrvMailbox.topic, stopic);
    if (tp) {                                                // tele/tasmota/SENSOR
      if (Settings.display_mode &0x04) {
        tp = tp + strlen(stopic);                              // tasmota/SENSOR
        char *topic = strtok(tp, "/");                         // tasmota
        DisplayAnalyzeJson(topic, XdrvMailbox.data);
      }
      return true;
    }
  }
  return false;
}

void DisplayLocalSensor(void)
{
  if ((Settings.display_mode &0x02) && (0 == tele_period)) {
    char no_topic[1] = { 0 };
//    DisplayAnalyzeJson(mqtt_topic, mqtt_data);  // Add local topic
    DisplayAnalyzeJson(no_topic, mqtt_data);    // Discard any topic
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void DisplayInitDriver(void)
{
  XdspCall(FUNC_DISPLAY_INIT_DRIVER);

  if (renderer) {
    renderer->setTextFont(Settings.display_font);
    renderer->setTextSize(Settings.display_size);
  }


//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Display model %d"), Settings.display_model);

  if (Settings.display_model) {
    devices_present++;
    disp_device = devices_present;

#ifndef USE_DISPLAY_MODES1TO5
    Settings.display_mode = 0;
#else
    DisplayLogBufferInit();
#endif  // USE_DISPLAY_MODES1TO5
  }
}

void DisplaySetPower(void)
{
  disp_power = bitRead(XdrvMailbox.index, disp_device -1);

AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DSP: Power %d"), disp_power);

  if (Settings.display_model) {
    if (!renderer) {
      XdspCall(FUNC_DISPLAY_POWER);
    } else {
      renderer->DisplayOnff(disp_power);
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDisplay(void)
{
  Response_P(PSTR("{\"" D_PRFX_DISPLAY "\":{\"" D_CMND_DISP_MODEL "\":%d,\"" D_CMND_DISP_WIDTH "\":%d,\"" D_CMND_DISP_HEIGHT "\":%d,\""
    D_CMND_DISP_MODE "\":%d,\"" D_CMND_DISP_DIMMER "\":%d,\"" D_CMND_DISP_SIZE "\":%d,\"" D_CMND_DISP_FONT "\":%d,\""
    D_CMND_DISP_ROTATE "\":%d,\"" D_CMND_DISP_REFRESH "\":%d,\"" D_CMND_DISP_COLS "\":[%d,%d],\"" D_CMND_DISP_ROWS "\":%d}}"),
    Settings.display_model, Settings.display_width, Settings.display_height,
    Settings.display_mode, Settings.display_dimmer, Settings.display_size, Settings.display_font,
    Settings.display_rotate, Settings.display_refresh, Settings.display_cols[0], Settings.display_cols[1], Settings.display_rows);
}

void CmndDisplayModel(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < DISPLAY_MAX_DRIVERS)) {
    uint32_t last_display_model = Settings.display_model;
    Settings.display_model = XdrvMailbox.payload;
    if (XdspCall(FUNC_DISPLAY_MODEL)) {
      restart_flag = 2;  // Restart to re-init interface and add/Remove MQTT subscribe
    } else {
      Settings.display_model = last_display_model;
    }
  }
  ResponseCmndNumber(Settings.display_model);
}

void CmndDisplayWidth(void)
{
  if (XdrvMailbox.payload > 0) {
    if (XdrvMailbox.payload != Settings.display_width) {
      Settings.display_width = XdrvMailbox.payload;
      restart_flag = 2;  // Restart to re-init width
    }
  }
  ResponseCmndNumber(Settings.display_width);
}

void CmndDisplayHeight(void)
{
  if (XdrvMailbox.payload > 0) {
    if (XdrvMailbox.payload != Settings.display_height) {
      Settings.display_height = XdrvMailbox.payload;
      restart_flag = 2;  // Restart to re-init height
    }
  }
  ResponseCmndNumber(Settings.display_height);
}

void CmndDisplayMode(void)
{
#ifdef USE_DISPLAY_MODES1TO5
/*     Matrix               LCD / Oled                           TFT
 * 1 = Text up and time     Time
 * 2 = Date                 Local sensors                        Local sensors
 * 3 = Day                  Local sensors and time               Local sensors and time
 * 4 = Mqtt left and time   Mqtt (incl local) sensors            Mqtt (incl local) sensors
 * 5 = Mqtt up and time     Mqtt (incl local) sensors and time   Mqtt (incl local) sensors and time
*/
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
    uint32_t last_display_mode = Settings.display_mode;
    Settings.display_mode = XdrvMailbox.payload;

    if (disp_subscribed != (Settings.display_mode &0x04)) {
      restart_flag = 2;  // Restart to Add/Remove MQTT subscribe
    } else {
      if (last_display_mode && !Settings.display_mode) {  // Switch to mode 0
        DisplayInit(DISPLAY_INIT_MODE);
        if (renderer) renderer->fillScreen(bg_color);
        else DisplayClear();
      } else {
        DisplayLogBufferInit();
        DisplayInit(DISPLAY_INIT_MODE);
      }
    }
  }
#endif  // USE_DISPLAY_MODES1TO5
  ResponseCmndNumber(Settings.display_mode);
}

void CmndDisplayDimmer(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    Settings.display_dimmer = ((XdrvMailbox.payload +1) * 100) / 666;  // Correction for Domoticz (0 - 15)
    if (Settings.display_dimmer && !(disp_power)) {
      ExecuteCommandPower(disp_device, POWER_ON, SRC_DISPLAY);
    }
    else if (!Settings.display_dimmer && disp_power) {
      ExecuteCommandPower(disp_device, POWER_OFF, SRC_DISPLAY);
    }
    if (renderer) renderer->dim(Settings.display_dimmer);
  }
  ResponseCmndNumber(Settings.display_dimmer);
}

void CmndDisplaySize(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
    Settings.display_size = XdrvMailbox.payload;
    if (renderer) renderer->setTextSize(Settings.display_size);
    else DisplaySetSize(Settings.display_size);
  }
  ResponseCmndNumber(Settings.display_size);
}

void CmndDisplayFont(void)
{
  if ((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 4)) {
    Settings.display_font = XdrvMailbox.payload;
    if (renderer) renderer->setTextFont(Settings.display_font);
    else DisplaySetFont(Settings.display_font);
  }
  ResponseCmndNumber(Settings.display_font);
}

void CmndDisplayRotate(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 4)) {
    if (Settings.display_rotate != XdrvMailbox.payload) {
/*
      // Needs font info regarding height and width
      if ((Settings.display_rotate &1) != (XdrvMailbox.payload &1)) {
        uint8_t temp_rows = Settings.display_rows;
        Settings.display_rows = Settings.display_cols[0];
        Settings.display_cols[0] = temp_rows;
#ifdef USE_DISPLAY_MODES1TO5
        DisplayReAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
      }
*/
      Settings.display_rotate = XdrvMailbox.payload;
      DisplayInit(DISPLAY_INIT_MODE);
#ifdef USE_DISPLAY_MODES1TO5
      DisplayLogBufferInit();
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  ResponseCmndNumber(Settings.display_rotate);
}

void CmndDisplayText(void)
{
  if (disp_device && XdrvMailbox.data_len > 0) {
#ifndef USE_DISPLAY_MODES1TO5
    DisplayText();
#else
    if (!Settings.display_mode) {
      DisplayText();
    } else {
      DisplayLogBufferAdd(XdrvMailbox.data);
    }
#endif  // USE_DISPLAY_MODES1TO5
    ResponseCmndChar(XdrvMailbox.data);
  }
}

void CmndDisplayAddress(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
      Settings.display_address[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings.display_address[XdrvMailbox.index -1]);
  }
}

void CmndDisplayRefresh(void)
{
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 7)) {
    Settings.display_refresh = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.display_refresh);
}

void CmndDisplayColumns(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_COLS)) {
      Settings.display_cols[XdrvMailbox.index -1] = XdrvMailbox.payload;
#ifdef USE_DISPLAY_MODES1TO5
      if (1 == XdrvMailbox.index) {
        DisplayLogBufferInit();
        DisplayReAllocScreenBuffer();
      }
#endif  // USE_DISPLAY_MODES1TO5
    }
    ResponseCmndIdxNumber(Settings.display_cols[XdrvMailbox.index -1]);
  }
}

void CmndDisplayRows(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_ROWS)) {
    Settings.display_rows = XdrvMailbox.payload;
#ifdef USE_DISPLAY_MODES1TO5
    DisplayLogBufferInit();
    DisplayReAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
  }
  ResponseCmndNumber(Settings.display_rows);
}

/*********************************************************************************************\
 * optional drivers
\*********************************************************************************************/

#if defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)
void Draw_RGB_Bitmap(char *file,uint16_t xp, uint16_t yp) {
  if (!renderer) return;

  //if (!strstr(file,".RGB")) return;
  File fp;
  fp=SD.open(file,FILE_READ);
  if (!fp) return;
  uint16_t xsize;
  fp.read((uint8_t*)&xsize,2);
  uint16_t ysize;
  fp.read((uint8_t*)&ysize,2);

#if 1
#define XBUFF 128
  uint16_t xdiv=xsize/XBUFF;
  renderer->setAddrWindow(xp,yp,xp+xsize,yp+ysize);
  for(int16_t j=0; j<ysize; j++) {
    for(int16_t i=0; i<xsize; i+=XBUFF) {
      uint16_t rgb[XBUFF];
      uint16_t len=fp.read((uint8_t*)rgb,XBUFF*2);
      if (len>=2) renderer->pushColors(rgb,len/2,true);
    }
    OsWatchLoop();
  }
  renderer->setAddrWindow(0,0,0,0);
#else
  for(int16_t j=0; j<ysize; j++) {
    for(int16_t i=0; i<xsize; i++ ) {
      uint16_t rgb;
      uint8_t res=fp.read((uint8_t*)&rgb,2);
      if (!res) break;
      renderer->writePixel(xp+i,yp,rgb);
    }
    delay(0);
    OsWatchLoop();
    yp++;
  }
#endif
  fp.close();
}
#endif

#ifdef USE_AWATCH
#define MINUTE_REDUCT 4

#ifndef pi
#define pi 3.14159265359
#endif

// draw analog watch, just for fun
void DrawAClock(uint16_t rad) {
    if (!renderer) return;
    float frad=rad;
    uint16_t hred=frad/3.0;
    renderer->fillCircle(disp_xpos, disp_ypos, rad, bg_color);
    renderer->drawCircle(disp_xpos, disp_ypos, rad, fg_color);
    renderer->fillCircle(disp_xpos, disp_ypos, 4, fg_color);
    for (uint8_t count=0; count<60; count+=5) {
      float p1=((float)count*(pi/30)-(pi/2));
      uint8_t len;
      if ((count%15)==0) {
        len=4;
      } else {
        len=2;
      }
      renderer->writeLine(disp_xpos+((float)(rad-len)*cosf(p1)), disp_ypos+((float)(rad-len)*sinf(p1)), disp_xpos+(frad*cosf(p1)), disp_ypos+(frad*sinf(p1)), fg_color);
    }

    // hour
    float hour=((float)RtcTime.hour*60.0+(float)RtcTime.minute)/60.0;
    float temp=(hour*(pi/6.0)-(pi/2.0));
    renderer->writeLine(disp_xpos, disp_ypos,disp_xpos+(frad-hred)*cosf(temp),disp_ypos+(frad-hred)*sinf(temp), fg_color);

    // minute
    temp=((float)RtcTime.minute*(pi/30.0)-(pi/2.0));
    renderer->writeLine(disp_xpos, disp_ypos,disp_xpos+(frad-MINUTE_REDUCT)*cosf(temp),disp_ypos+(frad-MINUTE_REDUCT)*sinf(temp), fg_color);
}
#endif


#ifdef USE_GRAPH

typedef union {
  uint8_t data;
  struct {
      uint8_t overlay : 1;
      uint8_t draw : 1;
      uint8_t nu3 : 1;
      uint8_t nu4 : 1;
      uint8_t nu5 : 1;
      uint8_t nu6 : 1;
      uint8_t nu7 : 1;
      uint8_t nu8 : 1;
  };
} GFLAGS;

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
  uint32_t last_ms_redrawn;
  int16_t decimation; // decimation or graph duration in minutes
  uint16_t dcnt;
  uint32_t summ;
  uint16_t xcnt;
  uint8_t *values;
  uint8_t xticks;
  uint8_t yticks;
  uint8_t last_val;
  uint8_t color_index;
  GFLAGS flags;
};


struct GRAPH *graph[NUM_GRAPHS];

#define TICKLEN 4
void ClrGraph(uint16_t num) {
  struct GRAPH *gp=graph[num];

  uint16_t xticks=gp->xticks;
  uint16_t yticks=gp->yticks;
  uint16_t count;

  // clr inside, but only 1.graph if overlapped
  if (gp->flags.overlay) return;

  renderer->fillRect(gp->xp+1,gp->yp+1,gp->xs-2,gp->ys-2,bg_color);

  if (xticks) {
    float cxp=gp->xp,xd=(float)gp->xs/(float)xticks;
    for (count=0; count<xticks; count++) {
      renderer->writeFastVLine(cxp,gp->yp+gp->ys-TICKLEN,TICKLEN,fg_color);
      cxp+=xd;
    }
  }
  if (yticks) {
    if (gp->ymin<0 && gp->ymax>0) {
      // draw zero seperator
      float cxp=0;
      float czp=gp->yp+(gp->ymax/gp->range);
      while (cxp<gp->xs) {
        renderer->writeFastHLine(gp->xp+cxp,czp,2,fg_color);
        cxp+=6.0;
      }
      // align ticks to zero line
      float cyp=0,yd=gp->ys/yticks;
      for (count=0; count<yticks; count++) {
        if ((czp-cyp)>gp->yp) {
          renderer->writeFastHLine(gp->xp,czp-cyp,TICKLEN,fg_color);
          renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,czp-cyp,TICKLEN,fg_color);
        }
        if ((czp+cyp)<(gp->yp+gp->ys)) {
          renderer->writeFastHLine(gp->xp,czp+cyp,TICKLEN,fg_color);
          renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,czp+cyp,TICKLEN,fg_color);
        }
        cyp+=yd;
      }
    } else {
      float cyp=gp->yp,yd=gp->ys/yticks;
      for (count=0; count<yticks; count++) {
        renderer->writeFastHLine(gp->xp,cyp,TICKLEN,fg_color);
        renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,cyp,TICKLEN,fg_color);
        cyp+=yd;
      }
    }
  }
}

// define a graph
void DefineGraph(uint16_t num,uint16_t xp,uint16_t yp,int16_t xs,uint16_t ys,int16_t dec,float ymin, float ymax,uint8_t icol) {
  if (!renderer) return;
  uint8_t rflg=0;
  if (xs<0) {
    rflg=1;
    xs=abs(xs);
  }
  struct GRAPH *gp;
  uint16_t count;
  uint16_t index=num%NUM_GRAPHS;
  if (!graph[index]) {
    gp=(struct GRAPH*)calloc(sizeof(struct GRAPH),1);
    if (!gp) return;
    graph[index]=gp;
  } else {
    gp=graph[index];
    if (rflg) {
      RedrawGraph(index,1);
      return;
    }
  }

  // 6 bits per axis
  gp->xticks=(num>>4)&0x3f;
  gp->yticks=(num>>10)&0x3f;
  gp->xp=xp;
  gp->yp=yp;
  gp->xs=xs;
  gp->ys=ys;
  if (!dec) dec=1;
  gp->decimation=dec;
  if (dec>0) {
    // is minutes per sweep prepare timing parameters in ms
    gp->x_time=((float)dec*60000.0)/(float)xs;
    gp->last_ms=millis()+gp->x_time;
  }
  gp->ymin=ymin;
  gp->ymax=ymax;
  gp->range=(ymax-ymin)/ys;
  gp->xcnt=0;
  gp->dcnt=0;
  gp->summ=0;
  if (gp->values) free(gp->values);
  gp->values=(uint8_t*) calloc(1,xs+2);
  if (!gp->values) {
    free(gp);
    graph[index]=0;
    return;
  }
  // start from zero
  gp->values[0]=0;

  gp->last_ms_redrawn=millis();

  if (!icol) icol=1;
  gp->color_index=icol;
  gp->flags.overlay=0;
  gp->flags.draw=1;

  // check if previous graph has same coordinates
  if (index>0) {
    for (uint8_t count=0; count<index; count++) {
      if (graph[count]) {
        // a previous graph is defined, compare
        // assume the same if corner is identical
        struct GRAPH *gp1=graph[count];
        if ((gp->xp==gp1->xp) && (gp->yp==gp1->yp)) {
          gp->flags.overlay=1;
          break;
        }
      }
    }
  }

  // draw rectangle
  renderer->drawRect(xp,yp,xs,ys,fg_color);
  // clr inside
  ClrGraph(index);

}

// check if to advance GRAPH
void DisplayCheckGraph() {
  int16_t count;
  struct GRAPH *gp;
  for (count=0;count<NUM_GRAPHS;count++) {
    gp=graph[count];
    if (gp) {
      if (gp->decimation>0) {
        // if time over add value
        while (millis()>gp->last_ms) {
          gp->last_ms+=gp->x_time;
          uint8_t val;
          if (gp->dcnt) {
            val=gp->summ/gp->dcnt;
            gp->dcnt=0;
            gp->summ=0;
            gp->last_val=val;
          } else {
            val=gp->last_val;
          }
          AddGraph(count,val);
        }
      }
    }
  }
}


#if defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)
#include <SD.h>

void Save_graph(uint8_t num, char *path) {
  if (!renderer) return;
  uint16_t index=num%NUM_GRAPHS;
  struct GRAPH *gp=graph[index];
  if (!gp) return;
  File fp;
  SD.remove(path);
  fp=SD.open(path,FILE_WRITE);
  if (!fp) return;
  char str[32];
  sprintf_P(str,PSTR("%d\t%d\t%d\t"),gp->xcnt,gp->xs,gp->ys);
  fp.print(str);
  dtostrfd(gp->ymin,2,str);
  fp.print(str);
  fp.print("\t");
  dtostrfd(gp->ymax,2,str);
  fp.print(str);
  fp.print("\t");
  for (uint32_t count=0;count<gp->xs;count++) {
    dtostrfd(gp->values[count],0,str);
    fp.print(str);
    fp.print("\t");
  }
  fp.print("\n");
  fp.close();
}
void Restore_graph(uint8_t num, char *path) {
  if (!renderer) return;
  uint16_t index=num%NUM_GRAPHS;
  struct GRAPH *gp=graph[index];
  if (!gp) return;
  File fp;
  fp=SD.open(path,FILE_READ);
  if (!fp) return;
  char vbuff[32];
  char *cp=vbuff;
  uint8_t buf[2];
  uint8_t findex=0;

  for (uint32_t count=0;count<=gp->xs+4;count++) {
    cp=vbuff;
    findex=0;
    while (fp.available()) {
      fp.read(buf,1);
      if (buf[0]=='\t' || buf[0]==',' || buf[0]=='\n' || buf[0]=='\r') {
        break;
      } else {
        *cp++=buf[0];
        findex++;
        if (findex>=sizeof(vbuff)-1) break;
      }
    }
    *cp=0;
    if (count<=4) {
      if (count==0) gp->xcnt=atoi(vbuff);
    } else {
      gp->values[count-5]=atoi(vbuff);
    }
  }
  fp.close();
  RedrawGraph(num,1);
}
#endif

void RedrawGraph(uint8_t num, uint8_t flags) {
  uint16_t index=num%NUM_GRAPHS;
  struct GRAPH *gp=graph[index];
  if (!gp) return;
  if (!flags) {
    gp->flags.draw=0;
    return;
  }
  if (!renderer) return;

  gp->flags.draw=1;
  uint16_t linecol=fg_color;

  if (color_type==COLOR_COLOR) {
    linecol=renderer->GetColorFromIndex(gp->color_index);
  }

  if (!gp->flags.overlay) {
    // draw rectangle
    renderer->drawRect(gp->xp,gp->yp,gp->xs,gp->ys,fg_color);
    // clr inside
    ClrGraph(index);
  }

  for (uint16_t count=0;count<gp->xs-1;count++) {
    renderer->writeLine(gp->xp+count,gp->yp+gp->ys-gp->values[count]-1,gp->xp+count+1,gp->yp+gp->ys-gp->values[count+1]-1,linecol);
  }
}

// add next value to graph
void AddGraph(uint8_t num,uint8_t val) {
  struct GRAPH *gp=graph[num];
  if (!renderer) return;

  uint16_t linecol=fg_color;
  if (color_type==COLOR_COLOR) {
    linecol=renderer->GetColorFromIndex(gp->color_index);
  }
  gp->xcnt++;
  if (gp->xcnt>gp->xs) {
    gp->xcnt=gp->xs;
    int16_t count;
    // shift values
    for (count=0;count<gp->xs-1;count++) {
      gp->values[count]=gp->values[count+1];
    }
    gp->values[gp->xcnt-1]=val;

    if (!gp->flags.draw) return;

    // only redraw every second or longer
    if (millis()-gp->last_ms_redrawn>1000) {
      gp->last_ms_redrawn=millis();
      // clr area and redraw graph
      if (!gp->flags.overlay) {
        // draw rectangle
        renderer->drawRect(gp->xp,gp->yp,gp->xs,gp->ys,fg_color);
        // clr inner and draw ticks
        ClrGraph(num);
      }

      for (count=0;count<gp->xs-1;count++) {
        renderer->writeLine(gp->xp+count,gp->yp+gp->ys-gp->values[count]-1,gp->xp+count+1,gp->yp+gp->ys-gp->values[count+1]-1,linecol);
      }
    }
  } else {
    // add value and draw a single line
    gp->values[gp->xcnt]=val;
    if (!gp->flags.draw) return;
    renderer->writeLine(gp->xp+gp->xcnt-1,gp->yp+gp->ys-gp->values[gp->xcnt-1]-1,gp->xp+gp->xcnt,gp->yp+gp->ys-gp->values[gp->xcnt]-1,linecol);
  }
}


// add next value
void AddValue(uint8_t num,float fval) {
  // not yet defined ???
  num=num%NUM_GRAPHS;
  struct GRAPH *gp=graph[num];
  if (!gp) return;

  if (fval>gp->ymax) fval=gp->ymax;
  if (fval<gp->ymin) fval=gp->ymin;

  int16_t val;
  val=(fval-gp->ymin)/gp->range;

  if (val>gp->ys-1) val=gp->ys-1;
  if (val<0) val=0;

  // summ values
  gp->summ+=val;
  gp->dcnt++;

  // decimation option
  if (gp->decimation<0) {
    if (gp->dcnt>=-gp->decimation) {
      gp->dcnt=0;
      // calc average
      val=gp->summ/-gp->decimation;
      gp->summ=0;
      // add to graph
      AddGraph(num,val);
    }
  }
}
#endif

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv13(uint8_t function)
{
  bool result = false;

  if ((i2c_flg || spi_flg || soft_spi_flg) && XdspPresent()) {
    switch (function) {
      case FUNC_PRE_INIT:
        DisplayInitDriver();
#ifdef USE_GRAPH
        for (uint8_t count=0;count<NUM_GRAPHS;count++) {
          graph[count]=0;
        }
#endif
        break;
      case FUNC_EVERY_50_MSECOND:
        if (Settings.display_model) { XdspCall(FUNC_DISPLAY_EVERY_50_MSECOND); }
        break;
      case FUNC_SET_POWER:
        DisplaySetPower();
        break;
      case FUNC_EVERY_SECOND:
#ifdef USE_GRAPH
        DisplayCheckGraph();
#endif

#ifdef USE_DISPLAY_MODES1TO5
        if (Settings.display_model && Settings.display_mode) { XdspCall(FUNC_DISPLAY_EVERY_SECOND); }
#endif
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_MQTT_SUBSCRIBE:
        DisplayMqttSubscribe();
        break;
      case FUNC_MQTT_DATA:
        result = DisplayMqttData();
        break;
      case FUNC_SHOW_SENSOR:
        DisplayLocalSensor();
        break;
#endif  // USE_DISPLAY_MODES1TO5
      case FUNC_COMMAND:
        result = DecodeCommand(kDisplayCommands, DisplayCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
