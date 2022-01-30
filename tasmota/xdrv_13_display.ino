/*
  xdrv_13_display.ino - Display support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#define XDRV_13       13

#include <renderer.h>

Renderer *renderer;

enum ColorType { COLOR_BW, COLOR_COLOR };

#ifndef DISP_BATCH_FILE
#define DISP_BATCH_FILE "/display.bat"
#endif

#ifdef USE_UFILESYS
extern FS *ufsp;
extern FS *ffsp;
#endif

#ifdef USE_TOUCH_BUTTONS
extern VButton *buttons[MAX_TOUCH_BUTTONS];
#endif

// drawing color is WHITE
// on epaper the whole display buffer is transfered inverted this results in white paper
uint16_t fg_color = 1;
uint16_t bg_color = 0;
uint8_t color_type = COLOR_BW;
uint8_t auto_draw = 1;
int16_t disp_xpos = 0;
int16_t disp_ypos = 0;

#ifdef USE_MULTI_DISPLAY

#ifndef MAX_MULTI_DISPLAYS
#define MAX_MULTI_DISPLAYS 3
#endif

struct MULTI_DISP {
  Renderer *display;
  uint16_t fg_color;
  uint16_t bg_color;
  int16_t disp_xpos;
  int16_t disp_ypos;
  uint8_t color_type;
  uint8_t auto_draw;
  uint8_t model;
  uint8_t used;
} displays[MAX_MULTI_DISPLAYS];
uint8_t cur_display;
Renderer *Init_uDisplay(const char *desc);

void Set_display(uint8_t index) {
  displays[index].display = renderer;
  displays[index].fg_color = fg_color;
  displays[index].bg_color = bg_color;
  displays[index].color_type = color_type;
  displays[index].auto_draw = auto_draw;
  displays[index].disp_xpos = disp_xpos;
  displays[index].disp_ypos = disp_ypos;
  displays[index].model = Settings->display_model;
  displays[index].used = 1;
  cur_display = index;
}

void Get_display(uint8_t index) {
  renderer = displays[index].display;
  fg_color = displays[index].fg_color;
  bg_color = displays[index].bg_color;
  color_type = displays[index].color_type;
  auto_draw = displays[index].auto_draw;
  disp_xpos = displays[index].disp_xpos;
  disp_ypos = displays[index].disp_ypos;
  if (renderer) renderer->setDrawMode(auto_draw >> 1);
  //Settings->display_model = displays[index].model;
  cur_display = index;
}
#endif // USE_MULTI_DISPLAY

#ifndef TXT_MAX_SFAC
#define TXT_MAX_SFAC 4
#endif // TXT_MAX_SFAC


const uint8_t DISPLAY_MAX_DRIVERS = 32;        // Max number of display drivers/models supported by xdsp_interface.ino
const uint8_t DISPLAY_MAX_COLS = 64;           // Max number of columns allowed with command DisplayCols
const uint8_t DISPLAY_MAX_ROWS = 64;           // Max number of lines allowed with command DisplayRows

const uint8_t DISPLAY_LOG_ROWS = 32;           // Number of lines in display log buffer

#define D_PRFX_DISPLAY "Display"
#define D_CMND_DISP_ADDRESS "Address"
#define D_CMND_DISP_COLS "Cols"
#define D_CMND_DISP_DIMMER "Dimmer"
#define D_CMND_DISP_MODE "Mode"
#define D_CMND_DISP_MODEL "Model"
#define D_CMND_DISP_TYPE "Type"
#define D_CMND_DISP_REFRESH "Refresh"
#define D_CMND_DISP_ROWS "Rows"
#define D_CMND_DISP_SIZE "Size"
#define D_CMND_DISP_FONT "Font"
#define D_CMND_DISP_ROTATE "Rotate"
#define D_CMND_DISP_INVERT "Invert"
#define D_CMND_DISP_WIDTH "Width"
#define D_CMND_DISP_HEIGHT "Height"
#define D_CMND_DISP_BLINKRATE "Blinkrate"
#define D_CMND_DISP_BATCH "Batch"
#define D_CMND_DISP_TEXT "Text"

#define D_CMND_DISP_CLEAR "Clear"
#define D_CMND_DISP_NUMBER "Number"
#define D_CMND_DISP_FLOAT "Float"
#define D_CMND_DISP_NUMBERNC "NumberNC"               // NC - "No Clear"
#define D_CMND_DISP_FLOATNC "FloatNC"                 // NC - "No Clear"
#define D_CMND_DISP_RAW "Raw"
#define D_CMND_DISP_LEVEL "Level"
#define D_CMND_DISP_SEVENSEG_TEXT "SevensegText"
#define D_CMND_DISP_SEVENSEG_TEXTNC "SevensegTextNC"  // NC - "No Clear"
#define D_CMND_DISP_SCROLLDELAY "ScrollDelay"
#define D_CMND_DISP_CLOCK "Clock"
#define D_CMND_DISP_TEXTNC "TextNC"                   // NC - "No Clear"
#define D_CMND_DISP_SCROLLTEXT "ScrollText"
#define D_CMND_DISP_REINIT "reinit"

enum XdspFunctions { FUNC_DISPLAY_INIT_DRIVER, FUNC_DISPLAY_INIT, FUNC_DISPLAY_EVERY_50_MSECOND, FUNC_DISPLAY_EVERY_SECOND,
                     FUNC_DISPLAY_MODEL, FUNC_DISPLAY_MODE, FUNC_DISPLAY_POWER,
                     FUNC_DISPLAY_CLEAR, FUNC_DISPLAY_DRAW_FRAME,
                     FUNC_DISPLAY_DRAW_HLINE, FUNC_DISPLAY_DRAW_VLINE, FUNC_DISPLAY_DRAW_LINE,
                     FUNC_DISPLAY_DRAW_CIRCLE, FUNC_DISPLAY_FILL_CIRCLE,
                     FUNC_DISPLAY_DRAW_RECTANGLE, FUNC_DISPLAY_FILL_RECTANGLE,
                     FUNC_DISPLAY_TEXT_SIZE, FUNC_DISPLAY_FONT_SIZE, FUNC_DISPLAY_ROTATION, FUNC_DISPLAY_DRAW_STRING,
                     FUNC_DISPLAY_DIM, FUNC_DISPLAY_BLINKRATE,
#ifdef USE_UFILESYS
                     FUNC_DISPLAY_BATCH,
#endif
                     FUNC_DISPLAY_NUMBER, FUNC_DISPLAY_FLOAT, FUNC_DISPLAY_NUMBERNC, FUNC_DISPLAY_FLOATNC,
                     FUNC_DISPLAY_RAW, FUNC_DISPLAY_LEVEL, FUNC_DISPLAY_SEVENSEG_TEXT, FUNC_DISPLAY_SEVENSEG_TEXTNC,
                     FUNC_DISPLAY_SCROLLDELAY, FUNC_DISPLAY_CLOCK, FUNC_DISPLAY_SCROLLTEXT
                   };

enum DisplayInitModes { DISPLAY_INIT_MODE, DISPLAY_INIT_PARTIAL, DISPLAY_INIT_FULL };

const char kDisplayCommands[] PROGMEM = D_PRFX_DISPLAY "|"  // Prefix
  "|" D_CMND_DISP_MODEL "|" D_CMND_DISP_TYPE "|" D_CMND_DISP_WIDTH "|" D_CMND_DISP_HEIGHT "|" D_CMND_DISP_MODE "|"
  D_CMND_DISP_INVERT "|" D_CMND_DISP_REFRESH "|" D_CMND_DISP_DIMMER "|" D_CMND_DISP_COLS "|" D_CMND_DISP_ROWS "|"
  D_CMND_DISP_SIZE "|" D_CMND_DISP_FONT "|" D_CMND_DISP_ROTATE "|" D_CMND_DISP_TEXT "|" D_CMND_DISP_ADDRESS "|" D_CMND_DISP_BLINKRATE "|"
#ifdef USE_UFILESYS
  D_CMND_DISP_BATCH "|"
#endif
  D_CMND_DISP_CLEAR "|" D_CMND_DISP_NUMBER "|" D_CMND_DISP_FLOAT "|" D_CMND_DISP_NUMBERNC "|" D_CMND_DISP_FLOATNC "|"
  D_CMND_DISP_RAW "|" D_CMND_DISP_LEVEL "|" D_CMND_DISP_SEVENSEG_TEXT "|" D_CMND_DISP_SEVENSEG_TEXTNC "|"
  D_CMND_DISP_SCROLLDELAY "|" D_CMND_DISP_CLOCK "|" D_CMND_DISP_TEXTNC "|" D_CMND_DISP_SCROLLTEXT "|" D_CMND_DISP_REINIT
  ;

void (* const DisplayCommand[])(void) PROGMEM = {
  &CmndDisplay, &CmndDisplayModel, &CmndDisplayType, &CmndDisplayWidth, &CmndDisplayHeight, &CmndDisplayMode,
  &CmndDisplayInvert, &CmndDisplayRefresh, &CmndDisplayDimmer, &CmndDisplayColumns, &CmndDisplayRows,
  &CmndDisplaySize, &CmndDisplayFont, &CmndDisplayRotate, &CmndDisplayText, &CmndDisplayAddress, &CmndDisplayBlinkrate,
#ifdef USE_UFILESYS
  &CmndDisplayBatch,
#endif
  &CmndDisplayClear, &CmndDisplayNumber, &CmndDisplayFloat, &CmndDisplayNumberNC, &CmndDisplayFloatNC,
  &CmndDisplayRaw, &CmndDisplayLevel, &CmndDisplaySevensegText, &CmndDisplaySevensegTextNC,
  &CmndDisplayScrollDelay, &CmndDisplayClock, &CmndDisplayTextNC, &CmndDisplayScrollText,&DisplayReInitDriver
};

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
  uint16_t bg_color;
  uint16_t fg_color;
  GFLAGS flags;
};

struct GRAPH *graph[NUM_GRAPHS];
#endif // USE_GRAPH

char *dsp_str;

uint16_t dsp_x;
uint16_t dsp_y;
uint16_t dsp_x2;
uint16_t dsp_y2;
uint16_t dsp_rad;
uint16_t dsp_color;
int16_t dsp_len;

uint8_t disp_power = 0;
uint8_t disp_device = 0;
uint8_t disp_refresh = 1;
uint8_t disp_autodraw = 1;
uint8_t dsp_init;
uint8_t dsp_font;
uint8_t dsp_flag;
uint8_t dsp_on;

#define PREDEF_INDEXCOLORS 19
uint16_t index_colors[MAX_INDEXCOLORS - PREDEF_INDEXCOLORS];

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
    renderer->DisplayInit(mode, Settings->display_size, Settings->display_rotate, Settings->display_font);
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
  ExecuteCommandPower(disp_device, on, SRC_DISPLAY);
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
uint32_t decode_te(char *line) {
  uint32_t skip = 0;
  char sbuf[3],*cp;
  while (*line) {
    if (*line==ESCAPE_CHAR) {
      cp=line+1;
      if (*cp!=0 && *cp==ESCAPE_CHAR) {
        // escape escape, discard one
        memmove(cp,cp+1,strlen(cp));
        skip++;
      } else {
        // escape HH
        if (strlen(cp)<2) {
          // illegal lenght, ignore
          return skip;
        }
        // take 2 hex chars
        sbuf[0]=*(cp);
        sbuf[1]=*(cp+1);
        sbuf[2]=0;
        *line=strtol(sbuf,0,16);
        // must shift string 2 bytes shift zero also
        memmove(cp,cp+2,strlen(cp)-1);
        skip += 2;
      }
    }
    line++;
  }
  return skip;
}

/*-------------------------------------------------------------------------------------------*/
// Getter and Setter for DisplayDimer
// Original encoding is range 0..15
// New encoding is range 0..100 using negative numbers, i.e. 0..-100
uint8_t GetDisplayDimmer(void) {
  if (Settings->display_dimmer_protected > 0) {
    return changeUIntScale(Settings->display_dimmer_protected, 0, 15, 0, 100);
  } else {
    if (Settings->display_dimmer_protected < -100) { Settings->display_dimmer_protected = -100; }
    return - Settings->display_dimmer_protected;
  }
}

