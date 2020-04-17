/*
  xdrv_90_heating.ino - Heating controller for Tasmota
  Copyright (C) 2020 Javier Arigita
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

#ifdef USE_HEATING

#define XDRV_39              39

enum HeatingModes { HEAT_OFF, HEAT_AUTOMATIC_OP, HEAT_MANUAL_OP, HEAT_TIME_PLAN };
enum ControllerModes { CTR_HYBRID, CTR_PI, CTR_RAMP_UP };
enum ControllerHybridPhases { CTR_HYBRID_RAMP_UP, CTR_HYBRID_PI };
enum InterfaceStates { IFACE_OFF, IFACE_ON };
enum HeatingSupportedInputSwitches {
  HEATING_INPUT_NONE,
  HEATING_INPUT_SWT1 = 1,           // Buttons
  HEATING_INPUT_SWT2,
  HEATING_INPUT_SWT3,
  HEATING_INPUT_SWT4
};
enum HeatingSupportedOutputRelays {
  HEATING_OUTPUT_NONE,
  HEATING_OUTPUT_REL1 = 1,           // Relays
  HEATING_OUTPUT_REL2,
  HEATING_OUTPUT_REL3,
  HEATING_OUTPUT_REL4,
  HEATING_OUTPUT_REL5,
  HEATING_OUTPUT_REL6,
  HEATING_OUTPUT_REL7,
  HEATING_OUTPUT_REL8
};

const char kHeatingCommands[] PROGMEM = "|" D_CMND_HEATINGMODESET "|" D_CMND_TEMPFROSTPROTECTSET "|" 
  D_CMND_CONTROLLERMODESET "|" D_CMND_INPUTSWITCHSET "|" D_CMND_OUTPUTRELAYSET "|" D_CMND_TIMEALLOWRAMPUPSET "|" 
  D_CMND_TEMPMEASUREDSET "|" D_CMND_TEMPTARGETSET "|" D_CMND_TIMEPLANSET "|" D_CMND_TEMPTARGETREAD "|" 
  D_CMND_TEMPMEASUREDREAD "|" D_CMND_TEMPMEASUREDGRDREAD "|" D_CMND_TEMPSENSNUMBERSET "|" 
  D_CMND_STATEEMERGENCYSET "|" D_CMND_POWERMAXSET "|" D_CMND_TIMEMANUALTOAUTOSET "|" D_CMND_TIMEONLIMITSET "|" 
  D_CMND_PROPBANDSET "|" D_CMND_TIMERESETSET "|" D_CMND_TIMEPICYCLESET "|" D_CMND_TEMPANTIWINDUPRESETSET "|" 
  D_CMND_TEMPHYSTSET "|" D_CMND_TIMEMAXACTIONSET "|" D_CMND_TIMEMINACTIONSET "|" D_CMND_TIMEMINTURNOFFACTIONSET "|"
  D_CMND_TEMPRUPDELTINSET "|" D_CMND_TEMPRUPDELTOUTSET "|" D_CMND_TIMERAMPUPMAXSET "|" D_CMND_TIMERAMPUPCYCLESET  "|" 
  D_CMND_TEMPRAMPUPPIACCERRSET "|" D_CMND_TIMEPIPROPORTREAD "|" D_CMND_TIMEPIINTEGRREAD;

void (* const HeatingCommand[])(void) PROGMEM = {
  &CmndHeatingModeSet, &CmndTempFrostProtectSet, &CmndControllerModeSet, &CmndInputSwitchSet, &CmndOutputRelaySet, 
  &CmndTimeAllowRampupSet, &CmndTempMeasuredSet, &CmndTempTargetSet, &CmndTimePlanSet, &CmndTempTargetRead, 
  &CmndTempMeasuredRead, &CmndTempMeasuredGrdRead, &CmndTempSensNumberSet, &CmndStateEmergencySet, 
  &CmndPowerMaxSet, &CmndTimeManualToAutoSet, &CmndTimeOnLimitSet, &CmndPropBandSet, &CmndTimeResetSet, 
  &CmndTimePiCycleSet, &CmndTempAntiWindupResetSet, &CmndTempHystSet, &CmndTimeMaxActionSet, 
  &CmndTimeMinActionSet, &CmndTimeMinTurnoffActionSet, &CmndTempRupDeltInSet, &CmndTempRupDeltOutSet,
  &CmndTimeRampupMaxSet, &CmndTimeRampupCycleSet, &CmndTempRampupPiAccErrSet, &CmndTimePiProportRead, 
  &CmndTimePiIntegrRead };

const char DOMOTICZ_MES[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}";

struct HEATING {
  uint32_t counter_seconds = 0;                  // Counter incremented every second
  uint8_t heating_mode = HEAT_OFF;               // Operation mode of the heating system
  uint8_t controller_mode = CTR_HYBRID;          // Operation mode of the heating controller
  bool sensor_alive = false;                     // Bool stating if temperature sensor is alive
  bool command_output = false;                   // Bool stating state to save the command to the output (true = active, false = inactive)
  uint8_t phase_hybrid_ctr = CTR_HYBRID_PI;      // Phase of the hybrid controller (Ramp-up or PI)
  uint8_t status_output = IFACE_OFF;             // Status of the output switch
  uint16_t temp_target_level = 180;              // Target level of the heating in tenths of degrees
  uint16_t temp_target_level_ctr = 180;          // Target level set for the controller
  int16_t temp_measured = 0;                     // Temperature measurement received from sensor in tenths of degrees
  uint32_t timestamp_temp_target_update = 0;     // Timestamp of latest target value update
  uint32_t timestamp_temp_measured_update = 0;   // Timestamp of latest measurement value update
  uint32_t timestamp_temp_meas_change_update = 0;// Timestamp of latest measurement value change (> or < to previous)
  uint32_t timestamp_output_on = 0;              // Timestamp of latest heating output On state
  uint32_t timestamp_output_off = 0;             // Timestamp of latest heating output Off state
  uint32_t timestamp_input_on = 0;               // Timestamp of latest input On state
  uint32_t time_heating_total = 0;               // Time heating on within a specific timeframe
  uint32_t time_pi_checkpoint = 0;               // Time to finalize the pi control cycle
  uint32_t time_pi_changepoint = 0;              // Time until switching off output within a pi control cycle
  uint32_t time_rampup_checkpoint = 0;           // Time to switch from ramp-up controller mode to PI
  uint32_t time_rampup_output_off = 0;           // Time to switch off relay output within the ramp-up controller
  uint32_t timestamp_rampup_start = 0;           // Timestamp where the ramp-up controller mode has been started
  uint32_t time_rampup_deadtime = 0;             // Time constant of the heating system (step response time)
  uint32_t time_rampup_nextcycle = 0;            // Time where the ramp-up controller shall start the next cycle
  uint32_t counter_rampup_cycles = 0;            // Counter of ramp-up cycles
  int32_t temp_measured_gradient = 0;            // Temperature measured gradient from sensor in thousandths of degrees per hour
  int32_t temp_rampup_meas_gradient = 0;         // Temperature measured gradient from sensor in thousandths of degrees per hour calculated during ramp-up
  int16_t temp_rampup_output_off = 0;            // Temperature to swith off relay output within the ramp-up controller in tenths of degrees
  int16_t temp_rampup_start = 0;                 // Temperature at start of ramp-up controller in tenths of degrees celsius
  int16_t temp_rampup_cycle = 0;                 // Temperature set at the beginning of each ramp-up cycle in tenths of degrees
  int16_t temp_pi_accum_error = 0;               // Temperature accumulated error for the PI controller in tenths of degrees
  int16_t temp_pi_error = 0;                     // Temperature error for the PI controller in tenths of degrees
  int32_t time_proportional_pi;                  // Time proportional part of the PI controller
  int32_t time_integral_pi;                      // Time integral part of the PI controller
  int32_t time_total_pi;                         // Time total (proportional + integral) of the PI controller
  uint16_t kP_pi = 0;                            // kP value for the PI controller
  uint16_t kI_pi = 0;                            // kP value for the PI controller multiplied by 100
  uint16_t heating_plan[7][6] = {                // Heating plan for the week (3 times/temperatures per day in tenths of degrees)
           {0,0,0,0,0,0},                        // Monday, format {time/temp, time/temp, time/temp}
           {0,0,0,0,0,0},                        // Tuesday, format {time/temp, time/temp, time/temp}
           {0,0,0,0,0,0},                        // Wednesday, format {time/temp, time/temp, time/temp}
           {0,0,0,0,0,0},                        // Thursday, format {time/temp, time/temp, time/temp}
           {0,0,0,0,0,0},                        // Friday, format {time/temp, time/temp, time/temp}
           {0,0,0,0,0,0},                        // Saturday, format {time/temp, time/temp, time/temp}
           {0,0,0,0,0,0}                         // Sunday, format {time/temp, time/temp, time/temp}
           };
  bool status_cycle_active = false;              // Status showing if cycle is active (Output ON) or not (Output OFF)
} Heating;

/*********************************************************************************************/

