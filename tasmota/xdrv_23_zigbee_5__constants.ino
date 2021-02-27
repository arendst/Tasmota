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
  "TuyaChildLock" "\x00"
  "TuyaComfortTemp" "\x00"
  "TuyaEcoTemp" "\x00"
  "TuyaFanMode" "\x00"
  "TuyaForceMode" "\x00"
  "TuyaMaxTemp" "\x00"
  "TuyaMinTemp" "\x00"
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
  Zo_LidlPower = 3351,
  Zo_LocalTemperature = 3361,
  Zo_LocalTemperatureCalibration = 3378,
  Zo_LocalTime = 3406,
  Zo_LocationAge = 3416,
  Zo_LocationMethod = 3428,
  Zo_LocationType = 3443,
  Zo_LockState = 3456,
  Zo_LockType = 3466,
  Zo_LongPollInterval = 3475,
  Zo_LongPollIntervalMin = 3492,
  Zo_MainsFrequency = 3512,
  Zo_MainsVoltage = 3527,
  Zo_Manufacturer = 3540,
  Zo_MaxTempExperienced = 3553,
  Zo_MeterTypeID = 3572,
  Zo_MinTempExperienced = 3584,
  Zo_Mode = 3603,
  Zo_Model = 3608,
  Zo_ModelId = 3614,
  Zo_MotorStepSize = 3622,
  Zo_Movement = 3636,
  Zo_MullerLightMode = 3645,
  Zo_MultiApplicationType = 3661,
  Zo_MultiDescription = 3682,
  Zo_MultiInApplicationType = 3699,
  Zo_MultiInDescription = 3722,
  Zo_MultiInNumberOfStates = 3741,
  Zo_MultiInOutOfService = 3763,
  Zo_MultiInReliability = 3783,
  Zo_MultiInStatusFlags = 3802,
  Zo_MultiInValue = 3821,
  Zo_MultiNumberOfStates = 3834,
  Zo_MultiOutApplicationType = 3854,
  Zo_MultiOutDescription = 3878,
  Zo_MultiOutNumberOfStates = 3898,
  Zo_MultiOutOfService = 3921,
  Zo_MultiOutOutOfService = 3939,
  Zo_MultiOutReliability = 3960,
  Zo_MultiOutRelinquishDefault = 3980,
  Zo_MultiOutStatusFlags = 4006,
  Zo_MultiOutValue = 4026,
  Zo_MultiReliability = 4040,
  Zo_MultiRelinquishDefault = 4057,
  Zo_MultiStatusFlags = 4080,
  Zo_MultiValue = 4097,
  Zo_MultipleScheduling = 4108,
  Zo_NumberOfDevices = 4127,
  Zo_NumberOfPrimaries = 4143,
  Zo_NumberOfResets = 4161,
  Zo_NumberofActuationsLift = 4176,
  Zo_NumberofActuationsTilt = 4199,
  Zo_Occupancy = 4222,
  Zo_OccupancySensorType = 4232,
  Zo_OccupiedCoolingSetpoint = 4252,
  Zo_OccupiedHeatingSetpoint = 4276,
  Zo_OnOffTransitionTime = 4300,
  Zo_OpenPeriod = 4320,
  Zo_OppleMode = 4331,
  Zo_OutdoorTemperature = 4341,
  Zo_OverTempTotalDwell = 4360,
  Zo_PICoolingDemand = 4379,
  Zo_PIHeatingDemand = 4395,
  Zo_POD = 4411,
  Zo_Panic = 4415,
  Zo_PartNumber = 4421,
  Zo_PersistentMemoryWrites = 4432,
  Zo_PersonalAlarm = 4455,
  Zo_PhysicalClosedLimit = 4469,
  Zo_PhysicalClosedLimitLift = 4489,
  Zo_PhysicalClosedLimitTilt = 4513,
  Zo_Power = 4537,
  Zo_Power2 = 4543,
  Zo_Power3 = 4550,
  Zo_Power4 = 4557,
  Zo_PowerOffEffect = 4564,
  Zo_PowerOnRecall = 4579,
  Zo_PowerOnTimer = 4593,
  Zo_PowerSource = 4606,
  Zo_PowerThreshold = 4618,
  Zo_Pressure = 4633,
  Zo_PressureMaxMeasuredValue = 4642,
  Zo_PressureMaxScaledValue = 4667,
  Zo_PressureMinMeasuredValue = 4690,
  Zo_PressureMinScaledValue = 4715,
  Zo_PressureScale = 4738,
  Zo_PressureScaledTolerance = 4752,
  Zo_PressureScaledValue = 4776,
  Zo_PressureTolerance = 4796,
  Zo_Primary1Intensity = 4814,
  Zo_Primary1X = 4832,
  Zo_Primary1Y = 4842,
  Zo_Primary2Intensity = 4852,
  Zo_Primary2X = 4870,
  Zo_Primary2Y = 4880,
  Zo_Primary3Intensity = 4890,
  Zo_Primary3X = 4908,
  Zo_Primary3Y = 4918,
  Zo_ProductCode = 4928,
  Zo_ProductRevision = 4940,
  Zo_ProductURL = 4956,
  Zo_QualityMeasure = 4967,
  Zo_RGB = 4982,
  Zo_RMSCurrent = 4986,
  Zo_RMSVoltage = 4997,
  Zo_ReactivePower = 5008,
  Zo_RecallScene = 5022,
  Zo_RemainingTime = 5034,
  Zo_RemoteSensing = 5048,
  Zo_RemoveAllGroups = 5062,
  Zo_RemoveAllScenes = 5078,
  Zo_RemoveGroup = 5094,
  Zo_RemoveScene = 5106,
  Zo_ResetAlarm = 5118,
  Zo_ResetAllAlarms = 5129,
  Zo_SWBuildID = 5144,
  Zo_Sat = 5154,
  Zo_SatMove = 5158,
  Zo_SatStep = 5166,
  Zo_SceneCount = 5174,
  Zo_SceneValid = 5185,
  Zo_ScheduleMode = 5196,
  Zo_SeaPressure = 5209,
  Zo_ShortPollInterval = 5221,
  Zo_Shutter = 5239,
  Zo_ShutterClose = 5247,
  Zo_ShutterLift = 5260,
  Zo_ShutterOpen = 5272,
  Zo_ShutterStop = 5284,
  Zo_ShutterTilt = 5296,
  Zo_SoftwareRevision = 5308,
  Zo_StackVersion = 5325,
  Zo_StandardTime = 5338,
  Zo_StartUpOnOff = 5351,
  Zo_Status = 5364,
  Zo_StoreScene = 5371,
  Zo_SwitchType = 5382,
  Zo_SystemMode = 5393,
  Zo_TRVBoost = 5404,
  Zo_TRVChildProtection = 5413,
  Zo_TRVMirrorDisplay = 5432,
  Zo_TRVMode = 5449,
  Zo_TRVWindowOpen = 5457,
  Zo_TempTarget = 5471,
  Zo_Temperature = 5482,
  Zo_TemperatureMaxMeasuredValue = 5494,
  Zo_TemperatureMinMeasuredValue = 5522,
  Zo_TemperatureTolerance = 5550,
  Zo_TerncyDuration = 5571,
  Zo_TerncyRotate = 5586,
  Zo_ThSetpoint = 5599,
  Zo_Time = 5610,
  Zo_TimeEpoch = 5615,
  Zo_TimeStatus = 5625,
  Zo_TimeZone = 5636,
  Zo_TotalProfileNum = 5645,
  Zo_TuyaAutoLock = 5661,
  Zo_TuyaAwayDays = 5674,
  Zo_TuyaAwayTemp = 5687,
  Zo_TuyaBattery = 5700,
  Zo_TuyaBoostTime = 5712,
  Zo_TuyaChildLock = 5726,
  Zo_TuyaComfortTemp = 5740,
  Zo_TuyaEcoTemp = 5756,
  Zo_TuyaFanMode = 5768,
  Zo_TuyaForceMode = 5780,
  Zo_TuyaMaxTemp = 5794,
  Zo_TuyaMinTemp = 5806,
  Zo_TuyaPreset = 5818,
  Zo_TuyaScheduleHolidays = 5829,
  Zo_TuyaScheduleWorkdays = 5850,
  Zo_TuyaTempTarget = 5871,
  Zo_TuyaValveDetection = 5886,
  Zo_TuyaValvePosition = 5905,
  Zo_TuyaWeekSelect = 5923,
  Zo_TuyaWindowDetection = 5938,
  Zo_UnoccupiedCoolingSetpoint = 5958,
  Zo_UnoccupiedHeatingSetpoint = 5984,
  Zo_UtilityName = 6010,
  Zo_ValidUntilTime = 6022,
  Zo_ValvePosition = 6037,
  Zo_VelocityLift = 6051,
  Zo_ViewGroup = 6064,
  Zo_ViewScene = 6074,
  Zo_Water = 6084,
  Zo_WhitePointX = 6090,
  Zo_WhitePointY = 6102,
  Zo_WindowCoveringType = 6114,
  Zo_X = 6133,
  Zo_Y = 6135,
  Zo_ZCLVersion = 6137,
  Zo_ZoneState = 6148,
  Zo_ZoneStatus = 6158,
  Zo_ZoneStatusChange = 6169,
  Zo_ZoneType = 6186,
  Zo__ = 6195,
  Zo_xx = 6197,
  Zo_xx000A00 = 6200,
  Zo_xx0A = 6209,
  Zo_xx0A00 = 6214,
  Zo_xx19 = 6221,
  Zo_xx190A = 6226,
  Zo_xx190A00 = 6233,
  Zo_xxxx = 6242,
  Zo_xxxx00 = 6247,
  Zo_xxxx0A00 = 6254,
  Zo_xxxxyy = 6263,
  Zo_xxxxyyyy = 6270,
  Zo_xxxxyyyy0A00 = 6279,
  Zo_xxxxyyzz = 6292,
  Zo_xxyy = 6301,
  Zo_xxyy0A00 = 6306,
  Zo_xxyyyy = 6315,
  Zo_xxyyyy000000000000 = 6322,
  Zo_xxyyyy0A0000000000 = 6341,
  Zo_xxyyyyzz = 6360,
  Zo_xxyyyyzzzz = 6369,
  Zo_xxyyzzzz = 6380,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
