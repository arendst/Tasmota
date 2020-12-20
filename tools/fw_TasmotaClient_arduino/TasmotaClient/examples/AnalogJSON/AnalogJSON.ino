/*
  AnalogJSON.ino - Example on how to send a JSON back to a Tasmota device
                   which requested it on teleperiod.

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

/*******************************************************************\
 * Normal setup() function for Arduino to configure the serial port
 * speed (which should match what was configured in Tasmota, and
 * attach the function which will be called when the Tasmota device
 * reuqests a new JSON (usually on Teleperiod on Tasmota device)
 \*******************************************************************/

void setup() {
  // Configure the serial port speed
  Serial.begin(57600);
  // Attach the callback function which will provide the JSON to Tasmota
  client.attach_FUNC_JSON(user_FUNC_JSON);
}

void loop() {
  // Call the client loop function every so often to process incoming requests
  client.loop();
}
