/*
  xsns_96_flowmeter.ino - flowmeter support for Tasmota
                          Up to two flowmeter YF-DN50 and similary supported
                          (f = 1 Hz up to 5 kHz)

  Copyright (C) 2022  Norbert Richter

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

#ifdef USE_FLOWMETER

#define XSNS_96                       96

#define FLOWMETER_WEIGHT_AVG_SAMPLE   20  // samples
#define FLOWMETER_MIN_FREQ             1  // Hz

// The Arduino standard GPIO routines are not enough,
// must use some from the Espressif SDK as well
extern "C" {
#include "gpio.h"
}


#ifdef USE_WEBSERVER
const char HTTP_SNS_FLOWMETER[] PROGMEM =
   "{s}" D_FLOWMETER_NAME "-%d " D_FLOWMETER_RATE            "{m}%*_f %s{e}"
   ;
#endif  // USE_WEBSERVER


int32_t flowmeter_period[MAX_FLOWMETER] = {0};
float   flowmeter_period_avg[MAX_FLOWMETER] = {0};
uint32_t flowmeter_count[MAX_FLOWMETER] = {0};
volatile uint32_t flowmeter_last_irq[MAX_FLOWMETER] = {0};

bool flowmeter_valuesread = false;


void IRAM_ATTR FlowMeterIR(uint16_t irq)
{
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
  uint32_t time = micros();
  uint32_t i = irq;
  if (irq < MAX_FLOWMETER) {
    if ((time - flowmeter_last_irq[i]) < (1000000 / FLOWMETER_MIN_FREQ)) {
      flowmeter_period[i] = time - flowmeter_last_irq[i];
    } else {
      flowmeter_period[i] = 0;
    }
    flowmeter_valuesread = true;
    flowmeter_last_irq[i] = time;
  }
}
// GPIO_STATUS is always 0 (?), so can only determine the IR source using this way:
void IRAM_ATTR FlowMeter1IR(void)
{
  FlowMeterIR(0);
}
void IRAM_ATTR FlowMeter2IR(void)
{
  FlowMeterIR(1);
}

void FlowMeterRead(void)
{
  for (uint32_t i = 0; i < MAX_FLOWMETER; i++) {
    if ((micros() - flowmeter_last_irq[i]) >= (1000000 / FLOWMETER_MIN_FREQ)) {
      flowmeter_period[i] = 0;
      flowmeter_period_avg[i] = 0;
    }

    // exponentially weighted average
    if (flowmeter_count[i] <= FLOWMETER_WEIGHT_AVG_SAMPLE) {
      flowmeter_count[i]++;
    }
    flowmeter_period_avg[i] -= flowmeter_period_avg[i] / flowmeter_count[i];
    flowmeter_period_avg[i] += float(flowmeter_period[i]) / flowmeter_count[i];
  }
}

void FlowMeterInit(void)
{
  void (* irq_service[MAX_FLOWMETER])(void)= {FlowMeter1IR, FlowMeter2IR};

  flowmeter_valuesread = false;
  for (uint32_t i = 0; i < MAX_FLOWMETER; i++) {
    pinMode(Pin(GPIO_FLOWMETER_IN, i), INPUT);
    attachInterrupt(Pin(GPIO_FLOWMETER_IN, i), irq_service[i], RISING);
  }
}

void FlowMeterShow(bool json)
{
  for (uint32_t i = 0; i < MAX_FLOWMETER; i++) {
    float flowmeter_rate_avg_float = 0;

    if (flowmeter_period[i]) {
      flowmeter_rate_avg_float =
        ((Settings->SensorBits1.flowmeter_unit ? (1000000.0 / 1000.0) : (1000000 / 60.0)) / 2.0) / flowmeter_period_avg[i] * (Settings->flowmeter_calibration[i] ? (float)Settings->flowmeter_calibration[i] : 1000.0);
    }

    if (PinUsed(GPIO_FLOWMETER_IN, i)) {
      if (json) {
        ResponseAppend_P(PSTR(",\"" D_FLOWMETER_NAME "-%d\":{\"" D_JSON_FLOWRATE "\":%*_f}"),
          i+1,
          Settings->flag2.frequency_resolution, &flowmeter_rate_avg_float
        );
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_FLOWMETER,
          i+1,
          Settings->flag2.frequency_resolution, &flowmeter_rate_avg_float,
          Settings->SensorBits1.flowmeter_unit ? D_UNIT_CUBICMETER_PER_HOUR : D_UNIT_LITER_PER_MINUTE
        );
#endif  // USE_WEBSERVER
      }
    }
  }
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_FLOW_UNIT "\":\"%s\""),
      Settings->SensorBits1.flowmeter_unit ? D_UNIT_CUBICMETER_PER_HOUR : D_UNIT_LITER_PER_MINUTE
    );
  }
}



/*********************************************************************************************\
 * Supported commands for Sensor96:
 *
 * Sensor96                        - Show current settings
 * Sensor96 1 <correction-factor>  - Set sensor 1 factor (x 1000) - to set to 0.2 enter 'Sensor96 1 200'
 * Sensor96 2 <correction-factor>  - Set sensor 2 factor (x 1000)
 * Sensor96 9 0|1                  - Show flow value in l/min (0) or m³/h (1)
 *
 * Flowmeter calibration:
 * - get the current displayed flow rate (D)
 * - get the current <correction-factor> (c)
 * - measure the real flow rate          (M)
 * - new <correction-factor> = M / (c * D)
 *
 * Example:
 * - displayed flow rate = 254.39 l/min (D)
 * - current <correction-factor> = 1.0  (c)
 * - real flow rate      =  83.42 l/min (M)
 *
 * new <correction-factor> = M / (c * D) = 83.42 / (1 * 254.39) = 0.328
 * Cmd: Sensor96 x 328
\*********************************************************************************************/

