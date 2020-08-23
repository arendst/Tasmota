/*
  xdrv_23_zigbee_converters.ino - zigbee support for Tasmota

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

/*********************************************************************************************\
 * ZCL
\*********************************************************************************************/


enum Z_DataTypes {
  Znodata   = 0x00,
  Zdata8    = 0x08, Zdata16, Zdata24, Zdata32, Zdata40, Zdata48, Zdata56, Zdata64,
  Zbool     = 0x10,
  Zmap8     = 0x18, Zmap16, Zmap24, Zmap32, Zmap40, Zmap48, Zmap56, Zmap64,
  Zuint8    = 0x20, Zuint16, Zuint24, Zuint32, Zuint40, Zuint48, Zuint56, Zuint64,
  Zint8     = 0x28, Zint16, Zint24, Zint32, Zint40, Zint48, Zint56, Zint64,
  Zenum8    = 0x30, Zenum16 = 0x31,
  Zsemi     = 0x38, Zsingle = 0x39, Zdouble = 0x3A,
  Zoctstr   = 0x41, Zstring = 0x42, Zoctstr16 = 0x43, Zstring16 = 0x44,
  Arrray    = 0x48,
  Zstruct   = 0x4C,
  Zset      = 0x50, Zbag = 0x51,
  ZToD      = 0xE0, Zdate = 0xE1, ZUTC = 0xE2,
  ZclusterId = 0xE8, ZattribId = 0xE9, ZbacOID = 0xEA,
  ZEUI64    = 0xF0, Zkey128 = 0xF1,
  Zunk      = 0xFF
};

//
// get the lenth in bytes for a data-type
// return 0 if unknown of type specific
//
// Note: this code is smaller than a static array
uint8_t Z_getDatatypeLen(uint8_t t) {
  if ( ((t >= 0x08) && (t <= 0x0F)) ||      // data8 - data64
       ((t >= 0x18) && (t <= 0x2F)) ) {     // map/uint/int
    return (t & 0x07) + 1;
  }
  switch (t) {
    case Zbool:
    case Zenum8:
      return 1;
    case Zenum16:
    case Zsemi:
    case ZclusterId:
    case ZattribId:
      return 2;
    case Zsingle:
    case ZToD:
    case Zdate:
    case ZUTC:
    case ZbacOID:
      return 4;
    case Zdouble:
    case ZEUI64:
      return 8;
    case Zkey128:
      return 16;
    case Znodata:
    default:
      return 0;
  }
}

// is the type a discrete type, cf. section 2.6.2 of ZCL spec
bool Z_isDiscreteDataType(uint8_t t) {
  if ( ((t >= 0x20) && (t <= 0x2F)) ||      // uint8 - int64
       ((t >= 0x38) && (t <= 0x3A)) ||      // semi - double
       ((t >= 0xE0) && (t <= 0xE2))  ) {    // ToD - UTC
    return false;
  } else {
    return true;
  }
}

// return value:
// 0 = keep initial value
// 1 = remove initial value
typedef int32_t (*Z_AttrConverter)(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr);
typedef struct Z_AttributeConverter {
  uint8_t  type;
  uint8_t  cluster_short;
  uint16_t attribute;
  uint16_t name_offset;
  int8_t   multiplier;     // multiplier for numerical value, (if > 0 multiply by x, if <0 device by x)
  uint8_t  cb;            // callback func from Z_ConvOperators
  // Z_AttrConverter func;
} Z_AttributeConverter;

// Cluster numbers are store in 8 bits format to save space,
// the following tables allows the conversion from 8 bits index Cx...
// to the 16 bits actual cluster number
enum Cx_cluster_short {
  Cx0000, Cx0001, Cx0002, Cx0003, Cx0004, Cx0005, Cx0006, Cx0007,
  Cx0008, Cx0009, Cx000A, Cx000B, Cx000C, Cx000D, Cx000E, Cx000F,
  Cx0010, Cx0011, Cx0012, Cx0013, Cx0014, Cx001A, Cx0020, Cx0100,
  Cx0101, Cx0102, Cx0300, Cx0400, Cx0401, Cx0402, Cx0403, Cx0404,
  Cx0405, Cx0406, Cx0500, Cx0B01, Cx0B05,
};

const uint16_t Cx_cluster[] PROGMEM = {
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x001A, 0x0020, 0x0100,
  0x0101, 0x0102, 0x0300, 0x0400, 0x0401, 0x0402, 0x0403, 0x0404,
  0x0405, 0x0406, 0x0500, 0x0B01, 0x0B05,
};

uint16_t CxToCluster(uint8_t cx) {
  if (cx < ARRAY_SIZE(Cx_cluster)) {
    return pgm_read_word(&Cx_cluster[cx]);
  }
  return 0xFFFF;
}

enum Z_ConvOperators {
  Z_Nop,                // copy value
  Z_AddPressureUnit,    // add pressure unit attribute (non numerical)
  Z_ManufKeep,          // copy and record Manufacturer attribute
  Z_ModelKeep,          // copy and record ModelId attribute
  Z_AqaraSensor,        // decode prioprietary Aqara Sensor message
  Z_AqaraVibration,     // decode Aqara vibration modes
  Z_AqaraCube,          // decode Aqara cube
  Z_AqaraButton,          // decode Aqara button
  Z_BatteryPercentage,  // memorize Battery Percentage in RAM
};

