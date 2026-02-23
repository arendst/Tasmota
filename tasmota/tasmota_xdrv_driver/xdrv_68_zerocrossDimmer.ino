/*
  xdrv_68_zerocrossdimmer.ino - Zero-Cross Dimmer support for Tasmota

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

#define XDRV_68             68

static const uint8_t TRIGGER_PERIOD = 75;

#define ZCDIMMERSET_SHOW 1
//#define ZC_DEBUG 1

struct AC_ZERO_CROSS_DIMMER {
  uint32_t cycle_time_us;                    // Time (in micros()) of last ZC signal
  uint32_t crossed_zero_at;                  // Time (in micros()) of last ZC signal
  bool     timer_iterrupt_started = false;   // verification of the interrupt running
  bool     dimmer_in_use = false;            // Check if interrupt has to be run. Is stopped if all lights off
  bool     fallingEdgeDimmer = false;        // Work as a fallwing edge dimmer
  bool     triggered[MAX_PWMS];
  uint32_t enable_time_us[MAX_PWMS];         // Time since last ZC pulse to enable gate pin. 0 means no disable.
  uint32_t disable_time_us[MAX_PWMS];        // 99% of cycle time 
  uint32_t lastlight[MAX_PWMS];              // Store the light value. Set 1 if controlled through ZCDimmerSet
  uint16_t detailpower[MAX_PWMS];            // replaces dimmer and light controll 0..10000. required savedata 0.
  uint32_t accurracy[MAX_PWMS];              // offset of the time to fire the triac and the real time when it fired
  uint32_t intr_counter = 0;                 // counter internally on interrerupt calls
  uint32_t missed_zero_cross;                // count up all missed Zero-cross events.
  uint8_t  actual_tigger_Period = TRIGGER_PERIOD; // copy of tigger period to change during runtime
} ac_zero_cross_dimmer;


#ifdef ESP32
  static hw_timer_t *dimmer_timer = nullptr;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
#endif

#define D_PRFX_ZCDIMMER "ZCDimmer"
#define D_CMND_DIMMERSET "Set"        // Set percent as float value for dimmer
#define D_CMND_GATEENABLE "GateTime"  // Override the GATE ON time DEBUG PURPOSE

const char kZCDimmerCommands[] PROGMEM = D_PRFX_ZCDIMMER "|" D_CMND_DIMMERSET "|" D_CMND_GATEENABLE;

void (* const ZCDimmerCommand[])(void) PROGMEM = {
  &CmndZCDimmerSet//,&CmndZCGateEnableTime
  };

extern "C" void IRAM_ATTR ACDimmerZeroCross(uint32_t time) {
  ac_zero_cross_dimmer.dimmer_in_use = false;
  ac_zero_cross_dimmer.cycle_time_us = time - ac_zero_cross_dimmer.crossed_zero_at;
  ac_zero_cross_dimmer.crossed_zero_at = time;
  for (uint8_t i=0; i < MAX_PWMS; i++) {
    if (Pin(GPIO_PWM1, i) == -1) continue;
    digitalWrite(Pin(GPIO_PWM1, i), LOW ^ ac_zero_cross_dimmer.fallingEdgeDimmer);
    ac_zero_cross_dimmer.dimmer_in_use |= ac_zero_cross_dimmer.lastlight[i] > 0;
    ac_zero_cross_dimmer.triggered[i] = false;
  }
}

void ACDimmerInit()
{
  for (uint8_t i = 0 ; i < 5; i++) {
    ac_zero_cross_dimmer.detailpower[i] = Settings->zcdimmerset[i];
    ac_zero_cross_dimmer.fallingEdgeDimmer = Settings->flag6.zcfallingedge;
  }
}

extern "C" void IRAM_ATTR ACDimmerTimer_intr() {

  uint32_t time_since_zc = micros() - ac_zero_cross_dimmer.crossed_zero_at;
  // If no ZC signal received yet.
  if (ac_zero_cross_dimmer.crossed_zero_at == 0) return;
 
#ifdef ZC_DEBUG  
  ac_zero_cross_dimmer.intr_counter++;
  // Check for missed Zero-Cross event. Single failure will correct
  if (time_since_zc > 10100) {
    ac_zero_cross_dimmer.crossed_zero_at += ac_zero_cross_dimmer.cycle_time_us;
    ac_zero_cross_dimmer.missed_zero_cross++;
    time_since_zc += ac_zero_cross_dimmer.cycle_time_us;
  }
#endif  
  ac_zero_cross_dimmer.actual_tigger_Period = TRIGGER_PERIOD;
  for (uint8_t i = 0 ; i < MAX_PWMS; i++ ) {
    if (Pin(GPIO_PWM1, i) == -1) continue;

    if (time_since_zc + TRIGGER_PERIOD + 30 >= ac_zero_cross_dimmer.enable_time_us[i]){
      // Very close to the fire event. Loop the last µseconds to wait.
#ifdef ESP8266
      // on ESP8266 we can change dynamically the trigger interval.
      ac_zero_cross_dimmer.actual_tigger_Period = tmin(ac_zero_cross_dimmer.actual_tigger_Period*2,ac_zero_cross_dimmer.enable_time_us[i] - time_since_zc);
#endif 
#ifdef ESP32         
      while (time_since_zc < ac_zero_cross_dimmer.enable_time_us[i]) {
        time_since_zc =  micros() - ac_zero_cross_dimmer.crossed_zero_at;
      }
#endif        
      if (time_since_zc+5 >= ac_zero_cross_dimmer.enable_time_us[i] && !ac_zero_cross_dimmer.triggered[i] ) {
        digitalWrite(Pin(GPIO_PWM1, i), HIGH ^ ac_zero_cross_dimmer.fallingEdgeDimmer );
        ac_zero_cross_dimmer.triggered[i] = true;
        ac_zero_cross_dimmer.accurracy[i] = time_since_zc-ac_zero_cross_dimmer.enable_time_us[i];
      }   
      if (time_since_zc >= ac_zero_cross_dimmer.disable_time_us[i]) {
        digitalWrite(Pin(GPIO_PWM1, i), LOW ^ ac_zero_cross_dimmer.fallingEdgeDimmer );
      }    
    }
  }
}

#ifdef ESP8266
extern "C" uint32_t IRAM_ATTR ACDimmerTimer_intr_ESP8266() {
  ACDimmerTimer_intr();
  return ac_zero_cross_dimmer.actual_tigger_Period * 80;
}
#endif // ESP8266

void ACDimmerInterruptDisable(bool disable)
{ 
  AddLog(LOG_LEVEL_INFO, PSTR("ZCD: Zero-CrossDimmer enabled: %d"),!disable);
  ac_zero_cross_dimmer.timer_iterrupt_started = !disable;
  if (disable) {
    //stop the interrupt
#ifdef ESP32   
    if (dimmer_timer != nullptr) { 
#if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3) )
      timerStop(dimmer_timer);
#else // ESP_ARDUINO_VERSION_MAJOR >= 3
      timerAlarmDisable(dimmer_timer);
#endif // ESP_ARDUINO_VERSION_MAJOR >= 3
    }
#endif  
  } else {
    for (uint8_t i = 0 ; i < MAX_PWMS; i++) {
      if (Pin(GPIO_PWM1, i) != -1) {
        pinMode(Pin(GPIO_PWM1, i), OUTPUT);
        AddLog(LOG_LEVEL_INFO, PSTR("ZCD: Zero-CrossDimmer Pin %d set"),Pin(GPIO_PWM1, i));
      }
    }
#ifdef ESP32
#if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3) )
    if (dimmer_timer == nullptr) {
      dimmer_timer = timerBegin(1000000);      // 1 MHz
      timerAttachInterrupt(dimmer_timer, &ACDimmerTimer_intr);
      // For ESP32, we can't use dynamic interval calculation because the timerX functions
      // are not callable from ISR (placed in flash storage).
      // Here we just use an interrupt firing every 75 µs.
      timerAlarm(dimmer_timer, TRIGGER_PERIOD, true, 0);
    }
    timerStart(dimmer_timer);
#else // ESP_ARDUINO_VERSION_MAJOR >= 3
    if (dimmer_timer == nullptr) {
      // 80 Divider -> 1 count=1µs
      dimmer_timer = timerBegin(0, 80, true);
      timerAttachInterrupt(dimmer_timer, &ACDimmerTimer_intr, true);
      // For ESP32, we can't use dynamic interval calculation because the timerX functions
      // are not callable from ISR (placed in flash storage).
      // Here we just use an interrupt firing every 75 µs.
      timerAlarmWrite(dimmer_timer, TRIGGER_PERIOD , true);
    }
    timerAlarmEnable(dimmer_timer);
#endif // ESP_ARDUINO_VERSION_MAJOR >= 3
#endif
  }
}

void ACDimmerControllTrigger(void) {
#ifdef ESP32
  if (ac_zero_cross_dimmer.timer_iterrupt_started != ac_zero_cross_dimmer.dimmer_in_use) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ZCD: ZeroEnable %d --> %d ... change..."),ac_zero_cross_dimmer.timer_iterrupt_started, ac_zero_cross_dimmer.dimmer_in_use);
    ACDimmerInterruptDisable(!ac_zero_cross_dimmer.dimmer_in_use);
  }
#endif  
  for (uint8_t i = 0; i < MAX_PWMS; i++){
    if (Pin(GPIO_PWM1, i) == -1) continue;
    ac_zero_cross_dimmer.disable_time_us[i] = (ac_zero_cross_dimmer.cycle_time_us * 99) / 100;
    if (ac_zero_cross_dimmer.detailpower[i]){
      ac_zero_cross_dimmer.lastlight[i] = changeUIntScale(ac_zero_cross_dimmer.detailpower[i]/10, 0, 1000, 0, 1023);
    } else {
      ac_zero_cross_dimmer.lastlight[i] = Light.fade_running ? Light.fade_cur_10[i] : Light.fade_start_10[i];
    }
    ac_zero_cross_dimmer.enable_time_us[i] = (ac_zero_cross_dimmer.cycle_time_us * (1023 - ac_zero_cross_power(ac_zero_cross_dimmer.lastlight[i]))) / 1023;
    if (ac_zero_cross_dimmer.enable_time_us[i] > ac_zero_cross_dimmer.disable_time_us[i]) {
      // do not set HIGH near to the zero cross
      ac_zero_cross_dimmer.enable_time_us[i] = 99999;
    }
#ifdef ESP32
    if (ac_zero_cross_dimmer.detailpower[i]){
      float state = (float)(1 - (ac_zero_cross_dimmer.detailpower[i]/10000.0));
      //state = std::acos(1 - (2 * state)) / 3.14159 * ac_zero_cross_dimmer.cycle_time_us;
      state = std::acos(1 - (2 * state)) / 3.14159 * ac_zero_cross_dimmer.cycle_time_us;
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ZCD: Float2: %*_f"),0,&state);
      ac_zero_cross_dimmer.enable_time_us[i] = (uint32_t)state;
    }    
#endif  
    
  }
  
}

void ACDimmerLogging(void)
{
    bool alarmEnabled = false;
    uint32_t timercounter = ac_zero_cross_dimmer.intr_counter;

#ifdef ESP32
    if (dimmer_timer != nullptr) {
#if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3) )
      alarmEnabled = true;    // we assume it's started, there is no API to check whether it's running
      timercounter = (uint32_t)timerRead(dimmer_timer);
#else // ESP_ARDUINO_VERSION_MAJOR >= 3
      alarmEnabled = timerAlarmEnabled(dimmer_timer);
      timercounter = (uint32_t)timerRead(dimmer_timer);
#endif // ESP_ARDUINO_VERSION_MAJOR >= 3
    }
#endif    

    AddLog(LOG_LEVEL_DEBUG, PSTR("ZCD: ZeroEnable %d -> %d, Alarm %d, intr: %ld, cycle time: %ld µs, missed zc %ld"),
      ac_zero_cross_dimmer.dimmer_in_use, ac_zero_cross_dimmer.timer_iterrupt_started, alarmEnabled, timercounter, 
      ac_zero_cross_dimmer.cycle_time_us, ac_zero_cross_dimmer.missed_zero_cross
      );
    for (uint8_t i = 0; i < MAX_PWMS; i++){
      if (Pin(GPIO_PWM1, i) == -1) continue;
      
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ZCD: PWM[%d] en: %ld µs, dis: %ld µs, fade: %d, cur: %d, end: %d, lastlight: %d, acc: %ld"), 
        i+1, ac_zero_cross_dimmer.enable_time_us[i], ac_zero_cross_dimmer.disable_time_us[i], 
        Light.fade_cur_10[i], Light.fade_start_10[i], Light.fade_end_10[i], ac_zero_cross_dimmer.lastlight[i],
        ac_zero_cross_dimmer.accurracy[i]
      );
      if (ac_zero_cross_dimmer.accurracy[i]) ac_zero_cross_dimmer.accurracy[i]=0;
    }
} 

#ifdef USE_WEBSERVER
#ifdef ZCDIMMERSET_SHOW
void ACDimmerShow(void)
{
  char c_ZCDimmerSetBuffer[8];
  for (uint8_t i = 0; i < MAX_PWMS; i++){
    if (Pin(GPIO_PWM1, i) == -1) continue;
    if (ac_zero_cross_dimmer.detailpower[i]){
      dtostrfd(ac_zero_cross_dimmer.detailpower[i]/100.0, 2, c_ZCDimmerSetBuffer);
      WSContentSend_PD(PSTR("{s}ZCDimmer%d{m}%s %%{e}"), i+1, c_ZCDimmerSetBuffer);
    }
  }
}
#endif  // ZCDIMMERSET_SHOW
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndZCDimmerSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PWMS)) {
    if (XdrvMailbox.data_len > 0) {
      Settings->zcdimmerset[XdrvMailbox.index-1] = ac_zero_cross_dimmer.detailpower[XdrvMailbox.index-1] = (uint16_t)(100 * CharToFloat(XdrvMailbox.data));
    }
    ResponseCmndIdxFloat((float)(ac_zero_cross_dimmer.detailpower[XdrvMailbox.index-1]) / 100, 2);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv68(uint32_t function)
{
  bool result = false;
  if (Settings->flag4.zerocross_dimmer) {
    switch (function) {
      case FUNC_INIT:
        ACDimmerInit();
#ifdef ESP32      
        //ACDimmerInterruptDisable(false);
#endif      
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
      case FUNC_COMMAND:
        result = DecodeCommand(kZCDimmerCommands, ZCDimmerCommand);
        break; 
#ifdef USE_WEBSERVER
#ifdef ZCDIMMERSET_SHOW
      case FUNC_WEB_SENSOR:
        ACDimmerShow();
        break;
#endif  // ZCDIMMERSET_SHOW
#endif  // USE_WEBSERVER        
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_AC_ZERO_CROSS_DIMMER
