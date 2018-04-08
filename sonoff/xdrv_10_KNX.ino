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
                            //           #define ALLOW_MULTIPLE_CALLBACKS_PER_ADDRESS  1
                            //           //#define ESP_KNX_DEBUG    <-- comment this line
                            // The ESP KNX IP library calls ESPAsyncUDP library (https://github.com/me-no-dev/ESPAsyncUDP)
                            //    use ESPAsyncUDP library patched with the PR #21 (https://github.com/me-no-dev/ESPAsyncUDP/pull/21)

/*
Variables in settings.h
-----------------------

bool          Settings.flag.knx_enabled             Enable/Disable KNX Protocol
uint16_t      Settings.knx_physsical_addr           Physical KNX address of this device
byte          Settings.knx_GA_registered            Number of group address to read
byte          Settings.knx_CB_registered            Number of group address to write
uint16_t      Settings.knx_GA_addr[MAX_KNX_GA]      Group address to read
uint16_t      Settings.knx_CB_addr[MAX_KNX_CB]      Group address to write
byte          Settings.knx_GA_param[MAX_KNX_GA]     Type of Input (relay changed, button pressed, sensor read)
byte          Settings.knx_CB_param[MAX_KNX_CB]     Type of Output (set relay, toggle relay, reply sensor value)

Constants in sonoff.h
-----------------------

#define MAX_KNX_GA             10            Max number of KNX Group Addresses to read that can be set
#define MAX_KNX_CB             10            Max number of KNX Group Addresses to write that can be set
                                             If you change MAX_KNX_CB you also have to change on esp-knx-ip.h file the following:
                                                       #define MAX_CALLBACK_ASSIGNMENTS  10
                                                       #define MAX_CALLBACKS             10
                                             Both to MAX_KNX_CB
*/

void KNX_CB_Action(message_t const &msg, void *arg); // Define function (action callback) to be called by the KNX_IP Library
                                                     // when an action is requested by another KNX Device

address_t KNX_physs_addr; // Physical KNX address of this device
address_t KNX_addr;       // KNX Address converter variable

#define KNX_Empty 255
#define KNX_temperature 17
#define KNX_humidity 18
#define KNX_MAX_device_param 18

float last_temp;
float last_hum;

typedef struct __device_parameters
{
  byte type;           // PARAMETER_ID. Used as type of GA = relay, button, sensor, etc, (INPUTS)
                       // used when an action on device triggers a MSG to send on KNX
                       // Needed because this is the value that the ESP_KNX_IP library will pass as parameter
                       // to identify the action to perform when a MSG is received

  bool show;           // HARDWARE related. to identify if the parameter exists on the device.

  bool last_state;     // LAST_STATE of relays

  callback_id_t CB_id; // ACTION_ID. To store the ID value of Registered_CB to the library.
                       // The ESP_KNX_IP requires to register the callbacks, and then, to assign an address to the registered callback
                       // So CB_id is needed to store the ID of the callback to then, assign multiple addresses to the same ID (callback)
                       // It is used as type of CB = set relay, toggle relay, reply sensor, etc, (OUTPUTS)
                       // used when a MSG receive  KNX triggers an action on the device
                       // - Multiples address to the same callback (i.e. Set Relay 1 Status) are used on scenes for example
} device_parameters_t;

// device parameters (information that can be sent)
device_parameters_t device_param[] = {
  {  1, true, false, KNX_Empty }, // device_param[ 0] = Relay 1
  {  2, true, false, KNX_Empty }, // device_param[ 1] = Relay 2
  {  3, false, false, KNX_Empty }, // device_param[ 2] = Relay 3
  {  4, false, false, KNX_Empty }, // device_param[ 3] = Relay 4
  {  5, false, false, KNX_Empty }, // device_param[ 4] = Relay 5
  {  6, false, false, KNX_Empty }, // device_param[ 5] = Relay 6
  {  7, false, false, KNX_Empty }, // device_param[ 6] = Relay 7
  {  8, false, false, KNX_Empty }, // device_param[ 7] = Relay 8
  {  9, true, false, KNX_Empty }, // device_param[ 8] = Button 1
  { 10, true, false, KNX_Empty }, // device_param[ 9] = Button 2
  { 11, false, false, KNX_Empty }, // device_param[10] = Button 3
  { 12, false, false, KNX_Empty }, // device_param[11] = Button 4
  { 13, false, false, KNX_Empty }, // device_param[12] = Button 5
  { 14, false, false, KNX_Empty }, // device_param[13] = Button 6
  { 15, false, false, KNX_Empty }, // device_param[14] = Button 7
  { 16, false, false, KNX_Empty }, // device_param[15] = Button 8
  { KNX_temperature, true, false, KNX_Empty }, // device_param[16] = Temperature
  { KNX_humidity   , true, false, KNX_Empty }, // device_param[17] = humidity
  { KNX_Empty, false, false, KNX_Empty}
};