// list of post-processing directives
const Z_AttributeConverter Z_PostProcess[] PROGMEM = {
  { Zuint8,   Cx0000, 0x0000,  Z_(ZCLVersion),           1,  Z_Nop },
  { Zuint8,   Cx0000, 0x0001,  Z_(AppVersion),           1,  Z_Nop },
  { Zuint8,   Cx0000, 0x0002,  Z_(StackVersion),         1,  Z_Nop },
  { Zuint8,   Cx0000, 0x0003,  Z_(HWVersion),            1,  Z_Nop },
  { Zstring,  Cx0000, 0x0004,  Z_(Manufacturer),         1,  Z_ManufKeep },    // record Manufacturer
  { Zstring,  Cx0000, 0x0005,  Z_(ModelId),              1,  Z_ModelKeep },    // record Model
  { Zstring,  Cx0000, 0x0006,  Z_(DateCode),             1,  Z_Nop },
  { Zenum8,   Cx0000, 0x0007,  Z_(PowerSource),          1,  Z_Nop },
  { Zenum8,   Cx0000, 0x0008,  Z_(GenericDeviceClass),   1,  Z_Nop },
  { Zenum8,   Cx0000, 0x0009,  Z_(GenericDeviceType),    1,  Z_Nop },
  { Zoctstr,  Cx0000, 0x000A,  Z_(ProductCode),          1,  Z_Nop },
  { Zstring,  Cx0000, 0x000B,  Z_(ProductURL),           1,  Z_Nop },
  { Zstring,  Cx0000, 0x4000,  Z_(SWBuildID),            1,  Z_Nop },
  // { Zunk,     Cx0000, 0xFFFF,  nullptr,                 0,  Z_Nop },    // Remove all other values
  // Cmd 0x0A - Cluster 0x0000, attribute 0xFF01 - proprietary
  { Zmap8,    Cx0000, 0xFF01,  Z_(),                     0,  Z_AqaraSensor },    // Occupancy (map8)

  // Power Configuration cluster
  { Zuint16,  Cx0001, 0x0000,  Z_(MainsVoltage),         1,  Z_Nop },
  { Zuint8,   Cx0001, 0x0001,  Z_(MainsFrequency),       1,  Z_Nop },
  { Zuint8,   Cx0001, 0x0020,  Z_(BatteryVoltage),       -10,Z_Nop },   // divide by 10
  { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    -2, Z_BatteryPercentage },   // divide by 2

  // Device Temperature Configuration cluster
  { Zint16,   Cx0002, 0x0000,  Z_(CurrentTemperature),   1,  Z_Nop },
  { Zint16,   Cx0002, 0x0001,  Z_(MinTempExperienced),   1,  Z_Nop },
  { Zint16,   Cx0002, 0x0002,  Z_(MaxTempExperienced),   1,  Z_Nop },
  { Zuint16,  Cx0002, 0x0003,  Z_(OverTempTotalDwell),   1,  Z_Nop },

  // Identify cluster
  { Zuint16,  Cx0003, 0x0000,  Z_(IdentifyTime),         1,  Z_Nop },

  // Groups cluster
  { Zmap8,    Cx0004, 0x0000,  Z_(GroupNameSupport),     1,  Z_Nop },

  // Scenes cluster
  { Zuint8,   Cx0005, 0x0000,  Z_(SceneCount),           1,  Z_Nop },
  { Zuint8,   Cx0005, 0x0001,  Z_(CurrentScene),         1,  Z_Nop },
  { Zuint16,  Cx0005, 0x0002,  Z_(CurrentGroup),         1,  Z_Nop },
  { Zbool,    Cx0005, 0x0003,  Z_(SceneValid),           1,  Z_Nop },
  //{ Zmap8,    Cx0005, 0x0004,  Z_(NameSupport),           1,  Z_Nop },

  // On/off cluster
  { Zbool,    Cx0006,    0x0000,  Z_(Power),             1,  Z_Nop },
  { Zenum8,   Cx0006,    0x4003,  Z_(StartUpOnOff),      1,  Z_Nop },
  { Zbool,    Cx0006,    0x8000,  Z_(Power),             1,  Z_Nop },   // See 7280

  // On/Off Switch Configuration cluster
  { Zenum8,   Cx0007, 0x0000,  Z_(SwitchType),           1,  Z_Nop },

  // Level Control cluster
  { Zuint8,   Cx0008, 0x0000,  Z_(Dimmer),               1,  Z_Nop },
  { Zmap8,    Cx0008, 0x000F,  Z_(DimmerOptions),        1,  Z_Nop },
  { Zuint16,  Cx0008, 0x0001,  Z_(DimmerRemainingTime),  1,  Z_Nop },
  { Zuint16,  Cx0008, 0x0010,  Z_(OnOffTransitionTime),   1,  Z_Nop },
  // { Zuint8, Cx0008, 0x0011,  Z_(OnLevel),              1,  Z_Nop },
  // { Zuint16, Cx0008, 0x0012,  Z_(OnTransitionTime),     1,  Z_Nop },
  // { Zuint16, Cx0008, 0x0013,  Z_(OffTransitionTime),    1,  Z_Nop },
  // { Zuint16, Cx0008, 0x0014,  Z_(DefaultMoveRate),      1,  Z_Nop },

  // Alarms cluster
  { Zuint16,  Cx0009, 0x0000,  Z_(AlarmCount),           1,  Z_Nop },

  // Time cluster
  { ZUTC,     Cx000A, 0x0000,  Z_(Time),                 1,  Z_Nop },
  { Zmap8,    Cx000A, 0x0001,  Z_(TimeStatus),           1,  Z_Nop },
  { Zint32,   Cx000A, 0x0002,  Z_(TimeZone),             1,  Z_Nop },
  { Zuint32,  Cx000A, 0x0003,  Z_(DstStart),             1,  Z_Nop },
  { Zuint32,  Cx000A, 0x0004,  Z_(DstEnd),               1,  Z_Nop },
  { Zint32,   Cx000A, 0x0005,  Z_(DstShift),             1,  Z_Nop },
  { Zuint32,  Cx000A, 0x0006,  Z_(StandardTime),         1,  Z_Nop },
  { Zuint32,  Cx000A, 0x0007,  Z_(LocalTime),            1,  Z_Nop },
  { ZUTC,     Cx000A, 0x0008,  Z_(LastSetTime),          1,  Z_Nop },
  { ZUTC,     Cx000A, 0x0009,  Z_(ValidUntilTime),       1,  Z_Nop },
  { ZUTC,     Cx000A, 0xFF00,  Z_(TimeEpoch),            1,  Z_Nop },    // Tasmota specific, epoch

  // RSSI Location cluster
  { Zdata8,   Cx000B, 0x0000,  Z_(LocationType),         1,  Z_Nop },
  { Zenum8,   Cx000B, 0x0001,  Z_(LocationMethod),       1,  Z_Nop },
  { Zuint16,  Cx000B, 0x0002,  Z_(LocationAge),          1,  Z_Nop },
  { Zuint8,   Cx000B, 0x0003,  Z_(QualityMeasure),       1,  Z_Nop },
  { Zuint8,   Cx000B, 0x0004,  Z_(NumberOfDevices),      1,  Z_Nop },

  // Analog Input cluster
  // { 0xFF, Cx000C, 0x0004,  Z_(AnalogInActiveText),   1,  Z_Nop },
  { Zstring,  Cx000C, 0x001C,  Z_(AnalogInDescription),  1,  Z_Nop },
  // { 0xFF, Cx000C, 0x002E,  Z_(AnalogInInactiveText), 1,  Z_Nop },
  { Zsingle,  Cx000C, 0x0041,  Z_(AnalogInMaxValue),     1,  Z_Nop },
  { Zsingle,  Cx000C, 0x0045,  Z_(AnalogInMinValue),     1,  Z_Nop },
  { Zbool,    Cx000C, 0x0051,  Z_(AnalogInOutOfService), 1,  Z_Nop },
  { Zsingle,  Cx000C, 0x0055,  Z_(AqaraRotate),          1,  Z_Nop },
  // { 0xFF, Cx000C, 0x0057,  Z_(AnalogInPriorityArray),1,  Z_Nop },
  { Zenum8,   Cx000C, 0x0067,  Z_(AnalogInReliability),  1,  Z_Nop },
  // { 0xFF, Cx000C, 0x0068,  Z_(AnalogInRelinquishDefault),1,  Z_Nop },
  { Zsingle,  Cx000C, 0x006A,  Z_(AnalogInResolution),   1,  Z_Nop },
  { Zmap8,    Cx000C, 0x006F,  Z_(AnalogInStatusFlags),  1,  Z_Nop },
  { Zenum16,  Cx000C, 0x0075,  Z_(AnalogInEngineeringUnits),1,  Z_Nop },
  { Zuint32,  Cx000C, 0x0100,  Z_(AnalogInApplicationType),1,  Z_Nop },
  { Zuint16,  Cx000C, 0xFF05,  Z_(Aqara_FF05),           1,  Z_Nop },

  // Analog Output cluster
  { Zstring,  Cx000D, 0x001C,  Z_(AnalogOutDescription), 1,  Z_Nop },
  { Zsingle,  Cx000D, 0x0041,  Z_(AnalogOutMaxValue),    1,  Z_Nop },
  { Zsingle,  Cx000D, 0x0045,  Z_(AnalogOutMinValue),    1,  Z_Nop },
  { Zbool,    Cx000D, 0x0051,  Z_(AnalogOutOutOfService),1,  Z_Nop },
  { Zsingle,  Cx000D, 0x0055,  Z_(AnalogOutValue),       1,  Z_Nop },
  // { Zunk,     Cx000D, 0x0057,  Z_(AnalogOutPriorityArray),1,  Z_Nop },
  { Zenum8,   Cx000D, 0x0067,  Z_(AnalogOutReliability), 1,  Z_Nop },
  { Zsingle,  Cx000D, 0x0068,  Z_(AnalogOutRelinquishDefault),1,  Z_Nop },
  { Zsingle,  Cx000D, 0x006A,  Z_(AnalogOutResolution),  1,  Z_Nop },
  { Zmap8,    Cx000D, 0x006F,  Z_(AnalogOutStatusFlags), 1,  Z_Nop },
  { Zenum16,  Cx000D, 0x0075,  Z_(AnalogOutEngineeringUnits),1,  Z_Nop },
  { Zuint32,  Cx000D, 0x0100,  Z_(AnalogOutApplicationType),1,  Z_Nop },

  // Analog Value cluster
  { Zstring,  Cx000E, 0x001C,  Z_(AnalogDescription),    1,  Z_Nop },
  { Zbool,    Cx000E, 0x0051,  Z_(AnalogOutOfService),   1,  Z_Nop },
  { Zsingle,  Cx000E, 0x0055,  Z_(AnalogValue),          1,  Z_Nop },
  { Zunk,     Cx000E, 0x0057,  Z_(AnalogPriorityArray),  1,  Z_Nop },
  { Zenum8,   Cx000E, 0x0067,  Z_(AnalogReliability),    1,  Z_Nop },
  { Zsingle,  Cx000E, 0x0068,  Z_(AnalogRelinquishDefault),1,  Z_Nop },
  { Zmap8,    Cx000E, 0x006F,  Z_(AnalogStatusFlags),    1,  Z_Nop },
  { Zenum16,  Cx000E, 0x0075,  Z_(AnalogEngineeringUnits),1,  Z_Nop },
  { Zuint32,  Cx000E, 0x0100,  Z_(AnalogApplicationType),1,  Z_Nop },

  // Binary Input cluster
  { Zstring,  Cx000F, 0x0004,  Z_(BinaryInActiveText),  1,  Z_Nop },
  { Zstring,  Cx000F, 0x001C,  Z_(BinaryInDescription), 1,  Z_Nop },
  { Zstring,  Cx000F, 0x002E,  Z_(BinaryInInactiveText),1,  Z_Nop },
  { Zbool,    Cx000F, 0x0051,  Z_(BinaryInOutOfService),1,  Z_Nop },
  { Zenum8,   Cx000F, 0x0054,  Z_(BinaryInPolarity),    1,  Z_Nop },
  { Zstring,  Cx000F, 0x0055,  Z_(BinaryInValue),       1,  Z_Nop },
  // { 0xFF, Cx000F, 0x0057,  Z_(BinaryInPriorityArray),1,  Z_Nop },
  { Zenum8,   Cx000F, 0x0067,  Z_(BinaryInReliability), 1,  Z_Nop },
  { Zmap8,    Cx000F, 0x006F,  Z_(BinaryInStatusFlags), 1,  Z_Nop },
  { Zuint32,  Cx000F, 0x0100,  Z_(BinaryInApplicationType),1,  Z_Nop },

  // Binary Output cluster
  { Zstring,  Cx0010, 0x0004,  Z_(BinaryOutActiveText),  1,  Z_Nop },
  { Zstring,  Cx0010, 0x001C,  Z_(BinaryOutDescription), 1,  Z_Nop },
  { Zstring,  Cx0010, 0x002E,  Z_(BinaryOutInactiveText),1,  Z_Nop },
  { Zuint32,  Cx0010, 0x0042,  Z_(BinaryOutMinimumOffTime),1,  Z_Nop },
  { Zuint32,  Cx0010, 0x0043,  Z_(BinaryOutMinimumOnTime),1,  Z_Nop },
  { Zbool,    Cx0010, 0x0051,  Z_(BinaryOutOutOfService),1,  Z_Nop },
  { Zenum8,   Cx0010, 0x0054,  Z_(BinaryOutPolarity),    1,  Z_Nop },
  { Zbool,    Cx0010, 0x0055,  Z_(BinaryOutValue),       1,  Z_Nop },
  // { Zunk,     Cx0010, 0x0057,  Z_(BinaryOutPriorityArray),1,  Z_Nop },
  { Zenum8,   Cx0010, 0x0067,  Z_(BinaryOutReliability), 1,  Z_Nop },
  { Zbool,    Cx0010, 0x0068,  Z_(BinaryOutRelinquishDefault),1,  Z_Nop },
  { Zmap8,    Cx0010, 0x006F,  Z_(BinaryOutStatusFlags), 1,  Z_Nop },
  { Zuint32,  Cx0010, 0x0100,  Z_(BinaryOutApplicationType),1,  Z_Nop },

  // Binary Value cluster
  { Zstring,  Cx0011, 0x0004,  Z_(BinaryActiveText),     1,  Z_Nop },
  { Zstring,  Cx0011, 0x001C,  Z_(BinaryDescription),    1,  Z_Nop },
  { Zstring,  Cx0011, 0x002E,  Z_(BinaryInactiveText),   1,  Z_Nop },
  { Zuint32,  Cx0011, 0x0042,  Z_(BinaryMinimumOffTime), 1,  Z_Nop },
  { Zuint32,  Cx0011, 0x0043,  Z_(BinaryMinimumOnTime),  1,  Z_Nop },
  { Zbool,    Cx0011, 0x0051,  Z_(BinaryOutOfService),   1,  Z_Nop },
  { Zbool,    Cx0011, 0x0055,  Z_(BinaryValue),          1,  Z_Nop },
  // { Zunk,     Cx0011, 0x0057,  Z_(BinaryPriorityArray),  1,  Z_Nop },
  { Zenum8,   Cx0011, 0x0067,  Z_(BinaryReliability),    1,  Z_Nop },
  { Zbool,    Cx0011, 0x0068,  Z_(BinaryRelinquishDefault),1,  Z_Nop },
  { Zmap8,    Cx0011, 0x006F,  Z_(BinaryStatusFlags),    1,  Z_Nop },
  { Zuint32,  Cx0011, 0x0100,  Z_(BinaryApplicationType),1,  Z_Nop },

  // Multistate Input cluster
  // { Zunk,     Cx0012, 0x000E,  Z_(MultiInStateText),     1,  Z_Nop },
  { Zstring,  Cx0012, 0x001C,  Z_(MultiInDescription),   1,  Z_Nop },
  { Zuint16,  Cx0012, 0x004A,  Z_(MultiInNumberOfStates),1,  Z_Nop },
  { Zbool,    Cx0012, 0x0051,  Z_(MultiInOutOfService),  1,  Z_Nop },
  { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         0,  Z_AqaraCube },
  { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         0,  Z_AqaraButton },
  { Zenum8,   Cx0012, 0x0067,  Z_(MultiInReliability),   1,  Z_Nop },
  { Zmap8,    Cx0012, 0x006F,  Z_(MultiInStatusFlags),   1,  Z_Nop },
  { Zuint32,  Cx0012, 0x0100,  Z_(MultiInApplicationType),1,  Z_Nop },

  // Multistate output
  // { Zunk,     Cx0013, 0x000E,  Z_(MultiOutStateText),    1,  Z_Nop },
  { Zstring,  Cx0013, 0x001C,  Z_(MultiOutDescription),  1,  Z_Nop },
  { Zuint16,  Cx0013, 0x004A,  Z_(MultiOutNumberOfStates),1,  Z_Nop },
  { Zbool,    Cx0013, 0x0051,  Z_(MultiOutOutOfService), 1,  Z_Nop },
  { Zuint16,  Cx0013, 0x0055,  Z_(MultiOutValue),        1,  Z_Nop },
  // { Zunk,     Cx0013, 0x0057,  Z_(MultiOutPriorityArray),1,  Z_Nop },
  { Zenum8,   Cx0013, 0x0067,  Z_(MultiOutReliability),  1,  Z_Nop },
  { Zuint16,  Cx0013, 0x0068,  Z_(MultiOutRelinquishDefault),1,  Z_Nop },
  { Zmap8,    Cx0013, 0x006F,  Z_(MultiOutStatusFlags),  1,  Z_Nop },
  { Zuint32,  Cx0013, 0x0100,  Z_(MultiOutApplicationType),1,  Z_Nop },

  // Multistate Value cluster
  // { Zunk,     Cx0014, 0x000E,  Z_(MultiStateText),       1,  Z_Nop },
  { Zstring,  Cx0014, 0x001C,  Z_(MultiDescription),     1,  Z_Nop },
  { Zuint16,  Cx0014, 0x004A,  Z_(MultiNumberOfStates),  1,  Z_Nop },
  { Zbool,    Cx0014, 0x0051,  Z_(MultiOutOfService),    1,  Z_Nop },
  { Zuint16,  Cx0014, 0x0055,  Z_(MultiValue),           1,  Z_Nop },
  { Zenum8,   Cx0014, 0x0067,  Z_(MultiReliability),     1,  Z_Nop },
  { Zuint16,  Cx0014, 0x0068,  Z_(MultiRelinquishDefault),1,  Z_Nop },
  { Zmap8,    Cx0014, 0x006F,  Z_(MultiStatusFlags),     1,  Z_Nop },
  { Zuint32,  Cx0014, 0x0100,  Z_(MultiApplicationType), 1,  Z_Nop },

  // Power Profile cluster
  { Zuint8,   Cx001A, 0x0000,  Z_(TotalProfileNum),      1,  Z_Nop },
  { Zbool,    Cx001A, 0x0001,  Z_(MultipleScheduling),   1,  Z_Nop },
  { Zmap8,    Cx001A, 0x0002,  Z_(EnergyFormatting),     1,  Z_Nop },
  { Zbool,    Cx001A, 0x0003,  Z_(EnergyRemote),         1,  Z_Nop },
  { Zmap8,    Cx001A, 0x0004,  Z_(ScheduleMode),         1,  Z_Nop },

  // Poll Control cluster
  { Zuint32,  Cx0020, 0x0000,  Z_(CheckinInterval),      1,  Z_Nop },
  { Zuint32,  Cx0020, 0x0001,  Z_(LongPollInterval),     1,  Z_Nop },
  { Zuint16,  Cx0020, 0x0002,  Z_(ShortPollInterval),    1,  Z_Nop },
  { Zuint16,  Cx0020, 0x0003,  Z_(FastPollTimeout),      1,  Z_Nop },
  { Zuint32,  Cx0020, 0x0004,  Z_(CheckinIntervalMin),   1,  Z_Nop },
  { Zuint32,  Cx0020, 0x0005,  Z_(LongPollIntervalMin),  1,  Z_Nop },
  { Zuint16,  Cx0020, 0x0006,  Z_(FastPollTimeoutMax),   1,  Z_Nop },

  // Shade Configuration cluster
  { Zuint16,  Cx0100, 0x0000,  Z_(PhysicalClosedLimit),  1,  Z_Nop },
  { Zuint8,   Cx0100, 0x0001,  Z_(MotorStepSize),        1,  Z_Nop },
  { Zmap8,    Cx0100, 0x0002,  Z_(Status),               1,  Z_Nop },
  { Zuint16,  Cx0100, 0x0010,  Z_(ClosedLimit),          1,  Z_Nop },
  { Zenum8,   Cx0100, 0x0011,  Z_(Mode),                 1,  Z_Nop },

  // Door Lock cluster
  { Zenum8,   Cx0101, 0x0000,  Z_(LockState),            1,  Z_Nop },
  { Zenum8,   Cx0101, 0x0001,  Z_(LockType),             1,  Z_Nop },
  { Zbool,    Cx0101, 0x0002,  Z_(ActuatorEnabled),      1,  Z_Nop },
  { Zenum8,   Cx0101, 0x0003,  Z_(DoorState),            1,  Z_Nop },
  { Zuint32,  Cx0101, 0x0004,  Z_(DoorOpenEvents),       1,  Z_Nop },
  { Zuint32,  Cx0101, 0x0005,  Z_(DoorClosedEvents),     1,  Z_Nop },
  { Zuint16,  Cx0101, 0x0006,  Z_(OpenPeriod),           1,  Z_Nop },

  // Aqara Lumi Vibration Sensor
  { Zuint16,  Cx0101, 0x0055,  Z_(AqaraVibrationMode),   0,  Z_AqaraVibration },
  { Zuint16,  Cx0101, 0x0503,  Z_(AqaraVibrationsOrAngle), 1,  Z_Nop },
  { Zuint32,  Cx0101, 0x0505,  Z_(AqaraVibration505),    1,  Z_Nop },
  { Zuint48,  Cx0101, 0x0508,  Z_(AqaraAccelerometer),   0,  Z_AqaraVibration },

  // Window Covering cluster
  { Zenum8,   Cx0102, 0x0000,  Z_(WindowCoveringType),   1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0001,  Z_(PhysicalClosedLimitLift),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0002,  Z_(PhysicalClosedLimitTilt),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0003,  Z_(CurrentPositionLift),  1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0004,  Z_(CurrentPositionTilt),  1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0005,  Z_(NumberofActuationsLift),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0006,  Z_(NumberofActuationsTilt),1,  Z_Nop },
  { Zmap8,    Cx0102, 0x0007,  Z_(ConfigStatus),         1,  Z_Nop },
  { Zuint8,   Cx0102, 0x0008,  Z_(CurrentPositionLiftPercentage),1,  Z_Nop },
  { Zuint8,   Cx0102, 0x0009,  Z_(CurrentPositionTiltPercentage),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0010,  Z_(InstalledOpenLimitLift),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0011,  Z_(InstalledClosedLimitLift),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0012,  Z_(InstalledOpenLimitTilt),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0013,  Z_(InstalledClosedLimitTilt),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0014,  Z_(VelocityLift),         1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0015,  Z_(AccelerationTimeLift),1,  Z_Nop },
  { Zuint16,  Cx0102, 0x0016,  Z_(DecelerationTimeLift), 1,  Z_Nop },
  { Zmap8,    Cx0102, 0x0017,  Z_(Mode),                 1,  Z_Nop },
  { Zoctstr,  Cx0102, 0x0018,  Z_(IntermediateSetpointsLift),1,  Z_Nop },
  { Zoctstr,  Cx0102, 0x0019,  Z_(IntermediateSetpointsTilt),1,  Z_Nop },

  // Color Control cluster
  { Zuint8,   Cx0300, 0x0000,  Z_(Hue),                  1,  Z_Nop },
  { Zuint8,   Cx0300, 0x0001,  Z_(Sat),                  1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0002,  Z_(RemainingTime),        1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0003,  Z_(X),                    1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0004,  Z_(Y),                    1,  Z_Nop },
  { Zenum8,   Cx0300, 0x0005,  Z_(DriftCompensation),    1,  Z_Nop },
  { Zstring,  Cx0300, 0x0006,  Z_(CompensationText),     1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0007,  Z_(CT),                   1,  Z_Nop },
  { Zenum8,   Cx0300, 0x0008,  Z_(ColorMode),            1,  Z_Nop },
  { Zuint8,   Cx0300, 0x0010,  Z_(NumberOfPrimaries),    1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0011,  Z_(Primary1X),            1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0012,  Z_(Primary1Y),            1,  Z_Nop },
  { Zuint8,   Cx0300, 0x0013,  Z_(Primary1Intensity),    1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0015,  Z_(Primary2X),            1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0016,  Z_(Primary2Y),            1,  Z_Nop },
  { Zuint8,   Cx0300, 0x0017,  Z_(Primary2Intensity),    1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0019,  Z_(Primary3X),            1,  Z_Nop },
  { Zuint16,  Cx0300, 0x001A,  Z_(Primary3Y),            1,  Z_Nop },
  { Zuint8,   Cx0300, 0x001B,  Z_(Primary3Intensity),    1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0030,  Z_(WhitePointX),          1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0031,  Z_(WhitePointY),          1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0032,  Z_(ColorPointRX),         1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0033,  Z_(ColorPointRY),         1,  Z_Nop },
  { Zuint8,   Cx0300, 0x0034,  Z_(ColorPointRIntensity), 1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0036,  Z_(ColorPointGX),         1,  Z_Nop },
  { Zuint16,  Cx0300, 0x0037,  Z_(ColorPointGY),         1,  Z_Nop },
  { Zuint8,   Cx0300, 0x0038,  Z_(ColorPointGIntensity), 1,  Z_Nop },
  { Zuint16,  Cx0300, 0x003A,  Z_(ColorPointBX),         1,  Z_Nop },
  { Zuint16,  Cx0300, 0x003B,  Z_(ColorPointBY),         1,  Z_Nop },
  { Zuint8,   Cx0300, 0x003C,  Z_(ColorPointBIntensity), 1,  Z_Nop },

  // Illuminance Measurement cluster
  { Zuint16,  Cx0400, 0x0000,  Z_(Illuminance),          1,  Z_Nop },    // Illuminance (in Lux)
  { Zuint16,  Cx0400, 0x0001,  Z_(IlluminanceMinMeasuredValue),     1,  Z_Nop },    //
  { Zuint16,  Cx0400, 0x0002,  Z_(IlluminanceMaxMeasuredValue),     1,  Z_Nop },    //
  { Zuint16,  Cx0400, 0x0003,  Z_(IlluminanceTolerance),            1,  Z_Nop },    //
  { Zenum8,   Cx0400, 0x0004,  Z_(IlluminanceLightSensorType),      1,  Z_Nop },    //
  { Zunk,     Cx0400, 0xFFFF,  Z_(),                    0,  Z_Nop },    // Remove all other values

  // Illuminance Level Sensing cluster
  { Zenum8,   Cx0401, 0x0000,  Z_(IlluminanceLevelStatus),          1,  Z_Nop },    // Illuminance (in Lux)
  { Zenum8,   Cx0401, 0x0001,  Z_(IlluminanceLightSensorType),      1,  Z_Nop },    // LightSensorType
  { Zuint16,  Cx0401, 0x0010,  Z_(IlluminanceTargetLevel),          1,  Z_Nop },    //
  { Zunk,     Cx0401, 0xFFFF,  Z_(),                    0, Z_Nop },    // Remove all other values

  // Temperature Measurement cluster
  { Zint16,   Cx0402, 0x0000,  Z_(Temperature),          -100, Z_Nop },   // divide by 100
  { Zint16,   Cx0402, 0x0001,  Z_(TemperatureMinMeasuredValue),     -100, Z_Nop },    //
  { Zint16,   Cx0402, 0x0002,  Z_(TemperatureMaxMeasuredValue),     -100, Z_Nop },    //
  { Zuint16,  Cx0402, 0x0003,  Z_(TemperatureTolerance),            -100, Z_Nop },    //
  { Zunk,     Cx0402, 0xFFFF,  Z_(),                    0, Z_Nop },     // Remove all other values

  // Pressure Measurement cluster
  { Zunk,     Cx0403, 0x0000,  Z_(PressureUnit),                 0, Z_AddPressureUnit },     // Pressure Unit
  { Zint16,   Cx0403, 0x0000,  Z_(Pressure),                     1,  Z_Nop },     // Pressure
  { Zint16,   Cx0403, 0x0001,  Z_(PressureMinMeasuredValue),     1,  Z_Nop },    //
  { Zint16,   Cx0403, 0x0002,  Z_(PressureMaxMeasuredValue),     1,  Z_Nop },    //
  { Zuint16,  Cx0403, 0x0003,  Z_(PressureTolerance),            1,  Z_Nop },    //
  { Zint16,   Cx0403, 0x0010,  Z_(PressureScaledValue),          1,  Z_Nop },    //
  { Zint16,   Cx0403, 0x0011,  Z_(PressureMinScaledValue),       1,  Z_Nop },    //
  { Zint16,   Cx0403, 0x0012,  Z_(PressureMaxScaledValue),       1,  Z_Nop },    //
  { Zuint16,  Cx0403, 0x0013,  Z_(PressureScaledTolerance),      1,  Z_Nop },    //
  { Zint8,    Cx0403, 0x0014,  Z_(PressureScale),                1,  Z_Nop },    //
  { Zunk,     Cx0403, 0xFFFF,  Z_(),                    0, Z_Nop },     // Remove all other Pressure values

  // Flow Measurement cluster
  { Zuint16,  Cx0404, 0x0000,  Z_(FlowRate),             -10, Z_Nop },    // Flow (in m3/h)
  { Zuint16,  Cx0404, 0x0001,  Z_(FlowMinMeasuredValue), 1,  Z_Nop },    //
  { Zuint16,  Cx0404, 0x0002,  Z_(FlowMaxMeasuredValue), 1,  Z_Nop },    //
  { Zuint16,  Cx0404, 0x0003,  Z_(FlowTolerance),        1,  Z_Nop },    //
  { Zunk,     Cx0404, 0xFFFF,  Z_(),                     0,  Z_Nop },    // Remove all other values

  // Relative Humidity Measurement cluster
  { Zuint16,  Cx0405, 0x0000,  Z_(Humidity),                     -100, Z_Nop },   // Humidity
  { Zuint16,  Cx0405, 0x0001,  Z_(HumidityMinMeasuredValue),     1,  Z_Nop },    //
  { Zuint16,  Cx0405, 0x0002,  Z_(HumidityMaxMeasuredValue),     1,  Z_Nop },    //
  { Zuint16,  Cx0405, 0x0003,  Z_(HumidityTolerance),            1,  Z_Nop },    //
  { Zunk,     Cx0405, 0xFFFF,  Z_(),                    0, Z_Nop },     // Remove all other values

  // Occupancy Sensing cluster
  { Zmap8,    Cx0406, 0x0000,  Z_(Occupancy),            1,  Z_Nop },    // Occupancy (map8)
  { Zenum8,   Cx0406, 0x0001,  Z_(OccupancySensorType),  1,  Z_Nop },    // OccupancySensorType
  { Zunk,     Cx0406, 0xFFFF,  Z_(),                    0, Z_Nop },    // Remove all other values

  // IAS Cluster (Intruder Alarm System)
  { Zenum8,   Cx0500, 0x0000,  Z_(ZoneState),            1,  Z_Nop },    // Occupancy (map8)
  { Zenum16,  Cx0500, 0x0001,  Z_(ZoneType),             1,  Z_Nop },    // Occupancy (map8)
  { Zmap16,   Cx0500, 0x0002,  Z_(ZoneStatus),           1,  Z_Nop },    // Occupancy (map8)

  // Meter Identification cluster
  { Zstring,  Cx0B01, 0x0000,  Z_(CompanyName),          1,  Z_Nop },
  { Zuint16,  Cx0B01, 0x0001,  Z_(MeterTypeID),          1,  Z_Nop },
  { Zuint16,  Cx0B01, 0x0004,  Z_(DataQualityID),        1,  Z_Nop },
  { Zstring,  Cx0B01, 0x0005,  Z_(CustomerName),         1,  Z_Nop },
  { Zoctstr,  Cx0B01, 0x0006,  Z_(Model),                1,  Z_Nop },
  { Zoctstr,  Cx0B01, 0x0007,  Z_(PartNumber),           1,  Z_Nop },
  { Zoctstr,  Cx0B01, 0x0008,  Z_(ProductRevision),      1,  Z_Nop },
  { Zoctstr,  Cx0B01, 0x000A,  Z_(SoftwareRevision),     1,  Z_Nop },
  { Zstring,  Cx0B01, 0x000B,  Z_(UtilityName),          1,  Z_Nop },
  { Zstring,  Cx0B01, 0x000C,  Z_(POD),                  1,  Z_Nop },
  { Zint24,   Cx0B01, 0x000D,  Z_(AvailablePower),       1,  Z_Nop },
  { Zint24,   Cx0B01, 0x000E,  Z_(PowerThreshold),       1,  Z_Nop },

  // Diagnostics cluster
  { Zuint16,  Cx0B05, 0x0000,  Z_(NumberOfResets),       1,  Z_Nop },
  { Zuint16,  Cx0B05, 0x0001,  Z_(PersistentMemoryWrites),1,  Z_Nop },
  { Zuint8,   Cx0B05, 0x011C,  Z_(LastMessageLQI),       1,  Z_Nop },
  { Zuint8,   Cx0B05, 0x011D,  Z_(LastMessageRSSI),      1,  Z_Nop },

};


