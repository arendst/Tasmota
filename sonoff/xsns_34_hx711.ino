/*
  xsns_34_hx711.ino - HX711 load cell support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
 * - Set reference weight once using command Sensor34 3 <reference weight in gram>
 * - Remove any weight from the scale
 * - Execute command Sensor34 2 and follow messages shown
\*********************************************************************************************/

#define XSNS_34             34

#ifndef HX_MAX_WEIGHT
#define HX_MAX_WEIGHT       20000   // Default max weight in gram
#endif
#ifndef HX_REFERENCE
#define HX_REFERENCE        250     // Default reference weight for calibration in gram
#endif
#ifndef HX_SCALE
#define HX_SCALE            120     // Default result of measured weight / reference weight when scale is 1
#endif

#define HX_TIMEOUT          120     // A reading at default 10Hz (pin RATE to Gnd on HX711) can take up to 100 milliseconds
#define HX_SAMPLES          10      // Number of samples for average calculation
#define HX_CAL_TIMEOUT      15      // Calibration step window in number of seconds

#define HX_GAIN_128         1       // Channel A, gain factor 128
#define HX_GAIN_32          2       // Channel B, gain factor 32
#define HX_GAIN_64          3       // Channel A, gain factor 64

#define D_JSON_WEIGHT_REF   "WeightRef"
#define D_JSON_WEIGHT_CAL   "WeightCal"
#define D_JSON_WEIGHT_MAX   "WeightMax"
#define D_JSON_WEIGHT_ITEM  "WeightItem"

enum HxCalibrationSteps { HX_CAL_END, HX_CAL_LIMBO, HX_CAL_FINISH, HX_CAL_FAIL, HX_CAL_DONE, HX_CAL_FIRST, HX_CAL_RESET, HX_CAL_START };

const char kHxCalibrationStates[] PROGMEM = D_HX_CAL_FAIL "|" D_HX_CAL_DONE "|" D_HX_CAL_REFERENCE "|" D_HX_CAL_REMOVE;

long hx_weight = 0;
long hx_sum_weight = 0;
long hx_offset = 0;
long hx_scale = 1;
uint8_t hx_type = 1;
uint8_t hx_sample_count = 0;
uint8_t hx_tare_flg = 0;
uint8_t hx_calibrate_step = HX_CAL_END;
uint8_t hx_calibrate_timer = 0;
uint8_t hx_calibrate_msg = 0;
uint8_t hx_pin_sck;
uint8_t hx_pin_dout;

/*********************************************************************************************/

bool HxIsReady(uint16_t timeout)
{
  // A reading can take up to 100 mS or 600mS after power on
  uint32_t start = millis();
  while ((digitalRead(hx_pin_dout) == HIGH) && (millis() - start < timeout)) { yield(); }
  return (digitalRead(hx_pin_dout) == LOW);
}

long HxRead()
{
  if (!HxIsReady(HX_TIMEOUT)) { return -1; }

  uint8_t data[3] = { 0 };
  uint8_t filler = 0x00;

  // pulse the clock pin 24 times to read the data
  data[2] = shiftIn(hx_pin_dout, hx_pin_sck, MSBFIRST);
  data[1] = shiftIn(hx_pin_dout, hx_pin_sck, MSBFIRST);
  data[0] = shiftIn(hx_pin_dout, hx_pin_sck, MSBFIRST);

  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < HX_GAIN_128; i++) {
    digitalWrite(hx_pin_sck, HIGH);
    digitalWrite(hx_pin_sck, LOW);
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

void HxReset(void)
{
  hx_tare_flg = 1;
  hx_sum_weight = 0;
  hx_sample_count = 0;
}

void HxCalibrationStateTextJson(uint8_t msg_id)
{
  char cal_text[30];

  hx_calibrate_msg = msg_id;
  snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_SENSOR_INDEX_SVALUE, XSNS_34, GetTextIndexed(cal_text, sizeof(cal_text), hx_calibrate_msg, kHxCalibrationStates));

  if (msg_id < 3) { MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR("Sensor34")); }
}

