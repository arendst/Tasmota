#include "touchscreen.h"

#ifdef USE_TOUCHSCREEN

TouchScreen *touchscreen = NULL;

void touchscreen_setup()
{
  touchscreen = new TouchScreen();
  
  touchscreen->_tft->fillScreen(ILI9341_BLUE);
  
  touchscreen->_tft->drawRoundRect(24, 48, 80, 80, 8, ILI9341_GREEN);
  touchscreen->_tft->drawRoundRect(120, 48, 80, 80, 8, ILI9341_GREEN);
  touchscreen->_tft->drawRoundRect(216, 48, 80, 176, 8, ILI9341_GREEN);
  touchscreen->_tft->drawRoundRect(24, 144, 176, 80, 8, ILI9341_GREEN);

  touchscreen->_tft->drawFastHLine(0, 24, 320, ILI9341_WHITE);
  touchscreen->_tft->drawFastHLine(0, 25, 320, ILI9341_WHITE);

  /*
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
  }
  Serial.println("OK!");
  */
}

void touchscreen_handle() 
{
  touchscreen->handle();
}

TouchScreen::TouchScreen()
{
  _tft = new Adafruit_ILI9341(TFT_CS, TFT_DC);
  _tft->begin();
  _tft->setRotation(1);

  _ts = new Adafruit_STMPE610(STMPE_CS);
  _ts->begin();
}

void TouchScreen::handle()
{
  _millis_now = millis();
  handleTop();
  handleTouch();
}

void TouchScreen::_handleWidget_Time(int period)
{
  static unsigned long _last = 0;
  unsigned long time_in_seconds;
  char _timestring[9]; // HH:MM:SS\0
  
  if ((_millis_now < _last+period) && _last > 0) 
    return;

  _last = _millis_now;

  time_in_seconds = (_millis_now / 1000) % (24*60*60);
  sprintf(_timestring, "%02d:%02d:%02d", (time_in_seconds/(60*60))%24, (time_in_seconds/(60))%60, time_in_seconds%60);
  _tft->setCursor(320-4-9*5,8);
  _tft->setTextColor(ILI9341_WHITE, ILI9341_BLUE);
  _tft->setTextSize(1);
  _tft->print(_timestring);
}

// wifi_signal_strength in [0..100]
void TouchScreen::_handleWidget_Wifi(int period, int wifi_signal_strength)
{
  static unsigned long _last = 0;
  int w;
  
  if ((_millis_now < _last+period) && _last > 0) 
    return;
  _last = _millis_now;
  _tft->fillTriangle(248, 18, 264, 18, 264, 2, ILI9341_DARKGREY);
  // Map signal strength from [0..100] to [0..16]
  w = map(wifi_signal_strength, 0, 100, 0, 16);
  if (w>0)
    _tft->fillTriangle(248, 18, 248+w, 18, 248+w, 18-w, ILI9341_WHITE);
  else {
  _tft->setCursor(248+10,10);
  _tft->setTextColor(ILI9341_RED, ILI9341_DARKGREY);
  _tft->setTextSize(1);
  _tft->print("X");
  }
}

void TouchScreen::NetworkSend()
{
  _handleWidget_NetworkSend(ILI9341_GREEN);
  _millis_network_send_clear = millis()+100;
}

void TouchScreen::NetworkRecv()
{
  _handleWidget_NetworkRecv(ILI9341_GREEN);
  _millis_network_recv_clear = millis()+100;
}


void TouchScreen::_handleWidget_Alarm(int period)
{
  static unsigned long _last;
  if ((_millis_now < _last+period) && _last > 0) 
    return;
  _last = _millis_now;  
}

void TouchScreen::_handleWidget_NetworkSend(int color)
{
  _tft->fillTriangle(224,10, 232, 10, 228, 2, color);
  _tft->fillRect(227, 10, 3, 8, color);
}

void TouchScreen::_handleWidget_NetworkRecv(int color)
{
  _tft->fillTriangle(232, 10, 240, 10, 236, 18, color);
  _tft->fillRect(235, 2, 3, 8, color);
}

