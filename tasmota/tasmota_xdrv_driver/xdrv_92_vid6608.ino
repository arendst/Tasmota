/*
  xdrv_92_vid6608.ino - Support for VID6608 automotive gauge stepper motor driver

  Copyright (C) 2025  Petr Golovachev

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
#ifdef USE_VID6608

#define XDRV_92             92
#define VID6608_MAX_DRIVES  4

/*
  VID6608 automotive gauge stepper motor driver

  This driver implements support for following driver chips for analog automotive gauges (Switec X25.168, X27.168 and clones) with microstepping support:

    * VID6606 (2 motors)
    * VID6608 (4 motors)
    * VT6608S
    * AX1201728SG
    * BY8920
    * many others

  Driver chips with microstepping is the recommended way to drive such motors,
  they provide much more relailabe and smooth movement with reduced noise and
  to avoid skipping steps.

  Driver is configured to perform 320° rotation angle with 12 steps per degree.
  Total capacity is 3840 steps for whole scale.

  Library homepage: https://github.com/petrows/arduino-vid6608

  Connection:

    * Connect IC VID6608 inputs F(scx) and CW/CCW to GPIO pins
    * Connect RESET pin of VID6608 to VCC
    * Define "VID6608 F" and "VID6608 CW" pins in Configuration / Module page

  Driver adds following commands:

    * Gauge         : returns current gauges state
    * GaugeSetX     : set gauge to absolute position in range 0..3840, where X - motor index from 1 to 4, 0 for all
    * GaugePercentX : set gauge position in percents in range 0..100, where X - motor index from 1 to 4, 0 for all
    * GaugeZeroX    : triggers gauge calibration and homing, where X - motor index from 1 to 4, 0 for all

  Performance notes:

    * ESP32: Driver uses background FreeRTOS task for impulse generation, as it requires microsecond precision
      for inpulses. ESP32 uses FreeRTOS API and movement is fast and smooth (very similar to real car gauges).
    * ESP8266: For ESP8266 driver also works, but movement is much slower (but it is still okay for slow
      changing values, i.e. temperature). On ESP8266 speed is ~2 sec per degree.

  Version history:

    * 2025-11-23 - fixes related with ESP8266 performance
    * 2025-11-22 - initial release
*/

/**
 * @brief Driver impulse mode decision here
 * ESP-32 has FreeRTOS, that allows us to perform precision inpulse control
 */
#ifdef ESP32
  #define VID6608_RTOS
#endif

#include "vid6608.h"

/**
 * @brief Command definition
 */
#define D_PRFX_GAUGE "Gauge"
#define D_CMND_GAUGE_SET "Set"
#define D_CMND_GAUGE_PERCENT "Percent"
#define D_CMND_GAUGE_ZERO "Zero"

const char kGaugeCommands[] PROGMEM = D_PRFX_GAUGE "|"  // Prefix
  "|" D_CMND_GAUGE_SET "|" D_CMND_GAUGE_PERCENT "|"
  D_CMND_GAUGE_ZERO
  ;

void (* const GaugeCommand[])(void) PROGMEM = {
  &CmndGauge, &CmndGaugeSet, &CmndGaugePercent,
  &CmndGaugeZero,
};

enum GaugeInternalCommand {
  GAUGE_ZERO,
  GAUGE_SET,
  GAUGE_SET_PERCENT
};

/**
 * @brief Global vars
 */
bool vid6608Present = false;
float vid6608StepsFloat = VID6608_DEFAULT_MAX_STEPS;
vid6608 *vid6608Drives[VID6608_MAX_DRIVES];
#ifdef VID6608_RTOS
  /**
   * @brief Mutex for RTOS precision timing
   *
   * We have to use "real" trheads under FreeRTOS, as precision timing is required
   * for stepper motor driving. This mutex protects data access from multiple
   * threads simultaniously. Else it will lead to stepper motor glitches and random move.
   *
   */
  SemaphoreHandle_t vid6608Mutex;
  // Macro for mutexs take/give
  #define VID6608_MUTEX_TAKE   xSemaphoreTake(vid6608Mutex, portMAX_DELAY);
  #define VID6608_MUTEX_GIVE   xSemaphoreGive(vid6608Mutex);
