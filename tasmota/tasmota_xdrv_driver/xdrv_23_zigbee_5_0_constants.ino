/*
  xdrv_23_zigbee_5__constants.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

// Below is a compilation of Strings used in Zigbee commands and converters.
// Instead of using pointer to strings (4 bytes), we are using an offset (16 bits)
// into an array of strings - which leads to a 1/3 more compact structure.

// To generate the code below use https://tasmota.hadinger.fr/util
// and copy/paste the entire arrays `Z_PostProcess` and `Z_Commands` concatenated
// Note: the 'C' syntax is irrelevant, the parser only looks for `Z_(<string>)`

// In addition the Python3 code used is below:

/*Python code to generate code below

import re
pat = r"Z\(([^\)]+)\)"      # extract text in Z() macro

def clean(s):
    return s.strip(" \t\n\r")

def strings_to_pmem(arg):
    #strings = arg.split("\n")
    strings = re.findall(pat, arg)

    # do some basic cleaning
    strings_cleaned = [ clean(x) for x in strings if clean(x) != ""]

    # remove duplicates
    strings_cleaned = list(dict.fromkeys(strings_cleaned))

    out_s = "const char Z_strings[] PROGMEM = \n"
    out_i = "enum Z_offsets {\n"

    index = 0;
    # add a first empty string
    out_s += "  \"\\x00\"\n"
    out_i +=  "  Zo_ = " + str(index) + ",\n"
    index += 1

    for s in strings_cleaned:
        out_s += "  \"" + s + "\" \"\\x00\"\n"
        out_i += "  Zo_" + s + " = " + str(index) + ",\n"
        index += len(s) + 1 # add one for null char

    out_s += "  \"\\x00\";"
    out_i += "};"

    return ("", out_s, out_i)


*/

/*
  DO NOT EDIT
*/

