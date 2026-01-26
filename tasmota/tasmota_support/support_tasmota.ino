/*
  support_tasmota.ino - Core support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  #include "HttpClientLight.h"
#endif

const char kSleepMode[] PROGMEM = "Dynamic|Normal";
const char kPrefixes[] PROGMEM = D_CMND "|" D_STAT "|" D_TELE;

char* Format(char* output, const char* input_p, int size)
{
  char *token;
  uint32_t digits = 0;
  char input[strlen_P(input_p)+1];  // copy from PMEM to RAM
  strcpy_P(input, input_p);

  if (strchr(input, '%') != nullptr) {
    strlcpy(output, input, size);
    token = strtok(output, "%");
    if (strchr(input, '%') == input) {
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
          snprintf_P(output, size, tmp, ESP_getChipId() & 0x1fff);            // %04d - short chip ID in dec, like in hostname
        } else {
          String mac_address = NetworkUniqueId();
          if (digits > 12) { digits = 12; }
          String mac_part = mac_address.substring(12 - digits);
          snprintf_P(output, size, PSTR("%s%s"), output, mac_part.c_str());  // %01X .. %12X - mac address in hex
        }
      } else {
        if (strchr(token, 'd')) {
          snprintf_P(output, size, PSTR("%s%d"), output, ESP_getChipId());   // %d - full chip ID in dec
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
    snprintf(otaurl, otaurl_size, SettingsText(SET_OTAURL), ESP_getChipId() & 0x1fff);
  }
  else if (strstr(SettingsText(SET_OTAURL), "%d") != nullptr) {  // OTA url contains placeholder for chip ID
    snprintf_P(otaurl, otaurl_size, SettingsText(SET_OTAURL), ESP_getChipId());
  }
  else {
    strlcpy(otaurl, SettingsText(SET_OTAURL), otaurl_size);
  }

  return otaurl;
}

String ResolveToken(const char* input) {
  String resolved = input;
  resolved.replace(F("%hostname%"), TasmotaGlobal.hostname);
  resolved.replace(F("%id%"), NetworkUniqueId());
  return resolved;
}

char* GetTopic_P(char *stopic, uint32_t prefix, const char *topic, const char* subtopic)
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
  if (TasmotaGlobal.fallback_topic_flag || (prefix > 3)) {
    bool fallback = (prefix < 8);
    prefix &= 3;
    char stemp[11];
    fulltopic = GetTextIndexed(stemp, sizeof(stemp), prefix, kPrefixes);
    fulltopic += F("/");
    if (fallback) {
      fulltopic += TasmotaGlobal.mqtt_client;
      fulltopic += F("_fb");                  // cmnd/<mqttclient>_fb
    } else {
      fulltopic += (const __FlashStringHelper *)topic;                     // cmnd/<grouptopic>
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

    fulltopic.replace(FPSTR(MQTT_TOKEN_TOPIC), (const __FlashStringHelper *)topic);
    fulltopic = ResolveToken(fulltopic.c_str());
  }
  fulltopic.replace(F("#"), "");
  fulltopic.replace(F("//"), "/");
  if (!fulltopic.endsWith("/")) {
    fulltopic += "/";
  }
  snprintf_P(stopic, TOPSZ, PSTR("%s%s"), fulltopic.c_str(), romram);
  return stopic;
}

char* GetGroupTopic_P(char *stopic, const char* subtopic, uint32_t itopic)
{
  // SetOption75 0: %prefix%/nothing/%topic% = cmnd/nothing/<grouptopic>/#
  // SetOption75 1: cmnd/<grouptopic>
  return GetTopic_P(stopic, (Settings->flag3.grouptopic_mode) ? CMND +8 : CMND, SettingsText(itopic), subtopic);  // SetOption75 - GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
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

/*********************************************************************************************\
 * Zero-cross support
\*********************************************************************************************/

//#define DEBUG_ZEROCROSS

void ZeroCrossMomentStart(void) {
  if (!TasmotaGlobal.zc_interval) { return; }

#ifdef DEBUG_ZEROCROSS
  uint32_t dbg_interval = TasmotaGlobal.zc_interval;
  uint32_t dbg_zctime = TasmotaGlobal.zc_time;
  uint32_t dbg_starttime = micros();
#endif

  uint32_t trigger_moment = TasmotaGlobal.zc_time + TasmotaGlobal.zc_interval - TasmotaGlobal.zc_offset - TasmotaGlobal.zc_code_offset;
  while (TimeReachedUsec(trigger_moment)) {    // Trigger moment already passed so try next
    trigger_moment += TasmotaGlobal.zc_interval;
  }
  while (!TimeReachedUsec(trigger_moment)) {}  // Wait for trigger moment

#ifdef DEBUG_ZEROCROSS
  uint32_t dbg_endtime = micros();
  AddLog(LOG_LEVEL_DEBUG, PSTR("ZCD: CodeExecTime %d, StartTime %u, EndTime %u, ZcTime %u, Interval %d"),
    dbg_endtime - dbg_starttime, dbg_starttime, dbg_endtime, dbg_zctime, dbg_interval);
#endif

  TasmotaGlobal.zc_code_offset = micros();
}

void ZeroCrossMomentEnd(void) {
  if (!TasmotaGlobal.zc_interval) { return; }
  TasmotaGlobal.zc_code_offset = (micros() - TasmotaGlobal.zc_code_offset) / 2;

#ifdef DEBUG_ZEROCROSS
  AddLog(LOG_LEVEL_DEBUG, PSTR("ZCD: CodeExecTime %d"), TasmotaGlobal.zc_code_offset * 2);
#endif
}

void IRAM_ATTR ZeroCrossIsr(void);
void ZeroCrossIsr(void) {
  uint32_t time = micros();
  TasmotaGlobal.zc_interval = ((int32_t) (time - TasmotaGlobal.zc_time));
  TasmotaGlobal.zc_time = time;
}

void ZeroCrossInit(uint32_t offset) {
  if (PinUsed(GPIO_ZEROCROSS)) {
    TasmotaGlobal.zc_offset = offset;

    uint32_t gpio = Pin(GPIO_ZEROCROSS);
    pinMode(gpio, INPUT_PULLUP);
    attachInterrupt(gpio, ZeroCrossIsr, CHANGE);

    AddLog(LOG_LEVEL_INFO, PSTR("ZCD: Activated"));  // Zero-cross detection activated
  }
}

/********************************************************************************************/

void XdrvXsnsCall(uint32_t function) {
  XdrvCall(function);
  XsnsCall(function);
}

void XsnsXdrvCall(uint32_t function) {
  XsnsCall(function);
  XdrvCall(function);
}

void SetLatchingRelay(power_t lpower, uint32_t state) {
  // TasmotaGlobal.power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
  // TasmotaGlobal.power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
  // TasmotaGlobal.power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
  // TasmotaGlobal.power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On
  static power_t latching_power = 0;     // Power state at latching start

  if (state && !TasmotaGlobal.latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
    latching_power = lpower;
    TasmotaGlobal.latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
  }

  for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
    uint32_t port = (i << 1) + ((latching_power >> i) &1);
    DigitalWrite(GPIO_REL1, port, bitRead(TasmotaGlobal.rel_inverted, port) ? !state : state);
  }
}

void SetDevicePower(power_t rpower, uint32_t source) {
  if (TasmotaGlobal.power_on_delay) {
    TasmotaGlobal.power_on_delay_state = rpower;
    return;
  }

  ShowSource(source);
  TasmotaGlobal.last_source = source;

  if (POWER_ALL_ALWAYS_ON == Settings->poweronstate) {  // All on and stay on
    TasmotaGlobal.power = POWER_MASK >> (POWER_SIZE - TasmotaGlobal.devices_present);
    rpower = TasmotaGlobal.power;
  }

  if (Settings->flag.interlock) {          // Allow only one or no relay set - CMND_INTERLOCK - Enable/disable interlock
    for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
      power_t mask = 1;
      uint32_t count = 0;
      for (uint32_t j = 0; j < TasmotaGlobal.devices_present; j++) {
        if ((Settings->interlock[i] & mask) && (rpower & mask)) {
          count++;
        }
        mask <<= 1;
      }
      if (count > 1) {
        mask = ~Settings->interlock[i];    // Turn interlocked group off as there would be multiple relays on
        TasmotaGlobal.power &= mask;
        rpower &= mask;
      }
    }
  }

  if (rpower) {                           // Any power set
    TasmotaGlobal.last_power = rpower;
  }

  XdrvMailbox.index = rpower;
  XdrvXsnsCall(FUNC_SET_POWER);           // Signal power state

  XdrvMailbox.index = rpower;
  XdrvMailbox.payload = source;
  if (XdrvCall(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
    // Serviced
  }
#ifdef ESP8266
  else if ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
    Serial.write(0xA0);
    Serial.write(0x04);
    Serial.write(rpower &0xFF);
    Serial.write(0xA1);
    Serial.write('\n');
    Serial.flush();
  }
  else if (EXS_RELAY == TasmotaGlobal.module_type) {
    SetLatchingRelay(rpower, 1);
  }
#endif  // ESP8266
  else {
    uint32_t port = 0;
    uint32_t port_next;
    power_t bistable = 0;

    ZeroCrossMomentStart();

    for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
      power_t state = rpower &1;

      port_next = 1;                              // Select next relay
      bool update = true;
      if (bitRead(TasmotaGlobal.rel_bistable, port)) {
        if (Settings->flag6.bistable_single_pin) {  // SetOption152 - (Power) Use single pin bistable
          if (0x80000000 == TasmotaGlobal.power_latching) {
            TasmotaGlobal.power_latching = TasmotaGlobal.power;  // Init last known state
          }
          update = (bitRead(TasmotaGlobal.power_latching, port) != state);
          if (update) {
            bitWrite(TasmotaGlobal.power_latching, port, state);
            bitSet(bistable, port);
          }

        } else {
          if (!state) { port_next = 2; }          // Skip highest relay
          port += state;                          // Relay<lowest> = Off, Relay<highest> = On
        }
        state = 1;                                // Set pulse
      }
      if (update && (i < MAX_RELAYS)) {
        DigitalWrite(GPIO_REL1, port, bitRead(TasmotaGlobal.rel_inverted, port) ? !state : state);
      }
      port += port_next;                          // Select next relay
      rpower >>= 1;                               // Select next power
    }

    ZeroCrossMomentEnd();

    // Reset bistable relay here to fix non-interlock situations due to fast switching
    if (TasmotaGlobal.rel_bistable) {             // If bistable relays in the mix reset them after 40ms
      delay(Settings->param[P_BISTABLE_PULSE]);   // SetOption45 - Keep energized for about 5 x operation time
      for (uint32_t i = 0; i < port; i++) {       // Reset up to detected amount of ports
        if (bitRead(TasmotaGlobal.rel_bistable, i)) {
          if (Settings->flag6.bistable_single_pin) {  // SetOption152 - (Power) Use single pin bistable
            if (!bitRead(bistable, i)) {
              continue;
            }
          }
          DigitalWrite(GPIO_REL1, i, bitRead(TasmotaGlobal.rel_inverted, i) ? 1 : 0);
        }
      }
    }
  }
}

void RestorePower(bool publish_power, uint32_t source)
{
  if (TasmotaGlobal.power != TasmotaGlobal.last_power) {
    TasmotaGlobal.power = TasmotaGlobal.last_power;
    SetDevicePower(TasmotaGlobal.power, source);
    if (publish_power) {
      MqttPublishAllPowerState();
    }
  }
}

void SetAllPower(uint32_t state, uint32_t source) {
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings->pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 5 = POWER_OFF_FORCE = Relay Off even if locked
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }
  if (((state >= POWER_OFF) && (state <= POWER_TOGGLE)) || (POWER_OFF_FORCE == state))  {
    power_t current_power = TasmotaGlobal.power;
    power_t all_on = POWER_MASK >> (POWER_SIZE - TasmotaGlobal.devices_present);
    switch (state) {
    case POWER_OFF:
      // Keep locked bits and set all other to 0
      TasmotaGlobal.power &= Settings->power_lock; 
      break;
    case POWER_ON:
      // Keep locked bits and set all other to 1
      TasmotaGlobal.power = (TasmotaGlobal.power & Settings->power_lock) | (all_on & ~Settings->power_lock);
      break;
    case POWER_TOGGLE:
      // Keep locked bits and toggle all other
      TasmotaGlobal.power ^= ~Settings->power_lock & all_on;
      break;
    case POWER_OFF_FORCE:
      // Set all off even if locked on (Used by overtemp and overcurrent)
      TasmotaGlobal.power = 0; 
      break;
    }
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) {
      // Do not touch Fan relays
      TasmotaGlobal.power &= 0x0001;
      TasmotaGlobal.power |= (current_power & 0xFFFE);
    }
