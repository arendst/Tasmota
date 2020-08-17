/*
Autogain tests for Tsl2561Util namespace.

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

#include <Tsl2561Util.h>

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
uint8_t id;

void setup() {
  Serial.begin(115200);
  Wire.begin(TSL2561_SDA, TSL2561_SCL);
  while( !Tsl.begin() )
    ; // wait until chip detected or wdt reset
  Serial.println("\nStarting Tsl2561Util autogain loop");
  Tsl.on();
  Tsl.id(id);
}

void loop() {
  uint16_t scaledFull = 0xffff, scaledIr = 0xffff;
  uint32_t full = 0xffffffff, ir = 0xffffffff, milliLux = 0xffffffff;
  bool gain = false;
  Tsl2561::exposure_t exposure = Tsl2561::EXP_OFF;

  if( Tsl2561Util::autoGain(Tsl, gain, exposure, scaledFull, scaledIr) ) {
    if( Tsl2561Util::normalizedLuminosity(gain, exposure, full = scaledFull, ir = scaledIr) ) {
      if( Tsl2561Util::milliLux(full, ir, milliLux, Tsl2561::packageCS(id), 5) ) {
        Serial.print(format("Tsl2561 addr: 0x%02x, id: 0x%02x, sfull: %5u, sir: %5u, full: %7lu, ir: %7lu, gain: %d, exp: %d, lux: %5lu.%03lu\n",
          Tsl.address(), id, scaledFull, scaledIr, (unsigned long)full, (unsigned long)ir, gain, exposure, (unsigned long)milliLux/1000, (unsigned long)milliLux%1000));
      }
      else {
        Serial.print(format("Tsl2561Util::milliLux(full=%lu, ir=%lu) error\n", (unsigned long)full, (unsigned long)ir));
      }
    }
    else {
      Serial.print(format("Tsl2561Util::normalizedLuminosity(gain=%u, exposure=%u, sfull=%u, sir=%u, full=%lu, ir=%lu) error\n",
        gain, exposure, scaledFull, scaledIr, (unsigned long)full, (unsigned long)ir));
    }
  }
  else {
    Serial.print(format("Tsl2561Util::autoGain(gain=%u, exposure=%u, sfull=%u, sir=%u) error\n",
      gain, exposure, scaledFull, scaledIr));
  }

  delay(1000);
}
