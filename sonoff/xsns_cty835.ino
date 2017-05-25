/*
  Copyright (c) 2016 Theo Arends.  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_CTY835
/*********************************************************************************************\
    Laundry monitor system for Candy CTY 835 washing machine

    Source: Victor Ferrer https://github.com/vicfergar/Sonoff-MQTT-OTA-Arduino
  \*********************************************************************************************/

#include <Servo.h>

#define LDY_SERVO_PIN 14 // Pin where the switch servo is connected

#define LDY_SERVO_OFF 180
#define LDY_SERVO_ON  0
#define LDY_MAX_WASH_TIME (75 * SECS_PER_MIN) // 75min Synthetic 30º
#define LDY_BUFFER_SIZE 10

typedef enum
{
  SHUTTED_OFF = 0,
  READY_TO_START,
  PROGRAMME_INIT,
  PROGRAMME_STARTED,
  WASHING,
  PROGRAMME_PAUSED,
  PROGRAMME_ENDED,
} LDY_WashStates;

typedef enum
{
  LDY_UNDEFINED = 0,
  LDY_LED_OFF = 1,
  LDY_LED_BLINK = 2,
  LDY_LED_ON = 4,
} LDY_LedsStates;

typedef enum
{
  LDY_START = 0,  // Remaining time above 60 minutes
  LDY_MIN_60,     // Remaining time less than 60 minutes
  LDY_MIN_30,     // Remaining time less than 30 minutes
  LDY_MIN_15,     // Remaining time less than 15 minutes
  LDY_STOP,       // End of the programme

  LDY_LAST
} LDY_LedsTypes;

int ldy_leds_pinout[] = { 5, 4, 12, 13, 15 };

LDY_WashStates ldy_wash_state = SHUTTED_OFF;
int ldy_one_sec = 0;
int ldy_remaining_sec = -1;
LDY_LedsStates ldy_leds_state[LDY_LedsTypes::LDY_LAST];

bool ldy_sensor_readings[LDY_LedsTypes::LDY_LAST][LDY_BUFFER_SIZE];
int ldy_sensor_totals[LDY_LedsTypes::LDY_LAST];
int ldy_sensor_read_index = 0;

Servo ldy_servo_switch;
Ticker ldy_ticker;

void ldy_init()
{
  for (int i = 0; i < LDY_LedsTypes::LDY_LAST; i++) {
    pinMode(ldy_leds_pinout[i], INPUT);
  }

  ldy_ticker.attach_ms(100, ld_100ms);
  ldy_servo_switch.attach(LDY_SERVO_PIN);
  ldy_servo_switch.write(LDY_SERVO_OFF);
}

void ld_100ms()
{
  ldy_update_leds_state();

  if (ldy_one_sec++ == LDY_BUFFER_SIZE) {
    ldy_one_sec = 0;

    auto prevState = ldy_wash_state;

    ldy_refresh_wash_state();

    if (ldy_wash_state == PROGRAMME_INIT) {
      ldy_servo_switch.write(LDY_SERVO_ON);
      ldy_wash_state = PROGRAMME_STARTED;
    }
    else if (ldy_wash_state == PROGRAMME_STARTED) {
      ldy_servo_switch.write(LDY_SERVO_OFF);
      ldy_wash_state = READY_TO_START;
    }
    else if (ldy_wash_state == WASHING) {
      ldy_update_remaining_wash_time();
    }
    else if (ldy_wash_state == PROGRAMME_ENDED) {
      ldy_remaining_sec = 0;
    }
    else {
      ldy_servo_switch.write(LDY_SERVO_OFF);
      ldy_remaining_sec = -1;
    }

    if (prevState != ldy_wash_state) {
      ldy_mqttPresent();
    }
  }
}

void ldy_update_leds_state()
{
  for (byte i = 0; i < LDY_LedsTypes::LDY_LAST ; i++) {
    ldy_sensor_totals[i] -= ldy_sensor_readings[i][ldy_sensor_read_index];
    ldy_sensor_readings[i][ldy_sensor_read_index] = digitalRead(ldy_leds_pinout[i]);
    ldy_sensor_totals[i] += ldy_sensor_readings[i][ldy_sensor_read_index];

    if (ldy_sensor_totals[i] < 2) {
      ldy_leds_state[i] = LDY_LED_OFF;
    }
    else if (ldy_sensor_totals[i] < 6) {
      ldy_leds_state[i] = LDY_LED_BLINK;
    }
    else {
      ldy_leds_state[i] = LDY_LED_ON;
    }
  }

  ldy_sensor_read_index = (ldy_sensor_read_index + 1) % LDY_BUFFER_SIZE;
}

