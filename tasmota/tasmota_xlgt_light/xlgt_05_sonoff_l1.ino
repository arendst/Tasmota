/*
  xlgt_05_sonoff_l1.ino - Sonoff L1 led support for Tasmota

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

#ifdef USE_LIGHT
#ifdef USE_SONOFF_L1
/*********************************************************************************************\
 * Sonoff L1
\*********************************************************************************************/

#define XLGT_05                           5

#define SONOFF_L1_START_DELAY                // Sync Nuvotron power state with Tasmota on power up
//#define SONOFF_L1_ALLOW_REMOTE_INTERRUPT     // During schemes 2..4 or if fade is active
#define SONOFF_L1_DEBUG1                     // Add send and receive logging

#define SONOFF_L1_BUSY                  200  // Time in milliseconds to handle a serial request

#define SONOFF_L1_BUFFER_SIZE           170

#define SONOFF_L1_MODE_COLORFUL           1  // [Color key] Colorful (static color)
#define SONOFF_L1_MODE_COLORFUL_GRADIENT  2  // [SMOOTH] Colorful Gradient
#define SONOFF_L1_MODE_COLORFUL_BREATH    3  // [FADE] Colorful Breath
#define SONOFF_L1_MODE_DIY_GRADIENT       4  // DIY Gradient (fade in and out) [Speed 1- 100, color]
#define SONOFF_L1_MODE_DIY_PULSE          5  // DIY Pulse  (faster fade in and out) [Speed 1- 100, color]
#define SONOFF_L1_MODE_DIY_BREATH         6  // DIY Breath (toggle on/off) [Speed 1- 100, color]
#define SONOFF_L1_MODE_DIY_STROBE         7  // DIY Strobe (faster toggle on/off) [Speed 1- 100, color]
#define SONOFF_L1_MODE_RGB_GRADIENT       8  // RGB Gradient
#define SONOFF_L1_MODE_RGB_PULSE          9  // [STROBE] RGB Pulse
#define SONOFF_L1_MODE_RGB_BREATH        10  // RGB Breath
#define SONOFF_L1_MODE_RGB_STROBE        11  // [FLASH] RGB strobe
#define SONOFF_L1_MODE_SYNC_TO_MUSIC     12  // Sync to music [Speed 1- 100, sensitivity 1 - 10]

struct SNFL1 {
  char *buffer;
  uint32_t process_time = 0;
  uint32_t unlock = 0;
  uint32_t busy = SONOFF_L1_BUSY;
  uint16_t sequence;
  uint8_t color[3];
  uint8_t dimmer;
  uint8_t power;
  uint8_t old_music_sync = 0;
  uint8_t sensitive;
  uint8_t speed;
} Snfl1;

const char kL1Commands[] PROGMEM = "L1|"  // Prefix
  "MusicSync";

void (* const L1Command[])(void) PROGMEM = {
  &CmndMusicSync };

/********************************************************************************************/

#ifdef SONOFF_L1_START_DELAY
#include <Ticker.h>
Ticker SnfL1StartDelay;

void SnfL1SendDelayed(void) {
  SnfL1Send();
}
#endif  // SONOFF_L1_START_DELAY

#include <Ticker.h>
Ticker SnfL1Backlog;

void SnfL1SendBacklog(void) {
//  AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: ++Exec backlog"));

  SnfL1SetChannels(true);
}

void SnfL1Send(void) {
#ifdef SONOFF_L1_DEBUG1
  AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: Send '%s'"), Snfl1.buffer);
#endif
  Serial.print(Snfl1.buffer);
  Serial.write(0x1B);
  Serial.flush();

  Snfl1.process_time = millis();
  Snfl1.unlock = Snfl1.process_time + Snfl1.busy;  // Wait for 'AT+RESULT="sequence":"1630250830439"'
}

void SnfL1SendOk(void) {
  snprintf_P(Snfl1.buffer, SONOFF_L1_BUFFER_SIZE, PSTR("AT+SEND=ok"));
  SnfL1Send();
}

