/*
  xdsp_17_universal.ino -  universal display driver support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and  Theo Arends

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
#ifdef USE_UNIVERSAL_DISPLAY

#define XDSP_17                17

#include <uDisplay.h>

uDisplay *udisp;
bool udisp_init_done = false;
extern uint8_t color_type;
extern uint16_t fg_color;
extern uint16_t bg_color;

#ifdef USE_UFILESYS
extern FS *ufsp;
#endif

#define DISPDESC_SIZE 1000

#define DSP_ROM_DESC

/*********************************************************************************************/
#ifdef DSP_ROM_DESC
/* sample descriptor */
const char DSP_SAMPLE_DESC[] PROGMEM =
// name,xs,ys,bpp,interface, (HEX) address, scl,sda,reset
// '*' means take pin number from tasmota
":H\n"
"SH1106,128,64,1,I2C,3c,*,*,*\n"
// splash settings, font, size, fgcol, bgcol, x,y
":S\n"
"0,1,1,0,40,20\n"
// init register settings, must be in HEX
":I\n"
"AE\n"
"D5,80\n"
"A8,3f\n"
"D3,00\n"
"40\n"
"8D,14\n"
"20,00\n"
"A1\n"
"C8\n"
"DA,12\n"
"81,CF\n"
"D9F1\n"
"DB,40\n"
"A4\n"
"A6\n"
"AF\n"
// switch display off
":o\n"
"AE\n"
// switch display on
":O\n"
"AF\n"
"#\n";

#endif // DSP_ROM_DESC
/*********************************************************************************************/

void Init_uDisp(void) {
char *ddesc = 0;
char *fbuff;

  if (TasmotaGlobal.gpio_optiona.udisplay_driver) {
    Settings.display_model = XDSP_17;

    fg_color = 1;
    bg_color = 0;
    color_type = COLOR_BW;

    fbuff = (char*)calloc(DISPDESC_SIZE, 1);
    if (!fbuff) return;

#ifdef USE_UFILESYS
    if (ufsp  && !TasmotaGlobal.no_autoexec) {
      File fp;
      fp = ufsp->open("/dispdesc.txt", "r");
      if (fp > 0) {
        uint32_t size = fp.size();
        fp.read((uint8_t*)fbuff, size);
        fp.close();
        ddesc = fbuff;
        AddLog(LOG_LEVEL_INFO, PSTR("DSP: File descriptor used"));
      }
    }
#endif


#ifdef USE_SCRIPT
    if (bitRead(Settings.rule_enabled, 0) && !ddesc) {
      uint8_t dfound = Run_Scripter(">d",-2,0);
      if (dfound == 99) {
        char *lp = glob_script_mem.section_ptr + 2;
        while (*lp != '\n') lp++;
        memcpy(fbuff, lp + 1, DISPDESC_SIZE - 1);
        ddesc = fbuff;
        AddLog(LOG_LEVEL_INFO, PSTR("DSP: Script descriptor used"));
      }
    }
#endif // USE_SCRIPT


#ifdef DSP_ROM_DESC
    if (!ddesc) {
      memcpy_P(fbuff, DSP_SAMPLE_DESC, sizeof(DSP_SAMPLE_DESC));
      ddesc = fbuff;
      AddLog(LOG_LEVEL_INFO, PSTR("DSP: Flash descriptor used"));
    }
#endif // DSP_ROM_DESC

    if (!ddesc) {
      AddLog(LOG_LEVEL_INFO, PSTR("DSP: No valid descriptor found"));
      if (fbuff) free(fbuff);
      return;
    }
    // now replace tasmota vars before passing to driver
    char *cp = strstr(ddesc, "I2C");
    if (cp) {
      cp += 4;
      //,3c,22,21,-1
      // i2c addr
      //if (*cp == '*') {
      //  Settings.display_address
      //}
      uint8_t i2caddr = strtol(cp, 0, 16);
      if (I2cSetDevice(i2caddr)) {
        I2cSetActiveFound(i2caddr, "DSP-I2C");
      }
      cp+=3;
      //replacepin(&cp, Settings.display_address);
      replacepin(&cp, Pin(GPIO_I2C_SCL));
      replacepin(&cp, Pin(GPIO_I2C_SDA));
      replacepin(&cp, Pin(GPIO_OLED_RESET));
    }

    cp = strstr(ddesc, "SPI");
    if (cp) {
      cp += 4;
      //; 7 params nr,cs,sclk,mosi,dc,bl,reset,miso
      //SPI,*,*,*,*,*,*,*
      if (*cp == '1') {
        cp+=2;
        replacepin(&cp, Pin(GPIO_SPI_CS));
        replacepin(&cp, Pin(GPIO_SPI_CLK));
        replacepin(&cp, Pin(GPIO_SPI_MOSI));
        replacepin(&cp, Pin(GPIO_SPI_DC));
        replacepin(&cp, Pin(GPIO_BACKLIGHT));
        replacepin(&cp, Pin(GPIO_OLED_RESET));
        replacepin(&cp, Pin(GPIO_SPI_MISO));
      } else {
        // soft spi pins
        cp+=2;
        replacepin(&cp, Pin(GPIO_SSPI_CS));
        replacepin(&cp, Pin(GPIO_SSPI_SCLK));
        replacepin(&cp, Pin(GPIO_SSPI_MOSI));
        replacepin(&cp, Pin(GPIO_SSPI_DC));
        replacepin(&cp, Pin(GPIO_BACKLIGHT));
        replacepin(&cp, Pin(GPIO_OLED_RESET));
        replacepin(&cp, Pin(GPIO_SSPI_MISO));
      }
    }

    // init renderer
    if (udisp) delete udisp;
    udisp  = new uDisplay(ddesc);

/*
    File fp;
    fp = ufsp->open("/dump.txt", "w");
    fp.write(ddesc, DISPDESC_SIZE);
    fp.close();
*/
    // release desc buffer
    if (fbuff) free(fbuff);

    renderer = udisp->Init();
    if (!renderer) return;

    Settings.display_width = renderer->width();
    Settings.display_height = renderer->height();
    fg_color = udisp->fgcol();
    bg_color = udisp->bgcol();

    renderer->DisplayInit(DISPLAY_INIT_MODE, Settings.display_size, Settings.display_rotate, Settings.display_font);


#ifdef SHOW_SPLASH
    udisp->Splash();
#endif

    udisp_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: %s!"), udisp->devname());
  }
}


