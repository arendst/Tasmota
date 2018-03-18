/*
  xdrv_08_KNX.ino - KNX IP Protocol support for Sonoff-Tasmota

  Copyright (C) 2018  Adrian Scillato

  Based on esp-knx-ip library for KNX/IP communication on an ESP8266
  Author: Nico Weichbrodt <envy>
  Web: https://github.com/envy/esp-knx-ip

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

#ifdef USE_KNX


#include <esp-knx-ip.h>     // Include KNX IP library

#define KNX_Empty_ID 255

void relay_cb(message_t const &msg, void *arg);
void temp_cb(message_t const &msg, void *arg);

address_t physaddr;
config_id_t enable_knx_id;
config_id_t disable_knx_id;
config_id_t ga_conf_id;
config_id_t cb_conf_id;
config_id_t cb_temp_id;
config_id_t update_rate_id;

typedef struct __device_parameters
{
  const char *name;
  byte type;
  config_id_t id;
  bool show;
} device_parameters_t;

device_parameters_t device_param[] = {
  { D_SENSOR_RELAY " 1",1 , 255, true}, // device_param[0] = Relay 1
  { D_SENSOR_RELAY " 2",2 , 255, true}, // device_param[1] = Relay 2
  { D_SENSOR_RELAY " 3",3 , 255, true},
  { D_SENSOR_RELAY " 4",4 , 255, true},
  { D_SENSOR_RELAY " 5",5 , 255, false},
  { D_SENSOR_RELAY " 6",6 , 255, false},
  { D_SENSOR_RELAY " 7",7 , 255, false}, // device_param[6] = Relay 7
  { D_SENSOR_RELAY " 8",8 , 255, false}, // device_param[7] = Relay 8
  { D_SENSOR_BUTTON " 1",9 , 255, true}, // device_param[8] = Button 1
  { D_SENSOR_BUTTON " 2",10 , 255, true}, // device_param[9] = Button 2
  { D_SENSOR_BUTTON " 3",11 , 255, true}, // device_param[10] = Button 3
  { D_SENSOR_BUTTON " 4",12 , 255, true}, // device_param[11] = Button 4
  { D_TEMPERATURE       ,13 , 255, true}, // device_param[12] = Temperature
  {nullptr,0 , 255, false}
};

const char *device_param_cb[] = {
  D_SENSOR_RELAY " 1",
  D_SENSOR_RELAY " 2",
  D_SENSOR_RELAY " 3",
  D_SENSOR_RELAY " 4",
  D_SENSOR_RELAY " 5",
  D_SENSOR_RELAY " 6",
  D_SENSOR_RELAY " 7",
  D_SENSOR_RELAY " 8",
  D_BUTTON_TOGGLE " " D_SENSOR_RELAY " 1",
  D_BUTTON_TOGGLE " " D_SENSOR_RELAY " 2",
  D_BUTTON_TOGGLE " " D_SENSOR_RELAY " 3",
  D_BUTTON_TOGGLE " " D_SENSOR_RELAY " 4",
  D_REPLY " " D_TEMPERATURE,
  nullptr
};

// Translations
config_webUI_t config_webUI = {
  D_CONFIGURE_KNX,
  D_KNX_PHYSICAL_ADDRESS,
  D_KNX_SET,
  D_KNX_ADD,
  D_DELETE
};

int device_param_quantity = 4;
bool flag_knx_enabled = true;
float last_temp = 0.0;

void KNXStart()
{

// Check which relays, buttons and sensors where consigured for this device
// for (int i = 0; i < MAX_GPIO_PIN; ++i)
// {
//   switch (GPIO(i))
//     {
//       case Relay:
//       break;
//       case Button:
//       break;
/*
for (byte j = 0; j < GPIO_SENSOR_END; j++) {
  if (!GetUsedInModule(j, cmodule.gp.io)) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SCRIPT_MODULE2, j, j, GetTextIndexed(stemp, sizeof(stemp), j, kSensorNames));
    page += mqtt_data;
  }
}
*/

//
//   device_param[i].show = (GPIO[i] == Relay)
// }
// for (int i = 0; i < MAX_BUTTONS; ++i)
// {
//   device_param[i+7].show = (GPIO[i] == Button)
// }
// for (int i = 0; i < MAX_SENSOR; ++i)
// {
//   device_param[i+7].show = (GPIO[i] == Button)
// }

// KNX WebPage Configuration
// -------------------------
// The order of the knx.***_register_*** code, is the order that is going to be shown on the web page.

