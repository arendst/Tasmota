/*
  xdrv_19_ps16dz.dimmer.ino - PS_16_DZ dimmer and Sonoff L1 support for Sonoff-Tasmota

  Copyright (C) 2019  Joel Stein and Theo Arends

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
 * PS 16 DZ Serial Dimmer and Sonoff L1
\*********************************************************************************************/

#define XDRV_19                19

#define PS16DZ_BUFFER_SIZE     140

#define PS16DZ_SONOFF_L1_MODE_COLORFUL 1          // Colorful (static color)
#define PS16DZ_SONOFF_L1_MODE_COLORFUL_GRADIENT 2 // Colorful Gradient
#define PS16DZ_SONOFF_L1_MODE_COLORFUL_BREATH 3   // Colorful Breath
#define PS16DZ_SONOFF_L1_MODE_DIY_GRADIENT 4      // DIY Gradient (fade in and out) [Speed 1- 100, color]
#define PS16DZ_SONOFF_L1_MODE_DIY_PULSE 5         // DIY Pulse  (faster fade in and out) [Speed 1- 100, color]
#define PS16DZ_SONOFF_L1_MODE_DIY_BREATH 6        // DIY Breath (toggle on/off) [Speed 1- 100, color]
#define PS16DZ_SONOFF_L1_MODE_DIY_STROBE 7        // DIY Strobe (faster toggle on/off) [Speed 1- 100, color]
#define PS16DZ_SONOFF_L1_MODE_RGB_GRADIENT 8      // RGB Gradient
#define PS16DZ_SONOFF_L1_MODE_RGB_PULSE 9         // RGB Pulse
#define PS16DZ_SONOFF_L1_MODE_RGB_BREATH 10       // RGB Breath
#define PS16DZ_SONOFF_L1_MODE_RGB_STROBE 11       // RGB strobe
#define PS16DZ_SONOFF_L1_MODE_SYNC_TO_MUSIC 12    // Sync to music [Speed 1- 100, sensitivity 1 - 10]

#include <TasmotaSerial.h>

TasmotaSerial *PS16DZSerial = nullptr;

struct PS16DZ {
  char *tx_buffer = nullptr;                 // Serial transmit buffer
  char *rx_buffer = nullptr;                 // Serial receive buffer
  int byte_counter = 0;
  uint8_t color[3];                          // Most recent serial sent/received values
  uint8_t dimmer = 0;
  bool supports_color = false;
  bool switch_state = false;
} Ps16dz;

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void PS16DZSerialSendTxBuffer(void)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send %s"), Ps16dz.tx_buffer);

  PS16DZSerial->print(Ps16dz.tx_buffer);
  PS16DZSerial->write(0x1B);
  PS16DZSerial->flush();
}

void PS16DZSerialSendOkCommand(void)
{
  snprintf_P(Ps16dz.tx_buffer, PS16DZ_BUFFER_SIZE, PSTR("AT+SEND=ok"));
  PS16DZSerialSendTxBuffer();
}