typedef union ZCLHeaderFrameControl_t {
  struct {
    uint8_t frame_type : 2;           // 00 = across entire profile, 01 = cluster specific
    uint8_t manuf_specific : 1;       // Manufacturer Specific Sub-field
    uint8_t direction : 1;            // 0 = tasmota to zigbee, 1 = zigbee to tasmota
    uint8_t disable_def_resp : 1;     // don't send back default response
    uint8_t reserved : 3;
  } b;
  uint32_t d8;                         // raw 8 bits field
} ZCLHeaderFrameControl_t;



// Find the attribute details by attribute name
// If not found:
//  - returns nullptr
const __FlashStringHelper* zigbeeFindAttributeByName(const char *command,
                                    uint16_t *cluster, uint16_t *attribute, int8_t *multiplier,
                                    uint8_t  *cb) {
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    if (0 == pgm_read_word(&converter->name_offset)) { continue; }         // avoid strcasecmp_P() from crashing
    if (0 == strcasecmp_P(command, Z_strings + pgm_read_word(&converter->name_offset))) {
      if (cluster)      { *cluster    = CxToCluster(pgm_read_byte(&converter->cluster_short)); }
      if (attribute)    { *attribute  = pgm_read_word(&converter->attribute); }
      if (multiplier)   { *multiplier = pgm_read_byte(&converter->multiplier); }
      if (cb)           { *cb         = pgm_read_byte(&converter->cb); }
      return (const __FlashStringHelper*) (Z_strings + pgm_read_word(&converter->name_offset));
    }
  }
  return nullptr;
}