// retro-compatible call to get range 0..15
uint8_t GetDisplayDimmer16(void) {
  return changeUIntScale(GetDisplayDimmer(), 0, 100, 0, 15);
}

// In: 0..100
void SetDisplayDimmer(uint8_t dimmer) {
  if (dimmer > 100) { dimmer = 100; }
  Settings->display_dimmer_protected = - dimmer;
}

/*-------------------------------------------------------------------------------------------*/

#define DISPLAY_BUFFER_COLS    128          // Max number of characters in linebuf

uint16_t GetColorFromIndex(uint32_t index) {
  if (index >= MAX_INDEXCOLORS) index = 0;

  if (index < PREDEF_INDEXCOLORS) {
    return renderer->GetColorFromIndex(index);
  } else {
    return index_colors[index - PREDEF_INDEXCOLORS];
  }
}

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
            DisplayOnOff(0);
            break;
          case 'O':
            DisplayOnOff(1);
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
              if (renderer) ftemp = GetColorFromIndex(temp);
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
              if (renderer) ftemp = GetColorFromIndex(temp);
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
#ifdef USE_UFILESYS
          case 'P':
            { char *ep=strchr(cp,':');
             if (ep) {
               *ep=0;
               ep++;
               Draw_RGB_Bitmap(cp,disp_xpos,disp_ypos, false);
               cp=ep;
             }
            }
            break;
#ifdef USE_MULTI_DISPLAY
          case 'S':
            { int16_t rot = -1, srot, model;
              var = atoiv(cp, &temp);
              cp += var;
              if (temp < 1 || temp > MAX_MULTI_DISPLAYS) {
                temp = 1;
              }
              temp--;
              if (*cp == 'r') {
                cp++;
                var = atoiv(cp, &rot);
                cp += var;
              }
              if (*cp == ':') {
                cp++;
                if (displays[temp].used) {
                  Set_display(cur_display);
                  Get_display(temp);
                }
              } else {
                char *ep=strchr(cp,':');
                if (ep) {
                  *ep=0;
                  ep++;
                  File fp;
                  if (ffsp) {
                    AddLog(LOG_LEVEL_INFO, PSTR("DSP: File: %s"),cp);
                    fp = ffsp->open(cp, "r");
                    if (fp > 0) {
                      uint32_t size = fp.size();
                      char *fdesc = (char *)calloc(size + 4, 1);
                      if (fdesc) {
                        model = Settings->display_model;
                        fp.read((uint8_t*)fdesc, size);
                        fp.close();
                        Get_display(temp);
                        if (rot >= 0) {
                          srot = Settings->display_rotate;
                          Settings->display_rotate = rot;
                        }
                        renderer = Init_uDisplay(fdesc);
                        if (rot >= 0) {
                          Settings->display_rotate = srot;
                        }
                        Set_display(temp);
                        AddLog(LOG_LEVEL_INFO, PSTR("DSP: File descriptor loaded %x"),renderer);
                        free(fdesc);
                        Settings->display_model = model;
                      }
                    }
                  }
                }
                cp = ep;
              }
            }
            break;
