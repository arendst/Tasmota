/*
  xdrv_39_thermostat.ino - Thermostat controller for Tasmota

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

#ifdef USE_THERMOSTAT

#define XDRV_39              39

// Enable/disable debugging
//#define DEBUG_THERMOSTAT

#ifdef DEBUG_THERMOSTAT
#define DOMOTICZ_IDX1        791
#define DOMOTICZ_IDX2        792
#define DOMOTICZ_IDX3        793
#endif

// Commands
#define D_CMND_THERMOSTATMODESET "ThermostatModeSet"
#define D_CMND_TEMPFROSTPROTECTSET "TempFrostProtectSet"
#define D_CMND_CONTROLLERMODESET "ControllerModeSet"
#define D_CMND_INPUTSWITCHSET "InputSwitchSet"
#define D_CMND_OUTPUTRELAYSET "OutputRelaySet"
#define D_CMND_TIMEALLOWRAMPUPSET "TimeAllowRampupSet"
#define D_CMND_TEMPMEASUREDSET "TempMeasuredSet"
#define D_CMND_TEMPTARGETSET "TempTargetSet"
#define D_CMND_TEMPTARGETREAD "TempTargetRead"
#define D_CMND_TEMPMEASUREDREAD "TempMeasuredRead"
#define D_CMND_TEMPMEASUREDGRDREAD "TempMeasuredGrdRead"
#define D_CMND_TEMPSENSNUMBERSET "TempSensNumberSet"
#define D_CMND_STATEEMERGENCYSET "StateEmergencySet"
#define D_CMND_POWERMAXSET "PowerMaxSet"
#define D_CMND_TIMEMANUALTOAUTOSET "TimeManualToAutoSet"
#define D_CMND_TIMEONLIMITSET "TimeOnLimitSet"
#define D_CMND_PROPBANDSET "PropBandSet"
#define D_CMND_TIMERESETSET "TimeResetSet"
#define D_CMND_TIMEPICYCLESET "TimePiCycleSet"
#define D_CMND_TEMPANTIWINDUPRESETSET "TempAntiWindupResetSet"
#define D_CMND_TEMPHYSTSET "TempHystSet"
#define D_CMND_TIMEMAXACTIONSET "TimeMaxActionSet"
#define D_CMND_TIMEMINACTIONSET "TimeMinActionSet"
#define D_CMND_TIMEMINTURNOFFACTIONSET "TimeMinTurnoffActionSet"
#define D_CMND_TEMPRUPDELTINSET "TempRupDeltInSet"
#define D_CMND_TEMPRUPDELTOUTSET "TempRupDeltOutSet"
#define D_CMND_TIMERAMPUPMAXSET "TimeRampupMaxSet"
#define D_CMND_TIMERAMPUPCYCLESET "TimeRampupCycleSet"
#define D_CMND_TEMPRAMPUPPIACCERRSET "TempRampupPiAccErrSet"
#define D_CMND_TIMEPIPROPORTREAD "TimePiProportRead"
#define D_CMND_TIMEPIINTEGRREAD "TimePiIntegrRead"
#define D_CMND_TIMESENSLOSTSET "TimeSensLostSet"

enum ThermostatModes { THERMOSTAT_OFF, THERMOSTAT_AUTOMATIC_OP, THERMOSTAT_MANUAL_OP, THERMOSTAT_MODES_MAX };
enum ControllerModes { CTR_HYBRID, CTR_PI, CTR_RAMP_UP, CTR_MODES_MAX };
enum ControllerHybridPhases { CTR_HYBRID_RAMP_UP, CTR_HYBRID_PI };
enum InterfaceStates { IFACE_OFF, IFACE_ON };
enum CtrCycleStates { CYCLE_OFF, CYCLE_ON };
enum EmergencyStates { EMERGENCY_OFF, EMERGENCY_ON };
enum ThermostatSupportedInputSwitches {
  THERMOSTAT_INPUT_NONE,
  THERMOSTAT_INPUT_SWT1 = 1,            // Buttons
  THERMOSTAT_INPUT_SWT2,
  THERMOSTAT_INPUT_SWT3,
  THERMOSTAT_INPUT_SWT4
};
enum ThermostatSupportedOutputRelays {
  THERMOSTAT_OUTPUT_NONE,
  THERMOSTAT_OUTPUT_REL1 = 1,           // Relays
  THERMOSTAT_OUTPUT_REL2,
  THERMOSTAT_OUTPUT_REL3,
  THERMOSTAT_OUTPUT_REL4,
  THERMOSTAT_OUTPUT_REL5,
  THERMOSTAT_OUTPUT_REL6,
  THERMOSTAT_OUTPUT_REL7,
  THERMOSTAT_OUTPUT_REL8
};

typedef union {
  uint16_t data;
  struct {
    uint16_t thermostat_mode : 2;       // Operation mode of the thermostat system
    uint16_t controller_mode : 2;       // Operation mode of the thermostat controller
    uint16_t sensor_alive : 1;          // Flag stating if temperature sensor is alive (0 = inactive, 1 = active)
    uint16_t command_output : 1;        // Flag stating state to save the command to the output (0 = inactive, 1 = active)
    uint16_t phase_hybrid_ctr : 1;      // Phase of the hybrid controller (Ramp-up or PI)
    uint16_t status_output : 1;         // Status of the output switch
    uint16_t status_cycle_active : 1;   // Status showing if cycle is active (Output ON) or not (Output OFF)
    uint16_t state_emergency : 1;       // State for thermostat emergency
    uint16_t counter_seconds : 6;       // Second counter used to track minutes
  };
} ThermostatBitfield;

#ifdef DEBUG_THERMOSTAT
const char DOMOTICZ_MES[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}";
#endif

const char kThermostatCommands[] PROGMEM = "|" D_CMND_THERMOSTATMODESET "|" D_CMND_TEMPFROSTPROTECTSET "|" 
  D_CMND_CONTROLLERMODESET "|" D_CMND_INPUTSWITCHSET "|" D_CMND_OUTPUTRELAYSET "|" D_CMND_TIMEALLOWRAMPUPSET "|" 
  D_CMND_TEMPMEASUREDSET "|" D_CMND_TEMPTARGETSET "|" D_CMND_TEMPTARGETREAD "|" 
  D_CMND_TEMPMEASUREDREAD "|" D_CMND_TEMPMEASUREDGRDREAD "|" D_CMND_TEMPSENSNUMBERSET "|" 
  D_CMND_STATEEMERGENCYSET "|" D_CMND_POWERMAXSET "|" D_CMND_TIMEMANUALTOAUTOSET "|" D_CMND_TIMEONLIMITSET "|" 
  D_CMND_PROPBANDSET "|" D_CMND_TIMERESETSET "|" D_CMND_TIMEPICYCLESET "|" D_CMND_TEMPANTIWINDUPRESETSET "|" 
  D_CMND_TEMPHYSTSET "|" D_CMND_TIMEMAXACTIONSET "|" D_CMND_TIMEMINACTIONSET "|" D_CMND_TIMEMINTURNOFFACTIONSET "|"
  D_CMND_TEMPRUPDELTINSET "|" D_CMND_TEMPRUPDELTOUTSET "|" D_CMND_TIMERAMPUPMAXSET "|" D_CMND_TIMERAMPUPCYCLESET  "|" 
  D_CMND_TEMPRAMPUPPIACCERRSET "|" D_CMND_TIMEPIPROPORTREAD "|" D_CMND_TIMEPIINTEGRREAD "|" D_CMND_TIMESENSLOSTSET;

void (* const ThermostatCommand[])(void) PROGMEM = {
  &CmndThermostatModeSet, &CmndTempFrostProtectSet, &CmndControllerModeSet, &CmndInputSwitchSet, &CmndOutputRelaySet, 
  &CmndTimeAllowRampupSet, &CmndTempMeasuredSet, &CmndTempTargetSet, &CmndTempTargetRead, 
  &CmndTempMeasuredRead, &CmndTempMeasuredGrdRead, &CmndTempSensNumberSet, &CmndStateEmergencySet, 
  &CmndPowerMaxSet, &CmndTimeManualToAutoSet, &CmndTimeOnLimitSet, &CmndPropBandSet, &CmndTimeResetSet, 
  &CmndTimePiCycleSet, &CmndTempAntiWindupResetSet, &CmndTempHystSet, &CmndTimeMaxActionSet, 
  &CmndTimeMinActionSet, &CmndTimeMinTurnoffActionSet, &CmndTempRupDeltInSet, &CmndTempRupDeltOutSet,
  &CmndTimeRampupMaxSet, &CmndTimeRampupCycleSet, &CmndTempRampupPiAccErrSet, &CmndTimePiProportRead, 
  &CmndTimePiIntegrRead, &CmndTimeSensLostSet };

struct THERMOSTAT {
  uint32_t timestamp_temp_measured_update = 0;                                // Timestamp of latest measurement update
  uint32_t timestamp_temp_meas_change_update = 0;                             // Timestamp of latest measurement value change (> or < to previous)
  uint32_t timestamp_output_off = 0;                                          // Timestamp of latest thermostat output Off state
  uint32_t timestamp_input_on = 0;                                            // Timestamp of latest input On state
  uint32_t time_thermostat_total = 0;                                         // Time thermostat on within a specific timeframe
  uint32_t time_ctr_checkpoint = 0;                                           // Time to finalize the control cycle within the PI strategy or to switch to PI from Rampup
  uint32_t time_ctr_changepoint = 0;                                          // Time until switching off output within the controller
  int32_t temp_measured_gradient = 0;                                         // Temperature measured gradient from sensor in thousandths of degrees per hour
  int16_t temp_target_level = THERMOSTAT_TEMP_INIT;                          // Target level of the thermostat in tenths of degrees
  int16_t temp_target_level_ctr = THERMOSTAT_TEMP_INIT;                      // Target level set for the controller
  int16_t temp_pi_accum_error = 0;                                            // Temperature accumulated error for the PI controller in tenths of degrees
  int16_t temp_pi_error = 0;                                                  // Temperature error for the PI controller in tenths of degrees
  int32_t time_proportional_pi;                                               // Time proportional part of the PI controller
  int32_t time_integral_pi;                                                   // Time integral part of the PI controller
  int32_t time_total_pi;                                                      // Time total (proportional + integral) of the PI controller
  uint16_t kP_pi = 0;                                                         // kP value for the PI controller
  uint16_t kI_pi = 0;                                                         // kP value for the PI controller multiplied by 100
  int32_t temp_rampup_meas_gradient = 0;                                      // Temperature measured gradient from sensor in thousandths of degrees per hour calculated during ramp-up
  uint32_t timestamp_rampup_start = 0;                                        // Timestamp where the ramp-up controller mode has been started
  uint32_t time_rampup_deadtime = 0;                                          // Time constant of the thermostat system (step response time)
  uint32_t time_rampup_nextcycle = 0;                                         // Time where the ramp-up controller shall start the next cycle
  int16_t temp_measured = 0;                                                  // Temperature measurement received from sensor in tenths of degrees
  uint8_t time_output_delay = THERMOSTAT_TIME_OUTPUT_DELAY;                   // Output delay between state change and real actuation event (f.i. valve open/closed)
  uint8_t  counter_rampup_cycles = 0;                                         // Counter of ramp-up cycles
  uint8_t output_relay_number = THERMOSTAT_RELAY_NUMBER;                      // Output relay number
  uint8_t input_switch_number = THERMOSTAT_SWITCH_NUMBER;                     // Input switch number
  uint8_t temp_sens_number = THERMOSTAT_TEMP_SENS_NUMBER;                     // Temperature sensor number
  uint8_t temp_rampup_pi_acc_error = THERMOSTAT_TEMP_PI_RAMPUP_ACC_E;         // Accumulated error when switching from ramp-up controller to PI
  uint8_t temp_rampup_delta_out = THERMOSTAT_TEMP_RAMPUP_DELTA_OUT;           // Minimum delta temperature to target to get out of the rampup mode, in tenths of degrees celsius
  uint8_t temp_rampup_delta_in = THERMOSTAT_TEMP_RAMPUP_DELTA_IN;             // Minimum delta temperature to target to get into rampup mode, in tenths of degrees celsius
  int16_t temp_rampup_output_off = 0;                                         // Temperature to swith off relay output within the ramp-up controller in tenths of degrees
  int16_t temp_rampup_start = 0;                                              // Temperature at start of ramp-up controller in tenths of degrees celsius
  int16_t temp_rampup_cycle = 0;                                              // Temperature set at the beginning of each ramp-up cycle in tenths of degrees
  uint16_t time_rampup_max = THERMOSTAT_TIME_RAMPUP_MAX;                      // Time maximum ramp-up controller duration in minutes
  uint16_t time_rampup_cycle = THERMOSTAT_TIME_RAMPUP_CYCLE;                  // Time ramp-up cycle in seconds
  uint16_t time_allow_rampup = THERMOSTAT_TIME_ALLOW_RAMPUP;                  // Time in minutes after last target update to allow ramp-up controller phase
  uint16_t time_sens_lost = THERMOSTAT_TIME_SENS_LOST;                        // Maximum time w/o sensor update to set it as lost
  uint16_t time_manual_to_auto = THERMOSTAT_TIME_MANUAL_TO_AUTO;              // Time without input switch active to change from manual to automatic in minutes
  uint16_t time_on_limit = THERMOSTAT_TIME_ON_LIMIT;                          // Maximum time with output active in minutes
  uint32_t time_reset = THERMOSTAT_TIME_RESET;                                // Reset time of the PI controller in seconds
  uint16_t time_pi_cycle = THERMOSTAT_TIME_PI_CYCLE;                          // Cycle time for the thermostat controller in seconds
  uint16_t time_max_action = THERMOSTAT_TIME_MAX_ACTION;                      // Maximum thermostat time per cycle in minutes
  uint16_t time_min_action = THERMOSTAT_TIME_MIN_ACTION;                      // Minimum thermostat time per cycle in minutes
  uint16_t time_min_turnoff_action = THERMOSTAT_TIME_MIN_TURNOFF_ACTION;      // Minimum turnoff time in minutes, below it the thermostat will be held on
  uint8_t val_prop_band = THERMOSTAT_PROP_BAND;                               // Proportional band of the PI controller in degrees celsius
  uint8_t temp_reset_anti_windup = THERMOSTAT_TEMP_RESET_ANTI_WINDUP;         // Range where reset antiwindup is disabled, in tenths of degrees celsius
  int8_t temp_hysteresis = THERMOSTAT_TEMP_HYSTERESIS;                        // Range hysteresis for temperature PI controller, in tenths of degrees celsius
  uint8_t temp_frost_protect = THERMOSTAT_TEMP_FROST_PROTECT;                 // Minimum temperature for frost protection, in tenths of degrees celsius
  uint16_t power_max = THERMOSTAT_POWER_MAX;                                  // Maximum output power in Watt
  ThermostatBitfield status;                                                  // Bittfield including states as well as several flags
} Thermostat;

/*********************************************************************************************/

