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

Tsl2561 Tsl(Wire);
uint8_t id;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while( !Tsl.begin() )
    ; // wait until chip detected or wdt reset
  Serial.println("\nStarting Tsl2561Util autogain loop");
  Tsl.on();
  Tsl.id(id);
}

void loop() {
  uint16_t scaledFull = ~0, scaledIr = ~0;
  uint32_t full = ~0, ir = ~0, milliLux = ~0;
  bool gain = false;
  Tsl2561::exposure_t exposure = Tsl2561::EXP_OFF;

  if( Tsl2561Util::autoGain(Tsl, gain, exposure, scaledFull, scaledIr) ) {
    if( Tsl2561Util::normalizedLuminosity(gain, exposure, full = scaledFull, ir = scaledIr) ) {
      if( Tsl2561Util::milliLux(full, ir, milliLux, Tsl2561::packageCS(id)) ) {
        Serial.printf("Tsl2561 addr: 0x%02x, id: 0x%02x, sfull: %5u, sir: %5u, full: %5u, ir: %5u, gain: %d, exp: %d, lux: %5u.%03u\n", 
          Tsl.address(), id, scaledFull, scaledIr, full, ir, gain, exposure, milliLux/1000, milliLux%1000);
      }
      else {
        Serial.printf("Tsl2561Util::milliLux(full=%u, ir=%u) error\n", full, ir);
      }
    }
    else {
      Serial.printf("Tsl2561Util::normalizedLuminosity(gain=%u, exposure=%u, sfull=%u, sir=%u, full=%u, ir=%u) error\n", 
        gain, exposure, scaledFull, scaledIr, full, ir);
    }
  }
  else {
    Serial.printf("Tsl2561Util::autoGain(gain=%u, exposure=%u, sfull=%u, sir=%u) error\n", 
      gain, exposure, scaledFull, scaledIr);
  }

  delay(1000);
}