// Send a serial update command to the LED controller
// For dimmer types:
//   AT+UPDATE="sequence":"1554682835320","switch":"on","bright":100
// For color types:
//   AT+UPDATE="sequence":"1554682835320","switch":"on","bright":100,"mode":1,"colorR":255,"colorG":46,"colorB":101,"light_types":1
void PS16DZSerialSendUpdateCommand(void)
{
  uint8_t light_state_dimmer = light_state.getDimmer();
  // Dimming acts odd below 10% - this mirrors the threshold set on the faceplate itself
  light_state_dimmer = (light_state_dimmer < 10) ? 10 : light_state_dimmer;

  snprintf_P(Ps16dz.tx_buffer, PS16DZ_BUFFER_SIZE, PSTR("AT+UPDATE=\"sequence\":\"%d%03d\",\"switch\":\"%s\",\"bright\":%d"),
    LocalTime(), millis()%1000, power?"on":"off", light_state_dimmer);

  if (Ps16dz.supports_color) {
    uint8_t light_state_rgb[3];
    light_state.getRGB(&light_state_rgb[0], &light_state_rgb[1], &light_state_rgb[2]);

    snprintf_P(Ps16dz.tx_buffer, PS16DZ_BUFFER_SIZE, PSTR("%s,\"mode\":%d,\"colorR\":%d,\"colorG\":%d,\"colorB\":%d,\"light_types\":1"),
      Ps16dz.tx_buffer, PS16DZ_SONOFF_L1_MODE_COLORFUL, light_state_rgb[0], light_state_rgb[1], light_state_rgb[2]);
  }
  PS16DZSerialSendTxBuffer();
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

bool PS16DZSerialSendUpdateCommandIfRequired(void)
{
  if (!PS16DZSerial) { return true; }

  bool is_switch_change = (XdrvMailbox.payload != SRC_SWITCH);
  bool is_brightness_change = (light_state.getDimmer() != Ps16dz.dimmer);

  uint8_t light_state_rgb[3];
  light_state.getRGB(&light_state_rgb[0], &light_state_rgb[1], &light_state_rgb[2]);
  bool is_color_change = (Ps16dz.supports_color && (memcmp(light_state_rgb, Ps16dz.color, 3) != 0));

  if (is_switch_change || is_brightness_change || is_color_change) {
    PS16DZSerialSendUpdateCommand();
  }

  return true;
}

bool PS16DZModuleSelected(void)
{
  switch (my_module_type)
  {
    case PS_16_DZ:
      light_type = LT_SERIAL1;
      break;

    case SONOFF_L1:
      light_type = LT_PWM3;
      break;
  }

  return true;
}

void PS16DZInit(void)
{
  Ps16dz.supports_color = (light_state.getColorMode() == LCM_RGB);

  Ps16dz.tx_buffer = (char*)(malloc(PS16DZ_BUFFER_SIZE));
  if (Ps16dz.tx_buffer != nullptr) {
    Ps16dz.rx_buffer = (char*)(malloc(PS16DZ_BUFFER_SIZE));
    if (Ps16dz.rx_buffer != nullptr) {
      PS16DZSerial = new TasmotaSerial(pin[GPIO_RXD], pin[GPIO_TXD], 2);
      if (PS16DZSerial->begin(19200)) {
        if (PS16DZSerial->hardwareSerial()) { ClaimSerial(); }
      }
    }
  }
}

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
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Received %s"), Ps16dz.rx_buffer);

      if (!strncmp(Ps16dz.rx_buffer+3, "UPDATE", 6)) {
        // AT+UPDATE="switch":"on","light_type":1,"colorR":255,"colorG":255,"colorB":255,"bright":100,"mode":19,"speed":100,"sensitive":100
        char *end_str;
        char *string = Ps16dz.rx_buffer+10;
        char *token = strtok_r(string, ",", &end_str);

        bool color_updated[3] = { false, false, false };
        memcpy(Ps16dz.color, Settings.light_color, 3);
        bool is_switch_change = false;
        bool is_color_change = false;
        bool is_brightness_change = false;

        while (token != nullptr) {
          char* end_token;
          char* token2 = strtok_r(token, ":", &end_token);
          char* token3 = strtok_r(nullptr, ":", &end_token);

          if (!strncmp(token2, "\"switch\"", 8)) {
            Ps16dz.switch_state = !strncmp(token3, "\"on\"", 4) ? true : false;

            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Switch %d"), Ps16dz.switch_state);

            is_switch_change = (Ps16dz.switch_state != power);
            if (is_switch_change) {
              ExecuteCommandPower(1, Ps16dz.switch_state, SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
            }
          }
          else if (!strncmp(token2, "\"color", 6)) {

            char color_channel_name = token2[6];
            int color_index;
            switch(color_channel_name)
            {
              case 'R': color_index = 0;
                break;
              case 'G': color_index = 1;
                break;
              case 'B': color_index = 2;
                break;
            }
            int color_value = atoi(token3);
            Ps16dz.color[color_index] = color_value;
            color_updated[color_index] = true;

            bool all_color_channels_updated = color_updated[0] && color_updated[1] && color_updated[2];
            if (all_color_channels_updated) {

              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Color R:%d, G:%d, B:%d"), Ps16dz.color[0], Ps16dz.color[1], Ps16dz.color[2]);

              is_color_change = (memcmp(Ps16dz.color, Settings.light_color, 3) != 0);
            }

            if (power && is_color_change) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLOR "2 %02x%02x%02x"), Ps16dz.color[0], Ps16dz.color[1], Ps16dz.color[2]);
              ExecuteCommand(scmnd, SRC_SWITCH);
            }
          }
          else if (!strncmp(token2, "\"bright\"", 8)) {
            Ps16dz.dimmer = atoi(token3);

            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Brightness %d"), Ps16dz.dimmer);

            is_brightness_change = Ps16dz.dimmer != Settings.light_dimmer;
            if (power && (Ps16dz.dimmer > 0) && is_brightness_change) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), Ps16dz.dimmer);
              ExecuteCommand(scmnd, SRC_SWITCH);
            }
          }
          else if (!strncmp(token2, "\"sequence\"", 10)) {

            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Sequence %s"), token3);

          }
          token = strtok_r(nullptr, ",", &end_str);
        }

        if (!is_color_change && !is_brightness_change) {

          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Update"));

          PS16DZSerialSendOkCommand();
        }
      }
      else if (!strncmp(Ps16dz.rx_buffer+3, "SETTING", 7)) {
        // AT+SETTING=enterESPTOUCH - When ON button is held for over 5 seconds
        // AT+SETTING=exitESPTOUCH  - When ON button is pressed
        if (!Settings.flag.button_restrict) {
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

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv19(uint8_t function)
{
  bool result = false;

  if ((PS_16_DZ == my_module_type) || (SONOFF_L1 == my_module_type)) {
    switch (function) {
      case FUNC_LOOP:
        if (PS16DZSerial) { PS16DZSerialInput(); }
        break;
      case FUNC_MODULE_INIT:
        result = PS16DZModuleSelected();
        break;
      case FUNC_INIT:
        PS16DZInit();
        break;
      case FUNC_SET_DEVICE_POWER:
      case FUNC_SET_CHANNELS:
        result = PS16DZSerialSendUpdateCommandIfRequired();
        break;
    }
  }
  return result;
}

#endif  // USE_PS_16_DZ
#endif  // USE_LIGHT