const char Z_strings[] PROGMEM =
  "\x00"
  "00" "\x00"
  "00190200" "\x00"
  "00xx0A00" "\x00"
  "00xxxx000000000000" "\x00"
  "01" "\x00"
  "0101" "\x00"
  "01190200" "\x00"
  "01xx0A00" "\x00"
  "01xxxx" "\x00"
  "01xxxx000000000000" "\x00"
  "01xxxx0A0000000000" "\x00"
  "03xx0A00" "\x00"
  "03xxxx000000000000" "\x00"
  "03xxxx0A0000000000" "\x00"
  "ACActivePowerOverload" "\x00"
  "ACAlarmsMask" "\x00"
  "ACCurrentDivisor" "\x00"
  "ACCurrentMultiplier" "\x00"
  "ACCurrentOverload" "\x00"
  "ACFrequency" "\x00"
  "ACFrequencyDivisor" "\x00"
  "ACFrequencyMax" "\x00"
  "ACFrequencyMin" "\x00"
  "ACFrequencyMultiplier" "\x00"
  "ACPowerDivisor" "\x00"
  "ACPowerMultiplier" "\x00"
  "ACReactivePowerOverload" "\x00"
  "ACVoltageDivisor" "\x00"
  "ACVoltageMultiplier" "\x00"
  "ACVoltageOverload" "\x00"
  "AbsMaxCoolSetpointLimit" "\x00"
  "AbsMaxHeatSetpointLimit" "\x00"
  "AbsMinCoolSetpointLimit" "\x00"
  "AbsMinHeatSetpointLimit" "\x00"
  "AccelerationTimeLift" "\x00"
  "ActiveCurrent" "\x00"
  "ActivePower" "\x00"
  "ActivePowerMax" "\x00"
  "ActivePowerMin" "\x00"
  "ActuatorEnabled" "\x00"
  "AddGroup" "\x00"
  "AddScene" "\x00"
  "AlarmCount" "\x00"
  "AlarmMask" "\x00"
  "AnalogApplicationType" "\x00"
  "AnalogDescription" "\x00"
  "AnalogEngineeringUnits" "\x00"
  "AnalogInApplicationType" "\x00"
  "AnalogInDescription" "\x00"
  "AnalogInEngineeringUnits" "\x00"
  "AnalogInMaxValue" "\x00"
  "AnalogInMinValue" "\x00"
  "AnalogInOutOfService" "\x00"
  "AnalogInReliability" "\x00"
  "AnalogInResolution" "\x00"
  "AnalogInStatusFlags" "\x00"
  "AnalogOutApplicationType" "\x00"
  "AnalogOutDescription" "\x00"
  "AnalogOutEngineeringUnits" "\x00"
  "AnalogOutMaxValue" "\x00"
  "AnalogOutMinValue" "\x00"
  "AnalogOutOfService" "\x00"
  "AnalogOutOutOfService" "\x00"
  "AnalogOutReliability" "\x00"
  "AnalogOutRelinquishDefault" "\x00"
  "AnalogOutResolution" "\x00"
  "AnalogOutStatusFlags" "\x00"
  "AnalogOutValue" "\x00"
  "AnalogPriorityArray" "\x00"
  "AnalogReliability" "\x00"
  "AnalogRelinquishDefault" "\x00"
  "AnalogStatusFlags" "\x00"
  "AnalogValue" "\x00"
  "AppVersion" "\x00"
  "ApparentPower" "\x00"
  "AqaraAccelerometer" "\x00"
  "AqaraRotate" "\x00"
  "AqaraVibration505" "\x00"
  "AqaraVibrationMode" "\x00"
  "AqaraVibrationsOrAngle" "\x00"
  "Aqara_FF05" "\x00"
  "ArrowClick" "\x00"
  "ArrowHold" "\x00"
  "ArrowRelease" "\x00"
  "AutoRelockTime" "\x00"
  "AvailablePower" "\x00"
  "AverageRMSOverVoltage" "\x00"
  "AverageRMSOverVoltageCounter" "\x00"
  "AverageRMSUnderVoltage" "\x00"
  "AverageRMSUnderVoltageCounter" "\x00"
  "AverageRMSVoltageMeasurementPeriod" "\x00"
  "BallastFactorAdjustment" "\x00"
  "BallastLampQuantity" "\x00"
  "BallastMaxLevel" "\x00"
  "BallastMinLevel" "\x00"
  "BallastPhysicalMaxLevel" "\x00"
  "BallastPhysicalMinLevel" "\x00"
  "BallastPowerOnFadeTime" "\x00"
  "BallastPowerOnLevel" "\x00"
  "BallastStatus" "\x00"
  "BatteryAHrRating" "\x00"
  "BatteryAlarmMask" "\x00"
  "BatteryAlarmState" "\x00"
  "BatteryManufacturer" "\x00"
  "BatteryPercentage" "\x00"
  "BatteryPercentageMinThreshold" "\x00"
  "BatteryPercentageThreshold1" "\x00"
  "BatteryPercentageThreshold2" "\x00"
  "BatteryPercentageThreshold3" "\x00"
  "BatteryQuantity" "\x00"
  "BatteryRatedVoltage" "\x00"
  "BatterySize" "\x00"
  "BatteryVoltage" "\x00"
  "BatteryVoltageMinThreshold" "\x00"
  "BatteryVoltageThreshold1" "\x00"
  "BatteryVoltageThreshold2" "\x00"
  "BatteryVoltageThreshold3" "\x00"
  "BinaryActiveText" "\x00"
  "BinaryApplicationType" "\x00"
  "BinaryDescription" "\x00"
  "BinaryInActiveText" "\x00"
  "BinaryInApplicationType" "\x00"
  "BinaryInDescription" "\x00"
  "BinaryInInactiveText" "\x00"
  "BinaryInOutOfService" "\x00"
  "BinaryInPolarity" "\x00"
  "BinaryInReliability" "\x00"
  "BinaryInStatusFlags" "\x00"
  "BinaryInValue" "\x00"
  "BinaryInactiveText" "\x00"
  "BinaryMinimumOffTime" "\x00"
  "BinaryMinimumOnTime" "\x00"
  "BinaryOutActiveText" "\x00"
  "BinaryOutApplicationType" "\x00"
  "BinaryOutDescription" "\x00"
  "BinaryOutInactiveText" "\x00"
  "BinaryOutMinimumOffTime" "\x00"
  "BinaryOutMinimumOnTime" "\x00"
  "BinaryOutOfService" "\x00"
  "BinaryOutOutOfService" "\x00"
  "BinaryOutPolarity" "\x00"
  "BinaryOutReliability" "\x00"
  "BinaryOutRelinquishDefault" "\x00"
  "BinaryOutStatusFlags" "\x00"
  "BinaryOutValue" "\x00"
  "BinaryReliability" "\x00"
  "BinaryRelinquishDefault" "\x00"
  "BinaryStatusFlags" "\x00"
  "BinaryValue" "\x00"
  "CIE" "\x00"
  "CO" "\x00"
  "CT" "\x00"
  "CalculationPeriod" "\x00"
  "CheckinInterval" "\x00"
  "CheckinIntervalMin" "\x00"
  "ClosedLimit" "\x00"
  "Color" "\x00"
  "ColorCapabilities" "\x00"
  "ColorLoopActive" "\x00"
  "ColorLoopDirection" "\x00"
  "ColorLoopStartEnhancedHue" "\x00"
  "ColorLoopStoredEnhancedHue" "\x00"
  "ColorLoopTime" "\x00"
  "ColorMode" "\x00"
  "ColorMove" "\x00"
  "ColorPointBIntensity" "\x00"
  "ColorPointBX" "\x00"
  "ColorPointBY" "\x00"
  "ColorPointGIntensity" "\x00"
  "ColorPointGX" "\x00"
  "ColorPointGY" "\x00"
  "ColorPointRIntensity" "\x00"
  "ColorPointRX" "\x00"
  "ColorPointRY" "\x00"
  "ColorStep" "\x00"
  "ColorTempMove" "\x00"
  "ColorTempMoveDown" "\x00"
  "ColorTempMoveStop" "\x00"
  "ColorTempMoveUp" "\x00"
  "ColorTempPhysicalMaxMireds" "\x00"
  "ColorTempPhysicalMinMireds" "\x00"
  "ColorTempStep" "\x00"
  "ColorTempStepDown" "\x00"
  "ColorTempStepUp" "\x00"
  "CompanyName" "\x00"
  "CompensationText" "\x00"
  "ConfigStatus" "\x00"
  "Contact" "\x00"
  "ControlSequenceOfOperation" "\x00"
  "Coordinate1" "\x00"
  "Coordinate2" "\x00"
  "Coordinate3" "\x00"
  "CurrentGroup" "\x00"
  "CurrentPositionLift" "\x00"
  "CurrentPositionLiftPercentage" "\x00"
  "CurrentPositionTilt" "\x00"
  "CurrentPositionTiltPercentage" "\x00"
  "CurrentScene" "\x00"
  "CurrentTemperature" "\x00"
  "CurrentTemperatureSetPoint" "\x00"
  "CurrentZoneSensitivityLevel" "\x00"
  "CustomerName" "\x00"
  "DCCurrent" "\x00"
  "DCCurrentDivisor" "\x00"
  "DCCurrentMax" "\x00"
  "DCCurrentMin" "\x00"
  "DCCurrentMultiplier" "\x00"
  "DCCurrentOverload" "\x00"
  "DCOverloadAlarmsMask" "\x00"
  "DCPower" "\x00"
  "DCPowerDivisor" "\x00"
  "DCPowerMax" "\x00"
  "DCPowerMin" "\x00"
  "DCPowerMultiplier" "\x00"
  "DCVoltage" "\x00"
  "DCVoltageDivisor" "\x00"
  "DCVoltageMax" "\x00"
  "DCVoltageMin" "\x00"
  "DCVoltageMultiplier" "\x00"
  "DCVoltageOverload" "\x00"
  "DataQualityID" "\x00"
  "DateCode" "\x00"
  "DecelerationTimeLift" "\x00"
  "DefaultMoveRate" "\x00"
  "DehumidificationCooling" "\x00"
  "DehumidificationHysteresis" "\x00"
  "DehumidificationLockout" "\x00"
  "DehumidificationMaxCool" "\x00"
  "DeviceEnabled" "\x00"
  "DeviceTempAlarmMask" "\x00"
  "Dimmer" "\x00"
  "DimmerDown" "\x00"
  "DimmerMove" "\x00"
  "DimmerOptions" "\x00"
  "DimmerRemainingTime" "\x00"
  "DimmerStep" "\x00"
  "DimmerStepDown" "\x00"
  "DimmerStepUp" "\x00"
  "DimmerStop" "\x00"
  "DimmerUp" "\x00"
  "DisableLocalConfig" "\x00"
  "DoorClosedEvents" "\x00"
  "DoorOpenEvents" "\x00"
  "DoorState" "\x00"
  "DriftCompensation" "\x00"
  "DstEnd" "\x00"
  "DstShift" "\x00"
  "DstStart" "\x00"
  "ElectricalMeasurementType" "\x00"
  "EnergyFormatting" "\x00"
  "EnergyRemote" "\x00"
  "EnergyTotal" "\x00"
  "EnhancedColorMode" "\x00"
  "EnhancedCurrentHue" "\x00"
  "EurotronicErrors" "\x00"
  "EurotronicHostFlags" "\x00"
  "FanMode" "\x00"
  "FanModeSequence" "\x00"
  "FastPollTimeout" "\x00"
  "FastPollTimeoutMax" "\x00"
  "Fire" "\x00"
  "FlowMaxMeasuredValue" "\x00"
  "FlowMinMeasuredValue" "\x00"
  "FlowRate" "\x00"
  "FlowTolerance" "\x00"
  "GenericDeviceClass" "\x00"
  "GenericDeviceType" "\x00"
  "GetAllGroups" "\x00"
  "GetGroup" "\x00"
  "GetSceneMembership" "\x00"
  "GlassBreak" "\x00"
  "GroupNameSupport" "\x00"
  "HVACSystemTypeConfiguration" "\x00"
  "HWVersion" "\x00"
  "HarmonicCurrentMultiplier" "\x00"
  "HighTempDwellTripPoint" "\x00"
  "HighTempThreshold" "\x00"
  "Hue" "\x00"
  "HueMove" "\x00"
  "HueSat" "\x00"
  "HueStep" "\x00"
  "HueStepDown" "\x00"
  "HueStepUp" "\x00"
  "Humidity" "\x00"
  "HumidityMaxMeasuredValue" "\x00"
  "HumidityMinMeasuredValue" "\x00"
  "HumidityTolerance" "\x00"
  "IASCIEAddress" "\x00"
  "Identify" "\x00"
  "IdentifyQuery" "\x00"
  "IdentifyTime" "\x00"
  "Illuminance" "\x00"
  "IlluminanceLevelStatus" "\x00"
  "IlluminanceLightSensorType" "\x00"
  "IlluminanceMaxMeasuredValue" "\x00"
  "IlluminanceMinMeasuredValue" "\x00"
  "IlluminanceTargetLevel" "\x00"
  "IlluminanceTolerance" "\x00"
  "InstalledClosedLimitLift" "\x00"
  "InstalledClosedLimitTilt" "\x00"
  "InstalledOpenLimitLift" "\x00"
  "InstalledOpenLimitTilt" "\x00"
  "IntermediateSetpointsLift" "\x00"
  "IntermediateSetpointsTilt" "\x00"
  "IntrinsicBallastFactor" "\x00"
  "LampAlarmMode" "\x00"
  "LampBurnHours" "\x00"
  "LampBurnHoursTripPoint" "\x00"
  "LampManufacturer" "\x00"
  "LampRatedHours" "\x00"
  "LampType" "\x00"
  "LastConfiguredBy" "\x00"
  "LastMessageLQI" "\x00"
  "LastMessageRSSI" "\x00"
  "LastSetTime" "\x00"
  "LegrandHeatingMode" "\x00"
  "LegrandMode" "\x00"
  "LegrandOpt1" "\x00"
  "LegrandOpt2" "\x00"
  "LegrandOpt3" "\x00"
  "LidlPower" "\x00"
  "LineCurrent" "\x00"
  "LocalTemperature" "\x00"
  "LocalTemperatureCalibration" "\x00"
  "LocalTime" "\x00"
  "LocationAge" "\x00"
  "LocationDescription" "\x00"
  "LocationMethod" "\x00"
  "LocationPower" "\x00"
  "LocationType" "\x00"
  "LockAlarmMask" "\x00"
  "LockDefaultConfigurationRegister" "\x00"
  "LockEnableInsideStatusLED" "\x00"
  "LockEnableLocalProgramming" "\x00"
  "LockEnableLogging" "\x00"
  "LockEnableOneTouchLocking" "\x00"
  "LockEnablePrivacyModeButton" "\x00"
  "LockKeypadOperationEventMask" "\x00"
  "LockKeypadProgrammingEventMask" "\x00"
  "LockLEDSettings" "\x00"
  "LockLanguage" "\x00"
  "LockManualOperationEventMask" "\x00"
  "LockOperatingMode" "\x00"
  "LockRFIDOperationEventMask" "\x00"
  "LockRFIDProgrammingEventMask" "\x00"
  "LockRFOperationEventMask" "\x00"
  "LockRFProgrammingEventMask" "\x00"
  "LockSoundVolume" "\x00"
  "LockState" "\x00"
  "LockSupportedOperatingModes" "\x00"
  "LockType" "\x00"
  "LongPollInterval" "\x00"
  "LongPollIntervalMin" "\x00"
  "LowTempDwellTripPoint" "\x00"
  "LowTempThreshold" "\x00"
  "MainsAlarmMask" "\x00"
  "MainsFrequency" "\x00"
  "MainsVoltage" "\x00"
  "MainsVoltageDwellTripPoint" "\x00"
  "MainsVoltageMaxThreshold" "\x00"
  "MainsVoltageMinThreshold" "\x00"
  "Manufacturer" "\x00"
  "MaxCoolSetpointLimit" "\x00"
  "MaxHeatSetpointLimit" "\x00"
  "MaxPINCodeLength" "\x00"
  "MaxRFIDCodeLength" "\x00"
  "MaxTempExperienced" "\x00"
  "Measured11thHarmonicCurrent" "\x00"
  "Measured1stHarmonicCurrent" "\x00"
  "Measured3rdHarmonicCurrent" "\x00"
  "Measured5thHarmonicCurrent" "\x00"
  "Measured7thHarmonicCurrent" "\x00"
  "Measured9thHarmonicCurrent" "\x00"
  "MeasuredPhase11thHarmonicCurrent" "\x00"
  "MeasuredPhase1stHarmonicCurrent" "\x00"
  "MeasuredPhase3rdHarmonicCurrent" "\x00"
  "MeasuredPhase5thHarmonicCurrent" "\x00"
  "MeasuredPhase7thHarmonicCurrent" "\x00"
  "MeasuredPhase9thHarmonicCurrent" "\x00"
  "MeterTypeID" "\x00"
  "MinCoolSetpointLimit" "\x00"
  "MinHeatSetpointLimit" "\x00"
  "MinPINCodeLength" "\x00"
  "MinRFIDCodeLength" "\x00"
  "MinSetpointDeadBand" "\x00"
  "MinTempExperienced" "\x00"
  "Mode" "\x00"
  "Model" "\x00"
  "ModelId" "\x00"
  "MotorStepSize" "\x00"
  "Movement" "\x00"
  "MullerLightMode" "\x00"
  "MultiApplicationType" "\x00"
  "MultiDescription" "\x00"
  "MultiInApplicationType" "\x00"
  "MultiInDescription" "\x00"
  "MultiInNumberOfStates" "\x00"
  "MultiInOutOfService" "\x00"
  "MultiInReliability" "\x00"
  "MultiInStatusFlags" "\x00"
  "MultiInValue" "\x00"
  "MultiNumberOfStates" "\x00"
  "MultiOutApplicationType" "\x00"
  "MultiOutDescription" "\x00"
  "MultiOutNumberOfStates" "\x00"
  "MultiOutOfService" "\x00"
  "MultiOutOutOfService" "\x00"
  "MultiOutReliability" "\x00"
  "MultiOutRelinquishDefault" "\x00"
  "MultiOutStatusFlags" "\x00"
  "MultiOutValue" "\x00"
  "MultiReliability" "\x00"
  "MultiRelinquishDefault" "\x00"
  "MultiStatusFlags" "\x00"
  "MultiValue" "\x00"
  "MultipleScheduling" "\x00"
  "NeutralCurrent" "\x00"
  "NumberOfDevices" "\x00"
  "NumberOfHolidaySchedulesSupported" "\x00"
  "NumberOfLogRecordsSupported" "\x00"
  "NumberOfPINUsersSupported" "\x00"
  "NumberOfPrimaries" "\x00"
  "NumberOfRFIDUsersSupported" "\x00"
  "NumberOfResets" "\x00"
  "NumberOfTotalUsersSupported" "\x00"
  "NumberOfWeekDaySchedulesSupportedPerUser" "\x00"
  "NumberOfYearDaySchedulesSupportedPerUser" "\x00"
  "NumberOfZoneSensitivityLevelsSupported" "\x00"
  "NumberRSSIMeasurements" "\x00"
  "NumberofActuationsLift" "\x00"
  "NumberofActuationsTilt" "\x00"
  "Occupancy" "\x00"
  "OccupancySensorType" "\x00"
  "OccupiedCoolingSetpoint" "\x00"
  "OccupiedHeatingSetpoint" "\x00"
  "OffTransitionTime" "\x00"
  "OffWaitTime" "\x00"
  "OnLevel" "\x00"
  "OnOff" "\x00"
  "OnOffTransitionTime" "\x00"
  "OnTime" "\x00"
  "OnTransitionTime" "\x00"
  "OpenPeriod" "\x00"
  "OppleMode" "\x00"
  "OutdoorTemperature" "\x00"
  "OverTempTotalDwell" "\x00"
  "PICoolingDemand" "\x00"
  "PIHeatingDemand" "\x00"
  "PIROccupiedToUnoccupiedDelay" "\x00"
  "PIRUnoccupiedToOccupiedDelay" "\x00"
  "PIRUnoccupiedToOccupiedThreshold" "\x00"
  "POD" "\x00"
  "Panic" "\x00"
  "PartNumber" "\x00"
  "PathLossExponent" "\x00"
  "PersistentMemoryWrites" "\x00"
  "PersonalAlarm" "\x00"
  "PhaseHarmonicCurrentMultiplier" "\x00"
  "PhysicalClosedLimit" "\x00"
  "PhysicalClosedLimitLift" "\x00"
  "PhysicalClosedLimitTilt" "\x00"
  "PhysicalEnvironment" "\x00"
  "Power" "\x00"
  "PowerDivisor" "\x00"
  "PowerFactor" "\x00"
  "PowerMultiplier" "\x00"
  "PowerOffEffect" "\x00"
  "PowerOnRecall" "\x00"
  "PowerOnTimer" "\x00"
  "PowerSource" "\x00"
  "PowerThreshold" "\x00"
  "Pressure" "\x00"
  "PressureMaxMeasuredValue" "\x00"
  "PressureMaxScaledValue" "\x00"
  "PressureMinMeasuredValue" "\x00"
  "PressureMinScaledValue" "\x00"
  "PressureScale" "\x00"
  "PressureScaledTolerance" "\x00"
  "PressureScaledValue" "\x00"
  "PressureTolerance" "\x00"
  "Primary1Intensity" "\x00"
  "Primary1X" "\x00"
  "Primary1Y" "\x00"
  "Primary2Intensity" "\x00"
  "Primary2X" "\x00"
  "Primary2Y" "\x00"
  "Primary3Intensity" "\x00"
  "Primary3X" "\x00"
  "Primary3Y" "\x00"
  "Primary4Intensity" "\x00"
  "Primary4X" "\x00"
  "Primary4Y" "\x00"
  "Primary5Intensity" "\x00"
  "Primary5X" "\x00"
  "Primary5Y" "\x00"
  "Primary6Intensity" "\x00"
  "Primary6X" "\x00"
  "Primary6Y" "\x00"
  "ProductCode" "\x00"
  "ProductRevision" "\x00"
  "ProductURL" "\x00"
  "QualityMeasure" "\x00"
  "RGB" "\x00"
  "RHDehumidificationSetpoint" "\x00"
  "RMSCurrent" "\x00"
  "RMSCurrentMax" "\x00"
  "RMSCurrentMin" "\x00"
  "RMSExtremeOverVoltage" "\x00"
  "RMSExtremeOverVoltagePeriod" "\x00"
  "RMSExtremeUnderVoltage" "\x00"
  "RMSExtremeUnderVoltagePeriod" "\x00"
  "RMSVoltage" "\x00"
  "RMSVoltageMax" "\x00"
  "RMSVoltageMin" "\x00"
  "RMSVoltageSag" "\x00"
  "RMSVoltageSagPeriod" "\x00"
  "RMSVoltageSwell" "\x00"
  "RMSVoltageSwellPeriod" "\x00"
  "ReactiveCurrent" "\x00"
  "ReactivePower" "\x00"
  "RecallScene" "\x00"
  "RelativeHumidity" "\x00"
  "RelativeHumidityDisplay" "\x00"
  "RelativeHumidityMode" "\x00"
  "RemainingTime" "\x00"
  "RemoteSensing" "\x00"
  "RemoveAllGroups" "\x00"
  "RemoveAllScenes" "\x00"
  "RemoveGroup" "\x00"
  "RemoveScene" "\x00"
  "ReportingPeriod" "\x00"
  "ResetAlarm" "\x00"
  "ResetAllAlarms" "\x00"
  "SWBuildID" "\x00"
  "Sat" "\x00"
  "SatMove" "\x00"
  "SatStep" "\x00"
  "SceneCount" "\x00"
  "SceneNameSupport" "\x00"
  "SceneValid" "\x00"
  "ScheduleMode" "\x00"
  "SeaPressure" "\x00"
  "ShortPollInterval" "\x00"
  "Shutter" "\x00"
  "ShutterClose" "\x00"
  "ShutterLift" "\x00"
  "ShutterOpen" "\x00"
  "ShutterStop" "\x00"
  "ShutterTilt" "\x00"
  "SoftwareRevision" "\x00"
  "StackVersion" "\x00"
  "StandardTime" "\x00"
  "StartUpOnOff" "\x00"
  "Status" "\x00"
  "StoreScene" "\x00"
  "SwitchActions" "\x00"
  "SwitchType" "\x00"
  "SystemMode" "\x00"
  "TRVBoost" "\x00"
  "TRVChildProtection" "\x00"
  "TRVMirrorDisplay" "\x00"
  "TRVMode" "\x00"
  "TRVWindowOpen" "\x00"
  "TempTarget" "\x00"
  "Temperature" "\x00"
  "TemperatureDisplayMode" "\x00"
  "TemperatureMaxMeasuredValue" "\x00"
  "TemperatureMinMeasuredValue" "\x00"
  "TemperatureTolerance" "\x00"
  "TerncyDuration" "\x00"
  "TerncyRotate" "\x00"
  "ThSetpoint" "\x00"
  "ThermostatAlarmMask" "\x00"
  "ThermostatKeypadLockout" "\x00"
  "ThermostatOccupancy" "\x00"
  "ThermostatRunningMode" "\x00"
  "ThermostatScheduleProgrammingVisibility" "\x00"
  "Time" "\x00"
  "TimeEpoch" "\x00"
  "TimeStatus" "\x00"
  "TimeZone" "\x00"
  "TotalActivePower" "\x00"
  "TotalApparentPower" "\x00"
  "TotalProfileNum" "\x00"
  "TotalReactivePower" "\x00"
  "TuyaCalibration" "\x00"
  "TuyaCalibrationTime" "\x00"
  "TuyaMCUVersion" "\x00"
  "TuyaMotorReversal" "\x00"
  "TuyaMovingState" "\x00"
  "TuyaQuery" "\x00"
  "UnoccupiedCoolingSetpoint" "\x00"
  "UnoccupiedHeatingSetpoint" "\x00"
  "UtilityName" "\x00"
  "ValidUntilTime" "\x00"
  "ValvePosition" "\x00"
  "VelocityLift" "\x00"
  "ViewGroup" "\x00"
  "ViewScene" "\x00"
  "Water" "\x00"
  "WhitePointX" "\x00"
  "WhitePointY" "\x00"
  "WindowCoveringType" "\x00"
  "X" "\x00"
  "Y" "\x00"
  "ZCLVersion" "\x00"
  "ZoneID" "\x00"
  "ZoneState" "\x00"
  "ZoneStatus" "\x00"
  "ZoneStatusChange" "\x00"
  "ZoneType" "\x00"
  "_" "\x00"
  "xx" "\x00"
  "xx000A00" "\x00"
  "xx0A" "\x00"
  "xx0A00" "\x00"
  "xx19" "\x00"
  "xx190A" "\x00"
  "xx190A00" "\x00"
  "xxxx" "\x00"
  "xxxx00" "\x00"
  "xxxx0A00" "\x00"
  "xxxxyy" "\x00"
  "xxxxyyyy" "\x00"
  "xxxxyyyy0A00" "\x00"
  "xxxxyyzz" "\x00"
  "xxyy" "\x00"
  "xxyy0A00" "\x00"
  "xxyyyy" "\x00"
  "xxyyyy000000000000" "\x00"
  "xxyyyy0A0000000000" "\x00"
  "xxyyyyzz" "\x00"
  "xxyyyyzzzz" "\x00"
  "xxyyzzzz" "\x00"
  ;
