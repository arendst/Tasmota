/*
  xdrv_10_KNX.ino - KNX IP Protocol support for Sonoff-Tasmota

  Copyright (C) 2018  Adrian Scillato  (https://github.com/ascillato)

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


#include <esp-knx-ip.h>     // Include ESP KNX IP library (https://github.com/envy/esp-knx-ip)
                            //    use the async-udp branch (https://github.com/envy/esp-knx-ip/tree/async-udp)
                            //    change on esp-knx-ip.h file the following:
                            //           #define MAX_CALLBACK_ASSIGNMENTS  20
                            //           #define MAX_CALLBACKS             20
                            //           #define ALLOW_MULTIPLE_CALLBACKS_PER_ADDRESS  1
                            //           //#define ESP_KNX_DEBUG    <-- comment this line
                            // The ESP KNX IP library calls ESPAsyncUDP library (https://github.com/me-no-dev/ESPAsyncUDP)
                            //    use ESPAsyncUDP library patched with the PR #21 (https://github.com/me-no-dev/ESPAsyncUDP/pull/21)

/* Variables in settings.h

bool          Settings.flag.knx_enabled             Enable/Disable KNX Protocol
uint16_t      Settings.knx_physsical_addr           Physical KNX address of this device
byte          Settings.knx_GA_registered            Number of group address to read
byte          Settings.knx_CB_registered            Number of group address to write
uint16_t      Settings.knx_GA_addr[MAX_KNX_GA]      Group address to read
uint16_t      Settings.knx_CB_addr[MAX_KNX_CB]      Group address to write
byte          Settings.knx_GA_param[MAX_KNX_GA]     Type of Input (relay changed, button pressed, sensor read)
byte          Settings.knx_CB_param[MAX_KNX_CB]     Type of Output (set relay, toggle relay, reply sensor value)

Constants in sonoff.h

#define MAX_KNX_GA             10           // Max number of KNX Group Addresses to read that can be set
#define MAX_KNX_CB             10           // Max number of KNX Group Addresses to write that can be set
*/

void KNX_CB_Action(message_t const &msg, void *arg); // Define function (action callback) to be called by the KNX_IP Library
                                                     // when an action is requested by another KNX Device

address_t KNX_physs_addr; // Physical KNX address of this device
address_t KNX_addr;       // KNX Address converter variable

#define KNX_Empty 255
#define KNX_temperature 17
#define KNX_humidity 18
#define KNX_MAX_device_param 18

//float last_temp;
//float last_hum;



void relay_cb(message_t const &msg, void *arg);
void temp_cb(message_t const &msg, void *arg);

#define KNX_Empty_ID 255

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
  { D_SENSOR_RELAY " 3",3 , 255, false},
  { D_SENSOR_RELAY " 4",4 , 255, false},
  { D_SENSOR_RELAY " 5",5 , 255, false},
  { D_SENSOR_RELAY " 6",6 , 255, false},
  { D_SENSOR_RELAY " 7",7 , 255, false}, // device_param[6] = Relay 7
  { D_SENSOR_RELAY " 8",8 , 255, false}, // device_param[7] = Relay 8
  { D_SENSOR_BUTTON " 1",9 , 255, true}, // device_param[8] = Button 1
  { D_SENSOR_BUTTON " 2",10 , 255, true}, // device_param[9] = Button 2
  { D_SENSOR_BUTTON " 3",11 , 255, false}, // device_param[10] = Button 3
  { D_SENSOR_BUTTON " 4",12 , 255, false}, // device_param[11] = Button 4
  { D_TEMPERATURE       ,13 , 255, false}, // device_param[12] = Temperature
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
/*
// Translations
config_webUI_t config_webUI = {
  D_CONFIGURE_KNX,
  D_KNX_PHYSICAL_ADDRESS,
  "SET",
  D_KNX_ADD,
  D_DELETE
};
*/
bool flag_knx_enabled = true;
float last_temp = 25.0;
int knx_update_rate = 5;
unsigned long next_change = 0;

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
//knx.config_web_UI(config_webUI);

//knx.config_register_Title( D_KNX_PARAMETERS );

//knx.config_register_line();

//knx.config_register_SubTitle( D_KNX_GENERAL_CONFIG );

//Set Physical KNX Address of the device
//knx.config_register_pa();
//knx.physical_address_set(Settings.knx_physs_addr);
//knx.physical_address_set(knx.PA_to_address(1, 1, 1));