void HeatingInit()
{
  ExecuteCommandPower(Settings.output_relay_number, POWER_OFF, SRC_HEATING); // Make sure the Output is OFF
}

bool HeatingMinuteCounter() 
{
  bool result = false;
  Heating.counter_seconds++;    // increment time
  
  if ((Heating.counter_seconds % 60) == 0) {
    result = true;    
  }
  return(result);
}

inline bool HeatingSwitchIdValid(uint8_t switchId) 
{
  return (switchId >= HEATING_INPUT_SWT1 && switchId <= HEATING_INPUT_SWT4);
}

inline bool HeatingRelayIdValid(uint8_t relayId) 
{
  return (relayId >= HEATING_OUTPUT_REL1 && relayId <= HEATING_OUTPUT_REL8);
}

uint8_t HeatingSwitchStatus(uint8_t input_switch)
{
  bool ifId = HeatingSwitchIdValid(input_switch);
  if(ifId) {
    return(SwitchGetVirtual(ifId - HEATING_INPUT_SWT1));
  }
  else return 255;
}

void HeatingSignalProcessingSlow()
{
  if ((uptime - Heating.timestamp_temp_measured_update) > Settings.time_sens_lost) { // Check if sensor alive
    Heating.sensor_alive = false;
    Heating.temp_measured_gradient = 0;
    Heating.temp_measured = 0;
  }
}

void HeatingSignalProcessingFast()
{
  if (HeatingSwitchStatus(Settings.input_switch_number)) { // Check if input switch active and register last update
    Heating.timestamp_input_on = uptime;
  }
}

