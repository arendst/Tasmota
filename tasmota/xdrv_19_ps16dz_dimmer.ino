/*
  xdrv_19_ps16dz.dimmer.ino - PS_16_DZ dimmer support for Tasmota

  Copyright (C) 2020  Joel Stein and Theo Arends

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

#ifdef USE_LIGHT
#ifdef USE_PS_16_DZ
/*********************************************************************************************\
 * PS 16 DZ Serial Dimmer
\*********************************************************************************************/

#define XDRV_19                19

#define PS16DZ_BUFFER_SIZE     80

#include <TasmotaSerial.h>

TasmotaSerial *PS16DZSerial = nullptr;

struct PS16DZ {
  char *rx_buffer = nullptr;                 // Serial receive buffer
  int byte_counter = 0;
  uint8_t dimmer = 0;
} Ps16dz;

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void PS16DZSerialSend(const char *tx_buffer)
{
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send %s"), tx_buffer);

  PS16DZSerial->print(tx_buffer);
  PS16DZSerial->write(0x1B);
  PS16DZSerial->flush();
}

void PS16DZSerialSendOk(void)
{
  char tx_buffer[16];
  snprintf_P(tx_buffer, sizeof(tx_buffer), PSTR("AT+SEND=ok"));
  PS16DZSerialSend(tx_buffer);
}

// Send a serial update command to the LED controller
// For dimmer types:
//   AT+UPDATE="sequence":"1554682835320","switch":"on","bright":100
void PS16DZSerialSendUpdateCommand(void)
{
  uint8_t light_state_dimmer = light_state.getDimmer();
  // Dimming acts odd below 10% - this mirrors the threshold set on the faceplate itself
  light_state_dimmer = (light_state_dimmer < Settings.dimmer_hw_min) ? Settings.dimmer_hw_min : light_state_dimmer;
  light_state_dimmer = (light_state_dimmer > Settings.dimmer_hw_max) ? Settings.dimmer_hw_max : light_state_dimmer;

  char tx_buffer[80];
  snprintf_P(tx_buffer, sizeof(tx_buffer), PSTR("AT+UPDATE=\"sequence\":\"%d%03d\",\"switch\":\"%s\",\"bright\":%d"),
    LocalTime(), millis()%1000, power?"on":"off", light_state_dimmer);

  PS16DZSerialSend(tx_buffer);
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

void PS16DZSerialInput(void)
{
  char scmnd[20];
  while (PS16DZSerial->available()) {
    yield();
    uint8_t serial_in_byte = PS16DZSerial->read();
    if (serial_in_byte != 0x1B) {
      if (Ps16dz.byte_counter >= PS16DZ_BUFFER_SIZE - 1) {
        memset(Ps16dz.rx_buffer, 0, PS16DZ_BUFFER_SIZE);
        Ps16dz.byte_counter = 0;
      }
      if (Ps16dz.byte_counter || (!Ps16dz.byte_counter && ('A' == serial_in_byte))) {
        Ps16dz.rx_buffer[Ps16dz.byte_counter++] = serial_in_byte;
      }
    } else {
      Ps16dz.rx_buffer[Ps16dz.byte_counter++] = 0x00;

      // AT+RESULT="sequence":"1554682835320"
//      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Rcvd %s"), Ps16dz.rx_buffer);

      if (!strncmp(Ps16dz.rx_buffer+3, "RESULT", 6)) {

      }
      else if (!strncmp(Ps16dz.rx_buffer+3, "UPDATE", 6)) {
        // AT+UPDATE="switch":"on","bright":100
        char *end_str;
        char *string = Ps16dz.rx_buffer+10;
        char *token = strtok_r(string, ",", &end_str);

        bool is_switch_change = false;
        bool is_brightness_change = false;

        while (token != nullptr) {
          char* end_token;
          char* token2 = strtok_r(token, ":", &end_token);
          char* token3 = strtok_r(nullptr, ":", &end_token);

          if (!strncmp(token2, "\"switch\"", 8)) {
            bool switch_state = !strncmp(token3, "\"on\"", 4) ? true : false;

//            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Switch %d"), switch_state);

            is_switch_change = (switch_state != power);
            if (is_switch_change) {
              ExecuteCommandPower(1, switch_state, SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
            }
          }
          else if (!strncmp(token2, "\"bright\"", 8)) {
            Ps16dz.dimmer = atoi(token3);

//            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Brightness %d"), Ps16dz.dimmer);

            is_brightness_change = Ps16dz.dimmer != Settings.light_dimmer;
            if (power && (Ps16dz.dimmer > 0) && is_brightness_change) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), Ps16dz.dimmer);
              ExecuteCommand(scmnd, SRC_SWITCH);
            }
          }
          else if (!strncmp(token2, "\"sequence\"", 10)) {

//            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Sequence %s"), token3);

          }
          token = strtok_r(nullptr, ",", &end_str);
        }

        if (!is_brightness_change) {

//          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Update"));

          PS16DZSerialSendOk();
        }
      }
      else if (!strncmp(Ps16dz.rx_buffer+3, "SETTING", 7)) {
        // AT+SETTING=enterESPTOUCH - When ON button is held for over 5 seconds
        // AT+SETTING=exitESPTOUCH  - When ON button is pressed
        if (!Settings.flag.button_restrict) {  // SetOption1 - Control button multipress
          int state = WIFI_MANAGER;
          if (!strncmp(Ps16dz.rx_buffer+10, "=exit", 5)) { state = WIFI_RETRY; }
          if (state != Settings.sta_config) {
            snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WIFICONFIG " %d"), state);
            ExecuteCommand(scmnd, SRC_BUTTON);
          }
        }
      }
      memset(Ps16dz.rx_buffer, 0, PS16DZ_BUFFER_SIZE);
      Ps16dz.byte_counter = 0;
    }
  }
}

bool PS16DZSerialSendUpdateCommandIfRequired(void)
{
  if (!PS16DZSerial) { return true; }

  bool is_switch_change = (XdrvMailbox.payload != SRC_SWITCH);
  bool is_brightness_change = (light_state.getDimmer() != Ps16dz.dimmer);

  if (is_switch_change || is_brightness_change) {
    PS16DZSerialSendUpdateCommand();
  }

  return true;
}

void PS16DZInit(void)
{
  Ps16dz.rx_buffer = (char*)(malloc(PS16DZ_BUFFER_SIZE));
  if (Ps16dz.rx_buffer != nullptr) {
    PS16DZSerial = new TasmotaSerial(Pin(GPIO_RXD), Pin(GPIO_TXD), 2);
    if (PS16DZSerial->begin(19200)) {
      if (PS16DZSerial->hardwareSerial()) { ClaimSerial(); }
    }
  }
}

bool PS16DZModuleSelected(void)
{
  devices_present++;
  light_type = LT_SERIAL1;

  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv19(uint8_t function)
{
  bool result = false;

  if (PS_16_DZ == my_module_type) {
    switch (function) {
      case FUNC_LOOP:
        if (PS16DZSerial) { PS16DZSerialInput(); }
        break;
      case FUNC_SET_DEVICE_POWER:
      case FUNC_SET_CHANNELS:
        result = PS16DZSerialSendUpdateCommandIfRequired();
        break;
      case FUNC_INIT:
        PS16DZInit();
        break;
      case FUNC_MODULE_INIT:
        result = PS16DZModuleSelected();
        break;
    }
  }
  return result;
}

#endif  // USE_PS_16_DZ
#endif  // USE_LIGHT