class ZCLFrame {
public:

  ZCLFrame(uint8_t frame_control, uint16_t manuf_code, uint8_t transact_seq, uint8_t cmd_id,
    const char *buf, size_t buf_len, uint16_t clusterid, uint16_t groupaddr,
    uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
    uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber):
    _manuf_code(manuf_code), _transact_seq(transact_seq), _cmd_id(cmd_id),
    _payload(buf_len ? buf_len : 250),      // allocate the data frame from source or preallocate big enough
    _cluster_id(clusterid), _groupaddr(groupaddr),
    _srcaddr(srcaddr), _srcendpoint(srcendpoint), _dstendpoint(dstendpoint), _wasbroadcast(wasbroadcast),
    _linkquality(linkquality), _securityuse(securityuse), _seqnumber(seqnumber)
    {
      _frame_control.d8 = frame_control;
      _payload.addBuffer(buf, buf_len);
    };


  void log(void) {
    char hex_char[_payload.len()*2+2];
		ToHex_P((unsigned char*)_payload.getBuffer(), _payload.len(), hex_char, sizeof(hex_char));
    Response_P(PSTR("{\"" D_JSON_ZIGBEEZCL_RECEIVED "\":{"
                    "\"groupid\":%d," "\"clusterid\":%d," "\"srcaddr\":\"0x%04X\","
                    "\"srcendpoint\":%d," "\"dstendpoint\":%d," "\"wasbroadcast\":%d,"
                    "\"" D_CMND_ZIGBEE_LINKQUALITY "\":%d," "\"securityuse\":%d," "\"seqnumber\":%d,"
                    "\"fc\":\"0x%02X\",\"manuf\":\"0x%04X\",\"transact\":%d,"
                    "\"cmdid\":\"0x%02X\",\"payload\":\"%s\"}}"),
                    _groupaddr, _cluster_id, _srcaddr,
                    _srcendpoint, _dstendpoint, _wasbroadcast,
                    _linkquality, _securityuse, _seqnumber,
                    _frame_control, _manuf_code, _transact_seq, _cmd_id,
                    hex_char);
    if (Settings.flag3.tuya_serial_mqtt_publish) {
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR));
    } else {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), mqtt_data);
    }
  }

  static ZCLFrame parseRawFrame(const SBuffer &buf, uint8_t offset, uint8_t len, uint16_t clusterid, uint16_t groupid,
                                uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
                                uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber) { // parse a raw frame and build the ZCL frame object
    uint32_t i = offset;
    ZCLHeaderFrameControl_t frame_control;
    uint16_t manuf_code = 0;
    uint8_t transact_seq;
    uint8_t cmd_id;

    frame_control.d8 = buf.get8(i++);
    if (frame_control.b.manuf_specific) {
      manuf_code = buf.get16(i);
      i += 2;
    }
    transact_seq = buf.get8(i++);
    cmd_id = buf.get8(i++);
    ZCLFrame zcl_frame(frame_control.d8, manuf_code, transact_seq, cmd_id,
                       (const char *)(buf.buf() + i), len + offset - i,
                       clusterid, groupid,
                       srcaddr, srcendpoint, dstendpoint, wasbroadcast,
                       linkquality, securityuse, seqnumber);
    return zcl_frame;
  }

  bool isClusterSpecificCommand(void) {
    return _frame_control.b.frame_type & 1;
  }

  static void generateAttributeName(const JsonObject& json, uint16_t cluster, uint16_t attr, char *key, size_t key_len);
  void parseReportAttributes(JsonObject& json, uint8_t offset = 0);
  void parseReadAttributes(JsonObject& json, uint8_t offset = 0);
  void parseReadAttributesResponse(JsonObject& json, uint8_t offset = 0);
  void parseReadConfigAttributes(JsonObject& json, uint8_t offset = 0);
  void parseConfigAttributes(JsonObject& json, uint8_t offset = 0);
  void parseResponse(void);
  void parseClusterSpecificCommand(JsonObject& json, uint8_t offset = 0);
  void postProcessAttributes(uint16_t shortaddr, JsonObject& json);
  void updateInternalAttributes(uint16_t shortaddr, JsonObject& json);

  inline void setGroupId(uint16_t groupid) {
    _groupaddr = groupid;
  }

  inline void setClusterId(uint16_t clusterid) {
    _cluster_id = clusterid;
  }

  inline uint16_t getSrcAddr(void) const { return _srcaddr; }
  inline uint16_t getGroupAddr(void) const { return _groupaddr; }
  inline uint16_t getClusterId(void) const { return _cluster_id; }
  inline uint8_t  getLinkQuality(void) const { return _linkquality; }
  inline uint8_t getCmdId(void) const { return _cmd_id; }
  inline uint16_t getSrcEndpoint(void) const { return _srcendpoint; }

  const SBuffer &getPayload(void) const {
    return _payload;
  }

  uint16_t getManufCode(void) const {
    return _manuf_code;
  }


