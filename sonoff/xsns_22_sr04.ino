/*
  xsns_22_sr04.ino - SR04 ultrasonic sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Nuno Ferreira and Theo Arends

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

#ifdef USE_SR04
/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/

uint8_t sr04_echo_pin = 0;
uint8_t sr04_trig_pin = 0;

/*********************************************************************************************\
 * Embedded stripped and tuned NewPing library from Tim Eckel - teckel@leethost.com
 * https://bitbucket.org/teckel12/arduino-new-ping
\*********************************************************************************************/
#define US_ROUNDTRIP_CM       58      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space. Default: 58
#define US_ROUNDTRIP_IN       148     // Microseconds (uS) it takes sound to travel round-trip 1 inch (2 inches total), uses integer to save compiled code space. Default: 148
#define PING_MEDIAN_DELAY     29000
#define MAX_SENSOR_DISTANCE   500
#define PING_OVERHEAD         5

// Conversion from uS to distance (round result to nearest cm or inch).
#define EchoConvert(echoTime, conversionFactor) (tmax(((unsigned int)echoTime + conversionFactor / 2) / conversionFactor, (echoTime ? 1 : 0)))

/********************************************************************************************/

void Sr04Init()
{
  sr04_echo_pin = pin[GPIO_SR04_ECHO];
  sr04_trig_pin = pin[GPIO_SR04_TRIG];
  pinMode(sr04_trig_pin, OUTPUT);
  pinMode(sr04_echo_pin, INPUT_PULLUP);
}

boolean Sr04Read(uint16_t *distance)
{
  uint16_t duration = 0;

  *distance = 0;

  /* Send ping and get delay */
  duration = Sr04GetSamples(9, 250);

  /* Calculate the distance (in cm) based on the speed of sound. */
  *distance = EchoConvert(duration, US_ROUNDTRIP_CM);

  return 1;
}

uint16_t Sr04Ping(uint16_t max_cm_distance)
{
  uint16_t duration = 0;
  uint16_t maxEchoTime;

  maxEchoTime = tmin(max_cm_distance + 1, (uint16_t) MAX_SENSOR_DISTANCE + 1) * US_ROUNDTRIP_CM;

  /* The following trigPin/echoPin cycle is used to determine the
     distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(sr04_trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(sr04_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sr04_trig_pin, LOW);

  /* Wait for the echo */
  duration = pulseIn(sr04_echo_pin, HIGH, 26000) - PING_OVERHEAD;

  return (duration > maxEchoTime) ? 0 : duration;
}

uint16_t Sr04GetSamples(uint8_t it, uint16_t max_cm_distance)
{
  uint16_t uS[it];
  uint16_t last;
  uint8_t j;
  uint8_t i = 0;
  uint16_t t;
  uS[0] = 0;

  while (i < it) {
    t = micros();
    last = Sr04Ping(max_cm_distance);

    if (last != 0) {
      if (i > 0) {
        for (j = i; j > 0 && uS[j - 1] < last; j--) {
          uS[j] = uS[j - 1];
        }
      } else {
        j = 0;
      }
      uS[j] = last;
      i++;
    } else {
      it--;
    }
    if (i < it && micros() - t < PING_MEDIAN_DELAY) {
      delay((PING_MEDIAN_DELAY + t - micros()) / 1000);
    }
  }

  return (uS[1]); // Return the ping distance from the 2nd highest reading
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_DISTANCE[] PROGMEM =
  "%s{s}SR04 " D_DISTANCE "{m}%d" D_UNIT_CENTIMETER "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Sr04Show(boolean json)
{
  uint16_t distance;

  if (Sr04Read(&distance)) {                // Check if read failed
    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SR04\":{\"" D_JSON_DISTANCE "\":%d}"), mqtt_data, distance);
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_DISTANCE, mqtt_data, distance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_22

boolean Xsns22(byte function)
{
  boolean result = false;

  if ((pin[GPIO_SR04_ECHO] < 99) && (pin[GPIO_SR04_TRIG] < 99)) {
    switch (function) {
      case FUNC_INIT:
        Sr04Init();
        break;
      case FUNC_JSON_APPEND:
        Sr04Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sr04Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SR04