#endif // USE_MULTI_DISPLAY
#endif // USE_UFILESYS
          case 'h':
            // hor line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
              //else DisplayDrawHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastHLine(disp_xpos, disp_ypos, temp, fg_color);
              //else DisplayDrawHLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_xpos += temp;
            break;
          case 'v':
            // vert line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
              //else DisplayDrawVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos, temp, fg_color);
              //else DisplayDrawVLine(disp_xpos, disp_ypos, temp, fg_color);
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
            //else DisplayDrawLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            disp_xpos += temp;
            disp_ypos += temp1;
            break;
          case 'k':
            // circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->drawCircle(disp_xpos, disp_ypos, temp, fg_color);
            //else DisplayDrawCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'K':
            // filled circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->fillCircle(disp_xpos, disp_ypos, temp, fg_color);
            //else DisplayDrawFilledCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'm':
            // epaper draw mode currently only for 4,7 inch displays
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->ep_update_mode(temp);
            break;
          case 'r':
            // rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->drawRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'R':
            // filled rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->fillRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'u':
            // rounded rectangle
            { int16_t rad, xp, yp, width, height;
            if (*cp == 'p') {
              // update epaper display
              cp++;
              var = atoiv(cp, &xp);
              cp += var;
              cp++;
              var = atoiv(cp, &yp);
              cp += var;
              cp++;
              var = atoiv(cp, &width);
              cp += var;
              cp++;
              var = atoiv(cp, &height);
              cp += var;
              cp++;
              var = atoiv(cp, &temp);
              cp += var;
              if (renderer) renderer->ep_update_area(xp, yp, width, height, temp);
              break;
            }
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
          case 'T': {
            uint8_t param1 = RtcTime.day_of_month;
            uint8_t param2 = RtcTime.month;
            if (*cp=='U') {
              cp++;
              param1 = RtcTime.month;
              param2 = RtcTime.day_of_month;
            }
            if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
              snprintf_P(dp, 9, PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), param1, param2, RtcTime.year%2000);
              dp += 8;
            }
            break; }
          case 'd':
            if (*cp == 'c') {
              cp++;
              // define index colo
              var = atoiv(cp, &temp);
              cp += var;
              cp++;
              var = fatoiv(cp, &ftemp);
              cp += var;
              if (temp >= MAX_INDEXCOLORS) temp = PREDEF_INDEXCOLORS;
              if (temp < PREDEF_INDEXCOLORS) temp = PREDEF_INDEXCOLORS;
              index_colors[temp - PREDEF_INDEXCOLORS] = ftemp;
              break;
            }
#ifdef USE_DT_VARS
          if (*cp == 'v') {
            cp++;
            { int16_t num, gxp, gyp, textbcol, textfcol, font, textsize, txlen, dp, time;
              var=atoiv(cp,&num);
              cp+=var;
              cp++;
              var=atoiv(cp,&gxp);
              cp+=var;
              cp++;
              var=atoiv(cp,&gyp);
              cp+=var;
              cp++;
              var=atoiv(cp,&textbcol);
              cp+=var;
              cp++;
              var=atoiv(cp,&textfcol);
              cp+=var;
              cp++;
              var=atoiv(cp,&font);
              cp+=var;
              cp++;
              var=atoiv(cp,&textsize);
              cp+=var;
              cp++;
              var=atoiv(cp,&txlen);
              cp+=var;
              cp++;
              var=atoiv(cp,&dp);
              cp+=var;
              cp++;
              var=atoiv(cp,&time);
              cp+=var;
              cp++;
              // text itself
              char bbuff[32];
              cp = get_string(bbuff, sizeof(bbuff), cp);
              char unit[4];
              cp = get_string(unit, sizeof(unit), cp);
	      decode_te(unit);
              define_dt_var(num, gxp, gyp, textbcol, textfcol, font, textsize, txlen, time, dp, bbuff, unit);
            }
          }
#endif // USE_DT_VARS
            // force draw grafics buffer
            if (renderer) renderer->Updateframe();
            //else DisplayDrawFrame();
            break;
          case 'D':
            // set auto draw mode
            auto_draw=*cp&3;
            if (renderer) renderer->setDrawMode(auto_draw>>1);
            cp += 1;
            break;
          case 's':
            // size sx
            var = atoiv(cp, &temp);
            if (temp > TXT_MAX_SFAC) temp = TXT_MAX_SFAC;
            if (renderer) renderer->setTextSize(temp);
            //else DisplaySetSize(*cp&3);
            cp+=var;
            break;
          case 'f':
            // font sx
            { uint8_t font = *cp&7;
              if (renderer) renderer->setTextFont(font);
              //else DisplaySetFont(font);
              if (font) {
                // for backward compatibility set size to 1 on non GFX fonts
                if (renderer) renderer->setTextSize(1);
                //else DisplaySetSize(1);
              }
              cp += 1;
            }
            break;
#ifdef USE_UFILESYS
#ifdef USE_RAMFONT
extern FS *ffsp;
          case 'F':
            { char *ep = strchr(cp,':');
              if (ep) {
                static uint8_t *ram_font;
                char fname[32];
                *ep = 0;
                ep++;
                if (*cp == '-' && *(cp + 1) == 0) {
                  if (ram_font) {
                    free (ram_font);
                    ram_font = 0;
                    if (renderer) renderer->SetRamfont(0);
                  }
                  cp = ep;
                } else {
                  if (*cp != '/') {
                    fname[0] = '/';
                    fname[1] = 0;
                  } else {
                    fname[0] = 0;
                  }
                  strlcat(fname, cp, sizeof(fname));
                  if (!strstr(cp, ".fnt")) {
                    strlcat(fname, ".fnt", sizeof(fname));
                  }
                  if (ffsp) {
                    File fp;
                    fp = ffsp->open(fname, "r");
                    if (fp > 0) {
                      uint32_t size = fp.size();
                      if (ram_font) free (ram_font);
                      ram_font = (uint8_t*)special_malloc(size + 4);
                      fp.read((uint8_t*)ram_font, size);
                      fp.close();
                      if (renderer) renderer->SetRamfont(ram_font);
                      //Serial.printf("Font loaded: %s\n",fname );
                    }
                  }
                }
                cp = ep;
              }
            }
            break;
#endif // USE_RAMFONT
#endif // USE_UFILESYS
          case 'a':
            // rotation angle
            if (renderer) renderer->setRotation(*cp&3);
            //else DisplaySetRotation(*cp&3);
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
#if (defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)) || defined(USE_UFILESYS)
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
#endif // USE_SCRIPT_FATFS
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
#endif // USE_GRAPH

#ifdef USE_AWATCH
          case 'w':
              var = atoiv(cp, &temp);
              cp += var;
              DrawAClock(temp);
              break;
#endif // USE_AWATCH

