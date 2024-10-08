/*
  xsns_34_hx711.ino - HX711 load cell support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef USE_HX711
/*********************************************************************************************\
 * HX711 - Load cell as used in a scale
 *
 * Source: Sparkfun and https://github.com/bogde/HX711
 *
 * To reset the scale:
 * - Execute command Sensor34 1
 *
 * To calibrate the scale perform the following tasks:
 * - (Optional) Set reference weight once using command Sensor34 3 <reference weight in gram>
 * - Remove any weight from the scale
 * - Execute command Sensor34 2 <reference weight in gram> and follow messages shown
 * -------------------------------------------------------------------------------------------
 * I2C M5Unit (Mini)Scales(HX711 STM32) supported
 * 
 * I2C Address: 0x26
\*********************************************************************************************/

#define XSNS_34              34
#define XI2C_89              89      // See I2CDEVICES.md

#ifndef HX_MAX_WEIGHT
#define HX_MAX_WEIGHT        20000   // Default max weight in gram
#endif
#ifndef HX_REFERENCE
#define HX_REFERENCE         250     // Default reference weight for calibration in gram
#endif
#ifndef HX_SCALE
#define HX_SCALE             120     // Default result of measured weight / reference weight when scale is 1
#endif
#ifndef HX711_CAL_PRECISION
#define HX711_CAL_PRECISION  1       // When calibration is to course, raise this value to max 20 (otherwise uint32_t overflow)
#endif

#define HX_TIMEOUT           120     // A reading at default 10Hz (pin RATE to Gnd on HX711) can take up to 100 milliseconds
#define HX_SAMPLES           10      // Number of samples for average calculation
#define HX_CAL_TIMEOUT       15      // Calibration step window in number of seconds

#define HX_GAIN_128          1       // Channel A, gain factor 128
#define HX_GAIN_32           2       // Channel B, gain factor 32
#define HX_GAIN_64           3       // Channel A, gain factor 64

#define D_JSON_WEIGHT_RAW    "WeightRaw"
#define D_JSON_WEIGHT_RAW_ABS "AbsRaw"
#define D_JSON_WEIGHT_REF    "Ref"
#define D_JSON_WEIGHT_CAL    "Cal"
#define D_JSON_WEIGHT_PREC   "Prec"
#define D_JSON_WEIGHT_MAX    "Max"
#define D_JSON_WEIGHT_ITEM   "Item"
#define D_JSON_WEIGHT_CHANGE "Change"
#define D_JSON_WEIGHT_DELTA  "Delta"
#define D_JSON_WEIGHT_TARE   "Tare"
#define D_JSON_WEIGHT_ABSC_A "AbsConvA"
#define D_JSON_WEIGHT_ABSC_B "AbsConvB"

enum HxCalibrationSteps { HX_CAL_END, HX_CAL_LIMBO, HX_CAL_FINISH, HX_CAL_FAIL, HX_CAL_DONE, HX_CAL_FIRST, HX_CAL_RESET, HX_CAL_START };
enum HxCalibrationMsgs  { HX_MSG_CAL_FAIL, HX_MSG_CAL_DONE, HX_MSG_CAL_REFERENCE, HX_MSG_CAL_REMOVE };
const char kHxCalibrationStates[] PROGMEM = D_HX_CAL_FAIL "|" D_HX_CAL_DONE "|" D_HX_CAL_REFERENCE "|" D_HX_CAL_REMOVE;

typedef struct Hx_t {
  long reads[HX_SAMPLES];
  long raw_empty;
  long raw_absolute;
  long raw;
  long weight;
  long last_weight;
  long offset;
  long scale;
  long weight_diff;
  uint16_t weight_delta;
  uint8_t sample_count;
  uint8_t calibrate_step;
  uint8_t calibrate_timer;
  uint8_t calibrate_msg;
  int8_t pin_sck;
  int8_t pin_dout;
  bool tare_flg;
  bool weight_changed;
} Hx_t;
Hx_t* Hx = nullptr;

/*********************************************************************************************/

#if defined(USE_I2C) && defined(USE_HX711_M5SCALES)
// M5Unit scales (STM32) need at least 800mS delay before ready after power on
//  hence HxInit() is called 2 seconds after power on using FUNC_EVERY_SECOND