#endif  // USE_SONOFF_IFAN
    SetDevicePower(TasmotaGlobal.power, source);
  }
  if (publish_power) {
    MqttPublishAllPowerState();
  }
}

void SetPowerOnState(void)
{
#ifdef ESP8266
  if (MOTOR == TasmotaGlobal.module_type) {
    Settings->poweronstate = POWER_ALL_ON;   // Needs always on else in limbo!
  }
#endif  // ESP8266
  if (POWER_ALL_ALWAYS_ON == Settings->poweronstate) {
    SetDevicePower(1, SRC_RESTART);
  } else {
    power_t devices_mask = POWER_MASK >> (POWER_SIZE - TasmotaGlobal.devices_present);
    if (ResetReasonPowerOn()) {
      TasmotaGlobal.power_latching = 0;   // Single pin latching relay is powered off after re-applying power
      switch (Settings->poweronstate) {
      case POWER_ALL_OFF:
      case POWER_ALL_OFF_PULSETIME_ON:
        TasmotaGlobal.power = 0;
        SetDevicePower(TasmotaGlobal.power, SRC_RESTART);
        break;
      case POWER_ALL_ON:  // All on
        TasmotaGlobal.power = devices_mask;
        SetDevicePower(TasmotaGlobal.power, SRC_RESTART);
        break;
      case POWER_ALL_SAVED_TOGGLE:
        TasmotaGlobal.power = (Settings->power & devices_mask) ^ POWER_MASK;
        if (Settings->flag.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(TasmotaGlobal.power, SRC_RESTART);
        }
        break;
      case POWER_ALL_SAVED:
        TasmotaGlobal.power = Settings->power & devices_mask;
        if (Settings->flag.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(TasmotaGlobal.power, SRC_RESTART);
        }
        break;
      }
    } else {
      TasmotaGlobal.power = Settings->power & devices_mask;
      if (Settings->flag.save_state) {    // SetOption0 - Save power state and use after restart
        SetDevicePower(TasmotaGlobal.power, SRC_RESTART);
      }
    }
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("PWR: PowerOnState %d restored"), Settings->poweronstate);

  // Issue #526 and #909
  uint32_t port = 0;
  for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
#ifdef ESP8266
    if (!Settings->flag3.no_power_feedback &&  // SetOption63 - Don't scan relay power state at restart - #5594 and #5663
        !TasmotaGlobal.power_on_delay          // SetOption47 - Delay switching relays to reduce power surge at power on
#ifdef USE_SHUTTER
        && !Settings->flag3.shutter_mode       // SetOption80 - Enable shutter support
#endif // USE_SHUTTER
       ) {
      if ((port < MAX_RELAYS) && PinUsed(GPIO_REL1, port)) {
        if (bitRead(TasmotaGlobal.rel_bistable, port)) {
          port++;                              // Skip both bistable relays as always 0
        } else {
          bitWrite(TasmotaGlobal.power, i, digitalRead(Pin(GPIO_REL1, port)) ^ bitRead(TasmotaGlobal.rel_inverted, port));
        }
      }
      port++;
    }
#endif  // ESP8266
    if (bitRead(TasmotaGlobal.power, i) || (POWER_ALL_OFF_PULSETIME_ON == Settings->poweronstate)) {
      SetPulseTimer(i % MAX_PULSETIMERS, Settings->pulse_timer[i % MAX_PULSETIMERS]);
    }
  }

  TasmotaGlobal.blink_powersave = TasmotaGlobal.power;
#ifdef USE_RULES
  RulesEvery50ms();
#endif
}

void UpdateLedPowerAll()
{
	for (uint32_t i = 0; i < TasmotaGlobal.leds_present; i++) {
		SetLedPowerIdx(i, bitRead(TasmotaGlobal.led_power, i));
	}
}

void SetLedPowerIdx(uint32_t led, uint32_t state)
{
/*
  // Fix legacy led support 20211016 (Notice: legacy led supports TWO leds max)
  if (!PinUsed(GPIO_LEDLNK) && (0 == led)) {  // Legacy - LED1 is link led only if LED2 is present
    if (PinUsed(GPIO_LED1, 1)) {
      led = 1;
    }
  }
*/
  if (PinUsed(GPIO_LED1, led)) {
    uint32_t mask = 1 << led;
    if (state) {
      state = 1;
      TasmotaGlobal.led_power |= mask;
    } else {
      TasmotaGlobal.led_power &= (0xFF ^ mask);
    }
    uint16_t pwm = 0;
    if (bitRead(Settings->ledpwm_mask, led)) {
#ifdef USE_LIGHT
      pwm = changeUIntScale(ledGamma10(state ? Settings->ledpwm_on : Settings->ledpwm_off), 0, 1023, 0, Settings->pwm_range); // gamma corrected
#else //USE_LIGHT
      pwm = changeUIntScale((uint16_t)(state ? Settings->ledpwm_on : Settings->ledpwm_off), 0, 255, 0, Settings->pwm_range); // linear
#endif //USE_LIGHT
#ifdef ESP32
      if (analogAttach(Pin(GPIO_LED1, led)) >= 0)
#endif
        analogWrite(Pin(GPIO_LED1, led), bitRead(TasmotaGlobal.led_inverted, led) ? Settings->pwm_range - pwm : pwm);
    } else {
      DigitalWrite(GPIO_LED1, led, bitRead(TasmotaGlobal.led_inverted, led) ? !state : state);
    }
  }
#ifdef USE_BUZZER
  if (led == 0) {
    BuzzerSetStateToLed(state);
  }
#endif // USE_BUZZER
}

void SetLedPower(bool state)
{
  if (!PinUsed(GPIO_LEDLNK)) {           // Legacy - Only use LED1 and/or LED2
/*
    SetLedPowerIdx(0, state);
*/
    // Fix legacy led support 20211016 (Notice: legacy led supports TWO leds max)
    uint32_t led = (PinUsed(GPIO_LED1, 1)) ? 1 : 0;
    SetLedPowerIdx(led, state);
  } else {
    power_t mask = 1;
    for (uint32_t i = 0; i < TasmotaGlobal.leds_present; i++) {  // Map leds to power
      bool tstate = (TasmotaGlobal.power & mask);
      SetLedPowerIdx(i, tstate);
      mask <<= 1;
    }
  }
}

void SetLedPowerAll(uint32_t state)
{
  for (uint32_t i = 0; i < TasmotaGlobal.leds_present; i++) {
    SetLedPowerIdx(i, state);
  }
}

void SetLedLink(uint32_t state) {
#ifdef ESP32
  uint32_t index = XdrvMailbox.index;
  XdrvMailbox.index = state;
  XdrvCall(FUNC_LED_LINK);
  XdrvMailbox.index = index;
#endif  // ESP32
  int led_pin = Pin(GPIO_LEDLNK);
  uint32_t led_inv = TasmotaGlobal.ledlnk_inverted;
  if (-1 == led_pin) {                    // Legacy - LED1 is status
    SetLedPowerIdx(0, state);
  }
  else if (led_pin >= 0) {
    if (state) { state = 1; }
    digitalWrite(led_pin, (led_inv) ? !state : state);
  }
#ifdef USE_BUZZER
  BuzzerSetStateToLed(state);
#endif // USE_BUZZER
#ifdef USE_PWM_DIMMER
  if (Settings->flag4.powered_off_led) TasmotaGlobal.restore_powered_off_led_counter = 3;
#endif  // USE_PWM_DIMMER
}

void DebugLed(uint32_t mode) {
  static bool toggle = false;

  if (PinUsed(GPIO_LEDLNK)) {
    if (2 == mode) {
      toggle != toggle;
      mode = toggle;
    }
    digitalWrite(Pin(GPIO_LEDLNK), (TasmotaGlobal.ledlnk_inverted) ? !mode : mode);
  }
}

void SetPulseTimer(uint32_t index, uint32_t time)
{
  TasmotaGlobal.pulse_timer[index] = (time > 111) ? millis() + (1000 * (time - 100)) : (time > 0) ? millis() + (100 * time) : 0L;
}

uint32_t GetPulseTimer(uint32_t index)
{
  long time = TimePassedSince(TasmotaGlobal.pulse_timer[index]);
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
// state 4 = POWER_INCREMENT = button still pressed
// state 5 = POWER_INV = button released
// state 6 = POWER_CLEAR = button released
// state 7 = POWER_RELEASE = button released
// state 9 = CLEAR_RETAIN = clear retain flag
// state 10 = POWER_DELAYED = button released delayed

  char stopic[TOPSZ];
  char scommand[CMDSZ];
  char key_topic[TOPSZ];
  bool result = false;
  uint32_t device_save = device;

  char *tmp = (key) ? SettingsText(SET_MQTT_SWITCH_TOPIC) : SettingsText(SET_MQTT_BUTTON_TOPIC);
  Format(key_topic, tmp, sizeof(key_topic));
  if (Settings->flag.mqtt_enabled && MqttIsConnected() && (strlen(key_topic) != 0) && strcmp(key_topic, "0")) {  // SetOption3 - Enable MQTT
    if (!key && (device > TasmotaGlobal.devices_present)) {
      device = 1;                  // Only allow number of buttons up to number of devices
    }
    GetTopic_P(stopic, CMND, key_topic,
               GetPowerDevice(scommand, device, sizeof(scommand), (key + Settings->flag.device_index_enable)));  // cmnd/switchtopic/POWERx - SetOption26 - Switch between POWER or POWER1
    if (CLEAR_RETAIN == state) {
      ResponseClear();
    } else {
      if ((Settings->flag3.button_switch_force_local ||      // SetOption61 - Force local operation when button/switch topic is set
           !strcmp(TasmotaGlobal.mqtt_topic, key_topic) ||
           !strcmp(SettingsText(SET_MQTT_GRP_TOPIC), key_topic)) &&
          (POWER_TOGGLE == state)) {
        state = ~(TasmotaGlobal.power >> (device -1)) &1;   // POWER_OFF or POWER_ON
      }
      Response_P(GetStateText(state));
    }
#ifdef USE_DOMOTICZ
    if (!(DomoticzSendKey(key, device, state, ResponseLength()))) {
#endif  // USE_DOMOTICZ
      MqttPublish(stopic, ((key) ? Settings->flag.mqtt_switch_retain                         // CMND_SWITCHRETAIN
                                 : Settings->flag.mqtt_button_retain) &&                     // CMND_BUTTONRETAIN
                                 (state != POWER_HOLD || !Settings->flag3.no_hold_retain));  // SetOption62 - Don't use retain flag on HOLD messages
#ifdef USE_DOMOTICZ
    }
#endif  // USE_DOMOTICZ
    result = !Settings->flag3.button_switch_force_local;  // SetOption61 - Force local operation when button/switch topic is set
  } else {
    Response_P(PSTR("{\"%s%d\":{\"State\":%d}}"), (key) ? PSTR("Switch") : PSTR("Button"), device, state);
    result = XdrvRulesProcess(0);
  }
#ifdef USE_PWM_DIMMER
  if (PWM_DIMMER != TasmotaGlobal.module_type || (!result && !Settings->flag3.mqtt_buttons)) {
#endif  // USE_PWM_DIMMER
  int32_t payload_save = XdrvMailbox.payload;
  XdrvMailbox.payload = device_save << 24 | key << 16 | state << 8 | device;
  XdrvCall(FUNC_ANY_KEY);
  XdrvMailbox.payload = payload_save;
#ifdef USE_PWM_DIMMER
    if (PWM_DIMMER == TasmotaGlobal.module_type) result = true;
  }
#endif  // USE_PWM_DIMMER
  return result;
}

