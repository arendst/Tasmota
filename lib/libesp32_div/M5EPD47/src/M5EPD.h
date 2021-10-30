#ifndef _M5EPD_H_
#define _M5EPD_H_

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include "Button.h"
#include "GT911.h"
#include "M5EPD_Driver.h"

#include "esp_adc_cal.h"

#define M5EPD_MAIN_PWR_PIN 2
#define M5EPD_CS_PIN 15
#define M5EPD_SCK_PIN 14
#define M5EPD_MOSI_PIN 12
#define M5EPD_BUSY_PIN 27
#define M5EPD_MISO_PIN 13
#define M5EPD_EXT_PWR_EN_PIN 5
#define M5EPD_EPD_PWR_EN_PIN 23
#define M5EPD_KEY_RIGHT_PIN 39
#define M5EPD_KEY_PUSH_PIN 38
#define M5EPD_KEY_LEFT_PIN 37
#define M5EPD_BAT_VOL_PIN 35
#define M5EPD_PORTC_W_PIN 19
#define M5EPD_PORTC_Y_PIN 18
#define M5EPD_PORTB_W_PIN 33
#define M5EPD_PORTB_Y_PIN 26
#define M5EPD_PORTA_W_PIN 32
#define M5EPD_PORTA_Y_PIN 25

#define M5EPD_CS_SD_PIN 4



class M5EPD
{
public:
    M5EPD();
    void begin();
    void update();
    void enableEXTPower() { digitalWrite(M5EPD_EXT_PWR_EN_PIN, 1); }
    void disableEXTPower() { digitalWrite(M5EPD_EXT_PWR_EN_PIN, 0); }
    void enableEPDPower() { digitalWrite(M5EPD_EPD_PWR_EN_PIN, 1); }
    void disableEPDPower() { digitalWrite(M5EPD_EPD_PWR_EN_PIN, 0); }
    void enableMainPower() { digitalWrite(M5EPD_MAIN_PWR_PIN, 1); }
    void disableMainPower() { digitalWrite(M5EPD_MAIN_PWR_PIN, 0); }
    void BatteryADCBegin();
    uint32_t getBatteryRaw();
    uint32_t getBatteryVoltage();

  //  void shutdown();
  //  int shutdown( int seconds );
  //  int shutdown( const rtc_time_t &RTC_TimeStruct);
  //  int shutdown( const rtc_date_t &RTC_DateStruct, const rtc_time_t &RTC_TimeStruct);

//    Button BtnL = Button(M5EPD_KEY_LEFT_PIN, true, 10);
//    Button BtnP = Button(M5EPD_KEY_PUSH_PIN, true, 10);
//    Button BtnR = Button(M5EPD_KEY_RIGHT_PIN, true, 10);

//    M5EPD_Driver EPD = M5EPD_Driver();
//    GT911 TP = GT911();
//    BM8563 RTC = BM8563();
//    SHT3x SHT30 = SHT3x();

private:
    bool _is_adc_start;
    bool _isInited;
    esp_adc_cal_characteristics_t *_adc_chars;
};

extern M5EPD M5;

#endif