//knx.config_register_label( D_KNX_PHYSICAL_ADDRESS_NOTE );

//knx.config_register_blankspace();

//knx.feedback_register_action("KNX: " D_ON, knx_toggle_flag_enabled, D_STOP, nullptr, knx_status_enabled);
//knx.feedback_register_action("KNX: " D_OFF, knx_toggle_flag_enabled, D_START, nullptr, knx_status_disabled);

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
//knx.config_set_ga(device_param[12].id, knx.GA_to_address(4,1,1));

//knx.config_register_blankspace();

//update_rate_id = knx.config_register_int( D_KNX_UPDATE_INTERVAL , Settings.knx_update_rate);
update_rate_id = knx.config_register_int( "UPDATE INTERVAL" , knx_update_rate);

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
//knx.callback_assign(cb_temp_id, knx.GA_to_address(4,1,1));

//knx.config_register_line();

//knx.feedback_register_action("", KNXSaveSettings, D_SAVE); // Save Button

//knx.feedback_register_action("", KNX_Return_button, D_CONFIGURATION); // Save Button

// END KNX WebPage Configuration


// Start KNX
//knx.start(nullptr);


}


void relay_cb(message_t const &msg, void *arg)
{
  device_parameters_t *chan = (device_parameters_t *)arg;
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX " RELAY CB"));
  AddLog(LOG_LEVEL_INFO);
  if (!flag_knx_enabled) { return; }
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

void KNX_Update_Power_State(byte device, power_t state)
{
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX " 1"));
  AddLog(LOG_LEVEL_INFO);
  if (!flag_knx_enabled) { return; }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX " 2"));
  AddLog(LOG_LEVEL_INFO);
  if ( device_param[device -1].id != KNX_Empty_ID ) // Group Address configured? 255 = empty
  {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX " 3"));
    AddLog(LOG_LEVEL_INFO);
    bool power = bitRead(state, device -1);
    knx.write_1bit(knx.config_get_ga(device_param[device -1].id), power);
  }
}

void KNX_Send_Button_Power(byte key, byte device, byte state)
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
  if (!flag_knx_enabled) { return; }
  if (device_param[device+7].id)
  {
    knx.write_1bit(knx.config_get_ga(device_param[device+7].id), !(state == 0));
  }
//  }
}

void temp_cb(message_t const &msg, void *arg)
{
  if (!flag_knx_enabled) { return; }
  switch (msg.ct)
  {
    case KNX_CT_READ:
    {
      knx.answer_2byte_float(msg.received_on, last_temp);
      break;
    }
  }
}








byte KNX_GA_Search( byte param, byte sequence = KNX_Empty )
{/*
  for (byte i = 0; i < Settings.knx_GA_registered; ++i)
  {
    if ( Settings.knx_GA_param[i] == param )
    {
      if ( Settings.knx_GA_addr[i] ) // Relay has group address set? GA=0/0/0 can not be used as KNX address, so it is used here as a: not set value
      {
         if ( i != sequence ) { return i; }
      }
    }
  }
  return 0;*/
}


void KNX_CB_Action(message_t const &msg, void *arg)
{
/*
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX " 1"));
  AddLog(LOG_LEVEL_INFO);

  device_parameters_t *chan = (device_parameters_t *)arg;
  if (!(Settings.flag.knx_enabled)) { return; }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX " 2"));
  AddLog(LOG_LEVEL_INFO);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX D_RECEIVED_FROM " %d.%d.%d " D_COMMAND " %s: %d " D_TO " %s"),
   msg.received_on.ga.area, msg.received_on.ga.line, msg.received_on.ga.member,
   (msg.ct == KNX_CT_WRITE) ? D_KNX_COMMAND_WRITE : (msg.ct == KNX_CT_READ) ? D_KNX_COMMAND_READ : D_KNX_COMMAND_OTHER,
   msg.data[0],
   device_param_cb[chan->type]);
  AddLog(LOG_LEVEL_DEBUG);

  switch (msg.ct)
  {
    case KNX_CT_WRITE:
      if (chan->type < 9) // Set Relays
      {
        ExecuteCommandPower(chan->type, msg.data[0]);
      }
      else if (chan->type < 17) // Toggle Relays
      {
        ExecuteCommandPower((chan->type) -8, 2);
      }
      break;
    case KNX_CT_READ:
      if (chan->type < 9) // reply Relays status
      {
        knx.answer_1bit(msg.received_on, chan->last_state);
      }
      else if (chan->type = KNX_temperature) // Reply Temperature
      {
        knx.answer_2byte_float(msg.received_on, last_temp);
      }
      else if (chan->type = KNX_humidity) // Reply Humidity
      {
        knx.answer_2byte_float(msg.received_on, last_hum);
      }
      break;
  }
  */
}