#ifdef USE_TOUCH_BUTTONS
          case 'b':
          { int16_t num, gxp, gyp, gxs, gys, outline, fill, textcolor, textsize; uint8_t dflg = 1, sbt = 0;
            if (*cp == 'e' || *cp == 'd') {
              // enable disable
              uint8_t dis = 0;
              if (*cp == 'd') dis = 1;
              cp++;
              var = atoiv(cp, &num);
              num = num % MAX_TOUCH_BUTTONS;
              cp += var;
              if (buttons[num]) {
                buttons[num]->vpower.disable = dis;
                if (!dis) {
                  if (buttons[num]->vpower.is_virtual) buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
                  else buttons[num]->xdrawButton(bitRead(TasmotaGlobal.power,num));
                }
              }
              break;
            }
            if (*cp == '-') {
              cp++;
              dflg = 0;
            }
            if (*cp == 's') {
              cp++;
              sbt = 1;
            }
            var=atoiv(cp,&num);
            cp+=var;
            uint8_t bflags=num>>8;
            num=num%MAX_TOUCH_BUTTONS;
            if (*cp == 's') {
              cp++;
              var=atoiv(cp,&gxp);
              if (buttons[num]) {
                // set slider or button
                if (buttons[num]->vpower.slider) {
                  buttons[num]->UpdateSlider(-gxp, -gxp);
                } else {
                  buttons[num]->vpower.on_off = gxp;
                  buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
                }
              }
              break;
            }
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
            if (!sbt) {
              // text itself
              cp = get_string(bbuff, sizeof(bbuff), cp);
            }
            if (buttons[num]) {
              delete buttons[num];
            }
            if (renderer) {
              buttons[num]= new VButton();
              if (buttons[num]) {
                if (!sbt) {
                  buttons[num]->vpower.slider = 0;
                  buttons[num]->xinitButtonUL(renderer, gxp, gyp, gxs, gys, GetColorFromIndex(outline),\
                    GetColorFromIndex(fill), GetColorFromIndex(textcolor), bbuff, textsize);
                  if (!bflags) {
                    // power button
                    if (dflg) buttons[num]->xdrawButton(bitRead(TasmotaGlobal.power, num));
                    buttons[num]->vpower.is_virtual = 0;
                  } else {
                    // virtual button
                    buttons[num]->vpower.is_virtual = 1;
                    if (bflags==2) {
                      // push
                      buttons[num]->vpower.is_pushbutton = 1;
                    } else {
                      // toggle
                      buttons[num]->vpower.is_pushbutton = 0;
                    }
                    if (dflg) buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
                    buttons[num]->vpower.disable = !dflg;
                  }
                } else {
                  // slider
                  buttons[num]->vpower.slider = 1;
                  buttons[num]->SliderInit(renderer, gxp, gyp, gxs, gys, outline, GetColorFromIndex(fill),\
                    GetColorFromIndex(textcolor), GetColorFromIndex(textsize));
                }
              }
            }
          }
          break;
#endif // USE_TOUCH_BUTTONS
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
    dp -= decode_te(linebuf);
    if ((uint32_t)dp - (uint32_t)linebuf) {
      if (!fill) {
        *dp = 0;
      } else {
        linebuf[abs(int(fill))] = 0;
      }
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
      //else DisplayDrawFrame();
    }
}



#ifdef USE_UFILESYS
void Display_Text_From_File(const char *file) {
  File fp;
  if (!ufsp) return;
  fp = ufsp->open(file, FS_FILE_READ);
  if (fp > 0) {
    char *savptr = XdrvMailbox.data;
    char linebuff[128];
    while (fp.available()) {
      uint16_t index = 0;
      while (fp.available()) {
        uint8_t buf[1];
        fp.read(buf,1);
        if (buf[0]=='\n' || buf[0]=='\r') {
          break;
        } else {
          linebuff[index] = buf[0];
          index++;
          if (index >= sizeof(linebuff) - 1) {
            break;
          }
        }
      }
      linebuff[index] = 0;
      char *cp = linebuff;
      while (*cp==' ') cp++;
      if (*cp == ';') continue;
      //AddLog(LOG_LEVEL_INFO, PSTR("displaytext %s"), cp);
      // execute display text here
      XdrvMailbox.data = cp;
      XdrvMailbox.data_len = 0;
      DisplayText();
    }
    XdrvMailbox.data = savptr;
    fp.close();
  }
}
#endif // USE_UFILESYS


#ifdef USE_DT_VARS

#ifndef MAX_DT_VARS
#define MAX_DT_VARS 8
#endif // MAX_DT_VARS

#define MAX_DVTSIZE 24

typedef struct {
  uint16_t xp;
  uint16_t yp;
  uint8_t txtbcol;
  uint8_t txtfcol;
  int8_t txtsiz;
  int8_t txtlen;
  int8_t dp;
  int8_t font;
  int8_t time;
  int8_t timer;
  char unit[6];
  char *jstrbuf;
  char rstr[32];
} DT_VARS;

DT_VARS *dt_vars[MAX_DT_VARS];

void define_dt_var(uint32_t num, uint32_t xp, uint32_t yp,  uint32_t txtbcol,  uint32_t txtfcol, int32_t font, int32_t txtsiz, int32_t txtlen, int32_t time, int32_t dp, char *jstr, char *unit) {
  if (num >= MAX_DT_VARS) return;

  if (dt_vars[num]) {
    if (dt_vars[num]->jstrbuf) free(dt_vars[num]->jstrbuf);
    free(dt_vars[num]);
  }
  //dt [dv0:100:100:0:3:2:1:10:2:WLAN#ID:uV:]

  DT_VARS *dtp = (DT_VARS*)malloc(sizeof(DT_VARS));
  if (!dtp) return;

  dt_vars[num] = dtp;

  dtp->xp = xp;
  dtp->yp = yp;
  dtp->txtbcol = txtbcol;
  dtp->txtfcol = txtfcol;
  dtp->font = font;
  dtp->txtsiz = txtsiz;
  dtp->time = time;
  if (txtlen > MAX_DVTSIZE) {txtlen = MAX_DVTSIZE;}
  dtp->txtlen = txtlen;
  dtp->dp = dp;
  uint8_t jlen = strlen(jstr);
  dtp->jstrbuf = (char*)calloc(jlen + 2,1);
  if (!dtp->jstrbuf) {
    free (dtp);
    return;
  }
  dtp->rstr[0] = 0;
  strcpy(dtp->unit, unit);
  strcpy(dtp->jstrbuf, jstr);
  if (!time) time = 1;
  dtp->timer = time;
}

void draw_dt_vars(void) {
  if (!renderer) return;

  for (uint32_t cnt = 0; cnt < MAX_DT_VARS; cnt++) {
    DT_VARS *dtp = dt_vars[cnt];
    if (dtp) {
      if (dtp->jstrbuf) {
        // draw
        dtp->timer--;
        if (!dtp->timer) {
          dtp->timer = dtp->time;
          char vstr[MAX_DVTSIZE + 7];
          memset(vstr, ' ', sizeof(vstr));
          strcpy(vstr, dtp->rstr);
          strcat(vstr, " ");
          strcat(vstr, dtp->unit);
          uint16_t slen = strlen(vstr);
          vstr[slen] = ' ';

          if (!dtp->txtlen) {
            vstr[slen] = 0;
          } else {
            vstr[abs(int(dtp->txtlen))] = 0;
          }
          if (dtp->txtlen < 0) {
            // right align
            alignright(vstr);
          }

          if (dtp->txtsiz > 0) {
            renderer->setDrawMode(0);
          } else {
            renderer->setDrawMode(2);
          }
          renderer->setTextColor(GetColorFromIndex(dtp->txtfcol),GetColorFromIndex(dtp->txtbcol));
          renderer->setTextFont(dtp->font);
          renderer->setTextSize(abs(dtp->txtsiz));

          if (dtp->jstrbuf[0]=='[') {
            uint16_t s_disp_xpos = disp_xpos;
            uint16_t s_disp_ypos = disp_ypos;
            uint16_t s_bg_color = bg_color;
            uint16_t s_fg_color = fg_color;
            disp_xpos = dtp->xp;
            disp_ypos = dtp->yp;
            bg_color = GetColorFromIndex(dtp->txtbcol);
            fg_color = GetColorFromIndex(dtp->txtfcol);
            char *savmbd = XdrvMailbox.data;
            XdrvMailbox.data = dtp->jstrbuf;
            DisplayText();
            XdrvMailbox.data = savmbd;
            disp_xpos = s_disp_xpos;
            disp_ypos = s_disp_ypos;
            bg_color = s_bg_color;
            fg_color = s_fg_color;
          } else {
            renderer->DrawStringAt(dtp->xp, dtp->yp, vstr, GetColorFromIndex(dtp->txtfcol), 0);
          }

          // restore display vars
          renderer->setTextColor(fg_color, bg_color);
          renderer->setDrawMode(auto_draw>>1);
        }
      }
    }
  }
}

#define DTV_JSON_SIZE 1024

