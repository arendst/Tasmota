/*
  support_tasmota.ino - Core support for Tasmota

  Copyright (C) 2020  Theo Arends

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

const char kSleepMode[] PROGMEM = "Dynamic|Normal";
const char kPrefixes[] PROGMEM = D_CMND "|" D_STAT "|" D_TELE;

char* Format(char* output, const char* input, int size)
{
  char *token;
  uint32_t digits = 0;

  if (strstr(input, "%") != nullptr) {
    strlcpy(output, input, size);
    token = strtok(output, "%");
    if (strstr(input, "%") == input) {
      output[0] = '\0';
    } else {
      token = strtok(nullptr, "");
    }
    if (token != nullptr) {
      digits = atoi(token);
      if (digits) {
        char tmp[size];
        if (strchr(token, 'd')) {
          snprintf_P(tmp, size, PSTR("%s%c0%dd"), output, '%', digits);
          snprintf_P(output, size, tmp, ESP.getChipId() & 0x1fff);            // %04d - short chip ID in dec, like in hostname
        } else {
          snprintf_P(tmp, size, PSTR("%s%c0%dX"), output, '%', digits);
          snprintf_P(output, size, tmp, ESP.getChipId());                   // %06X - full chip ID in hex
        }
      } else {
        if (strchr(token, 'd')) {
          snprintf_P(output, size, PSTR("%s%d"), output, ESP.getChipId());  // %d - full chip ID in dec
          digits = 8;
        }
      }
    }
  }
  if (!digits) {
    strlcpy(output, input, size);
  }
  return output;
}

char* GetOtaUrl(char *otaurl, size_t otaurl_size)
{
  if (strstr(SettingsText(SET_OTAURL), "%04d") != nullptr) {     // OTA url contains placeholder for chip ID
    snprintf(otaurl, otaurl_size, SettingsText(SET_OTAURL), ESP.getChipId() & 0x1fff);
  }
  else if (strstr(SettingsText(SET_OTAURL), "%d") != nullptr) {  // OTA url contains placeholder for chip ID
    snprintf_P(otaurl, otaurl_size, SettingsText(SET_OTAURL), ESP.getChipId());
  }
  else {
    strlcpy(otaurl, SettingsText(SET_OTAURL), otaurl_size);
  }

  return otaurl;
}

char* GetTopic_P(char *stopic, uint32_t prefix, char *topic, const char* subtopic)
{
  /* prefix 0 = Cmnd
     prefix 1 = Stat
     prefix 2 = Tele
     prefix 4 = Cmnd fallback
     prefix 5 = Stat fallback
     prefix 6 = Tele fallback
     prefix 8 = Cmnd topic
     prefix 9 = Stat topic
     prefix 10 = Tele topic
  */
  char romram[CMDSZ];
  String fulltopic;

  snprintf_P(romram, sizeof(romram), subtopic);
  if (fallback_topic_flag || (prefix > 3)) {
    bool fallback = (prefix < 8);
    prefix &= 3;
    char stemp[11];
    fulltopic = GetTextIndexed(stemp, sizeof(stemp), prefix, kPrefixes);
    fulltopic += F("/");
    if (fallback) {
      fulltopic += mqtt_client;
      fulltopic += F("_fb");                  // cmnd/<mqttclient>_fb
    } else {
      fulltopic += topic;                     // cmnd/<grouptopic>
    }
  } else {
    fulltopic = SettingsText(SET_MQTT_FULLTOPIC);
    if ((0 == prefix) && (-1 == fulltopic.indexOf(FPSTR(MQTT_TOKEN_PREFIX)))) {
      fulltopic += F("/");
      fulltopic += FPSTR(MQTT_TOKEN_PREFIX);  // Need prefix for commands to handle mqtt topic loops
    }
    for (uint32_t i = 0; i < MAX_MQTT_PREFIXES; i++) {
      if (!strlen(SettingsText(SET_MQTTPREFIX1 + i))) {
        char temp[TOPSZ];
        SettingsUpdateText(SET_MQTTPREFIX1 + i, GetTextIndexed(temp, sizeof(temp), i, kPrefixes));
      }
    }
    fulltopic.replace(FPSTR(MQTT_TOKEN_PREFIX), SettingsText(SET_MQTTPREFIX1 + prefix));

    fulltopic.replace(FPSTR(MQTT_TOKEN_TOPIC), topic);
    fulltopic.replace(F("%hostname%"), my_hostname);
    String token_id = WiFi.macAddress();
    token_id.replace(":", "");
    fulltopic.replace(F("%id%"), token_id);
  }
  fulltopic.replace(F("#"), "");
  fulltopic.replace(F("//"), "/");
  if (!fulltopic.endsWith("/")) {
    fulltopic += "/";
  }
  snprintf_P(stopic, TOPSZ, PSTR("%s%s"), fulltopic.c_str(), romram);
  return stopic;
}

char* GetGroupTopic_P(char *stopic, const char* subtopic, int itopic)
{
  // SetOption75 0: %prefix%/nothing/%topic% = cmnd/nothing/<grouptopic>/#
  // SetOption75 1: cmnd/<grouptopic>
  return GetTopic_P(stopic, (Settings.flag3.grouptopic_mode) ? CMND +8 : CMND, SettingsText(itopic), subtopic);  // SetOption75 - GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
}

char* GetFallbackTopic_P(char *stopic, const char* subtopic)
{
  return GetTopic_P(stopic, CMND +4, nullptr, subtopic);
}

char* GetStateText(uint32_t state)
{
  if (state >= MAX_STATE_TEXT) {
    state = 1;
  }
  return SettingsText(SET_STATE_TXT1 + state);
}

/********************************************************************************************/

void SetLatchingRelay(power_t lpower, uint32_t state)
{
  // power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
  // power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
  // power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
  // power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On

  if (state && !latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
    latching_power = lpower;
    latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
  }

  for (uint32_t i = 0; i < devices_present; i++) {
    uint32_t port = (i << 1) + ((latching_power >> i) &1);
    DigitalWrite(GPIO_REL1 +port, bitRead(rel_inverted, port) ? !state : state);
  }
}

void SetDevicePower(power_t rpower, uint32_t source)
{
  ShowSource(source);
  last_source = source;

  if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {  // All on and stay on
    power = (1 << devices_present) -1;
    rpower = power;
  }

  if (Settings.flag.interlock) {          // Allow only one or no relay set - CMND_INTERLOCK - Enable/disable interlock
    for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
      power_t mask = 1;
      uint32_t count = 0;
      for (uint32_t j = 0; j < devices_present; j++) {
        if ((Settings.interlock[i] & mask) && (rpower & mask)) {
          count++;
        }
        mask <<= 1;
      }
      if (count > 1) {
        mask = ~Settings.interlock[i];    // Turn interlocked group off as there would be multiple relays on
        power &= mask;
        rpower &= mask;
      }
    }
  }

  if (rpower) {                           // Any power set
    last_power = rpower;
  }

  XdrvMailbox.index = rpower;
  XdrvCall(FUNC_SET_POWER);               // Signal power state

  XdrvMailbox.index = rpower;
  XdrvMailbox.payload = source;
  if (XdrvCall(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
    // Serviced
  }
  else if ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
    Serial.write(0xA0);
    Serial.write(0x04);
    Serial.write(rpower &0xFF);
    Serial.write(0xA1);
    Serial.write('\n');
    Serial.flush();
  }
  else if (EXS_RELAY == my_module_type) {
    SetLatchingRelay(rpower, 1);
  }
  else {
    for (uint32_t i = 0; i < devices_present; i++) {
      power_t state = rpower &1;
      if (i < MAX_RELAYS) {
        DigitalWrite(GPIO_REL1 +i, bitRead(rel_inverted, i) ? !state : state);
      }
      rpower >>= 1;
    }
  }
}

