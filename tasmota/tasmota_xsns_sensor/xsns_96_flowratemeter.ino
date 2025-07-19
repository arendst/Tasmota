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

#define D_JSON_FLOWRATEMETER_RATE           "Rate"
#define D_JSON_FLOWRATEMETER_VALUE          "Source"
#define D_JSON_FLOWRATEMETER_UNIT           "Unit"
#define D_JSON_FLOWRATEMETER_AMOUNT_TODAY   "AmountToday"
#define D_JSON_FLOWRATEMETER_AMOUNT_UNIT    "AmountUnit"
#define D_JSON_FLOWRATEMETER_DURATION_TODAY "DurationToday"
#define D_JSON_FLOWRATEMETER_VALUE_AVG      "average"
#define D_JSON_FLOWRATEMETER_VALUE_RAW      "raw"


#define FLOWRATEMETER_INVALID (uint32_t)-1

int32_t flowratemeter_period[MAX_FLOWRATEMETER] = {0};
float   flowratemeter_period_avg[MAX_FLOWRATEMETER] = {0};
uint32_t flowratemeter_count[MAX_FLOWRATEMETER] = {0};
volatile uint32_t flowratemeter_last_irq[MAX_FLOWRATEMETER] = {FLOWRATEMETER_INVALID};

int32_t flowratemeter_period_sum[MAX_FLOWRATEMETER];
int32_t flowratemeter_period_sum_dT[MAX_FLOWRATEMETER];
int32_t flowratemeter_period_duration[MAX_FLOWRATEMETER];


bool flowratemeter_raw_value = false;


#define FlowRateMeterIsValid(time, meter) flowratemeter_last_irq[meter] != FLOWRATEMETER_INVALID && flowratemeter_last_irq[meter] < time

void IRAM_ATTR FlowRateMeterIR(uint16_t irq)
{
  uint32_t time = micros();
#if defined(ESP8266)
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
#endif
  if (irq < MAX_FLOWRATEMETER) {
    if (FlowRateMeterIsValid(time, irq)) {
      if ((time - flowratemeter_last_irq[irq]) < (1000000 / FLOWRATEMETER_MIN_FREQ)) {
        flowratemeter_period_sum_dT[irq] = millis();
        flowratemeter_period_sum[irq]++;
        flowratemeter_period[irq] = time - flowratemeter_last_irq[irq];
        flowratemeter_period_duration[irq] += flowratemeter_period[irq] / 100;
      } else {
        flowratemeter_period[irq] = 0;
      }
    }
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

void FlowRateMeterMidnightReset(void)
{
  uint32_t t = millis();
  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    flowratemeter_period_sum[i] = 0;
    flowratemeter_period_duration[i] = 0;
    flowratemeter_period_sum_dT[i] = t;
  }
}

void FlowRateMeterRead(void)
{
  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    uint32_t time = micros();
    if (PinUsed(GPIO_FLOWRATEMETER_IN, i) && FlowRateMeterIsValid(time, i)) {
      if ((time - flowratemeter_last_irq[i]) >= (1000000 / FLOWRATEMETER_MIN_FREQ)) {
        // invalid in case of pulse outage
        flowratemeter_period[i] = 0;
        flowratemeter_period_avg[i] = 0;
        flowratemeter_count[i] = 0;
        flowratemeter_last_irq[i] = FLOWRATEMETER_INVALID;
      }

      // exponentially weighted average
      if (flowratemeter_count[i] <= FLOWRATEMETER_WEIGHT_AVG_SAMPLE) {
        flowratemeter_count[i]++;
      }
      flowratemeter_period_avg[i] -= flowratemeter_period_avg[i] / flowratemeter_count[i];
      flowratemeter_period_avg[i] += float(flowratemeter_period[i]) / flowratemeter_count[i];
    }
  }
}

void FlowRateMeterInit(void)
{
  void (* irq_service[MAX_FLOWRATEMETER])(void)= {FlowRateMeter1IR, FlowRateMeter2IR};

  FlowRateMeterMidnightReset();
  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
      pinMode(Pin(GPIO_FLOWRATEMETER_IN, i), INPUT);
      attachInterrupt(Pin(GPIO_FLOWRATEMETER_IN, i), irq_service[i], RISING);
    }
  }
}

void FlowRateMeterGetValue(uint32_t meter, float *rate_float, float *amount_today)
{
  if (nullptr != rate_float) {
    *rate_float = 0;
    if (meter < MAX_FLOWRATEMETER && flowratemeter_period[meter]) {
      *rate_float =
        (1000000.0 / 60.0 / 2.0)
        / (flowratemeter_raw_value ? flowratemeter_period[meter] : flowratemeter_period_avg[meter])
        * (Settings->flowratemeter_calibration[meter] ? (float)Settings->flowratemeter_calibration[meter] : 1000.0);
    }
  }

  if (nullptr != amount_today) {
    *amount_today = 0;
    if (meter < MAX_FLOWRATEMETER && flowratemeter_period_sum[meter]) {
      uint32_t _flowratemeter_period = (uint32_t)((float)flowratemeter_period_sum_dT[meter] / (float)flowratemeter_period_sum[meter] * 1000.0);
      float lmin = (((1000000.0 / 60.0) / 2.0) / _flowratemeter_period * (Settings->flowratemeter_calibration[meter] ? (float)Settings->flowratemeter_calibration[meter] : 1000.0));
      *amount_today = (lmin / 60000) * flowratemeter_period_sum_dT[meter];
    }
  }
}

