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
  "BlockedGPDID" "\x00"
  "CIE" "\x00"
  "CO" "\x00"
  "CT" "\x00"
  "CalculationPeriod" "\x00"
  "CcommissioningExitMode" "\x00"
  "CheckinInterval" "\x00"
  "CheckinIntervalMin" "\x00"
  "ClientActiveFunctionality" "\x00"
  "ClientFunctionality" "\x00"
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
  "ColorStartUpColorTempireds" "\x00"
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
  "CommissioningWindow" "\x00"
  "CommunicationMode" "\x00"
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
  "DimmerCurrentFrequency" "\x00"
  "DimmerDown" "\x00"
  "DimmerMaxFrequency" "\x00"
  "DimmerMaxLevel" "\x00"
  "DimmerMinFrequency" "\x00"
  "DimmerMinLevel" "\x00"
  "DimmerMove" "\x00"
  "DimmerOptions" "\x00"
  "DimmerRemainingTime" "\x00"
  "DimmerStartUpLevel" "\x00"
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
  "LinkKey" "\x00"
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
  "MaxProxyTableEntries" "\x00"
  "MaxRFIDCodeLength" "\x00"
  "MaxSearchCounter" "\x00"
  "MaxSinkTableEntries" "\x00"
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
  "NotificationRetryNumber" "\x00"
  "NotificationRetryTimer" "\x00"
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
  "ProxyTable" "\x00"
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
  "SecurityLevel" "\x00"
  "ServerActiveFunctionality" "\x00"
  "ServerFunctionality" "\x00"
  "SharedSecurityKey" "\x00"
  "SharedSecurityKeyType" "\x00"
  "ShortPollInterval" "\x00"
  "Shutter" "\x00"
  "ShutterClose" "\x00"
  "ShutterLift" "\x00"
  "ShutterOpen" "\x00"
  "ShutterStop" "\x00"
  "ShutterTilt" "\x00"
  "SinkTable" "\x00"
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
  Zo_BlockedGPDID = 3162,
  Zo_CIE = 3175,
  Zo_CO = 3179,
  Zo_CT = 3182,
  Zo_CalculationPeriod = 3185,
  Zo_CcommissioningExitMode = 3203,
  Zo_CheckinInterval = 3226,
  Zo_CheckinIntervalMin = 3242,
  Zo_ClientActiveFunctionality = 3261,
  Zo_ClientFunctionality = 3287,
  Zo_ClosedLimit = 3307,
  Zo_Color = 3319,
  Zo_ColorCapabilities = 3325,
  Zo_ColorLoopActive = 3343,
  Zo_ColorLoopDirection = 3359,
  Zo_ColorLoopStartEnhancedHue = 3378,
  Zo_ColorLoopStoredEnhancedHue = 3404,
  Zo_ColorLoopTime = 3431,
  Zo_ColorMode = 3445,
  Zo_ColorMove = 3455,
  Zo_ColorPointBIntensity = 3465,
  Zo_ColorPointBX = 3486,
  Zo_ColorPointBY = 3499,
  Zo_ColorPointGIntensity = 3512,
  Zo_ColorPointGX = 3533,
  Zo_ColorPointGY = 3546,
  Zo_ColorPointRIntensity = 3559,
  Zo_ColorPointRX = 3580,
  Zo_ColorPointRY = 3593,
  Zo_ColorStartUpColorTempireds = 3606,
  Zo_ColorStep = 3633,
  Zo_ColorTempMove = 3643,
  Zo_ColorTempMoveDown = 3657,
  Zo_ColorTempMoveStop = 3675,
  Zo_ColorTempMoveUp = 3693,
  Zo_ColorTempPhysicalMaxMireds = 3709,
  Zo_ColorTempPhysicalMinMireds = 3736,
  Zo_ColorTempStep = 3763,
  Zo_ColorTempStepDown = 3777,
  Zo_ColorTempStepUp = 3795,
  Zo_CommissioningWindow = 3811,
  Zo_CommunicationMode = 3831,
  Zo_CompanyName = 3849,
  Zo_CompensationText = 3861,
  Zo_ConfigStatus = 3878,
  Zo_Contact = 3891,
  Zo_ControlSequenceOfOperation = 3899,
  Zo_Coordinate1 = 3926,
  Zo_Coordinate2 = 3938,
  Zo_Coordinate3 = 3950,
  Zo_CurrentGroup = 3962,
  Zo_CurrentPositionLift = 3975,
  Zo_CurrentPositionLiftPercentage = 3995,
  Zo_CurrentPositionTilt = 4025,
  Zo_CurrentPositionTiltPercentage = 4045,
  Zo_CurrentScene = 4075,
  Zo_CurrentTemperature = 4088,
  Zo_CurrentTemperatureSetPoint = 4107,
  Zo_CurrentZoneSensitivityLevel = 4134,
  Zo_CustomerName = 4162,
  Zo_DCCurrent = 4175,
  Zo_DCCurrentDivisor = 4185,
  Zo_DCCurrentMax = 4202,
  Zo_DCCurrentMin = 4215,
  Zo_DCCurrentMultiplier = 4228,
  Zo_DCCurrentOverload = 4248,
  Zo_DCOverloadAlarmsMask = 4266,
  Zo_DCPower = 4287,
  Zo_DCPowerDivisor = 4295,
  Zo_DCPowerMax = 4310,
  Zo_DCPowerMin = 4321,
  Zo_DCPowerMultiplier = 4332,
  Zo_DCVoltage = 4350,
  Zo_DCVoltageDivisor = 4360,
  Zo_DCVoltageMax = 4377,
  Zo_DCVoltageMin = 4390,
  Zo_DCVoltageMultiplier = 4403,
  Zo_DCVoltageOverload = 4423,
  Zo_DataQualityID = 4441,
  Zo_DateCode = 4455,
  Zo_DecelerationTimeLift = 4464,
  Zo_DefaultMoveRate = 4485,
  Zo_DehumidificationCooling = 4501,
  Zo_DehumidificationHysteresis = 4525,
  Zo_DehumidificationLockout = 4552,
  Zo_DehumidificationMaxCool = 4576,
  Zo_DeviceEnabled = 4600,
  Zo_DeviceTempAlarmMask = 4614,
  Zo_Dimmer = 4634,
  Zo_DimmerCurrentFrequency = 4641,
  Zo_DimmerDown = 4664,
  Zo_DimmerMaxFrequency = 4675,
  Zo_DimmerMaxLevel = 4694,
  Zo_DimmerMinFrequency = 4709,
  Zo_DimmerMinLevel = 4728,
  Zo_DimmerMove = 4743,
  Zo_DimmerOptions = 4754,
  Zo_DimmerRemainingTime = 4768,
  Zo_DimmerStartUpLevel = 4788,
  Zo_DimmerStep = 4807,
  Zo_DimmerStepDown = 4818,
  Zo_DimmerStepUp = 4833,
  Zo_DimmerStop = 4846,
  Zo_DimmerUp = 4857,
  Zo_DisableLocalConfig = 4866,
  Zo_DoorClosedEvents = 4885,
  Zo_DoorOpenEvents = 4902,
  Zo_DoorState = 4917,
  Zo_DriftCompensation = 4927,
  Zo_DstEnd = 4945,
  Zo_DstShift = 4952,
  Zo_DstStart = 4961,
  Zo_ElectricalMeasurementType = 4970,
  Zo_EnergyFormatting = 4996,
  Zo_EnergyRemote = 5013,
  Zo_EnergyTotal = 5026,
  Zo_EnhancedColorMode = 5038,
  Zo_EnhancedCurrentHue = 5056,
  Zo_EurotronicErrors = 5075,
  Zo_EurotronicHostFlags = 5092,
  Zo_FanMode = 5112,
  Zo_FanModeSequence = 5120,
  Zo_FastPollTimeout = 5136,
  Zo_FastPollTimeoutMax = 5152,
  Zo_Fire = 5171,
  Zo_FlowMaxMeasuredValue = 5176,
  Zo_FlowMinMeasuredValue = 5197,
  Zo_FlowRate = 5218,
  Zo_FlowTolerance = 5227,
  Zo_GenericDeviceClass = 5241,
  Zo_GenericDeviceType = 5260,
  Zo_GetAllGroups = 5278,
  Zo_GetGroup = 5291,
  Zo_GetSceneMembership = 5300,
  Zo_GlassBreak = 5319,
  Zo_GroupNameSupport = 5330,
  Zo_HVACSystemTypeConfiguration = 5347,
  Zo_HWVersion = 5375,
  Zo_HarmonicCurrentMultiplier = 5385,
  Zo_HighTempDwellTripPoint = 5411,
  Zo_HighTempThreshold = 5434,
  Zo_Hue = 5452,
  Zo_HueMove = 5456,
  Zo_HueSat = 5464,
  Zo_HueStep = 5471,
  Zo_HueStepDown = 5479,
  Zo_HueStepUp = 5491,
  Zo_Humidity = 5501,
  Zo_HumidityMaxMeasuredValue = 5510,
  Zo_HumidityMinMeasuredValue = 5535,
  Zo_HumidityTolerance = 5560,
  Zo_IASCIEAddress = 5578,
  Zo_Identify = 5592,
  Zo_IdentifyQuery = 5601,
  Zo_IdentifyTime = 5615,
  Zo_Illuminance = 5628,
  Zo_IlluminanceLevelStatus = 5640,
  Zo_IlluminanceLightSensorType = 5663,
  Zo_IlluminanceMaxMeasuredValue = 5690,
  Zo_IlluminanceMinMeasuredValue = 5718,
  Zo_IlluminanceTargetLevel = 5746,
  Zo_IlluminanceTolerance = 5769,
  Zo_InstalledClosedLimitLift = 5790,
  Zo_InstalledClosedLimitTilt = 5815,
  Zo_InstalledOpenLimitLift = 5840,
  Zo_InstalledOpenLimitTilt = 5863,
  Zo_IntermediateSetpointsLift = 5886,
  Zo_IntermediateSetpointsTilt = 5912,
  Zo_IntrinsicBallastFactor = 5938,
  Zo_LampAlarmMode = 5961,
  Zo_LampBurnHours = 5975,
  Zo_LampBurnHoursTripPoint = 5989,
  Zo_LampManufacturer = 6012,
  Zo_LampRatedHours = 6029,
  Zo_LampType = 6044,
  Zo_LastConfiguredBy = 6053,
  Zo_LastMessageLQI = 6070,
  Zo_LastMessageRSSI = 6085,
  Zo_LastSetTime = 6101,
  Zo_LegrandHeatingMode = 6113,
  Zo_LegrandMode = 6132,
  Zo_LegrandOpt1 = 6144,
  Zo_LegrandOpt2 = 6156,
  Zo_LegrandOpt3 = 6168,
  Zo_LidlPower = 6180,
  Zo_LineCurrent = 6190,
  Zo_LineCurrentPhB = 6202,
  Zo_LineCurrentPhC = 6217,
  Zo_LinkKey = 6232,
  Zo_LocalTemperature = 6240,
  Zo_LocalTemperatureCalibration = 6257,
  Zo_LocalTime = 6285,
  Zo_LocationAge = 6295,
  Zo_LocationDescription = 6307,
  Zo_LocationMethod = 6327,
  Zo_LocationPower = 6342,
  Zo_LocationType = 6356,
  Zo_LockAlarmMask = 6369,
  Zo_LockDefaultConfigurationRegister = 6383,
  Zo_LockEnableInsideStatusLED = 6416,
  Zo_LockEnableLocalProgramming = 6442,
  Zo_LockEnableLogging = 6469,
  Zo_LockEnableOneTouchLocking = 6487,
  Zo_LockEnablePrivacyModeButton = 6513,
  Zo_LockKeypadOperationEventMask = 6541,
  Zo_LockKeypadProgrammingEventMask = 6570,
  Zo_LockLEDSettings = 6601,
  Zo_LockLanguage = 6617,
  Zo_LockManualOperationEventMask = 6630,
  Zo_LockOperatingMode = 6659,
  Zo_LockRFIDOperationEventMask = 6677,
  Zo_LockRFIDProgrammingEventMask = 6704,
  Zo_LockRFOperationEventMask = 6733,
  Zo_LockRFProgrammingEventMask = 6758,
  Zo_LockSoundVolume = 6785,
  Zo_LockState = 6801,
  Zo_LockSupportedOperatingModes = 6811,
  Zo_LockType = 6839,
  Zo_LongPollInterval = 6848,
  Zo_LongPollIntervalMin = 6865,
  Zo_LowTempDwellTripPoint = 6885,
  Zo_LowTempThreshold = 6907,
  Zo_MainsAlarmMask = 6924,
  Zo_MainsFrequency = 6939,
  Zo_MainsVoltage = 6954,
  Zo_MainsVoltageDwellTripPoint = 6967,
  Zo_MainsVoltageMaxThreshold = 6994,
  Zo_MainsVoltageMinThreshold = 7019,
  Zo_Manufacturer = 7044,
  Zo_MaxCoolSetpointLimit = 7057,
  Zo_MaxHeatSetpointLimit = 7078,
  Zo_MaxPINCodeLength = 7099,
  Zo_MaxProxyTableEntries = 7116,
  Zo_MaxRFIDCodeLength = 7137,
  Zo_MaxSearchCounter = 7155,
  Zo_MaxSinkTableEntries = 7172,
  Zo_MaxTempExperienced = 7192,
  Zo_Measured11thHarmonicCurrent = 7211,
  Zo_Measured1stHarmonicCurrent = 7239,
  Zo_Measured3rdHarmonicCurrent = 7266,
  Zo_Measured5thHarmonicCurrent = 7293,
  Zo_Measured7thHarmonicCurrent = 7320,
  Zo_Measured9thHarmonicCurrent = 7347,
  Zo_MeasuredPhase11thHarmonicCurrent = 7374,
  Zo_MeasuredPhase1stHarmonicCurrent = 7407,
  Zo_MeasuredPhase3rdHarmonicCurrent = 7439,
  Zo_MeasuredPhase5thHarmonicCurrent = 7471,
  Zo_MeasuredPhase7thHarmonicCurrent = 7503,
  Zo_MeasuredPhase9thHarmonicCurrent = 7535,
  Zo_MeterTypeID = 7567,
  Zo_MinCoolSetpointLimit = 7579,
  Zo_MinHeatSetpointLimit = 7600,
  Zo_MinPINCodeLength = 7621,
  Zo_MinRFIDCodeLength = 7638,
  Zo_MinSetpointDeadBand = 7656,
  Zo_MinTempExperienced = 7676,
  Zo_Mode = 7695,
  Zo_Model = 7700,
  Zo_ModelId = 7706,
  Zo_MotorStepSize = 7714,
  Zo_Movement = 7728,
  Zo_MullerLightMode = 7737,
  Zo_MultiApplicationType = 7753,
  Zo_MultiDescription = 7774,
  Zo_MultiInApplicationType = 7791,
  Zo_MultiInDescription = 7814,
  Zo_MultiInNumberOfStates = 7833,
  Zo_MultiInOutOfService = 7855,
  Zo_MultiInReliability = 7875,
  Zo_MultiInStatusFlags = 7894,
  Zo_MultiInValue = 7913,
  Zo_MultiNumberOfStates = 7926,
  Zo_MultiOutApplicationType = 7946,
  Zo_MultiOutDescription = 7970,
  Zo_MultiOutNumberOfStates = 7990,
  Zo_MultiOutOfService = 8013,
  Zo_MultiOutOutOfService = 8031,
  Zo_MultiOutReliability = 8052,
  Zo_MultiOutRelinquishDefault = 8072,
  Zo_MultiOutStatusFlags = 8098,
  Zo_MultiOutValue = 8118,
  Zo_MultiReliability = 8132,
  Zo_MultiRelinquishDefault = 8149,
  Zo_MultiStatusFlags = 8172,
  Zo_MultiValue = 8189,
  Zo_MultipleScheduling = 8200,
  Zo_NeutralCurrent = 8219,
  Zo_NotificationRetryNumber = 8234,
  Zo_NotificationRetryTimer = 8258,
  Zo_NumberOfDevices = 8281,
  Zo_NumberOfHolidaySchedulesSupported = 8297,
  Zo_NumberOfLogRecordsSupported = 8331,
  Zo_NumberOfPINUsersSupported = 8359,
  Zo_NumberOfPrimaries = 8385,
  Zo_NumberOfRFIDUsersSupported = 8403,
  Zo_NumberOfResets = 8430,
  Zo_NumberOfTotalUsersSupported = 8445,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 8473,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 8514,
  Zo_NumberOfZoneSensitivityLevelsSupported = 8555,
  Zo_NumberRSSIMeasurements = 8594,
  Zo_NumberofActuationsLift = 8617,
  Zo_NumberofActuationsTilt = 8640,
  Zo_Occupancy = 8663,
  Zo_OccupancySensorType = 8673,
  Zo_OccupiedCoolingSetpoint = 8693,
  Zo_OccupiedHeatingSetpoint = 8717,
  Zo_OffTransitionTime = 8741,
  Zo_OffWaitTime = 8759,
  Zo_OnLevel = 8771,
  Zo_OnOff = 8779,
  Zo_OnOffTransitionTime = 8785,
  Zo_OnTime = 8805,
  Zo_OnTransitionTime = 8812,
  Zo_OpenPeriod = 8829,
  Zo_OppleMode = 8840,
  Zo_OutdoorTemperature = 8850,
  Zo_OverTempTotalDwell = 8869,
  Zo_PICoolingDemand = 8888,
  Zo_PIHeatingDemand = 8904,
  Zo_PIROccupiedToUnoccupiedDelay = 8920,
  Zo_PIRUnoccupiedToOccupiedDelay = 8949,
  Zo_PIRUnoccupiedToOccupiedThreshold = 8978,
  Zo_POD = 9011,
  Zo_Panic = 9015,
  Zo_PartNumber = 9021,
  Zo_PathLossExponent = 9032,
  Zo_PersistentMemoryWrites = 9049,
  Zo_PersonalAlarm = 9072,
  Zo_PhaseHarmonicCurrentMultiplier = 9086,
  Zo_PhysicalClosedLimit = 9117,
  Zo_PhysicalClosedLimitLift = 9137,
  Zo_PhysicalClosedLimitTilt = 9161,
  Zo_PhysicalEnvironment = 9185,
  Zo_Power = 9205,
  Zo_PowerDivisor = 9211,
  Zo_PowerFactor = 9224,
  Zo_PowerFactorPhB = 9236,
  Zo_PowerFactorPhC = 9251,
  Zo_PowerMultiplier = 9266,
  Zo_PowerOffEffect = 9282,
  Zo_PowerOnRecall = 9297,
  Zo_PowerOnTimer = 9311,
  Zo_PowerSource = 9324,
  Zo_PowerThreshold = 9336,
  Zo_Pressure = 9351,
  Zo_PressureMaxMeasuredValue = 9360,
  Zo_PressureMaxScaledValue = 9385,
  Zo_PressureMinMeasuredValue = 9408,
  Zo_PressureMinScaledValue = 9433,
  Zo_PressureScale = 9456,
  Zo_PressureScaledTolerance = 9470,
  Zo_PressureScaledValue = 9494,
  Zo_PressureTolerance = 9514,
  Zo_Primary1Intensity = 9532,
  Zo_Primary1X = 9550,
  Zo_Primary1Y = 9560,
  Zo_Primary2Intensity = 9570,
  Zo_Primary2X = 9588,
  Zo_Primary2Y = 9598,
  Zo_Primary3Intensity = 9608,
  Zo_Primary3X = 9626,
  Zo_Primary3Y = 9636,
  Zo_Primary4Intensity = 9646,
  Zo_Primary4X = 9664,
  Zo_Primary4Y = 9674,
  Zo_Primary5Intensity = 9684,
  Zo_Primary5X = 9702,
  Zo_Primary5Y = 9712,
  Zo_Primary6Intensity = 9722,
  Zo_Primary6X = 9740,
  Zo_Primary6Y = 9750,
  Zo_ProductCode = 9760,
  Zo_ProductRevision = 9772,
  Zo_ProductURL = 9788,
  Zo_ProxyTable = 9799,
  Zo_QualityMeasure = 9810,
  Zo_RGB = 9825,
  Zo_RHDehumidificationSetpoint = 9829,
  Zo_RMSCurrent = 9856,
  Zo_RMSCurrentMax = 9867,
  Zo_RMSCurrentMaxPhB = 9881,
  Zo_RMSCurrentMaxPhC = 9898,
  Zo_RMSCurrentMin = 9915,
  Zo_RMSCurrentMinPhB = 9929,
  Zo_RMSCurrentMinPhC = 9946,
  Zo_RMSCurrentPhB = 9963,
  Zo_RMSCurrentPhC = 9977,
  Zo_RMSExtremeOverVoltage = 9991,
  Zo_RMSExtremeOverVoltagePeriod = 10013,
  Zo_RMSExtremeOverVoltagePeriodPhB = 10041,
  Zo_RMSExtremeOverVoltagePeriodPhC = 10072,
  Zo_RMSExtremeUnderVoltage = 10103,
  Zo_RMSExtremeUnderVoltagePeriod = 10126,
  Zo_RMSExtremeUnderVoltagePeriodPhB = 10155,
  Zo_RMSExtremeUnderVoltagePeriodPhC = 10187,
  Zo_RMSVoltage = 10219,
  Zo_RMSVoltageMax = 10230,
  Zo_RMSVoltageMaxPhB = 10244,
  Zo_RMSVoltageMaxPhC = 10261,
  Zo_RMSVoltageMin = 10278,
  Zo_RMSVoltageMinPhB = 10292,
  Zo_RMSVoltageMinPhC = 10309,
  Zo_RMSVoltagePhB = 10326,
  Zo_RMSVoltagePhC = 10340,
  Zo_RMSVoltageSag = 10354,
  Zo_RMSVoltageSagPeriod = 10368,
  Zo_RMSVoltageSagPeriodPhB = 10388,
  Zo_RMSVoltageSagPeriodPhC = 10411,
  Zo_RMSVoltageSwell = 10434,
  Zo_RMSVoltageSwellPeriod = 10450,
  Zo_RMSVoltageSwellPeriodPhB = 10472,
  Zo_RMSVoltageSwellPeriodPhC = 10497,
  Zo_ReactiveCurrent = 10522,
  Zo_ReactiveCurrentPhB = 10538,
  Zo_ReactiveCurrentPhC = 10557,
  Zo_ReactivePower = 10576,
  Zo_ReactivePowerPhB = 10590,
  Zo_ReactivePowerPhC = 10607,
  Zo_RecallScene = 10624,
  Zo_RelativeHumidity = 10636,
  Zo_RelativeHumidityDisplay = 10653,
  Zo_RelativeHumidityMode = 10677,
  Zo_RemainingTime = 10698,
  Zo_RemoteSensing = 10712,
  Zo_RemoveAllGroups = 10726,
  Zo_RemoveAllScenes = 10742,
  Zo_RemoveGroup = 10758,
  Zo_RemoveScene = 10770,
  Zo_ReportingPeriod = 10782,
  Zo_ResetAlarm = 10798,
  Zo_ResetAllAlarms = 10809,
  Zo_SWBuildID = 10824,
  Zo_Sat = 10834,
  Zo_SatMove = 10838,
  Zo_SatStep = 10846,
  Zo_SceneCount = 10854,
  Zo_SceneNameSupport = 10865,
  Zo_SceneValid = 10882,
  Zo_ScheduleMode = 10893,
  Zo_SeaPressure = 10906,
  Zo_SecurityLevel = 10918,
  Zo_ServerActiveFunctionality = 10932,
  Zo_ServerFunctionality = 10958,
  Zo_SharedSecurityKey = 10978,
  Zo_SharedSecurityKeyType = 10996,
  Zo_ShortPollInterval = 11018,
  Zo_Shutter = 11036,
  Zo_ShutterClose = 11044,
  Zo_ShutterLift = 11057,
  Zo_ShutterOpen = 11069,
  Zo_ShutterStop = 11081,
  Zo_ShutterTilt = 11093,
  Zo_SinkTable = 11105,
  Zo_SoftwareRevision = 11115,
  Zo_StackVersion = 11132,
  Zo_StandardTime = 11145,
  Zo_StartUpOnOff = 11158,
  Zo_Status = 11171,
  Zo_StoreScene = 11178,
  Zo_SwitchActions = 11189,
  Zo_SwitchType = 11203,
  Zo_SystemMode = 11214,
  Zo_TRVBoost = 11225,
  Zo_TRVChildProtection = 11234,
  Zo_TRVMirrorDisplay = 11253,
  Zo_TRVMode = 11270,
  Zo_TRVWindowOpen = 11278,
  Zo_TempTarget = 11292,
  Zo_Temperature = 11303,
  Zo_TemperatureDisplayMode = 11315,
  Zo_TemperatureMaxMeasuredValue = 11338,
  Zo_TemperatureMinMeasuredValue = 11366,
  Zo_TemperatureTolerance = 11394,
  Zo_TerncyDuration = 11415,
  Zo_TerncyRotate = 11430,
  Zo_ThSetpoint = 11443,
  Zo_ThermostatAlarmMask = 11454,
  Zo_ThermostatKeypadLockout = 11474,
  Zo_ThermostatOccupancy = 11498,
  Zo_ThermostatRunningMode = 11518,
  Zo_ThermostatScheduleProgrammingVisibility = 11540,
  Zo_Time = 11580,
  Zo_TimeEpoch = 11585,
  Zo_TimeStatus = 11595,
  Zo_TimeZone = 11606,
  Zo_TotalActivePower = 11615,
  Zo_TotalApparentPower = 11632,
  Zo_TotalProfileNum = 11651,
  Zo_TotalReactivePower = 11667,
  Zo_TuyaCalibration = 11686,
  Zo_TuyaCalibrationTime = 11702,
  Zo_TuyaMCUVersion = 11722,
  Zo_TuyaMotorReversal = 11737,
  Zo_TuyaMovingState = 11755,
  Zo_TuyaQuery = 11771,
  Zo_UnoccupiedCoolingSetpoint = 11781,
  Zo_UnoccupiedHeatingSetpoint = 11807,
  Zo_UtilityName = 11833,
  Zo_ValidUntilTime = 11845,
  Zo_ValvePosition = 11860,
  Zo_VelocityLift = 11874,
  Zo_ViewGroup = 11887,
  Zo_ViewScene = 11897,
  Zo_Water = 11907,
  Zo_WhitePointX = 11913,
  Zo_WhitePointY = 11925,
  Zo_WindowCoveringType = 11937,
  Zo_X = 11956,
  Zo_Y = 11958,
  Zo_ZCLVersion = 11960,
  Zo_ZoneID = 11971,
  Zo_ZoneState = 11978,
  Zo_ZoneStatus = 11988,
  Zo_ZoneStatusChange = 11999,
  Zo_ZoneType = 12016,
  Zo__ = 12025,
  Zo_xx = 12027,
  Zo_xx000A00 = 12030,
  Zo_xx0A = 12039,
  Zo_xx0A00 = 12044,
  Zo_xx19 = 12051,
  Zo_xx190A = 12056,
  Zo_xx190A00 = 12063,
  Zo_xxxx = 12072,
  Zo_xxxx00 = 12077,
  Zo_xxxx0A00 = 12084,
  Zo_xxxxyy = 12093,
  Zo_xxxxyyyy = 12100,
  Zo_xxxxyyyy0A00 = 12109,
  Zo_xxxxyyzz = 12122,
  Zo_xxyy = 12131,
  Zo_xxyy0A00 = 12136,
  Zo_xxyyyy = 12145,
  Zo_xxyyyy000000000000 = 12152,
  Zo_xxyyyy0A0000000000 = 12171,
  Zo_xxyyyyzz = 12190,
  Zo_xxyyyyzzzz = 12199,
  Zo_xxyyzzzz = 12210,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
