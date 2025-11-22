/*
  xdrv_91_esp32_twai.ino - ESP32 TWAI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_VID6608

#define XDRV_92             92
#define VID6608_MAX_DRIVES  4

// Decide about current implementation
#ifdef ESP32
  #define VID6608_RTOS
#endif

#include "vid6608.h"

#define D_PRFX_GAUGE "Gauge"
#define D_CMND_GAUGE_SET "Set"
#define D_CMND_GAUGE_PERCENT "Percent"

#define D_CMND_GAUGE_ZERO "Zero"

const char kGaugeCommands[] PROGMEM = D_PRFX_GAUGE "|"  // Prefix
//const char kGaugeCommands[] PROGMEM = "|"  // No Prefix
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

bool vid6608Present = false;
float vid6608StepsFloat = VID6608_DEFAULT_MAX_STEPS;
vid6608 *vid6608Drives[VID6608_MAX_DRIVES];

void CmndGauge(void) {
  Response_P(PSTR("{\"" D_PRFX_GAUGE "\", \"Payload\": %d}"), XdrvMailbox.payload);
}

void CmndGaugeSet(void) {  
  CmndGaugeCommand(GAUGE_SET, XdrvMailbox.index, XdrvMailbox.payload);
}

void CmndGaugePercent(void) {
  CmndGaugeCommand(GAUGE_SET_PERCENT, XdrvMailbox.index, XdrvMailbox.payload);
}

void CmndGaugeZero(void) {  
  CmndGaugeCommand(GAUGE_ZERO, XdrvMailbox.index, XdrvMailbox.payload);
}

void CmndGaugeCommand(int32_t command, uint32_t index, int32_t payload) {
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    if (index == 0 || index == (x+1)) {
      vid6608 *driver = vid6608Drives[x];
      if (driver) {
        switch (command) {
          case GAUGE_ZERO:
            driver->zero();
            break;
          case GAUGE_SET:
            driver->moveTo(payload);
            break;
          case GAUGE_SET_PERCENT:
            float moveSteps = vid6608StepsFloat * ( (float)payload / 100.0 );
            driver->moveTo(moveSteps);
            break;
        }
      }
    }
  }
  Response_P(PSTR("{\"" D_PRFX_GAUGE "\", \"Command\": %d}"), XdrvMailbox.payload);
}

#ifdef VID6608_RTOS
void VID6608XvTask(void *) {
  while(true) {
    bool needToMove = false;
    for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
      vid6608 *driver = vid6608Drives[x];
      if (driver) {
        driver->loop();
        if (driver->isMoving()) {
          needToMove = true;
        }
      }
    }
    // If we dont need to move any -> go sleep
    // This will delay next move begin up to 500ms, but freeds up CPU a lot
    if (!needToMove) {
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}
#endif

void VID6608Init() {
  AddLog(LOG_LEVEL_INFO, PSTR("VID: Init"));
  for (uint32_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    if (PinUsed(GPIO_VID6608_F, x) && PinUsed(GPIO_VID6608_CW, x)) {
      // We have motor defined at number x
      uint32_t pinStep = Pin(GPIO_VID6608_F, x);
      uint32_t pinDir = Pin(GPIO_VID6608_CW, x);
      AddLog(LOG_LEVEL_DEBUG, PSTR("VID: detected drive at pin %d, %d"), pinStep, pinDir);
      vid6608Drives[x] = new vid6608(pinStep, pinDir);
      vid6608Present = true;
      // Perform homing operation
      vid6608Drives[x]->zero();
      // FIXME: test
      vid6608Drives[x]->moveTo(300 * 12);
    } else {
      vid6608Drives[x] = nullptr;
    }
  }
  // If no drives present -> skip rest of initalization
  if (!vid6608Present) {
    return;
  }
#ifdef VID6608_RTOS
  // Start background RTOS thread -> required for precision timing
  xTaskCreate(
    VID6608XvTask,                /* Function to implement the task */
    "VID6608XvTask",              /* Name of the task */
    1024,                         /* Stack size in words */
    NULL,                         /* Task input parameter */
    0,                            /* Priority of the task, lowest */
    NULL                          /* Task handle. */
  );                           
#endif
}

void VID6608SetPos(uint8_t index, uint16_t pos) {
  vid6608 *driver = vid6608Drives[index];
  if (driver) {
    driver->moveTo(pos);
  }
}

// Classical loop implementation
#ifndef VID6608_RTOS
bool VID6608Loop() {
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    vid6608 *driver = vid6608Drives[x];
    if (driver) {
      driver->loop();
    }
  }
  return true;
}
#endif

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
  // ESP uses FreeRTOS to manage moving tasks, as it requires precision timings
  // Others should use regular loop -> slower, but still works  
      result = VID6608Loop();  
#else
      result = true;
#endif
      break;
    case FUNC_COMMAND:
      AddLog(LOG_LEVEL_DEBUG, PSTR("VID: command called"));
      // AddLog(LOG_LEVEL_DEBUG, PSTR("VID: command called: %s"), XdrvMailbox.command);
      result = DecodeCommand(kGaugeCommands, GaugeCommand);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }

  return result;
}

#endif  // USE_VID6608
