/*
  xdrv_19_ps16dz_dimmer.ino - PS_16_DZ dimmer support for Sonoff-Tasmota

  Copyright (C) 2019 Joel Stein and Theo Arends

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

#define XDRV_19                19

#define PS16DZ_BUFFER_SIZE     140

#define PS16DZ_TYPE_ACK        0
#define PS16DZ_TYPE_PWR        1
#define PS16DZ_TYPE_DIM        2

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

bool ps16dz_ignore_dim = false;            // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction

uint8_t color_channel_values[3];           // Most recent serial sent/received values
char *ps16dz_tx_buffer = nullptr;          // Serial transmit buffer
char *ps16dz_rx_buffer = nullptr;          // Serial receive buffer
int ps16dz_byte_counter = 0;

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void printTimestamp(void)
{
    snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s%d%03d"), ps16dz_tx_buffer, LocalTime(), millis()%1000);
}

void PS16DZSendCommand(char type = 0, uint8_t value = 0)
{
  switch(type){
    case PS16DZ_TYPE_ACK:
      snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "AT+SEND=ok"));
      break;
    case PS16DZ_TYPE_PWR:
    case PS16DZ_TYPE_DIM:
      snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "AT+UPDATE=\"sequence\":\""));
      printTimestamp();
      if ( type == PS16DZ_TYPE_PWR) {
        snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s\",\"switch\":\"%s\""), ps16dz_tx_buffer, value?"on":"off");
      }
      else if ( type == PS16DZ_TYPE_DIM) {
        snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s\",\"bright\":%d"), ps16dz_tx_buffer, round(value * (100. / 255.)));
      }
      break;
    }

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send serial command: %s"), ps16dz_tx_buffer );

    PS16DZSerial->print(ps16dz_tx_buffer);
    PS16DZSerial->write(0x1B);
    PS16DZSerial->flush();
}

bool PS16DZSetPower(void)
{
  bool status = false;

  uint8_t rpower = XdrvMailbox.index;
  int16_t source = XdrvMailbox.payload;

  if (source != SRC_SWITCH && PS16DZSerial) {  // ignore to prevent loop from pushing state from faceplate interaction

    PS16DZSendCommand(PS16DZ_TYPE_PWR, rpower);

    status = true;
  }
  return status;
}

bool PS16DZSetChannels(void)
{
  if(PS16DZSerial) {
    switch (light_subtype) {
      case LST_SINGLE:
        // PS16DZSerialDuty(((uint8_t*)XdrvMailbox.data)[0]);
        break;
      case LST_RGB:
        P16DZSetChannelsIfRequired();
        break;
    }
  }

  return true;
}

void P16DZSetChannelsIfRequired()
{
  bool is_color_change = memcmp(XdrvMailbox.data, color_channel_values, 3) != 0;

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: channels were: R:%d, G:%d, B:%d"),
    color_channel_values[0],
    color_channel_values[1],
    color_channel_values[2]);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: channels received: R:%d, G:%d, B:%d, changed:%d"),
    ((uint8_t*)XdrvMailbox.data)[0],
    ((uint8_t*)XdrvMailbox.data)[1],
    ((uint8_t*)XdrvMailbox.data)[2],
    is_color_change);

  if(!is_color_change) return;
  PS16DZSerialRGB(((uint8_t*)XdrvMailbox.data)[0], ((uint8_t*)XdrvMailbox.data)[1], ((uint8_t*)XdrvMailbox.data)[2]);
}

void PS16DZSerialDuty(uint8_t duty)
{
  if (duty > 0 && !ps16dz_ignore_dim) {
    if (duty < 25) {
      duty = 25;  // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
    }

    PS16DZSendCommand(PS16DZ_TYPE_DIM, duty);

  } else {
    ps16dz_ignore_dim = false;  // reset flag

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send Dim Level skipped due to 0 or already set. Value=%d"), duty);
  }
}

void PS16DZSerialRGB(uint8_t red, uint8_t green, uint8_t blue)
{
  // Write out static color update eg.
  // AT+UPDATE="sequence":"1554682835320","mode":1,"colorR":255,"colorB":101,"colorG":46,"light_types":1

  snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "AT+UPDATE=\"sequence\":\""));
  printTimestamp();
  int light_types_value = 1;
  snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s\",\"mode\":%d"), ps16dz_tx_buffer, PS16DZ_SONOFF_L1_MODE_COLORFUL);
  snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s,\"colorR\":%d"), ps16dz_tx_buffer, red);
  snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s,\"colorG\":%d"), ps16dz_tx_buffer, green);
  snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s,\"colorB\":%d"), ps16dz_tx_buffer, blue);
  snprintf_P(ps16dz_tx_buffer, PS16DZ_BUFFER_SIZE, PSTR( "%s,\"light_types\":%d"), ps16dz_tx_buffer, light_types_value);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send serial command: %s"), ps16dz_tx_buffer );

  PS16DZSerial->print(ps16dz_tx_buffer);
  PS16DZSerial->write(0x1B);
  PS16DZSerial->flush();
}

void PS16DZResetWifi(void)
{
  if (!Settings.flag.button_restrict) {
    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " %d", 2);
    ExecuteCommand(scmnd, SRC_BUTTON);
  }
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

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
  ps16dz_tx_buffer = (char*)(malloc(PS16DZ_BUFFER_SIZE));
  if (ps16dz_tx_buffer != nullptr) {
    ps16dz_rx_buffer = (char*)(malloc(PS16DZ_BUFFER_SIZE));
    if (ps16dz_rx_buffer != nullptr) {
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
    if (serial_in_byte != 0x1B){
      if (ps16dz_byte_counter >= PS16DZ_BUFFER_SIZE - 1) {
        memset(ps16dz_rx_buffer, 0, PS16DZ_BUFFER_SIZE);
        ps16dz_byte_counter = 0;
      }
      if (ps16dz_byte_counter || (!ps16dz_byte_counter && serial_in_byte == 'A')) {
        ps16dz_rx_buffer[ps16dz_byte_counter++] = serial_in_byte;
      }
    }
    else {
      ps16dz_rx_buffer[ps16dz_byte_counter++] = 0x00;
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: command received: %s"), ps16dz_rx_buffer);
      if(!strncmp(ps16dz_rx_buffer+3, "UPDATE", 6)) {
        char *end_str;
        char *string = ps16dz_rx_buffer+10;
        char* token = strtok_r(string, ",", &end_str);

        char color_channel_name;
        bool color_channel_updated[3] = { false, false, false };
        memcpy(color_channel_values, Settings.light_color, 3);
        bool is_color_change = false;

        while (token != nullptr) {
          char* end_token;
          char* token2 = strtok_r(token, ":", &end_token);
          char* token3 = strtok_r(nullptr, ":", &end_token);

          if(!strncmp(token2, "\"switch\"", 8)){
            bool ps16dz_power = !strncmp(token3, "\"on\"", 4)?true:false;
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: power received: %s"), token3);
            // if((power || Settings.light_dimmer > 0) && (power !=ps16dz_power)) {
            //   ExecuteCommandPower(1, ps16dz_power, SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
            // }
          }
          else if(sscanf(token2, "\"color%c\"", &color_channel_name)==1){

            int color_channel_index;

            switch(color_channel_name)
            {
              case 'R':
                color_channel_index = 0;
                break;
              case 'G':
                color_channel_index = 1;
                break;
              case 'B':
                color_channel_index = 2;
                break;
            }

            int color_channel_value = atoi(token3);
            color_channel_values[color_channel_index] = color_channel_value;
            color_channel_updated[color_channel_index] = true;

            bool all_color_channels_updated =
              color_channel_updated[0] &&
              color_channel_updated[1] &&
              color_channel_updated[2];

            if(all_color_channels_updated)
            {
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: color received: R:%d, G:%d, B:%d"),
                color_channel_values[0],
                color_channel_values[1],
                color_channel_values[2]);
            }

            is_color_change = memcmp(color_channel_values, Settings.light_color, 3) != 0;

            if(power && all_color_channels_updated && is_color_change)
            {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLOR " %02x%02x%02x"),
                color_channel_values[0],
                color_channel_values[1],
                color_channel_values[2]);

              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send CMND_COLOR_STR=%s"), scmnd );

              ExecuteCommand(scmnd, SRC_SWITCH);
            }
          }
          else if(!strncmp(token2, "\"bright\"", 8)){
            uint8_t ps16dz_bright = atoi(token3);
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: brightness received: %d"), ps16dz_bright);
            // if(power && ps16dz_bright > 0 && ps16dz_bright != Settings.light_dimmer) {

            //   snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), ps16dz_bright );

            //   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Send CMND_DIMMER_STR=%s"), scmnd );

            //   ps16dz_ignore_dim = true;
            //   ExecuteCommand(scmnd, SRC_SWITCH);
            // }
          }
          else if(!strncmp(token2, "\"sequence\"", 10)){
            //ps16dz_seq = strtoull(token3+1, nullptr, 10);
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: sequence received: %s"), token3);
          }
          token = strtok_r(nullptr, ",", &end_str);
        }

        if(!is_color_change)
        {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PSZ: Update received"));
          PS16DZSendCommand(PS16DZ_TYPE_ACK);
        }
      }
      else if(!strncmp(ps16dz_rx_buffer+3, "SETTING", 7)) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("PSZ: Reset"));
        PS16DZResetWifi();
      }
      memset(ps16dz_rx_buffer, 0, PS16DZ_BUFFER_SIZE);
      ps16dz_byte_counter = 0;
    }
  }
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv19(uint8_t function)
{
  bool result = false;

  if (PS_16_DZ == my_module_type ||
      SONOFF_L1 == my_module_type) {
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
        result = PS16DZSetPower();
        break;
      case FUNC_SET_CHANNELS:
        result = PS16DZSetChannels();
        break;
    }
  }
  return result;
}

#endif  // USE_PS_16_DZ
#endif  // USE_LIGHT
