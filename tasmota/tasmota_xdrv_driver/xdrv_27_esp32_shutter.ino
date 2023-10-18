/*
  xdrv_27_esp32_shutter.ino - Shutter/Blind support for Tasmota

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

#ifdef ESP32
#ifdef USE_SHUTTER
/*********************************************************************************************\
 * Shutter or Blind support using two consecutive relays
 * Shutters for ESP32 with max eight shutters using more RAM and Settings from filesystem
\*********************************************************************************************/

#define XDRV_27            27
#ifndef SHUTTER_STEPPER
  #define SHUTTER_STEPPER
#endif

#ifndef SHUTTER_RELAY_OPERATION_TIME
  #define SHUTTER_RELAY_OPERATION_TIME 100 // wait for direction relay 0.1sec before power up main relay
#endif

//#define SHUTTER_UNITTEST

#define D_SHUTTER "SHUTTER"

// Allow up to 16 shutters on ESP32
#undef  MAX_SHUTTERS_ESP32
#define MAX_SHUTTERS_ESP32 16

// Define Std Text Strings
#define D_ERROR_FILESYSTEM_NOT_READY "SHT: ERROR File system not enabled"
#define D_ERROR_FILE_NOT_FOUND "SHT: ERROR File system not ready or file not found"

const char HTTP_MSG_SLIDER_SHUTTER[] PROGMEM =
  "<div><span class='p'>%s</span><span class='q'>%s</span></div>"
  "<div><input type='range' min='0' max='100' value='%d' onchange='lc(\"u\",%d,value)'></div>";

const uint16_t SHUTTER_VERSION = 0x0100;  // Latest driver version (See settings deltas below)

typedef struct { // depreciated 2023-04-28
  int8_t pos;
  int8_t tilt;
  bool   mqtt_broadcast;
} tPosition_old;

typedef struct {
  int8_t pos;
  int8_t tilt;
  bool   mqtt_broadcast;
  int8_t pos_incrdecr;
  int8_t tilt_incrdecr;
} tPosition;

typedef struct { // depreciated 2023-04-28
  bool         enabled;
  bool         mqtt_all;
  uint8_t      shutter_number;
  tPosition_old    position[4]; 
} tButtonSettings_old;

typedef struct {
  bool         enabled;
  bool         mqtt_all;
  uint8_t      shutter_number;
  tPosition    position[4]; 
} tButtonSettings;

// Global structure containing shutter saved variables
struct SHUTTERSETTINGS {
  uint32_t      crc32;                                     // To detect file changes
  uint16_t      version;                                   // To detect driver function changes
  uint16_t      spare;
  uint8_t       shutter_accuracy;
  uint8_t       shutter_mode;
  uint16_t      shutter_motorstop;
  uint16_t      open_velocity_max;
  int8_t        shutter_tilt_config[5][MAX_SHUTTERS_ESP32];
  int8_t        shutter_tilt_pos[MAX_SHUTTERS_ESP32];
  uint16_t      shutter_opentime[MAX_SHUTTERS_ESP32];
  uint16_t      shutter_closetime[MAX_SHUTTERS_ESP32];
  int16_t       shuttercoeff[5][MAX_SHUTTERS_ESP32];
  uint8_t       shutter_options[MAX_SHUTTERS_ESP32];
  uint8_t       shutter_set50percent[MAX_SHUTTERS_ESP32];
  uint8_t       shutter_position[MAX_SHUTTERS_ESP32];
  uint8_t       shutter_startrelay[MAX_SHUTTERS_ESP32];
  uint8_t       shutter_motordelay[MAX_SHUTTERS_ESP32];
  uint16_t      shutter_pwmrange[2][MAX_SHUTTERS_ESP32];
  tButtonSettings_old shutter_button_old[MAX_SHUTTERS_ESP32*2]; // depreciated 2023-04-28
  tButtonSettings shutter_button[MAX_SHUTTERS_ESP32*2];
} ShutterSettings;

const uint16_t RESOLUTION = 1000;       // incresed to 1000 in 8.5 to ramp servos
const uint8_t  STEPS_PER_SECOND = 20;   // FUNC_EVERY_50_MSECOND
const uint16_t pwm_servo_max = 500;
const uint16_t pwm_servo_min = 90;

uint8_t calibrate_pos[6] = {0,30,50,70,90,100};
uint16_t messwerte[5] = {30,50,70,90,100};

int32_t  velocity_max = 0;
int32_t  velocity_change_per_step_max = 0;
int32_t  min_runtime_ms = 0;
int32_t  current_stop_way = 0;
int32_t  next_possible_stop_position = 0;
int32_t  current_real_position = 0;
int32_t  current_pwm_velocity = 0;

const uint8_t MAX_MODES = 8;
enum Shutterposition_mode {SHT_UNDEF, SHT_TIME, SHT_TIME_UP_DOWN, SHT_TIME_GARAGE, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME,SHT_AUTOCONFIG};
enum Shutterswitch_mode   {SHT_SWITCH, SHT_PULSE,};
enum ShutterButtonStates  { SHT_NOT_PRESSED, SHT_PRESSED_MULTI, SHT_PRESSED_HOLD, SHT_PRESSED_IMMEDIATE, SHT_PRESSED_EXT_HOLD, SHT_PRESSED_MULTI_SIMULTANEOUS, SHT_PRESSED_HOLD_SIMULTANEOUS, SHT_PRESSED_EXT_HOLD_SIMULTANEOUS,};

const char kShutterCommands[] PROGMEM = D_PRFX_SHUTTER "|"
  D_CMND_SHUTTER_OPEN "|" D_CMND_SHUTTER_CLOSE "|" D_CMND_SHUTTER_TOGGLE "|" D_CMND_SHUTTER_TOGGLEDIR "|" D_CMND_SHUTTER_STOP "|" D_CMND_SHUTTER_POSITION "|"
  D_CMND_SHUTTER_OPENTIME "|" D_CMND_SHUTTER_CLOSETIME "|" D_CMND_SHUTTER_RELAY "|" D_CMND_SHUTTER_MODE "|"  D_CMND_SHUTTER_PWMRANGE "|"
  D_CMND_SHUTTER_SETHALFWAY "|" D_CMND_SHUTTER_SETCLOSE "|" D_CMND_SHUTTER_SETOPEN "|" D_CMND_SHUTTER_INVERT "|" D_CMND_SHUTTER_CLIBRATION "|"
  D_CMND_SHUTTER_MOTORDELAY "|" D_CMND_SHUTTER_FREQUENCY "|" D_CMND_SHUTTER_BUTTON "|" D_CMND_SHUTTER_LOCK "|" D_CMND_SHUTTER_ENABLEENDSTOPTIME "|" D_CMND_SHUTTER_INVERTWEBBUTTONS "|"
  D_CMND_SHUTTER_STOPOPEN "|" D_CMND_SHUTTER_STOPCLOSE "|" D_CMND_SHUTTER_STOPTOGGLE "|" D_CMND_SHUTTER_STOPTOGGLEDIR "|" D_CMND_SHUTTER_STOPPOSITION "|" D_CMND_SHUTTER_INCDEC "|"
  D_CMND_SHUTTER_UNITTEST "|" D_CMND_SHUTTER_TILTCONFIG "|" D_CMND_SHUTTER_SETTILT "|" D_CMND_SHUTTER_TILTINCDEC "|" D_CMND_SHUTTER_MOTORSTOP "|" D_CMND_SHUTTER_SETUP;

void (* const ShutterCommand[])(void) PROGMEM = {
  &CmndShutterOpen, &CmndShutterClose, &CmndShutterToggle, &CmndShutterToggleDir, &CmndShutterStop, &CmndShutterPosition,
  &CmndShutterOpenTime, &CmndShutterCloseTime, &CmndShutterRelay, &CmndShutterMode, &CmndShutterPwmRange,
  &CmndShutterSetHalfway, &CmndShutterSetClose, &CmndShutterSetOpen, &CmndShutterInvert, &CmndShutterCalibration , &CmndShutterMotorDelay,
  &CmndShutterFrequency, &CmndShutterButton, &CmndShutterLock, &CmndShutterEnableEndStopTime, &CmndShutterInvertWebButtons,
  &CmndShutterStopOpen, &CmndShutterStopClose, &CmndShutterStopToggle, &CmndShutterStopToggleDir, &CmndShutterStopPosition, &CmndShutterIncDec,
  &CmndShutterUnitTest,&CmndShutterTiltConfig,&CmndShutterSetTilt,&CmndShutterTiltIncDec,&CmndShutterMotorStop,&CmndShutterSetup
  };

  const char JSON_SHUTTER_POS[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Position\":%d,\"Direction\":%d,\"Target\":%d,\"Tilt\":%d}";
  const char JSON_SHUTTER_BUTTON[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Button%d\":%d}";

#include <Ticker.h>

Ticker TickerShutter;

struct SHUTTER {
  uint32_t time;               // operating time of the shutter in 0.05sec
  int32_t  open_max;           // max value on maximum open calculated
  int32_t  target_position;    // position to go to
  int32_t  start_position;     // position before a movement is started. init at start
  int32_t  real_position;      // value between 0 and Shutter[i].open_max
  uint16_t open_time;          // duration to open the Shutter[i]. 112 = 11.2sec
  uint16_t close_time;         // duration to close the Shutter[i]. 112 = 11.2sec
  uint16_t close_velocity;     // in relation to open velocity. higher value = faster
  int8_t   direction;          // 1 == UP , 0 == stop; -1 == down
  int8_t   lastdirection;      // last direction (1 == UP , -1 == down)
  uint8_t  switch_mode;        // how to switch relays: SHT_SWITCH, SHT_PULSE
  int8_t   motordelay;         // initial motorstarttime in 0.05sec. Also uses for ramp at steppers and servos, negative if motor stops late
  int16_t  pwm_velocity;       // frequency of PWN for stepper motors or PWM duty cycle change for PWM servo
  uint16_t pwm_value;          // dutyload of PWM 0..1023 on ESP8266
  uint16_t close_velocity_max; // maximum of PWM change during closeing. Defines velocity on opening. Steppers and Servos only
  int32_t  accelerator;        // speed of ramp-up, ramp down of shutters with velocity control. Steppers and Servos only
  int8_t   tilt_config[5];     // tilt_min, tilt_max, duration, tilt_closed_value, tilt_opened_value
  int8_t   tilt_real_pos;      // -90 to 90
  int8_t   tilt_target_pos;    // target positon for movements of the tilt
  int8_t   tilt_target_pos_override;   // one time override of automatic calculation of tilt_target
  int8_t   tilt_start_pos;             // saved start position before shutter moves
  uint8_t  tilt_velocity;              // degree rotation per step 0.05sec
  int8_t   tiltmoving;                 // 0  operating move, 1 = operating tilt
  uint16_t venetian_delay = 0;         // Delay in steps before venetian shutter start physical moving. Based on tilt position
  uint16_t min_realPositionChange = 0; // minimum change of the position before the shutter operates. different for PWM and time based operations
  uint16_t min_TiltChange = 0;         // minimum change of the tilt before the shutter operates. different for PWM and time based operations
  uint16_t last_reported_time = 0;     // get information on skipped 50ms loop() slots
  uint32_t last_stop_time = 0;         // record the last time the relay was switched off
  uint8_t  button_simu_pressed = 0;    // record if both button where pressed simultanously
} Shutter[MAX_SHUTTERS_ESP32];

struct SHUTTERGLOBAL {
  power_t  RelayShutterMask = 0;             // bit mask with 11 at the position of relays that belong to at least ONE shutter
  power_t  RelayOldMask = 0;                 // bitmatrix that contain the last known state of all relays. Required to detemine the manual changed relay.
  power_t  RelayCurrentMask = 0;             // bitmatrix that contain the current state of all relays
  uint8_t  LastChangedRelay = 0;             // Relay 1..32, 0 no change
  uint8_t  position_mode = 0;                // how to calculate actual position: SHT_TIME, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME
  uint8_t  skip_relay_change;                // avoid overrun at endstops
  uint8_t  start_reported = 0;               // indicates of the shutter start was reported through MQTT JSON
  uint16_t open_velocity_max = RESOLUTION;   // maximum of PWM change during opening. Defines velocity on opening. Steppers and Servos only
  bool     callibration_run = false;         // if true a callibration is running and additional measures are captured
  uint8_t  stopp_armed = 0;                  // Count each step power usage is below limit of 1 Watt
  uint16_t cycle_time = 0;                   // used for shuttersetup to get accurate timing
} ShutterGlobal;

#define SHT_DIV_ROUND(__A, __B) (((__A) + (__B)/2) / (__B))

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/
void ExecuteCommandPowerShutter(uint32_t device, uint32_t state, uint32_t source)
{
  // first implementation for virtual relays. Avoid switching relay numbers that do not exist.
  if (device <= TasmotaGlobal.devices_present) ExecuteCommandPower(device,state,source);
}

void ShutterAllowPreStartProcedure(uint8_t i) {
  // Tricky!!! Execute command status 2 while in the 10 sec loop and you'll end up in an exception
  // Prestart allow e.g. to release a LOCK or something else before the movement start
  // Anyway, as long var1 != 99 this is skipped (luckily)
#ifdef USE_RULES
  uint32_t uptime_Local = 0;
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Delay Start? var%d <99>=<%s>, max10s?"),i + 1, rules_vars[i]);
  uptime_Local = TasmotaGlobal.uptime;
  while (uptime_Local + 10 > TasmotaGlobal.uptime 
          && (String)rules_vars[i] == "99") {
    loop();
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Delay Start. Done"));
#endif  // USE_RULES
}

bool ShutterButtonIsSimultaneousHold(uint32_t button_index, uint32_t shutter_index)
{
  // check for simultaneous shutter button hold
  // used for: Legrand button 077025
  uint32 min_shutterbutton_hold_timer = -1; // -1 == max(uint32)
  for (uint32_t i = 0; i < MAX_SHUTTERS_ESP32*2 ; i++) {
    if ((button_index != i) && (ShutterSettings.shutter_button[i].enabled) && (ShutterSettings.shutter_button[i].shutter_number == shutter_index) && (Button.hold_timer[i] < min_shutterbutton_hold_timer))
      min_shutterbutton_hold_timer = Button.hold_timer[i];
  }
  return ((-1 != min_shutterbutton_hold_timer) && (min_shutterbutton_hold_timer > (Button.hold_timer[button_index]>>1)));
}

bool ShutterButtonHandlerMulti(void)
{
  uint8_t  button               = XdrvMailbox.payload;
  uint32_t button_index         = XdrvMailbox.index;
  uint8_t  shutter_index        = ShutterSettings.shutter_button[button_index].shutter_number;
  uint8_t  button_press_counter = Button.press_counter[button_index];

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MULTI: SHT: Shtr%d, Btn %d, hold %d, dir %d, idx %d, payload %d, last state %d, press cnt %d, window %d, simu_press %d"),
    shutter_index+1, button_index+1, Button.hold_timer[button_index],Shutter[shutter_index].direction,XdrvMailbox.index,XdrvMailbox.payload,
    Button.last_state[button_index], Button.press_counter[button_index], Button.window_timer[button_index], Shutter[shutter_index].button_simu_pressed);

  // multipress event handle back to main procedure
  if (Button.press_counter[button_index]>3) return false;

  if (!Shutter[shutter_index].button_simu_pressed) {
    uint8_t pos_press_index = Button.press_counter[button_index]-1;

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Button %d = %d (single=1, double=2, tripple=3, hold=4)"), shutter_index+1, button_index+1, pos_press_index+1);
    XdrvMailbox.index         = shutter_index +1;
    TasmotaGlobal.last_source = SRC_BUTTON;
    XdrvMailbox.data_len      = 0;
    char databuf[1]           = "";
    XdrvMailbox.data          = databuf;
    XdrvMailbox.command       = NULL;
    int8_t position = ShutterSettings.shutter_button[button_index].position[pos_press_index].pos;
    if (position == -1) {
      position = tmin(100, tmax(0,ShutterRealToPercentPosition(Shutter[XdrvMailbox.index - 1].real_position, XdrvMailbox.index - 1)
                + ShutterSettings.shutter_button[button_index].position[pos_press_index].pos_incrdecr));
    }
    XdrvMailbox.payload = position;
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d -> %d"), shutter_index+1, position);
    if (102 == position) {
      XdrvMailbox.payload = XdrvMailbox.index;
      CmndShutterToggle();
    } else {
      if (position == ShutterRealToPercentPosition(Shutter[XdrvMailbox.index-1].real_position, XdrvMailbox.index-1) ) {
        Shutter[XdrvMailbox.index -1].tilt_target_pos = position == 0 ? Shutter[XdrvMailbox.index -1].tilt_config[0] : (position==100?Shutter[XdrvMailbox.index -1].tilt_config[1]:Shutter[XdrvMailbox.index -1].tilt_target_pos);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d -> Endpoint movement detected at %d. Set Tilt: %d"), shutter_index+1, position, Shutter[XdrvMailbox.index -1].tilt_target_pos);
      }
      // set the tilt
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Target tilt %d for button %d"), ShutterSettings.shutter_button[button_index].position[pos_press_index].tilt, button_index+1);
      switch (ShutterSettings.shutter_button[button_index].position[pos_press_index].tilt) {
        // No change in tilt defined
        case -128:
        break;
        // tilt change defined on position or (127) incr/decr
        case 127:
          Shutter[shutter_index].tilt_target_pos_override = Shutter[shutter_index].tilt_real_pos + ShutterSettings.shutter_button[button_index].position[pos_press_index].tilt_incrdecr;
        break;
        default:
          Shutter[shutter_index].tilt_target_pos_override = ShutterSettings.shutter_button[button_index].position[pos_press_index].tilt;
      }

      // set the tilt
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Target tilt %d for button %d"), Shutter[shutter_index].tilt_target_pos_override, button_index+1);

      // reset button to default
      Button.press_counter[button_index] = 0;
        
      CmndShutterPosition();
    }

    if (ShutterSettings.shutter_button[button_index].position[pos_press_index].mqtt_broadcast) {
      // MQTT broadcast to grouptopic
      char scommand[CMDSZ];
      char stopic[TOPSZ];
      for (uint32_t i = 0; i < MAX_SHUTTERS_ESP32; i++) {
        if ((i==shutter_index) || (ShutterSettings.shutter_button[button_index].mqtt_all)) {
          snprintf_P(scommand, sizeof(scommand),PSTR("ShutterPosition%d"), i+1);
          GetGroupTopic_P(stopic, scommand, SET_MQTT_GRP_TOPIC);
          Response_P("%d", position);
          MqttPublish(stopic, false);
        }
      } // for (uint32_t)
    } // ShutterSettings.shutter_button[button_index].positionmatrix & ((0x01<<26)<<pos_press_index
    // reset counter is served by shutter driver
  } // !Shutter[shutter_index].button_simu_pressed

  Response_P(PSTR("{"));
  ResponseAppend_P(JSON_SHUTTER_BUTTON, shutter_index+1, Shutter[shutter_index].button_simu_pressed ? 0 : button_index+1, button_press_counter);
  ResponseJsonEnd();
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_SHUTTER));
  
  // reset simu pressed record
  Shutter[shutter_index].button_simu_pressed = 0;

  // reset all buttons on this shutter to prevent further actions with the second button comming in
  for (uint32_t i = 0; i < MAX_SHUTTERS_ESP32*2 ; i++) {
    if ((ShutterSettings.shutter_button[i].enabled) && (ShutterSettings.shutter_button[i].shutter_number == shutter_index) ) {
      Button.press_counter[i] = 0;
    }    
  }

  return true;
}

