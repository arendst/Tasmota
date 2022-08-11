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
  "AccelerationTimeLift" "\x00"
  "ActivePower" "\x00"
  "ActuatorEnabled" "\x00"
  "AddGroup" "\x00"
  "AddScene" "\x00"
  "AlarmCount" "\x00"
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
  "AvailablePower" "\x00"
  "BatteryPercentage" "\x00"
  "BatteryVoltage" "\x00"
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
  "CheckinInterval" "\x00"
  "CheckinIntervalMin" "\x00"
  "ClosedLimit" "\x00"
  "Color" "\x00"
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
  "ColorTempStep" "\x00"
  "ColorTempStepDown" "\x00"
  "ColorTempStepUp" "\x00"
  "CompanyName" "\x00"
  "CompensationText" "\x00"
  "ConfigStatus" "\x00"
  "Contact" "\x00"
  "ControlSequenceOfOperation" "\x00"
  "CurrentGroup" "\x00"
  "CurrentPositionLift" "\x00"
  "CurrentPositionLiftPercentage" "\x00"
  "CurrentPositionTilt" "\x00"
  "CurrentPositionTiltPercentage" "\x00"
  "CurrentScene" "\x00"
  "CurrentTemperature" "\x00"
  "CurrentTemperatureSetPoint" "\x00"
  "CustomerName" "\x00"
  "DataQualityID" "\x00"
  "DateCode" "\x00"
  "DecelerationTimeLift" "\x00"
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
  "DoorClosedEvents" "\x00"
  "DoorOpenEvents" "\x00"
  "DoorState" "\x00"
  "DriftCompensation" "\x00"
  "DstEnd" "\x00"
  "DstShift" "\x00"
  "DstStart" "\x00"
  "EnergyFormatting" "\x00"
  "EnergyRemote" "\x00"
  "EnergyTotal" "\x00"
  "EurotronicErrors" "\x00"
  "EurotronicHostFlags" "\x00"
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
  "HWVersion" "\x00"
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
  "LastMessageLQI" "\x00"
  "LastMessageRSSI" "\x00"
  "LastSetTime" "\x00"
  "LegrandHeatingMode" "\x00"
  "LegrandMode" "\x00"
  "LegrandOpt1" "\x00"
  "LegrandOpt2" "\x00"
  "LegrandOpt3" "\x00"
  "LidlPower" "\x00"
  "LocalTemperature" "\x00"
  "LocalTemperatureCalibration" "\x00"
  "LocalTime" "\x00"
  "LocationAge" "\x00"
  "LocationMethod" "\x00"
  "LocationType" "\x00"
  "LockState" "\x00"
  "LockType" "\x00"
  "LongPollInterval" "\x00"
  "LongPollIntervalMin" "\x00"
  "MainsFrequency" "\x00"
  "MainsVoltage" "\x00"
  "Manufacturer" "\x00"
  "MaxTempExperienced" "\x00"
  "MeterTypeID" "\x00"
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
  "NumberOfDevices" "\x00"
  "NumberOfPrimaries" "\x00"
  "NumberOfResets" "\x00"
  "NumberofActuationsLift" "\x00"
  "NumberofActuationsTilt" "\x00"
  "Occupancy" "\x00"
  "OccupancySensorType" "\x00"
  "OccupiedCoolingSetpoint" "\x00"
  "OccupiedHeatingSetpoint" "\x00"
  "OnOffTransitionTime" "\x00"
  "OpenPeriod" "\x00"
  "OppleMode" "\x00"
  "OutdoorTemperature" "\x00"
  "OverTempTotalDwell" "\x00"
  "PICoolingDemand" "\x00"
  "PIHeatingDemand" "\x00"
  "POD" "\x00"
  "Panic" "\x00"
  "PartNumber" "\x00"
  "PersistentMemoryWrites" "\x00"
  "PersonalAlarm" "\x00"
  "PhysicalClosedLimit" "\x00"
  "PhysicalClosedLimitLift" "\x00"
  "PhysicalClosedLimitTilt" "\x00"
  "Power" "\x00"
  "Power2" "\x00"
  "Power3" "\x00"
  "Power4" "\x00"
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
  "ProductCode" "\x00"
  "ProductRevision" "\x00"
  "ProductURL" "\x00"
  "QualityMeasure" "\x00"
  "RGB" "\x00"
  "RMSCurrent" "\x00"
  "RMSVoltage" "\x00"
  "ReactivePower" "\x00"
  "RecallScene" "\x00"
  "RemainingTime" "\x00"
  "RemoteSensing" "\x00"
  "RemoveAllGroups" "\x00"
  "RemoveAllScenes" "\x00"
  "RemoveGroup" "\x00"
  "RemoveScene" "\x00"
  "ResetAlarm" "\x00"
  "ResetAllAlarms" "\x00"
  "SWBuildID" "\x00"
  "Sat" "\x00"
  "SatMove" "\x00"
  "SatStep" "\x00"
  "SceneCount" "\x00"
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
  "SwitchType" "\x00"
  "SystemMode" "\x00"
  "TRVBoost" "\x00"
  "TRVChildProtection" "\x00"
  "TRVMirrorDisplay" "\x00"
  "TRVMode" "\x00"
  "TRVWindowOpen" "\x00"
  "TempTarget" "\x00"
  "Temperature" "\x00"
  "TemperatureMaxMeasuredValue" "\x00"
  "TemperatureMinMeasuredValue" "\x00"
  "TemperatureTolerance" "\x00"
  "TerncyDuration" "\x00"
  "TerncyRotate" "\x00"
  "ThSetpoint" "\x00"
  "Time" "\x00"
  "TimeEpoch" "\x00"
  "TimeStatus" "\x00"
  "TimeZone" "\x00"
  "TotalProfileNum" "\x00"
  "TuyaAutoLock" "\x00"
  "TuyaAwayDays" "\x00"
  "TuyaAwayTemp" "\x00"
  "TuyaBattery" "\x00"
  "TuyaBoostTime" "\x00"
  "TuyaCalibration" "\x00"
  "TuyaCalibrationTime" "\x00"
  "TuyaChildLock" "\x00"
  "TuyaComfortTemp" "\x00"
  "TuyaEcoTemp" "\x00"
  "TuyaFanMode" "\x00"
  "TuyaForceMode" "\x00"
  "TuyaMCUVersion" "\x00"
  "TuyaMaxTemp" "\x00"
  "TuyaMinTemp" "\x00"
  "TuyaMotorReversal" "\x00"
  "TuyaMovingState" "\x00"
  "TuyaPreset" "\x00"
  "TuyaQuery" "\x00"
  "TuyaScheduleHolidays" "\x00"
  "TuyaScheduleWorkdays" "\x00"
  "TuyaTempTarget" "\x00"
  "TuyaValveDetection" "\x00"
  "TuyaValvePosition" "\x00"
  "TuyaWeekSelect" "\x00"
  "TuyaWindowDetection" "\x00"
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
  Zo_AccelerationTimeLift = 159,
  Zo_ActivePower = 180,
  Zo_ActuatorEnabled = 192,
  Zo_AddGroup = 208,
  Zo_AddScene = 217,
  Zo_AlarmCount = 226,
  Zo_AnalogApplicationType = 237,
  Zo_AnalogDescription = 259,
  Zo_AnalogEngineeringUnits = 277,
  Zo_AnalogInApplicationType = 300,
  Zo_AnalogInDescription = 324,
  Zo_AnalogInEngineeringUnits = 344,
  Zo_AnalogInMaxValue = 369,
  Zo_AnalogInMinValue = 386,
  Zo_AnalogInOutOfService = 403,
  Zo_AnalogInReliability = 424,
  Zo_AnalogInResolution = 444,
  Zo_AnalogInStatusFlags = 463,
  Zo_AnalogOutApplicationType = 483,
  Zo_AnalogOutDescription = 508,
  Zo_AnalogOutEngineeringUnits = 529,
  Zo_AnalogOutMaxValue = 555,
  Zo_AnalogOutMinValue = 573,
  Zo_AnalogOutOfService = 591,
  Zo_AnalogOutOutOfService = 610,
  Zo_AnalogOutReliability = 632,
  Zo_AnalogOutRelinquishDefault = 653,
  Zo_AnalogOutResolution = 680,
  Zo_AnalogOutStatusFlags = 700,
  Zo_AnalogOutValue = 721,
  Zo_AnalogPriorityArray = 736,
  Zo_AnalogReliability = 756,
  Zo_AnalogRelinquishDefault = 774,
  Zo_AnalogStatusFlags = 798,
  Zo_AnalogValue = 816,
  Zo_AppVersion = 828,
  Zo_ApparentPower = 839,
  Zo_AqaraAccelerometer = 853,
  Zo_AqaraRotate = 872,
  Zo_AqaraVibration505 = 884,
  Zo_AqaraVibrationMode = 902,
  Zo_AqaraVibrationsOrAngle = 921,
  Zo_Aqara_FF05 = 944,
  Zo_ArrowClick = 955,
  Zo_ArrowHold = 966,
  Zo_ArrowRelease = 976,
  Zo_AvailablePower = 989,
  Zo_BatteryPercentage = 1004,
  Zo_BatteryVoltage = 1022,
  Zo_BinaryActiveText = 1037,
  Zo_BinaryApplicationType = 1054,
  Zo_BinaryDescription = 1076,
  Zo_BinaryInActiveText = 1094,
  Zo_BinaryInApplicationType = 1113,
  Zo_BinaryInDescription = 1137,
  Zo_BinaryInInactiveText = 1157,
  Zo_BinaryInOutOfService = 1178,
  Zo_BinaryInPolarity = 1199,
  Zo_BinaryInReliability = 1216,
  Zo_BinaryInStatusFlags = 1236,
  Zo_BinaryInValue = 1256,
  Zo_BinaryInactiveText = 1270,
  Zo_BinaryMinimumOffTime = 1289,
  Zo_BinaryMinimumOnTime = 1310,
  Zo_BinaryOutActiveText = 1330,
  Zo_BinaryOutApplicationType = 1350,
  Zo_BinaryOutDescription = 1375,
  Zo_BinaryOutInactiveText = 1396,
  Zo_BinaryOutMinimumOffTime = 1418,
  Zo_BinaryOutMinimumOnTime = 1442,
  Zo_BinaryOutOfService = 1465,
  Zo_BinaryOutOutOfService = 1484,
  Zo_BinaryOutPolarity = 1506,
  Zo_BinaryOutReliability = 1524,
  Zo_BinaryOutRelinquishDefault = 1545,
  Zo_BinaryOutStatusFlags = 1572,
  Zo_BinaryOutValue = 1593,
  Zo_BinaryReliability = 1608,
  Zo_BinaryRelinquishDefault = 1626,
  Zo_BinaryStatusFlags = 1650,
  Zo_BinaryValue = 1668,
  Zo_CIE = 1680,
  Zo_CO = 1684,
  Zo_CT = 1687,
  Zo_CheckinInterval = 1690,
  Zo_CheckinIntervalMin = 1706,
  Zo_ClosedLimit = 1725,
  Zo_Color = 1737,
  Zo_ColorMode = 1743,
  Zo_ColorMove = 1753,
  Zo_ColorPointBIntensity = 1763,
  Zo_ColorPointBX = 1784,
  Zo_ColorPointBY = 1797,
  Zo_ColorPointGIntensity = 1810,
  Zo_ColorPointGX = 1831,
  Zo_ColorPointGY = 1844,
  Zo_ColorPointRIntensity = 1857,
  Zo_ColorPointRX = 1878,
  Zo_ColorPointRY = 1891,
  Zo_ColorStep = 1904,
  Zo_ColorTempMove = 1914,
  Zo_ColorTempMoveDown = 1928,
  Zo_ColorTempMoveStop = 1946,
  Zo_ColorTempMoveUp = 1964,
  Zo_ColorTempStep = 1980,
  Zo_ColorTempStepDown = 1994,
  Zo_ColorTempStepUp = 2012,
  Zo_CompanyName = 2028,
  Zo_CompensationText = 2040,
  Zo_ConfigStatus = 2057,
  Zo_Contact = 2070,
  Zo_ControlSequenceOfOperation = 2078,
  Zo_CurrentGroup = 2105,
  Zo_CurrentPositionLift = 2118,
  Zo_CurrentPositionLiftPercentage = 2138,
  Zo_CurrentPositionTilt = 2168,
  Zo_CurrentPositionTiltPercentage = 2188,
  Zo_CurrentScene = 2218,
  Zo_CurrentTemperature = 2231,
  Zo_CurrentTemperatureSetPoint = 2250,
  Zo_CustomerName = 2277,
  Zo_DataQualityID = 2290,
  Zo_DateCode = 2304,
  Zo_DecelerationTimeLift = 2313,
  Zo_Dimmer = 2334,
  Zo_DimmerDown = 2341,
  Zo_DimmerMove = 2352,
  Zo_DimmerOptions = 2363,
  Zo_DimmerRemainingTime = 2377,
  Zo_DimmerStep = 2397,
  Zo_DimmerStepDown = 2408,
  Zo_DimmerStepUp = 2423,
  Zo_DimmerStop = 2436,
  Zo_DimmerUp = 2447,
  Zo_DoorClosedEvents = 2456,
  Zo_DoorOpenEvents = 2473,
  Zo_DoorState = 2488,
  Zo_DriftCompensation = 2498,
  Zo_DstEnd = 2516,
  Zo_DstShift = 2523,
  Zo_DstStart = 2532,
  Zo_EnergyFormatting = 2541,
  Zo_EnergyRemote = 2558,
  Zo_EnergyTotal = 2571,
  Zo_EurotronicErrors = 2583,
  Zo_EurotronicHostFlags = 2600,
  Zo_FastPollTimeout = 2620,
  Zo_FastPollTimeoutMax = 2636,
  Zo_Fire = 2655,
  Zo_FlowMaxMeasuredValue = 2660,
  Zo_FlowMinMeasuredValue = 2681,
  Zo_FlowRate = 2702,
  Zo_FlowTolerance = 2711,
  Zo_GenericDeviceClass = 2725,
  Zo_GenericDeviceType = 2744,
  Zo_GetAllGroups = 2762,
  Zo_GetGroup = 2775,
  Zo_GetSceneMembership = 2784,
  Zo_GlassBreak = 2803,
  Zo_GroupNameSupport = 2814,
  Zo_HWVersion = 2831,
  Zo_Hue = 2841,
  Zo_HueMove = 2845,
  Zo_HueSat = 2853,
  Zo_HueStep = 2860,
  Zo_HueStepDown = 2868,
  Zo_HueStepUp = 2880,
  Zo_Humidity = 2890,
  Zo_HumidityMaxMeasuredValue = 2899,
  Zo_HumidityMinMeasuredValue = 2924,
  Zo_HumidityTolerance = 2949,
  Zo_Identify = 2967,
  Zo_IdentifyQuery = 2976,
  Zo_IdentifyTime = 2990,
  Zo_Illuminance = 3003,
  Zo_IlluminanceLevelStatus = 3015,
  Zo_IlluminanceLightSensorType = 3038,
  Zo_IlluminanceMaxMeasuredValue = 3065,
  Zo_IlluminanceMinMeasuredValue = 3093,
  Zo_IlluminanceTargetLevel = 3121,
  Zo_IlluminanceTolerance = 3144,
  Zo_InstalledClosedLimitLift = 3165,
  Zo_InstalledClosedLimitTilt = 3190,
  Zo_InstalledOpenLimitLift = 3215,
  Zo_InstalledOpenLimitTilt = 3238,
  Zo_IntermediateSetpointsLift = 3261,
  Zo_IntermediateSetpointsTilt = 3287,
  Zo_LastMessageLQI = 3313,
  Zo_LastMessageRSSI = 3328,
  Zo_LastSetTime = 3344,
  Zo_LegrandHeatingMode = 3356,
  Zo_LegrandMode = 3375,
  Zo_LegrandOpt1 = 3387,
  Zo_LegrandOpt2 = 3399,
  Zo_LegrandOpt3 = 3411,
  Zo_LidlPower = 3423,
  Zo_LocalTemperature = 3433,
  Zo_LocalTemperatureCalibration = 3450,
  Zo_LocalTime = 3478,
  Zo_LocationAge = 3488,
  Zo_LocationMethod = 3500,
  Zo_LocationType = 3515,
  Zo_LockState = 3528,
  Zo_LockType = 3538,
  Zo_LongPollInterval = 3547,
  Zo_LongPollIntervalMin = 3564,
  Zo_MainsFrequency = 3584,
  Zo_MainsVoltage = 3599,
  Zo_Manufacturer = 3612,
  Zo_MaxTempExperienced = 3625,
  Zo_MeterTypeID = 3644,
  Zo_MinTempExperienced = 3656,
  Zo_Mode = 3675,
  Zo_Model = 3680,
  Zo_ModelId = 3686,
  Zo_MotorStepSize = 3694,
  Zo_Movement = 3708,
  Zo_MullerLightMode = 3717,
  Zo_MultiApplicationType = 3733,
  Zo_MultiDescription = 3754,
  Zo_MultiInApplicationType = 3771,
  Zo_MultiInDescription = 3794,
  Zo_MultiInNumberOfStates = 3813,
  Zo_MultiInOutOfService = 3835,
  Zo_MultiInReliability = 3855,
  Zo_MultiInStatusFlags = 3874,
  Zo_MultiInValue = 3893,
  Zo_MultiNumberOfStates = 3906,
  Zo_MultiOutApplicationType = 3926,
  Zo_MultiOutDescription = 3950,
  Zo_MultiOutNumberOfStates = 3970,
  Zo_MultiOutOfService = 3993,
  Zo_MultiOutOutOfService = 4011,
  Zo_MultiOutReliability = 4032,
  Zo_MultiOutRelinquishDefault = 4052,
  Zo_MultiOutStatusFlags = 4078,
  Zo_MultiOutValue = 4098,
  Zo_MultiReliability = 4112,
  Zo_MultiRelinquishDefault = 4129,
  Zo_MultiStatusFlags = 4152,
  Zo_MultiValue = 4169,
  Zo_MultipleScheduling = 4180,
  Zo_NumberOfDevices = 4199,
  Zo_NumberOfPrimaries = 4215,
  Zo_NumberOfResets = 4233,
  Zo_NumberofActuationsLift = 4248,
  Zo_NumberofActuationsTilt = 4271,
  Zo_Occupancy = 4294,
  Zo_OccupancySensorType = 4304,
  Zo_OccupiedCoolingSetpoint = 4324,
  Zo_OccupiedHeatingSetpoint = 4348,
  Zo_OnOffTransitionTime = 4372,
  Zo_OpenPeriod = 4392,
  Zo_OppleMode = 4403,
  Zo_OutdoorTemperature = 4413,
  Zo_OverTempTotalDwell = 4432,
  Zo_PICoolingDemand = 4451,
  Zo_PIHeatingDemand = 4467,
  Zo_POD = 4483,
  Zo_Panic = 4487,
  Zo_PartNumber = 4493,
  Zo_PersistentMemoryWrites = 4504,
  Zo_PersonalAlarm = 4527,
  Zo_PhysicalClosedLimit = 4541,
  Zo_PhysicalClosedLimitLift = 4561,
  Zo_PhysicalClosedLimitTilt = 4585,
  Zo_Power = 4609,
  Zo_Power2 = 4615,
  Zo_Power3 = 4622,
  Zo_Power4 = 4629,
  Zo_PowerOffEffect = 4636,
  Zo_PowerOnRecall = 4651,
  Zo_PowerOnTimer = 4665,
  Zo_PowerSource = 4678,
  Zo_PowerThreshold = 4690,
  Zo_Pressure = 4705,
  Zo_PressureMaxMeasuredValue = 4714,
  Zo_PressureMaxScaledValue = 4739,
  Zo_PressureMinMeasuredValue = 4762,
  Zo_PressureMinScaledValue = 4787,
  Zo_PressureScale = 4810,
  Zo_PressureScaledTolerance = 4824,
  Zo_PressureScaledValue = 4848,
  Zo_PressureTolerance = 4868,
  Zo_Primary1Intensity = 4886,
  Zo_Primary1X = 4904,
  Zo_Primary1Y = 4914,
  Zo_Primary2Intensity = 4924,
  Zo_Primary2X = 4942,
  Zo_Primary2Y = 4952,
  Zo_Primary3Intensity = 4962,
  Zo_Primary3X = 4980,
  Zo_Primary3Y = 4990,
  Zo_ProductCode = 5000,
  Zo_ProductRevision = 5012,
  Zo_ProductURL = 5028,
  Zo_QualityMeasure = 5039,
  Zo_RGB = 5054,
  Zo_RMSCurrent = 5058,
  Zo_RMSVoltage = 5069,
  Zo_ReactivePower = 5080,
  Zo_RecallScene = 5094,
  Zo_RemainingTime = 5106,
  Zo_RemoteSensing = 5120,
  Zo_RemoveAllGroups = 5134,
  Zo_RemoveAllScenes = 5150,
  Zo_RemoveGroup = 5166,
  Zo_RemoveScene = 5178,
  Zo_ResetAlarm = 5190,
  Zo_ResetAllAlarms = 5201,
  Zo_SWBuildID = 5216,
  Zo_Sat = 5226,
  Zo_SatMove = 5230,
  Zo_SatStep = 5238,
  Zo_SceneCount = 5246,
  Zo_SceneValid = 5257,
  Zo_ScheduleMode = 5268,
  Zo_SeaPressure = 5281,
  Zo_ShortPollInterval = 5293,
  Zo_Shutter = 5311,
  Zo_ShutterClose = 5319,
  Zo_ShutterLift = 5332,
  Zo_ShutterOpen = 5344,
  Zo_ShutterStop = 5356,
  Zo_ShutterTilt = 5368,
  Zo_SoftwareRevision = 5380,
  Zo_StackVersion = 5397,
  Zo_StandardTime = 5410,
  Zo_StartUpOnOff = 5423,
  Zo_Status = 5436,
  Zo_StoreScene = 5443,
  Zo_SwitchType = 5454,
  Zo_SystemMode = 5465,
  Zo_TRVBoost = 5476,
  Zo_TRVChildProtection = 5485,
  Zo_TRVMirrorDisplay = 5504,
  Zo_TRVMode = 5521,
  Zo_TRVWindowOpen = 5529,
  Zo_TempTarget = 5543,
  Zo_Temperature = 5554,
  Zo_TemperatureMaxMeasuredValue = 5566,
  Zo_TemperatureMinMeasuredValue = 5594,
  Zo_TemperatureTolerance = 5622,
  Zo_TerncyDuration = 5643,
  Zo_TerncyRotate = 5658,
  Zo_ThSetpoint = 5671,
  Zo_Time = 5682,
  Zo_TimeEpoch = 5687,
  Zo_TimeStatus = 5697,
  Zo_TimeZone = 5708,
  Zo_TotalProfileNum = 5717,
  Zo_TuyaAutoLock = 5733,
  Zo_TuyaAwayDays = 5746,
  Zo_TuyaAwayTemp = 5759,
  Zo_TuyaBattery = 5772,
  Zo_TuyaBoostTime = 5784,
  Zo_TuyaCalibration = 5798,
  Zo_TuyaCalibrationTime = 5814,
  Zo_TuyaChildLock = 5834,
  Zo_TuyaComfortTemp = 5848,
  Zo_TuyaEcoTemp = 5864,
  Zo_TuyaFanMode = 5876,
  Zo_TuyaForceMode = 5888,
  Zo_TuyaMCUVersion = 5902,
  Zo_TuyaMaxTemp = 5917,
  Zo_TuyaMinTemp = 5929,
  Zo_TuyaMotorReversal = 5941,
  Zo_TuyaMovingState = 5959,
  Zo_TuyaPreset = 5975,
  Zo_TuyaQuery = 5986,
  Zo_TuyaScheduleHolidays = 5996,
  Zo_TuyaScheduleWorkdays = 6017,
  Zo_TuyaTempTarget = 6038,
  Zo_TuyaValveDetection = 6053,
  Zo_TuyaValvePosition = 6072,
  Zo_TuyaWeekSelect = 6090,
  Zo_TuyaWindowDetection = 6105,
  Zo_UnoccupiedCoolingSetpoint = 6125,
  Zo_UnoccupiedHeatingSetpoint = 6151,
  Zo_UtilityName = 6177,
  Zo_ValidUntilTime = 6189,
  Zo_ValvePosition = 6204,
  Zo_VelocityLift = 6218,
  Zo_ViewGroup = 6231,
  Zo_ViewScene = 6241,
  Zo_Water = 6251,
  Zo_WhitePointX = 6257,
  Zo_WhitePointY = 6269,
  Zo_WindowCoveringType = 6281,
  Zo_X = 6300,
  Zo_Y = 6302,
  Zo_ZCLVersion = 6304,
  Zo_ZoneState = 6315,
  Zo_ZoneStatus = 6325,
  Zo_ZoneStatusChange = 6336,
  Zo_ZoneType = 6353,
  Zo__ = 6362,
  Zo_xx = 6364,
  Zo_xx000A00 = 6367,
  Zo_xx0A = 6376,
  Zo_xx0A00 = 6381,
  Zo_xx19 = 6388,
  Zo_xx190A = 6393,
  Zo_xx190A00 = 6400,
  Zo_xxxx = 6409,
  Zo_xxxx00 = 6414,
  Zo_xxxx0A00 = 6421,
  Zo_xxxxyy = 6430,
  Zo_xxxxyyyy = 6437,
  Zo_xxxxyyyy0A00 = 6446,
  Zo_xxxxyyzz = 6459,
  Zo_xxyy = 6468,
  Zo_xxyy0A00 = 6473,
  Zo_xxyyyy = 6482,
  Zo_xxyyyy000000000000 = 6489,
  Zo_xxyyyy0A0000000000 = 6508,
  Zo_xxyyyyzz = 6527,
  Zo_xxyyyyzzzz = 6536,
  Zo_xxyyzzzz = 6547,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