/*********************************************************************************************/

void replacepin(char **cp, uint16_t pin) {
  char *lp = *cp;
  if (*lp == ',') lp++;
  if (*lp == '*') {
    char val[8];
    itoa(pin, val, 10);
    uint16_t slen = strlen(val);
    //AddLog(LOG_LEVEL_INFO, PSTR("replace pin: %d"), pin);
    memmove(lp + slen, lp + 1, strlen(lp));
    memmove(lp, val, slen);
  }
  char *np = strchr(lp, ',');
  if (np) {
    *cp = np + 1;
  }
}

#ifdef USE_DISPLAY_MODES1TO5

void UDISP_PrintLog(void)
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }

    char* txt = DisplayLogBuffer('\370');
    if (txt != NULL) {
      uint8_t last_row = Settings.display_rows -1;

      renderer->clearDisplay();
      renderer->setTextSize(Settings.display_size);
      renderer->setCursor(0,0);
      for (byte i = 0; i < last_row; i++) {
        strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
        renderer->println(disp_screen_buffer[i]);
      }
      strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
      DisplayFillScreen(last_row);

      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);

      renderer->println(disp_screen_buffer[last_row]);
      renderer->Updateframe();
    }
  }
}

void UDISP_Time(void)
{
  char line[12];

  renderer->clearDisplay();
  renderer->setTextSize(Settings.display_size);
  renderer->setTextFont(Settings.display_font);
  renderer->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  renderer->println(line);
  renderer->println();
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  renderer->println(line);
  renderer->Updateframe();
}

void UDISP_Refresh(void)  // Every second
{
  if (!renderer) return;
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        UDISP_Time();
        break;
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        UDISP_PrintLog();
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp17(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    Init_uDisp();
  }
  else if (udisp_init_done && (XDSP_17 == Settings.display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        UDISP_Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif  // USE_UNIVERSAL_DISPLAY
#endif  // USE_DISPLAY
