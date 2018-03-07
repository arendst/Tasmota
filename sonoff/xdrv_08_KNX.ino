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

#define ROOT_PREFIX "/knx"  // [Default ""] This gets prepended to all webserver paths, default is empty string "". Set this to "/knx" if you want the config to be available on http://<ip>/knx

#include <esp-knx-ip-lite.h>     // Include KNX IP Lite library

config_id_t hostname_id;
callback_id_t callback_id;
config_id_t enable_sending_id;
config_id_t update_rate_id;
config_id_t status_ga_id;
bool button_knx_state;

void KNXStart()
{

  button_knx_state = false;

  snprintf_P(log_data, sizeof(log_data), PSTR("KNX START"));
  AddLog(LOG_LEVEL_DEBUG);
  
  // Set Physical KNX Address of ESP KNX
  knx.physical_address_set(knx.GA_to_address(1,1,1));

  // Register the config options
  hostname_id = knx.config_register_string("Hostname", 20, String("sonoff_DHT11"));
  //type_id = knx.config_register_options("Type", type_options, SONOFF_TYPE_BASIC);
  enable_sending_id = knx.config_register_bool("Send on update", true);
  update_rate_id = knx.config_register_int("Update rate (s)", 1);

  // Register and Set Group Addresses to Write to
  status_ga_id = knx.config_register_ga("Channel 1 Status GA");
  knx.config_set_ga(status_ga_id, knx.GA_to_address(2,2,1));
  //channels[1].status_ga_id = knx.config_register_ga("Channel 2 Status GA", is_4ch_or_4ch_pro);
  //channels[2].status_ga_id = knx.config_register_ga("Channel 3 Status GA", is_4ch_or_4ch_pro);
  //channels[3].status_ga_id = knx.config_register_ga("Channel 4 Status GA", is_4ch_or_4ch_pro);
  //temp_ga = knx.config_register_ga("Temperature", show_periodic_options);
  //knx.config_set_ga(temp_ga,4,1,1);
  //hum_ga = knx.config_register_ga("Humidity", show_periodic_options);
  //knx.config_set_ga(hum_ga,4,1,2);

  // Register and set Group Addresses to Receive data from and execute callbacks
//  callback_id = knx.callback_register("Channel 1", channel_cb, &button_knx_state);
//  knx.callback_assign(callback_id, knx.GA_to_address(2,2,1));
  //knx.callback_register("Channel 2", channel_cb, &channels[1], is_4ch_or_4ch_pro);
  //knx.callback_register("Channel 3", channel_cb, &channels[2], is_4ch_or_4ch_pro);
  //knx.callback_register("Channel 4", channel_cb, &channels[3], is_4ch_or_4ch_pro);
  //knx.callback_register("Read Temperature", temp_cb);
  //knx.callback_register("Read Humidity", hum_cb);

  // Register data to be shown on the webserver
  knx.feedback_register_bool("Channel 1 is on", &button_knx_state);
  knx.feedback_register_action("Toogle channel 1", toggle_chan);
  //knx.feedback_register_bool("Channel 2 is on", &(channels[1].state), is_4ch_or_4ch_pro);
  //knx.feedback_register_action("Toogle channel 2", toggle_chan, &channels[1], is_4ch_or_4ch_pro);
  //knx.feedback_register_bool("Channel 3 is on", &(channels[2].state), is_4ch_or_4ch_pro);
  //knx.feedback_register_action("Toogle channel 3", toggle_chan, &channels[2], is_4ch_or_4ch_pro);
  //knx.feedback_register_bool("Channel 4 is on", &(channels[3].state), is_4ch_or_4ch_pro);
  //knx.feedback_register_action("Toogle channel 4", toggle_chan, &channels[3], is_4ch_or_4ch_pro);
  //knx.feedback_register_float("Temperature (°C)", &last_temp);
  //knx.feedback_register_float("Humidity (%)", &last_hum);




  // Start knx
  #ifdef USE_WEBSERVER
    //knx.start(); // Start KNX with its own webserver
    knx.start(WebServer); // Start KNX with a webserver already running on 'WebServer'
                          // On this case you might want to change ROOT_PREFIX to
                          // #define ROOT_PREFIX   "/knx"  before  #include <esp-knx-ip.h>
    snprintf_P(log_data, sizeof(log_data), PSTR("KNX START WEBSERVER"));
    AddLog(LOG_LEVEL_DEBUG);                       
  #else
    knx.start(nullptr); // Start KNX WITHOUT webserver
    snprintf_P(log_data, sizeof(log_data), PSTR("KNX START WITHOUT WEBSERVER"));
    AddLog(LOG_LEVEL_DEBUG);          
  #endif  // USE_WEBSERVER
  
}


void KNXLoop()
{
  knx.loop();  // Process knx events
}

/*
void KNX_EVERY_SECOND() {
  
}
*/

void toggle_chan(void *arg)
{
//  sonoff_channel_t *chan = (sonoff_channel_t *)arg;
  button_knx_state = !button_knx_state;
//  digitalWrite(chan->pin, chan->state ? HIGH : LOW);
//  digitalWrite(LED_PIN, chan->state ? LOW : HIGH);
  knx.write_1bit(knx.config_get_ga(status_ga_id), button_knx_state);
  snprintf_P(log_data, sizeof(log_data), PSTR("KNX TOGGLE"));
  AddLog(LOG_LEVEL_DEBUG);  
}

/*
void channel_cb(message_t const &msg, void *arg)
{
//  sonoff_channel_t *chan = (sonoff_channel_t *)arg;
  switch (msg.ct)
  {
    case KNX_CT_WRITE:
//      chan->state = msg.data[0];
//      Serial.println(chan->state ? "Toggle on" : "Toggle off");
//      digitalWrite(chan->pin, chan->state ? HIGH : LOW);
//      digitalWrite(LED_PIN, chan->state ? LOW : HIGH);
      knx.write_1bit(knx.config_get_ga(status_ga_id), button_knx_state);
      snprintf_P(log_data, sizeof(log_data), PSTR("KNX WRITE"));
      AddLog(LOG_LEVEL_DEBUG);      
      break;
     case KNX_CT_READ:
      knx.answer_1bit(msg.received_on, button_knx_state);
      snprintf_P(log_data, sizeof(log_data), PSTR("KNX TELEGRAM RECEIVED"));
      AddLog(LOG_LEVEL_DEBUG);         
      break;
  }
}
*/

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_08

boolean Xdrv08(byte function)
{
  boolean result = false;
//  if (Settings.flag.knx_enabled) {
    switch (function) {
      case FUNC_INIT:
        KNXStart();
        break;
      case FUNC_LOOP:
        KNXLoop();
        break;
//      case FUNC_EVERY_SECOND;
//        KNX_EVERY_SECOND();
//        break;
//#ifdef USE_WEBSERVER
//      case FUNC_WEB_APPEND:
//        EnergyShow(0);
//        break;
//#endif  // USE_WEBSERVER
//      case FUNC_SAVE_BEFORE_RESTART:
//        EnergySaveState();
//        break;                   
//      case FUNC_COMMAND:
//        result = MqttCommand();
//        break;
    }
//  }    
  return result;
}

#endif  // USE_KNX
