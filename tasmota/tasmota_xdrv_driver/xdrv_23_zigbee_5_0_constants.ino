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
  Zo_ColorStep = 3606,
  Zo_ColorTempMove = 3616,
  Zo_ColorTempMoveDown = 3630,
  Zo_ColorTempMoveStop = 3648,
  Zo_ColorTempMoveUp = 3666,
  Zo_ColorTempPhysicalMaxMireds = 3682,
  Zo_ColorTempPhysicalMinMireds = 3709,
  Zo_ColorTempStep = 3736,
  Zo_ColorTempStepDown = 3750,
  Zo_ColorTempStepUp = 3768,
  Zo_CommissioningWindow = 3784,
  Zo_CommunicationMode = 3804,
  Zo_CompanyName = 3822,
  Zo_CompensationText = 3834,
  Zo_ConfigStatus = 3851,
  Zo_Contact = 3864,
  Zo_ControlSequenceOfOperation = 3872,
  Zo_Coordinate1 = 3899,
  Zo_Coordinate2 = 3911,
  Zo_Coordinate3 = 3923,
  Zo_CurrentGroup = 3935,
  Zo_CurrentPositionLift = 3948,
  Zo_CurrentPositionLiftPercentage = 3968,
  Zo_CurrentPositionTilt = 3998,
  Zo_CurrentPositionTiltPercentage = 4018,
  Zo_CurrentScene = 4048,
  Zo_CurrentTemperature = 4061,
  Zo_CurrentTemperatureSetPoint = 4080,
  Zo_CurrentZoneSensitivityLevel = 4107,
  Zo_CustomerName = 4135,
  Zo_DCCurrent = 4148,
  Zo_DCCurrentDivisor = 4158,
  Zo_DCCurrentMax = 4175,
  Zo_DCCurrentMin = 4188,
  Zo_DCCurrentMultiplier = 4201,
  Zo_DCCurrentOverload = 4221,
  Zo_DCOverloadAlarmsMask = 4239,
  Zo_DCPower = 4260,
  Zo_DCPowerDivisor = 4268,
  Zo_DCPowerMax = 4283,
  Zo_DCPowerMin = 4294,
  Zo_DCPowerMultiplier = 4305,
  Zo_DCVoltage = 4323,
  Zo_DCVoltageDivisor = 4333,
  Zo_DCVoltageMax = 4350,
  Zo_DCVoltageMin = 4363,
  Zo_DCVoltageMultiplier = 4376,
  Zo_DCVoltageOverload = 4396,
  Zo_DataQualityID = 4414,
  Zo_DateCode = 4428,
  Zo_DecelerationTimeLift = 4437,
  Zo_DefaultMoveRate = 4458,
  Zo_DehumidificationCooling = 4474,
  Zo_DehumidificationHysteresis = 4498,
  Zo_DehumidificationLockout = 4525,
  Zo_DehumidificationMaxCool = 4549,
  Zo_DeviceEnabled = 4573,
  Zo_DeviceTempAlarmMask = 4587,
  Zo_Dimmer = 4607,
  Zo_DimmerDown = 4614,
  Zo_DimmerMove = 4625,
  Zo_DimmerOptions = 4636,
  Zo_DimmerRemainingTime = 4650,
  Zo_DimmerStep = 4670,
  Zo_DimmerStepDown = 4681,
  Zo_DimmerStepUp = 4696,
  Zo_DimmerStop = 4709,
  Zo_DimmerUp = 4720,
  Zo_DisableLocalConfig = 4729,
  Zo_DoorClosedEvents = 4748,
  Zo_DoorOpenEvents = 4765,
  Zo_DoorState = 4780,
  Zo_DriftCompensation = 4790,
  Zo_DstEnd = 4808,
  Zo_DstShift = 4815,
  Zo_DstStart = 4824,
  Zo_ElectricalMeasurementType = 4833,
  Zo_EnergyFormatting = 4859,
  Zo_EnergyRemote = 4876,
  Zo_EnergyTotal = 4889,
  Zo_EnhancedColorMode = 4901,
  Zo_EnhancedCurrentHue = 4919,
  Zo_EurotronicErrors = 4938,
  Zo_EurotronicHostFlags = 4955,
  Zo_FanMode = 4975,
  Zo_FanModeSequence = 4983,
  Zo_FastPollTimeout = 4999,
  Zo_FastPollTimeoutMax = 5015,
  Zo_Fire = 5034,
  Zo_FlowMaxMeasuredValue = 5039,
  Zo_FlowMinMeasuredValue = 5060,
  Zo_FlowRate = 5081,
  Zo_FlowTolerance = 5090,
  Zo_GenericDeviceClass = 5104,
  Zo_GenericDeviceType = 5123,
  Zo_GetAllGroups = 5141,
  Zo_GetGroup = 5154,
  Zo_GetSceneMembership = 5163,
  Zo_GlassBreak = 5182,
  Zo_GroupNameSupport = 5193,
  Zo_HVACSystemTypeConfiguration = 5210,
  Zo_HWVersion = 5238,
  Zo_HarmonicCurrentMultiplier = 5248,
  Zo_HighTempDwellTripPoint = 5274,
  Zo_HighTempThreshold = 5297,
  Zo_Hue = 5315,
  Zo_HueMove = 5319,
  Zo_HueSat = 5327,
  Zo_HueStep = 5334,
  Zo_HueStepDown = 5342,
  Zo_HueStepUp = 5354,
  Zo_Humidity = 5364,
  Zo_HumidityMaxMeasuredValue = 5373,
  Zo_HumidityMinMeasuredValue = 5398,
  Zo_HumidityTolerance = 5423,
  Zo_IASCIEAddress = 5441,
  Zo_Identify = 5455,
  Zo_IdentifyQuery = 5464,
  Zo_IdentifyTime = 5478,
  Zo_Illuminance = 5491,
  Zo_IlluminanceLevelStatus = 5503,
  Zo_IlluminanceLightSensorType = 5526,
  Zo_IlluminanceMaxMeasuredValue = 5553,
  Zo_IlluminanceMinMeasuredValue = 5581,
  Zo_IlluminanceTargetLevel = 5609,
  Zo_IlluminanceTolerance = 5632,
  Zo_InstalledClosedLimitLift = 5653,
  Zo_InstalledClosedLimitTilt = 5678,
  Zo_InstalledOpenLimitLift = 5703,
  Zo_InstalledOpenLimitTilt = 5726,
  Zo_IntermediateSetpointsLift = 5749,
  Zo_IntermediateSetpointsTilt = 5775,
  Zo_IntrinsicBallastFactor = 5801,
  Zo_LampAlarmMode = 5824,
  Zo_LampBurnHours = 5838,
  Zo_LampBurnHoursTripPoint = 5852,
  Zo_LampManufacturer = 5875,
  Zo_LampRatedHours = 5892,
  Zo_LampType = 5907,
  Zo_LastConfiguredBy = 5916,
  Zo_LastMessageLQI = 5933,
  Zo_LastMessageRSSI = 5948,
  Zo_LastSetTime = 5964,
  Zo_LegrandHeatingMode = 5976,
  Zo_LegrandMode = 5995,
  Zo_LegrandOpt1 = 6007,
  Zo_LegrandOpt2 = 6019,
  Zo_LegrandOpt3 = 6031,
  Zo_LidlPower = 6043,
  Zo_LineCurrent = 6053,
  Zo_LineCurrentPhB = 6065,
  Zo_LineCurrentPhC = 6080,
  Zo_LinkKey = 6095,
  Zo_LocalTemperature = 6103,
  Zo_LocalTemperatureCalibration = 6120,
  Zo_LocalTime = 6148,
  Zo_LocationAge = 6158,
  Zo_LocationDescription = 6170,
  Zo_LocationMethod = 6190,
  Zo_LocationPower = 6205,
  Zo_LocationType = 6219,
  Zo_LockAlarmMask = 6232,
  Zo_LockDefaultConfigurationRegister = 6246,
  Zo_LockEnableInsideStatusLED = 6279,
  Zo_LockEnableLocalProgramming = 6305,
  Zo_LockEnableLogging = 6332,
  Zo_LockEnableOneTouchLocking = 6350,
  Zo_LockEnablePrivacyModeButton = 6376,
  Zo_LockKeypadOperationEventMask = 6404,
  Zo_LockKeypadProgrammingEventMask = 6433,
  Zo_LockLEDSettings = 6464,
  Zo_LockLanguage = 6480,
  Zo_LockManualOperationEventMask = 6493,
  Zo_LockOperatingMode = 6522,
  Zo_LockRFIDOperationEventMask = 6540,
  Zo_LockRFIDProgrammingEventMask = 6567,
  Zo_LockRFOperationEventMask = 6596,
  Zo_LockRFProgrammingEventMask = 6621,
  Zo_LockSoundVolume = 6648,
  Zo_LockState = 6664,
  Zo_LockSupportedOperatingModes = 6674,
  Zo_LockType = 6702,
  Zo_LongPollInterval = 6711,
  Zo_LongPollIntervalMin = 6728,
  Zo_LowTempDwellTripPoint = 6748,
  Zo_LowTempThreshold = 6770,
  Zo_MainsAlarmMask = 6787,
  Zo_MainsFrequency = 6802,
  Zo_MainsVoltage = 6817,
  Zo_MainsVoltageDwellTripPoint = 6830,
  Zo_MainsVoltageMaxThreshold = 6857,
  Zo_MainsVoltageMinThreshold = 6882,
  Zo_Manufacturer = 6907,
  Zo_MaxCoolSetpointLimit = 6920,
  Zo_MaxHeatSetpointLimit = 6941,
  Zo_MaxPINCodeLength = 6962,
  Zo_MaxProxyTableEntries = 6979,
  Zo_MaxRFIDCodeLength = 7000,
  Zo_MaxSearchCounter = 7018,
  Zo_MaxSinkTableEntries = 7035,
  Zo_MaxTempExperienced = 7055,
  Zo_Measured11thHarmonicCurrent = 7074,
  Zo_Measured1stHarmonicCurrent = 7102,
  Zo_Measured3rdHarmonicCurrent = 7129,
  Zo_Measured5thHarmonicCurrent = 7156,
  Zo_Measured7thHarmonicCurrent = 7183,
  Zo_Measured9thHarmonicCurrent = 7210,
  Zo_MeasuredPhase11thHarmonicCurrent = 7237,
  Zo_MeasuredPhase1stHarmonicCurrent = 7270,
  Zo_MeasuredPhase3rdHarmonicCurrent = 7302,
  Zo_MeasuredPhase5thHarmonicCurrent = 7334,
  Zo_MeasuredPhase7thHarmonicCurrent = 7366,
  Zo_MeasuredPhase9thHarmonicCurrent = 7398,
  Zo_MeterTypeID = 7430,
  Zo_MinCoolSetpointLimit = 7442,
  Zo_MinHeatSetpointLimit = 7463,
  Zo_MinPINCodeLength = 7484,
  Zo_MinRFIDCodeLength = 7501,
  Zo_MinSetpointDeadBand = 7519,
  Zo_MinTempExperienced = 7539,
  Zo_Mode = 7558,
  Zo_Model = 7563,
  Zo_ModelId = 7569,
  Zo_MotorStepSize = 7577,
  Zo_Movement = 7591,
  Zo_MullerLightMode = 7600,
  Zo_MultiApplicationType = 7616,
  Zo_MultiDescription = 7637,
  Zo_MultiInApplicationType = 7654,
  Zo_MultiInDescription = 7677,
  Zo_MultiInNumberOfStates = 7696,
  Zo_MultiInOutOfService = 7718,
  Zo_MultiInReliability = 7738,
  Zo_MultiInStatusFlags = 7757,
  Zo_MultiInValue = 7776,
  Zo_MultiNumberOfStates = 7789,
  Zo_MultiOutApplicationType = 7809,
  Zo_MultiOutDescription = 7833,
  Zo_MultiOutNumberOfStates = 7853,
  Zo_MultiOutOfService = 7876,
  Zo_MultiOutOutOfService = 7894,
  Zo_MultiOutReliability = 7915,
  Zo_MultiOutRelinquishDefault = 7935,
  Zo_MultiOutStatusFlags = 7961,
  Zo_MultiOutValue = 7981,
  Zo_MultiReliability = 7995,
  Zo_MultiRelinquishDefault = 8012,
  Zo_MultiStatusFlags = 8035,
  Zo_MultiValue = 8052,
  Zo_MultipleScheduling = 8063,
  Zo_NeutralCurrent = 8082,
  Zo_NotificationRetryNumber = 8097,
  Zo_NotificationRetryTimer = 8121,
  Zo_NumberOfDevices = 8144,
  Zo_NumberOfHolidaySchedulesSupported = 8160,
  Zo_NumberOfLogRecordsSupported = 8194,
  Zo_NumberOfPINUsersSupported = 8222,
  Zo_NumberOfPrimaries = 8248,
  Zo_NumberOfRFIDUsersSupported = 8266,
  Zo_NumberOfResets = 8293,
  Zo_NumberOfTotalUsersSupported = 8308,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 8336,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 8377,
  Zo_NumberOfZoneSensitivityLevelsSupported = 8418,
  Zo_NumberRSSIMeasurements = 8457,
  Zo_NumberofActuationsLift = 8480,
  Zo_NumberofActuationsTilt = 8503,
  Zo_Occupancy = 8526,
  Zo_OccupancySensorType = 8536,
  Zo_OccupiedCoolingSetpoint = 8556,
  Zo_OccupiedHeatingSetpoint = 8580,
  Zo_OffTransitionTime = 8604,
  Zo_OffWaitTime = 8622,
  Zo_OnLevel = 8634,
  Zo_OnOff = 8642,
  Zo_OnOffTransitionTime = 8648,
  Zo_OnTime = 8668,
  Zo_OnTransitionTime = 8675,
  Zo_OpenPeriod = 8692,
  Zo_OppleMode = 8703,
  Zo_OutdoorTemperature = 8713,
  Zo_OverTempTotalDwell = 8732,
  Zo_PICoolingDemand = 8751,
  Zo_PIHeatingDemand = 8767,
  Zo_PIROccupiedToUnoccupiedDelay = 8783,
  Zo_PIRUnoccupiedToOccupiedDelay = 8812,
  Zo_PIRUnoccupiedToOccupiedThreshold = 8841,
  Zo_POD = 8874,
  Zo_Panic = 8878,
  Zo_PartNumber = 8884,
  Zo_PathLossExponent = 8895,
  Zo_PersistentMemoryWrites = 8912,
  Zo_PersonalAlarm = 8935,
  Zo_PhaseHarmonicCurrentMultiplier = 8949,
  Zo_PhysicalClosedLimit = 8980,
  Zo_PhysicalClosedLimitLift = 9000,
  Zo_PhysicalClosedLimitTilt = 9024,
  Zo_PhysicalEnvironment = 9048,
  Zo_Power = 9068,
  Zo_PowerDivisor = 9074,
  Zo_PowerFactor = 9087,
  Zo_PowerFactorPhB = 9099,
  Zo_PowerFactorPhC = 9114,
  Zo_PowerMultiplier = 9129,
  Zo_PowerOffEffect = 9145,
  Zo_PowerOnRecall = 9160,
  Zo_PowerOnTimer = 9174,
  Zo_PowerSource = 9187,
  Zo_PowerThreshold = 9199,
  Zo_Pressure = 9214,
  Zo_PressureMaxMeasuredValue = 9223,
  Zo_PressureMaxScaledValue = 9248,
  Zo_PressureMinMeasuredValue = 9271,
  Zo_PressureMinScaledValue = 9296,
  Zo_PressureScale = 9319,
  Zo_PressureScaledTolerance = 9333,
  Zo_PressureScaledValue = 9357,
  Zo_PressureTolerance = 9377,
  Zo_Primary1Intensity = 9395,
  Zo_Primary1X = 9413,
  Zo_Primary1Y = 9423,
  Zo_Primary2Intensity = 9433,
  Zo_Primary2X = 9451,
  Zo_Primary2Y = 9461,
  Zo_Primary3Intensity = 9471,
  Zo_Primary3X = 9489,
  Zo_Primary3Y = 9499,
  Zo_Primary4Intensity = 9509,
  Zo_Primary4X = 9527,
  Zo_Primary4Y = 9537,
  Zo_Primary5Intensity = 9547,
  Zo_Primary5X = 9565,
  Zo_Primary5Y = 9575,
  Zo_Primary6Intensity = 9585,
  Zo_Primary6X = 9603,
  Zo_Primary6Y = 9613,
  Zo_ProductCode = 9623,
  Zo_ProductRevision = 9635,
  Zo_ProductURL = 9651,
  Zo_ProxyTable = 9662,
  Zo_QualityMeasure = 9673,
  Zo_RGB = 9688,
  Zo_RHDehumidificationSetpoint = 9692,
  Zo_RMSCurrent = 9719,
  Zo_RMSCurrentMax = 9730,
  Zo_RMSCurrentMaxPhB = 9744,
  Zo_RMSCurrentMaxPhC = 9761,
  Zo_RMSCurrentMin = 9778,
  Zo_RMSCurrentMinPhB = 9792,
  Zo_RMSCurrentMinPhC = 9809,
  Zo_RMSCurrentPhB = 9826,
  Zo_RMSCurrentPhC = 9840,
  Zo_RMSExtremeOverVoltage = 9854,
  Zo_RMSExtremeOverVoltagePeriod = 9876,
  Zo_RMSExtremeOverVoltagePeriodPhB = 9904,
  Zo_RMSExtremeOverVoltagePeriodPhC = 9935,
  Zo_RMSExtremeUnderVoltage = 9966,
  Zo_RMSExtremeUnderVoltagePeriod = 9989,
  Zo_RMSExtremeUnderVoltagePeriodPhB = 10018,
  Zo_RMSExtremeUnderVoltagePeriodPhC = 10050,
  Zo_RMSVoltage = 10082,
  Zo_RMSVoltageMax = 10093,
  Zo_RMSVoltageMaxPhB = 10107,
  Zo_RMSVoltageMaxPhC = 10124,
  Zo_RMSVoltageMin = 10141,
  Zo_RMSVoltageMinPhB = 10155,
  Zo_RMSVoltageMinPhC = 10172,
  Zo_RMSVoltagePhB = 10189,
  Zo_RMSVoltagePhC = 10203,
  Zo_RMSVoltageSag = 10217,
  Zo_RMSVoltageSagPeriod = 10231,
  Zo_RMSVoltageSagPeriodPhB = 10251,
  Zo_RMSVoltageSagPeriodPhC = 10274,
  Zo_RMSVoltageSwell = 10297,
  Zo_RMSVoltageSwellPeriod = 10313,
  Zo_RMSVoltageSwellPeriodPhB = 10335,
  Zo_RMSVoltageSwellPeriodPhC = 10360,
  Zo_ReactiveCurrent = 10385,
  Zo_ReactiveCurrentPhB = 10401,
  Zo_ReactiveCurrentPhC = 10420,
  Zo_ReactivePower = 10439,
  Zo_ReactivePowerPhB = 10453,
  Zo_ReactivePowerPhC = 10470,
  Zo_RecallScene = 10487,
  Zo_RelativeHumidity = 10499,
  Zo_RelativeHumidityDisplay = 10516,
  Zo_RelativeHumidityMode = 10540,
  Zo_RemainingTime = 10561,
  Zo_RemoteSensing = 10575,
  Zo_RemoveAllGroups = 10589,
  Zo_RemoveAllScenes = 10605,
  Zo_RemoveGroup = 10621,
  Zo_RemoveScene = 10633,
  Zo_ReportingPeriod = 10645,
  Zo_ResetAlarm = 10661,
  Zo_ResetAllAlarms = 10672,
  Zo_SWBuildID = 10687,
  Zo_Sat = 10697,
  Zo_SatMove = 10701,
  Zo_SatStep = 10709,
  Zo_SceneCount = 10717,
  Zo_SceneNameSupport = 10728,
  Zo_SceneValid = 10745,
  Zo_ScheduleMode = 10756,
  Zo_SeaPressure = 10769,
  Zo_SecurityLevel = 10781,
  Zo_ServerActiveFunctionality = 10795,
  Zo_ServerFunctionality = 10821,
  Zo_SharedSecurityKey = 10841,
  Zo_SharedSecurityKeyType = 10859,
  Zo_ShortPollInterval = 10881,
  Zo_Shutter = 10899,
  Zo_ShutterClose = 10907,
  Zo_ShutterLift = 10920,
  Zo_ShutterOpen = 10932,
  Zo_ShutterStop = 10944,
  Zo_ShutterTilt = 10956,
  Zo_SinkTable = 10968,
  Zo_SoftwareRevision = 10978,
  Zo_StackVersion = 10995,
  Zo_StandardTime = 11008,
  Zo_StartUpOnOff = 11021,
  Zo_Status = 11034,
  Zo_StoreScene = 11041,
  Zo_SwitchActions = 11052,
  Zo_SwitchType = 11066,
  Zo_SystemMode = 11077,
  Zo_TRVBoost = 11088,
  Zo_TRVChildProtection = 11097,
  Zo_TRVMirrorDisplay = 11116,
  Zo_TRVMode = 11133,
  Zo_TRVWindowOpen = 11141,
  Zo_TempTarget = 11155,
  Zo_Temperature = 11166,
  Zo_TemperatureDisplayMode = 11178,
  Zo_TemperatureMaxMeasuredValue = 11201,
  Zo_TemperatureMinMeasuredValue = 11229,
  Zo_TemperatureTolerance = 11257,
  Zo_TerncyDuration = 11278,
  Zo_TerncyRotate = 11293,
  Zo_ThSetpoint = 11306,
  Zo_ThermostatAlarmMask = 11317,
  Zo_ThermostatKeypadLockout = 11337,
  Zo_ThermostatOccupancy = 11361,
  Zo_ThermostatRunningMode = 11381,
  Zo_ThermostatScheduleProgrammingVisibility = 11403,
  Zo_Time = 11443,
  Zo_TimeEpoch = 11448,
  Zo_TimeStatus = 11458,
  Zo_TimeZone = 11469,
  Zo_TotalActivePower = 11478,
  Zo_TotalApparentPower = 11495,
  Zo_TotalProfileNum = 11514,
  Zo_TotalReactivePower = 11530,
  Zo_TuyaCalibration = 11549,
  Zo_TuyaCalibrationTime = 11565,
  Zo_TuyaMCUVersion = 11585,
  Zo_TuyaMotorReversal = 11600,
  Zo_TuyaMovingState = 11618,
  Zo_TuyaQuery = 11634,
  Zo_UnoccupiedCoolingSetpoint = 11644,
  Zo_UnoccupiedHeatingSetpoint = 11670,
  Zo_UtilityName = 11696,
  Zo_ValidUntilTime = 11708,
  Zo_ValvePosition = 11723,
  Zo_VelocityLift = 11737,
  Zo_ViewGroup = 11750,
  Zo_ViewScene = 11760,
  Zo_Water = 11770,
  Zo_WhitePointX = 11776,
  Zo_WhitePointY = 11788,
  Zo_WindowCoveringType = 11800,
  Zo_X = 11819,
  Zo_Y = 11821,
  Zo_ZCLVersion = 11823,
  Zo_ZoneID = 11834,
  Zo_ZoneState = 11841,
  Zo_ZoneStatus = 11851,
  Zo_ZoneStatusChange = 11862,
  Zo_ZoneType = 11879,
  Zo__ = 11888,
  Zo_xx = 11890,
  Zo_xx000A00 = 11893,
  Zo_xx0A = 11902,
  Zo_xx0A00 = 11907,
  Zo_xx19 = 11914,
  Zo_xx190A = 11919,
  Zo_xx190A00 = 11926,
  Zo_xxxx = 11935,
  Zo_xxxx00 = 11940,
  Zo_xxxx0A00 = 11947,
  Zo_xxxxyy = 11956,
  Zo_xxxxyyyy = 11963,
  Zo_xxxxyyyy0A00 = 11972,
  Zo_xxxxyyzz = 11985,
  Zo_xxyy = 11994,
  Zo_xxyy0A00 = 11999,
  Zo_xxyyyy = 12008,
  Zo_xxyyyy000000000000 = 12015,
  Zo_xxyyyy0A0000000000 = 12034,
  Zo_xxyyyyzz = 12053,
  Zo_xxyyyyzzzz = 12062,
  Zo_xxyyzzzz = 12073,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