bool ShutterButtonHandler(void)
{
  uint8_t  button           = XdrvMailbox.payload;
  uint32_t button_index     = XdrvMailbox.index;
  uint8_t  shutter_index    = ShutterSettings.shutter_button[button_index].shutter_number;

  if (Button.hold_timer[button_index] > 0 || (button != Button.last_state[button_index] || Button.window_timer[button_index] > 0))  {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Button %d, hold %d, dir %d, index %d, payload %d, last state %d, press counter %d, window %d"),
      shutter_index+1, button_index+1, Button.hold_timer[button_index],Shutter[shutter_index].direction,XdrvMailbox.index,XdrvMailbox.payload,
      Button.last_state[button_index], Button.press_counter[button_index], Button.window_timer[button_index]);
      
      Shutter[shutter_index].button_simu_pressed |= ShutterButtonIsSimultaneousHold( button_index, shutter_index);
      if (Button.hold_timer[button_index] > 100 ) {
        Button.hold_timer[button_index] = 1;         // Reset button hold counter to stay below hold trigger
      }
  }

  // handle on button release: start shutter on shortpress and stop running shutter after longpress.
  if (NOT_PRESSED == button
      && !ShutterSettings.shutter_button[button_index].position[3].mqtt_broadcast // do not stop on hold release if broadcast
      && Shutter[shutter_index].direction != 0                                    // only act on shutters activly moving
      && Button.hold_timer[button_index]   > 0                                    // kick in on first release of botton. do not check for multipress
      ) 
  {
    XdrvMailbox.index   = shutter_index +1;
    XdrvMailbox.payload = -99;  // reset any payload to invalid
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Stop moving shutter"));
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Button %d, hold %d, dir %d, index %d, payload %d"), shutter_index+1, button_index+1, Button.hold_timer[button_index],Shutter[shutter_index].direction,XdrvMailbox.index,XdrvMailbox.payload);
    CmndShutterStop();
    Button.press_counter[button_index] = 0;
    return true;
  }

  // simulatanous press. Stop 
  if (PRESSED == button
      && Shutter[shutter_index].button_simu_pressed  // only if both buttons are pressed simultanously
      && Button.window_timer[button_index] == 0      // time for waiting for multipress window expired
      && Button.press_counter[button_index] > 0      // only execute if at least pressed ONCE. 0==disable
      )
  {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Simu Hold detected"));
    Button.press_counter[button_index] = 0;
    
    // only execute on one of the two pressed buttons
    if ( button_index % 2 ) {
      ShutterButtonHandlerMulti();
    }
  }
  //long press detected. Start moving shutter into direction
  if (PRESSED == button
      && Shutter[shutter_index].direction  == 0   // shutter in STOP Position
      && Button.window_timer[button_index] == 0  // time for waiting for multipress window expired
      && Button.press_counter[button_index] > 0  // only execute if at least pressed ONCE. 0==disable
      ) 
  {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Start shutter after long press"));
    Button.press_counter[button_index] = 4;
    ShutterButtonHandlerMulti();
  }
  return false;
}

void ShutterCalculateAccelerator(uint8_t i)
{
  // No Logging allowed. Part of RTC Timer
  if (Shutter[i].direction != 0) {
    switch (ShutterGlobal.position_mode) {
      case SHT_COUNTER:
      case SHT_PWM_VALUE:
        current_real_position =  Shutter[i].real_position;
        current_pwm_velocity = Shutter[i].pwm_velocity;
        // calculate max velocity allowed in this direction
        velocity_max = Shutter[i].direction == 1 ? ShutterGlobal.open_velocity_max : Shutter[i].close_velocity_max;
        // calculate max change of velocyty based on the defined motordelay in steps
        velocity_change_per_step_max =  velocity_max / (Shutter[i].motordelay > 0 ? Shutter[i].motordelay : 1);
        // minimumtime required from current velocity to stop
        min_runtime_ms = current_pwm_velocity * 1000 / STEPS_PER_SECOND / velocity_change_per_step_max;
        // decellaration way from current velocity
        current_stop_way = min_runtime_ms * STEPS_PER_SECOND * (current_pwm_velocity + velocity_change_per_step_max) * Shutter[i].direction  / 2 / ShutterGlobal.open_velocity_max - (Shutter[i].accelerator<0?Shutter[i].direction*1000*current_pwm_velocity/ShutterGlobal.open_velocity_max:0);
        next_possible_stop_position = current_real_position + current_stop_way ;
        // ensure that the accelerotor kicks in at the first overrun of the target position
        if (  Shutter[i].accelerator < 0 || next_possible_stop_position * Shutter[i].direction > Shutter[i].target_position * Shutter[i].direction ) {
            // if startet to early because of 0.05sec maximum accuracy and final position is to far away (200) accelerate a bit less
            if (next_possible_stop_position * Shutter[i].direction + 200 < Shutter[i].target_position * Shutter[i].direction) {
              Shutter[i].accelerator = -velocity_change_per_step_max * 9/10;
            } else {
              // in any case increase accelleration if overrun is detected during decelleration
              if (next_possible_stop_position * Shutter[i].direction > Shutter[i].target_position * Shutter[i].direction && Shutter[i].accelerator < 0) {
                Shutter[i].accelerator = -velocity_change_per_step_max * 11/10;
              } else {
                // as long as the calculated end position is ok stay with proposed decelleration
                Shutter[i].accelerator = -velocity_change_per_step_max;
              }
            }
            // detect during the acceleration phase the point final speed is reached
        } else if (  Shutter[i].accelerator > 0 && current_pwm_velocity == velocity_max) {
          Shutter[i].accelerator = 0;
        }
      break;
    }
  }
}

int32_t ShutterCalculatePosition(uint32_t i)
{
  // No Logging allowed. Part of RTC Timer
  if (Shutter[i].direction != 0) {
    switch (ShutterGlobal.position_mode) {
      case SHT_COUNTER:
        return ((int64_t)RtcSettings.pulse_counter[i] * Shutter[i].direction * STEPS_PER_SECOND * RESOLUTION / ShutterGlobal.open_velocity_max)+Shutter[i].start_position;
        break;
      case SHT_TIME:
      case SHT_TIME_UP_DOWN:
      case SHT_TIME_GARAGE:
        if (Shutter[i].tilt_config[2] > 0) {
          // the tilt time is defined and therefore the tiltposition must be calculated
          if (Shutter[i].time <= Shutter[i].venetian_delay+Shutter[i].motordelay) {
           Shutter[i].tilt_real_pos = (Shutter[i].tilt_start_pos + ((Shutter[i].direction * (int16_t)(Shutter[i].time - tmin(Shutter[i].motordelay, Shutter[i].time)) * (Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0])) / Shutter[i].tilt_config[2]));
          } else {
            Shutter[i].tilt_real_pos = Shutter[i].direction == 1 ? Shutter[i].tilt_config[1] : Shutter[i].tilt_config[0];
          }
        }
        return Shutter[i].start_position + ( (Shutter[i].time - tmin(Shutter[i].venetian_delay+Shutter[i].motordelay, Shutter[i].time)) * (Shutter[i].direction > 0 ? RESOLUTION : -Shutter[i].close_velocity));
        break;
      case SHT_PWM_TIME:
        break;
      case SHT_PWM_VALUE:
        return Shutter[i].real_position;
      break;
      default:
        break;
    }
  } else {
    return Shutter[i].real_position;
  }
  return 0;  // Never reaches here, Satisfy compiler
}

