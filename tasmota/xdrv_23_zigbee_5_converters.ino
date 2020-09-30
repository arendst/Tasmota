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

typedef struct Z_AttributeConverter {
  uint8_t  type;
  uint8_t  cluster_short;
  uint16_t attribute;
  uint16_t name_offset;
  int8_t   multiplier;     // multiplier for numerical value, (if > 0 multiply by x, if <0 device by x)
  // still room for a byte
} Z_AttributeConverter;

// Cluster numbers are store in 8 bits format to save space,
// the following tables allows the conversion from 8 bits index Cx...
// to the 16 bits actual cluster number
enum Cx_cluster_short {
  Cx0000, Cx0001, Cx0002, Cx0003, Cx0004, Cx0005, Cx0006, Cx0007,
  Cx0008, Cx0009, Cx000A, Cx000B, Cx000C, Cx000D, Cx000E, Cx000F,
  Cx0010, Cx0011, Cx0012, Cx0013, Cx0014, Cx001A, Cx0020, Cx0100,
  Cx0101, Cx0102, Cx0201, Cx0300, Cx0400, Cx0401, Cx0402, Cx0403,
  Cx0404, Cx0405, Cx0406, Cx0500, Cx0702, Cx0B01, Cx0B04, Cx0B05,
};

const uint16_t Cx_cluster[] PROGMEM = {
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x001A, 0x0020, 0x0100,
  0x0101, 0x0102, 0x0201, 0x0300, 0x0400, 0x0401, 0x0402, 0x0403,
  0x0404, 0x0405, 0x0406, 0x0500, 0x0702, 0x0B01, 0x0B04, 0x0B05,
};

uint16_t CxToCluster(uint8_t cx) {
  if (cx < ARRAY_SIZE(Cx_cluster)) {
    return pgm_read_word(&Cx_cluster[cx]);
  }
  return 0xFFFF;
}

uint8_t ClusterToCx(uint16_t cluster) {
  for (uint8_t i=0; i<ARRAY_SIZE(Cx_cluster); i++) {
    if (pgm_read_word(&Cx_cluster[i]) == cluster) {
      return i;
    }
  }
  return 0xFF;
}