// Translations
knx.config_web_UI(config_webUI);

//knx.config_register_Title( D_KNX_PARAMETERS );

//knx.config_register_line();

//knx.config_register_SubTitle( D_KNX_GENERAL_CONFIG );

//Set Physical KNX Address of the device
knx.config_register_pa();
//knx.physical_address_set(Settings.knx_physs_addr);
knx.physical_address_set(knx.PA_to_address(1, 1, 1));

//knx.config_register_label( D_KNX_PHYSICAL_ADDRESS_NOTE );

//knx.config_register_blankspace();

knx.feedback_register_action("KNX: " D_ON, knx_toggle_flag_enabled, D_STOP, nullptr, knx_status_enabled);
knx.feedback_register_action("KNX: " D_OFF, knx_toggle_flag_enabled, D_START, nullptr, knx_status_disabled);

//knx.config_register_line();

//knx.config_register_SubTitle( D_KNX_GROUP_ADDRESS_TO_WRITE );

// Register Group Addresses to Send Data to
//for (int i = 0; i < Settings.knx_Registered_GA; ++i)
for (int i = 0; i < 13; ++i)
{
  if (device_param[i].show)
  {
    device_param[i].id = knx.config_register_ga(String(device_param[i].name));
  }


  //////buscar en la config el param para setearlo
  //knx.config_set_ga(ga_conf, Settings.knx_GA_addr(i));

  //knx.config_set_ga(device_param[i].id, knx.GA_to_address(2,2,1));
}

knx.config_set_ga(device_param[0].id, knx.GA_to_address(2,2,1));

//knx.config_register_blankspace();

//update_rate_id = knx.config_register_int( D_KNX_UPDATE_INTERVAL , Settings.knx_update_rate);
update_rate_id = knx.config_register_int( D_KNX_UPDATE_INTERVAL , 5);

//knx.config_register_line();

//knx.config_register_SubTitle( D_KNX_GROUP_ADDRESS_TO_READ );

// Register Group Addresses to Receive data from and execute callbacks
//for (int i = 0; i < Settings.knx_Registered_CB; ++i)
for (int i = 0; i < 12; ++i)
{
  if (device_param[i].show)
  {
    cb_conf_id = knx.callback_register( String( device_param_cb[i] ), relay_cb, &device_param[i]);
    //////buscar en la config el param para setearlo
    //knx.callback_assign(ga_conf, Settings.knx_CB_addr(i));
    if (i==0)
    {
      knx.callback_assign(cb_conf_id, knx.GA_to_address(2,2,1));
    }
  }
}

// Temperature
int j=12;
if (device_param[j].show)
{
  cb_temp_id = knx.callback_register( String( device_param_cb[j] ), temp_cb, &device_param[j]);
}

//knx.config_register_line();

knx.feedback_register_action("", KNXSaveSettings, D_SAVE); // Save Button

knx.feedback_register_action("", KNX_Return_button, D_CONFIGURATION); // Save Button

// END KNX WebPage Configuration
}


void KNXLoop()
{
//  if (Settings.flag.knx_enabled) {
  if (flag_knx_enabled) {
    knx.loop();  // Process knx events
  }
}


void KNX_EVERY_SECOND() {
  //  if (Settings.flag.knx_enabled) {
    if (flag_knx_enabled)
    {

    }
}


void relay_cb(message_t const &msg, void *arg)
{
  device_parameters_t *chan = (device_parameters_t *)arg;
  switch (msg.ct)
  {
    case KNX_CT_WRITE:
      if (chan->type > 8)
      {
        ExecuteCommandPower(chan->id, 2);
        knx.write_1bit(knx.config_get_ga(chan->id), msg.data[0]==0);
      }
      else
      {
        ExecuteCommandPower(chan->id, msg.data[0]);
        knx.write_1bit(knx.config_get_ga(chan->id), msg.data[0]);
      }
      break;
    //case KNX_CT_READ:
      //knx.answer_1bit(msg.received_on, chan->state);
  }
}

void KNXUpdatePowerState(byte device, power_t state)
{
  if ( device_param[device -1].id != KNX_Empty_ID ) // Group Address configured? 255 = empty
  {
    bool power = bitRead(state, device -1);
    knx.write_1bit(knx.config_get_ga(device_param[device -1].id), power);
  }
}