bool SnfL1SerialInput(void) {
  if (TasmotaGlobal.serial_in_byte != 0x1B) {
    if (TasmotaGlobal.serial_in_byte_counter >= SONOFF_L1_BUFFER_SIZE) {
      TasmotaGlobal.serial_in_byte_counter = 0;
    }
    if (TasmotaGlobal.serial_in_byte_counter || (!TasmotaGlobal.serial_in_byte_counter && ('A' == TasmotaGlobal.serial_in_byte))) {  // A from AT
      TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = TasmotaGlobal.serial_in_byte;
    }
  } else {
    TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = 0x00;

    // AT+RESULT="sequence":"0458"          = L1
    // AT+RESULT="sequence":"0458","switch" = L1 lite - just returns part of receive buffer
    // AT+RESULT="sequence":"1554682835320" = L1 both
    // AT+UPDATE="sequence":"1554682835320","switch":"on","light_type":1,"colorR":0,"colorG":16,"colorB":0,"bright":6,"mode":1
    // AT+UPDATE="switch":"on","light_type":1,"colorR":255,"colorG":0,"colorB":0,"bright":6,"mode":1,"speed":100,"sensitive":10
#ifdef SONOFF_L1_DEBUG1
    AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: Rcvd '%s'"), TasmotaGlobal.serial_in_buffer);
#endif
    if (!strncmp(TasmotaGlobal.serial_in_buffer +3, "RESULT", 6)) {
#ifdef SONOFF_L1_ALLOW_REMOTE_INTERRUPT
      Snfl1.busy = 500;
#else
/*
      // Read sequence number and calculate time it took from send of same sequence
      //   this indicates Nuvoton processing speed (50-60 for L1, 30-40 for L1 lite)
      // Important for constant color change schemes and fade
      char *end_str;
      char *string = TasmotaGlobal.serial_in_buffer +10;
      char *token = strtok_r(string, ",", &end_str);
      if (token) {
        char* end_token;
        char* token2 = strtok_r(token, ":", &end_token);
        char* token3 = strtok_r(nullptr, ":", &end_token);
        if (!strncmp(token2, "\"sequence\"", 10) && (strlen(token3) > 3)) {
          token3 = token3 + strlen(token3) - 4;                 // Last three digits
          if (Snfl1.sequence == atoi(token3)) {
            Snfl1.busy = (millis() - Snfl1.process_time) + 30;  // Add some scatter time
            if (Snfl1.busy > SONOFF_L1_BUSY) {
              Snfl1.busy = SONOFF_L1_BUSY;
            }
          }
        }
      }
*/
      Snfl1.busy = 80;
#endif  // SONOFF_L1_ALLOW_REMOTE_INTERRUPT
//      AddLog(LOG_LEVEL_DEBUG, PSTR("Sl1: ++Busy %d"), Snfl1.busy);

      SnfL1SendOk();
      return true;

    }
    else if (!strncmp(TasmotaGlobal.serial_in_buffer +3, "UPDATE", 6)) {
      char cmnd_dimmer[20];
      char cmnd_color[20];
      char *end_str;
      char *string = TasmotaGlobal.serial_in_buffer +10;
      char *token = strtok_r(string, ",", &end_str);

      bool color_updated[3] = { false, false, false };

      bool switch_state = false;
      bool is_power_change = false;
      bool is_color_change = false;
      bool is_brightness_change = false;

      while (token != nullptr) {
        char* end_token;
        char* token2 = strtok_r(token, ":", &end_token);
        char* token3 = strtok_r(nullptr, ":", &end_token);

        if (!strncmp(token2, "\"sequence\"", 10)) {

//          AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: Rcvd sequence %s"), token3);

          token = nullptr;
        }

        else if (!strncmp(token2, "\"switch\"", 8)) {
          switch_state = !strncmp(token3, "\"on\"", 4) ? true : false;

//          AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: Rcvd switch %d (%d)"), switch_state, Light.power);

          is_power_change = (switch_state != Light.power);
        }

        else if (!strncmp(token2, "\"color", 6)) {
          char color_channel_name = token2[6];
          int color_index;
          switch(color_channel_name) {
            case 'R': color_index = 0;
              break;
            case 'G': color_index = 1;
              break;
            case 'B': color_index = 2;
              break;
          }
          int color_value = atoi(token3);
          Snfl1.color[color_index] = color_value;
          color_updated[color_index] = true;

          bool all_color_channels_updated = color_updated[0] && color_updated[1] && color_updated[2];
          if (all_color_channels_updated) {

//            AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: Rcvd color R%d G%d B%d (R%d G%d B%d)"),
//              Snfl1.color[0], Snfl1.color[1], Snfl1.color[2],
//              Settings->light_color[0], Settings->light_color[1], Settings->light_color[2]);

            is_color_change = (Light.power && (memcmp(Snfl1.color, Settings->light_color, 3) != 0));
          }
          snprintf_P(cmnd_color, sizeof(cmnd_color), PSTR(D_CMND_COLOR "2 %02x%02x%02x"), Snfl1.color[0], Snfl1.color[1], Snfl1.color[2]);
        }

        else if (!strncmp(token2, "\"bright\"", 8)) {
          uint8_t dimmer = atoi(token3);

//          AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: Rcvd dimmer %d (%d)"), dimmer, Settings->light_dimmer);

          is_brightness_change = (Light.power && (dimmer > 0) && (dimmer != Settings->light_dimmer));
          snprintf_P(cmnd_dimmer, sizeof(cmnd_dimmer), PSTR(D_CMND_DIMMER " %d"), dimmer);
        }

        else if (!strncmp(token2, "\"mode\"", 6)) {
          uint8_t received_mode = atoi(token3);
          Settings->sbflag1.sonoff_l1_music_sync = (SONOFF_L1_MODE_SYNC_TO_MUSIC == received_mode);
        }

        token = strtok_r(nullptr, ",", &end_str);
      }

      SnfL1SendOk();

      if (is_power_change) {
        if (Settings->light_scheme > 0) {
          if (!switch_state) {  // If power off RC button pressed stop schemes
            char cmnd_scheme[20];
            snprintf_P(cmnd_scheme, sizeof(cmnd_scheme), PSTR(D_CMND_SCHEME " 0"));
            ExecuteCommand(cmnd_scheme, SRC_REMOTE);
          }
        }
//      else {
          ExecuteCommandPower(1, switch_state, SRC_REMOTE);
//        }
      }
      else if (is_brightness_change) {
        ExecuteCommand(cmnd_dimmer, SRC_REMOTE);
      }
      else if (Light.power && is_color_change) {
        if (0 == Settings->light_scheme) {  // Fix spurious color receptions when scheme > 0
          if (Settings->light_fade) {  // Disable fade as RC button colors overrule and are immediate supressing ghost colors
            char cmnd_fade[20];
            snprintf_P(cmnd_fade, sizeof(cmnd_fade), PSTR(D_CMND_FADE " 0"));
            ExecuteCommand(cmnd_fade, SRC_REMOTE);
          }
          if (Settings->light_correction) {  // Disable ledtable as RC button colors overrule and are immediate supressing ghost colors
            char cmnd_fade[20];
            snprintf_P(cmnd_fade, sizeof(cmnd_fade), PSTR(D_CMND_LEDTABLE " 0"));
            ExecuteCommand(cmnd_fade, SRC_REMOTE);
          }
          ExecuteCommand(cmnd_color, SRC_REMOTE);
        }
      }
    }

    return true;
  }
  TasmotaGlobal.serial_in_byte = 0;
  return false;
}

