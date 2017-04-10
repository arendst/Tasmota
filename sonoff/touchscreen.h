#ifndef __TOUCHSCREEN_H
#define __TOUCHSCREEN_H

#include "user_config.h"

#ifdef USE_TOUCHSCREEN

#include <Arduino.h>
#include <inttypes.h>
#include <SPI.h>
#include <Wire.h>                  // this is needed even tho we aren't using it

#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_ILI9341.h>     // Hardware-specific library
#include <SD.h>
#include <Adafruit_STMPE610.h>

// Pinout specific to Adafruit Huzzah Feather:
// ESP8266 - https://www.adafruit.com/product/2821
// TFT     - https://www.adafruit.com/product/3315
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 250
#define TS_MINY 150
#define TS_MAXX 3750
#define TS_MAXY 3750

#define PENRADIUS 2

#define SCREEN_MAX_BUTTONS 10
typedef enum { TOUCH_NONE, TOUCH_DOWN, TOUCH_UP } ScreenTouch_t;

class TouchScreen
{
  public:
  TouchScreen();
  ~TouchScreen();

  void handle();
  
  Adafruit_ILI9341 *_tft;
  Adafruit_STMPE610 *_ts;
  TS_Point _touch_last_down;    // Last TOUCH_DOWN (x,y)
  TS_Point _touch_last_up;      // Last TOUCH_UP (x,y)

  void NetworkSend();           // Activate Up-Arrow icon, will clear in 100ms
  void NetworkRecv();           // Activate Down-Arrow icon, will clear in 100ms
  
  private:
  ScreenTouch_t _touch_last_type;
  TS_Point _touch_last;         // Last (x,y) registered touch
  unsigned long _millis_now;
  unsigned long _millis_network_send_clear;
  unsigned long _millis_network_recv_clear;

  void handleTouch();
  void handleTop();
  void _handleWidget_Time(int period=1000);
  void _handleWidget_Wifi(int period=5000, int wifi_signal_strength=0);
  void _handleWidget_Alarm(int period=1000);
  void _handleWidget_Name(int period=60000);
  void _handleWidget_Network(int period=100);
  void _handleWidget_NetworkSend(int color);
  void _handleWidget_NetworkRecv(int color);
};

#endif // USE_TOUCHSCREEN

#endif // __TOUCHSCREEN_H
