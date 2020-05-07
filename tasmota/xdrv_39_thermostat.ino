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
#define DOMOTICZ_MAX_IDX     4
#define DOMOTICZ_IDX1        791
#define DOMOTICZ_IDX2        792
#define DOMOTICZ_IDX3        799
#define DOMOTICZ_IDX4        800
#define DOMOTICZ_IDX5        801
#endif // DEBUG_THERMOSTAT

// Commands
#define D_CMND_THERMOSTATMODESET "ThermostatModeSet"
#define D_CMND_CLIMATEMODESET "ClimateModeSet"
#define D_CMND_TEMPFROSTPROTECTSET "TempFrostProtectSet"
#define D_CMND_CONTROLLERMODESET "ControllerModeSet"
#define D_CMND_INPUTSWITCHSET "InputSwitchSet"
#define D_CMND_INPUTSWITCHUSE "InputSwitchUse"
#define D_CMND_OUTPUTRELAYSET "OutputRelaySet"
#define D_CMND_TIMEALLOWRAMPUPSET "TimeAllowRampupSet"
#define D_CMND_TEMPFORMATSET "TempFormatSet"
#define D_CMND_TEMPMEASUREDSET "TempMeasuredSet"
#define D_CMND_TEMPTARGETSET "TempTargetSet"
#define D_CMND_TEMPMEASUREDGRDREAD "TempMeasuredGrdRead"
#define D_CMND_TEMPSENSNUMBERSET "TempSensNumberSet"
#define D_CMND_SENSORINPUTSET "SensorInputSet"
#define D_CMND_STATEEMERGENCYSET "StateEmergencySet"
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
#define D_CMND_DIAGNOSTICMODESET "DiagnosticModeSet"

enum ThermostatModes { THERMOSTAT_OFF, THERMOSTAT_AUTOMATIC_OP, THERMOSTAT_MANUAL_OP, THERMOSTAT_MODES_MAX };
enum ControllerModes { CTR_HYBRID, CTR_PI, CTR_RAMP_UP, CTR_MODES_MAX };
enum ControllerHybridPhases { CTR_HYBRID_RAMP_UP, CTR_HYBRID_PI };
enum ClimateModes { CLIMATE_HEATING, CLIMATE_COOLING, CLIMATE_MODES_MAX };
enum InterfaceStates { IFACE_OFF, IFACE_ON };
enum InputUsage { INPUT_NOT_USED, INPUT_USED };
enum CtrCycleStates { CYCLE_OFF, CYCLE_ON };
enum EmergencyStates { EMERGENCY_OFF, EMERGENCY_ON };
enum SensorType { SENSOR_MQTT, SENSOR_LOCAL, SENSOR_MAX };
enum TempFormat { TEMP_CELSIUS, TEMP_FAHRENHEIT };
enum TempConvType { TEMP_CONV_ABSOLUTE, TEMP_CONV_RELATIVE };
enum DiagnosticModes { DIAGNOSTIC_OFF, DIAGNOSTIC_ON };
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
  uint32_t data;
  struct {
    uint32_t thermostat_mode : 2;       // Operation mode of the thermostat system
    uint32_t controller_mode : 2;       // Operation mode of the thermostat controller
    uint32_t climate_mode : 1;          // Climate mode of the thermostat (0 = heating / 1 = cooling)
    uint32_t sensor_alive : 1;          // Flag stating if temperature sensor is alive (0 = inactive, 1 = active)
    uint32_t sensor_type : 1;           // Sensor type: MQTT/local
    uint32_t temp_format : 1;           // Temperature format (0 = Celsius, 1 = Fahrenheit)
    uint32_t command_output : 1;        // Flag stating the desired command to the output (0 = inactive, 1 = active)
    uint32_t status_output : 1;         // Flag stating state of the output (0 = inactive, 1 = active)
    uint32_t status_input : 1;          // Flag stating state of the input (0 = inactive, 1 = active)
    uint32_t use_input : 1;             // Flag stating if the input switch shall be used to switch to manual mode
    uint32_t phase_hybrid_ctr : 1;      // Phase of the hybrid controller (Ramp-up or PI)
    uint32_t status_cycle_active : 1;   // Status showing if cycle is active (Output ON) or not (Output OFF)
    uint32_t state_emergency : 1;       // State for thermostat emergency
    uint32_t counter_seconds : 6;       // Second counter used to track minutes
    uint32_t output_relay_number : 4;   // Output relay number
    uint32_t input_switch_number : 3;   // Input switch number
    uint32_t output_inconsist_ctr : 2;  // Counter of the minutes where the output state is inconsistent with the command
    uint32_t diagnostic_mode : 1;       // Diagnostic mode selected
    uint32_t free : 1;                  // Free bits in Bitfield
  };
} ThermostatBitfield;

#ifdef DEBUG_THERMOSTAT
const char DOMOTICZ_MES[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}";
uint16_t Domoticz_Virtual_Switches[DOMOTICZ_MAX_IDX] = { DOMOTICZ_IDX1, DOMOTICZ_IDX3, DOMOTICZ_IDX4, DOMOTICZ_IDX5 };
#endif // DEBUG_THERMOSTAT

const char kThermostatCommands[] PROGMEM = "|" D_CMND_THERMOSTATMODESET "|" D_CMND_CLIMATEMODESET "|" 
  D_CMND_TEMPFROSTPROTECTSET "|" D_CMND_CONTROLLERMODESET "|" D_CMND_INPUTSWITCHSET "|" D_CMND_INPUTSWITCHUSE "|" 
  D_CMND_OUTPUTRELAYSET "|" D_CMND_TIMEALLOWRAMPUPSET "|" D_CMND_TEMPFORMATSET "|" D_CMND_TEMPMEASUREDSET "|" 
  D_CMND_TEMPTARGETSET "|" D_CMND_TEMPMEASUREDGRDREAD "|" D_CMND_SENSORINPUTSET "|" D_CMND_STATEEMERGENCYSET "|" 
  D_CMND_TIMEMANUALTOAUTOSET "|" D_CMND_TIMEONLIMITSET "|" D_CMND_PROPBANDSET "|" D_CMND_TIMERESETSET "|" 
  D_CMND_TIMEPICYCLESET "|" D_CMND_TEMPANTIWINDUPRESETSET "|" D_CMND_TEMPHYSTSET "|" D_CMND_TIMEMAXACTIONSET "|" 
  D_CMND_TIMEMINACTIONSET "|" D_CMND_TIMEMINTURNOFFACTIONSET "|" D_CMND_TEMPRUPDELTINSET "|" D_CMND_TEMPRUPDELTOUTSET "|" 
  D_CMND_TIMERAMPUPMAXSET "|" D_CMND_TIMERAMPUPCYCLESET "|" D_CMND_TEMPRAMPUPPIACCERRSET "|" D_CMND_TIMEPIPROPORTREAD "|" 
  D_CMND_TIMEPIINTEGRREAD "|" D_CMND_TIMESENSLOSTSET "|" D_CMND_DIAGNOSTICMODESET;

void (* const ThermostatCommand[])(void) PROGMEM = {
  &CmndThermostatModeSet, &CmndClimateModeSet, &CmndTempFrostProtectSet, &CmndControllerModeSet, &CmndInputSwitchSet, 
  &CmndInputSwitchUse, &CmndOutputRelaySet, &CmndTimeAllowRampupSet, &CmndTempFormatSet, &CmndTempMeasuredSet, 
  &CmndTempTargetSet, &CmndTempMeasuredGrdRead, &CmndSensorInputSet, &CmndStateEmergencySet, &CmndTimeManualToAutoSet, 
  &CmndTimeOnLimitSet, &CmndPropBandSet, &CmndTimeResetSet, &CmndTimePiCycleSet, &CmndTempAntiWindupResetSet, 
  &CmndTempHystSet, &CmndTimeMaxActionSet, &CmndTimeMinActionSet, &CmndTimeMinTurnoffActionSet, &CmndTempRupDeltInSet, 
  &CmndTempRupDeltOutSet, &CmndTimeRampupMaxSet, &CmndTimeRampupCycleSet, &CmndTempRampupPiAccErrSet, 
  &CmndTimePiProportRead, &CmndTimePiIntegrRead, &CmndTimeSensLostSet, &CmndDiagnosticModeSet };