void ShutterDecellerateForStop(uint8_t i)
{
  bool pwm_apply = false;   // ESP32 only, do we need to apply PWM changes
  switch (ShutterGlobal.position_mode) {
    case SHT_PWM_VALUE:
    case SHT_COUNTER:
      int16_t missing_steps;
      Shutter[i].accelerator = -(ShutterGlobal.open_velocity_max / (Shutter[i].motordelay>4 ? (Shutter[i].motordelay*11)/10 : 4) );

      while (Shutter[i].pwm_velocity > -2*Shutter[i].accelerator &&  Shutter[i].pwm_velocity != PWM_MIN) {
	      delay(50);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Velocity %ld, Delta %d"), Shutter[i].pwm_velocity, Shutter[i].accelerator );
        // Control will be done in RTC Ticker.
      }
      if (ShutterGlobal.position_mode == SHT_COUNTER){
        missing_steps = ((Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND) - RtcSettings.pulse_counter[i];
        //prepare for stop PWM
        Shutter[i].accelerator  = 0;
        Shutter[i].pwm_velocity = 0;
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Remain %d count %d -> target %d, dir %d"), missing_steps, RtcSettings.pulse_counter[i], (uint32_t)(Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND, Shutter[i].direction);
        while (RtcSettings.pulse_counter[i] < (uint32_t)(Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND && missing_steps > 0) {
        }
        TasmotaGlobal.pwm_value[i] = 0;
        pwm_apply = true;
        Shutter[i].real_position = ShutterCalculatePosition(i);
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Remain steps %d"), missing_steps);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Real %d, Pulsecount %d, tobe %d, Start %d"), Shutter[i].real_position,RtcSettings.pulse_counter[i],  (uint32_t)(Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND, Shutter[i].start_position);
      }
      Shutter[i].direction = 0;
      Shutter[i].pwm_velocity = 0;
    break;
  }
  if (pwm_apply) { PwmApplyGPIO(false); }
}

uint16_t ShutterGetCycleTime(uint8_t i, uint8_t  max_runtime) {
  uint32_t cycle_time = 0;
  bool     started    = false;
  uint32_t last_time;
  char     time_chr[10];

  last_time = millis();
  while (!started && millis() - last_time < max_runtime * 1000) {
    loop();
    if (Shutter[i].direction) {
      started   = true;
    }
  }
  if (!started) return 0;
  AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup. Start detected. Waiting for STOP"));
  while (Shutter[i].direction && millis() - last_time < max_runtime * 1000) {
    loop();
  }
  if (Shutter[i].direction) {
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup. No stop detected... Cancel"));
    return 0;
  } 
  cycle_time =  (ShutterGlobal.cycle_time / 2) - (Shutter[i].motordelay * 10 / STEPS_PER_SECOND) ;
  dtostrfd((float)(cycle_time) / 10, 1, time_chr);
  AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup. Cycletime is: %s sec"), time_chr);
  return cycle_time;
}

uint8_t ShutterGetOptions(uint8_t index) {
   return ShutterSettings.shutter_options[index];
}

uint8_t ShutterGetRelayNoFromBitfield(power_t number) {
  int position = 0;
  while (number != 0) {
    position++;
    if (number & 1) return position;
    number >>= 1;
  }
  return 0; // return 0 if no relay found
}

uint8_t ShutterGetStartRelay(uint8_t index) {
  return ShutterSettings.shutter_startrelay[index];
}

int8_t ShutterGetTiltConfig(uint8_t config_idx,uint8_t index) {
  return Shutter[index].tilt_config[config_idx];
}

void ShutterInit(void)
{
  TasmotaGlobal.shutters_present = 0;
  ShutterGlobal.RelayShutterMask = 0;
  //Initialize to get relay that changed
  ShutterGlobal.RelayOldMask = TasmotaGlobal.power;

  ShutterGlobal.open_velocity_max = ShutterSettings.open_velocity_max;
  for (uint32_t i = 0; i < MAX_SHUTTERS_ESP32; i++) {
    // set startrelay to 1 on first init, but only to shutter 1. 90% usecase
    if (ShutterSettings.shutter_startrelay[i] && (ShutterSettings.shutter_startrelay[i] <= 32 )) {
      bool relay_in_interlock = false;
      TasmotaGlobal.shutters_present++;

      // Add the two relays to the mask to knaw they belong to shutters
      ShutterGlobal.RelayShutterMask |= 3 << (ShutterSettings.shutter_startrelay[i] -1)  ;

      // All shutters must have same mode. Switch OR Pulse. N
      switch (Settings->pulse_timer[i]) {
        case 0:
          Shutter[i].switch_mode = SHT_SWITCH;
        break;
        default:
          Shutter[i].switch_mode = SHT_PULSE;
        break;
      }

      // Check if the relay is in an INTERLOCK group. required to set the right mode or
      // verify that on SHT_TIME INTERLOCK is set
      for (uint32_t j = 0; j < MAX_INTERLOCKS * Settings->flag.interlock; j++) {  // CMND_INTERLOCK - Enable/disable interlock
        //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Interlock state i=%d %d, flag %d, Shuttermask %d, MaskedIL %d"),i, ShutterSettings.interlock[i], ShutterSettings.flag.interlock,ShutterGlobal.RelayShutterMask, ShutterSettings.interlock[i]&ShutterGlobal.RelayShutterMask);
        if (Settings->interlock[j] && (Settings->interlock[j] & ShutterGlobal.RelayShutterMask)) {
          //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Relay in Interlock group"));
          relay_in_interlock = true;
        }
      }

      if (ShutterSettings.shutter_mode == SHT_AUTOCONFIG || ShutterSettings.shutter_mode == SHT_UNDEF) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Mode undef.. calculate..."));
        ShutterGlobal.position_mode = SHT_TIME;
        if (!relay_in_interlock) {
          // temporary to maintain old functionality
          if (ShutterSettings.shutter_mode == SHT_UNDEF) {
            ShutterGlobal.position_mode = SHT_TIME_UP_DOWN;
          }
          if (PinUsed(GPIO_PWM1, i) && PinUsed(GPIO_CNTR1, i)) {
            ShutterGlobal.position_mode = SHT_COUNTER;
          }
        }
      } else {
        ShutterGlobal.position_mode = ShutterSettings.shutter_mode;
      }
      AddLog(LOG_LEVEL_INFO, PSTR("SHT: ShutterMode: %d"), ShutterGlobal.position_mode);
      // main function for stepper and servos to control velocity and acceleration.
      TickerShutter.attach_ms(50, ShutterRtc50mS );

      // default the 50 percent should not have any impact without changing it. set to 60
      ShutterSettings.shutter_set50percent[i] = (ShutterSettings.shutter_set50percent[i] > 0) ? ShutterSettings.shutter_set50percent[i] : 50;

      // use 10 sec. as default to allow everybody to play without deep initialize
      Shutter[i].open_time  = ShutterSettings.shutter_opentime[i]  = (ShutterSettings.shutter_opentime[i]  > 0) ? ShutterSettings.shutter_opentime[i] : 100;
      Shutter[i].close_time = ShutterSettings.shutter_closetime[i] = (ShutterSettings.shutter_closetime[i] > 0) ? ShutterSettings.shutter_closetime[i] : 100;

      // Update Calculation 20 because time interval is 0.05 sec ans time is in 0.1sec
      Shutter[i].open_max = STEPS_PER_SECOND * RESOLUTION * Shutter[i].open_time / 10;
      Shutter[i].close_velocity =  Shutter[i].open_max / Shutter[i].close_time / 2 ;

      // calculate a ramp slope at the first 5 percent to compensate that shutters move with down part later than the upper part
      if (ShutterSettings.shutter_set50percent[i] != 50) {
       	ShutterSettings.shuttercoeff[1][i] = Shutter[i].open_max/10 * (100 - ShutterSettings.shutter_set50percent[i] ) / 5000 ;
      	ShutterSettings.shuttercoeff[0][i] = Shutter[i].open_max/100 - (ShutterSettings.shuttercoeff[1][i] * 10);
      	ShutterSettings.shuttercoeff[2][i] = (int32_t)(ShutterSettings.shuttercoeff[0][i] * 10 + 5 * ShutterSettings.shuttercoeff[1][i]) / 5;
        //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Shutter[i].open_max %d, 50perc:%d, 0:%d, 1:%d 2:%d"), i, Shutter[i].open_max, ShutterSettings.shutter_set50percent[i], ShutterSettings.shuttercoeff[0][i],ShutterSettings.shuttercoeff[1][i],ShutterSettings.shuttercoeff[2][i]);
      }
      ShutterGlobal.RelayShutterMask |= 3 << (ShutterSettings.shutter_startrelay[i] - 1);

      Shutter[i].real_position  = ShutterPercentToRealPosition(ShutterSettings.shutter_position[i], i);

      Shutter[i].start_position = Shutter[i].target_position = Shutter[i].real_position;
      Shutter[i].motordelay     = ShutterSettings.shutter_motordelay[i];
      Shutter[i].lastdirection  = (50 < ShutterSettings.shutter_position[i]) ? 1 : -1;

      // Venetian Blind
      // ensure min is smaller than max
      ShutterSettings.shutter_tilt_config[2][i] = ShutterSettings.shutter_tilt_config[0][i] >= ShutterSettings.shutter_tilt_config[1][i]?0:ShutterSettings.shutter_tilt_config[2][i];
      //copy config to shutter
      for (uint8_t k = 0; k < 5; k++) {
        Shutter[i].tilt_config[k] =  ShutterSettings.shutter_tilt_config[k][i];
      }
      // wipe open/close position if duration is 0
      if (Shutter[i].tilt_config[2] == 0) {
        Shutter[i].tilt_config[3] = Shutter[i].tilt_config[4] = 0;
      }
      Shutter[i].tilt_target_pos        = Shutter[i].tilt_real_pos = ShutterSettings.shutter_tilt_pos[i];
      Shutter[i].tilt_velocity          = Shutter[i].tilt_config[2] > 0 ? ((Shutter[i].tilt_config[1] - Shutter[i].tilt_config[0]) / Shutter[i].tilt_config[2]) + 1  : 1;
      Shutter[i].close_velocity_max     = ShutterGlobal.open_velocity_max*Shutter[i].open_time / Shutter[i].close_time;
      Shutter[i].min_realPositionChange = 2 * tmax(ShutterGlobal.open_velocity_max, Shutter[i].close_velocity_max);
      Shutter[i].min_TiltChange         = 2 * Shutter[i].tilt_velocity;

      switch (ShutterGlobal.position_mode) {
        case SHT_PWM_VALUE:
          ShutterGlobal.open_velocity_max =  RESOLUTION;
          // Initiate pwm range with defaults if not already set.
          ShutterSettings.shutter_pwmrange[0][i] = ShutterSettings.shutter_pwmrange[0][i] > 0 ? ShutterSettings.shutter_pwmrange[0][i] : pwm_servo_min;
          ShutterSettings.shutter_pwmrange[1][i] = ShutterSettings.shutter_pwmrange[1][i] > 0 ? ShutterSettings.shutter_pwmrange[1][i] : pwm_servo_max;
          Shutter[i].min_realPositionChange = 0;
          Shutter[i].min_TiltChange         = 0;
        break;
        case SHT_TIME:
          // Test is the relays are in interlock mode. Disable shuttermode if error
          if (!relay_in_interlock) {
            TasmotaGlobal.shutters_present = 0,
            AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ERROR: Shtr%d Relays are not in INTERLOCK. Pls read documentation. Shutter DISABLE. Fix and REBOOT"), i+1);
            return;
          }
        break;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d min realpos_chg: %d, min tilt_chg %d"), i+1, Shutter[i].min_realPositionChange, Shutter[i].min_TiltChange);
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d Openvel %d, Closevel: %d"), i+1, ShutterGlobal.open_velocity_max, Shutter[i].close_velocity_max);
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Init. Pos %d, Inv %d, Locked %d, Endstop enab %d, webButt inv %d, Motordel: %d"),
        i+1,  Shutter[i].real_position,
        (ShutterSettings.shutter_options[i] & 1) ? 1 : 0, (ShutterSettings.shutter_options[i] & 2) ? 1 : 0, (ShutterSettings.shutter_options[i] & 4) ? 1 : 0, (ShutterSettings.shutter_options[i] & 8) ? 1 : 0, Shutter[i].motordelay);

    } else {
      // terminate loop at first INVALID Shutter[i].
      break;
    }
    ShutterLimitRealAndTargetPositions(i);
    ShutterSettings.shutter_accuracy = 1;
    ShutterSettings.shutter_mode = ShutterGlobal.position_mode;
    // initialize MotorStop time with 500ms if not set
    // typical not set start values are 0 and 65535
    if (ShutterSettings.shutter_motorstop > 5000 || ShutterSettings.shutter_motorstop == 0) {
      ShutterSettings.shutter_motorstop = 500;
    }
  }
}

void ShutterLimitRealAndTargetPositions(uint32_t i)
{
  if (Shutter[i].real_position<0) Shutter[i].real_position                       = 0;
  if (Shutter[i].real_position>Shutter[i].open_max) Shutter[i].real_position     = Shutter[i].open_max;
  if (Shutter[i].target_position<0) Shutter[i].target_position                   = 0;
  if (Shutter[i].target_position>Shutter[i].open_max) Shutter[i].target_position = Shutter[i].open_max;
}

void ShutterLogPos(uint32_t i)
{
  char stemp2[10];
  dtostrfd((float)Shutter[i].time / STEPS_PER_SECOND, 2, stemp2);
  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Real %d, Start %d, Stop %d, Dir %d, Delay %d, Rtc %s [s], Freq %d, PWM %d, Tilt %d"),
    i+1, Shutter[i].real_position, Shutter[i].start_position, Shutter[i].target_position, Shutter[i].direction, Shutter[i].motordelay, stemp2,
    Shutter[i].pwm_velocity, Shutter[i].pwm_value,Shutter[i].tilt_real_pos);
}

void ShutterOptionsSetHelper(uint16_t option)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.payload == 0) {
      ShutterSettings.shutter_options[XdrvMailbox.index -1] &= ~(option);
    } else if (XdrvMailbox.payload == 1) {
      ShutterSettings.shutter_options[XdrvMailbox.index -1] |= (option);
    }
    ResponseCmndIdxNumber((ShutterSettings.shutter_options[XdrvMailbox.index -1] & option) ? 1 : 0);
  }
}

int32_t ShutterPercentToRealPosition(int16_t percent, uint32_t index)
{
	if (ShutterSettings.shutter_set50percent[index] != 50) {
    return (percent <= 5) ? ShutterSettings.shuttercoeff[2][index] * percent*10 : (ShutterSettings.shuttercoeff[1][index] * percent + (ShutterSettings.shuttercoeff[0][index]*10))*10;
	} else {
    int64_t realpos;
    // check against DIV 0
    for (uint32_t j = 0; j < 5; j++) {
      if (0 == ShutterSettings.shuttercoeff[j][index]) {
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: RESET/INIT CALIBRATION MATRIX DIV 0"));
        for (uint32_t k = 0; k < 5; k++) {
          ShutterSettings.shuttercoeff[k][index] = SHT_DIV_ROUND(calibrate_pos[k+1] * 1000, calibrate_pos[5]);
        }
      }
    }
    for (uint32_t k = 0; k < 5; k++) {
      if ((percent * 10) >= ShutterSettings.shuttercoeff[k][index]) {
        realpos = SHT_DIV_ROUND(Shutter[index].open_max * calibrate_pos[k + 1], 100);
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realposition TEMP1: %d, %d %%, coeff %d"), realpos, percent, ShutterSettings.shuttercoeff[k][index]);
      } else {
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Shutter[%d].open_max: %d"),index, Shutter[index].open_max);
        if (0 == k) {
          realpos =  SHT_DIV_ROUND((int64_t)percent * Shutter[index].open_max * calibrate_pos[k + 1], ShutterSettings.shuttercoeff[k][index]*10 );
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realposition TEMP3: %d, %d %%, coeff %d"), realpos, percent, ShutterSettings.shuttercoeff[k][index]);
        } else {
          //uint32_t addon = ( percent*10 - ShutterSettings.shuttercoeff[k-1][index] ) * Shutter[index].open_max * (calibrate_pos[k+1] - calibrate_pos[k]) / (ShutterSettings.shuttercoeff[k][index] -ShutterSettings.shuttercoeff[k-1][index]) / 100;
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realposition TEMP2: %d, %d %%, coeff %d"), addon, (calibrate_pos[k+1] - calibrate_pos[k]), (ShutterSettings.shuttercoeff[k][index] -ShutterSettings.shuttercoeff[k-1][index]));
          realpos += SHT_DIV_ROUND(((int64_t)percent * 10 - ShutterSettings.shuttercoeff[k - 1][index] ) * Shutter[index].open_max * (calibrate_pos[k + 1] - calibrate_pos[k]), (ShutterSettings.shuttercoeff[k][index] - ShutterSettings.shuttercoeff[k - 1][index]) * 100);
        }
        break;
      }
    }
		return realpos < 0 ? 0 : realpos;
	}
}

void ShutterPowerOff(uint8_t i)
{
  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Stop %d Mode %d time %d, last source %d"), i+1,Shutter[i].switch_mode, Shutter[i].time, TasmotaGlobal.last_source); // fix log to indicate correct shutter number
  ShutterDecellerateForStop(i);
  uint8_t cur_relay = ShutterSettings.shutter_startrelay[i] + (Shutter[i].direction == 1 ? 0 : (uint8_t)(ShutterGlobal.position_mode == SHT_TIME)) ;
  if (Shutter[i].direction !=0) {
    Shutter[i].direction = 0;
  }
  if (Shutter[i].real_position == Shutter[i].start_position)  {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Update target tilt shutter %d from %d to %d"), i+1,  Shutter[i].tilt_target_pos , Shutter[i].tilt_real_pos);
    Shutter[i].tilt_target_pos = Shutter[i].tilt_real_pos;
  }
  TasmotaGlobal.rules_flag.shutter_moved = 1;
  switch (Shutter[i].switch_mode) {
    case SHT_SWITCH:
      for (int8_t k = 0; k < 2; k++) {
        if ((1 << (ShutterSettings.shutter_startrelay[i] + k - 1)) & TasmotaGlobal.power) {
          ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[i] + k, 0, SRC_SHUTTER);
        }
      }
    break;
    case SHT_PULSE:
      // we have a momentary switch here. Needs additional pulse on same relay after the end
      switch (TasmotaGlobal.last_source) {
        case SRC_PULSETIMER:
        case SRC_SHUTTER:
        case SRC_WEBGUI:
          ExecuteCommandPowerShutter(cur_relay, 1, SRC_SHUTTER);
          // switch off direction relay to make it power less
          if (((1 << (ShutterSettings.shutter_startrelay[i])) & TasmotaGlobal.power)  && ShutterSettings.shutter_startrelay[i] + 1 != cur_relay) {
            ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[i] + 1, 0, SRC_SHUTTER);
          }
          break;
        default:
          TasmotaGlobal.last_source = SRC_SHUTTER;
      }
    break;
  }
  // Store current PWM value to ensure proper position after reboot.
  switch (ShutterGlobal.position_mode) {
    case SHT_PWM_VALUE:
      Shutter[i].pwm_value = SHT_DIV_ROUND((ShutterSettings.shutter_pwmrange[1][i]-ShutterSettings.shutter_pwmrange[0][i]) * Shutter[i].target_position , Shutter[i].open_max)+ShutterSettings.shutter_pwmrange[0][i];
      analogWrite(Pin(GPIO_PWM1, i), Shutter[i].pwm_value);
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: PWM final %d"),Shutter[i].pwm_value);
      char scmnd[20];
  #ifdef SHUTTER_CLEAR_PWM_ONSTOP
      // free the PWM servo lock on stop.
      analogWrite(Pin(GPIO_PWM1, i), 0);
  #endif
      break;
  }

  // restore save_data behavior if all shutters are in stopped state
  bool shutter_all_stopped = true;
  for (uint8_t j = 0 ; j < TasmotaGlobal.shutters_present ; j++) {
    if (Shutter[j].direction != 0)
      shutter_all_stopped = false;
  }
  if (shutter_all_stopped)
    TasmotaGlobal.save_data_counter = Settings->save_data;

  Shutter[i].last_stop_time = millis();
}