void HeatingCtrState()
{
  switch (Heating.controller_mode) {
    case CTR_HYBRID:                    // Ramp-up phase with gradient control
        HeatingHybridCtrPhase();
      break;
    case CTR_PI:
      break;
    case CTR_RAMP_UP:
      break;
  }
}

void HeatingHybridCtrPhase()
{
  if (Heating.controller_mode == CTR_HYBRID) {
    switch (Heating.phase_hybrid_ctr) {
      case CTR_HYBRID_RAMP_UP:                    // Ramp-up phase with gradient control
          // If ramp-up offtime counter has been initalized    
          // AND ramp-up offtime counter value reached
          if((Heating.time_rampup_checkpoint != 0) 
            && (uptime >= Heating.time_rampup_checkpoint)) {
            // Reset pause period
            Heating.time_rampup_checkpoint = 0;
            // Set PI controller
            Heating.phase_hybrid_ctr = CTR_HYBRID_PI;
          }
        break;
      case CTR_HYBRID_PI:                         // PI controller phase
          // If no output action for a pre-defined time
          // AND temp target has changed
          // AND temp target - target actual bigger than threshold
          // then go to ramp-up
          if (((uptime - Heating.timestamp_output_off) > Settings.time_allow_rampup)
            && (Heating.temp_target_level != Heating.temp_target_level_ctr)
            &&((Heating.temp_target_level - Heating.temp_measured) > Settings.temp_rampup_delta_in)) {
              Heating.phase_hybrid_ctr = CTR_HYBRID_RAMP_UP;
              Heating.timestamp_rampup_start = uptime;
              Heating.temp_rampup_start = Heating.temp_measured;
              Heating.temp_rampup_meas_gradient = 0;
              Heating.time_rampup_checkpoint = 0;
              Heating.time_rampup_deadtime = 0;
              Heating.counter_rampup_cycles = 1;
          }
        break;
    }
  }
}

bool HeatStateAutoOrPlanToManual()
{
  bool change_state = false;

  // If switch input is active
  // OR temperature sensor is not alive
  // then go to manual
  if ((HeatingSwitchStatus(Settings.input_switch_number) == 1)
    || (Heating.sensor_alive == false)) {
    change_state = true;
  }
  return change_state;
}

bool HeatStateManualToAuto()
{
  bool change_state;

  // If switch input inactive
  // AND no switch input action (time in current state) bigger than a pre-defined time
  // then go to automatic
  if ((HeatingSwitchStatus(Settings.input_switch_number) == 0) 
    && ((uptime - Heating.timestamp_input_on) > Settings.time_manual_to_auto)) {
    change_state = true;
  }
  return change_state;
}

bool HeatStateAllToOff()
{
  bool change_state;

  // If emergency mode then switch OFF the output inmediately
  if (Settings.state_emergency) {
    Heating.heating_mode = HEAT_OFF;  // Emergency switch to HEAT_OFF
  }
  return change_state;
}

void HeatingState()
{
  switch (Heating.heating_mode) {
    case HEAT_OFF:                              // State if Off or Emergency
      // No change of state possible without external command
      break;
    case HEAT_AUTOMATIC_OP:                     // State automatic heating active following to command target temp.
      if (HeatStateAllToOff()) {
        Heating.heating_mode = HEAT_OFF;        // Emergency switch to HEAT_OFF
      }
      if (HeatStateAutoOrPlanToManual()) {
        Heating.heating_mode = HEAT_MANUAL_OP;  // If sensor not alive change to HEAT_MANUAL_OP
      }
      HeatingCtrState();
      break;
    case HEAT_MANUAL_OP:                        // State manual operation following input switch
      if (HeatStateAllToOff()) {
        Heating.heating_mode = HEAT_OFF;        // Emergency switch to HEAT_OFF
      }
      if (HeatStateManualToAuto()) {
        Heating.heating_mode = HEAT_AUTOMATIC_OP;  // Input switch inactive and timeout reached change to HEAT_AUTOMATIC_OP
      }
      break;
    case HEAT_TIME_PLAN:                        // State automatic heating active following set heating plan
      if (HeatStateAllToOff()) {
        Heating.heating_mode = HEAT_OFF;        // Emergency switch to HEAT_OFF
      }
      if (HeatStateAutoOrPlanToManual()) {
        Heating.heating_mode = HEAT_MANUAL_OP;  // If sensor not alive change to HEAT_MANUAL_OP
      }
      HeatingCtrState();
      break;
  }
}

void HeatingOutputRelay(bool active)
{
  // TODO: See if the real output state can be read by f.i. bitRead(power, Settings.output_relay_number))
  // If command received to enable output
  // AND current output status is OFF
  // then switch output to ON
  if ((active == true) 
    && (Heating.status_output == IFACE_OFF)) {
    ExecuteCommandPower(Settings.output_relay_number, POWER_ON, SRC_HEATING);
    Heating.timestamp_output_on = uptime;
    Heating.status_output = IFACE_ON;
  }
  // If command received to disable output
  // AND current output status is ON
  // then switch output to OFF
  else if ((active == false) && (Heating.status_output == IFACE_ON)) {
    ExecuteCommandPower(Settings.output_relay_number, POWER_OFF, SRC_HEATING);
    Heating.timestamp_output_off = uptime;
    Heating.status_output = IFACE_OFF;
  }
}

