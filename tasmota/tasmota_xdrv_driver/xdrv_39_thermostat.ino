/*
  xdrv_39_thermostat.ino - Thermostat controller for Tasmota

  Copyright (C) 2021  Javier Arigita

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

// Enable/disable experimental PI auto-tuning inspired by the Arduino
// Autotune Library by Brett Beauregard
//#define USE_PI_AUTOTUNING // (Ziegler-Nichols closed loop method)

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
#ifdef USE_PI_AUTOTUNING
#define D_CMND_PERFLEVELAUTOTUNE "PerfLevelAutotune"
#endif // USE_PI_AUTOTUNING
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
#define D_CMND_CTRDUTYCYCLEREAD "CtrDutyCycleRead"
#define D_CMND_ENABLEOUTPUTSET "EnableOutputSet"

enum ThermostatModes { THERMOSTAT_OFF, THERMOSTAT_AUTOMATIC_OP, THERMOSTAT_MANUAL_OP, THERMOSTAT_MODES_MAX };
#ifdef USE_PI_AUTOTUNING
enum ControllerModes { CTR_HYBRID, CTR_PI, CTR_RAMP_UP, CTR_PI_AUTOTUNE, CTR_MODES_MAX };
enum ControllerHybridPhases { CTR_HYBRID_RAMP_UP, CTR_HYBRID_PI, CTR_HYBRID_PI_AUTOTUNE };
enum AutotuneStates { AUTOTUNE_OFF, AUTOTUNE_ON, AUTOTUNE_MAX };
enum AutotunePerformanceParam { AUTOTUNE_PERF_FAST, AUTOTUNE_PERF_NORMAL, AUTOTUNE_PERF_SLOW, AUTOTUNE_PERF_MAX };
#else
enum ControllerModes { CTR_HYBRID, CTR_PI, CTR_RAMP_UP, CTR_MODES_MAX };
enum ControllerHybridPhases { CTR_HYBRID_RAMP_UP, CTR_HYBRID_PI };
#endif // USE_PI_AUTOTUNING
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
  THERMOSTAT_INPUT_SWT4,
  THERMOSTAT_INPUT_SWT5,
  THERMOSTAT_INPUT_SWT6,
  THERMOSTAT_INPUT_SWT7,
  THERMOSTAT_INPUT_SWT8
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
    uint32_t phase_hybrid_ctr : 2;      // Phase of the hybrid controller (Ramp-up, PI or Autotune)
    uint32_t status_cycle_active : 1;   // Status showing if cycle is active (Output ON) or not (Output OFF)
    uint32_t counter_seconds : 6;       // Second counter used to track minutes
    uint32_t output_relay_number : 4;   // Output relay number
    uint32_t input_switch_number : 3;   // Input switch number
    uint32_t enable_output : 1;         // Enables / disables the physical output
#ifdef USE_PI_AUTOTUNING
    uint32_t autotune_flag : 1;         // Enable/disable autotune
    uint32_t autotune_perf_mode : 2;    // Autotune performance mode
#else
    uint32_t free : 3;                  // Free bits
#endif // USE_PI_AUTOTUNING
  };
} ThermostatStateBitfield;

typedef union {
  uint8_t data;
  struct {
    uint8_t state_emergency : 1;       // State for thermostat emergency
    uint8_t diagnostic_mode : 1;       // Diagnostic mode selected
    uint8_t output_inconsist_ctr : 2;  // Counter of the minutes where the output state is inconsistent with the command
  };
} ThermostatDiagBitfield;

#ifdef DEBUG_THERMOSTAT
const char DOMOTICZ_MES[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}";
uint16_t Domoticz_Virtual_Switches[DOMOTICZ_MAX_IDX] = { DOMOTICZ_IDX1, DOMOTICZ_IDX3, DOMOTICZ_IDX4, DOMOTICZ_IDX5 };
#endif // DEBUG_THERMOSTAT

const char kThermostatCommands[] PROGMEM = "|" D_CMND_THERMOSTATMODESET "|" D_CMND_CLIMATEMODESET "|"
  D_CMND_TEMPFROSTPROTECTSET "|" D_CMND_CONTROLLERMODESET "|" D_CMND_INPUTSWITCHSET "|" D_CMND_INPUTSWITCHUSE "|"
  D_CMND_OUTPUTRELAYSET "|" D_CMND_TIMEALLOWRAMPUPSET "|" D_CMND_TEMPFORMATSET "|" D_CMND_TEMPMEASUREDSET "|"
  D_CMND_TEMPTARGETSET "|" D_CMND_TEMPMEASUREDGRDREAD "|" D_CMND_SENSORINPUTSET "|" D_CMND_STATEEMERGENCYSET "|"
  D_CMND_TIMEMANUALTOAUTOSET "|" D_CMND_PROPBANDSET "|" D_CMND_TIMERESETSET "|" D_CMND_TIMEPICYCLESET "|"
#ifdef USE_PI_AUTOTUNING
  D_CMND_TEMPANTIWINDUPRESETSET "|" D_CMND_TEMPHYSTSET "|" D_CMND_PERFLEVELAUTOTUNE "|" D_CMND_TIMEMAXACTIONSET "|"
#else
  D_CMND_TEMPANTIWINDUPRESETSET "|" D_CMND_TEMPHYSTSET "|" D_CMND_TIMEMAXACTIONSET "|"
#endif // USE_PI_AUTOTUNING
  D_CMND_TIMEMINACTIONSET "|" D_CMND_TIMEMINTURNOFFACTIONSET "|" D_CMND_TEMPRUPDELTINSET "|" D_CMND_TEMPRUPDELTOUTSET "|"
  D_CMND_TIMERAMPUPMAXSET "|" D_CMND_TIMERAMPUPCYCLESET "|" D_CMND_TEMPRAMPUPPIACCERRSET "|" D_CMND_TIMEPIPROPORTREAD "|"
  D_CMND_TIMEPIINTEGRREAD "|" D_CMND_TIMESENSLOSTSET "|" D_CMND_DIAGNOSTICMODESET "|" D_CMND_CTRDUTYCYCLEREAD "|"
  D_CMND_ENABLEOUTPUTSET;

void (* const ThermostatCommand[])(void) PROGMEM = {
  &CmndThermostatModeSet, &CmndClimateModeSet, &CmndTempFrostProtectSet, &CmndControllerModeSet, &CmndInputSwitchSet,
  &CmndInputSwitchUse, &CmndOutputRelaySet, &CmndTimeAllowRampupSet, &CmndTempFormatSet, &CmndTempMeasuredSet,
  &CmndTempTargetSet, &CmndTempMeasuredGrdRead, &CmndSensorInputSet, &CmndStateEmergencySet, &CmndTimeManualToAutoSet,
  &CmndPropBandSet, &CmndTimeResetSet, &CmndTimePiCycleSet, &CmndTempAntiWindupResetSet, &CmndTempHystSet,
#ifdef USE_PI_AUTOTUNING
  &CmndPerfLevelAutotune, &CmndTimeMaxActionSet, &CmndTimeMinActionSet, &CmndTimeMinTurnoffActionSet, &CmndTempRupDeltInSet,
#else
  &CmndTimeMaxActionSet, &CmndTimeMinActionSet, &CmndTimeMinTurnoffActionSet, &CmndTempRupDeltInSet,
#endif // USE_PI_AUTOTUNING
  &CmndTempRupDeltOutSet, &CmndTimeRampupMaxSet, &CmndTimeRampupCycleSet, &CmndTempRampupPiAccErrSet,
  &CmndTimePiProportRead, &CmndTimePiIntegrRead, &CmndTimeSensLostSet, &CmndDiagnosticModeSet, &CmndCtrDutyCycleRead,
  &CmndEnableOutputSet };

struct THERMOSTAT {
  ThermostatStateBitfield status;                                             // Bitfield including states as well as several flags
  uint32_t timestamp_temp_measured_update = 0;                                // Timestamp of latest measurement update
  uint32_t timestamp_temp_meas_change_update = 0;                             // Timestamp of latest measurement value change (> or < to previous)
  uint32_t timestamp_output_off = 0;                                          // Timestamp of latest thermostat output Off state
  uint32_t timestamp_input_on = 0;                                            // Timestamp of latest input On state
  uint32_t time_thermostat_total = 0;                                         // Time thermostat on within a specific timeframe
  uint32_t time_ctr_checkpoint = 0;                                           // Time to finalize the control cycle within the PI strategy or to switch to PI from Rampup in seconds
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
  uint16_t kI_pi = 0;                                                         // kI value for the PI controller multiplied by 100 (to avoid floating point operations)
  int32_t temp_rampup_meas_gradient = 0;                                      // Temperature measured gradient from sensor in thousandths of degrees celsius per hour calculated during ramp-up
  uint32_t timestamp_rampup_start = 0;                                        // Timestamp where the ramp-up controller mode has been started
  uint32_t time_rampup_deadtime = 0;                                          // Time constant of the thermostat system (step response time)
  uint32_t time_rampup_nextcycle = 0;                                         // Time where the ramp-up controller shall start the next cycle
  int16_t temp_measured = 0;                                                  // Temperature measurement received from sensor in tenths of degrees celsius
  int16_t temp_rampup_output_off = 0;                                         // Temperature to switch off relay output within the ramp-up controller in tenths of degrees celsius
  uint8_t time_output_delay = THERMOSTAT_TIME_OUTPUT_DELAY;                   // Output delay between state change and real actuation event (eg. valve open/closed)
  uint8_t counter_rampup_cycles = 0;                                          // Counter of ramp-up cycles
  uint8_t temp_rampup_pi_acc_error = THERMOSTAT_TEMP_PI_RAMPUP_ACC_E;         // Accumulated error when switching from ramp-up controller to PI in hundreths of degrees celsius
  uint8_t temp_rampup_delta_out = THERMOSTAT_TEMP_RAMPUP_DELTA_OUT;           // Minimum delta temperature to target to get out of the rampup mode, in tenths of degrees celsius
  uint8_t temp_rampup_delta_in = THERMOSTAT_TEMP_RAMPUP_DELTA_IN;             // Minimum delta temperature to target to get into rampup mode, in tenths of degrees celsius
  uint8_t val_prop_band = THERMOSTAT_PROP_BAND;                               // Proportional band of the PI controller in degrees celsius
  int16_t temp_rampup_start = 0;                                              // Temperature at start of ramp-up controller in tenths of degrees celsius
  int16_t temp_rampup_cycle = 0;                                              // Temperature set at the beginning of each ramp-up cycle in tenths of degrees
  uint16_t time_rampup_max = THERMOSTAT_TIME_RAMPUP_MAX;                      // Time maximum ramp-up controller duration in minutes
  uint16_t time_rampup_cycle = THERMOSTAT_TIME_RAMPUP_CYCLE;                  // Time ramp-up cycle in minutes
  uint16_t time_allow_rampup = THERMOSTAT_TIME_ALLOW_RAMPUP;                  // Time in minutes after last target update to allow ramp-up controller phase
  uint16_t time_sens_lost = THERMOSTAT_TIME_SENS_LOST;                        // Maximum time without sensor update to set it as lost in minutes
  uint16_t time_manual_to_auto = THERMOSTAT_TIME_MANUAL_TO_AUTO;              // Time without input switch active to change from manual to automatic in minutes
  uint32_t time_reset = THERMOSTAT_TIME_RESET;                                // Reset time of the PI controller in seconds
  uint16_t time_pi_cycle = THERMOSTAT_TIME_PI_CYCLE;                          // Cycle time for the thermostat controller in minutes
  uint16_t time_max_action = THERMOSTAT_TIME_MAX_ACTION;                      // Maximum thermostat time per cycle in minutes
  uint16_t time_min_action = THERMOSTAT_TIME_MIN_ACTION;                      // Minimum thermostat time per cycle in minutes
  uint16_t time_min_turnoff_action = THERMOSTAT_TIME_MIN_TURNOFF_ACTION;      // Minimum turnoff time in minutes, below which the thermostat will stay on
  int16_t temp_frost_protect = THERMOSTAT_TEMP_FROST_PROTECT;                 // Minimum temperature for frost protection, in tenths of degrees celsius
  uint8_t temp_reset_anti_windup = THERMOSTAT_TEMP_RESET_ANTI_WINDUP;         // Range where reset antiwindup is disabled, in tenths of degrees celsius
  int8_t temp_hysteresis = THERMOSTAT_TEMP_HYSTERESIS;                        // Range hysteresis for temperature PI controller, in tenths of degrees celsius
  ThermostatDiagBitfield diag;                                                // Bitfield including diagnostic flags
#ifdef USE_PI_AUTOTUNING
  uint8_t dutycycle_step_autotune = THERMOSTAT_DUTYCYCLE_AUTOTUNE;            // Duty cycle for the step response of the autotune PI function in %
  uint8_t peak_ctr = 0;                                                       // Peak counter for the autotuning function
  uint8_t temp_band_no_peak_det = THERMOSTAT_TEMP_BAND_NO_PEAK_DET;           // Temperature band in thenths of degrees celsius within no peak will be detected
  uint8_t val_prop_band_atune = 0;                                            // Proportional band calculated from the the PI autotune function in degrees celsius
  uint32_t time_reset_atune = 0;                                              // Reset time calculated from the PI autotune function in seconds
  uint16_t pU_pi_atune = 0;                                                   // pU value ("Ultimate" period) period of self-sustaining oscillations determined when the controller gain was set to Ku in minutes (for PI autotune)
  uint16_t kU_pi_atune = 0;                                                   // kU value ("Ultimate" gain) determined by increasing controller gain until self-sustaining oscillations are achieved (for PI autotune)
  uint16_t kP_pi_atune = 0;                                                   // kP value calculated by the autotune PI function multiplied by 100 (to avoid floating point operations)
  uint16_t kI_pi_atune = 0;                                                   // kI value calulated by the autotune PI function multiplied by 100 (to avoid floating point operations)
  int16_t temp_peaks_atune[THERMOSTAT_PEAKNUMBER_AUTOTUNE];                   // Array to store temperature peaks to be used by the autotune PI function
  int16_t temp_abs_max_atune;                                                 // Maximum temperature reached within autotune
  int16_t temp_abs_min_atune;                                                 // Minimum temperature reached within autotune
  uint16_t time_peak_timestamps_atune[THERMOSTAT_PEAKNUMBER_AUTOTUNE];        // Array to store timestamps in minutes of the temperature peaks to be used by the autotune PI function
  uint16_t time_std_dev_peak_det_ok = THERMOSTAT_TIME_STD_DEV_PEAK_DET_OK;    // Standard deviation in minutes of the oscillation periods within the peak detection is successful
#endif // USE_PI_AUTOTUNING
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
  Thermostat[ctr_output].diag.state_emergency = EMERGENCY_OFF;
  Thermostat[ctr_output].status.counter_seconds = 0;
  Thermostat[ctr_output].status.output_relay_number = (THERMOSTAT_RELAY_NUMBER + ctr_output);
  Thermostat[ctr_output].status.input_switch_number = (THERMOSTAT_SWITCH_NUMBER + ctr_output);
  Thermostat[ctr_output].status.use_input = INPUT_NOT_USED;
  Thermostat[ctr_output].status.enable_output = IFACE_ON;
  Thermostat[ctr_output].diag.output_inconsist_ctr = 0;
  Thermostat[ctr_output].diag.diagnostic_mode = DIAGNOSTIC_ON;
#ifdef USE_PI_AUTOTUNING
  Thermostat[ctr_output].status.autotune_flag = AUTOTUNE_OFF;
  Thermostat[ctr_output].status.autotune_perf_mode = AUTOTUNE_PERF_FAST;
#endif // USE_PI_AUTOTUNING
  // Make sure the Output is OFF
  if (Thermostat[ctr_output].status.enable_output == IFACE_ON) {
    ExecuteCommandPower(Thermostat[ctr_output].status.output_relay_number, POWER_OFF, SRC_THERMOSTAT);
  }
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
  return (switchId >= THERMOSTAT_INPUT_SWT1 && switchId <= THERMOSTAT_INPUT_SWT8);
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
    value = SwitchGetState(ifId - THERMOSTAT_INPUT_SWT1);
  }
  return value;
}

uint8_t ThermostatOutputStatus(uint8_t output_switch)
{
  return (uint8_t)bitRead(TasmotaGlobal.power, (output_switch - 1));
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
  if ((TasmotaGlobal.uptime - Thermostat[ctr_output].timestamp_temp_measured_update) > ((uint32_t)Thermostat[ctr_output].time_sens_lost * 60)) {
    Thermostat[ctr_output].status.sensor_alive = IFACE_OFF;
    Thermostat[ctr_output].temp_measured_gradient = 0;
    Thermostat[ctr_output].temp_measured = 0;

    char result_chr[FLOATSZ];

    dtostrfd((TasmotaGlobal.uptime - Thermostat[ctr_output].timestamp_temp_measured_update), 0, result_chr);
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_THERMOSTAT "Thermostat sensor has not been seen for %s seconds"), result_chr);

  }
}

void ThermostatSignalPostProcessingSlow(uint8_t ctr_output)
{
  // Increate counter when inconsistent output state exists
  if ((Thermostat[ctr_output].status.status_output != Thermostat[ctr_output].status.command_output)
    &&(Thermostat[ctr_output].status.enable_output == IFACE_ON)) {
    Thermostat[ctr_output].diag.output_inconsist_ctr++;
  }
  else {
    Thermostat[ctr_output].diag.output_inconsist_ctr = 0;
  }
}

void ThermostatSignalProcessingFast(uint8_t ctr_output)
{
  // Update real status of the input
  Thermostat[ctr_output].status.status_input = (uint32_t)ThermostatInputStatus(Thermostat[ctr_output].status.input_switch_number);
  // Update timestamp of last input
  if (Thermostat[ctr_output].status.status_input == IFACE_ON) {
    Thermostat[ctr_output].timestamp_input_on = TasmotaGlobal.uptime;
  }
  // Update real status of the output
  Thermostat[ctr_output].status.status_output = (uint32_t)ThermostatOutputStatus(Thermostat[ctr_output].status.output_relay_number);
}

void ThermostatCtrState(uint8_t ctr_output)
{
#ifdef USE_PI_AUTOTUNING
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
#endif //USE_PI_AUTOTUNING

  switch (Thermostat[ctr_output].status.controller_mode) {
    // Hybrid controller (Ramp-up + PI)
    case CTR_HYBRID:
        ThermostatHybridCtrPhase(ctr_output);
      break;
    // PI controller
    case CTR_PI:
#ifdef USE_PI_AUTOTUNING
      // If Autotune has been enabled (via flag)
      // AND we have just reached the setpoint temperature
      // AND the temperature gradient is negative for heating and positive for cooling
      // then switch state to PI autotuning
      if ((Thermostat[ctr_output].status.autotune_flag == AUTOTUNE_ON)
        &&(Thermostat[ctr_output].temp_measured == Thermostat[ctr_output].temp_target_level)
        && ((flag_heating && (Thermostat[ctr_output].temp_measured_gradient < 0))
          ||(!flag_heating && (Thermostat[ctr_output].temp_measured_gradient > 0))))
      {
        Thermostat[ctr_output].status.controller_mode = CTR_PI_AUTOTUNE;
        ThermostatPeakDetectorInit(ctr_output);
      }
#endif // USE_PI_AUTOTUNING
      break;
    // Ramp-up controller (predictive)
    case CTR_RAMP_UP:
      // If ramp-up off time counter has been initialized
      // AND ramp-up off time counter value reached
      if ((Thermostat[ctr_output].time_ctr_checkpoint != 0) &&
          (TasmotaGlobal.uptime >= Thermostat[ctr_output].time_ctr_checkpoint)) {
        // Reset times
        Thermostat[ctr_output].time_ctr_checkpoint = 0;
        Thermostat[ctr_output].timestamp_rampup_start = TasmotaGlobal.uptime;
      }
      break;
#ifdef USE_PI_AUTOTUNING
    // PI autotune
    case CTR_PI_AUTOTUNE:
      // If autotune finalized (flag Off)
      // then go back to the PI controller
      if (Thermostat[ctr_output].status.autotune_flag == AUTOTUNE_OFF)
      {
        Thermostat[ctr_output].status.controller_mode = CTR_PI;
      }
      break;
#endif //USE_PI_AUTOTUNING
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
            && (TasmotaGlobal.uptime >= Thermostat[ctr_output].time_ctr_checkpoint)) {
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
          if (((TasmotaGlobal.uptime - Thermostat[ctr_output].timestamp_output_off) > (60 * (uint32_t)Thermostat[ctr_output].time_allow_rampup))
            && (Thermostat[ctr_output].temp_target_level != Thermostat[ctr_output].temp_target_level_ctr)
            && ( ( (Thermostat[ctr_output].temp_target_level - Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_rampup_delta_in)
                && (flag_heating))
              || ( (Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_target_level > Thermostat[ctr_output].temp_rampup_delta_in)
                && (!flag_heating)))) {
              Thermostat[ctr_output].timestamp_rampup_start = TasmotaGlobal.uptime;
              Thermostat[ctr_output].temp_rampup_start = Thermostat[ctr_output].temp_measured;
              Thermostat[ctr_output].temp_rampup_meas_gradient = 0;
              Thermostat[ctr_output].time_rampup_deadtime = 0;
              Thermostat[ctr_output].counter_rampup_cycles = 1;
              Thermostat[ctr_output].time_ctr_changepoint = 0;
              Thermostat[ctr_output].time_ctr_checkpoint = 0;
              Thermostat[ctr_output].status.phase_hybrid_ctr = CTR_HYBRID_RAMP_UP;
          }
#ifdef USE_PI_AUTOTUNING
          // If Autotune has been enabled (via flag)
          // AND we have just reached the setpoint temperature
          // AND the temperature gradient is negative for heating and positive for cooling
          // then switch state to PI autotuning
          if ((Thermostat[ctr_output].status.autotune_flag == AUTOTUNE_ON)
            &&(Thermostat[ctr_output].temp_measured == Thermostat[ctr_output].temp_target_level)
            && ((flag_heating && (Thermostat[ctr_output].temp_measured_gradient < 0))
              ||(!flag_heating && (Thermostat[ctr_output].temp_measured_gradient > 0))))
          {
            Thermostat[ctr_output].status.phase_hybrid_ctr = CTR_HYBRID_PI_AUTOTUNE;
            ThermostatPeakDetectorInit(ctr_output);
          }
#endif // USE_PI_AUTOTUNING
        break;
#ifdef USE_PI_AUTOTUNING
        // PI autotune controller phase
      case CTR_HYBRID_PI_AUTOTUNE:
        // If autotune finalized (flag Off)
        // then go back to the PI controller
        if (Thermostat[ctr_output].status.autotune_flag == AUTOTUNE_OFF)
        {
          Thermostat[ctr_output].status.phase_hybrid_ctr = CTR_HYBRID_PI;
        }
      break;
#endif // USE_PI_AUTOTUNING
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
    && ((TasmotaGlobal.uptime - Thermostat[ctr_output].timestamp_input_on) > ((uint32_t)Thermostat[ctr_output].time_manual_to_auto * 60))) {
    change_state = true;
  }
  return change_state;
}

void ThermostatEmergencyShutdown(uint8_t ctr_output)
{
  // Emergency switch to THERMOSTAT_OFF
  Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_OFF;
  Thermostat[ctr_output].status.command_output = IFACE_OFF;
  if (Thermostat[ctr_output].status.enable_output == IFACE_ON) {
    ThermostatOutputRelay(ctr_output, Thermostat[ctr_output].status.command_output);
  }
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
    if (Thermostat[ctr_output].status.enable_output == IFACE_ON) {
      ExecuteCommandPower(Thermostat[ctr_output].status.output_relay_number, POWER_ON, SRC_THERMOSTAT);
    }
    Thermostat[ctr_output].status.status_output = IFACE_ON;
#ifdef DEBUG_THERMOSTAT
    ThermostatVirtualSwitch(ctr_output);
#endif // DEBUG_THERMOSTAT
  }
  // If command received to disable output
  // AND current output status is ON
  // then switch output to OFF
  else if ((command == IFACE_OFF) && (Thermostat[ctr_output].status.status_output == IFACE_ON)) {
    if (Thermostat[ctr_output].status.enable_output == IFACE_ON) {
      ExecuteCommandPower(Thermostat[ctr_output].status.output_relay_number, POWER_OFF, SRC_THERMOSTAT);
    }
    Thermostat[ctr_output].timestamp_output_off = TasmotaGlobal.uptime;
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
  // result will be calculated with the accumulated previous error anterior
  // and current error will be accumulated to the previous one
  else {
    // Hysteresis limiter
    // If error is less than or equal than hysteresis, limit output to 0, when temperature
    // is rising, never when falling. Limit accumulated error. If this is not done,
    // there will be very strong control actions from the integral part due to a
    // very high accumulated error when beingin hysteresis. This triggers high
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
    // to the cycle time and error will not be accumulated
    if (Thermostat[ctr_output].time_integral_pi > ((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60)) {
      Thermostat[ctr_output].time_integral_pi = ((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60);
    }
  }

  // Calculate output
  Thermostat[ctr_output].time_total_pi = Thermostat[ctr_output].time_proportional_pi + Thermostat[ctr_output].time_integral_pi;

  // Antiwindup of the output
  // If result is bigger than cycle time, the result will be adjusted
  // to the cylce time minus safety time and error will not be accumulated
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
  // If result is less than the minimum action time, adjust to minimum value
  if ((Thermostat[ctr_output].time_total_pi <= abs(((int32_t)Thermostat[ctr_output].time_min_action * 60)))
    && (Thermostat[ctr_output].time_total_pi != 0)) {
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_min_action * 60);
  }
  // Maximum action limiter
  // If result is more than the maximum action time, adjust to maximum value
  else if (Thermostat[ctr_output].time_total_pi > abs(((int32_t)Thermostat[ctr_output].time_max_action * 60))) {
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_max_action * 60);
  }
  // If switched off less time than safety time, do not switch off
  else if (Thermostat[ctr_output].time_total_pi > (((int32_t)Thermostat[ctr_output].time_pi_cycle * 60) - ((int32_t)Thermostat[ctr_output].time_min_turnoff_action * 60))) {
    Thermostat[ctr_output].time_total_pi = ((int32_t)Thermostat[ctr_output].time_pi_cycle * 60);
  }

  // Adjust output switch point
  Thermostat[ctr_output].time_ctr_changepoint = TasmotaGlobal.uptime + (uint32_t)Thermostat[ctr_output].time_total_pi;
  // Adjust next cycle point
  Thermostat[ctr_output].time_ctr_checkpoint = TasmotaGlobal.uptime + ((uint32_t)Thermostat[ctr_output].time_pi_cycle * 60);
}

void ThermostatWorkAutomaticPI(uint8_t ctr_output)
{
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
  if ( (TasmotaGlobal.uptime >= Thermostat[ctr_output].time_ctr_checkpoint)
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
  if (TasmotaGlobal.uptime < Thermostat[ctr_output].time_ctr_changepoint) {
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
  time_in_rampup = TasmotaGlobal.uptime - Thermostat[ctr_output].timestamp_rampup_start;
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
      Thermostat[ctr_output].time_rampup_nextcycle = TasmotaGlobal.uptime + ((uint32_t)Thermostat[ctr_output].time_rampup_cycle * 60);
      // Set auxiliary variables
      Thermostat[ctr_output].temp_rampup_cycle = Thermostat[ctr_output].temp_measured;
      Thermostat[ctr_output].time_ctr_changepoint = TasmotaGlobal.uptime + (60 * (uint32_t)Thermostat[ctr_output].time_rampup_max);
      Thermostat[ctr_output].temp_rampup_output_off =  Thermostat[ctr_output].temp_target_level_ctr;
    }
    // Gradient calculation every time_rampup_cycle
    else if ((Thermostat[ctr_output].time_rampup_deadtime > 0) && (TasmotaGlobal.uptime >= Thermostat[ctr_output].time_rampup_nextcycle)) {
      // Calculate temp. gradient in º/hour and set again time_rampup_nextcycle and temp_rampup_cycle
      // temp_rampup_meas_gradient = ((3600 * temp_delta_rampup) / (os.time() - time_rampup_nextcycle))
      temp_delta_rampup = Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_rampup_cycle;
      uint32_t time_total_rampup = (uint32_t)Thermostat[ctr_output].time_rampup_cycle * 60 * Thermostat[ctr_output].counter_rampup_cycles;
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
        Thermostat[ctr_output].temp_rampup_output_off = (int16_t)(((int32_t)temp_delta_rampup * (int32_t)(Thermostat[ctr_output].time_ctr_changepoint - (TasmotaGlobal.uptime - (time_total_rampup)))) / (int32_t)(time_total_rampup * Thermostat[ctr_output].counter_rampup_cycles)) + Thermostat[ctr_output].temp_rampup_cycle;
        // Set auxiliary variables
        Thermostat[ctr_output].time_rampup_nextcycle = TasmotaGlobal.uptime + ((uint32_t)Thermostat[ctr_output].time_rampup_cycle * 60);
        Thermostat[ctr_output].temp_rampup_cycle = Thermostat[ctr_output].temp_measured;
        // Reset period counter
        Thermostat[ctr_output].counter_rampup_cycles = 1;
      }
      else {
        // Increase the period counter
        Thermostat[ctr_output].counter_rampup_cycles++;
        // Set another period
        Thermostat[ctr_output].time_rampup_nextcycle = TasmotaGlobal.uptime + ((uint32_t)Thermostat[ctr_output].time_rampup_cycle * 60);
        // Reset time_ctr_changepoint and temp_rampup_output_off
        Thermostat[ctr_output].time_ctr_changepoint = TasmotaGlobal.uptime + (60 * (uint32_t)Thermostat[ctr_output].time_rampup_max) - time_in_rampup;
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
      || (TasmotaGlobal.uptime < Thermostat[ctr_output].time_ctr_changepoint)
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
    Thermostat[ctr_output].time_ctr_checkpoint = TasmotaGlobal.uptime;
    // Switch Off output
    Thermostat[ctr_output].status.command_output = IFACE_OFF;
  }
}

#ifdef USE_PI_AUTOTUNING

void ThermostatPeakDetectorInit(uint8_t ctr_output)
{
  for (uint8_t i = 0; i < THERMOSTAT_PEAKNUMBER_AUTOTUNE; i++) {
    Thermostat[ctr_output].temp_peaks_atune[i] = 0;
  }
  Thermostat[ctr_output].pU_pi_atune = 0;
  Thermostat[ctr_output].kP_pi_atune = 0;
  Thermostat[ctr_output].kI_pi_atune = 0;
  Thermostat[ctr_output].kU_pi_atune = 0;
  Thermostat[ctr_output].peak_ctr = 0;
  Thermostat[ctr_output].temp_abs_max_atune = 0;
  Thermostat[ctr_output].temp_abs_min_atune = 100;
  Thermostat[ctr_output].time_ctr_checkpoint = TasmotaGlobal.uptime + THERMOSTAT_TIME_MAX_AUTOTUNE;
}

void ThermostatPeakDetector(uint8_t ctr_output)
{
  uint8_t peak_num = Thermostat[ctr_output].peak_ctr;
  int16_t peak_avg = 0;
  bool peak_transition = false;
  // Update Max/Min Thermostat[ctr_output].temp_abs_max_atune
  if (Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_abs_max_atune) {
    Thermostat[ctr_output].temp_abs_max_atune = Thermostat[ctr_output].temp_measured;
  }
  if (Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_abs_min_atune) {
    Thermostat[ctr_output].temp_abs_min_atune = Thermostat[ctr_output].temp_measured;
  }
  // For heating, even peak numbers look for maxes, odd for minds, the contrary for cooling
  // If we did not found all peaks yet
  if (peak_num < THERMOSTAT_PEAKNUMBER_AUTOTUNE) {
    bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
    bool cond_peak_1 = (   (Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_peaks_atune[peak_num])
                        && (flag_heating)
                      ||   (Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_peaks_atune[peak_num])
                        && (!flag_heating));
    bool cond_peak_2 = (   (Thermostat[ctr_output].temp_measured < Thermostat[ctr_output].temp_peaks_atune[peak_num])
                        && (flag_heating)
                      ||   (Thermostat[ctr_output].temp_measured > Thermostat[ctr_output].temp_peaks_atune[peak_num])
                        && (!flag_heating));
    bool cond_gradient_1 = (   (Thermostat[ctr_output].temp_measured_gradient > 0)
                            && (flag_heating)
                          ||   (Thermostat[ctr_output].temp_measured_gradient < 0)
                            && (!flag_heating));
    bool cond_gradient_2 = (   (Thermostat[ctr_output].temp_measured_gradient < 0)
                            && (flag_heating)
                          ||   (Thermostat[ctr_output].temp_measured_gradient > 0)
                            && (!flag_heating));
    // If peak number is even (look for max if heating and min if cooling)
    if ((peak_num % 2) == 0) {
      // If current temperature higher (heating) or lower (cooling) than registered value for peak
      // AND temperature gradient > 0 for heating or < 0 for cooling
      // then, update value
      if (cond_peak_1 && cond_gradient_1) {
        Thermostat[ctr_output].temp_peaks_atune[peak_num] = Thermostat[ctr_output].temp_measured;
      }
      // Else if current temperature lower (heating) or higher (cooling) then registered value for peak
      // AND difference to peak is outside of the peak no detection band
      // then the current peak value is the peak (max for heating, min for cooling), switch detection
      if ( (cond_peak_2)
        && (abs(Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_peaks_atune[peak_num]) > Thermostat[ctr_output].temp_band_no_peak_det)) {
        // Register peak timestamp;
        Thermostat[ctr_output].time_peak_timestamps_atune[peak_num] = (TasmotaGlobal.uptime / 60);
        Thermostat[ctr_output].peak_ctr++;
        peak_transition = true;
      }
    }
    // Peak number is odd (look for min if heating and max if cooling)
    else {
      // If current temperature lower (heating) or higher (cooling) than registered value for peak
      // AND temperature gradient < 0 for heating or > 0 for cooling
      // then, update value
      if (cond_peak_2 && cond_gradient_2) {
        Thermostat[ctr_output].temp_peaks_atune[peak_num] = Thermostat[ctr_output].temp_measured;
      }
      // Else if current temperature higher (heating) or lower (cooling) then registered value for peak
      // AND difference to peak is outside of the peak no detection band
      // then the current peak value is the peak (min for heating, max for cooling), switch detection
      if ( (cond_peak_1)
        && (abs(Thermostat[ctr_output].temp_measured - Thermostat[ctr_output].temp_peaks_atune[peak_num]) > Thermostat[ctr_output].temp_band_no_peak_det)) {
        // Calculate period
        // Register peak timestamp;
        Thermostat[ctr_output].time_peak_timestamps_atune[peak_num] = (TasmotaGlobal.uptime / 60);
        Thermostat[ctr_output].peak_ctr++;
        peak_transition = true;
      }
    }
  }
  else {
    // Peak detection done, proceed to evaluate results
    ThermostatAutotuneParamCalc(ctr_output);
    Thermostat[ctr_output].status.autotune_flag = AUTOTUNE_OFF;
  }

  // If peak detection not finalized but bigger than 3 and we have just found a peak, check if results can be extracted
  if ((Thermostat[ctr_output].peak_ctr > 2) && (peak_transition)) {
    //Update peak_num
    peak_num = Thermostat[ctr_output].peak_ctr;
    // Calculate average value among the last 3 peaks
    peak_avg = (abs(Thermostat[ctr_output].temp_peaks_atune[peak_num - 1]
                    - Thermostat[ctr_output].temp_peaks_atune[peak_num - 2])
              + abs(Thermostat[ctr_output].temp_peaks_atune[peak_num - 2]
                    - Thermostat[ctr_output].temp_peaks_atune[peak_num - 3])) / 2;

    if ((20 * (int32_t)peak_avg) < (int32_t)(Thermostat[ctr_output].temp_abs_max_atune - Thermostat[ctr_output].temp_abs_min_atune)) {
      // Calculate average temperature among all peaks
      for (uint8_t i = 0; i < peak_num; i++) {
        peak_avg += Thermostat[ctr_output].temp_peaks_atune[i];
      }
      peak_avg /= peak_num;
      // If last period crosses the average value, result valid
      if (10 * abs(Thermostat[ctr_output].temp_peaks_atune[peak_num - 1] - Thermostat[ctr_output].temp_peaks_atune[peak_num - 2]) < (Thermostat[ctr_output].temp_abs_max_atune - peak_avg)) {
        // Peak detection done, proceed to evaluate results
        ThermostatAutotuneParamCalc(ctr_output);
        Thermostat[ctr_output].status.autotune_flag = AUTOTUNE_OFF;
      }
    }
  }
  peak_transition = false;
}

void ThermostatAutotuneParamCalc(uint8_t ctr_output)
{
  uint8_t peak_num = Thermostat[ctr_output].peak_ctr;

  // Calculate the tunning parameters
  // Resolution increased to avoid float operations
  Thermostat[ctr_output].kU_pi_atune = (uint16_t)(100 * ((uint32_t)400000 * (uint32_t)(Thermostat[ctr_output].dutycycle_step_autotune)) / ((uint32_t)(Thermostat[ctr_output].temp_abs_max_atune - Thermostat[ctr_output].temp_abs_min_atune) * (uint32_t)314159));
  Thermostat[ctr_output].pU_pi_atune = (Thermostat[ctr_output].time_peak_timestamps_atune[peak_num - 1] - Thermostat[ctr_output].time_peak_timestamps_atune[peak_num - 2]);

  switch (Thermostat[ctr_output].status.autotune_perf_mode) {
    case AUTOTUNE_PERF_FAST:
      // Calculate kP/Ki autotune
      Thermostat[ctr_output].kP_pi_atune = (4 * Thermostat[ctr_output].kU_pi_atune) / 10;
      break;
    case AUTOTUNE_PERF_NORMAL:
      // Calculate kP/Ki autotune
      Thermostat[ctr_output].kP_pi_atune = (18 * Thermostat[ctr_output].kU_pi_atune) / 100;
      break;
    case AUTOTUNE_PERF_SLOW:
      // Calculate kP/Ki autotune
      Thermostat[ctr_output].kP_pi_atune = (13 * Thermostat[ctr_output].kU_pi_atune) / 100;
      break;
  }

  // Resolution increased to avoid float operations
  Thermostat[ctr_output].kI_pi_atune = (12 * (6000 * Thermostat[ctr_output].kU_pi_atune / Thermostat[ctr_output].pU_pi_atune)) / 10;

  // Calculate PropBand Autotune
  Thermostat[ctr_output].val_prop_band_atune = 100 / Thermostat[ctr_output].kP_pi_atune;
  // Calculate Reset Time Autotune
  Thermostat[ctr_output].time_reset_atune = (uint32_t)((((uint32_t)Thermostat[ctr_output].kP_pi_atune * (uint32_t)Thermostat[ctr_output].time_pi_cycle * 6000)) / (uint32_t)Thermostat[ctr_output].kI_pi_atune);
}

void ThermostatWorkAutomaticPIAutotune(uint8_t ctr_output)
{
  bool flag_heating = (Thermostat[ctr_output].status.climate_mode == CLIMATE_HEATING);
  // If no timeout of the PI Autotune function
  // AND no change in setpoint
  if ((TasmotaGlobal.uptime < Thermostat[ctr_output].time_ctr_checkpoint)
    &&(Thermostat[ctr_output].temp_target_level_ctr == Thermostat[ctr_output].temp_target_level)) {
    if (TasmotaGlobal.uptime >= Thermostat[ctr_output].time_ctr_checkpoint) {
      Thermostat[ctr_output].temp_target_level_ctr = Thermostat[ctr_output].temp_target_level;
      // Calculate time_ctr_changepoint
      Thermostat[ctr_output].time_ctr_changepoint = TasmotaGlobal.uptime + (((uint32_t)Thermostat[ctr_output].time_pi_cycle * (uint32_t)Thermostat[ctr_output].dutycycle_step_autotune) / (uint32_t)100);
      // Reset cycle active
      Thermostat[ctr_output].status.status_cycle_active = CYCLE_OFF;
    }
    // Set Output On/Off depending on the changepoint
    if (TasmotaGlobal.uptime < Thermostat[ctr_output].time_ctr_changepoint) {
      Thermostat[ctr_output].status.status_cycle_active = CYCLE_ON;
      Thermostat[ctr_output].status.command_output = IFACE_ON;
    }
    else {
      Thermostat[ctr_output].status.command_output = IFACE_OFF;
    }
    // Update peak values
    ThermostatPeakDetector(ctr_output);
  }
  else {
    // Disable Autotune flag
    Thermostat[ctr_output].status.autotune_flag = AUTOTUNE_OFF;
  }

  if (Thermostat[ctr_output].status.autotune_flag == AUTOTUNE_OFF) {
    // Set output Off
    Thermostat[ctr_output].status.command_output = IFACE_OFF;
  }
}
#endif //USE_PI_AUTOTUNING

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
#ifdef USE_PI_AUTOTUNING
        // PI autotune
        case CTR_HYBRID_PI_AUTOTUNE:
          ThermostatWorkAutomaticPIAutotune(ctr_output);
          break;
#endif //USE_PI_AUTOTUNING
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
#ifdef USE_PI_AUTOTUNING
    // PI autotune
    case CTR_PI_AUTOTUNE:
      ThermostatWorkAutomaticPIAutotune(ctr_output);
      break;
#endif //USE_PI_AUTOTUNING
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
  if ((Thermostat[ctr_output].diag.diagnostic_mode == DIAGNOSTIC_ON)
    &&(Thermostat[ctr_output].diag.output_inconsist_ctr >= THERMOSTAT_TIME_MAX_OUTPUT_INCONSIST)) {
    Thermostat[ctr_output].status.thermostat_mode = THERMOSTAT_OFF;
    Thermostat[ctr_output].diag.state_emergency = EMERGENCY_ON;
  }

  // Diagnostic related to the plausibility of the output power implemented
  // already into the energy driver

  // If diagnostics fail, emergency enabled and thermostat shutdown triggered
  if (Thermostat[ctr_output].diag.state_emergency == EMERGENCY_ON) {
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
    && (tempVal <= 2000)
    && (tempVal >= Thermostat[ctr_output].temp_frost_protect)) {
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
#endif // DEBUG_THERMOSTAT

void ThermostatDebug(uint8_t ctr_output)
{
  char ctr_output_chr[FLOATSZ];
  char result_chr[FLOATSZ];
  dtostrfd(ctr_output + 1, 0, ctr_output_chr);
  dtostrfd(Thermostat[ctr_output].status.counter_seconds, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.counter_seconds: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.thermostat_mode, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.thermostat_mode: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].diag.state_emergency, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].diag.state_emergency: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].diag.output_inconsist_ctr, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].diag.output_inconsist_ctr: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.controller_mode, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.controller_mode: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.command_output, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.command_output: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.status_output, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.status_output: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.status_input, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.status_input: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.phase_hybrid_ctr, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.phase_hybrid_ctr: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.sensor_alive, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.sensor_alive: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].status.status_cycle_active, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].status.status_cycle_active: %s"), ctr_output_chr, result_chr);
  dtostrfd((float)Thermostat[ctr_output].temp_pi_error/100, 2, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].temp_pi_error: %s degrees"), ctr_output_chr, result_chr);
  dtostrfd((float)Thermostat[ctr_output].temp_pi_accum_error/100, 2, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].temp_pi_accum_error: %s degrees"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_proportional_pi, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].time_proportional_pi: %s seconds"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_integral_pi, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].time_integral_pi: %s seconds"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_total_pi, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].time_total_pi: %s seconds"), ctr_output_chr, result_chr);
  dtostrfd((float)Thermostat[ctr_output].temp_measured_gradient/1000, 3, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].temp_measured_gradient: %s degrees/hour"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_rampup_deadtime, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].time_rampup_deadtime: %s seconds"), ctr_output_chr, result_chr);
  dtostrfd((float)Thermostat[ctr_output].temp_rampup_meas_gradient/1000, 3, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].temp_rampup_meas_gradient: %s degrees/hour"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_ctr_changepoint, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].time_ctr_changepoint: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].temp_rampup_output_off, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].temp_rampup_output_off: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_ctr_checkpoint, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].time_ctr_checkpoint: %s"), ctr_output_chr, result_chr);
  dtostrfd(TasmotaGlobal.uptime, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "uptime: %s"), result_chr);
  dtostrfd(TasmotaGlobal.power, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_THERMOSTAT "power: %s"), result_chr);
}

void DebugControllerParameters(uint8_t ctr_output)
{
  char ctr_output_chr[FLOATSZ];
  char result_chr[FLOATSZ];
  dtostrfd(ctr_output + 1, 0, ctr_output_chr);
  dtostrfd(Thermostat[ctr_output].status.controller_mode, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].CONTROLLERMODESET: %s"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_pi_cycle, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMEPICYCLESET: %s minutes"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_min_action, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMEMINACTIONSET: %s minutes"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_max_action, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMEMAXACTIONSET: %s minutes"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_allow_rampup, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMEALLOWRAMPUPSET: %s minutes"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_rampup_cycle, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMERAMPUPCYCLESET: %s minutes"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_rampup_max, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMERAMPUPMAXSET: %s minutes"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].time_reset, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TIMERESETSET: %s seconds"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].val_prop_band, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].PROPBANDSET: %s"), ctr_output_chr, result_chr);
  dtostrfd((float)Thermostat[ctr_output].temp_reset_anti_windup/10, 1, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TEMPANTIWINDUPRESETSET: %s degrees"), ctr_output_chr, result_chr);
  dtostrfd((float)Thermostat[ctr_output].temp_hysteresis/10, 1, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TEMPHYSTSET: %s degrees"), ctr_output_chr, result_chr);
  dtostrfd(Thermostat[ctr_output].temp_rampup_delta_in, 0, result_chr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_THERMOSTAT "Thermostat[%s].TEMPRUPDELTINSET: %s degrees"), ctr_output_chr, result_chr);
 }

uint8_t ThermostatGetDutyCycle(uint8_t ctr_output)
{
  uint8_t value = 0;
  if ( (Thermostat[ctr_output].status.controller_mode == CTR_PI)
    || ((Thermostat[ctr_output].status.controller_mode == CTR_HYBRID)
      &&(Thermostat[ctr_output].status.phase_hybrid_ctr == CTR_HYBRID_PI))) {
    value = 100*Thermostat[ctr_output].time_total_pi / ((uint32_t)60*(uint32_t)Thermostat[ctr_output].time_pi_cycle);
  }
  else if ( (Thermostat[ctr_output].status.controller_mode == CTR_RAMP_UP)
        || ((Thermostat[ctr_output].status.controller_mode == CTR_HYBRID)
          &&(Thermostat[ctr_output].status.phase_hybrid_ctr == CTR_HYBRID_RAMP_UP))) {
    if (Thermostat[ctr_output].status.status_output == IFACE_ON) {
      value = 100;
    }
    else {
      value = 0;
    }
  }
  return value;
}

void ThermostatGetLocalSensor(uint8_t ctr_output) {
  String buf = ResponseData();   // copy the string into a new buffer that will be modified
  JsonParser parser((char*)buf.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) {
    String sensor_name = THERMOSTAT_SENSOR_NAME;
    if (  (THERMOSTAT_SENSOR_NUMBER > 1)
        &&(THERMOSTAT_CONTROLLER_OUTPUTS > 1)
        &&(ctr_output < THERMOSTAT_SENSOR_NUMBER)) {
      sensor_name.concat(IndexSeparator() + String(ctr_output + 1));
    }
    JsonParserToken value_token = root[sensor_name].getObject()[PSTR(D_JSON_TEMPERATURE)];
    if (value_token.isNum()) {
      int16_t value = value_token.getFloat() * 10;
      if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
        value = ThermostatFahrenheitToCelsius(value, TEMP_CONV_ABSOLUTE);
      }
      if ( (value >= -1000)
        && (value <= 2000)
        && (Thermostat[ctr_output].status.sensor_type == SENSOR_LOCAL)) {
        uint32_t timestamp = TasmotaGlobal.uptime;
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
      if ((value == THERMOSTAT_OFF) && (Thermostat[ctr_output].status.enable_output == IFACE_ON)) {
        // Make sure the relay is switched to off once if the thermostat is being disabled,
        // or it will get stuck on (danger!)
        Thermostat[ctr_output].status.command_output = IFACE_OFF;
        struct XDRVMAILBOX save_XdrvMailbox = XdrvMailbox;
        ThermostatOutputRelay(ctr_output, Thermostat[ctr_output].status.command_output);
        XdrvMailbox = save_XdrvMailbox;
      }
      if ((value > THERMOSTAT_OFF) && (value < THERMOSTAT_MODES_MAX)) {
        DebugControllerParameters(ctr_output);
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.thermostat_mode);
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
        // Trigger a restart of the controller
        Thermostat[ctr_output].time_ctr_checkpoint = TasmotaGlobal.uptime;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.climate_mode);
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
      if ( (value >= -1000)
        && (value <= 2000)) {
        Thermostat[ctr_output].temp_frost_protect = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_frost_protect, TEMP_CONV_ABSOLUTE);
    }
    else {
      value = Thermostat[ctr_output].temp_frost_protect;
    }
    ResponseCmndIdxFloat((float)value / 10, 1);
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
        // Reset controller variables
        Thermostat[ctr_output].timestamp_rampup_start = TasmotaGlobal.uptime;
        Thermostat[ctr_output].temp_rampup_start = Thermostat[ctr_output].temp_measured;
        Thermostat[ctr_output].temp_rampup_meas_gradient = 0;
        Thermostat[ctr_output].time_rampup_deadtime = 0;
        Thermostat[ctr_output].counter_rampup_cycles = 1;
        Thermostat[ctr_output].time_ctr_changepoint = 0;
        Thermostat[ctr_output].time_ctr_checkpoint = 0;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.controller_mode);
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
        Thermostat[ctr_output].timestamp_input_on = TasmotaGlobal.uptime;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.input_switch_number);
  }
}

void CmndInputSwitchUse(void)
{
  if ((XdrvMailbox.index >= INPUT_NOT_USED) && (XdrvMailbox.index <= INPUT_USED)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      Thermostat[ctr_output].status.use_input = (uint32_t)(XdrvMailbox.payload);
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.use_input);
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
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.sensor_type);
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
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.output_relay_number);
  }
}

void CmndTimeAllowRampupSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value < 1440)) {
        Thermostat[ctr_output].time_allow_rampup = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_allow_rampup));
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
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.temp_format);
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
        && (value <= 2000)
        && (Thermostat[ctr_output].status.sensor_type == SENSOR_MQTT)) {
        uint32_t timestamp = TasmotaGlobal.uptime;
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
    ResponseCmndIdxFloat((float)value / 10, 1);
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
        && (value <= 2000)
        && (value >= Thermostat[ctr_output].temp_frost_protect)) {
        Thermostat[ctr_output].temp_target_level = value;
      }
    }
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_target_level, TEMP_CONV_ABSOLUTE);
    }
    else {
      value = Thermostat[ctr_output].temp_target_level;
    }
    ResponseCmndIdxFloat((float)value / 10, 1);
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
    ResponseCmndIdxFloat(((float)value) / 1000, 1);
  }
}

void CmndStateEmergencySet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1)) {
        Thermostat[ctr_output].diag.state_emergency = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].diag.state_emergency);
  }
}

void CmndTimeManualToAutoSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_manual_to_auto = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_manual_to_auto));
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
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].val_prop_band);
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
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].time_reset);
  }
}

void CmndTimePiProportRead(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].time_proportional_pi);
  }
}

void CmndTimePiIntegrRead(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].time_integral_pi);
  }
}

void CmndTimePiCycleSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_pi_cycle = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_pi_cycle));
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
    ResponseCmndIdxFloat((float)value / 10, 1);
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
    ResponseCmndIdxFloat((float)value / 10, 1);
  }
}

#ifdef USE_PI_AUTOTUNING
void CmndPerfLevelAutotune(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= AUTOTUNE_PERF_MAX)) {
        Thermostat[ctr_output].status.autotune_perf_mode = value;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.autotune_perf_mode);
  }
}
#endif // USE_PI_AUTOTUNING

void CmndTimeMaxActionSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_max_action = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_max_action));
  }
}

void CmndTimeMinActionSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_min_action = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_min_action));
  }
}

void CmndTimeSensLostSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_sens_lost = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_sens_lost));
  }
}

void CmndTimeMinTurnoffActionSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_min_turnoff_action = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_min_turnoff_action));
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
    ResponseCmndIdxFloat((float)value / 10, 1);
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
    ResponseCmndIdxFloat((float)value / 10, 1);
  }
}

void CmndTimeRampupMaxSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_rampup_max = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)((uint32_t)Thermostat[ctr_output].time_rampup_max));
  }
}

void CmndTimeRampupCycleSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint32_t value = (uint32_t)(XdrvMailbox.payload);
      if ((value >= 0) && (value <= 1440)) {
        Thermostat[ctr_output].time_rampup_cycle = (uint16_t)value;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].time_rampup_cycle);
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
    ResponseCmndIdxFloat((float)value / 100, 1);
  }
}

void CmndDiagnosticModeSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
      if ((value >= DIAGNOSTIC_OFF) && (value <= DIAGNOSTIC_ON)) {
        Thermostat[ctr_output].diag.diagnostic_mode = value;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].diag.diagnostic_mode);
  }
}

void CmndCtrDutyCycleRead(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;

        ResponseCmndIdxNumber((int)ThermostatGetDutyCycle(ctr_output) );
  }
}

void CmndEnableOutputSet(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= THERMOSTAT_CONTROLLER_OUTPUTS)) {
    uint8_t ctr_output = XdrvMailbox.index - 1;
    if (XdrvMailbox.data_len > 0) {
      uint8_t value = (uint8_t)(CharToFloat(XdrvMailbox.data));
      if ((value >= IFACE_OFF) && (value <= IFACE_ON)) {
        Thermostat[ctr_output].status.enable_output = value;
      }
    }
    ResponseCmndIdxNumber((int)Thermostat[ctr_output].status.enable_output);
  }
}



/*********************************************************************************************\
 * Web UI
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_THERMOSTAT_INFO[]           PROGMEM = "{s}" D_THERMOSTAT " %d{m}%s{e}";
const char HTTP_THERMOSTAT_TEMPERATURE[]    PROGMEM = "{s}%s{m}%*_f " D_UNIT_DEGREE "%c{e}";
const char HTTP_THERMOSTAT_TEMP_GRAD[]      PROGMEM = "{s}" D_THERMOSTAT_GRADIENT "{m}%c%*_f " D_UNIT_DEGREE "%c/" D_UNIT_HOUR "{e}";
const char HTTP_THERMOSTAT_DUTY_CYCLE[]     PROGMEM = "{s}" D_THERMOSTAT_DUTY_CYCLE "{m}%d " D_UNIT_PERCENT "{e}";
const char HTTP_THERMOSTAT_CYCLE_TIME[]     PROGMEM = "{s}" D_THERMOSTAT_CYCLE_TIME "{m}%d " D_UNIT_MINUTE "{e}";
const char HTTP_THERMOSTAT_CONTROL_METHOD[] PROGMEM = "{s}" D_THERMOSTAT_CONTROL_METHOD "{m}%s{e}";
const char HTTP_THERMOSTAT_PI_AUTOTUNE[]    PROGMEM = "{s}" D_THERMOSTAT_PI_AUTOTUNE "{m}%s{e}";

#endif  // USE_WEBSERVER

#define D_THERMOSTAT_JSON_NAME_CONTROL_METHOD "ControlMethod"
#define D_THERMOSTAT_JSON_NAME_HYBRID_CONTROLLER_PHASE "HybridControllerPhase"
#define D_THERMOSTAT_JSON_NAME_EMERGENCY_STATE "EmergencyState"

void ThermostatShow(uint8_t ctr_output, bool json)
{
  if (json) {
    float f_target_temp = Thermostat[ctr_output].temp_target_level / 10.0f;
    ResponseAppend_P(PSTR(",\"Thermostat%i\":{"), ctr_output + 1);
    ResponseAppend_P(PSTR("%s\"%s\":%i"), "", D_CMND_THERMOSTATMODESET, Thermostat[ctr_output].status.thermostat_mode);
    ResponseAppend_P(PSTR("%s\"%s\":%2_f"), ",", D_CMND_TEMPTARGETSET, &f_target_temp);
    ResponseAppend_P(PSTR("%s\"%s\":%i"), ",", D_CMND_CTRDUTYCYCLEREAD, ThermostatGetDutyCycle(ctr_output));
    ResponseAppend_P(PSTR("%s\"%s\":%i"), ",", D_THERMOSTAT_JSON_NAME_CONTROL_METHOD, Thermostat[ctr_output].status.controller_mode);
    ResponseAppend_P(PSTR("%s\"%s\":%i"), ",", D_THERMOSTAT_JSON_NAME_HYBRID_CONTROLLER_PHASE, Thermostat[ctr_output].status.phase_hybrid_ctr);
    ResponseAppend_P(PSTR("%s\"%s\":%i"), ",", D_THERMOSTAT_JSON_NAME_EMERGENCY_STATE, Thermostat[ctr_output].diag.state_emergency == EMERGENCY_ON);
    ResponseJsonEnd();
    return;
  }
#ifdef USE_WEBSERVER

  if (ctr_output) WSContentSend_P(HTTP_SNS_HR_THIN);

  if (Thermostat[ctr_output].status.thermostat_mode == THERMOSTAT_OFF) {
    WSContentSend_P(HTTP_THERMOSTAT_INFO, ctr_output + 1, D_DISABLED );

  } else {
    char c_unit = Thermostat[ctr_output].status.temp_format==TEMP_CELSIUS ? D_UNIT_CELSIUS[0] : D_UNIT_FAHRENHEIT[0];
    float f_temperature;

    WSContentSend_P(HTTP_THERMOSTAT_INFO, ctr_output + 1, D_ENABLED);

    f_temperature = Thermostat[ctr_output].temp_target_level / 10.0f;
    WSContentSend_PD(HTTP_THERMOSTAT_TEMPERATURE, D_THERMOSTAT_SET_POINT, Settings->flag2.temperature_resolution, &f_temperature, c_unit);

    f_temperature = Thermostat[ctr_output].temp_measured / 10.0f;
    WSContentSend_PD(HTTP_THERMOSTAT_TEMPERATURE, D_THERMOSTAT_SENSOR, Settings->flag2.temperature_resolution, &f_temperature, c_unit);

    int16_t value = Thermostat[ctr_output].temp_measured_gradient;
    if (Thermostat[ctr_output].status.temp_format == TEMP_FAHRENHEIT) {
      value = ThermostatCelsiusToFahrenheit((int32_t)Thermostat[ctr_output].temp_measured_gradient, TEMP_CONV_RELATIVE);
    }
    f_temperature = abs(value) / 1000.0f;
    WSContentSend_PD(HTTP_THERMOSTAT_TEMP_GRAD, value < 0 ? '-' : '+', Settings->flag2.temperature_resolution, &f_temperature, c_unit);

    WSContentSend_P(HTTP_THERMOSTAT_DUTY_CYCLE, ThermostatGetDutyCycle(ctr_output));

    switch (Thermostat[ctr_output].status.controller_mode) {
      case CTR_HYBRID:
        switch (Thermostat[ctr_output].status.phase_hybrid_ctr) {
          case CTR_HYBRID_RAMP_UP:
            WSContentSend_P(HTTP_THERMOSTAT_CONTROL_METHOD, D_THERMOSTAT_RAMP_UP_HYBRID);
            break;
          case CTR_HYBRID_PI:
            WSContentSend_P(HTTP_THERMOSTAT_CONTROL_METHOD, D_THERMOSTAT_PI_HYBRID);
            break;
        #ifdef USE_PI_AUTOTUNING
          case CTR_HYBRID_PI_AUTOTUNE:
            WSContentSend_P(HTTP_THERMOSTAT_CONTROL_METHOD, D_THERMOSTAT_AUTOTUNE_HYBRID);
            break;
        #endif
        }
        break;
      case CTR_PI:
        WSContentSend_P(HTTP_THERMOSTAT_CONTROL_METHOD, D_THERMOSTAT_PI);
        break;
      case CTR_RAMP_UP:
        WSContentSend_P(HTTP_THERMOSTAT_CONTROL_METHOD, D_THERMOSTAT_RAMP_UP);
        break;
    #ifdef USE_PI_AUTOTUNING
      case CTR_PI_AUTOTUNE:
        WSContentSend_P(HTTP_THERMOSTAT_CONTROL_METHOD, D_THERMOSTAT_AUTOTUNE);
        break;
    #endif
      }

    WSContentSend_P(HTTP_THERMOSTAT_CYCLE_TIME, Thermostat[ctr_output].time_pi_cycle);

  #ifdef USE_PI_AUTOTUNING
    WSContentSend_P(HTTP_THERMOSTAT_PI_AUTOTUNE, D_ENABLED);
  #else
    WSContentSend_P(HTTP_THERMOSTAT_PI_AUTOTUNE, D_DISABLED);
  #endif
  }
#endif  // USE_WEBSERVER
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv39(uint32_t function)
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
          ThermostatDebug(ctr_output);
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
    case FUNC_JSON_APPEND:
      for (ctr_output = 0; ctr_output < THERMOSTAT_CONTROLLER_OUTPUTS; ctr_output++) {
        ThermostatShow(ctr_output, true);
      }
      break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      for (ctr_output = 0; ctr_output < THERMOSTAT_CONTROLLER_OUTPUTS; ctr_output++) {
        ThermostatShow(ctr_output, false);
      }
      break;
#endif  // USE_WEBSERVER

    case FUNC_COMMAND:
      result = DecodeCommand(kThermostatCommands, ThermostatCommand);
      break;

    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif // USE_THERMOSTAT