struct THERMOSTAT {
  ThermostatBitfield status;                                                  // Bittfield including states as well as several flags
  uint32_t timestamp_temp_measured_update = 0;                                // Timestamp of latest measurement update
  uint32_t timestamp_temp_meas_change_update = 0;                             // Timestamp of latest measurement value change (> or < to previous)
  uint32_t timestamp_output_off = 0;                                          // Timestamp of latest thermostat output Off state
  uint32_t timestamp_input_on = 0;                                            // Timestamp of latest input On state
  uint32_t time_thermostat_total = 0;                                         // Time thermostat on within a specific timeframe
  uint32_t time_ctr_checkpoint = 0;                                           // Time to finalize the control cycle within the PI strategy or to switch to PI from Rampup
  uint32_t time_ctr_changepoint = 0;                                          // Time until switching off output within the controller in seconds
  int32_t temp_measured_gradient = 0;                                         // Temperature measured gradient from sensor in thousandths of degrees per hour
  int16_t temp_target_level = THERMOSTAT_TEMP_INIT;                           // Target level of the thermostat in tenths of degrees
  int16_t temp_target_level_ctr = THERMOSTAT_TEMP_INIT;                       // Target level set for the controller
  int16_t temp_pi_accum_error = 0;                                            // Temperature accumulated error for the PI controller in hundredths of degrees
  int16_t temp_pi_error = 0;                                                  // Temperature error for the PI controller in hundredths of degrees
  int32_t time_proportional_pi;                                               // Time proportional part of the PI controller
  int32_t time_integral_pi;                                                   // Time integral part of the PI controller
  int32_t time_total_pi;                                                      // Time total (proportional + integral) of the PI controller
  uint16_t kP_pi = 0;                                                         // kP value for the PI controller multiplied by 100 (to avoid floating point operations)
  uint16_t kI_pi = 0;                                                         // kP value for the PI controller multiplied by 100 (to avoid floating point operations)
  int32_t temp_rampup_meas_gradient = 0;                                      // Temperature measured gradient from sensor in thousandths of degrees celsius per hour calculated during ramp-up
  uint32_t timestamp_rampup_start = 0;                                        // Timestamp where the ramp-up controller mode has been started
  uint32_t time_rampup_deadtime = 0;                                          // Time constant of the thermostat system (step response time)
  uint32_t time_rampup_nextcycle = 0;                                         // Time where the ramp-up controller shall start the next cycle
  int16_t temp_measured = 0;                                                  // Temperature measurement received from sensor in tenths of degrees celsius
  int16_t temp_rampup_output_off = 0;                                         // Temperature to swith off relay output within the ramp-up controller in tenths of degrees celsius
  uint8_t time_output_delay = THERMOSTAT_TIME_OUTPUT_DELAY;                   // Output delay between state change and real actuation event (f.i. valve open/closed)
  uint8_t counter_rampup_cycles = 0;                                          // Counter of ramp-up cycles
  uint8_t temp_rampup_pi_acc_error = THERMOSTAT_TEMP_PI_RAMPUP_ACC_E;         // Accumulated error when switching from ramp-up controller to PI in hundreths of degrees celsius
  uint8_t temp_rampup_delta_out = THERMOSTAT_TEMP_RAMPUP_DELTA_OUT;           // Minimum delta temperature to target to get out of the rampup mode, in tenths of degrees celsius
  uint8_t temp_rampup_delta_in = THERMOSTAT_TEMP_RAMPUP_DELTA_IN;             // Minimum delta temperature to target to get into rampup mode, in tenths of degrees celsius
  uint8_t val_prop_band = THERMOSTAT_PROP_BAND;                               // Proportional band of the PI controller in degrees celsius
  int16_t temp_rampup_start = 0;                                              // Temperature at start of ramp-up controller in tenths of degrees celsius
  int16_t temp_rampup_cycle = 0;                                              // Temperature set at the beginning of each ramp-up cycle in tenths of degrees
  uint16_t time_rampup_max = THERMOSTAT_TIME_RAMPUP_MAX;                      // Time maximum ramp-up controller duration in minutes
  uint16_t time_rampup_cycle = THERMOSTAT_TIME_RAMPUP_CYCLE;                  // Time ramp-up cycle in seconds
  uint16_t time_allow_rampup = THERMOSTAT_TIME_ALLOW_RAMPUP;                  // Time in minutes after last target update to allow ramp-up controller phase
  uint16_t time_sens_lost = THERMOSTAT_TIME_SENS_LOST;                        // Maximum time w/o sensor update to set it as lost
  uint16_t time_manual_to_auto = THERMOSTAT_TIME_MANUAL_TO_AUTO;              // Time without input switch active to change from manual to automatic in minutes
  uint16_t time_on_limit = THERMOSTAT_TIME_ON_LIMIT;                          // Maximum time with output active in minutes
  uint16_t time_pi_cycle = THERMOSTAT_TIME_PI_CYCLE;                          // Cycle time for the thermostat controller in seconds
  uint32_t time_reset = THERMOSTAT_TIME_RESET;                                // Reset time of the PI controller in seconds
  uint16_t time_max_action = THERMOSTAT_TIME_MAX_ACTION;                      // Maximum thermostat time per cycle in minutes
  uint16_t time_min_action = THERMOSTAT_TIME_MIN_ACTION;                      // Minimum thermostat time per cycle in minutes
  uint16_t time_min_turnoff_action = THERMOSTAT_TIME_MIN_TURNOFF_ACTION;      // Minimum turnoff time in minutes, below it the thermostat will be held on
  uint8_t temp_reset_anti_windup = THERMOSTAT_TEMP_RESET_ANTI_WINDUP;         // Range where reset antiwindup is disabled, in tenths of degrees celsius
  int8_t temp_hysteresis = THERMOSTAT_TEMP_HYSTERESIS;                        // Range hysteresis for temperature PI controller, in tenths of degrees celsius
  uint8_t temp_frost_protect = THERMOSTAT_TEMP_FROST_PROTECT;                 // Minimum temperature for frost protection, in tenths of degrees celsius
} Thermostat[THERMOSTAT_CONTROLLER_OUTPUTS];

/*********************************************************************************************/

void ThermostatInit(uint8_t ctr_output)
{
  // Init Thermostat[ctr_output].status bitfield:
  Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_OFF;
  Thermostat[ctr_output].status.controller_mode = CTR_HYBRID;
  Thermostat[ctr_output].status.climate_mode = CLIMATE_HEATING;
  Thermostat[ctr_output].status.sensor_alive = IFACE_OFF;
  Thermostat[ctr_output].status.sensor_type = SENSOR_MQTT;
  Thermostat[ctr_output].status.temp_format = TEMP_CELSIUS;
  Thermostat[ctr_output].status.command_output = IFACE_OFF;
  Thermostat[ctr_output].status.status_output = IFACE_OFF;
  Thermostat[ctr_output].status.phase_hybrid_ctr = CTR_HYBRID_PI;
  Thermostat[ctr_output].status.status_cycle_active = CYCLE_OFF;
  Thermostat[ctr_output].status.state_emergency = EMERGENCY_OFF;
  Thermostat[ctr_output].status.counter_seconds = 0;
  Thermostat[ctr_output].status.output_relay_number = (THERMOSTAT_RELAY_NUMBER + ctr_output);
  Thermostat[ctr_output].status.input_switch_number = (THERMOSTAT_SWITCH_NUMBER + ctr_output);
  Thermostat[ctr_output].status.use_input = INPUT_NOT_USED;
  Thermostat[ctr_output].status.output_inconsist_ctr = 0;
  Thermostat[ctr_output].status.diagnostic_mode = DIAGNOSTIC_ON;
  // Make sure the Output is OFF
  ExecuteCommandPower(Thermostat[ctr_output].status.output_relay_number, POWER_OFF, SRC_THERMOSTAT);
}