/*********************************************************************************************\
 * Supported commands for Sensor34:
 *
 * Sensor34 1                      - Reset display to 0
 * Sensor34 2                      - Start calibration
 * Sensor34 2 <weight in gram>     - Set reference weight and start calibration
 * Sensor34 3                      - Show reference weight in gram
 * Sensor34 3 <weight in gram>     - Set reference weight
 * Sensor34 4                      - Show calibrated scale value
 * Sensor34 4 <scale value>        - Set calibrated scale value
 * Sensor34 5                      - Show max weigth in gram
 * Sensor34 5 <weight in gram>     - Set max weight
 * Sensor34 6                      - Show item weigth in decigram
 * Sensor34 6 <weight in decigram> - Set item weight
\*********************************************************************************************/

bool HxCommand(void)
{
  bool serviced = true;
  bool show_parms = false;
  char sub_string[XdrvMailbox.data_len +1];

  for (byte ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }

  switch (XdrvMailbox.payload) {
    case 1:  // Reset scale
      HxReset();
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_SENSOR_INDEX_SVALUE, XSNS_34, "Reset");
      break;
    case 2:  // Calibrate
      if (strstr(XdrvMailbox.data, ",")) {
        Settings.weight_reference = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), NULL, 10);
      }
      hx_scale = 1;
      HxReset();
      hx_calibrate_step = HX_CAL_START;
      hx_calibrate_timer = 1;
      HxCalibrationStateTextJson(3);
      break;
    case 3:  // WeightRef to user reference
      if (strstr(XdrvMailbox.data, ",")) {
        Settings.weight_reference = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), NULL, 10);
      }
      show_parms = true;
      break;
    case 4:  // WeightCal to user calculated value
      if (strstr(XdrvMailbox.data, ",")) {
        Settings.weight_calibration = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), NULL, 10);
        hx_scale = Settings.weight_calibration;
      }
      show_parms = true;
      break;
    case 5:  // WeightMax
      if (strstr(XdrvMailbox.data, ",")) {
        Settings.weight_max = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), NULL, 10) / 1000;
      }
      show_parms = true;
      break;
    case 6:  // WeightItem
      if (strstr(XdrvMailbox.data, ",")) {
        Settings.weight_item = (unsigned long)(CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 2)) * 10);
      }
      show_parms = true;
      break;
    default:
      serviced = false;
  }

  if (show_parms) {
    char item[33];
    dtostrfd((float)Settings.weight_item / 10, 1, item);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Sensor34\":{\"" D_JSON_WEIGHT_REF "\":%d,\"" D_JSON_WEIGHT_CAL "\":%d,\"" D_JSON_WEIGHT_MAX "\":%d,\"" D_JSON_WEIGHT_ITEM "\":%s}}"),
      Settings.weight_reference, Settings.weight_calibration, Settings.weight_max * 1000, item);
  }

  return serviced;
}

/*********************************************************************************************/

long HxWeight()
{
  return (hx_calibrate_step < HX_CAL_FAIL) ? hx_weight : 0;
}

void HxInit(void)
{
  hx_type = 0;
  if ((pin[GPIO_HX711_DAT] < 99) && (pin[GPIO_HX711_SCK] < 99)) {
    hx_pin_sck = pin[GPIO_HX711_SCK];
    hx_pin_dout = pin[GPIO_HX711_DAT];

    pinMode(hx_pin_sck, OUTPUT);
    pinMode(hx_pin_dout, INPUT);

    digitalWrite(hx_pin_sck, LOW);

    if (HxIsReady(8 * HX_TIMEOUT)) {                 // Can take 600 milliseconds after power on
      if (!Settings.weight_max) { Settings.weight_max = HX_MAX_WEIGHT / 1000; }
      if (!Settings.weight_calibration) { Settings.weight_calibration = HX_SCALE; }
      if (!Settings.weight_reference) { Settings.weight_reference = HX_REFERENCE; }
      hx_scale = Settings.weight_calibration;
      HxRead();
      HxReset();

      hx_type = 1;
    }
  }
}