void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{
// device  = Relay number 1 and up
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings->pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 3 = POWER_BLINK = Blink relay
// state 4 = POWER_BLINK_STOP = Stop blinking relay
// state 5 = POWER_OFF_FORCE = Relay off even if locked
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

//  ShowSource(source);

//  if (1049 == LANGUAGE_LCID) { return; }

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) {
    TasmotaGlobal.blink_mask &= 1;       // No blinking on the fan relays
    Settings->flag.interlock = 0;        // No interlock mode as it is already done by the microcontroller - CMND_INTERLOCK - Enable/disable interlock
    Settings->pulse_timer[1] = 0;        // No pulsetimers on the fan relays
    Settings->pulse_timer[2] = 0;
    Settings->pulse_timer[3] = 0;
  }
#endif  // USE_SONOFF_IFAN

  bool force_power_off = false;
  if (POWER_OFF_FORCE == state) {
    force_power_off = true;
    state = POWER_OFF;
  }

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                          // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }

  if ((device < 1) || (device > TasmotaGlobal.devices_present)) {
    device = 1;
  }
  TasmotaGlobal.active_device = device;

  if (!force_power_off && bitRead(Settings->power_lock, device -1)) {
    AddLog(LOG_LEVEL_INFO, PSTR("CMD: Power%d is LOCKED"), device);
    state = POWER_SHOW_STATE;            // Only show state. Make no change
  }

  if (state != POWER_SHOW_STATE) {
    SetPulseTimer((device -1) % MAX_PULSETIMERS, 0);
  }

  static bool interlock_mutex = false;   // Interlock power command pending
  power_t mask = 1 << (device -1);       // Device to control
  if (state <= POWER_TOGGLE) {
    if ((TasmotaGlobal.blink_mask & mask)) {
      TasmotaGlobal.blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      MqttPublishPowerBlinkState(device);
    }

    if (Settings->flag.interlock &&      // CMND_INTERLOCK - Enable/disable interlock
        !interlock_mutex &&
        ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(TasmotaGlobal.power & mask)))
       ) {
      interlock_mutex = true;            // Clear all but masked relay in interlock group if new set requested
      bool perform_interlock_delay = false;
      for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
        if (Settings->interlock[i] & mask) {  // Find interlock group
          for (uint32_t j = 0; j < TasmotaGlobal.devices_present; j++) {
            power_t imask = 1 << j;
            if ((Settings->interlock[i] & imask) && (TasmotaGlobal.power & imask) && (mask != imask)) {
              ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
              perform_interlock_delay = true;
            }
          }
          break;                         // An interlocked relay is only present in one group so quit
        }
      }
      if (perform_interlock_delay) {
        delay(50);                       // Add some delay to make sure never have more than one relay on
      }
      interlock_mutex = false;
    }

#ifdef USE_DEVICE_GROUPS
    power_t old_power = TasmotaGlobal.power;
#endif  // USE_DEVICE_GROUPS
    switch (state) {
    case POWER_OFF: {
      TasmotaGlobal.power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      TasmotaGlobal.power |= mask;
      break;
    case POWER_TOGGLE:
      TasmotaGlobal.power ^= mask;
    }
#ifdef USE_DEVICE_GROUPS
    if (TasmotaGlobal.power != old_power && SRC_REMOTE != source && SRC_RETRY != source) {
      power_t dgr_power = TasmotaGlobal.power;
      if (Settings->flag4.multiple_device_groups) {  // SetOption88 - Enable relays in separate device groups
        dgr_power = (dgr_power >> (device - 1)) & 1;
      }
      SendDeviceGroupMessage(device, DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, dgr_power);
    }
#endif  // USE_DEVICE_GROUPS
    SetDevicePower(TasmotaGlobal.power, source);
#ifdef USE_DOMOTICZ
    DomoticzUpdatePowerState(device);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    KnxUpdatePowerState(device, TasmotaGlobal.power);
#endif  // USE_KNX
    if (publish_power && Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
      MqttPublishTeleState();
    }

    // Restart PulseTime if powered On
    SetPulseTimer((device -1) % MAX_PULSETIMERS, (((POWER_ALL_OFF_PULSETIME_ON == Settings->poweronstate) ? ~TasmotaGlobal.power : TasmotaGlobal.power) & mask) ? Settings->pulse_timer[(device -1) % MAX_PULSETIMERS] : 0);
  }
  else if (POWER_BLINK == state) {
    if (!(TasmotaGlobal.blink_mask & mask)) {
      TasmotaGlobal.blink_powersave = (TasmotaGlobal.blink_powersave & (POWER_MASK ^ mask)) | (TasmotaGlobal.power & mask);  // Save state
      TasmotaGlobal.blink_power = (TasmotaGlobal.power >> (device -1))&1;  // Prep to Toggle
    }
    TasmotaGlobal.blink_timer = millis() + 100;
    TasmotaGlobal.blink_counter = ((!Settings->blinkcount) ? 64000 : (Settings->blinkcount *2)) +1;
    TasmotaGlobal.blink_mask |= mask;    // Set device mask
    MqttPublishPowerBlinkState(device);
    return;
  }
  else if (POWER_BLINK_STOP == state) {
    bool flag = (TasmotaGlobal.blink_mask & mask);
    TasmotaGlobal.blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
    MqttPublishPowerBlinkState(device);
    if (flag) {
      ExecuteCommandPower(device, (TasmotaGlobal.blink_powersave >> (device -1))&1, SRC_IGNORE);  // Restore state
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

  for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {
    mask = 1 << (i -1);
    if (TasmotaGlobal.blink_mask & mask) {
      TasmotaGlobal.blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      MqttPublishPowerBlinkState(i);
      ExecuteCommandPower(i, (TasmotaGlobal.blink_powersave >> (i -1))&1, SRC_IGNORE);  // Restore state
    }
  }
}

void MqttShowState(void)
{
  char stemp1[TOPSZ];

  ResponseAppendTime();
  ResponseAppend_P(PSTR(",\"" D_JSON_UPTIME "\":\"%s\",\"UptimeSec\":%u"), GetUptime().c_str(), UpTime());

  // Battery Level expliciet for deepsleep devices
  if (Settings->battery_level_percent != 101) {
    ResponseAppend_P(PSTR(",\"" D_CMND_ZIGBEE_BATTPERCENT "\":%d"), Settings->battery_level_percent);
  }

#ifdef ESP8266
#ifdef USE_ADC_VCC
  dtostrfd((double)ESP.getVcc()/1000, 3, stemp1);
  ResponseAppend_P(PSTR(",\"" D_JSON_VCC "\":%s"), stemp1);
#endif  // USE_ADC_VCC
#endif  // ESP8266

  ResponseAppend_P(PSTR(",\"" D_JSON_HEAPSIZE "\":%d,\"SleepMode\":\"%s\",\"Sleep\":%u,\"LoadAvg\":%u,\"MqttCount\":%u"),
    ESP_getFreeHeap1024(), GetTextIndexed(stemp1, sizeof(stemp1), Settings->flag3.sleep_normal, kSleepMode),  // SetOption60 - Enable normal sleep instead of dynamic sleep
    TasmotaGlobal.sleep, TasmotaGlobal.loop_load_avg, MqttConnectCount());

#ifdef USE_BERRY
    extern void BrShowState(void);
    BrShowState();
#endif // USE_BERRY

  for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {
#ifdef USE_LIGHT
    if ((LightDevice()) && (i >= LightDevice())) {
      if (i == LightDevice())  { ResponseLightState(1); }    // call it only once
    } else {
#endif
      ResponseAppend_P(PSTR(",\"%s\":\"%s\""), GetPowerDevice(stemp1, i, sizeof(stemp1), Settings->flag.device_index_enable),  // SetOption26 - Switch between POWER or POWER1
                                               GetStateText(bitRead(TasmotaGlobal.power, i-1)));
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

  if (TasmotaGlobal.pwm_present) {
    ResponseAppend_P(PSTR(","));
    MqttShowPWMState();
  }
  
  char *hostname = TasmotaGlobal.hostname;
  uint32_t ipaddress = 0;
#if defined(ESP32) && defined(USE_ETHERNET)
  if (static_cast<uint32_t>(EthernetLocalIP()) != 0) {
    hostname = EthernetHostname();           // Set ethernet as IP connection
    ipaddress = (uint32_t)EthernetLocalIP();
  }
#endif

  if (!TasmotaGlobal.global_state.wifi_down) {
    int32_t rssi = WiFi.RSSI();
    ResponseAppend_P(PSTR(",\"" D_JSON_WIFI "\":{\"" D_JSON_AP "\":%d,\"" D_JSON_SSID "\":\"%s\",\"" D_JSON_BSSID "\":\"%s\",\"" D_JSON_CHANNEL "\":%d,\"" D_JSON_WIFI_MODE "\":\"%s\",\"" D_JSON_RSSI "\":%d,\"" D_JSON_SIGNAL "\":%d,\"" D_JSON_LINK_COUNT "\":%d,\"" D_JSON_DOWNTIME "\":\"%s\"}"),
      Settings->sta_active +1, EscapeJSONString(SettingsText(SET_STASSID1 + Settings->sta_active)).c_str(), WiFi.BSSIDstr().c_str(), WiFi.channel(),
      WifiGetPhyMode().c_str(), WifiGetRssiAsQuality(rssi), rssi,
      WifiLinkCount(), WifiDowntime().c_str());

    if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
      hostname = TasmotaGlobal.hostname;     // Overrule ethernet as primary IP connection
      ipaddress = (uint32_t)WiFi.localIP();
    }
  }
  // I only want to show one active connection for device access
  ResponseAppend_P(PSTR(",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%_I\""),
    hostname, ipaddress);

  ResponseJsonEnd();
}

void MqttPublishTeleState(void)
{
  ResponseClear();
  MqttShowState();
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), Settings->flag5.mqtt_state_retain);
  XdrvRulesProcess(1);
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

String GetSwitchText(uint32_t i) {
  String switch_text = "";
  if (i < MAX_SWITCHES_TXT) {
    switch_text = SettingsText(SET_SWITCH_TXT1 + i);
  }
  if ('\0' == switch_text[0]) {
    switch_text = F(D_JSON_SWITCH);
    switch_text += String(i+1);
  }
  return switch_text;
}

const char kGlobalValues[] PROGMEM = D_JSON_TEMPERATURE "|" D_JSON_HUMIDITY "|" D_JSON_PRESSURE;

void GetSensorValues(void) {
  char *start = ResponseData();
  int data_start = ResponseLength();

  XsnsXdrvCall(FUNC_JSON_APPEND);

  if (data_start == ResponseLength()) { return; }

  for (uint32_t type = 0; type < 3; type++) {
    if (!Settings->global_sensor_index[type] || TasmotaGlobal.user_globals[type]) { continue; }

    char key[20];
    GetTextIndexed(key, sizeof(key), type, kGlobalValues);

    float value = -9999;
    uint32_t idx = 0;
//    char *data = ResponseData();
    char *data = start;  // Invalid JSON ,"HTU21":{"Temperature":30.7,"Humidity":39.0,"DewPoint":15.2},"BME680":{"Temperature":30.0,"Humidity":50.4,"DewPoint":18.5,"Pressure":1009.6,"Gas":1660.52},"ESP32":{"Temperature":53.3}
    while (data) {
      data = strstr(data, key);
      if (data) {
        idx++;
        data += strlen(key) + 2;
        float new_value = CharToFloat(data);
        if (1 == idx) { value = new_value; }

//        AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: %s value%d = %2_f"), key, idx, &new_value);

        if (idx == Settings->global_sensor_index[type]) {
          value = new_value;
          break;
        }
      }
    }
    if (value != -9999) {
      switch (type) {
        case 0:  // Temperature
          TasmotaGlobal.temperature_celsius = ConvertTempToCelsius(value);
          break;
        case 1:  // Humidity
          TasmotaGlobal.humidity = value;
          break;
        case 2:  // Pressure
          TasmotaGlobal.pressure_hpa = ConvertHgToHpa(value);
          break;
      }
      TasmotaGlobal.global_update = TasmotaGlobal.uptime;
    }
  }
}

void MqttAppendSensorUnits(void)
{
  if (ResponseContains_P(PSTR(D_JSON_PRESSURE))) {
    ResponseAppend_P(PSTR(",\"" D_JSON_PRESSURE_UNIT "\":\"%s\""), PressureUnit().c_str());
  }
  if (ResponseContains_P(PSTR(D_JSON_TEMPERATURE))) {
    ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE_UNIT "\":\"%c\""), TempUnit());
  }
  if (ResponseContains_P(PSTR(D_JSON_SPEED)) && Settings->flag2.speed_conversion) {
    ResponseAppend_P(PSTR(",\"" D_JSON_SPEED_UNIT "\":\"%s\""), SpeedUnit().c_str());
  }
}