// list of post-processing directives
const Z_AttributeConverter Z_PostProcess[] PROGMEM = {
  { Zuint8,   Cx0000, 0x0000,  Z_(ZCLVersion),           1 },
  { Zuint8,   Cx0000, 0x0001,  Z_(AppVersion),           1 },
  { Zuint8,   Cx0000, 0x0002,  Z_(StackVersion),         1 },
  { Zuint8,   Cx0000, 0x0003,  Z_(HWVersion),            1 },
  { Zstring,  Cx0000, 0x0004,  Z_(Manufacturer),         1 },    // record Manufacturer
  { Zstring,  Cx0000, 0x0005,  Z_(ModelId),              1 },    // record Model
  // { Zstring,  Cx0000, 0x0004,  Z_(Manufacturer),         1,  Z_ManufKeep },    // record Manufacturer
  // { Zstring,  Cx0000, 0x0005,  Z_(ModelId),              1,  Z_ModelKeep },    // record Model
  { Zstring,  Cx0000, 0x0006,  Z_(DateCode),             1 },
  { Zenum8,   Cx0000, 0x0007,  Z_(PowerSource),          1 },
  { Zenum8,   Cx0000, 0x0008,  Z_(GenericDeviceClass),   1 },
  { Zenum8,   Cx0000, 0x0009,  Z_(GenericDeviceType),    1 },
  { Zoctstr,  Cx0000, 0x000A,  Z_(ProductCode),          1 },
  { Zstring,  Cx0000, 0x000B,  Z_(ProductURL),           1 },
  { Zstring,  Cx0000, 0x4000,  Z_(SWBuildID),            1 },
  // { Zunk,     Cx0000, 0xFFFF,  nullptr,                 0 },    // Remove all other values
  // Cmd 0x0A - Cluster 0x0000, attribute 0xFF01 - proprietary
  { Zmap8,    Cx0000, 0xFF01,  Z_(),                     0 },
  { Zmap8,    Cx0000, 0xFF02,  Z_(),                     0 },
  // { Zmap8,    Cx0000, 0xFF01,  Z_(),                     0,  Z_AqaraSensor },
  // { Zmap8,    Cx0000, 0xFF02,  Z_(),                     0,  Z_AqaraSensor2 },

  // Power Configuration cluster
  { Zuint16,  Cx0001, 0x0000,  Z_(MainsVoltage),         1 },
  { Zuint8,   Cx0001, 0x0001,  Z_(MainsFrequency),       1 },
  { Zuint8,   Cx0001, 0x0020,  Z_(BatteryVoltage),       -10 },   // divide by 10
  { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    -2 },   // divide by 2
  // { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    -2, Z_BatteryPercentage },   // divide by 2

  // Device Temperature Configuration cluster
  { Zint16,   Cx0002, 0x0000,  Z_(CurrentTemperature),   1 },
  { Zint16,   Cx0002, 0x0001,  Z_(MinTempExperienced),   1 },
  { Zint16,   Cx0002, 0x0002,  Z_(MaxTempExperienced),   1 },
  { Zuint16,  Cx0002, 0x0003,  Z_(OverTempTotalDwell),   1 },

  // Identify cluster
  { Zuint16,  Cx0003, 0x0000,  Z_(IdentifyTime),         1 },

  // Groups cluster
  { Zmap8,    Cx0004, 0x0000,  Z_(GroupNameSupport),     1 },

  // Scenes cluster
  { Zuint8,   Cx0005, 0x0000,  Z_(SceneCount),           1 },
  { Zuint8,   Cx0005, 0x0001,  Z_(CurrentScene),         1 },
  { Zuint16,  Cx0005, 0x0002,  Z_(CurrentGroup),         1 },
  { Zbool,    Cx0005, 0x0003,  Z_(SceneValid),           1 },
  //{ Zmap8,    Cx0005, 0x0004,  (NameSupport),           1 },

  // On/off cluster
  { Zbool,    Cx0006,    0x0000,  Z_(Power),             1 },
  { Zenum8,   Cx0006,    0x4003,  Z_(StartUpOnOff),      1 },
  { Zbool,    Cx0006,    0x8000,  Z_(Power),             1 },   // See 7280

  // On/Off Switch Configuration cluster
  { Zenum8,   Cx0007, 0x0000,  Z_(SwitchType),           1 },

  // Level Control cluster
  { Zuint8,   Cx0008, 0x0000,  Z_(Dimmer),               1 },
  { Zmap8,    Cx0008, 0x000F,  Z_(DimmerOptions),        1 },
  { Zuint16,  Cx0008, 0x0001,  Z_(DimmerRemainingTime),  1 },
  { Zuint16,  Cx0008, 0x0010,  Z_(OnOffTransitionTime),   1 },
  // { Zuint8, Cx0008, 0x0011,  (OnLevel),              1 },
  // { Zuint16, Cx0008, 0x0012,  (OnTransitionTime),     1 },
  // { Zuint16, Cx0008, 0x0013,  (OffTransitionTime),    1 },
  // { Zuint16, Cx0008, 0x0014,  (DefaultMoveRate),      1 },

  // Alarms cluster
  { Zuint16,  Cx0009, 0x0000,  Z_(AlarmCount),           1 },

  // Time cluster
  { ZUTC,     Cx000A, 0x0000,  Z_(Time),                 1 },
  { Zmap8,    Cx000A, 0x0001,  Z_(TimeStatus),           1 },
  { Zint32,   Cx000A, 0x0002,  Z_(TimeZone),             1 },
  { Zuint32,  Cx000A, 0x0003,  Z_(DstStart),             1 },
  { Zuint32,  Cx000A, 0x0004,  Z_(DstEnd),               1 },
  { Zint32,   Cx000A, 0x0005,  Z_(DstShift),             1 },
  { Zuint32,  Cx000A, 0x0006,  Z_(StandardTime),         1 },
  { Zuint32,  Cx000A, 0x0007,  Z_(LocalTime),            1 },
  { ZUTC,     Cx000A, 0x0008,  Z_(LastSetTime),          1 },
  { ZUTC,     Cx000A, 0x0009,  Z_(ValidUntilTime),       1 },
  { ZUTC,     Cx000A, 0xFF00,  Z_(TimeEpoch),            1 },    // Tasmota specific, epoch

  // RSSI Location cluster
  { Zdata8,   Cx000B, 0x0000,  Z_(LocationType),         1 },
  { Zenum8,   Cx000B, 0x0001,  Z_(LocationMethod),       1 },
  { Zuint16,  Cx000B, 0x0002,  Z_(LocationAge),          1 },
  { Zuint8,   Cx000B, 0x0003,  Z_(QualityMeasure),       1 },
  { Zuint8,   Cx000B, 0x0004,  Z_(NumberOfDevices),      1 },

  // Analog Input cluster
  // { 0xFF, Cx000C, 0x0004,  (AnalogInActiveText),   1 },
  { Zstring,  Cx000C, 0x001C,  Z_(AnalogInDescription),  1 },
  // { 0xFF, Cx000C, 0x002E,  (AnalogInInactiveText), 1 },
  { Zsingle,  Cx000C, 0x0041,  Z_(AnalogInMaxValue),     1 },
  { Zsingle,  Cx000C, 0x0045,  Z_(AnalogInMinValue),     1 },
  { Zbool,    Cx000C, 0x0051,  Z_(AnalogInOutOfService), 1 },
  { Zsingle,  Cx000C, 0x0055,  Z_(AqaraRotate),          1 },
  // { 0xFF, Cx000C, 0x0057,  (AnalogInPriorityArray),1 },
  { Zenum8,   Cx000C, 0x0067,  Z_(AnalogInReliability),  1 },
  // { 0xFF, Cx000C, 0x0068,  (AnalogInRelinquishDefault),1 },
  { Zsingle,  Cx000C, 0x006A,  Z_(AnalogInResolution),   1 },
  { Zmap8,    Cx000C, 0x006F,  Z_(AnalogInStatusFlags),  1 },
  { Zenum16,  Cx000C, 0x0075,  Z_(AnalogInEngineeringUnits),1 },
  { Zuint32,  Cx000C, 0x0100,  Z_(AnalogInApplicationType),1 },
  { Zuint16,  Cx000C, 0xFF05,  Z_(Aqara_FF05),           1 },

  // Analog Output cluster
  { Zstring,  Cx000D, 0x001C,  Z_(AnalogOutDescription), 1 },
  { Zsingle,  Cx000D, 0x0041,  Z_(AnalogOutMaxValue),    1 },
  { Zsingle,  Cx000D, 0x0045,  Z_(AnalogOutMinValue),    1 },
  { Zbool,    Cx000D, 0x0051,  Z_(AnalogOutOutOfService),1 },
  { Zsingle,  Cx000D, 0x0055,  Z_(AnalogOutValue),       1 },
  // { Zunk,     Cx000D, 0x0057,  (AnalogOutPriorityArray),1 },
  { Zenum8,   Cx000D, 0x0067,  Z_(AnalogOutReliability), 1 },
  { Zsingle,  Cx000D, 0x0068,  Z_(AnalogOutRelinquishDefault),1 },
  { Zsingle,  Cx000D, 0x006A,  Z_(AnalogOutResolution),  1 },
  { Zmap8,    Cx000D, 0x006F,  Z_(AnalogOutStatusFlags), 1 },
  { Zenum16,  Cx000D, 0x0075,  Z_(AnalogOutEngineeringUnits),1 },
  { Zuint32,  Cx000D, 0x0100,  Z_(AnalogOutApplicationType),1 },

  // Analog Value cluster
  { Zstring,  Cx000E, 0x001C,  Z_(AnalogDescription),    1 },
  { Zbool,    Cx000E, 0x0051,  Z_(AnalogOutOfService),   1 },
  { Zsingle,  Cx000E, 0x0055,  Z_(AnalogValue),          1 },
  { Zunk,     Cx000E, 0x0057,  Z_(AnalogPriorityArray),  1 },
  { Zenum8,   Cx000E, 0x0067,  Z_(AnalogReliability),    1 },
  { Zsingle,  Cx000E, 0x0068,  Z_(AnalogRelinquishDefault),1 },
  { Zmap8,    Cx000E, 0x006F,  Z_(AnalogStatusFlags),    1 },
  { Zenum16,  Cx000E, 0x0075,  Z_(AnalogEngineeringUnits),1 },
  { Zuint32,  Cx000E, 0x0100,  Z_(AnalogApplicationType),1 },

  // Binary Input cluster
  { Zstring,  Cx000F, 0x0004,  Z_(BinaryInActiveText),  1 },
  { Zstring,  Cx000F, 0x001C,  Z_(BinaryInDescription), 1 },
  { Zstring,  Cx000F, 0x002E,  Z_(BinaryInInactiveText),1 },
  { Zbool,    Cx000F, 0x0051,  Z_(BinaryInOutOfService),1 },
  { Zenum8,   Cx000F, 0x0054,  Z_(BinaryInPolarity),    1 },
  { Zstring,  Cx000F, 0x0055,  Z_(BinaryInValue),       1 },
  // { 0xFF, Cx000F, 0x0057,  (BinaryInPriorityArray),1 },
  { Zenum8,   Cx000F, 0x0067,  Z_(BinaryInReliability), 1 },
  { Zmap8,    Cx000F, 0x006F,  Z_(BinaryInStatusFlags), 1 },
  { Zuint32,  Cx000F, 0x0100,  Z_(BinaryInApplicationType),1 },

  // Binary Output cluster
  { Zstring,  Cx0010, 0x0004,  Z_(BinaryOutActiveText),  1 },
  { Zstring,  Cx0010, 0x001C,  Z_(BinaryOutDescription), 1 },
  { Zstring,  Cx0010, 0x002E,  Z_(BinaryOutInactiveText),1 },
  { Zuint32,  Cx0010, 0x0042,  Z_(BinaryOutMinimumOffTime),1 },
  { Zuint32,  Cx0010, 0x0043,  Z_(BinaryOutMinimumOnTime),1 },
  { Zbool,    Cx0010, 0x0051,  Z_(BinaryOutOutOfService),1 },
  { Zenum8,   Cx0010, 0x0054,  Z_(BinaryOutPolarity),    1 },
  { Zbool,    Cx0010, 0x0055,  Z_(BinaryOutValue),       1 },
  // { Zunk,     Cx0010, 0x0057,  (BinaryOutPriorityArray),1 },
  { Zenum8,   Cx0010, 0x0067,  Z_(BinaryOutReliability), 1 },
  { Zbool,    Cx0010, 0x0068,  Z_(BinaryOutRelinquishDefault),1 },
  { Zmap8,    Cx0010, 0x006F,  Z_(BinaryOutStatusFlags), 1 },
  { Zuint32,  Cx0010, 0x0100,  Z_(BinaryOutApplicationType),1 },

  // Binary Value cluster
  { Zstring,  Cx0011, 0x0004,  Z_(BinaryActiveText),     1 },
  { Zstring,  Cx0011, 0x001C,  Z_(BinaryDescription),    1 },
  { Zstring,  Cx0011, 0x002E,  Z_(BinaryInactiveText),   1 },
  { Zuint32,  Cx0011, 0x0042,  Z_(BinaryMinimumOffTime), 1 },
  { Zuint32,  Cx0011, 0x0043,  Z_(BinaryMinimumOnTime),  1 },
  { Zbool,    Cx0011, 0x0051,  Z_(BinaryOutOfService),   1 },
  { Zbool,    Cx0011, 0x0055,  Z_(BinaryValue),          1 },
  // { Zunk,     Cx0011, 0x0057,  (BinaryPriorityArray),  1 },
  { Zenum8,   Cx0011, 0x0067,  Z_(BinaryReliability),    1 },
  { Zbool,    Cx0011, 0x0068,  Z_(BinaryRelinquishDefault),1 },
  { Zmap8,    Cx0011, 0x006F,  Z_(BinaryStatusFlags),    1 },
  { Zuint32,  Cx0011, 0x0100,  Z_(BinaryApplicationType),1 },

  // Multistate Input cluster
  // { Zunk,     Cx0012, 0x000E,  (MultiInStateText),     1 },
  { Zstring,  Cx0012, 0x001C,  Z_(MultiInDescription),   1 },
  { Zuint16,  Cx0012, 0x004A,  Z_(MultiInNumberOfStates),1 },
  { Zbool,    Cx0012, 0x0051,  Z_(MultiInOutOfService),  1 },
  { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         1 },
  // { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         0,  Z_AqaraCube },
  // { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         0,  Z_AqaraButton },
  { Zenum8,   Cx0012, 0x0067,  Z_(MultiInReliability),   1 },
  { Zmap8,    Cx0012, 0x006F,  Z_(MultiInStatusFlags),   1 },
  { Zuint32,  Cx0012, 0x0100,  Z_(MultiInApplicationType),1 },

  // Multistate output
  // { Zunk,     Cx0013, 0x000E,  (MultiOutStateText),    1 },
  { Zstring,  Cx0013, 0x001C,  Z_(MultiOutDescription),  1 },
  { Zuint16,  Cx0013, 0x004A,  Z_(MultiOutNumberOfStates),1 },
  { Zbool,    Cx0013, 0x0051,  Z_(MultiOutOutOfService), 1 },
  { Zuint16,  Cx0013, 0x0055,  Z_(MultiOutValue),        1 },
  // { Zunk,     Cx0013, 0x0057,  (MultiOutPriorityArray),1 },
  { Zenum8,   Cx0013, 0x0067,  Z_(MultiOutReliability),  1 },
  { Zuint16,  Cx0013, 0x0068,  Z_(MultiOutRelinquishDefault),1 },
  { Zmap8,    Cx0013, 0x006F,  Z_(MultiOutStatusFlags),  1 },
  { Zuint32,  Cx0013, 0x0100,  Z_(MultiOutApplicationType),1 },

  // Multistate Value cluster
  // { Zunk,     Cx0014, 0x000E,  (MultiStateText),       1 },
  { Zstring,  Cx0014, 0x001C,  Z_(MultiDescription),     1 },
  { Zuint16,  Cx0014, 0x004A,  Z_(MultiNumberOfStates),  1 },
  { Zbool,    Cx0014, 0x0051,  Z_(MultiOutOfService),    1 },
  { Zuint16,  Cx0014, 0x0055,  Z_(MultiValue),           1 },
  { Zenum8,   Cx0014, 0x0067,  Z_(MultiReliability),     1 },
  { Zuint16,  Cx0014, 0x0068,  Z_(MultiRelinquishDefault),1 },
  { Zmap8,    Cx0014, 0x006F,  Z_(MultiStatusFlags),     1 },
  { Zuint32,  Cx0014, 0x0100,  Z_(MultiApplicationType), 1 },

  // Power Profile cluster
  { Zuint8,   Cx001A, 0x0000,  Z_(TotalProfileNum),      1 },
  { Zbool,    Cx001A, 0x0001,  Z_(MultipleScheduling),   1 },
  { Zmap8,    Cx001A, 0x0002,  Z_(EnergyFormatting),     1 },
  { Zbool,    Cx001A, 0x0003,  Z_(EnergyRemote),         1 },
  { Zmap8,    Cx001A, 0x0004,  Z_(ScheduleMode),         1 },

  // Poll Control cluster
  { Zuint32,  Cx0020, 0x0000,  Z_(CheckinInterval),      1 },
  { Zuint32,  Cx0020, 0x0001,  Z_(LongPollInterval),     1 },
  { Zuint16,  Cx0020, 0x0002,  Z_(ShortPollInterval),    1 },
  { Zuint16,  Cx0020, 0x0003,  Z_(FastPollTimeout),      1 },
  { Zuint32,  Cx0020, 0x0004,  Z_(CheckinIntervalMin),   1 },
  { Zuint32,  Cx0020, 0x0005,  Z_(LongPollIntervalMin),  1 },
  { Zuint16,  Cx0020, 0x0006,  Z_(FastPollTimeoutMax),   1 },

  // Shade Configuration cluster
  { Zuint16,  Cx0100, 0x0000,  Z_(PhysicalClosedLimit),  1 },
  { Zuint8,   Cx0100, 0x0001,  Z_(MotorStepSize),        1 },
  { Zmap8,    Cx0100, 0x0002,  Z_(Status),               1 },
  { Zuint16,  Cx0100, 0x0010,  Z_(ClosedLimit),          1 },
  { Zenum8,   Cx0100, 0x0011,  Z_(Mode),                 1 },

  // Door Lock cluster
  { Zenum8,   Cx0101, 0x0000,  Z_(LockState),            1 },
  { Zenum8,   Cx0101, 0x0001,  Z_(LockType),             1 },
  { Zbool,    Cx0101, 0x0002,  Z_(ActuatorEnabled),      1 },
  { Zenum8,   Cx0101, 0x0003,  Z_(DoorState),            1 },
  { Zuint32,  Cx0101, 0x0004,  Z_(DoorOpenEvents),       1 },
  { Zuint32,  Cx0101, 0x0005,  Z_(DoorClosedEvents),     1 },
  { Zuint16,  Cx0101, 0x0006,  Z_(OpenPeriod),           1 },

  // Aqara Lumi Vibration Sensor
  { Zuint16,  Cx0101, 0x0055,  Z_(AqaraVibrationMode),   1 },
  { Zuint16,  Cx0101, 0x0503,  Z_(AqaraVibrationsOrAngle), 1 },
  { Zuint32,  Cx0101, 0x0505,  Z_(AqaraVibration505),    1 },
  { Zuint48,  Cx0101, 0x0508,  Z_(AqaraAccelerometer),   1 },

  // Window Covering cluster
  { Zenum8,   Cx0102, 0x0000,  Z_(WindowCoveringType),   1 },
  { Zuint16,  Cx0102, 0x0001,  Z_(PhysicalClosedLimitLift),1 },
  { Zuint16,  Cx0102, 0x0002,  Z_(PhysicalClosedLimitTilt),1 },
  { Zuint16,  Cx0102, 0x0003,  Z_(CurrentPositionLift),  1 },
  { Zuint16,  Cx0102, 0x0004,  Z_(CurrentPositionTilt),  1 },
  { Zuint16,  Cx0102, 0x0005,  Z_(NumberofActuationsLift),1 },
  { Zuint16,  Cx0102, 0x0006,  Z_(NumberofActuationsTilt),1 },
  { Zmap8,    Cx0102, 0x0007,  Z_(ConfigStatus),         1 },
  { Zuint8,   Cx0102, 0x0008,  Z_(CurrentPositionLiftPercentage),1 },
  { Zuint8,   Cx0102, 0x0009,  Z_(CurrentPositionTiltPercentage),1 },
  { Zuint16,  Cx0102, 0x0010,  Z_(InstalledOpenLimitLift),1 },
  { Zuint16,  Cx0102, 0x0011,  Z_(InstalledClosedLimitLift),1 },
  { Zuint16,  Cx0102, 0x0012,  Z_(InstalledOpenLimitTilt),1 },
  { Zuint16,  Cx0102, 0x0013,  Z_(InstalledClosedLimitTilt),1 },
  { Zuint16,  Cx0102, 0x0014,  Z_(VelocityLift),         1 },
  { Zuint16,  Cx0102, 0x0015,  Z_(AccelerationTimeLift),1 },
  { Zuint16,  Cx0102, 0x0016,  Z_(DecelerationTimeLift), 1 },
  { Zmap8,    Cx0102, 0x0017,  Z_(Mode),                 1 },
  { Zoctstr,  Cx0102, 0x0018,  Z_(IntermediateSetpointsLift),1 },
  { Zoctstr,  Cx0102, 0x0019,  Z_(IntermediateSetpointsTilt),1 },

  // Thermostat
  { Zint16,   Cx0201, 0x0000,  Z_(LocalTemperature),  -100 },
  { Zint16,   Cx0201, 0x0001,  Z_(OutdoorTemperature),-100 },
  { Zuint8,   Cx0201, 0x0007,  Z_(PICoolingDemand),      1 },
  { Zuint8,   Cx0201, 0x0008,  Z_(PIHeatingDemand),      1 },
  { Zint8,    Cx0201, 0x0010,  Z_(LocalTemperatureCalibration), -10 },
  { Zint16,   Cx0201, 0x0011,  Z_(OccupiedCoolingSetpoint), -100 },
  { Zint16,   Cx0201, 0x0012,  Z_(OccupiedHeatingSetpoint), -100 },
  { Zint16,   Cx0201, 0x0013,  Z_(UnoccupiedCoolingSetpoint), -100 },
  { Zint16,   Cx0201, 0x0014,  Z_(UnoccupiedHeatingSetpoint), -100 },
  { Zmap8,    Cx0201, 0x001A,  Z_(RemoteSensing),        1 },
  { Zenum8,   Cx0201, 0x001B,  Z_(ControlSequenceOfOperation), 1 },
  { Zenum8,   Cx0201, 0x001C,  Z_(SystemMode),           1 },
  // below is Eurotronic specific
  { Zenum8,   Cx0201, 0x4000, Z_(TRVMode),               1 },
  { Zuint8,   Cx0201, 0x4001, Z_(SetValvePosition),      1 },
  { Zuint8,   Cx0201, 0x4002, Z_(EurotronicErrors),      1 },
  { Zint16,   Cx0201, 0x4003, Z_(CurrentTemperatureSetPoint), -100 },

  // Color Control cluster
  { Zuint8,   Cx0300, 0x0000,  Z_(Hue),                  1 },
  { Zuint8,   Cx0300, 0x0001,  Z_(Sat),                  1 },
  { Zuint16,  Cx0300, 0x0002,  Z_(RemainingTime),        1 },
  { Zuint16,  Cx0300, 0x0003,  Z_(X),                    1 },
  { Zuint16,  Cx0300, 0x0004,  Z_(Y),                    1 },
  { Zenum8,   Cx0300, 0x0005,  Z_(DriftCompensation),    1 },
  { Zstring,  Cx0300, 0x0006,  Z_(CompensationText),     1 },
  { Zuint16,  Cx0300, 0x0007,  Z_(CT),                   1 },
  { Zenum8,   Cx0300, 0x0008,  Z_(ColorMode),            1 },
  { Zuint8,   Cx0300, 0x0010,  Z_(NumberOfPrimaries),    1 },
  { Zuint16,  Cx0300, 0x0011,  Z_(Primary1X),            1 },
  { Zuint16,  Cx0300, 0x0012,  Z_(Primary1Y),            1 },
  { Zuint8,   Cx0300, 0x0013,  Z_(Primary1Intensity),    1 },
  { Zuint16,  Cx0300, 0x0015,  Z_(Primary2X),            1 },
  { Zuint16,  Cx0300, 0x0016,  Z_(Primary2Y),            1 },
  { Zuint8,   Cx0300, 0x0017,  Z_(Primary2Intensity),    1 },
  { Zuint16,  Cx0300, 0x0019,  Z_(Primary3X),            1 },
  { Zuint16,  Cx0300, 0x001A,  Z_(Primary3Y),            1 },
  { Zuint8,   Cx0300, 0x001B,  Z_(Primary3Intensity),    1 },
  { Zuint16,  Cx0300, 0x0030,  Z_(WhitePointX),          1 },
  { Zuint16,  Cx0300, 0x0031,  Z_(WhitePointY),          1 },
  { Zuint16,  Cx0300, 0x0032,  Z_(ColorPointRX),         1 },
  { Zuint16,  Cx0300, 0x0033,  Z_(ColorPointRY),         1 },
  { Zuint8,   Cx0300, 0x0034,  Z_(ColorPointRIntensity), 1 },
  { Zuint16,  Cx0300, 0x0036,  Z_(ColorPointGX),         1 },
  { Zuint16,  Cx0300, 0x0037,  Z_(ColorPointGY),         1 },
  { Zuint8,   Cx0300, 0x0038,  Z_(ColorPointGIntensity), 1 },
  { Zuint16,  Cx0300, 0x003A,  Z_(ColorPointBX),         1 },
  { Zuint16,  Cx0300, 0x003B,  Z_(ColorPointBY),         1 },
  { Zuint8,   Cx0300, 0x003C,  Z_(ColorPointBIntensity), 1 },

  // Illuminance Measurement cluster
  { Zuint16,  Cx0400, 0x0000,  Z_(Illuminance),          1 },    // Illuminance (in Lux)
  { Zuint16,  Cx0400, 0x0001,  Z_(IlluminanceMinMeasuredValue),     1 },    //
  { Zuint16,  Cx0400, 0x0002,  Z_(IlluminanceMaxMeasuredValue),     1 },    //
  { Zuint16,  Cx0400, 0x0003,  Z_(IlluminanceTolerance),            1 },    //
  { Zenum8,   Cx0400, 0x0004,  Z_(IlluminanceLightSensorType),      1 },    //
  { Zunk,     Cx0400, 0xFFFF,  Z_(),                    0 },    // Remove all other values

  // Illuminance Level Sensing cluster
  { Zenum8,   Cx0401, 0x0000,  Z_(IlluminanceLevelStatus),          1 },    // Illuminance (in Lux)
  { Zenum8,   Cx0401, 0x0001,  Z_(IlluminanceLightSensorType),      1 },    // LightSensorType
  { Zuint16,  Cx0401, 0x0010,  Z_(IlluminanceTargetLevel),          1 },    //
  { Zunk,     Cx0401, 0xFFFF,  Z_(),                    0 },    // Remove all other values

  // Temperature Measurement cluster
  { Zint16,   Cx0402, 0x0000,  Z_(Temperature),          -100 },   // divide by 100
  { Zint16,   Cx0402, 0x0001,  Z_(TemperatureMinMeasuredValue),     -100 },    //
  { Zint16,   Cx0402, 0x0002,  Z_(TemperatureMaxMeasuredValue),     -100 },    //
  { Zuint16,  Cx0402, 0x0003,  Z_(TemperatureTolerance),            -100 },    //
  { Zunk,     Cx0402, 0xFFFF,  Z_(),                    0 },     // Remove all other values

  // Pressure Measurement cluster
  { Zint16,   Cx0403, 0x0000,  Z_(Pressure),                     1 },     // Pressure
  { Zint16,   Cx0403, 0x0001,  Z_(PressureMinMeasuredValue),     1 },    //
  { Zint16,   Cx0403, 0x0002,  Z_(PressureMaxMeasuredValue),     1 },    //
  { Zuint16,  Cx0403, 0x0003,  Z_(PressureTolerance),            1 },    //
  { Zint16,   Cx0403, 0x0010,  Z_(PressureScaledValue),          1 },    //
  { Zint16,   Cx0403, 0x0011,  Z_(PressureMinScaledValue),       1 },    //
  { Zint16,   Cx0403, 0x0012,  Z_(PressureMaxScaledValue),       1 },    //
  { Zuint16,  Cx0403, 0x0013,  Z_(PressureScaledTolerance),      1 },    //
  { Zint8,    Cx0403, 0x0014,  Z_(PressureScale),                1 },    //
  { Zint16,   Cx0403, 0xFF00,  Z_(SeaPressure),                  1 },     // Pressure at Sea Level, Tasmota specific
  { Zunk,     Cx0403, 0xFFFF,  Z_(),                    0 },     // Remove all other Pressure values

  // Flow Measurement cluster
  { Zuint16,  Cx0404, 0x0000,  Z_(FlowRate),             -10 },    // Flow (in m3/h)
  { Zuint16,  Cx0404, 0x0001,  Z_(FlowMinMeasuredValue), 1 },    //
  { Zuint16,  Cx0404, 0x0002,  Z_(FlowMaxMeasuredValue), 1 },    //
  { Zuint16,  Cx0404, 0x0003,  Z_(FlowTolerance),        1 },    //
  { Zunk,     Cx0404, 0xFFFF,  Z_(),                     0 },    // Remove all other values

  // Relative Humidity Measurement cluster
  { Zuint16,  Cx0405, 0x0000,  Z_(Humidity),                     -100 },   // Humidity
  { Zuint16,  Cx0405, 0x0001,  Z_(HumidityMinMeasuredValue),     1 },    //
  { Zuint16,  Cx0405, 0x0002,  Z_(HumidityMaxMeasuredValue),     1 },    //
  { Zuint16,  Cx0405, 0x0003,  Z_(HumidityTolerance),            1 },    //
  { Zunk,     Cx0405, 0xFFFF,  Z_(),                    0 },     // Remove all other values

  // Occupancy Sensing cluster
  { Zmap8,    Cx0406, 0x0000,  Z_(Occupancy),            1 },    // Occupancy (map8)
  { Zenum8,   Cx0406, 0x0001,  Z_(OccupancySensorType),  1 },    // OccupancySensorType
  { Zunk,     Cx0406, 0xFFFF,  Z_(),                    0 },    // Remove all other values

  // IAS Cluster (Intruder Alarm System)
  { Zenum8,   Cx0500, 0x0000,  Z_(ZoneState),            1 },    // Occupancy (map8)
  { Zenum16,  Cx0500, 0x0001,  Z_(ZoneType),             1 },    // Occupancy (map8)
  { Zmap16,   Cx0500, 0x0002,  Z_(ZoneStatus),           1 },    // Occupancy (map8)

  // Metering (Smart Energy) cluster
  { Zuint48,  Cx0702, 0x0000,  Z_(CurrentSummDelivered), 1 },

  // Meter Identification cluster
  { Zstring,  Cx0B01, 0x0000,  Z_(CompanyName),          1 },
  { Zuint16,  Cx0B01, 0x0001,  Z_(MeterTypeID),          1 },
  { Zuint16,  Cx0B01, 0x0004,  Z_(DataQualityID),        1 },
  { Zstring,  Cx0B01, 0x0005,  Z_(CustomerName),         1 },
  { Zoctstr,  Cx0B01, 0x0006,  Z_(Model),                1 },
  { Zoctstr,  Cx0B01, 0x0007,  Z_(PartNumber),           1 },
  { Zoctstr,  Cx0B01, 0x0008,  Z_(ProductRevision),      1 },
  { Zoctstr,  Cx0B01, 0x000A,  Z_(SoftwareRevision),     1 },
  { Zstring,  Cx0B01, 0x000B,  Z_(UtilityName),          1 },
  { Zstring,  Cx0B01, 0x000C,  Z_(POD),                  1 },
  { Zint24,   Cx0B01, 0x000D,  Z_(AvailablePower),       1 },
  { Zint24,   Cx0B01, 0x000E,  Z_(PowerThreshold),       1 },

  // Electrical Measurement cluster
  { Zuint16,  Cx0B04, 0x0505,  Z_(RMSVoltage),            1 },
  { Zuint16,  Cx0B04, 0x0508,  Z_(RMSCurrent),            1 },
  { Zint16,   Cx0B04, 0x050B,  Z_(ActivePower),           1 },

  // Diagnostics cluster
  { Zuint16,  Cx0B05, 0x0000,  Z_(NumberOfResets),       1 },
  { Zuint16,  Cx0B05, 0x0001,  Z_(PersistentMemoryWrites),1 },
  { Zuint8,   Cx0B05, 0x011C,  Z_(LastMessageLQI),       1 },
  { Zuint8,   Cx0B05, 0x011D,  Z_(LastMessageRSSI),      1 },

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
                                    uint16_t *cluster, uint16_t *attribute, int8_t *multiplier) {
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    if (0 == pgm_read_word(&converter->name_offset)) { continue; }         // avoid strcasecmp_P() from crashing
    if (0 == strcasecmp_P(command, Z_strings + pgm_read_word(&converter->name_offset))) {
      if (cluster)      { *cluster    = CxToCluster(pgm_read_byte(&converter->cluster_short)); }
      if (attribute)    { *attribute  = pgm_read_word(&converter->attribute); }
      if (multiplier)   { *multiplier = pgm_read_byte(&converter->multiplier); }
      return (const __FlashStringHelper*) (Z_strings + pgm_read_word(&converter->name_offset));
    }
  }
  return nullptr;
}