void RestorePower(bool publish_power, uint32_t source)
{
  if (power != last_power) {
    power = last_power;
    SetDevicePower(power, source);
    if (publish_power) {
      MqttPublishAllPowerState();
    }
  }
}

void SetAllPower(uint32_t state, uint32_t source)
{
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }
  if ((state >= POWER_OFF) && (state <= POWER_TOGGLE)) {
    power_t all_on = (1 << devices_present) -1;
    switch (state) {
    case POWER_OFF:
      power = 0;
      break;
    case POWER_ON:
      power = all_on;
      break;
    case POWER_TOGGLE:
      power ^= all_on;                    // Complement current state
    }
    SetDevicePower(power, source);
  }
  if (publish_power) {
    MqttPublishAllPowerState();
  }
}

void SetPowerOnState(void)
{
  if (MOTOR == my_module_type) {
    Settings.poweronstate = POWER_ALL_ON;   // Needs always on else in limbo!
  }
  if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
    SetDevicePower(1, SRC_RESTART);
  } else {
    if ((ResetReason() == REASON_DEFAULT_RST) || (ResetReason() == REASON_EXT_SYS_RST)) {
      switch (Settings.poweronstate) {
      case POWER_ALL_OFF:
      case POWER_ALL_OFF_PULSETIME_ON:
        power = 0;
        SetDevicePower(power, SRC_RESTART);
        break;
      case POWER_ALL_ON:  // All on
        power = (1 << devices_present) -1;
        SetDevicePower(power, SRC_RESTART);
        break;
      case POWER_ALL_SAVED_TOGGLE:
        power = (Settings.power & ((1 << devices_present) -1)) ^ POWER_MASK;
        if (Settings.flag.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(power, SRC_RESTART);
        }
        break;
      case POWER_ALL_SAVED:
        power = Settings.power & ((1 << devices_present) -1);
        if (Settings.flag.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(power, SRC_RESTART);
        }
        break;
      }
    } else {
      power = Settings.power & ((1 << devices_present) -1);
      if (Settings.flag.save_state) {    // SetOption0 - Save power state and use after restart
        SetDevicePower(power, SRC_RESTART);
      }
    }
  }

  // Issue #526 and #909
  for (uint32_t i = 0; i < devices_present; i++) {
    if (!Settings.flag3.no_power_feedback) {  // SetOption63 - Don't scan relay power state at restart - #5594 and #5663
      if ((i < MAX_RELAYS) && (pin[GPIO_REL1 +i] < 99)) {
        bitWrite(power, i, digitalRead(pin[GPIO_REL1 +i]) ^ bitRead(rel_inverted, i));
      }
    }
    if ((i < MAX_PULSETIMERS) && (bitRead(power, i) || (POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate))) {
      SetPulseTimer(i, Settings.pulse_timer[i]);
    }
  }
  blink_powersave = power;
}

void SetLedPowerIdx(uint32_t led, uint32_t state)
{
  if ((99 == pin[GPIO_LEDLNK]) && (0 == led)) {  // Legacy - LED1 is link led only if LED2 is present
    if (pin[GPIO_LED2] < 99) {
      led = 1;
    }
  }
  if (pin[GPIO_LED1 + led] < 99) {
    uint32_t mask = 1 << led;
    if (state) {
      state = 1;
      led_power |= mask;
    } else {
      led_power &= (0xFF ^ mask);
    }
    DigitalWrite(GPIO_LED1 + led, bitRead(led_inverted, led) ? !state : state);
  }
#ifdef USE_BUZZER
  if (led == 0) {
    BuzzerSetStateToLed(state);
  }
#endif // USE_BUZZER
}

void SetLedPower(uint32_t state)
{
  if (99 == pin[GPIO_LEDLNK]) {           // Legacy - Only use LED1 and/or LED2
    SetLedPowerIdx(0, state);
  } else {
    power_t mask = 1;
    for (uint32_t i = 0; i < leds_present; i++) {  // Map leds to power
      bool tstate = (power & mask);
      SetLedPowerIdx(i, tstate);
      mask <<= 1;
    }
  }
}

void SetLedPowerAll(uint32_t state)
{
  for (uint32_t i = 0; i < leds_present; i++) {
    SetLedPowerIdx(i, state);
  }
}

void SetLedLink(uint32_t state)
{
  uint32_t led_pin = pin[GPIO_LEDLNK];
  uint32_t led_inv = ledlnk_inverted;
  if (99 == led_pin) {                    // Legacy - LED1 is status
    led_pin = pin[GPIO_LED1];
    led_inv = bitRead(led_inverted, 0);
  }
  if (led_pin < 99) {
    if (state) { state = 1; }
    digitalWrite(led_pin, (led_inv) ? !state : state);
  }
#ifdef USE_BUZZER
  BuzzerSetStateToLed(state);
#endif // USE_BUZZER
}

void SetPulseTimer(uint32_t index, uint32_t time)
{
  pulse_timer[index] = (time > 111) ? millis() + (1000 * (time - 100)) : (time > 0) ? millis() + (100 * time) : 0L;
}

uint32_t GetPulseTimer(uint32_t index)
{
  long time = TimePassedSince(pulse_timer[index]);
  if (time < 0) {
    time *= -1;
    return (time > 11100) ? (time / 1000) + 100 : (time > 0) ? time / 100 : 0;
  }
  return 0;
}

/********************************************************************************************/

bool SendKey(uint32_t key, uint32_t device, uint32_t state)
{
// key 0 = KEY_BUTTON = button_topic
// key 1 = KEY_SWITCH = switch_topic
// state 0 = POWER_OFF = off
// state 1 = POWER_ON = on
// state 2 = POWER_TOGGLE = toggle
// state 3 = POWER_HOLD = hold
// state 9 = CLEAR_RETAIN = clear retain flag

  char stopic[TOPSZ];
  char scommand[CMDSZ];
  char key_topic[TOPSZ];
  bool result = false;
  uint32_t device_save = device;

  char *tmp = (key) ? SettingsText(SET_MQTT_SWITCH_TOPIC) : SettingsText(SET_MQTT_BUTTON_TOPIC);
  Format(key_topic, tmp, sizeof(key_topic));
  if (Settings.flag.mqtt_enabled && MqttIsConnected() && (strlen(key_topic) != 0) && strcmp(key_topic, "0")) {  // SetOption3 - Enable MQTT
    if (!key && (device > devices_present)) {
      device = 1;                  // Only allow number of buttons up to number of devices
    }
    GetTopic_P(stopic, CMND, key_topic,
               GetPowerDevice(scommand, device, sizeof(scommand), (key + Settings.flag.device_index_enable)));  // cmnd/switchtopic/POWERx - SetOption26 - Switch between POWER or POWER1
    if (CLEAR_RETAIN == state) {
      mqtt_data[0] = '\0';
    } else {
      if ((Settings.flag3.button_switch_force_local ||      // SetOption61 - Force local operation when button/switch topic is set
           !strcmp(mqtt_topic, key_topic) ||
           !strcmp(SettingsText(SET_MQTT_GRP_TOPIC), key_topic)) &&
          (POWER_TOGGLE == state)) {
        state = ~(power >> (device -1)) &1;                 // POWER_OFF or POWER_ON
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), GetStateText(state));
    }
#ifdef USE_DOMOTICZ
    if (!(DomoticzSendKey(key, device, state, strlen(mqtt_data)))) {
#endif  // USE_DOMOTICZ
      MqttPublish(stopic, ((key) ? Settings.flag.mqtt_switch_retain                         // CMND_SWITCHRETAIN
                                 : Settings.flag.mqtt_button_retain) &&                     // CMND_BUTTONRETAIN
                                 (state != POWER_HOLD || !Settings.flag3.no_hold_retain));  // SetOption62 - Don't use retain flag on HOLD messages
#ifdef USE_DOMOTICZ
    }
#endif  // USE_DOMOTICZ
    result = !Settings.flag3.button_switch_force_local;  // SetOption61 - Force local operation when button/switch topic is set
  } else {
    Response_P(PSTR("{\"%s%d\":{\"State\":%d}}"), (key) ? "Switch" : "Button", device, state);
    result = XdrvRulesProcess();
  }
  int32_t payload_save = XdrvMailbox.payload;
  XdrvMailbox.payload = device_save << 24 | key << 16 | state << 8 | device;
  XdrvCall(FUNC_ANY_KEY);
  XdrvMailbox.payload = payload_save;
  return result;
}

