/*
  xdrv_91_zerocrossdimmer.ino - Zero-Cross Dimmer support for Tasmota

  Copyright (C) 2023  Stefan Bode

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

#ifdef USE_AC_ZERO_CROSS_DIMMER
/*********************************************************************************************\
 * Zero-Cross AC Dimmer PMM 1..xx use 
\*********************************************************************************************/

#define XDRV_91             1

// #define AC_DIMMER_DETAILED_POWER_MGMT 1

static const uint32_t GATE_ENABLE_TIME = 100;

struct AC_ZERO_CROSS_DIMMER {
  uint32_t cycle_time_us;
  /// Time (in micros()) of last ZC signal
  uint32_t crossed_zero_at;
  /// Time since last ZC pulse to enable gate pin. 0 means not set.
  bool     timer_iterrupt_started = false;
  bool     dimmer_in_use = false;
  // Check if 50µs timer is running.
  uint32_t enable_time_us[MAX_PWMS];
  /// Time since last ZC pulse to disable gate pin. 0 means no disable.
  uint32_t disable_time_us[MAX_PWMS];
  uint8_t  current_state_in_phase[MAX_PWMS];  // 0=before fire HIGH, 1=HIGH, 2=after setting LOW, 3=before HIGH without setting LOW (POWER ON)
  uint32_t lastlight[MAX_PWMS];
  uint32_t intr_counter = 0;
} ac_zero_cross_dimmer;


#ifdef ESP32
  static hw_timer_t *dimmer_timer = nullptr;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
#endif

void IRAM_ATTR ACDimmerZeroCross(uint32_t time) {
    ac_zero_cross_dimmer.dimmer_in_use = false;
    ac_zero_cross_dimmer.cycle_time_us = time - ac_zero_cross_dimmer.crossed_zero_at;
    ac_zero_cross_dimmer.crossed_zero_at = time;
    for (uint8_t i=0; i < MAX_PWMS; i++) {
        if (Pin(GPIO_PWM1, i) == -1) continue;
        ac_zero_cross_dimmer.dimmer_in_use |= ac_zero_cross_dimmer.enable_time_us[i] != ac_zero_cross_dimmer.cycle_time_us;
        // Dimmer is physically off. Skip swich on
        ac_zero_cross_dimmer.current_state_in_phase[i] = 0;
        if (100 * ac_zero_cross_dimmer.enable_time_us[i] > 95 * ac_zero_cross_dimmer.cycle_time_us ) {
        ac_zero_cross_dimmer.current_state_in_phase[i] = 1;
        ac_zero_cross_dimmer.disable_time_us[i] = ac_zero_cross_dimmer.cycle_time_us / 2;
        } 
        // If full cycle is required keep pin HIGH, skip LOW by skipping phase
        if (100 * ac_zero_cross_dimmer.enable_time_us[i] < 15 * ac_zero_cross_dimmer.cycle_time_us) {
        ac_zero_cross_dimmer.current_state_in_phase[i] = 3;
        } 
    }
}

uint32_t IRAM_ATTR ACDimmerTimer_intr_ESP8266() {
  //ACDimmerTimer_intr();
  ACDimmerTimer_intr();
  return 4000;
}

void ACDimmerInterruptDisable(bool disable)
{ 
  AddLog(LOG_LEVEL_INFO, PSTR("CNT: Zero-CrossDimmer enabled: %d"),!disable);
  ac_zero_cross_dimmer.timer_iterrupt_started = !disable;
  if (disable) {
    //stop the interrupt
#ifdef ESP32   
    if (dimmer_timer != nullptr) { 
     timerAlarmDisable(dimmer_timer);
    }
#endif  
#ifdef ESP8266  
    //setTimer1Callback(NULL);
#endif  
  } else {
    for (uint8_t i = 0 ; i < MAX_PWMS; i++) {
      if (Pin(GPIO_PWM1, i) > -1)
        pinMode(Pin(GPIO_PWM1, i), OUTPUT);
    }
#ifdef ESP32
    if (dimmer_timer == nullptr) {
      // 80 Divider -> 1 count=1µs
      dimmer_timer = timerBegin(0, 80, true);
      timerAttachInterrupt(dimmer_timer, &ACDimmerTimer_intr, true);
      // For ESP32, we can't use dynamic interval calculation because the timerX functions
      // are not callable from ISR (placed in flash storage).
      // Here we just use an interrupt firing every 50 µs.
      timerAlarmWrite(dimmer_timer, 50, true);
    }
    timerAlarmEnable(dimmer_timer);
#endif

#ifdef ESP8266
    // Uses ESP8266 waveform (soft PWM) class
    // PWM and AcDimmer can even run at the same time this way
    //setTimer1Callback(&ACDimmerTimer_intr_ESP8266);
#endif    
  }
}