// device parameters (information that can be sent)
const char * device_param_ga[] = {
  D_SENSOR_RELAY  " 1",   // Relay 1
  D_SENSOR_RELAY  " 2",   // Relay 2
  D_SENSOR_RELAY  " 3",   // Relay 3
  D_SENSOR_RELAY  " 4",   // Relay 4
  D_SENSOR_RELAY  " 5",   // Relay 5
  D_SENSOR_RELAY  " 6",   // Relay 6
  D_SENSOR_RELAY  " 7",   // Relay 7
  D_SENSOR_RELAY  " 8",   // Relay 8
  D_SENSOR_BUTTON " 1",   // Button 1
  D_SENSOR_BUTTON " 2",   // Button 2
  D_SENSOR_BUTTON " 3",   // Button 3
  D_SENSOR_BUTTON " 4",   // Button 4
  D_SENSOR_BUTTON " 5",   // Button 5
  D_SENSOR_BUTTON " 6",   // Button 6
  D_SENSOR_BUTTON " 7",   // Button 7
  D_SENSOR_BUTTON " 8",   // Button 8
  D_TEMPERATURE       ,   // Temperature
  D_HUMIDITY          ,   // Humidity
  nullptr
};

// device actions (posible actions to be performed on the device)
const char *device_param_cb[] = {
  D_SENSOR_RELAY " 1", // Set Relay 1 (1-On or 0-OFF)
  D_SENSOR_RELAY " 2",
  D_SENSOR_RELAY " 3",
  D_SENSOR_RELAY " 4",
  D_SENSOR_RELAY " 5",
  D_SENSOR_RELAY " 6",
  D_SENSOR_RELAY " 7",
  D_SENSOR_RELAY " 8",
  D_SENSOR_RELAY " 1 " D_BUTTON_TOGGLE, // Relay 1 Toggle (1 or 0 will toggle)
  D_SENSOR_RELAY " 2 " D_BUTTON_TOGGLE,
  D_SENSOR_RELAY " 3 " D_BUTTON_TOGGLE,
  D_SENSOR_RELAY " 4 " D_BUTTON_TOGGLE,
  D_SENSOR_RELAY " 5 " D_BUTTON_TOGGLE,
  D_SENSOR_RELAY " 6 " D_BUTTON_TOGGLE,
  D_SENSOR_RELAY " 7 " D_BUTTON_TOGGLE,
  D_SENSOR_RELAY " 8 " D_BUTTON_TOGGLE,
  D_REPLY " " D_TEMPERATURE, // Reply Temperature
  D_REPLY " " D_HUMIDITY,    // Reply Humidity
  nullptr
};

#ifndef USE_WEBSERVER
  ESP8266WebServer *WebServer;
#endif


byte KNX_GA_Search( byte param, byte start = 0 )
{
  for (byte i = start; i < Settings.knx_GA_registered; ++i)
  {
    if ( Settings.knx_GA_param[i] == param )
    {
      if ( Settings.knx_GA_addr[i] != 0 ) // Relay has group address set? GA=0/0/0 can not be used as KNX address, so it is used here as a: not set value
      {
         if ( i >= start ) { return i; }
      }
    }
  }
  return KNX_Empty;
}


void KNXStart()
{

#ifndef USE_WEBSERVER
  WebServer = new ESP8266WebServer(80);
  knx.start(WebServer, false); // Start knx and pass the webserver object to be used by UDP. False is for not showing the library webpage.
#endif

  
  
  

  Settings.flag.knx_enabled =  true;

  KNX_physs_addr.pa.area = 1;
  KNX_physs_addr.pa.line = 1;
  KNX_physs_addr.pa.member = 0;
  Settings.knx_physsical_addr = KNX_physs_addr.value;

  Settings.knx_GA_registered = 2;

  Settings.knx_GA_param[0] = 1;
  KNX_addr.ga.area = 2;
  KNX_addr.ga.line = 2;
  KNX_addr.ga.member = 1;
  Settings.knx_GA_addr[0] = KNX_addr.value;

  Settings.knx_GA_param[1] = KNX_temperature;
  KNX_addr.ga.area = 4;
  KNX_addr.ga.line = 1;
  KNX_addr.ga.member = 1;
  Settings.knx_GA_addr[1] = KNX_addr.value;

  Settings.knx_CB_registered = 1;

  Settings.knx_CB_param[0] = 1;
  KNX_addr.ga.area = 2;
  KNX_addr.ga.line = 2;
  KNX_addr.ga.member = 1;
  Settings.knx_CB_addr[0] = KNX_addr.value;





  // Set Physical KNX Address of the device
  KNX_physs_addr.value = Settings.knx_physsical_addr;
  knx.physical_address_set( KNX_physs_addr );

  // Register Group Addresses to listen to
  //     Search on the settings if there is a group address set for receive KNX messages for the type: device_param[j].type
  //     If there is, register the group address on the KNX_IP Library to Receive data for Executing Callbacks
  for (byte i = 0; i < Settings.knx_CB_registered; ++i)
  {
    byte j = Settings.knx_CB_param[i];
    if ( j > 0 )
    {
      device_param[j-1].CB_id = knx.callback_register("", KNX_CB_Action, &device_param[j-1]); // KNX IP Library requires a parameter
                                                                                                   // to identify which action was requested on the KNX network
                                                                                                   // to be performed on this device (set relay, etc.)
                                                                                                   // Is going to be used device_param[j].type that stores the type number (1: relay 1, etc)
      KNX_addr.value = Settings.knx_CB_addr[i];
      knx.callback_assign( device_param[j-1].CB_id, KNX_addr );
    }
  }

}