void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{
// device  = Relay number 1 and up
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 3 = POWER_BLINK = Blink relay
// state 4 = POWER_BLINK_STOP = Stop blinking relay
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

//  ShowSource(source);

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) {
    blink_mask &= 1;                 // No blinking on the fan relays
    Settings.flag.interlock = 0;     // No interlock mode as it is already done by the microcontroller - CMND_INTERLOCK - Enable/disable interlock
    Settings.pulse_timer[1] = 0;     // No pulsetimers on the fan relays
    Settings.pulse_timer[2] = 0;
    Settings.pulse_timer[3] = 0;
  }
#endif  // USE_SONOFF_IFAN

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                      // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  active_device = device;

  if (device <= MAX_PULSETIMERS) {
    SetPulseTimer(device -1, 0);
  }
  power_t mask = 1 << (device -1);        // Device to control
  if (state <= POWER_TOGGLE) {
    if ((blink_mask & mask)) {
      blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      MqttPublishPowerBlinkState(device);
    }

    if (Settings.flag.interlock &&        // CMND_INTERLOCK - Enable/disable interlock
        !interlock_mutex &&
        ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(power & mask)))
       ) {
      interlock_mutex = true;                           // Clear all but masked relay in interlock group if new set requested
      for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
        if (Settings.interlock[i] & mask) {             // Find interlock group
          for (uint32_t j = 0; j < devices_present; j++) {
            power_t imask = 1 << j;
            if ((Settings.interlock[i] & imask) && (power & imask) && (mask != imask)) {
              ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
              delay(50);                                // Add some delay to make sure never have more than one relay on
            }
          }
          break;                                        // An interlocked relay is only present in one group so quit
        }
      }
      interlock_mutex = false;
    }

    switch (state) {
    case POWER_OFF: {
      power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      power |= mask;
      break;
    case POWER_TOGGLE:
      power ^= mask;
    }
#ifdef USE_DEVICE_GROUPS
    if (SRC_REMOTE != source && SRC_RETRY != source) SendLocalDeviceGroupMessage(DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, power);
#endif  // USE_DEVICE_GROUPS
    SetDevicePower(power, source);
#ifdef USE_DOMOTICZ
    DomoticzUpdatePowerState(device);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    KnxUpdatePowerState(device, power);
#endif  // USE_KNX
    if (publish_power && Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
      MqttPublishTeleState();
    }
    if (device <= MAX_PULSETIMERS) {  // Restart PulseTime if powered On
      SetPulseTimer(device -1, (((POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? ~power : power) & mask) ? Settings.pulse_timer[device -1] : 0);
    }
  }
  else if (POWER_BLINK == state) {
    if (!(blink_mask & mask)) {
      blink_powersave = (blink_powersave & (POWER_MASK ^ mask)) | (power & mask);  // Save state
      blink_power = (power >> (device -1))&1;  // Prep to Toggle
    }
    blink_timer = millis() + 100;
    blink_counter = ((!Settings.blinkcount) ? 64000 : (Settings.blinkcount *2)) +1;
    blink_mask |= mask;  // Set device mask
    MqttPublishPowerBlinkState(device);
    return;
  }
  else if (POWER_BLINK_STOP == state) {
    bool flag = (blink_mask & mask);
    blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
    MqttPublishPowerBlinkState(device);
    if (flag) {
      ExecuteCommandPower(device, (blink_powersave >> (device -1))&1, SRC_IGNORE);  // Restore state
    }
    return;
  }
  if (publish_power) {
    MqttPublishPowerState(device);
  }
}

void StopAllPowerBlink(void)
{
  power_t mask;

  for (uint32_t i = 1; i <= devices_present; i++) {
    mask = 1 << (i -1);
    if (blink_mask & mask) {
      blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      MqttPublishPowerBlinkState(i);
      ExecuteCommandPower(i, (blink_powersave >> (i -1))&1, SRC_IGNORE);  // Restore state
    }
  }
}

void MqttShowPWMState(void)
{
  ResponseAppend_P(PSTR("\"" D_CMND_PWM "\":{"));
  bool first = true;
  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    if (pin[GPIO_PWM1 + i] < 99) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_PWM "%d\":%d"), first ? "" : ",", i+1, Settings.pwm_value[i]);
      first = false;
    }
  }
  ResponseJsonEnd();
}

void MqttShowState(void)
{
  char stemp1[TOPSZ];

  ResponseAppendTime();
  ResponseAppend_P(PSTR(",\"" D_JSON_UPTIME "\":\"%s\",\"UptimeSec\":%u"), GetUptime().c_str(), UpTime());

#ifdef USE_ADC_VCC
  dtostrfd((double)ESP.getVcc()/1000, 3, stemp1);
  ResponseAppend_P(PSTR(",\"" D_JSON_VCC "\":%s"), stemp1);
#endif

  ResponseAppend_P(PSTR(",\"" D_JSON_HEAPSIZE "\":%d,\"SleepMode\":\"%s\",\"Sleep\":%u,\"LoadAvg\":%u,\"MqttCount\":%u"),
    ESP.getFreeHeap()/1024, GetTextIndexed(stemp1, sizeof(stemp1), Settings.flag3.sleep_normal, kSleepMode),  // SetOption60 - Enable normal sleep instead of dynamic sleep
    sleep, loop_load_avg, MqttConnectCount());

  for (uint32_t i = 1; i <= devices_present; i++) {
#ifdef USE_LIGHT
    if ((LightDevice()) && (i >= LightDevice())) {
      if (i == LightDevice())  { LightState(1); }    // call it only once
    } else {
#endif
      ResponseAppend_P(PSTR(",\"%s\":\"%s\""), GetPowerDevice(stemp1, i, sizeof(stemp1), Settings.flag.device_index_enable),  // SetOption26 - Switch between POWER or POWER1
                                               GetStateText(bitRead(power, i-1)));
#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan()) {
        ResponseAppend_P(PSTR(",\"" D_CMND_FANSPEED "\":%d"), GetFanspeed());
        break;
      }
#endif  // USE_SONOFF_IFAN
#ifdef USE_LIGHT
    }
#endif
  }

  if (pwm_present) {
    ResponseAppend_P(PSTR(","));
    MqttShowPWMState();
  }

  int32_t rssi = WiFi.RSSI();
  ResponseAppend_P(PSTR(",\"" D_JSON_WIFI "\":{\"" D_JSON_AP "\":%d,\"" D_JSON_SSID "\":\"%s\",\"" D_JSON_BSSID "\":\"%s\",\"" D_JSON_CHANNEL "\":%d,\"" D_JSON_RSSI "\":%d,\"" D_JSON_SIGNAL "\":%d,\"" D_JSON_LINK_COUNT "\":%d,\"" D_JSON_DOWNTIME "\":\"%s\"}}"),
    Settings.sta_active +1, SettingsText(SET_STASSID1 + Settings.sta_active), WiFi.BSSIDstr().c_str(), WiFi.channel(),
    WifiGetRssiAsQuality(rssi), rssi, WifiLinkCount(), WifiDowntime().c_str());
}