bool FlowMeterCommand(void) {
  bool show_parms = true;
  char argument[XdrvMailbox.data_len];

  long value = 0;
  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }
  bool any_value = (strchr(XdrvMailbox.data, ',') != nullptr);
  if (any_value) {
    value = strtol(ArgV(argument, 2), nullptr, 10);
  }

  switch (XdrvMailbox.payload) {
    case 1:  // Calibration value
    case 2:  // Calibration value
      if (any_value) {
        Settings->flowmeter_calibration[XdrvMailbox.payload - 1] = value;
        ResponseCmndNumber(value);
        show_parms = false;
      }
      break;
    case 9:  // Unit
      if (any_value) {
        Settings->SensorBits1.flowmeter_unit = value & 1;
        ResponseCmndNumber(value & 1);
        show_parms = false;
      }
      break;
  }

  if (show_parms) {
      Response_P(PSTR("{\"Sensor%d\":{"), XSNS_96);
      for (uint32_t i = 0; i < MAX_FLOWMETER; i++) {
        float flowmeter_factor = Settings->flowmeter_calibration[i] ? (float)Settings->flowmeter_calibration[i] / 1000 : 1.0;
        ResponseAppend_P(PSTR("\"" D_JSON_POWERFACTOR "-%d\":%3_f,"), i+1, &flowmeter_factor);
      }
      ResponseAppend_P(PSTR("\"" D_JSON_FLOW_UNIT "\":\"%s\"}}"),
          Settings->SensorBits1.flowmeter_unit ? D_UNIT_CUBICMETER_PER_HOUR : D_UNIT_LITER_PER_MINUTE
          );
  }

  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns96(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_FLOWMETER_IN, GPIO_ANY)) {
    switch (function) {
      case FUNC_INIT:
        FlowMeterInit();
        break;
      case FUNC_EVERY_250_MSECOND:
        FlowMeterRead();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_96 == XdrvMailbox.index) {
          result = FlowMeterCommand();
        }
        break;
      case FUNC_JSON_APPEND:
        FlowMeterShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        FlowMeterShow(false);
        break;
#endif  // USE_WEBSERVER

    }
  }
  return result;
}

#endif  // USE_FLOWMETER
