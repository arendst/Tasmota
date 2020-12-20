/*
  TasmotaClient.cpp - Library for microcontrollers enclientd by Tasmota
  
  Copyright (C) 2019  Andre Thomas
  
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

#include <Arduino.h>
#include <TasmotaClient.h>

/*************************************************\
 * TasmotaClient shared structures
\*************************************************/

typedef union {
  uint32_t data;
  struct {
    uint32_t func_json_append : 1;               // Supports FUNC_JSON_APPEND callback
    uint32_t func_every_second : 1;              // Supports FUNC_EVERY_SECOND callback (No JSON)
    uint32_t func_every_100_msecond : 1;         // Supports FUNC_EVERY_100_MSECOND callback (No JSON)
    uint32_t func_client_send : 1;                // Supports FUNC_COMMAND
    uint32_t spare4 : 1;
    uint32_t spare5 : 1;
    uint32_t spare6 : 1;
    uint32_t spare7 : 1;
    uint32_t spare8 : 1;
    uint32_t spare9 : 1;
    uint32_t spare10 : 1;
    uint32_t spare11 : 1;
    uint32_t spare12 : 1;
    uint32_t spare13 : 1;
    uint32_t spare14 : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t spare17 : 1;
    uint32_t spare18 : 1;
    uint32_t spare19 : 1;
    uint32_t spare20 : 1;
    uint32_t spare21 : 1;
    uint32_t spare22 : 1;
    uint32_t spare23 : 1;
    uint32_t spare24 : 1;
    uint32_t spare25 : 1;
    uint32_t spare26 : 1;
    uint32_t spare27 : 1;
    uint32_t spare28 : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t spare31 : 1;
  };
} FeatureCfg;

/**************************************************\
 * Settings structure - MUST remain 4 byte aligned
\**************************************************/

struct FEATURES {
  uint32_t features_version;
  FeatureCfg features;
} Settings;

/********************************************************************************\
 * Command structure for sending/receiving commands - MUST remain 4 byte aligned
\********************************************************************************/

struct COMMAND {
  uint8_t command;
  uint8_t parameter;
  uint8_t unused2;
  uint8_t unused3;
} Command;

TasmotaClient::TasmotaClient(HardwareSerial *device)
{
  serial = device;
  Settings.features_version = TASMOTA_CLIENT_LIB_VERSION;
  Settings.features.func_json_append = 0;
  Settings.features.func_every_second = 0;
  Settings.features.func_every_100_msecond = 0;
  Settings.features.func_client_send = 0;
  Settings.features.spare4 = 0;
  Settings.features.spare5 = 0;
  Settings.features.spare6 = 0;
  Settings.features.spare7 = 0;
  Settings.features.spare8 = 0;
  Settings.features.spare9 = 0;
  Settings.features.spare10 = 0;
  Settings.features.spare11 = 0;
  Settings.features.spare12 = 0;
  Settings.features.spare13 = 0;
  Settings.features.spare14 = 0;
  Settings.features.spare15 = 0;
  Settings.features.spare16 = 0;
  Settings.features.spare17 = 0;
  Settings.features.spare18 = 0;
  Settings.features.spare19 = 0;
  Settings.features.spare20 = 0;
  Settings.features.spare21 = 0;
  Settings.features.spare22 = 0;
  Settings.features.spare23 = 0;
  Settings.features.spare24 = 0;
  Settings.features.spare25 = 0;
  Settings.features.spare26 = 0;
  Settings.features.spare27 = 0;
  Settings.features.spare28 = 0;
  Settings.features.spare29 = 0;
  Settings.features.spare30 = 0;
  Settings.features.spare31 = 0;
}

void TasmotaClient::sendFeatures(void)
{
  char buffer[sizeof(Settings)];
  memcpy(&buffer, &Settings, sizeof(Settings));
  serial->write(char(PARAM_DATA_START));
  for (uint8_t ca = 0; ca < sizeof(buffer); ca++) {
    serial->write(char(buffer[ca]));
  }
  serial->write(char(PARAM_DATA_END));
}