void MqttPublishTeleState(void)
{
  mqtt_data[0] = '\0';
  MqttShowState();
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
#if defined(USE_RULES) || defined(USE_SCRIPT)
  RulesTeleperiod();  // Allow rule based HA messages
#endif  // USE_SCRIPT
}

void TempHumDewShow(bool json, bool pass_on, const char *types, float f_temperature, float f_humidity)
{
  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{"), types);
    ResponseAppendTHD(f_temperature, f_humidity);
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (pass_on) {
      DomoticzTempHumPressureSensor(f_temperature, f_humidity);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (pass_on) {
      KnxSensor(KNX_TEMPERATURE, f_temperature);
      KnxSensor(KNX_HUMIDITY, f_humidity);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_THD(types, f_temperature, f_humidity);
#endif  // USE_WEBSERVER
  }
}

bool MqttShowSensor(void)
{
  ResponseAppendTime();

  int json_data_start = strlen(mqtt_data);
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
#ifdef USE_TM1638
    if ((pin[GPIO_SWT1 +i] < 99) || ((pin[GPIO_TM16CLK] < 99) && (pin[GPIO_TM16DIO] < 99) && (pin[GPIO_TM16STB] < 99))) {
#else
    if (pin[GPIO_SWT1 +i] < 99) {
#endif  // USE_TM1638
      ResponseAppend_P(PSTR(",\"" D_JSON_SWITCH "%d\":\"%s\""), i +1, GetStateText(SwitchState(i)));
    }
  }
  XsnsCall(FUNC_JSON_APPEND);
  XdrvCall(FUNC_JSON_APPEND);

  bool json_data_available = (strlen(mqtt_data) - json_data_start);
  if (strstr_P(mqtt_data, PSTR(D_JSON_PRESSURE)) != nullptr) {
    ResponseAppend_P(PSTR(",\"" D_JSON_PRESSURE_UNIT "\":\"%s\""), PressureUnit().c_str());
  }
  if (strstr_P(mqtt_data, PSTR(D_JSON_TEMPERATURE)) != nullptr) {
    ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE_UNIT "\":\"%c\""), TempUnit());
  }
  if ((strstr_P(mqtt_data, PSTR(D_JSON_SPEED)) != nullptr) && Settings.flag2.speed_conversion) {
    ResponseAppend_P(PSTR(",\"" D_JSON_SPEED_UNIT "\":\"%s\""), SpeedUnit().c_str());
  }
  ResponseJsonEnd();

  if (json_data_available) { XdrvCall(FUNC_SHOW_SENSOR); }
  return json_data_available;
}

void MqttPublishSensor(void)
{
  mqtt_data[0] = '\0';
  if (MqttShowSensor()) {
    MqttPublishTeleSensor();
  }
}

/*********************************************************************************************\
 * State loops
\*********************************************************************************************/
/*-------------------------------------------------------------------------------------------*\
 * Every second
\*-------------------------------------------------------------------------------------------*/

void PerformEverySecond(void)
{
  uptime++;

  if (POWER_CYCLE_TIME == uptime) {
    UpdateQuickPowerCycle(false);
  }

  if (BOOT_LOOP_TIME == uptime) {
    RtcRebootReset();

#ifdef USE_DEEPSLEEP
    if (!(DeepSleepEnabled() && !Settings.flag3.bootcount_update)) {
#endif
      Settings.bootcount++;              // Moved to here to stop flash writes during start-up
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " %d"), Settings.bootcount);
#ifdef USE_DEEPSLEEP
    }
#endif
  }

  if (mqtt_cmnd_blocked_reset) {
    mqtt_cmnd_blocked_reset--;
    if (!mqtt_cmnd_blocked_reset) {
      mqtt_cmnd_blocked = 0;             // Clean up MQTT cmnd loop block
    }
  }

  if (seriallog_timer) {
    seriallog_timer--;
    if (!seriallog_timer) {
      if (seriallog_level) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
      }
      seriallog_level = 0;
    }
  }

  if (syslog_timer) {  // Restore syslog level
    syslog_timer--;
    if (!syslog_timer) {
      syslog_level = Settings.syslog_level;
      if (Settings.syslog_level) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
      }
    }
  }

  ResetGlobalValues();

  if (Settings.tele_period) {
    if (tele_period >= 9999) {
      if (!global_state.wifi_down) {
        tele_period = 0;  // Allow teleperiod once wifi is connected
      }
    } else {
      tele_period++;
      if (tele_period >= Settings.tele_period) {
        tele_period = 0;

        MqttPublishTeleState();

        mqtt_data[0] = '\0';
        if (MqttShowSensor()) {
          MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
#if defined(USE_RULES) || defined(USE_SCRIPT)
          RulesTeleperiod();  // Allow rule based HA messages
#endif  // USE_RULES
        }

        XsnsCall(FUNC_AFTER_TELEPERIOD);
        XdrvCall(FUNC_AFTER_TELEPERIOD);
      }
    }
  }
}

/*-------------------------------------------------------------------------------------------*\
 * Every 0.1 second
\*-------------------------------------------------------------------------------------------*/

void Every100mSeconds(void)
{
  // As the max amount of sleep = 250 mSec this loop will shift in time...
  power_t power_now;

  if (prepped_loglevel) {
    AddLog(prepped_loglevel);
  }

  if (latching_relay_pulse) {
    latching_relay_pulse--;
    if (!latching_relay_pulse) SetLatchingRelay(0, 0);
  }

  for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
    if (pulse_timer[i] != 0L) {           // Timer active?
      if (TimeReached(pulse_timer[i])) {  // Timer finished?
        pulse_timer[i] = 0L;              // Turn off this timer
        ExecuteCommandPower(i +1, (POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? POWER_ON : POWER_OFF, SRC_PULSETIMER);
      }
    }
  }

  if (blink_mask) {
    if (TimeReached(blink_timer)) {
      SetNextTimeInterval(blink_timer, 100 * Settings.blinktime);
      blink_counter--;
      if (!blink_counter) {
        StopAllPowerBlink();
      } else {
        blink_power ^= 1;
        power_now = (power & (POWER_MASK ^ blink_mask)) | ((blink_power) ? blink_mask : 0);
        SetDevicePower(power_now, SRC_IGNORE);
      }
    }
  }
}

/*-------------------------------------------------------------------------------------------*\
 * Every 0.25 second
\*-------------------------------------------------------------------------------------------*/