//
// Find attribute details: Name, Type, Multiplier by cuslter/attr_id
//
const __FlashStringHelper* zigbeeFindAttributeById(uint16_t cluster, uint16_t attr_id,
                                      uint8_t *attr_type, int8_t *multiplier) {
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
    uint16_t conv_attr_id = pgm_read_word(&converter->attribute);

    if ((conv_cluster == cluster) && (conv_attr_id == attr_id)) {
      if (multiplier)   { *multiplier = pgm_read_byte(&converter->multiplier); }
      if (attr_type)    { *attr_type  = pgm_read_byte(&converter->type); }
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

  void parseReportAttributes(Z_attribute_list& attr_list);
  void generateSyntheticAttributes(Z_attribute_list& attr_list);
  void computeSyntheticAttributes(Z_attribute_list& attr_list);
  void generateCallBacks(Z_attribute_list& attr_list);
  void parseReadAttributes(Z_attribute_list& attr_list);
  void parseReadAttributesResponse(Z_attribute_list& attr_list);
  void parseReadConfigAttributes(Z_attribute_list& attr_list);
  void parseConfigAttributes(Z_attribute_list& attr_list);
  void parseResponse(void);
  void parseResponseOld(void);
  void parseClusterSpecificCommand(Z_attribute_list& attr_list);
  void postProcessAttributes(uint16_t shortaddr, Z_attribute_list& attr_list);

  // synthetic attributes converters
  void syntheticAqaraSensor(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraSensor2(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraCubeOrButton(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraVibration(Z_attribute_list &attr_list, class Z_attribute &attr);


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
//   attr: attribute object to store to
//   buf:  the buffer to read from
//   offset: location in the buffer to read from
//   attrtype: type of attribute (byte) or -1 to read from the stream as first byte
// Output:
//   return: the length in bytes of the attribute
uint32_t parseSingleAttribute(Z_attribute & attr, const SBuffer &buf,
                              uint32_t offset, int32_t attrtype = -1) {

  uint32_t i = offset;
  if (attrtype < 0) {
    attrtype = buf.get8(i++);
  }

  // fallback - enter a null value
  attr.setNone();   // set to null by default

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
          attr.setUInt(uint8_val);
        }
      }
      break;
    case Zuint16:      // uint16
    case Zenum16:      // enum16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        if (0xFFFF != uint16_val) {
          attr.setUInt(uint16_val);
        }
      }
      break;
    case Zuint32:      // uint32
    case ZUTC:      // UTC
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        if (0xFFFFFFFF != uint32_val) {
          attr.setUInt(uint32_val);
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
        // print as HEX "0x...."
        char hex[2*len+3];
        snprintf_P(hex, sizeof(hex), PSTR("0x"));
        for (uint32_t j=0; j<len; j++) {
          snprintf_P(hex, sizeof(hex), PSTR("%s%02X"), hex, buf.get8(i+len-j-1));
        }
        attr.setStr(hex);
        // i += len;
      }
      break;
    case Zint8:      // int8
      {
        int8_t int8_val = buf.get8(i);
        // i += 1;
        if (0x80 != int8_val) {
          attr.setInt(int8_val);
        }
      }
      break;
    case Zint16:      // int16
      {
        int16_t int16_val = buf.get16(i);
        // i += 2;
        if (0x8000 != int16_val) {
          attr.setInt(int16_val);
        }
      }
      break;
    case Zint32:      // int32
      {
        int32_t int32_val = buf.get32(i);
        // i += 4;
        if (0x80000000 != int32_val) {
          attr.setInt(int32_val);
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
          attr.setStr(str);
        } else {
          attr.setBuf(buf, i, len);
        }

        // i += len;
        // break;
      }
      // i += buf.get8(i) + 1;
      break;

    case Zstruct:
      {
        uint16_t struct_size = buf.get16(i);
        len = 2;
        if (0xFFFF != struct_size) {
          if (struct_size > 16) { struct_size = 16; }
          // parse inner attributes - supports only fixed length for now
          for (uint32_t j = 0; j < struct_size; j++) {
            uint8_t attr_type = buf.get8(i+len);
            len += Z_getDatatypeLen(attr_type) + 1;
          }
        attr.setBuf(buf, i, len);
        }
      }
      break;

    case Zdata8:      // data8
    case Zmap8:      // map8
      {
        uint8_t uint8_val = buf.get8(i);
        // i += 1;
        attr.setUInt(uint8_val);
      }
      break;
    case Zdata16:      // data16
    case Zmap16:      // map16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        attr.setUInt(uint16_val);
      }
      break;
    case Zdata32:      // data32
    case Zmap32:      // map32
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        attr.setUInt(uint32_val);
      }
      break;

    case Zsingle:      // float
      {
        uint32_t uint32_val = buf.get32(i);
        float  * float_val = (float*) &uint32_val;
        // i += 4;
        attr.setFloat(*float_val);
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
        attr.setFloat((float) *double_val);
      }
      break;
  }
  i += len;

  return i - offset;    // how much have we increased the index
}