#else
  #define VID6608_MUTEX_TAKE
  #define VID6608_MUTEX_GIVE
#endif

/**
 * @brief Command Gauge
 * Displays currnt state for all Gauge drives
 */
void CmndGauge(void) {
  Response_P(PSTR("{"));
  VID6608StatusJson();
  ResponseJsonEnd();
}

/**
 * @brief Command GaugeSet, GaugeSet0 and GaugeSetX
 */
void CmndGaugeSet(void) {
  CmndGaugeCommand(GAUGE_SET, XdrvMailbox.index, XdrvMailbox.payload);
}

/**
 * @brief Command GaugePercent, GaugePercent0 and GaugePercentX
 */
void CmndGaugePercent(void) {
  CmndGaugeCommand(GAUGE_SET_PERCENT, XdrvMailbox.index, XdrvMailbox.payload);
}

/**
 * @brief Command GaugeZero, GaugeZero0 and GaugeZeroX
 */
void CmndGaugeZero(void) {
  CmndGaugeCommand(GAUGE_ZERO, XdrvMailbox.index, XdrvMailbox.payload);
}

/**
 * @brief Driver common command function
 *
 * @param command what to issue
 * @param index drive number (0 - all, 1..4 - by number)
 * @param payload command argument
 */
void CmndGaugeCommand(int32_t command, uint32_t index, int32_t payload) {
  VID6608_MUTEX_TAKE
  Response_P(PSTR("{\"" D_PRFX_GAUGE "\":{"));
  bool isFirstItem = true;
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    if (index == 0 || index == (x+1)) {
      vid6608 *driver = vid6608Drives[x];
      if (driver) {
        if (!isFirstItem) {
          ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("\"%d\":{"), (int32_t)(x+1));
        switch (command) {
          case GAUGE_ZERO:
            driver->zero();
            ResponseAppend_P(PSTR("\"cmd\":\"zero\",\"pos\":0"));
            break;
          case GAUGE_SET:
            driver->moveTo(payload);
            ResponseAppend_P(PSTR("\"cmd\":\"set\",\"pos\":%d"), payload);
            break;
          case GAUGE_SET_PERCENT:
            float moveSteps = vid6608StepsFloat * ( (float)payload / 100.0 );
            driver->moveTo(moveSteps);
            ResponseAppend_P(PSTR("\"cmd\":\"perc\",\"perc\":%d,\"pos\":%d"), payload, (int32_t)moveSteps);
            break;
        }
        ResponseAppend_P(PSTR("}"), x+1);
        isFirstItem = false;
      }
    }
  }
  ResponseAppend_P(PSTR("}}"));
  VID6608_MUTEX_GIVE
}

/**
 * @brief Function to display current Gauge state in JSON format
 * Prefix (i.e. "," or "{") should be added externally
 */
void VID6608StatusJson() {
  ResponseAppend_P(PSTR("\"" D_PRFX_GAUGE "\":{"));
  bool isFirstItem = true;
  VID6608_MUTEX_TAKE
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    vid6608 *driver = vid6608Drives[x];
    if (driver) {
      if (!isFirstItem) {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("\"%d\":{\"pos\":%d}"), (int32_t)(x+1), (int32_t)driver->getPosition());
      isFirstItem = false;
    }
  }
  VID6608_MUTEX_GIVE
  ResponseJsonEnd();
}

#ifdef USE_WEBSERVER
/**
 * @brief Function to display current Gauge state in HTML format
 * Displayes HTML table for web browser
 */
void VID6608StatusWeb() {
  WSContentSend_PD(HTTP_TABLE100);
  VID6608_MUTEX_TAKE
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    vid6608 *driver = vid6608Drives[x];
    if (driver) {
      WSContentSend_PD(PSTR("<tr><th>Gauge %d</th><td>%d</td></tr>"), (int32_t)(x+1), (int32_t)driver->getPosition());
    }
  }
  VID6608_MUTEX_GIVE
  WSContentSend_PD(PSTR("</table>"));
}
#endif