private:
  ZCLHeaderFrameControl_t _frame_control = { .d8 = 0 };
  uint16_t                _manuf_code = 0;      // optional
  uint8_t                 _transact_seq = 0;    // transaction sequence number
  uint8_t                 _cmd_id = 0;
  SBuffer                 _payload;
  uint16_t                _cluster_id = 0;
  uint16_t                _groupaddr = 0;
  // information from decoded ZCL frame
  uint16_t                _srcaddr;
  uint8_t                 _srcendpoint;
  uint8_t                 _dstendpoint;
  uint8_t                 _wasbroadcast;
  uint8_t                 _linkquality;
  uint8_t                 _securityuse;
  uint8_t                 _seqnumber;
};

// Zigbee ZCL converters

// from https://github.com/Koenkk/zigbee-shepherd-converters/blob/638d29f0cace6343052b9a4e7fd60980fa785479/converters/fromZigbee.js#L55
// Input voltage in mV, i.e. 3000 = 3.000V
// Output percentage from 0 to 100 as int
uint8_t toPercentageCR2032(uint32_t voltage) {
  uint32_t percentage;
  if (voltage < 2100) {
      percentage = 0;
  } else if (voltage < 2440) {
      percentage = 6 - ((2440 - voltage) * 6) / 340;
  } else if (voltage < 2740) {
      percentage = 18 - ((2740 - voltage) * 12) / 300;
  } else if (voltage < 2900) {
      percentage = 42 - ((2900 - voltage) * 24) / 160;
  } else if (voltage < 3000) {
      percentage = 100 - ((3000 - voltage) * 58) / 100;
  } else if (voltage >= 3000) {
      percentage = 100;
  }
  return percentage;
}