void ThermostatInit()
{
  ExecuteCommandPower(Thermostat.output_relay_number, POWER_OFF, SRC_THERMOSTAT); // Make sure the Output is OFF
  // Init Thermostat.status bitfield:
  Thermostat.status.thermostat_mode = THERMOSTAT_OFF;
  Thermostat.status.controller_mode = CTR_HYBRID;
  Thermostat.status.sensor_alive = IFACE_OFF;
  Thermostat.status.command_output = IFACE_OFF;
  Thermostat.status.phase_hybrid_ctr = CTR_HYBRID_PI;
  Thermostat.status.status_output = IFACE_OFF;
  Thermostat.status.status_cycle_active = CYCLE_OFF;
  Thermostat.status.state_emergency = EMERGENCY_OFF;
  Thermostat.status.counter_seconds = 0;
}

bool ThermostatMinuteCounter() 
{
  bool result = false;
  Thermostat.status.counter_seconds++;    // increment time
  
  if ((Thermostat.status.counter_seconds % 60) == 0) {
    result = true; 
    Thermostat.status.counter_seconds = 0;   
  }
  return result;
}

inline bool ThermostatSwitchIdValid(uint8_t switchId) 
{
  return (switchId >= THERMOSTAT_INPUT_SWT1 && switchId <= THERMOSTAT_INPUT_SWT4);
}