bool ThermostatMinuteCounter(uint8_t ctr_output) 
{
  bool result = false;
  Thermostat[ctr_output].status.counter_seconds++;    // increment time
  
  if ((Thermostat[ctr_output].status.counter_seconds % 60) == 0) {
    result = true; 
    Thermostat[ctr_output].status.counter_seconds = 0;
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

uint8_t ThermostatInputStatus(uint8_t input_switch)
{
  bool ifId = ThermostatSwitchIdValid(input_switch);
  uint8_t value = 0;
  if(ifId) {
    value = SwitchGetVirtual(ifId - THERMOSTAT_INPUT_SWT1);
  }
  return value;
}

uint8_t ThermostatOutputStatus(uint8_t output_switch)
{
  return (uint8_t)bitRead(power, (output_switch - 1));
}

int16_t ThermostatCelsiusToFahrenheit(const int32_t deg, uint8_t conv_type) { 
  int32_t value;
  value = (int32_t)(((int32_t)deg * (int32_t)90) / (int32_t)50);
  if (conv_type == TEMP_CONV_ABSOLUTE) {
    value += (int32_t)320;
  }

  // Protect overflow
  if (value <= (int32_t)(INT16_MIN)) {
    value = (int32_t)(INT16_MIN);
  }
  else if (value >= (int32_t)INT16_MAX) {
    value = (int32_t)INT16_MAX;
  }

  return (int16_t)value;
}

int16_t ThermostatFahrenheitToCelsius(const int32_t deg, uint8_t conv_type) { 
  int16_t offset = 0;
  int32_t value;
  if (conv_type == TEMP_CONV_ABSOLUTE) {
    offset = 320;
  }

  value = (int32_t)(((deg - (int32_t)offset) * (int32_t)50) / (int32_t)90);

  // Protect overflow
  if (value <= (int32_t)(INT16_MIN)) {
    value = (int32_t)(INT16_MIN);
  }
  else if (value >= (int32_t)INT16_MAX) {
    value = (int32_t)INT16_MAX;
  }

  return (int16_t)value;
}

void ThermostatSignalPreProcessingSlow(uint8_t ctr_output)
{
  // Update input sensor status
  if ((uptime - Thermostat[ctr_output].timestamp_temp_measured_update) > ((uint32_t)Thermostat[ctr_output].time_sens_lost * 60)) {
    Thermostat[ctr_output].status.sensor_alive = IFACE_OFF;
    Thermostat[ctr_output].temp_measured_gradient = 0;
    Thermostat[ctr_output].temp_measured = 0;
  }
}

void ThermostatSignalPostProcessingSlow(uint8_t ctr_output)
{
  // Increate counter when inconsistent output state exists
  if (Thermostat[ctr_output].status.status_output != Thermostat[ctr_output].status.command_output) {
    Thermostat[ctr_output].status.output_inconsist_ctr++;
  }
  else {
    Thermostat[ctr_output].status.output_inconsist_ctr = 0;
  }
}

void ThermostatSignalProcessingFast(uint8_t ctr_output)
{
  // Update real status of the input
  Thermostat[ctr_output].status.status_input = (uint32_t)ThermostatInputStatus(Thermostat[ctr_output].status.input_switch_number);
  // Update timestamp of last input
  if (Thermostat[ctr_output].status.status_input == IFACE_ON) {
    Thermostat[ctr_output].timestamp_input_on = uptime;
  }
  // Update real status of the output
  Thermostat[ctr_output].status.status_output = (uint32_t)ThermostatOutputStatus(Thermostat[ctr_output].status.output_relay_number);
}

void ThermostatCtrState(uint8_t ctr_output)
{
  switch (Thermostat[ctr_output].status.controller_mode) {
    // Hybrid controller (Ramp-up + PI)
    case CTR_HYBRID:
        ThermostatHybridCtrPhase(ctr_output);
      break;
    // PI controller
    case CTR_PI:
      break;
    // Ramp-up controller (predictive)
    case CTR_RAMP_UP:
      break;
  }
}

void ThermostatHybridCtrPhase(uint8_t ctr_output)
{
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
  if (Thermostat[ctr_output].status.controller_mode == CTR_HYBRID) {
    switch (Thermostat[ctr_output].status.phase_hybrid_ctr) {
      // Ramp-up phase with gradient control
      case CTR_HYBRID_RAMP_UP:
          // If ramp-up offtime counter has been initalized    
          // AND ramp-up offtime counter value reached
          if((Thermostat[ctr_output].time_ctr_checkpoint != 0) 
            && (uptime >= Thermostat[ctr_output].time_ctr_checkpoint)) {
            // Reset pause period
            Thermostat[ctr_output].time_ctr_checkpoint = 0;
            // Reset timers
            Thermostat[ctr_output].time_ctr_changepoint = 0;
            // Set PI controller
            Thermostat[ctr_output].status.phase_hybrid_ctr = CTR_HYBRID_PI;
          }
        break;
      // PI controller phase
      case CTR_HYBRID_PI:
          // If no output action for a pre-defined time
          // AND temp target has changed
          // AND value of temp target - actual temperature bigger than threshold for heating and lower for cooling
          // then go to ramp-up
          if (((uptime - Thermostat[ctr_output].timestamp_output_off) > (60 * (uint32_t)Thermostat[ctr_output].time_allow_rampup))
            && (Thermostat[ctr_output].temp_target_level != Thermostat[ctr_output].temp_target_level_ctr)
            && ( ( (Thermostat[ctr_output].temp_target_level - Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_rampup_delta_in)
                && (flag_heating))
              || ( (Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_target_level > Thermostat[ctr_output].temp_rampup_delta_in)
                && (!flag_heating)))) {
              Thermostat[ctr_output].timestamp_rampup_start = uptime;
              Thermostat[ctr_output].temp_rampup_start = Thermostat[ctr_output].temp_measured;
              Thermostat[ctr_output].temp_rampup_meas_gradient = 0;
              Thermostat[ctr_output].time_rampup_deadtime = 0;
              Thermostat[ctr_output].counter_rampup_cycles = 1;
              Thermostat[ctr_output].time_ctr_changepoint = 0;
              Thermostat[ctr_output].time_ctr_checkpoint = 0;
              Thermostat[ctr_output].status.phase_hybrid_ctr = CTR_HYBRID_RAMP_UP;
          }
        break;
    }
  }
#ifdef DEBUG_THERMOSTAT
  ThermostatVirtualSwitchCtrState(ctr_output);
#endif // DEBUG_THERMOSTAT
}

bool ThermostatStateAutoToManual(uint8_t ctr_output)
{
  bool change_state = false;
  // If input is used
  // AND switch input is active
  //  OR temperature sensor is not alive
  // then go to manual
  if ((Thermostat[ctr_output].status.use_input == INPUT_USED)
    &&((Thermostat[ctr_output].status.status_input == IFACE_ON)
      || (Thermostat[ctr_output].status.sensor_alive == IFACE_OFF))) {
    change_state = true;
  }
  return change_state;
}

bool ThermostatStateManualToAuto(uint8_t ctr_output)
{
  bool change_state = false;

  // If switch input inactive
  // AND sensor alive
  // AND no switch input action (time in current state) bigger than a pre-defined time
  // then go to automatic
  if ((Thermostat[ctr_output].status.status_input == IFACE_OFF) 
    &&(Thermostat[ctr_output].status.sensor_alive ==  IFACE_ON)
    && ((uptime - Thermostat[ctr_output].timestamp_input_on) > ((uint32_t)Thermostat[ctr_output].time_manual_to_auto * 60))) {
    change_state = true;
  }
  return change_state;
}

void ThermostatEmergencyShutdown(uint8_t ctr_output)
{
  // Emergency switch to THERMOSTAT_OFF
  Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_OFF;
  Thermostat[ctr_output].status.command_output = IFACE_OFF;
  ThermostatOutputRelay(ctr_output, Thermostat[ctr_output].status.command_output);
}

void ThermostatState(uint8_t ctr_output)
{
  switch (Thermostat[ctr_output].status.thermostat_mode) {
    // State if Off or Emergency
    case THERMOSTAT_OFF:
      // No change of state possible without external command
      break;
    // State automatic, thermostat active following the command target temp.
    case THERMOSTAT_AUTOMATIC_OP:
      if (ThermostatStateAutoToManual(ctr_output)) {
        // If sensor not alive change to THERMOSTAT_MANUAL_OP
        Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_MANUAL_OP;
      }
      ThermostatCtrState(ctr_output);
      break;
    // State manual operation following input switch
    case THERMOSTAT_MANUAL_OP:
      if (ThermostatStateManualToAuto(ctr_output)) {
        // Input switch inactive and timeout reached change to THERMOSTAT_AUTOMATIC_OP
        Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_AUTOMATIC_OP;
      }
      break;
  }
}

void ThermostatOutputRelay(uint8_t ctr_output, uint32_t command)
{
  // If command received to enable output
  // AND current output status is OFF
  // then switch output to ON
  if ((command == IFACE_ON) 
    && (Thermostat[ctr_output].status.status_output == IFACE_OFF)) {
//#ifndef DEBUG_THERMOSTAT
    ExecuteCommandPower(Thermostat[ctr_output].status.output_relay_number, POWER_ON, SRC_THERMOSTAT);
//#endif // DEBUG_THERMOSTAT
    Thermostat[ctr_output].status.status_output = IFACE_ON;
#ifdef DEBUG_THERMOSTAT
    ThermostatVirtualSwitch(ctr_output);
#endif // DEBUG_THERMOSTAT
  }
  // If command received to disable output
  // AND current output status is ON
  // then switch output to OFF
  else if ((command == IFACE_OFF) && (Thermostat[ctr_output].status.status_output == IFACE_ON)) {
//#ifndef DEBUG_THERMOSTAT
    ExecuteCommandPower(Thermostat[ctr_output].status.output_relay_number, POWER_OFF, SRC_THERMOSTAT);
//#endif // DEBUG_THERMOSTAT
    Thermostat[ctr_output].timestamp_output_off = uptime;
    Thermostat[ctr_output].status.status_output = IFACE_OFF;
#ifdef DEBUG_THERMOSTAT
    ThermostatVirtualSwitch(ctr_output);
#endif // DEBUG_THERMOSTAT
  }
}

void ThermostatCalculatePI(uint8_t ctr_output)
{
  // General comment: Some variables have been increased in resolution to avoid loosing accuracy in division operations
  
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
  int32_t aux_temp_error;
  
  // Calculate error
  aux_temp_error = (int32_t)(Thermostat[ctr_output].temp_target_level_ctr - Thermostat[ctr_output].temp_measured) * 10;
  
  // Invert error for cooling
  if (Thermostat[ctr_output].status.climate_mode == CLIMATE_COOLING) {
    aux_temp_error *= -1;
  }

  // Protect overflow
  if (aux_temp_error <= (int32_t)(INT16_MIN)) {
    Thermostat[ctr_output].temp_pi_error = (int16_t)(INT16_MIN);
  }
  else if (aux_temp_error >= (int32_t)INT16_MAX) {
    Thermostat[ctr_output].temp_pi_error = (int16_t)INT16_MAX;
  }
  else {
    Thermostat[ctr_output].temp_pi_error = (int16_t)aux_temp_error;
  }
  
  // Kp = 100/PI.propBand. PI.propBand(Xp) = Proportional range (4K in 4K/200 controller)
  Thermostat[ctr_output].kP_pi = 100 / (uint16_t)(Thermostat[ctr_output].val_prop_band);
  // Calculate proportional
  Thermostat[ctr_output].time_proportional_pi = ((int32_t)(Thermostat[ctr_output].temp_pi_error * (int16_t)Thermostat[ctr_output].kP_pi) * ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60)) / 10000;

  // Minimum proportional action limiter
  // If proportional action is less than the minimum action time
  // AND proportional > 0
  // then adjust to minimum value
  if ((Thermostat[ctr_output].time_proportional_pi < abs(((int32_t)Thermostat[ctr_output].time_min_action * 60)))
    && (Thermostat[ctr_output].time_proportional_pi > 0)) {
    Thermostat[ctr_output].time_proportional_pi = ((int32_t)Thermostat[ctr_output].time_min_action * 60);
  }
  
  if (Thermostat[ctr_output].time_proportional_pi < 0) {
    Thermostat[ctr_output].time_proportional_pi = 0;
  } 
  else if (Thermostat[ctr_output].time_proportional_pi > ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60)) {
    Thermostat[ctr_output].time_proportional_pi = ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60);
  }

  // Calculate integral (resolution increased to avoid use of floats in consequent operations)
  Thermostat[ctr_output].kI_pi = (uint16_t)((((uint32_t)Thermostat[ctr_output].kP_pi * (uint32_t)Thermostat[ctr_output].time_pi_cycle * 6000)) / (uint32_t)Thermostat[ctr_output].time_reset);
  
  // Reset of antiwindup
  // If error does not lay within the integrator scope range, do not use the integral
  // and accumulate error = 0
  if (abs((Thermostat[ctr_output].temp_pi_error) / 10) > Thermostat[ctr_output].temp_reset_anti_windup) {
    Thermostat[ctr_output].time_integral_pi = 0;
    Thermostat[ctr_output].temp_pi_accum_error = 0;
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

    // Update accumulated error
    aux_temp_error = (int32_t)Thermostat[ctr_output].temp_pi_accum_error + (int32_t)Thermostat[ctr_output].temp_pi_error;

    // Protect overflow
    if (aux_temp_error <= (int32_t)INT16_MIN) {
      Thermostat[ctr_output].temp_pi_accum_error = INT16_MIN;
    }
    else if (aux_temp_error >= (int32_t)INT16_MAX) {
      Thermostat[ctr_output].temp_pi_accum_error = INT16_MAX;
    }
    else {
      Thermostat[ctr_output].temp_pi_accum_error = (int16_t)aux_temp_error;
    }

    // If we are under setpoint
    // AND we are within the hysteresis
    // AND the temperature is rising for heating or sinking for cooling
    if ( (Thermostat[ctr_output].temp_pi_error >= 0)
      && (abs((Thermostat[ctr_output].temp_pi_error) / 10) <= (int16_t)Thermostat[ctr_output].temp_hysteresis)
      && (  ((Thermostat[ctr_output].temp_measured_gradient > 0)
          && (flag_heating))
        || ( (Thermostat[ctr_output].temp_measured_gradient < 0)
          && (!flag_heating)))) {
      // Reduce accumulator error 20% in each cycle
      Thermostat[ctr_output].temp_pi_accum_error *= 0.8;
    }
    // If we are over setpoint
    // AND temperature is rising for heating or sinking for cooling
    else if ((Thermostat[ctr_output].temp_pi_error < 0)
      && (  ((Thermostat[ctr_output].temp_measured_gradient > 0)
          && (flag_heating))
        || ( (Thermostat[ctr_output].temp_measured_gradient < 0)
          && (!flag_heating)))) {
      // Reduce accumulator error 20% in each cycle
      Thermostat[ctr_output].temp_pi_accum_error *= 0.8;
    }

    // Limit lower limit of acumErr to 0
    if (Thermostat[ctr_output].temp_pi_accum_error < 0) {
      Thermostat[ctr_output].temp_pi_accum_error = 0;
    }

    // Integral calculation
    Thermostat[ctr_output].time_integral_pi = (((int32_t)Thermostat[ctr_output].temp_pi_accum_error * (int32_t)Thermostat[ctr_output].kI_pi) * (int32_t)((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60)) / 1000000;

    // Antiwindup of the integrator
    // If integral calculation is bigger than cycle time, adjust result
    // to the cycle time and error will not be cummulated]]
    if (Thermostat[ctr_output].time_integral_pi > ((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60)) {
      Thermostat[ctr_output].time_integral_pi = ((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60);
    }
  }

  // Calculate output
  Thermostat[ctr_output].time_total_pi = Thermostat[ctr_output].time_proportional_pi + Thermostat[ctr_output].time_integral_pi;
  
  // Antiwindup of the output
  // If result is bigger than cycle time, the result will be adjusted
  // to the cylce time minus safety time and error will not be cummulated]]
  if (Thermostat[ctr_output].time_total_pi >= ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60)) {
    // Limit to cycle time //at least switch down a minimum time
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60);
  }
  else if (Thermostat[ctr_output].time_total_pi < 0) {
    Thermostat[ctr_output].time_total_pi = 0;
  }

  // Target value limiter
  // If target value has been reached or we are over it for heating or under it for cooling
  if (Thermostat[ctr_output].temp_pi_error <= 0) {
    // If we are over the hysteresis or the gradient is positive for heating or negative for cooling
    if ((abs((Thermostat[ctr_output].temp_pi_error) / 10) > Thermostat[ctr_output].temp_hysteresis)
      || (  ((Thermostat[ctr_output].temp_measured_gradient >= 0)
          && (flag_heating))
        || ( (Thermostat[ctr_output].temp_measured_gradient <= 0)
          && (!flag_heating)))){
      Thermostat[ctr_output].time_total_pi = 0;
    }
  } 
  // If target value has not been reached
  // AND we are within the histeresis
  // AND gradient is positive for heating or negative for cooling
  // then set value to 0
  else if ((Thermostat[ctr_output].temp_pi_error > 0)
    && (abs((Thermostat[ctr_output].temp_pi_error) / 10) <= Thermostat[ctr_output].temp_hysteresis)
    && (((Thermostat[ctr_output].temp_measured_gradient > 0)
        && (flag_heating))
      || ( (Thermostat[ctr_output].temp_measured_gradient < 0)
        && (!flag_heating)))) {
    Thermostat[ctr_output].time_total_pi = 0;
  }

  // Minimum action limiter
  // If result is less than the minimum action time, adjust to minimum value]]
  if ((Thermostat[ctr_output].time_total_pi <= abs(((uint32_t)Thermostat[ctr_output].time_min_action * 60)))
    && (Thermostat[ctr_output].time_total_pi != 0)) {
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_min_action * 60);
  }
  // Maximum action limiter
  // If result is more than the maximum action time, adjust to maximum value]]
  else if (Thermostat[ctr_output].time_total_pi > abs(((int32_t)Thermostat[ctr_output].time_max_action * 60))) {
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_max_action * 60);
  }
  // If switched off less time than safety time, do not switch off
  else if (Thermostat[ctr_output].time_total_pi > (((int32_t)Thermostat[ctr_output].time_pi_cycle * 60) - ((int32_t)Thermostat[ctr_output].time_min_turnoff_action * 60))) {
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60);
  }
  
  // Adjust output switch point
  Thermostat[ctr_output].time_ctr_changepoint = uptime + (uint32_t)Thermostat[ctr_output].time_total_pi;
  // Adjust next cycle point
  Thermostat[ctr_output].time_ctr_checkpoint = uptime + ((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60);
}