//
// Appends the attribute value to Write or to Report
// Adds to buf:
// - n bytes: value (typically between 1 and 4 bytes, or bigger for strings)
// returns number of bytes of attribute, or <0 if error
int32_t encodeSingleAttribute(class SBuffer &buf, double val_d, const char *val_str, uint8_t attrtype) {
  uint32_t len = Z_getDatatypeLen(attrtype);    // pre-compute lenght, overloaded for variable length attributes
  uint32_t u32 = val_d;
  int32_t  i32 = val_d;
  float    f32 = val_d;

  switch (attrtype) {
    // unsigned 8
    case Zbool:       // bool
    case Zuint8:      // uint8
    case Zenum8:      // enum8
    case Zdata8:      // data8
    case Zmap8:       // map8
      buf.add8(u32);
      break;
    // unsigned 16
    case Zuint16:     // uint16
    case Zenum16:     // enum16
    case Zdata16:     // data16
    case Zmap16:      // map16
      buf.add16(u32);
      break;
    // unisgned 32
    case Zuint32:     // uint32
    case Zdata32:     // data32
    case Zmap32:      // map32
    case ZUTC:        // UTC - epoch 32 bits, seconds since 1-Jan-2000
      buf.add32(u32);
      break;

    // signed 8
    case Zint8:      // int8
      buf.add8(i32);
      break;
    case Zint16:      // int16
      buf.add16(i32);
      break;
    case Zint32:      // int32
      buf.add32(i32);
      break;

    case Zsingle:      // float
      uint32_t *f_ptr;
      buf.add32( *((uint32_t*)&f32) );    // cast float as uint32_t
      break;

    case Zstring:
    case Zstring16:
      {
        if (nullptr == val_str) { return -2; }
        size_t val_len = strlen(val_str);
        if (val_len > 32) { val_len = 32; }
        len = val_len + 1;
        buf.add8(val_len);
        if (Zstring16 == attrtype) {
          buf.add8(0);    // len is on 2 bytes
          len++;
        }
        for (uint32_t i = 0; i < val_len; i++) {
          buf.add8(val_str[i]);
        }
      }
      break;

    default:
      return -1;
  }
  return len;
}

//
// parse a single attribute
//
// Input:
//   json: json Object where to add the attribute
//   attrid_str: the key for the attribute
//   buf:  the buffer to read from
//   offset: location in the buffer to read from
//   attrtype: type of attribute (byte) or -1 to read from the stream as first byte
// Output:
//   return: the length in bytes of the attribute
uint32_t parseSingleAttribute(JsonObject& json, char *attrid_str, class SBuffer &buf,
                              uint32_t offset, int32_t attrtype = -1) {

  uint32_t i = offset;
  if (attrtype < 0) {
    attrtype = buf.get8(i++);
  }

  // fallback - enter a null value
  json[attrid_str] = (char*) nullptr;

  uint32_t len = Z_getDatatypeLen(attrtype);    // pre-compute lenght, overloaded for variable length attributes

  // now parse accordingly to attr type
  switch (attrtype) {
    // case Znodata:      // nodata
    // case Zunk:      // unk
    //   break;
    case Zbool:      // bool
    case Zuint8:      // uint8
    case Zenum8:      // enum8
      {
        uint8_t uint8_val = buf.get8(i);
        // i += 1;
        if (0xFF != uint8_val) {
          json[attrid_str] = uint8_val;
        }
      }
      break;
    case Zuint16:      // uint16
    case Zenum16:      // enum16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        if (0xFFFF != uint16_val) {
          json[attrid_str] = uint16_val;
        }
      }
      break;
    case Zuint32:      // uint32
    case ZUTC:      // UTC
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        if (0xFFFFFFFF != uint32_val) {
          json[attrid_str] = uint32_val;
        }
      }
      break;
    // Note: uint40, uint48, uint56, uint64 are displayed as Hex
    // Note: int40, int48, int56, int64 are displayed as Hex
    case Zuint40:    // uint40
    case Zuint48:    // uint48
    case Zuint56:    // uint56
    case Zuint64:    // uint64
    case Zint40:    // int40
    case Zint48:    // int48
    case Zint56:    // int56
    case Zint64:    // int64
      {
        // uint8_t len = attrtype - 0x27;   // 5 - 8
        // print as HEX
        char hex[2*len+1];
        ToHex_P(buf.buf(i), len, hex, sizeof(hex));
        json[attrid_str] = hex;
        // i += len;
      }
      break;
    case Zint8:      // int8
      {
        int8_t int8_val = buf.get8(i);
        // i += 1;
        if (0x80 != int8_val) {
          json[attrid_str] = int8_val;
        }
      }
      break;
    case Zint16:      // int16
      {
        int16_t int16_val = buf.get16(i);
        // i += 2;
        if (0x8000 != int16_val) {
          json[attrid_str] = int16_val;
        }
      }
      break;
    case Zint32:      // int32
      {
        int32_t int32_val = buf.get32(i);
        // i += 4;
        if (0x80000000 != int32_val) {
          json[attrid_str] = int32_val;
        }
      }
      break;

    case Zoctstr:      // octet string, 1 byte len
    case Zstring:      // char string, 1 byte len
    case Zoctstr16:      // octet string, 2 bytes len
    case Zstring16:      // char string, 2 bytes len
      // For strings, default is to try to do a real string, but reverts to octet stream if null char is present or on some exceptions
      {
        bool parse_as_string = true;
        len = (attrtype <= 0x42) ? buf.get8(i) : buf.get16(i);    // len is 8 or 16 bits
        i += (attrtype <= 0x42) ? 1 : 2;                                   // increment pointer
        if (i + len > buf.len()) {        // make sure we don't get past the buffer
          len = buf.len() - i;
        }

        // check if we can safely use a string
        if ((0x41 == attrtype) || (0x43 == attrtype)) { parse_as_string = false; }

        if (parse_as_string) {
          char str[len+1];
          strncpy(str, buf.charptr(i), len);
          str[len] = 0x00;
          json[attrid_str] = str;
        } else {
          // print as HEX
          char hex[2*len+1];
          ToHex_P(buf.buf(i), len, hex, sizeof(hex));
          json[attrid_str] = hex;
        }

        // i += len;
        // break;
      }
      // i += buf.get8(i) + 1;
      break;

    case Zdata8:      // data8
    case Zmap8:      // map8
      {
        uint8_t uint8_val = buf.get8(i);
        // i += 1;
        json[attrid_str] = uint8_val;
      }
      break;
    case Zdata16:      // data16
    case Zmap16:      // map16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        json[attrid_str] = uint16_val;
      }
      break;
    case Zdata32:      // data32
    case Zmap32:      // map32
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        json[attrid_str] = uint32_val;
      }
      break;

    case Zsingle:      // float
      {
        uint32_t uint32_val = buf.get32(i);
        float  * float_val = (float*) &uint32_val;
        // i += 4;
        json[attrid_str] = *float_val;
      }
      break;

    // TODO
    case ZToD:      // ToD
    case Zdate:      // date
    case ZclusterId:      // clusterId
    case ZattribId:      // attribId
    case ZbacOID:      // bacOID
    case ZEUI64:      // EUI64
    case Zkey128:      // key128
    case Zsemi:      // semi (float on 2 bytes)
      break;

    // Other un-implemented data types
    case Zdata24:      // data24
    case Zdata40:      // data40
    case Zdata48:      // data48
    case Zdata56:      // data56
    case Zdata64:      // data64
      break;
    // map<x>
    case Zmap24:      // map24
    case Zmap40:      // map40
    case Zmap48:      // map48
    case Zmap56:      // map56
    case Zmap64:      // map64
      break;
    case Zdouble:      // double precision
      {
        uint64_t uint64_val = buf.get64(i);
        double  * double_val = (double*) &uint64_val;
        // i += 8;
        json[attrid_str] = *double_val;
      }
      break;
  }
  i += len;

  // String pp;    // pretty print
  // json[attrid_str].prettyPrintTo(pp);
  // // now store the attribute
  // AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZCL attribute decoded, id %s, type 0x%02X, val=%s"),
  //                                attrid_str, attrtype, pp.c_str());
  return i - offset;    // how much have we increased the index
}

// Generate an attribute name based on cluster number, attribute, and suffix if duplicates
void ZCLFrame::generateAttributeName(const JsonObject& json, uint16_t cluster, uint16_t attr, char *key, size_t key_len) {
  uint32_t suffix = 1;

  snprintf_P(key, key_len, PSTR("%04X/%04X"), cluster, attr);
  while (json.containsKey(key)) {
    suffix++;
    snprintf_P(key, key_len, PSTR("%04X/%04X+%d"), cluster, attr, suffix);    // add "0008/0001+2" suffix if duplicate
  }
}

// First pass, parse all attributes in their native format
void ZCLFrame::parseReportAttributes(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  while (len >= i + 3) {
    uint16_t attrid = _payload.get16(i);
    i += 2;

    char key[16];
    generateAttributeName(json, _cluster_id, attrid, key, sizeof(key));

    // exception for Xiaomi lumi.weather - specific field to be treated as octet and not char
    if ((0x0000 == _cluster_id) && (0xFF01 == attrid)) {
      if (0x42 == _payload.get8(i)) {
        _payload.set8(i, 0x41);   // change type from 0x42 to 0x41
      }
    }
    i += parseSingleAttribute(json, key, _payload, i);
  }
}

// ZCL_READ_ATTRIBUTES
// TODO
void ZCLFrame::parseReadAttributes(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  json[F(D_CMND_ZIGBEE_CLUSTER)] = _cluster_id;

  JsonArray &attr_list = json.createNestedArray(F("Read"));
  JsonObject &attr_names = json.createNestedObject(F("ReadNames"));
  while (len >= 2 + i) {
    uint16_t attrid = _payload.get16(i);
    attr_list.add(attrid);

    // find the attribute name
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint16_t conv_attribute = pgm_read_word(&converter->attribute);

      if ((conv_cluster == _cluster_id) && (conv_attribute == attrid)) {
        attr_names[(const __FlashStringHelper*) (Z_strings + pgm_read_word(&converter->name_offset))] = true;
        break;
      }
    }
    i += 2;
  }
}

// ZCL_CONFIGURE_REPORTING_RESPONSE
void ZCLFrame::parseConfigAttributes(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  JsonObject &config_rsp = json.createNestedObject(F("ConfigResponse"));
  uint8_t  status = _payload.get8(i);
  config_rsp[F("Status")] = status;
  config_rsp[F("StatusMsg")] = getZigbeeStatusMessage(status);
}

