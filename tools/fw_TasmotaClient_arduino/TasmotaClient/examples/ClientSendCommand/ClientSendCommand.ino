/*
  ClientSendCommand.ino - Example for TasmotaClient to respond to ClientSend ON and
                         ClientSend OFF commands via console or telemetry.
                         In this example the ON and OFF is case sensitive so needs
                         to be sent in capital letters from the Tasmota device.
                         Upon receiving ON/OFF the client will turn the LED on/off
                         respectively.
                         
                         A second function callback is also attached which will
                         respond with a JSON message on Tasmota Teleperiod when
                         requested from the Tasmota device.
                         
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

TasmotaClient client(&Serial);

/*******************************************************************\
 * Normal setup() function for Arduino to configure the serial port
 * speed (which should match what was configured in Tasmota) and
 * attach any callback functions associated with specific requests
 * or commands that are sent by Tasmota.
\*******************************************************************/

void setup() {
  // Configure the LED pin as OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
  // Configure the serial port baud rate
  Serial.begin(57600);
  // Attach the function which will respond to the JSON request from Tasmota
  client.attach_FUNC_JSON(user_FUNC_JSON);
  // Attach the function which will be called when the Tasmota device sends data using ClientSend command
  client.attach_FUNC_COMMAND_SEND(user_FUNC_RECEIVE);
}

/*******************************************************************\
 * Function which will be called when this client device receives a
 * ClientSend command from the Tasmota device.
\*******************************************************************/

void user_FUNC_RECEIVE(char *data)
{
  if (!strcasecmp(data, "ON")) { // ClientSend ON
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (!strcasecmp(data, "OFF")) { // ClientSend OFF
    digitalWrite(LED_BUILTIN, LOW);
  }
}

/*******************************************************************\
 * user_FUNC_JSON creates the JSON which will be sent back to the
 * Tasmota device upon receiving a request to do so
\*******************************************************************/

void user_FUNC_JSON(void)
{
  uint8_t a = 0;
  char myjson[100];
  sprintf(myjson,"{\"A0\":%u,\"A1\":%u,\"A2\":%u,\"A3\":%u,\"A4\":%u,\"A5\":%u,\"A6\":%u,\"A7\":%u}", analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5), analogRead(A6), analogRead(A7));
  client.sendJSON(myjson);
}

void loop() {
  // Call the client loop every so often to process any incoming requests
  client.loop();
}
