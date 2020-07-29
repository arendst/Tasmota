/*
Tests for the Tsl2561 class.
It shows how to use every available method.

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

void showError( Tsl2561 &tsl ) {
  Tsl2561::status_t status = tsl.status();
  Serial.print(format("Error was %u: ", status));
  switch( status ) {
    case Tsl2561::ERR_OK: Serial.println("None"); break;
    case Tsl2561::ERR_RW: Serial.println("Read/Write"); break;
    case Tsl2561::ERR_BUSY: Serial.println("Busy"); break;
    case Tsl2561::ERR_GONE: Serial.println("Gone"); break;
    case Tsl2561::ERR_GENERAL: Serial.println("General"); break;
    default: Serial.println("Unknown"); break;
  }
}

void testSensitivity( Tsl2561 &tsl, bool newGain, Tsl2561::exposure_t newExp ) {
  if( tsl.on() ) {
    uint32_t start = millis();
    Serial.print(format("Chip powered on at %lu\n", (unsigned long)start));

    bool chipGain;
    Tsl2561::exposure_t chipExp;
    bool change = true;
    if( tsl.getSensitivity(chipGain, chipExp) ) {
      if( chipGain == newGain && chipExp == newExp ) {
        change = false;
      }
    }
    else {
      Serial.print("getSensitivity failed. ");
      showError(tsl);
    }

    bool check = true;
    if( change ) {
      if( tsl.setSensitivity(newGain, newExp) ) {
        Serial.print(format("New gain = %d, exposure = 0x%02x\n", newGain, newExp));
      }
      else {
        check = false;
        Serial.print("setSensitivity failed. ");
        showError(tsl);
      }
    }

    if( check ) {
      uint16_t ir, full = 0;

      while( !full && millis() - start < 1000 ) {
        if( !tsl.fullLuminosity(full) ) {
          Serial.print("Check full luminosity failed. ");
          showError(tsl);
        }
        if( full ) {
          if( !tsl.irLuminosity(ir) ) {
            Serial.print("Check ir luminosity failed. ");
            showError(tsl);
          }
        }
        else {
          delay(10);
        }
      }

      if( !full ) {
        Serial.println("No luminosity reading after 1s. Too dark?");
      }
      else {
        Serial.print(format("Got luminosity after %lu ms. Full spectrum is %u and IR only is %u\n", (unsigned long)millis() - start, full, ir));
      }
    }

    if( !tsl.off() ) {
      Serial.print("Power off failed. ");
      showError(tsl);
    }
  }
  else {
    Serial.print("Power on failed. ");
    showError(tsl);
  }
}

bool testPackage( Tsl2561 &tsl ) {
  uint8_t id;
  if( tsl.id(id) ) {
    Serial.print(format("Chip has type %02x and revision %x\n", Tsl2561::type(id), Tsl2561::revision(id)));
    if( Tsl2561::packageT_FN_CL(id) ) {
      Serial.println("Chip is a T, FN or CL type package");
    }
    else if( Tsl2561::packageCS(id) ) {
      Serial.println("Chip is a CS type package");
    }
    else {
      Serial.println("Chip is an unknown package");
    }
    return true;
  }
  else {
    Serial.print("Get Chip ID failed. ");
    showError(tsl);
  }
  return false;
}

void test( Tsl2561 &tsl ) {
  bool ok = tsl.available();
  Serial.print(format("\nTesting Tsl2561 at address %02x: %sfound\n", tsl.address(), ok ? "" : "NOT "));
  if( ok ) {
    if( testPackage(tsl) ) {
      testSensitivity(tsl, Tsl2561::GAIN_OFF, Tsl2561::EXP_402);
      testSensitivity(tsl, Tsl2561::GAIN_ON,  Tsl2561::EXP_402);
      testSensitivity(tsl, Tsl2561::GAIN_OFF, Tsl2561::EXP_101);
      testSensitivity(tsl, Tsl2561::GAIN_ON,  Tsl2561::EXP_101);
      testSensitivity(tsl, Tsl2561::GAIN_OFF, Tsl2561::EXP_14);
      testSensitivity(tsl, Tsl2561::GAIN_ON,  Tsl2561::EXP_14);
    }
  }
  else {
    showError(tsl);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(TSL2561_SDA, TSL2561_SCL);
  Serial.println("\nStarting Tsl2561 testing loop");
}

void loop() {
  Tsl.begin(Tsl2561::ADDR_GND);
  test(Tsl);
  Tsl.begin(Tsl2561::ADDR_FLOAT);
  test(Tsl);
  Tsl.begin(Tsl2561::ADDR_VDD);
  test(Tsl);
  Serial.println("\nNext test in 5s\n");
  delay(5000);
}
