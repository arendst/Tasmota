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
  "TuyaMaxTemp" "\x00"
  "TuyaMinTemp" "\x00"
  "TuyaMotorReversal" "\x00"
  "TuyaMovingState" "\x00"
  "TuyaPreset" "\x00"
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
  Zo_01190200 = 44,
  Zo_01xx0A00 = 53,
  Zo_01xxxx = 62,
  Zo_01xxxx000000000000 = 69,
  Zo_01xxxx0A0000000000 = 88,
  Zo_03xx0A00 = 107,
  Zo_03xxxx000000000000 = 116,
  Zo_03xxxx0A0000000000 = 135,
  Zo_AccelerationTimeLift = 154,
  Zo_ActivePower = 175,
  Zo_ActuatorEnabled = 187,
  Zo_AddGroup = 203,
  Zo_AddScene = 212,
  Zo_AlarmCount = 221,
  Zo_AnalogApplicationType = 232,
  Zo_AnalogDescription = 254,
  Zo_AnalogEngineeringUnits = 272,
  Zo_AnalogInApplicationType = 295,
  Zo_AnalogInDescription = 319,
  Zo_AnalogInEngineeringUnits = 339,
  Zo_AnalogInMaxValue = 364,
  Zo_AnalogInMinValue = 381,
  Zo_AnalogInOutOfService = 398,
  Zo_AnalogInReliability = 419,
  Zo_AnalogInResolution = 439,
  Zo_AnalogInStatusFlags = 458,
  Zo_AnalogOutApplicationType = 478,
  Zo_AnalogOutDescription = 503,
  Zo_AnalogOutEngineeringUnits = 524,
  Zo_AnalogOutMaxValue = 550,
  Zo_AnalogOutMinValue = 568,
  Zo_AnalogOutOfService = 586,
  Zo_AnalogOutOutOfService = 605,
  Zo_AnalogOutReliability = 627,
  Zo_AnalogOutRelinquishDefault = 648,
  Zo_AnalogOutResolution = 675,
  Zo_AnalogOutStatusFlags = 695,
  Zo_AnalogOutValue = 716,
  Zo_AnalogPriorityArray = 731,
  Zo_AnalogReliability = 751,
  Zo_AnalogRelinquishDefault = 769,
  Zo_AnalogStatusFlags = 793,
  Zo_AnalogValue = 811,
  Zo_AppVersion = 823,
  Zo_ApparentPower = 834,
  Zo_AqaraAccelerometer = 848,
  Zo_AqaraRotate = 867,
  Zo_AqaraVibration505 = 879,
  Zo_AqaraVibrationMode = 897,
  Zo_AqaraVibrationsOrAngle = 916,
  Zo_Aqara_FF05 = 939,
  Zo_ArrowClick = 950,
  Zo_ArrowHold = 961,
  Zo_ArrowRelease = 971,
  Zo_AvailablePower = 984,
  Zo_BatteryPercentage = 999,
  Zo_BatteryVoltage = 1017,
  Zo_BinaryActiveText = 1032,
  Zo_BinaryApplicationType = 1049,
  Zo_BinaryDescription = 1071,
  Zo_BinaryInActiveText = 1089,
  Zo_BinaryInApplicationType = 1108,
  Zo_BinaryInDescription = 1132,
  Zo_BinaryInInactiveText = 1152,
  Zo_BinaryInOutOfService = 1173,
  Zo_BinaryInPolarity = 1194,
  Zo_BinaryInReliability = 1211,
  Zo_BinaryInStatusFlags = 1231,
  Zo_BinaryInValue = 1251,
  Zo_BinaryInactiveText = 1265,
  Zo_BinaryMinimumOffTime = 1284,
  Zo_BinaryMinimumOnTime = 1305,
  Zo_BinaryOutActiveText = 1325,
  Zo_BinaryOutApplicationType = 1345,
  Zo_BinaryOutDescription = 1370,
  Zo_BinaryOutInactiveText = 1391,
  Zo_BinaryOutMinimumOffTime = 1413,
  Zo_BinaryOutMinimumOnTime = 1437,
  Zo_BinaryOutOfService = 1460,
  Zo_BinaryOutOutOfService = 1479,
  Zo_BinaryOutPolarity = 1501,
  Zo_BinaryOutReliability = 1519,
  Zo_BinaryOutRelinquishDefault = 1540,
  Zo_BinaryOutStatusFlags = 1567,
  Zo_BinaryOutValue = 1588,
  Zo_BinaryReliability = 1603,
  Zo_BinaryRelinquishDefault = 1621,
  Zo_BinaryStatusFlags = 1645,
  Zo_BinaryValue = 1663,
  Zo_CIE = 1675,
  Zo_CO = 1679,
  Zo_CT = 1682,
  Zo_CheckinInterval = 1685,
  Zo_CheckinIntervalMin = 1701,
  Zo_ClosedLimit = 1720,
  Zo_Color = 1732,
  Zo_ColorMode = 1738,
  Zo_ColorMove = 1748,
  Zo_ColorPointBIntensity = 1758,
  Zo_ColorPointBX = 1779,
  Zo_ColorPointBY = 1792,
  Zo_ColorPointGIntensity = 1805,
  Zo_ColorPointGX = 1826,
  Zo_ColorPointGY = 1839,
  Zo_ColorPointRIntensity = 1852,
  Zo_ColorPointRX = 1873,
  Zo_ColorPointRY = 1886,
  Zo_ColorStep = 1899,
  Zo_ColorTempMove = 1909,
  Zo_ColorTempMoveDown = 1923,
  Zo_ColorTempMoveStop = 1941,
  Zo_ColorTempMoveUp = 1959,
  Zo_ColorTempStep = 1975,
  Zo_ColorTempStepDown = 1989,
  Zo_ColorTempStepUp = 2007,
  Zo_CompanyName = 2023,
  Zo_CompensationText = 2035,
  Zo_ConfigStatus = 2052,
  Zo_Contact = 2065,
  Zo_ControlSequenceOfOperation = 2073,
  Zo_CurrentGroup = 2100,
  Zo_CurrentPositionLift = 2113,
  Zo_CurrentPositionLiftPercentage = 2133,
  Zo_CurrentPositionTilt = 2163,
  Zo_CurrentPositionTiltPercentage = 2183,
  Zo_CurrentScene = 2213,
  Zo_CurrentTemperature = 2226,
  Zo_CurrentTemperatureSetPoint = 2245,
  Zo_CustomerName = 2272,
  Zo_DataQualityID = 2285,
  Zo_DateCode = 2299,
  Zo_DecelerationTimeLift = 2308,
  Zo_Dimmer = 2329,
  Zo_DimmerDown = 2336,
  Zo_DimmerMove = 2347,
  Zo_DimmerOptions = 2358,
  Zo_DimmerRemainingTime = 2372,
  Zo_DimmerStep = 2392,
  Zo_DimmerStepDown = 2403,
  Zo_DimmerStepUp = 2418,
  Zo_DimmerStop = 2431,
  Zo_DimmerUp = 2442,
  Zo_DoorClosedEvents = 2451,
  Zo_DoorOpenEvents = 2468,
  Zo_DoorState = 2483,
  Zo_DriftCompensation = 2493,
  Zo_DstEnd = 2511,
  Zo_DstShift = 2518,
  Zo_DstStart = 2527,
  Zo_EnergyFormatting = 2536,
  Zo_EnergyRemote = 2553,
  Zo_EnergyTotal = 2566,
  Zo_EurotronicErrors = 2578,
  Zo_EurotronicHostFlags = 2595,
  Zo_FastPollTimeout = 2615,
  Zo_FastPollTimeoutMax = 2631,
  Zo_Fire = 2650,
  Zo_FlowMaxMeasuredValue = 2655,
  Zo_FlowMinMeasuredValue = 2676,
  Zo_FlowRate = 2697,
  Zo_FlowTolerance = 2706,
  Zo_GenericDeviceClass = 2720,
  Zo_GenericDeviceType = 2739,
  Zo_GetAllGroups = 2757,
  Zo_GetGroup = 2770,
  Zo_GetSceneMembership = 2779,
  Zo_GlassBreak = 2798,
  Zo_GroupNameSupport = 2809,
  Zo_HWVersion = 2826,
  Zo_Hue = 2836,
  Zo_HueMove = 2840,
  Zo_HueSat = 2848,
  Zo_HueStep = 2855,
  Zo_HueStepDown = 2863,
  Zo_HueStepUp = 2875,
  Zo_Humidity = 2885,
  Zo_HumidityMaxMeasuredValue = 2894,
  Zo_HumidityMinMeasuredValue = 2919,
  Zo_HumidityTolerance = 2944,
  Zo_Identify = 2962,
  Zo_IdentifyQuery = 2971,
  Zo_IdentifyTime = 2985,
  Zo_Illuminance = 2998,
  Zo_IlluminanceLevelStatus = 3010,
  Zo_IlluminanceLightSensorType = 3033,
  Zo_IlluminanceMaxMeasuredValue = 3060,
  Zo_IlluminanceMinMeasuredValue = 3088,
  Zo_IlluminanceTargetLevel = 3116,
  Zo_IlluminanceTolerance = 3139,
  Zo_InstalledClosedLimitLift = 3160,
  Zo_InstalledClosedLimitTilt = 3185,
  Zo_InstalledOpenLimitLift = 3210,
  Zo_InstalledOpenLimitTilt = 3233,
  Zo_IntermediateSetpointsLift = 3256,
  Zo_IntermediateSetpointsTilt = 3282,
  Zo_LastMessageLQI = 3308,
  Zo_LastMessageRSSI = 3323,
  Zo_LastSetTime = 3339,
  Zo_LegrandHeatingMode = 3351,
  Zo_LegrandMode = 3370,
  Zo_LegrandOpt1 = 3382,
  Zo_LegrandOpt2 = 3394,
  Zo_LegrandOpt3 = 3406,
  Zo_LidlPower = 3418,
  Zo_LocalTemperature = 3428,
  Zo_LocalTemperatureCalibration = 3445,
  Zo_LocalTime = 3473,
  Zo_LocationAge = 3483,
  Zo_LocationMethod = 3495,
  Zo_LocationType = 3510,
  Zo_LockState = 3523,
  Zo_LockType = 3533,
  Zo_LongPollInterval = 3542,
  Zo_LongPollIntervalMin = 3559,
  Zo_MainsFrequency = 3579,
  Zo_MainsVoltage = 3594,
  Zo_Manufacturer = 3607,
  Zo_MaxTempExperienced = 3620,
  Zo_MeterTypeID = 3639,
  Zo_MinTempExperienced = 3651,
  Zo_Mode = 3670,
  Zo_Model = 3675,
  Zo_ModelId = 3681,
  Zo_MotorStepSize = 3689,
  Zo_Movement = 3703,
  Zo_MullerLightMode = 3712,
  Zo_MultiApplicationType = 3728,
  Zo_MultiDescription = 3749,
  Zo_MultiInApplicationType = 3766,
  Zo_MultiInDescription = 3789,
  Zo_MultiInNumberOfStates = 3808,
  Zo_MultiInOutOfService = 3830,
  Zo_MultiInReliability = 3850,
  Zo_MultiInStatusFlags = 3869,
  Zo_MultiInValue = 3888,
  Zo_MultiNumberOfStates = 3901,
  Zo_MultiOutApplicationType = 3921,
  Zo_MultiOutDescription = 3945,
  Zo_MultiOutNumberOfStates = 3965,
  Zo_MultiOutOfService = 3988,
  Zo_MultiOutOutOfService = 4006,
  Zo_MultiOutReliability = 4027,
  Zo_MultiOutRelinquishDefault = 4047,
  Zo_MultiOutStatusFlags = 4073,
  Zo_MultiOutValue = 4093,
  Zo_MultiReliability = 4107,
  Zo_MultiRelinquishDefault = 4124,
  Zo_MultiStatusFlags = 4147,
  Zo_MultiValue = 4164,
  Zo_MultipleScheduling = 4175,
  Zo_NumberOfDevices = 4194,
  Zo_NumberOfPrimaries = 4210,
  Zo_NumberOfResets = 4228,
  Zo_NumberofActuationsLift = 4243,
  Zo_NumberofActuationsTilt = 4266,
  Zo_Occupancy = 4289,
  Zo_OccupancySensorType = 4299,
  Zo_OccupiedCoolingSetpoint = 4319,
  Zo_OccupiedHeatingSetpoint = 4343,
  Zo_OnOffTransitionTime = 4367,
  Zo_OpenPeriod = 4387,
  Zo_OppleMode = 4398,
  Zo_OutdoorTemperature = 4408,
  Zo_OverTempTotalDwell = 4427,
  Zo_PICoolingDemand = 4446,
  Zo_PIHeatingDemand = 4462,
  Zo_POD = 4478,
  Zo_Panic = 4482,
  Zo_PartNumber = 4488,
  Zo_PersistentMemoryWrites = 4499,
  Zo_PersonalAlarm = 4522,
  Zo_PhysicalClosedLimit = 4536,
  Zo_PhysicalClosedLimitLift = 4556,
  Zo_PhysicalClosedLimitTilt = 4580,
  Zo_Power = 4604,
  Zo_Power2 = 4610,
  Zo_Power3 = 4617,
  Zo_Power4 = 4624,
  Zo_PowerOffEffect = 4631,
  Zo_PowerOnRecall = 4646,
  Zo_PowerOnTimer = 4660,
  Zo_PowerSource = 4673,
  Zo_PowerThreshold = 4685,
  Zo_Pressure = 4700,
  Zo_PressureMaxMeasuredValue = 4709,
  Zo_PressureMaxScaledValue = 4734,
  Zo_PressureMinMeasuredValue = 4757,
  Zo_PressureMinScaledValue = 4782,
  Zo_PressureScale = 4805,
  Zo_PressureScaledTolerance = 4819,
  Zo_PressureScaledValue = 4843,
  Zo_PressureTolerance = 4863,
  Zo_Primary1Intensity = 4881,
  Zo_Primary1X = 4899,
  Zo_Primary1Y = 4909,
  Zo_Primary2Intensity = 4919,
  Zo_Primary2X = 4937,
  Zo_Primary2Y = 4947,
  Zo_Primary3Intensity = 4957,
  Zo_Primary3X = 4975,
  Zo_Primary3Y = 4985,
  Zo_ProductCode = 4995,
  Zo_ProductRevision = 5007,
  Zo_ProductURL = 5023,
  Zo_QualityMeasure = 5034,
  Zo_RGB = 5049,
  Zo_RMSCurrent = 5053,
  Zo_RMSVoltage = 5064,
  Zo_ReactivePower = 5075,
  Zo_RecallScene = 5089,
  Zo_RemainingTime = 5101,
  Zo_RemoteSensing = 5115,
  Zo_RemoveAllGroups = 5129,
  Zo_RemoveAllScenes = 5145,
  Zo_RemoveGroup = 5161,
  Zo_RemoveScene = 5173,
  Zo_ResetAlarm = 5185,
  Zo_ResetAllAlarms = 5196,
  Zo_SWBuildID = 5211,
  Zo_Sat = 5221,
  Zo_SatMove = 5225,
  Zo_SatStep = 5233,
  Zo_SceneCount = 5241,
  Zo_SceneValid = 5252,
  Zo_ScheduleMode = 5263,
  Zo_SeaPressure = 5276,
  Zo_ShortPollInterval = 5288,
  Zo_Shutter = 5306,
  Zo_ShutterClose = 5314,
  Zo_ShutterLift = 5327,
  Zo_ShutterOpen = 5339,
  Zo_ShutterStop = 5351,
  Zo_ShutterTilt = 5363,
  Zo_SoftwareRevision = 5375,
  Zo_StackVersion = 5392,
  Zo_StandardTime = 5405,
  Zo_StartUpOnOff = 5418,
  Zo_Status = 5431,
  Zo_StoreScene = 5438,
  Zo_SwitchType = 5449,
  Zo_SystemMode = 5460,
  Zo_TRVBoost = 5471,
  Zo_TRVChildProtection = 5480,
  Zo_TRVMirrorDisplay = 5499,
  Zo_TRVMode = 5516,
  Zo_TRVWindowOpen = 5524,
  Zo_TempTarget = 5538,
  Zo_Temperature = 5549,
  Zo_TemperatureMaxMeasuredValue = 5561,
  Zo_TemperatureMinMeasuredValue = 5589,
  Zo_TemperatureTolerance = 5617,
  Zo_TerncyDuration = 5638,
  Zo_TerncyRotate = 5653,
  Zo_ThSetpoint = 5666,
  Zo_Time = 5677,
  Zo_TimeEpoch = 5682,
  Zo_TimeStatus = 5692,
  Zo_TimeZone = 5703,
  Zo_TotalProfileNum = 5712,
  Zo_TuyaAutoLock = 5728,
  Zo_TuyaAwayDays = 5741,
  Zo_TuyaAwayTemp = 5754,
  Zo_TuyaBattery = 5767,
  Zo_TuyaBoostTime = 5779,
  Zo_TuyaCalibration = 5793,
  Zo_TuyaCalibrationTime = 5809,
  Zo_TuyaChildLock = 5829,
  Zo_TuyaComfortTemp = 5843,
  Zo_TuyaEcoTemp = 5859,
  Zo_TuyaFanMode = 5871,
  Zo_TuyaForceMode = 5883,
  Zo_TuyaMaxTemp = 5897,
  Zo_TuyaMinTemp = 5909,
  Zo_TuyaMotorReversal = 5921,
  Zo_TuyaMovingState = 5939,
  Zo_TuyaPreset = 5955,
  Zo_TuyaScheduleHolidays = 5966,
  Zo_TuyaScheduleWorkdays = 5987,
  Zo_TuyaTempTarget = 6008,
  Zo_TuyaValveDetection = 6023,
  Zo_TuyaValvePosition = 6042,
  Zo_TuyaWeekSelect = 6060,
  Zo_TuyaWindowDetection = 6075,
  Zo_UnoccupiedCoolingSetpoint = 6095,
  Zo_UnoccupiedHeatingSetpoint = 6121,
  Zo_UtilityName = 6147,
  Zo_ValidUntilTime = 6159,
  Zo_ValvePosition = 6174,
  Zo_VelocityLift = 6188,
  Zo_ViewGroup = 6201,
  Zo_ViewScene = 6211,
  Zo_Water = 6221,
  Zo_WhitePointX = 6227,
  Zo_WhitePointY = 6239,
  Zo_WindowCoveringType = 6251,
  Zo_X = 6270,
  Zo_Y = 6272,
  Zo_ZCLVersion = 6274,
  Zo_ZoneState = 6285,
  Zo_ZoneStatus = 6295,
  Zo_ZoneStatusChange = 6306,
  Zo_ZoneType = 6323,
  Zo__ = 6332,
  Zo_xx = 6334,
  Zo_xx000A00 = 6337,
  Zo_xx0A = 6346,
  Zo_xx0A00 = 6351,
  Zo_xx19 = 6358,
  Zo_xx190A = 6363,
  Zo_xx190A00 = 6370,
  Zo_xxxx = 6379,
  Zo_xxxx00 = 6384,
  Zo_xxxx0A00 = 6391,
  Zo_xxxxyy = 6400,
  Zo_xxxxyyyy = 6407,
  Zo_xxxxyyyy0A00 = 6416,
  Zo_xxxxyyzz = 6429,
  Zo_xxyy = 6438,
  Zo_xxyy0A00 = 6443,
  Zo_xxyyyy = 6452,
  Zo_xxyyyy000000000000 = 6459,
  Zo_xxyyyy0A0000000000 = 6478,
  Zo_xxyyyyzz = 6497,
  Zo_xxyyyyzzzz = 6506,
  Zo_xxyyzzzz = 6517,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