/********************************************************************************************/

void SnfL1SetChannels(bool backlog) {
  // Takes about 100ms at 9600 bps

  if (!backlog) {
    uint8_t power = Light.power;
    bool power_changed = (Snfl1.power != power);
    Snfl1.power = power;

    uint8_t dimmer = light_state.getDimmer();
    bool dimmer_changed = (Snfl1.dimmer != dimmer);
    Snfl1.dimmer = dimmer;

    uint8_t *scale_col = (uint8_t*)XdrvMailbox.topic;
    bool color_changed = false;
    if (!power_changed) {
      for (uint32_t i = 0; i < 3; i++) {
        if (Snfl1.color[i] != scale_col[i]) {
          color_changed = true;
        }
        Snfl1.color[i] = scale_col[i];
      }
    }
    if (!power_changed && !dimmer_changed && !color_changed && (Snfl1.old_music_sync == Settings->sbflag1.sonoff_l1_music_sync)) { return; }
  }

  if (TimeReached(Snfl1.unlock)) {

//    AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: ++SC snd"));

    uint32_t mode = (Settings->sbflag1.sonoff_l1_music_sync) ? SONOFF_L1_MODE_SYNC_TO_MUSIC : SONOFF_L1_MODE_COLORFUL;
    Snfl1.sequence = millis()%1000;
    snprintf_P(Snfl1.buffer, SONOFF_L1_BUFFER_SIZE, PSTR("AT+UPDATE=\"sequence\":\"%d%03d\",\"switch\":\"%s\",\"light_type\":1,\"colorR\":%d,\"colorG\":%d,\"colorB\":%d,\"bright\":%d,\"mode\":%d"),
      LocalTime(), Snfl1.sequence,
      Snfl1.power ? "on" : "off",
      Snfl1.color[0], Snfl1.color[1], Snfl1.color[2],
      Snfl1.dimmer,
      mode);
    if (SONOFF_L1_MODE_SYNC_TO_MUSIC == mode) {
      snprintf_P(Snfl1.buffer, SONOFF_L1_BUFFER_SIZE, PSTR("%s,\"sensitive\":%d,\"speed\":%d"),
        Snfl1.buffer,
        Snfl1.sensitive,
        Snfl1.speed);
    }

#ifdef SONOFF_L1_START_DELAY
    static bool first_call = true;
    if (first_call) {
      SnfL1StartDelay.once_ms(900, SnfL1SendDelayed);  // Allow startup time for Nuvotron microcontroller
      first_call = false;
    } else
#endif  // SONOFF_L1_START_DELAY
    SnfL1Send();
  } else {
    if (Settings->light_scheme == 0) {
      // Fix last fade state
//      AddLog(LOG_LEVEL_DEBUG, PSTR("SL1: ++SC bck"));

      SnfL1Backlog.once_ms(SONOFF_L1_BUSY, SnfL1SendBacklog);  // Set backlog
    }
  }
}