void Every250mSeconds(void)
{
// As the max amount of sleep = 250 mSec this loop should always be taken...

  uint32_t blinkinterval = 1;

  state_250mS++;
  state_250mS &= 0x3;

  if (!Settings.flag.global_state) {                      // Problem blinkyblinky enabled - SetOption31 - Control link led blinking
    if (global_state.data) {                              // Any problem
      if (global_state.mqtt_down) { blinkinterval = 7; }  // MQTT problem so blink every 2 seconds (slowest)
      if (global_state.wifi_down) { blinkinterval = 3; }  // Wifi problem so blink every second (slow)
      blinks = 201;                                       // Allow only a single blink in case the problem is solved
    }
  }
  if (blinks || restart_flag || ota_state_flag) {
    if (restart_flag || ota_state_flag) {                 // Overrule blinks and keep led lit
      blinkstate = true;                                  // Stay lit
    } else {
      blinkspeed--;
      if (!blinkspeed) {
        blinkspeed = blinkinterval;                       // Set interval to 0.2 (default), 1 or 2 seconds
        blinkstate ^= 1;                                  // Blink
      }
    }
    if ((!(Settings.ledstate &0x08)) && ((Settings.ledstate &0x06) || (blinks > 200) || (blinkstate))) {
      SetLedLink(blinkstate);                            // Set led on or off
    }
    if (!blinkstate) {
      blinks--;
      if (200 == blinks) blinks = 0;                      // Disable blink
    }
  }
  if (Settings.ledstate &1 && (pin[GPIO_LEDLNK] < 99 || !(blinks || restart_flag || ota_state_flag)) ) {
    bool tstate = power & Settings.ledmask;
    if ((SONOFF_TOUCH == my_module_type) || (SONOFF_T11 == my_module_type) || (SONOFF_T12 == my_module_type) || (SONOFF_T13 == my_module_type)) {
      tstate = (!power) ? 1 : 0;                          // As requested invert signal for Touch devices to find them in the dark
    }
    SetLedPower(tstate);
  }

/*-------------------------------------------------------------------------------------------*\
 * Every second at 0.25 second interval
\*-------------------------------------------------------------------------------------------*/

  switch (state_250mS) {
  case 0:                                                 // Every x.0 second
    if (ota_state_flag && BACKLOG_EMPTY) {
      ota_state_flag--;
      if (2 == ota_state_flag) {
        RtcSettings.ota_loader = 0;  // Try requested image first
        ota_retry_counter = OTA_ATTEMPTS;
        ESPhttpUpdate.rebootOnUpdate(false);
        SettingsSave(1);  // Free flash for OTA update
      }
      if (ota_state_flag <= 0) {
#ifdef USE_WEBSERVER
        if (Settings.webserver) StopWebserver();
#endif  // USE_WEBSERVER
#ifdef USE_ARILUX_RF
        AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
#endif  // USE_ARILUX_RF
        ota_state_flag = 92;
        ota_result = 0;
        ota_retry_counter--;
        if (ota_retry_counter) {
          strlcpy(mqtt_data, GetOtaUrl(log_data, sizeof(log_data)), sizeof(mqtt_data));
#ifndef FIRMWARE_MINIMAL
          if (RtcSettings.ota_loader) {
            // OTA File too large so try OTA minimal version
            // Replace tasmota                                         with tasmota-minimal
            // Replace tasmota-DE                                      with tasmota-minimal
            // Replace tasmota.bin                                     with tasmota-minimal.bin
            // Replace tasmota.xyz                                     with tasmota-minimal.xyz
            // Replace tasmota.bin.gz                                  with tasmota-minimal.bin.gz
            // Replace tasmota.xyz.gz                                  with tasmota-minimal.xyz.gz
            // Replace tasmota.ino.bin                                 with tasmota-minimal.ino.bin
            // Replace tasmota.ino.bin.gz                              with tasmota-minimal.ino.bin.gz
            // Replace http://domus1:80/api/arduino/tasmota.bin        with http://domus1:80/api/arduino/tasmota-minimal.bin
            // Replace http://domus1:80/api/arduino/tasmota.bin.gz     with http://domus1:80/api/arduino/tasmota-minimal.bin.gz
            // Replace http://domus1:80/api/arduino/tasmota-DE.bin.gz  with http://domus1:80/api/arduino/tasmota-minimal.bin.gz
            // Replace http://domus1:80/api/ard-uino/tasmota-DE.bin.gz with http://domus1:80/api/ard-uino/tasmota-minimal.bin.gz
            // Replace http://192.168.2.17:80/api/arduino/tasmota.bin  with http://192.168.2.17:80/api/arduino/tasmota-minimal.bin
            // Replace http://192.168.2.17/api/arduino/tasmota.bin.gz  with http://192.168.2.17/api/arduino/tasmota-minimal.bin.gz

            char *bch = strrchr(mqtt_data, '/');                       // Only consider filename after last backslash prevent change of urls having "-" in it
            if (bch == nullptr) { bch = mqtt_data; }                   // No path found so use filename only
/*
            char *ech = strrchr(bch, '.');                             // Find file type in filename (none, .bin or .gz)
            if ((ech != nullptr) && (0 == strncasecmp_P(ech, PSTR(".GZ"), 3))) {
              char *fch = ech;
              *fch = '\0';
              ech = strrchr(bch, '.');                                 // Find file type .bin.gz
              *fch = '.';
            }
*/
            char *ech = strchr(bch, '.');                              // Find file type in filename (none, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz)
            if (ech == nullptr) { ech = mqtt_data + strlen(mqtt_data); }  // Point to '/0' at end of mqtt_data becoming an empty string

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("OTA: File type [%s]"), ech);

            char ota_url_type[strlen(ech) +1];
            strncpy(ota_url_type, ech, sizeof(ota_url_type));          // Either empty, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz

            char *pch = strrchr(bch, '-');                             // Find last dash (-) and ignore remainder - handles tasmota-DE
            if (pch == nullptr) { pch = ech; }                         // No dash so ignore filetype
            *pch = '\0';                                               // mqtt_data = http://domus1:80/api/arduino/tasmota
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s-" D_JSON_MINIMAL "%s"), mqtt_data, ota_url_type);  // Minimal filename must be filename-minimal
          }
#endif  // FIRMWARE_MINIMAL
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "%s"), mqtt_data);
#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
          ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(mqtt_data));
#else
          // If using core stage or 2.5.0+ the syntax has changed
          WiFiClient OTAclient;
          ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(OTAclient, mqtt_data));