void TasmotaClient::sendJSON(char *json)
{
  serial->write(char(PARAM_DATA_START));
  for (uint8_t ca = 0; ca < strlen(json); ca++) {
    serial->write(json[ca]);
  }
  serial->write(char(PARAM_DATA_END));
}

void TasmotaClient::attach_FUNC_JSON(callbackFunc func)
{
  Settings.features.func_json_append = 1;
  FUNC_JSON = func;
}

void TasmotaClient::attach_FUNC_EVERY_SECOND(callbackFunc func)
{
  Settings.features.func_every_second = 1;
  FUNC_EVERY_SECOND = func;
}

void TasmotaClient::attach_FUNC_EVERY_100_MSECOND(callbackFunc func)
{
  Settings.features.func_every_100_msecond = 1;
  FUNC_EVERY_100_MSECOND = func;
}

void TasmotaClient::attach_FUNC_COMMAND_SEND(callbackFunc1 func)
{
  Settings.features.func_client_send = 1;
  FUNC_SEND = func;
}

uint8_t TasmotaClient::waitforbytes(uint16_t num, uint16_t timeout)
{
  uint16_t timer = 0;
  while (timer < timeout) {
    if (serial->available() > (int)(num-1)) {
      return 1;
    }
    delay(1);
    timer++;
  }
  return 0;
}

void TasmotaClient::ProcessSend(uint8_t sz)
{
  if (waitforbytes(sz+2,50)) {
    serial->read(); // read leading character
    for (uint8_t idx = 0; idx < sz; idx++) {
      receive_buffer[idx] = serial->read();
    }
    serial->read(); // read trailing byte
    receive_buffer[sz] = '\0';
    FUNC_SEND(receive_buffer);
  }
}

void TasmotaClient::ProcessCommand(void)
{
  if (waitforbytes(sizeof(Command)+1, 100)) {
    char buffer[sizeof(Command)];
    for (uint8_t idx = 0; idx < sizeof(Command); idx++) {
      buffer[idx] = serial->read();
    }
    serial->read(); // Remove end of command character
    memcpy(&Command, &buffer, sizeof(Command));
    switch (Command.command) {
      case CMND_FEATURES:
        sendFeatures();
        break;
      case CMND_FUNC_JSON:
        FUNC_JSON();
        break;
      case CMND_FUNC_EVERY_SECOND:
        FUNC_EVERY_SECOND();
        break;
      case CMND_FUNC_EVERY_100_MSECOND:
        FUNC_EVERY_100_MSECOND();
        break;
      case CMND_CLIENT_SEND:
        ProcessSend(Command.parameter);
        break;
      default:
        break;
    }
  }
}

void TasmotaClient::SendCommand(uint8_t cmnd, uint8_t param)
{
 Command.command = cmnd;
 Command.parameter = param;
 Command.unused2 = 0;
 Command.unused3 = 0;
 uint8_t tmp[sizeof(Command)];
 memcpy(&tmp, &Command, sizeof(Command));
 serial->write(char(CMND_START));
 for (uint8_t idx = 0; idx < sizeof(Command); idx++) {
   serial->write(tmp[idx]);
 }
 serial->write(char(CMND_END));
}

void TasmotaClient::SendTele(char *data)
{
  SendCommand(CMND_PUBLISH_TELE, strlen(data));
  serial->write(char(PARAM_DATA_START));
  for (uint8_t idx = 0; idx < strlen(data); idx++) {
	  serial->write(data[idx]);
  }
  serial->write(char(PARAM_DATA_END));
}

void TasmotaClient::ExecuteCommand(char *cmnd)
{
  SendCommand(CMND_EXECUTE_CMND, strlen(cmnd));
  serial->write(char(PARAM_DATA_START));
  for (uint8_t idx = 0; idx < strlen(cmnd); idx++) {
    serial->write(cmnd[idx]);
  }
  serial->write(char(PARAM_DATA_END));
}

void TasmotaClient::loop(void)
{
  if (serial->available()) {
    uint8_t cmnd = serial->read();
    switch (cmnd) {
      case CMND_START:
        ProcessCommand();
        break;
      default:
        break;
    }
  }
}