void HeatingCalculatePI()
{
  // Calculate error
  Heating.temp_pi_error = Heating.temp_target_level_ctr - Heating.temp_measured;
  // Kp = 100/PI.propBand. PI.propBand(Xp) = Proportional range (4K in 4K/200 controller)
  Heating.kP_pi = 100 / (uint16_t)(Settings.val_prop_band);
  // Calculate proportional
  Heating.time_proportional_pi = ((int32_t)(Heating.temp_pi_error * (int16_t)Heating.kP_pi) * Settings.time_pi_cycle) / 1000;

  // Minimum proportional action limiter
  // If proportional action is less than the minimum action time
  // AND proportional > 0
  // then adjust to minimum value
  if ((Heating.time_proportional_pi < abs(Settings.time_min_action))
    && (Heating.time_proportional_pi > 0)) {
    Heating.time_proportional_pi = Settings.time_min_action;
  }
  
  if (Heating.time_proportional_pi < 0) {
    Heating.time_proportional_pi = 0;
  } 
  else if (Heating.time_proportional_pi > Settings.time_pi_cycle) {
    Heating.time_proportional_pi = Settings.time_pi_cycle;
  }

  // Calculate integral
  Heating.kI_pi = (uint16_t)(((float)Heating.kP_pi * ((float)Settings.time_pi_cycle / (float)Settings.time_reset)) * 100);
  
  // Reset of antiwindup
  // If error does not lay within the integrator scope range, do not use the integral
  // and accumulate error = 0
  if (abs(Heating.temp_pi_error) > Settings.temp_reset_anti_windup) {
    Heating.time_integral_pi = 0;
    Heating.temp_pi_accum_error = 0;
  }
  // Normal use of integrator
  // result will be calculated with the cummulated previous error anterior 
  // and current error will be cummulated to the previous one
  else {
    // Hysteresis limiter
    // If error is less than or equal than hysteresis, limit output to 0, when temperature
    // is rising, never when falling. Limit cummulated error. If this is not done,
    // there will be very strong control actions from the integral part due to a 
    // very high cummulated error when beingin hysteresis. This triggers high
    // integral actions

    // If we are under setpoint
    // AND we are within the hysteresis
    // AND we are rising
    if ((Heating.temp_pi_error >= 0)
      && (abs(Heating.temp_pi_error) <= (int16_t)Settings.temp_hysteresis)
      && (Heating.temp_measured_gradient > 0)) {
      Heating.temp_pi_accum_error += Heating.temp_pi_error;
      // Reduce accumulator error 20% in each cycle
      Heating.temp_pi_accum_error *= 0.8;
    }
    // If we are over setpoint
    // AND temperature is rising
    else if ((Heating.temp_pi_error < 0)
      && (Heating.temp_measured_gradient > 0)) {
      Heating.temp_pi_accum_error += Heating.temp_pi_error;
      // Reduce accumulator error 20% in each cycle
      Heating.temp_pi_accum_error *= 0.8;
    }
    else {
      Heating.temp_pi_accum_error += Heating.temp_pi_error;
    }

    // Limit lower limit of acumErr to 0
    if (Heating.temp_pi_accum_error < 0) {
      Heating.temp_pi_accum_error = 0;
    }

    // Integral calculation
    Heating.time_integral_pi = ((((int32_t)Heating.temp_pi_accum_error * (int32_t)Heating.kI_pi) / 100) * (int32_t)(Settings.time_pi_cycle)) / 1000;

    // Antiwindup of the integrator
    // If integral calculation is bigger than cycle time, adjust result
    // to the cycle time and error will not be cummulated]]
    if (Heating.time_integral_pi > Settings.time_pi_cycle) {
      Heating.time_integral_pi = Settings.time_pi_cycle;
    }
  }

  // Calculate output
  Heating.time_total_pi = Heating.time_proportional_pi + Heating.time_integral_pi;
  
  // Antiwindup of the output
  // If result is bigger than cycle time, the result will be adjusted
  // to the cylce time minus safety time and error will not be cummulated]]
  if (Heating.time_total_pi > Settings.time_pi_cycle) {
    // Limit to cycle time //at least switch down a minimum time
    Heating.time_total_pi = Settings.time_pi_cycle;
  }
  else if (Heating.time_total_pi < 0) {
    Heating.time_total_pi = 0;
  }

  // Target value limiter
  // If target value has been reached or we are over it]]
  if (Heating.temp_pi_error <= 0) {
    // If we are over the hysteresis or the gradient is positive
    if ((abs(Heating.temp_pi_error) > Settings.temp_hysteresis)
      || (Heating.temp_measured_gradient >= 0)) {
      Heating.time_total_pi = 0;
    }
  } 
  // If target value has not been reached
  // AND we are withing the histeresis
  // AND gradient is positive
  // then set value to 0
  else if ((Heating.temp_pi_error > 0)
    && (abs(Heating.temp_pi_error) <= Settings.temp_hysteresis)
    && (Heating.temp_measured_gradient > 0)) {
    Heating.time_total_pi = 0;
  }

  // Minimum action limiter
  // If result is less than the minimum action time, adjust to minimum value]]
  if ((Heating.time_total_pi <= abs(Settings.time_min_action))
    && (Heating.time_total_pi != 0)) {
    Heating.time_total_pi = Settings.time_min_action;
  }
  // Maximum action limiter
  // If result is more than the maximum action time, adjust to maximum value]]
  else if (Heating.time_total_pi > abs(Settings.time_max_action)) {
    Heating.time_total_pi = Settings.time_max_action;
  }
  // If switched off less time than safety time, do not switch off
  else if (Heating.time_total_pi > (Settings.time_pi_cycle - Settings.time_min_turnoff_action)) {
    Heating.time_total_pi = Settings.time_pi_cycle;
  }
  
  // Adjust output switch point
  Heating.time_pi_changepoint = uptime + Heating.time_total_pi;
  // Adjust next cycle point
  Heating.time_pi_checkpoint = uptime + Settings.time_pi_cycle;
}