inline bool ThermostatRelayIdValid(uint8_t relayId) 
{
  return (relayId >= THERMOSTAT_OUTPUT_REL1 && relayId <= THERMOSTAT_OUTPUT_REL8);
}

uint8_t ThermostatSwitchStatus(uint8_t input_switch)
{
  bool ifId = ThermostatSwitchIdValid(input_switch);
  if(ifId) {
    return(SwitchGetVirtual(ifId - THERMOSTAT_INPUT_SWT1));
  }
  else return 255;
}

void ThermostatSignalProcessingSlow()
{
  if ((uptime - Thermostat.timestamp_temp_measured_update) > ((uint32_t)Thermostat.time_sens_lost * 60)) { // Check if sensor alive
    Thermostat.status.sensor_alive = IFACE_OFF;
    Thermostat.temp_measured_gradient = 0;
    Thermostat.temp_measured = 0;
  }
}

void ThermostatSignalProcessingFast()
{
  if (ThermostatSwitchStatus(Thermostat.input_switch_number)) { // Check if input switch active and register last update
    Thermostat.timestamp_input_on = uptime;
  }
}

void ThermostatCtrState()
{
  switch (Thermostat.status.controller_mode) {
    case CTR_HYBRID:                    // Hybrid controller (Ramp-up + PI)
        ThermostatHybridCtrPhase();
      break;
    case CTR_PI:                        // PI controller
      break;
    case CTR_RAMP_UP:                   // Ramp-up controller (predictive)
      break;
  }
}

void ThermostatHybridCtrPhase()
{
  if (Thermostat.status.controller_mode == CTR_HYBRID) {
    switch (Thermostat.status.phase_hybrid_ctr) {
      case CTR_HYBRID_RAMP_UP:                    // Ramp-up phase with gradient control
          // If ramp-up offtime counter has been initalized    
          // AND ramp-up offtime counter value reached
          if((Thermostat.time_ctr_checkpoint != 0) 
            && (uptime >= Thermostat.time_ctr_checkpoint)) {
            // Reset pause period
            Thermostat.time_ctr_checkpoint = 0;
            // Reset timers
            Thermostat.time_ctr_changepoint = 0;
            // Set PI controller
            Thermostat.status.phase_hybrid_ctr = CTR_HYBRID_PI;
          }
        break;
      case CTR_HYBRID_PI:                         // PI controller phase
          // If no output action for a pre-defined time
          // AND temp target has changed
          // AND temp target - target actual bigger than threshold
          // then go to ramp-up
          if (((uptime - Thermostat.timestamp_output_off) > (60 * (uint32_t)Thermostat.time_allow_rampup))
            && (Thermostat.temp_target_level != Thermostat.temp_target_level_ctr)
            &&((Thermostat.temp_target_level - Thermostat.temp_measured) > Thermostat.temp_rampup_delta_in)) {
              Thermostat.timestamp_rampup_start = uptime;
              Thermostat.temp_rampup_start = Thermostat.temp_measured;
              Thermostat.temp_rampup_meas_gradient = 0;
              Thermostat.time_rampup_deadtime = 0;
              Thermostat.counter_rampup_cycles = 1;
              Thermostat.time_ctr_changepoint = 0;
              Thermostat.time_ctr_checkpoint = 0;
              Thermostat.status.phase_hybrid_ctr = CTR_HYBRID_RAMP_UP;
          }
        break;
    }
  }
#ifdef DEBUG_THERMOSTAT
  ThermostatVirtualSwitchCtrState();
#endif
}

