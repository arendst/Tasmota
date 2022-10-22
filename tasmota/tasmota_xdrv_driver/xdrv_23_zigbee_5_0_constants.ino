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
  "GPHueStop" "\x00"
  "GPIdentify" "\x00"
  "GPLevelStop" "\x00"
  "GPLockDoor" "\x00"
  "GPMoveColor" "\x00"
  "GPMoveDown" "\x00"
  "GPMoveDownOnOff" "\x00"
  "GPMoveHueDown" "\x00"
  "GPMoveHueUp" "\x00"
  "GPMoveSatDown" "\x00"
  "GPMoveSatUp" "\x00"
  "GPMoveUp" "\x00"
  "GPMoveUpOnOff" "\x00"
  "GPOff" "\x00"
  "GPOn" "\x00"
  "GPPress1of1" "\x00"
  "GPPress1of2" "\x00"
  "GPPress2of2" "\x00"
  "GPRelease" "\x00"
  "GPRelease1of1" "\x00"
  "GPRelease1of2" "\x00"
  "GPRelease2of2" "\x00"
  "GPSatStop" "\x00"
  "GPScene0" "\x00"
  "GPScene1" "\x00"
  "GPScene10" "\x00"
  "GPScene11" "\x00"
  "GPScene12" "\x00"
  "GPScene13" "\x00"
  "GPScene14" "\x00"
  "GPScene15" "\x00"
  "GPScene2" "\x00"
  "GPScene3" "\x00"
  "GPScene4" "\x00"
  "GPScene5" "\x00"
  "GPScene6" "\x00"
  "GPScene7" "\x00"
  "GPScene8" "\x00"
  "GPScene9" "\x00"
  "GPShortPress1of1" "\x00"
  "GPShortPress1of2" "\x00"
  "GPShortPress2of2" "\x00"
  "GPStepColor" "\x00"
  "GPStepDown" "\x00"
  "GPStepDownOnOff" "\x00"
  "GPStepHueDown" "\x00"
  "GPStepHueUp" "\x00"
  "GPStepSatDown" "\x00"
  "GPStepSatUp" "\x00"
  "GPStepUp" "\x00"
  "GPStepUpOnOff" "\x00"
  "GPToggle" "\x00"
  "GPUnlockDoor" "\x00"
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
  Zo_GPHueStop = 5241,
  Zo_GPIdentify = 5251,
  Zo_GPLevelStop = 5262,
  Zo_GPLockDoor = 5274,
  Zo_GPMoveColor = 5285,
  Zo_GPMoveDown = 5297,
  Zo_GPMoveDownOnOff = 5308,
  Zo_GPMoveHueDown = 5324,
  Zo_GPMoveHueUp = 5338,
  Zo_GPMoveSatDown = 5350,
  Zo_GPMoveSatUp = 5364,
  Zo_GPMoveUp = 5376,
  Zo_GPMoveUpOnOff = 5385,
  Zo_GPOff = 5399,
  Zo_GPOn = 5405,
  Zo_GPPress1of1 = 5410,
  Zo_GPPress1of2 = 5422,
  Zo_GPPress2of2 = 5434,
  Zo_GPRelease = 5446,
  Zo_GPRelease1of1 = 5456,
  Zo_GPRelease1of2 = 5470,
  Zo_GPRelease2of2 = 5484,
  Zo_GPSatStop = 5498,
  Zo_GPScene0 = 5508,
  Zo_GPScene1 = 5517,
  Zo_GPScene10 = 5526,
  Zo_GPScene11 = 5536,
  Zo_GPScene12 = 5546,
  Zo_GPScene13 = 5556,
  Zo_GPScene14 = 5566,
  Zo_GPScene15 = 5576,
  Zo_GPScene2 = 5586,
  Zo_GPScene3 = 5595,
  Zo_GPScene4 = 5604,
  Zo_GPScene5 = 5613,
  Zo_GPScene6 = 5622,
  Zo_GPScene7 = 5631,
  Zo_GPScene8 = 5640,
  Zo_GPScene9 = 5649,
  Zo_GPShortPress1of1 = 5658,
  Zo_GPShortPress1of2 = 5675,
  Zo_GPShortPress2of2 = 5692,
  Zo_GPStepColor = 5709,
  Zo_GPStepDown = 5721,
  Zo_GPStepDownOnOff = 5732,
  Zo_GPStepHueDown = 5748,
  Zo_GPStepHueUp = 5762,
  Zo_GPStepSatDown = 5774,
  Zo_GPStepSatUp = 5788,
  Zo_GPStepUp = 5800,
  Zo_GPStepUpOnOff = 5809,
  Zo_GPToggle = 5823,
  Zo_GPUnlockDoor = 5832,
  Zo_GenericDeviceClass = 5845,
  Zo_GenericDeviceType = 5864,
  Zo_GetAllGroups = 5882,
  Zo_GetGroup = 5895,
  Zo_GetSceneMembership = 5904,
  Zo_GlassBreak = 5923,
  Zo_GroupNameSupport = 5934,
  Zo_HVACSystemTypeConfiguration = 5951,
  Zo_HWVersion = 5979,
  Zo_HarmonicCurrentMultiplier = 5989,
  Zo_HighTempDwellTripPoint = 6015,
  Zo_HighTempThreshold = 6038,
  Zo_Hue = 6056,
  Zo_HueMove = 6060,
  Zo_HueSat = 6068,
  Zo_HueStep = 6075,
  Zo_HueStepDown = 6083,
  Zo_HueStepUp = 6095,
  Zo_Humidity = 6105,
  Zo_HumidityMaxMeasuredValue = 6114,
  Zo_HumidityMinMeasuredValue = 6139,
  Zo_HumidityTolerance = 6164,
  Zo_IASCIEAddress = 6182,
  Zo_Identify = 6196,
  Zo_IdentifyQuery = 6205,
  Zo_IdentifyTime = 6219,
  Zo_Illuminance = 6232,
  Zo_IlluminanceLevelStatus = 6244,
  Zo_IlluminanceLightSensorType = 6267,
  Zo_IlluminanceMaxMeasuredValue = 6294,
  Zo_IlluminanceMinMeasuredValue = 6322,
  Zo_IlluminanceTargetLevel = 6350,
  Zo_IlluminanceTolerance = 6373,
  Zo_InstalledClosedLimitLift = 6394,
  Zo_InstalledClosedLimitTilt = 6419,
  Zo_InstalledOpenLimitLift = 6444,
  Zo_InstalledOpenLimitTilt = 6467,
  Zo_IntermediateSetpointsLift = 6490,
  Zo_IntermediateSetpointsTilt = 6516,
  Zo_IntrinsicBallastFactor = 6542,
  Zo_LampAlarmMode = 6565,
  Zo_LampBurnHours = 6579,
  Zo_LampBurnHoursTripPoint = 6593,
  Zo_LampManufacturer = 6616,
  Zo_LampRatedHours = 6633,
  Zo_LampType = 6648,
  Zo_LastConfiguredBy = 6657,
  Zo_LastMessageLQI = 6674,
  Zo_LastMessageRSSI = 6689,
  Zo_LastSetTime = 6705,
  Zo_LegrandHeatingMode = 6717,
  Zo_LegrandMode = 6736,
  Zo_LegrandOpt1 = 6748,
  Zo_LegrandOpt2 = 6760,
  Zo_LegrandOpt3 = 6772,
  Zo_LidlPower = 6784,
  Zo_LineCurrent = 6794,
  Zo_LineCurrentPhB = 6806,
  Zo_LineCurrentPhC = 6821,
  Zo_LinkKey = 6836,
  Zo_LocalTemperature = 6844,
  Zo_LocalTemperatureCalibration = 6861,
  Zo_LocalTime = 6889,
  Zo_LocationAge = 6899,
  Zo_LocationDescription = 6911,
  Zo_LocationMethod = 6931,
  Zo_LocationPower = 6946,
  Zo_LocationType = 6960,
  Zo_LockAlarmMask = 6973,
  Zo_LockDefaultConfigurationRegister = 6987,
  Zo_LockEnableInsideStatusLED = 7020,
  Zo_LockEnableLocalProgramming = 7046,
  Zo_LockEnableLogging = 7073,
  Zo_LockEnableOneTouchLocking = 7091,
  Zo_LockEnablePrivacyModeButton = 7117,
  Zo_LockKeypadOperationEventMask = 7145,
  Zo_LockKeypadProgrammingEventMask = 7174,
  Zo_LockLEDSettings = 7205,
  Zo_LockLanguage = 7221,
  Zo_LockManualOperationEventMask = 7234,
  Zo_LockOperatingMode = 7263,
  Zo_LockRFIDOperationEventMask = 7281,
  Zo_LockRFIDProgrammingEventMask = 7308,
  Zo_LockRFOperationEventMask = 7337,
  Zo_LockRFProgrammingEventMask = 7362,
  Zo_LockSoundVolume = 7389,
  Zo_LockState = 7405,
  Zo_LockSupportedOperatingModes = 7415,
  Zo_LockType = 7443,
  Zo_LongPollInterval = 7452,
  Zo_LongPollIntervalMin = 7469,
  Zo_LowTempDwellTripPoint = 7489,
  Zo_LowTempThreshold = 7511,
  Zo_MainsAlarmMask = 7528,
  Zo_MainsFrequency = 7543,
  Zo_MainsVoltage = 7558,
  Zo_MainsVoltageDwellTripPoint = 7571,
  Zo_MainsVoltageMaxThreshold = 7598,
  Zo_MainsVoltageMinThreshold = 7623,
  Zo_Manufacturer = 7648,
  Zo_MaxCoolSetpointLimit = 7661,
  Zo_MaxHeatSetpointLimit = 7682,
  Zo_MaxPINCodeLength = 7703,
  Zo_MaxProxyTableEntries = 7720,
  Zo_MaxRFIDCodeLength = 7741,
  Zo_MaxSearchCounter = 7759,
  Zo_MaxSinkTableEntries = 7776,
  Zo_MaxTempExperienced = 7796,
  Zo_Measured11thHarmonicCurrent = 7815,
  Zo_Measured1stHarmonicCurrent = 7843,
  Zo_Measured3rdHarmonicCurrent = 7870,
  Zo_Measured5thHarmonicCurrent = 7897,
  Zo_Measured7thHarmonicCurrent = 7924,
  Zo_Measured9thHarmonicCurrent = 7951,
  Zo_MeasuredPhase11thHarmonicCurrent = 7978,
  Zo_MeasuredPhase1stHarmonicCurrent = 8011,
  Zo_MeasuredPhase3rdHarmonicCurrent = 8043,
  Zo_MeasuredPhase5thHarmonicCurrent = 8075,
  Zo_MeasuredPhase7thHarmonicCurrent = 8107,
  Zo_MeasuredPhase9thHarmonicCurrent = 8139,
  Zo_MeterTypeID = 8171,
  Zo_MinCoolSetpointLimit = 8183,
  Zo_MinHeatSetpointLimit = 8204,
  Zo_MinPINCodeLength = 8225,
  Zo_MinRFIDCodeLength = 8242,
  Zo_MinSetpointDeadBand = 8260,
  Zo_MinTempExperienced = 8280,
  Zo_Mode = 8299,
  Zo_Model = 8304,
  Zo_ModelId = 8310,
  Zo_MotorStepSize = 8318,
  Zo_Movement = 8332,
  Zo_MullerLightMode = 8341,
  Zo_MultiApplicationType = 8357,
  Zo_MultiDescription = 8378,
  Zo_MultiInApplicationType = 8395,
  Zo_MultiInDescription = 8418,
  Zo_MultiInNumberOfStates = 8437,
  Zo_MultiInOutOfService = 8459,
  Zo_MultiInReliability = 8479,
  Zo_MultiInStatusFlags = 8498,
  Zo_MultiInValue = 8517,
  Zo_MultiNumberOfStates = 8530,
  Zo_MultiOutApplicationType = 8550,
  Zo_MultiOutDescription = 8574,
  Zo_MultiOutNumberOfStates = 8594,
  Zo_MultiOutOfService = 8617,
  Zo_MultiOutOutOfService = 8635,
  Zo_MultiOutReliability = 8656,
  Zo_MultiOutRelinquishDefault = 8676,
  Zo_MultiOutStatusFlags = 8702,
  Zo_MultiOutValue = 8722,
  Zo_MultiReliability = 8736,
  Zo_MultiRelinquishDefault = 8753,
  Zo_MultiStatusFlags = 8776,
  Zo_MultiValue = 8793,
  Zo_MultipleScheduling = 8804,
  Zo_NeutralCurrent = 8823,
  Zo_NotificationRetryNumber = 8838,
  Zo_NotificationRetryTimer = 8862,
  Zo_NumberOfDevices = 8885,
  Zo_NumberOfHolidaySchedulesSupported = 8901,
  Zo_NumberOfLogRecordsSupported = 8935,
  Zo_NumberOfPINUsersSupported = 8963,
  Zo_NumberOfPrimaries = 8989,
  Zo_NumberOfRFIDUsersSupported = 9007,
  Zo_NumberOfResets = 9034,
  Zo_NumberOfTotalUsersSupported = 9049,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 9077,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 9118,
  Zo_NumberOfZoneSensitivityLevelsSupported = 9159,
  Zo_NumberRSSIMeasurements = 9198,
  Zo_NumberofActuationsLift = 9221,
  Zo_NumberofActuationsTilt = 9244,
  Zo_Occupancy = 9267,
  Zo_OccupancySensorType = 9277,
  Zo_OccupiedCoolingSetpoint = 9297,
  Zo_OccupiedHeatingSetpoint = 9321,
  Zo_OffTransitionTime = 9345,
  Zo_OffWaitTime = 9363,
  Zo_OnLevel = 9375,
  Zo_OnOff = 9383,
  Zo_OnOffTransitionTime = 9389,
  Zo_OnTime = 9409,
  Zo_OnTransitionTime = 9416,
  Zo_OpenPeriod = 9433,
  Zo_OppleMode = 9444,
  Zo_OutdoorTemperature = 9454,
  Zo_OverTempTotalDwell = 9473,
  Zo_PICoolingDemand = 9492,
  Zo_PIHeatingDemand = 9508,
  Zo_PIROccupiedToUnoccupiedDelay = 9524,
  Zo_PIRUnoccupiedToOccupiedDelay = 9553,
  Zo_PIRUnoccupiedToOccupiedThreshold = 9582,
  Zo_POD = 9615,
  Zo_Panic = 9619,
  Zo_PartNumber = 9625,
  Zo_PathLossExponent = 9636,
  Zo_PersistentMemoryWrites = 9653,
  Zo_PersonalAlarm = 9676,
  Zo_PhaseHarmonicCurrentMultiplier = 9690,
  Zo_PhysicalClosedLimit = 9721,
  Zo_PhysicalClosedLimitLift = 9741,
  Zo_PhysicalClosedLimitTilt = 9765,
  Zo_PhysicalEnvironment = 9789,
  Zo_Power = 9809,
  Zo_PowerDivisor = 9815,
  Zo_PowerFactor = 9828,
  Zo_PowerFactorPhB = 9840,
  Zo_PowerFactorPhC = 9855,
  Zo_PowerMultiplier = 9870,
  Zo_PowerOffEffect = 9886,
  Zo_PowerOnRecall = 9901,
  Zo_PowerOnTimer = 9915,
  Zo_PowerSource = 9928,
  Zo_PowerThreshold = 9940,
  Zo_Pressure = 9955,
  Zo_PressureMaxMeasuredValue = 9964,
  Zo_PressureMaxScaledValue = 9989,
  Zo_PressureMinMeasuredValue = 10012,
  Zo_PressureMinScaledValue = 10037,
  Zo_PressureScale = 10060,
  Zo_PressureScaledTolerance = 10074,
  Zo_PressureScaledValue = 10098,
  Zo_PressureTolerance = 10118,
  Zo_Primary1Intensity = 10136,
  Zo_Primary1X = 10154,
  Zo_Primary1Y = 10164,
  Zo_Primary2Intensity = 10174,
  Zo_Primary2X = 10192,
  Zo_Primary2Y = 10202,
  Zo_Primary3Intensity = 10212,
  Zo_Primary3X = 10230,
  Zo_Primary3Y = 10240,
  Zo_Primary4Intensity = 10250,
  Zo_Primary4X = 10268,
  Zo_Primary4Y = 10278,
  Zo_Primary5Intensity = 10288,
  Zo_Primary5X = 10306,
  Zo_Primary5Y = 10316,
  Zo_Primary6Intensity = 10326,
  Zo_Primary6X = 10344,
  Zo_Primary6Y = 10354,
  Zo_ProductCode = 10364,
  Zo_ProductRevision = 10376,
  Zo_ProductURL = 10392,
  Zo_ProxyTable = 10403,
  Zo_QualityMeasure = 10414,
  Zo_RGB = 10429,
  Zo_RHDehumidificationSetpoint = 10433,
  Zo_RMSCurrent = 10460,
  Zo_RMSCurrentMax = 10471,
  Zo_RMSCurrentMaxPhB = 10485,
  Zo_RMSCurrentMaxPhC = 10502,
  Zo_RMSCurrentMin = 10519,
  Zo_RMSCurrentMinPhB = 10533,
  Zo_RMSCurrentMinPhC = 10550,
  Zo_RMSCurrentPhB = 10567,
  Zo_RMSCurrentPhC = 10581,
  Zo_RMSExtremeOverVoltage = 10595,
  Zo_RMSExtremeOverVoltagePeriod = 10617,
  Zo_RMSExtremeOverVoltagePeriodPhB = 10645,
  Zo_RMSExtremeOverVoltagePeriodPhC = 10676,
  Zo_RMSExtremeUnderVoltage = 10707,
  Zo_RMSExtremeUnderVoltagePeriod = 10730,
  Zo_RMSExtremeUnderVoltagePeriodPhB = 10759,
  Zo_RMSExtremeUnderVoltagePeriodPhC = 10791,
  Zo_RMSVoltage = 10823,
  Zo_RMSVoltageMax = 10834,
  Zo_RMSVoltageMaxPhB = 10848,
  Zo_RMSVoltageMaxPhC = 10865,
  Zo_RMSVoltageMin = 10882,
  Zo_RMSVoltageMinPhB = 10896,
  Zo_RMSVoltageMinPhC = 10913,
  Zo_RMSVoltagePhB = 10930,
  Zo_RMSVoltagePhC = 10944,
  Zo_RMSVoltageSag = 10958,
  Zo_RMSVoltageSagPeriod = 10972,
  Zo_RMSVoltageSagPeriodPhB = 10992,
  Zo_RMSVoltageSagPeriodPhC = 11015,
  Zo_RMSVoltageSwell = 11038,
  Zo_RMSVoltageSwellPeriod = 11054,
  Zo_RMSVoltageSwellPeriodPhB = 11076,
  Zo_RMSVoltageSwellPeriodPhC = 11101,
  Zo_ReactiveCurrent = 11126,
  Zo_ReactiveCurrentPhB = 11142,
  Zo_ReactiveCurrentPhC = 11161,
  Zo_ReactivePower = 11180,
  Zo_ReactivePowerPhB = 11194,
  Zo_ReactivePowerPhC = 11211,
  Zo_RecallScene = 11228,
  Zo_RelativeHumidity = 11240,
  Zo_RelativeHumidityDisplay = 11257,
  Zo_RelativeHumidityMode = 11281,
  Zo_RemainingTime = 11302,
  Zo_RemoteSensing = 11316,
  Zo_RemoveAllGroups = 11330,
  Zo_RemoveAllScenes = 11346,
  Zo_RemoveGroup = 11362,
  Zo_RemoveScene = 11374,
  Zo_ReportingPeriod = 11386,
  Zo_ResetAlarm = 11402,
  Zo_ResetAllAlarms = 11413,
  Zo_SWBuildID = 11428,
  Zo_Sat = 11438,
  Zo_SatMove = 11442,
  Zo_SatStep = 11450,
  Zo_SceneCount = 11458,
  Zo_SceneNameSupport = 11469,
  Zo_SceneValid = 11486,
  Zo_ScheduleMode = 11497,
  Zo_SeaPressure = 11510,
  Zo_SecurityLevel = 11522,
  Zo_ServerActiveFunctionality = 11536,
  Zo_ServerFunctionality = 11562,
  Zo_SharedSecurityKey = 11582,
  Zo_SharedSecurityKeyType = 11600,
  Zo_ShortPollInterval = 11622,
  Zo_Shutter = 11640,
  Zo_ShutterClose = 11648,
  Zo_ShutterLift = 11661,
  Zo_ShutterOpen = 11673,
  Zo_ShutterStop = 11685,
  Zo_ShutterTilt = 11697,
  Zo_SinkTable = 11709,
  Zo_SoftwareRevision = 11719,
  Zo_StackVersion = 11736,
  Zo_StandardTime = 11749,
  Zo_StartUpOnOff = 11762,
  Zo_Status = 11775,
  Zo_StoreScene = 11782,
  Zo_SwitchActions = 11793,
  Zo_SwitchType = 11807,
  Zo_SystemMode = 11818,
  Zo_TRVBoost = 11829,
  Zo_TRVChildProtection = 11838,
  Zo_TRVMirrorDisplay = 11857,
  Zo_TRVMode = 11874,
  Zo_TRVWindowOpen = 11882,
  Zo_TempTarget = 11896,
  Zo_Temperature = 11907,
  Zo_TemperatureDisplayMode = 11919,
  Zo_TemperatureMaxMeasuredValue = 11942,
  Zo_TemperatureMinMeasuredValue = 11970,
  Zo_TemperatureTolerance = 11998,
  Zo_TerncyDuration = 12019,
  Zo_TerncyRotate = 12034,
  Zo_ThSetpoint = 12047,
  Zo_ThermostatAlarmMask = 12058,
  Zo_ThermostatKeypadLockout = 12078,
  Zo_ThermostatOccupancy = 12102,
  Zo_ThermostatRunningMode = 12122,
  Zo_ThermostatScheduleProgrammingVisibility = 12144,
  Zo_Time = 12184,
  Zo_TimeEpoch = 12189,
  Zo_TimeStatus = 12199,
  Zo_TimeZone = 12210,
  Zo_TotalActivePower = 12219,
  Zo_TotalApparentPower = 12236,
  Zo_TotalProfileNum = 12255,
  Zo_TotalReactivePower = 12271,
  Zo_TuyaCalibration = 12290,
  Zo_TuyaCalibrationTime = 12306,
  Zo_TuyaMCUVersion = 12326,
  Zo_TuyaMotorReversal = 12341,
  Zo_TuyaMovingState = 12359,
  Zo_TuyaQuery = 12375,
  Zo_UnoccupiedCoolingSetpoint = 12385,
  Zo_UnoccupiedHeatingSetpoint = 12411,
  Zo_UtilityName = 12437,
  Zo_ValidUntilTime = 12449,
  Zo_ValvePosition = 12464,
  Zo_VelocityLift = 12478,
  Zo_ViewGroup = 12491,
  Zo_ViewScene = 12501,
  Zo_Water = 12511,
  Zo_WhitePointX = 12517,
  Zo_WhitePointY = 12529,
  Zo_WindowCoveringType = 12541,
  Zo_X = 12560,
  Zo_Y = 12562,
  Zo_ZCLVersion = 12564,
  Zo_ZoneID = 12575,
  Zo_ZoneState = 12582,
  Zo_ZoneStatus = 12592,
  Zo_ZoneStatusChange = 12603,
  Zo_ZoneType = 12620,
  Zo__ = 12629,
  Zo_xx = 12631,
  Zo_xx000A00 = 12634,
  Zo_xx0A = 12643,
  Zo_xx0A00 = 12648,
  Zo_xx19 = 12655,
  Zo_xx190A = 12660,
  Zo_xx190A00 = 12667,
  Zo_xxxx = 12676,
  Zo_xxxx00 = 12681,
  Zo_xxxx0A00 = 12688,
  Zo_xxxxyy = 12697,
  Zo_xxxxyyyy = 12704,
  Zo_xxxxyyyy0A00 = 12713,
  Zo_xxxxyyzz = 12726,
  Zo_xxyy = 12735,
  Zo_xxyy0A00 = 12740,
  Zo_xxyyyy = 12749,
  Zo_xxyyyy000000000000 = 12756,
  Zo_xxyyyy0A0000000000 = 12775,
  Zo_xxyyyyzz = 12794,
  Zo_xxyyyyzzzz = 12803,
  Zo_xxyyzzzz = 12814,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
