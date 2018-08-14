/*
  xdrv_98_display.ino - EXPERIMENTAL display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz and Adafruit

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
#ifdef USE_DISPLAY

// get asci number until delimiter and return asci number lenght and value
uint8_t atoiv(char *cp,int16_t *res) {
  uint8_t index=0;
  *res=atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

enum DisplayCommands { CMND_DISP_TEXT };
const char kDisplayCommands[] PROGMEM = D_CMND_DISP_TEXT ;
#define D_CMND_DISPLAY "Display"
const char S_JSON_DISPLAY_COMMAND_VALUE[] PROGMEM =        "{\"" D_CMND_DISPLAY "%s\":\"%s\"}";
int16_t xpos,ypos;


#define XDRV_98

boolean Xdrv98(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      if (i2c_flg || spi_flg) {
        DisplayInit();
      }
      break;
    case FUNC_EVERY_50_MSECOND:
      //DisplayRefresh();
      break;
    case FUNC_COMMAND:
      result = DisplayCommand();
      break;
    case FUNC_MQTT_SUBSCRIBE:
      //DisplayMqttSubscribe();
      break;
    case FUNC_MQTT_DATA:
      //result = DisplayMqttData();
      break;
    case FUNC_SET_POWER:
      //DisplaySetPower();
      break;
    case FUNC_SHOW_SENSOR:
      //DisplayLocalSensor();
      break;
  }
  return result;
}

#endif  // USE_DISPLAY