#ifndef HX_SCALES_ADDR
#define HX_SCALES_ADDR       0x26                    // M5Unit (Mini)Scales(HX711 STM32) default I2C address
#endif
#ifndef HX_SCALES_RAW_ADC
#define HX_SCALES_RAW_ADC    0x00                    // M5Unit MiniScales raw ADC register (U177)
//#define HX_SCALES_RAW_ADC    0x10                    // M5Unit Scales raw ADC register (U108)
#endif
#define HX_SCALES_I2C_ADDR   0xFF                    // M5Unit (Mini)Scales get I2C address register

bool HxM5Found(void) {
  uint8_t data = 0;
  if (!I2cReadBuffer(HX_SCALES_ADDR, HX_SCALES_I2C_ADDR, &data, 1, Hx->pin_dout)) {
    return (HX_SCALES_ADDR == data);                 // Verify I2C address with stored register
  }
  return false;
}

long HxM5ReadRaw(void) {
  long rawADC = -1;
  uint8_t data[4] = { 0 };
  if (!I2cReadBuffer(HX_SCALES_ADDR, HX_SCALES_RAW_ADC, data, 4, Hx->pin_dout)) {
#if (HX_SCALES_RAW_ADC == 0x00)                      // M5Unit MiniScales (U177)
    long rawADC1 = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
#else                                                // M5Unit Scales (U108)
    long rawADC1 = data[0];
    rawADC1 = (rawADC1 << 8) | data[1];
    rawADC1 = (rawADC1 << 8) | data[2];
    rawADC1 = (rawADC1 << 8) | data[3];
#endif  // M5Unit Scales (U108)
    rawADC = 0x01FFFFFF - rawADC1;                   // Additional weight on miniscale decreases ADC value
  }
  return rawADC;
}
#endif  // USE_I2C && USE_HX711_M5SCALES

/*********************************************************************************************/

bool HxIsReady(uint16_t timeout) {

#if defined(USE_I2C) && defined(USE_HX711_M5SCALES)
  if (Hx->pin_dout == Hx->pin_sck) { 
    return HxM5Found();
  }
#endif  // USE_I2C && USE_HX711_M5SCALES

  // A reading can take up to 100 mS or 600mS after power on
  uint32_t start = millis();
  while ((digitalRead(Hx->pin_dout) == HIGH) && (millis() - start < timeout)) {
    yield();
  }
  return (digitalRead(Hx->pin_dout) == LOW);
}

long HxRead(void) {

#if defined(USE_I2C) && defined(USE_HX711_M5SCALES)
  if (Hx->pin_dout == Hx->pin_sck) { 
    return HxM5ReadRaw();
  }
#endif  // USE_I2C && USE_HX711_M5SCALES

  if (!HxIsReady(HX_TIMEOUT)) { return -1; }

  uint8_t data[3] = { 0 };
  uint8_t filler = 0x00;

  // pulse the clock pin 24 times to read the data
  data[2] = TasShiftIn(Hx->pin_dout, Hx->pin_sck, MSBFIRST);
  data[1] = TasShiftIn(Hx->pin_dout, Hx->pin_sck, MSBFIRST);
  data[0] = TasShiftIn(Hx->pin_dout, Hx->pin_sck, MSBFIRST);

  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < HX_GAIN_128; i++) {
    digitalWrite(Hx->pin_sck, HIGH);
#ifdef ESP32
    delayMicroseconds(1);
#endif
    digitalWrite(Hx->pin_sck, LOW);
#ifdef ESP32
    delayMicroseconds(1);
#endif
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) { filler = 0xFF; }

  // Construct a 32-bit signed integer
  unsigned long value = ( static_cast<unsigned long>(filler) << 24
                        | static_cast<unsigned long>(data[2]) << 16
                        | static_cast<unsigned long>(data[1]) << 8
                        | static_cast<unsigned long>(data[0]) );

  return static_cast<long>(value);
}

/*********************************************************************************************/

void HxTareInit(void) {
  Hx->offset = (Settings->weight_user_tare != 0) ? Settings->weight_user_tare * Hx->scale : Settings->weight_offset;
  if (0 == Hx->offset) {
    Hx->tare_flg = true;
  }
}

void HxCalibrationStateTextJson(uint8_t msg_id) {
  char cal_text[30];

  Hx->calibrate_msg = msg_id;
  Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_34, GetTextIndexed(cal_text, sizeof(cal_text), Hx->calibrate_msg, kHxCalibrationStates));

  if (msg_id < HX_MSG_CAL_REMOVE) { MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("Sensor34")); }
}