bool MqttShowSensor(bool call_show_sensor) {
  ResponseAppendTime();

  int json_data_start = ResponseLength();
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (SwitchUsed(i)) {
      ResponseAppend_P(PSTR(",\"%s\":\"%s\""), GetSwitchText(i).c_str(), GetStateText(SwitchState(i)));
    }
  }

  GetSensorValues();

#ifndef FIRMWARE_MINIMAL
  if (TasmotaGlobal.global_update && Settings->flag.mqtt_add_global_info) {  // SetOption2 (MQTT) Add global temperature/humidity/pressure info to JSON sensor message
    if ((TasmotaGlobal.humidity > 0) || !isnan(TasmotaGlobal.temperature_celsius) || (TasmotaGlobal.pressure_hpa != 0)) {
      uint32_t add_comma = 0;
      ResponseAppend_P(PSTR(",\"Global\":{"));
      if (!isnan(TasmotaGlobal.temperature_celsius)) {
        float t = ConvertTempToFahrenheit(TasmotaGlobal.temperature_celsius);
        ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%*_f"),
          Settings->flag2.temperature_resolution, &t);
        add_comma++;
      }
      if (TasmotaGlobal.humidity > 0) {
        ResponseAppend_P(PSTR("%s\"" D_JSON_HUMIDITY "\":%*_f"),
          (add_comma)?",":"", Settings->flag2.humidity_resolution, &TasmotaGlobal.humidity);
        add_comma++;
      }
      if (2 == add_comma) {
        float dewpoint = CalcTempHumToDew(TasmotaGlobal.temperature_celsius, TasmotaGlobal.humidity);
        ResponseAppend_P(PSTR(",\"" D_JSON_DEWPOINT "\":%*_f"),
          Settings->flag2.temperature_resolution, &dewpoint);
#ifdef USE_HEAT_INDEX
        float heatindex = CalcTemHumToHeatIndex(TasmotaGlobal.temperature_celsius, TasmotaGlobal.humidity);
        ResponseAppend_P(PSTR(",\"" D_JSON_HEATINDEX "\":%*_f"),
          Settings->flag2.temperature_resolution, &heatindex);
#endif  // USE_HEAT_INDEX
      }
      if (TasmotaGlobal.pressure_hpa != 0) {
        float p = ConvertPressure(TasmotaGlobal.pressure_hpa);
        float s = ConvertPressureForSeaLevel(TasmotaGlobal.pressure_hpa);
        ResponseAppend_P(PSTR("%s\"" D_JSON_PRESSURE "\":%*_f,\"" D_JSON_PRESSUREATSEALEVEL "\":%*_f"),
          (add_comma)?",":"", Settings->flag2.pressure_resolution, &p, Settings->flag2.pressure_resolution, &s);
      }
      ResponseJsonEnd();
    }
  }
#endif // FIRMWARE_MINIMAL

  bool json_data_available = (ResponseLength() - json_data_start);
  MqttAppendSensorUnits();
  ResponseJsonEnd();

  if (call_show_sensor && json_data_available) { XdrvCall(FUNC_SHOW_SENSOR); }
  return json_data_available;
}

void MqttPublishSensor(void) {
  ResponseClear();
  if (MqttShowSensor(true)) {
    MqttPublishTeleSensor();
  }
}

void MqttPublishTeleperiodSensor(void) {
  ResponseClear();
  if (MqttShowSensor(true)) {
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
    XdrvRulesProcess(1);
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
  TasmotaGlobal.uptime++;

  if (POWER_CYCLE_TIME == TasmotaGlobal.uptime) {
    UpdateQuickPowerCycle(false);
  }

  if (BOOT_LOOP_TIME == TasmotaGlobal.uptime) {
    RtcRebootReset();

    Settings->last_module = Settings->module;


#ifdef USE_DEEPSLEEP
    if (!(DeepSleepEnabled() && !Settings->flag3.bootcount_update)) {  // SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
#endif
      Settings->bootcount++;              // Moved to here to stop flash writes during start-up
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " %d"), Settings->bootcount);
#ifdef USE_DEEPSLEEP
    }
#endif
  }

  if (TasmotaGlobal.power_on_delay) {
    if (1 == Settings->param[P_POWER_ON_DELAY2]) {       // SetOption47 1
      // Allow relay power on once network is available
      if (!TasmotaGlobal.global_state.network_down) {
        TasmotaGlobal.power_on_delay = 0;
      }
    }
    else if (2 == Settings->param[P_POWER_ON_DELAY2]) {  // SetOption47 2
      // Allow relay power on once mqtt is available
      if (!TasmotaGlobal.global_state.mqtt_down) {
        TasmotaGlobal.power_on_delay = 0;
      }
    }
    else {                                               // SetOption47 3..255
      // Allow relay power on after x seconds
      TasmotaGlobal.power_on_delay--;
    }
    if (!TasmotaGlobal.power_on_delay && TasmotaGlobal.power_on_delay_state) {
      // Set relays according to last SetDevicePower() request
      SetDevicePower(TasmotaGlobal.power_on_delay_state, SRC_SO47);
    }
  }

  if (TasmotaGlobal.mqtt_cmnd_blocked_reset) {
    TasmotaGlobal.mqtt_cmnd_blocked_reset--;
    if (!TasmotaGlobal.mqtt_cmnd_blocked_reset) {
      TasmotaGlobal.mqtt_cmnd_blocked = 0;             // Clean up MQTT cmnd loop block
    }
  }

  if (TasmotaGlobal.seriallog_timer) {
    TasmotaGlobal.seriallog_timer--;
    if (!TasmotaGlobal.seriallog_timer) {
      if (TasmotaGlobal.seriallog_level) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
      }
      TasmotaGlobal.seriallog_level = 0;
    }
  }

  if (TasmotaGlobal.syslog_timer) {  // Restore syslog level
    TasmotaGlobal.syslog_timer--;
    if (!TasmotaGlobal.syslog_timer) {
      TasmotaGlobal.syslog_level = Settings->syslog_level;
      if (Settings->syslog_level) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
      }
    }
  }

  MqttPublishLoggingAsync(false);
#ifdef SYSLOG_UPDATE_SECOND
  SyslogAsync(false);
#endif  // SYSLOG_UPDATE_SECOND
#ifdef USE_UFILESYS
  FileLoggingAsync(false);
#endif  // USE_UFILESYS

  ResetGlobalValues();

  if ((TasmotaGlobal.init_state >= INIT_GPIOS) && PinUsed(GPIO_HEARTBEAT)) {
    digitalWrite(Pin(GPIO_HEARTBEAT), ~TasmotaGlobal.heartbeat_inverted &1);
    delayMicroseconds(50);
    digitalWrite(Pin(GPIO_HEARTBEAT), TasmotaGlobal.heartbeat_inverted);
  }

  // Teleperiod
  if (Settings->tele_period || (3601 == TasmotaGlobal.tele_period)) {
    if (TasmotaGlobal.tele_period >= 9999) {
      if (!TasmotaGlobal.global_state.network_down) {
        TasmotaGlobal.tele_period = 0;  // Allow teleperiod once wifi is connected
      }
    } else {
      TasmotaGlobal.tele_period++;
      if (TasmotaGlobal.tele_period >= Settings->tele_period) {
        TasmotaGlobal.tele_period = 0;

        MqttPublishTeleState();
        MqttPublishTeleperiodSensor();

        XsnsXdrvCall(FUNC_AFTER_TELEPERIOD);
      } else {
        // Global values (Temperature, Humidity and Pressure) update every 10 seconds
        if (!(TasmotaGlobal.tele_period % 10)) {
          for (uint32_t type = 0; type < 3; type++) {
            if (!Settings->global_sensor_index[type] || TasmotaGlobal.user_globals[type]) { continue; }
            GetSensorValues();
            break;
          }
        }
      }
    }
  }

#ifdef ESP8266
  // Wifi keep alive to send Gratuitous ARP
  wifiKeepAlive();
#endif

  WifiPollNtp();

#ifdef ESP32
  if (11 == TasmotaGlobal.uptime) {  // Perform one-time ESP32 houskeeping
    ESP_getSketchSize();             // Init sketchsize as it can take up to 2 seconds
  }
#endif

#ifdef USE_UFILESYS
  static bool settings_lkg = false;  // Settings saved as Last Known Good
  // Copy Settings as Last Known Good if no changes have been saved since 30 minutes
  if (!settings_lkg && (UtcTime() > START_VALID_TIME) && (Settings->cfg_timestamp < UtcTime() - (30 * 60))) {
    TfsSaveFile(TASM_FILE_SETTINGS_LKG, (const uint8_t*)Settings, sizeof(TSettings));
    settings_lkg = true;
  }
#endif
}

/*-------------------------------------------------------------------------------------------*\
 * Every 0.1 second
\*-------------------------------------------------------------------------------------------*/

void Every100mSeconds(void)
{
  // As the max amount of sleep = 250 mSec this loop will shift in time...
  power_t power_now;

  if (TasmotaGlobal.latching_relay_pulse) {
    TasmotaGlobal.latching_relay_pulse--;
    if (!TasmotaGlobal.latching_relay_pulse) {
#ifdef ESP8266
      if (EXS_RELAY == TasmotaGlobal.module_type) {
        SetLatchingRelay(0, 0);
      }
#endif  // ESP8266
    }
  }

  for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
    if (TasmotaGlobal.pulse_timer[i] != 0L) {           // Timer active?
      if (TimeReached(TasmotaGlobal.pulse_timer[i])) {  // Timer finished?
        TasmotaGlobal.pulse_timer[i] = 0L;              // Turn off this timer
        for (uint32_t j = 0; (i + j) < TasmotaGlobal.devices_present; j = j +MAX_PULSETIMERS) {
          ExecuteCommandPower(i + j +1, (POWER_ALL_OFF_PULSETIME_ON == Settings->poweronstate) ? POWER_ON : POWER_OFF, SRC_PULSETIMER);
        }
      }
    }
  }

  if (TasmotaGlobal.blink_mask) {
    if (TimeReached(TasmotaGlobal.blink_timer)) {
      SetNextTimeInterval(TasmotaGlobal.blink_timer, 100 * Settings->blinktime);
      TasmotaGlobal.blink_counter--;
      if (!TasmotaGlobal.blink_counter) {
        StopAllPowerBlink();
      } else {
        TasmotaGlobal.blink_power ^= 1;
        power_now = (TasmotaGlobal.power & (POWER_MASK ^ TasmotaGlobal.blink_mask)) | ((TasmotaGlobal.blink_power) ? TasmotaGlobal.blink_mask : 0);
        SetDevicePower(power_now, SRC_IGNORE);
      }
    }
  }

  WiFiSetTXpowerBasedOnRssi();
}

/*-------------------------------------------------------------------------------------------*\
 * Every 0.25 second
\*-------------------------------------------------------------------------------------------*/

bool CommandsReady(void) {
  bool ready = BACKLOG_EMPTY ;
#ifdef USE_UFILESYS
  ready |= UfsExecuteCommandFileReady();
#endif  // USE_UFILESYS
  return ready;
}

