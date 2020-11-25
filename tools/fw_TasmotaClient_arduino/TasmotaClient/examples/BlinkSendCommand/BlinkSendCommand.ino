/*
  BlinkRelay.ino - Example for TasmotaClient receiving the FUNC_EVERY_SECOND
                   callback and respond by toggling the LED as configured.
                   This example also extends sending commands back to the
                   Tasmota device by which could be any command normally
                   executed from the console but in this particular example
                   we just use a simple publish command.

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

bool ledstate = false;

/*******************************************************************\
 * Function which will be called when Tasmota sends a
 * FUNC_EVERY_SECOND command
\*******************************************************************/

void user_FUNC_EVERY_SECOND(void)
{
  if (ledstate) {
    ledstate = false;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    ledstate = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (ledstate) {
    client.ExecuteCommand((char*)"publish tele/mytopic/power on");
  } else {
    client.ExecuteCommand((char*)"publish tele/mytopic/power off");
  }
}

/*******************************************************************\
 * Normal setup() function for Arduino to configure the serial port
 * speed (which should match what was configured in Tasmota) and
 * attach any callback functions associated with specific requests
 * or commands that are sent by Tasmota.
\*******************************************************************/

void setup() {
  // Configure serial port
  Serial.begin(57600);
  // We're going to use the builtin LED so lets configure the pin as OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
  // Attach the callback function which will be called when Tasmota requests it
  client.attach_FUNC_EVERY_SECOND(user_FUNC_EVERY_SECOND);
}

void loop() {
  // Call the client loop function every so often to process incoming requests
  client.loop();
}