uint8_t ShutterRealToPercentPosition(int32_t realpos, uint32_t index)
{
  int64_t realpercent;
  if (realpos == -9999) {
    realpos = Shutter[index].real_position;
  }
	if (ShutterSettings.shutter_set50percent[index] != 50) {
		realpercent = (ShutterSettings.shuttercoeff[2][index] * 5 > realpos / 10) ? SHT_DIV_ROUND(realpos / 10, ShutterSettings.shuttercoeff[2][index]) : SHT_DIV_ROUND(realpos / 10 - ShutterSettings.shuttercoeff[0][index] * 10, ShutterSettings.shuttercoeff[1][index]);
	} else {
    for (uint32_t j = 0; j < 5; j++) {
      if (realpos >= Shutter[index].open_max * calibrate_pos[j + 1] / 100) {
        realpercent = SHT_DIV_ROUND(ShutterSettings.shuttercoeff[j][index], 10);
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realpercent TEMP1: %d %%, %d, coeff %d"), realpercent, realpos, Shutter[index].open_max * calibrate_pos[j+1] / 100);
      } else {
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Shutter[%d].open_max: %d"),index, Shutter[index].open_max);
        if (0 == j) {
          realpercent  = SHT_DIV_ROUND(((int64_t)realpos - SHT_DIV_ROUND(Shutter[index].open_max * calibrate_pos[j], 100)) * ShutterSettings.shuttercoeff[j][index], calibrate_pos[j + 1] / 10 * Shutter[index].open_max);
        } else {
          //uint16_t addon = ( realpos - (Shutter[index].open_max * calibrate_pos[j] / 100) ) * 10 * (ShutterSettings.shuttercoeff[j][index] - ShutterSettings.shuttercoeff[j-1][index]) / (calibrate_pos[j+1] - calibrate_pos[j])/Shutter[index].open_max;
          //uint16_t addon = ( realpercent*10 - ShutterSettings.shuttercoeff[j-1][index] ) * Shutter[index].open_max * (calibrate_pos[j+1] - calibrate_pos[j]) / (ShutterSettings.shuttercoeff[j][index] -ShutterSettings.shuttercoeff[j-1][index]) / 100;
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realpercent TEMP2: %d %%, delta %d, %d, coeff %d"), addon,( realpos - (Shutter[index].open_max * calibrate_pos[j] / 100) ) , (calibrate_pos[j+1] - calibrate_pos[j])* Shutter[index].open_max/100, (ShutterSettings.shuttercoeff[j][index] -ShutterSettings.shuttercoeff[j-1][index]));
         realpercent += SHT_DIV_ROUND(((int64_t)realpos - SHT_DIV_ROUND(Shutter[index].open_max * calibrate_pos[j], 100)) * (ShutterSettings.shuttercoeff[j][index] - ShutterSettings.shuttercoeff[j - 1][index]), (calibrate_pos[j + 1] - calibrate_pos[j]) / 10 * Shutter[index].open_max) ;
	      }
        break;
      }
    }
  }
  realpercent = realpercent < 0 ? 0 : realpercent;
  return realpercent;
}

void ShutterRelayChanged(void)
{
  // ShutterGlobal.RelayCurrentMask = binary relay that was recently changed and cause an Action
  // powerstate_local = binary powermatrix and relays from shutter: 0..3
  // relays_changed = bool if one of the relays that belong to the shutter changed not by shutter or pulsetimer
  char stemp1[10];

	for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
		power_t powerstate_local = (TasmotaGlobal.power >> (ShutterSettings.shutter_startrelay[i] - 1)) & 3;
    // SRC_IGNORE added because INTERLOCK function bite causes this as last source for changing the relay.
		//uint8   manual_relays_changed = ((ShutterGlobal.RelayCurrentMask >> (ShutterSettings.shutter_startrelay[i] -1)) & 3) && SRC_IGNORE != TasmotaGlobal.last_source && SRC_SHUTTER != TasmotaGlobal.last_source && SRC_PULSETIMER != TasmotaGlobal.last_source ;
    uint8   manual_relays_changed = ((ShutterGlobal.RelayCurrentMask >> (ShutterSettings.shutter_startrelay[i] - 1)) & 3) && SRC_SHUTTER != TasmotaGlobal.last_source && SRC_PULSETIMER != TasmotaGlobal.last_source ;
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Source %s, Powerstate %ld, RelayMask %d, ManualChange %d"),
    //  i+1, GetTextIndexed(stemp1, sizeof(stemp1), TasmotaGlobal.last_source, kCommandSource), powerstate_local,ShutterGlobal.RelayCurrentMask,manual_relays_changed);
    if (manual_relays_changed) {
      ShutterLimitRealAndTargetPositions(i);
      switch (Shutter[i].switch_mode ) {
        case SHT_PULSE:
          if (Shutter[i].direction != 0 && powerstate_local) {
            Shutter[i].target_position = Shutter[i].real_position;
            powerstate_local = 0;
  					AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Switch OFF motor. Target %ld, Source %s, Powerstate %ld, RelayMask %d, ManualChange %d"),
              i+1, Shutter[i].target_position, GetTextIndexed(stemp1, sizeof(stemp1), TasmotaGlobal.last_source, kCommandSource), powerstate_local,ShutterGlobal.RelayCurrentMask,manual_relays_changed);
          }
        break;
        default:
          TasmotaGlobal.last_source = SRC_SHUTTER; // avoid switch off in the next loop
          if (Shutter[i].direction != 0 ) Shutter[i].target_position = Shutter[i].real_position;
      }
      if (powerstate_local > 0) {
        Shutter[i].tiltmoving = 0;
      }
         switch (ShutterGlobal.position_mode) {
          // enum Shutterposition_mode {SHT_TIME, SHT_TIME_UP_DOWN, SHT_TIME_GARAGE, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME,};
          case SHT_TIME_UP_DOWN:
          case SHT_COUNTER:
          case SHT_PWM_VALUE:
          case SHT_PWM_TIME:
            ShutterPowerOff(i);			 
          case SHT_TIME: {
            // powerstate_local == 0 => direction=0, stop
            // powerstate_local == 1 => direction=1, target=Shutter[i].open_max
            // powerstate_local == 2 => direction=-1, target=0 // only happen on SHT_TIME
            // powerstate_local == 3 => direction=-1, target=0 // only happen if NOT SHT_TIME
            int8_t direction = (powerstate_local == 0) ? 0 : (powerstate_local == 1) ? 1 : -1;
            int32_t target   = (powerstate_local == 1) ? Shutter[i].open_max : 0;

            if (direction != 0) {
              ShutterStartInit(i, direction, target);
            } else {
              Shutter[i].target_position = Shutter[i].real_position;
              Shutter[i].last_stop_time  = millis();
            }
            break;
          }
          case SHT_TIME_GARAGE:
            switch (powerstate_local) {
              case 1:
                ShutterStartInit(i, Shutter[i].lastdirection * -1, Shutter[i].lastdirection == 1 ? 0 : Shutter[i].open_max);
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d Garage. NewTarget %d"), i, Shutter[i].target_position);
                break;
              default:
                Shutter[i].target_position = Shutter[i].real_position;
            }
        }        
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Target %ld, Power: %d, tiltmv: %d"), i+1, Shutter[i].target_position, powerstate_local,Shutter[i].tiltmoving);
		 } // if (manual_relays_changed)
  } // for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++)
}

void ShutterReportPosition(bool always, uint32_t index)
{
  Response_P(PSTR("{"));
  uint32_t i = 0;
  uint32_t n = TasmotaGlobal.shutters_present;
  uint8_t shutter_running = 0;
  for (i; i < n; i++) {
    if (Shutter[i].direction != 0) {
      shutter_running++;
    }
  }

  // Allow function exit if nothing to report (99.9% use case)
  if (!always && !shutter_running) return;

  if( index != MAX_SHUTTERS_ESP32) {
    i = index;
    n = index+1;
  } else {
    i = 0;
  }
  for (i; i < n; i++) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Real Pos %d"), i+1,Shutter[i].real_position);

    if (Shutter[i].direction != 0) {
      ShutterLogPos(i);
      shutter_running++;
    }
    if (i && index == MAX_SHUTTERS_ESP32) { ResponseAppend_P(PSTR(",")); }
    uint32_t position = ShutterRealToPercentPosition(Shutter[i].real_position, i);
    uint32_t target   = ShutterRealToPercentPosition(Shutter[i].target_position, i);
    ResponseAppend_P(JSON_SHUTTER_POS, i + 1, (ShutterSettings.shutter_options[i] & 1) ? 100 - position : position, Shutter[i].direction,(ShutterSettings.shutter_options[i] & 1) ? 100 - target : target, Shutter[i].tilt_real_pos );
   }
  ResponseJsonEnd();
  if (always || shutter_running) {
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_SHUTTER));  // RulesProcess() now re-entry protected
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: rules_flag.shutter_moving: %d, moved %d"), TasmotaGlobal.rules_flag.shutter_moving, TasmotaGlobal.rules_flag.shutter_moved);
}

void ShutterRtc50mS(void)
{
  bool pwm_apply = false;   // ESP32 only, do we need to apply PWM changes
  // No Logging allowed. RTC Timer
  for (uint8_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
    if (Shutter[i].direction) {
      // update position data before increasing counter
      Shutter[i].real_position =  ShutterCalculatePosition(i);
      Shutter[i].time++;
      ShutterCalculateAccelerator(i);
      switch (ShutterGlobal.position_mode) {
        case SHT_PWM_VALUE:
          ShutterUpdateVelocity(i);
          Shutter[i].real_position += Shutter[i].direction > 0 ? Shutter[i].pwm_velocity : (Shutter[i].direction < 0 ? -Shutter[i].pwm_velocity : 0);
          Shutter[i].pwm_value      = SHT_DIV_ROUND((ShutterSettings.shutter_pwmrange[1][i]-ShutterSettings.shutter_pwmrange[0][i]) * Shutter[i].real_position , Shutter[i].open_max)+ShutterSettings.shutter_pwmrange[0][i];
          analogWrite(Pin(GPIO_PWM1, i), Shutter[i].pwm_value);
        break;

        case SHT_COUNTER:
          if (Shutter[i].accelerator) {
            //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Accelerator i=%d -> %d"),i, Shutter[i].accelerator);
            ShutterUpdateVelocity(i);
            digitalWrite(Pin(GPIO_PWM1, i), LOW);
            analogWriteFreq(Shutter[i].pwm_velocity,Pin(GPIO_PWM1, i));
            TasmotaGlobal.pwm_value[i] = 512;
            pwm_apply = true;
          }
        break;
      }
    } // if (Shutter[i].direction)
  }
  if (pwm_apply) { PwmApplyGPIO(false); }
}

void ShutterSetPosition(uint32_t device, uint32_t position)
{
  char svalue[32];                   // Command and number parameter
  snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_POSITION "%d %d"), device, position);
  ExecuteCommand(svalue, SRC_SHUTTER);
}

void ShutterSettingsDefault(void) {
  // Init default values in case file is not found
  AddLog(LOG_LEVEL_INFO, PSTR("SHT: " D_USE_DEFAULTS));

  memset(&ShutterSettings, 0x00, sizeof(ShutterSettings));
  ShutterSettings.version = SHUTTER_VERSION;

  // Init any other parameter in struct ShutterSettings
  ShutterSettings.open_velocity_max = ShutterGlobal.open_velocity_max;
  ShutterSettings.shutter_accuracy  = Settings->shutter_accuracy;
  ShutterSettings.shutter_mode      = Settings->shutter_mode;
  ShutterSettings.shutter_motorstop = Settings->shutter_motorstop;
  for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
    // copy values from settings
    for (uint32_t j = 0; j < 5; j++) {
      if (j<2) ShutterSettings.shutter_pwmrange[j][i] = Settings->shutter_pwmrange[j][i];
      ShutterSettings.shutter_tilt_config[j][i]  = Settings->shutter_tilt_config[j][i];
      ShutterSettings.shuttercoeff[j][i]         = Settings->shuttercoeff[j][i];
    }
    ShutterSettings.shutter_tilt_pos[i]     = Settings->shutter_tilt_pos[i];
    ShutterSettings.shutter_opentime[i]     = Settings->shutter_opentime[i];
    ShutterSettings.shutter_closetime[i]    = Settings->shutter_closetime[i];
    ShutterSettings.shutter_options[i]      = Settings->shutter_options[i];
    ShutterSettings.shutter_set50percent[i] = Settings->shutter_set50percent[i];
    ShutterSettings.shutter_position[i]     = Settings->shutter_position[i];
    ShutterSettings.shutter_startrelay[i]   = Settings->shutter_startrelay[i];
    ShutterSettings.shutter_motordelay[i]   = Settings->shutter_motordelay[i];
    
  }
  for (uint32_t i = 0; i < MAX_SHUTTER_KEYS; i++) { 
    ShutterSettings.shutter_button[i].shutter_number = Settings->shutter_button[i] & 0x03;
    ShutterSettings.shutter_button[i].enabled        = Settings->shutter_button[i] &(1<<31);
    for (uint8_t j = 0; j < 4; j++) {
      ShutterSettings.shutter_button[i].position[j].pos            = (((Settings->shutter_button[i]>> (2+6*j))&(0x3f))-1)<<1;
      ShutterSettings.shutter_button[i].position[j].tilt           = -128;  // -128 == DISBALED
      ShutterSettings.shutter_button[i].position[j].mqtt_broadcast = ((Settings->shutter_button[i]>>(26+j))&(0x01)!=0);
    }
  }
  for (uint32_t i = MAX_SHUTTERS; i < MAX_SHUTTERS_ESP32; i++) {
    ShutterSettings.shutter_set50percent[i] = 50;
    ShutterSettings.shutter_opentime[i]     = 100;
    ShutterSettings.shutter_closetime[i]    = 100;
    ShutterSettings.shutter_pwmrange[0][i]  = pwm_servo_min;
    ShutterSettings.shutter_pwmrange[1][i]  = pwm_servo_max;
  }
}