void KNX_CB_Action(message_t const &msg, void *arg)
{
  device_parameters_t *chan = (device_parameters_t *)arg;

  if (!(Settings.flag.knx_enabled)) { return; }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX D_RECEIVED_FROM " %d.%d.%d " D_COMMAND " %s: %d " D_TO " %s"),
   msg.received_on.ga.area, msg.received_on.ga.line, msg.received_on.ga.member,
   (msg.ct == KNX_CT_WRITE) ? D_KNX_COMMAND_WRITE : (msg.ct == KNX_CT_READ) ? D_KNX_COMMAND_READ : D_KNX_COMMAND_OTHER,
   msg.data[0],
   device_param_cb[(chan->type)-1]);
  AddLog(LOG_LEVEL_INFO);

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
}

void KNX_Update_Power_State(byte device, power_t state)
{
  if (!(Settings.flag.knx_enabled)) { return; }

  device_param[device -1].last_state = bitRead(state, device -1); // power state (on/off)

  // Search all the registered GA that has that output (variable: device) as parameter
  byte i = KNX_GA_Search(device);
  while ( i != KNX_Empty ) {
    KNX_addr.value = Settings.knx_GA_addr[i];
    knx.write_1bit(KNX_addr, device_param[device -1].last_state);

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX "%s = %d " D_SENT_TO " %d.%d.%d"),
     device_param_ga[device -1], device_param[device -1].last_state,
     KNX_addr.ga.area, KNX_addr.ga.line, KNX_addr.ga.member);
    AddLog(LOG_LEVEL_INFO);

    i = KNX_GA_Search(device, i + 1);
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
  if (!(Settings.flag.knx_enabled)) { return; }
//  if (key)
//  {

// Search all the registered GA that has that output (variable: device) as parameter
  byte i = KNX_GA_Search(device + 7);
  while ( i != KNX_Empty ) {
    KNX_addr.value = Settings.knx_GA_addr[i];
    knx.write_1bit(KNX_addr, !(state == 0));

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX "%s = %d " D_SENT_TO " %d.%d.%d"),
     device_param_ga[device + 7], !(state == 0),
     KNX_addr.ga.area, KNX_addr.ga.line, KNX_addr.ga.member);
    AddLog(LOG_LEVEL_INFO);

    i = KNX_GA_Search(device + 7, i + 1);
  }
//  }
}


void KNX_Sensor(byte sensor_type, float value)
{
  if (sensor_type == KNX_temperature)
  {
    last_temp = value;
  } else if (sensor_type == KNX_humidity)
  {
    last_hum = value;
  }

  if (!(Settings.flag.knx_enabled)) { return; }

  byte i = KNX_GA_Search(sensor_type);
  while ( i != KNX_Empty ) {
    KNX_addr.value = Settings.knx_GA_addr[i];
    knx.write_2byte_float(KNX_addr, value);

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_KNX "%s " D_SENT_TO " %d.%d.%d "),
     device_param_ga[sensor_type],
     KNX_addr.ga.area, KNX_addr.ga.line, KNX_addr.ga.member);
    AddLog(LOG_LEVEL_INFO);

    i = KNX_GA_Search(sensor_type, i+1);
  }
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
{
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

}


void KNX_Save_Settings()
{
  // Read all data from the webpage
  // Write to settings.knx
  // Write to ESP_KNX_IP library
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
                     //    It is not used by actual config of asyncUDP branch of ESP-KNX-IP Library,
                     //    but is left here for compatibility with upcoming features of ESP-KNX-IP Library
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