void Every250mSeconds(void)
{
// As the max amount of sleep = 250 mSec this loop should always be taken...

  static uint8_t blinkspeed = 1;                          // LED blink rate
  uint32_t blinkinterval = 1;

  TasmotaGlobal.state_250mS++;
  TasmotaGlobal.state_250mS &= 0x3;

  TasmotaGlobal.global_state.network_down = (TasmotaGlobal.global_state.wifi_down && TasmotaGlobal.global_state.eth_down) ? 1 : 0;

  if (!Settings->flag.global_state) {                      // SetOption31 - Control link led blinking
    if (TasmotaGlobal.global_state.data &0x03) {                        // Network or MQTT problem
      if (TasmotaGlobal.global_state.mqtt_down) { blinkinterval = 7; }  // MQTT problem so blink every 2 seconds (slowest)
      if (TasmotaGlobal.global_state.network_down) { blinkinterval = 3; }  // Network problem so blink every second (slow)
      TasmotaGlobal.blinks = 201;                         // Allow only a single blink in case the problem is solved
    }
  }
  if (TasmotaGlobal.blinks || TasmotaGlobal.restart_flag || TasmotaGlobal.ota_state_flag) {
    if (TasmotaGlobal.restart_flag || TasmotaGlobal.ota_state_flag) {  // Overrule blinks and keep led lit
      TasmotaGlobal.blinkstate = true;                                  // Stay lit
    } else {
      blinkspeed--;
      if (!blinkspeed) {
        blinkspeed = blinkinterval;                       // Set interval to 0.2 (default), 1 or 2 seconds
        TasmotaGlobal.blinkstate ^= 1;                                  // Blink
      }
    }
    if ((!(Settings->ledstate &0x08)) && ((Settings->ledstate &0x06) || (TasmotaGlobal.blinks > 200) || (TasmotaGlobal.blinkstate))) {
      SetLedLink(TasmotaGlobal.blinkstate);                            // Set led on or off
    }
    if (!TasmotaGlobal.blinkstate) {
      TasmotaGlobal.blinks--;
      if (200 == TasmotaGlobal.blinks) { TasmotaGlobal.blinks = 0; }  // Disable blink
    }
  }
  if (Settings->ledstate &1 && (PinUsed(GPIO_LEDLNK) || !(TasmotaGlobal.blinks || TasmotaGlobal.restart_flag || TasmotaGlobal.ota_state_flag)) ) {
    bool tstate = TasmotaGlobal.power & Settings->ledmask;
#ifdef ESP8266
    if ((SONOFF_TOUCH == TasmotaGlobal.module_type) || (SONOFF_T11 == TasmotaGlobal.module_type) || (SONOFF_T12 == TasmotaGlobal.module_type) || (SONOFF_T13 == TasmotaGlobal.module_type)) {
      tstate = (!TasmotaGlobal.power) ? 1 : 0;           // As requested invert signal for Touch devices to find them in the dark
    }
#endif  // ESP8266
    SetLedPower(tstate);
  }

  // Check if log refresh needed in case of fast buffer fill
  MqttPublishLoggingAsync(true);
  SyslogAsync(true);
#ifdef USE_UFILESYS
  FileLoggingAsync(true);
#endif  // USE_UFILESYS

/*-------------------------------------------------------------------------------------------*\
 * Every second at 0.25 second interval
\*-------------------------------------------------------------------------------------------*/

  static int ota_result = 0;
  static uint8_t ota_retry_counter = OTA_ATTEMPTS;

  switch (TasmotaGlobal.state_250mS) {
  case 0:                                                 // Every x.0 second
    if (TasmotaGlobal.ota_state_flag && CommandsReady()) {
      TasmotaGlobal.ota_state_flag--;
      if (2 == TasmotaGlobal.ota_state_flag) {
//#ifdef CONFIG_IDF_TARGET_ESP32C3
#ifdef ESP32
        OtaFactoryWrite(false);
#endif
        RtcSettings.ota_loader = 0;                       // Try requested image first
        ota_retry_counter = OTA_ATTEMPTS;
        SettingsSave(1);                                  // Free flash for OTA update
      }
      if (TasmotaGlobal.ota_state_flag <= 0) {
        AllowInterrupts(0);
#ifdef USE_WEBSERVER
//        if (Settings->webserver) StopWebserver();       // 20230107 No more need for disabling webserver during OTA
#endif  // USE_WEBSERVER

        TasmotaGlobal.ota_state_flag = 92;
        ota_result = 0;
        char full_ota_url[200];
        ota_retry_counter--;
        if (ota_retry_counter) {
          char ota_url[TOPSZ];
          strlcpy(full_ota_url, GetOtaUrl(ota_url, sizeof(ota_url)), sizeof(full_ota_url));
#ifdef ESP8266
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

            char *bch = strrchr(full_ota_url, '/');         // Only consider filename after last backslash prevent change of urls having "-" in it
            if (bch == nullptr) { bch = full_ota_url; }     // No path found so use filename only
            char *ech = strchr(bch, '.');                              // Find file type in filename (none, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz)
            if (ech == nullptr) { ech = full_ota_url + strlen(full_ota_url); }  // Point to '/0' at end of full_ota_url becoming an empty string

//AddLog(LOG_LEVEL_DEBUG, PSTR("OTA: File type [%s]"), ech);

            char ota_url_type[strlen(ech) +1];
            strncpy(ota_url_type, ech, sizeof(ota_url_type));          // Either empty, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz

            char *pch = strrchr(bch, '-');                             // Find last dash (-) and ignore remainder - handles tasmota-DE
            if (pch == nullptr) { pch = ech; }                         // No dash so ignore filetype
            *pch = '\0';                                               // full_ota_url = http://domus1:80/api/arduino/tasmota
            snprintf_P(full_ota_url, sizeof(full_ota_url), PSTR("%s-" D_JSON_MINIMAL "%s"), full_ota_url, ota_url_type);  // Minimal filename must be filename-minimal
          }
#endif  // FIRMWARE_MINIMAL
          if (ota_retry_counter < OTA_ATTEMPTS / 2) {
            if (StrCaseStr_P(full_ota_url, PSTR(".gz"))) {
              ota_retry_counter = 1;
            } else {
              strcat_P(full_ota_url, PSTR(".gz"));
            }
          }
#endif  // ESP8266

#ifdef ESP32
#ifndef FIRMWARE_MINIMAL
#ifdef USE_WEBCLIENT_HTTPS
          if (TasmotaGlobal.ota_factory) {
            char *bch = strrchr(full_ota_url, '/');            // Only consider filename after last backslash prevent change of urls having "-" in it
            if (bch == nullptr) { bch = full_ota_url; }        // No path found so use filename only
            char *ech = strchr(bch, '.');                      // Find file type in filename (none, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz)
            if (ech == nullptr) { ech = full_ota_url + strlen(full_ota_url); }  // Point to '/0' at end of full_ota_url becoming an empty string
            char ota_url_type[strlen(ech) +1];
            strncpy(ota_url_type, ech, sizeof(ota_url_type));  // Either empty, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz

            char *pch = strrchr(bch, '-');                     // Find last dash (-) and ignore remainder - handles tasmota-DE
            if (pch == nullptr) { pch = ech; }                 // No dash so ignore filetype
            *pch = '\0';                                       // full_ota_url = http://domus1:80/api/arduino/tasmota
            snprintf_P(full_ota_url, sizeof(full_ota_url), PSTR("%s-safeboot%s"), full_ota_url, ota_url_type);  // Safeboot filename must be filename-safeboot
          } else
#endif  // USE_WEBCLIENT_HTTPS
          if (EspSingleOtaPartition()) {
//#ifdef CONFIG_IDF_TARGET_ESP32C3
#ifdef ESP32
            OtaFactoryWrite(true);
#endif
            RtcSettings.ota_loader = 1;                 // Try safeboot image next
            XsnsXdrvCall(FUNC_ABOUT_TO_RESTART);
            SettingsSaveAll();
            AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
            EspPrepRestartToSafeBoot();
            EspRestart();
          }
#endif  // FIRMWARE_MINIMAL
#endif  // ESP32

          char version[50];
          snprintf_P(version, sizeof(version), PSTR("%s%s"), TasmotaGlobal.version, TasmotaGlobal.image_name);
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "%s %s"), full_ota_url, version);
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
          HTTPClientLight OTAclient;
          if (!OTAclient.begin(full_ota_url)) {
            AddLog(LOG_LEVEL_INFO, "OTA: unsupported protocol");
            ota_result = -999;
          } else {
            httpUpdateLight.rebootOnUpdate(false);
            httpUpdateLight.setFactory(TasmotaGlobal.ota_factory);
            ota_result = (HTTP_UPDATE_FAILED != httpUpdateLight.update(OTAclient, version));
          }
#else // standard OTA over HTTP
          WiFiClient OTAclient;
          ESPhttpUpdate.rebootOnUpdate(false);
          ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(OTAclient, full_ota_url, version));
#endif
          if (!ota_result) {
#ifndef FIRMWARE_MINIMAL
            int ota_error = ESPhttpUpdate.getLastError();
            DEBUG_CORE_LOG(PSTR("OTA: Error %d"), ota_error);
#ifdef ESP8266
            if ((HTTP_UE_TOO_LESS_SPACE == ota_error) || (HTTP_UE_BIN_FOR_WRONG_FLASH == ota_error)) {
              RtcSettings.ota_loader = 1;                 // Try minimal image next
            }
#endif  // ESP8266
#endif  // FIRMWARE_MINIMAL
            TasmotaGlobal.ota_state_flag = 2;             // Upgrade failed - retry
          }
        }
      }
      if (90 == TasmotaGlobal.ota_state_flag) {           // Allow MQTT to reconnect
        TasmotaGlobal.ota_state_flag = 0;
        Response_P(PSTR("{\"" D_CMND_UPGRADE "\":\""));
        if (ota_result) {
          ResponseAppend_P(PSTR(D_JSON_SUCCESSFUL ". " D_JSON_RESTARTING));
          TasmotaGlobal.restart_flag = 5;                 // Allow time for webserver to update console
        } else {
          ResponseAppend_P(PSTR(D_JSON_FAILED " %s"), ESPhttpUpdate.getLastErrorString().c_str());
        }
        ResponseAppend_P(PSTR("\"}"));
        MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR(D_CMND_UPGRADE));
        AllowInterrupts(1);
      }
    }
    break;
  case 1:                                                 // Every x.25 second
    if (MidnightNow()) {
      XsnsCall(FUNC_SAVE_AT_MIDNIGHT);
    }

    if (TasmotaGlobal.save_data_counter && CommandsReady()) {
      TasmotaGlobal.save_data_counter--;
      if (TasmotaGlobal.save_data_counter <= 0) {
        if (Settings->flag.save_state) {                   // SetOption0 - Save power state and use after restart
          power_t mask = POWER_MASK;
          for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
            if ((Settings->pulse_timer[i % MAX_PULSETIMERS] > 0) && (Settings->pulse_timer[i % MAX_PULSETIMERS] < 30)) {  // 3 seconds
              mask &= ~(1 << i);
            }
          }
          if (!((Settings->power &mask) == (TasmotaGlobal.power &mask))) {
            Settings->power = TasmotaGlobal.power;
          }
        } else {
          Settings->power = 0;
        }
        if (!TasmotaGlobal.restart_flag) { SettingsSave(0); }
        TasmotaGlobal.save_data_counter = Settings->save_data;
      }
    }

    if (TasmotaGlobal.restart_flag && CommandsReady()) {
      if ((214 == TasmotaGlobal.restart_flag) ||          // Reset 4
          (215 == TasmotaGlobal.restart_flag) ||          // Reset 5
          (216 == TasmotaGlobal.restart_flag)) {          // Reset 6
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
        uint16_t mqtt_port = Settings->mqtt_port;

//        if (216 == TasmotaGlobal.restart_flag) {
          // Backup mqtt host, port, client, username and password
//        }
        if ((215 == TasmotaGlobal.restart_flag) ||        // Reset 5
            (216 == TasmotaGlobal.restart_flag)) {        // Reset 6
          SettingsErase(2);  // Erase all flash from program end to end of physical excluding optional filesystem
        }
        SettingsDefault();
        // Restore current SSIDs and Passwords
        SettingsUpdateText(SET_STASSID1, storage_ssid1);
        SettingsUpdateText(SET_STASSID2, storage_ssid2);
        SettingsUpdateText(SET_STAPWD1, storage_pass1);
        SettingsUpdateText(SET_STAPWD2, storage_pass2);
        if (216 == TasmotaGlobal.restart_flag) {          // Reset 6
          // Restore the mqtt host, port, client, username and password
          SettingsUpdateText(SET_MQTT_HOST, storage_mqtthost);
          SettingsUpdateText(SET_MQTT_USER, storage_mqttuser);
          SettingsUpdateText(SET_MQTT_PWD, storage_mqttpwd);
          SettingsUpdateText(SET_MQTT_TOPIC, storage_mqtttopic);
          Settings->mqtt_port = mqtt_port;
        }

        XdrvCall(FUNC_RESET_SETTINGS);

        TasmotaGlobal.restart_flag = 3;                   // Finish backlog then Restart 1
      }
      else if (213 == TasmotaGlobal.restart_flag) {       // Reset 3
        SettingsSdkErase();  // Erase flash SDK parameters
        TasmotaGlobal.restart_flag = 2;                   // Restart 1
      }
      else if (212 == TasmotaGlobal.restart_flag) {       // Reset 2
        SettingsErase(0);    // Erase all flash from program end to end of physical flash
        TasmotaGlobal.restart_flag = 211;                 // Reset 1
      }

      if (211 == TasmotaGlobal.restart_flag) {            // Reset 1
        SettingsDefault();
        TasmotaGlobal.restart_flag = 3;                   // Finish backlog then Restart 1
      }

      if (2 == TasmotaGlobal.restart_flag) {              // Restart 1
        XsnsXdrvCall(FUNC_ABOUT_TO_RESTART);
        SettingsSaveAll();
      }

      TasmotaGlobal.restart_flag--;
      if (TasmotaGlobal.restart_flag <= 0) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION "%s"), (TasmotaGlobal.restart_halt) ? PSTR("Halted") : (TasmotaGlobal.restart_deepsleep) ? PSTR("Sleeping") : PSTR(D_RESTARTING));
        EspRestart();
      }
    }
    break;
  case 2:                                                 // Every x.5 second
    if (Settings->flag4.network_wifi) {
      WifiCheck(TasmotaGlobal.wifi_state_flag);
      TasmotaGlobal.wifi_state_flag = WIFI_RESTART;
    } else {
      WifiDisable();
    }
    break;
  case 3:
    {
      if (!TasmotaGlobal.global_state.network_down) {
#ifdef FIRMWARE_MINIMAL
//#ifdef CONFIG_IDF_TARGET_ESP32C3
#ifdef ESP32
        if (OtaFactoryRead()) {
          OtaFactoryWrite(false);
          RtcSettings.ota_loader = 1;
        }
#endif
        if (1 == RtcSettings.ota_loader) {
          RtcSettings.ota_loader = 0;
          AddLog(LOG_LEVEL_DEBUG, PSTR("OTA: Propagating upload"));
          TasmotaGlobal.ota_state_flag = 6;               // Allow time for webserver to update console
        }
#endif  // FIRMWARE_MINIMAL

#ifdef USE_DISCOVERY
        StartMdns();
#endif  // USE_DISCOVERY

#ifdef USE_WEBSERVER
        if (Settings->webserver) {

#ifdef ESP8266
          if (!WifiIsInManagerMode()) { StartWebserver(Settings->webserver); }
#endif  // ESP8266
#ifdef ESP32
          StartWebserver(Settings->webserver);
#endif  // ESP32

#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
          MdnsAddServiceHttp();
#endif  // WEBSERVER_ADVERTISE
#endif  // USE_DISCOVERY

        } else {
          StopWebserver();
        }
#endif  // USE_WEBSERVER

#ifdef USE_DEVICE_GROUPS
        DeviceGroupsStart();
#endif  // USE_DEVICE_GROUPS

        // send FUNC_NETWORK_UP to all modules
//        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WIF: Sending FUNC_NETWORK_UP"));
        XdrvXsnsCall(FUNC_NETWORK_UP);

        MqttCheck();
      } else {

#ifdef USE_DEVICE_GROUPS
        DeviceGroupsStop();
#endif  // USE_DEVICE_GROUPS

        // send FUNC_NETWORK_DOWN to all modules
//        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WIF: Sending FUNC_NETWORK_DOWN"));
        XdrvXsnsCall(FUNC_NETWORK_DOWN);
      }                                           // Every x.75 second
    }
    break;
  }
}

