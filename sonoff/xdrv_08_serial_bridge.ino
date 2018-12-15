/*
  xdrv_08_serial_bridge.ino - serial bridge support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends and Dániel Zoltán Tolnai

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
 * Serial Bridge using Software Serial library (TasmotaSerial)
\*********************************************************************************************/

#define XDRV_08                    8

#define SERIAL_BRIDGE_BUFFER_SIZE  130

#include <TasmotaSerial.h>

enum SerialBridgeCommands { CMND_SSERIALSEND, CMND_SBAUDRATE };
const char kSerialBridgeCommands[] PROGMEM = D_CMND_SSERIALSEND "|" D_CMND_SBAUDRATE;

TasmotaSerial *SerialBridgeSerial;

uint8_t serial_bridge_active = 1;
uint8_t serial_bridge_in_byte_counter = 0;
unsigned long serial_bridge_polling_window = 0;
char *serial_bridge_buffer = NULL;

void SerialBridgeInput(void)
{
  while (SerialBridgeSerial->available()) {
    yield();
    uint8_t serial_in_byte = SerialBridgeSerial->read();

    if (serial_in_byte > 127) {                   // binary data...
      serial_bridge_in_byte_counter = 0;
      SerialBridgeSerial->flush();
      return;
    }
    if (serial_in_byte) {
      if ((serial_in_byte_counter < SERIAL_BRIDGE_BUFFER_SIZE -1) && (serial_in_byte != Settings.serial_delimiter)) {  // add char to string if it still fits
        serial_bridge_buffer[serial_bridge_in_byte_counter++] = serial_in_byte;
        serial_bridge_polling_window = millis();  // Wait for more data
      } else {
        serial_bridge_polling_window = 0;         // Publish now
        break;
      }
    }
  }

  if (serial_bridge_in_byte_counter && (millis() > (serial_bridge_polling_window + SERIAL_POLLING))) {
    serial_bridge_buffer[serial_bridge_in_byte_counter] = 0;  // serial data completed
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_SSERIALRECEIVED "\":\"%s\"}"), serial_bridge_buffer);
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_SSERIALRECEIVED));
//    XdrvRulesProcess();
    serial_bridge_in_byte_counter = 0;
  }
}

/********************************************************************************************/

void SerialBridgeInit(void)
{
  serial_bridge_active = 0;
  if ((pin[GPIO_SBR_RX] < 99) && (pin[GPIO_SBR_TX] < 99)) {
    serial_bridge_buffer = (char*)(malloc(SERIAL_BRIDGE_BUFFER_SIZE));
    if (serial_bridge_buffer != NULL) {
      SerialBridgeSerial = new TasmotaSerial(pin[GPIO_SBR_RX], pin[GPIO_SBR_TX]);
      if (SerialBridgeSerial->begin(Settings.sbaudrate * 1200)) {  // Baud rate is stored div 1200 so it fits into one byte
        serial_bridge_active = 1;
        SerialBridgeSerial->flush();
      }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean SerialBridgeCommand(void)
{
  char command [CMDSZ];
  boolean serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kSerialBridgeCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ((CMND_SSERIALSEND == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
    if (XdrvMailbox.data_len > 0) {
      if (1 == XdrvMailbox.index) {
        SerialBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len);
        SerialBridgeSerial->write("\n");
      }
      else if (2 == XdrvMailbox.index) {
        SerialBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len);
      }
      else if (3 == XdrvMailbox.index) {
        SerialBridgeSerial->write(Unescape(XdrvMailbox.data, &XdrvMailbox.data_len), XdrvMailbox.data_len);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
    }
  }
  else if (CMND_SBAUDRATE == command_code) {
    char *p;
    int baud = strtol(XdrvMailbox.data, &p, 10);
    if (baud > 0) {
      baud /= 1200;  // Make it a valid baudrate
      Settings.sbaudrate = (1 == XdrvMailbox.payload) ? SOFT_BAUDRATE / 1200 : baud;
      SerialBridgeSerial->begin(Settings.sbaudrate * 1200);  // Reinitialize serial port with new baud rate
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE, command, Settings.sbaudrate * 1200);
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv08(byte function)
{
  boolean result = false;

  if (serial_bridge_active) {
    switch (function) {
      case FUNC_PRE_INIT:
        SerialBridgeInit();
        break;
      case FUNC_LOOP:
        SerialBridgeInput();
        break;
      case FUNC_COMMAND:
        result = SerialBridgeCommand();
        break;
    }
  }
  return result;
}

#endif // USE_SERIAL_BRIDGE