bool HeatStateAutoToManual()
{
  bool change_state = false;

  // If switch input is active
  // OR temperature sensor is not alive
  // then go to manual
  if ((ThermostatSwitchStatus(Thermostat.input_switch_number) == 1)
    || (Thermostat.status.sensor_alive == IFACE_OFF)) {
    change_state = true;
  }
  return change_state;
}

bool HeatStateManualToAuto()
{
  bool change_state;

  // If switch input inactive
  // AND sensor alive
  // AND no switch input action (time in current state) bigger than a pre-defined time
  // then go to automatic
  if ((ThermostatSwitchStatus(Thermostat.input_switch_number) == 0) 
    &&(Thermostat.status.sensor_alive ==  IFACE_ON)
    && ((uptime - Thermostat.timestamp_input_on) > ((uint32_t)Thermostat.time_manual_to_auto * 60))) {
    change_state = true;
  }
  return change_state;
}

bool HeatStateAllToOff()
{
  bool change_state;

  // If emergency mode then switch OFF the output inmediately
  if (Thermostat.status.state_emergency == EMERGENCY_ON) {
    Thermostat.status.thermostat_mode = THERMOSTAT_OFF;  // Emergency switch to THERMOSTAT_OFF
  }
  return change_state;
}

void ThermostatState()
{
  switch (Thermostat.status.thermostat_mode) {
    case THERMOSTAT_OFF:                              // State if Off or Emergency
      // No change of state possible without external command
      break;
    case THERMOSTAT_AUTOMATIC_OP:                     // State automatic thermostat active following to command target temp.
      if (HeatStateAllToOff()) {
        Thermostat.status.thermostat_mode = THERMOSTAT_OFF;           // Emergency switch to THERMOSTAT_OFF
      }
      if (HeatStateAutoToManual()) {
        Thermostat.status.thermostat_mode = THERMOSTAT_MANUAL_OP;     // If sensor not alive change to THERMOSTAT_MANUAL_OP
      }
      ThermostatCtrState();
      break;
    case THERMOSTAT_MANUAL_OP:                        // State manual operation following input switch
      if (HeatStateAllToOff()) {
        Thermostat.status.thermostat_mode = THERMOSTAT_OFF;           // Emergency switch to THERMOSTAT_OFF
      }
      if (HeatStateManualToAuto()) {
        Thermostat.status.thermostat_mode = THERMOSTAT_AUTOMATIC_OP;  // Input switch inactive and timeout reached change to THERMOSTAT_AUTOMATIC_OP
      }
      break;
  }
}

void ThermostatOutputRelay(bool active)
{
  // TODO: See if the real output state can be read by f.i. bitRead(power, Thermostat.output_relay_number))
  // If command received to enable output
  // AND current output status is OFF
  // then switch output to ON
  if ((active == true) 
    && (Thermostat.status.status_output == IFACE_OFF)) {
    ExecuteCommandPower(Thermostat.output_relay_number, POWER_ON, SRC_THERMOSTAT);
    Thermostat.status.status_output = IFACE_ON;
#ifdef DEBUG_THERMOSTAT
    ThermostatVirtualSwitch();
#endif
  }
  // If command received to disable output
  // AND current output status is ON
  // then switch output to OFF
  else if ((active == false) && (Thermostat.status.status_output == IFACE_ON)) {
    ExecuteCommandPower(Thermostat.output_relay_number, POWER_OFF, SRC_THERMOSTAT);
    Thermostat.timestamp_output_off = uptime;
    Thermostat.status.status_output = IFACE_OFF;
#ifdef DEBUG_THERMOSTAT
    ThermostatVirtualSwitch();
#endif
  }
}