void ThermostatWorkAutomaticPI(uint8_t ctr_output)
{
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
  if ( (uptime >= Thermostat[ctr_output].time_ctr_checkpoint) 
    || (Thermostat[ctr_output].temp_target_level != Thermostat[ctr_output].temp_target_level_ctr)
    || (  (( (Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_target_level)
          && (Thermostat[ctr_output].temp_measured_gradient < 0)
          && (flag_heating))
        || ((Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_target_level)
          && (Thermostat[ctr_output].temp_measured_gradient > 0)
          && (!flag_heating)))
        && (Thermostat[ctr_output].status.status_cycle_active == CYCLE_OFF))) {
    Thermostat[ctr_output].temp_target_level_ctr = Thermostat[ctr_output].temp_target_level;
    ThermostatCalculatePI(ctr_output);
    // Reset cycle active
    Thermostat[ctr_output].status.status_cycle_active = CYCLE_OFF;
  }
  if (uptime < Thermostat[ctr_output].time_ctr_changepoint) {
    Thermostat[ctr_output].status.status_cycle_active = CYCLE_ON;
    Thermostat[ctr_output].status.command_output = IFACE_ON;
  }
  else {
    Thermostat[ctr_output].status.command_output = IFACE_OFF;
  }
}

void ThermostatWorkAutomaticRampUp(uint8_t ctr_output)
{
  uint32_t time_in_rampup;
  int16_t aux_temp_delta;
  int16_t temp_delta_rampup;
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);

  // Update timestamp for temperature at start of ramp-up if temperature still 
  // dropping for heating or rising for cooling
  if (    ((Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_rampup_start)
        && (flag_heating))
      ||  ((Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_rampup_start)
        && (!flag_heating)))
     {
    Thermostat[ctr_output].temp_rampup_start = Thermostat[ctr_output].temp_measured;
  }

  // Update time in ramp-up as well as delta temp
  time_in_rampup = uptime - Thermostat[ctr_output].timestamp_rampup_start;
  temp_delta_rampup = Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_rampup_start;
  // Init command output status to true
  Thermostat[ctr_output].status.command_output = IFACE_ON;
  // Update temperature target level for controller
  Thermostat[ctr_output].temp_target_level_ctr = Thermostat[ctr_output].temp_target_level;

  // If time in ramp-up < max time
  // AND temperature measured < target for heating or > for cooling
  if ((time_in_rampup <= (60 * (uint32_t)Thermostat[ctr_output].time_rampup_max))
    && (  ((Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_target_level)
        && (flag_heating))
      ||  ((Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_target_level)
        && (!flag_heating)))){
    // DEADTIME point reached
    // If temperature measured minus temperature at start of ramp-up >= threshold
    // AND deadtime still 0
    if ( (abs(temp_delta_rampup) >= Thermostat[ctr_output].temp_rampup_delta_out) 
      && (Thermostat[ctr_output].time_rampup_deadtime == 0)) {
      // Set deadtime, assuming it is half of the time until slope, since thermal inertia of the temp. fall needs to be considered
      // minus open time of the valve (arround 3 minutes). If rise/sink very fast limit it to delay of output valve     
      int32_t time_aux;
      time_aux = ((time_in_rampup / 2) - Thermostat[ctr_output].time_output_delay);
      if (time_aux >= Thermostat[ctr_output].time_output_delay) {
        Thermostat[ctr_output].time_rampup_deadtime = (uint32_t)time_aux;
      }
      else {
        Thermostat[ctr_output].time_rampup_deadtime = Thermostat[ctr_output].time_output_delay;
      }
      // Calculate absolute gradient since start of ramp-up (considering deadtime) in thousandths of º/hour
      Thermostat[ctr_output].temp_rampup_meas_gradient = (int32_t)((360000 * (int32_t)temp_delta_rampup) / (int32_t)time_in_rampup);
      Thermostat[ctr_output].time_rampup_nextcycle = uptime + (uint32_t)Thermostat[ctr_output].time_rampup_cycle;
      // Set auxiliary variables
      Thermostat[ctr_output].temp_rampup_cycle = Thermostat[ctr_output].temp_measured;
      Thermostat[ctr_output].time_ctr_changepoint = uptime + (60 * (uint32_t)Thermostat[ctr_output].time_rampup_max);
      Thermostat[ctr_output].temp_rampup_output_off =  Thermostat[ctr_output].temp_target_level_ctr;
    }
    // Gradient calculation every time_rampup_cycle
    else if ((Thermostat[ctr_output].time_rampup_deadtime > 0) && (uptime >= Thermostat[ctr_output].time_rampup_nextcycle)) {
      // Calculate temp. gradient in º/hour and set again time_rampup_nextcycle and temp_rampup_cycle
      // temp_rampup_meas_gradient = ((3600 * temp_delta_rampup) / (os.time() - time_rampup_nextcycle))
      temp_delta_rampup = Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_rampup_cycle;
      uint32_t time_total_rampup = (uint32_t)Thermostat[ctr_output].time_rampup_cycle * Thermostat[ctr_output].counter_rampup_cycles;
      // Translate into gradient per hour (thousandths of ° per hour)
      Thermostat[ctr_output].temp_rampup_meas_gradient = int32_t((360000 * (int32_t)temp_delta_rampup) / (int32_t)time_total_rampup);
      if (   ((Thermostat[ctr_output].temp_rampup_meas_gradient > 0)
          && ((flag_heating)))
        ||   ((Thermostat[ctr_output].temp_rampup_meas_gradient < 0)
          && ((!flag_heating)))) {
        // Calculate time to switch Off and come out of ramp-up
        // y-y1 = m(x-x1) -> x = ((y-y1) / m) + x1 -> y1 = temp_rampup_cycle, x1 = (time_rampup_nextcycle - time_rampup_cycle), m = gradient in º/sec
        // Better Alternative -> (y-y1)/(x-x1) = ((y2-y1)/(x2-x1)) -> where y = temp (target) and x = time (to switch off, what its needed)
        // x = ((y-y1)/(y2-y1))*(x2-x1) + x1 - deadtime        
        aux_temp_delta =Thermostat[ctr_output].temp_target_level_ctr - Thermostat[ctr_output].temp_rampup_cycle;
        Thermostat[ctr_output].time_ctr_changepoint = (uint32_t)(uint32_t)(((uint32_t)(aux_temp_delta) * (uint32_t)(time_total_rampup)) / (uint32_t)temp_delta_rampup) + (uint32_t)Thermostat[ctr_output].time_rampup_nextcycle - (uint32_t)time_total_rampup - (uint32_t)Thermostat[ctr_output].time_rampup_deadtime;

        // Calculate temperature for switching off the output
        // y = (((y2-y1)/(x2-x1))*(x-x1)) + y1
        Thermostat[ctr_output].temp_rampup_output_off = (int16_t)(((int32_t)temp_delta_rampup * (int32_t)(Thermostat[ctr_output].time_ctr_changepoint - (uptime - (time_total_rampup)))) / (int32_t)(time_total_rampup * Thermostat[ctr_output].counter_rampup_cycles)) + Thermostat[ctr_output].temp_rampup_cycle;
        // Set auxiliary variables
        Thermostat[ctr_output].time_rampup_nextcycle = uptime + (uint32_t)Thermostat[ctr_output].time_rampup_cycle;
        Thermostat[ctr_output].temp_rampup_cycle = Thermostat[ctr_output].temp_measured;
        // Reset period counter
        Thermostat[ctr_output].counter_rampup_cycles = 1;
      }
      else {
        // Increase the period counter
        Thermostat[ctr_output].counter_rampup_cycles++;
        // Set another period
        Thermostat[ctr_output].time_rampup_nextcycle = uptime + (uint32_t)Thermostat[ctr_output].time_rampup_cycle;
        // Reset time_ctr_changepoint and temp_rampup_output_off
        Thermostat[ctr_output].time_ctr_changepoint = uptime + (60 * (uint32_t)Thermostat[ctr_output].time_rampup_max) - time_in_rampup;
        Thermostat[ctr_output].temp_rampup_output_off =  Thermostat[ctr_output].temp_target_level_ctr;
      }
      // Set time to get out of ramp-up
      Thermostat[ctr_output].time_ctr_checkpoint = Thermostat[ctr_output].time_ctr_changepoint + Thermostat[ctr_output].time_rampup_deadtime;
    }

    // Set output switch ON or OFF
    // If deadtime has not been calculated
    // or checkpoint has not been calculated
    // or it is not yet time and temperature to switch it off acc. to calculations
    // or gradient is <= 0 for heating of >= 0 for cooling
    if ((Thermostat[ctr_output].time_rampup_deadtime == 0)
      || (Thermostat[ctr_output].time_ctr_checkpoint == 0)
      || (uptime < Thermostat[ctr_output].time_ctr_changepoint)
      || (  ((Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_rampup_output_off)
          && (flag_heating))
        ||  ((Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_rampup_output_off)
          && (!flag_heating)))
      || (  ((Thermostat[ctr_output].temp_rampup_meas_gradient <= 0)
          && (flag_heating))
        ||  ((Thermostat[ctr_output].temp_rampup_meas_gradient >= 0)
          && (!flag_heating)))) {
      Thermostat[ctr_output].status.command_output = IFACE_ON;
    }
    else {
      Thermostat[ctr_output].status.command_output = IFACE_OFF;
    }
  }
  else {
    // If we have not reached the temperature, start with an initial value for accumulated error for the PI controller
    if (  ((Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_target_level_ctr)
        && (flag_heating))
      ||  ((Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_target_level_ctr)
        && (!flag_heating))) {
      Thermostat[ctr_output].temp_pi_accum_error = Thermostat[ctr_output].temp_rampup_pi_acc_error;
    }
    // Set to now time to get out of ramp-up
    Thermostat[ctr_output].time_ctr_checkpoint = uptime;
    // Switch Off output
    Thermostat[ctr_output].status.command_output = IFACE_OFF;
  }
}