void HeatingWorkAutomaticPI()
{
  char result_chr[FLOATSZ]; // Remove!

  if ((uptime >= Heating.time_pi_checkpoint) 
    || (Heating.temp_target_level != Heating.temp_target_level_ctr)
    || ((Heating.temp_measured < Heating.temp_target_level)
        && (Heating.temp_measured_gradient < 0)
        && (Heating.status_cycle_active == false))) {
    Heating.temp_target_level_ctr = Heating.temp_target_level;
    HeatingCalculatePI();
    // Reset cycle active
    Heating.status_cycle_active = false;
  }
  if (uptime < Heating.time_pi_changepoint) {
    Heating.status_cycle_active = true;
    Heating.command_output = true;
  }
  else {
    Heating.command_output = false;
  }
}

void HeatingWorkAutomaticRampUp()
{
  uint32_t time_in_rampup;
  int16_t temp_delta_rampup;

  // Update timestamp for temperature at start of ramp-up if temperature still dropping
  if (Heating.temp_measured < Heating.temp_rampup_start) {
    Heating.temp_rampup_start = Heating.temp_measured;
  }

  // Update time in ramp-up as well as delta temp
  time_in_rampup = uptime - Heating.timestamp_rampup_start;
  temp_delta_rampup = Heating.temp_measured - Heating.temp_rampup_start;
  // Init command output status to true
  Heating.command_output = true;
  // Update temperature target level for controller
  Heating.temp_target_level_ctr = Heating.temp_target_level;

  // If time in ramp-up < max time
  // AND temperature measured < target
  if ((time_in_rampup <= Settings.time_rampup_max)
    && (Heating.temp_measured < Heating.temp_target_level)) {
    // DEADTIME point reached
    // If temperature measured minus temperature at start of ramp-up >= threshold
    // AND deadtime still 0
    if ((temp_delta_rampup >= Settings.temp_rampup_delta_out) 
      && (Heating.time_rampup_deadtime == 0)) {
      // Set deadtime, assuming it is half of the time until slope, since thermal inertia of the temp. fall needs to be considered
      // minus open time of the valve (arround 3 minutes). If rise very fast limit it to delay of output valve     
      int32_t time_aux;
      time_aux = ((time_in_rampup / 2) - Settings.time_output_delay);
      if (time_aux >= Settings.time_output_delay) {
        Heating.time_rampup_deadtime = (uint32_t)time_aux;
      }
      else {
        Heating.time_rampup_deadtime = Settings.time_output_delay;
      }
      // Calculate gradient since start of ramp-up (considering deadtime) in thousandths of º/hour
      Heating.temp_rampup_meas_gradient = (int32_t)((360000 * (int32_t)temp_delta_rampup) / (int32_t)time_in_rampup);
      Heating.time_rampup_nextcycle = uptime + Settings.time_rampup_cycle;
      // Set auxiliary variables
      Heating.temp_rampup_cycle = Heating.temp_measured;
      Heating.time_rampup_output_off = uptime + Settings.time_rampup_max;
      Heating.temp_rampup_output_off =  Heating.temp_target_level_ctr;
    }
    // Gradient calculation every time_rampup_cycle
    else if ((Heating.time_rampup_deadtime > 0) && (uptime >= Heating.time_rampup_nextcycle)) {
      // Calculate temp. gradient in º/hour and set again time_rampup_nextcycle and temp_rampup_cycle
      // temp_rampup_meas_gradient = ((3600 * temp_delta_rampup) / (os.time() - time_rampup_nextcycle))
      temp_delta_rampup = Heating.temp_measured - Heating.temp_rampup_cycle;
      uint32_t time_total_rampup = Settings.time_rampup_cycle * Heating.counter_rampup_cycles;
      // Translate into gradient per hour (thousandths of ° per hour)
      Heating.temp_rampup_meas_gradient = int32_t((360000 * (int32_t)temp_delta_rampup) / (int32_t)time_total_rampup);
      if (Heating.temp_rampup_meas_gradient > 0) {
        // Calculate time to switch Off and come out of ramp-up
        // y-y1 = m(x-x1) -> x = ((y-y1) / m) + x1 -> y1 = temp_rampup_cycle, x1 = (time_rampup_nextcycle - time_rampup_cycle), m = gradient in º/sec
        // Better Alternative -> (y-y1)/(x-x1) = ((y2-y1)/(x2-x1)) -> where y = temp (target) and x = time (to switch off, what its needed)
        // x = ((y-y1)/(y2-y1))*(x2-x1) + x1 - deadtime
        // Heating.time_rampup_output_off = (uint32_t)(((float)(Heating.temp_target_level_ctr - Heating.temp_rampup_cycle) / (float)temp_delta_rampup) * (float)(time_total_rampup)) + (uint32_t)(Heating.time_rampup_nextcycle - (time_total_rampup)) - Heating.time_rampup_deadtime;
        Heating.time_rampup_output_off = (uint32_t)(((float)(Heating.temp_target_level_ctr - Heating.temp_rampup_cycle) * (float)(time_total_rampup)) / (float)temp_delta_rampup) + (uint32_t)(Heating.time_rampup_nextcycle - (time_total_rampup)) - Heating.time_rampup_deadtime;      
        
        // Calculate temperature for switching off the output
        // y = (((y2-y1)/(x2-x1))*(x-x1)) + y1
        // Heating.temp_rampup_output_off = (int16_t)(((float)(temp_delta_rampup) / (float)(time_total_rampup * Heating.counter_rampup_cycles)) * (float)(Heating.time_rampup_output_off - (uptime - (time_total_rampup)))) + Heating.temp_rampup_cycle;
        Heating.temp_rampup_output_off = (int16_t)(((float)temp_delta_rampup * (float)(Heating.time_rampup_output_off - (uptime - (time_total_rampup)))) / (float)(time_total_rampup * Heating.counter_rampup_cycles)) + Heating.temp_rampup_cycle;
        // Set auxiliary variables
        Heating.time_rampup_nextcycle = uptime + Settings.time_rampup_cycle;
        Heating.temp_rampup_cycle = Heating.temp_measured;
        // Reset period counter
        Heating.counter_rampup_cycles = 1;
      }
      else {
        // Increase the period counter
        Heating.counter_rampup_cycles++;
        // Set another period
        Heating.time_rampup_nextcycle = uptime + Settings.time_rampup_cycle;
        // Reset time_rampup_output_off and temp_rampup_output_off
        Heating.time_rampup_output_off = uptime + Settings.time_rampup_max - time_in_rampup;
        Heating.temp_rampup_output_off =  Heating.temp_target_level_ctr;
      }
      // Set time to get out of calibration
      Heating.time_rampup_checkpoint = Heating.time_rampup_output_off + Heating.time_rampup_deadtime;
    }

    // Set output switch ON or OFF
    // If deadtime has not been calculated
    // or checkpoint has not been calculated
    // or it is not yet time and temperature to switch it off acc. to calculations
    // or gradient is <= 0
    if ((Heating.time_rampup_deadtime == 0)
      || (Heating.time_rampup_checkpoint == 0)
      || (uptime < Heating.time_rampup_output_off)
      || (Heating.temp_measured < Heating.temp_rampup_output_off)
      || (Heating.temp_rampup_meas_gradient <= 0)) {
      Heating.command_output = true;
    }
    else {
      Heating.command_output = false;
    }
  }
  else {
    // If we have not reached the temperature, start with an initial value for accumulated error for the PI controller
    if (Heating.temp_measured < Heating.temp_target_level_ctr) {
      Heating.temp_pi_accum_error = Settings.temp_rampup_pi_acc_error;
    }
    // Set to now time to get out of calibration
    Heating.time_rampup_checkpoint = uptime;
    // Switch Off output
    Heating.command_output = false;
  }
}

