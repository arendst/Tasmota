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
  "ControlTemperature" "\x00"
  "Coordinate1" "\x00"
  "Coordinate2" "\x00"
  "Coordinate3" "\x00"
  "CurrentBlock" "\x00"
  "CurrentBlockPeriodConsumptionDelivered" "\x00"
  "CurrentGroup" "\x00"
  "CurrentInletEnergyCarrierDemand" "\x00"
  "CurrentInletEnergyCarrierSummation" "\x00"
  "CurrentMaxDemandDelivered" "\x00"
  "CurrentMaxDemandDeliveredTime" "\x00"
  "CurrentMaxDemandReceived" "\x00"
  "CurrentMaxDemandReceivedTime" "\x00"
  "CurrentOutletEnergyCarrierDemand" "\x00"
  "CurrentOutletEnergyCarrierSummation" "\x00"
  "CurrentPositionLift" "\x00"
  "CurrentPositionLiftPercentage" "\x00"
  "CurrentPositionTilt" "\x00"
  "CurrentPositionTiltPercentage" "\x00"
  "CurrentScene" "\x00"
  "CurrentSummationDelivered" "\x00"
  "CurrentSummationReceived" "\x00"
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
  "DFTSummation" "\x00"
  "DailyConsumptionTarget" "\x00"
  "DailyFreezeTime" "\x00"
  "DataQualityID" "\x00"
  "DateCode" "\x00"
  "DecelerationTimeLift" "\x00"
  "DefaultMoveRate" "\x00"
  "DefaultUpdatePeriod" "\x00"
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
  "EnhancedColorMode" "\x00"
  "EnhancedCurrentHue" "\x00"
  "EurotronicErrors" "\x00"
  "EurotronicHostFlags" "\x00"
  "FanMode" "\x00"
  "FanModeSequence" "\x00"
  "FastPollTimeout" "\x00"
  "FastPollTimeoutMax" "\x00"
  "FastPollUpdatePeriod" "\x00"
  "Fire" "\x00"
  "FlowMaxMeasuredValue" "\x00"
  "FlowMinMeasuredValue" "\x00"
  "FlowRate" "\x00"
  "FlowRestriction" "\x00"
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
  "InletTemperature" "\x00"
  "InstalledClosedLimitLift" "\x00"
  "InstalledClosedLimitTilt" "\x00"
  "InstalledOpenLimitLift" "\x00"
  "InstalledOpenLimitTilt" "\x00"
  "IntermediateSetpointsLift" "\x00"
  "IntermediateSetpointsTilt" "\x00"
  "IntervalReadReportingPeriod" "\x00"
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
  "OutletTemperature" "\x00"
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
  "PresetReadingTime" "\x00"
  "Pressure" "\x00"
  "PressureMaxMeasuredValue" "\x00"
  "PressureMaxScaledValue" "\x00"
  "PressureMinMeasuredValue" "\x00"
  "PressureMinScaledValue" "\x00"
  "PressureScale" "\x00"
  "PressureScaledTolerance" "\x00"
  "PressureScaledValue" "\x00"
  "PressureTolerance" "\x00"
  "PreviousBlockPeriodConsumptionDelivered" "\x00"
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
  "ProfileIntervalPeriod" "\x00"
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
  "ReadingSnapShotTime" "\x00"
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
  "SupplyStatus" "\x00"
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
  "VolumePerReport" "\x00"
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
  Zo_ControlTemperature = 3926,
  Zo_Coordinate1 = 3945,
  Zo_Coordinate2 = 3957,
  Zo_Coordinate3 = 3969,
  Zo_CurrentBlock = 3981,
  Zo_CurrentBlockPeriodConsumptionDelivered = 3994,
  Zo_CurrentGroup = 4033,
  Zo_CurrentInletEnergyCarrierDemand = 4046,
  Zo_CurrentInletEnergyCarrierSummation = 4078,
  Zo_CurrentMaxDemandDelivered = 4113,
  Zo_CurrentMaxDemandDeliveredTime = 4139,
  Zo_CurrentMaxDemandReceived = 4169,
  Zo_CurrentMaxDemandReceivedTime = 4194,
  Zo_CurrentOutletEnergyCarrierDemand = 4223,
  Zo_CurrentOutletEnergyCarrierSummation = 4256,
  Zo_CurrentPositionLift = 4292,
  Zo_CurrentPositionLiftPercentage = 4312,
  Zo_CurrentPositionTilt = 4342,
  Zo_CurrentPositionTiltPercentage = 4362,
  Zo_CurrentScene = 4392,
  Zo_CurrentSummationDelivered = 4405,
  Zo_CurrentSummationReceived = 4431,
  Zo_CurrentTemperature = 4456,
  Zo_CurrentTemperatureSetPoint = 4475,
  Zo_CurrentZoneSensitivityLevel = 4502,
  Zo_CustomerName = 4530,
  Zo_DCCurrent = 4543,
  Zo_DCCurrentDivisor = 4553,
  Zo_DCCurrentMax = 4570,
  Zo_DCCurrentMin = 4583,
  Zo_DCCurrentMultiplier = 4596,
  Zo_DCCurrentOverload = 4616,
  Zo_DCOverloadAlarmsMask = 4634,
  Zo_DCPower = 4655,
  Zo_DCPowerDivisor = 4663,
  Zo_DCPowerMax = 4678,
  Zo_DCPowerMin = 4689,
  Zo_DCPowerMultiplier = 4700,
  Zo_DCVoltage = 4718,
  Zo_DCVoltageDivisor = 4728,
  Zo_DCVoltageMax = 4745,
  Zo_DCVoltageMin = 4758,
  Zo_DCVoltageMultiplier = 4771,
  Zo_DCVoltageOverload = 4791,
  Zo_DFTSummation = 4809,
  Zo_DailyConsumptionTarget = 4822,
  Zo_DailyFreezeTime = 4845,
  Zo_DataQualityID = 4861,
  Zo_DateCode = 4875,
  Zo_DecelerationTimeLift = 4884,
  Zo_DefaultMoveRate = 4905,
  Zo_DefaultUpdatePeriod = 4921,
  Zo_DehumidificationCooling = 4941,
  Zo_DehumidificationHysteresis = 4965,
  Zo_DehumidificationLockout = 4992,
  Zo_DehumidificationMaxCool = 5016,
  Zo_DeviceEnabled = 5040,
  Zo_DeviceTempAlarmMask = 5054,
  Zo_Dimmer = 5074,
  Zo_DimmerCurrentFrequency = 5081,
  Zo_DimmerDown = 5104,
  Zo_DimmerMaxFrequency = 5115,
  Zo_DimmerMaxLevel = 5134,
  Zo_DimmerMinFrequency = 5149,
  Zo_DimmerMinLevel = 5168,
  Zo_DimmerMove = 5183,
  Zo_DimmerOptions = 5194,
  Zo_DimmerRemainingTime = 5208,
  Zo_DimmerStartUpLevel = 5228,
  Zo_DimmerStep = 5247,
  Zo_DimmerStepDown = 5258,
  Zo_DimmerStepUp = 5273,
  Zo_DimmerStop = 5286,
  Zo_DimmerUp = 5297,
  Zo_DisableLocalConfig = 5306,
  Zo_DoorClosedEvents = 5325,
  Zo_DoorOpenEvents = 5342,
  Zo_DoorState = 5357,
  Zo_DriftCompensation = 5367,
  Zo_DstEnd = 5385,
  Zo_DstShift = 5392,
  Zo_DstStart = 5401,
  Zo_ElectricalMeasurementType = 5410,
  Zo_EnergyFormatting = 5436,
  Zo_EnergyRemote = 5453,
  Zo_EnhancedColorMode = 5466,
  Zo_EnhancedCurrentHue = 5484,
  Zo_EurotronicErrors = 5503,
  Zo_EurotronicHostFlags = 5520,
  Zo_FanMode = 5540,
  Zo_FanModeSequence = 5548,
  Zo_FastPollTimeout = 5564,
  Zo_FastPollTimeoutMax = 5580,
  Zo_FastPollUpdatePeriod = 5599,
  Zo_Fire = 5620,
  Zo_FlowMaxMeasuredValue = 5625,
  Zo_FlowMinMeasuredValue = 5646,
  Zo_FlowRate = 5667,
  Zo_FlowRestriction = 5676,
  Zo_FlowTolerance = 5692,
  Zo_GPHueStop = 5706,
  Zo_GPIdentify = 5716,
  Zo_GPLevelStop = 5727,
  Zo_GPLockDoor = 5739,
  Zo_GPMoveColor = 5750,
  Zo_GPMoveDown = 5762,
  Zo_GPMoveDownOnOff = 5773,
  Zo_GPMoveHueDown = 5789,
  Zo_GPMoveHueUp = 5803,
  Zo_GPMoveSatDown = 5815,
  Zo_GPMoveSatUp = 5829,
  Zo_GPMoveUp = 5841,
  Zo_GPMoveUpOnOff = 5850,
  Zo_GPOff = 5864,
  Zo_GPOn = 5870,
  Zo_GPPress1of1 = 5875,
  Zo_GPPress1of2 = 5887,
  Zo_GPPress2of2 = 5899,
  Zo_GPRelease = 5911,
  Zo_GPRelease1of1 = 5921,
  Zo_GPRelease1of2 = 5935,
  Zo_GPRelease2of2 = 5949,
  Zo_GPSatStop = 5963,
  Zo_GPScene0 = 5973,
  Zo_GPScene1 = 5982,
  Zo_GPScene10 = 5991,
  Zo_GPScene11 = 6001,
  Zo_GPScene12 = 6011,
  Zo_GPScene13 = 6021,
  Zo_GPScene14 = 6031,
  Zo_GPScene15 = 6041,
  Zo_GPScene2 = 6051,
  Zo_GPScene3 = 6060,
  Zo_GPScene4 = 6069,
  Zo_GPScene5 = 6078,
  Zo_GPScene6 = 6087,
  Zo_GPScene7 = 6096,
  Zo_GPScene8 = 6105,
  Zo_GPScene9 = 6114,
  Zo_GPShortPress1of1 = 6123,
  Zo_GPShortPress1of2 = 6140,
  Zo_GPShortPress2of2 = 6157,
  Zo_GPStepColor = 6174,
  Zo_GPStepDown = 6186,
  Zo_GPStepDownOnOff = 6197,
  Zo_GPStepHueDown = 6213,
  Zo_GPStepHueUp = 6227,
  Zo_GPStepSatDown = 6239,
  Zo_GPStepSatUp = 6253,
  Zo_GPStepUp = 6265,
  Zo_GPStepUpOnOff = 6274,
  Zo_GPToggle = 6288,
  Zo_GPUnlockDoor = 6297,
  Zo_GenericDeviceClass = 6310,
  Zo_GenericDeviceType = 6329,
  Zo_GetAllGroups = 6347,
  Zo_GetGroup = 6360,
  Zo_GetSceneMembership = 6369,
  Zo_GlassBreak = 6388,
  Zo_GroupNameSupport = 6399,
  Zo_HVACSystemTypeConfiguration = 6416,
  Zo_HWVersion = 6444,
  Zo_HarmonicCurrentMultiplier = 6454,
  Zo_HighTempDwellTripPoint = 6480,
  Zo_HighTempThreshold = 6503,
  Zo_Hue = 6521,
  Zo_HueMove = 6525,
  Zo_HueSat = 6533,
  Zo_HueStep = 6540,
  Zo_HueStepDown = 6548,
  Zo_HueStepUp = 6560,
  Zo_Humidity = 6570,
  Zo_HumidityMaxMeasuredValue = 6579,
  Zo_HumidityMinMeasuredValue = 6604,
  Zo_HumidityTolerance = 6629,
  Zo_IASCIEAddress = 6647,
  Zo_Identify = 6661,
  Zo_IdentifyQuery = 6670,
  Zo_IdentifyTime = 6684,
  Zo_Illuminance = 6697,
  Zo_IlluminanceLevelStatus = 6709,
  Zo_IlluminanceLightSensorType = 6732,
  Zo_IlluminanceMaxMeasuredValue = 6759,
  Zo_IlluminanceMinMeasuredValue = 6787,
  Zo_IlluminanceTargetLevel = 6815,
  Zo_IlluminanceTolerance = 6838,
  Zo_InletTemperature = 6859,
  Zo_InstalledClosedLimitLift = 6876,
  Zo_InstalledClosedLimitTilt = 6901,
  Zo_InstalledOpenLimitLift = 6926,
  Zo_InstalledOpenLimitTilt = 6949,
  Zo_IntermediateSetpointsLift = 6972,
  Zo_IntermediateSetpointsTilt = 6998,
  Zo_IntervalReadReportingPeriod = 7024,
  Zo_IntrinsicBallastFactor = 7052,
  Zo_LampAlarmMode = 7075,
  Zo_LampBurnHours = 7089,
  Zo_LampBurnHoursTripPoint = 7103,
  Zo_LampManufacturer = 7126,
  Zo_LampRatedHours = 7143,
  Zo_LampType = 7158,
  Zo_LastConfiguredBy = 7167,
  Zo_LastMessageLQI = 7184,
  Zo_LastMessageRSSI = 7199,
  Zo_LastSetTime = 7215,
  Zo_LegrandHeatingMode = 7227,
  Zo_LegrandMode = 7246,
  Zo_LegrandOpt1 = 7258,
  Zo_LegrandOpt2 = 7270,
  Zo_LegrandOpt3 = 7282,
  Zo_LidlPower = 7294,
  Zo_LineCurrent = 7304,
  Zo_LineCurrentPhB = 7316,
  Zo_LineCurrentPhC = 7331,
  Zo_LinkKey = 7346,
  Zo_LocalTemperature = 7354,
  Zo_LocalTemperatureCalibration = 7371,
  Zo_LocalTime = 7399,
  Zo_LocationAge = 7409,
  Zo_LocationDescription = 7421,
  Zo_LocationMethod = 7441,
  Zo_LocationPower = 7456,
  Zo_LocationType = 7470,
  Zo_LockAlarmMask = 7483,
  Zo_LockDefaultConfigurationRegister = 7497,
  Zo_LockEnableInsideStatusLED = 7530,
  Zo_LockEnableLocalProgramming = 7556,
  Zo_LockEnableLogging = 7583,
  Zo_LockEnableOneTouchLocking = 7601,
  Zo_LockEnablePrivacyModeButton = 7627,
  Zo_LockKeypadOperationEventMask = 7655,
  Zo_LockKeypadProgrammingEventMask = 7684,
  Zo_LockLEDSettings = 7715,
  Zo_LockLanguage = 7731,
  Zo_LockManualOperationEventMask = 7744,
  Zo_LockOperatingMode = 7773,
  Zo_LockRFIDOperationEventMask = 7791,
  Zo_LockRFIDProgrammingEventMask = 7818,
  Zo_LockRFOperationEventMask = 7847,
  Zo_LockRFProgrammingEventMask = 7872,
  Zo_LockSoundVolume = 7899,
  Zo_LockState = 7915,
  Zo_LockSupportedOperatingModes = 7925,
  Zo_LockType = 7953,
  Zo_LongPollInterval = 7962,
  Zo_LongPollIntervalMin = 7979,
  Zo_LowTempDwellTripPoint = 7999,
  Zo_LowTempThreshold = 8021,
  Zo_MainsAlarmMask = 8038,
  Zo_MainsFrequency = 8053,
  Zo_MainsVoltage = 8068,
  Zo_MainsVoltageDwellTripPoint = 8081,
  Zo_MainsVoltageMaxThreshold = 8108,
  Zo_MainsVoltageMinThreshold = 8133,
  Zo_Manufacturer = 8158,
  Zo_MaxCoolSetpointLimit = 8171,
  Zo_MaxHeatSetpointLimit = 8192,
  Zo_MaxPINCodeLength = 8213,
  Zo_MaxProxyTableEntries = 8230,
  Zo_MaxRFIDCodeLength = 8251,
  Zo_MaxSearchCounter = 8269,
  Zo_MaxSinkTableEntries = 8286,
  Zo_MaxTempExperienced = 8306,
  Zo_Measured11thHarmonicCurrent = 8325,
  Zo_Measured1stHarmonicCurrent = 8353,
  Zo_Measured3rdHarmonicCurrent = 8380,
  Zo_Measured5thHarmonicCurrent = 8407,
  Zo_Measured7thHarmonicCurrent = 8434,
  Zo_Measured9thHarmonicCurrent = 8461,
  Zo_MeasuredPhase11thHarmonicCurrent = 8488,
  Zo_MeasuredPhase1stHarmonicCurrent = 8521,
  Zo_MeasuredPhase3rdHarmonicCurrent = 8553,
  Zo_MeasuredPhase5thHarmonicCurrent = 8585,
  Zo_MeasuredPhase7thHarmonicCurrent = 8617,
  Zo_MeasuredPhase9thHarmonicCurrent = 8649,
  Zo_MeterTypeID = 8681,
  Zo_MinCoolSetpointLimit = 8693,
  Zo_MinHeatSetpointLimit = 8714,
  Zo_MinPINCodeLength = 8735,
  Zo_MinRFIDCodeLength = 8752,
  Zo_MinSetpointDeadBand = 8770,
  Zo_MinTempExperienced = 8790,
  Zo_Mode = 8809,
  Zo_Model = 8814,
  Zo_ModelId = 8820,
  Zo_MotorStepSize = 8828,
  Zo_Movement = 8842,
  Zo_MullerLightMode = 8851,
  Zo_MultiApplicationType = 8867,
  Zo_MultiDescription = 8888,
  Zo_MultiInApplicationType = 8905,
  Zo_MultiInDescription = 8928,
  Zo_MultiInNumberOfStates = 8947,
  Zo_MultiInOutOfService = 8969,
  Zo_MultiInReliability = 8989,
  Zo_MultiInStatusFlags = 9008,
  Zo_MultiInValue = 9027,
  Zo_MultiNumberOfStates = 9040,
  Zo_MultiOutApplicationType = 9060,
  Zo_MultiOutDescription = 9084,
  Zo_MultiOutNumberOfStates = 9104,
  Zo_MultiOutOfService = 9127,
  Zo_MultiOutOutOfService = 9145,
  Zo_MultiOutReliability = 9166,
  Zo_MultiOutRelinquishDefault = 9186,
  Zo_MultiOutStatusFlags = 9212,
  Zo_MultiOutValue = 9232,
  Zo_MultiReliability = 9246,
  Zo_MultiRelinquishDefault = 9263,
  Zo_MultiStatusFlags = 9286,
  Zo_MultiValue = 9303,
  Zo_MultipleScheduling = 9314,
  Zo_NeutralCurrent = 9333,
  Zo_NotificationRetryNumber = 9348,
  Zo_NotificationRetryTimer = 9372,
  Zo_NumberOfDevices = 9395,
  Zo_NumberOfHolidaySchedulesSupported = 9411,
  Zo_NumberOfLogRecordsSupported = 9445,
  Zo_NumberOfPINUsersSupported = 9473,
  Zo_NumberOfPrimaries = 9499,
  Zo_NumberOfRFIDUsersSupported = 9517,
  Zo_NumberOfResets = 9544,
  Zo_NumberOfTotalUsersSupported = 9559,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 9587,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 9628,
  Zo_NumberOfZoneSensitivityLevelsSupported = 9669,
  Zo_NumberRSSIMeasurements = 9708,
  Zo_NumberofActuationsLift = 9731,
  Zo_NumberofActuationsTilt = 9754,
  Zo_Occupancy = 9777,
  Zo_OccupancySensorType = 9787,
  Zo_OccupiedCoolingSetpoint = 9807,
  Zo_OccupiedHeatingSetpoint = 9831,
  Zo_OffTransitionTime = 9855,
  Zo_OffWaitTime = 9873,
  Zo_OnLevel = 9885,
  Zo_OnOff = 9893,
  Zo_OnOffTransitionTime = 9899,
  Zo_OnTime = 9919,
  Zo_OnTransitionTime = 9926,
  Zo_OpenPeriod = 9943,
  Zo_OppleMode = 9954,
  Zo_OutdoorTemperature = 9964,
  Zo_OutletTemperature = 9983,
  Zo_OverTempTotalDwell = 10001,
  Zo_PICoolingDemand = 10020,
  Zo_PIHeatingDemand = 10036,
  Zo_PIROccupiedToUnoccupiedDelay = 10052,
  Zo_PIRUnoccupiedToOccupiedDelay = 10081,
  Zo_PIRUnoccupiedToOccupiedThreshold = 10110,
  Zo_POD = 10143,
  Zo_Panic = 10147,
  Zo_PartNumber = 10153,
  Zo_PathLossExponent = 10164,
  Zo_PersistentMemoryWrites = 10181,
  Zo_PersonalAlarm = 10204,
  Zo_PhaseHarmonicCurrentMultiplier = 10218,
  Zo_PhysicalClosedLimit = 10249,
  Zo_PhysicalClosedLimitLift = 10269,
  Zo_PhysicalClosedLimitTilt = 10293,
  Zo_PhysicalEnvironment = 10317,
  Zo_Power = 10337,
  Zo_PowerDivisor = 10343,
  Zo_PowerFactor = 10356,
  Zo_PowerFactorPhB = 10368,
  Zo_PowerFactorPhC = 10383,
  Zo_PowerMultiplier = 10398,
  Zo_PowerOffEffect = 10414,
  Zo_PowerOnRecall = 10429,
  Zo_PowerOnTimer = 10443,
  Zo_PowerSource = 10456,
  Zo_PowerThreshold = 10468,
  Zo_PresetReadingTime = 10483,
  Zo_Pressure = 10501,
  Zo_PressureMaxMeasuredValue = 10510,
  Zo_PressureMaxScaledValue = 10535,
  Zo_PressureMinMeasuredValue = 10558,
  Zo_PressureMinScaledValue = 10583,
  Zo_PressureScale = 10606,
  Zo_PressureScaledTolerance = 10620,
  Zo_PressureScaledValue = 10644,
  Zo_PressureTolerance = 10664,
  Zo_PreviousBlockPeriodConsumptionDelivered = 10682,
  Zo_Primary1Intensity = 10722,
  Zo_Primary1X = 10740,
  Zo_Primary1Y = 10750,
  Zo_Primary2Intensity = 10760,
  Zo_Primary2X = 10778,
  Zo_Primary2Y = 10788,
  Zo_Primary3Intensity = 10798,
  Zo_Primary3X = 10816,
  Zo_Primary3Y = 10826,
  Zo_Primary4Intensity = 10836,
  Zo_Primary4X = 10854,
  Zo_Primary4Y = 10864,
  Zo_Primary5Intensity = 10874,
  Zo_Primary5X = 10892,
  Zo_Primary5Y = 10902,
  Zo_Primary6Intensity = 10912,
  Zo_Primary6X = 10930,
  Zo_Primary6Y = 10940,
  Zo_ProductCode = 10950,
  Zo_ProductRevision = 10962,
  Zo_ProductURL = 10978,
  Zo_ProfileIntervalPeriod = 10989,
  Zo_ProxyTable = 11011,
  Zo_QualityMeasure = 11022,
  Zo_RGB = 11037,
  Zo_RHDehumidificationSetpoint = 11041,
  Zo_RMSCurrent = 11068,
  Zo_RMSCurrentMax = 11079,
  Zo_RMSCurrentMaxPhB = 11093,
  Zo_RMSCurrentMaxPhC = 11110,
  Zo_RMSCurrentMin = 11127,
  Zo_RMSCurrentMinPhB = 11141,
  Zo_RMSCurrentMinPhC = 11158,
  Zo_RMSCurrentPhB = 11175,
  Zo_RMSCurrentPhC = 11189,
  Zo_RMSExtremeOverVoltage = 11203,
  Zo_RMSExtremeOverVoltagePeriod = 11225,
  Zo_RMSExtremeOverVoltagePeriodPhB = 11253,
  Zo_RMSExtremeOverVoltagePeriodPhC = 11284,
  Zo_RMSExtremeUnderVoltage = 11315,
  Zo_RMSExtremeUnderVoltagePeriod = 11338,
  Zo_RMSExtremeUnderVoltagePeriodPhB = 11367,
  Zo_RMSExtremeUnderVoltagePeriodPhC = 11399,
  Zo_RMSVoltage = 11431,
  Zo_RMSVoltageMax = 11442,
  Zo_RMSVoltageMaxPhB = 11456,
  Zo_RMSVoltageMaxPhC = 11473,
  Zo_RMSVoltageMin = 11490,
  Zo_RMSVoltageMinPhB = 11504,
  Zo_RMSVoltageMinPhC = 11521,
  Zo_RMSVoltagePhB = 11538,
  Zo_RMSVoltagePhC = 11552,
  Zo_RMSVoltageSag = 11566,
  Zo_RMSVoltageSagPeriod = 11580,
  Zo_RMSVoltageSagPeriodPhB = 11600,
  Zo_RMSVoltageSagPeriodPhC = 11623,
  Zo_RMSVoltageSwell = 11646,
  Zo_RMSVoltageSwellPeriod = 11662,
  Zo_RMSVoltageSwellPeriodPhB = 11684,
  Zo_RMSVoltageSwellPeriodPhC = 11709,
  Zo_ReactiveCurrent = 11734,
  Zo_ReactiveCurrentPhB = 11750,
  Zo_ReactiveCurrentPhC = 11769,
  Zo_ReactivePower = 11788,
  Zo_ReactivePowerPhB = 11802,
  Zo_ReactivePowerPhC = 11819,
  Zo_ReadingSnapShotTime = 11836,
  Zo_RecallScene = 11856,
  Zo_RelativeHumidity = 11868,
  Zo_RelativeHumidityDisplay = 11885,
  Zo_RelativeHumidityMode = 11909,
  Zo_RemainingTime = 11930,
  Zo_RemoteSensing = 11944,
  Zo_RemoveAllGroups = 11958,
  Zo_RemoveAllScenes = 11974,
  Zo_RemoveGroup = 11990,
  Zo_RemoveScene = 12002,
  Zo_ReportingPeriod = 12014,
  Zo_ResetAlarm = 12030,
  Zo_ResetAllAlarms = 12041,
  Zo_SWBuildID = 12056,
  Zo_Sat = 12066,
  Zo_SatMove = 12070,
  Zo_SatStep = 12078,
  Zo_SceneCount = 12086,
  Zo_SceneNameSupport = 12097,
  Zo_SceneValid = 12114,
  Zo_ScheduleMode = 12125,
  Zo_SeaPressure = 12138,
  Zo_SecurityLevel = 12150,
  Zo_ServerActiveFunctionality = 12164,
  Zo_ServerFunctionality = 12190,
  Zo_SharedSecurityKey = 12210,
  Zo_SharedSecurityKeyType = 12228,
  Zo_ShortPollInterval = 12250,
  Zo_Shutter = 12268,
  Zo_ShutterClose = 12276,
  Zo_ShutterLift = 12289,
  Zo_ShutterOpen = 12301,
  Zo_ShutterStop = 12313,
  Zo_ShutterTilt = 12325,
  Zo_SinkTable = 12337,
  Zo_SoftwareRevision = 12347,
  Zo_StackVersion = 12364,
  Zo_StandardTime = 12377,
  Zo_StartUpOnOff = 12390,
  Zo_Status = 12403,
  Zo_StoreScene = 12410,
  Zo_SupplyStatus = 12421,
  Zo_SwitchActions = 12434,
  Zo_SwitchType = 12448,
  Zo_SystemMode = 12459,
  Zo_TRVBoost = 12470,
  Zo_TRVChildProtection = 12479,
  Zo_TRVMirrorDisplay = 12498,
  Zo_TRVMode = 12515,
  Zo_TRVWindowOpen = 12523,
  Zo_TempTarget = 12537,
  Zo_Temperature = 12548,
  Zo_TemperatureDisplayMode = 12560,
  Zo_TemperatureMaxMeasuredValue = 12583,
  Zo_TemperatureMinMeasuredValue = 12611,
  Zo_TemperatureTolerance = 12639,
  Zo_TerncyDuration = 12660,
  Zo_TerncyRotate = 12675,
  Zo_ThSetpoint = 12688,
  Zo_ThermostatAlarmMask = 12699,
  Zo_ThermostatKeypadLockout = 12719,
  Zo_ThermostatOccupancy = 12743,
  Zo_ThermostatRunningMode = 12763,
  Zo_ThermostatScheduleProgrammingVisibility = 12785,
  Zo_Time = 12825,
  Zo_TimeEpoch = 12830,
  Zo_TimeStatus = 12840,
  Zo_TimeZone = 12851,
  Zo_TotalActivePower = 12860,
  Zo_TotalApparentPower = 12877,
  Zo_TotalProfileNum = 12896,
  Zo_TotalReactivePower = 12912,
  Zo_TuyaCalibration = 12931,
  Zo_TuyaCalibrationTime = 12947,
  Zo_TuyaMCUVersion = 12967,
  Zo_TuyaMotorReversal = 12982,
  Zo_TuyaMovingState = 13000,
  Zo_TuyaQuery = 13016,
  Zo_UnoccupiedCoolingSetpoint = 13026,
  Zo_UnoccupiedHeatingSetpoint = 13052,
  Zo_UtilityName = 13078,
  Zo_ValidUntilTime = 13090,
  Zo_ValvePosition = 13105,
  Zo_VelocityLift = 13119,
  Zo_ViewGroup = 13132,
  Zo_ViewScene = 13142,
  Zo_VolumePerReport = 13152,
  Zo_Water = 13168,
  Zo_WhitePointX = 13174,
  Zo_WhitePointY = 13186,
  Zo_WindowCoveringType = 13198,
  Zo_X = 13217,
  Zo_Y = 13219,
  Zo_ZCLVersion = 13221,
  Zo_ZoneID = 13232,
  Zo_ZoneState = 13239,
  Zo_ZoneStatus = 13249,
  Zo_ZoneStatusChange = 13260,
  Zo_ZoneType = 13277,
  Zo__ = 13286,
  Zo_xx = 13288,
  Zo_xx000A00 = 13291,
  Zo_xx0A = 13300,
  Zo_xx0A00 = 13305,
  Zo_xx19 = 13312,
  Zo_xx190A = 13317,
  Zo_xx190A00 = 13324,
  Zo_xxxx = 13333,
  Zo_xxxx00 = 13338,
  Zo_xxxx0A00 = 13345,
  Zo_xxxxyy = 13354,
  Zo_xxxxyyyy = 13361,
  Zo_xxxxyyyy0A00 = 13370,
  Zo_xxxxyyzz = 13383,
  Zo_xxyy = 13392,
  Zo_xxyy0A00 = 13397,
  Zo_xxyyyy = 13406,
  Zo_xxyyyy000000000000 = 13413,
  Zo_xxyyyy0A0000000000 = 13432,
  Zo_xxyyyyzz = 13451,
  Zo_xxyyyyzzzz = 13460,
  Zo_xxyyzzzz = 13471,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