void ThermostatCtrWork(uint8_t ctr_output)
{
  switch (Thermostat[ctr_output].status.controller_mode) {
    // Hybrid controller (Ramp-up + PI)
    case CTR_HYBRID:
      switch (Thermostat[ctr_output].status.phase_hybrid_ctr) {
        case CTR_HYBRID_RAMP_UP:
          ThermostatWorkAutomaticRampUp(ctr_output);
          break;
        case CTR_HYBRID_PI:
          ThermostatWorkAutomaticPI(ctr_output);
          break;
      }
      break;
    // PI controller
    case CTR_PI:
      ThermostatWorkAutomaticPI(ctr_output);
      break;
    // Ramp-up controller (predictive)
    case CTR_RAMP_UP:
      ThermostatWorkAutomaticRampUp(ctr_output);
      break;
  }
}

void ThermostatWork(uint8_t ctr_output)
{
  switch (Thermostat[ctr_output].status.thermostat_mode) {
    // State if thermostat Off or Emergency
    case THERMOSTAT_OFF:
      Thermostat[ctr_output].status.command_output = IFACE_OFF;
      break;
    // State automatic thermostat active following to command target temp.
    case THERMOSTAT_AUTOMATIC_OP:
      ThermostatCtrWork(ctr_output);
      
      break;
    // State manual operation following input switch
    case THERMOSTAT_MANUAL_OP:
      Thermostat[ctr_output].time_ctr_checkpoint = 0;
      Thermostat[ctr_output].status.command_output = Thermostat[ctr_output].status.status_input;   
      break;
  }
  ThermostatOutputRelay(ctr_output, Thermostat[ctr_output].status.command_output);
}