void SetWeightDelta(void) {
  if (Settings->weight_change == 0) {
    // backwards compatible: restore old default value of 4 grams
    Hx->weight_delta = 4;
  }
  else if (Settings->weight_change > 100) {
    // map upper values 101-255 to
    Hx->weight_delta = (Settings->weight_change - 100) * 10 + 100;
  }
  else {
    // map 1..100 to 0..99 grams
    Hx->weight_delta = Settings->weight_change - 1;
  }
}

/*********************************************************************************************\
 * Supported commands for Sensor34:
 *
 * Sensor34                        - Show current settings
 * Sensor34 1                      - Reset display to 0
 * Sensor34 2                      - Start calibration
 * Sensor34 2 <weight in gram>     - Set reference weight and start calibration
 * Sensor34 2 <weight in gram> <precision> - Set reference weight with larger precision (1..10) and start calibration
 * Sensor34 3 <weight in gram>     - Set reference weight
 * Sensor34 4 <scale value>        - Set calibrated scale value
 * Sensor34 5 <weight in gram>     - Set max weight
 * Sensor34 6 <weight in decigram> - Set item weight
 * Sensor34 7                      - Save current weight to be used as start weight on restart (removed v11.0.0.7)
 * Sensor34 8 0                    - Disable JSON weight change message
 * Sensor34 8 1                    - Enable JSON weight change message
 * Sensor34 9 <weight code>        - Set minimum delta to trigger JSON message
 * Sensor34 10 0                   - Disable fixed tare and enable auto tare
 * Sensor34 10 1                   - Set fixed tare offset using current calibrated raw weight value
 * Sensor34 10 <weight in gram>    - Set fixed tare offset
 * Sensor34 11 0                   - Disable absolute weight conversion
 * Sensor34 11 <value A>           - Set A = a * 10^9 for raw to absolute weight conversion: y=a*x+b
 * Sensor34 12 <value B>           - Set B = b * 10^6 for raw to absolute weight conversion: y=a*x+b
\*********************************************************************************************/

bool HxCommand(void) {
  bool serviced = true;
  bool show_parms = true;

  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }
  bool any_value = (ArgC() > 1);
  long value;
  char argument[32];
  if (any_value) { 
    value = strtol(ArgV(argument, 2), nullptr, 10);
  }

  switch (XdrvMailbox.payload) {
    case 1:  // Reset scale
      if (0 == Settings->weight_user_tare) {
        Hx->tare_flg = true;
        Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_34, "Reset");
      }
      show_parms = false;
      break;
    case 2:  // Calibrate in gram and precision 1 to 10
      if (any_value) { 
        Settings->weight_reference = (value && (value < (Settings->weight_max * 1000))) ? value : HX_REFERENCE;
        if (ArgC() > 2) {
          value = strtol(ArgV(argument, 3), nullptr, 10);
          Settings->weight_precision = (value && (value <= 20)) ? value : HX711_CAL_PRECISION;
        }
      }
      Hx->scale = 1;                                 // Uncalibrated
      Hx->sample_count = 0;
      Hx->offset = 0;                                // Disable tare while calibrating
      Hx->calibrate_step = HX_CAL_START;
      Hx->calibrate_timer = 1;
//      HxCalibrationStateTextJson(HX_MSG_CAL_REMOVE);
      HxCalibrationStateTextJson(HX_MSG_CAL_REFERENCE);
      show_parms = false;
      break;
    case 3:  // WeightRef to user reference in gram
      if (any_value) { 
        Settings->weight_reference = (value && (value < (Settings->weight_max * 1000))) ? value : HX_REFERENCE;
      }
      break;
    case 4:  // WeightCal to user calculated value
      if (any_value) {
        Settings->weight_calibration = value;        // Allow zero for re-init
        Hx->scale = (value) ? Settings->weight_calibration : 1;  // Fix divide by zero
      }
      break;
    case 5:  // WeightMax in kg
      if (any_value) { 
        Settings->weight_max = (value) ? value : HX_MAX_WEIGHT / 1000;
      }
      break;
    case 6:  // WeightItem in gram
      if (any_value) { 
        Settings->weight_item = (unsigned long)(CharToFloat(ArgV(argument, 2)) * 10);
      }
      break;