void HxEvery100mSecond(void)
{
  hx_sum_weight += HxRead();

  hx_sample_count++;
  if (HX_SAMPLES == hx_sample_count) {
    long average = hx_sum_weight / hx_sample_count;  // grams
    long value = average - hx_offset;                // grams
    hx_weight = value / hx_scale;                    // grams
    if (hx_weight < 0) { hx_weight = 0; }

    if (hx_tare_flg) {
      hx_tare_flg = 0;
      hx_offset = average;                           // grams
    }

    if (hx_calibrate_step) {
      hx_calibrate_timer--;

      if (HX_CAL_START == hx_calibrate_step) {       // Skip reset just initiated
        hx_calibrate_step--;
        hx_calibrate_timer = HX_CAL_TIMEOUT * (10 / HX_SAMPLES);
      }
      else if (HX_CAL_RESET == hx_calibrate_step) {  // Wait for stable reset
        if (hx_calibrate_timer) {
          if (hx_weight < Settings.weight_reference) {
            hx_calibrate_step--;
            hx_calibrate_timer = HX_CAL_TIMEOUT * (10 / HX_SAMPLES);
            HxCalibrationStateTextJson(2);
          }
        } else {
          hx_calibrate_step = HX_CAL_FAIL;
        }
      }
      else if (HX_CAL_FIRST == hx_calibrate_step) {  // Wait for first reference weight
        if (hx_calibrate_timer) {
          if (hx_weight > Settings.weight_reference) {
            hx_calibrate_step--;
          }
        } else {
          hx_calibrate_step = HX_CAL_FAIL;
        }
      }
      else if (HX_CAL_DONE == hx_calibrate_step) {   // Second stable reference weight
        if (hx_weight > Settings.weight_reference) {
          hx_calibrate_step = HX_CAL_FINISH;         // Calibration done
          Settings.weight_calibration = hx_weight / Settings.weight_reference;
          hx_weight = 0;                             // Reset calibration value
          HxCalibrationStateTextJson(1);
        } else {
          hx_calibrate_step = HX_CAL_FAIL;
        }
      }

      if (HX_CAL_FAIL == hx_calibrate_step) {        // Calibration failed
        hx_calibrate_step--;
        hx_tare_flg = 1;                             // Perform a reset using old scale
        HxCalibrationStateTextJson(0);
      }
      if (HX_CAL_FINISH == hx_calibrate_step) {      // Calibration finished
        hx_calibrate_step--;
        hx_calibrate_timer = 3 * (10 / HX_SAMPLES);
        hx_scale = Settings.weight_calibration;
      }

      if (!hx_calibrate_timer) {
        hx_calibrate_step = HX_CAL_END;              // End of calibration
      }
    }

    hx_sum_weight = 0;
    hx_sample_count = 0;
  }
}

#ifdef USE_WEBSERVER
const char HTTP_HX711_WEIGHT[] PROGMEM = "%s"
  "{s}HX711 " D_WEIGHT "{m}%s " D_UNIT_KILOGRAM "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_HX711_COUNT[] PROGMEM = "%s"
  "{s}HX711 " D_COUNT "{m}%d{e}";
const char HTTP_HX711_CAL[] PROGMEM = "%s"
  "{s}HX711 %s{m}{e}";
#endif  // USE_WEBSERVER

void HxShow(boolean json)
{
  char scount[30] = { 0 };

  uint16_t count = 0;
  float weight = 0;
  if (hx_calibrate_step < HX_CAL_FAIL) {
    if (hx_weight && Settings.weight_item) {
      count = (hx_weight * 10) / Settings.weight_item;
      if (count > 1) {
        snprintf_P(scount, sizeof(scount), PSTR(",\"" D_JSON_COUNT "\":%d"), count);
      }
    }
    weight = (float)hx_weight / 1000;                // kilograms
  }
  char weight_chr[33];
  dtostrfd(weight, Settings.flag2.weight_resolution, weight_chr);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"HX711\":{\"" D_JSON_WEIGHT "\":%s%s}"), mqtt_data, weight_chr, scount);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_HX711_WEIGHT, mqtt_data, weight_chr);
    if (count > 1) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_HX711_COUNT, mqtt_data, count);
    }
    if (hx_calibrate_step) {
      char cal_text[30];
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_HX711_CAL, mqtt_data, GetTextIndexed(cal_text, sizeof(cal_text), hx_calibrate_msg, kHxCalibrationStates));
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

const char S_CONFIGURE_HX711[] PROGMEM = D_CONFIGURE_HX711;

const char HTTP_BTN_MENU_MAIN_HX711[] PROGMEM =
  "<br/><form action='" WEB_HANDLE_HX711 "' method='get'><button name='reset'>" D_RESET_HX711 "</button></form>";