#endif
          if (!ota_result) {
#ifndef FIRMWARE_MINIMAL
            int ota_error = ESPhttpUpdate.getLastError();
            DEBUG_CORE_LOG(PSTR("OTA: Error %d"), ota_error);
            if ((HTTP_UE_TOO_LESS_SPACE == ota_error) || (HTTP_UE_BIN_FOR_WRONG_FLASH == ota_error)) {
              RtcSettings.ota_loader = 1;  // Try minimal image next
            }
#endif  // FIRMWARE_MINIMAL
            ota_state_flag = 2;    // Upgrade failed - retry
          }
        }
      }
      if (90 == ota_state_flag) {  // Allow MQTT to reconnect
        ota_state_flag = 0;
        Response_P(PSTR("{\"" D_CMND_UPGRADE "\":\""));
        if (ota_result) {
//          SetFlashModeDout();      // Force DOUT for both ESP8266 and ESP8285
          if (!VersionCompatible()) {
            ResponseAppend_P(PSTR(D_JSON_FAILED " " D_UPLOAD_ERR_14));
          } else {
            ResponseAppend_P(PSTR(D_JSON_SUCCESSFUL ". " D_JSON_RESTARTING));
            restart_flag = 2;
          }
        } else {
          ResponseAppend_P(PSTR(D_JSON_FAILED " %s"), ESPhttpUpdate.getLastErrorString().c_str());
        }
        ResponseAppend_P(PSTR("\"}"));
//        restart_flag = 2;          // Restart anyway to keep memory clean webserver
        MqttPublishPrefixTopic_P(STAT, PSTR(D_CMND_UPGRADE));
      }
    }
    break;
  case 1:                                                 // Every x.25 second
    if (MidnightNow()) {
      XsnsCall(FUNC_SAVE_AT_MIDNIGHT);
    }
    if (save_data_counter && BACKLOG_EMPTY) {
      save_data_counter--;
      if (save_data_counter <= 0) {
        if (Settings.flag.save_state) {                   // SetOption0 - Save power state and use after restart
          power_t mask = POWER_MASK;
          for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
            if ((Settings.pulse_timer[i] > 0) && (Settings.pulse_timer[i] < 30)) {  // 3 seconds
              mask &= ~(1 << i);
            }
          }
          if (!((Settings.power &mask) == (power &mask))) {
            Settings.power = power;
          }
        } else {
          Settings.power = 0;
        }
        SettingsSave(0);
        save_data_counter = Settings.save_data;
      }
    }
    if (restart_flag && BACKLOG_EMPTY) {
      if ((214 == restart_flag) || (215 == restart_flag) || (216 == restart_flag)) {
        // Backup current SSIDs and Passwords
        char storage_ssid1[strlen(SettingsText(SET_STASSID1)) +1];
        strncpy(storage_ssid1, SettingsText(SET_STASSID1), sizeof(storage_ssid1));
        char storage_ssid2[strlen(SettingsText(SET_STASSID2)) +1];
        strncpy(storage_ssid2, SettingsText(SET_STASSID2), sizeof(storage_ssid2));
        char storage_pass1[strlen(SettingsText(SET_STAPWD1)) +1];
        strncpy(storage_pass1, SettingsText(SET_STAPWD1), sizeof(storage_pass1));
        char storage_pass2[strlen(SettingsText(SET_STAPWD2)) +1];
        strncpy(storage_pass2, SettingsText(SET_STAPWD2), sizeof(storage_pass2));

        char storage_mqtthost[strlen(SettingsText(SET_MQTT_HOST)) +1];
        strncpy(storage_mqtthost, SettingsText(SET_MQTT_HOST), sizeof(storage_mqtthost));
        char storage_mqttuser[strlen(SettingsText(SET_MQTT_USER)) +1];
        strncpy(storage_mqttuser, SettingsText(SET_MQTT_USER), sizeof(storage_mqttuser));
        char storage_mqttpwd[strlen(SettingsText(SET_MQTT_PWD)) +1];
        strncpy(storage_mqttpwd, SettingsText(SET_MQTT_PWD), sizeof(storage_mqttpwd));
        char storage_mqtttopic[strlen(SettingsText(SET_MQTT_TOPIC)) +1];
        strncpy(storage_mqtttopic, SettingsText(SET_MQTT_TOPIC), sizeof(storage_mqtttopic));
        uint16_t mqtt_port = Settings.mqtt_port;

//        if (216 == restart_flag) {
          // Backup mqtt host, port, client, username and password
//        }
        if ((215 == restart_flag) || (216 == restart_flag)) {
          SettingsErase(0);  // Erase all flash from program end to end of physical flash
        }
        SettingsDefault();
        // Restore current SSIDs and Passwords
        SettingsUpdateText(SET_STASSID1, storage_ssid1);
        SettingsUpdateText(SET_STASSID2, storage_ssid2);
        SettingsUpdateText(SET_STAPWD1, storage_pass1);
        SettingsUpdateText(SET_STAPWD2, storage_pass2);
        if (216 == restart_flag) {
          // Restore the mqtt host, port, client, username and password
          SettingsUpdateText(SET_MQTT_HOST, storage_mqtthost);
          SettingsUpdateText(SET_MQTT_USER, storage_mqttuser);
          SettingsUpdateText(SET_MQTT_PWD, storage_mqttpwd);
          SettingsUpdateText(SET_MQTT_TOPIC, storage_mqtttopic);
          Settings.mqtt_port = mqtt_port;
        }
        restart_flag = 2;
      }
      else if (213 == restart_flag) {
        SettingsSdkErase();  // Erase flash SDK parameters
        restart_flag = 2;
      }
      else if (212 == restart_flag) {
        SettingsErase(0);    // Erase all flash from program end to end of physical flash
        restart_flag = 211;
      }
      if (211 == restart_flag) {
        SettingsDefault();
        restart_flag = 2;
      }
      if (2 == restart_flag) {
        SettingsSaveAll();
      }
      restart_flag--;
      if (restart_flag <= 0) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
        EspRestart();
      }
    }
    break;
  case 2:                                                 // Every x.5 second
    WifiCheck(wifi_state_flag);
    wifi_state_flag = WIFI_RESTART;
    break;
  case 3:                                                 // Every x.75 second
    if (!global_state.wifi_down) { MqttCheck(); }
    break;
  }
}

#ifdef USE_ARDUINO_OTA
/*********************************************************************************************\
 * Allow updating via the Arduino OTA-protocol.
 *
 * - Once started disables current wifi clients and udp
 * - Perform restart when done to re-init wifi clients
\*********************************************************************************************/

bool arduino_ota_triggered = false;
uint16_t arduino_ota_progress_dot_count = 0;

void ArduinoOTAInit(void)
{
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(my_hostname);
  if (strlen(SettingsText(SET_WEBPWD))) {
    ArduinoOTA.setPassword(SettingsText(SET_WEBPWD));
  }

  ArduinoOTA.onStart([]()
  {
    SettingsSave(1);         // Free flash for OTA update
#ifdef USE_WEBSERVER
    if (Settings.webserver) { StopWebserver(); }
#endif  // USE_WEBSERVER
#ifdef USE_ARILUX_RF
    AriluxRfDisable();       // Prevent restart exception on Arilux Interrupt routine
#endif  // USE_ARILUX_RF
    if (Settings.flag.mqtt_enabled) {
      MqttDisconnect();      // SetOption3  - Enable MQTT
    }
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_UPLOAD_STARTED));
    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;
    delay(100);              // Allow time for message xfer
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    if ((LOG_LEVEL_DEBUG <= seriallog_level)) {
      arduino_ota_progress_dot_count++;
      Serial.printf(".");
      if (!(arduino_ota_progress_dot_count % 80)) { Serial.println(); }
    }
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[100];

    if ((LOG_LEVEL_DEBUG <= seriallog_level) && arduino_ota_progress_dot_count) { Serial.println(); }
    switch (error) {
      case OTA_BEGIN_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA  %s. " D_RESTARTING), error_str);
    EspRestart();
  });

  ArduinoOTA.onEnd([]()
  {
    if ((LOG_LEVEL_DEBUG <= seriallog_level)) { Serial.println(); }
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_SUCCESSFUL ". " D_RESTARTING));
    EspRestart();
	});

  ArduinoOTA.begin();
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_ENABLED " " D_PORT " 8266"));
}

void ArduinoOtaLoop(void)
{
  MDNS.update();
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  while (arduino_ota_triggered) { ArduinoOTA.handle(); }
}
#endif  // USE_ARDUINO_OTA

/********************************************************************************************/

