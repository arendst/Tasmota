/*
  xsns_chirp.ino - CHIRP ambient light sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))

/*********************************************************************************************\
 * HS-SR04 - Ultrasonic distance sensor
\*********************************************************************************************/
void sr04_init()
{
  pinMode(pin[GPIO_SEN_TRIG], OUTPUT);
  pinMode(pin[GPIO_SEN_ECHO], INPUT);
  sr04_flg = 1;
  snprintf_P(log_data, sizeof(log_data), PSTR("HS-SR04: Device configured: Trig: %d Echo: %d"),pin[GPIO_SEN_TRIG],pin[GPIO_SEN_ECHO]);
  addLog(LOG_LEVEL_DEBUG);
}

String sr04_readDistance(void)
{
  bool reading_valid = false;

  if (sr04_flg) {
    char stemp1[6];
    String page = "";
    snprintf_P(log_data, sizeof(log_data), PSTR("HS-SR04: Start measurement: Trig: %d Echo: %d"),pin[GPIO_SEN_TRIG],pin[GPIO_SEN_ECHO]);
    addLog(LOG_LEVEL_DEBUG);
    int  duration = 0, counter = 0, max = 0;
    float distance = 0;
    while ( counter < 10) {
      digitalWrite(pin[GPIO_SEN_TRIG], LOW);  // Added this line
      delayMicroseconds(3); // Added this line
      noInterrupts();
      digitalWrite(pin[GPIO_SEN_TRIG], HIGH);
      //  delayMicroseconds(1000); - Removed this line
      delayMicroseconds(10); // Added this line
      digitalWrite(pin[GPIO_SEN_TRIG], LOW);
      duration = pulseIn(pin[GPIO_SEN_ECHO], HIGH);
      interrupts();
      yield();
      max = max(duration,max);
      dtostrfi(duration, 2, stemp1);
      snprintf_P(log_data, sizeof(log_data), PSTR("HS-SR04 In Duration: %s"),stemp1);
      addLog(LOG_LEVEL_ALL);
      counter++;
    }

    distance = (max/2) / 29.1;

    dtostrfd(distance, 2, stemp1);
    snprintf_P(log_data, sizeof(log_data), PSTR("HS-SR04: Max Distance: %s"),stemp1);
    addLog(LOG_LEVEL_DEBUG);

    if (distance > 0 && distance < 200) {
      page += stemp1;
    }
    return page;

  } else {
    return "-1";
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void sr04_mqttPresent(uint8_t* djson)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, \"Distance\":%s"), mqtt_data, sr04_readDistance().c_str());
  *djson = 1;
}

#ifdef USE_WEBSERVER
const char HTTP_HSSR04_DISTANCE[] PROGMEM =
  "<tr><th>Distance</th><td>%s cm</td></tr>";


String sr04_webPresent()
{
  String page = "";
  char sensor[80];
  snprintf_P(sensor, sizeof(sensor), HTTP_HSSR04_DISTANCE, sr04_readDistance().c_str());
  page += sensor;
  return page;
}
#endif  // USE_WEBSERVER