void DisplayDTVarsTeleperiod(void) {
  ResponseClear();
  MqttShowState();
  uint32_t jlen = ResponseLength();

  if (jlen < DTV_JSON_SIZE) {
    char *json = (char*)malloc(jlen + 2);
    if (json) {
      strlcpy(json, ResponseData(), jlen + 1);
      get_dt_vars(json);
      free(json);
    }
  }
}

void get_dt_mqtt(void) {
  static uint8_t xsns_index = 0;

  ResponseClear();
  uint16_t script_tele_period_save = TasmotaGlobal.tele_period;
  TasmotaGlobal.tele_period = 2;
  XsnsNextCall(FUNC_JSON_APPEND, xsns_index);
  TasmotaGlobal.tele_period = script_tele_period_save;
  if (ResponseLength()) {
    ResponseJsonStart();
    ResponseJsonEnd();
  }
  get_dt_vars(ResponseData());
}

void get_dt_vars(char *json) {
  if (strlen(json)) {
    JsonParser parser(json);
    JsonParserObject obj = parser.getRootObject();

    for (uint32_t cnt = 0; cnt < MAX_DT_VARS; cnt++) {
      if (dt_vars[cnt]) {
        if (dt_vars[cnt]->jstrbuf && dt_vars[cnt]->jstrbuf[0]!='[') {
          char sbuf[32];
          uint32_t res = JsonParsePath(&obj, dt_vars[cnt]->jstrbuf, '#', NULL, sbuf, sizeof(sbuf));
          if (res) {
            if (dt_vars[cnt]->dp < 0) {
              // use string
              strcpy(dt_vars[cnt]->rstr, sbuf);
            } else {
              // convert back and forth
              dtostrfd(CharToFloat(sbuf), dt_vars[cnt]->dp, dt_vars[cnt]->rstr);
            }
          }
        }
      }
    }
  }
}

void free_dt_vars(void) {
  for (uint32_t cnt = 0; cnt < MAX_DT_VARS; cnt++) {
    if (dt_vars[cnt]) {
      if (dt_vars[cnt]->jstrbuf) free(dt_vars[cnt]->jstrbuf);
      free(dt_vars[cnt]);
      dt_vars[cnt] = 0;
    }
  }
}

#endif // USE_DT_VARS

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
    disp_screen_buffer_rows = Settings->display_rows;
    disp_screen_buffer = (char**)malloc(sizeof(*disp_screen_buffer) * disp_screen_buffer_rows);
    if (disp_screen_buffer != nullptr) {
      for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
        disp_screen_buffer[i] = (char*)malloc(sizeof(*disp_screen_buffer[i]) * (Settings->display_cols[0] +1));
        if (disp_screen_buffer[i] == nullptr) {
          DisplayFreeScreenBuffer();
          break;
        }
      }
    }
    if (disp_screen_buffer != nullptr) {
      disp_screen_buffer_cols = Settings->display_cols[0] +1;
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
        disp_log_buffer[i] = (char*)malloc(sizeof(*disp_log_buffer[i]) * (Settings->display_cols[0] +1));
        if (disp_log_buffer[i] == nullptr) {
          DisplayFreeLogBuffer();
          break;
        }
      }
    }
    if (disp_log_buffer != nullptr) {
      disp_log_buffer_cols = Settings->display_cols[0] +1;
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
  if (Settings->display_mode) {
    disp_log_buffer_idx = 0;
    disp_log_buffer_ptr = 0;
    disp_refresh = Settings->display_refresh;

    snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%c"), TempUnit());
    snprintf_P(disp_pres, sizeof(disp_pres), PressureUnit().c_str());

    DisplayReAllocLogBuffer();

    char buffer[40];
    snprintf_P(buffer, sizeof(buffer), PSTR(D_VERSION " %s%s"), TasmotaGlobal.version, TasmotaGlobal.image_name);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR("Display mode %d"), Settings->display_mode);
    DisplayLogBufferAdd(buffer);

    snprintf_P(buffer, sizeof(buffer), PSTR(D_CMND_HOSTNAME " %s"), NetworkHostname());
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_MAC " %s"), NetworkMacAddress().c_str());
    DisplayLogBufferAdd(buffer);
    ext_snprintf_P(buffer, sizeof(buffer), PSTR("IP %_I"), (uint32_t)NetworkAddress());
    DisplayLogBufferAdd(buffer);
    if (!TasmotaGlobal.global_state.wifi_down) {
      snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_SSID " %s"), SettingsText(SET_STASSID1 + Settings->sta_active));
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
  char buffer[Settings->display_cols[0] +1];
  char spaces[Settings->display_cols[0]];
  char source[Settings->display_cols[0] - Settings->display_cols[1]];
  char svalue[Settings->display_cols[1] +1];

  SHOW_FREE_MEM(PSTR("DisplayJsonValue"));

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

//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "mkey [%s], source [%s], value [%s], quantity_code %d, log_buffer [%s]"), mkey, source, value, quantity_code, buffer);

  DisplayLogBufferAdd(buffer);
}