// ZCL_READ_REPORTING_CONFIGURATION_RESPONSE
void ZCLFrame::parseReadConfigAttributes(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  // json[F(D_CMND_ZIGBEE_CLUSTER)] = _cluster_id;   // TODO is it necessary?

  JsonObject &attr_names = json.createNestedObject(F("ReadConfig"));
  while (len >= i + 4) {
    uint8_t  status = _payload.get8(i);
    uint8_t  direction = _payload.get8(i+1);
    uint16_t attrid = _payload.get16(i+2);
    char attr_hex[12];
    snprintf_P(attr_hex, sizeof(attr_hex), "%04X/%04X", _cluster_id, attrid);
    JsonObject &attr_details = attr_names.createNestedObject(attr_hex);

    if (direction) {
      attr_details[F("DirectionReceived")] = true;
    }

    // find the attribute name
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint16_t conv_attribute = pgm_read_word(&converter->attribute);

      if ((conv_cluster == _cluster_id) && (conv_attribute == attrid)) {
        attr_details[(const __FlashStringHelper*) (Z_strings + pgm_read_word(&converter->name_offset))] = true;
        break;
      }
    }
    i += 4;
    if (0 != status) {
      attr_details[F("Status")] = status;
      attr_details[F("StatusMsg")] = getZigbeeStatusMessage(status);
    } else {
      // no error, decode data
      if (direction) {
        // only Timeout period is present
        uint16_t attr_timeout = _payload.get16(i);
        i += 2;
        attr_details[F("TimeoutPeriod")] = (0xFFFF == attr_timeout) ? -1 : attr_timeout;
      } else {
        // direction == 0, we have a data type
        uint8_t attr_type = _payload.get8(i);
        bool attr_discrete = Z_isDiscreteDataType(attr_type);
        uint16_t attr_min_interval = _payload.get16(i+1);
        uint16_t attr_max_interval = _payload.get16(i+3);
        i += 5;
        attr_details[F("MinInterval")] = (0xFFFF == attr_min_interval) ? -1 : attr_min_interval;
        attr_details[F("MaxInterval")] = (0xFFFF == attr_max_interval) ? -1 : attr_max_interval;
        if (!attr_discrete) {
          // decode Reportable Change
          char attr_name[20];
          strcpy_P(attr_name, PSTR("ReportableChange"));
          i += parseSingleAttribute(attr_details, attr_name, _payload, i, attr_type);
        }
      }
    }
  }
}

// ZCL_READ_ATTRIBUTES_RESPONSE
void ZCLFrame::parseReadAttributesResponse(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  while (len >= i + 4) {
    uint16_t attrid = _payload.get16(i);
    i += 2;
    uint8_t status = _payload.get8(i++);

    if (0 == status) {
      char key[16];
      generateAttributeName(json, _cluster_id, attrid, key, sizeof(key));

      i += parseSingleAttribute(json, key, _payload, i);
    }
  }
}

// ZCL_DEFAULT_RESPONSE
void ZCLFrame::parseResponse(void) {
  if (_payload.len() < 2) { return; }   // wrong format
  uint8_t cmd = _payload.get8(0);
  uint8_t status = _payload.get8(1);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  // "Device"
  char s[12];
  snprintf_P(s, sizeof(s), PSTR("0x%04X"), _srcaddr);
  json[F(D_JSON_ZIGBEE_DEVICE)] = s;
  // "Name"
  const char * friendlyName = zigbee_devices.getFriendlyName(_srcaddr);
  if (friendlyName) {
    json[F(D_JSON_ZIGBEE_NAME)] = (char*) friendlyName;
  }
  // "Command"
  snprintf_P(s, sizeof(s), PSTR("%04X!%02X"), _cluster_id, cmd);
  json[F(D_JSON_ZIGBEE_CMD)] = s;
  // "Status"
  json[F(D_JSON_ZIGBEE_STATUS)] = status;
  // "StatusMessage"
  json[F(D_JSON_ZIGBEE_STATUS_MSG)] = getZigbeeStatusMessage(status);
  // Add Endpoint
  json[F(D_CMND_ZIGBEE_ENDPOINT)] = _srcendpoint;
  // Add Group if non-zero
  if (_groupaddr) {
    json[F(D_CMND_ZIGBEE_GROUP)] = _groupaddr;
  }
  // Add linkquality
  json[F(D_CMND_ZIGBEE_LINKQUALITY)] = _linkquality;

  String msg("");
  msg.reserve(100);
  json.printTo(msg);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_RESPONSE "\":%s}"), msg.c_str());
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
}


// Parse non-normalized attributes
void ZCLFrame::parseClusterSpecificCommand(JsonObject& json, uint8_t offset) {
  convertClusterSpecific(json, _cluster_id, _cmd_id, _frame_control.b.direction, _srcaddr, _srcendpoint, _payload);
#ifndef USE_ZIGBEE_NO_READ_ATTRIBUTES   // read attributes unless disabled
  sendHueUpdate(_srcaddr, _groupaddr, _cluster_id, _cmd_id, _frame_control.b.direction);
#endif
}

// ======================================================================
// Record Manuf
int32_t Z_ManufKeepFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  zigbee_devices.setManufId(shortaddr, value.as<const char*>());
  return 1;
}
// Record ModelId
int32_t Z_ModelKeepFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  zigbee_devices.setModelId(shortaddr, value.as<const char*>());
  return 1;
}
// Record BatteryPercentage
int32_t Z_BatteryPercentageKeepFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  zigbee_devices.setBatteryPercent(shortaddr, json[new_name]);
  return 1;
}

// Add pressure unit
int32_t Z_AddPressureUnitFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = F(D_UNIT_PRESSURE);
  return 0;   // keep original key
}

// Publish a message for `"Occupancy":0` when the timer expired
int32_t Z_OccupancyCallback(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json[F(OCCUPANCY)] = 0;
  zigbee_devices.jsonPublishNow(shortaddr, json);
  return 0;  // Fix GCC 10.1 warning
}

// Aqara Cube
int32_t Z_AqaraCubeFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  const char * modelId_c = zigbee_devices.findShortAddr(shortaddr).modelId;  // null if unknown
  String modelId((char*) modelId_c);

  if (modelId.startsWith(F("lumi.sensor_cube"))) {   // only for Aqara cube
    int32_t val = value;
    const __FlashStringHelper *aqara_cube = F("AqaraCube");
    const __FlashStringHelper *aqara_cube_side = F("AqaraCubeSide");
    const __FlashStringHelper *aqara_cube_from_side = F("AqaraCubeFromSide");

    switch (val) {
      case 0:
        json[aqara_cube] = F("shake");
        break;
      case 2:
        json[aqara_cube] = F("wakeup");
        break;
      case 3:
        json[aqara_cube] = F("fall");
        break;
      case 64 ... 127:
        json[aqara_cube] = F("flip90");
        json[aqara_cube_side] = val % 8;
        json[aqara_cube_from_side] = (val - 64) / 8;
        break;
      case 128 ... 132:
        json[aqara_cube] = F("flip180");
        json[aqara_cube_side] = val - 128;
        break;
      case 256 ... 261:
        json[aqara_cube] = F("slide");
        json[aqara_cube_side] = val - 256;
        break;
      case 512 ... 517:
        json[aqara_cube] = F("tap");
        json[aqara_cube_side] = val - 512;
        break;
    }
    return 1;
  }

  //     Source: https://github.com/kirovilya/ioBroker.zigbee
  //         +---+
  //         | 2 |
  //     +---+---+---+
  //     | 4 | 0 | 1 |
  //     +---+---+---+
  //         |M5I|
  //         +---+
  //         | 3 |
  //         +---+
  //     Side 5 is with the MI logo, side 3 contains the battery door.
  //     presentValue = 0 = shake
  //     presentValue = 2 = wakeup
  //     presentValue = 3 = fly/fall
  //     presentValue = y + x * 8 + 64 = 90º Flip from side x on top to side y on top
  //     presentValue = x + 128 = 180º flip to side x on top
  //     presentValue = x + 256 = push/slide cube while side x is on top
  //     presentValue = x + 512 = double tap while side x is on top
  return 0;
}

// Aqara Button
int32_t Z_AqaraButtonFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  const char * modelId_c = zigbee_devices.getModelId(shortaddr);  // null if unknown
  String modelId((char*) modelId_c);

  if (modelId.startsWith(F("lumi.remote"))) {   // only for Aqara button
    int32_t val = value;
    const __FlashStringHelper *aqara_click = F("click");
    const __FlashStringHelper *aqara_action = F("action");

    switch (val) {
      case 0:
        json[aqara_action] = F("hold");
        break;
      case 1:
        json[aqara_click] = F("single");
        break;
      case 2:
        json[aqara_click] = F("double");
        break;
      case 255:
        json[aqara_action] = F("release");
        break;
      default:
        json[aqara_action] = val;
        break;
    }
    return 1;
  }

  return 0;
}

