/*
  xdrv_23_zigbee_5__constants.ino - zigbee support for Tasmota

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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
  "ZCLVersion" "\x00"
  "AppVersion" "\x00"
  "StackVersion" "\x00"
  "HWVersion" "\x00"
  "Manufacturer" "\x00"
  "ModelId" "\x00"
  "DateCode" "\x00"
  "PowerSource" "\x00"
  "GenericDeviceClass" "\x00"
  "GenericDeviceType" "\x00"
  "ProductCode" "\x00"
  "ProductURL" "\x00"
  "SWBuildID" "\x00"
  "MainsVoltage" "\x00"
  "MainsFrequency" "\x00"
  "BatteryVoltage" "\x00"
  "BatteryPercentage" "\x00"
  "CurrentTemperature" "\x00"
  "MinTempExperienced" "\x00"
  "MaxTempExperienced" "\x00"
  "OverTempTotalDwell" "\x00"
  "IdentifyTime" "\x00"
  "GroupNameSupport" "\x00"
  "SceneCount" "\x00"
  "CurrentScene" "\x00"
  "CurrentGroup" "\x00"
  "SceneValid" "\x00"
  "NameSupport" "\x00"
  "Power" "\x00"
  "StartUpOnOff" "\x00"
  "SwitchType" "\x00"
  "Dimmer" "\x00"
  "DimmerOptions" "\x00"
  "DimmerRemainingTime" "\x00"
  "OnOffTransitionTime" "\x00"
  "OnLevel" "\x00"
  "OnTransitionTime" "\x00"
  "OffTransitionTime" "\x00"
  "DefaultMoveRate" "\x00"
  "AlarmCount" "\x00"
  "Time" "\x00"
  "TimeStatus" "\x00"
  "TimeZone" "\x00"
  "DstStart" "\x00"
  "DstEnd" "\x00"
  "DstShift" "\x00"
  "StandardTime" "\x00"
  "LocalTime" "\x00"
  "LastSetTime" "\x00"
  "ValidUntilTime" "\x00"
  "TimeEpoch" "\x00"
  "LocationType" "\x00"
  "LocationMethod" "\x00"
  "LocationAge" "\x00"
  "QualityMeasure" "\x00"
  "NumberOfDevices" "\x00"
  "AnalogInActiveText" "\x00"
  "AnalogInDescription" "\x00"
  "AnalogInInactiveText" "\x00"
  "AnalogInMaxValue" "\x00"
  "AnalogInMinValue" "\x00"
  "AnalogInOutOfService" "\x00"
  "AqaraRotate" "\x00"
  "AnalogInPriorityArray" "\x00"
  "AnalogInReliability" "\x00"
  "AnalogInRelinquishDefault" "\x00"
  "AnalogInResolution" "\x00"
  "AnalogInStatusFlags" "\x00"
  "AnalogInEngineeringUnits" "\x00"
  "AnalogInApplicationType" "\x00"
  "Aqara_FF05" "\x00"
  "AnalogOutDescription" "\x00"
  "AnalogOutMaxValue" "\x00"
  "AnalogOutMinValue" "\x00"
  "AnalogOutOutOfService" "\x00"
  "AnalogOutValue" "\x00"
  "AnalogOutPriorityArray" "\x00"
  "AnalogOutReliability" "\x00"
  "AnalogOutRelinquishDefault" "\x00"
  "AnalogOutResolution" "\x00"
  "AnalogOutStatusFlags" "\x00"
  "AnalogOutEngineeringUnits" "\x00"
  "AnalogOutApplicationType" "\x00"
  "AnalogDescription" "\x00"
  "AnalogOutOfService" "\x00"
  "AnalogValue" "\x00"
  "AnalogPriorityArray" "\x00"
  "AnalogReliability" "\x00"
  "AnalogRelinquishDefault" "\x00"
  "AnalogStatusFlags" "\x00"
  "AnalogEngineeringUnits" "\x00"
  "AnalogApplicationType" "\x00"
  "BinaryInActiveText" "\x00"
  "BinaryInDescription" "\x00"
  "BinaryInInactiveText" "\x00"
  "BinaryInOutOfService" "\x00"
  "BinaryInPolarity" "\x00"
  "BinaryInValue" "\x00"
  "BinaryInPriorityArray" "\x00"
  "BinaryInReliability" "\x00"
  "BinaryInStatusFlags" "\x00"
  "BinaryInApplicationType" "\x00"
  "BinaryOutActiveText" "\x00"
  "BinaryOutDescription" "\x00"
  "BinaryOutInactiveText" "\x00"
  "BinaryOutMinimumOffTime" "\x00"
  "BinaryOutMinimumOnTime" "\x00"
  "BinaryOutOutOfService" "\x00"
  "BinaryOutPolarity" "\x00"
  "BinaryOutValue" "\x00"
  "BinaryOutPriorityArray" "\x00"
  "BinaryOutReliability" "\x00"
  "BinaryOutRelinquishDefault" "\x00"
  "BinaryOutStatusFlags" "\x00"
  "BinaryOutApplicationType" "\x00"
  "BinaryActiveText" "\x00"
  "BinaryDescription" "\x00"
  "BinaryInactiveText" "\x00"
  "BinaryMinimumOffTime" "\x00"
  "BinaryMinimumOnTime" "\x00"
  "BinaryOutOfService" "\x00"
  "BinaryValue" "\x00"
  "BinaryPriorityArray" "\x00"
  "BinaryReliability" "\x00"
  "BinaryRelinquishDefault" "\x00"
  "BinaryStatusFlags" "\x00"
  "BinaryApplicationType" "\x00"
  "MultiInStateText" "\x00"
  "MultiInDescription" "\x00"
  "MultiInNumberOfStates" "\x00"
  "MultiInOutOfService" "\x00"
  "MultiInValue" "\x00"
  "MultiInReliability" "\x00"
  "MultiInStatusFlags" "\x00"
  "MultiInApplicationType" "\x00"
  "MultiOutStateText" "\x00"
  "MultiOutDescription" "\x00"
  "MultiOutNumberOfStates" "\x00"
  "MultiOutOutOfService" "\x00"
  "MultiOutValue" "\x00"
  "MultiOutPriorityArray" "\x00"
  "MultiOutReliability" "\x00"
  "MultiOutRelinquishDefault" "\x00"
  "MultiOutStatusFlags" "\x00"
  "MultiOutApplicationType" "\x00"
  "MultiStateText" "\x00"
  "MultiDescription" "\x00"
  "MultiNumberOfStates" "\x00"
  "MultiOutOfService" "\x00"
  "MultiValue" "\x00"
  "MultiReliability" "\x00"
  "MultiRelinquishDefault" "\x00"
  "MultiStatusFlags" "\x00"
  "MultiApplicationType" "\x00"
  "TotalProfileNum" "\x00"
  "MultipleScheduling" "\x00"
  "EnergyFormatting" "\x00"
  "EnergyRemote" "\x00"
  "ScheduleMode" "\x00"
  "CheckinInterval" "\x00"
  "LongPollInterval" "\x00"
  "ShortPollInterval" "\x00"
  "FastPollTimeout" "\x00"
  "CheckinIntervalMin" "\x00"
  "LongPollIntervalMin" "\x00"
  "FastPollTimeoutMax" "\x00"
  "PhysicalClosedLimit" "\x00"
  "MotorStepSize" "\x00"
  "Status" "\x00"
  "ClosedLimit" "\x00"
  "Mode" "\x00"
  "LockState" "\x00"
  "LockType" "\x00"
  "ActuatorEnabled" "\x00"
  "DoorState" "\x00"
  "DoorOpenEvents" "\x00"
  "DoorClosedEvents" "\x00"
  "OpenPeriod" "\x00"
  "AqaraVibrationMode" "\x00"
  "AqaraVibrationsOrAngle" "\x00"
  "AqaraVibration505" "\x00"
  "AqaraAccelerometer" "\x00"
  "WindowCoveringType" "\x00"
  "PhysicalClosedLimitLift" "\x00"
  "PhysicalClosedLimitTilt" "\x00"
  "CurrentPositionLift" "\x00"
  "CurrentPositionTilt" "\x00"
  "NumberofActuationsLift" "\x00"
  "NumberofActuationsTilt" "\x00"
  "ConfigStatus" "\x00"
  "CurrentPositionLiftPercentage" "\x00"
  "CurrentPositionTiltPercentage" "\x00"
  "InstalledOpenLimitLift" "\x00"
  "InstalledClosedLimitLift" "\x00"
  "InstalledOpenLimitTilt" "\x00"
  "InstalledClosedLimitTilt" "\x00"
  "VelocityLift" "\x00"
  "AccelerationTimeLift" "\x00"
  "DecelerationTimeLift" "\x00"
  "IntermediateSetpointsLift" "\x00"
  "IntermediateSetpointsTilt" "\x00"
  "Hue" "\x00"
  "Sat" "\x00"
  "RemainingTime" "\x00"
  "X" "\x00"
  "Y" "\x00"
  "DriftCompensation" "\x00"
  "CompensationText" "\x00"
  "CT" "\x00"
  "ColorMode" "\x00"
  "NumberOfPrimaries" "\x00"
  "Primary1X" "\x00"
  "Primary1Y" "\x00"
  "Primary1Intensity" "\x00"
  "Primary2X" "\x00"
  "Primary2Y" "\x00"
  "Primary2Intensity" "\x00"
  "Primary3X" "\x00"
  "Primary3Y" "\x00"
  "Primary3Intensity" "\x00"
  "WhitePointX" "\x00"
  "WhitePointY" "\x00"
  "ColorPointRX" "\x00"
  "ColorPointRY" "\x00"
  "ColorPointRIntensity" "\x00"
  "ColorPointGX" "\x00"
  "ColorPointGY" "\x00"
  "ColorPointGIntensity" "\x00"
  "ColorPointBX" "\x00"
  "ColorPointBY" "\x00"
  "ColorPointBIntensity" "\x00"
  "Illuminance" "\x00"
  "IlluminanceMinMeasuredValue" "\x00"
  "IlluminanceMaxMeasuredValue" "\x00"
  "IlluminanceTolerance" "\x00"
  "IlluminanceLightSensorType" "\x00"
  "IlluminanceLevelStatus" "\x00"
  "IlluminanceTargetLevel" "\x00"
  "Temperature" "\x00"
  "TemperatureMinMeasuredValue" "\x00"
  "TemperatureMaxMeasuredValue" "\x00"
  "TemperatureTolerance" "\x00"
  "PressureUnit" "\x00"
  "Pressure" "\x00"
  "PressureMinMeasuredValue" "\x00"
  "PressureMaxMeasuredValue" "\x00"
  "PressureTolerance" "\x00"
  "PressureScaledValue" "\x00"
  "PressureMinScaledValue" "\x00"
  "PressureMaxScaledValue" "\x00"
  "PressureScaledTolerance" "\x00"
  "PressureScale" "\x00"
  "FlowRate" "\x00"
  "FlowMinMeasuredValue" "\x00"
  "FlowMaxMeasuredValue" "\x00"
  "FlowTolerance" "\x00"
  "Humidity" "\x00"
  "HumidityMinMeasuredValue" "\x00"
  "HumidityMaxMeasuredValue" "\x00"
  "HumidityTolerance" "\x00"
  "Occupancy" "\x00"
  "OccupancySensorType" "\x00"
  "ZoneState" "\x00"
  "ZoneType" "\x00"
  "ZoneStatus" "\x00"
  "CompanyName" "\x00"
  "MeterTypeID" "\x00"
  "DataQualityID" "\x00"
  "CustomerName" "\x00"
  "Model" "\x00"
  "PartNumber" "\x00"
  "ProductRevision" "\x00"
  "SoftwareRevision" "\x00"
  "UtilityName" "\x00"
  "POD" "\x00"
  "AvailablePower" "\x00"
  "PowerThreshold" "\x00"
  "NumberOfResets" "\x00"
  "PersistentMemoryWrites" "\x00"
  "LastMessageLQI" "\x00"
  "LastMessageRSSI" "\x00"
  "Identify" "\x00"
  "xxxx" "\x00"
  "IdentifyQuery" "\x00"
  "AddGroup" "\x00"
  "xxxx00" "\x00"
  "ViewGroup" "\x00"
  "GetGroup" "\x00"
  "01xxxx" "\x00"
  "GetAllGroups" "\x00"
  "00" "\x00"
  "RemoveGroup" "\x00"
  "RemoveAllGroups" "\x00"
  "ViewScene" "\x00"
  "xxxxyy" "\x00"
  "RemoveScene" "\x00"
  "RemoveAllScenes" "\x00"
  "RecallScene" "\x00"
  "GetSceneMembership" "\x00"
  "PowerOffEffect" "\x00"
  "xxyy" "\x00"
  "PowerOnRecall" "\x00"
  "PowerOnTimer" "\x00"
  "xxyyyyzzzz" "\x00"
  "xx0A00" "\x00"
  "DimmerUp" "\x00"
  "00190200" "\x00"
  "DimmerDown" "\x00"
  "01190200" "\x00"
  "DimmerStop" "\x00"
  "ResetAlarm" "\x00"
  "xxyyyy" "\x00"
  "ResetAllAlarms" "\x00"
  "xx000A00" "\x00"
  "HueSat" "\x00"
  "xxyy0A00" "\x00"
  "Color" "\x00"
  "xxxxyyyy0A00" "\x00"
  "xxxx0A00" "\x00"
  "ShutterOpen" "\x00"
  "ShutterClose" "\x00"
  "ShutterStop" "\x00"
  "ShutterLift" "\x00"
  "xx" "\x00"
  "ShutterTilt" "\x00"
  "Shutter" "\x00"
  "DimmerMove" "\x00"
  "xx0A" "\x00"
  "DimmerStepUp" "\x00"
  "00xx0A00" "\x00"
  "DimmerStepDown" "\x00"
  "01xx0A00" "\x00"
  "DimmerStep" "\x00"
  "xx190A00" "\x00"
  "01" "\x00"
  "HueMove" "\x00"
  "xx19" "\x00"
  "HueStepUp" "\x00"
  "HueStepDown" "\x00"
  "03xx0A00" "\x00"
  "HueStep" "\x00"
  "SatMove" "\x00"
  "SatStep" "\x00"
  "xx190A" "\x00"
  "ColorMove" "\x00"
  "xxxxyyyy" "\x00"
  "ColorStep" "\x00"
  "ColorTempMoveUp" "\x00"
  "01xxxx000000000000" "\x00"
  "ColorTempMoveDown" "\x00"
  "03xxxx000000000000" "\x00"
  "ColorTempMoveStop" "\x00"
  "00xxxx000000000000" "\x00"
  "ColorTempMove" "\x00"
  "xxyyyy000000000000" "\x00"
  "ColorTempStepUp" "\x00"
  "01xxxx0A0000000000" "\x00"
  "ColorTempStepDown" "\x00"
  "03xxxx0A0000000000" "\x00"
  "ColorTempStep" "\x00"
  "xxyyyy0A0000000000" "\x00"
  "ArrowClick" "\x00"
  "ArrowHold" "\x00"
  "ArrowRelease" "\x00"
  "ZoneStatusChange" "\x00"
  "xxxxyyzz" "\x00"
  "xxyyzzzz" "\x00"
  "AddScene" "\x00"
  "xxyyyyzz" "\x00"
  "StoreScene" "\x00"
  ;


enum Z_offsets {
  Zo_ = 0,
  Zo_ZCLVersion = 1,
  Zo_AppVersion = 12,
  Zo_StackVersion = 23,
  Zo_HWVersion = 36,
  Zo_Manufacturer = 46,
  Zo_ModelId = 59,
  Zo_DateCode = 67,
  Zo_PowerSource = 76,
  Zo_GenericDeviceClass = 88,
  Zo_GenericDeviceType = 107,
  Zo_ProductCode = 125,
  Zo_ProductURL = 137,
  Zo_SWBuildID = 148,
  Zo_MainsVoltage = 158,
  Zo_MainsFrequency = 171,
  Zo_BatteryVoltage = 186,
  Zo_BatteryPercentage = 201,
  Zo_CurrentTemperature = 219,
  Zo_MinTempExperienced = 238,
  Zo_MaxTempExperienced = 257,
  Zo_OverTempTotalDwell = 276,
  Zo_IdentifyTime = 295,
  Zo_GroupNameSupport = 308,
  Zo_SceneCount = 325,
  Zo_CurrentScene = 336,
  Zo_CurrentGroup = 349,
  Zo_SceneValid = 362,
  Zo_NameSupport = 373,
  Zo_Power = 385,
  Zo_StartUpOnOff = 391,
  Zo_SwitchType = 404,
  Zo_Dimmer = 415,
  Zo_DimmerOptions = 422,
  Zo_DimmerRemainingTime = 436,
  Zo_OnOffTransitionTime = 456,
  Zo_OnLevel = 476,
  Zo_OnTransitionTime = 484,
  Zo_OffTransitionTime = 501,
  Zo_DefaultMoveRate = 519,
  Zo_AlarmCount = 535,
  Zo_Time = 546,
  Zo_TimeStatus = 551,
  Zo_TimeZone = 562,
  Zo_DstStart = 571,
  Zo_DstEnd = 580,
  Zo_DstShift = 587,
  Zo_StandardTime = 596,
  Zo_LocalTime = 609,
  Zo_LastSetTime = 619,
  Zo_ValidUntilTime = 631,
  Zo_TimeEpoch = 646,
  Zo_LocationType = 656,
  Zo_LocationMethod = 669,
  Zo_LocationAge = 684,
  Zo_QualityMeasure = 696,
  Zo_NumberOfDevices = 711,
  Zo_AnalogInActiveText = 727,
  Zo_AnalogInDescription = 746,
  Zo_AnalogInInactiveText = 766,
  Zo_AnalogInMaxValue = 787,
  Zo_AnalogInMinValue = 804,
  Zo_AnalogInOutOfService = 821,
  Zo_AqaraRotate = 842,
  Zo_AnalogInPriorityArray = 854,
  Zo_AnalogInReliability = 876,
  Zo_AnalogInRelinquishDefault = 896,
  Zo_AnalogInResolution = 922,
  Zo_AnalogInStatusFlags = 941,
  Zo_AnalogInEngineeringUnits = 961,
  Zo_AnalogInApplicationType = 986,
  Zo_Aqara_FF05 = 1010,
  Zo_AnalogOutDescription = 1021,
  Zo_AnalogOutMaxValue = 1042,
  Zo_AnalogOutMinValue = 1060,
  Zo_AnalogOutOutOfService = 1078,
  Zo_AnalogOutValue = 1100,
  Zo_AnalogOutPriorityArray = 1115,
  Zo_AnalogOutReliability = 1138,
  Zo_AnalogOutRelinquishDefault = 1159,
  Zo_AnalogOutResolution = 1186,
  Zo_AnalogOutStatusFlags = 1206,
  Zo_AnalogOutEngineeringUnits = 1227,
  Zo_AnalogOutApplicationType = 1253,
  Zo_AnalogDescription = 1278,
  Zo_AnalogOutOfService = 1296,
  Zo_AnalogValue = 1315,
  Zo_AnalogPriorityArray = 1327,
  Zo_AnalogReliability = 1347,
  Zo_AnalogRelinquishDefault = 1365,
  Zo_AnalogStatusFlags = 1389,
  Zo_AnalogEngineeringUnits = 1407,
  Zo_AnalogApplicationType = 1430,
  Zo_BinaryInActiveText = 1452,
  Zo_BinaryInDescription = 1471,
  Zo_BinaryInInactiveText = 1491,
  Zo_BinaryInOutOfService = 1512,
  Zo_BinaryInPolarity = 1533,
  Zo_BinaryInValue = 1550,
  Zo_BinaryInPriorityArray = 1564,
  Zo_BinaryInReliability = 1586,
  Zo_BinaryInStatusFlags = 1606,
  Zo_BinaryInApplicationType = 1626,
  Zo_BinaryOutActiveText = 1650,
  Zo_BinaryOutDescription = 1670,
  Zo_BinaryOutInactiveText = 1691,
  Zo_BinaryOutMinimumOffTime = 1713,
  Zo_BinaryOutMinimumOnTime = 1737,
  Zo_BinaryOutOutOfService = 1760,
  Zo_BinaryOutPolarity = 1782,
  Zo_BinaryOutValue = 1800,
  Zo_BinaryOutPriorityArray = 1815,
  Zo_BinaryOutReliability = 1838,
  Zo_BinaryOutRelinquishDefault = 1859,
  Zo_BinaryOutStatusFlags = 1886,
  Zo_BinaryOutApplicationType = 1907,
  Zo_BinaryActiveText = 1932,
  Zo_BinaryDescription = 1949,
  Zo_BinaryInactiveText = 1967,
  Zo_BinaryMinimumOffTime = 1986,
  Zo_BinaryMinimumOnTime = 2007,
  Zo_BinaryOutOfService = 2027,
  Zo_BinaryValue = 2046,
  Zo_BinaryPriorityArray = 2058,
  Zo_BinaryReliability = 2078,
  Zo_BinaryRelinquishDefault = 2096,
  Zo_BinaryStatusFlags = 2120,
  Zo_BinaryApplicationType = 2138,
  Zo_MultiInStateText = 2160,
  Zo_MultiInDescription = 2177,
  Zo_MultiInNumberOfStates = 2196,
  Zo_MultiInOutOfService = 2218,
  Zo_MultiInValue = 2238,
  Zo_MultiInReliability = 2251,
  Zo_MultiInStatusFlags = 2270,
  Zo_MultiInApplicationType = 2289,
  Zo_MultiOutStateText = 2312,
  Zo_MultiOutDescription = 2330,
  Zo_MultiOutNumberOfStates = 2350,
  Zo_MultiOutOutOfService = 2373,
  Zo_MultiOutValue = 2394,
  Zo_MultiOutPriorityArray = 2408,
  Zo_MultiOutReliability = 2430,
  Zo_MultiOutRelinquishDefault = 2450,
  Zo_MultiOutStatusFlags = 2476,
  Zo_MultiOutApplicationType = 2496,
  Zo_MultiStateText = 2520,
  Zo_MultiDescription = 2535,
  Zo_MultiNumberOfStates = 2552,
  Zo_MultiOutOfService = 2572,
  Zo_MultiValue = 2590,
  Zo_MultiReliability = 2601,
  Zo_MultiRelinquishDefault = 2618,
  Zo_MultiStatusFlags = 2641,
  Zo_MultiApplicationType = 2658,
  Zo_TotalProfileNum = 2679,
  Zo_MultipleScheduling = 2695,
  Zo_EnergyFormatting = 2714,
  Zo_EnergyRemote = 2731,
  Zo_ScheduleMode = 2744,
  Zo_CheckinInterval = 2757,
  Zo_LongPollInterval = 2773,
  Zo_ShortPollInterval = 2790,
  Zo_FastPollTimeout = 2808,
  Zo_CheckinIntervalMin = 2824,
  Zo_LongPollIntervalMin = 2843,
  Zo_FastPollTimeoutMax = 2863,
  Zo_PhysicalClosedLimit = 2882,
  Zo_MotorStepSize = 2902,
  Zo_Status = 2916,
  Zo_ClosedLimit = 2923,
  Zo_Mode = 2935,
  Zo_LockState = 2940,
  Zo_LockType = 2950,
  Zo_ActuatorEnabled = 2959,
  Zo_DoorState = 2975,
  Zo_DoorOpenEvents = 2985,
  Zo_DoorClosedEvents = 3000,
  Zo_OpenPeriod = 3017,
  Zo_AqaraVibrationMode = 3028,
  Zo_AqaraVibrationsOrAngle = 3047,
  Zo_AqaraVibration505 = 3070,
  Zo_AqaraAccelerometer = 3088,
  Zo_WindowCoveringType = 3107,
  Zo_PhysicalClosedLimitLift = 3126,
  Zo_PhysicalClosedLimitTilt = 3150,
  Zo_CurrentPositionLift = 3174,
  Zo_CurrentPositionTilt = 3194,
  Zo_NumberofActuationsLift = 3214,
  Zo_NumberofActuationsTilt = 3237,
  Zo_ConfigStatus = 3260,
  Zo_CurrentPositionLiftPercentage = 3273,
  Zo_CurrentPositionTiltPercentage = 3303,
  Zo_InstalledOpenLimitLift = 3333,
  Zo_InstalledClosedLimitLift = 3356,
  Zo_InstalledOpenLimitTilt = 3381,
  Zo_InstalledClosedLimitTilt = 3404,
  Zo_VelocityLift = 3429,
  Zo_AccelerationTimeLift = 3442,
  Zo_DecelerationTimeLift = 3463,
  Zo_IntermediateSetpointsLift = 3484,
  Zo_IntermediateSetpointsTilt = 3510,
  Zo_Hue = 3536,
  Zo_Sat = 3540,
  Zo_RemainingTime = 3544,
  Zo_X = 3558,
  Zo_Y = 3560,
  Zo_DriftCompensation = 3562,
  Zo_CompensationText = 3580,
  Zo_CT = 3597,
  Zo_ColorMode = 3600,
  Zo_NumberOfPrimaries = 3610,
  Zo_Primary1X = 3628,
  Zo_Primary1Y = 3638,
  Zo_Primary1Intensity = 3648,
  Zo_Primary2X = 3666,
  Zo_Primary2Y = 3676,
  Zo_Primary2Intensity = 3686,
  Zo_Primary3X = 3704,
  Zo_Primary3Y = 3714,
  Zo_Primary3Intensity = 3724,
  Zo_WhitePointX = 3742,
  Zo_WhitePointY = 3754,
  Zo_ColorPointRX = 3766,
  Zo_ColorPointRY = 3779,
  Zo_ColorPointRIntensity = 3792,
  Zo_ColorPointGX = 3813,
  Zo_ColorPointGY = 3826,
  Zo_ColorPointGIntensity = 3839,
  Zo_ColorPointBX = 3860,
  Zo_ColorPointBY = 3873,
  Zo_ColorPointBIntensity = 3886,
  Zo_Illuminance = 3907,
  Zo_IlluminanceMinMeasuredValue = 3919,
  Zo_IlluminanceMaxMeasuredValue = 3947,
  Zo_IlluminanceTolerance = 3975,
  Zo_IlluminanceLightSensorType = 3996,
  Zo_IlluminanceLevelStatus = 4023,
  Zo_IlluminanceTargetLevel = 4046,
  Zo_Temperature = 4069,
  Zo_TemperatureMinMeasuredValue = 4081,
  Zo_TemperatureMaxMeasuredValue = 4109,
  Zo_TemperatureTolerance = 4137,
  Zo_PressureUnit = 4158,
  Zo_Pressure = 4171,
  Zo_PressureMinMeasuredValue = 4180,
  Zo_PressureMaxMeasuredValue = 4205,
  Zo_PressureTolerance = 4230,
  Zo_PressureScaledValue = 4248,
  Zo_PressureMinScaledValue = 4268,
  Zo_PressureMaxScaledValue = 4291,
  Zo_PressureScaledTolerance = 4314,
  Zo_PressureScale = 4338,
  Zo_FlowRate = 4352,
  Zo_FlowMinMeasuredValue = 4361,
  Zo_FlowMaxMeasuredValue = 4382,
  Zo_FlowTolerance = 4403,
  Zo_Humidity = 4417,
  Zo_HumidityMinMeasuredValue = 4426,
  Zo_HumidityMaxMeasuredValue = 4451,
  Zo_HumidityTolerance = 4476,
  Zo_Occupancy = 4494,
  Zo_OccupancySensorType = 4504,
  Zo_ZoneState = 4524,
  Zo_ZoneType = 4534,
  Zo_ZoneStatus = 4543,
  Zo_CompanyName = 4554,
  Zo_MeterTypeID = 4566,
  Zo_DataQualityID = 4578,
  Zo_CustomerName = 4592,
  Zo_Model = 4605,
  Zo_PartNumber = 4611,
  Zo_ProductRevision = 4622,
  Zo_SoftwareRevision = 4638,
  Zo_UtilityName = 4655,
  Zo_POD = 4667,
  Zo_AvailablePower = 4671,
  Zo_PowerThreshold = 4686,
  Zo_NumberOfResets = 4701,
  Zo_PersistentMemoryWrites = 4716,
  Zo_LastMessageLQI = 4739,
  Zo_LastMessageRSSI = 4754,
  Zo_Identify = 4770,
  Zo_xxxx = 4779,
  Zo_IdentifyQuery = 4784,
  Zo_AddGroup = 4798,
  Zo_xxxx00 = 4807,
  Zo_ViewGroup = 4814,
  Zo_GetGroup = 4824,
  Zo_01xxxx = 4833,
  Zo_GetAllGroups = 4840,
  Zo_00 = 4853,
  Zo_RemoveGroup = 4856,
  Zo_RemoveAllGroups = 4868,
  Zo_ViewScene = 4884,
  Zo_xxxxyy = 4894,
  Zo_RemoveScene = 4901,
  Zo_RemoveAllScenes = 4913,
  Zo_RecallScene = 4929,
  Zo_GetSceneMembership = 4941,
  Zo_PowerOffEffect = 4960,
  Zo_xxyy = 4975,
  Zo_PowerOnRecall = 4980,
  Zo_PowerOnTimer = 4994,
  Zo_xxyyyyzzzz = 5007,
  Zo_xx0A00 = 5018,
  Zo_DimmerUp = 5025,
  Zo_00190200 = 5034,
  Zo_DimmerDown = 5043,
  Zo_01190200 = 5054,
  Zo_DimmerStop = 5063,
  Zo_ResetAlarm = 5074,
  Zo_xxyyyy = 5085,
  Zo_ResetAllAlarms = 5092,
  Zo_xx000A00 = 5107,
  Zo_HueSat = 5116,
  Zo_xxyy0A00 = 5123,
  Zo_Color = 5132,
  Zo_xxxxyyyy0A00 = 5138,
  Zo_xxxx0A00 = 5151,
  Zo_ShutterOpen = 5160,
  Zo_ShutterClose = 5172,
  Zo_ShutterStop = 5185,
  Zo_ShutterLift = 5197,
  Zo_xx = 5209,
  Zo_ShutterTilt = 5212,
  Zo_Shutter = 5224,
  Zo_DimmerMove = 5232,
  Zo_xx0A = 5243,
  Zo_DimmerStepUp = 5248,
  Zo_00xx0A00 = 5261,
  Zo_DimmerStepDown = 5270,
  Zo_01xx0A00 = 5285,
  Zo_DimmerStep = 5294,
  Zo_xx190A00 = 5305,
  Zo_01 = 5314,
  Zo_HueMove = 5317,
  Zo_xx19 = 5325,
  Zo_HueStepUp = 5330,
  Zo_HueStepDown = 5340,
  Zo_03xx0A00 = 5352,
  Zo_HueStep = 5361,
  Zo_SatMove = 5369,
  Zo_SatStep = 5377,
  Zo_xx190A = 5385,
  Zo_ColorMove = 5392,
  Zo_xxxxyyyy = 5402,
  Zo_ColorStep = 5411,
  Zo_ColorTempMoveUp = 5421,
  Zo_01xxxx000000000000 = 5437,
  Zo_ColorTempMoveDown = 5456,
  Zo_03xxxx000000000000 = 5474,
  Zo_ColorTempMoveStop = 5493,
  Zo_00xxxx000000000000 = 5511,
  Zo_ColorTempMove = 5530,
  Zo_xxyyyy000000000000 = 5544,
  Zo_ColorTempStepUp = 5563,
  Zo_01xxxx0A0000000000 = 5579,
  Zo_ColorTempStepDown = 5598,
  Zo_03xxxx0A0000000000 = 5616,
  Zo_ColorTempStep = 5635,
  Zo_xxyyyy0A0000000000 = 5649,
  Zo_ArrowClick = 5668,
  Zo_ArrowHold = 5679,
  Zo_ArrowRelease = 5689,
  Zo_ZoneStatusChange = 5702,
  Zo_xxxxyyzz = 5719,
  Zo_xxyyzzzz = 5728,
  Zo_AddScene = 5737,
  Zo_xxyyyyzz = 5746,
  Zo_StoreScene = 5755,
};


/*
  DO NOT EDIT
*/


#endif // USE_ZIGBEE
