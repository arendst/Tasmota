/*
  TasmotaClient.h - Library for microcontrollers enslaved by Tasmota
  
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

#ifndef __TASMOTACLIENT_H__
#define __TASMOTACLIENT_H__

#include <Arduino.h>

/*************************************************\
 * TasmotaClient Configuration Defaults
\*************************************************/

#define TASMOTA_CLIENT_LIB_VERSION      20191129

/*************************************************\
 * TasmotaClient Command definitions
\*************************************************/

#define CMND_START                     0xFC
#define CMND_END                       0xFD

#define CMND_FEATURES                  0x01
#define CMND_FUNC_JSON                 0x02
#define CMND_FUNC_EVERY_SECOND         0x03
#define CMND_FUNC_EVERY_100_MSECOND    0x04
#define CMND_CLIENT_SEND                0x05
#define CMND_PUBLISH_TELE              0x06
#define CMND_EXECUTE_CMND              0x07

/*************************************************\
 * TasmotaClient Parameter defintions
\*************************************************/

#define PARAM_DATA_START               0xFE
#define PARAM_DATA_END                 0xFF

/*************************************************\
 * Prototypes for passing functions as parameters
\*************************************************/

typedef void (*callbackFunc) (void);
typedef void (*callbackFunc1) (char*);

/*************************************************\
 * TasmotaClient Class
\*************************************************/

class TasmotaClient {
    public:
     char receive_buffer[100];
     // Constructor
     TasmotaClient(HardwareSerial *device = nullptr);
     // Sends configured features back to Tasmota so it knows which callbacks are supported
     void sendFeatures(void);
     // Send JSON back to Tasmota device
     void sendJSON(char *json);
     // Configure a callback for FUNC_JSON
     void attach_FUNC_JSON(callbackFunc func = nullptr);
     // Configure a callback for FUNC_EVERY_SECOND
     void attach_FUNC_EVERY_SECOND(callbackFunc func = nullptr);
     // Configure a callback for FUNC_EVERY_100_MSECOND
     void attach_FUNC_EVERY_100_MSECOND(callbackFunc func = nullptr);
     // Configure a callback for FUNC_COMMAND_SEND (ClientSend on Tasmota device)
     void attach_FUNC_COMMAND_SEND(callbackFunc1 func = nullptr);
     // Send command to Tasmota device using the prescribed protocol
     void SendCommand(uint8_t cmnd, uint8_t param);
     // Send telemetry data back to the Tasmota device
     void SendTele(char *data);
     // Used internally (should probably be moved to private:)
     uint8_t waitforbytes(uint16_t num, uint16_t timeout);     
     // Used internally to process incoming ClientSend command
     void ProcessSend(uint8_t sz);
     // Used internally to decode and process incoming commands from the Tasmota device
     void ProcessCommand(void);
     // Main client loop which needs to be serviced occasionally to process incoming requests
     void ExecuteCommand(char *cmnd);
     void loop(void);
    private:
     HardwareSerial *serial;
     callbackFunc FUNC_JSON;
     callbackFunc FUNC_EVERY_SECOND;
     callbackFunc FUNC_EVERY_100_MSECOND;
     callbackFunc1 FUNC_SEND;
};

#endif // __TASMOTACLIENT_H__
