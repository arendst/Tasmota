/*
  xdsp_16_esp32_epaper_47.ino - LILIGO47 e-paper support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz and LILIGO

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

#ifdef ESP32
#ifdef USE_DISPLAY
#ifdef USE_LILYGO47

#define XDSP_16                16

#define EPD47_BLACK  0
#define EPD47_WHITE 15

#include <epd4in7.h>

Epd47 *epd47;
bool epd47_init_done = false;
extern uint8_t color_type;
extern uint16_t fg_color;
extern uint16_t bg_color;

/*********************************************************************************************/

void EpdInitDriver47(void) {
  if (PinUsed(GPIO_EPD_DATA)) {

    Settings->display_model = XDSP_16;

    if (Settings->display_width != EPD47_WIDTH) {
      Settings->display_width = EPD47_WIDTH;
    }
    if (Settings->display_height != EPD47_HEIGHT) {
      Settings->display_height = EPD47_HEIGHT;
    }

    // init renderer
    epd47 = new Epd47(Settings->display_width, Settings->display_height);
    epd47->Init();

    renderer = epd47;
    renderer->DisplayInit(DISPLAY_INIT_MODE, Settings->display_size, Settings->display_rotate, Settings->display_font);
    renderer->setTextColor(EPD47_BLACK, EPD47_WHITE);

#ifdef SHOW_SPLASH
    if (!Settings->flag5.display_no_splash) {  // SetOption135 - (Display & LVGL) force disabling default splash screen
      // Welcome text
      renderer->setTextFont(2);
      renderer->DrawStringAt(50, 50, "LILYGO 4.7 E-Paper Display!", EPD47_BLACK, 0);
      renderer->Updateframe();
    }
#endif

    fg_color = EPD47_BLACK;
    bg_color = EPD47_WHITE;
    color_type = COLOR_COLOR;

#ifdef USE_TOUCH_BUTTONS
    // start digitizer
    EPD47_Touch_Init();
#endif // USE_TOUCH_BUTTONS

    epd47_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: E-Paper 4.7"));
  }
}

/*********************************************************************************************/


#ifdef USE_TOUCH_BUTTONS

#define TOUCH_SLAVE_ADDRESS 0x5a
class TouchClass {

    typedef struct {
        uint8_t id;
        uint8_t state;
        uint16_t x;
        uint16_t y;
    } TouchData_t;

public:
    bool    begin(TwoWire &port = Wire, uint8_t addr = TOUCH_SLAVE_ADDRESS);
    uint8_t scanPoint();
    void    getPoint(int16_t &x, int16_t &y, uint8_t index);
//    void    sleep(void);
//    void    wakeup(void);
    TouchData_t data[5];

private:
    void    clearFlags(void);
    void    readBytes(uint8_t *data, uint8_t nbytes);
    uint8_t _address;
    bool initialization = false;
    TwoWire *_i2cPort;
};

void TouchClass::readBytes(uint8_t *data, uint8_t nbytes) {
    _i2cPort->beginTransmission(_address);  // Initialize the Tx buffer
    _i2cPort->write(data, 2);                // Put data in Tx buffer
    if (0 != _i2cPort->endTransmission()) {
        Serial.println("readBytes error!");
    }
    uint8_t i = 0;
    _i2cPort->requestFrom(_address, nbytes);  // Read bytes from slave register address
    while (_i2cPort->available()) {
        data[i++] = _i2cPort->read();
    }
}

void TouchClass::clearFlags(void) {
    uint8_t buf[3] = {0xD0, 0X00, 0XAB};
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(buf, 3);
    _i2cPort->endTransmission();
}

bool TouchClass::begin(TwoWire &port, uint8_t addr) {
    _i2cPort = &port;
    _address = addr;
    _i2cPort->beginTransmission(_address);
    if (0 == _i2cPort->endTransmission()) {
      //  wakeup();
        return true;
    }
    return false;
}

uint8_t TouchClass::scanPoint() {
    uint8_t point = 0;
    uint8_t buffer[40] = {0};
    uint32_t sumL = 0, sumH = 0;

    buffer[0] = 0xD0;
    buffer[1] = 0x00;
    readBytes(buffer, 7);

    if (buffer[0] == 0xAB) {
        clearFlags();
        return 0;
    }

    point = buffer[5] & 0xF;

    if (point == 1) {
        buffer[5] = 0xD0;
        buffer[6] = 0x07;
        readBytes( &buffer[5], 2);
        sumL = buffer[5] << 8 | buffer [6];

    } else if (point > 1) {
        buffer[5] = 0xD0;
        buffer[6] = 0x07;
        readBytes( &buffer[5], 5 * (point - 1) + 3);
        sumL = buffer[5 * point + 1] << 8 | buffer[5 * point + 2];
    }
    clearFlags();

    for (int i = 0 ; i < 5 * point; ++i) {
        sumH += buffer[i];
    }

    if (sumH != sumL) {
        point = 0;
    }
    if (point) {
        uint8_t offset;
        for (int i = 0; i < point; ++i) {
            if (i == 0) {
                offset = 0;
            } else {
                offset = 4;
            }
            data[i].id =  (buffer[i * 5 + offset] >> 4) & 0x0F;
            data[i].state = buffer[i * 5 + offset] & 0x0F;
            if (data[i].state == 0x06) {
                data[i].state = 0x07;
            } else {
                data[i].state = 0x06;
            }
            data[i].y = (uint16_t)((buffer[i * 5 + 1 + offset] << 4) | ((buffer[i * 5 + 3 + offset] >> 4) & 0x0F));
            data[i].x = (uint16_t)((buffer[i * 5 + 2 + offset] << 4) | (buffer[i * 5 + 3 + offset] & 0x0F));
        }
    } else {
        point = 1;
        data[0].id = (buffer[0] >> 4) & 0x0F;
        data[0].state = 0x06;
        data[0].y = (uint16_t)((buffer[0 * 5 + 1] << 4) | ((buffer[0 * 5 + 3] >> 4) & 0x0F));
        data[0].x = (uint16_t)((buffer[0 * 5 + 2] << 4) | (buffer[0 * 5 + 3] & 0x0F));
    }
    // Serial.printf("X:%d Y:%d\n", data[0].x, data[0].y);
    return point;
}