void DisplayAnalyzeJson(char *topic, const char *json)
{
// //tele/pow2/STATE    {"Time":"2017-09-20T11:53:03", "Uptime":10, "Vcc":3.123, "POWER":"ON", "Wifi":{"AP":2, "SSId":"indebuurt2", "RSSI":68, "APMac":"00:22:6B:FE:8E:20"}}
// //tele/pow2/ENERGY   {"Time":"2017-09-20T11:53:03", "Total":6.522, "Yesterday":0.150, "Today":0.073, "Period":0.5, "Power":12.1, "Factor":0.56, "Voltage":210.1, "Current":0.102}

// tele/pow1/SENSOR = {"Time":"2018-01-02T17:13:17","ENERGY":{"Total":13.091,"Yesterday":0.060,"Today":0.046,"Period":0.2,"Power":9.8,"Factor":0.49,"Voltage":206.8,"Current":0.096}}
// tele/dual/STATE    {"Time":"2017-09-20T11:53:03","Uptime":25,"Vcc":3.178,"POWER1":"OFF","POWER2":"OFF","Wifi":{"AP":2,"SSId":"indebuurt2","RSSI":100,"APMac":"00:22:6B:FE:8E:20"}}
// tele/sc/SENSOR     {"Time":"2017-09-20T11:53:09","Temperature":24.0,"Humidity":16.0,"Light":30,"Noise":20,"AirQuality":100,"TempUnit":"C"}
// tele/rf1/SENSOR    {"Time":"2017-09-20T11:53:23","BH1750":{"Illuminance":57}}
// tele/wemos5/SENSOR {"Time":"2017-09-20T11:53:53","SHT1X":{"Temperature":20.1,"Humidity":58.9},"HTU21":{"Temperature":20.7,"Humidity":58.5},"BMP280":{"Temperature":21.6,"Pressure":1020.3},"TempUnit":"C"}
// tele/th1/SENSOR    {"Time":"2017-09-20T11:54:48","DS18B20":{"Temperature":49.7},"TempUnit":"C"}

  String jsonStr = json;  // Move from stack to heap to fix watchdogs (20180626)
  JsonParser parser((char*)jsonStr.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) {   // did JSON parsing went ok?

    const char *unit = root.getStr(PSTR(D_JSON_TEMPERATURE_UNIT), nullptr);   // nullptr if not found
    if (unit) {
      snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%s"), unit);  // C or F
    }
    unit = root.getStr(PSTR(D_JSON_PRESSURE_UNIT), nullptr);   // nullptr if not found
    if (unit) {
      snprintf_P(disp_pres, sizeof(disp_pres), PSTR("%s"), unit);  // hPa or mmHg
    }
    for (auto key1 : root) {
      JsonParserToken value1 = key1.getValue();
      if (value1.isObject()) {
        JsonParserObject Object2 = value1.getObject();
        for (auto key2 : Object2) {
          JsonParserToken value2 = key2.getValue();
          if (value2.isObject()) {
            JsonParserObject Object3 = value2.getObject();
            for (auto key3 : Object3) {
              const char* value3 = key3.getValue().getStr(nullptr);
              if (value3 != nullptr) {  // "DHT11":{"Temperature":null,"Humidity":null} - ignore null as it will raise exception 28
                DisplayJsonValue(topic, key1.getStr(), key3.getStr(), value3);  // Sensor 56%
              }
            }
          } else {
            const char* value = value2.getStr(nullptr);
            if (value != nullptr) {
              DisplayJsonValue(topic, key1.getStr(), key2.getStr(), value);  // Sensor  56%
            }
          }
        }
      } else {
        const char* value = value1.getStr(nullptr);
        if (value != nullptr) {
          DisplayJsonValue(topic, key1.getStr(), key1.getStr(), value);  // Topic  56%
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
  if (Settings->display_model && (Settings->display_mode &0x04)) {

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
      if (Settings->display_mode &0x04) {
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
  if ((Settings->display_mode &0x02) && (0 == TasmotaGlobal.tele_period)) {
    char no_topic[1] = { 0 };
//    DisplayAnalyzeJson(TasmotaGlobal.mqtt_topic, ResponseData());  // Add local topic
    DisplayAnalyzeJson(no_topic, ResponseData());    // Discard any topic
  }
}

#endif  // USE_DISPLAY_MODES1TO5


/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void DisplayInitDriver(void)
{
  XdspCall(FUNC_DISPLAY_INIT_DRIVER);

//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Display model %d"), Settings->display_model);

  if (Settings->display_model) {
//    ApplyDisplayDimmer();  // Not allowed here. Way too early in initi sequence. IE power state has not even been set at this point in time

#ifdef USE_MULTI_DISPLAY
    Set_display(0);
#endif // USE_MULTI_DISPLAY

    if (renderer) {
      renderer->setTextFont(Settings->display_font);
      renderer->setTextSize(Settings->display_size);
      // force opaque mode
      renderer->setDrawMode(0);

      for (uint32_t cnt = 0; cnt < (MAX_INDEXCOLORS - PREDEF_INDEXCOLORS); cnt++) {
        index_colors[cnt] = 0;
      }
    }

#ifdef USE_DT_VARS
    free_dt_vars();
#endif

#ifdef USE_UFILESYS
    Display_Text_From_File(DISP_BATCH_FILE);
#endif

#ifdef USE_GRAPH
    for (uint8_t count = 0; count < NUM_GRAPHS; count++) { graph[count] = 0; }
#endif

    TasmotaGlobal.devices_present++;
    if (!PinUsed(GPIO_BACKLIGHT)) {
      if (TasmotaGlobal.light_type && (4 == Settings->display_model)) {
        TasmotaGlobal.devices_present--;  // Assume PWM channel is used for backlight
      }
    }
    disp_device = TasmotaGlobal.devices_present;

#ifndef USE_DISPLAY_MODES1TO5
    Settings->display_mode = 0;
#else
    DisplayLogBufferInit();
#endif  // USE_DISPLAY_MODES1TO5
  }
}

void DisplaySetPower(void)
{
  disp_power = bitRead(XdrvMailbox.index, disp_device -1);

//AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: Power %d"), disp_power);

  if (Settings->display_model) {
    if (!renderer) {
      XdspCall(FUNC_DISPLAY_POWER);
    } else {
      renderer->DisplayOnff(disp_power);
#ifdef USE_BERRY
      // still call Berry virtual display in case it is not managed entirely by renderer
      Xdsp18(FUNC_DISPLAY_POWER);
#endif // USE_BERRY
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDisplay(void) {
  Response_P(PSTR("{\"" D_PRFX_DISPLAY "\":{\"" D_CMND_DISP_MODEL "\":%d,\"" D_CMND_DISP_TYPE "\":%d,\"" D_CMND_DISP_WIDTH "\":%d,\"" D_CMND_DISP_HEIGHT "\":%d,\""
    D_CMND_DISP_MODE "\":%d,\"" D_CMND_DISP_DIMMER "\":%d,\"" D_CMND_DISP_SIZE "\":%d,\"" D_CMND_DISP_FONT "\":%d,\""
    D_CMND_DISP_ROTATE "\":%d,\"" D_CMND_DISP_INVERT "\":%d,\"" D_CMND_DISP_REFRESH "\":%d,\"" D_CMND_DISP_COLS "\":[%d,%d],\"" D_CMND_DISP_ROWS "\":%d}}"),
    Settings->display_model, Settings->display_options.type, Settings->display_width, Settings->display_height,
    Settings->display_mode, GetDisplayDimmer(), Settings->display_size, Settings->display_font,
    Settings->display_rotate, Settings->display_options.invert, Settings->display_refresh, Settings->display_cols[0], Settings->display_cols[1], Settings->display_rows);
}

void CmndDisplayModel(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < DISPLAY_MAX_DRIVERS)) {
    uint32_t last_display_model = Settings->display_model;
    Settings->display_model = XdrvMailbox.payload;
    if (XdspCall(FUNC_DISPLAY_MODEL)) {
      TasmotaGlobal.restart_flag = 2;  // Restart to re-init interface and add/Remove MQTT subscribe
    } else {
      Settings->display_model = last_display_model;
    }
  }
  ResponseCmndNumber(Settings->display_model);
}

void CmndDisplayType(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 7)) {
    Settings->display_options.type = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->display_options.type);
}

void CmndDisplayWidth(void) {
  if (XdrvMailbox.payload > 0) {
    if (XdrvMailbox.payload != Settings->display_width) {
      Settings->display_width = XdrvMailbox.payload;
      TasmotaGlobal.restart_flag = 2;  // Restart to re-init width
    }
  }
  ResponseCmndNumber(Settings->display_width);
}

void CmndDisplayHeight(void) {
  if (XdrvMailbox.payload > 0) {
    if (XdrvMailbox.payload != Settings->display_height) {
      Settings->display_height = XdrvMailbox.payload;
      TasmotaGlobal.restart_flag = 2;  // Restart to re-init height
    }
  }
  ResponseCmndNumber(Settings->display_height);
}

void CmndDisplayMode(void) {
#ifdef USE_DISPLAY_MODES1TO5
/*     Matrix / 7-segment   LCD / Oled                           TFT
 * 1 = Text up and time     Time
 * 2 = Date                 Local sensors                        Local sensors
 * 3 = Day                  Local sensors and time               Local sensors and time
 * 4 = Mqtt left and time   Mqtt (incl local) sensors            Mqtt (incl local) sensors
 * 5 = Mqtt up and time     Mqtt (incl local) sensors and time   Mqtt (incl local) sensors and time
*/
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
    uint32_t last_display_mode = Settings->display_mode;
    Settings->display_mode = XdrvMailbox.payload;

    if (disp_subscribed != (Settings->display_mode &0x04)) {
      TasmotaGlobal.restart_flag = 2;  // Restart to Add/Remove MQTT subscribe
    } else {
      if (last_display_mode && !Settings->display_mode) {  // Switch to mode 0
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
  ResponseCmndNumber(Settings->display_mode);
}

// Apply the current display dimmer
void ApplyDisplayDimmer(void) {
  uint8_t dimmer8 = changeUIntScale(GetDisplayDimmer(), 0, 100, 0, 255);
  uint16_t dimmer10_gamma = ledGamma10(dimmer8);
  if (dimmer8 && !(disp_power)) {
    ExecuteCommandPower(disp_device, POWER_ON, SRC_DISPLAY);
  }
  else if (!dimmer8 && disp_power) {
    ExecuteCommandPower(disp_device, POWER_OFF, SRC_DISPLAY);
  }
  if (renderer) {
    renderer->dim10(dimmer8, dimmer10_gamma);   // provide 8 bits and gamma corrected dimmer in 8 bits
#ifdef USE_BERRY
    // still call Berry virtual display in case it is not managed entirely by renderer
    Xdsp18(FUNC_DISPLAY_DIM);
#endif // USE_BERRY

  } else {
    XdspCall(FUNC_DISPLAY_DIM);
  }
}

void CmndDisplayDimmer(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    uint8_t dimmer = XdrvMailbox.payload;
    SetDisplayDimmer(dimmer);
    ApplyDisplayDimmer();
  }
  ResponseCmndNumber(GetDisplayDimmer());
}

void CmndDisplaySize(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= TXT_MAX_SFAC)) {
    Settings->display_size = XdrvMailbox.payload;
    if (renderer) renderer->setTextSize(Settings->display_size);
    //else DisplaySetSize(Settings->display_size);
  }
  ResponseCmndNumber(Settings->display_size);
}

void CmndDisplayFont(void) {
  if ((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 4)) {
    Settings->display_font = XdrvMailbox.payload;
    if (renderer) renderer->setTextFont(Settings->display_font);
    //else DisplaySetFont(Settings->display_font);
  }
  ResponseCmndNumber(Settings->display_font);
}

void CmndDisplayRotate(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 4)) {
    if ((Settings->display_rotate) != XdrvMailbox.payload) {
/*
      // Needs font info regarding height and width
      if ((Settings->display_rotate &1) != (XdrvMailbox.payload &1)) {
        uint8_t temp_rows = Settings->display_rows;
        Settings->display_rows = Settings->display_cols[0];
        Settings->display_cols[0] = temp_rows;
#ifdef USE_DISPLAY_MODES1TO5
        DisplayReAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
      }
*/
      Settings->display_rotate = XdrvMailbox.payload;
      DisplayInit(DISPLAY_INIT_MODE);
#ifdef USE_DISPLAY_MODES1TO5
      DisplayLogBufferInit();
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  ResponseCmndNumber(Settings->display_rotate);
}

void CmndDisplayInvert(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->display_options.invert = XdrvMailbox.payload;
    if (renderer) renderer->invertDisplay(Settings->display_options.invert);
  }
  ResponseCmndNumber(Settings->display_options.invert);
}

void CmndDisplayRefresh(void) {
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 7)) {
    Settings->display_refresh = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->display_refresh);
}

void CmndDisplayColumns(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_COLS)) {
      Settings->display_cols[XdrvMailbox.index -1] = XdrvMailbox.payload;
#ifdef USE_DISPLAY_MODES1TO5
      if (1 == XdrvMailbox.index) {
        DisplayLogBufferInit();
        DisplayReAllocScreenBuffer();
      }
#endif  // USE_DISPLAY_MODES1TO5
    }
    ResponseCmndIdxNumber(Settings->display_cols[XdrvMailbox.index -1]);
  }
}