#ifdef VID6608_RTOS
/**
 * @brief FreeRTOS background process function
 * This function is required to handle movement with precision timings.
 * Used in ESP-32 only, the ESP8266 uses classical loop() thread.
 */
void VID6608XvTask(void *) {
  while(true) {
    bool needToMove = false;
    VID6608_MUTEX_TAKE
    for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
      vid6608 *driver = vid6608Drives[x];
      if (driver) {
        driver->loop();
        if (driver->isMoving()) {
          needToMove = true;
        }
      }
    }
    VID6608_MUTEX_GIVE
    /*
      If we dont need to move any -> go sleep.
      This will delay next move begin up to 500ms, but freeds up CPU a lot.
    */
    if (!needToMove) {
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}
#endif // VID6608_RTOS

/**
 * @brief Driver initalisation
 * Checks the defined pins and creates Drives objects, if pins found.
 */
void VID6608Init() {
  AddLog(LOG_LEVEL_INFO, PSTR("VID: Init"));
  for (uint32_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    if (PinUsed(GPIO_VID6608_F, x) && PinUsed(GPIO_VID6608_CW, x)) {
      // We have motor defined at number x
      uint32_t pinStep = Pin(GPIO_VID6608_F, x);
      uint32_t pinDir = Pin(GPIO_VID6608_CW, x);
      AddLog(LOG_LEVEL_DEBUG, PSTR("VID: detected drive %d at pin %d, %d"), x, pinStep, pinDir);
      vid6608Drives[x] = new vid6608(pinStep, pinDir);

      // Perform homing operation
      vid6608Drives[x]->zero();
      AddLog(LOG_LEVEL_DEBUG, PSTR("VID: zero %d done"), x);
      vid6608Present = true;
    } else {
      vid6608Drives[x] = nullptr;
    }
  }
  // If no drives present -> skip rest of initalization
  if (!vid6608Present) {
    return;
  }
#ifdef VID6608_RTOS
  // Create mutex for RTOS thread safety
  vid6608Mutex = xSemaphoreCreateMutex();
  // Start background RTOS thread -> required for precision timing
  xTaskCreate(
    VID6608XvTask,                /* Function to implement the task */
    "VID6608XvTask",              /* Name of the task */
    1024,                         /* Stack size in words */
    NULL,                         /* Task input parameter */
    0,                            /* Priority of the task, lowest */
    NULL                          /* Task handle. */
  );
#endif // VID6608_RTOS
}

// Classical loop implementation
#ifndef VID6608_RTOS
/**
 * @brief Non-FreeRTOS background process function
 * ESP8266 classical loop() thread function, used where is no FreeRTOS.
 */
bool VID6608Loop() {
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    vid6608 *driver = vid6608Drives[x];
    if (driver) {
      driver->loop();
    }
  }
  return true;
}
#endif // VID6608_RTOS

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv92(uint32_t function) {
  // Driver initalization
  if (FUNC_INIT == function) {
    VID6608Init();
    return false;
  }
  // We are not initilized?
  if (!vid6608Present) {
    return false;
  }
  // Normal callbacks
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
#ifndef VID6608_RTOS
      // ESP32 uses FreeRTOS to manage moving tasks, as it requires precision timings
      // Others should use regular loop -> slower, but still works
      result = VID6608Loop();
#else
      result = true;
#endif
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kGaugeCommands, GaugeCommand);
      break;
    case FUNC_JSON_APPEND:
      ResponseAppend_P(PSTR(","));
      VID6608StatusJson();
      result = true;
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      VID6608StatusWeb();
      result = true;
      break;
#endif // USE_WEBSERVER
    case FUNC_ACTIVE:
      result = true;
      break;
  }

  return result;
}

#endif  // USE_VID6608