void ShutterSettingsDelta(void) {
  // Fix possible setting deltas
  if (ShutterSettings.version != SHUTTER_VERSION) {      // Fix version dependent changes

    if (ShutterSettings.version < 0x0100) {
      for (uint8_t i=0; i < MAX_SHUTTERS_ESP32; i++){
        if (ShutterSettings.shutter_startrelay[i] == 0) continue;
        AddLog(LOG_LEVEL_INFO, PSTR("SHT:    %s SHT%d:%d"),D_CMND_SHUTTER_RELAY,i+1,ShutterSettings.shutter_startrelay[i]);
        AddLog(LOG_LEVEL_INFO, PSTR("SHT:  %s: %d"),D_CMND_SHUTTER_OPENTIME,ShutterSettings.shutter_opentime[i]);
        AddLog(LOG_LEVEL_INFO, PSTR("SHT: %s: %d"),D_CMND_SHUTTER_CLOSETIME,ShutterSettings.shutter_closetime[i]);
      }
      for (uint8_t i=0; i < MAX_SHUTTERS_ESP32*2; i++){
        ShutterSettings.shutter_button[i].enabled        = ShutterSettings.shutter_button_old[i].enabled;
        ShutterSettings.shutter_button[i].mqtt_all       = ShutterSettings.shutter_button_old[i].mqtt_all;
        ShutterSettings.shutter_button[i].shutter_number = ShutterSettings.shutter_button_old[i].shutter_number;
        for (uint8_t j = 0; j < 4; j++){
          ShutterSettings.shutter_button[i].position[j].pos            = ShutterSettings.shutter_button_old[i].position[j].pos;
          ShutterSettings.shutter_button[i].position[j].tilt           = ShutterSettings.shutter_button_old[i].position[j].tilt;
          ShutterSettings.shutter_button[i].position[j].mqtt_broadcast = ShutterSettings.shutter_button_old[i].position[j].mqtt_broadcast;
        }
      }
    }

    // Set current version and save settings
    ShutterSettings.version = SHUTTER_VERSION;
    ShutterSettingsSave();
  }
}

void ShutterSettingsLoad(bool erase) {
  // Called from FUNC_PRE_INIT once at restart

  // Init default values in case file is not found
  ShutterSettingsDefault();

  // Try to load file /.drvset027
  char filename[20];
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_27);

  AddLog(LOG_LEVEL_INFO, PSTR("SHT: About to load settings from file %s"), filename);

#ifdef USE_UFILESYS
  if (erase) {
    TfsDeleteFile(filename);  // Use defaults
  }
  else if (TfsLoadFile(filename, (uint8_t*)&ShutterSettings, sizeof(ShutterSettings))) {
    // Fix possible setting deltas
    ShutterSettingsDelta();
  } else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_INFO, PSTR(D_ERROR_FILE_NOT_FOUND));

  }
#else
  AddLog(LOG_LEVEL_INFO, D_ERROR_FILESYSTEM_NOT_READY);
#endif  // USE_UFILESYS

}

bool ShutterSettingsRestore(void) {
  XdrvMailbox.data  = (char*)&ShutterSettings;
  XdrvMailbox.index = sizeof(ShutterSettings);
  return true;
}

void ShutterSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&ShutterSettings +4, sizeof(ShutterSettings) -4);  // Skip crc32
  if (crc32 != ShutterSettings.crc32 && ShutterSettings.version > 0) {
    // Try to save file /.drvset027
    ShutterSettings.crc32 = crc32;

    char filename[20];
    // Use for drivers:
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_27);

    AddLog(LOG_LEVEL_INFO, PSTR("SHT: About to save settings to file %s"), filename);

#ifdef USE_UFILESYS
    if (!TfsSaveFile(filename, (const uint8_t*)&ShutterSettings, sizeof(ShutterSettings))) {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_INFO, D_ERROR_FILE_NOT_FOUND);
    }
#else
    AddLog(LOG_LEVEL_INFO, D_ERROR_FILESYSTEM_NOT_READY);
#endif  // USE_UFILESYS
  }
}

void ShutterShow()
{
  for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
    WSContentSend_P(HTTP_MSG_SLIDER_SHUTTER,  (ShutterGetOptions(i) & 1) ? D_OPEN : D_CLOSE,(ShutterGetOptions(i) & 1) ? D_CLOSE : D_OPEN, (ShutterGetOptions(i) & 1) ? (100 - ShutterRealToPercentPosition(-9999, i)) : ShutterRealToPercentPosition(-9999, i), i+1);
  }
}

void ShutterStartInit(uint32_t i, int32_t direction, int32_t target_pos)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: dir %d, delta1 %d, delta2 %d"),direction, (Shutter[i].open_max - Shutter[i].real_position) / Shutter[i].close_velocity, Shutter[i].real_position / Shutter[i].close_velocity);
  if ( ( ( (1 == direction) && ((Shutter[i].open_max - Shutter[i].real_position)  <= Shutter[i].min_realPositionChange))
      || ( (-1 == direction) &&  (Shutter[i].real_position <= Shutter[i].min_realPositionChange)) )
     && abs(Shutter[i].tilt_real_pos-Shutter[i].tilt_target_pos) <=  Shutter[i].min_TiltChange) {
    ShutterGlobal.skip_relay_change = 1;
  } else {
    Shutter[i].pwm_velocity = 0;
    ShutterWaitForMotorStart(i);
    switch (ShutterGlobal.position_mode) {
#ifdef SHUTTER_STEPPER
      case SHT_COUNTER:
        analogWriteFreq(PWM_MIN,Pin(GPIO_PWM1, i));
        TasmotaGlobal.pwm_value[i] = 0;
        PwmApplyGPIO(false);
        RtcSettings.pulse_counter[i] = 0;
      break;
#endif
    }

    Shutter[i].accelerator                  = ShutterGlobal.open_velocity_max / (Shutter[i].motordelay>0 ? Shutter[i].motordelay : 1);
    Shutter[i].target_position              = target_pos;
    Shutter[i].start_position               = Shutter[i].real_position;
    TasmotaGlobal.rules_flag.shutter_moving = 1;
    ShutterAllowPreStartProcedure(i);
    Shutter[i].time                         = Shutter[i].last_reported_time = 0;

    ShutterGlobal.skip_relay_change         = 0;
    TasmotaGlobal.rules_flag.shutter_moved  = 0;
    ShutterGlobal.start_reported            = 0;
    Shutter[i].tilt_real_pos                = tmax(tmin(Shutter[i].tilt_real_pos,Shutter[i].tilt_config[1]),Shutter[i].tilt_config[0]);
    Shutter[i].tilt_start_pos               = Shutter[i].tilt_real_pos;
    if (Shutter[i].tilt_config[1] - Shutter[i].tilt_config[0] != 0) {
      Shutter[i].venetian_delay = SHT_DIV_ROUND((direction > 0 ? Shutter[i].tilt_config[1]-Shutter[i].tilt_real_pos : Shutter[i].tilt_real_pos-Shutter[i].tilt_config[0]) * Shutter[i].tilt_config[2], Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0]);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: real %d, start %d, counter %d,freq_max %d, dir %d, freq %d"),Shutter[i].real_position, Shutter[i].start_position ,RtcSettings.pulse_counter[i],ShutterGlobal.open_velocity_max , direction ,ShutterGlobal.open_velocity_max );
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: VenetianDelay: %d, Pos: %d, Dir: %d, Delta: %d, Dur: %d, StartP: %d, TgtP: %d"),
        Shutter[i].venetian_delay, Shutter[i].tilt_real_pos,direction,(Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0]), Shutter[i].tilt_config[2],Shutter[i].tilt_start_pos,Shutter[i].tilt_target_pos);
    }

    // avoid file system writes during move to minimize missing steps. 15min diabled. Will re renabled on full stop
    TasmotaGlobal.save_data_counter = 900;

  }
  //AddLog(LOG_LEVEL_DEBUG,  PSTR("SHT: Start shtr%d from %d to %d in dir: %d"), i, Shutter[i].start_position, Shutter[i].target_position, direction);

  Shutter[i].direction = direction; // Last action. This causes RTC to start.
}

bool ShutterState(uint32_t device)
{
  if (device > 4) { return false; }
  device--;
  device &= 3;
  return (Settings->flag3.shutter_mode &&  // SetOption80 - Enable shutter support
          (ShutterGlobal.RelayShutterMask & (1 << (ShutterSettings.shutter_startrelay[device]-1))) );
}

bool ShutterStatus(void) {
  if (Settings->flag3.shutter_mode) {  // SetOption80  - (Shutter) Enable shutter support (1)
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS13_SHUTTER "\":{"));
    for (uint32_t i = 0; i < MAX_SHUTTERS_ESP32; i++) {
      if (0 == ShutterSettings.shutter_startrelay[i]) { break; }
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"" D_STATUS13_SHUTTER "%d\":{\"Relay1\":%d,\"Relay2\":%d,\"Open\":%d,\"Close\":%d,"
                                  "\"50perc\":%d,\"Delay\":%d,\"Opt\":\"%s\","
                                  "\"Calib\":[%d,%d,%d,%d,%d],"
                                  "\"Mode\":\"%d\","
                                  "\"TiltConfig\":[%d,%d,%d,%d,%d]}"),
                                  i, ShutterSettings.shutter_startrelay[i], ShutterSettings.shutter_startrelay[i] +1, ShutterSettings.shutter_opentime[i], ShutterSettings.shutter_closetime[i],
                                  ShutterSettings.shutter_set50percent[i], ShutterSettings.shutter_motordelay[i], GetBinary8(ShutterSettings.shutter_options[i], 4).c_str(),
                                  ShutterSettings.shuttercoeff[0][i], ShutterSettings.shuttercoeff[1][i], ShutterSettings.shuttercoeff[2][i], ShutterSettings.shuttercoeff[3][i], ShutterSettings.shuttercoeff[4][i],
                                  ShutterSettings.shutter_mode, 
                                  ShutterSettings.shutter_tilt_config[0][i], ShutterSettings.shutter_tilt_config[1][i], ShutterSettings.shutter_tilt_config[2][i], ShutterSettings.shutter_tilt_config[3][i], ShutterSettings.shutter_tilt_config[4][i]
                                  );
    }
    ResponseJsonEndEnd();
    return true;
  }
  return false;
}

void ShutterToggle(bool dir)
{
  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Toggle: %d, i %d, dir %d, lastdir %d"), XdrvMailbox.payload, XdrvMailbox.index, dir, Shutter[XdrvMailbox.index-1].lastdirection);
  if ((1 == XdrvMailbox.index) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (dir) {
      XdrvMailbox.payload = (Shutter[index].direction==0 ? ((Shutter[index].lastdirection > 0) ? 0 : 100) : (Shutter[index].direction > 0) ? 0 : 100);
    }
    else {
      XdrvMailbox.payload = (50 < ShutterRealToPercentPosition(Shutter[index].real_position, index)) ? 0 : 100;
    }
    XdrvMailbox.data_len      = 0;
    TasmotaGlobal.last_source = SRC_WEBGUI;
    CmndShutterPosition();
  }
}

void ShutterUpdatePosition(void)
{
  char scommand[CMDSZ];
  char stopic[TOPSZ];
  
  // Iterate through all available shutters
  for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
    // Check if the shutter is in motion
    if (Shutter[i].direction != 0) {
      // Report the position of the shutter if not already done
      if (!ShutterGlobal.start_reported) {
        ShutterReportPosition(true, i);
        ShutterGlobal.start_reported = 1;
      }
      // Update time information
      int32_t deltatime = Shutter[i].time - Shutter[i].last_reported_time;
      Shutter[i].last_reported_time = Shutter[i].time + 1;
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d Time %d(%d), cStop %d, cVelo %d, mVelo %d, aVelo %d, mRun %d, aPos %d, aPos2 %d, nStop %d, Trgt %d, mVelo %d, Dir %d, Tilt %d, TrgtTilt: %d, Tiltmove: %d"),
        i+1, Shutter[i].time, deltatime, current_stop_way, current_pwm_velocity, velocity_max, Shutter[i].accelerator, min_runtime_ms, current_real_position,Shutter[i].real_position,
        next_possible_stop_position, Shutter[i].target_position, velocity_change_per_step_max, Shutter[i].direction,Shutter[i].tilt_real_pos, Shutter[i].tilt_target_pos,
         Shutter[i].tiltmoving);

      // Check calibration mode and energy information
      // only execute every second step to remove some stress from the current sensor.
      if (ShutterGlobal.callibration_run && Shutter[i].time%2 == 0) {
        // update energy consumption on every loop to dectect stop of the shutter
        XnrgCall(FUNC_ENERGY_EVERY_SECOND);
        // fency calculation with direction gives index 0 and 1 of the energy meter
        // stop if endpoint is reached
        if (Energy->active_power[0] + Energy->active_power[1] < 1.0 && Shutter[i].time > 100){
          ShutterGlobal.stopp_armed++;
          if (ShutterGlobal.stopp_armed == 1) {
            ShutterGlobal.cycle_time = Shutter[i].time;
          } 
          AddLog(LOG_LEVEL_INFO, PSTR("SHT: %d stopp_armed:%d"),Shutter[i].time, ShutterGlobal.stopp_armed);
          if (ShutterGlobal.stopp_armed > 5) {
            Shutter[i].target_position = Shutter[i].real_position;
          }
        } else {
          //AddLog(LOG_LEVEL_INFO, PSTR("SHT: %d stopp_armed:%d, power:%.3f"),Shutter[i].time, ShutterGlobal.stopp_armed,Energy->active_power[0] + Energy->active_power[1]);
          ShutterGlobal.stopp_armed = 0;
        }
      }
      // Check if shutter reached its target position or if the speed falls below the minimum value
      if ( ((Shutter[i].real_position * Shutter[i].direction >= Shutter[i].target_position * Shutter[i].direction &&  Shutter[i].tiltmoving==0) ||
           ((int16_t)Shutter[i].tilt_real_pos * Shutter[i].direction * Shutter[i].tilt_config[2] >= (int16_t)Shutter[i].tilt_target_pos * Shutter[i].direction * Shutter[i].tilt_config[2] && Shutter[i].tiltmoving==1))
           || (ShutterGlobal.position_mode == SHT_COUNTER && Shutter[i].accelerator < 0 && Shutter[i].pwm_velocity + Shutter[i].accelerator<PWM_MIN)) {
        // Save the last direction if the shutter is in motion
        if (Shutter[i].direction != 0) {
          Shutter[i].lastdirection = Shutter[i].direction;
        }
        ShutterPowerOff(i);
        ShutterLimitRealAndTargetPositions(i);

        // Update the shutter position setting to the current real position as a percentage
        ShutterSettings.shutter_position[i] = ShutterRealToPercentPosition(Shutter[i].real_position, i);
        
        // Update the shutter start position to the current real position
        Shutter[i].start_position = Shutter[i].real_position;

        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Pre: Tilt not match %d -> %d, moving: %d"),Shutter[i].tilt_real_pos,Shutter[i].tilt_target_pos,Shutter[i].tiltmoving);
        // Check if the tilt position doesn't match the target tilt position and the shutter is not currently tilting
        if (abs(Shutter[i].tilt_real_pos - Shutter[i].tilt_target_pos) > Shutter[i].min_TiltChange && Shutter[i].tiltmoving == 0) {
          AddLog(LOG_LEVEL_INFO, PSTR("SHT: Tilt not match %d -> %d"),Shutter[i].tilt_real_pos,Shutter[i].tilt_target_pos);
          // Prepare the command to update the shutter position
          char databuf[1]       = "";
          XdrvMailbox.data      = databuf;
          XdrvMailbox.payload   = -99;
          XdrvMailbox.index     = i + 1;
          // Set the shutter to tilting mode
          Shutter[i].tiltmoving = 1;
          CmndShutterPosition();
          return;
        } else {
          // Update the shutter tilt position setting to the current real tilt position
          ShutterSettings.shutter_tilt_pos[i] = Shutter[i].tilt_real_pos;
        }
        ShutterLogPos(i);

        if (!Settings->flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
          // sending MQTT result to broker
          snprintf_P(scommand, sizeof(scommand),PSTR(D_SHUTTER "%d"), i + 1);
          GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, scommand);
          Response_P("%d", ShutterSettings.shutter_position[i]);
          MqttPublish(stopic, Settings->flag.mqtt_power_retain);  // CMND_POWERRETAIN
        }

        // Report the shutter position
        ShutterReportPosition(true, i);
        TasmotaGlobal.rules_flag.shutter_moved = 1;
      }
    }
  }
}