void CmndDisplayRows(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_ROWS)) {
    Settings->display_rows = XdrvMailbox.payload;
#ifdef USE_DISPLAY_MODES1TO5
    DisplayLogBufferInit();
    DisplayReAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
  }
  ResponseCmndNumber(Settings->display_rows);
}

void CmndDisplayAddress(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
      Settings->display_address[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings->display_address[XdrvMailbox.index -1]);
  }
}

void CmndDisplayBlinkrate(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    if (!renderer) {
      XdspCall(FUNC_DISPLAY_BLINKRATE);
    }
  }
  ResponseCmndNumber(XdrvMailbox.payload);
}

#ifdef USE_UFILESYS
void CmndDisplayBatch(void) {
  if (XdrvMailbox.data_len > 0) {
    if (!Settings->display_mode) {
      Display_Text_From_File(XdrvMailbox.data);
    }
    ResponseCmndChar(XdrvMailbox.data);
  }
}
#endif

void CmndDisplayText(void) {
  if (disp_device && XdrvMailbox.data_len > 0) {
#ifndef USE_DISPLAY_MODES1TO5
    DisplayText();
#else
    if(Settings->display_model == 15) {
      XdspCall(FUNC_DISPLAY_SEVENSEG_TEXT);
    } else if (!Settings->display_mode) {
      DisplayText();
    } else {
      DisplayLogBufferAdd(XdrvMailbox.data);
    }
#endif  // USE_DISPLAY_MODES1TO5
    ResponseCmndChar(XdrvMailbox.data);
  }
}

/*********************************************************************************************\
 * Currently 7-segement specific - should have been handled by (extended) DisplayText command
\*********************************************************************************************/

void CmndDisplayClear(void) {
  if (!renderer)
    XdspCall(FUNC_DISPLAY_CLEAR);
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayNumber(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_NUMBER);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayFloat(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_FLOAT);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayNumberNC(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_NUMBERNC);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayFloatNC(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_FLOATNC);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayRaw(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_RAW);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayLevel(void) {
  bool result = false;
  if (!renderer) {
    result = XdspCall(FUNC_DISPLAY_LEVEL);
  }
  if(result) ResponseCmndNumber(XdrvMailbox.payload);
}

void CmndDisplaySevensegText(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_SEVENSEG_TEXT);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayTextNC(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_SEVENSEG_TEXTNC);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplaySevensegTextNC(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_SEVENSEG_TEXTNC);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndDisplayScrollDelay(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_SCROLLDELAY);
  }
  ResponseCmndNumber(XdrvMailbox.payload);
}

void CmndDisplayClock(void) {
  if (!renderer) {
    XdspCall(FUNC_DISPLAY_CLOCK);
  }
  ResponseCmndNumber(XdrvMailbox.payload);
}

void CmndDisplayScrollText(void) {
  bool result = false;
  if (!renderer) {
    result = XdspCall(FUNC_DISPLAY_SCROLLTEXT);
  }
  if(result) ResponseCmndChar(XdrvMailbox.data);
}