void ThermostatCalculatePI()
{
  // Calculate error
  Thermostat.temp_pi_error = Thermostat.temp_target_level_ctr - Thermostat.temp_measured;
  // Kp = 100/PI.propBand. PI.propBand(Xp) = Proportional range (4K in 4K/200 controller)
  Thermostat.kP_pi = 100 / (uint16_t)(Thermostat.val_prop_band);
  // Calculate proportional
  Thermostat.time_proportional_pi = ((int32_t)(Thermostat.temp_pi_error * (int16_t)Thermostat.kP_pi) * ((int32_t)Thermostat.time_pi_cycle * 60)) / 1000;

  // Minimum proportional action limiter
  // If proportional action is less than the minimum action time
  // AND proportional > 0
  // then adjust to minimum value
  if ((Thermostat.time_proportional_pi < abs(((int32_t)Thermostat.time_min_action * 60)))
    && (Thermostat.time_proportional_pi > 0)) {
    Thermostat.time_proportional_pi = ((int32_t)Thermostat.time_min_action * 60);
  }
  
  if (Thermostat.time_proportional_pi < 0) {
    Thermostat.time_proportional_pi = 0;
  } 
  else if (Thermostat.time_proportional_pi > ((int32_t)Thermostat.time_pi_cycle * 60)) {
    Thermostat.time_proportional_pi = ((int32_t)Thermostat.time_pi_cycle * 60);
  }

  // Calculate integral
  Thermostat.kI_pi = (uint16_t)(((float)Thermostat.kP_pi * ((float)((uint32_t)Thermostat.time_pi_cycle * 60) / (float)Thermostat.time_reset)) * 100);
  
  // Reset of antiwindup
  // If error does not lay within the integrator scope range, do not use the integral
  // and accumulate error = 0
  if (abs(Thermostat.temp_pi_error) > (int16_t)Thermostat.temp_reset_anti_windup) {
    Thermostat.time_integral_pi = 0;
    Thermostat.temp_pi_accum_error = 0;
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
    if ((Thermostat.temp_pi_error >= 0)
      && (abs(Thermostat.temp_pi_error) <= (int16_t)Thermostat.temp_hysteresis)
      && (Thermostat.temp_measured_gradient > 0)) {
      Thermostat.temp_pi_accum_error += Thermostat.temp_pi_error;
      // Reduce accumulator error 20% in each cycle
      Thermostat.temp_pi_accum_error *= 0.8;
    }
    // If we are over setpoint
    // AND temperature is rising
    else if ((Thermostat.temp_pi_error < 0)
      && (Thermostat.temp_measured_gradient > 0)) {
      Thermostat.temp_pi_accum_error += Thermostat.temp_pi_error;
      // Reduce accumulator error 20% in each cycle
      Thermostat.temp_pi_accum_error *= 0.8;
    }
    else {
      Thermostat.temp_pi_accum_error += Thermostat.temp_pi_error;
    }

    // Limit lower limit of acumErr to 0
    if (Thermostat.temp_pi_accum_error < 0) {
      Thermostat.temp_pi_accum_error = 0;
    }

    // Integral calculation
    Thermostat.time_integral_pi = (((int32_t)Thermostat.temp_pi_accum_error * (int32_t)Thermostat.kI_pi) * (int32_t)((uint32_t)Thermostat.time_pi_cycle * 60)) / 100000;

    // Antiwindup of the integrator
    // If integral calculation is bigger than cycle time, adjust result
    // to the cycle time and error will not be cummulated]]
    if (Thermostat.time_integral_pi > ((uint32_t)Thermostat.time_pi_cycle * 60)) {
      Thermostat.time_integral_pi = ((uint32_t)Thermostat.time_pi_cycle * 60);
    }
  }

  // Calculate output
  Thermostat.time_total_pi = Thermostat.time_proportional_pi + Thermostat.time_integral_pi;
  
  // Antiwindup of the output
  // If result is bigger than cycle time, the result will be adjusted
  // to the cylce time minus safety time and error will not be cummulated]]
  if (Thermostat.time_total_pi >= ((int32_t)Thermostat.time_pi_cycle * 60)) {
    // Limit to cycle time //at least switch down a minimum time
    Thermostat.time_total_pi = ((int32_t)Thermostat.time_pi_cycle * 60);
  }
  else if (Thermostat.time_total_pi < 0) {
    Thermostat.time_total_pi = 0;
  }

  // Target value limiter
  // If target value has been reached or we are over it]]
  if (Thermostat.temp_pi_error <= 0) {
    // If we are over the hysteresis or the gradient is positive
    if ((abs(Thermostat.temp_pi_error) > Thermostat.temp_hysteresis)
      || (Thermostat.temp_measured_gradient >= 0)) {
      Thermostat.time_total_pi = 0;
    }
  } 
  // If target value has not been reached
  // AND we are withinvr the histeresis
  // AND gradient is positive
  // then set value to 0
  else if ((Thermostat.temp_pi_error > 0)
    && (abs(Thermostat.temp_pi_error) <= Thermostat.temp_hysteresis)
    && (Thermostat.temp_measured_gradient > 0)) {
    Thermostat.time_total_pi = 0;
  }

  // Minimum action limiter
  // If result is less than the minimum action time, adjust to minimum value]]
  if ((Thermostat.time_total_pi <= abs(((uint32_t)Thermostat.time_min_action * 60)))
    && (Thermostat.time_total_pi != 0)) {
    Thermostat.time_total_pi = ((int32_t)Thermostat.time_min_action * 60);
  }
  // Maximum action limiter
  // If result is more than the maximum action time, adjust to maximum value]]
  else if (Thermostat.time_total_pi > abs(((int32_t)Thermostat.time_max_action * 60))) {
    Thermostat.time_total_pi = ((int32_t)Thermostat.time_max_action * 60);
  }
  // If switched off less time than safety time, do not switch off
  else if (Thermostat.time_total_pi > (((int32_t)Thermostat.time_pi_cycle * 60) - ((int32_t)Thermostat.time_min_turnoff_action * 60))) {
    Thermostat.time_total_pi = ((int32_t)Thermostat.time_pi_cycle * 60);
  }
  
  // Adjust output switch point
  Thermostat.time_ctr_changepoint = uptime + (uint32_t)Thermostat.time_total_pi;
  // Adjust next cycle point
  Thermostat.time_ctr_checkpoint = uptime + ((uint32_t)Thermostat.time_pi_cycle * 60);
}

void ThermostatWorkAutomaticPI()
{
  char result_chr[FLOATSZ]; // Remove!

  if ((uptime >= Thermostat.time_ctr_checkpoint) 
    || (Thermostat.temp_target_level != Thermostat.temp_target_level_ctr)
    || ((Thermostat.temp_measured < Thermostat.temp_target_level)
        && (Thermostat.temp_measured_gradient < 0)
        && (Thermostat.status.status_cycle_active == CYCLE_OFF))) {
    Thermostat.temp_target_level_ctr = Thermostat.temp_target_level;
    ThermostatCalculatePI();
    // Reset cycle active
    Thermostat.status.status_cycle_active = CYCLE_OFF;
  }
  if (uptime < Thermostat.time_ctr_changepoint) {
    Thermostat.status.status_cycle_active = CYCLE_ON;
    Thermostat.status.command_output = IFACE_ON;
  }
  else {
    Thermostat.status.command_output = IFACE_OFF;
  }
}