/*
   LDR_LDY_STOP                   // Blink (READY_TO_START) On (PROGRAMME_ENDED)
   LDR_LDY_START .. LDR_15MIN     // Blink (PROGRAMME_PAUSED) On (WASHING)
   EVERYTHING                     // Off (SHUTTED_OFF)
*/
void ldy_refresh_wash_state()
{
  if (ldy_leds_state[LDY_LedsTypes::LDY_STOP] == LDY_LED_BLINK) {
    if (ldy_wash_state != PROGRAMME_INIT && ldy_wash_state != PROGRAMME_STARTED) {
      ldy_wash_state = READY_TO_START;
    }
  }
  else if (ldy_leds_state[LDY_LedsTypes::LDY_STOP] == LDY_LED_ON) {
    ldy_wash_state = PROGRAMME_ENDED;
  }
  else {
    // LDY_STOP is OFF
    byte orState = 0;
    for (int i = 0; i < LDY_LedsTypes::LDY_STOP; i++) {
      orState |= ldy_leds_state[i];
    }

    if ((orState & LDY_LED_ON) == LDY_LED_ON) {
      // Any is on
      ldy_wash_state = WASHING;
    }
    else if ((orState & LDY_LED_BLINK) == LDY_LED_BLINK) {
      // Any is blinking
      ldy_wash_state = PROGRAMME_PAUSED;
    }
    else {
      ldy_wash_state = SHUTTED_OFF;
    }
  }
}

void ldy_update_remaining_wash_time()
{
  if (ldy_remaining_sec > 0) {
    ldy_remaining_sec--;
  }
  else if (ldy_remaining_sec < 0) {
    ldy_remaining_sec = LDY_MAX_WASH_TIME;
  }

  if (ldy_leds_state[LDY_LedsTypes::LDY_START] == LDY_LED_ON) {
    ldy_remaining_sec = _min(ldy_remaining_sec, LDY_MAX_WASH_TIME);
  }
  else if (ldy_leds_state[LDY_LedsTypes::LDY_MIN_60] == LDY_LED_ON) {
    ldy_remaining_sec = _min(ldy_remaining_sec, SECS_PER_MIN * 60);
  }
  else if (ldy_leds_state[LDY_LedsTypes::LDY_MIN_30] == LDY_LED_ON) {
    ldy_remaining_sec = _min(ldy_remaining_sec, SECS_PER_MIN * 30);
  }
  else if (ldy_leds_state[LDY_LedsTypes::LDY_MIN_15] == LDY_LED_ON) {
    ldy_remaining_sec = _min(ldy_remaining_sec, SECS_PER_MIN * 15);
  }
}

bool ldy_command(char *type, uint16_t index, char *dataBufUc, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  bool serviced = true;

  if (!strcmp_P(type, PSTR("WASHSTART"))) {
    if (ldy_wash_state == READY_TO_START) {
      ldy_wash_state = PROGRAMME_INIT;
      snprintf_P(svalue, ssvalue, PSTR("{\"WashStart\":\"Started\"}"));
    }
    else {
      snprintf_P(svalue, ssvalue, PSTR("{\"WashStart\":\"Not ready\"}"));
    }
  }
  else {
    serviced = false;  // Unknown command
  }

  return serviced;
}

void ldy_mqttPresent()
{
  // {"Time":"2017-03-04T13:37:24", "State":"SHUTTED_OFF", "RemainingSec":352}
  char svalue[60];

  char statusStr[20];
  switch (ldy_wash_state)
  {
    case SHUTTED_OFF: snprintf(statusStr, 20, "SHUTTED OFF");
      break;
    case READY_TO_START: snprintf(statusStr, 20, "READY TO START");
      break;
    case PROGRAMME_INIT: snprintf(statusStr, 20, "PROGRAMME INIT");
      break;
    case PROGRAMME_STARTED: snprintf(statusStr, 20, "PROGRAMME STARTED");
      break;
    case WASHING: snprintf(statusStr, 20, "WASHING");
      break;
    case PROGRAMME_PAUSED: snprintf(statusStr, 20, "PROGRAMME_PAUSED");
      break;
    case PROGRAMME_ENDED: snprintf(statusStr, 20, "PROGRAMME ENDED");
      break;
  }

  snprintf_P(svalue, sizeof(svalue), PSTR("{\"State\":\"%s\", \"RemainingSec\":\"%d\"}"), statusStr, ldy_remaining_sec);
  mqtt_publish_topic_P(1, PSTR("LAUNDRY"), svalue);
}
#endif  // USE_CTY835
