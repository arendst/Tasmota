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
  "CO2" "\x00"
  "CO2MaxMeasuredValue" "\x00"
  "CO2MinMeasuredValue" "\x00"
  "CO2Tolerance" "\x00"
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
  Zo_CO2 = 3182,
  Zo_CO2MaxMeasuredValue = 3186,
  Zo_CO2MinMeasuredValue = 3206,
  Zo_CO2Tolerance = 3226,
  Zo_CT = 3239,
  Zo_CalculationPeriod = 3242,
  Zo_CcommissioningExitMode = 3260,
  Zo_CheckinInterval = 3283,
  Zo_CheckinIntervalMin = 3299,
  Zo_ClientActiveFunctionality = 3318,
  Zo_ClientFunctionality = 3344,
  Zo_ClosedLimit = 3364,
  Zo_Color = 3376,
  Zo_ColorCapabilities = 3382,
  Zo_ColorLoopActive = 3400,
  Zo_ColorLoopDirection = 3416,
  Zo_ColorLoopStartEnhancedHue = 3435,
  Zo_ColorLoopStoredEnhancedHue = 3461,
  Zo_ColorLoopTime = 3488,
  Zo_ColorMode = 3502,
  Zo_ColorMove = 3512,
  Zo_ColorPointBIntensity = 3522,
  Zo_ColorPointBX = 3543,
  Zo_ColorPointBY = 3556,
  Zo_ColorPointGIntensity = 3569,
  Zo_ColorPointGX = 3590,
  Zo_ColorPointGY = 3603,
  Zo_ColorPointRIntensity = 3616,
  Zo_ColorPointRX = 3637,
  Zo_ColorPointRY = 3650,
  Zo_ColorStartUpColorTempireds = 3663,
  Zo_ColorStep = 3690,
  Zo_ColorTempMove = 3700,
  Zo_ColorTempMoveDown = 3714,
  Zo_ColorTempMoveStop = 3732,
  Zo_ColorTempMoveUp = 3750,
  Zo_ColorTempPhysicalMaxMireds = 3766,
  Zo_ColorTempPhysicalMinMireds = 3793,
  Zo_ColorTempStep = 3820,
  Zo_ColorTempStepDown = 3834,
  Zo_ColorTempStepUp = 3852,
  Zo_CommissioningWindow = 3868,
  Zo_CommunicationMode = 3888,
  Zo_CompanyName = 3906,
  Zo_CompensationText = 3918,
  Zo_ConfigStatus = 3935,
  Zo_Contact = 3948,
  Zo_ControlSequenceOfOperation = 3956,
  Zo_ControlTemperature = 3983,
  Zo_Coordinate1 = 4002,
  Zo_Coordinate2 = 4014,
  Zo_Coordinate3 = 4026,
  Zo_CurrentBlock = 4038,
  Zo_CurrentBlockPeriodConsumptionDelivered = 4051,
  Zo_CurrentGroup = 4090,
  Zo_CurrentInletEnergyCarrierDemand = 4103,
  Zo_CurrentInletEnergyCarrierSummation = 4135,
  Zo_CurrentMaxDemandDelivered = 4170,
  Zo_CurrentMaxDemandDeliveredTime = 4196,
  Zo_CurrentMaxDemandReceived = 4226,
  Zo_CurrentMaxDemandReceivedTime = 4251,
  Zo_CurrentOutletEnergyCarrierDemand = 4280,
  Zo_CurrentOutletEnergyCarrierSummation = 4313,
  Zo_CurrentPositionLift = 4349,
  Zo_CurrentPositionLiftPercentage = 4369,
  Zo_CurrentPositionTilt = 4399,
  Zo_CurrentPositionTiltPercentage = 4419,
  Zo_CurrentScene = 4449,
  Zo_CurrentSummationDelivered = 4462,
  Zo_CurrentSummationReceived = 4488,
  Zo_CurrentTemperature = 4513,
  Zo_CurrentTemperatureSetPoint = 4532,
  Zo_CurrentZoneSensitivityLevel = 4559,
  Zo_CustomerName = 4587,
  Zo_DCCurrent = 4600,
  Zo_DCCurrentDivisor = 4610,
  Zo_DCCurrentMax = 4627,
  Zo_DCCurrentMin = 4640,
  Zo_DCCurrentMultiplier = 4653,
  Zo_DCCurrentOverload = 4673,
  Zo_DCOverloadAlarmsMask = 4691,
  Zo_DCPower = 4712,
  Zo_DCPowerDivisor = 4720,
  Zo_DCPowerMax = 4735,
  Zo_DCPowerMin = 4746,
  Zo_DCPowerMultiplier = 4757,
  Zo_DCVoltage = 4775,
  Zo_DCVoltageDivisor = 4785,
  Zo_DCVoltageMax = 4802,
  Zo_DCVoltageMin = 4815,
  Zo_DCVoltageMultiplier = 4828,
  Zo_DCVoltageOverload = 4848,
  Zo_DFTSummation = 4866,
  Zo_DailyConsumptionTarget = 4879,
  Zo_DailyFreezeTime = 4902,
  Zo_DataQualityID = 4918,
  Zo_DateCode = 4932,
  Zo_DecelerationTimeLift = 4941,
  Zo_DefaultMoveRate = 4962,
  Zo_DefaultUpdatePeriod = 4978,
  Zo_DehumidificationCooling = 4998,
  Zo_DehumidificationHysteresis = 5022,
  Zo_DehumidificationLockout = 5049,
  Zo_DehumidificationMaxCool = 5073,
  Zo_DeviceEnabled = 5097,
  Zo_DeviceTempAlarmMask = 5111,
  Zo_Dimmer = 5131,
  Zo_DimmerCurrentFrequency = 5138,
  Zo_DimmerDown = 5161,
  Zo_DimmerMaxFrequency = 5172,
  Zo_DimmerMaxLevel = 5191,
  Zo_DimmerMinFrequency = 5206,
  Zo_DimmerMinLevel = 5225,
  Zo_DimmerMove = 5240,
  Zo_DimmerOptions = 5251,
  Zo_DimmerRemainingTime = 5265,
  Zo_DimmerStartUpLevel = 5285,
  Zo_DimmerStep = 5304,
  Zo_DimmerStepDown = 5315,
  Zo_DimmerStepUp = 5330,
  Zo_DimmerStop = 5343,
  Zo_DimmerUp = 5354,
  Zo_DisableLocalConfig = 5363,
  Zo_DoorClosedEvents = 5382,
  Zo_DoorOpenEvents = 5399,
  Zo_DoorState = 5414,
  Zo_DriftCompensation = 5424,
  Zo_DstEnd = 5442,
  Zo_DstShift = 5449,
  Zo_DstStart = 5458,
  Zo_ElectricalMeasurementType = 5467,
  Zo_EnergyFormatting = 5493,
  Zo_EnergyRemote = 5510,
  Zo_EnhancedColorMode = 5523,
  Zo_EnhancedCurrentHue = 5541,
  Zo_EurotronicErrors = 5560,
  Zo_EurotronicHostFlags = 5577,
  Zo_FanMode = 5597,
  Zo_FanModeSequence = 5605,
  Zo_FastPollTimeout = 5621,
  Zo_FastPollTimeoutMax = 5637,
  Zo_FastPollUpdatePeriod = 5656,
  Zo_Fire = 5677,
  Zo_FlowMaxMeasuredValue = 5682,
  Zo_FlowMinMeasuredValue = 5703,
  Zo_FlowRate = 5724,
  Zo_FlowRestriction = 5733,
  Zo_FlowTolerance = 5749,
  Zo_GPHueStop = 5763,
  Zo_GPIdentify = 5773,
  Zo_GPLevelStop = 5784,
  Zo_GPLockDoor = 5796,
  Zo_GPMoveColor = 5807,
  Zo_GPMoveDown = 5819,
  Zo_GPMoveDownOnOff = 5830,
  Zo_GPMoveHueDown = 5846,
  Zo_GPMoveHueUp = 5860,
  Zo_GPMoveSatDown = 5872,
  Zo_GPMoveSatUp = 5886,
  Zo_GPMoveUp = 5898,
  Zo_GPMoveUpOnOff = 5907,
  Zo_GPOff = 5921,
  Zo_GPOn = 5927,
  Zo_GPPress1of1 = 5932,
  Zo_GPPress1of2 = 5944,
  Zo_GPPress2of2 = 5956,
  Zo_GPRelease = 5968,
  Zo_GPRelease1of1 = 5978,
  Zo_GPRelease1of2 = 5992,
  Zo_GPRelease2of2 = 6006,
  Zo_GPSatStop = 6020,
  Zo_GPScene0 = 6030,
  Zo_GPScene1 = 6039,
  Zo_GPScene10 = 6048,
  Zo_GPScene11 = 6058,
  Zo_GPScene12 = 6068,
  Zo_GPScene13 = 6078,
  Zo_GPScene14 = 6088,
  Zo_GPScene15 = 6098,
  Zo_GPScene2 = 6108,
  Zo_GPScene3 = 6117,
  Zo_GPScene4 = 6126,
  Zo_GPScene5 = 6135,
  Zo_GPScene6 = 6144,
  Zo_GPScene7 = 6153,
  Zo_GPScene8 = 6162,
  Zo_GPScene9 = 6171,
  Zo_GPShortPress1of1 = 6180,
  Zo_GPShortPress1of2 = 6197,
  Zo_GPShortPress2of2 = 6214,
  Zo_GPStepColor = 6231,
  Zo_GPStepDown = 6243,
  Zo_GPStepDownOnOff = 6254,
  Zo_GPStepHueDown = 6270,
  Zo_GPStepHueUp = 6284,
  Zo_GPStepSatDown = 6296,
  Zo_GPStepSatUp = 6310,
  Zo_GPStepUp = 6322,
  Zo_GPStepUpOnOff = 6331,
  Zo_GPToggle = 6345,
  Zo_GPUnlockDoor = 6354,
  Zo_GenericDeviceClass = 6367,
  Zo_GenericDeviceType = 6386,
  Zo_GetAllGroups = 6404,
  Zo_GetGroup = 6417,
  Zo_GetSceneMembership = 6426,
  Zo_GlassBreak = 6445,
  Zo_GroupNameSupport = 6456,
  Zo_HVACSystemTypeConfiguration = 6473,
  Zo_HWVersion = 6501,
  Zo_HarmonicCurrentMultiplier = 6511,
  Zo_HighTempDwellTripPoint = 6537,
  Zo_HighTempThreshold = 6560,
  Zo_Hue = 6578,
  Zo_HueMove = 6582,
  Zo_HueSat = 6590,
  Zo_HueStep = 6597,
  Zo_HueStepDown = 6605,
  Zo_HueStepUp = 6617,
  Zo_Humidity = 6627,
  Zo_HumidityMaxMeasuredValue = 6636,
  Zo_HumidityMinMeasuredValue = 6661,
  Zo_HumidityTolerance = 6686,
  Zo_IASCIEAddress = 6704,
  Zo_Identify = 6718,
  Zo_IdentifyQuery = 6727,
  Zo_IdentifyTime = 6741,
  Zo_Illuminance = 6754,
  Zo_IlluminanceLevelStatus = 6766,
  Zo_IlluminanceLightSensorType = 6789,
  Zo_IlluminanceMaxMeasuredValue = 6816,
  Zo_IlluminanceMinMeasuredValue = 6844,
  Zo_IlluminanceTargetLevel = 6872,
  Zo_IlluminanceTolerance = 6895,
  Zo_InletTemperature = 6916,
  Zo_InstalledClosedLimitLift = 6933,
  Zo_InstalledClosedLimitTilt = 6958,
  Zo_InstalledOpenLimitLift = 6983,
  Zo_InstalledOpenLimitTilt = 7006,
  Zo_IntermediateSetpointsLift = 7029,
  Zo_IntermediateSetpointsTilt = 7055,
  Zo_IntervalReadReportingPeriod = 7081,
  Zo_IntrinsicBallastFactor = 7109,
  Zo_LampAlarmMode = 7132,
  Zo_LampBurnHours = 7146,
  Zo_LampBurnHoursTripPoint = 7160,
  Zo_LampManufacturer = 7183,
  Zo_LampRatedHours = 7200,
  Zo_LampType = 7215,
  Zo_LastConfiguredBy = 7224,
  Zo_LastMessageLQI = 7241,
  Zo_LastMessageRSSI = 7256,
  Zo_LastSetTime = 7272,
  Zo_LegrandHeatingMode = 7284,
  Zo_LegrandMode = 7303,
  Zo_LegrandOpt1 = 7315,
  Zo_LegrandOpt2 = 7327,
  Zo_LegrandOpt3 = 7339,
  Zo_LidlPower = 7351,
  Zo_LineCurrent = 7361,
  Zo_LineCurrentPhB = 7373,
  Zo_LineCurrentPhC = 7388,
  Zo_LinkKey = 7403,
  Zo_LocalTemperature = 7411,
  Zo_LocalTemperatureCalibration = 7428,
  Zo_LocalTime = 7456,
  Zo_LocationAge = 7466,
  Zo_LocationDescription = 7478,
  Zo_LocationMethod = 7498,
  Zo_LocationPower = 7513,
  Zo_LocationType = 7527,
  Zo_LockAlarmMask = 7540,
  Zo_LockDefaultConfigurationRegister = 7554,
  Zo_LockEnableInsideStatusLED = 7587,
  Zo_LockEnableLocalProgramming = 7613,
  Zo_LockEnableLogging = 7640,
  Zo_LockEnableOneTouchLocking = 7658,
  Zo_LockEnablePrivacyModeButton = 7684,
  Zo_LockKeypadOperationEventMask = 7712,
  Zo_LockKeypadProgrammingEventMask = 7741,
  Zo_LockLEDSettings = 7772,
  Zo_LockLanguage = 7788,
  Zo_LockManualOperationEventMask = 7801,
  Zo_LockOperatingMode = 7830,
  Zo_LockRFIDOperationEventMask = 7848,
  Zo_LockRFIDProgrammingEventMask = 7875,
  Zo_LockRFOperationEventMask = 7904,
  Zo_LockRFProgrammingEventMask = 7929,
  Zo_LockSoundVolume = 7956,
  Zo_LockState = 7972,
  Zo_LockSupportedOperatingModes = 7982,
  Zo_LockType = 8010,
  Zo_LongPollInterval = 8019,
  Zo_LongPollIntervalMin = 8036,
  Zo_LowTempDwellTripPoint = 8056,
  Zo_LowTempThreshold = 8078,
  Zo_MainsAlarmMask = 8095,
  Zo_MainsFrequency = 8110,
  Zo_MainsVoltage = 8125,
  Zo_MainsVoltageDwellTripPoint = 8138,
  Zo_MainsVoltageMaxThreshold = 8165,
  Zo_MainsVoltageMinThreshold = 8190,
  Zo_Manufacturer = 8215,
  Zo_MaxCoolSetpointLimit = 8228,
  Zo_MaxHeatSetpointLimit = 8249,
  Zo_MaxPINCodeLength = 8270,
  Zo_MaxProxyTableEntries = 8287,
  Zo_MaxRFIDCodeLength = 8308,
  Zo_MaxSearchCounter = 8326,
  Zo_MaxSinkTableEntries = 8343,
  Zo_MaxTempExperienced = 8363,
  Zo_Measured11thHarmonicCurrent = 8382,
  Zo_Measured1stHarmonicCurrent = 8410,
  Zo_Measured3rdHarmonicCurrent = 8437,
  Zo_Measured5thHarmonicCurrent = 8464,
  Zo_Measured7thHarmonicCurrent = 8491,
  Zo_Measured9thHarmonicCurrent = 8518,
  Zo_MeasuredPhase11thHarmonicCurrent = 8545,
  Zo_MeasuredPhase1stHarmonicCurrent = 8578,
  Zo_MeasuredPhase3rdHarmonicCurrent = 8610,
  Zo_MeasuredPhase5thHarmonicCurrent = 8642,
  Zo_MeasuredPhase7thHarmonicCurrent = 8674,
  Zo_MeasuredPhase9thHarmonicCurrent = 8706,
  Zo_MeterTypeID = 8738,
  Zo_MinCoolSetpointLimit = 8750,
  Zo_MinHeatSetpointLimit = 8771,
  Zo_MinPINCodeLength = 8792,
  Zo_MinRFIDCodeLength = 8809,
  Zo_MinSetpointDeadBand = 8827,
  Zo_MinTempExperienced = 8847,
  Zo_Mode = 8866,
  Zo_Model = 8871,
  Zo_ModelId = 8877,
  Zo_MotorStepSize = 8885,
  Zo_Movement = 8899,
  Zo_MullerLightMode = 8908,
  Zo_MultiApplicationType = 8924,
  Zo_MultiDescription = 8945,
  Zo_MultiInApplicationType = 8962,
  Zo_MultiInDescription = 8985,
  Zo_MultiInNumberOfStates = 9004,
  Zo_MultiInOutOfService = 9026,
  Zo_MultiInReliability = 9046,
  Zo_MultiInStatusFlags = 9065,
  Zo_MultiInValue = 9084,
  Zo_MultiNumberOfStates = 9097,
  Zo_MultiOutApplicationType = 9117,
  Zo_MultiOutDescription = 9141,
  Zo_MultiOutNumberOfStates = 9161,
  Zo_MultiOutOfService = 9184,
  Zo_MultiOutOutOfService = 9202,
  Zo_MultiOutReliability = 9223,
  Zo_MultiOutRelinquishDefault = 9243,
  Zo_MultiOutStatusFlags = 9269,
  Zo_MultiOutValue = 9289,
  Zo_MultiReliability = 9303,
  Zo_MultiRelinquishDefault = 9320,
  Zo_MultiStatusFlags = 9343,
  Zo_MultiValue = 9360,
  Zo_MultipleScheduling = 9371,
  Zo_NeutralCurrent = 9390,
  Zo_NotificationRetryNumber = 9405,
  Zo_NotificationRetryTimer = 9429,
  Zo_NumberOfDevices = 9452,
  Zo_NumberOfHolidaySchedulesSupported = 9468,
  Zo_NumberOfLogRecordsSupported = 9502,
  Zo_NumberOfPINUsersSupported = 9530,
  Zo_NumberOfPrimaries = 9556,
  Zo_NumberOfRFIDUsersSupported = 9574,
  Zo_NumberOfResets = 9601,
  Zo_NumberOfTotalUsersSupported = 9616,
  Zo_NumberOfWeekDaySchedulesSupportedPerUser = 9644,
  Zo_NumberOfYearDaySchedulesSupportedPerUser = 9685,
  Zo_NumberOfZoneSensitivityLevelsSupported = 9726,
  Zo_NumberRSSIMeasurements = 9765,
  Zo_NumberofActuationsLift = 9788,
  Zo_NumberofActuationsTilt = 9811,
  Zo_Occupancy = 9834,
  Zo_OccupancySensorType = 9844,
  Zo_OccupiedCoolingSetpoint = 9864,
  Zo_OccupiedHeatingSetpoint = 9888,
  Zo_OffTransitionTime = 9912,
  Zo_OffWaitTime = 9930,
  Zo_OnLevel = 9942,
  Zo_OnOff = 9950,
  Zo_OnOffTransitionTime = 9956,
  Zo_OnTime = 9976,
  Zo_OnTransitionTime = 9983,
  Zo_OpenPeriod = 10000,
  Zo_OppleMode = 10011,
  Zo_OutdoorTemperature = 10021,
  Zo_OutletTemperature = 10040,
  Zo_OverTempTotalDwell = 10058,
  Zo_PICoolingDemand = 10077,
  Zo_PIHeatingDemand = 10093,
  Zo_PIROccupiedToUnoccupiedDelay = 10109,
  Zo_PIRUnoccupiedToOccupiedDelay = 10138,
  Zo_PIRUnoccupiedToOccupiedThreshold = 10167,
  Zo_POD = 10200,
  Zo_Panic = 10204,
  Zo_PartNumber = 10210,
  Zo_PathLossExponent = 10221,
  Zo_PersistentMemoryWrites = 10238,
  Zo_PersonalAlarm = 10261,
  Zo_PhaseHarmonicCurrentMultiplier = 10275,
  Zo_PhysicalClosedLimit = 10306,
  Zo_PhysicalClosedLimitLift = 10326,
  Zo_PhysicalClosedLimitTilt = 10350,
  Zo_PhysicalEnvironment = 10374,
  Zo_Power = 10394,
  Zo_PowerDivisor = 10400,
  Zo_PowerFactor = 10413,
  Zo_PowerFactorPhB = 10425,
  Zo_PowerFactorPhC = 10440,
  Zo_PowerMultiplier = 10455,
  Zo_PowerOffEffect = 10471,
  Zo_PowerOnRecall = 10486,
  Zo_PowerOnTimer = 10500,
  Zo_PowerSource = 10513,
  Zo_PowerThreshold = 10525,
  Zo_PresetReadingTime = 10540,
  Zo_Pressure = 10558,
  Zo_PressureMaxMeasuredValue = 10567,
  Zo_PressureMaxScaledValue = 10592,
  Zo_PressureMinMeasuredValue = 10615,
  Zo_PressureMinScaledValue = 10640,
  Zo_PressureScale = 10663,
  Zo_PressureScaledTolerance = 10677,
  Zo_PressureScaledValue = 10701,
  Zo_PressureTolerance = 10721,
  Zo_PreviousBlockPeriodConsumptionDelivered = 10739,
  Zo_Primary1Intensity = 10779,
  Zo_Primary1X = 10797,
  Zo_Primary1Y = 10807,
  Zo_Primary2Intensity = 10817,
  Zo_Primary2X = 10835,
  Zo_Primary2Y = 10845,
  Zo_Primary3Intensity = 10855,
  Zo_Primary3X = 10873,
  Zo_Primary3Y = 10883,
  Zo_Primary4Intensity = 10893,
  Zo_Primary4X = 10911,
  Zo_Primary4Y = 10921,
  Zo_Primary5Intensity = 10931,
  Zo_Primary5X = 10949,
  Zo_Primary5Y = 10959,
  Zo_Primary6Intensity = 10969,
  Zo_Primary6X = 10987,
  Zo_Primary6Y = 10997,
  Zo_ProductCode = 11007,
  Zo_ProductRevision = 11019,
  Zo_ProductURL = 11035,
  Zo_ProfileIntervalPeriod = 11046,
  Zo_ProxyTable = 11068,
  Zo_QualityMeasure = 11079,
  Zo_RGB = 11094,
  Zo_RHDehumidificationSetpoint = 11098,
  Zo_RMSCurrent = 11125,
  Zo_RMSCurrentMax = 11136,
  Zo_RMSCurrentMaxPhB = 11150,
  Zo_RMSCurrentMaxPhC = 11167,
  Zo_RMSCurrentMin = 11184,
  Zo_RMSCurrentMinPhB = 11198,
  Zo_RMSCurrentMinPhC = 11215,
  Zo_RMSCurrentPhB = 11232,
  Zo_RMSCurrentPhC = 11246,
  Zo_RMSExtremeOverVoltage = 11260,
  Zo_RMSExtremeOverVoltagePeriod = 11282,
  Zo_RMSExtremeOverVoltagePeriodPhB = 11310,
  Zo_RMSExtremeOverVoltagePeriodPhC = 11341,
  Zo_RMSExtremeUnderVoltage = 11372,
  Zo_RMSExtremeUnderVoltagePeriod = 11395,
  Zo_RMSExtremeUnderVoltagePeriodPhB = 11424,
  Zo_RMSExtremeUnderVoltagePeriodPhC = 11456,
  Zo_RMSVoltage = 11488,
  Zo_RMSVoltageMax = 11499,
  Zo_RMSVoltageMaxPhB = 11513,
  Zo_RMSVoltageMaxPhC = 11530,
  Zo_RMSVoltageMin = 11547,
  Zo_RMSVoltageMinPhB = 11561,
  Zo_RMSVoltageMinPhC = 11578,
  Zo_RMSVoltagePhB = 11595,
  Zo_RMSVoltagePhC = 11609,
  Zo_RMSVoltageSag = 11623,
  Zo_RMSVoltageSagPeriod = 11637,
  Zo_RMSVoltageSagPeriodPhB = 11657,
  Zo_RMSVoltageSagPeriodPhC = 11680,
  Zo_RMSVoltageSwell = 11703,
  Zo_RMSVoltageSwellPeriod = 11719,
  Zo_RMSVoltageSwellPeriodPhB = 11741,
  Zo_RMSVoltageSwellPeriodPhC = 11766,
  Zo_ReactiveCurrent = 11791,
  Zo_ReactiveCurrentPhB = 11807,
  Zo_ReactiveCurrentPhC = 11826,
  Zo_ReactivePower = 11845,
  Zo_ReactivePowerPhB = 11859,
  Zo_ReactivePowerPhC = 11876,
  Zo_ReadingSnapShotTime = 11893,
  Zo_RecallScene = 11913,
  Zo_RelativeHumidity = 11925,
  Zo_RelativeHumidityDisplay = 11942,
  Zo_RelativeHumidityMode = 11966,
  Zo_RemainingTime = 11987,
  Zo_RemoteSensing = 12001,
  Zo_RemoveAllGroups = 12015,
  Zo_RemoveAllScenes = 12031,
  Zo_RemoveGroup = 12047,
  Zo_RemoveScene = 12059,
  Zo_ReportingPeriod = 12071,
  Zo_ResetAlarm = 12087,
  Zo_ResetAllAlarms = 12098,
  Zo_SWBuildID = 12113,
  Zo_Sat = 12123,
  Zo_SatMove = 12127,
  Zo_SatStep = 12135,
  Zo_SceneCount = 12143,
  Zo_SceneNameSupport = 12154,
  Zo_SceneValid = 12171,
  Zo_ScheduleMode = 12182,
  Zo_SeaPressure = 12195,
  Zo_SecurityLevel = 12207,
  Zo_ServerActiveFunctionality = 12221,
  Zo_ServerFunctionality = 12247,
  Zo_SharedSecurityKey = 12267,
  Zo_SharedSecurityKeyType = 12285,
  Zo_ShortPollInterval = 12307,
  Zo_Shutter = 12325,
  Zo_ShutterClose = 12333,
  Zo_ShutterLift = 12346,
  Zo_ShutterOpen = 12358,
  Zo_ShutterStop = 12370,
  Zo_ShutterTilt = 12382,
  Zo_SinkTable = 12394,
  Zo_SoftwareRevision = 12404,
  Zo_StackVersion = 12421,
  Zo_StandardTime = 12434,
  Zo_StartUpOnOff = 12447,
  Zo_Status = 12460,
  Zo_StoreScene = 12467,
  Zo_SupplyStatus = 12478,
  Zo_SwitchActions = 12491,
  Zo_SwitchType = 12505,
  Zo_SystemMode = 12516,
  Zo_TRVBoost = 12527,
  Zo_TRVChildProtection = 12536,
  Zo_TRVMirrorDisplay = 12555,
  Zo_TRVMode = 12572,
  Zo_TRVWindowOpen = 12580,
  Zo_TempTarget = 12594,
  Zo_Temperature = 12605,
  Zo_TemperatureDisplayMode = 12617,
  Zo_TemperatureMaxMeasuredValue = 12640,
  Zo_TemperatureMinMeasuredValue = 12668,
  Zo_TemperatureTolerance = 12696,
  Zo_TerncyDuration = 12717,
  Zo_TerncyRotate = 12732,
  Zo_ThSetpoint = 12745,
  Zo_ThermostatAlarmMask = 12756,
  Zo_ThermostatKeypadLockout = 12776,
  Zo_ThermostatOccupancy = 12800,
  Zo_ThermostatRunningMode = 12820,
  Zo_ThermostatScheduleProgrammingVisibility = 12842,
  Zo_Time = 12882,
  Zo_TimeEpoch = 12887,
  Zo_TimeStatus = 12897,
  Zo_TimeZone = 12908,
  Zo_TotalActivePower = 12917,
  Zo_TotalApparentPower = 12934,
  Zo_TotalProfileNum = 12953,
  Zo_TotalReactivePower = 12969,
  Zo_TuyaCalibration = 12988,
  Zo_TuyaCalibrationTime = 13004,
  Zo_TuyaMCUVersion = 13024,
  Zo_TuyaMotorReversal = 13039,
  Zo_TuyaMovingState = 13057,
  Zo_TuyaQuery = 13073,
  Zo_UnoccupiedCoolingSetpoint = 13083,
  Zo_UnoccupiedHeatingSetpoint = 13109,
  Zo_UtilityName = 13135,
  Zo_ValidUntilTime = 13147,
  Zo_ValvePosition = 13162,
  Zo_VelocityLift = 13176,
  Zo_ViewGroup = 13189,
  Zo_ViewScene = 13199,
  Zo_VolumePerReport = 13209,
  Zo_Water = 13225,
  Zo_WhitePointX = 13231,
  Zo_WhitePointY = 13243,
  Zo_WindowCoveringType = 13255,
  Zo_X = 13274,
  Zo_Y = 13276,
  Zo_ZCLVersion = 13278,
  Zo_ZoneID = 13289,
  Zo_ZoneState = 13296,
  Zo_ZoneStatus = 13306,
  Zo_ZoneStatusChange = 13317,
  Zo_ZoneType = 13334,
  Zo__ = 13343,
  Zo_xx = 13345,
  Zo_xx000A00 = 13348,
  Zo_xx0A = 13357,
  Zo_xx0A00 = 13362,
  Zo_xx19 = 13369,
  Zo_xx190A = 13374,
  Zo_xx190A00 = 13381,
  Zo_xxxx = 13390,
  Zo_xxxx00 = 13395,
  Zo_xxxx0A00 = 13402,
  Zo_xxxxyy = 13411,
  Zo_xxxxyyyy = 13418,
  Zo_xxxxyyyy0A00 = 13427,
  Zo_xxxxyyzz = 13440,
  Zo_xxyy = 13449,
  Zo_xxyy0A00 = 13454,
  Zo_xxyyyy = 13463,
  Zo_xxyyyy000000000000 = 13470,
  Zo_xxyyyy0A0000000000 = 13489,
  Zo_xxyyyyzz = 13508,
  Zo_xxyyyyzzzz = 13517,
  Zo_xxyyzzzz = 13528,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