void HeatingCtrWork()
{
  switch (Heating.controller_mode) {
    case CTR_HYBRID:                    // Ramp-up phase with gradient control
      switch (Heating.phase_hybrid_ctr) {
        case CTR_HYBRID_RAMP_UP:
          HeatingWorkAutomaticRampUp();
          break;
        case CTR_HYBRID_PI:
          HeatingWorkAutomaticPI();
          break;
      }
      break;
    case CTR_PI:
      HeatingWorkAutomaticPI();
      break;
    case CTR_RAMP_UP:
      HeatingWorkAutomaticRampUp();
      break;
  }
}

void HeatingPlanTempTarget()
{
  int16_t tmp_minute_delta[3]; // Array of deltas in minute for 3 different time minutes of a day to the current utc time minute of the day
  uint8_t time_selected = 0; // Index of time selected
  int16_t time_lowest_delta = 1440; // lowest time delta in minutes from the array values to UTC, initiated to 1 day (1440 minutes)
  uint8_t day_of_week = RtcTime.day_of_week; // Current day of week (1 = Sun)

  // For each of the three times within the current day of week
  for (uint8_t i=0; i<3; i++) {

    // Store time difference between current minute of the day and the minute of the day of each planned time wihtin array
    tmp_minute_delta[i] = ((((int16_t)RtcTime.hour * 60) + (int16_t)RtcTime.minute) - (int16_t)Heating.heating_plan[day_of_week - 1][(i * 2)]);
    
    if ((tmp_minute_delta[i] >= 0) &&
      (tmp_minute_delta[i] < time_lowest_delta)) {
      time_lowest_delta = tmp_minute_delta[i];
      time_selected = i;
    }
  }

  // Update target value if time delta to selected time is 0 or positive
  if ((tmp_minute_delta[time_selected] >= 0)
    && (Heating.heating_plan[day_of_week - 1][(2 * time_selected) + 1] >= Settings.temp_frost_protect)) {
    Heating.temp_target_level = Heating.heating_plan[day_of_week - 1][(2 * time_selected) + 1];
  }
}

void HeatingWork()
{
  switch (Heating.heating_mode) {
    case HEAT_OFF:                              // State if Off or Emergency
      Heating.command_output = false;
      break;
    case HEAT_AUTOMATIC_OP:                     // State automatic heating active following to command target temp.
      HeatingCtrWork();
      break;
    case HEAT_MANUAL_OP:                        // State manual operation following input switch
      Heating.time_rampup_checkpoint = 0;
      break;
    case HEAT_TIME_PLAN:                        // State automatic heating active following set heating plan
      // Set target temperature based on plan
      HeatingPlanTempTarget();
      HeatingCtrWork();
      break;
  }
  HeatingOutputRelay(Heating.command_output);
}