// First pass, parse all attributes in their native format
void ZCLFrame::parseReportAttributes(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = _payload.len();

  while (len >= i + 3) {
    uint16_t attrid = _payload.get16(i);
    i += 2;

    // exception for Xiaomi lumi.weather - specific field to be treated as octet and not char
    if ((0x0000 == _cluster_id) && (0xFF01 == attrid)) {
      if (0x42 == _payload.get8(i)) {
        _payload.set8(i, 0x41);   // change type from 0x42 to 0x41
      }
    }

    // TODO look for suffix
    Z_attribute & attr = attr_list.addAttribute(_cluster_id, attrid);

    i += parseSingleAttribute(attr, _payload, i);
  }

  // Issue Philips outdoor motion sensor SML002, see https://github.com/Koenkk/zigbee2mqtt/issues/897
  // The sensor expects the coordinator to send a Default Response to acknowledge the attribute reporting
  if (0 == _frame_control.b.disable_def_resp) {
    // the device expects a default response
    SBuffer buf(2);
    buf.add8(_cmd_id);
    buf.add8(0x00);   // Status = OK

    ZigbeeZCLSend_Raw(ZigbeeZCLSendMessage({
      _srcaddr,
      0x0000,
      _cluster_id,
      _srcendpoint,
      ZCL_DEFAULT_RESPONSE,
      _manuf_code,
      false /* not cluster specific */,
      false /* noresponse */,
      _transact_seq,  /* zcl transaction id */
      buf.getBuffer(), buf.len()
    }));
  }
}