void ThermostatWorkAutomaticRampUp()
{
  uint32_t time_in_rampup;
  int16_t temp_delta_rampup;

  // Update timestamp for temperature at start of ramp-up if temperature still dropping
  if (Thermostat.temp_measured < Thermostat.temp_rampup_start) {
    Thermostat.temp_rampup_start = Thermostat.temp_measured;
  }

  // Update time in ramp-up as well as delta temp
  time_in_rampup = uptime - Thermostat.timestamp_rampup_start;
  temp_delta_rampup = Thermostat.temp_measured - Thermostat.temp_rampup_start;
  // Init command output status to true
  Thermostat.status.command_output = IFACE_ON;
  // Update temperature target level for controller
  Thermostat.temp_target_level_ctr = Thermostat.temp_target_level;

  // If time in ramp-up < max time
  // AND temperature measured < target
  if ((time_in_rampup <= (60 * (uint32_t)Thermostat.time_rampup_max))
    && (Thermostat.temp_measured < Thermostat.temp_target_level)) {
    // DEADTIME point reached
    // If temperature measured minus temperature at start of ramp-up >= threshold
    // AND deadtime still 0
    if ((temp_delta_rampup >= Thermostat.temp_rampup_delta_out) 
      && (Thermostat.time_rampup_deadtime == 0)) {
      // Set deadtime, assuming it is half of the time until slope, since thermal inertia of the temp. fall needs to be considered
      // minus open time of the valve (arround 3 minutes). If rise very fast limit it to delay of output valve     
      int32_t time_aux;
      time_aux = ((time_in_rampup / 2) - Thermostat.time_output_delay);
      if (time_aux >= Thermostat.time_output_delay) {
        Thermostat.time_rampup_deadtime = (uint32_t)time_aux;
      }
      else {
        Thermostat.time_rampup_deadtime = Thermostat.time_output_delay;
      }
      // Calculate gradient since start of ramp-up (considering deadtime) in thousandths of º/hour
      Thermostat.temp_rampup_meas_gradient = (int32_t)((360000 * (int32_t)temp_delta_rampup) / (int32_t)time_in_rampup);
      Thermostat.time_rampup_nextcycle = uptime + (uint32_t)Thermostat.time_rampup_cycle;
      // Set auxiliary variables
      Thermostat.temp_rampup_cycle = Thermostat.temp_measured;
      Thermostat.time_ctr_changepoint = uptime + (60 * (uint32_t)Thermostat.time_rampup_max);
      Thermostat.temp_rampup_output_off =  Thermostat.temp_target_level_ctr;
    }
    // Gradient calculation every time_rampup_cycle
    else if ((Thermostat.time_rampup_deadtime > 0) && (uptime >= Thermostat.time_rampup_nextcycle)) {
      // Calculate temp. gradient in º/hour and set again time_rampup_nextcycle and temp_rampup_cycle
      // temp_rampup_meas_gradient = ((3600 * temp_delta_rampup) / (os.time() - time_rampup_nextcycle))
      temp_delta_rampup = Thermostat.temp_measured - Thermostat.temp_rampup_cycle;
      uint32_t time_total_rampup = (uint32_t)Thermostat.time_rampup_cycle * Thermostat.counter_rampup_cycles;
      // Translate into gradient per hour (thousandths of ° per hour)
      Thermostat.temp_rampup_meas_gradient = int32_t((360000 * (int32_t)temp_delta_rampup) / (int32_t)time_total_rampup);
      if (Thermostat.temp_rampup_meas_gradient > 0) {
        // Calculate time to switch Off and come out of ramp-up
        // y-y1 = m(x-x1) -> x = ((y-y1) / m) + x1 -> y1 = temp_rampup_cycle, x1 = (time_rampup_nextcycle - time_rampup_cycle), m = gradient in º/sec
        // Better Alternative -> (y-y1)/(x-x1) = ((y2-y1)/(x2-x1)) -> where y = temp (target) and x = time (to switch off, what its needed)
        // x = ((y-y1)/(y2-y1))*(x2-x1) + x1 - deadtime
        // Thermostat.time_ctr_changepoint = (uint32_t)(((float)(Thermostat.temp_target_level_ctr - Thermostat.temp_rampup_cycle) / (float)temp_delta_rampup) * (float)(time_total_rampup)) + (uint32_t)(Thermostat.time_rampup_nextcycle - (time_total_rampup)) - Thermostat.time_rampup_deadtime;
        Thermostat.time_ctr_changepoint = (uint32_t)(((float)(Thermostat.temp_target_level_ctr - Thermostat.temp_rampup_cycle) * (float)(time_total_rampup)) / (float)temp_delta_rampup) + (uint32_t)(Thermostat.time_rampup_nextcycle - (time_total_rampup)) - Thermostat.time_rampup_deadtime;      
        
        // Calculate temperature for switching off the output
        // y = (((y2-y1)/(x2-x1))*(x-x1)) + y1
        // Thermostat.temp_rampup_output_off = (int16_t)(((float)(temp_delta_rampup) / (float)(time_total_rampup * Thermostat.counter_rampup_cycles)) * (float)(Thermostat.time_ctr_changepoint - (uptime - (time_total_rampup)))) + Thermostat.temp_rampup_cycle;
        Thermostat.temp_rampup_output_off = (int16_t)(((float)temp_delta_rampup * (float)(Thermostat.time_ctr_changepoint - (uptime - (time_total_rampup)))) / (float)(time_total_rampup * Thermostat.counter_rampup_cycles)) + Thermostat.temp_rampup_cycle;
        // Set auxiliary variables
        Thermostat.time_rampup_nextcycle = uptime + (uint32_t)Thermostat.time_rampup_cycle;
        Thermostat.temp_rampup_cycle = Thermostat.temp_measured;
        // Reset period counter
        Thermostat.counter_rampup_cycles = 1;
      }
      else {
        // Increase the period counter
        Thermostat.counter_rampup_cycles++;
        // Set another period
        Thermostat.time_rampup_nextcycle = uptime + (uint32_t)Thermostat.time_rampup_cycle;
        // Reset time_ctr_changepoint and temp_rampup_output_off
        Thermostat.time_ctr_changepoint = uptime + (60 * (uint32_t)Thermostat.time_rampup_max) - time_in_rampup;
        Thermostat.temp_rampup_output_off =  Thermostat.temp_target_level_ctr;
      }
      // Set time to get out of ramp-up
      Thermostat.time_ctr_checkpoint = Thermostat.time_ctr_changepoint + Thermostat.time_rampup_deadtime;
    }

    // Set output switch ON or OFF
    // If deadtime has not been calculated
    // or checkpoint has not been calculated
    // or it is not yet time and temperature to switch it off acc. to calculations
    // or gradient is <= 0
    if ((Thermostat.time_rampup_deadtime == 0)
      || (Thermostat.time_ctr_checkpoint == 0)
      || (uptime < Thermostat.time_ctr_changepoint)
      || (Thermostat.temp_measured < Thermostat.temp_rampup_output_off)
      || (Thermostat.temp_rampup_meas_gradient <= 0)) {
      Thermostat.status.command_output = IFACE_ON;
    }
    else {
      Thermostat.status.command_output = IFACE_OFF;
    }
  }
  else {
    // If we have not reached the temperature, start with an initial value for accumulated error for the PI controller
    if (Thermostat.temp_measured < Thermostat.temp_target_level_ctr) {
      Thermostat.temp_pi_accum_error = Thermostat.temp_rampup_pi_acc_error;
    }
    // Set to now time to get out of ramp-up
    Thermostat.time_ctr_checkpoint = uptime;
    // Switch Off output
    Thermostat.status.command_output = IFACE_OFF;
  }
}