bool SnfL1SetChannelsFromFunc(void) {
  static bool first_call = true;
  if (first_call) {
    first_call = false;                          // Allow MusicSync at init time
  } else {
    Settings->sbflag1.sonoff_l1_music_sync = 0;  // Disable MusicSync on user color change
  }
  SnfL1SetChannels(false);
  return true;
}

bool SnfL1ModuleSelected(void) {
  if (SONOFF_L1 == TasmotaGlobal.module_type) {
    if (PinUsed(GPIO_RXD) && PinUsed(GPIO_TXD)) {
      Snfl1.buffer = (char*)malloc(SONOFF_L1_BUFFER_SIZE);
      if (Snfl1.buffer) {
        SetSerial(19200, TS_SERIAL_8N1);

        Snfl1.power = !Light.power;
        Snfl1.dimmer = !light_state.getDimmer();
        Snfl1.sensitive = 5;   // 1..10
        Snfl1.speed = 50;      // 1..100

        TasmotaGlobal.light_type = LT_RGB;
        TasmotaGlobal.light_driver = XLGT_05;
        AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: Sonoff L1 Found"));
        return true;
      }
    }
  }
  return false;
}

void CmndMusicSync(void) {
  // Format is L1MusicSync on/off/toggle, sensitivity, speed
  // sensitivity 1..10, speed 1..100
  if (XdrvMailbox.data_len > 0) {
    Snfl1.old_music_sync = Settings->sbflag1.sonoff_l1_music_sync;
    uint32_t parm[3] = { 0 };
    ParseParameters(3, parm);
    if (2 == parm[0]) {
      Settings->sbflag1.sonoff_l1_music_sync ^= 1;                 // Toggle
    } else {
      Settings->sbflag1.sonoff_l1_music_sync = parm[0] & 1;        // On or Off
    }
    if ((parm[1] > 0) && (parm[1] < 11)) {
      Snfl1.sensitive = parm[1];             // 1..10
    }
    if ((parm[2] > 0) && (parm[2] < 101)) {
      Snfl1.speed = parm[2];                 // 1..100
    }
    SnfL1SetChannels(false);
  }
  Response_P(PSTR("{\"%s\":{\"Mode\":\"%s\",\"Sensitive\":%d,\"Speed\":%d}}"),
    XdrvMailbox.command, GetStateText(Settings->sbflag1.sonoff_l1_music_sync), Snfl1.sensitive, Snfl1.speed);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt05(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SERIAL:
      result = SnfL1SerialInput();
      break;
    case FUNC_SET_CHANNELS:
      result = SnfL1SetChannelsFromFunc();
      break;
    case FUNC_MODULE_INIT:
      result = SnfL1ModuleSelected();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kL1Commands, L1Command);
      break;
  }
  return result;
}

#endif  // USE_SONOFF_L1
#endif  // USE_LIGHT