//
// Extract attributes hidden in other compound attributes
//
void ZCLFrame::generateSyntheticAttributes(Z_attribute_list& attr_list) {
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.key.id.cluster << 16) | attr.key.id.attr_id;

    switch (ccccaaaa) {      // 0xccccaaaa . c=cluster, a=attribute
      case 0x0000FF01:
        syntheticAqaraSensor(attr_list, attr);
        break;
      case 0x0000FF02:
        syntheticAqaraSensor2(attr_list, attr);
        break;
      case 0x00120055:
        syntheticAqaraCubeOrButton(attr_list, attr);
        break;
      case 0x01010055:
      case 0x01010508:
        syntheticAqaraVibration(attr_list, attr);
        break;
    }
  }
}

//
// Compute new attributes based on the standard set
// Note: both function are now split to compute on extracted attributes
//
void ZCLFrame::computeSyntheticAttributes(Z_attribute_list& attr_list) {
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.key.id.cluster << 16) | attr.key.id.attr_id;

    switch (ccccaaaa) {      // 0xccccaaaa . c=cluster, a=attribute
      case 0x00010020:       // BatteryVoltage
        if (attr_list.countAttribute(0x0001,0x0021) == 0) {   // if it does not already contain BatteryPercentage
          uint32_t mv = attr.getUInt()*100;
          attr_list.addAttribute(0x0001, 0x0021).setUInt(toPercentageCR2032(mv) * 2);
        }
        break;
      case 0x02010008:    // Pi Heating Demand - solve Eutotronic bug
        {
          const char * manufacturer_c = zigbee_devices.getManufacturerId(_srcaddr);  // null if unknown
          String manufacturerId((char*) manufacturer_c);
          if (manufacturerId.equals(F("Eurotronic"))) {
            // Eurotronic does not report 0..100 but 0..255, including 255 which is normally an ivalid value
            uint8_t valve = attr.getUInt();
            if (attr.isNone()) { valve = 255; }
            uint8_t valve_100 = changeUIntScale(valve, 0, 255, 0, 100);
            attr.setUInt(valve_100);
          }
        }
        break;
      case 0x04030000:    // Pressure
        {
          int16_t pressure = attr.getInt();
          int16_t pressure_sealevel = (pressure / FastPrecisePow(1.0 - ((float)Settings.altitude / 44330.0f), 5.255f)) - 21.6f;
          attr_list.addAttribute(0x0403, 0xFF00).setInt(pressure_sealevel);
          // We create a synthetic attribute 0403/FF00 to indicate sea level
        }
        break;
    }
  }
}