#ifdef ESP8266
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
  ArduinoOTA.setHostname(NetworkHostname());
  if (strlen(SettingsText(SET_WEBPWD))) {
    ArduinoOTA.setPassword(SettingsText(SET_WEBPWD));
  }

  ArduinoOTA.onStart([]()
  {
    SettingsSave(1);         // Free flash for OTA update
#ifdef USE_WEBSERVER
    if (Settings->webserver) { StopWebserver(); }
#endif  // USE_WEBSERVER
    AllowInterrupts(0);
    if (Settings->flag.mqtt_enabled) {
      MqttDisconnect();      // SetOption3  - Enable MQTT
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_UPLOAD_STARTED));
    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;
    delay(100);              // Allow time for message xfer
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    if ((LOG_LEVEL_DEBUG <= TasmotaGlobal.seriallog_level)) {
      arduino_ota_progress_dot_count++;
      TasConsole.printf(".");
      if (!(arduino_ota_progress_dot_count % 80)) { TasConsole.println(); }
    }
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[100];

    if ((LOG_LEVEL_DEBUG <= TasmotaGlobal.seriallog_level) && arduino_ota_progress_dot_count) { TasConsole.println(); }
    switch (error) {
      case OTA_BEGIN_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA  %s. " D_RESTARTING), error_str);
    EspRestart();
  });

  ArduinoOTA.onEnd([]()
  {
    if ((LOG_LEVEL_DEBUG <= TasmotaGlobal.seriallog_level)) { TasConsole.println(); }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_SUCCESSFUL ". " D_RESTARTING));
    EspRestart();
	});

  ArduinoOTA.begin();
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_ENABLED " " D_PORT " 8266"));
}

void ArduinoOtaLoop(void)
{
  MDNS.update();
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  while (arduino_ota_triggered) { ArduinoOTA.handle(); }
}
#endif  // USE_ARDUINO_OTA
#endif  // ESP8266

/********************************************************************************************/

void SerialInput(void) {
#ifdef USE_XYZMODEM
  if (XYZModemActive(TXMP_TASCONSOLE)) { return; }
#endif  // USE_XYZMODEM

  static uint32_t serial_polling_window = 0;
  static bool serial_buffer_overrun = false;

  while (Serial.available()) {
//    yield();
    delay(0);
    TasmotaGlobal.serial_in_byte = Serial.read();

    if (0 == TasmotaGlobal.serial_in_byte_counter) {
      serial_buffer_overrun = false;
    }
    else if ((TasmotaGlobal.serial_in_byte_counter == INPUT_BUFFER_SIZE)
#ifdef ESP8266
             || Serial.hasOverrun()
#endif
                                                             ) {
      serial_buffer_overrun = true;
    }

#ifdef ESP8266
/*-------------------------------------------------------------------------------------------*\
 * Sonoff dual and ch4 19200 baud serial interface
\*-------------------------------------------------------------------------------------------*/
    if ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
      TasmotaGlobal.serial_in_byte = ButtonSerial(TasmotaGlobal.serial_in_byte);
    }
#endif  // ESP8266
/*-------------------------------------------------------------------------------------------*/

    if (XdrvCall(FUNC_SERIAL)) {
      TasmotaGlobal.serial_in_byte_counter = 0;
      Serial.flush();
      return;
    }

/*-------------------------------------------------------------------------------------------*/

#ifdef USE_XYZMODEM
    if (XYZModemStart(TXMP_TASCONSOLE, TasmotaGlobal.serial_in_byte)) { return; }
#endif  // USE_XYZMODEM

/*-------------------------------------------------------------------------------------------*/

    if (TasmotaGlobal.serial_in_byte > 127 && !Settings->flag.mqtt_serial_raw) {   // Discard binary data above 127 if no raw reception allowed - CMND_SERIALSEND3
      TasmotaGlobal.serial_in_byte_counter = 0;
      Serial.flush();
      return;
    }
    if (!Settings->flag.mqtt_serial) {                                             // SerialSend active - CMND_SERIALSEND and CMND_SERIALLOG
      if (isprint(TasmotaGlobal.serial_in_byte)) {                                 // Any char between 32 and 127
        if (TasmotaGlobal.serial_in_byte_counter < INPUT_BUFFER_SIZE -1) {         // Add char to string if it still fits
          TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = TasmotaGlobal.serial_in_byte;
        } else {
          serial_buffer_overrun = true;                                            // Signal overrun but continue reading input to flush until '\n' (EOL)
        }
      }
    } else {
      if (TasmotaGlobal.serial_in_byte || Settings->flag.mqtt_serial_raw) {        // Any char between 1 and 127 or any char (0 - 255) - CMND_SERIALSEND3
        bool in_byte_is_delimiter =                                                // Char is delimiter when...
          (((Settings->serial_delimiter < 128) && (TasmotaGlobal.serial_in_byte == Settings->serial_delimiter)) || // Any char between 1 and 127 and being delimiter
          ((Settings->serial_delimiter == 128) && !isprint(TasmotaGlobal.serial_in_byte))) &&   // Any char not between 32 and 127
          !Settings->flag.mqtt_serial_raw;                                         // In raw mode (CMND_SERIALSEND3) there is never a delimiter

        if ((TasmotaGlobal.serial_in_byte_counter < INPUT_BUFFER_SIZE -1) &&       // Add char to string if it still fits and ...
            !in_byte_is_delimiter) {                                               // Char is not a delimiter
          TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = TasmotaGlobal.serial_in_byte;
        }

        if ((TasmotaGlobal.serial_in_byte_counter >= INPUT_BUFFER_SIZE -1) ||      // Send message when buffer is full or ...
            in_byte_is_delimiter) {                                                // Char is delimiter
          serial_polling_window = 0;                                               // Reception done - send mqtt
          break;
        }

        serial_polling_window = millis();                                          // Wait for next char
      }
    }

#ifdef USE_SONOFF_SC
/*-------------------------------------------------------------------------------------------*\
 * Sonoff SC 19200 baud serial interface
\*-------------------------------------------------------------------------------------------*/
    if (SONOFF_SC == TasmotaGlobal.module_type) {
      if (TasmotaGlobal.serial_in_byte == '\x1B') {                                // Sonoff SC status from ATMEGA328P
        TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter] = 0;  // Serial data completed
        SonoffScSerialInput(TasmotaGlobal.serial_in_buffer);
        TasmotaGlobal.serial_in_byte_counter = 0;
        Serial.flush();
        return;
      }
    } else
#endif  // USE_SONOFF_SC
/*-------------------------------------------------------------------------------------------*/

#ifdef ESP32
    if (tasconsole_serial) {
#endif  // ESP32

    if (!Settings->flag.mqtt_serial && (TasmotaGlobal.serial_in_byte == '\n')) {   // CMND_SERIALSEND and CMND_SERIALLOG
      TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter] = 0;    // Serial data completed
      SetMinimumSeriallog();
      if (serial_buffer_overrun) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "Serial buffer overrun"));
      } else {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), TasmotaGlobal.serial_in_buffer);
        ExecuteCommand(TasmotaGlobal.serial_in_buffer, SRC_SERIAL);
      }
      TasmotaGlobal.serial_in_byte_counter = 0;
      serial_polling_window = 0;
      Serial.flush();
      return;
    }

#ifdef ESP32
    }
#endif  // ESP32

  }  // endWhile

  if (Settings->flag.mqtt_serial && TasmotaGlobal.serial_in_byte_counter && (millis() > (serial_polling_window + SERIAL_POLLING))) {  // CMND_SERIALSEND and CMND_SERIALLOG
    TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter] = 0;      // Serial data completed
    bool assume_json = (!Settings->flag.mqtt_serial_raw && (TasmotaGlobal.serial_in_buffer[0] == '{'));

    if (serial_buffer_overrun) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "Serial buffer overrun"));
    }

    Response_P(PSTR("{\"" D_JSON_SERIALRECEIVED "\":"));
    if (assume_json) {
      ResponseAppend_P(TasmotaGlobal.serial_in_buffer);
    } else {
      ResponseAppend_P(PSTR("\""));
      if (Settings->flag.mqtt_serial_raw) {
        ResponseAppend_P(PSTR("%*_H"), TasmotaGlobal.serial_in_byte_counter, TasmotaGlobal.serial_in_buffer);
      } else {
        ResponseAppend_P(EscapeJSONString(TasmotaGlobal.serial_in_buffer).c_str());
      }
      ResponseAppend_P(PSTR("\""));
    }
    ResponseJsonEnd();

    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_SERIALRECEIVED));
    TasmotaGlobal.serial_in_byte_counter = 0;
  }
}

/********************************************************************************************/

#ifdef ESP32

String console_buffer = "";