void FlowRateMeterShow(bool json)
{
  uint16_t flowmeter_count = 0;
  const char* open_square_bracket;
  const char* close_square_bracket;
  float flowratemeter_rate_float[MAX_FLOWRATEMETER];
  float floatrate_amount_today[MAX_FLOWRATEMETER];

  for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
    FlowRateMeterGetValue(i, &flowratemeter_rate_float[i], &floatrate_amount_today[i]);
    if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
      flowmeter_count++;
    }
  }
  if (flowmeter_count > 1) {
    open_square_bracket = PSTR("[");
    close_square_bracket = PSTR("]");
  } else {
    open_square_bracket = PSTR("");
    close_square_bracket = PSTR("");
  }

  if (json) {
    ResponseAppend_P(PSTR(",\"" D_FLOWRATEMETER_NAME "\":{\"" D_JSON_FLOWRATEMETER_RATE "\":%s"), open_square_bracket);
    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
      if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
        float rate = Settings->SensorBits1.flowratemeter_unit ? flowratemeter_rate_float[i] * 60 / 1000 : flowratemeter_rate_float[i];
        ResponseAppend_P(PSTR("%s%*_f"), i ? PSTR(",") : PSTR(""), Settings->flag2.frequency_resolution, &rate);
      }
    }
    ResponseAppend_P(PSTR("%s,\"" D_JSON_FLOWRATEMETER_AMOUNT_TODAY "\":%s"), close_square_bracket, open_square_bracket);
    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
      if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
        float amount_today = Settings->SensorBits1.flowratemeter_unit ? floatrate_amount_today[i] / 1000 : floatrate_amount_today[i];
        ResponseAppend_P(PSTR("%s%*_f"), i ? PSTR(",") : PSTR(""), Settings->flag2.frequency_resolution, &amount_today);
      }
    }
    ResponseAppend_P(PSTR("%s,\"" D_JSON_FLOWRATEMETER_DURATION_TODAY "\":%s"), close_square_bracket, open_square_bracket);
    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
      if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
        ResponseAppend_P(PSTR("%s%ld"), i ? PSTR(",") : PSTR(""), flowratemeter_period_duration[i] / 10000);
      }
    }
    ResponseAppend_P(PSTR("%s,\"" D_JSON_FLOWRATEMETER_VALUE "\":\"%s\""),
      close_square_bracket,
      flowratemeter_raw_value ? PSTR(D_JSON_FLOWRATEMETER_VALUE_RAW) : PSTR(D_JSON_FLOWRATEMETER_VALUE_AVG)
    );
    ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\""),
      Settings->SensorBits1.flowratemeter_unit ? PSTR(D_UNIT_CUBIC_METER) : PSTR(D_UNIT_LITERS));
    ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\"}"),
      Settings->SensorBits1.flowratemeter_unit ? PSTR(D_UNIT_CUBICMETER_PER_HOUR) : PSTR(D_UNIT_LITER_PER_MINUTE));
#ifdef USE_WEBSERVER
    } else {
      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
      if (flowmeter_count > 1) {
        // head
        WSContentSend_PD(PSTR("{s}&nbsp;</th>&nbsp;<th></th>"));
        for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
          if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
            WSContentSend_PD(PSTR("<th style=\"text-align:%s\">%d</th><th>&nbsp;</th>"),
              Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
              i+1
            );
          }
        }
        WSContentSend_PD(PSTR("<th> </th></tr>"));
      }

      // Flowrate
      WSContentSend_PD(PSTR("{s}" D_FLOWRATEMETER_NAME "{m}&nbsp;</td>"));
      for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
          float rate = Settings->SensorBits1.flowratemeter_unit ? flowratemeter_rate_float[i] * 60 / 1000 : flowratemeter_rate_float[i];
          WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%*_f</td><td>&nbsp;</td>"),
            Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
            Settings->flag2.frequency_resolution, &rate
          );
        }
      }
      WSContentSend_PD(PSTR("<td>%s{e}"), Settings->SensorBits1.flowratemeter_unit ? PSTR(D_UNIT_CUBICMETER_PER_HOUR) : PSTR(D_UNIT_LITER_PER_MINUTE));

      // Amount today
      WSContentSend_PD(PSTR("{s}" D_FLOWRATEMETER_NAME " " D_FLOWRATEMETER_AMOUNT_TODAY "{m}&nbsp;</td>"));
      for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
          float amount_today = Settings->SensorBits1.flowratemeter_unit ? floatrate_amount_today[i] / 1000 : floatrate_amount_today[i];
          WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%*_f</td><td>&nbsp;</td>"),
            Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
            Settings->flag2.frequency_resolution, &amount_today
          );
        }
      }
      WSContentSend_PD(PSTR("<td>%s{e}"), Settings->SensorBits1.flowratemeter_unit ? PSTR(D_UNIT_CUBIC_METER) : PSTR(D_UNIT_LITERS));

      // Duration today
      WSContentSend_PD(PSTR("{s}" D_FLOWRATEMETER_NAME " " D_FLOWRATEMETER_DURATION_TODAY "{m}&nbsp;</td>"));
      for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
          float amount_today = Settings->SensorBits1.flowratemeter_unit ? floatrate_amount_today[i] / 1000 : floatrate_amount_today[i];
          WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%s</td><td>&nbsp;</td>"),
            (Settings->flag5.gui_table_align)?PSTR("right"):PSTR("center"),
            GetDuration(flowratemeter_period_duration[i] / 10000).c_str()
          );
        }
      }
      WSContentSend_PD(PSTR("<td>{e}"));
#endif  // USE_WEBSERVER
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

bool Xsns96(uint32_t function)
{
  bool result = false;

  if (PinUsed(GPIO_FLOWRATEMETER_IN, GPIO_ANY)) {
    switch (function) {
      case FUNC_INIT:
        FlowRateMeterInit();
        break;
      case FUNC_SAVE_AT_MIDNIGHT:
        FlowRateMeterMidnightReset();
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