const char HTTP_BTN_MENU_HX711[] PROGMEM =
  "<br/><form action='" WEB_HANDLE_HX711 "' method='get'><button>" D_CONFIGURE_HX711 "</button></form>";

const char HTTP_FORM_HX711[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_CALIBRATION "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_HX711 "'>"
  "<br/><b>" D_REFERENCE_WEIGHT "</b> (" D_UNIT_KILOGRAM ")<br/><input type='number' step='0.001' id='p1' name='p1' placeholder='0' value='{1'><br/>"
  "<br/><button name='calibrate' type='submit'>" D_CALIBRATE "</button><br/>"
  "</form>"
  "</fieldset><br/><br/>"

  "<fieldset><legend><b>&nbsp;" D_HX711_PARAMETERS "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_HX711 "'>"
  "<br/><b>" D_ITEM_WEIGHT "</b> (" D_UNIT_KILOGRAM ")<br/><input type='number' max='6.5535' step='0.0001' id='p2' name='p2' placeholder='0.0' value='{2'><br/>";

void HandleHxAction(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_HX711);

  if (WebServer->hasArg("save")) {
    HxSaveSettings();
    HandleConfiguration();
    return;
  }

  char tmp[100];

  if (WebServer->hasArg("reset")) {
    snprintf_P(tmp, sizeof(tmp), PSTR("Sensor34 1"));  // Reset
    ExecuteWebCommand(tmp, SRC_WEBGUI);

    HandleRoot();  // Return to main screen
    return;
  }

  if (WebServer->hasArg("calibrate")) {
    WebGetArg("p1", tmp, sizeof(tmp));
    Settings.weight_reference = (!strlen(tmp)) ? 0 : (unsigned long)(CharToDouble(tmp) * 1000);

    HxLogUpdates();

    snprintf_P(tmp, sizeof(tmp), PSTR("Sensor34 2"));  // Start calibration
    ExecuteWebCommand(tmp, SRC_WEBGUI);

    HandleRoot();  // Return to main screen
    return;
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(D_CONFIGURE_HX711));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_HX711);
  dtostrfd((float)Settings.weight_reference / 1000, 3, tmp);
  page.replace("{1", String(tmp));
  dtostrfd((float)Settings.weight_item / 10000, 4, tmp);
  page.replace("{2", String(tmp));

  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void HxSaveSettings(void)
{
  char tmp[100];

  WebGetArg("p2", tmp, sizeof(tmp));
  Settings.weight_item = (!strlen(tmp)) ? 0 : (unsigned long)(CharToDouble(tmp) * 10000);

  HxLogUpdates();
}

void HxLogUpdates(void)
{
  char weigth_ref_chr[33];
  dtostrfd((float)Settings.weight_reference / 1000, Settings.flag2.weight_resolution, weigth_ref_chr);
  char weigth_item_chr[33];
  dtostrfd((float)Settings.weight_item / 10000, 4, weigth_item_chr);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_JSON_WEIGHT_REF " %s, " D_JSON_WEIGHT_ITEM " %s"),
    weigth_ref_chr, weigth_item_chr);
  AddLog(LOG_LEVEL_INFO);
}

#endif  // USE_HX711_GUI
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns34(byte function)
{
  boolean result = false;

  if (hx_type) {
    switch (function) {
      case FUNC_INIT:
        HxInit();
        break;
      case FUNC_EVERY_100_MSECOND:
        HxEvery100mSecond();
        break;
      case FUNC_COMMAND:
        if (XSNS_34 == XdrvMailbox.index) {
          result = HxCommand();
        }
        break;
      case FUNC_JSON_APPEND:
        HxShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        HxShow(0);
        break;
#ifdef USE_HX711_GUI
      case FUNC_WEB_ADD_MAIN_BUTTON:
        strncat_P(mqtt_data, HTTP_BTN_MENU_MAIN_HX711, sizeof(mqtt_data) - strlen(mqtt_data) -1);
        break;
      case FUNC_WEB_ADD_BUTTON:
        strncat_P(mqtt_data, HTTP_BTN_MENU_HX711, sizeof(mqtt_data) - strlen(mqtt_data) -1);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer->on("/" WEB_HANDLE_HX711, HandleHxAction);
        break;
#endif  // USE_HX711_GUI
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HX711