// Aqara Vibration Sensor - special proprietary attributes
int32_t Z_AqaraVibrationFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  //json[new_name] = value;
  switch (attr) {
    case 0x0055:
      {
        int32_t ivalue = value;
        const __FlashStringHelper * svalue;
        switch (ivalue) {
          case 1: svalue = F("vibrate"); break;
          case 2: svalue = F("tilt"); break;
          case 3: svalue = F("drop"); break;
          default: svalue = F("unknown"); break;
        }
        json[new_name] = svalue;
      }
      break;
    // case 0x0503:
    //   break;
    // case 0x0505:
    //   break;
    case 0x0508:
      {
        // see https://github.com/Koenkk/zigbee2mqtt/issues/295 and http://faire-ca-soi-meme.fr/domotique/2018/09/03/test-xiaomi-aqara-vibration-sensor/
        // report accelerometer measures
        String hex = value;
        SBuffer buf2 = SBuffer::SBufferFromHex(hex.c_str(), hex.length());
        int16_t x, y, z;
        z = buf2.get16(0);
        y = buf2.get16(2);
        x = buf2.get16(4);
        JsonArray& xyz = json.createNestedArray(new_name);
        xyz.add(x);
        xyz.add(y);
        xyz.add(z);
        // calculate angles
        float X = x;
        float Y = y;
        float Z = z;
        int32_t Angle_X = 0.5f + atanf(X/sqrtf(z*z+y*y)) * f_180pi;
        int32_t Angle_Y = 0.5f + atanf(Y/sqrtf(x*x+z*z)) * f_180pi;
        int32_t Angle_Z = 0.5f + atanf(Z/sqrtf(x*x+y*y)) * f_180pi;
        JsonArray& angles = json.createNestedArray(F("AqaraAngles"));
        angles.add(Angle_X);
        angles.add(Angle_Y);
        angles.add(Angle_Z);
      }
      break;
  }
  return 1;   // remove original key
}

int32_t Z_AqaraSensorFunc(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  String hex = value;
  SBuffer buf2 = SBuffer::SBufferFromHex(hex.c_str(), hex.length());
  uint32_t i = 0;
  uint32_t len = buf2.len();
  char tmp[] = "tmp";   // for obscure reasons, it must be converted from const char* to char*, otherwise ArduinoJson gets confused

  JsonVariant sub_value;
  const char * modelId_c = zigbee_devices.getModelId(shortaddr);  // null if unknown
  String modelId((char*) modelId_c);

  while (len >= 2 + i) {
    uint8_t attrid = buf2.get8(i++);

    i += parseSingleAttribute(json, tmp, buf2, i);
    float val = json[tmp];
    json.remove(tmp);
    bool translated = false;    // were we able to translate to a known format?
    if (0x01 == attrid) {
      float batteryvoltage = val / 1000.0f;
      json[F("BatteryVoltage")] = batteryvoltage;
      uint8_t batterypercentage = toPercentageCR2032(val);
      json[F("BatteryPercentage")] = batterypercentage;
      zigbee_devices.setBatteryPercent(shortaddr, batterypercentage);
      // deprecated
      json[F(D_JSON_VOLTAGE)] = batteryvoltage;
      json[F("Battery")] = toPercentageCR2032(val);
    } else if ((nullptr != modelId) && (0 == zcl->getManufCode())) {
      translated = true;
      if (modelId.startsWith(F("lumi.sensor_ht")) ||
          modelId.startsWith(F("lumi.weather"))) {     // Temp sensor
        // Filter according to prefix of model name
        // onla Aqara Temp/Humidity has manuf_code of zero. If non-zero we skip the parameters
        if (0x64 == attrid) {
          json[F(D_JSON_TEMPERATURE)] = val / 100.0f;
        } else if (0x65 == attrid) {
          json[F(D_JSON_HUMIDITY)] = val / 100.0f;
        } else if (0x66 == attrid) {
          json[F(D_JSON_PRESSURE)] = val / 100.0f;
          json[F(D_JSON_PRESSURE_UNIT)] = F(D_UNIT_PRESSURE);   // hPa
        }
      } else if (modelId.startsWith(F("lumi.sensor_smoke"))) {   // gas leak
        if (0x64 == attrid) {
          json[F("SmokeDensity")] = val;
        }
      } else if (modelId.startsWith(F("lumi.sensor_natgas"))) {   // gas leak
        if (0x64 == attrid) {
          json[F("GasDensity")] = val;
        }
      } else {
        translated = false;     // we didn't find a match
      }
 //   } else if (0x115F == zcl->getManufCode()) {      // Aqara Motion Sensor, still unknown field
    }
    if (!translated) {
      if (attrid >= 100) {    // payload is always above 0x64 or 100
        char attr_name[12];
        snprintf_P(attr_name, sizeof(attr_name), PSTR("Xiaomi_%02X"), attrid);
        json[attr_name] = val;
      }
    }
  }
  return 1;   // remove original key
}
// ======================================================================

// apply the transformation from the converter
int32_t Z_ApplyConverter(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name,
                        uint16_t cluster, uint16_t attr, int8_t multiplier, uint8_t cb) {
  // apply multiplier if needed
  if (1 == multiplier) {          // copy unchanged
      json[new_name] = value;
  } else if (0 != multiplier) {
      if (multiplier > 0) {
        json[new_name] = ((float)value) * multiplier;
      } else {
        json[new_name] = ((float)value) / (-multiplier);
      }
  }

  // apply callback if needed
  Z_AttrConverter func = nullptr;
  switch (cb) {
    case Z_Nop:
      return 1;             // drop original key
    case Z_AddPressureUnit:
      func = &Z_AddPressureUnitFunc;
      break;
    case Z_ManufKeep:
      func = &Z_ManufKeepFunc;
      break;
    case Z_ModelKeep:
      func = &Z_ModelKeepFunc;
      break;
    case Z_AqaraSensor:
      func = &Z_AqaraSensorFunc;
      break;
    case Z_AqaraVibration:
      func = &Z_AqaraVibrationFunc;
      break;
    case Z_AqaraCube:
      func = &Z_AqaraCubeFunc;
      break;
    case Z_AqaraButton:
      func = &Z_AqaraButtonFunc;
      break;
    case Z_BatteryPercentage:
      func = &Z_BatteryPercentageKeepFunc;
      break;
  };

  if (func) {
    return (*func)(zcl, shortaddr, json, name, value, new_name, cluster, attr);
  }
  return 1;  // Fix GCC 10.1 warning
}

// Scan all the final attributes and update any internal representation like sensors
void ZCLFrame::updateInternalAttributes(uint16_t shortaddr, JsonObject& json) {
  Z_Device & device = zigbee_devices.getShortAddr(shortaddr);
  for (auto kv : json) {
    String key_string = kv.key;
    const char * key = key_string.c_str();
    JsonVariant& value = kv.value;

    if (key_string.equalsIgnoreCase(F("Temperature"))) {
      device.temperature = value.as<float>() * 10 + 0.5f;
    } else if (key_string.equalsIgnoreCase(F("Humidity"))) {
      device.humidity = value.as<float>() + 0.5f;
    } else if (key_string.equalsIgnoreCase(F("Pressure"))) {
      device.pressure = value.as<float>() + 0.5f;
    }
  }
}

void ZCLFrame::postProcessAttributes(uint16_t shortaddr, JsonObject& json) {
  // source endpoint
  uint8_t src_ep = _srcendpoint;
  // iterate on json elements
  for (auto kv : json) {
    String key_string = kv.key;
    const char * key = key_string.c_str();
    JsonVariant& value = kv.value;
    // Check that format looks like "CCCC/AAAA" or "CCCC/AAAA+d"
    char * delimiter = strchr(key, '/');
    char * delimiter2 = strchr(key, '+');
    if (delimiter) {
      uint16_t attribute;
      uint16_t suffix = 1;
      uint16_t cluster = strtoul(key, &delimiter, 16);
      if (!delimiter2) {
        attribute = strtoul(delimiter+1, nullptr, 16);
      } else {
        attribute = strtoul(delimiter+1, &delimiter2, 16);
        suffix = strtoul(delimiter2+1, nullptr, 10);
      }

      Z_Device & device = zigbee_devices.getShortAddr(shortaddr);
      uint16_t val16 = value;     // call converter from JSonVariant to int only once
      // see if we need to update the Hue bulb status
      if ((cluster == 0x0006) && ((attribute == 0x0000) || (attribute == 0x8000))) {
        bool power = value;
        device.setPower(power);
      } else if ((cluster == 0x0008) && (attribute == 0x0000)) {
        device.dimmer = val16;
      } else if ((cluster == 0x0300) && (attribute == 0x0000)) {
        device.hue = changeUIntScale(val16, 0, 254, 0, 360);     // change range from 0..254 to 0..360
      } else if ((cluster == 0x0300) && (attribute == 0x0001)) {
        device.sat = val16;
      } else if ((cluster == 0x0300) && (attribute == 0x0003)) {
        device.x = val16;
      } else if ((cluster == 0x0300) && (attribute == 0x0004)) {
        device.y = val16;
      } else if ((cluster == 0x0300) && (attribute == 0x0007)) {
        device.ct = val16;
      } else if ((cluster == 0x0300) && (attribute == 0x0008)) {
        device.colormode = val16;
      }

      // Iterate on filter
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
        uint16_t conv_attribute = pgm_read_word(&converter->attribute);
        int8_t   conv_multiplier = pgm_read_byte(&converter->multiplier);
        uint8_t  conv_cb = pgm_read_byte(&converter->cb);                   // callback id

        if ((conv_cluster == cluster) &&
            ((conv_attribute == attribute) || (conv_attribute == 0xFFFF)) ) {
          String new_name_str = (const __FlashStringHelper*) (Z_strings + pgm_read_word(&converter->name_offset));
          if (suffix > 1) { new_name_str += suffix; }   // append suffix number
          // apply the transformation
          int32_t drop = Z_ApplyConverter(this, shortaddr, json, key, value, new_name_str, conv_cluster, conv_attribute, conv_multiplier, conv_cb);
          if (drop) {
            json.remove(key);
          }

        }
      }
    }
  }

  updateInternalAttributes(shortaddr, json);
}

#endif // USE_ZIGBEE
