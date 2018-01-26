/*
  xdrv_interface.ino - Driver interface support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends inspired by ESPEasy

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

boolean (* const xdrv_func_ptr[])(byte) PROGMEM = {   // Driver Function Pointers
#ifdef XDRV_01
  &Xdrv01,
#endif

#ifdef XDRV_02
  &Xdrv02,
#endif

#ifdef XDRV_03
  &Xdrv03,
#endif

#ifdef XDRV_04
  &Xdrv04,
#endif

#ifdef XDRV_05
  &Xdrv05,
#endif

#ifdef XDRV_06
  &Xdrv06,
#endif

#ifdef XDRV_07
  &Xdrv07,
#endif

#ifdef XDRV_08
  &Xdrv08,
#endif

#ifdef XDRV_09
  &Xdrv09,
#endif

#ifdef XDRV_10
  &Xdrv10,
#endif
};

const uint8_t xdrv_present = sizeof(xdrv_func_ptr) / sizeof(xdrv_func_ptr[0]);  // Number of drivers found

boolean XdrvCommand(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
//  XdrvMailbox.valid = 1;
  XdrvMailbox.index = index;
  XdrvMailbox.data_len = data_len;
  XdrvMailbox.payload = payload;
  XdrvMailbox.topic = type;
  XdrvMailbox.data = dataBuf;

  return XdrvCall(FUNC_COMMAND);
}

void XdrvSetPower(power_t mpower)
{
//  XdrvMailbox.valid = 1;
  XdrvMailbox.index = mpower;

  XdrvCall(FUNC_SET_POWER);
}

boolean XdrvMqttData(char *topicBuf, uint16_t stopicBuf, char *dataBuf, uint16_t sdataBuf)
{
  XdrvMailbox.index = stopicBuf;
  XdrvMailbox.data_len = sdataBuf;
  XdrvMailbox.topic = topicBuf;
  XdrvMailbox.data = dataBuf;

  return XdrvCall(FUNC_MQTT_DATA);
}

/*********************************************************************************************\
 * Function call to all xdrv
 *
 * FUNC_INIT
 * FUNC_MQTT_SUBSCRIBE
 * FUNC_MQTT_INIT
 * return FUNC_MQTT_DATA
 * return FUNC_COMMAND
 * FUNC_SET_POWER
 * FUNC_SHOW_SENSOR
 * FUNC_EVERY_SECOND
 * FUNC_EVERY_50_MSECOND
\*********************************************************************************************/

boolean XdrvCall(byte Function)
{
  boolean result = false;

  for (byte x = 0; x < xdrv_present; x++) {
    result = xdrv_func_ptr[x](Function);
    if (result) {
      break;
    }
  }

  return result;
}