void TasConsoleInput(void) {
#ifdef USE_XYZMODEM
  if (XYZModemActive(TXMP_TASCONSOLE)) { return; }
#endif  // USE_XYZMODEM

  static bool console_buffer_overrun = false;

  while (TasConsole.available()) {
    delay(0);
    char console_in_byte = TasConsole.read();

#ifdef USE_XYZMODEM
    if (XYZModemStart(TXMP_TASCONSOLE, console_in_byte)) { return; }
#endif  // USE_XYZMODEM

    if (isprint(console_in_byte)) {                       // Any char between 32 and 127
      if (console_buffer.length() < INPUT_BUFFER_SIZE) {  // Add char to string if it still fits
        console_buffer += console_in_byte;
      } else {
        console_buffer_overrun = true;                    // Signal overrun but continue reading input to flush until '\n' (EOL)
      }
    }
    else if (console_in_byte == '\n') {
      SetMinimumSeriallog();
      if (console_buffer_overrun) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "USB buffer overrun"));
      } else {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), console_buffer.c_str());
        ExecuteCommand(console_buffer.c_str(), SRC_USBCONSOLE);
      }
      console_buffer = "";
      console_buffer_overrun = false;
      TasConsole.flush();
      return;
    }
  }
}

#endif  // ESP32

/********************************************************************************************/

void GpioInit(void)
{
  if (!ValidModule(Settings->module)) {
    uint32_t module = MODULE;
    if (!ValidModule(MODULE)) {
#ifdef ESP8266
      module = SONOFF_BASIC;
#endif  // ESP8266
#ifdef ESP32
      module = WEMOS;
#endif  // ESP32
    }

    Settings->module = module;
    Settings->last_module = module;
  }
  SetModuleType();

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Used GPIOs %d"), GPIO_SENSOR_END);

#ifdef ESP8266
  ConvertGpios();
#endif  // ESP8266

  for (uint32_t i = 0; i < nitems(Settings->user_template.gp.io); i++) {
    if ((Settings->user_template.gp.io[i] >= AGPIO(GPIO_SENSOR_END)) && (Settings->user_template.gp.io[i] < AGPIO(GPIO_USER))) {
      Settings->user_template.gp.io[i] = AGPIO(GPIO_USER);  // Fix not supported sensor ids in template
    }
  }

  myio template_gp;
  TemplateGpios(&template_gp);
  for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
    if ((Settings->my_gp.io[i] >= AGPIO(GPIO_SENSOR_END)) && (Settings->my_gp.io[i] < AGPIO(GPIO_USER))) {
      Settings->my_gp.io[i] = GPIO_NONE;             // Fix not supported sensor ids in module
    }
    else if (Settings->my_gp.io[i] > GPIO_NONE) {
      TasmotaGlobal.my_module.io[i] = Settings->my_gp.io[i];       // Set User selected Module sensors
    }
    if ((template_gp.io[i] > GPIO_NONE) && (template_gp.io[i] < AGPIO(GPIO_USER))) {
      TasmotaGlobal.my_module.io[i] = template_gp.io[i];               // Force Template override
    }
  }

  for (uint32_t i = 0; i < nitems(TasmotaGlobal.my_module.io); i++) {
    uint32_t mpin = ValidPin(i, TasmotaGlobal.my_module.io[i]);

    DEBUG_CORE_LOG(PSTR("INI: gpio pin %d, mpin %d"), i, mpin);

    if (mpin) {                                     // Above GPIO_NONE
      XdrvMailbox.index = mpin;
      XdrvMailbox.payload = i;

      if ((mpin >= AGPIO(GPIO_OPTION_A)) && (mpin < (AGPIO(GPIO_OPTION_A) + MAX_OPTIONS_A))) {
        bitSet(TasmotaGlobal.gpio_optiona.data, mpin - AGPIO(GPIO_OPTION_A));
        mpin = GPIO_NONE;
      }
#ifdef ROTARY_V1
      else if ((mpin >= AGPIO(GPIO_ROT1A_NP)) && (mpin < (AGPIO(GPIO_ROT1A_NP) + MAX_ROTARIES))) {
        RotaryAPullupFlag(mpin - AGPIO(GPIO_ROT1A_NP));
        mpin -= (AGPIO(GPIO_ROT1A_NP) - AGPIO(GPIO_ROT1A));
      }
      else if ((mpin >= AGPIO(GPIO_ROT1B_NP)) && (mpin < (AGPIO(GPIO_ROT1B_NP) + MAX_ROTARIES))) {
        RotaryBPullupFlag(mpin - AGPIO(GPIO_ROT1B_NP));
        mpin -= (AGPIO(GPIO_ROT1B_NP) - AGPIO(GPIO_ROT1B));
      }
#endif  // ROTARY_V1
      else if ((mpin >= AGPIO(GPIO_SWT1_NP)) && (mpin < (AGPIO(GPIO_SWT1_NP) + MAX_SWITCHES))) {
        SwitchPullupFlag(mpin - AGPIO(GPIO_SWT1_NP));
        mpin -= (AGPIO(GPIO_SWT1_NP) - AGPIO(GPIO_SWT1));
      }
      else if ((mpin >= AGPIO(GPIO_KEY1_NP)) && (mpin < (AGPIO(GPIO_KEY1_NP) + MAX_KEYS))) {
        ButtonPullupFlag(mpin - AGPIO(GPIO_KEY1_NP));      //  0 .. 3
        mpin -= (AGPIO(GPIO_KEY1_NP) - AGPIO(GPIO_KEY1));
      }
      else if ((mpin >= AGPIO(GPIO_KEY1_INV)) && (mpin < (AGPIO(GPIO_KEY1_INV) + MAX_KEYS))) {
        ButtonInvertFlag(mpin - AGPIO(GPIO_KEY1_INV));     //  0 .. 3
        mpin -= (AGPIO(GPIO_KEY1_INV) - AGPIO(GPIO_KEY1));
      }
      else if ((mpin >= AGPIO(GPIO_KEY1_INV_NP)) && (mpin < (AGPIO(GPIO_KEY1_INV_NP) + MAX_KEYS))) {
        ButtonPullupFlag(mpin - AGPIO(GPIO_KEY1_INV_NP));  //  0 .. 3
        ButtonInvertFlag(mpin - AGPIO(GPIO_KEY1_INV_NP));  //  0 .. 3
        mpin -= (AGPIO(GPIO_KEY1_INV_NP) - AGPIO(GPIO_KEY1));
      }
#ifdef ESP32
      else if ((mpin >= AGPIO(GPIO_OPTION_E)) && (mpin < (AGPIO(GPIO_OPTION_E) + MAX_OPTIONS_E))) {
        TasmotaGlobal.emulated_module_type = pgm_read_byte(kModuleEmulationList + (mpin - AGPIO(GPIO_OPTION_E)));
        SetModuleType();
        mpin = GPIO_NONE;
      }
      else if ((mpin >= AGPIO(GPIO_SWT1_PD)) && (mpin < (AGPIO(GPIO_SWT1_PD) + MAX_SWITCHES))) {
        SwitchPulldownFlag(mpin - AGPIO(GPIO_SWT1_PD));
        mpin -= (AGPIO(GPIO_SWT1_PD) - AGPIO(GPIO_SWT1));
      }
      else if ((mpin >= AGPIO(GPIO_KEY1_PD)) && (mpin < (AGPIO(GPIO_KEY1_PD) + MAX_KEYS))) {
        ButtonPulldownFlag(mpin - AGPIO(GPIO_KEY1_PD));    //  0 .. 3
        mpin -= (AGPIO(GPIO_KEY1_PD) - AGPIO(GPIO_KEY1));
      }
      else if ((mpin >= AGPIO(GPIO_KEY1_INV_PD)) && (mpin < (AGPIO(GPIO_KEY1_INV_PD) + MAX_KEYS))) {
        ButtonPulldownFlag(mpin - AGPIO(GPIO_KEY1_INV_PD));  //  0 .. 3
        ButtonInvertFlag(mpin - AGPIO(GPIO_KEY1_INV_PD));    //  0 .. 3
        mpin -= (AGPIO(GPIO_KEY1_INV_PD) - AGPIO(GPIO_KEY1));
      }
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
      else if ((mpin >= AGPIO(GPIO_KEY1_TC)) && (mpin < (AGPIO(GPIO_KEY1_TC) + MAX_KEYS))) {
        ButtonTouchFlag(mpin - AGPIO(GPIO_KEY1_TC));  //  0 .. 3
        mpin -= (AGPIO(GPIO_KEY1_TC) - AGPIO(GPIO_KEY1));
      }
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2
#endif //ESP32
      else if ((mpin >= AGPIO(GPIO_REL1_INV)) && (mpin < (AGPIO(GPIO_REL1_INV) + MAX_RELAYS))) {
        bitSet(TasmotaGlobal.rel_inverted, mpin - AGPIO(GPIO_REL1_INV));
        mpin -= (AGPIO(GPIO_REL1_INV) - AGPIO(GPIO_REL1));
      }
      else if ((mpin >= AGPIO(GPIO_REL1_BI)) && (mpin < (AGPIO(GPIO_REL1_BI) + MAX_RELAYS))) {
        bitSet(TasmotaGlobal.rel_bistable, mpin - AGPIO(GPIO_REL1_BI));
        mpin -= (AGPIO(GPIO_REL1_BI) - AGPIO(GPIO_REL1));
      }
      else if ((mpin >= AGPIO(GPIO_REL1_BI_INV)) && (mpin < (AGPIO(GPIO_REL1_BI_INV) + MAX_RELAYS))) {
        bitSet(TasmotaGlobal.rel_bistable, mpin - AGPIO(GPIO_REL1_BI_INV));
        bitSet(TasmotaGlobal.rel_inverted, mpin - AGPIO(GPIO_REL1_BI_INV));
        mpin -= (AGPIO(GPIO_REL1_BI_INV) - AGPIO(GPIO_REL1));
      }
      else if ((mpin >= AGPIO(GPIO_LED1_INV)) && (mpin < (AGPIO(GPIO_LED1_INV) + MAX_LEDS))) {
        bitSet(TasmotaGlobal.led_inverted, mpin - AGPIO(GPIO_LED1_INV));
        mpin -= (AGPIO(GPIO_LED1_INV) - AGPIO(GPIO_LED1));
      }
      else if (mpin == AGPIO(GPIO_LEDLNK_INV)) {
        TasmotaGlobal.ledlnk_inverted = 1;
        mpin -= (AGPIO(GPIO_LEDLNK_INV) - AGPIO(GPIO_LEDLNK));
      }
      else if (mpin == AGPIO(GPIO_HEARTBEAT_INV)) {
        TasmotaGlobal.heartbeat_inverted = 1;
        mpin -= (AGPIO(GPIO_HEARTBEAT_INV) - AGPIO(GPIO_HEARTBEAT));
      }
      else if ((mpin >= AGPIO(GPIO_PWM1_INV)) && (mpin < (AGPIO(GPIO_PWM1_INV) + MAX_PWMS))) {
        bitSet(TasmotaGlobal.pwm_inverted, mpin - AGPIO(GPIO_PWM1_INV));    // PWMi are later converted to PMW, but marked as inverted in TasmotaGlobal.pwm_inverted
        mpin -= (AGPIO(GPIO_PWM1_INV) - AGPIO(GPIO_PWM1));
      }
      else if (XdrvCall(FUNC_PIN_STATE)) {
        mpin = XdrvMailbox.index;
      }
      else if (XsnsCall(FUNC_PIN_STATE)) {
        mpin = XdrvMailbox.index;
      };
    }
    if (mpin) { SetPin(i, mpin); }                  // Anything above GPIO_NONE and below GPIO_SENSOR_END
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: TasmotaGlobal.gpio_pin %*_V"), nitems(TasmotaGlobal.gpio_pin), (uint8_t*)TasmotaGlobal.gpio_pin);

  if (ResetReasonPowerOn()) {
    TasmotaGlobal.power_on_delay = Settings->param[P_POWER_ON_DELAY2];  // SetOption47 - Delay switching relays to reduce power surge at power on
    if (TasmotaGlobal.power_on_delay) {
      // This is the earliest possibility to disable relays connected to esp8266/esp32 gpios at power up to reduce power surge
      for (uint32_t i = 0; i < MAX_RELAYS; i++) {
        if (PinUsed(GPIO_REL1, i)) {
          DigitalWrite(GPIO_REL1, i, bitRead(TasmotaGlobal.rel_inverted, i) ? 1 : 0);  // Off
        }
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("INI: SO47 %d Power off relays"), Settings->param[P_POWER_ON_DELAY2]);
    }
  }

  analogWriteRange(Settings->pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(Settings->pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)

#ifdef ESP8266
  if ((2 == Pin(GPIO_TXD)) || (H801 == TasmotaGlobal.module_type)) { Serial.set_tx(2); }
  SetSerialSwap();
#endif

  uint32_t sspi_mosi = (PinUsed(GPIO_SSPI_SCLK) && PinUsed(GPIO_SSPI_MOSI)) ? SPI_MOSI : SPI_NONE;
  uint32_t sspi_miso = (PinUsed(GPIO_SSPI_SCLK) && PinUsed(GPIO_SSPI_MISO)) ? SPI_MISO : SPI_NONE;
  TasmotaGlobal.soft_spi_enabled = sspi_mosi + sspi_miso;
  AddLogSpi(0, Pin(GPIO_SSPI_SCLK), Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_MISO));

#ifdef USE_SPI
#ifdef ESP8266
  if (!TasmotaGlobal.soft_spi_enabled) {
    uint32_t spi_mosi = (14 == Pin(GPIO_SPI_CLK)) && (13 == Pin(GPIO_SPI_MOSI)) ? SPI_MOSI : SPI_NONE;
    uint32_t spi_miso = (14 == Pin(GPIO_SPI_CLK)) && (12 == Pin(GPIO_SPI_MISO)) ? SPI_MISO : SPI_NONE;
    TasmotaGlobal.spi_enabled = spi_mosi + spi_miso;
    if (!TasmotaGlobal.spi_enabled) {
      bool valid_cs = (ValidSpiPinUsed(GPIO_SPI_CS) ||
                      ValidSpiPinUsed(GPIO_RC522_CS) ||
                      (ValidSpiPinUsed(GPIO_NRF24_CS) && ValidSpiPinUsed(GPIO_NRF24_DC)) ||
                      ValidSpiPinUsed(GPIO_ILI9341_CS) ||
                      ValidSpiPinUsed(GPIO_ILI9341_DC) || // there are also boards without cs
                      ValidSpiPinUsed(GPIO_EPAPER29_CS) ||
                      ValidSpiPinUsed(GPIO_EPAPER42_CS) ||
                      ValidSpiPinUsed(GPIO_ILI9488_CS) ||
                      ValidSpiPinUsed(GPIO_SSD1351_CS) ||
                      ValidSpiPinUsed(GPIO_RA8876_CS) ||
                      ValidSpiPinUsed(GPIO_ST7789_DC) ||  // ST7789 CS may be omitted so chk DC too
                      ValidSpiPinUsed(GPIO_ST7789_CS) ||
                      (ValidSpiPinUsed(GPIO_SSD1331_CS) && ValidSpiPinUsed(GPIO_SSD1331_DC)) ||
                      ValidSpiPinUsed(GPIO_SDCARD_CS) ||
                      ValidSpiPinUsed(GPIO_MCP2515_CS)
                      );
      // If SPI_CS and/or SPI_DC is used they must be valid
      TasmotaGlobal.spi_enabled = (valid_cs) ? SPI_MOSI_MISO : SPI_NONE;
    }
    if (TasmotaGlobal.spi_enabled) {
      TasmotaGlobal.my_module.io[12] = AGPIO(GPIO_SPI_MISO);
      SetPin(12, AGPIO(GPIO_SPI_MISO));
      TasmotaGlobal.my_module.io[13] = AGPIO(GPIO_SPI_MOSI);
      SetPin(13, AGPIO(GPIO_SPI_MOSI));
      TasmotaGlobal.my_module.io[14] = AGPIO(GPIO_SPI_CLK);
      SetPin(14, AGPIO(GPIO_SPI_CLK));
    }
  }
  AddLogSpi(1, Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_MISO));