void TouchScreen::_handleWidget_Network(int period)
{
  static unsigned long _last = 0;
  if ((_millis_now < _last+period) && _last > 0) 
    return;

  if (random(100) < 3)
    NetworkSend();
  if (random(100) < 10)
    NetworkRecv();

  if ((_millis_now > _millis_network_send_clear && _millis_network_send_clear != 0) || _last==0) {
    _millis_network_send_clear=0;
    _handleWidget_NetworkSend(ILI9341_DARKGREY);
  }
  if ((_millis_now > _millis_network_recv_clear && _millis_network_recv_clear != 0) || _last==0) {
    _millis_network_recv_clear=0;
    _handleWidget_NetworkRecv(ILI9341_DARKGREY);
  }
  _last = _millis_now;  
}

void TouchScreen::_handleWidget_Name(int period)
{
  char _namestring[21];
  static unsigned long _last = 0;
  if ((_millis_now < _last+period) && _last > 0) 
    return;
  _last = _millis_now;

  const char names[7][21] = { "Livingroom", "Attic", "Basement", "1234", "", "TestSetup", "01234567890123456789" };
  sprintf(_namestring, "%-20s", names[random(7)]);
  _tft->setCursor(4,8);
  _tft->setTextColor(ILI9341_WHITE, ILI9341_BLUE);
  _tft->setTextSize(1);
  _tft->print(_namestring);
}

void TouchScreen::handleTop()
{
  _handleWidget_Time(1000);
  _handleWidget_Wifi(1800, random(100));
  _handleWidget_Alarm(1000);
  _handleWidget_Network();
  _handleWidget_Name(1300);
}

void TouchScreen::handleTouch()
{
  // Handle buffered touchscreen
  if (!_ts->touched()) {
    if (_touch_last_type == TOUCH_DOWN) {
      _touch_last_type = TOUCH_UP;
      _touch_last_up = _touch_last;
      // Callback TOUCH_UP
      Serial.printf("TouchScreen:: TOUCH_UP on (%u,%u)\r\n", _touch_last_up.x, _touch_last_up.y);
      if (((_touch_last_up.y-PENRADIUS) > 0) && ((_touch_last_up.y+PENRADIUS) < _tft->height())) {
        _tft->fillCircle(_touch_last_up.x, _touch_last_up.y, PENRADIUS, ILI9341_RED);
      }
    }
    _touch_last_type = TOUCH_NONE;
    return;
  }

  TS_Point p = _ts->getPoint();
  if (p.z == 0) {
    _touch_last_type = TOUCH_NONE;
    return;
  }

  // The TouchScreen hardware yields a number between 0..4000
  // but is a bit noisy at the edges. First, constrain outliers
  // into [TS_MINX,TS_MAXX] and [TS_MINY,TS_MAXY] respectively.
  p.x=constrain(p.x,TS_MINX, TS_MAXX);
  p.y=constrain(p.y,TS_MINY, TS_MAXY);

  // Then, map the raw coordinates to the screen dimension,
  // making the rotation mapping along the way.
  // PS will have values from [0,tftwidth) and [0,tftheight)
  TS_Point ps;
  ps.x = map(p.y, TS_MINY, TS_MAXY, 0, _tft->width()-1);
  ps.y = map(p.x, TS_MINX, TS_MAXX, 0, _tft->height()-1);
  ps.z = p.z;
  
  if (ps == _touch_last) {
    return;
  }
  _touch_last = ps;
  if (_touch_last_type != TOUCH_DOWN) {
    _touch_last_type = TOUCH_DOWN;
    _touch_last_down = _touch_last;
    // Callback TOUCH_DOWN
    Serial.printf("TouchScreen:: TOUCH_DOWN on (%u,%u)\r\n", _touch_last_down.x, _touch_last_down.y);
    if (((_touch_last_down.y-PENRADIUS) > 0) && ((_touch_last_down.y+PENRADIUS) < _tft->height())) {
      _tft->fillCircle(_touch_last_down.x, _touch_last_down.y, PENRADIUS, ILI9341_WHITE);
    }
  }
}

#endif // USE_TOUCHSCREEN