// Set deferred callbacks for Occupancy
// TODO make delay a parameter
void ZCLFrame::generateCallBacks(Z_attribute_list& attr_list) {
  static const uint32_t OCCUPANCY_TIMEOUT = 90 * 1000;  // 90 s
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.key.id.cluster << 16) | attr.key.id.attr_id;

    switch (ccccaaaa) {       // 0xccccaaaa . c=cluster, a=attribute
      case 0x04060000:        // Occupancy
        uint32_t occupancy = attr.getUInt();
        if (occupancy) {
          zigbee_devices.setTimer(_srcaddr, 0 /* groupaddr */, OCCUPANCY_TIMEOUT, _cluster_id, _srcendpoint, Z_CAT_VIRTUAL_OCCUPANCY, 0, &Z_OccupancyCallback);
        } else {
          zigbee_devices.resetTimersForDevice(_srcaddr, 0 /* groupaddr */, Z_CAT_VIRTUAL_OCCUPANCY);
        }        
        break;
    }
  }
}


// A command has been sent to a device this device, or to a group
// Set timers to read back values.
// If it's a device address, also set a timer for reachability test
void sendHueUpdate(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint = 0) {
  uint32_t wait_ms = 0xFFFF;

  switch (cluster) {
    case 0x0006:
      wait_ms = 200;    // wait 0.2 s
      break;
    case 0x0008:
      wait_ms = 1050;   // wait 1.0 s
      break;
    case 0x0102:
      wait_ms = 10000;   // wait 10.0 s
      break;
    case 0x0300:
      wait_ms = 1050;   // wait 1.0 s
      break;
    default:
      break;
  }
  if (0xFFFF != wait_ms) {
    if ((BAD_SHORTADDR != shortaddr) && (0 == endpoint)) {
      endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
    }
    if ((BAD_SHORTADDR == shortaddr) || (endpoint)) {   // send if group address or endpoint is known
      zigbee_devices.queueTimer(shortaddr, groupaddr, wait_ms, cluster, endpoint, Z_CAT_READ_CLUSTER, 0 /* value */, &Z_ReadAttrCallback);
      if (BAD_SHORTADDR != shortaddr) {      // reachability test is not possible for group addresses, since we don't know the list of devices in the group
        zigbee_devices.setTimer(shortaddr, groupaddr, wait_ms + Z_CAT_REACHABILITY_TIMEOUT, cluster, endpoint, Z_CAT_REACHABILITY, 0 /* value */, &Z_Unreachable);
      }

    }
  }
}

// ZCL_READ_ATTRIBUTES
void ZCLFrame::parseReadAttributes(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = _payload.len();

  uint16_t read_attr_ids[len/2];

  attr_list.addAttribute(F(D_CMND_ZIGBEE_CLUSTER)).setUInt(_cluster_id);

  Z_json_array attr_numbers;
  Z_attribute_list attr_names;
  while (len >= 2 + i) {
    uint16_t attrid = _payload.get16(i);
    attr_numbers.add(attrid);
    read_attr_ids[i/2] = attrid;

    // find the attribute name
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint16_t conv_attribute = pgm_read_word(&converter->attribute);

      if ((conv_cluster == _cluster_id) && (conv_attribute == attrid)) {
        attr_names.addAttribute(Z_strings + pgm_read_word(&converter->name_offset), true).setBool(true);
        break;
      }
    }
    i += 2;
  }
  attr_list.addAttribute(F("Read")).setStrRaw(attr_numbers.toString().c_str());
  attr_list.addAttribute(F("ReadNames")).setStrRaw(attr_names.toString(true).c_str());
 
  // call auto-responder
  Z_AutoResponder(_srcaddr, _cluster_id, _srcendpoint, read_attr_ids, len/2);
}

// ZCL_CONFIGURE_REPORTING_RESPONSE
void ZCLFrame::parseConfigAttributes(Z_attribute_list& attr_list) {
  uint32_t len = _payload.len();

  Z_attribute_list attr_config_list;
  for (uint32_t i=0; len >= i+4; i+=4) {
    uint8_t  status = _payload.get8(i);
    uint16_t attr_id = _payload.get8(i+2);

    Z_attribute_list attr_config_response;
    attr_config_response.addAttribute(F("Status")).setUInt(status);
    attr_config_response.addAttribute(F("StatusMsg")).setStr(getZigbeeStatusMessage(status).c_str());

    const __FlashStringHelper* attr_name = zigbeeFindAttributeById(_cluster_id, attr_id, nullptr, nullptr);
    if (attr_name) {
      attr_config_list.addAttribute(attr_name).setStrRaw(attr_config_response.toString(true).c_str());
    } else {
      attr_config_list.addAttribute(_cluster_id, attr_id).setStrRaw(attr_config_response.toString(true).c_str());
    }
  }

  Z_attribute &attr_1 = attr_list.addAttribute(F("ConfigResponse"));
  attr_1.setStrRaw(attr_config_list.toString(true).c_str());
}

// ZCL_READ_REPORTING_CONFIGURATION_RESPONSE
void ZCLFrame::parseReadConfigAttributes(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = _payload.len();

  Z_attribute &attr_root = attr_list.addAttribute(F("ReadConfig"));
  Z_attribute_list attr_1;

  while (len >= i + 4) {
    uint8_t  status = _payload.get8(i);
    uint8_t  direction = _payload.get8(i+1);
    uint16_t attrid = _payload.get16(i+2);

    Z_attribute_list attr_2;
    if (direction) {
      attr_2.addAttribute(F("DirectionReceived")).setBool(true);
    }

    // find the attribute name
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint16_t conv_attribute = pgm_read_word(&converter->attribute);

      if ((conv_cluster == _cluster_id) && (conv_attribute == attrid)) {
        const char * attr_name = Z_strings + pgm_read_word(&converter->name_offset);
        attr_2.addAttribute(attr_name, true).setBool(true);
        break;
      }
    }
    i += 4;
    if (0 != status) {
      attr_2.addAttribute(F("Status")).setUInt(status);
      attr_2.addAttribute(F("StatusMsg")).setStr(getZigbeeStatusMessage(status).c_str());
    } else {
      // no error, decode data
      if (direction) {
        // only Timeout period is present
        uint16_t attr_timeout = _payload.get16(i);
        i += 2;
        attr_2.addAttribute(F("TimeoutPeriod")).setUInt((0xFFFF == attr_timeout) ? -1 : attr_timeout);
      } else {
        // direction == 0, we have a data type
        uint8_t attr_type = _payload.get8(i);
        bool attr_discrete = Z_isDiscreteDataType(attr_type);
        uint16_t attr_min_interval = _payload.get16(i+1);
        uint16_t attr_max_interval = _payload.get16(i+3);
        i += 5;
        attr_2.addAttribute(F("MinInterval")).setUInt((0xFFFF == attr_min_interval) ? -1 : attr_min_interval);
        attr_2.addAttribute(F("MaxInterval")).setUInt((0xFFFF == attr_max_interval) ? -1 : attr_max_interval);
        if (!attr_discrete) {
          // decode Reportable Change
          Z_attribute &attr_change = attr_2.addAttribute(F("ReportableChange"));
          i += parseSingleAttribute(attr_change, _payload, i, attr_type);
        }
      }
    }
    attr_1.addAttribute(_cluster_id, attrid).setStrRaw(attr_2.toString(true).c_str());
  }
  attr_root.setStrRaw(attr_1.toString(true).c_str());
}

// ZCL_READ_ATTRIBUTES_RESPONSE
void ZCLFrame::parseReadAttributesResponse(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = _payload.len();

  while (len >= i + 4) {
    uint16_t attrid = _payload.get16(i);
    i += 2;
    uint8_t status = _payload.get8(i++);

    if (0 == status) {
      Z_attribute & attr = attr_list.addAttribute(_cluster_id, attrid);
      i += parseSingleAttribute(attr, _payload, i);
    }
  }
}