void HeatingDiagnostics()
{
  // TODOs: 
  // 1. Check time max for output switch on not exceeded
  // 2. Check state of output corresponds to command
  // 3. Check maximum power at output switch not exceeded
}

void HeatingController()
{
  HeatingState();
  HeatingWork();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndHeatingModeSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
    if ((value >= HEAT_OFF) && (value <= HEAT_TIME_PLAN)) {
      Heating.heating_mode = value;
      Heating.timestamp_input_on = 0;     // Reset last manual switch timer if command set externally
    }
  }
  ResponseCmndNumber((int)Heating.heating_mode);
}

void CmndTempFrostProtectSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 255)) {
      Settings.temp_frost_protect = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_frost_protect) / 10, 1);
}

void CmndControllerModeSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= CTR_RAMP_UP)) {
      Heating.controller_mode = value;
    }
  }
  ResponseCmndNumber((int)Heating.controller_mode);
}

void CmndInputSwitchSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if (HeatingSwitchIdValid(value)) {
      Settings.input_switch_number = value;
      Heating.timestamp_input_on = uptime;
    }
  }
  ResponseCmndNumber((int)Settings.input_switch_number);
}

void CmndOutputRelaySet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if (HeatingRelayIdValid(value)) {
      Settings.output_relay_number = value;
    }
  }
  ResponseCmndNumber((int)Settings.output_relay_number);
}

void CmndTimeAllowRampupSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value < 86400)) {
      Settings.time_allow_rampup = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_allow_rampup);
}

void CmndTempMeasuredSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    int16_t value = (int16_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= -1000) && (value <= 1000)) {
      uint32_t timestamp = uptime;
      // Calculate temperature gradient if temperature value has changed
      if (value != Heating.temp_measured) {
        int16_t temp_delta = (value - Heating.temp_measured); // in tenths of degrees
        uint32_t time_delta = (timestamp - Heating.timestamp_temp_meas_change_update); // in seconds
        Heating.temp_measured_gradient = (int32_t)((360000 * (int32_t)temp_delta) / (int32_t)time_delta); // hundreths of degrees per hour
        Heating.temp_measured = value;
        Heating.timestamp_temp_meas_change_update = timestamp;
      }
      Heating.timestamp_temp_measured_update = timestamp;
      Heating.sensor_alive = true;
    }
  }
  ResponseCmndFloat(((float)Heating.temp_measured) / 10, 1);
}

void CmndTempTargetSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint16_t value = (uint16_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= -1000) 
      && (value <= 1000)
      && (value >= Settings.temp_frost_protect)) {
      Heating.temp_target_level = value;
      Heating.timestamp_temp_target_update = uptime;
    }
  }
  ResponseCmndFloat(((float)Heating.temp_target_level) / 10, 1);
}

void CmndTimePlanSet(void)
{
  // TimePlanSet1 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet1 3 sets of hour and related target temperature for Monday´s
  // TimePlanSet2 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet2 3 sets of hour and related target temperature for Tuesday´s
  // TimePlanSet3 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet3 3 sets of hour and related target temperature for Wednesday´s
  // TimePlanSet4 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet4 3 sets of hour and related target temperature for Thursday´s
  // TimePlanSet5 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet5 3 sets of hour and related target temperature for Friday´s
  // TimePlanSet6 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet6 3 sets of hour and related target temperature for Saturday´s
  // TimePlanSet7 05:00/21.0, 15:30/22.5, 23:00/18.0 - TimePlanSet7 3 sets of hour and related target temperature for Sunday´s

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 7)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t aux_index = 0;
      char *p;
      char *str = strtok_r(XdrvMailbox.data, ", ", &p);  // Extract first pair time/temp -> 05:00/21.0

      while ((str != nullptr) && (aux_index < 6)) {
        char *q;
        char *minutes;
        char *temp;
        char *temp_decimal;
        float temp_f;
        uint8_t str_len =strlen(str);
        
        // Check basic structure of the data, length matching, position of ":" and "/" matching
        if((str_len > 0) && (str_len < 11) && (str[2] == ':') && (str[5] == '/')) {
          // Extract the time
          uint16_t value = strtol(str, &q, 10);            // extract 5

          char value_c[33];
          dtostrfd((double)value, 0, value_c);

          if ((value >= 0) && (value < 24)) {                                // Below 24 is hours
            uint8_t day_of_week = XdrvMailbox.index -1;
            minutes = strtok_r(nullptr, ":/.", &q);
            Heating.heating_plan[day_of_week][aux_index] = (value * 60);// Multiply hours by 60 minutes
            if (minutes) {
              value = strtol(minutes, nullptr, 10);         // extract 00
              if (value <= 59) {
                Heating.heating_plan[day_of_week][aux_index] += value;
              }
            }
            aux_index++;

            // Extract the whole-number part of the temperature
            temp = strtok_r(nullptr, ":/.", &q);

            if (temp) {
              value = strtol(temp, nullptr, 10);         // extract 00             
              temp_f = CharToFloat((char*)temp);
              temp_decimal = strtok_r(nullptr, ":/.", &q);
              if (temp_decimal) {
                value = strtol(temp_decimal, nullptr, 10);         // extract decimal part              
                if (value <= 9) {              
                  temp_f += (CharToFloat((char*)temp_decimal) / 10);
                }
              }
              if ((temp_f > -100) && (temp_f < 100)) {
                Heating.heating_plan[day_of_week][aux_index] = (uint16_t)(temp_f * 10); // Multiply degrees by 10 to convert to decidegrees
              }
            }
            aux_index++;
          }
          else {
            aux_index += 2;
          }
        }
        else {
          aux_index += 2;
        }
        str = strtok_r(nullptr, ", ", &p);
      }
    }
    uint8_t index_d = XdrvMailbox.index;
    char day[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char *day_p = &day[index_d - 1][0];
    char index_s[strlen(day_p)+1];
    strcpy(index_s, day_p);
    char temp1[5];
    char temp2[5];
    char temp3[5];
    dtostrfd((double)((float)Heating.heating_plan[index_d - 1][1] / 10), 1, temp1);
    dtostrfd((double)((float)Heating.heating_plan[index_d - 1][3] / 10), 1, temp2);
    dtostrfd((double)((float)Heating.heating_plan[index_d - 1][5] / 10), 1, temp3);

    Response_P(PSTR("{\"%s\":{\"1stTime\":{\"Time\":\"%s\",\"Temperature\":\"%s\"},\"2ndTime\":{\"Time\":\"%s\",\"Temperature\":\"%s\"},\"3rdTime\":{\"Time\":\"%s\",\"Temperature\":\"%s\"}}"),
      index_s,
      GetMinuteTime(Heating.heating_plan[index_d - 1][0]).c_str(),temp1,
      GetMinuteTime(Heating.heating_plan[index_d - 1][2]).c_str(),temp2,
      GetMinuteTime(Heating.heating_plan[index_d - 1][4]).c_str(),temp3);
  }
}