//    case 7:  // WeightSave
//      Settings->energy_frequency_calibration = Hx->weight;
//      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_34, PSTR(D_JSON_DONE));
//      show_parms = false;
//      break;
    case 8:  // Json on weight change
      if (any_value) { 
        Settings->SensorBits1.hx711_json_weight_change = value &1;
      }
      break;
    case 9:  // WeightDelta
      if (any_value) {
        Settings->weight_change = value;
        SetWeightDelta();
      }
      break;
    case 10:  // Fixed (user) tare
      if (any_value) {
        Settings->weight_user_tare = (1 == value) ? Hx->raw : value;
        HxTareInit();
        Hx->weight_diff = Hx->weight_delta +1;       // Force display of current weight
      }
      break;
    case 11:  // AbsoluteConversion, A
      if (any_value) { 
        Settings->weight_absconv_a = value;
      }
      break;
    case 12:  // AbsoluteConversion, B
      if (any_value) { 
        Settings->weight_absconv_b = value;
      }
      break;
  }

  if (show_parms) {
    char item[33];
    dtostrfd((float)Settings->weight_item / 10, 1, item);
    Response_P(PSTR("{\"Sensor34\":{\"" 
      D_JSON_WEIGHT_REF "\":%d,\"" D_JSON_WEIGHT_CAL "\":%d,\"" D_JSON_WEIGHT_PREC "\":%d,\"" 
      D_JSON_WEIGHT_MAX "\":%d,\""
      D_JSON_WEIGHT_ITEM "\":%s,\"" D_JSON_WEIGHT_CHANGE "\":\"%s\",\"" D_JSON_WEIGHT_DELTA "\":%d,\""
      D_JSON_WEIGHT_TARE "\":%d,\"" D_JSON_WEIGHT_ABSC_A "\":%d,\"" D_JSON_WEIGHT_ABSC_B "\":%d}}"),
      Settings->weight_reference, Settings->weight_calibration, Settings->weight_precision,
      Settings->weight_max * 1000,
	    item, GetStateText(Settings->SensorBits1.hx711_json_weight_change), Settings->weight_change,
      Settings->weight_user_tare, Settings->weight_absconv_a, Settings->weight_absconv_b);
  }

  return serviced;
}

/*********************************************************************************************/

long HxWeight(void) {
  return (Hx->calibrate_step < HX_CAL_FAIL) ? Hx->weight : 0;
}

void HxInit(void) {
  uint32_t hx711_config = (PinUsed(GPIO_HX711_DAT) && PinUsed(GPIO_HX711_SCK)) ? 1 : 0;

#if defined(USE_I2C) && defined(USE_HX711_M5SCALES)
  uint32_t bus;
  if (!hx711_config && I2cEnabled(XI2C_89)) { 
    for (bus = 0; bus < 2; bus++) {
      if (!I2cSetDevice(HX_SCALES_ADDR, bus)) { continue; }
      I2cSetActiveFound(HX_SCALES_ADDR, "M5Unit Scales", bus);
      hx711_config = 2;
      break;
    }
  }
#endif  // USE_I2C && USE_HX711_M5SCALES

  if (hx711_config > 0) {
    Hx = (Hx_t*)calloc(sizeof(Hx_t), 1);             // Need calloc to reset registers to 0/false
    if (nullptr == Hx) { return; }

//    Hx->calibrate_step = HX_CAL_END;               // HX_CAL_END = 0

#if defined(USE_I2C) && defined(USE_HX711_M5SCALES)
    if (2 == hx711_config) {
      Hx->pin_sck = bus;                             // If both are equal use M5Scale instead of HX711
      Hx->pin_dout = bus;
    } else {
#endif  // USE_I2C && USE_HX711_M5SCALES
      Hx->pin_sck = Pin(GPIO_HX711_SCK);
      Hx->pin_dout = Pin(GPIO_HX711_DAT);
      pinMode(Hx->pin_sck, OUTPUT);
      pinMode(Hx->pin_dout, INPUT);
      digitalWrite(Hx->pin_sck, LOW);
#if defined(USE_I2C) && defined(USE_HX711_M5SCALES)
    }
#endif  // USE_I2C && USE_HX711_M5SCALES

    SetWeightDelta();

    if (HxIsReady(8 * HX_TIMEOUT)) {                 // Could take 600 milliseconds after power on
      if (!Settings->weight_max) { Settings->weight_max = HX_MAX_WEIGHT / 1000; }
      if (!Settings->weight_precision) { Settings->weight_precision = HX711_CAL_PRECISION; }
      if (!Settings->weight_calibration) { Settings->weight_calibration = HX_SCALE * Settings->weight_precision; }
      if (!Settings->weight_reference) { Settings->weight_reference = HX_REFERENCE; }
      Hx->scale = Settings->weight_calibration;
      HxTareInit();
      HxRead();
    } else {
      free(Hx);
      Hx = nullptr;
    }
  }
}