void ThermostatCtrWork()
{
  switch (Thermostat.status.controller_mode) {
    case CTR_HYBRID:            // Hybrid controller (Ramp-up + PI)
      switch (Thermostat.status.phase_hybrid_ctr) {
        case CTR_HYBRID_RAMP_UP:
          ThermostatWorkAutomaticRampUp();
          break;
        case CTR_HYBRID_PI:
          ThermostatWorkAutomaticPI();
          break;
      }
      break;
    case CTR_PI:                // PI controller
      ThermostatWorkAutomaticPI();
      break;
    case CTR_RAMP_UP:           // Ramp-up controller (predictive)
      ThermostatWorkAutomaticRampUp();
      break;
  }
}

void ThermostatWork()
{
  switch (Thermostat.status.thermostat_mode) {
    case THERMOSTAT_OFF:                              // State if Off or Emergency
      Thermostat.status.command_output = IFACE_OFF;
      break;
    case THERMOSTAT_AUTOMATIC_OP:                     // State automatic thermostat active following to command target temp.
      ThermostatCtrWork();
      break;
    case THERMOSTAT_MANUAL_OP:                        // State manual operation following input switch
      Thermostat.time_ctr_checkpoint = 0;
      if (ThermostatSwitchStatus(Thermostat.input_switch_number) == 1) {
        Thermostat.status.command_output = IFACE_ON;
      }
      else {
        Thermostat.status.command_output = IFACE_OFF;
      }      
      break;
  }
  bool output_command;
  if (Thermostat.status.command_output == IFACE_OFF) {
    output_command = false;
  }
  else {
    output_command = true;
  }
  ThermostatOutputRelay(output_command);
}

void ThermostatDiagnostics()
{
  // TODOs: 
  // 1. Check time max for output switch on not exceeded
  // 2. Check state of output corresponds to command
  // 3. Check maximum power at output switch not exceeded
}

void ThermostatController()
{
  ThermostatState();
  ThermostatWork();
}

bool ThermostatTimerArm(int16_t tempVal)
{
  bool result = false;
  // TempVal unit is tenths of degrees celsius
  if ((tempVal >= -1000) 
    && (tempVal <= 1000)
    && (tempVal >= (int16_t)Thermostat.temp_frost_protect)) {
      Thermostat.temp_target_level = tempVal;
      Thermostat.status.thermostat_mode = THERMOSTAT_AUTOMATIC_OP;
      result = true;
  }
  // Returns true if setpoint plausible and thermostat armed, false on the contrary
  return result;
}

void ThermostatTimerDisarm()
{
  Thermostat.temp_target_level = THERMOSTAT_TEMP_INIT;
  Thermostat.status.thermostat_mode = THERMOSTAT_OFF;
}

#ifdef DEBUG_THERMOSTAT
void ThermostatVirtualSwitch()
{
  char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
  Response_P(DOMOTICZ_MES, DOMOTICZ_IDX1, (0 == Thermostat.status.status_output) ? 0 : 1, "");
  MqttPublish(domoticz_in_topic);
}

void ThermostatVirtualSwitchCtrState()
{
  char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
  Response_P(DOMOTICZ_MES, DOMOTICZ_IDX2, (0 == Thermostat.status.phase_hybrid_ctr) ? 0 : 1, "");
  MqttPublish(domoticz_in_topic);

  //Response_P(DOMOTICZ_MES, DOMOTICZ_IDX3, (0 == Thermostat.time_ctr_changepoint) ? 0 : 1, "");
  //MqttPublish(domoticz_in_topic);
}
#endif

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndThermostatModeSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
    if ((value >= THERMOSTAT_OFF) && (value < THERMOSTAT_MODES_MAX)) {
      Thermostat.status.thermostat_mode = value;
      Thermostat.timestamp_input_on = 0;     // Reset last manual switch timer if command set externally
    }
  }
  ResponseCmndNumber((int)Thermostat.status.thermostat_mode);
}

void CmndTempFrostProtectSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 255)) {
      Thermostat.temp_frost_protect = value;
    }
  }
  ResponseCmndFloat((float)(Thermostat.temp_frost_protect) / 10, 1);
}

void CmndControllerModeSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value < CTR_MODES_MAX)) {
      Thermostat.status.controller_mode = value;
    }
  }
  ResponseCmndNumber((int)Thermostat.status.controller_mode);
}

void CmndInputSwitchSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if (ThermostatSwitchIdValid(value)) {
      Thermostat.input_switch_number = value;
      Thermostat.timestamp_input_on = uptime;
    }
  }
  ResponseCmndNumber((int)Thermostat.input_switch_number);
}

void CmndOutputRelaySet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if (ThermostatRelayIdValid(value)) {
      Thermostat.output_relay_number = value;
    }
  }
  ResponseCmndNumber((int)Thermostat.output_relay_number);
}

void CmndTimeAllowRampupSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value < 86400)) {
      Thermostat.time_allow_rampup = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_allow_rampup * 60));
}

void CmndTempMeasuredSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    int16_t value = (int16_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= -1000) && (value <= 1000)) {
      uint32_t timestamp = uptime;
      // Calculate temperature gradient if temperature value has changed
      if (value != Thermostat.temp_measured) {
        int32_t temp_delta = (value - Thermostat.temp_measured); // in tenths of degrees
        uint32_t time_delta = (timestamp - Thermostat.timestamp_temp_meas_change_update); // in seconds
        Thermostat.temp_measured_gradient = (int32_t)((360000 * temp_delta) / ((int32_t)time_delta)); // hundreths of degrees per hour
        Thermostat.temp_measured = value;
        Thermostat.timestamp_temp_meas_change_update = timestamp;
      }
      Thermostat.timestamp_temp_measured_update = timestamp;
      Thermostat.status.sensor_alive = IFACE_ON;
    }
  }
  ResponseCmndFloat(((float)Thermostat.temp_measured) / 10, 1);
}

void CmndTempTargetSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint16_t value = (uint16_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= -1000) 
      && (value <= 1000)
      && (value >= (int16_t)Thermostat.temp_frost_protect)) {
      Thermostat.temp_target_level = value;
    }
  }
  ResponseCmndFloat(((float)Thermostat.temp_target_level) / 10, 1);
}

void CmndTempTargetRead(void)
{
  ResponseCmndFloat(((float)Thermostat.temp_target_level) / 10, 1);
}

void CmndTempMeasuredRead(void)
{
  ResponseCmndFloat((float)(Thermostat.temp_measured) / 10, 1);
}

