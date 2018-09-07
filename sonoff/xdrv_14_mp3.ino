/*
  xdrv_14_mp3.ino - MP3 support for Sonoff-Tasmota

  Copyright (C) 2018  gemu2015, mike2nl and Theo Arends

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

#ifdef USE_MP3_PLAYER
/*********************************************************************************************\
 * MP3 control for RB-DFR-562 DFRobot mini MP3 player
 * https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299
\*********************************************************************************************/

#include <TasmotaSerial.h>

TasmotaSerial *MP3Player;

#define D_CMND_MP3 "MP3"

const char S_JSON_MP3_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_MP3 "%s\":%d}";
const char S_JSON_MP3_COMMAND[] PROGMEM        = "{\"" D_CMND_MP3 "%s\"}";

enum MP3_Commands { CMND_MP3_PLAY, CMND_MP3_STOP, CMND_MP3_VOLUME};
const char kMP3_Commands[] PROGMEM = "Play|Stop|Volume";

#define MP3_CMD_PLAY 3
#define MP3_CMD_VOLUME 6
#define MP3_CMD_STOP 0x0e

uint16_t MP3_Checksum(uint8_t *array)
{
  uint16_t checksum = 0;
  for (uint8_t i = 0; i < 6; i++) {
    checksum += array[i];
  }
  checksum = checksum^0xffff;
  return checksum+1;
}

// init player define serial tx port
void MP3PlayerInit() {
  MP3Player = new TasmotaSerial(-1, pin[GPIO_MP3_DFR562]);

  if (MP3Player->begin(9600)) {
    MP3Player->flush();
  }
}

void MP3_CMD(uint8_t mp3cmd,uint16_t val) {
  uint8_t cmd[10] = {0x7e,0xff,6,0,0,0,0,0,0,0xef};
  cmd[3] = mp3cmd;
  cmd[5] = val>>8;
  cmd[6] = val;
  uint16_t chks = MP3_Checksum(&cmd[1]);								// calculate out
  cmd[7] = chks>>8;
  cmd[8] = chks;
  MP3Player->write(cmd, sizeof(cmd));
}

boolean MP3PlayerCmd() {
  char command[CMDSZ];
  boolean serviced = true;
  uint8_t disp_len = strlen(D_CMND_MP3);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_MP3), disp_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kMP3_Commands);

    if (CMND_MP3_PLAY == command_code) {
      if (XdrvMailbox.data_len > 0) {									// play
        MP3_CMD(MP3_CMD_PLAY, XdrvMailbox.payload);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_MP3_COMMAND_NVALUE, command, XdrvMailbox.payload);
    }
    else if (CMND_MP3_VOLUME == command_code) {
      if (XdrvMailbox.data_len > 0) {									// set volume
        MP3_CMD(MP3_CMD_VOLUME, XdrvMailbox.payload * 30 / 100);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_MP3_COMMAND_NVALUE, command, XdrvMailbox.payload);
    }
	else if (CMND_MP3_STOP == command_code) {							// stop
      MP3_CMD(MP3_CMD_STOP, 0);
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_MP3_COMMAND, command, XdrvMailbox.payload);
    } else {
      serviced = false;  												// Unknown command
    }
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_14

boolean Xdrv14(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      MP3PlayerInit();
      break;
    case FUNC_COMMAND:
      result = MP3PlayerCmd();
      break;
  }
  return result;
}

#endif  // USE_MP3_PLAYER