void SerialInput(void)
{
  while (Serial.available()) {
//    yield();
    delay(0);
    serial_in_byte = Serial.read();

/*-------------------------------------------------------------------------------------------*\
 * Sonoff dual and ch4 19200 baud serial interface
\*-------------------------------------------------------------------------------------------*/
    if ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
      serial_in_byte = ButtonSerial(serial_in_byte);
    }

/*-------------------------------------------------------------------------------------------*/

    if (XdrvCall(FUNC_SERIAL)) {
      serial_in_byte_counter = 0;
      Serial.flush();
      return;
    }

/*-------------------------------------------------------------------------------------------*/

    if (serial_in_byte > 127 && !Settings.flag.mqtt_serial_raw) {                // Discard binary data above 127 if no raw reception allowed - CMND_SERIALSEND3
      serial_in_byte_counter = 0;
      Serial.flush();
      return;
    }
    if (!Settings.flag.mqtt_serial) {                                            // SerialSend active - CMND_SERIALSEND and CMND_SERIALLOG
      if (isprint(serial_in_byte)) {                                             // Any char between 32 and 127
        if (serial_in_byte_counter < INPUT_BUFFER_SIZE -1) {                     // Add char to string if it still fits
          serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
        } else {
          serial_in_byte_counter = 0;
        }
      }
    } else {
      if (serial_in_byte || Settings.flag.mqtt_serial_raw) {                     // Any char between 1 and 127 or any char (0 - 255) - CMND_SERIALSEND3
        if ((serial_in_byte_counter < INPUT_BUFFER_SIZE -1) &&                   // Add char to string if it still fits and ...
            ((isprint(serial_in_byte) && (128 == Settings.serial_delimiter)) ||  // Any char between 32 and 127
            ((serial_in_byte != Settings.serial_delimiter) && (128 != Settings.serial_delimiter)) ||  // Any char between 1 and 127 and not being delimiter
              Settings.flag.mqtt_serial_raw)) {                                  // Any char between 0 and 255 - CMND_SERIALSEND3
          serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
          serial_polling_window = millis();
        } else {
          serial_polling_window = 0;                                             // Reception done - send mqtt
          break;
        }
      }
    }

#ifdef USE_SONOFF_SC
/*-------------------------------------------------------------------------------------------*\
 * Sonoff SC 19200 baud serial interface
\*-------------------------------------------------------------------------------------------*/
    if (SONOFF_SC == my_module_type) {
      if (serial_in_byte == '\x1B') {                                            // Sonoff SC status from ATMEGA328P
        serial_in_buffer[serial_in_byte_counter] = 0;                            // Serial data completed
        SonoffScSerialInput(serial_in_buffer);
        serial_in_byte_counter = 0;
        Serial.flush();
        return;
      }
    } else
#endif  // USE_SONOFF_SC
/*-------------------------------------------------------------------------------------------*/

    if (!Settings.flag.mqtt_serial && (serial_in_byte == '\n')) {                // CMND_SERIALSEND and CMND_SERIALLOG
      serial_in_buffer[serial_in_byte_counter] = 0;                              // Serial data completed
      seriallog_level = (Settings.seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings.seriallog_level;
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), serial_in_buffer);
      ExecuteCommand(serial_in_buffer, SRC_SERIAL);
      serial_in_byte_counter = 0;
      serial_polling_window = 0;
      Serial.flush();
      return;
    }
  }

  if (Settings.flag.mqtt_serial && serial_in_byte_counter && (millis() > (serial_polling_window + SERIAL_POLLING))) {  // CMND_SERIALSEND and CMND_SERIALLOG
    serial_in_buffer[serial_in_byte_counter] = 0;                                // Serial data completed
    char hex_char[(serial_in_byte_counter * 2) + 2];
    bool assume_json = (!Settings.flag.mqtt_serial_raw && (serial_in_buffer[0] == '{'));
    Response_P(PSTR("{\"" D_JSON_SERIALRECEIVED "\":%s%s%s}"),
      (assume_json) ? "" : """",
      (Settings.flag.mqtt_serial_raw) ? ToHex_P((unsigned char*)serial_in_buffer, serial_in_byte_counter, hex_char, sizeof(hex_char)) : serial_in_buffer,
      (assume_json) ? "" : """");
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_SERIALRECEIVED));
    XdrvRulesProcess();
    serial_in_byte_counter = 0;
  }
}

/********************************************************************************************/

void ResetPwm(void)
{
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (pin[GPIO_PWM1 +i] < 99) {
      analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range : 0);
//      analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - Settings.pwm_value[i] : Settings.pwm_value[i]);
    }
  }
}

/********************************************************************************************/

void GpioInit(void)
{
  uint32_t mpin;

  if (!ValidModule(Settings.module)) {
    uint32_t module = MODULE;
    if (!ValidModule(MODULE)) { module = SONOFF_BASIC; }
    Settings.module = module;
    Settings.last_module = module;
  }
  SetModuleType();

  if (Settings.module != Settings.last_module) {
    Settings.baudrate = APP_BAUDRATE / 300;
    Settings.serial_config = TS_SERIAL_8N1;
  }

  for (uint32_t i = 0; i < sizeof(Settings.user_template.gp); i++) {
    if ((Settings.user_template.gp.io[i] >= GPIO_SENSOR_END) && (Settings.user_template.gp.io[i] < GPIO_USER)) {
      Settings.user_template.gp.io[i] = GPIO_USER;  // Fix not supported sensor ids in template
    }
  }

  myio def_gp;
  ModuleGpios(&def_gp);
  for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
    if ((Settings.my_gp.io[i] >= GPIO_SENSOR_END) && (Settings.my_gp.io[i] < GPIO_USER)) {
      Settings.my_gp.io[i] = GPIO_NONE;             // Fix not supported sensor ids in module
    }
    else if (Settings.my_gp.io[i] > GPIO_NONE) {
      my_module.io[i] = Settings.my_gp.io[i];       // Set User selected Module sensors
    }
    if ((def_gp.io[i] > GPIO_NONE) && (def_gp.io[i] < GPIO_USER)) {
      my_module.io[i] = def_gp.io[i];               // Force Template override
    }
  }
  if ((Settings.my_adc0 >= ADC0_END) && (Settings.my_adc0 < ADC0_USER)) {
    Settings.my_adc0 = ADC0_NONE;                   // Fix not supported sensor ids in module
  }
  else if (Settings.my_adc0 > ADC0_NONE) {
    my_adc0 = Settings.my_adc0;                     // Set User selected Module sensors
  }
  my_module_flag = ModuleFlag();
  uint32_t template_adc0 = my_module_flag.data &15;
  if ((template_adc0 > ADC0_NONE) && (template_adc0 < ADC0_USER)) {
    my_adc0 = template_adc0;                        // Force Template override
  }

  for (uint32_t i = 0; i < GPIO_MAX; i++) {
    pin[i] = 99;
  }
  for (uint32_t i = 0; i < sizeof(my_module.io); i++) {
    mpin = ValidPin(i, my_module.io[i]);

    DEBUG_CORE_LOG(PSTR("INI: gpio pin %d, mpin %d"), i, mpin);

    if (mpin) {
      XdrvMailbox.index = mpin;
      XdrvMailbox.payload = i;

      if ((mpin >= GPIO_SWT1_NP) && (mpin < (GPIO_SWT1_NP + MAX_SWITCHES))) {
        SwitchPullupFlag(mpin - GPIO_SWT1_NP);
        mpin -= (GPIO_SWT1_NP - GPIO_SWT1);
      }
      else if ((mpin >= GPIO_KEY1_NP) && (mpin < (GPIO_KEY1_NP + MAX_KEYS))) {
        ButtonPullupFlag(mpin - GPIO_KEY1_NP);       //  0 .. 3
        mpin -= (GPIO_KEY1_NP - GPIO_KEY1);
      }
      else if ((mpin >= GPIO_KEY1_INV) && (mpin < (GPIO_KEY1_INV + MAX_KEYS))) {
        ButtonInvertFlag(mpin - GPIO_KEY1_INV);      //  0 .. 3
        mpin -= (GPIO_KEY1_INV - GPIO_KEY1);
      }
      else if ((mpin >= GPIO_KEY1_INV_NP) && (mpin < (GPIO_KEY1_INV_NP + MAX_KEYS))) {
        ButtonPullupFlag(mpin - GPIO_KEY1_INV_NP);   //  0 .. 3
        ButtonInvertFlag(mpin - GPIO_KEY1_INV_NP);   //  0 .. 3
        mpin -= (GPIO_KEY1_INV_NP - GPIO_KEY1);
      }
      else if ((mpin >= GPIO_REL1_INV) && (mpin < (GPIO_REL1_INV + MAX_RELAYS))) {
        bitSet(rel_inverted, mpin - GPIO_REL1_INV);
        mpin -= (GPIO_REL1_INV - GPIO_REL1);
      }
      else if ((mpin >= GPIO_LED1_INV) && (mpin < (GPIO_LED1_INV + MAX_LEDS))) {
        bitSet(led_inverted, mpin - GPIO_LED1_INV);
        mpin -= (GPIO_LED1_INV - GPIO_LED1);
      }
      else if (mpin == GPIO_LEDLNK_INV) {
        ledlnk_inverted = 1;
        mpin -= (GPIO_LEDLNK_INV - GPIO_LEDLNK);
      }
      else if ((mpin >= GPIO_PWM1_INV) && (mpin < (GPIO_PWM1_INV + MAX_PWMS))) {
        bitSet(pwm_inverted, mpin - GPIO_PWM1_INV);
        mpin -= (GPIO_PWM1_INV - GPIO_PWM1);
      }
      else if (XdrvCall(FUNC_PIN_STATE)) {
        mpin = XdrvMailbox.index;
      }
      else if (XsnsCall(FUNC_PIN_STATE)) {
        mpin = XdrvMailbox.index;
      };
    }
    if (mpin) pin[mpin] = i;
  }

  if ((2 == pin[GPIO_TXD]) || (H801 == my_module_type)) { Serial.set_tx(2); }

  analogWriteRange(Settings.pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)