void CmndTempTargetRead(void)
{
  ResponseCmndFloat(((float)Heating.temp_target_level) / 10, 1);
}

void CmndTempMeasuredRead(void)
{
  ResponseCmndFloat((float)(Heating.temp_measured) / 10, 1);
}

void CmndTempMeasuredGrdRead(void)
{
  ResponseCmndFloat((float)(Heating.temp_measured_gradient) / 1000, 1);
}

void CmndTempSensNumberSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 255)) {
      Settings.temp_sens_number = value;
    }
  }
  ResponseCmndNumber((int)Settings.temp_sens_number);
}

void CmndStateEmergencySet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 1)) {
      Settings.state_emergency = (bool)value;
    }
  }
  ResponseCmndNumber((int)Settings.state_emergency);
}

void CmndPowerMaxSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint16_t value = (uint16_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 1300)) {
      Settings.power_max = value;
    }
  }
  ResponseCmndNumber((int)Settings.power_max);
}

void CmndTimeManualToAutoSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_manual_to_auto = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_manual_to_auto);
}

void CmndTimeOnLimitSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_on_limit = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_on_limit);
}

void CmndPropBandSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 20)) {
      Settings.val_prop_band = value;
    }
  }
  ResponseCmndNumber((int)Settings.val_prop_band);
}

void CmndTimeResetSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_reset = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_reset);
}

void CmndTimePiCycleSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_pi_cycle = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_pi_cycle);
}

void CmndTempAntiWindupResetSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= (float)(0)) && (value <= (float)(100.0))) {
      Settings.temp_reset_anti_windup = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_reset_anti_windup) / 10, 1);
}

void CmndTempHystSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    int8_t value = (int8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= -100) && (value <= 100)) {
      Settings.temp_hysteresis = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_hysteresis) / 10, 1);
}

void CmndTimeMaxActionSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_max_action = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_max_action);
}

void CmndTimeMinActionSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_min_action = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_min_action);
}

void CmndTimeMinTurnoffActionSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_min_turnoff_action = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_min_turnoff_action);
}

void CmndTempRupDeltInSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 100)) {
      Settings.temp_rampup_delta_in = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_rampup_delta_in) / 10, 1);
}

void CmndTempRupDeltOutSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 100)) {
      Settings.temp_rampup_delta_out = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_rampup_delta_out) / 10, 1);
}

void CmndTimeRampupMaxSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_rampup_max = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_rampup_max);
}

void CmndTimeRampupCycleSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Settings.time_rampup_cycle = value;
    }
  }
  ResponseCmndNumber((int)Settings.time_rampup_cycle);
}

void CmndTempRampupPiAccErrSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 250)) {
      Settings.temp_rampup_pi_acc_error = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_rampup_pi_acc_error) / 10, 1);
}

void CmndTimePiProportRead(void)
{
  ResponseCmndNumber((int)Heating.time_proportional_pi);
}

void CmndTimePiIntegrRead(void)
{
  ResponseCmndNumber((int)Heating.time_integral_pi);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv39(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      HeatingInit();
      break;
    case FUNC_LOOP:
      HeatingSignalProcessingFast();
      HeatingDiagnostics();
      break;
    case FUNC_SERIAL:
      break;
    case FUNC_EVERY_SECOND:
      if (HeatingMinuteCounter()) {
        HeatingSignalProcessingSlow();
        HeatingController();
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kHeatingCommands, HeatingCommand);
      break;
  }
  return result;
}

#endif // USE_HEATING
