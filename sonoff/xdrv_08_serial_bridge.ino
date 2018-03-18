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

enum SerialBridgeCommands { CMND_SET_SBR_BAUDRATE, CMND_SET_SBR_DELIMITER };
const char kSerialBridgeCommands[] PROGMEM = D_CMND_SET_SBR_BAUDRATE "|" D_CMND_SET_SBR_DELIMITER;
const char SerialBridgeTopic[] PROGMEM = "serialbr";

TasmotaSerial *SerialBridgeSerial;

#ifdef USE_SERIAL_BRIDGE_DELIMITER
char SerialBridgeBuffer[TM_SERIAL_BUFFER_SIZE * 2 + 1];
uint8_t SerialBridgeBufferP = 0;
#endif

void SerialBridgeInit(void)
{
  if ((pin[GPIO_SBR_RX] < 99) && (pin[GPIO_SBR_TX] < 99)) {
    SerialBridgeSerial = new TasmotaSerial(pin[GPIO_SBR_RX], pin[GPIO_SBR_TX]);
    SerialBridgeSerial->begin(1200 * Settings.serial_br_baudrate_div1200); // Baud rate is stored div 1200 so it fits into one byte
    SerialBridgeSerial->flush();
  }
}

void SerialBridge50ms()
{
  boolean transmit = false;

#ifdef USE_SERIAL_BRIDGE_DELIMITER
  while (SerialBridgeSerial->available()) {
    if (Settings.serial_br_delimiter == 0xff) {
      // Delimiter disabled
      transmit = true;
    }
    else if (SerialBridgeSerial->peek() == Settings.serial_br_delimiter) {
      SerialBridgeSerial->read();
      transmit = true;
      break;
    }

    SerialBridgeSerial->readBytes(SerialBridgeBuffer + SerialBridgeBufferP, 1);
    ++SerialBridgeBufferP;
    if (SerialBridgeBufferP == (TM_SERIAL_BUFFER_SIZE * 2)) {
      transmit = true;
      break;
    }
  }
  if (SerialBridgeBufferP == 0) {
    transmit = false;
  }
#else
  int SerialBridgeBufferP;
  uint8_t SerialBridgeBuffer[SerialBridgeBufferP+1];
  if (SerialBridgeSerial->available()) {
    SerialBridgeBufferP = SerialBridgeSerial->available();
    SerialBridgeSerial->readBytes(SerialBridgeBuffer, SerialBridgeBufferP);
    transmit = true;
  }
#endif

  if (transmit) {
    SerialBridgeBuffer[SerialBridgeBufferP] = '\0';
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s"), SerialBridgeBuffer);
    MqttPublishPrefixTopic_P(STAT, SerialBridgeTopic, false);
    SerialBridgeBufferP = 0;
  }
}

boolean SerialBridgeMqttData()
{
  // Do not process irrelevant topics
  char stopic[TOPSZ];
  GetTopic_P(stopic, CMND, Settings.mqtt_topic, SerialBridgeTopic);
  if (strncmp(XdrvMailbox.topic, stopic, strlen(stopic))) {
    return false;
  }

  // Do not process empty commands
  if (XdrvMailbox.data_len == 0) {
    return false;
  }

#ifdef USE_SERIAL_BRIDGE_ESCAPE_WS
  uint8_t unesc_buffer[XdrvMailbox.data_len];
  uint8_t p = 0;
  boolean skip_next = false;

  for (uint8_t i = 0; i < XdrvMailbox.data_len; ++i) {
    if (skip_next) {
      skip_next = false;
      continue;
    }

    if (XdrvMailbox.data[i] != '\\') {
      unesc_buffer[p++] = XdrvMailbox.data[i];
      continue;
    }

    skip_next = true;
    switch (XdrvMailbox.data[i + 1]) {
      case 't':  unesc_buffer[p++] = '\t'; break;
      case 'n':  unesc_buffer[p++] = '\n'; break;
      case 'v':  unesc_buffer[p++] = '\v'; break;
      case 'f':  unesc_buffer[p++] = '\f'; break;
      case 'r':  unesc_buffer[p++] = '\r'; break;
      case 's':  unesc_buffer[p++] = ' ';  break;
      case '\\': unesc_buffer[p++] = '\\'; break;
    }
  }
  SerialBridgeSerial->write(unesc_buffer, p - 1); // Send data to serial
#else
  SerialBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len - 1); // Send data to serial
#endif

  return true;
}

boolean SerialBridgeCommand()
{
  char command [CMDSZ];
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kSerialBridgeCommands);

  if (CMND_SET_SBR_BAUDRATE == command_code) {
    char *p;
    uint32_t baud = strtol(XdrvMailbox.data, &p, 10);
    if ((baud >= 1200) && (baud <= 115200)) {
      Settings.serial_br_baudrate_div1200 = baud / 1200;
      SerialBridgeSerial->begin(1200 * Settings.serial_br_baudrate_div1200); // Reinitialize serial port with new baud rate
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE, command, 1200 * Settings.serial_br_baudrate_div1200);
    return true;
  }
  else if (CMND_SET_SBR_DELIMITER == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
      Settings.serial_br_delimiter = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.serial_br_delimiter);
    return true;
  }

  return false;
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
      case FUNC_COMMAND:
        result = SerialBridgeCommand();
        break;
      case FUNC_MQTT_DATA:
        result = SerialBridgeMqttData();
        break;
    }
  }
  return result;
}

#endif // USE_SERIAL_BRIDGE