#endif  // ESP8266
#ifdef ESP32
  uint32_t spi_mosi = (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MOSI)) ? SPI_MOSI : SPI_NONE;
  uint32_t spi_miso = (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MISO)) ? SPI_MISO : SPI_NONE;
  TasmotaGlobal.spi_enabled = spi_mosi + spi_miso;
  AddLogSpi(1, Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_MISO));

  spi_mosi = (PinUsed(GPIO_SPI_CLK, 1) && PinUsed(GPIO_SPI_MOSI, 1)) ? SPI_MOSI : SPI_NONE;
  spi_miso = (PinUsed(GPIO_SPI_CLK, 1) && PinUsed(GPIO_SPI_MISO, 1)) ? SPI_MISO : SPI_NONE;
  TasmotaGlobal.spi_enabled2 = spi_mosi + spi_miso;
  AddLogSpi(2, Pin(GPIO_SPI_CLK, 1), Pin(GPIO_SPI_MOSI, 1), Pin(GPIO_SPI_MISO, 1));
#endif  // ESP32
#endif  // USE_SPI

  for (uint32_t i = 0; i < nitems(TasmotaGlobal.my_module.io); i++) {
    uint32_t mpin = ValidPin(i, TasmotaGlobal.my_module.io[i]);
//    AddLog(LOG_LEVEL_DEBUG, PSTR("INI: gpio pin %d, mpin %d"), i, mpin);
    if (AGPIO(GPIO_OUTPUT_HI) == mpin) {
      pinMode(i, OUTPUT);
      digitalWrite(i, 1);
    }
    else if (AGPIO(GPIO_OUTPUT_LO) == mpin) {
      pinMode(i, OUTPUT);
      digitalWrite(i, 0);
    }

/*
  // Until 20210726

    // Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
    // Doing it here solves relay toggles at restart.
#if CONFIG_IDF_TARGET_ESP32C3
    else if (((i < 11) || (i > 17)) && (GPIO_NONE == mpin)) {  // Skip SPI flash interface
      if (!((20 == i) || (21 == i))) {             // Skip serial
        pinMode(i, INPUT);
      }
    }
#else // CONFIG_IDF_TARGET_ESP32C3
    else if (((i < 6) || (i > 11)) && (GPIO_NONE == mpin)) {  // Skip SPI flash interface
      if (!((1 == i) || (3 == i))) {             // Skip serial
        pinMode(i, INPUT);
      }
    }
#endif // CONFIG_IDF_TARGET_ESP32C3
*/
#ifdef ESP8266
    // Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
    // Doing it here solves relay toggles at restart.
    else if (((i < 6) || (i > 11)) && (GPIO_NONE == mpin)) {  // Skip SPI flash interface
      if (!((1 == i) || (3 == i))) {             // Skip serial
        pinMode(i, INPUT);
      }
    }
#endif  // ESP8266
  }

  DigitalWrite(GPIO_HEARTBEAT, 0, TasmotaGlobal.heartbeat_inverted);

  // Digital input
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    if (PinUsed(GPIO_INPUT, i)) {
      pinMode(Pin(GPIO_INPUT, i), INPUT);
    }
  }

  if (Settings->param[P_POWER_ON_DELAY]) {                 // SetOption46 - Allow Wemos D1 power to stabilize before starting I2C polling for devices powered locally
    uint32_t init_delay = Settings->param[P_POWER_ON_DELAY] * 10;
    AddLog(LOG_LEVEL_DEBUG, PSTR("INI: SO46 Wait %d msec"), init_delay);
    delay(init_delay);
  }

#ifdef USE_I2C
/*
  if (PinUsed(GPIO_I2C_SCL) && PinUsed(GPIO_I2C_SDA)) {
    TasmotaGlobal.i2c_enabled[0] = I2cBegin(Pin(GPIO_I2C_SDA), Pin(GPIO_I2C_SCL));
#ifdef ESP32
    if (TasmotaGlobal.i2c_enabled[0]) {
      AddLog(LOG_LEVEL_INFO, PSTR("I2C: Bus1 using GPIO%02d(SCL) and GPIO%02d(SDA)"), Pin(GPIO_I2C_SCL), Pin(GPIO_I2C_SDA));
    }
#endif
  }
#ifdef ESP32
  if (PinUsed(GPIO_I2C_SCL, 1) && PinUsed(GPIO_I2C_SDA, 1)) {
    TasmotaGlobal.i2c_enabled[1] = I2cBegin(Pin(GPIO_I2C_SDA, 1), Pin(GPIO_I2C_SCL, 1), 1);
    if (TasmotaGlobal.i2c_enabled[1]) {
      AddLog(LOG_LEVEL_INFO, PSTR("I2C: Bus2 using GPIO%02d(SCL) and GPIO%02d(SDA)"), Pin(GPIO_I2C_SCL, 1), Pin(GPIO_I2C_SDA, 1));
    }
  }
#endif
*/
  uint32_t max_bus = 1;
#ifdef USE_I2C_BUS2
  max_bus = 2;
#endif  // USE_I2C_BUS2
  for (uint32_t bus = 0; bus < max_bus; bus++) {
    if (PinUsed(GPIO_I2C_SCL, bus) && PinUsed(GPIO_I2C_SDA, bus)) {
      if (I2cBegin(Pin(GPIO_I2C_SDA, bus), Pin(GPIO_I2C_SCL, bus), bus)) {
        if (0 == bus) { 
          TasmotaGlobal.i2c_enabled[0] = true;
        }
#ifdef USE_I2C_BUS2
        else { 
          TasmotaGlobal.i2c_enabled[1] = true;
        }
        AddLog(LOG_LEVEL_INFO, PSTR("I2C: Bus%d using GPIO%02d(SCL) and GPIO%02d(SDA)"), bus +1, Pin(GPIO_I2C_SCL, bus), Pin(GPIO_I2C_SDA, bus));
#endif  // USE_I2C_BUS2
      }
    }
  }
#endif  // USE_I2C

  TasmotaGlobal.devices_present = 0;
  uint32_t bi_device = 0;
  for (uint32_t i = 0; i < MAX_RELAYS; i++) {
    if (PinUsed(GPIO_REL1, i)) {
      TasmotaGlobal.devices_present++;
#ifdef ESP8266
      if (EXS_RELAY == TasmotaGlobal.module_type) {
        if (i &1) { TasmotaGlobal.devices_present--; }
      }
#endif  // ESP8266
      if (!Settings->flag6.bistable_single_pin) {  // SetOption152 - (Power) Use single pin bistable
        if (bitRead(TasmotaGlobal.rel_bistable, i)) {
          if (bi_device &1) {
            TasmotaGlobal.devices_present--;
          }
        }
        bi_device++;
      }
    }
  }

#ifdef USE_UFILESYS
#ifdef USE_SDCARD
  UfsCheckSDCardInit();
#endif  // USE_SDCARD
#endif  // USE_UFILESYS

  XdrvCall(FUNC_SETUP_RING1);                              // Setup RTC hardware
  XsnsXdrvCall(FUNC_SETUP_RING2);                          // Setup hardware supporting virtual switches/buttons/relays

  TasmotaGlobal.light_type = LT_BASIC;                     // Use basic PWM control if SetOption15 = 0

  if (XdrvCall(FUNC_MODULE_INIT)) {                        // Init and claim single module (like tuya, armtronix, ifan, light)
    // Serviced
  }
#ifdef ESP8266
  else if (YTF_IR_BRIDGE == TasmotaGlobal.module_type) {
    ClaimSerial();  // Stop serial loopback mode
  }
  else if (SONOFF_DUAL == TasmotaGlobal.module_type) {
    UpdateDevicesPresent(2);
    SetSerial(19200, TS_SERIAL_8N1);
  }
  else if (CH4 == TasmotaGlobal.module_type) {
    UpdateDevicesPresent(4);
    SetSerial(19200, TS_SERIAL_8N1);
  }
#endif  // ESP8266

  GpioInitPwm();

  for (uint32_t i = 0; i < MAX_LEDS; i++) {
    if (PinUsed(GPIO_LED1, i)) {
#ifdef USE_ARILUX_RF
      if ((3 == i) && (TasmotaGlobal.leds_present < 2) && !PinUsed(GPIO_ARIRFSEL)) {
        SetPin(Pin(GPIO_LED1, i), AGPIO(GPIO_ARIRFSEL));  // Legacy support where LED4 was Arilux RF enable
      } else {
#endif
        TasmotaGlobal.leds_present++;
        DigitalWrite(GPIO_LED1, i, bitRead(TasmotaGlobal.led_inverted, i));
#ifdef USE_ARILUX_RF
      }
#endif
    }
  }
  DigitalWrite(GPIO_LEDLNK, 0, TasmotaGlobal.ledlnk_inverted);

#ifdef USE_PWM_DIMMER
  if (PWM_DIMMER == TasmotaGlobal.module_type && PinUsed(GPIO_REL1)) { TasmotaGlobal.devices_present--; }
#endif  // USE_PWM_DIMMER

  SetLedPower(Settings->ledstate &8);
  SetLedLink(Settings->ledstate &8);
}