void IRAM_ATTR ACDimmerTimer_intr() {
  // If no ZC signal received yet.
  uint32_t now = micros();
  ac_zero_cross_dimmer.intr_counter++;
  if (ac_zero_cross_dimmer.crossed_zero_at == 0)
    return;

  uint32_t time_since_zc = now - ac_zero_cross_dimmer.crossed_zero_at;
  if (time_since_zc > 10100) {
    memset(&ac_zero_cross_dimmer.current_state_in_phase, 0x00, sizeof(ac_zero_cross_dimmer.current_state_in_phase));
    ac_zero_cross_dimmer.crossed_zero_at += ac_zero_cross_dimmer.cycle_time_us;
    time_since_zc = now - ac_zero_cross_dimmer.crossed_zero_at;
  }

  for (uint8_t i = 0 ; i < MAX_PWMS; i++ ) {
    if (Pin(GPIO_PWM1, i) == -1) continue;
    switch (ac_zero_cross_dimmer.current_state_in_phase[i]) {
      case 1:
        if (time_since_zc >= ac_zero_cross_dimmer.disable_time_us[i]) {
          digitalWrite(Pin(GPIO_PWM1, i), LOW);
          ac_zero_cross_dimmer.current_state_in_phase[i]++;
        }
        break;    
      case 0:
      case 3:
        if (time_since_zc >= ac_zero_cross_dimmer.enable_time_us[i]) {
          digitalWrite(Pin(GPIO_PWM1, i), HIGH);
          ac_zero_cross_dimmer.current_state_in_phase[i]++;
        }    
        break;
    } 
  }
}

void ACDimmerControllTrigger(void) {

  if (ac_zero_cross_dimmer.timer_iterrupt_started != ac_zero_cross_dimmer.dimmer_in_use) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CNT: ZeroEnable %d --> %d ... change..."),ac_zero_cross_dimmer.timer_iterrupt_started, ac_zero_cross_dimmer.dimmer_in_use);
    ACDimmerInterruptDisable(!ac_zero_cross_dimmer.dimmer_in_use);
  }
  for (uint8_t i = 0; i < MAX_PWMS; i++){
    if (Pin(GPIO_PWM1, i) == -1) continue;
#ifdef AC_DIMMER_DETAILED_POWER_MGMT    
    float state = (float)(1023 - (Light.fade_running ? Light.fade_cur_10[i] : Light.fade_start_10[i]))/1023.00;
    state = std::acos(1 - (2 * state)) / 3.14159 * ac_zero_cross_dimmer.cycle_time_us;
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CNT: Float2: %*_f"),0,&state);
    ac_zero_cross_dimmer.enable_time_us[i] = (uint32_t)state;
#else    
    ac_zero_cross_dimmer.enable_time_us[i] = (ac_zero_cross_dimmer.cycle_time_us * (1023 - ac_zero_cross_power(Light.fade_running ? Light.fade_cur_10[i] : Light.fade_start_10[i]))) / 1023;
#endif    
    ac_zero_cross_dimmer.disable_time_us[i] = ac_zero_cross_dimmer.enable_time_us[i] + GATE_ENABLE_TIME;
  }
  
}

void ACDimmerLogging(void)
{
    bool alarmEnabled = false;
    uint32_t timercounter = ac_zero_cross_dimmer.intr_counter;

#ifdef ESP32    
    if (dimmer_timer != nullptr) {
      alarmEnabled = timerAlarmEnabled(dimmer_timer);
      timercounter = (uint32_t)timerRead(dimmer_timer);
    }
#endif    

    AddLog(LOG_LEVEL_DEBUG, PSTR("CNT: ZeroEnable %d -> %d, Alarm %d, intr: %ld, cycle time: %ld µs"),
      ac_zero_cross_dimmer.dimmer_in_use, ac_zero_cross_dimmer.timer_iterrupt_started, alarmEnabled, timercounter, ac_zero_cross_dimmer.cycle_time_us
      );
    for (uint8_t i = 0; i < MAX_PWMS; i++){
      if (Pin(GPIO_PWM1, i) == -1) continue;
       AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CNT: PWM[%d] en: %ld µs, dis: %ld µs, fade: %d, cur: %d"), 
        i+1, ac_zero_cross_dimmer.enable_time_us[i], ac_zero_cross_dimmer.disable_time_us[i], Light.fade_cur_10[i], Light.fade_start_10[i]
      );
    }

} 


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv91(uint32_t function)
{
  bool result = false;
    switch (function) {
      case FUNC_INIT:
#ifdef ESP8266
        setTimer1Callback(&ACDimmerTimer_intr_ESP8266);       
#endif   
        break;
      case FUNC_EVERY_SECOND:
        ACDimmerLogging();
        break;
     
      case FUNC_EVERY_100_MSECOND:
        ACDimmerControllTrigger();
        break;

      case FUNC_INTERRUPT_STOP:
        ACDimmerInterruptDisable(true);
        break;
      case FUNC_INTERRUPT_START:
        ACDimmerInterruptDisable(false);
        break;
    }
  return result;
}

#endif  // USE_AC_ZERO_CROSS_DIMMER