#ifdef USE_SPI
  spi_flg = ((((pin[GPIO_SPI_CS] < 99) && (pin[GPIO_SPI_CS] > 14)) || (pin[GPIO_SPI_CS] < 12)) || (((pin[GPIO_SPI_DC] < 99) && (pin[GPIO_SPI_DC] > 14)) || (pin[GPIO_SPI_DC] < 12)));
  if (spi_flg) {
    for (uint32_t i = 0; i < GPIO_MAX; i++) {
      if ((pin[i] >= 12) && (pin[i] <=14)) pin[i] = 99;
    }
    my_module.io[12] = GPIO_SPI_MISO;
    pin[GPIO_SPI_MISO] = 12;
    my_module.io[13] = GPIO_SPI_MOSI;
    pin[GPIO_SPI_MOSI] = 13;
    my_module.io[14] = GPIO_SPI_CLK;
    pin[GPIO_SPI_CLK] = 14;
  }
  soft_spi_flg = ((pin[GPIO_SSPI_CS] < 99) && (pin[GPIO_SSPI_SCLK] < 99) && ((pin[GPIO_SSPI_MOSI] < 99) || (pin[GPIO_SSPI_MOSI] < 99)));
#endif  // USE_SPI

  // Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
  // Doing it here solves relay toggles at restart.
  for (uint32_t i = 0; i < sizeof(my_module.io); i++) {
    mpin = ValidPin(i, my_module.io[i]);
//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("INI: gpio pin %d, mpin %d"), i, mpin);
    if (((i < 6) || (i > 11)) && (0 == mpin)) {  // Skip SPI flash interface
      if (!((1 == i) || (3 == i))) {             // Skip serial
        pinMode(i, INPUT);
      }
    }
  }

#ifdef USE_I2C
  i2c_flg = ((pin[GPIO_I2C_SCL] < 99) && (pin[GPIO_I2C_SDA] < 99));
  if (i2c_flg) {
    Wire.begin(pin[GPIO_I2C_SDA], pin[GPIO_I2C_SCL]);
  }
#endif  // USE_I2C

  devices_present = 0;
  light_type = LT_BASIC;                     // Use basic PWM control if SetOption15 = 0
  if (XdrvCall(FUNC_MODULE_INIT)) {
    // Serviced
  }
  else if (YTF_IR_BRIDGE == my_module_type) {
    ClaimSerial();  // Stop serial loopback mode
//    devices_present = 1;
  }
  else if (SONOFF_DUAL == my_module_type) {
    devices_present = 2;
    SetSerial(19200, TS_SERIAL_8N1);
  }
  else if (CH4 == my_module_type) {
    devices_present = 4;
    SetSerial(19200, TS_SERIAL_8N1);
  }
#ifdef USE_SONOFF_SC
  else if (SONOFF_SC == my_module_type) {
    SetSerial(19200, TS_SERIAL_8N1);
  }
#endif  // USE_SONOFF_SC

  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (pin[GPIO_PWM1 +i] < 99) {
      pinMode(pin[GPIO_PWM1 +i], OUTPUT);
      if (light_type) {
        // force PWM GPIOs to low or high mode, see #7165
        analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range : 0);
      } else {
        pwm_present = true;
        analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - Settings.pwm_value[i] : Settings.pwm_value[i]);
      }
    }
  }

  for (uint32_t i = 0; i < MAX_RELAYS; i++) {
    if (pin[GPIO_REL1 +i] < 99) {
      pinMode(pin[GPIO_REL1 +i], OUTPUT);
      devices_present++;
      if (EXS_RELAY == my_module_type) {
        digitalWrite(pin[GPIO_REL1 +i], bitRead(rel_inverted, i) ? 1 : 0);
        if (i &1) { devices_present--; }
      }
    }
  }

  for (uint32_t i = 0; i < MAX_LEDS; i++) {
    if (pin[GPIO_LED1 +i] < 99) {
#ifdef USE_ARILUX_RF
      if ((3 == i) && (leds_present < 2) && (99 == pin[GPIO_ARIRFSEL])) {
        pin[GPIO_ARIRFSEL] = pin[GPIO_LED4];  // Legacy support where LED4 was Arilux RF enable
        pin[GPIO_LED4] = 99;
      } else {
#endif
        pinMode(pin[GPIO_LED1 +i], OUTPUT);
        leds_present++;
        digitalWrite(pin[GPIO_LED1 +i], bitRead(led_inverted, i));
#ifdef USE_ARILUX_RF
      }
#endif
    }
  }
  if (pin[GPIO_LEDLNK] < 99) {
    pinMode(pin[GPIO_LEDLNK], OUTPUT);
    digitalWrite(pin[GPIO_LEDLNK], ledlnk_inverted);
  }

#ifdef USE_PWM_DIMMER
  if (PWM_DIMMER == my_module_type && pin[GPIO_REL1] < 99) devices_present--;
#endif  // USE_PWM_DIMMER

  ButtonInit();
  SwitchInit();
#ifdef ROTARY_V1
  RotaryInit();
#endif

  SetLedPower(Settings.ledstate &8);
  SetLedLink(Settings.ledstate &8);

  XdrvCall(FUNC_PRE_INIT);
}
