/*
  xdrv_83_esp32_watch.ino - ESP32 TTGO watch support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#ifdef USE_TTGO_WATCH

#include <axp20x.h>
#include <i2c_bus.h>
#include <bma.h>

#define XDRV_83           83

#define AXP202_INT        35

struct TTGO_ADC {
  float vbus_v;
  float vbus_c;
  float batt_v;
  float batt_c;
  float temp;
  uint16_t per;
} ttgo_adc;

enum {
    Q_EVENT_WIFI_SCAN_DONE,
    Q_EVENT_WIFI_CONNECT,
    Q_EVENT_BMA_INT,
    Q_EVENT_AXP_INT,
} ;

#define BMA423_INT1         39
#define BMA423_INT2         0

#define WATCH_FLAG_SLEEP_MODE   _BV(1)
#define WATCH_FLAG_SLEEP_EXIT   _BV(2)
#define WATCH_FLAG_BMA_IRQ      _BV(3)
#define WATCH_FLAG_AXP_IRQ      _BV(4)

struct TTGO_globs {
  AXP20X_Class *ttgo_power = nullptr;
  I2CBus *i2c = nullptr;
  BMA *bma = nullptr;
  QueueHandle_t g_event_queue_handle = NULL;
  //EventGroupHandle_t g_event_group = NULL;
  EventGroupHandle_t isr_group = NULL;
  bool lenergy = false;
  bool bma_double_click = false;
  bool bma_click = false;
  bool bma_button = false;
  bool power_ok = false;
} ttgo_globs;


void TTGO_Init(void) {
  ttgo_globs.ttgo_power = new AXP20X_Class();
  ttgo_globs.i2c = new I2CBus();
  TTGO_initPower();

#ifdef USE_BMA423
  ttgo_globs.bma = new BMA(*ttgo_globs.i2c);
  if (ttgo_globs.bma->begin()) {
    I2cSetActiveFound(BMA4_I2C_ADDR_SECONDARY, "BMA423");
  } else {
    return;
  }

  ttgo_globs.bma->attachInterrupt();
  pinMode(BMA423_INT1, INPUT);
  attachInterrupt(BMA423_INT1, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        EventBits_t  bits = xEventGroupGetBitsFromISR(ttgo_globs.isr_group);
        if (bits & WATCH_FLAG_SLEEP_MODE)
        {
            // Use an XEvent when waking from low energy sleep mode.
            xEventGroupSetBitsFromISR(ttgo_globs.isr_group, WATCH_FLAG_SLEEP_EXIT | WATCH_FLAG_BMA_IRQ, &xHigherPriorityTaskWoken);
        } else
        {
            // Use the XQueue mechanism when we are already awake.
            uint8_t data = Q_EVENT_BMA_INT;
            xQueueSendFromISR(ttgo_globs.g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        }

        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
  }, RISING);
  struct bma423_axes_remap remap_data;

   remap_data.x_axis = 0;
   remap_data.x_axis_sign = 1;
   remap_data.y_axis = 1;
   remap_data.y_axis_sign = 0;
   remap_data.z_axis  = 2;
   remap_data.z_axis_sign  = 1;

   ttgo_globs.bma->set_remap_axes(&remap_data);
    // Enable the double tap wakeup.
   ttgo_globs.bma->enableWakeupInterrupt(true);
   ttgo_globs.bma->enableAnyNoMotionInterrupt(true);
   ttgo_globs.bma->enableAccel();
#endif // USE_BMA423
}

void TTGO_initPower(void) {
  int ret = ttgo_globs.ttgo_power->begin(axpReadBytes, axpWriteBytes);
  if (ret == AXP_FAIL) {
      //DBGX("AXP Power begin failed");
    //  Serial.printf("AXP202 failed\n" );
  } else {
    I2cSetActiveFound(AXP202_SLAVE_ADDRESS, "AXP202");
    ttgo_globs.power_ok = true;
  //  Serial.printf("AXP202 OK\n" );

    //Change the button boot time to 4 seconds
    ttgo_globs.ttgo_power->setShutdownTime(AXP_POWER_OFF_TIME_4S);
    // Turn off the charging instructions, there should be no
    ttgo_globs.ttgo_power->setChgLEDMode(AXP20X_LED_OFF);
    // Turn off external enable
    ttgo_globs.ttgo_power->setPowerOutPut(AXP202_EXTEN, false);
    //axp202 allows maximum charging current of 1800mA, minimum 300mA
    ttgo_globs.ttgo_power->setChargeControlCur(300);

    ttgo_globs.ttgo_power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);

    ttgo_globs.ttgo_power->adc2Enable(AXP202_TEMP_MONITORING_ADC2, true);

    ttgo_globs.ttgo_power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo_globs.ttgo_power->clearIRQ();

    ttgo_globs.ttgo_power->setPowerOutPut(AXP202_LDO2, AXP202_ON);

    ttgo_globs.isr_group = xEventGroupCreate();
    ttgo_globs.g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));

    // Connection interrupted to the specified pin
    pinMode(AXP202_INT, INPUT);
    attachInterrupt(AXP202_INT, [] {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            EventBits_t  bits = xEventGroupGetBitsFromISR(ttgo_globs.isr_group);
            if (bits & WATCH_FLAG_SLEEP_MODE) {
                // Use an XEvent when waking from low energy sleep mode.
                xEventGroupSetBitsFromISR(ttgo_globs.isr_group, WATCH_FLAG_SLEEP_EXIT | WATCH_FLAG_AXP_IRQ, &xHigherPriorityTaskWoken);
            } else {
                // Use the XQueue mechanism when we are already awake.
                uint8_t data = Q_EVENT_AXP_INT;
                xQueueSendFromISR(ttgo_globs.g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
            }
            if (xHigherPriorityTaskWoken) {
                portYIELD_FROM_ISR ();
            }
    }, FALLING);

  }

}

static uint8_t axpWriteBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t len) {
  ttgo_globs.i2c->writeBytes(devAddress, regAddress, data, len);
  return 0;
}

static uint8_t axpReadBytes(uint8_t devAddress, uint8_t regAddress,  uint8_t *data, uint8_t len) {
  ttgo_globs.i2c->readBytes(devAddress, regAddress, data, len);
  return 0;
}


void TTGO_GetADC(void) {
    ttgo_adc.vbus_v = ttgo_globs.ttgo_power->getVbusVoltage();
    ttgo_adc.vbus_c = ttgo_globs.ttgo_power->getVbusCurrent();
    ttgo_adc.batt_v = ttgo_globs.ttgo_power->getBattVoltage();
    ttgo_adc.per = ttgo_globs.ttgo_power->getBattPercentage();
    ttgo_adc.batt_c = ttgo_globs.ttgo_power->getBattDischargeCurrent();
    ttgo_adc.temp = ttgo_globs.ttgo_power->getTemp();
}

#ifdef USE_WEBSERVER
const char HTTP_TTGO[] PROGMEM =
 "{s}TTGO " "VBUS Voltage" "{m}%s mV" "{e}"
 "{s}TTGO " "VBUS Current" "{m}%s mA" "{e}"
 "{s}TTGO " "BATT Voltage" "{m}%s mV" "{e}"
 "{s}TTGO " "BATT Current" "{m}%s mA" "{e}"
 "{s}TTGO " "BATT Percentage" "{m}%d %%" "{e}"
 "{s}TTGO " "Temperature" "{m}%s C" "{e}";
#ifdef USE_BMA423
const char HTTP_TTGO_BMA[] PROGMEM =
 "{s}TTGO " "BMA x" "{m}%d mg" "{e}"
 "{s}TTGO " "BMA y" "{m}%d mg" "{e}"
 "{s}TTGO " "BMA z" "{m}%d mg" "{e}";
#endif // USE_BMA423
#endif  // USE_WEBSERVER


void TTGO_WebShow(uint32_t json) {

  if (ttgo_globs.power_ok == false) return;

  TTGO_GetADC();

  char vstring[32];
  char cstring[32];
  char bvstring[32];
  char bcstring[32];
  char tstring[32];
  dtostrfd(ttgo_adc.vbus_v,2,vstring);
  dtostrfd(ttgo_adc.vbus_c,2,cstring);
  dtostrfd(ttgo_adc.batt_v,2,bvstring);
  dtostrfd(ttgo_adc.batt_c,2,bcstring);
  dtostrfd(ttgo_adc.temp,2,tstring);

#ifdef USE_BMA423
    Accel acc;
    bool res = ttgo_globs.bma->getAccel(acc);
#endif // USE_BMA423

  if (json) {
    ResponseAppend_P(PSTR(",\"TTGO_WATCH\":{\"VBV\":%s,\"VBC\":%s,\"BV\":%s,\"BC\":%s,\"BP\":%d,\"CT\":%s"),
                     vstring, cstring, bvstring, bcstring, ttgo_adc.per, tstring);
#ifdef USE_BMA423
    ResponseAppend_P(PSTR(",\"BMAX\":%d,\"BMAY\":%d,\"BMAZ\":%d"),acc.x,acc.y,acc.z);
#endif
    ResponseJsonEnd();
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_TTGO,vstring,cstring,bvstring,bcstring,ttgo_adc.per,tstring);
#ifdef USE_BMA423
    WSContentSend_PD(HTTP_TTGO_BMA,acc.x,acc.y,acc.z);
#endif // USE_BMA423
#endif  // USE_WEBSERVER
  }
}


void TTGO_enableLDO3(bool en = true) {
  if (!ttgo_globs.ttgo_power) return;
  ttgo_globs.ttgo_power->setLDO3Mode(1);
  ttgo_globs.ttgo_power->setPowerOutPut(AXP202_LDO3, en);
}

void TTGO_audio_power(bool power) { // Not every watch has audio
    TTGO_enableLDO3(power);
}

const char TTGO_Commands[] PROGMEM = "TTGO|"
  "LSLP";

void (* const TTTGO_Command[])(void) PROGMEM = {
  &TTGO_LightSleep};

void TTGO_LightSleep(void) {
int32_t ttgo_sleeptime;

    // switch device off
    if ((abs(XdrvMailbox.payload) >= 10) && (abs(XdrvMailbox.payload) <= 3600*24)) {
        ttgo_sleeptime = XdrvMailbox.payload;
    } else {
        ttgo_sleeptime = 0;
    }

    ResponseCmndNumber(ttgo_sleeptime);

    TTGO_Sleep(ttgo_sleeptime);

}

void TTGO_Sleep(int32_t stime) {
int32_t ttgo_sleeptime;

    ttgo_sleeptime = stime;

#ifdef USE_DISPLAY
    DisplayOnOff(0);
#endif
    if (ttgo_sleeptime>=0) {
      // ligh sleep mode
      WifiShutdown();
      SettingsSaveAll();
      RtcSettingsSave();
      ttgo_globs.lenergy = true;
      setCpuFrequencyMhz(10); 
      xEventGroupSetBits(ttgo_globs.isr_group, WATCH_FLAG_SLEEP_MODE);
      gpio_wakeup_enable ((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
      gpio_wakeup_enable ((gpio_num_t)BMA423_INT1, GPIO_INTR_HIGH_LEVEL);
      esp_sleep_enable_gpio_wakeup();
      if (ttgo_sleeptime) {
        esp_sleep_enable_timer_wakeup(ttgo_sleeptime * 1000000);
      }
      esp_light_sleep_start();
    } else {
      ttgo_globs.ttgo_power->setPowerOutPut(0xFF, false);
      Settings->deepsleep = -ttgo_sleeptime;
#ifdef USE_DEEPSLEEP
      RtcSettings.nextwakeup = 0;
      deepsleep_flag = (0 == XdrvMailbox.payload) ? 0 : DEEPSLEEP_START_COUNTDOWN;
      if (deepsleep_flag) {
        if (!Settings->tele_period) {
          Settings->tele_period = TELE_PERIOD;  // Need teleperiod to go back to sleep
        }
      }
#endif
      return;
    }

    if (ttgo_sleeptime) {
      ttgo_globs.lenergy = false;
      setCpuFrequencyMhz(240);
#ifdef USE_DISPLAY
      DisplayOnOff(1);
#endif
    } else {
      while (ttgo_globs.lenergy == true) {
        TTGO_loop(0);
        OsWatchLoop();
      }
    }
}


void TTGO_loop(uint32_t flg) {
bool  rlst;
uint8_t data;

  if (!flg) {
// An XEvent signifies that there has been a wakeup interrupt, bring the CPU out of low energy mode
    EventBits_t  bits = xEventGroupGetBits(ttgo_globs.isr_group);
    if (bits & WATCH_FLAG_SLEEP_EXIT) {
        if (ttgo_globs.lenergy) {
            ttgo_globs.lenergy = false;
            setCpuFrequencyMhz(240);
#ifdef USE_DISPLAY
            DisplayOnOff(1);
#endif
        }

#ifdef USE_BMA423
        if (bits & WATCH_FLAG_BMA_IRQ) {
          do {
            rlst =  ttgo_globs.bma->readInterrupt();
          } while (!rlst);
          xEventGroupClearBits(ttgo_globs.isr_group, WATCH_FLAG_BMA_IRQ);
        }
#endif

        if (bits & WATCH_FLAG_AXP_IRQ) {
          ttgo_globs.ttgo_power->readIRQ();
          ttgo_globs.ttgo_power->clearIRQ();
          xEventGroupClearBits(ttgo_globs.isr_group, WATCH_FLAG_AXP_IRQ);
        }
        xEventGroupClearBits(ttgo_globs.isr_group, WATCH_FLAG_SLEEP_EXIT);
        xEventGroupClearBits(ttgo_globs.isr_group, WATCH_FLAG_SLEEP_MODE);
    }
  } else {
    //! Normal polling
    if (xQueueReceive(ttgo_globs.g_event_queue_handle, &data, 5 / portTICK_RATE_MS) == pdPASS) {
      switch (data) {
#ifdef USE_BMA423
        case Q_EVENT_BMA_INT:
          //    DSERIAL(println, "Q_EVENT_BMA_IRQ");
          do {
            rlst =  ttgo_globs.bma->readInterrupt();
          } while (!rlst);

          if (ttgo_globs.bma->isDoubleClick()) {
            ttgo_globs.bma_double_click = true;
            //AddLog(LOG_LEVEL_INFO, PSTR("double click"));
          }
          if (ttgo_globs.bma->isAnyNoMotion()) {
            ttgo_globs.bma_click = true;
            //AddLog(LOG_LEVEL_INFO, PSTR("click"));
          }

          //! setp counter
          if (ttgo_globs.bma->isStepCounter()) {
            //updateStepCounter(ttgo_globs.bma->getCounter());
          }
          break;
#endif
        case Q_EVENT_AXP_INT:
          // DSERIAL(println, "Q_EVENT_AXP_INT");
          ttgo_globs.ttgo_power->readIRQ();
          if (ttgo_globs.ttgo_power->isVbusPlugInIRQ()) {
            //batState = LV_ICON_CHARGE;
            //updateBatteryIcon(LV_ICON_CHARGE);
          }
          if (ttgo_globs.ttgo_power->isVbusRemoveIRQ()) {
            //batState = LV_ICON_CALCULATION;
            //updateBatteryIcon(LV_ICON_CALCULATION);
          }
          if (ttgo_globs.ttgo_power->isChargingDoneIRQ()) {
            //batState = LV_ICON_CALCULATION;
            //updateBatteryIcon(LV_ICON_CALCULATION);
          }
          if (ttgo_globs.ttgo_power->isPEKShortPressIRQ()) {
            ttgo_globs.bma_button = true;
            //AddLog(LOG_LEVEL_INFO, PSTR("button press"));
          }
          ttgo_globs.ttgo_power->clearIRQ();
          break;
        default:
          break;
      }
    }
  }
}

bool TTGO_doubleclick(void) {
  bool retval = ttgo_globs.bma_double_click;
  ttgo_globs.bma_double_click = false;
  return retval;
}

bool TTGO_button(void) {
  bool retval = ttgo_globs.bma_button;
  ttgo_globs.bma_button = false;
  return retval;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv83(uint32_t function) {
  bool result = false;

  switch (function) {

    case FUNC_WEB_SENSOR:
#ifdef USE_WEBSERVER
      TTGO_WebShow(0);
#endif
      break;
    case FUNC_JSON_APPEND:
      TTGO_WebShow(1);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(TTGO_Commands, TTTGO_Command);
      break;
    case FUNC_INIT:
      TTGO_Init();
      break;
    case FUNC_LOOP:
      TTGO_loop(1);
      break;

  }
  return result;
}

#endif  // USE_TTGO_WATCH
#endif  // ESP32