void HxEvery100mSecond(void) {
  long raw = HxRead();
  if (-1 == raw) { return; }

  if (Hx->sample_count < HX_SAMPLES) {               // Test for HxSaveBeforeRestart()
    Hx->reads[Hx->sample_count] = raw;
  }

  Hx->sample_count++;
  if (HX_SAMPLES == Hx->sample_count) {
    Hx->sample_count = 0;
    // Sort HX_SAMPLES
    for (uint32_t i = 0; i < HX_SAMPLES; i++) {
      for (uint32_t j = i + 1; j < HX_SAMPLES; j++) {
        if (Hx->reads[j] > Hx->reads[i]) {
          std::swap(Hx->reads[i], Hx->reads[j]);
        }
      }
    }
    // Drop two lows and two highs from average
    long sum_raw = 0;
    for (uint32_t i = 2; i < HX_SAMPLES -2; i++) {
      sum_raw += Hx->reads[i];
    }
    Hx->raw_absolute = (sum_raw / (HX_SAMPLES -4)) * Settings->weight_precision;  // Uncalibrated value
    Hx->raw = Hx->raw_absolute / Hx->scale;          // grams

    if ((0 == Settings->weight_user_tare) && Hx->tare_flg) {  // Reset scale based on current load
      Hx->tare_flg = false;
      Settings->weight_offset = Hx->raw_absolute;    // Save for restart use
      Hx->offset = Hx->raw_absolute;
    }

    long value = Hx->raw_absolute - Hx->offset;      // Uncalibrated value
    Hx->weight = value / Hx->scale;                  // grams
    if (Hx->weight < 0) {                            // We currently do not support negative weight
      Hx->weight = 0;
    }

    if (Hx->calibrate_step) {
      Hx->calibrate_timer--;

//      AddLog(LOG_LEVEL_DEBUG, PSTR("HX7: Step %d, weight %d, last %d, raw %d, empty %d"), Hx->calibrate_step, Hx->weight, Hx->last_weight, Hx->raw, Hx->raw_empty);

      if (HX_CAL_START == Hx->calibrate_step) {      // Skip reset just initiated
        if (0 == Hx->offset) {
          Hx->calibrate_step--;                      // HX_CAL_RESET
          Hx->last_weight = Hx->weight;              // Uncalibrated value
          Hx->raw_empty = Hx->raw;
        }
        Hx->calibrate_timer = HX_CAL_TIMEOUT * (10 / HX_SAMPLES);
      }
      else if (HX_CAL_RESET == Hx->calibrate_step) { // Wait for stable reset
        if (Hx->calibrate_timer) {
          if (Hx->weight < Hx->last_weight -100) {   // Load decrease detected
            Hx->last_weight = Hx->weight;
            Hx->raw_empty = Hx->raw;
//            HxCalibrationStateTextJson(HX_MSG_CAL_REFERENCE);
          }
          else if (Hx->weight > Hx->last_weight +100) {  // Load increase detected
            Hx->calibrate_step--;                    // HX_CAL_FIRST
            Hx->calibrate_timer = HX_CAL_TIMEOUT * (10 / HX_SAMPLES);
          }
        } else {
          Hx->calibrate_step = HX_CAL_FAIL;
        }
      }
      else if (HX_CAL_FIRST == Hx->calibrate_step) { // Wait for first reference weight
        if (Hx->calibrate_timer) {
          if (Hx->weight > Hx->last_weight +100) {
            Hx->calibrate_step--;                    // HX_CAL_DONE
          }
        } else {
          Hx->calibrate_step = HX_CAL_FAIL;
        }
      }
      else if (HX_CAL_DONE == Hx->calibrate_step) {  // Second stable reference weight
        if (Hx->weight > Hx->last_weight +100) {
          Hx->calibrate_step = HX_CAL_FINISH;        // Calibration done
          Settings->weight_offset = Hx->raw_empty;
          Hx->offset = Hx->raw_empty;
          Settings->weight_calibration = (Hx->weight - Hx->raw_empty) / Settings->weight_reference;  // 1 gram
          Hx->weight = 0;                            // Reset calibration value
          HxCalibrationStateTextJson(HX_MSG_CAL_DONE);
        } else {
          Hx->calibrate_step = HX_CAL_FAIL;
        }
      }
      if (HX_CAL_FAIL == Hx->calibrate_step) {       // Calibration failed
        Hx->calibrate_step--;                        // HX_CAL_FINISH
        HxTareInit();
        HxCalibrationStateTextJson(HX_MSG_CAL_FAIL);
      }
      if (HX_CAL_FINISH == Hx->calibrate_step) {     // Calibration finished
        Hx->calibrate_step--;                        // HX_CAL_LIMBO
        Hx->calibrate_timer = 3 * (10 / HX_SAMPLES);
        Hx->scale = Settings->weight_calibration;

        if (Settings->weight_user_tare != 0) {       // Re-enable fixed tare if needed
          Settings->weight_user_tare = Hx->raw_empty / Hx->scale;
          HxTareInit();
        }

      }
      if (!Hx->calibrate_timer) {
        Hx->calibrate_step = HX_CAL_END;             // End of calibration
        Hx->weight_diff = Hx->weight_delta +2;
      }
    } else {
      if (Settings->SensorBits1.hx711_json_weight_change) {
        if (abs(Hx->weight - Hx->weight_diff) > Hx->weight_delta) {  // Use weight_delta threshold to decrease "ghost" weights
          Hx->weight_diff = Hx->weight;
          Hx->weight_changed = true;
        }
        else if (Hx->weight_changed && (abs(Hx->weight - Hx->weight_diff) < Hx->weight_delta)) {
          ResponseClear();
          ResponseAppendTime();
          HxShow(true);
          ResponseJsonEnd();
          MqttPublishTeleSensor();
          Hx->weight_changed = false;
        }
      }
    }
  }
}