void ShutterUpdateVelocity(uint8_t i)
{
  // No Logging allowed. Part of RTC Timer
  // will be calles through RTC every 50ms.
  // do not allow accellerator to stop movement
  Shutter[i].pwm_velocity = tmax(velocity_change_per_step_max, Shutter[i].pwm_velocity+Shutter[i].accelerator);
  Shutter[i].pwm_velocity = tmin(Shutter[i].direction==1 ? ShutterGlobal.open_velocity_max : Shutter[i].close_velocity_max,Shutter[i].pwm_velocity);
  // respect hard coded SDK limit of PWM_MIN on PWM frequency.
  if (ShutterGlobal.position_mode == SHT_COUNTER) {
  	Shutter[i].pwm_velocity = tmax(PWM_MIN,Shutter[i].pwm_velocity);
  }
}

void ShutterWaitForMotorStart(uint8_t i)
{
  while (millis() < Shutter[i].last_stop_time + ShutterSettings.shutter_motorstop) {
    loop();
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Stoptime done"));
}

void ShutterWaitForMotorStop(uint8_t i)
{
  Shutter[i].last_stop_time = millis();
  ShutterWaitForMotorStart(i);
}


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndShutterButton(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SHUTTERS_ESP32)) {
    tButtonSettings setting;
    memset(&setting, 0x00, sizeof(setting));
    // (setting>>31)&(0x01) : enabled
    // (setting>>30)&(0x01) : mqtt broadcast to all index
    // (setting>>29)&(0x01) : mqtt broadcast hold
    // (setting>>28)&(0x01) : mqtt broadcast tripple press
    // (setting>>27)&(0x01) : mqtt broadcast double press
    // (setting>>26)&(0x01) : mqtt broadcast single press
    // (setting>>20)&(0x3f) : shutter_position hold; 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>>14)&(0x3f) : shutter_position tripple press 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>> 8)&(0x3f) : shutter_position double press 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>> 2)&(0x3f) : shutter_position single press 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>> 0)&(0x03) : shutter_index
    if (XdrvMailbox.data_len > 0) {
        uint32_t i            = 0; // tokencount on " "
        uint32_t button_index = 0;
        bool done             = false;
        bool isShortCommand   = false;
        char *str_ptr;
        char *str_ptr2;

        char data_copy[strlen(XdrvMailbox.data) +1];
        strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
        // Loop through the data string, splitting on ' ' seperators.
        for (char *str = strtok_r(data_copy, " ", &str_ptr); str && i < (1+4+4+1); str = strtok_r(nullptr, " ", &str_ptr), i++) {
          int field         = -1;
          int tilt          = -128;
          int pos_incrdecr  = 0;
          int tilt_incrdecr = 0;
          int j = 0;  // tokencount on "/"
          char field_copy[strlen(str) + 1];
          strncpy(field_copy, str, sizeof(field_copy));  // Duplicate data as strtok_r will modify it.
          // Loop through the data string, splitting on '/' seperators. Syntax position[0..100]/tilt[-90..90]
          for (char *str2 = strtok_r(field_copy, "/", &str_ptr2); str2 && j < 2; str2 = strtok_r(nullptr, "/", &str_ptr2), j++) {
            switch (j) {
              case 0:
                switch (str2[0]) {
                  case 't': // toggle the tilt. currently not implemented
                    field = 102;
                    break;
                  case '-':
                  case '+':
                    field = -1;
                    // check for trigger -- or ++ for incremental change
                    if (str2[1] == '-' || str2[1] == '+') {
                      str2[0] = ' ';
                      pos_incrdecr = atoi(str2);
                    }
                    break;
                  default:
                  field = atoi(str2);
                  break;
                }
                if (i>0)
                  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: FINAL: pos:%d, inc:%d, string:%s"), field, pos_incrdecr, str2);
                break;
              case 1:
                switch (str2[0]) {
                  case 't':
                    tilt = 127;
                    break;
                  case '-': // special handling to seperate a - from a negative number. e.g. -90
                  case '+':
                    if (strlen(str2)==1 && str2[0] == '-') {
                      tilt = -128;
                    } else {
                      if (str2[1] == '-' || str2[1] == '+') {
                        str2[0] = ' ';
                        tilt = 127;
                        tilt_incrdecr = atoi(str2);
                      } else {
                        tilt = atoi(str2);
                      }
                    }
                    break;
                  default:
                  tilt = atoi(str2);
                  break;
                }
                AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: FINAL: tilt:%d, inc:%d, string:%s"), tilt, tilt_incrdecr, str2);
                break;
            }
          } //  end scanning "++10/-90" tokens
          switch (i) {
            case 0:
              if ((field >= -1) && (field<=MAX_SHUTTERS_ESP32*2)) {
                button_index = (field <= 0)?(-1):field;
                done = (button_index == -1);
              } else
                done = true;
            break;
            case 1:
              if (!strcmp_P(str, PSTR("up"))) {
                setting.position[0].pos = 100;
                setting.position[1].pos = 50;
                setting.position[2].pos = 75;
                setting.position[3].pos = 100;
                isShortCommand = true;
                break;
              } else if (!strcmp_P(str, PSTR("down"))) {
                setting.position[0].pos = 0;
                setting.position[1].pos = 50;
                setting.position[2].pos = 25;
                setting.position[3].pos = 0;
                isShortCommand = true;
                break;
              } else if (!strcmp_P(str, PSTR("updown"))) {
                setting.position[0].pos = 100;
                setting.position[1].pos = 0;
                setting.position[2].pos = 50;          
                isShortCommand = true;
                break;
              } else if (!strcmp_P(str, PSTR("toggle"))) {
                setting.position[0].pos = 102;
                setting.position[1].pos = 50;
                isShortCommand = true;
                break;
              }
            case 2:
            /* Currently not supported
              if (isShortCommand) {
                if ((field==1) && (setting & (0x3F<<(2+6*3))))
                  // if short command up or down (hold press position set) then also enable MQTT broadcast
                  setting |= (0x3<<29);
                done = true;
                break;
              }
            */
            case 3:
            case 4:
              AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: CASE4: pos:%d, tilt:%d, incpos:%d, inctilt:%d"), 
                 field, tilt, pos_incrdecr, tilt_incrdecr);

              if ((field >= -1) && (field <= 102))
                setting.position[i-1].pos = field;
              if ((tilt >= -128) && (tilt <= 127))
                setting.position[i-1].tilt = tilt;  
              if ((pos_incrdecr >= -100) && (pos_incrdecr <= 100))
                setting.position[i-1].pos_incrdecr = pos_incrdecr; 
              if ((tilt_incrdecr >= -90) && (tilt_incrdecr <= 90))
                setting.position[i-1].tilt_incrdecr = tilt_incrdecr;              
            break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
              if (field==1)
                setting.position[i-6].mqtt_broadcast = true;
            break;
          }
          if (isShortCommand) {
            for (uint8_t j = 0; j < 4; j++) setting.position[j].tilt = -128;
          }
          if (done) break;
        }

        if (button_index) {
          if (button_index == -1) {
            // remove all buttons for this shutter
            for (uint32_t i = 0 ; i < MAX_SHUTTERS_ESP32*2 ; i++)
              if (ShutterSettings.shutter_button[i].shutter_number == XdrvMailbox.index - 1)
                ShutterSettings.shutter_button[i].enabled        = false;
                ShutterSettings.shutter_button[i].shutter_number = 0;
                for (uint8_t j = 0; j < 4; j++)
                  ShutterSettings.shutter_button[i].position[j] = {-1, -128, 0};
          } else {
            setting.enabled = true;
            setting.shutter_number = XdrvMailbox.index - 1;
            ShutterSettings.shutter_button[button_index-1] = setting;
            //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: ENABLE SHT:%d -> %d"),XdrvMailbox.index-1,ShutterSettings.shutter_button[button_index-1]);
          }
        }
      }
      char setting_chr[30*MAX_SHUTTER_KEYS] = "-", *setting_chr_ptr = setting_chr;
      for (uint32_t i = 0 ; i < MAX_SHUTTERS_ESP32*2 ; i++) {
        setting = ShutterSettings.shutter_button[i];
        //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Setting: SHT on BTN:%d, index:%d"),ShutterSettings.shutter_button[i].shutter_number,XdrvMailbox.index-1);
        if ((setting.enabled) && (ShutterSettings.shutter_button[i].shutter_number == XdrvMailbox.index - 1)) {
          if (*setting_chr_ptr == 0)
            setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR("|"));
          setting_chr_ptr += snprintf_P(setting_chr_ptr, 3, PSTR("%d"), i + 1);

          for (uint32_t j = 0 ; j < 4 ; j++) {
            int8_t pos           = setting.position[j].pos;
            int8_t postilt       = setting.position[j].tilt;
            int8_t pos_incrdecr  = setting.position[j].pos_incrdecr;
            int8_t tilt_incrdecr = setting.position[j].tilt_incrdecr;
            AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Merging: pos:%d, tilt:%d, incpos:%d, inctilt:%d"), 
                 pos, postilt, pos_incrdecr, tilt_incrdecr);
            if (0 <= pos)
              if (102 == pos) {
                setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" t"));
              } else {
                setting_chr_ptr += snprintf_P(setting_chr_ptr, 5, PSTR(" %d"), pos);
              }
            else {
              switch (pos_incrdecr) {
                  case 0:
                    setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" -"));
                  break;
                  default:
                    if (pos_incrdecr > 0)
                      setting_chr_ptr += snprintf_P(setting_chr_ptr, 6, PSTR(" ++%d"), pos_incrdecr);
                    else
                      setting_chr_ptr += snprintf_P(setting_chr_ptr, 6, PSTR(" -%d"), pos_incrdecr);
                  break;
              }
            }
              
            if (-128 != postilt || tilt_incrdecr !=0) {
              setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR("/"));
              if (127 == postilt) {
                switch (tilt_incrdecr) {
                  case 0:
                    setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR("t"));
                  break;
                  default:
                    if (tilt_incrdecr > 0)
                      setting_chr_ptr += snprintf_P(setting_chr_ptr, 5, PSTR("++%d"), tilt_incrdecr);
                    else
                      setting_chr_ptr += snprintf_P(setting_chr_ptr, 5, PSTR("-%d"), tilt_incrdecr);
                  break;
                }
              } else {
                setting_chr_ptr += snprintf_P(setting_chr_ptr, 5, PSTR("%d"), postilt);
              }
            }
          }
          for (uint32_t j = 0 ; j < 4 ; j++) {
            if (setting.position[j].mqtt_broadcast)
              setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" 1"));
            else
              setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" -"));
          }
        }
      }
      ShutterSettingsSave();
      ResponseCmndIdxChar(setting_chr);
   }
}

void CmndShutterCalibration(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t i = 0;
      char *str_ptr;

      char data_copy[strlen(XdrvMailbox.data) + 1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ", &str_ptr); str && i < 5; str = strtok_r(nullptr, " ", &str_ptr), i++) {
        int field = atoi(str);
        // The fields in a data string can only range from 1-30000.
        // and following value must be higher than previous one
        if ((field <= 0) || (field > 30000) || ( (i>0) && (field <= messwerte[i-1]) ) ) {
          break;
        }
        messwerte[i] = field;
      }
      ShutterSettings.shutter_set50percent[XdrvMailbox.index - 1] = 50;
      for (i = 0; i < 5; i++) {
        ShutterSettings.shuttercoeff[i][XdrvMailbox.index - 1] = SHT_DIV_ROUND((uint32_t)messwerte[i] * 1000, messwerte[4]);
        AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shuttercoeff %d, i %d, Value %d, MeasuredValue %d"), i, XdrvMailbox.index - 1, ShutterSettings.shuttercoeff[i][XdrvMailbox.index - 1], messwerte[i]);
      }
      ShutterInit();
      ResponseCmndIdxChar(XdrvMailbox.data);
    } else {
      char setting_chr[30] = "0";
      snprintf_P(setting_chr, sizeof(setting_chr), PSTR("%d %d %d %d %d"), ShutterSettings.shuttercoeff[0][XdrvMailbox.index -1], ShutterSettings.shuttercoeff[1][XdrvMailbox.index - 1], ShutterSettings.shuttercoeff[2][XdrvMailbox.index - 1], ShutterSettings.shuttercoeff[3][XdrvMailbox.index - 1], ShutterSettings.shuttercoeff[4][XdrvMailbox.index - 1]);
      ResponseCmndIdxChar(setting_chr);
    }
  }
}

void CmndShutterClose(void)
{
  //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Payload close: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.index);
  if ((!XdrvMailbox.usridx) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload       = 0;
  TasmotaGlobal.last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterCloseTime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      ShutterSettings.shutter_closetime[XdrvMailbox.index - 1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
    ResponseCmndIdxFloat((float)(ShutterSettings.shutter_closetime[XdrvMailbox.index -1]) / 10, 1);
  }
}

void CmndShutterEnableEndStopTime(void)
{
  ShutterOptionsSetHelper(4);
}

void CmndShutterFrequency(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 20000)) {
    ShutterGlobal.open_velocity_max =  XdrvMailbox.payload;
    ShutterSettings.open_velocity_max = ShutterGlobal.open_velocity_max;
    ShutterInit();
  }
  ResponseCmndNumber(ShutterGlobal.open_velocity_max);
}

void CmndShutterIncDec(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Change in: payload %s (%d), payload %d, idx %d, src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, TasmotaGlobal.last_source );
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      XdrvMailbox.payload =  ShutterRealToPercentPosition(Shutter[XdrvMailbox.index - 1].target_position, XdrvMailbox.index - 1) + XdrvMailbox.payload;
      // limit position to boundaries
      XdrvMailbox.payload = XdrvMailbox.payload < 0 ? 0 : (XdrvMailbox.payload > 100 ? 100 : XdrvMailbox.payload);
      CmndShutterPosition();
    }
  }
}

void CmndShutterInvert(void)
{
  ShutterOptionsSetHelper(1);
}

void CmndShutterInvertWebButtons(void)
{
  ShutterOptionsSetHelper(8);
}

void CmndShutterLock(void)
{
  ShutterOptionsSetHelper(2);
}

void CmndShutterMode(void)
{
  if (!XdrvMailbox.usridx) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_MODES)) {
      ShutterSettings.shutter_mode =  ShutterGlobal.position_mode =  XdrvMailbox.payload;
      ShutterInit();
    }
    ResponseCmndNumber(ShutterGlobal.position_mode);
  }
}

void CmndShutterMotorDelay(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      ShutterSettings.shutter_motordelay[XdrvMailbox.index - 1] = (uint8_t)(STEPS_PER_SECOND * CharToFloat(XdrvMailbox.data));
      ShutterInit();
      //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr Init1. realdelay %d"),Shutter[XdrvMailbox.index -1].motordelay);
    }
    ResponseCmndIdxFloat((float)(Shutter[XdrvMailbox.index - 1].motordelay) / STEPS_PER_SECOND, 2);
  }
}

void CmndShutterMotorStop(void)
{
  if (!XdrvMailbox.usridx) {
    if ((XdrvMailbox.payload >= 0) ) {
      ShutterSettings.shutter_motorstop = XdrvMailbox.payload;
      ShutterInit();
    }
    ResponseCmndNumber(ShutterSettings.shutter_motorstop);
  }
}