// ZCL_DEFAULT_RESPONSE
void ZCLFrame::parseResponse(void) {
  if (_payload.len() < 2) { return; }   // wrong format
  uint8_t cmd = _payload.get8(0);
  uint8_t status = _payload.get8(1);

  Z_attribute_list attr_list;

  // "Device"
  char s[12];
  snprintf_P(s, sizeof(s), PSTR("0x%04X"), _srcaddr);
  attr_list.addAttribute(F(D_JSON_ZIGBEE_DEVICE)).setStr(s);
  // "Name"
  const char * friendlyName = zigbee_devices.getFriendlyName(_srcaddr);
  if (friendlyName) {
    attr_list.addAttribute(F(D_JSON_ZIGBEE_NAME)).setStr(friendlyName);
  }
  // "Command"
  snprintf_P(s, sizeof(s), PSTR("%04X!%02X"), _cluster_id, cmd);
  attr_list.addAttribute(F(D_JSON_ZIGBEE_CMD)).setStr(s);
  // "Status"
  attr_list.addAttribute(F(D_JSON_ZIGBEE_STATUS)).setUInt(status);
  // "StatusMessage"
  attr_list.addAttribute(F(D_JSON_ZIGBEE_STATUS_MSG)).setStr(getZigbeeStatusMessage(status).c_str());
  // Add Endpoint
  attr_list.addAttribute(F(D_CMND_ZIGBEE_ENDPOINT)).setUInt(_srcendpoint);
  // Add Group if non-zero
  if (_groupaddr) {     // TODO what about group zero
    attr_list.group_id = _groupaddr;
  }
  // Add linkquality
  attr_list.lqi = _linkquality;

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_RESPONSE "\":%s}"), attr_list.toString(true).c_str());
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
}

// Parse non-normalized attributes
void ZCLFrame::parseClusterSpecificCommand(Z_attribute_list& attr_list) {
  convertClusterSpecific(attr_list, _cluster_id, _cmd_id, _frame_control.b.direction, _srcaddr, _srcendpoint, _payload);
#ifndef USE_ZIGBEE_NO_READ_ATTRIBUTES   // read attributes unless disabled
  if (!_frame_control.b.direction) {    // only handle server->client (i.e. device->coordinator)
    if (_wasbroadcast) {                // only update for broadcast messages since we don't see unicast from device to device and we wouldn't know the target
      sendHueUpdate(BAD_SHORTADDR, _groupaddr, _cluster_id);
    }
  }
#endif
}

// ======================================================================
// New version of synthetic attribute generation
void ZCLFrame::syntheticAqaraSensor(Z_attribute_list &attr_list, class Z_attribute &attr) {
  const SBuffer * buf = attr.getRaw();
  if (buf) {
    const SBuffer & buf2 = *buf;
    uint32_t i = 0;
    uint32_t len = buf2.len();

    const char * modelId_c = zigbee_devices.getModelId(_srcaddr);  // null if unknown
    String modelId((char*) modelId_c);

    while (len >= 2 + i) {
      uint8_t attrid = buf2.get8(i++);

      Z_attribute attr;     // temporary attribute
      i += parseSingleAttribute(attr, buf2, i);
      int32_t ival32 = attr.getInt();
      float fval = attr.getFloat();
      bool translated = false;    // were we able to translate to a known format?
      if (0x01 == attrid) {
        float batteryvoltage = fval / 100;
        attr_list.addAttribute(0x0001, 0x0020).setFloat(batteryvoltage);
        uint8_t batterypercentage = toPercentageCR2032(fval);
        attr_list.addAttribute(0x0001, 0x0021).setUInt(batterypercentage * 2);
      } else if ((nullptr != modelId) && (0 == getManufCode())) {
        translated = true;
        if (modelId.startsWith(F("lumi.sensor_ht")) ||
            modelId.equals(F("lumi.sens")) ||
            modelId.startsWith(F("lumi.weather"))) {     // Temp sensor
          // Filter according to prefix of model name
          // onla Aqara Temp/Humidity has manuf_code of zero. If non-zero we skip the parameters
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0402, 0x0000).setInt(ival32);   // Temperature
          } else if (0x65 == attrid) {
            attr_list.addAttribute(0x0405, 0x0000).setFloat(fval);         // Humidity * 100
          } else if (0x66 == attrid) {
            attr_list.addAttribute(0x0403, 0x0000).setUInt((ival32 + 50) / 100);  // Pressure
          }
        } else if (modelId.startsWith(F("lumi.sensor_smoke"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttribute(F("SmokeDensity")).copyVal(attr);
          }
        } else if (modelId.startsWith(F("lumi.sensor_natgas"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttribute(F("GasDensity")).copyVal(attr);
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
          attr_list.addAttribute(attr_name).copyVal(attr);
        }
      }
    }
  }
}

void ZCLFrame::syntheticAqaraSensor2(class Z_attribute_list &attr_list, class Z_attribute &attr) {
  const SBuffer * buf = attr.getRaw();
  if (buf) {
    const SBuffer & buf2 = *buf;
    uint32_t len = buf2.len();

    // Look for battery value which is the first attribute of type 0x21
    uint16_t struct_size = buf2.get16(0);
    size_t struct_len = 2;
    if (0xFFFF != struct_size) {
      if (struct_size > 16) { struct_size = 16; }
      for (uint32_t j = 0; (j < struct_size) && (struct_len < len); j++) {
        uint8_t attr_type = buf2.get8(struct_len);
        if (0x21 == attr_type) {
          uint16_t val = buf2.get16(struct_len+1);
          float batteryvoltage = (float)val / 100;
          attr_list.addAttribute(0x0001, 0x0020).setFloat(batteryvoltage);
          uint8_t batterypercentage = toPercentageCR2032(val);
          attr_list.addAttribute(0x0001, 0x0021).setUInt(batterypercentage * 2);
          break;
        }
        struct_len += Z_getDatatypeLen(attr_type) + 1;
      }
    }
  }
  attr_list.removeAttribute(&attr);
}

// Aqara Cube and Button
void ZCLFrame::syntheticAqaraCubeOrButton(class Z_attribute_list &attr_list, class Z_attribute &attr) {
  const char * modelId_c = zigbee_devices.findShortAddr(_srcaddr).modelId;  // null if unknown
  String modelId((char*) modelId_c);

  if (modelId.startsWith(F("lumi.sensor_cube"))) {   // only for Aqara cube
    int32_t val = attr.getInt();
    const __FlashStringHelper *aqara_cube = F("AqaraCube");
    const __FlashStringHelper *aqara_cube_side = F("AqaraCubeSide");
    const __FlashStringHelper *aqara_cube_from_side = F("AqaraCubeFromSide");

    switch (val) {
      case 0:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("shake"));
        break;
      case 2:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("wakeup"));
        break;
      case 3:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("fall"));
        break;
      case 64 ... 127:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("flip90"));
        attr_list.addAttribute(aqara_cube_side).setInt(val % 8);
        attr_list.addAttribute(aqara_cube_from_side).setInt((val - 64) / 8);
        break;
      case 128 ... 132:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("flip180"));
        attr_list.addAttribute(aqara_cube_side).setInt(val - 128);
        break;
      case 256 ... 261:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("slide"));
        attr_list.addAttribute(aqara_cube_side).setInt(val - 256);
        break;
      case 512 ... 517:
        attr_list.addAttribute(aqara_cube).setStr(PSTR("tap"));
        attr_list.addAttribute(aqara_cube_side).setInt(val - 512);
        break;
    }
    attr_list.removeAttribute(&attr);
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
  } else if (modelId.startsWith(F("lumi.remote")) || modelId.startsWith(F("lumi.sensor_switch"))) {   // only for Aqara buttons WXKG11LM & WXKG12LM
    int32_t val = attr.getInt();
    const __FlashStringHelper *aqara_click = F("click");
    const __FlashStringHelper *aqara_action = F("action");

    switch (val) {
      case 0:
        attr_list.addAttribute(aqara_action).setStr(PSTR("hold"));
        break;
      case 1:
        attr_list.addAttribute(aqara_click).setStr(PSTR("single"));
        break;
      case 2:
        attr_list.addAttribute(aqara_click).setStr(PSTR("double"));
        break;
      case 16:
        attr_list.addAttribute(aqara_action).setStr(PSTR("hold"));
        break;
      case 17:
        attr_list.addAttribute(aqara_action).setStr(PSTR("release"));
        break;
      case 18:
        attr_list.addAttribute(aqara_action).setStr(PSTR("shake"));
        break;
      case 255:
        attr_list.addAttribute(aqara_action).setStr(PSTR("release"));
        break;
      default:
        attr_list.addAttribute(aqara_click).setUInt(val);
        break;
    }
  }
}