void TouchClass::getPoint(int16_t &x, int16_t &y, uint8_t index) {
    if (index >= 4)return;
    x = data[index].x;
    y = data[index].y;
}

//if (touch.scanPoint()) {
  //          touch.getPoint(x, y, 0);

#define EPD47_address 0x5A

TouchClass *EPD47_touchp;

void EPD47_Touch_Init(void) {
FT5206_found = false;
EPD47_touchp = new TouchClass();
  if (EPD47_touchp->begin(Wire, EPD47_address)) {
    I2cSetActiveFound(EPD47_address, "EPD47");
    FT5206_found = true;
  }
}

uint8_t EPD47_ctouch_counter = 0;
// no rotation support
void EPD47_RotConvert(int16_t *x, int16_t *y) {
int16_t temp;
  if (renderer) {
    uint8_t rot=renderer->getRotation();
    switch (rot) {
      case 0:
        break;
      case 1:
        temp=*y;
        *y=renderer->height()-*x;
        *x=temp;
        break;
      case 2:
        *x=renderer->width()-*x;
        *y=renderer->height()-*y;
        break;
      case 3:
        temp=*y;
        *y=*x;
        *x=renderer->width()-temp;
        break;
    }
  }
}

// check digitizer hit
void EPD47_CheckTouch(void) {
  EPD47_ctouch_counter++;
  if (2 == EPD47_ctouch_counter) {
    // every 100 ms should be enough
    EPD47_ctouch_counter = 0;
    touched = EPD47_touchp->scanPoint();
    if (touched) {
      EPD47_touchp->getPoint(touch_xp, touch_yp, 0);
      EPD47_RotConvert(&touch_xp, &touch_yp);
    }
    //Touch_Check(EPD47_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS

#ifdef USE_DISPLAY_MODES1TO5

void EPD47_PrintLog(void) {
  // This can take over 3 seconds depending on renderer->Updateframe() speed
  //   due to not connected busy pin (configure as MISO)
  static bool printlog_mutex = false;

  if (disp_refresh) { disp_refresh--; }
  if (disp_refresh || printlog_mutex || TasmotaGlobal.restart_flag || TasmotaGlobal.ota_state_flag) {
    return;
  }
  printlog_mutex = true;
  disp_refresh = Settings->display_refresh;
  if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }

  char* txt = DisplayLogBuffer('\370');
  if (txt != nullptr) {
    uint8_t last_row = Settings->display_rows -1;

//      renderer->clearDisplay();
    renderer->setTextSize(Settings->display_size);
    renderer->setCursor(0,0);
    for (byte i = 0; i < last_row; i++) {
      strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
      renderer->println(disp_screen_buffer[i]);
    }
    strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
    DisplayFillScreen(last_row);
    renderer->println(disp_screen_buffer[last_row]);
    renderer->Updateframe();
  }
  printlog_mutex = false;
}

void EPD47_Time(void) {
  if (disp_refresh) { disp_refresh--; }
  if (disp_refresh || TasmotaGlobal.restart_flag || TasmotaGlobal.ota_state_flag) {
    return;
  }
  disp_refresh = Settings->display_refresh;

  char line[12];

//  renderer->clearDisplay();
  renderer->setTextSize(Settings->display_size);
  renderer->setTextFont(Settings->display_font);
  renderer->setCursor(0, 10);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  renderer->println(line);
  renderer->println();
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  renderer->println(line);
  renderer->Updateframe();
}

void EPD47_Refresh(void) {  // Every second
  if (!renderer) return;
  if (DM_TIME == Settings->display_mode) {
    EPD47_Time();
  }
  else if (Settings->display_mode > DM_TIME) {
    EPD47_PrintLog();
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp16(uint32_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    EpdInitDriver47();
  }
  else if (epd47_init_done && (XDSP_16 == Settings->display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_TOUCH_BUTTONS
      case FUNC_DISPLAY_EVERY_50_MSECOND:
        if (FT5206_found) {
          EPD47_CheckTouch();
        }
        break;
#endif // USE_TOUCH_BUTTONS

#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        EPD47_Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif  // USE_LILYGO47
#endif  // USE_DISPLAY
#endif  // ESP32