void CmndShutterOpen(void)
{
  //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Payload open: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.index);
  if ((!XdrvMailbox.usridx) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload       = 100;
  TasmotaGlobal.last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterOpenTime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      ShutterSettings.shutter_opentime[XdrvMailbox.index - 1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
    ResponseCmndIdxFloat((float)(ShutterSettings.shutter_opentime[XdrvMailbox.index - 1]) / 10, 1);
  }
}

void CmndShutterPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (!(ShutterSettings.shutter_options[XdrvMailbox.index - 1] & 2)) {
      uint32_t index = XdrvMailbox.index - 1;
      //limit the payload
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Pos. payload <%s> (%d), payload %d, idx %d (%d), src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, XdrvMailbox.usridx, TasmotaGlobal.last_source );

      if (XdrvMailbox.data_len >= 3) {
        // check if input is of format "position,tilt"
        uint32_t i = 0;
        char *str_ptr;
        char data_copy[strlen(XdrvMailbox.data) + 1];
        strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
        // Loop through the data string, splitting on ',' seperators.
        for (char *str = strtok_r(data_copy, ",", &str_ptr); str && i < 2; str = strtok_r(nullptr, ",", &str_ptr), i++) {
          switch(i) {
            case 0:
              XdrvMailbox.payload = atoi(str);
              break;
            case 1:
              Shutter[index].tilt_target_pos_override = atoi(str);
              break;
          }
        }
      }


      // value 0 with data_len > 0 can mean Open
      // special handling fo UP,DOWN,TOGGLE,STOP command comming with payload -99
      // STOP will come with payload 0 because predefined value in TASMOTA
      if ((XdrvMailbox.data_len > 3) && (XdrvMailbox.payload <= 0)) {
        // set len to 0 to avoid loop on close where payload is 0
        XdrvMailbox.data_len = 0;
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_UP) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_OPEN) || ((Shutter[index].direction==0) && !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPOPEN))) {
          CmndShutterOpen();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_DOWN) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_CLOSE) || ((Shutter[index].direction==0) && !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPCLOSE))) {
          CmndShutterClose();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_TOGGLE)) {
          CmndShutterToggle();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_TOGGLEDIR)) {
          CmndShutterToggleDir();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOP) || ((Shutter[index].direction) && (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPOPEN) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPCLOSE)))) {
          // Back to normal: all -99 if not a clear position
          XdrvMailbox.payload = -99;
          CmndShutterStop();
          return;
        }

      }

      //override tiltposition if explicit set (shutterbutton)
      if (Shutter[index].tilt_target_pos_override != -128) {
        Shutter[index].tilt_target_pos = tmin(tmax( Shutter[index].tilt_config[0],Shutter[index].tilt_target_pos_override ), Shutter[index].tilt_config[1]);
        Shutter[index].tilt_target_pos_override = -128;
      }

      int8_t target_pos_percent = (XdrvMailbox.payload < 0) ? (XdrvMailbox.payload == -99 ? ShutterRealToPercentPosition(Shutter[index].real_position, index) : 0) : ((XdrvMailbox.payload > 100) ? 100 : XdrvMailbox.payload);
      target_pos_percent = ((ShutterSettings.shutter_options[index] & 1) && ((SRC_MQTT       != TasmotaGlobal.last_source) // 1
                                                                          && (SRC_SERIAL     != TasmotaGlobal.last_source) // 6
                                                                          && (SRC_WEBGUI     != TasmotaGlobal.last_source) // 7
                                                                          && (SRC_WEBCOMMAND != TasmotaGlobal.last_source) // 8
                                                                             )) ? 100 - target_pos_percent : target_pos_percent;

      // if position is either 0 or 100 reset the tilt to avoid tilt moving at the end
      if (target_pos_percent ==   0 && ShutterRealToPercentPosition(Shutter[index].real_position, index)  > 0  ) {Shutter[index].tilt_target_pos = Shutter[index].tilt_config[4];}
      if (target_pos_percent == 100 && ShutterRealToPercentPosition(Shutter[index].real_position, index)  < 100) {Shutter[index].tilt_target_pos = Shutter[index].tilt_config[3];}

      if (XdrvMailbox.payload != -99) {
        Shutter[index].target_position = ShutterPercentToRealPosition(target_pos_percent, index);
        //Shutter[i].accelerator[index] = ShutterGlobal.open_velocity_max / ((Shutter[i].motordelay[index] > 0) ? Shutter[i].motordelay[index] : 1);
        //Shutter[i].target_position[index] = XdrvMailbox.payload < 5 ?  ShutterSettings.shuttercoeff[2][index] * XdrvMailbox.payload : ShutterSettings.shuttercoeff[1][index] * XdrvMailbox.payload + ShutterSettings.shuttercoeff[0,index];
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: lastsource %d:, real %d, target %d, tiltreal: %d, tilttarget: %d, payload %d"), TasmotaGlobal.last_source, Shutter[index].real_position ,Shutter[index].target_position,Shutter[index].tilt_real_pos, Shutter[index].tilt_target_pos,target_pos_percent);
      }
      if ( (target_pos_percent >= 0) && (target_pos_percent <= 100) &&
           (abs(Shutter[index].target_position - Shutter[index].real_position ) > Shutter[index].min_realPositionChange ||
            abs(Shutter[index].tilt_target_pos - Shutter[index].tilt_real_pos ) > Shutter[index].min_TiltChange) ) {
        if (ShutterSettings.shutter_options[index] & 4) {
          if (0   == target_pos_percent && Shutter[index].real_position > 0) Shutter[index].target_position -= 1 * RESOLUTION * STEPS_PER_SECOND;
          if (100 == target_pos_percent && Shutter[index].real_position < Shutter[index].open_max) Shutter[index].target_position += 1 * RESOLUTION * STEPS_PER_SECOND;
        }
        int8_t new_shutterdirection;
        if (abs(Shutter[index].target_position - Shutter[index].real_position ) > Shutter[index].min_realPositionChange) {
          new_shutterdirection = Shutter[index].real_position < Shutter[index].target_position ? 1 : -1;
          Shutter[index].tiltmoving = 0;
        } else {
          new_shutterdirection = Shutter[index].tilt_real_pos < Shutter[index].tilt_target_pos ? 1 : -1;
          Shutter[index].tiltmoving = 1;
        }

        if (Shutter[index].direction == -new_shutterdirection) {
          //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Stop shutter to reverse direction"));
          ShutterPowerOff(index);
        }
        if (Shutter[index].direction != new_shutterdirection) {
          ShutterStartInit(index, new_shutterdirection, Shutter[index].target_position);
          uint8_t save_direction = Shutter[index].direction;
          Shutter[index].direction = 0; // set temporary direction = 0 to avoid RTC timer sarting. Some delay may happen before shutter starts moving
          switch (ShutterGlobal.position_mode) {
            case SHT_COUNTER:
            case SHT_PWM_TIME:
            case SHT_PWM_VALUE:
            case SHT_TIME_UP_DOWN:
              if (!ShutterGlobal.skip_relay_change) {
                // Code for shutters with circuit safe configuration, switch the direction Relay
                ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index] +1, new_shutterdirection == 1 ? 0 : 1, SRC_SHUTTER);
                delay(SHUTTER_RELAY_OPERATION_TIME);
                // power on
                ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index], 1, SRC_SHUTTER);
              }
              //if (ShutterGlobal.position_mode != SHT_TIME_UP_DOWN) ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index]+2, 1, SRC_SHUTTER);
            break;
            case SHT_TIME:
              if (!ShutterGlobal.skip_relay_change) {
                if ( (TasmotaGlobal.power >> (ShutterSettings.shutter_startrelay[index] - 1)) & 3 > 0 ) {
                  ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 1 : 0), Shutter[index].switch_mode == SHT_SWITCH ? 0 : 1, SRC_SHUTTER);
                }
                ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
              }
            break;
            case SHT_TIME_GARAGE:
              //AddLog(LOG_LEVEL_INFO, PSTR("SHT: Garage Skip Relay: %d"), ShutterGlobal.skip_relay_change);
              if (!ShutterGlobal.skip_relay_change) {
                if (new_shutterdirection == Shutter[index].lastdirection) {
                  AddLog(LOG_LEVEL_INFO, PSTR("SHT: Garage not move in this direction: %d"), Shutter[index].switch_mode == SHT_PULSE);
                  for (uint8_t k = 0 ; k <= (uint8_t)(Shutter[index].switch_mode == SHT_PULSE) ; k++) {
                    ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index], 1, SRC_SHUTTER);
                    ShutterWaitForMotorStop(index);
                    ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index], 0, SRC_SHUTTER);
                    ShutterWaitForMotorStop(index);
                  }
                  // reset shutter time to avoid 2 seconds above count as runtime
                  Shutter[index].time = 0;
                } // if (new_shutterdirection == Shutter[i].lastdirection[index])
                ExecuteCommandPowerShutter(ShutterSettings.shutter_startrelay[index], 1, SRC_SHUTTER);
              } // if (!ShutterGlobal.skip_relay_change)
            break;
          } // switch (ShutterGlobal.position_mode)
          Shutter[index].direction = save_direction;
          ShutterGlobal.RelayCurrentMask = 0;
        } // if (Shutter[i].direction[index] != new_shutterdirection)
      } else {
        target_pos_percent = ShutterRealToPercentPosition(Shutter[index].real_position, index);
      }
      index = (!XdrvMailbox.usridx && !XdrvMailbox.data_len)?MAX_SHUTTERS_ESP32:index;
      ShutterReportPosition(true, index);
      ShutterGlobal.start_reported = 1;
      XdrvMailbox.index = index + 1;  // Fix random index for ShutterClose
    } else {
      ShutterReportPosition(true, MAX_SHUTTERS_ESP32);
      if (XdrvMailbox.command)
        ResponseCmndIdxChar("Locked");
    }
  }
}

void CmndShutterPwmRange(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t i = 0;
      char *str_ptr;

      char data_copy[strlen(XdrvMailbox.data) +1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ", &str_ptr); str && i < 2; str = strtok_r(nullptr, " ", &str_ptr), i++) {
        uint16_t field = atoi(str);
        // The fields in a data string can only range from 1-30000.
        // and following value must be higher than previous one
        if ((field <= 0) || (field > 1023)) {
          break;
        }
        ShutterSettings.shutter_pwmrange[i][XdrvMailbox.index - 1] = field;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Init1. pwmmin %d, pwmmax %d"), XdrvMailbox.index , ShutterSettings.shutter_pwmrange[0][XdrvMailbox.index -1], ShutterSettings.shutter_pwmrange[1][XdrvMailbox.index -1]);
      ShutterInit();
      ResponseCmndIdxChar(XdrvMailbox.data);
    } else {
      char setting_chr[30] = "0";
      snprintf_P(setting_chr, sizeof(setting_chr), PSTR("Shutter %d: min:%d max:%d"), XdrvMailbox.index, ShutterSettings.shutter_pwmrange[0][XdrvMailbox.index -1], ShutterSettings.shutter_pwmrange[1][XdrvMailbox.index -1]);
      ResponseCmndIdxChar(setting_chr);
    }
  }
}

void CmndShutterRelay(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 32) && (XdrvMailbox.index <= MAX_SHUTTERS_ESP32)) {
    //ShutterSettings.shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;
    if (XdrvMailbox.payload > 0) {
      ShutterGlobal.RelayShutterMask |= 3 << (XdrvMailbox.payload - 1);
    } else {
      ShutterGlobal.RelayShutterMask ^= 3 << (ShutterSettings.shutter_startrelay[XdrvMailbox.index -1] - 1);
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: relold:%d index:%d, mode:%d, relaymask: %ld"),
    ShutterSettings.shutter_startrelay[XdrvMailbox.index -1] , XdrvMailbox.index ,ShutterSettings.shutter_mode, ShutterGlobal.RelayShutterMask );
    if (ShutterSettings.shutter_startrelay[XdrvMailbox.index -1] == 0 && XdrvMailbox.index == 1 && ShutterSettings.shutter_mode == SHT_UNDEF) {
      // first shutter was not defined, maybe init
      ShutterSettings.shutter_mode = SHT_AUTOCONFIG;
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Autoconfig"));
    }
    ShutterSettings.shutter_startrelay[XdrvMailbox.index - 1] = XdrvMailbox.payload;

    ShutterInit();
    // if payload is 0 to disable the relay there must be a reboot. Otherwhise does not work
  }
  uint32_t start = (!XdrvMailbox.usridx && !XdrvMailbox.data_len)?0:XdrvMailbox.index - 1;
  uint32_t end   = (!XdrvMailbox.usridx && !XdrvMailbox.data_len)?TasmotaGlobal.shutters_present:XdrvMailbox.index;
  // {"ShutterRelay1":"1","ShutterRelay2":"3","ShutterRelay3":"5"}
  Response_P(PSTR("{"));
  for (uint32_t i = start; i < end; i++) {
    ResponseAppend_P(PSTR("%s\"" D_PRFX_SHUTTER D_CMND_SHUTTER_RELAY "%d\":%d"), (i>start)?",":"", i+1, ShutterSettings.shutter_startrelay[i]);
  }
  ResponseAppend_P(PSTR("}"));
}

void CmndShutterSetClose(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    Shutter[XdrvMailbox.index - 1].real_position            = 0;
    Shutter[XdrvMailbox.index - 1].tilt_real_pos            = Shutter[XdrvMailbox.index - 1].tilt_config[0];
    Shutter[XdrvMailbox.index - 1].lastdirection            = -1;
    ShutterStartInit(XdrvMailbox.index -1, 0, 0);
    ShutterSettings.shutter_position[XdrvMailbox.index - 1] = 0;
    ResponseCmndIdxChar(D_CONFIGURATION_RESET);
  }
}

void CmndShutterSetHalfway(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      ShutterSettings.shutter_set50percent[XdrvMailbox.index - 1] = (ShutterSettings.shutter_options[XdrvMailbox.index -1] & 1) ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
      ShutterSettings.shuttercoeff[0][XdrvMailbox.index -1 ]      = 0;
      if (XdrvMailbox.payload == ShutterSettings.shutter_position[XdrvMailbox.index -1]){
        ShutterSettings.shutter_position[XdrvMailbox.index - 1] = 50;
      }
      // Init calculates the realposition from the %-Position
      ShutterInit();
    }
  ResponseCmndIdxNumber((ShutterSettings.shutter_options[XdrvMailbox.index - 1] & 1) ? 100 - ShutterSettings.shutter_set50percent[XdrvMailbox.index - 1] : ShutterSettings.shutter_set50percent[XdrvMailbox.index - 1]);
  }
}

void CmndShutterSetOpen(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    Shutter[XdrvMailbox.index - 1].real_position = Shutter[XdrvMailbox.index - 1].open_max;
    Shutter[XdrvMailbox.index - 1].tilt_real_pos = Shutter[XdrvMailbox.index - 1].tilt_config[1];
    Shutter[XdrvMailbox.index - 1].lastdirection = 1;
    ShutterStartInit(XdrvMailbox.index -1, 0, Shutter[XdrvMailbox.index - 1].open_max);
    ShutterSettings.shutter_position[XdrvMailbox.index - 1] = 100;
    ResponseCmndIdxChar(D_CONFIGURATION_RESET);
  }
}

void CmndShutterSetTilt(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.payload != -99 ) {
      Shutter[XdrvMailbox.index - 1].tilt_target_pos = tmin(tmax(XdrvMailbox.payload, Shutter[XdrvMailbox.index - 1].tilt_config[0]), Shutter[XdrvMailbox.index -1].tilt_config[1]);
    }
    // assuming OPEN=100=tilt_config[3]/CLOSE=0=tilt_config[4]
    if (XdrvMailbox.data_len > 3 && XdrvMailbox.payload >= 0 ) {
      Shutter[XdrvMailbox.index - 1].tilt_target_pos = Shutter[XdrvMailbox.index - 1].tilt_config[XdrvMailbox.payload ? 3 : 4];
    }
  }
  XdrvMailbox.data[0] = '\0';
  AddLog(LOG_LEVEL_INFO, PSTR("SHT: TiltTarget %d, payload %d"), Shutter[XdrvMailbox.index - 1].tilt_target_pos,XdrvMailbox.payload);
  Shutter[XdrvMailbox.index - 1].tiltmoving = 1;
  // Avoid shutterposition try to interpret "open/close or payload"
  XdrvMailbox.data_len = 0;
  XdrvMailbox.payload  = -99;
  CmndShutterPosition();
}