// Aqara vibration device
void ZCLFrame::syntheticAqaraVibration(class Z_attribute_list &attr_list, class Z_attribute &attr) {
  switch (attr.key.id.attr_id) {
    case 0x0055:
      {
        int32_t ivalue = attr.getInt();
        const __FlashStringHelper * svalue;
        switch (ivalue) {
          case 1: svalue = F("vibrate"); break;
          case 2: svalue = F("tilt"); break;
          case 3: svalue = F("drop"); break;
          default: svalue = F("unknown"); break;
        }
        attr.setStr((const char*)svalue);
      }
      break;
    case 0x0503:
      break;
    case 0x0505:
      break;
    case 0x0508:
      {
        // see https://github.com/Koenkk/zigbee2mqtt/issues/295 and http://faire-ca-soi-meme.fr/domotique/2018/09/03/test-xiaomi-aqara-vibration-sensor/
        // report accelerometer measures
        const SBuffer * buf = attr.getRaw();
        if (buf) {
          const SBuffer & buf2 = *buf;
          int16_t x, y, z;
          z = buf2.get16(0);
          y = buf2.get16(2);
          x = buf2.get16(4);
          char temp[32];
          snprintf_P(temp, sizeof(temp), "[%i,%i,%i]", x, y, z);
          attr.setStrRaw(temp);
          // calculate angles
          float X = x;
          float Y = y;
          float Z = z;
          int32_t Angle_X = 0.5f + atanf(X/sqrtf(z*z+y*y)) * f_180pi;
          int32_t Angle_Y = 0.5f + atanf(Y/sqrtf(x*x+z*z)) * f_180pi;
          int32_t Angle_Z = 0.5f + atanf(Z/sqrtf(x*x+y*y)) * f_180pi;
          snprintf_P(temp, sizeof(temp), "[%i,%i,%i]", Angle_X, Angle_Y, Angle_Z);
          attr_list.addAttribute(F("AqaraAngles")).setStrRaw(temp);
        }
      }
      break;
  }
}

/// Publish a message for `"Occupancy":0` when the timer expired
void Z_OccupancyCallback(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  Z_attribute_list attr_list;
  attr_list.addAttribute(F(OCCUPANCY)).setUInt(0);
  zigbee_devices.jsonPublishNow(shortaddr, attr_list);
}

// ======================================================================
void ZCLFrame::postProcessAttributes(uint16_t shortaddr, Z_attribute_list& attr_list) {
  // source endpoint
  uint8_t src_ep = _srcendpoint;
  
  for (auto &attr : attr_list) {
    // attr is Z_attribute&
    if (!attr.key_is_str) {
      uint16_t cluster = attr.key.id.cluster;
      uint16_t attribute = attr.key.id.attr_id;
      uint32_t ccccaaaa = (attr.key.id.cluster << 16) | attr.key.id.attr_id;
      Z_Device & device = zigbee_devices.getShortAddr(shortaddr);

      // Look for an entry in the converter table
      bool found = false;
      int8_t conv_multiplier;
      const char * conv_name;
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
        uint16_t conv_attribute = pgm_read_word(&converter->attribute);

        if ((conv_cluster == cluster) &&
            ((conv_attribute == attribute) || (conv_attribute == 0xFFFF)) ) {
          conv_multiplier = pgm_read_byte(&converter->multiplier);
          conv_name = Z_strings + pgm_read_word(&converter->name_offset);
          found = true;
          break;
        }
      }

      // apply multiplier if needed
      float    fval   = attr.getFloat();
      if (found) {
        if (0 == conv_multiplier)  { attr_list.removeAttribute(&attr); continue; }      // remove attribute if multiplier is zero
        if (1 != conv_multiplier) {
          if (conv_multiplier > 0) { fval =  fval * conv_multiplier; }
          else                     { fval =  fval / (-conv_multiplier); }
          attr.setFloat(fval);
        }
      }

      uint16_t uval16 = attr.getUInt();     // call converter to uint only once
      int16_t  ival16 = attr.getInt();     // call converter to int only once
      // update any internal structure
      switch (ccccaaaa) {
        case 0x00000004: zigbee_devices.setManufId(shortaddr, attr.getStr());         break;
        case 0x00000005: zigbee_devices.setModelId(shortaddr, attr.getStr());         break;
        case 0x00010021: zigbee_devices.setBatteryPercent(shortaddr, uval16);         break;
        case 0x00060000:
        case 0x00068000: device.setPower(attr.getBool());                             break;
        case 0x00080000: device.dimmer = uval16;                                      break;
        case 0x02010000: device.temperature = fval * 10 + 0.5f;                       break;
        case 0x02010008: device.th_setpoint = uval16;                                 break;
        case 0x02010012: device.temperature_target = fval * 10 + 0.5f;                break;
        case 0x02010007: device.th_setpoint = uval16;                                 break;
        case 0x02010011: device.temperature_target = fval * 10 + 0.5f;                break;
        case 0x03000000: device.hue = changeUIntScale(uval16, 0, 254, 0, 360);        break;
        case 0x03000001: device.sat = uval16;                                         break;
        case 0x03000003: device.x = uval16;                                           break;
        case 0x03000004: device.y = uval16;                                           break;
        case 0x03000007: device.ct = uval16;                                          break;
        case 0x03000008: device.colormode = uval16;                                   break;
        case 0x04020000: device.temperature = fval * 10 + 0.5f;                       break;
        case 0x0403FF00: device.pressure = fval + 0.5f;                               break;  // give priority to sea level
        case 0x04030000: device.pressure = fval + 0.5f;                               break;
        case 0x04050000: device.humidity = fval + 0.5f;                               break;
        case 0x0B040505: device.mains_voltage = uval16;                               break;
        case 0x0B04050B: device.mains_power = ival16;                                 break;
      }

      // Replace cluster/attribute with name
      if (found) {
        if (0x00 != pgm_read_byte(conv_name)) {// if name is not null, replace it
          attr.setKeyName(conv_name, true);    // PMEM string so no need to copy
        }
      }
    }
  }
}

//
// Given an attribute string, retrieve all attribute details.
// Input: the attribute has a key name, either: <cluster>/<attr> or <cluster>/<attr>%<type> or "<attribute_name>"
// Ex: "0008/0000", "0008/0000%20", "Dimmer"
// Use:
//    Z_attribute attr;
//    attr.setKeyName("0008/0000%20")
//    if (Z_parseAttributeKey(attr)) {
//      // ok
//    }
//
// Output:
//   The `attr` attribute has the correct cluster, attr_id, attr_type, attr_multiplier
//   Note: the attribute value is unchanged and unparsed
//
// Note: if the type is specified in the key, the multiplier is not applied, no conversion happens
bool Z_parseAttributeKey(class Z_attribute & attr) {
  // check if the name has the format "XXXX/YYYY" where XXXX is the cluster, YYYY the attribute id
  // alternative "XXXX/YYYY%ZZ" where ZZ is the type (for unregistered attributes)
  if (attr.key_is_str) {
    const char * key = attr.key.key;
    char * delimiter = strchr(key, '/');
    char * delimiter2 = strchr(key, '%');
    if (delimiter) {
      uint16_t attr_id = 0xFFFF;
      uint16_t cluster_id = 0xFFFF;
      uint8_t  type_id = Zunk;

      cluster_id = strtoul(key, &delimiter, 16);
      if (!delimiter2) {
        attr_id = strtoul(delimiter+1, nullptr, 16);
      } else {
        attr_id = strtoul(delimiter+1, &delimiter2, 16);
        type_id = strtoul(delimiter2+1, nullptr, 16);
      }
      attr.setKeyId(cluster_id, attr_id);
      attr.attr_type = type_id;
    }
  }
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X"), cluster_id, attr_id);

  // do we already know the type, i.e. attribute and cluster are also known
  if (Zunk == attr.attr_type) {
    // scan attributes to find by name, and retrieve type
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      bool match = false;
      uint16_t local_attr_id = pgm_read_word(&converter->attribute);
      uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint8_t  local_type_id = pgm_read_byte(&converter->type);
      int8_t   local_multiplier = pgm_read_byte(&converter->multiplier);
      // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Try cluster = 0x%04X, attr = 0x%04X, type_id = 0x%02X"), local_cluster_id, local_attr_id, local_type_id);

      if (!attr.key_is_str) {
        if ((attr.key.id.cluster == local_cluster_id) && (attr.key.id.attr_id == local_attr_id)) {
          attr.attr_type = local_type_id;
          break;
        }
      } else if (pgm_read_word(&converter->name_offset)) {
        const char * key = attr.key.key;
        // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Comparing '%s' with '%s'"), attr_name, converter->name);
        if (0 == strcasecmp_P(key, Z_strings + pgm_read_word(&converter->name_offset))) {
          // match
          attr.setKeyId(local_cluster_id, local_attr_id);
          attr.attr_type = local_type_id;
          attr.attr_multiplier = local_multiplier;
          break;
        }
      }
    }
  }
  return (Zunk != attr.attr_type) ? true : false;
}

#endif // USE_ZIGBEE
