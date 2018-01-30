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

void XDrvInit()
{
  for (byte i = 0; i < XDRV_MAX; i++) {
    xdrv_func_ptr[i] = NULL;
  }
  xdrv_present = 0;

#ifdef XDRV_01
  xdrv_func_ptr[xdrv_present++] = &Xdrv01;
#endif

#ifdef XDRV_02
  xdrv_func_ptr[xdrv_present++] = &Xdrv02;
#endif

#ifdef XDRV_03
  xdrv_func_ptr[xdrv_present++] = &Xdrv03;
#endif

#ifdef XDRV_04
  xdrv_func_ptr[xdrv_present++] = &Xdrv04;
#endif

#ifdef XDRV_05
  xdrv_func_ptr[xdrv_present++] = &Xdrv05;
#endif

#ifdef XDRV_06
  xdrv_func_ptr[xdrv_present++] = &Xdrv06;
#endif

#ifdef XDRV_07
  xdrv_func_ptr[xdrv_present++] = &Xdrv07;
#endif

#ifdef XDRV_08
  xdrv_func_ptr[xdrv_present++] = &Xdrv08;
#endif

#ifdef XDRV_09
  xdrv_func_ptr[xdrv_present++] = &Xdrv09;
#endif

#ifdef XDRV_10
  xdrv_func_ptr[xdrv_present++] = &Xdrv10;
#endif

//  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "Drivers %d"), xdrv_present);
//  AddLog(LOG_LEVEL_DEBUG);

  XdrvCall(FUNC_INIT);
}

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

void XdrvSetPower(uint8_t mpower)
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