void CmndShutterSetup(void) {
  uint8_t  index_no;
  char     time_chr[10];
  uint32_t new_opentime;
  uint32_t new_closetime;
  uint8_t  max_runtime = 120; // max 120 seconds runtime
  float daily_kWh[ENERGY_MAX_PHASES];           // 123.123 kWh

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    index_no = XdrvMailbox.index-1; // save, because will be changed in following operations
    // init shutter to default settings
    ShutterGlobal.callibration_run              = true;
    ShutterSettings.shutter_position[index_no]  = 0;
    ShutterSettings.shutter_closetime[index_no] = max_runtime * 10;
    ShutterSettings.shutter_opentime[index_no]  = max_runtime * 10;
    for (uint8_t i = 0; i < ENERGY_MAX_PHASES; i++) {
      daily_kWh[i] = Energy->daily_kWh[i];
    }
    ShutterInit();
    if (Energy->phase_count > 1) {
      AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup: Ensure shutter is close. Now open, autostop detect. max duration is 2min Phase:%d"),Energy->phase_count);
      ShutterWaitForMotorStop(index_no);
      CmndShutterOpen();
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup: Ensure shutter is close. Now open and stop when open. max duration is 2min"));
    }

    new_opentime = ShutterGetCycleTime(index_no, max_runtime);
    if (new_opentime) {
      ShutterSettings.shutter_position[index_no] = 100;
      ShutterInit();  
      if (Energy->phase_count > 1) {
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup: Now close, autostop detect. max duration is 2min"));
        ShutterWaitForMotorStop(index_no); 
        CmndShutterClose();
      }else {
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Setup: Now close and stop when closed. max duration is 2min"));
      }

      new_closetime = ShutterGetCycleTime(index_no, max_runtime);
      ShutterSettings.shutter_position[index_no] = 0;
      if (new_closetime)  {
        ShutterSettings.shutter_opentime[index_no]     = new_opentime;
        ShutterSettings.shutter_closetime[index_no]    = new_closetime;
        //good default value for normal european shutters. Setting here because Position == 0
        ShutterSettings.shutter_set50percent[index_no] = 70;
        ShutterInit();
      } 
    } 
    ShutterGlobal.callibration_run = false;
    for (uint8_t i = 0; i < ENERGY_MAX_PHASES; i++) {
       Energy->daily_kWh[i] = daily_kWh[i];
    }
  } else {
    // print out help instructions
    // will only work without TILT configuration
  }
  return;
}

void CmndShutterStop(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Try Stop %d: dir: %d"), XdrvMailbox.index, Shutter[XdrvMailbox.index -1].direction);
    if (!(ShutterSettings.shutter_options[XdrvMailbox.index - 1] & 2)) {
      if ((!XdrvMailbox.usridx) && (XdrvMailbox.payload != -99)) {
        XdrvMailbox.index = XdrvMailbox.payload;
      }
      uint32_t i = XdrvMailbox.index - 1;
      if (Shutter[i].direction != 0) {

        AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Stop %d: dir: %d"), XdrvMailbox.index, Shutter[i].direction);
        Shutter[i].target_position = Shutter[i].real_position;
        TasmotaGlobal.last_source = SRC_SHUTTER;
      }
      if (XdrvMailbox.command)
        ResponseCmndDone();
        ShutterUpdatePosition();
    } else {
      if (XdrvMailbox.command)
        ResponseCmndIdxChar("Locked");
    }
  }
}

void CmndShutterStopClose(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (Shutter[XdrvMailbox.index - 1].direction) {
      CmndShutterStop();
    } else {
      CmndShutterClose();
    }
  }
}

void CmndShutterStopOpen(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (Shutter[XdrvMailbox.index - 1].direction) {
      CmndShutterStop();
    } else {
      CmndShutterOpen();
    }
  }
}

void CmndShutterStopPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (Shutter[XdrvMailbox.index - 1].direction) {
      XdrvMailbox.payload = -99;
      CmndShutterStop();
    } else {
      CmndShutterPosition();
    }
  }
}

void CmndShutterStopToggle(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (Shutter[XdrvMailbox.index - 1].direction) {
      CmndShutterStop();
    } else {
      CmndShutterToggle();
    }
  }
}

void CmndShutterStopToggleDir(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (Shutter[XdrvMailbox.index - 1].direction) {
      CmndShutterStop();
    } else {
      CmndShutterToggleDir();
    }
  }
}

void CmndShutterTiltConfig(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t i = 0;
      char *str_ptr;
      char data_copy[strlen(XdrvMailbox.data) +1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ", &str_ptr); str && i < 6; str = strtok_r(nullptr, " ", &str_ptr), i++) {
        Shutter[XdrvMailbox.index -1].tilt_config[i] = ShutterSettings.shutter_tilt_config[i][XdrvMailbox.index -1] = atoi(str);
      }
      // avoid negative runtime
      ShutterSettings.shutter_tilt_config[2][XdrvMailbox.index - 1] = Shutter[XdrvMailbox.index - 1].tilt_config[2] = Shutter[XdrvMailbox.index - 1].tilt_config[2] >= 0 ? Shutter[XdrvMailbox.index - 1].tilt_config[2] : 127;
      ShutterInit();
    }
    char setting_chr[30] = "0";
    snprintf_P(setting_chr, sizeof(setting_chr), PSTR("%d %d %d %d %d"), Shutter[XdrvMailbox.index - 1].tilt_config[0], Shutter[XdrvMailbox.index - 1].tilt_config[1],Shutter[XdrvMailbox.index -  1].tilt_config[2],Shutter[XdrvMailbox.index - 1].tilt_config[3],Shutter[XdrvMailbox.index - 1].tilt_config[4]);
    ResponseCmndIdxChar(setting_chr);
    AddLog(LOG_LEVEL_INFO, PSTR("SHT: TiltConfig %d, min: %d, max %d, runtime %d, close_pos: %d, open_pos: %d"), 
      XdrvMailbox.index ,
      Shutter[XdrvMailbox.index - 1].tilt_config[0], 
      Shutter[XdrvMailbox.index - 1].tilt_config[1],
      Shutter[XdrvMailbox.index - 1].tilt_config[2],
      Shutter[XdrvMailbox.index - 1].tilt_config[3],
      Shutter[XdrvMailbox.index - 1].tilt_config[4]
    );
  }
}

void CmndShutterTiltIncDec(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Change in: payload %s (%d), payload %d, idx %d, src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, TasmotaGlobal.last_source );
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present) && XdrvMailbox.data_len > 0) {
    XdrvMailbox.payload = Shutter[XdrvMailbox.index - 1].tilt_target_pos + XdrvMailbox.payload;
    CmndShutterSetTilt();
  } else {
    ResponseCmndIdxNumber(XdrvMailbox.payload);
  }
}

void CmndShutterToggle(void)
{
  ShutterToggle(false);
}

void CmndShutterToggleDir(void)
{
  ShutterToggle(true);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv27(uint32_t function)
{
  bool result = false;

  if (Settings->flag3.shutter_mode) {  // SetOption80 - Enable shutter support
    uint8_t  counter         = XdrvMailbox.index == 0 ? 1 : XdrvMailbox.index;
    uint8_t  counterend      = XdrvMailbox.index == 0 ? TasmotaGlobal.shutters_present : XdrvMailbox.index;
    int32_t  rescue_payload  = XdrvMailbox.payload;
    uint32_t rescue_data_len = XdrvMailbox.data_len;
    char stemp1[10];
    power_t save_powermatrix;
    switch (function) {
      case FUNC_RESTORE_SETTINGS:
        result = ShutterSettingsRestore();
        break;
      case FUNC_SAVE_SETTINGS:
        ShutterSettingsSave();
        break;
      case FUNC_PRE_INIT:
        ShutterSettingsLoad(0);
        ShutterInit();
        break;
      case FUNC_RESET_SETTINGS:
        ShutterSettingsLoad(1);
      break;
      case FUNC_EVERY_50_MSECOND:
        ShutterUpdatePosition();
        break;
      case FUNC_EVERY_SECOND:
      //case FUNC_EVERY_250_MSECOND:
        ShutterReportPosition(false, MAX_SHUTTERS_ESP32);
        break;
      case FUNC_COMMAND:
        for (uint8_t i = counter; i <= counterend; i++) {
          XdrvMailbox.index    = i;
          XdrvMailbox.payload  = rescue_payload;
          XdrvMailbox.data_len = rescue_data_len;
          result = DecodeCommand(kShutterCommands, ShutterCommand);
        }
        break;
      case FUNC_JSON_APPEND:
        for (uint8_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
          ResponseAppend_P(",");
          uint8_t position = ShutterRealToPercentPosition(Shutter[i].real_position, i);
          uint8_t target   = ShutterRealToPercentPosition(Shutter[i].target_position, i);
          ResponseAppend_P(JSON_SHUTTER_POS, i + 1, (ShutterSettings.shutter_options[i] & 1) ? 100 - position : position, Shutter[i].direction,(ShutterSettings.shutter_options[i] & 1) ? 100 - target : target, Shutter[i].tilt_real_pos );
#ifdef USE_DOMOTICZ
          if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
             DomoticzSensor(DZ_SHUTTER, ShutterRealToPercentPosition(Shutter[i].real_position, i));
          }
#endif  // USE_DOMOTICZ
        }
        break;
      case FUNC_SET_POWER:

        // extract the number of the relay that was switched and save for later in Update Position.
        ShutterGlobal.RelayCurrentMask = XdrvMailbox.index ^ ShutterGlobal.RelayOldMask;
        ShutterGlobal.LastChangedRelay = ShutterGetRelayNoFromBitfield(XdrvMailbox.index ^ ShutterGlobal.RelayOldMask);
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: FUNC_SET_POWER Relaymask %d SwitchedRelay:%d by %s, payload %d, powermask %d"), ShutterGlobal.RelayOldMask, ShutterGlobal.LastChangedRelay,GetTextIndexed(stemp1, sizeof(stemp1), TasmotaGlobal.last_source, kCommandSource),XdrvMailbox.payload, TasmotaGlobal.power);
        save_powermatrix = TasmotaGlobal.power; // can be changed in ShutterRelayChanged
        if (!ShutterGlobal.LastChangedRelay) {
          ShutterGlobal.skip_relay_change = 1;
          //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("INVALID REQUEST"));
        } else {
          ShutterRelayChanged();
          ShutterGlobal.RelayOldMask = XdrvMailbox.index; // may be changed and now revert
          TasmotaGlobal.power = save_powermatrix;
        }
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: FUNC_SET_POWER end. powermask %d"), TasmotaGlobal.power);
      break;
      case FUNC_SET_DEVICE_POWER:
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: FUNC_SET_DEVICE_POWER Skipping:%d, Source %s"), ShutterGlobal.skip_relay_change,GetTextIndexed(stemp1, sizeof(stemp1),  XdrvMailbox.payload, kCommandSource));
        if (ShutterGlobal.skip_relay_change ) {
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Skip relay change %d"), i+1);
          result = true;
          ShutterGlobal.skip_relay_change = 0;
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Skipping switch off relay %d"), ShutterGlobal.LastChangedRelay);
          //ExecuteCommandPowerShutter(i+1, 0, SRC_SHUTTER); // should not required anymore. check for bugs
          if (ShutterGlobal.LastChangedRelay) ShutterGlobal.RelayOldMask = TasmotaGlobal.power ^=  1<<(ShutterGlobal.LastChangedRelay-1);
        }
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: FUNC_SET_DEVICE_POWER end. powermask %ld, current rel: %ld"), TasmotaGlobal.power, ShutterGlobal.RelayOldMask);
      break;
      case FUNC_BUTTON_MULTI_PRESSED:
      if (XdrvMailbox.index < MAX_SHUTTERS_ESP32*2 && ShutterSettings.shutter_button[XdrvMailbox.index].enabled) {
          result = ShutterButtonHandlerMulti();
        }
      break;
      case FUNC_BUTTON_PRESSED:
        if (XdrvMailbox.index < MAX_SHUTTERS_ESP32*2 && ShutterSettings.shutter_button[XdrvMailbox.index].enabled) {
          if (!Settings->flag3.mqtt_buttons) Settings->flag3.mqtt_buttons = 1; // ensure to detach buttons from relay to let the shutter controll the relay
          ShutterButtonHandler();
          result = false;
        }
      break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ShutterShow();
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif //USE_SHUTTER

#ifdef SHUTTER_UNITTEST
void CmndShutterUnitTest(void) {
  int16_t input_percent[10] = {-5,0,10,26,35,55,80,99,100,105};
  int16_t output_percent[10] = {0,0,10,26,35,55,80,99,100,100};
  uint32_t result_percent[2][2][10] = {{{0,0,24000,62400,84000,132000,192000,237600,240000,240000},
                                  {0,0,360000,936000,1260000,1980000,2880000,3564000,3600000,3600000}},
                                  {{0,0,76296,100000,113333,174299,205795,237983,240000,240000},
                                  {0,0,1144444,1500000,1700000,2614488,3086929,3569748,3600000,3600000}}};

  uint32_t result = 0;
  char svalue[50];                   // Command and number parameter
  ShutterSettings.shuttercoeff[0][0] = 0;
  for (uint8_t i=0; i<2 ; i++){
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 12);
    ExecuteCommand(svalue, SRC_SHUTTER);
    ShutterInit();
    for (uint8_t j=0; j<2 ; j++){
      for (uint8_t k=0; k<10 ; k++){
        result += (result_percent[i][j][k] == ShutterPercentToRealPosition(input_percent[k] , 0) ? 0 : 1);
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterPercentToRealPosition error %d: %d <-> %d"),result, ShutterPercentToRealPosition(input_percent[k] , 0), result_percent[i][j][k]);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 180);
      ExecuteCommand(svalue, SRC_SHUTTER);
    }
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_CLIBRATION "%d %s"), 1, "15 83 105 185 210");
    ExecuteCommand(svalue, SRC_SHUTTER);
  }
  if (!result){
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterPercentToRealPosition:     PASS"));
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterPercentToRealPosition:     FAIL"));
  }
  ShutterSettings.shuttercoeff[0][0] = 0;
  for (uint8_t i=0; i<2 ; i++){
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 12);
    ExecuteCommand(svalue, SRC_SHUTTER);
    ShutterInit();
    for (uint8_t j=0; j<2 ; j++){
      for (uint8_t k=0; k<10 ; k++){
        result += (output_percent[k] == ShutterRealToPercentPosition(result_percent[i][j][k]  , 0) ? 0 : 1);
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterRealToPercentPosition error %d: %d <-> %d"),result, ShutterRealToPercentPosition(result_percent[i][j][k] , 0), output_percent[k]);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 180);
      ExecuteCommand(svalue, SRC_SHUTTER);
    }
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_CLIBRATION "%d %s"), 1, "15 83 105 185 210");
    ExecuteCommand(svalue, SRC_SHUTTER);
  }
  if (!result){
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterRealToPercentPosition:     PASS"));
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterRealToPercentPosition:     FAIL"));
  }
}
#else
void CmndShutterUnitTest(void) {}
#endif // SHUTTER_UNITTEST

#endif  // ESP32
