/*
  xdrv_08_serial_bridge.ino - serial bridge support for Sonoff-Tasmota

  Copyright (C) 2018  Dániel Zoltán Tolnai

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

#ifdef USE_SERIAL_BRIDGE
/*********************************************************************************************\
 * Serial-MQTT Bridge
\*********************************************************************************************/
#include <TasmotaSerial.h>

TasmotaSerial *SerialBridgeSerial;
char SerialBridgeTopic[TOPSZ];

void SerialBridgeInit(void)
{
  if ((pin[GPIO_SBR_RX] < 99) && (pin[GPIO_SBR_TX] < 99)) {
    GetTopic_P(SerialBridgeTopic, CMND, Settings.mqtt_topic, PSTR("serialbr"));
    SerialBridgeSerial = new TasmotaSerial(pin[GPIO_SBR_RX], pin[GPIO_SBR_TX]);
    SerialBridgeSerial->begin();
    SerialBridgeSerial->flush();
  }
}

void SerialBridge50ms()
{
  if (SerialBridgeSerial->available()) {
    size_t len = SerialBridgeSerial->available();
    uint8_t sbuf[len+1];
    SerialBridgeSerial->readBytes(sbuf, len);
    sbuf[len] = '\0';
    snprintf_P(mqtt_data, sizeof(mqtt_data), "%s", sbuf);
    MqttPublishPrefixTopic_P(STAT, PSTR("serialbr"), false);
  }
}

boolean SerialBridgeMqttData()
{
  // Do not process irrelevant topics
  if (strncmp(XdrvMailbox.topic, SerialBridgeTopic, strlen(SerialBridgeTopic))) {
    return false;
  }

  // Do not process empty commands
  if (XdrvMailbox.data_len == 0) {
    return false;
  }

  // Send data to serial
  SerialBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len - 1);
  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_08

boolean Xdrv08(byte function)
{
  boolean result = false;

  if ((pin[GPIO_SBR_RX] < 99) && (pin[GPIO_SBR_TX] < 99)) {
    switch (function) {
      case FUNC_INIT:
        SerialBridgeInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        SerialBridge50ms();
        break;
      case FUNC_MQTT_DATA:
        result = SerialBridgeMqttData();
        break;
    }
  }
  return result;
}

#endif // USE_SERIAL_BRIDGE