void KNX_Sensor(byte sensor_type, float value)
{
  /*
  if (sensor_type == KNX_temperature)
  {
    last_temp = value;
  } else if (sensor_type == KNX_humidity)
  {
    last_hum = value;
  }

  if (!(Settings.flag.knx_enabled)) { return; }

  byte i = KNX_GA_Search(sensor_type);
  while ( i > 0 ) {
    KNX_addr.value = Settings.knx_GA_addr[i];
    knx.write_2byte_float(KNX_addr, value);

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX "%s " D_SENT_TO " %d.%d.%d "),
     device_param_ga[i],
     KNX_addr.ga.area, KNX_addr.ga.line, KNX_addr.ga.member);
    AddLog(LOG_LEVEL_DEBUG);

    i = KNX_GA_Search(sensor_type, i);
  }*/
}


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char S_CONFIGURE_KNX[] PROGMEM = D_CONFIGURE_KNX;

const char HTTP_FORM_KNX[] PROGMEM =
  "<fieldset><legend style='text-align:left;'><b>&nbsp;" D_KNX_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='8,0' hidden>"
  "<br/><center>"
  "<b>" D_KNX_PHYSICAL_ADDRESS " </b>"
  "<input style='width:12%;' type='number' name='area' min='0' max='15' value='{kna'> . "
  "<input style='width:12%;' type='number' name='line' min='0' max='15' value='{knl'> . "
  "<input style='width:12%;' type='number' name='member' min='0' max='255' value='{knm'>"
  "<br/><br/>" D_KNX_PHYSICAL_ADDRESS_NOTE "<br/><br/>"
  "<input style='width:10%;' id='b1' name='b1' type='checkbox'";

const char HTTP_FORM_KNX2[] PROGMEM =
  "><b>" D_KNX_ENABLE "</b><br/></center><br/>"

  "<fieldset><center>"
  "<b>" D_KNX_GROUP_ADDRESS_TO_WRITE "</b><hr>"

  "<select name='GAop' style='width:25%;'>";

const char HTTP_FORM_KNX_OPT[] PROGMEM =
  "<option value='{vop}'>{nop}</option>";

const char HTTP_FORM_KNX_GA[] PROGMEM =
  "<input style='width:12%;' type='number' name='GAfnum' min='0' max='31' value='0'> / "
  "<input style='width:12%;' type='number' name='GAarea' min='0' max='7' value='0'> / "
  "<input style='width:12%;' type='number' name='GAfdef' min='0' max='255' value='0'> ";

const char HTTP_FORM_KNX_ADD_BTN[] PROGMEM =
  "<button type='submit' name='btn_add' value='{btnval}' style='width:18%;'>" D_ADD "</button><br/><br/>"
  "<table style='width:80%; font-size: 14px;'><col width='250'><col width='30'>";

const char HTTP_FORM_KNX_ADD_TABLE_ROW[] PROGMEM =
  "<tr><td><b>{optex} -> GAfnum / GAarea / GAfdef </b></td>"
  "<td><button type='submit' name='btn_del' value='{opval}' style='background-color: #f44336;'> " D_DELETE " </button></td></tr>";

const char HTTP_FORM_KNX3[] PROGMEM =
  "</table></center></fieldset><br/>"
  "<fieldset><center>"
  "<b>" D_KNX_GROUP_ADDRESS_TO_READ "</b><hr>";

const char HTTP_FORM_KNX4[] PROGMEM =
  "-> <select name='CBop' style='width:25%;'>";

const char HTTP_FORM_KNX_ADD_TABLE_ROW2[] PROGMEM =
  "<tr><td><b>GAfnum / GAarea / GAfdef -> {optex}</b></td>"
  "<td><button type='submit' name='btn_del' value='{opval}' style='background-color: #f44336;'> " D_DELETE " </button></td></tr>";