enum Z_offsets {
  Zo_ = 0,
  Zo_00 = 1,
  Zo_00190200 = 4,
  Zo_00xx0A00 = 13,
  Zo_00xxxx000000000000 = 22,
  Zo_01 = 41,
  Zo_0101 = 44,
  Zo_01190200 = 49,
  Zo_01xx0A00 = 58,
  Zo_01xxxx = 67,
  Zo_01xxxx000000000000 = 74,
  Zo_01xxxx0A0000000000 = 93,
  Zo_03xx0A00 = 112,
  Zo_03xxxx000000000000 = 121,
  Zo_03xxxx0A0000000000 = 140,
  Zo_ACActivePowerOverload = 159,
  Zo_ACAlarmsMask = 181,
  Zo_ACCurrentDivisor = 194,
  Zo_ACCurrentMultiplier = 211,
  Zo_ACCurrentOverload = 231,
  Zo_ACFrequency = 249,
  Zo_ACFrequencyDivisor = 261,
  Zo_ACFrequencyMax = 280,
  Zo_ACFrequencyMin = 295,
  Zo_ACFrequencyMultiplier = 310,
  Zo_ACPowerDivisor = 332,
  Zo_ACPowerMultiplier = 347,
  Zo_ACReactivePowerOverload = 365,
  Zo_ACVoltageDivisor = 389,
  Zo_ACVoltageMultiplier = 406,
  Zo_ACVoltageOverload = 426,
  Zo_AbsMaxCoolSetpointLimit = 444,
  Zo_AbsMaxHeatSetpointLimit = 468,
  Zo_AbsMinCoolSetpointLimit = 492,
  Zo_AbsMinHeatSetpointLimit = 516,
  Zo_AccelerationTimeLift = 540,
  Zo_ActiveCurrent = 561,
  Zo_ActivePower = 575,
  Zo_ActivePowerMax = 587,
  Zo_ActivePowerMin = 602,
  Zo_ActuatorEnabled = 617,
  Zo_AddGroup = 633,
  Zo_AddScene = 642,
  Zo_AlarmCount = 651,
  Zo_AlarmMask = 662,
  Zo_AnalogApplicationType = 672,
  Zo_AnalogDescription = 694,
  Zo_AnalogEngineeringUnits = 712,
  Zo_AnalogInApplicationType = 735,
  Zo_AnalogInDescription = 759,
  Zo_AnalogInEngineeringUnits = 779,
  Zo_AnalogInMaxValue = 804,
  Zo_AnalogInMinValue = 821,
  Zo_AnalogInOutOfService = 838,
  Zo_AnalogInReliability = 859,
  Zo_AnalogInResolution = 879,
  Zo_AnalogInStatusFlags = 898,
  Zo_AnalogOutApplicationType = 918,
  Zo_AnalogOutDescription = 943,
  Zo_AnalogOutEngineeringUnits = 964,
  Zo_AnalogOutMaxValue = 990,
  Zo_AnalogOutMinValue = 1008,
  Zo_AnalogOutOfService = 1026,
  Zo_AnalogOutOutOfService = 1045,
  Zo_AnalogOutReliability = 1067,
  Zo_AnalogOutRelinquishDefault = 1088,
  Zo_AnalogOutResolution = 1115,
  Zo_AnalogOutStatusFlags = 1135,
  Zo_AnalogOutValue = 1156,
  Zo_AnalogPriorityArray = 1171,
  Zo_AnalogReliability = 1191,
  Zo_AnalogRelinquishDefault = 1209,
  Zo_AnalogStatusFlags = 1233,
  Zo_AnalogValue = 1251,
  Zo_AppVersion = 1263,
  Zo_ApparentPower = 1274,
  Zo_AqaraAccelerometer = 1288,
  Zo_AqaraRotate = 1307,
  Zo_AqaraVibration505 = 1319,
  Zo_AqaraVibrationMode = 1337,
  Zo_AqaraVibrationsOrAngle = 1356,
  Zo_Aqara_FF05 = 1379,
  Zo_ArrowClick = 1390,
  Zo_ArrowHold = 1401,
  Zo_ArrowRelease = 1411,
  Zo_AutoRelockTime = 1424,
  Zo_AvailablePower = 1439,
  Zo_AverageRMSOverVoltage = 1454,
  Zo_AverageRMSOverVoltageCounter = 1476,
  Zo_AverageRMSUnderVoltage = 1505,
  Zo_AverageRMSUnderVoltageCounter = 1528,
  Zo_AverageRMSVoltageMeasurementPeriod = 1558,
  Zo_BallastFactorAdjustment = 1593,
  Zo_BallastLampQuantity = 1617,
  Zo_BallastMaxLevel = 1637,
  Zo_BallastMinLevel = 1653,
  Zo_BallastPhysicalMaxLevel = 1669,
  Zo_BallastPhysicalMinLevel = 1693,
  Zo_BallastPowerOnFadeTime = 1717,
  Zo_BallastPowerOnLevel = 1740,
  Zo_BallastStatus = 1760,
  Zo_BatteryAHrRating = 1774,
  Zo_BatteryAlarmMask = 1791,
  Zo_BatteryAlarmState = 1808,
  Zo_BatteryManufacturer = 1826,
  Zo_BatteryPercentage = 1846,
  Zo_BatteryPercentageMinThreshold = 1864,
  Zo_BatteryPercentageThreshold1 = 1894,
  Zo_BatteryPercentageThreshold2 = 1922,
  Zo_BatteryPercentageThreshold3 = 1950,
  Zo_BatteryQuantity = 1978,
  Zo_BatteryRatedVoltage = 1994,
  Zo_BatterySize = 2014,
  Zo_BatteryVoltage = 2026,
  Zo_BatteryVoltageMinThreshold = 2041,
  Zo_BatteryVoltageThreshold1 = 2068,
  Zo_BatteryVoltageThreshold2 = 2093,
  Zo_BatteryVoltageThreshold3 = 2118,
  Zo_BinaryActiveText = 2143,
  Zo_BinaryApplicationType = 2160,
  Zo_BinaryDescription = 2182,
  Zo_BinaryInActiveText = 2200,
  Zo_BinaryInApplicationType = 2219,
  Zo_BinaryInDescription = 2243,
  Zo_BinaryInInactiveText = 2263,
  Zo_BinaryInOutOfService = 2284,
  Zo_BinaryInPolarity = 2305,
  Zo_BinaryInReliability = 2322,
  Zo_BinaryInStatusFlags = 2342,
  Zo_BinaryInValue = 2362,
  Zo_BinaryInactiveText = 2376,
  Zo_BinaryMinimumOffTime = 2395,
  Zo_BinaryMinimumOnTime = 2416,
  Zo_BinaryOutActiveText = 2436,
  Zo_BinaryOutApplicationType = 2456,
  Zo_BinaryOutDescription = 2481,
  Zo_BinaryOutInactiveText = 2502,
  Zo_BinaryOutMinimumOffTime = 2524,
  Zo_BinaryOutMinimumOnTime = 2548,
  Zo_BinaryOutOfService = 2571,
  Zo_BinaryOutOutOfService = 2590,
  Zo_BinaryOutPolarity = 2612,
  Zo_BinaryOutReliability = 2630,
  Zo_BinaryOutRelinquishDefault = 2651,
  Zo_BinaryOutStatusFlags = 2678,
  Zo_BinaryOutValue = 2699,
  Zo_BinaryReliability = 2714,
  Zo_BinaryRelinquishDefault = 2732,
  Zo_BinaryStatusFlags = 2756,
  Zo_BinaryValue = 2774,
  Zo_CIE = 2786,
  Zo_CO = 2790,
  Zo_CT = 2793,
  Zo_CalculationPeriod = 2796,
  Zo_CheckinInterval = 2814,
  Zo_CheckinIntervalMin = 2830,
  Zo_ClosedLimit = 2849,
  Zo_Color = 2861,
  Zo_ColorCapabilities = 2867,
  Zo_ColorLoopActive = 2885,
  Zo_ColorLoopDirection = 2901,
  Zo_ColorLoopStartEnhancedHue = 2920,
  Zo_ColorLoopStoredEnhancedHue = 2946,
  Zo_ColorLoopTime = 2973,
  Zo_ColorMode = 2987,
  Zo_ColorMove = 2997,
  Zo_ColorPointBIntensity = 3007,
  Zo_ColorPointBX = 3028,
  Zo_ColorPointBY = 3041,
  Zo_ColorPointGIntensity = 3054,
  Zo_ColorPointGX = 3075,
  Zo_ColorPointGY = 3088,
  Zo_ColorPointRIntensity = 3101,
  Zo_ColorPointRX = 3122,
  Zo_ColorPointRY = 3135,
  Zo_ColorStep = 3148,
  Zo_ColorTempMove = 3158,
  Zo_ColorTempMoveDown = 3172,
  Zo_ColorTempMoveStop = 3190,
  Zo_ColorTempMoveUp = 3208,
  Zo_ColorTempPhysicalMaxMireds = 3224,
  Zo_ColorTempPhysicalMinMireds = 3251,
  Zo_ColorTempStep = 3278,
  Zo_ColorTempStepDown = 3292,
  Zo_ColorTempStepUp = 3310,
  Zo_CompanyName = 3326,
  Zo_CompensationText = 3338,
  Zo_ConfigStatus = 3355,
  Zo_Contact = 3368,
  Zo_ControlSequenceOfOperation = 3376,
  Zo_Coordinate1 = 3403,
  Zo_Coordinate2 = 3415,
  Zo_Coordinate3 = 3427,
  Zo_CurrentGroup = 3439,
  Zo_CurrentPositionLift = 3452,
  Zo_CurrentPositionLiftPercentage = 3472,
  Zo_CurrentPositionTilt = 3502,
  Zo_CurrentPositionTiltPercentage = 3522,
  Zo_CurrentScene = 3552,
  Zo_CurrentTemperature = 3565,
  Zo_CurrentTemperatureSetPoint = 3584,
  Zo_CurrentZoneSensitivityLevel = 3611,
  Zo_CustomerName = 3639,
  Zo_DCCurrent = 3652,
  Zo_DCCurrentDivisor = 3662,
  Zo_DCCurrentMax = 3679,
  Zo_DCCurrentMin = 3692,
  Zo_DCCurrentMultiplier = 3705,
  Zo_DCCurrentOverload = 3725,
  Zo_DCOverloadAlarmsMask = 3743,
  Zo_DCPower = 3764,
  Zo_DCPowerDivisor = 3772,
  Zo_DCPowerMax = 3787,
  Zo_DCPowerMin = 3798,
  Zo_DCPowerMultiplier = 3809,
  Zo_DCVoltage = 3827,
  Zo_DCVoltageDivisor = 3837,
  Zo_DCVoltageMax = 3854,
  Zo_DCVoltageMin = 3867,
  Zo_DCVoltageMultiplier = 3880,
  Zo_DCVoltageOverload = 3900,
  Zo_DataQualityID = 3918,
  Zo_DateCode = 3932,
  Zo_DecelerationTimeLift = 3941,
  Zo_DefaultMoveRate = 3962,
  Zo_DehumidificationCooling = 3978,
  Zo_DehumidificationHysteresis = 4002,
  Zo_DehumidificationLockout = 4029,
  Zo_DehumidificationMaxCool = 4053,
  Zo_DeviceEnabled = 4077,
  Zo_DeviceTempAlarmMask = 4091,
  Zo_Dimmer = 4111,
  Zo_DimmerDown = 4118,
  Zo_DimmerMove = 4129,
  Zo_DimmerOptions = 4140,
  Zo_DimmerRemainingTime = 4154,
  Zo_DimmerStep = 4174,
  Zo_DimmerStepDown = 4185,
  Zo_DimmerStepUp = 4200,
  Zo_DimmerStop = 4213,
  Zo_DimmerUp = 4224,
  Zo_DisableLocalConfig = 4233,
  Zo_DoorClosedEvents = 4252,
  Zo_DoorOpenEvents = 4269,
  Zo_DoorState = 4284,
  Zo_DriftCompensation = 4294,
  Zo_DstEnd = 4312,
  Zo_DstShift = 4319,
  Zo_DstStart = 4328,
  Zo_ElectricalMeasurementType = 4337,
  Zo_EnergyFormatting = 4363,
  Zo_EnergyRemote = 4380,
  Zo_EnergyTotal = 4393,
  Zo_EnhancedColorMode = 4405,
  Zo_EnhancedCurrentHue = 4423,
  Zo_EurotronicErrors = 4442,
  Zo_EurotronicHostFlags = 4459,
  Zo_FanMode = 4479,
  Zo_FanModeSequence = 4487,
  Zo_FastPollTimeout = 4503,
  Zo_FastPollTimeoutMax = 4519,
  Zo_Fire = 4538,
  Zo_FlowMaxMeasuredValue = 4543,
  Zo_FlowMinMeasuredValue = 4564,
  Zo_FlowRate = 4585,
  Zo_FlowTolerance = 4594,
  Zo_GenericDeviceClass = 4608,
  Zo_GenericDeviceType = 4627,
  Zo_GetAllGroups = 4645,
  Zo_GetGroup = 4658,
  Zo_GetSceneMembership = 4667,
  Zo_GlassBreak = 4686,
  Zo_GroupNameSupport = 4697,
  Zo_HVACSystemTypeConfiguration = 4714,
  Zo_HWVersion = 4742,
  Zo_HarmonicCurrentMultiplier = 4752,
  Zo_HighTempDwellTripPoint = 4778,
  Zo_HighTempThreshold = 4801,
  Zo_Hue = 4819,
  Zo_HueMove = 4823,
  Zo_HueSat = 4831,
  Zo_HueStep = 4838,
  Zo_HueStepDown = 4846,
  Zo_HueStepUp = 4858,
  Zo_Humidity = 4868,
  Zo_HumidityMaxMeasuredValue = 4877,
  Zo_HumidityMinMeasuredValue = 4902,
  Zo_HumidityTolerance = 4927,
  Zo_IASCIEAddress = 4945,
  Zo_Identify = 4959,
  Zo_IdentifyQuery = 4968,
  Zo_IdentifyTime = 4982,
  Zo_Illuminance = 4995,
  Zo_IlluminanceLevelStatus = 5007,
  Zo_IlluminanceLightSensorType = 5030,
  Zo_IlluminanceMaxMeasuredValue = 5057,
  Zo_IlluminanceMinMeasuredValue = 5085,
  Zo_IlluminanceTargetLevel = 5113,
  Zo_IlluminanceTolerance = 5136,
  Zo_InstalledClosedLimitLift = 5157,
  Zo_InstalledClosedLimitTilt = 5182,
  Zo_InstalledOpenLimitLift = 5207,
  Zo_InstalledOpenLimitTilt = 5230,
  Zo_IntermediateSetpointsLift = 5253,
  Zo_IntermediateSetpointsTilt = 5279,
  Zo_IntrinsicBallastFactor = 5305,
  Zo_LampAlarmMode = 5328,
  Zo_LampBurnHours = 5342,
  Zo_LampBurnHoursTripPoint = 5356,
  Zo_LampManufacturer = 5379,
  Zo_LampRatedHours = 5396,
  Zo_LampType = 5411,
  Zo_LastConfiguredBy = 5420,
  Zo_LastMessageLQI = 5437,
  Zo_LastMessageRSSI = 5452,
  Zo_LastSetTime = 5468,
  Zo_LegrandHeatingMode = 5480,
  Zo_LegrandMode = 5499,
  Zo_LegrandOpt1 = 5511,
  Zo_LegrandOpt2 = 5523,
  Zo_LegrandOpt3 = 5535,
  Zo_LidlPower = 5547,
  Zo_LineCurrent = 5557,
  Zo_LocalTemperature = 5569,
  Zo_LocalTemperatureCalibration = 5586,
  Zo_LocalTime = 5614,
  Zo_LocationAge = 5624,
  Zo_LocationDescription = 5636,
  Zo_LocationMethod = 5656,
  Zo_LocationPower = 5671,
  Zo_LocationType = 5685,
  Zo_LockAlarmMask = 5698,
  Zo_LockDefaultConfigurationRegister = 5712,
  Zo_LockEnableInsideStatusLED = 5745,
  Zo_LockEnableLocalProgramming = 5771,
  Zo_LockEnableLogging = 5798,
  Zo_LockEnableOneTouchLocking = 5816,
  Zo_LockEnablePrivacyModeButton = 5842,
  Zo_LockKeypadOperationEventMask = 5870,
  Zo_LockKeypadProgrammingEventMask = 5899,
  Zo_LockLEDSettings = 5930,
  Zo_LockLanguage = 5946,
  Zo_LockManualOperationEventMask = 5959,
  Zo_LockOperatingMode = 5988,
  Zo_LockRFIDOperationEventMask = 6006,
  Zo_LockRFIDProgrammingEventMask = 6033,
  Zo_LockRFOperationEventMask = 6062,
  Zo_LockRFProgrammingEventMask = 6087,
  Zo_LockSoundVolume = 6114,
  Zo_LockState = 6130,
  Zo_LockSupportedOperatingModes = 6140,
  Zo_LockType = 6168,
  Zo_LongPollInterval = 6177,
  Zo_LongPollIntervalMin = 6194,
  Zo_LowTempDwellTripPoint = 6214,
  Zo_LowTempThreshold = 6236,
  Zo_MainsAlarmMask = 6253,
  Zo_MainsFrequency = 6268,
  Zo_MainsVoltage = 6283,
  Zo_MainsVoltageDwellTripPoint = 6296,
  Zo_MainsVoltageMaxThreshold = 6323,
  Zo_MainsVoltageMinThreshold = 6348,
  Zo_Manufacturer = 6373,
  Zo_MaxCoolSetpointLimit = 6386,
  Zo_MaxHeatSetpointLimit = 6407,
  Zo_MaxPINCodeLength = 6428,
  Zo_MaxRFIDCodeLength = 6445,
  Zo_MaxTempExperienced = 6463,
  Zo_Measured11thHarmonicCurrent = 6482,
  Zo_Measured1stHarmonicCurrent = 6510,
  Zo_Measured3rdHarmonicCurrent = 6537,
  Zo_Measured5thHarmonicCurrent = 6564,
  Zo_Measured7thHarmonicCurrent = 6591,
  Zo_Measured9thHarmonicCurrent = 6618,
  Zo_MeasuredPhase11thHarmonicCurrent = 6645,
  Zo_MeasuredPhase1stHarmonicCurrent = 6678,
  Zo_MeasuredPhase3rdHarmonicCurrent = 6710,
  Zo_MeasuredPhase5thHarmonicCurrent = 6742,
  Zo_MeasuredPhase7thHarmonicCurrent = 6774,
  Zo_MeasuredPhase9thHarmonicCurrent = 6806,
  Zo_MeterTypeID = 6838,
  Zo_MinCoolSetpointLimit = 6850,
  Zo_MinHeatSetpointLimit = 6871,
  Zo_MinPINCodeLength = 6892,
  Zo_MinRFIDCodeLength = 6909,
  Zo_MinSetpointDeadBand = 6927,
  Zo_MinTempExperienced = 6947,
  Zo_Mode = 6966,
  Zo_Model = 6971,
  Zo_ModelId = 6977,
  Zo_MotorStepSize = 6985,
  Zo_Movement = 6999,
  Zo_MullerLightMode = 7008,
  Zo_MultiApplicationType = 7024,
  Zo_MultiDescription = 7045,
  Zo_MultiInApplicationType = 7062,
  Zo_MultiInDescription = 7085,
  Zo_MultiInNumberOfStates = 7104,
  Zo_MultiInOutOfService = 7126,
  Zo_MultiInReliability = 7146,
  Zo_MultiInStatusFlags = 7165,
  Zo_MultiInValue = 7184,
  Zo_MultiNumberOfStates = 7197,
  Zo_MultiOutApplicationType = 7217,
  Zo_MultiOutDescription = 7241,
  Zo_MultiOutNumberOfStates = 7261,
  Zo_MultiOutOfService = 7284,
  Zo_MultiOutOutOfService = 7302,
  Zo_MultiOutReliability = 7323,
  Zo_MultiOutRelinquishDefault = 7343,
  Zo_MultiOutStatusFlags = 7369,
  Zo_MultiOutValue = 7389,
  Zo_MultiReliability = 7403,
  Zo_MultiRelinquishDefault = 7420,
  Zo_MultiStatusFlags = 7443,
  Zo_MultiValue = 7460,
  Zo_MultipleScheduling = 7471,
  Zo_NeutralCurrent = 7490,
  Zo_NumberOfDevices = 7505,
  Zo_NumberOfHolidaySchedulesSupported = 7521,
  Zo_NumberOfLogRecordsSupported = 7555,
  Zo_NumberOfPINUsersSupported = 7583,
  Zo_NumberOfPrimaries = 7609,
  Zo_NumberOfRFIDUsersSupported = 7627,
  Zo_NumberOfResets = 7654,
  Zo_NumberOfTotalUsersSupported = 7669,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 7697,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 7738,
  Zo_NumberOfZoneSensitivityLevelsSupported = 7779,
  Zo_NumberRSSIMeasurements = 7818,
  Zo_NumberofActuationsLift = 7841,
  Zo_NumberofActuationsTilt = 7864,
  Zo_Occupancy = 7887,
  Zo_OccupancySensorType = 7897,
  Zo_OccupiedCoolingSetpoint = 7917,
  Zo_OccupiedHeatingSetpoint = 7941,
  Zo_OffTransitionTime = 7965,
  Zo_OffWaitTime = 7983,
  Zo_OnLevel = 7995,
  Zo_OnOff = 8003,
  Zo_OnOffTransitionTime = 8009,
  Zo_OnTime = 8029,
  Zo_OnTransitionTime = 8036,
  Zo_OpenPeriod = 8053,
  Zo_OppleMode = 8064,
  Zo_OutdoorTemperature = 8074,
  Zo_OverTempTotalDwell = 8093,
  Zo_PICoolingDemand = 8112,
  Zo_PIHeatingDemand = 8128,
  Zo_PIROccupiedToUnoccupiedDelay = 8144,
  Zo_PIRUnoccupiedToOccupiedDelay = 8173,
  Zo_PIRUnoccupiedToOccupiedThreshold = 8202,
  Zo_POD = 8235,
  Zo_Panic = 8239,
  Zo_PartNumber = 8245,
  Zo_PathLossExponent = 8256,
  Zo_PersistentMemoryWrites = 8273,
  Zo_PersonalAlarm = 8296,
  Zo_PhaseHarmonicCurrentMultiplier = 8310,
  Zo_PhysicalClosedLimit = 8341,
  Zo_PhysicalClosedLimitLift = 8361,
  Zo_PhysicalClosedLimitTilt = 8385,
  Zo_PhysicalEnvironment = 8409,
  Zo_Power = 8429,
  Zo_PowerDivisor = 8435,
  Zo_PowerFactor = 8448,
  Zo_PowerMultiplier = 8460,
  Zo_PowerOffEffect = 8476,
  Zo_PowerOnRecall = 8491,
  Zo_PowerOnTimer = 8505,
  Zo_PowerSource = 8518,
  Zo_PowerThreshold = 8530,
  Zo_Pressure = 8545,
  Zo_PressureMaxMeasuredValue = 8554,
  Zo_PressureMaxScaledValue = 8579,
  Zo_PressureMinMeasuredValue = 8602,
  Zo_PressureMinScaledValue = 8627,
  Zo_PressureScale = 8650,
  Zo_PressureScaledTolerance = 8664,
  Zo_PressureScaledValue = 8688,
  Zo_PressureTolerance = 8708,
  Zo_Primary1Intensity = 8726,
  Zo_Primary1X = 8744,
  Zo_Primary1Y = 8754,
  Zo_Primary2Intensity = 8764,
  Zo_Primary2X = 8782,
  Zo_Primary2Y = 8792,
  Zo_Primary3Intensity = 8802,
  Zo_Primary3X = 8820,
  Zo_Primary3Y = 8830,
  Zo_Primary4Intensity = 8840,
  Zo_Primary4X = 8858,
  Zo_Primary4Y = 8868,
  Zo_Primary5Intensity = 8878,
  Zo_Primary5X = 8896,
  Zo_Primary5Y = 8906,
  Zo_Primary6Intensity = 8916,
  Zo_Primary6X = 8934,
  Zo_Primary6Y = 8944,
  Zo_ProductCode = 8954,
  Zo_ProductRevision = 8966,
  Zo_ProductURL = 8982,
  Zo_QualityMeasure = 8993,
  Zo_RGB = 9008,
  Zo_RHDehumidificationSetpoint = 9012,
  Zo_RMSCurrent = 9039,
  Zo_RMSCurrentMax = 9050,
  Zo_RMSCurrentMin = 9064,
  Zo_RMSExtremeOverVoltage = 9078,
  Zo_RMSExtremeOverVoltagePeriod = 9100,
  Zo_RMSExtremeUnderVoltage = 9128,
  Zo_RMSExtremeUnderVoltagePeriod = 9151,
  Zo_RMSVoltage = 9180,
  Zo_RMSVoltageMax = 9191,
  Zo_RMSVoltageMin = 9205,
  Zo_RMSVoltageSag = 9219,
  Zo_RMSVoltageSagPeriod = 9233,
  Zo_RMSVoltageSwell = 9253,
  Zo_RMSVoltageSwellPeriod = 9269,
  Zo_ReactiveCurrent = 9291,
  Zo_ReactivePower = 9307,
  Zo_RecallScene = 9321,
  Zo_RelativeHumidity = 9333,
  Zo_RelativeHumidityDisplay = 9350,
  Zo_RelativeHumidityMode = 9374,
  Zo_RemainingTime = 9395,
  Zo_RemoteSensing = 9409,
  Zo_RemoveAllGroups = 9423,
  Zo_RemoveAllScenes = 9439,
  Zo_RemoveGroup = 9455,
  Zo_RemoveScene = 9467,
  Zo_ReportingPeriod = 9479,
  Zo_ResetAlarm = 9495,
  Zo_ResetAllAlarms = 9506,
  Zo_SWBuildID = 9521,
  Zo_Sat = 9531,
  Zo_SatMove = 9535,
  Zo_SatStep = 9543,
  Zo_SceneCount = 9551,
  Zo_SceneNameSupport = 9562,
  Zo_SceneValid = 9579,
  Zo_ScheduleMode = 9590,
  Zo_SeaPressure = 9603,
  Zo_ShortPollInterval = 9615,
  Zo_Shutter = 9633,
  Zo_ShutterClose = 9641,
  Zo_ShutterLift = 9654,
  Zo_ShutterOpen = 9666,
  Zo_ShutterStop = 9678,
  Zo_ShutterTilt = 9690,
  Zo_SoftwareRevision = 9702,
  Zo_StackVersion = 9719,
  Zo_StandardTime = 9732,
  Zo_StartUpOnOff = 9745,
  Zo_Status = 9758,
  Zo_StoreScene = 9765,
  Zo_SwitchActions = 9776,
  Zo_SwitchType = 9790,
  Zo_SystemMode = 9801,
  Zo_TRVBoost = 9812,
  Zo_TRVChildProtection = 9821,
  Zo_TRVMirrorDisplay = 9840,
  Zo_TRVMode = 9857,
  Zo_TRVWindowOpen = 9865,
  Zo_TempTarget = 9879,
  Zo_Temperature = 9890,
  Zo_TemperatureDisplayMode = 9902,
  Zo_TemperatureMaxMeasuredValue = 9925,
  Zo_TemperatureMinMeasuredValue = 9953,
  Zo_TemperatureTolerance = 9981,
  Zo_TerncyDuration = 10002,
  Zo_TerncyRotate = 10017,
  Zo_ThSetpoint = 10030,
  Zo_ThermostatAlarmMask = 10041,
  Zo_ThermostatKeypadLockout = 10061,
  Zo_ThermostatOccupancy = 10085,
  Zo_ThermostatRunningMode = 10105,
  Zo_ThermostatScheduleProgrammingVisibility = 10127,
  Zo_Time = 10167,
  Zo_TimeEpoch = 10172,
  Zo_TimeStatus = 10182,
  Zo_TimeZone = 10193,
  Zo_TotalActivePower = 10202,
  Zo_TotalApparentPower = 10219,
  Zo_TotalProfileNum = 10238,
  Zo_TotalReactivePower = 10254,
  Zo_TuyaCalibration = 10273,
  Zo_TuyaCalibrationTime = 10289,
  Zo_TuyaMCUVersion = 10309,
  Zo_TuyaMotorReversal = 10324,
  Zo_TuyaMovingState = 10342,
  Zo_TuyaQuery = 10358,
  Zo_UnoccupiedCoolingSetpoint = 10368,
  Zo_UnoccupiedHeatingSetpoint = 10394,
  Zo_UtilityName = 10420,
  Zo_ValidUntilTime = 10432,
  Zo_ValvePosition = 10447,
  Zo_VelocityLift = 10461,
  Zo_ViewGroup = 10474,
  Zo_ViewScene = 10484,
  Zo_Water = 10494,
  Zo_WhitePointX = 10500,
  Zo_WhitePointY = 10512,
  Zo_WindowCoveringType = 10524,
  Zo_X = 10543,
  Zo_Y = 10545,
  Zo_ZCLVersion = 10547,
  Zo_ZoneID = 10558,
  Zo_ZoneState = 10565,
  Zo_ZoneStatus = 10575,
  Zo_ZoneStatusChange = 10586,
  Zo_ZoneType = 10603,
  Zo__ = 10612,
  Zo_xx = 10614,
  Zo_xx000A00 = 10617,
  Zo_xx0A = 10626,
  Zo_xx0A00 = 10631,
  Zo_xx19 = 10638,
  Zo_xx190A = 10643,
  Zo_xx190A00 = 10650,
  Zo_xxxx = 10659,
  Zo_xxxx00 = 10664,
  Zo_xxxx0A00 = 10671,
  Zo_xxxxyy = 10680,
  Zo_xxxxyyyy = 10687,
  Zo_xxxxyyyy0A00 = 10696,
  Zo_xxxxyyzz = 10709,
  Zo_xxyy = 10718,
  Zo_xxyy0A00 = 10723,
  Zo_xxyyyy = 10732,
  Zo_xxyyyy000000000000 = 10739,
  Zo_xxyyyy0A0000000000 = 10758,
  Zo_xxyyyyzz = 10777,
  Zo_xxyyyyzzzz = 10786,
  Zo_xxyyzzzz = 10797,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