void DisplayReInitDriver(void) {
  XdspCall(FUNC_DISPLAY_INIT_DRIVER);
#ifdef USE_MULTI_DISPLAY
  Set_display(0);
#endif // USE_MULTI_DISPLAY
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Optional drivers
\*********************************************************************************************/

#ifdef USE_TOUCH_BUTTONS
// very limited path size, so, add .jpg
void draw_picture(char *path, uint32_t xp, uint32_t yp, uint32_t xs, uint32_t ys, uint32_t ocol, bool inverted) {
char ppath[16];
  strcpy(ppath, path);
  uint8_t plen = strlen(path) -1;
  if (ppath[plen]=='1') {
    // index mode
    if (inverted) {
      ppath[plen] = '2';
    }
    inverted = false;
  }
  if (ocol == 9) {
    strcat(ppath, ".rgb");
  } else {
    strcat(ppath, ".jpg");
  }
  Draw_RGB_Bitmap(ppath, xp, yp, inverted);
}
#endif  // USE_TOUCH_BUTTONS


#ifdef ESP32
#ifdef JPEG_PICTS
#include "img_converters.h"
#include "esp_jpg_decode.h"
bool jpg2rgb888(const uint8_t *src, size_t src_len, uint8_t * out, jpg_scale_t scale);
char get_jpeg_size(unsigned char* data, unsigned int data_size, unsigned short *width, unsigned short *height);
#endif // JPEG_PICTS
#endif // ESP32

#ifdef USE_UFILESYS
extern FS *ufsp;
#define XBUFF_LEN 128
void Draw_RGB_Bitmap(char *file,uint16_t xp, uint16_t yp, bool inverted ) {
  if (!renderer) return;
  File fp;
  char *ending = strrchr(file,'.');
  if (!ending) return;
  ending++;
  char estr[8];
  memset(estr,0,sizeof(estr));
  for (uint32_t cnt=0; cnt<strlen(ending); cnt++) {
    estr[cnt]=tolower(ending[cnt]);
  }

  if (!strcmp(estr,"rgb")) {
    // special rgb format
    fp=ufsp->open(file,FS_FILE_READ);
    if (!fp) return;
    uint16_t xsize;
    fp.read((uint8_t*)&xsize,2);
    uint16_t ysize;
    fp.read((uint8_t*)&ysize,2);
#if 1
    renderer->setAddrWindow(xp,yp,xp+xsize,yp+ysize);
    uint16_t rgb[xsize];
    for (int16_t j=0; j<ysize; j++) {
    //  for(int16_t i=0; i<xsize; i+=XBUFF_LEN) {
        fp.read((uint8_t*)rgb,xsize*2);
        renderer->pushColors(rgb,xsize,true);
    //  }
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
  } else if (!strcmp(estr,"jpg")) {
    // jpeg files on ESP32 with more memory
#ifdef ESP32
#ifdef JPEG_PICTS
    fp=ufsp->open(file,FS_FILE_READ);
    if (!fp) return;
    uint32_t size = fp.size();
    uint8_t *mem = (uint8_t *)special_malloc(size+4);
    if (mem) {
      uint8_t res=fp.read(mem, size);
      if (res) {
        uint16_t xsize;
        uint16_t ysize;
        if (mem[0]==0xff && mem[1]==0xd8) {
          get_jpeg_size(mem, size, &xsize, &ysize);
          //Serial.printf(" x,y,fs %d - %d - %d\n",xsize, ysize, size );
          if (xsize && ysize) {
            uint8_t *out_buf = (uint8_t *)special_malloc((xsize*ysize*3)+4);
            if (out_buf) {
              uint16_t *pixb = (uint16_t *)special_malloc((xsize*2)+4);
              if (pixb) {
                uint8_t *ob=out_buf;
                if (jpg2rgb888(mem, size, out_buf, (jpg_scale_t)JPG_SCALE_NONE)) {
                  renderer->setAddrWindow(xp,yp,xp+xsize,yp+ysize);
                  for(int32_t j=0; j<ysize; j++) {
                    if (inverted==false) {
                      rgb888_to_565(ob, pixb, xsize);
                    } else {
                      rgb888_to_565i(ob, pixb, xsize);
                    }
                    ob+=xsize*3;
                    renderer->pushColors(pixb, xsize, true);
                    OsWatchLoop();
                  }
                  renderer->setAddrWindow(0,0,0,0);
                }
                free(out_buf);
                free(pixb);
              } else {
                free(out_buf);
              }
            }
          }
        }
        free(mem);
      }
      fp.close();
    }
#endif // JPEG_PICTS
#endif // ESP32
  }
}
#endif // USE_UFILESYS

/*********************************************************************************************\
 * AWatch
\*********************************************************************************************/

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
#endif // USE_AWATCH

/*********************************************************************************************\
 * Graphics
\*********************************************************************************************/


#ifdef USE_GRAPH


#define TICKLEN 4
void ClrGraph(uint16_t num) {
  struct GRAPH *gp=graph[num];

  uint16_t xticks=gp->xticks;
  uint16_t yticks=gp->yticks;
  uint16_t count;

  // clr inside, but only 1.graph if overlapped
  if (gp->flags.overlay) return;

  renderer->fillRect(gp->xp+1,gp->yp+1,gp->xs-2,gp->ys-2,gp->bg_color);

  if (xticks) {
    float cxp=gp->xp,xd=(float)gp->xs/(float)xticks;
    for (count=0; count<xticks; count++) {
      renderer->writeFastVLine(cxp,gp->yp+gp->ys-TICKLEN,TICKLEN,gp->fg_color);
      cxp+=xd;
    }
  }
  if (yticks) {
    if (gp->ymin<0 && gp->ymax>0) {
      // draw zero seperator
      float cxp=0;
      float czp=gp->yp+(gp->ymax/gp->range);
      while (cxp<gp->xs) {
        renderer->writeFastHLine(gp->xp+cxp,czp,2,gp->fg_color);
        cxp+=6.0;
      }
      // align ticks to zero line
      float cyp=0,yd=gp->ys/yticks;
      for (count=0; count<yticks; count++) {
        if ((czp-cyp)>gp->yp) {
          renderer->writeFastHLine(gp->xp,czp-cyp,TICKLEN,gp->fg_color);
          renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,czp-cyp,TICKLEN,gp->fg_color);
        }
        if ((czp+cyp)<(gp->yp+gp->ys)) {
          renderer->writeFastHLine(gp->xp,czp+cyp,TICKLEN,fg_color);
          renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,czp+cyp,TICKLEN,gp->fg_color);
        }
        cyp+=yd;
      }
    } else {
      float cyp=gp->yp,yd=gp->ys/yticks;
      for (count=0; count<yticks; count++) {
        renderer->writeFastHLine(gp->xp,cyp,TICKLEN,gp->fg_color);
        renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,cyp,TICKLEN,gp->fg_color);
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

  gp->bg_color=bg_color;
  gp->fg_color=fg_color;

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
  renderer->drawRect(xp,yp,xs,ys,gp->fg_color);
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


#if (defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)) || defined(USE_UFILESYS)
#ifdef ESP32
#include <SD.h>
#endif

void Save_graph(uint8_t num, char *path) {
  if (!renderer) return;
  uint16_t index=num%NUM_GRAPHS;
  struct GRAPH *gp=graph[index];
  if (!gp) return;
  File fp;
  ufsp->remove(path);
  fp=ufsp->open(path,FS_FILE_WRITE);
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
  fp=ufsp->open(path,FS_FILE_READ);
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
      uint8_t yval = atoi(vbuff);
      if (yval >= gp->ys) {
        yval = gp->ys - 1;
      }
      gp->values[count-5] = yval;
    }
  }
  fp.close();
  RedrawGraph(num,1);
}
#endif // USE_SCRIPT_FATFS

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
  uint16_t linecol=gp->fg_color;

  if (color_type==COLOR_COLOR) {
    linecol = GetColorFromIndex(gp->color_index);
  }

  if (!gp->flags.overlay) {
    // draw rectangle
    renderer->drawRect(gp->xp,gp->yp,gp->xs,gp->ys,gp->fg_color);
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

  uint16_t linecol=gp->fg_color;
  if (color_type==COLOR_COLOR) {
    linecol = GetColorFromIndex(gp->color_index);
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
        renderer->drawRect(gp->xp,gp->yp,gp->xs,gp->ys,gp->fg_color);
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
      // calc average
      val=gp->summ/gp->dcnt;
      gp->dcnt=0;
      gp->summ=0;
      // add to graph
      AddGraph(num,val);
    }
  }
}
#endif // USE_GRAPH

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv13(uint8_t function)
{
  bool result = false;

  if (XdspPresent()) {
    switch (function) {
      case FUNC_PRE_INIT:
        DisplayInitDriver();
        break;
      case FUNC_EVERY_50_MSECOND:
        if (Settings->display_model) { XdspCall(FUNC_DISPLAY_EVERY_50_MSECOND); }
        break;
      case FUNC_SET_POWER:
        DisplaySetPower();
        break;
      case FUNC_EVERY_SECOND:
#ifdef USE_GRAPH
        DisplayCheckGraph();
#endif
#ifdef USE_DT_VARS
        get_dt_mqtt();
        draw_dt_vars();
#endif // USE_DT_VARS

#ifdef USE_DISPLAY_MODES1TO5
        if (Settings->display_model && Settings->display_mode) { XdspCall(FUNC_DISPLAY_EVERY_SECOND); }
#endif
        break;
      case FUNC_AFTER_TELEPERIOD:
#ifdef USE_DT_VARS
        DisplayDTVarsTeleperiod();
#endif // USE_DT_VARS
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