void CmndTempMeasuredGrdRead(void)
{
  ResponseCmndFloat((float)(Thermostat.temp_measured_gradient) / 1000, 1);
}

void CmndTempSensNumberSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 255)) {
      Thermostat.temp_sens_number = value;
    }
  }
  ResponseCmndNumber((int)Thermostat.temp_sens_number);
}

void CmndStateEmergencySet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 1)) {
      Thermostat.status.state_emergency = (uint16_t)value;
    }
  }
  ResponseCmndNumber((int)Thermostat.status.state_emergency);
}

void CmndPowerMaxSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint16_t value = (uint16_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 1300)) {
      Thermostat.power_max = value;
    }
  }
  ResponseCmndNumber((int)Thermostat.power_max);
}

void CmndTimeManualToAutoSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_manual_to_auto = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_manual_to_auto * 60));
}

void CmndTimeOnLimitSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_on_limit = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_on_limit * 60));
}

void CmndPropBandSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 20)) {
      Thermostat.val_prop_band = value;
    }
  }
  ResponseCmndNumber((int)Thermostat.val_prop_band);
}

void CmndTimeResetSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_reset = value;
    }
  }
  ResponseCmndNumber((int)Thermostat.time_reset);
}

void CmndTimePiCycleSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_pi_cycle = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_pi_cycle * 60));
}

void CmndTempAntiWindupResetSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= (float)(0)) && (value <= (float)(100.0))) {
      Thermostat.temp_reset_anti_windup = value;
    }
  }
  ResponseCmndFloat((float)(Thermostat.temp_reset_anti_windup) / 10, 1);
}

void CmndTempHystSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    int8_t value = (int8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= -100) && (value <= 100)) {
      Thermostat.temp_hysteresis = value;
    }
  }
  ResponseCmndFloat((float)(Thermostat.temp_hysteresis) / 10, 1);
}

void CmndTimeMaxActionSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_max_action = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_max_action * 60));
}

void CmndTimeMinActionSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_min_action = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_min_action * 60));
}

void CmndTimeSensLostSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_sens_lost = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_sens_lost * 60));
}

void CmndTimeMinTurnoffActionSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_min_turnoff_action = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)((uint32_t)Thermostat.time_min_turnoff_action * 60));
}

void CmndTempRupDeltInSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 100)) {
      Thermostat.temp_rampup_delta_in = value;
    }
  }
  ResponseCmndFloat((float)(Thermostat.temp_rampup_delta_in) / 10, 1);
}

void CmndTempRupDeltOutSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 100)) {
      Thermostat.temp_rampup_delta_out = value;
    }
  }
  ResponseCmndFloat((float)(Thermostat.temp_rampup_delta_out) / 10, 1);
}

void CmndTimeRampupMaxSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 86400)) {
      Thermostat.time_rampup_max = (uint16_t)(value / 60);
    }
  }
  ResponseCmndNumber((int)(((uint32_t)Thermostat.time_rampup_max) * 60));
}

void CmndTimeRampupCycleSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t value = (uint32_t)(XdrvMailbox.payload);
    if ((value >= 0) && (value <= 54000)) {
      Thermostat.time_rampup_cycle = (uint16_t)value;
    }
  }
  ResponseCmndNumber((int)Thermostat.time_rampup_cycle);
}

void CmndTempRampupPiAccErrSet(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value >= 0) && (value <= 250)) {
      Thermostat.temp_rampup_pi_acc_error = value;
    }
  }
  ResponseCmndFloat((float)(Thermostat.temp_rampup_pi_acc_error) / 10, 1);
}

void CmndTimePiProportRead(void)
{
  ResponseCmndNumber((int)Thermostat.time_proportional_pi);
}

void CmndTimePiIntegrRead(void)
{
  ResponseCmndNumber((int)Thermostat.time_integral_pi);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv39(uint8_t function)
{
#ifdef DEBUG_THERMOSTAT
  char result_chr[FLOATSZ];
#endif
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      ThermostatInit();
      break;
    case FUNC_LOOP:
      ThermostatSignalProcessingFast();
      ThermostatDiagnostics();
      break;
    case FUNC_SERIAL:
      break;
    case FUNC_EVERY_SECOND:
      if (ThermostatMinuteCounter()) {
        ThermostatSignalProcessingSlow();
        ThermostatController();
#ifdef DEBUG_THERMOSTAT
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(""));
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("------ Thermostat Start ------"));
        dtostrfd(Thermostat.status.counter_seconds, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.counter_seconds: %s"), result_chr);
        dtostrfd(Thermostat.status.thermostat_mode, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.thermostat_mode: %s"), result_chr);
        dtostrfd(Thermostat.status.controller_mode, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.controller_mode: %s"), result_chr);
        dtostrfd(Thermostat.status.phase_hybrid_ctr, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.phase_hybrid_ctr: %s"), result_chr);
        dtostrfd(Thermostat.status.sensor_alive, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.sensor_alive: %s"), result_chr);
        dtostrfd(Thermostat.status.status_output, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.status_output: %s"), result_chr);
        dtostrfd(Thermostat.status.status_cycle_active, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.status.status_cycle_active: %s"), result_chr);   
        dtostrfd(Thermostat.temp_pi_error, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.temp_pi_error: %s"), result_chr);
        dtostrfd(Thermostat.temp_pi_accum_error, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.temp_pi_accum_error: %s"), result_chr);
        dtostrfd(Thermostat.time_proportional_pi, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.time_proportional_pi: %s"), result_chr);
        dtostrfd(Thermostat.time_integral_pi, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.time_integral_pi: %s"), result_chr);
        dtostrfd(Thermostat.time_total_pi, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.time_total_pi: %s"), result_chr);
        dtostrfd(Thermostat.temp_measured_gradient, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.temp_measured_gradient: %s"), result_chr);
        dtostrfd(Thermostat.time_rampup_deadtime, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.time_rampup_deadtime: %s"), result_chr);
        dtostrfd(Thermostat.temp_rampup_meas_gradient, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.temp_rampup_meas_gradient: %s"), result_chr);
        dtostrfd(Thermostat.time_ctr_changepoint, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.time_ctr_changepoint: %s"), result_chr);
        dtostrfd(Thermostat.temp_rampup_output_off, 0, result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat.temp_rampup_output_off: %s"), result_chr);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("------ Thermostat End ------"));
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(""));
#endif
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kThermostatCommands, ThermostatCommand);
      break;
  }
  return result;
}

#endif // USE_THERMOSTAT