void ThermostatDiagnostics(uint8_t ctr_output)
{ 
  // Diagnostic related to the plausibility of the output state
  if ((Thermostat[ctr_output].status.diagnostic_mode == DIAGNOSTIC_ON)
    &&(Thermostat[ctr_output].status.output_inconsist_ctr >= THERMOSTAT_TIME_MAX_OUTPUT_INCONSIST)) {
    Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_OFF;
    Thermostat[ctr_output].status.state_emergency = EMERGENCY_ON;  
  }

  // Diagnostic related to the plausibility of the output power implemented 
  // already into the energy driver

  // If diagnostics fail, emergency enabled and thermostat shutdown triggered
  if (Thermostat[ctr_output].status.state_emergency == EMERGENCY_ON) {
    ThermostatEmergencyShutdown(ctr_output);
  }
}

void ThermostatController(uint8_t ctr_output)
{
  ThermostatState(ctr_output);
  ThermostatWork(ctr_output);
}

bool ThermostatTimerArm(uint8_t ctr_output, int16_t tempVal)
{
  bool result = false;
  // TempVal unit is tenths of degrees celsius
  if ((tempVal >= -1000) 
    && (tempVal <= 1000)
    && (tempVal >= (int16_t)Thermostat[ctr_output].temp_frost_protect)) {
      Thermostat[ctr_output].temp_target_level = tempVal;
      Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_AUTOMATIC_OP;
      result = true;
  }
  // Returns true if setpoint plausible and thermostat armed, false on the contrary
  return result;
}

