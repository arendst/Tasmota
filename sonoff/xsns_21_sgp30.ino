/*
  sgp30 sensor interface
  adapted for Tasmota by Gerhard Mutz

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

#ifdef USE_I2C
#ifdef USE_SGP30

#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
uint8_t SGP30_type = 0;

const char SGP30_JSON[] PROGMEM = "%s,\"%s\":{\"" "TVOC" "\":%d,\"" "eCO2" "\":%d }";

const char SGP30_WEB[] PROGMEM = "%s"
  "{s}SGP30 " "TVOC: " "{m}%d " "ppb" "{e}"
  "{s}SGP30 " "eCO2: " "{m}%d " "ppm" "{e}";

void SGP30_Show(boolean json) {
  if (!SGP30_type) return;

  if (!sgp.IAQmeasure()) {
    //Serial.println("SGP30 Measurement failed");
    return;
  }

  //Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  //Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), SGP30_JSON, mqtt_data, "SGP30", sgp.TVOC,sgp.eCO2);
  } else {
#ifdef USE_WEBSERVER
    // web interface
    snprintf_P(mqtt_data, sizeof(mqtt_data), SGP30_WEB, mqtt_data,sgp.TVOC, sgp.eCO2);
#endif
  }
}

void SGP30_Detect(void) {
  if (SGP30_type) return;
  if (!sgp.begin(&Wire)){
    //Serial.println("Sensor not found :(");
    return;
  }
  SGP30_type=1;
}

void SGP30_Init(void) {
uint16_t count;
  if (!SGP30_type) return;
/*
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
*/

  for (count=0; count<30; count++) {
    if (!sgp.IAQmeasure()) {
      //Serial.println("Measurement failed");
      return;
    }
  }

  uint16_t TVOC_base, eCO2_base;
  if (!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
    //Serial.println("Failed to get baseline readings");
    return;
  }
  //Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
  //Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_93

boolean Xsns93(byte function) {
  boolean result = false;
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        SGP30_Detect();
        break;
      case FUNC_INIT:
        SGP30_Init();
        break;
      case FUNC_JSON_APPEND:
        SGP30_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SGP30_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  return result;
}

#endif  // USE_SGP30
#endif