void HxSaveBeforeRestart(void) {
  Hx->sample_count = HX_SAMPLES +1;                  // Stop updating Hx->weight
}

#ifdef USE_WEBSERVER
const char HTTP_HX711_WEIGHT[] PROGMEM =
  "{s}HX711 " D_WEIGHT "{m}%s " D_UNIT_KILOGRAM "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_HX711_COUNT[] PROGMEM =
  "{s}HX711 " D_COUNT "{m}%d{e}";
const char HTTP_HX711_CAL[] PROGMEM =
  "{s}HX711 %s{m}{e}";
#endif  // USE_WEBSERVER

void HxShow(bool json) {
  char scount[30] = { 0 };

  uint16_t count = 0;
  float weight = 0;
  if (Hx->calibrate_step < HX_CAL_FAIL) {
    if ((Settings->weight_absconv_a != 0) && (Settings->weight_absconv_b != 0)) {
      weight = (float)Settings->weight_absconv_a / 1e9 * Hx->raw_absolute + (float)Settings->weight_absconv_b / 1e6;
    }
    else {
      if (Hx->weight && Settings->weight_item) {
        count = (Hx->weight * 10) / Settings->weight_item;
        if (count > 1) {
          snprintf_P(scount, sizeof(scount), PSTR(",\"" D_JSON_COUNT "\":%d"), count);
        }
      }
      weight = (float)Hx->weight / 1000;             // kilograms
    }
  }
  char weight_chr[33];
  dtostrfd(weight, Settings->flag2.weight_resolution, weight_chr);

  if (json) {
    ResponseAppend_P(PSTR(",\"HX711\":{\"" D_JSON_WEIGHT "\":%s%s,\"" D_JSON_WEIGHT_RAW "\":%d,\"" D_JSON_WEIGHT_RAW_ABS "\":%d}"),
      weight_chr, scount, Hx->raw, Hx->raw_absolute);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_HX711_WEIGHT, weight_chr);
    if (count > 1) {
      WSContentSend_PD(HTTP_HX711_COUNT, count);
    }
    if (Hx->calibrate_step) {
      char cal_text[30];
      WSContentSend_PD(HTTP_HX711_CAL, GetTextIndexed(cal_text, sizeof(cal_text), Hx->calibrate_msg, kHxCalibrationStates));
    }
#endif  // USE_WEBSERVER
  }
}