void HandleKNXConfiguration()
{/*
  if (HTTP_USER == webserver_state) {
    HandleRoot();
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_KNX);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_KNX));
  page += FPSTR(HTTP_HEAD_STYLE);
  page.replace(F("340px"), F("530px"));
  page += FPSTR(HTTP_FORM_KNX);
  page.replace(F("{kna"), String(KNX_physs_addr.pa.area));
  page.replace(F("{knl"), String(KNX_physs_addr.pa.line));
  page.replace(F("{knm"), String(KNX_physs_addr.pa.member));
  if ( Settings.flag.knx_enabled ) { page += F(" checked"); }

  page += FPSTR(HTTP_FORM_KNX2);
  for (byte i = 0; i < KNX_MAX_device_param ; i++)
  {
    if ( device_param[i].show )
    {
      page += FPSTR(HTTP_FORM_KNX_OPT);
      page.replace(F("{vop}"), String(device_param[i].type));
      page.replace(F("{nop}"), String(device_param_ga[i]));
    }
  }
  page += F("</select> -> ");
  page += FPSTR(HTTP_FORM_KNX_GA);
  page += FPSTR(HTTP_FORM_KNX_ADD_BTN);
  page.replace(F("{btnval}"), String(1));
  for (byte i = 0; i < Settings.knx_GA_registered ; i++)
  {
    if ( Settings.knx_GA_param[i] )
    {
      page += FPSTR(HTTP_FORM_KNX_ADD_TABLE_ROW);
      page.replace(F("{opval}"), String(Settings.knx_GA_param[i]));
      page.replace(F("{optex}"), String(device_param_ga[Settings.knx_GA_param[i]-1]));
      KNX_addr.value = Settings.knx_GA_addr[i];
      page.replace(F("GAfnum"), String(KNX_addr.ga.area));
      page.replace(F("GAarea"), String(KNX_addr.ga.line));
      page.replace(F("GAfdef"), String(KNX_addr.ga.member));
    }
  }
  page += FPSTR(HTTP_FORM_KNX3);
  page += FPSTR(HTTP_FORM_KNX_GA);
  page.replace(F("GAfnum"), F("CBfnum"));
  page.replace(F("GAarea"), F("CBarea"));
  page.replace(F("GAfdef"), F("CBfdef"));
  page += FPSTR(HTTP_FORM_KNX4);
  for (byte i = 0; i < KNX_MAX_device_param ; i++)
  {
    if ( device_param[i].show )
    {
      page += FPSTR(HTTP_FORM_KNX_OPT);
      page.replace(F("{vop}"), String(device_param[i].type));
      page.replace(F("{nop}"), String(device_param_cb[i]));
    }
  }
  page += F("</select> ");
  page += FPSTR(HTTP_FORM_KNX_ADD_BTN);
  page.replace(F("{btnval}"), String(2));
  for (byte i = 0; i < Settings.knx_CB_registered ; i++)
  {
    if ( Settings.knx_CB_param[i] )
    {
      page += FPSTR(HTTP_FORM_KNX_ADD_TABLE_ROW2);
      page.replace(F("{opval}"), String(Settings.knx_CB_param[i]));
      page.replace(F("{optex}"), String(device_param_cb[Settings.knx_CB_param[i]-1]));
      KNX_addr.value = Settings.knx_CB_addr[i];
      page.replace(F("GAfnum"), String(KNX_addr.ga.area));
      page.replace(F("GAarea"), String(KNX_addr.ga.line));
      page.replace(F("GAfdef"), String(KNX_addr.ga.member));
    }
  }
  page += F("</table></center></fieldset>");
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
  */
}


void KNX_Save_Settings()
{
  // Read all data from the webpage
  // Write to settings.knx
  // Write to ESP_KNX_IP library


  /* /////config
  Settings.knx.physs_addr = physical_address_get();
  Settings.flag.knx_enabled

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

  //snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX D_SAVE_CONFIGURATION));
  //AddLog(LOG_LEVEL_INFO);
}
#endif  // USE_WEBSERVER







/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_10

boolean Xdrv10(byte function)
{
  boolean result = false;
    switch (function) {
      case FUNC_INIT:
        KNXStart();
        break;
      case FUNC_LOOP:
        knx.loop();  // Process knx events
        break;
//      case FUNC_COMMAND:
//        result = KNXCommand();
//        break;
//      case FUNC_SET_POWER:
//        break;
    }
  return result;
}

#endif  // USE_KNX
