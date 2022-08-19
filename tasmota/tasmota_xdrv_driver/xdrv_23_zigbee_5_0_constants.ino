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
  "ActiveCurrentPhB" "\x00"
  "ActiveCurrentPhC" "\x00"
  "ActivePower" "\x00"
  "ActivePowerMax" "\x00"
  "ActivePowerMaxPhB" "\x00"
  "ActivePowerMaxPhC" "\x00"
  "ActivePowerMin" "\x00"
  "ActivePowerMinPhB" "\x00"
  "ActivePowerMinPhC" "\x00"
  "ActivePowerPhB" "\x00"
  "ActivePowerPhC" "\x00"
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
  "ApparentPowerPhB" "\x00"
  "ApparentPowerPhC" "\x00"
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
  "AverageRMSOverVoltageCounterPhB" "\x00"
  "AverageRMSOverVoltageCounterPhC" "\x00"
  "AverageRMSUnderVoltage" "\x00"
  "AverageRMSUnderVoltageCounter" "\x00"
  "AverageRMSUnderVoltageCounterPhB" "\x00"
  "AverageRMSUnderVoltageCounterPhC" "\x00"
  "AverageRMSVoltageMeasurementPeriod" "\x00"
  "AverageRMSVoltageMeasurementPeriodPhB" "\x00"
  "AverageRMSVoltageMeasurementPeriodPhC" "\x00"
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
  "LineCurrentPhB" "\x00"
  "LineCurrentPhC" "\x00"
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
  "PowerFactorPhB" "\x00"
  "PowerFactorPhC" "\x00"
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
  "RMSCurrentMaxPhB" "\x00"
  "RMSCurrentMaxPhC" "\x00"
  "RMSCurrentMin" "\x00"
  "RMSCurrentMinPhB" "\x00"
  "RMSCurrentMinPhC" "\x00"
  "RMSCurrentPhB" "\x00"
  "RMSCurrentPhC" "\x00"
  "RMSExtremeOverVoltage" "\x00"
  "RMSExtremeOverVoltagePeriod" "\x00"
  "RMSExtremeOverVoltagePeriodPhB" "\x00"
  "RMSExtremeOverVoltagePeriodPhC" "\x00"
  "RMSExtremeUnderVoltage" "\x00"
  "RMSExtremeUnderVoltagePeriod" "\x00"
  "RMSExtremeUnderVoltagePeriodPhB" "\x00"
  "RMSExtremeUnderVoltagePeriodPhC" "\x00"
  "RMSVoltage" "\x00"
  "RMSVoltageMax" "\x00"
  "RMSVoltageMaxPhB" "\x00"
  "RMSVoltageMaxPhC" "\x00"
  "RMSVoltageMin" "\x00"
  "RMSVoltageMinPhB" "\x00"
  "RMSVoltageMinPhC" "\x00"
  "RMSVoltagePhB" "\x00"
  "RMSVoltagePhC" "\x00"
  "RMSVoltageSag" "\x00"
  "RMSVoltageSagPeriod" "\x00"
  "RMSVoltageSagPeriodPhB" "\x00"
  "RMSVoltageSagPeriodPhC" "\x00"
  "RMSVoltageSwell" "\x00"
  "RMSVoltageSwellPeriod" "\x00"
  "RMSVoltageSwellPeriodPhB" "\x00"
  "RMSVoltageSwellPeriodPhC" "\x00"
  "ReactiveCurrent" "\x00"
  "ReactiveCurrentPhB" "\x00"
  "ReactiveCurrentPhC" "\x00"
  "ReactivePower" "\x00"
  "ReactivePowerPhB" "\x00"
  "ReactivePowerPhC" "\x00"
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
  Zo_ActiveCurrentPhB = 575,
  Zo_ActiveCurrentPhC = 592,
  Zo_ActivePower = 609,
  Zo_ActivePowerMax = 621,
  Zo_ActivePowerMaxPhB = 636,
  Zo_ActivePowerMaxPhC = 654,
  Zo_ActivePowerMin = 672,
  Zo_ActivePowerMinPhB = 687,
  Zo_ActivePowerMinPhC = 705,
  Zo_ActivePowerPhB = 723,
  Zo_ActivePowerPhC = 738,
  Zo_ActuatorEnabled = 753,
  Zo_AddGroup = 769,
  Zo_AddScene = 778,
  Zo_AlarmCount = 787,
  Zo_AlarmMask = 798,
  Zo_AnalogApplicationType = 808,
  Zo_AnalogDescription = 830,
  Zo_AnalogEngineeringUnits = 848,
  Zo_AnalogInApplicationType = 871,
  Zo_AnalogInDescription = 895,
  Zo_AnalogInEngineeringUnits = 915,
  Zo_AnalogInMaxValue = 940,
  Zo_AnalogInMinValue = 957,
  Zo_AnalogInOutOfService = 974,
  Zo_AnalogInReliability = 995,
  Zo_AnalogInResolution = 1015,
  Zo_AnalogInStatusFlags = 1034,
  Zo_AnalogOutApplicationType = 1054,
  Zo_AnalogOutDescription = 1079,
  Zo_AnalogOutEngineeringUnits = 1100,
  Zo_AnalogOutMaxValue = 1126,
  Zo_AnalogOutMinValue = 1144,
  Zo_AnalogOutOfService = 1162,
  Zo_AnalogOutOutOfService = 1181,
  Zo_AnalogOutReliability = 1203,
  Zo_AnalogOutRelinquishDefault = 1224,
  Zo_AnalogOutResolution = 1251,
  Zo_AnalogOutStatusFlags = 1271,
  Zo_AnalogOutValue = 1292,
  Zo_AnalogPriorityArray = 1307,
  Zo_AnalogReliability = 1327,
  Zo_AnalogRelinquishDefault = 1345,
  Zo_AnalogStatusFlags = 1369,
  Zo_AnalogValue = 1387,
  Zo_AppVersion = 1399,
  Zo_ApparentPower = 1410,
  Zo_ApparentPowerPhB = 1424,
  Zo_ApparentPowerPhC = 1441,
  Zo_AqaraAccelerometer = 1458,
  Zo_AqaraRotate = 1477,
  Zo_AqaraVibration505 = 1489,
  Zo_AqaraVibrationMode = 1507,
  Zo_AqaraVibrationsOrAngle = 1526,
  Zo_Aqara_FF05 = 1549,
  Zo_ArrowClick = 1560,
  Zo_ArrowHold = 1571,
  Zo_ArrowRelease = 1581,
  Zo_AutoRelockTime = 1594,
  Zo_AvailablePower = 1609,
  Zo_AverageRMSOverVoltage = 1624,
  Zo_AverageRMSOverVoltageCounter = 1646,
  Zo_AverageRMSOverVoltageCounterPhB = 1675,
  Zo_AverageRMSOverVoltageCounterPhC = 1707,
  Zo_AverageRMSUnderVoltage = 1739,
  Zo_AverageRMSUnderVoltageCounter = 1762,
  Zo_AverageRMSUnderVoltageCounterPhB = 1792,
  Zo_AverageRMSUnderVoltageCounterPhC = 1825,
  Zo_AverageRMSVoltageMeasurementPeriod = 1858,
  Zo_AverageRMSVoltageMeasurementPeriodPhB = 1893,
  Zo_AverageRMSVoltageMeasurementPeriodPhC = 1931,
  Zo_BallastFactorAdjustment = 1969,
  Zo_BallastLampQuantity = 1993,
  Zo_BallastMaxLevel = 2013,
  Zo_BallastMinLevel = 2029,
  Zo_BallastPhysicalMaxLevel = 2045,
  Zo_BallastPhysicalMinLevel = 2069,
  Zo_BallastPowerOnFadeTime = 2093,
  Zo_BallastPowerOnLevel = 2116,
  Zo_BallastStatus = 2136,
  Zo_BatteryAHrRating = 2150,
  Zo_BatteryAlarmMask = 2167,
  Zo_BatteryAlarmState = 2184,
  Zo_BatteryManufacturer = 2202,
  Zo_BatteryPercentage = 2222,
  Zo_BatteryPercentageMinThreshold = 2240,
  Zo_BatteryPercentageThreshold1 = 2270,
  Zo_BatteryPercentageThreshold2 = 2298,
  Zo_BatteryPercentageThreshold3 = 2326,
  Zo_BatteryQuantity = 2354,
  Zo_BatteryRatedVoltage = 2370,
  Zo_BatterySize = 2390,
  Zo_BatteryVoltage = 2402,
  Zo_BatteryVoltageMinThreshold = 2417,
  Zo_BatteryVoltageThreshold1 = 2444,
  Zo_BatteryVoltageThreshold2 = 2469,
  Zo_BatteryVoltageThreshold3 = 2494,
  Zo_BinaryActiveText = 2519,
  Zo_BinaryApplicationType = 2536,
  Zo_BinaryDescription = 2558,
  Zo_BinaryInActiveText = 2576,
  Zo_BinaryInApplicationType = 2595,
  Zo_BinaryInDescription = 2619,
  Zo_BinaryInInactiveText = 2639,
  Zo_BinaryInOutOfService = 2660,
  Zo_BinaryInPolarity = 2681,
  Zo_BinaryInReliability = 2698,
  Zo_BinaryInStatusFlags = 2718,
  Zo_BinaryInValue = 2738,
  Zo_BinaryInactiveText = 2752,
  Zo_BinaryMinimumOffTime = 2771,
  Zo_BinaryMinimumOnTime = 2792,
  Zo_BinaryOutActiveText = 2812,
  Zo_BinaryOutApplicationType = 2832,
  Zo_BinaryOutDescription = 2857,
  Zo_BinaryOutInactiveText = 2878,
  Zo_BinaryOutMinimumOffTime = 2900,
  Zo_BinaryOutMinimumOnTime = 2924,
  Zo_BinaryOutOfService = 2947,
  Zo_BinaryOutOutOfService = 2966,
  Zo_BinaryOutPolarity = 2988,
  Zo_BinaryOutReliability = 3006,
  Zo_BinaryOutRelinquishDefault = 3027,
  Zo_BinaryOutStatusFlags = 3054,
  Zo_BinaryOutValue = 3075,
  Zo_BinaryReliability = 3090,
  Zo_BinaryRelinquishDefault = 3108,
  Zo_BinaryStatusFlags = 3132,
  Zo_BinaryValue = 3150,
  Zo_CIE = 3162,
  Zo_CO = 3166,
  Zo_CT = 3169,
  Zo_CalculationPeriod = 3172,
  Zo_CheckinInterval = 3190,
  Zo_CheckinIntervalMin = 3206,
  Zo_ClosedLimit = 3225,
  Zo_Color = 3237,
  Zo_ColorCapabilities = 3243,
  Zo_ColorLoopActive = 3261,
  Zo_ColorLoopDirection = 3277,
  Zo_ColorLoopStartEnhancedHue = 3296,
  Zo_ColorLoopStoredEnhancedHue = 3322,
  Zo_ColorLoopTime = 3349,
  Zo_ColorMode = 3363,
  Zo_ColorMove = 3373,
  Zo_ColorPointBIntensity = 3383,
  Zo_ColorPointBX = 3404,
  Zo_ColorPointBY = 3417,
  Zo_ColorPointGIntensity = 3430,
  Zo_ColorPointGX = 3451,
  Zo_ColorPointGY = 3464,
  Zo_ColorPointRIntensity = 3477,
  Zo_ColorPointRX = 3498,
  Zo_ColorPointRY = 3511,
  Zo_ColorStep = 3524,
  Zo_ColorTempMove = 3534,
  Zo_ColorTempMoveDown = 3548,
  Zo_ColorTempMoveStop = 3566,
  Zo_ColorTempMoveUp = 3584,
  Zo_ColorTempPhysicalMaxMireds = 3600,
  Zo_ColorTempPhysicalMinMireds = 3627,
  Zo_ColorTempStep = 3654,
  Zo_ColorTempStepDown = 3668,
  Zo_ColorTempStepUp = 3686,
  Zo_CompanyName = 3702,
  Zo_CompensationText = 3714,
  Zo_ConfigStatus = 3731,
  Zo_Contact = 3744,
  Zo_ControlSequenceOfOperation = 3752,
  Zo_Coordinate1 = 3779,
  Zo_Coordinate2 = 3791,
  Zo_Coordinate3 = 3803,
  Zo_CurrentGroup = 3815,
  Zo_CurrentPositionLift = 3828,
  Zo_CurrentPositionLiftPercentage = 3848,
  Zo_CurrentPositionTilt = 3878,
  Zo_CurrentPositionTiltPercentage = 3898,
  Zo_CurrentScene = 3928,
  Zo_CurrentTemperature = 3941,
  Zo_CurrentTemperatureSetPoint = 3960,
  Zo_CurrentZoneSensitivityLevel = 3987,
  Zo_CustomerName = 4015,
  Zo_DCCurrent = 4028,
  Zo_DCCurrentDivisor = 4038,
  Zo_DCCurrentMax = 4055,
  Zo_DCCurrentMin = 4068,
  Zo_DCCurrentMultiplier = 4081,
  Zo_DCCurrentOverload = 4101,
  Zo_DCOverloadAlarmsMask = 4119,
  Zo_DCPower = 4140,
  Zo_DCPowerDivisor = 4148,
  Zo_DCPowerMax = 4163,
  Zo_DCPowerMin = 4174,
  Zo_DCPowerMultiplier = 4185,
  Zo_DCVoltage = 4203,
  Zo_DCVoltageDivisor = 4213,
  Zo_DCVoltageMax = 4230,
  Zo_DCVoltageMin = 4243,
  Zo_DCVoltageMultiplier = 4256,
  Zo_DCVoltageOverload = 4276,
  Zo_DataQualityID = 4294,
  Zo_DateCode = 4308,
  Zo_DecelerationTimeLift = 4317,
  Zo_DefaultMoveRate = 4338,
  Zo_DehumidificationCooling = 4354,
  Zo_DehumidificationHysteresis = 4378,
  Zo_DehumidificationLockout = 4405,
  Zo_DehumidificationMaxCool = 4429,
  Zo_DeviceEnabled = 4453,
  Zo_DeviceTempAlarmMask = 4467,
  Zo_Dimmer = 4487,
  Zo_DimmerDown = 4494,
  Zo_DimmerMove = 4505,
  Zo_DimmerOptions = 4516,
  Zo_DimmerRemainingTime = 4530,
  Zo_DimmerStep = 4550,
  Zo_DimmerStepDown = 4561,
  Zo_DimmerStepUp = 4576,
  Zo_DimmerStop = 4589,
  Zo_DimmerUp = 4600,
  Zo_DisableLocalConfig = 4609,
  Zo_DoorClosedEvents = 4628,
  Zo_DoorOpenEvents = 4645,
  Zo_DoorState = 4660,
  Zo_DriftCompensation = 4670,
  Zo_DstEnd = 4688,
  Zo_DstShift = 4695,
  Zo_DstStart = 4704,
  Zo_ElectricalMeasurementType = 4713,
  Zo_EnergyFormatting = 4739,
  Zo_EnergyRemote = 4756,
  Zo_EnergyTotal = 4769,
  Zo_EnhancedColorMode = 4781,
  Zo_EnhancedCurrentHue = 4799,
  Zo_EurotronicErrors = 4818,
  Zo_EurotronicHostFlags = 4835,
  Zo_FanMode = 4855,
  Zo_FanModeSequence = 4863,
  Zo_FastPollTimeout = 4879,
  Zo_FastPollTimeoutMax = 4895,
  Zo_Fire = 4914,
  Zo_FlowMaxMeasuredValue = 4919,
  Zo_FlowMinMeasuredValue = 4940,
  Zo_FlowRate = 4961,
  Zo_FlowTolerance = 4970,
  Zo_GenericDeviceClass = 4984,
  Zo_GenericDeviceType = 5003,
  Zo_GetAllGroups = 5021,
  Zo_GetGroup = 5034,
  Zo_GetSceneMembership = 5043,
  Zo_GlassBreak = 5062,
  Zo_GroupNameSupport = 5073,
  Zo_HVACSystemTypeConfiguration = 5090,
  Zo_HWVersion = 5118,
  Zo_HarmonicCurrentMultiplier = 5128,
  Zo_HighTempDwellTripPoint = 5154,
  Zo_HighTempThreshold = 5177,
  Zo_Hue = 5195,
  Zo_HueMove = 5199,
  Zo_HueSat = 5207,
  Zo_HueStep = 5214,
  Zo_HueStepDown = 5222,
  Zo_HueStepUp = 5234,
  Zo_Humidity = 5244,
  Zo_HumidityMaxMeasuredValue = 5253,
  Zo_HumidityMinMeasuredValue = 5278,
  Zo_HumidityTolerance = 5303,
  Zo_IASCIEAddress = 5321,
  Zo_Identify = 5335,
  Zo_IdentifyQuery = 5344,
  Zo_IdentifyTime = 5358,
  Zo_Illuminance = 5371,
  Zo_IlluminanceLevelStatus = 5383,
  Zo_IlluminanceLightSensorType = 5406,
  Zo_IlluminanceMaxMeasuredValue = 5433,
  Zo_IlluminanceMinMeasuredValue = 5461,
  Zo_IlluminanceTargetLevel = 5489,
  Zo_IlluminanceTolerance = 5512,
  Zo_InstalledClosedLimitLift = 5533,
  Zo_InstalledClosedLimitTilt = 5558,
  Zo_InstalledOpenLimitLift = 5583,
  Zo_InstalledOpenLimitTilt = 5606,
  Zo_IntermediateSetpointsLift = 5629,
  Zo_IntermediateSetpointsTilt = 5655,
  Zo_IntrinsicBallastFactor = 5681,
  Zo_LampAlarmMode = 5704,
  Zo_LampBurnHours = 5718,
  Zo_LampBurnHoursTripPoint = 5732,
  Zo_LampManufacturer = 5755,
  Zo_LampRatedHours = 5772,
  Zo_LampType = 5787,
  Zo_LastConfiguredBy = 5796,
  Zo_LastMessageLQI = 5813,
  Zo_LastMessageRSSI = 5828,
  Zo_LastSetTime = 5844,
  Zo_LegrandHeatingMode = 5856,
  Zo_LegrandMode = 5875,
  Zo_LegrandOpt1 = 5887,
  Zo_LegrandOpt2 = 5899,
  Zo_LegrandOpt3 = 5911,
  Zo_LidlPower = 5923,
  Zo_LineCurrent = 5933,
  Zo_LineCurrentPhB = 5945,
  Zo_LineCurrentPhC = 5960,
  Zo_LocalTemperature = 5975,
  Zo_LocalTemperatureCalibration = 5992,
  Zo_LocalTime = 6020,
  Zo_LocationAge = 6030,
  Zo_LocationDescription = 6042,
  Zo_LocationMethod = 6062,
  Zo_LocationPower = 6077,
  Zo_LocationType = 6091,
  Zo_LockAlarmMask = 6104,
  Zo_LockDefaultConfigurationRegister = 6118,
  Zo_LockEnableInsideStatusLED = 6151,
  Zo_LockEnableLocalProgramming = 6177,
  Zo_LockEnableLogging = 6204,
  Zo_LockEnableOneTouchLocking = 6222,
  Zo_LockEnablePrivacyModeButton = 6248,
  Zo_LockKeypadOperationEventMask = 6276,
  Zo_LockKeypadProgrammingEventMask = 6305,
  Zo_LockLEDSettings = 6336,
  Zo_LockLanguage = 6352,
  Zo_LockManualOperationEventMask = 6365,
  Zo_LockOperatingMode = 6394,
  Zo_LockRFIDOperationEventMask = 6412,
  Zo_LockRFIDProgrammingEventMask = 6439,
  Zo_LockRFOperationEventMask = 6468,
  Zo_LockRFProgrammingEventMask = 6493,
  Zo_LockSoundVolume = 6520,
  Zo_LockState = 6536,
  Zo_LockSupportedOperatingModes = 6546,
  Zo_LockType = 6574,
  Zo_LongPollInterval = 6583,
  Zo_LongPollIntervalMin = 6600,
  Zo_LowTempDwellTripPoint = 6620,
  Zo_LowTempThreshold = 6642,
  Zo_MainsAlarmMask = 6659,
  Zo_MainsFrequency = 6674,
  Zo_MainsVoltage = 6689,
  Zo_MainsVoltageDwellTripPoint = 6702,
  Zo_MainsVoltageMaxThreshold = 6729,
  Zo_MainsVoltageMinThreshold = 6754,
  Zo_Manufacturer = 6779,
  Zo_MaxCoolSetpointLimit = 6792,
  Zo_MaxHeatSetpointLimit = 6813,
  Zo_MaxPINCodeLength = 6834,
  Zo_MaxRFIDCodeLength = 6851,
  Zo_MaxTempExperienced = 6869,
  Zo_Measured11thHarmonicCurrent = 6888,
  Zo_Measured1stHarmonicCurrent = 6916,
  Zo_Measured3rdHarmonicCurrent = 6943,
  Zo_Measured5thHarmonicCurrent = 6970,
  Zo_Measured7thHarmonicCurrent = 6997,
  Zo_Measured9thHarmonicCurrent = 7024,
  Zo_MeasuredPhase11thHarmonicCurrent = 7051,
  Zo_MeasuredPhase1stHarmonicCurrent = 7084,
  Zo_MeasuredPhase3rdHarmonicCurrent = 7116,
  Zo_MeasuredPhase5thHarmonicCurrent = 7148,
  Zo_MeasuredPhase7thHarmonicCurrent = 7180,
  Zo_MeasuredPhase9thHarmonicCurrent = 7212,
  Zo_MeterTypeID = 7244,
  Zo_MinCoolSetpointLimit = 7256,
  Zo_MinHeatSetpointLimit = 7277,
  Zo_MinPINCodeLength = 7298,
  Zo_MinRFIDCodeLength = 7315,
  Zo_MinSetpointDeadBand = 7333,
  Zo_MinTempExperienced = 7353,
  Zo_Mode = 7372,
  Zo_Model = 7377,
  Zo_ModelId = 7383,
  Zo_MotorStepSize = 7391,
  Zo_Movement = 7405,
  Zo_MullerLightMode = 7414,
  Zo_MultiApplicationType = 7430,
  Zo_MultiDescription = 7451,
  Zo_MultiInApplicationType = 7468,
  Zo_MultiInDescription = 7491,
  Zo_MultiInNumberOfStates = 7510,
  Zo_MultiInOutOfService = 7532,
  Zo_MultiInReliability = 7552,
  Zo_MultiInStatusFlags = 7571,
  Zo_MultiInValue = 7590,
  Zo_MultiNumberOfStates = 7603,
  Zo_MultiOutApplicationType = 7623,
  Zo_MultiOutDescription = 7647,
  Zo_MultiOutNumberOfStates = 7667,
  Zo_MultiOutOfService = 7690,
  Zo_MultiOutOutOfService = 7708,
  Zo_MultiOutReliability = 7729,
  Zo_MultiOutRelinquishDefault = 7749,
  Zo_MultiOutStatusFlags = 7775,
  Zo_MultiOutValue = 7795,
  Zo_MultiReliability = 7809,
  Zo_MultiRelinquishDefault = 7826,
  Zo_MultiStatusFlags = 7849,
  Zo_MultiValue = 7866,
  Zo_MultipleScheduling = 7877,
  Zo_NeutralCurrent = 7896,
  Zo_NumberOfDevices = 7911,
  Zo_NumberOfHolidaySchedulesSupported = 7927,
  Zo_NumberOfLogRecordsSupported = 7961,
  Zo_NumberOfPINUsersSupported = 7989,
  Zo_NumberOfPrimaries = 8015,
  Zo_NumberOfRFIDUsersSupported = 8033,
  Zo_NumberOfResets = 8060,
  Zo_NumberOfTotalUsersSupported = 8075,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 8103,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 8144,
  Zo_NumberOfZoneSensitivityLevelsSupported = 8185,
  Zo_NumberRSSIMeasurements = 8224,
  Zo_NumberofActuationsLift = 8247,
  Zo_NumberofActuationsTilt = 8270,
  Zo_Occupancy = 8293,
  Zo_OccupancySensorType = 8303,
  Zo_OccupiedCoolingSetpoint = 8323,
  Zo_OccupiedHeatingSetpoint = 8347,
  Zo_OffTransitionTime = 8371,
  Zo_OffWaitTime = 8389,
  Zo_OnLevel = 8401,
  Zo_OnOff = 8409,
  Zo_OnOffTransitionTime = 8415,
  Zo_OnTime = 8435,
  Zo_OnTransitionTime = 8442,
  Zo_OpenPeriod = 8459,
  Zo_OppleMode = 8470,
  Zo_OutdoorTemperature = 8480,
  Zo_OverTempTotalDwell = 8499,
  Zo_PICoolingDemand = 8518,
  Zo_PIHeatingDemand = 8534,
  Zo_PIROccupiedToUnoccupiedDelay = 8550,
  Zo_PIRUnoccupiedToOccupiedDelay = 8579,
  Zo_PIRUnoccupiedToOccupiedThreshold = 8608,
  Zo_POD = 8641,
  Zo_Panic = 8645,
  Zo_PartNumber = 8651,
  Zo_PathLossExponent = 8662,
  Zo_PersistentMemoryWrites = 8679,
  Zo_PersonalAlarm = 8702,
  Zo_PhaseHarmonicCurrentMultiplier = 8716,
  Zo_PhysicalClosedLimit = 8747,
  Zo_PhysicalClosedLimitLift = 8767,
  Zo_PhysicalClosedLimitTilt = 8791,
  Zo_PhysicalEnvironment = 8815,
  Zo_Power = 8835,
  Zo_PowerDivisor = 8841,
  Zo_PowerFactor = 8854,
  Zo_PowerFactorPhB = 8866,
  Zo_PowerFactorPhC = 8881,
  Zo_PowerMultiplier = 8896,
  Zo_PowerOffEffect = 8912,
  Zo_PowerOnRecall = 8927,
  Zo_PowerOnTimer = 8941,
  Zo_PowerSource = 8954,
  Zo_PowerThreshold = 8966,
  Zo_Pressure = 8981,
  Zo_PressureMaxMeasuredValue = 8990,
  Zo_PressureMaxScaledValue = 9015,
  Zo_PressureMinMeasuredValue = 9038,
  Zo_PressureMinScaledValue = 9063,
  Zo_PressureScale = 9086,
  Zo_PressureScaledTolerance = 9100,
  Zo_PressureScaledValue = 9124,
  Zo_PressureTolerance = 9144,
  Zo_Primary1Intensity = 9162,
  Zo_Primary1X = 9180,
  Zo_Primary1Y = 9190,
  Zo_Primary2Intensity = 9200,
  Zo_Primary2X = 9218,
  Zo_Primary2Y = 9228,
  Zo_Primary3Intensity = 9238,
  Zo_Primary3X = 9256,
  Zo_Primary3Y = 9266,
  Zo_Primary4Intensity = 9276,
  Zo_Primary4X = 9294,
  Zo_Primary4Y = 9304,
  Zo_Primary5Intensity = 9314,
  Zo_Primary5X = 9332,
  Zo_Primary5Y = 9342,
  Zo_Primary6Intensity = 9352,
  Zo_Primary6X = 9370,
  Zo_Primary6Y = 9380,
  Zo_ProductCode = 9390,
  Zo_ProductRevision = 9402,
  Zo_ProductURL = 9418,
  Zo_QualityMeasure = 9429,
  Zo_RGB = 9444,
  Zo_RHDehumidificationSetpoint = 9448,
  Zo_RMSCurrent = 9475,
  Zo_RMSCurrentMax = 9486,
  Zo_RMSCurrentMaxPhB = 9500,
  Zo_RMSCurrentMaxPhC = 9517,
  Zo_RMSCurrentMin = 9534,
  Zo_RMSCurrentMinPhB = 9548,
  Zo_RMSCurrentMinPhC = 9565,
  Zo_RMSCurrentPhB = 9582,
  Zo_RMSCurrentPhC = 9596,
  Zo_RMSExtremeOverVoltage = 9610,
  Zo_RMSExtremeOverVoltagePeriod = 9632,
  Zo_RMSExtremeOverVoltagePeriodPhB = 9660,
  Zo_RMSExtremeOverVoltagePeriodPhC = 9691,
  Zo_RMSExtremeUnderVoltage = 9722,
  Zo_RMSExtremeUnderVoltagePeriod = 9745,
  Zo_RMSExtremeUnderVoltagePeriodPhB = 9774,
  Zo_RMSExtremeUnderVoltagePeriodPhC = 9806,
  Zo_RMSVoltage = 9838,
  Zo_RMSVoltageMax = 9849,
  Zo_RMSVoltageMaxPhB = 9863,
  Zo_RMSVoltageMaxPhC = 9880,
  Zo_RMSVoltageMin = 9897,
  Zo_RMSVoltageMinPhB = 9911,
  Zo_RMSVoltageMinPhC = 9928,
  Zo_RMSVoltagePhB = 9945,
  Zo_RMSVoltagePhC = 9959,
  Zo_RMSVoltageSag = 9973,
  Zo_RMSVoltageSagPeriod = 9987,
  Zo_RMSVoltageSagPeriodPhB = 10007,
  Zo_RMSVoltageSagPeriodPhC = 10030,
  Zo_RMSVoltageSwell = 10053,
  Zo_RMSVoltageSwellPeriod = 10069,
  Zo_RMSVoltageSwellPeriodPhB = 10091,
  Zo_RMSVoltageSwellPeriodPhC = 10116,
  Zo_ReactiveCurrent = 10141,
  Zo_ReactiveCurrentPhB = 10157,
  Zo_ReactiveCurrentPhC = 10176,
  Zo_ReactivePower = 10195,
  Zo_ReactivePowerPhB = 10209,
  Zo_ReactivePowerPhC = 10226,
  Zo_RecallScene = 10243,
  Zo_RelativeHumidity = 10255,
  Zo_RelativeHumidityDisplay = 10272,
  Zo_RelativeHumidityMode = 10296,
  Zo_RemainingTime = 10317,
  Zo_RemoteSensing = 10331,
  Zo_RemoveAllGroups = 10345,
  Zo_RemoveAllScenes = 10361,
  Zo_RemoveGroup = 10377,
  Zo_RemoveScene = 10389,
  Zo_ReportingPeriod = 10401,
  Zo_ResetAlarm = 10417,
  Zo_ResetAllAlarms = 10428,
  Zo_SWBuildID = 10443,
  Zo_Sat = 10453,
  Zo_SatMove = 10457,
  Zo_SatStep = 10465,
  Zo_SceneCount = 10473,
  Zo_SceneNameSupport = 10484,
  Zo_SceneValid = 10501,
  Zo_ScheduleMode = 10512,
  Zo_SeaPressure = 10525,
  Zo_ShortPollInterval = 10537,
  Zo_Shutter = 10555,
  Zo_ShutterClose = 10563,
  Zo_ShutterLift = 10576,
  Zo_ShutterOpen = 10588,
  Zo_ShutterStop = 10600,
  Zo_ShutterTilt = 10612,
  Zo_SoftwareRevision = 10624,
  Zo_StackVersion = 10641,
  Zo_StandardTime = 10654,
  Zo_StartUpOnOff = 10667,
  Zo_Status = 10680,
  Zo_StoreScene = 10687,
  Zo_SwitchActions = 10698,
  Zo_SwitchType = 10712,
  Zo_SystemMode = 10723,
  Zo_TRVBoost = 10734,
  Zo_TRVChildProtection = 10743,
  Zo_TRVMirrorDisplay = 10762,
  Zo_TRVMode = 10779,
  Zo_TRVWindowOpen = 10787,
  Zo_TempTarget = 10801,
  Zo_Temperature = 10812,
  Zo_TemperatureDisplayMode = 10824,
  Zo_TemperatureMaxMeasuredValue = 10847,
  Zo_TemperatureMinMeasuredValue = 10875,
  Zo_TemperatureTolerance = 10903,
  Zo_TerncyDuration = 10924,
  Zo_TerncyRotate = 10939,
  Zo_ThSetpoint = 10952,
  Zo_ThermostatAlarmMask = 10963,
  Zo_ThermostatKeypadLockout = 10983,
  Zo_ThermostatOccupancy = 11007,
  Zo_ThermostatRunningMode = 11027,
  Zo_ThermostatScheduleProgrammingVisibility = 11049,
  Zo_Time = 11089,
  Zo_TimeEpoch = 11094,
  Zo_TimeStatus = 11104,
  Zo_TimeZone = 11115,
  Zo_TotalActivePower = 11124,
  Zo_TotalApparentPower = 11141,
  Zo_TotalProfileNum = 11160,
  Zo_TotalReactivePower = 11176,
  Zo_TuyaCalibration = 11195,
  Zo_TuyaCalibrationTime = 11211,
  Zo_TuyaMCUVersion = 11231,
  Zo_TuyaMotorReversal = 11246,
  Zo_TuyaMovingState = 11264,
  Zo_TuyaQuery = 11280,
  Zo_UnoccupiedCoolingSetpoint = 11290,
  Zo_UnoccupiedHeatingSetpoint = 11316,
  Zo_UtilityName = 11342,
  Zo_ValidUntilTime = 11354,
  Zo_ValvePosition = 11369,
  Zo_VelocityLift = 11383,
  Zo_ViewGroup = 11396,
  Zo_ViewScene = 11406,
  Zo_Water = 11416,
  Zo_WhitePointX = 11422,
  Zo_WhitePointY = 11434,
  Zo_WindowCoveringType = 11446,
  Zo_X = 11465,
  Zo_Y = 11467,
  Zo_ZCLVersion = 11469,
  Zo_ZoneID = 11480,
  Zo_ZoneState = 11487,
  Zo_ZoneStatus = 11497,
  Zo_ZoneStatusChange = 11508,
  Zo_ZoneType = 11525,
  Zo__ = 11534,
  Zo_xx = 11536,
  Zo_xx000A00 = 11539,
  Zo_xx0A = 11548,
  Zo_xx0A00 = 11553,
  Zo_xx19 = 11560,
  Zo_xx190A = 11565,
  Zo_xx190A00 = 11572,
  Zo_xxxx = 11581,
  Zo_xxxx00 = 11586,
  Zo_xxxx0A00 = 11593,
  Zo_xxxxyy = 11602,
  Zo_xxxxyyyy = 11609,
  Zo_xxxxyyyy0A00 = 11618,
  Zo_xxxxyyzz = 11631,
  Zo_xxyy = 11640,
  Zo_xxyy0A00 = 11645,
  Zo_xxyyyy = 11654,
  Zo_xxyyyy000000000000 = 11661,
  Zo_xxyyyy0A0000000000 = 11680,
  Zo_xxyyyyzz = 11699,
  Zo_xxyyyyzzzz = 11708,
  Zo_xxyyzzzz = 11719,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