void ThermostatTimerDisarm(uint8_t ctr_output)
{
  Thermostat[ctr_output].temp_target_level = THERMOSTAT_TEMP_INIT;
  Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_OFF;
}

#ifdef DEBUG_THERMOSTAT
void ThermostatVirtualSwitch(uint8_t ctr_output)
{
  char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
  if (ctr_output < DOMOTICZ_MAX_IDX) {
    Response_P(DOMOTICZ_MES, Domoticz_Virtual_Switches[ctr_output], (0 == Thermostat[ctr_output].status.command_output) ? 0 : 1, "");
    MqttPublish(domoticz_in_topic);
  }
}

void ThermostatVirtualSwitchCtrState(uint8_t ctr_output)
{
  char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
  Response_P(DOMOTICZ_MES, DOMOTICZ_IDX2, (0 == Thermostat[0].status.phase_hybrid_ctr) ? 0 : 1, "");
  MqttPublish(domoticz_in_topic);
}

void ThermostatDebug(uint8_t ctr_output)
{
  char result_chr[FLOATSZ];
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(""));
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("------ Thermostat Start ------"));
  dtostrfd(Thermostat[ctr_output].status.counter_seconds, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.counter_seconds: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.thermostat_mode, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.thermostat_mode: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.state_emergency, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.state_emergency: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.output_inconsist_ctr, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.output_inconsist_ctr: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.controller_mode, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.controller_mode: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.command_output, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.command_output: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.status_output, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.status_output: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.status_input, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.status_input: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.phase_hybrid_ctr, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.phase_hybrid_ctr: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.sensor_alive, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.sensor_alive: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].status.status_cycle_active, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].status.status_cycle_active: %s"), result_chr);   
  dtostrfd(Thermostat[ctr_output].temp_pi_error, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].temp_pi_error: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].temp_pi_accum_error, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].temp_pi_accum_error: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].time_proportional_pi, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].time_proportional_pi: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].time_integral_pi, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].time_integral_pi: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].time_total_pi, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].time_total_pi: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].temp_measured_gradient, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].temp_measured_gradient: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].time_rampup_deadtime, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].time_rampup_deadtime: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].temp_rampup_meas_gradient, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].temp_rampup_meas_gradient: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].time_ctr_changepoint, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].time_ctr_changepoint: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].temp_rampup_output_off, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].temp_rampup_output_off: %s"), result_chr);
  dtostrfd(Thermostat[ctr_output].time_ctr_checkpoint, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Thermostat[ctr_output].time_ctr_checkpoint: %s"), result_chr);
  dtostrfd(uptime, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("uptime: %s"), result_chr);
  dtostrfd(power, 0, result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("power: %s"), result_chr);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("------ Thermostat End ------"));
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(""));
}
#endif // DEBUG_THERMOSTAT

void ThermostatGetLocalSensor(uint8_t ctr_output) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject((const char*)mqtt_data);
  if (root.success()) {
    const char* value_c = root["THERMOSTAT_SENSOR_NAME"]["Temperature"];
    if (value_c != NULL && strlen(value_c) > 0 && (isdigit(value_c[0]) || (value_c[0] == '-' && isdigit(value_c[1])) ) ) {
      int16_t value = (int16_t)(CharToFloat(value_c) * 10);
      if ( (value >= -1000) 
        && (value <= 1000)
        && (Thermostat[ctr_output].status.sensor_type == SENSOR_LOCAL)) {
        uint32_t timestamp = uptime;
        // Calculate temperature gradient if temperature value has changed
        if (value != Thermostat[ctr_output].temp_measured) {
          int32_t temp_delta = (value - Thermostat[ctr_output].temp_measured); // in tenths of degrees
          uint32_t time_delta = (timestamp - Thermostat[ctr_output].timestamp_temp_meas_change_update); // in seconds
          Thermostat[ctr_output].temp_measured_gradient = (int32_t)((360000 * temp_delta) / ((int32_t)time_delta)); // thousandths of degrees per hour
          Thermostat[ctr_output].temp_measured = value;
          Thermostat[ctr_output].timestamp_temp_meas_change_update = timestamp;
        }
        Thermostat[ctr_output].timestamp_temp_measured_update = timestamp;
        Thermostat[ctr_output].status.sensor_alive = IFACE_ON;
      }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndThermostatModeSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
      if ((value >= THERMOSTAT_OFF) && (value < THERMOSTAT_MODES_MAX)) {
        Thermostat[ctr_output].status.thermostat_mode = value;
        Thermostat[ctr_output].timestamp_input_on = 0;     // Reset last manual switch timer if command set externally
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.thermostat_mode);
  }
}

void CmndClimateModeSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
      if ((value >= CLIMATE_HEATING) && (value < CLIMATE_MODES_MAX)) {
        Thermostat[ctr_output].status.climate_mode = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.climate_mode);
  }
}

void CmndTempFrostProtectSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    int16_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = (int16_t)ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_ABSOLUTE);
      }
      else {
        value = (int16_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= 0) 
        && (value <= 127)) {
        Thermostat[ctr_output].temp_frost_protect = (uint8_t)value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_frost_protect, TEMP_CONV_ABSOLUTE);
    }
    else {
      value = (int16_t)Thermostat[ctr_output].temp_frost_protect;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndControllerModeSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= CTR_HYBRID) && (value < CTR_MODES_MAX)) {
        Thermostat[ctr_output].status.controller_mode = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.controller_mode);
  }
}

void CmndInputSwitchSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if (ThermostatSwitchIdValid(value)) {
        Thermostat[ctr_output].status.input_switch_number = value;
        Thermostat[ctr_output].timestamp_input_on = uptime;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.input_switch_number);
  }
}

void CmndInputSwitchUse(void)
{
  if ((XdrvMailbox.index >= INPUT_NOT_USED) && (XdrvMailbox.index <= INPUT_USED)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      Thermostat[ctr_output].status.use_input = (uint32_t)(XdrvMailbox.payload);
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.use_input);
  }
}

void CmndSensorInputSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= SENSOR_MQTT) && (value < SENSOR_MAX)) {
        Thermostat[ctr_output].status.sensor_type = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.sensor_type);
  }
}

void CmndOutputRelaySet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if (ThermostatRelayIdValid(value)) {
        Thermostat[ctr_output].status.output_relay_number = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.output_relay_number);
  }
}

void CmndTimeAllowRampupSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value < 86400)) {
        Thermostat[ctr_output].time_allow_rampup = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_allow_rampup * 60));
  }
}

void CmndTempFormatSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= TEMP_FAHRENHEIT)) {
        Thermostat[ctr_output].status.temp_format = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.temp_format);
  }
}

void CmndTempMeasuredSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    int16_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_ABSOLUTE);
      }
      else {
        value = (int16_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= -1000) 
        && (value <= 1000)
        && (Thermostat[ctr_output].status.sensor_type == SENSOR_MQTT)) {
        uint32_t timestamp = uptime;
        // Calculate temperature gradient if temperature value has changed
        if (value != Thermostat[ctr_output].temp_measured) {
          int32_t temp_delta = (value - Thermostat[ctr_output].temp_measured); // in tenths of degrees
          uint32_t time_delta = (timestamp - Thermostat[ctr_output].timestamp_temp_meas_change_update); // in seconds
          Thermostat[ctr_output].temp_measured_gradient = (int32_t)((360000 * temp_delta) / ((int32_t)time_delta)); // thousandths of degrees per hour
          Thermostat[ctr_output].temp_measured = value;
          Thermostat[ctr_output].timestamp_temp_meas_change_update = timestamp;
        }
        Thermostat[ctr_output].timestamp_temp_measured_update = timestamp;
        Thermostat[ctr_output].status.sensor_alive = IFACE_ON;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_measured, TEMP_CONV_ABSOLUTE);
    }
    else {
      value = Thermostat[ctr_output].temp_measured;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndTempTargetSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    int16_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_ABSOLUTE);
      }
      else {
        value = (int16_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= -1000) 
        && (value <= 1000)
        && (value >= (int16_t)Thermostat[ctr_output].temp_frost_protect)) {
        Thermostat[ctr_output].temp_target_level = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_target_level, TEMP_CONV_ABSOLUTE);
    }
    else {
      value = Thermostat[ctr_output].temp_target_level;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndTempMeasuredGrdRead(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    int16_t value;
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_measured_gradient, TEMP_CONV_RELATIVE);
    }
    else {
      value = Thermostat[ctr_output].temp_measured_gradient;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndStateEmergencySet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1)) {
        Thermostat[ctr_output].status.state_emergency = (uint16_t)value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.state_emergency);
  }
}

void CmndTimeManualToAutoSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_manual_to_auto = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_manual_to_auto * 60));
  }
}

void CmndTimeOnLimitSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_on_limit = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_on_limit * 60));
  }
}

void CmndPropBandSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 20)) {
        Thermostat[ctr_output].val_prop_band = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].val_prop_band);
  }
}

void CmndTimeResetSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_reset = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].time_reset);
  }
}

void CmndTimePiCycleSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_pi_cycle = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60));
  }
}

void CmndTempAntiWindupResetSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    uint8_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = (uint8_t)ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_RELATIVE);
      }
      else {
        value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= 0) 
        && (value <= 100)) {
        Thermostat[ctr_output].temp_reset_anti_windup = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_reset_anti_windup, TEMP_CONV_RELATIVE);
    }
    else {
      value = Thermostat[ctr_output].temp_reset_anti_windup;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndTempHystSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    int8_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = (int8_t)ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_RELATIVE);
      }
      else {
        value = (int8_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= -100) 
        && (value <= 100)) {
        Thermostat[ctr_output].temp_hysteresis = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_hysteresis, TEMP_CONV_RELATIVE);
    }
    else {
      value = Thermostat[ctr_output].temp_hysteresis;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndTimeMaxActionSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_max_action = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_max_action * 60));
  }
}

void CmndTimeMinActionSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_min_action = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_min_action * 60));
  }
}

void CmndTimeSensLostSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_sens_lost = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_sens_lost * 60));
  }
}

void CmndTimeMinTurnoffActionSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_min_turnoff_action = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)((uint32_t)Thermostat[ctr_output].time_min_turnoff_action * 60));
  }
}

void CmndTempRupDeltInSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    uint8_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = (uint8_t)ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_RELATIVE);
      }
      else {
        value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= 0) 
        && (value <= 100)) {
        Thermostat[ctr_output].temp_rampup_delta_in = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_rampup_delta_in, TEMP_CONV_RELATIVE);
    }
    else {
      value = Thermostat[ctr_output].temp_rampup_delta_in;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndTempRupDeltOutSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    uint8_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = (uint8_t)ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 10), TEMP_CONV_RELATIVE);
      }
      else {
        value = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
      }
      if ( (value >= 0) 
        && (value <= 100)) {
        Thermostat[ctr_output].temp_rampup_delta_out = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_rampup_delta_out, TEMP_CONV_RELATIVE);
    }
    else {
      value = Thermostat[ctr_output].temp_rampup_delta_out;
    }
    ResponseCmndFloat((float)value / 10, 1);
  }
}

void CmndTimeRampupMaxSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 86400)) {
        Thermostat[ctr_output].time_rampup_max = (uint16_t)(value / 60);
      }
    }
    ResponseCmndNumber((int)(((uint32_t)Thermostat[ctr_output].time_rampup_max) * 60));
  }
}

void CmndTimeRampupCycleSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 54000)) {
        Thermostat[ctr_output].time_rampup_cycle = (uint16_t)value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].time_rampup_cycle);
  }
}

void CmndTempRampupPiAccErrSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    uint16_t value;
    if (XdrvMailbox.data_len > 0) {
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = (uint16_t)ThermostatFahrenheitToCelsius((int32_t)(CharToFloat(XdrvMailbox.data) * 100), TEMP_CONV_RELATIVE);
      }
      else {
        value = (uint16_t)(CharToFloat(XdrvMailbox.data) * 100);
      }
      if ( (value >= 0) 
        && (value <= 2500)) {
        Thermostat[ctr_output].temp_rampup_pi_acc_error = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_rampup_pi_acc_error, TEMP_CONV_RELATIVE);
    }
    else {
      value = Thermostat[ctr_output].temp_rampup_pi_acc_error;
    }
    ResponseCmndFloat((float)value / 100, 1);
  }
}

void CmndTimePiProportRead(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    ResponseCmndNumber((int)Thermostat[ctr_output].time_proportional_pi);
  }
}

void CmndTimePiIntegrRead(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    ResponseCmndNumber((int)Thermostat[ctr_output].time_integral_pi);
  }
}

void CmndDiagnosticModeSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
      if ((value >= DIAGNOSTIC_OFF) && (value <= DIAGNOSTIC_ON)) {
        Thermostat[ctr_output].status.diagnostic_mode = value;
      }
    }
    ResponseCmndNumber((int)Thermostat[ctr_output].status.diagnostic_mode);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv39(uint8_t function)
{
  bool result = false;
  uint8_t ctr_output;

  switch (function) {
    case FUNC_INIT:
      for (ctr_output = 0; ctr_output < THERMOSTAT_CONTROLLER_OUTPUTS; ctr_output++) {
        ThermostatInit(ctr_output);
      }
      break;
    case FUNC_LOOP:
      for (ctr_output = 0; ctr_output < THERMOSTAT_CONTROLLER_OUTPUTS; ctr_output++) {
        if (Thermostat[ctr_output].status.thermostat_mode != THERMOSTAT_OFF) {
          ThermostatSignalProcessingFast(ctr_output);
          ThermostatDiagnostics(ctr_output);
        }
      }
      break;
    case FUNC_SERIAL:
      break;
    case FUNC_EVERY_SECOND:
      for (ctr_output = 0; ctr_output < THERMOSTAT_CONTROLLER_OUTPUTS; ctr_output++) {
        if ((ThermostatMinuteCounter(ctr_output))
          && (Thermostat[ctr_output].status.thermostat_mode != THERMOSTAT_OFF)) {
          ThermostatSignalPreProcessingSlow(ctr_output);
          ThermostatController(ctr_output);
          ThermostatSignalPostProcessingSlow(ctr_output);
#ifdef DEBUG_THERMOSTAT
          ThermostatDebug(ctr_output);
#endif // DEBUG_THERMOSTAT
        }
      }
      break;
    case FUNC_SHOW_SENSOR:
      for (ctr_output = 0; ctr_output < THERMOSTAT_CONTROLLER_OUTPUTS; ctr_output++) {
        if (Thermostat[ctr_output].status.thermostat_mode != THERMOSTAT_OFF) {
          ThermostatGetLocalSensor(ctr_output);
        }
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kThermostatCommands, ThermostatCommand);
      break;
  }
  return result;
}

#endif // USE_THERMOSTAT
