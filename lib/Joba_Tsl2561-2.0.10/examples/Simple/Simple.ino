/*
Simple tests for Tsl2561 class. No error checking is done.

Copyright: Joachim Banzhaf, 2018

This file is part of the Joba_Tsl2561 Library.

  Joba_Tsl2561 is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Joba_Tsl2561 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Joba_Tsl2561.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Tsl2561.h>

// to mimic Serial.printf() of esp cores for other platforms
char *format( const char *fmt, ... ) {
  static char buf[128];
  va_list arg;
  va_start(arg, fmt);
  vsnprintf(buf, sizeof(buf), fmt, arg);
  buf[sizeof(buf)-1] = '\0';
  va_end(arg);
  return buf;
}

Tsl2561 Tsl(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin(TSL2561_SDA, TSL2561_SCL);
  Serial.println("\nStarting Tsl2561 simple loop");
}

void loop() {
  Tsl.begin();
  if( Tsl.available() ) {
    Tsl.on();

    Tsl.setSensitivity(true, Tsl2561::EXP_14);
    delay(16);

    uint8_t id;
    uint16_t full, ir;

    Tsl.id(id);
    Tsl.fullLuminosity(full);
    Tsl.irLuminosity(ir);

    Serial.print(format("Tsl2561 at 0x%02x(id=0x%02x) luminosity is %5u (full) and %5u (ir)\n", Tsl.address(), id, full, ir));

    Tsl.off();
  }
  else {
    Serial.print(format("No Tsl2561 found. Check wiring: SCL=%u, SDA=%u\n", TSL2561_SCL, TSL2561_SDA));
  }

  delay(5000);
}
