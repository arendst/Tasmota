/*
  xsns_96_flowratemeter.ino - flowratemeter support for Tasmota
                              - up to two flowratemeter YF-DN50 and similary
                              - flow rate frequencies f = 1 Hz up to 5 kHz
                              - uses the FreqRes resolution

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

#ifdef USE_FLOWRATEMETER

#define XSNS_96                       96



#define FLOWRATEMETER_WEIGHT_AVG_SAMPLE   20  // number of samples for smooth weigted average
#define FLOWRATEMETER_MIN_FREQ             1  // Hz

#define D_JSON_FLOWRATEMETER_RATE         "Rate"
#define D_JSON_FLOWRATEMETER_VALUE        "Value"
#define D_JSON_FLOWRATEMETER_UNIT         "Unit"
#define D_JSON_FLOWRATEMETER_VALUE_AVG    "average"
#define D_JSON_FLOWRATEMETER_VALUE_RAW    "raw"



#ifdef USE_WEBSERVER
const char HTTP_SNS_FLOWRATEMETER[] PROGMEM =
   "{s}" D_FLOWRATEMETER_NAME "-%d{m}%*_f %s{e}"
   ;
#endif  // USE_WEBSERVER


int32_t flowratemeter_period[MAX_FLOWRATEMETER] = {0};
float   flowratemeter_period_avg[MAX_FLOWRATEMETER] = {0};
uint32_t flowratemeter_count[MAX_FLOWRATEMETER] = {0};
volatile uint32_t flowratemeter_last_irq[MAX_FLOWRATEMETER] = {0};

bool flowratemeter_valuesread = false;
bool flowratemeter_raw_value = false;



void IRAM_ATTR FlowRateMeterIR(uint16_t irq)
{
  uint32_t time = micros();
#if defined(ESP8266)
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
#endif
  if (irq < MAX_FLOWRATEMETER) {
    if ((time - flowratemeter_last_irq[irq]) < (1000000 / FLOWRATEMETER_MIN_FREQ)) {
      flowratemeter_period[irq] = time - flowratemeter_last_irq[irq];
    } else {
      flowratemeter_period[irq] = 0;
    }
    flowratemeter_valuesread = true;
    flowratemeter_last_irq[irq] = time;
  }
}
// GPIO_STATUS is always 0 (?), so can only determine the IR source using this way
void IRAM_ATTR FlowRateMeter1IR(void)
{
  FlowRateMeterIR(0);
}
void IRAM_ATTR FlowRateMeter2IR(void)
{
  FlowRateMeterIR(1);
}

void FlowRateMeterRead(void)
{
  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    if ((micros() - flowratemeter_last_irq[i]) >= (1000000 / FLOWRATEMETER_MIN_FREQ)) {
      flowratemeter_period[i] = 0;
      flowratemeter_period_avg[i] = 0;
    }

    // exponentially weighted average
    if (flowratemeter_count[i] <= FLOWRATEMETER_WEIGHT_AVG_SAMPLE) {
      flowratemeter_count[i]++;
    }
    flowratemeter_period_avg[i] -= flowratemeter_period_avg[i] / flowratemeter_count[i];
    flowratemeter_period_avg[i] += float(flowratemeter_period[i]) / flowratemeter_count[i];
  }
}

void FlowRateMeterInit(void)
{
  void (* irq_service[MAX_FLOWRATEMETER])(void)= {FlowRateMeter1IR, FlowRateMeter2IR};

  flowratemeter_valuesread = false;
  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    pinMode(Pin(GPIO_FLOWRATEMETER_IN, i), INPUT);
    attachInterrupt(Pin(GPIO_FLOWRATEMETER_IN, i), irq_service[i], RISING);
  }
}

void FlowRateMeterShow(bool json)
{
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_FLOWRATEMETER_NAME "\":{\"" D_JSON_FLOWRATEMETER_RATE "\":["));
  }

  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    float flowratemeter_rate_avg_float = 0;

    if (flowratemeter_period[i]) {
      flowratemeter_rate_avg_float =
        ((Settings->SensorBits1.flowratemeter_unit ? (1000000.0 / 1000.0) : (1000000 / 60.0)) / 2.0)
        / (flowratemeter_raw_value ? flowratemeter_period[i] : flowratemeter_period_avg[i])
        * (Settings->flowratemeter_calibration[i] ? (float)Settings->flowratemeter_calibration[i] : 1000.0);
    }

    if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
      if (json) {
        ResponseAppend_P(PSTR("%s%*_f"),
          i ? PSTR(",") : PSTR(""),
          Settings->flag2.frequency_resolution, &flowratemeter_rate_avg_float
        );

#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_FLOWRATEMETER,
          i+1,
          Settings->flag2.frequency_resolution, &flowratemeter_rate_avg_float,
          Settings->SensorBits1.flowratemeter_unit ? D_UNIT_CUBICMETER_PER_HOUR : D_UNIT_LITER_PER_MINUTE
        );
#endif  // USE_WEBSERVER
      }
    }
  }
  if (json) {
    ResponseAppend_P(PSTR("],\"" D_JSON_FLOWRATEMETER_VALUE "\":\"%s\""),
        flowratemeter_raw_value ? PSTR(D_JSON_FLOWRATEMETER_VALUE_RAW) : PSTR(D_JSON_FLOWRATEMETER_VALUE_AVG)
    );
    ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\"}"),
      Settings->SensorBits1.flowratemeter_unit ? PSTR(D_UNIT_CUBICMETER_PER_HOUR) : PSTR(D_UNIT_LITER_PER_MINUTE)
    );
  }
}

/*********************************************************************************************\
 * Supported commands for Sensor96:
 *
 * Sensor96                        - Show current settings
 * Sensor96 0 0|1                  - Show flow value in l/min (0) or m³/h (1)
 * Sensor96 1 <correction-factor>  - Set sensor 1 factor (x 1000) - to set to 0.2 enter 'Sensor96 1 200'
 * Sensor96 2 <correction-factor>  - Set sensor 2 factor (x 1000)
 * Sensor96 9 0|1                  - Value mode: Switch between displaying avg(0) / raw(1) readings (not permanently)
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
bool FlowRateMeterCommand(void) {
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
    case 0:  // Unit
      if (any_value) {
        Settings->SensorBits1.flowratemeter_unit = value & 1;
        ResponseCmndNumber(value & 1);
        show_parms = false;
      }
      break;
    case 1:  // Sensor calibration value
    case 2:
      if (any_value) {
        Settings->flowratemeter_calibration[XdrvMailbox.payload - 1] = value;
        ResponseCmndNumber(value);
        show_parms = false;
      }
      break;
    case 9:  // avg/raw values
      if (any_value) {
        flowratemeter_raw_value = value & 1;
        ResponseCmndNumber(value & 1);
        show_parms = false;
      }
      break;
  }

  if (show_parms) {
      Response_P(PSTR("{\"Sensor%d\":{\"" D_JSON_POWERFACTOR "\":["), XSNS_96);
      for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        float flowratemeter_factor = Settings->flowratemeter_calibration[i] ? (float)Settings->flowratemeter_calibration[i] / 1000 : 1;
        ResponseAppend_P(PSTR("%s%3_f"), i ? PSTR(",") : PSTR(""), &flowratemeter_factor);
      }
      ResponseAppend_P(PSTR("],\"" D_JSON_FLOWRATEMETER_VALUE "\":\"%s\""),
          flowratemeter_raw_value ? PSTR(D_JSON_FLOWRATEMETER_VALUE_RAW) : PSTR(D_JSON_FLOWRATEMETER_VALUE_AVG)
          );
      ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\"}}"),
          Settings->SensorBits1.flowratemeter_unit ? PSTR(D_UNIT_CUBICMETER_PER_HOUR) : PSTR(D_UNIT_LITER_PER_MINUTE)
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

  if (PinUsed(GPIO_FLOWRATEMETER_IN, GPIO_ANY)) {
    switch (function) {
      case FUNC_INIT:
        FlowRateMeterInit();
        break;
      case FUNC_EVERY_250_MSECOND:
        FlowRateMeterRead();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_96 == XdrvMailbox.index) {
          result = FlowRateMeterCommand();
        }
        break;
      case FUNC_JSON_APPEND:
        FlowRateMeterShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        FlowRateMeterShow(false);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_FLOWRATEMETER