#ifdef USE_WEBSERVER
#ifdef USE_HX711_GUI
/*********************************************************************************************\
 * Optional GUI
\*********************************************************************************************/

#define WEB_HANDLE_HX711 "s34"

const char HTTP_BTN_MENU_MAIN_HX711[] PROGMEM =
  "<p><form action='" WEB_HANDLE_HX711 "' method='get'><button name='reset'>" D_RESET_HX711 "</button></form></p>";

const char HTTP_BTN_MENU_HX711[] PROGMEM =
  "<p><form action='" WEB_HANDLE_HX711 "' method='get'><button>" D_CONFIGURE_HX711 "</button></form></p>";

const char HTTP_FORM_HX711[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_CALIBRATION "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_HX711 "'>"
  "<p><b>" D_REFERENCE_WEIGHT "</b> (" D_UNIT_KILOGRAM ")<br><input type='number' step='0.001' id='p1' placeholder='0' value='%s'></p>"
  "<br><button name='calibrate' type='submit'>" D_CALIBRATE "</button>"
  "</form>"
  "</fieldset><br><br>"

  "<fieldset><legend><b>&nbsp;" D_HX711_PARAMETERS "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_HX711 "'>"
  "<p><b>" D_ITEM_WEIGHT "</b> (" D_UNIT_KILOGRAM ")<br><input type='number' max='6.5535' step='0.0001' id='p2' placeholder='0.0' value='%s'></p>";

void HandleHxAction(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_HX711));

  char stemp1[32];

  if (Webserver->hasArg("save")) {
    WebGetArg("p2", stemp1, sizeof(stemp1));
    unsigned long weight_item = (!strlen(stemp1)) ? 0 : (unsigned long)(CharToFloat(stemp1) * 1000);
    snprintf_P(stemp1, sizeof(stemp1), PSTR("Sensor34 6,%d"), weight_item);  // WeightItem
    ExecuteWebCommand(stemp1);

    HandleConfiguration();
    return;
  }

  if (Webserver->hasArg("reset")) {
    snprintf_P(stemp1, sizeof(stemp1), PSTR("Sensor34 1"));  // Reset
    ExecuteWebCommand(stemp1);

    HandleRoot();  // Return to main screen
    return;
  }

  if (Webserver->hasArg("calibrate")) {
    WebGetArg("p1", stemp1, sizeof(stemp1));
    unsigned long weight_ref = (!strlen(stemp1)) ? 0 : (unsigned long)(CharToFloat(stemp1) * 1000);
    snprintf_P(stemp1, sizeof(stemp1), PSTR("Sensor34 2,%d"), weight_ref);  // Start calibration
    ExecuteWebCommand(stemp1);

    HandleRoot();  // Return to main screen
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_HX711));
  WSContentSendStyle();
  dtostrfd((float)Settings->weight_reference / 1000, 3, stemp1);
  char stemp2[20];
  dtostrfd((float)Settings->weight_item / 10000, 4, stemp2);
  WSContentSend_P(HTTP_FORM_HX711, stemp1, stemp2);
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}
#endif  // USE_HX711_GUI
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns34(uint32_t function) {
  bool result = false;

  if (FUNC_EVERY_SECOND == function) {
    if (2 == TasmotaGlobal.uptime) {                 // Fix power on init
      HxInit();
    }
  }
  else if (Hx) {
    switch (function) {
      case FUNC_EVERY_100_MSECOND:
        HxEvery100mSecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_34 == XdrvMailbox.index) {
          result = HxCommand();
        }
        break;
      case FUNC_JSON_APPEND:
        HxShow(1);
        break;
      case FUNC_SAVE_BEFORE_RESTART:
        HxSaveBeforeRestart();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HxShow(0);
        break;
#ifdef USE_HX711_GUI
      case FUNC_WEB_ADD_MAIN_BUTTON:
        if (0 == Settings->weight_user_tare) {       // Allow reset scale when no user tare is defined
          WSContentSend_P(HTTP_BTN_MENU_MAIN_HX711);
        }
        break;
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_HX711);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/" WEB_HANDLE_HX711), HandleHxAction);
        break;
#endif  // USE_HX711_GUI
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HX711