void knx_send_button_power(byte key, byte device, byte state)
{
// key 0 = button_topic
// key 1 = switch_topic
// state 0 = off
// state 1 = on
// state 2 = toggle
// state 3 = hold
// state 9 = clear retain flag
//  if (key)
//  {
    if (device_param[device+7].id)
    {
      knx.write_1bit(knx.config_get_ga(device_param[device+7].id), !(state == 0));
    }
//  }
}

void temp_cb(message_t const &msg, void *arg)
{
  switch (msg.ct)
  {
    case KNX_CT_READ:
    {
      knx.answer_2byte_float(msg.received_on, last_temp);
      break;
    }
  }
}

void knx_toggle_flag_enabled(void *arg)
{
  flag_knx_enabled = !flag_knx_enabled;
  /*if (Settings.flag.knx_enabled)
  {
    knx.pause();
  }
  else
  {
    knx.continue();
  }*/
}

bool knx_status_enabled() { return flag_knx_enabled; }

bool knx_status_disabled() { return !flag_knx_enabled; }

void KNX_Return_button(void *arg)
{
  WebServer->sendHeader(F("Location"),F("/cn"));
  WebServer->send(302);
}

void KNXSaveSettings(void *arg)
{


  /* /////config
  Settings.knx.physs_addr = physical_address_get();
  Settings.knx.flag_knx_enabled

  k = 0
  for j = 0 to max cant items (relay1,2,3,etc)
    for i = 0 to max cant items por param (cant de addr a relay 1)
      address_t = knx.config_get_ga(device_param[j].id, i + 1);
      if address_t then
        settings.knx.paramGA(k) = j;
        settings.knx.addressGA(k) = address_t
        k++
      end if
    next i
  next j
  settings.knx.Registered_GA = k - 1

  settings.knx.update_rate = knx.config_get_int(update_rate_id)

  k = 0
  for j = 0 to max cant items (relay1,2,3,etc)
    for i = 0 to max cant items por param (cant de addr a relay 1)
      address_t = knx.config_get_cb(device_param[j].id, i + 1);
      if address_t then
        settings.knx.paramCB(k) = j;
        settings.knx.addressCB(k) = address_t
        k++
      end if
    next i
  next j
  settings.knx.Registered_CB = k - 1


  */ //////



  /*
  char stemp[20];
  char ssensor_indices[6 * MAX_DOMOTICZ_SNS_IDX];
  char tmp[100];

  for (byte i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("r%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_relay_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("k%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_key_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("s%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_switch_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
  }
  ssensor_indices[0] = '\0';
  for (byte i = 0; i < DZ_MAX_SENSORS; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("l%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_sensor_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(ssensor_indices, sizeof(ssensor_indices), PSTR("%s%s%d"), ssensor_indices, (strlen(ssensor_indices)) ? "," : "",  Settings.domoticz_sensor_idx[i]);
  }
  WebGetArg("ut", tmp, sizeof(tmp));
  Settings.domoticz_update_timer = (!strlen(tmp)) ? DOMOTICZ_UPDATE_TIMER : atoi(tmp);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_CMND_IDX " %d,%d,%d,%d, " D_CMND_KEYIDX " %d,%d,%d,%d, " D_CMND_SWITCHIDX " %d,%d,%d,%d, " D_CMND_SENSORIDX " %s, " D_CMND_UPDATETIMER " %d"),
    Settings.domoticz_relay_idx[0], Settings.domoticz_relay_idx[1], Settings.domoticz_relay_idx[2], Settings.domoticz_relay_idx[3],
    Settings.domoticz_key_idx[0], Settings.domoticz_key_idx[1], Settings.domoticz_key_idx[2], Settings.domoticz_key_idx[3],
    Settings.domoticz_switch_idx[0], Settings.domoticz_switch_idx[1], Settings.domoticz_switch_idx[2], Settings.domoticz_switch_idx[3],
    ssensor_indices, Settings.domoticz_update_timer);
  AddLog(LOG_LEVEL_INFO);
  */
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_08

boolean Xdrv08(byte function)
{
  boolean result = false;
    switch (function) {
      case FUNC_INIT:
        KNXStart();
        break;
      case FUNC_LOOP:
        KNXLoop();
        break;
      case FUNC_EVERY_SECOND:
        KNX_EVERY_SECOND();
        break;
//      case FUNC_COMMAND:
//        result = MqttCommand();
//        break;
//      case FUNC_SET_POWER:
//        break;
    }
  return result;
}

#endif  // USE_KNX
