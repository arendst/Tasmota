/*
  xdrv_23_zigbee_converters.ino - zigbee support for Tasmota

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
  Zunk      = 0xFF,
  // adding fake type for Tuya specific encodings
  Ztuya0    = Zoctstr,
  Ztuya1    = Zbool,
  Ztuya2    = Zint32,
  Ztuya3    = Zstring,
  Ztuya4    = Zuint8,
  Ztuya5    = Zuint32
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
  uint8_t     type;
  uint8_t     cluster_short;
  uint16_t    attribute;
  uint16_t    name_offset;
  uint8_t     multiplier_idx;     // multiplier index for numerical value, use CmToMultiplier(), (if > 0 multiply by x, if <0 device by x)
                                  // the high 4 bits are used to encode flags
                                  // currently: 0x80 = this parameter needs to be exported to ZbData
  uint8_t     mapping;        // high 4 bits = type, low 4 bits = offset in bytes from header
  // still room for a byte
} Z_AttributeConverter;

// Get offset in bytes of attributes, starting after the header (skipping first 4 bytes)
#define Z_OFFSET(c,a) (offsetof(class c, a) - sizeof(Z_Data))
#define Z_CLASS(c) c      // necessary to get a valid token without concatenation (which wouldn't work)
#define Z_MAPPING(c,a) (((((uint8_t)Z_CLASS(c)::type) & 0x0F) << 4) | Z_OFFSET(c,a))

// lines with this marker, will be used to export automatically data to `ZbData`
// at the condition Z_MAPPING() is also used
const uint8_t Z_EXPORT_DATA = 0x80;

// Cluster numbers are store in 8 bits format to save space,
// the following tables allows the conversion from 8 bits index Cx...
// to the 16 bits actual cluster number
enum Cx_cluster_short {
  Cx0000, Cx0001, Cx0002, Cx0003, Cx0004, Cx0005, Cx0006, Cx0007,
  Cx0008, Cx0009, Cx000A, Cx000B, Cx000C, Cx000D, Cx000E, Cx000F,
  Cx0010, Cx0011, Cx0012, Cx0013, Cx0014, Cx001A, Cx0020, Cx0100,
  Cx0101, Cx0102, Cx0201, Cx0300, Cx0400, Cx0401, Cx0402, Cx0403,
  Cx0404, Cx0405, Cx0406, Cx0500, Cx0702, Cx0B01, Cx0B04, Cx0B05,
  CxEF00, CxFCC0, CxFCCC,
};

const uint16_t Cx_cluster[] PROGMEM = {
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x001A, 0x0020, 0x0100,
  0x0101, 0x0102, 0x0201, 0x0300, 0x0400, 0x0401, 0x0402, 0x0403,
  0x0404, 0x0405, 0x0406, 0x0500, 0x0702, 0x0B01, 0x0B04, 0x0B05,
  0xEF00, 0xFCC0, 0xFCCC,
};

uint16_t CxToCluster(uint8_t cx) {
  if (cx < ARRAY_SIZE(Cx_cluster)) {
    return pgm_read_word(&Cx_cluster[cx]);
  }
  return 0xFFFF;
}

uint8_t ClusterToCx(uint16_t cluster) {
  for (uint32_t i=0; i<ARRAY_SIZE(Cx_cluster); i++) {
    if (pgm_read_word(&Cx_cluster[i]) == cluster) {
      return i;
    }
  }
  return 0xFF;
}

// Multiplier contains only a limited set of values, so instead of storing the value
// we store an index in a table, and reduce it to 4 bits
enum Cm_multiplier_nibble {
  Cm0 = 0, Cm1 = 1, Cm2, Cm5, Cm10, Cm100,
  // negative numbers
  Cm_2, Cm_5, Cm_10, Cm_100
};

const int8_t Cm_multiplier[] PROGMEM = {
  0, 1, 2, 5, 10, 100,
  -2, -5, -10, -100,
};

int8_t CmToMultiplier(uint8_t cm) {
  cm = cm & 0x0F;     // get only low nibble
  if (cm < ARRAY_SIZE(Cm_multiplier)) {
    return pgm_read_byte(&Cm_multiplier[cm]);
  }
  return 1;
}

// list of post-processing directives
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"   // avoid warnings since we're using offsetof() in a risky way
const Z_AttributeConverter Z_PostProcess[] PROGMEM = {
  { Zuint8,   Cx0000, 0x0000,  Z_(ZCLVersion),           Cm1, 0 },
  { Zuint8,   Cx0000, 0x0001,  Z_(AppVersion),           Cm1, 0 },
  { Zuint8,   Cx0000, 0x0002,  Z_(StackVersion),         Cm1, 0 },
  { Zuint8,   Cx0000, 0x0003,  Z_(HWVersion),            Cm1, 0 },
  { Zstring,  Cx0000, 0x0004,  Z_(Manufacturer),         Cm1, 0 },    // record Manufacturer
  { Zstring,  Cx0000, 0x0005,  Z_(ModelId),              Cm1, 0 },    // record Model
  // { Zstring,  Cx0000, 0x0004,  Z_(Manufacturer),         Cm1,  Z_ManufKeep, 0 },    // record Manufacturer
  // { Zstring,  Cx0000, 0x0005,  Z_(ModelId),              Cm1,  Z_ModelKeep, 0 },    // record Model
  { Zstring,  Cx0000, 0x0006,  Z_(DateCode),             Cm1, 0 },
  { Zenum8,   Cx0000, 0x0007,  Z_(PowerSource),          Cm1, 0 },
  { Zenum8,   Cx0000, 0x0008,  Z_(GenericDeviceClass),   Cm1, 0 },
  { Zenum8,   Cx0000, 0x0009,  Z_(GenericDeviceType),    Cm1, 0 },
  { Zoctstr,  Cx0000, 0x000A,  Z_(ProductCode),          Cm1, 0 },
  { Zstring,  Cx0000, 0x000B,  Z_(ProductURL),           Cm1, 0 },
  { Zstring,  Cx0000, 0x4000,  Z_(SWBuildID),            Cm1, 0 },
  // { Zunk,     Cx0000, 0xFFFF,  nullptr,                 Cm0, 0 },    // Remove all other values
  // Cmd 0x0A - Cluster 0x0000, attribute 0xFF01 - proprietary
  { Zmap8,    Cx0000, 0xFF01,  Z_(),                     Cm0, 0 },
  { Zmap8,    Cx0000, 0xFF02,  Z_(),                     Cm0, 0 },
  // { Zmap8,    Cx0000, 0xFF01,  Z_(),                     Cm0,  Z_AqaraSensor, 0 },
  // { Zmap8,    Cx0000, 0xFF02,  Z_(),                     Cm0,  Z_AqaraSensor2, 0 },

  // Power Configuration cluster
  { Zuint16,  Cx0001, 0x0000,  Z_(MainsVoltage),         Cm1, 0 },
  { Zuint8,   Cx0001, 0x0001,  Z_(MainsFrequency),       Cm1, 0 },
  { Zuint8,   Cx0001, 0x0020,  Z_(BatteryVoltage),       Cm_10, 0 },   // divide by 10
  { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    Cm_2, 0 },   // divide by 2
  // { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    Cm_2, Z_BatteryPercentage, 0 },   // divide by 2

  // Device Temperature Configuration cluster
  { Zint16,   Cx0002, 0x0000,  Z_(CurrentTemperature),   Cm1, 0 },
  { Zint16,   Cx0002, 0x0001,  Z_(MinTempExperienced),   Cm1, 0 },
  { Zint16,   Cx0002, 0x0002,  Z_(MaxTempExperienced),   Cm1, 0 },
  { Zuint16,  Cx0002, 0x0003,  Z_(OverTempTotalDwell),   Cm1, 0 },

  // Identify cluster
  { Zuint16,  Cx0003, 0x0000,  Z_(IdentifyTime),         Cm1, 0 },

  // Groups cluster
  { Zmap8,    Cx0004, 0x0000,  Z_(GroupNameSupport),     Cm1, 0 },

  // Scenes cluster
  { Zuint8,   Cx0005, 0x0000,  Z_(SceneCount),           Cm1, 0 },
  { Zuint8,   Cx0005, 0x0001,  Z_(CurrentScene),         Cm1, 0 },
  { Zuint16,  Cx0005, 0x0002,  Z_(CurrentGroup),         Cm1, 0 },
  { Zbool,    Cx0005, 0x0003,  Z_(SceneValid),           Cm1, 0 },
  //{ Zmap8,    Cx0005, 0x0004,  (NameSupport),           Cm1, 0 },

  // On/off cluster
  { Zbool,    Cx0006,    0x0000,  Z_(Power),             Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_OnOff, power) },
  { Zenum8,   Cx0006,    0x4003,  Z_(StartUpOnOff),      Cm1, 0 },
  { Zbool,    Cx0006,    0x8000,  Z_(Power),             Cm1, 0 },   // See 7280

  // On/Off Switch Configuration cluster
  { Zenum8,   Cx0007, 0x0000,  Z_(SwitchType),           Cm1, 0 },

  // Level Control cluster
  { Zuint8,   Cx0008, 0x0000,  Z_(Dimmer),               Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, dimmer) },
  { Zmap8,    Cx0008, 0x000F,  Z_(DimmerOptions),        Cm1, 0 },
  { Zuint16,  Cx0008, 0x0001,  Z_(DimmerRemainingTime),  Cm1, 0 },
  { Zuint16,  Cx0008, 0x0010,  Z_(OnOffTransitionTime),   Cm1, 0 },
  // { Zuint8, Cx0008, 0x0011,  (OnLevel),              Cm1, 0 },
  // { Zuint16, Cx0008, 0x0012,  (OnTransitionTime),     Cm1, 0 },
  // { Zuint16, Cx0008, 0x0013,  (OffTransitionTime),    Cm1, 0 },
  // { Zuint16, Cx0008, 0x0014,  (DefaultMoveRate),      Cm1, 0 },

  // Alarms cluster
  { Zuint16,  Cx0009, 0x0000,  Z_(AlarmCount),           Cm1, 0 },

  // Time cluster
  { ZUTC,     Cx000A, 0x0000,  Z_(Time),                 Cm1, 0 },
  { Zmap8,    Cx000A, 0x0001,  Z_(TimeStatus),           Cm1, 0 },
  { Zint32,   Cx000A, 0x0002,  Z_(TimeZone),             Cm1, 0 },
  { Zuint32,  Cx000A, 0x0003,  Z_(DstStart),             Cm1, 0 },
  { Zuint32,  Cx000A, 0x0004,  Z_(DstEnd),               Cm1, 0 },
  { Zint32,   Cx000A, 0x0005,  Z_(DstShift),             Cm1, 0 },
  { Zuint32,  Cx000A, 0x0006,  Z_(StandardTime),         Cm1, 0 },
  { Zuint32,  Cx000A, 0x0007,  Z_(LocalTime),            Cm1, 0 },
  { ZUTC,     Cx000A, 0x0008,  Z_(LastSetTime),          Cm1, 0 },
  { ZUTC,     Cx000A, 0x0009,  Z_(ValidUntilTime),       Cm1, 0 },
  { ZUTC,     Cx000A, 0xFF00,  Z_(TimeEpoch),            Cm1, 0 },    // Tasmota specific, epoch

  // RSSI Location cluster
  { Zdata8,   Cx000B, 0x0000,  Z_(LocationType),         Cm1, 0 },
  { Zenum8,   Cx000B, 0x0001,  Z_(LocationMethod),       Cm1, 0 },
  { Zuint16,  Cx000B, 0x0002,  Z_(LocationAge),          Cm1, 0 },
  { Zuint8,   Cx000B, 0x0003,  Z_(QualityMeasure),       Cm1, 0 },
  { Zuint8,   Cx000B, 0x0004,  Z_(NumberOfDevices),      Cm1, 0 },

  // Analog Input cluster
  // { 0xFF, Cx000C, 0x0004,  (AnalogInActiveText),   Cm1, 0 },
  { Zstring,  Cx000C, 0x001C,  Z_(AnalogInDescription),  Cm1, 0 },
  // { 0xFF, Cx000C, 0x002E,  (AnalogInInactiveText), Cm1, 0 },
  { Zsingle,  Cx000C, 0x0041,  Z_(AnalogInMaxValue),     Cm1, 0 },
  { Zsingle,  Cx000C, 0x0045,  Z_(AnalogInMinValue),     Cm1, 0 },
  { Zbool,    Cx000C, 0x0051,  Z_(AnalogInOutOfService), Cm1, 0 },
  { Zsingle,  Cx000C, 0x0055,  Z_(AnalogValue),          Cm1, 0 },
  // { 0xFF, Cx000C, 0x0057,  (AnalogInPriorityArray),Cm1, 0 },
  { Zenum8,   Cx000C, 0x0067,  Z_(AnalogInReliability),  Cm1, 0 },
  // { 0xFF, Cx000C, 0x0068,  (AnalogInRelinquishDefault),Cm1, 0 },
  { Zsingle,  Cx000C, 0x006A,  Z_(AnalogInResolution),   Cm1, 0 },
  { Zmap8,    Cx000C, 0x006F,  Z_(AnalogInStatusFlags),  Cm1, 0 },
  { Zenum16,  Cx000C, 0x0075,  Z_(AnalogInEngineeringUnits),Cm1, 0 },
  { Zuint32,  Cx000C, 0x0100,  Z_(AnalogInApplicationType),Cm1, 0 },
  { Zuint16,  Cx000C, 0xFF55,  Z_(AqaraRotate),          Cm1, 0 },
  { Zuint16,  Cx000C, 0xFF05,  Z_(Aqara_FF05),           Cm1, 0 },

  // Analog Output cluster
  { Zstring,  Cx000D, 0x001C,  Z_(AnalogOutDescription), Cm1, 0 },
  { Zsingle,  Cx000D, 0x0041,  Z_(AnalogOutMaxValue),    Cm1, 0 },
  { Zsingle,  Cx000D, 0x0045,  Z_(AnalogOutMinValue),    Cm1, 0 },
  { Zbool,    Cx000D, 0x0051,  Z_(AnalogOutOutOfService),Cm1, 0 },
  { Zsingle,  Cx000D, 0x0055,  Z_(AnalogOutValue),       Cm1, 0 },
  // { Zunk,     Cx000D, 0x0057,  (AnalogOutPriorityArray),Cm1, 0 },
  { Zenum8,   Cx000D, 0x0067,  Z_(AnalogOutReliability), Cm1, 0 },
  { Zsingle,  Cx000D, 0x0068,  Z_(AnalogOutRelinquishDefault), Cm1, 0 },
  { Zsingle,  Cx000D, 0x006A,  Z_(AnalogOutResolution),  Cm1, 0 },
  { Zmap8,    Cx000D, 0x006F,  Z_(AnalogOutStatusFlags), Cm1, 0 },
  { Zenum16,  Cx000D, 0x0075,  Z_(AnalogOutEngineeringUnits), Cm1, 0 },
  { Zuint32,  Cx000D, 0x0100,  Z_(AnalogOutApplicationType), Cm1, 0 },

  // Analog Value cluster
  { Zstring,  Cx000E, 0x001C,  Z_(AnalogDescription),    Cm1, 0 },
  { Zbool,    Cx000E, 0x0051,  Z_(AnalogOutOfService),   Cm1, 0 },
  { Zsingle,  Cx000E, 0x0055,  Z_(AnalogValue),          Cm1, 0 },
  { Zunk,     Cx000E, 0x0057,  Z_(AnalogPriorityArray),  Cm1, 0 },
  { Zenum8,   Cx000E, 0x0067,  Z_(AnalogReliability),    Cm1, 0 },
  { Zsingle,  Cx000E, 0x0068,  Z_(AnalogRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx000E, 0x006F,  Z_(AnalogStatusFlags),    Cm1, 0 },
  { Zenum16,  Cx000E, 0x0075,  Z_(AnalogEngineeringUnits),Cm1, 0 },
  { Zuint32,  Cx000E, 0x0100,  Z_(AnalogApplicationType),Cm1, 0 },

  // Binary Input cluster
  { Zstring,  Cx000F, 0x0004,  Z_(BinaryInActiveText),  Cm1, 0 },
  { Zstring,  Cx000F, 0x001C,  Z_(BinaryInDescription), Cm1, 0 },
  { Zstring,  Cx000F, 0x002E,  Z_(BinaryInInactiveText),Cm1, 0 },
  { Zbool,    Cx000F, 0x0051,  Z_(BinaryInOutOfService),Cm1, 0 },
  { Zenum8,   Cx000F, 0x0054,  Z_(BinaryInPolarity),    Cm1, 0 },
  { Zstring,  Cx000F, 0x0055,  Z_(BinaryInValue),       Cm1, 0 },
  // { 0xFF, Cx000F, 0x0057,  (BinaryInPriorityArray),Cm1, 0 },
  { Zenum8,   Cx000F, 0x0067,  Z_(BinaryInReliability), Cm1, 0 },
  { Zmap8,    Cx000F, 0x006F,  Z_(BinaryInStatusFlags), Cm1, 0 },
  { Zuint32,  Cx000F, 0x0100,  Z_(BinaryInApplicationType),Cm1, 0 },

  // Binary Output cluster
  { Zstring,  Cx0010, 0x0004,  Z_(BinaryOutActiveText),  Cm1, 0 },
  { Zstring,  Cx0010, 0x001C,  Z_(BinaryOutDescription), Cm1, 0 },
  { Zstring,  Cx0010, 0x002E,  Z_(BinaryOutInactiveText),Cm1, 0 },
  { Zuint32,  Cx0010, 0x0042,  Z_(BinaryOutMinimumOffTime),Cm1, 0 },
  { Zuint32,  Cx0010, 0x0043,  Z_(BinaryOutMinimumOnTime),Cm1, 0 },
  { Zbool,    Cx0010, 0x0051,  Z_(BinaryOutOutOfService),Cm1, 0 },
  { Zenum8,   Cx0010, 0x0054,  Z_(BinaryOutPolarity),    Cm1, 0 },
  { Zbool,    Cx0010, 0x0055,  Z_(BinaryOutValue),       Cm1, 0 },
  // { Zunk,     Cx0010, 0x0057,  (BinaryOutPriorityArray),Cm1, 0 },
  { Zenum8,   Cx0010, 0x0067,  Z_(BinaryOutReliability), Cm1, 0 },
  { Zbool,    Cx0010, 0x0068,  Z_(BinaryOutRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0010, 0x006F,  Z_(BinaryOutStatusFlags), Cm1, 0 },
  { Zuint32,  Cx0010, 0x0100,  Z_(BinaryOutApplicationType),Cm1, 0 },

  // Binary Value cluster
  { Zstring,  Cx0011, 0x0004,  Z_(BinaryActiveText),     Cm1, 0 },
  { Zstring,  Cx0011, 0x001C,  Z_(BinaryDescription),    Cm1, 0 },
  { Zstring,  Cx0011, 0x002E,  Z_(BinaryInactiveText),   Cm1, 0 },
  { Zuint32,  Cx0011, 0x0042,  Z_(BinaryMinimumOffTime), Cm1, 0 },
  { Zuint32,  Cx0011, 0x0043,  Z_(BinaryMinimumOnTime),  Cm1, 0 },
  { Zbool,    Cx0011, 0x0051,  Z_(BinaryOutOfService),   Cm1, 0 },
  { Zbool,    Cx0011, 0x0055,  Z_(BinaryValue),          Cm1, 0 },
  // { Zunk,     Cx0011, 0x0057,  (BinaryPriorityArray),  Cm1, 0 },
  { Zenum8,   Cx0011, 0x0067,  Z_(BinaryReliability),    Cm1, 0 },
  { Zbool,    Cx0011, 0x0068,  Z_(BinaryRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0011, 0x006F,  Z_(BinaryStatusFlags),    Cm1, 0 },
  { Zuint32,  Cx0011, 0x0100,  Z_(BinaryApplicationType),Cm1, 0 },

  // Multistate Input cluster
  // { Zunk,     Cx0012, 0x000E,  (MultiInStateText),     Cm1, 0 },
  { Zstring,  Cx0012, 0x001C,  Z_(MultiInDescription),   Cm1, 0 },
  { Zuint16,  Cx0012, 0x004A,  Z_(MultiInNumberOfStates),Cm1, 0 },
  { Zbool,    Cx0012, 0x0051,  Z_(MultiInOutOfService),  Cm1, 0 },
  { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         Cm1, 0 },
  // { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         Cm0,  Z_AqaraCube, 0 },
  // { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         Cm0,  Z_AqaraButton, 0 },
  { Zenum8,   Cx0012, 0x0067,  Z_(MultiInReliability),   Cm1, 0 },
  { Zmap8,    Cx0012, 0x006F,  Z_(MultiInStatusFlags),   Cm1, 0 },
  { Zuint32,  Cx0012, 0x0100,  Z_(MultiInApplicationType),Cm1, 0 },

  // Multistate output
  // { Zunk,     Cx0013, 0x000E,  (MultiOutStateText),    Cm1, 0 },
  { Zstring,  Cx0013, 0x001C,  Z_(MultiOutDescription),  Cm1, 0 },
  { Zuint16,  Cx0013, 0x004A,  Z_(MultiOutNumberOfStates),Cm1, 0 },
  { Zbool,    Cx0013, 0x0051,  Z_(MultiOutOutOfService), Cm1, 0 },
  { Zuint16,  Cx0013, 0x0055,  Z_(MultiOutValue),        Cm1, 0 },
  // { Zunk,     Cx0013, 0x0057,  (MultiOutPriorityArray),Cm1, 0 },
  { Zenum8,   Cx0013, 0x0067,  Z_(MultiOutReliability),  Cm1, 0 },
  { Zuint16,  Cx0013, 0x0068,  Z_(MultiOutRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0013, 0x006F,  Z_(MultiOutStatusFlags),  Cm1, 0 },
  { Zuint32,  Cx0013, 0x0100,  Z_(MultiOutApplicationType),Cm1, 0 },

  // Multistate Value cluster
  // { Zunk,     Cx0014, 0x000E,  (MultiStateText),       Cm1, 0 },
  { Zstring,  Cx0014, 0x001C,  Z_(MultiDescription),     Cm1, 0 },
  { Zuint16,  Cx0014, 0x004A,  Z_(MultiNumberOfStates),  Cm1, 0 },
  { Zbool,    Cx0014, 0x0051,  Z_(MultiOutOfService),    Cm1, 0 },
  { Zuint16,  Cx0014, 0x0055,  Z_(MultiValue),           Cm1, 0 },
  { Zenum8,   Cx0014, 0x0067,  Z_(MultiReliability),     Cm1, 0 },
  { Zuint16,  Cx0014, 0x0068,  Z_(MultiRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0014, 0x006F,  Z_(MultiStatusFlags),     Cm1, 0 },
  { Zuint32,  Cx0014, 0x0100,  Z_(MultiApplicationType), Cm1, 0 },

  // Power Profile cluster
  { Zuint8,   Cx001A, 0x0000,  Z_(TotalProfileNum),      Cm1, 0 },
  { Zbool,    Cx001A, 0x0001,  Z_(MultipleScheduling),   Cm1, 0 },
  { Zmap8,    Cx001A, 0x0002,  Z_(EnergyFormatting),     Cm1, 0 },
  { Zbool,    Cx001A, 0x0003,  Z_(EnergyRemote),         Cm1, 0 },
  { Zmap8,    Cx001A, 0x0004,  Z_(ScheduleMode),         Cm1, 0 },

  // Poll Control cluster
  { Zuint32,  Cx0020, 0x0000,  Z_(CheckinInterval),      Cm1, 0 },
  { Zuint32,  Cx0020, 0x0001,  Z_(LongPollInterval),     Cm1, 0 },
  { Zuint16,  Cx0020, 0x0002,  Z_(ShortPollInterval),    Cm1, 0 },
  { Zuint16,  Cx0020, 0x0003,  Z_(FastPollTimeout),      Cm1, 0 },
  { Zuint32,  Cx0020, 0x0004,  Z_(CheckinIntervalMin),   Cm1, 0 },
  { Zuint32,  Cx0020, 0x0005,  Z_(LongPollIntervalMin),  Cm1, 0 },
  { Zuint16,  Cx0020, 0x0006,  Z_(FastPollTimeoutMax),   Cm1, 0 },

  // Shade Configuration cluster
  { Zuint16,  Cx0100, 0x0000,  Z_(PhysicalClosedLimit),  Cm1, 0 },
  { Zuint8,   Cx0100, 0x0001,  Z_(MotorStepSize),        Cm1, 0 },
  { Zmap8,    Cx0100, 0x0002,  Z_(Status),               Cm1, 0 },
  { Zuint16,  Cx0100, 0x0010,  Z_(ClosedLimit),          Cm1, 0 },
  { Zenum8,   Cx0100, 0x0011,  Z_(Mode),                 Cm1, 0 },

  // Door Lock cluster
  { Zenum8,   Cx0101, 0x0000,  Z_(LockState),            Cm1, 0 },
  { Zenum8,   Cx0101, 0x0001,  Z_(LockType),             Cm1, 0 },
  { Zbool,    Cx0101, 0x0002,  Z_(ActuatorEnabled),      Cm1, 0 },
  { Zenum8,   Cx0101, 0x0003,  Z_(DoorState),            Cm1, 0 },
  { Zuint32,  Cx0101, 0x0004,  Z_(DoorOpenEvents),       Cm1, 0 },
  { Zuint32,  Cx0101, 0x0005,  Z_(DoorClosedEvents),     Cm1, 0 },
  { Zuint16,  Cx0101, 0x0006,  Z_(OpenPeriod),           Cm1, 0 },

  // Aqara Lumi Vibration Sensor
  { Zuint16,  Cx0101, 0x0055,  Z_(AqaraVibrationMode),   Cm1, 0 },
  { Zuint16,  Cx0101, 0x0503,  Z_(AqaraVibrationsOrAngle), Cm1, 0 },
  { Zuint32,  Cx0101, 0x0505,  Z_(AqaraVibration505),    Cm1, 0 },
  { Zuint48,  Cx0101, 0x0508,  Z_(AqaraAccelerometer),   Cm1, 0 },

  // Window Covering cluster
  { Zenum8,   Cx0102, 0x0000,  Z_(WindowCoveringType),   Cm1, 0 },
  { Zuint16,  Cx0102, 0x0001,  Z_(PhysicalClosedLimitLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0002,  Z_(PhysicalClosedLimitTilt),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0003,  Z_(CurrentPositionLift),  Cm1, 0 },
  { Zuint16,  Cx0102, 0x0004,  Z_(CurrentPositionTilt),  Cm1, 0 },
  { Zuint16,  Cx0102, 0x0005,  Z_(NumberofActuationsLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0006,  Z_(NumberofActuationsTilt),Cm1, 0 },
  { Zmap8,    Cx0102, 0x0007,  Z_(ConfigStatus),         Cm1, 0 },
  { Zuint8,   Cx0102, 0x0008,  Z_(CurrentPositionLiftPercentage),Cm1, 0 },
  { Zuint8,   Cx0102, 0x0009,  Z_(CurrentPositionTiltPercentage),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0010,  Z_(InstalledOpenLimitLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0011,  Z_(InstalledClosedLimitLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0012,  Z_(InstalledOpenLimitTilt),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0013,  Z_(InstalledClosedLimitTilt),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0014,  Z_(VelocityLift),         Cm1, 0 },
  { Zuint16,  Cx0102, 0x0015,  Z_(AccelerationTimeLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0016,  Z_(DecelerationTimeLift), Cm1, 0 },
  { Zmap8,    Cx0102, 0x0017,  Z_(Mode),                 Cm1, 0 },
  { Zoctstr,  Cx0102, 0x0018,  Z_(IntermediateSetpointsLift),Cm1, 0 },
  { Zoctstr,  Cx0102, 0x0019,  Z_(IntermediateSetpointsTilt),Cm1, 0 },

  // Thermostat
  { Zint16,   Cx0201, 0x0000,  Z_(LocalTemperature),  Cm_100, Z_MAPPING(Z_Data_Thermo, temperature) },
  { Zint16,   Cx0201, 0x0001,  Z_(OutdoorTemperature),Cm_100, 0 },
  { Zuint8,   Cx0201, 0x0007,  Z_(PICoolingDemand),      Cm1, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Zuint8,   Cx0201, 0x0008,  Z_(PIHeatingDemand),      Cm1, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Zint8,    Cx0201, 0x0010,  Z_(LocalTemperatureCalibration), Cm_10, 0 },
  { Zint16,   Cx0201, 0x0011,  Z_(OccupiedCoolingSetpoint), Cm_100, Z_MAPPING(Z_Data_Thermo, temperature_target) },
  { Zint16,   Cx0201, 0x0012,  Z_(OccupiedHeatingSetpoint), Cm_100, Z_MAPPING(Z_Data_Thermo, temperature_target) },
  { Zint16,   Cx0201, 0x0013,  Z_(UnoccupiedCoolingSetpoint), Cm_100, 0 },
  { Zint16,   Cx0201, 0x0014,  Z_(UnoccupiedHeatingSetpoint), Cm_100, 0 },
  { Zmap8,    Cx0201, 0x001A,  Z_(RemoteSensing),        Cm1, 0 },
  { Zenum8,   Cx0201, 0x001B,  Z_(ControlSequenceOfOperation), Cm1, 0 },
  { Zenum8,   Cx0201, 0x001C,  Z_(SystemMode),           Cm1, 0 },
  // below is Eurotronic specific
  { Zenum8,   Cx0201, 0x4000, Z_(TRVMode),               Cm1, 0 },
  { Zuint8,   Cx0201, 0x4001, Z_(ValvePosition),         Cm1, 0 },
  { Zuint8,   Cx0201, 0x4002, Z_(EurotronicErrors),      Cm1, 0 },
  { Zint16,   Cx0201, 0x4003, Z_(CurrentTemperatureSetPoint), Cm_100, 0 },
  { Zuint24,  Cx0201, 0x4008, Z_(EurotronicHostFlags),   Cm1, 0 },
  // below are synthetic virtual attributes used to decode EurotronicHostFlags
  // Last byte acts as a field mask for the lowest byte value
  { Zbool,    Cx0201, 0xF002, Z_(TRVMirrorDisplay),      Cm1, 0 },
  { Zbool,    Cx0201, 0xF004, Z_(TRVBoost),              Cm1, 0 },
  { Zbool,    Cx0201, 0xF010, Z_(TRVWindowOpen),         Cm1, 0 },
  { Zbool,    Cx0201, 0xF080, Z_(TRVChildProtection),    Cm1, 0 },
  // below are virtual attributes to simplify ZbData import/export
  { Zuint8,   Cx0201, 0xFFF0,  Z_(ThSetpoint),           Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Zint16,   Cx0201, 0xFFF1,  Z_(TempTarget),        Cm_100 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, temperature_target) },

  // Color Control cluster
  { Zuint8,   Cx0300, 0x0000,  Z_(Hue),                  Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, hue) },
  { Zuint8,   Cx0300, 0x0001,  Z_(Sat),                  Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, sat) },
  { Zuint16,  Cx0300, 0x0002,  Z_(RemainingTime),        Cm1, 0 },
  { Zuint16,  Cx0300, 0x0003,  Z_(X),                    Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, x) },
  { Zuint16,  Cx0300, 0x0004,  Z_(Y),                    Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, y) },
  { Zenum8,   Cx0300, 0x0005,  Z_(DriftCompensation),    Cm1, 0 },
  { Zstring,  Cx0300, 0x0006,  Z_(CompensationText),     Cm1, 0 },
  { Zuint16,  Cx0300, 0x0007,  Z_(CT),                   Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, ct) },
  { Zenum8,   Cx0300, 0x0008,  Z_(ColorMode),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, colormode) },
  { Zuint8,   Cx0300, 0x0010,  Z_(NumberOfPrimaries),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0011,  Z_(Primary1X),            Cm1, 0 },
  { Zuint16,  Cx0300, 0x0012,  Z_(Primary1Y),            Cm1, 0 },
  { Zuint8,   Cx0300, 0x0013,  Z_(Primary1Intensity),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0015,  Z_(Primary2X),            Cm1, 0 },
  { Zuint16,  Cx0300, 0x0016,  Z_(Primary2Y),            Cm1, 0 },
  { Zuint8,   Cx0300, 0x0017,  Z_(Primary2Intensity),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0019,  Z_(Primary3X),            Cm1, 0 },
  { Zuint16,  Cx0300, 0x001A,  Z_(Primary3Y),            Cm1, 0 },
  { Zuint8,   Cx0300, 0x001B,  Z_(Primary3Intensity),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0030,  Z_(WhitePointX),          Cm1, 0 },
  { Zuint16,  Cx0300, 0x0031,  Z_(WhitePointY),          Cm1, 0 },
  { Zuint16,  Cx0300, 0x0032,  Z_(ColorPointRX),         Cm1, 0 },
  { Zuint16,  Cx0300, 0x0033,  Z_(ColorPointRY),         Cm1, 0 },
  { Zuint8,   Cx0300, 0x0034,  Z_(ColorPointRIntensity), Cm1, 0 },
  { Zuint16,  Cx0300, 0x0036,  Z_(ColorPointGX),         Cm1, 0 },
  { Zuint16,  Cx0300, 0x0037,  Z_(ColorPointGY),         Cm1, 0 },
  { Zuint8,   Cx0300, 0x0038,  Z_(ColorPointGIntensity), Cm1, 0 },
  { Zuint16,  Cx0300, 0x003A,  Z_(ColorPointBX),         Cm1, 0 },
  { Zuint16,  Cx0300, 0x003B,  Z_(ColorPointBY),         Cm1, 0 },
  { Zuint8,   Cx0300, 0x003C,  Z_(ColorPointBIntensity), Cm1, 0 },

  // Illuminance Measurement cluster
  { Zuint16,  Cx0400, 0x0000,  Z_(Illuminance),          Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_PIR, illuminance) }, // Illuminance (in Lux)
  { Zuint16,  Cx0400, 0x0001,  Z_(IlluminanceMinMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0400, 0x0002,  Z_(IlluminanceMaxMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0400, 0x0003,  Z_(IlluminanceTolerance),            Cm1, 0 },    //
  { Zenum8,   Cx0400, 0x0004,  Z_(IlluminanceLightSensorType),      Cm1, 0 },    //
  { Zunk,     Cx0400, 0xFFFF,  Z_(),                    Cm0, 0 },    // Remove all other values

  // Illuminance Level Sensing cluster
  { Zenum8,   Cx0401, 0x0000,  Z_(IlluminanceLevelStatus),          Cm1, 0 },    // Illuminance (in Lux)
  { Zenum8,   Cx0401, 0x0001,  Z_(IlluminanceLightSensorType),      Cm1, 0 },    // LightSensorType
  { Zuint16,  Cx0401, 0x0010,  Z_(IlluminanceTargetLevel),          Cm1, 0 },    //
  { Zunk,     Cx0401, 0xFFFF,  Z_(),                    Cm0, 0 },    // Remove all other values

  // Temperature Measurement cluster
  { Zint16,   Cx0402, 0x0000,  Z_(Temperature),          Cm_100 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, temperature) },
  { Zint16,   Cx0402, 0x0001,  Z_(TemperatureMinMeasuredValue),     Cm_100, 0 },    //
  { Zint16,   Cx0402, 0x0002,  Z_(TemperatureMaxMeasuredValue),     Cm_100, 0 },    //
  { Zuint16,  Cx0402, 0x0003,  Z_(TemperatureTolerance),            Cm_100, 0 },    //
  { Zunk,     Cx0402, 0xFFFF,  Z_(),                    Cm0, 0 },     // Remove all other values

  // Pressure Measurement cluster
  { Zint16,   Cx0403, 0x0000,  Z_(Pressure),                     Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, pressure) },     // Pressure
  { Zint16,   Cx0403, 0x0001,  Z_(PressureMinMeasuredValue),     Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0002,  Z_(PressureMaxMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0403, 0x0003,  Z_(PressureTolerance),            Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0010,  Z_(PressureScaledValue),          Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0011,  Z_(PressureMinScaledValue),       Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0012,  Z_(PressureMaxScaledValue),       Cm1, 0 },    //
  { Zuint16,  Cx0403, 0x0013,  Z_(PressureScaledTolerance),      Cm1, 0 },    //
  { Zint8,    Cx0403, 0x0014,  Z_(PressureScale),                Cm1, 0 },    //
  { Zint16,   Cx0403, 0xFFF0,  Z_(SeaPressure),                  Cm1, Z_MAPPING(Z_Data_Thermo, pressure) },     // Pressure at Sea Level, Tasmota specific
  { Zunk,     Cx0403, 0xFFFF,  Z_(),                    Cm0, 0 },     // Remove all other Pressure values

  // Flow Measurement cluster
  { Zuint16,  Cx0404, 0x0000,  Z_(FlowRate),             Cm_10, 0 },    // Flow (in m3/h)
  { Zuint16,  Cx0404, 0x0001,  Z_(FlowMinMeasuredValue), Cm1, 0 },    //
  { Zuint16,  Cx0404, 0x0002,  Z_(FlowMaxMeasuredValue), Cm1, 0 },    //
  { Zuint16,  Cx0404, 0x0003,  Z_(FlowTolerance),        Cm1, 0 },    //
  { Zunk,     Cx0404, 0xFFFF,  Z_(),                     Cm0, 0 },    // Remove all other values

  // Relative Humidity Measurement cluster
  { Zuint16,  Cx0405, 0x0000,  Z_(Humidity),                     Cm_100 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, humidity) },   // Humidity
  { Zuint16,  Cx0405, 0x0001,  Z_(HumidityMinMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0405, 0x0002,  Z_(HumidityMaxMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0405, 0x0003,  Z_(HumidityTolerance),            Cm1, 0 },    //
  { Zunk,     Cx0405, 0xFFFF,  Z_(),                    Cm0, 0 },     // Remove all other values

  // Occupancy Sensing cluster
  { Zmap8,    Cx0406, 0x0000,  Z_(Occupancy),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_PIR, occupancy) }, // Occupancy (map8)
  { Zenum8,   Cx0406, 0x0001,  Z_(OccupancySensorType),  Cm1, 0 },    // OccupancySensorType
  { Zunk,     Cx0406, 0xFFFF,  Z_(),                    Cm0, 0 },    // Remove all other values

  // IAS Cluster (Intruder Alarm System)
  { Zenum8,   Cx0500, 0x0000,  Z_(ZoneState),             Cm1, 0 },    // Occupancy (map8)
  { Zenum16,  Cx0500, 0x0001,  Z_(ZoneType),              Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Alarm, zone_type) },    // Zone type for sensor
  { Zmap16,   Cx0500, 0x0002,  Z_(ZoneStatus),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Alarm, zone_status) },    // Zone status for sensor
  { Zuint8,   Cx0500, 0xFFF0 + ZA_CIE, Z_(CIE),           Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_PIR, Z_(Occupancy),     Cm1, 0 },    // normally converted to the actual Occupancy 0406/0000
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Contact, Z_(Contact),   Cm1, Z_MAPPING(Z_Data_Alarm, zone_status) },    // We fit the first bit in the LSB
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Fire, Z_(Fire),         Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Water, Z_(Water),        Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_CO, Z_(CO),             Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Personal, Z_(PersonalAlarm),Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Movement, Z_(Movement), Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Panic, Z_(Panic),       Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_GlassBreak, Z_(GlassBreak),Cm1, 0 },

  // Metering (Smart Energy) cluster
  { Zuint48,  Cx0702, 0x0000,  Z_(EnergyTotal),          Cm1, 0 },

  // Meter Identification cluster
  { Zstring,  Cx0B01, 0x0000,  Z_(CompanyName),          Cm1, 0 },
  { Zuint16,  Cx0B01, 0x0001,  Z_(MeterTypeID),          Cm1, 0 },
  { Zuint16,  Cx0B01, 0x0004,  Z_(DataQualityID),        Cm1, 0 },
  { Zstring,  Cx0B01, 0x0005,  Z_(CustomerName),         Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x0006,  Z_(Model),                Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x0007,  Z_(PartNumber),           Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x0008,  Z_(ProductRevision),      Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x000A,  Z_(SoftwareRevision),     Cm1, 0 },
  { Zstring,  Cx0B01, 0x000B,  Z_(UtilityName),          Cm1, 0 },
  { Zstring,  Cx0B01, 0x000C,  Z_(POD),                  Cm1, 0 },
  { Zint24,   Cx0B01, 0x000D,  Z_(AvailablePower),       Cm1, 0 },
  { Zint24,   Cx0B01, 0x000E,  Z_(PowerThreshold),       Cm1, 0 },

  // Electrical Measurement cluster
  { Zuint16,  Cx0B04, 0x0505,  Z_(RMSVoltage),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Plug, mains_voltage) },
  { Zuint16,  Cx0B04, 0x0508,  Z_(RMSCurrent),            Cm1, 0 },
  { Zint16,   Cx0B04, 0x050B,  Z_(ActivePower),           Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Plug, mains_power) },
  { Zint16,   Cx0B04, 0x050E,  Z_(ReactivePower),         Cm1, 0 },
  { Zint16,   Cx0B04, 0x050F,  Z_(ApparentPower),         Cm1, 0 },

  // Diagnostics cluster
  { Zuint16,  Cx0B05, 0x0000,  Z_(NumberOfResets),       Cm1, 0 },
  { Zuint16,  Cx0B05, 0x0001,  Z_(PersistentMemoryWrites),Cm1, 0 },
  { Zuint8,   Cx0B05, 0x011C,  Z_(LastMessageLQI),       Cm1, 0 },
  { Zuint8,   Cx0B05, 0x011D,  Z_(LastMessageRSSI),      Cm1, 0 },

  // Tuya Moes specific - 0xEF00
  // Mapping of Tuya type with internal mapping
  // 0x00 - Zoctstr (len N)
  // 0x01 - Ztuya1 (len 1) - equivalent to Zuint8 without invalid value handling
  // 0x02 - Ztuya4 (len 4) - equivalent to Zint32 in big endian and without invalid value handling
  // 0x03 - Zstr (len N)
  // 0x04 - Ztuya1 (len 1)
  // 0x05 - Ztuya4u (len 1/2/4) - equivalent to Zuint32
  { Ztuya0,   CxEF00, 0x0070,  Z_(TuyaScheduleWorkdays), Cm1, 0 },
  { Ztuya0,   CxEF00, 0x0071,  Z_(TuyaScheduleHolidays), Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0101,  Z_(Power),                Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0102,  Z_(Power2),               Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0103,  Z_(Power3),               Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0104,  Z_(Power4),               Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0107,  Z_(TuyaChildLock),        Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0112,  Z_(TuyaWindowDetection),  Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0114,  Z_(TuyaValveDetection),   Cm1, 0 },
  { Ztuya1,   CxEF00, 0x0174,  Z_(TuyaAutoLock),         Cm1, 0 },
  { Zint16,   CxEF00, 0x0202,  Z_(TuyaTempTarget),       Cm_10, Z_MAPPING(Z_Data_Thermo, temperature_target) },
  { Zint16,   CxEF00, 0x0203,  Z_(LocalTemperature),     Cm_10, Z_MAPPING(Z_Data_Thermo, temperature) },  // will be overwritten by actual LocalTemperature
  { Ztuya2,   CxEF00, 0x0215,  Z_(TuyaBattery),          Cm1, 0 },   // TODO check equivalent?
  { Ztuya2,   CxEF00, 0x0266,  Z_(TuyaMinTemp),          Cm1, 0 },
  { Ztuya2,   CxEF00, 0x0267,  Z_(TuyaMaxTemp),          Cm1, 0 },
  { Ztuya2,   CxEF00, 0x0269,  Z_(TuyaBoostTime),        Cm1, 0 },
  { Ztuya2,   CxEF00, 0x026B,  Z_(TuyaComfortTemp),      Cm1, 0 },
  { Ztuya2,   CxEF00, 0x026C,  Z_(TuyaEcoTemp),          Cm1, 0 },
  { Zuint8,   CxEF00, 0x026D,  Z_(TuyaValvePosition),    Cm1, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Ztuya2,   CxEF00, 0x0272,  Z_(TuyaAwayTemp),         Cm1, 0 },
  { Ztuya2,   CxEF00, 0x0275,  Z_(TuyaAwayDays),         Cm1, 0 },
  { Ztuya4,   CxEF00, 0x0404,  Z_(TuyaPreset),           Cm1, 0 },
  { Ztuya4,   CxEF00, 0x0405,  Z_(TuyaFanMode),          Cm1, 0 },
  { Ztuya4,   CxEF00, 0x046A,  Z_(TuyaForceMode),        Cm1, 0 },
  { Ztuya4,   CxEF00, 0x046F,  Z_(TuyaWeekSelect),       Cm1, 0 },

  // Aqara Opple spacific
  { Zuint8,   CxFCC0, 0x0009,  Z_(OppleMode),            Cm1, 0 },

  // Terncy specific - 0xFCCC
  { Zuint16, CxFCCC, 0x001A,  Z_(TerncyDuration),        Cm1, 0 },
  { Zint16,  CxFCCC, 0x001B,  Z_(TerncyRotate),          Cm1, 0 },
};
#pragma GCC diagnostic pop

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
                                    uint8_t *zigbee_type = nullptr, Z_Data_Type *data_type = nullptr, uint8_t *map_offset = nullptr) {
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    if (0 == pgm_read_word(&converter->name_offset)) { continue; }         // avoid strcasecmp_P() from crashing
    if (0 == strcasecmp_P(command, Z_strings + pgm_read_word(&converter->name_offset))) {
      if (cluster)      { *cluster    = CxToCluster(pgm_read_byte(&converter->cluster_short)); }
      if (attribute)    { *attribute  = pgm_read_word(&converter->attribute); }
      if (multiplier)   { *multiplier = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx)); }
      if (zigbee_type)  { *zigbee_type = pgm_read_byte(&converter->type); }
      uint8_t conv_mapping = pgm_read_byte(&converter->mapping);
      if (data_type)    { *data_type = (Z_Data_Type) ((conv_mapping & 0xF0)>>4); }
      if (map_offset)   { *map_offset = (conv_mapping & 0x0F); }
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
      if (multiplier)   { *multiplier = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx)); }
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
                    "\"groupid\":%d," "\"clusterid\":\"0x%04X\"," "\"srcaddr\":\"0x%04X\","
                    "\"srcendpoint\":%d," "\"dstendpoint\":%d," "\"wasbroadcast\":%d,"
                    "\"" D_CMND_ZIGBEE_LINKQUALITY "\":%d," "\"securityuse\":%d," "\"seqnumber\":%d,"
                    "\"fc\":\"0x%02X\","
                    "\"frametype\":%d,\"direction\":%d,\"disableresp\":%d,"
                    "\"manuf\":\"0x%04X\",\"transact\":%d,"
                    "\"cmdid\":\"0x%02X\",\"payload\":\"%s\"}}"),
                    _groupaddr, _cluster_id, _srcaddr,
                    _srcendpoint, _dstendpoint, _wasbroadcast,
                    _linkquality, _securityuse, _seqnumber,
                    _frame_control,
                    _frame_control.b.frame_type, _frame_control.b.direction, _frame_control.b.disable_def_resp,
                    _manuf_code, _transact_seq, _cmd_id,
                    hex_char);
    if (Settings.flag3.tuya_serial_mqtt_publish) {
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR));
    } else {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), TasmotaGlobal.mqtt_data);
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
  void removeInvalidAttributes(Z_attribute_list& attr_list);
  void computeSyntheticAttributes(Z_attribute_list& attr_list);
  void generateCallBacks(Z_attribute_list& attr_list);
  void parseReadAttributes(Z_attribute_list& attr_list);
  void parseReadAttributesResponse(Z_attribute_list& attr_list);
  void parseReadConfigAttributes(Z_attribute_list& attr_list);
  void parseConfigAttributes(Z_attribute_list& attr_list);
  void parseWriteAttributesResponse(Z_attribute_list& attr_list);
  void parseResponse(void);
  void parseResponse_inner(uint8_t cmd, bool cluster_specific, uint8_t status);
  void parseClusterSpecificCommand(Z_attribute_list& attr_list);

  // synthetic attributes converters
  void syntheticAqaraSensor(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraSensor2(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraCubeOrButton(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraVibration(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAnalogValue(Z_attribute_list &attr_list, class Z_attribute &attr);

  // handle read attributes auto-responder
  void autoResponder(const uint16_t *attr_list_ids, size_t attr_len);

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
  uint32_t len = Z_getDatatypeLen(attrtype);    // pre-compute length, overloaded for variable length attributes
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
    case Zuint24:
      buf.add16(u32);
      buf.add8(u32 >> 16);
      break;
    // unisgned 24
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
    case Zuint24:
      {
        uint32_t uint24_val = buf.get16(i) + (buf.get8(i+2) >> 16);
        // i += 3;
        if (0xFFFFFF != uint24_val) {
          attr.setUInt(uint24_val);
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
        if (-0x80000000 != int32_val) {
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
        len = (attrtype <= Zstring) ? buf.get8(i) : buf.get16(i);    // len is 8 or 16 bits
        i += (attrtype <= Zstring) ? 1 : 2;                                   // increment pointer
        if (i + len > buf.len()) {        // make sure we don't get past the buffer
          len = buf.len() - i;
        }

        // check if we can safely use a string
        if ((Zoctstr == attrtype) || (Zoctstr16 == attrtype)) { parse_as_string = false; }

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
      true /* direct no retry */,
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
      case 0x000C0055:    // Analog Value
        syntheticAnalogValue(attr_list, attr);
        break;
    }
  }
}

//
// Remove invalid values
//
void ZCLFrame::removeInvalidAttributes(Z_attribute_list& attr_list) {
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.key.id.cluster << 16) | attr.key.id.attr_id;

    switch (ccccaaaa) {      // 0xccccaaaa . c=cluster, a=attribute
      case 0x04020000:       // Temperature
        if (attr.getInt() <= -10000) {
          // #9978, remove temperature of -100.00°C sent by lumi.weather
          attr_list.removeAttribute(&attr);
        }
        break;
    }
  }
}

//
// Compute new attributes based on the standard set
// Note: both function are now split to compute on extracted attributes
//
void ZCLFrame::computeSyntheticAttributes(Z_attribute_list& attr_list) {
  const char * model_c = zigbee_devices.getModelId(_srcaddr);  // null if unknown
  String modelId((char*) model_c);
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
      case 0x00010021:       // BatteryPercentage
        if (modelId.startsWith(F("TRADFRI"))) {
          attr.setUInt(attr.getUInt() * 2);   // bug in TRADFRI battery, need to double the value
        }
        break;
      case 0x00060000:    // "Power" for lumi Door/Window is converted to "Contact"
        if (modelId.startsWith(F("lumi.sensor_magnet"))) {
          attr.setKeyId(0x0500, 0xFFF0 + ZA_Contact);    // change cluster and attribute to 0500/FFF0
        }
        break;
      case 0x02010008:    // Pi Heating Demand - solve Eutotronic bug
      case 0x02014008:    // Eurotronic Host Flags decoding
        {
          const char * manufacturer_c = zigbee_devices.getManufacturerId(_srcaddr);  // null if unknown
          String manufacturerId((char*) manufacturer_c);
          if (manufacturerId.equals(F("Eurotronic"))) {
            if (ccccaaaa == 0x02010008) {
              // Eurotronic does not report 0..100 but 0..255, including 255 which is normally an ivalid value
              uint8_t valve = attr.getUInt();
              if (attr.isNone()) { valve = 255; }
              uint8_t valve_100 = changeUIntScale(valve, 0, 255, 0, 100);
              attr.setUInt(valve_100);
            } else if (ccccaaaa == 0x02014008) {
              uint32_t mode = attr.getUInt();
              if (mode & 0x02) { attr_list.addAttribute(0x0201, 0xF002).setUInt(1); }
              if (mode & 0x04) { attr_list.addAttribute(0x0201, 0xF004).setUInt(1); }
              if (mode & 0x10) { attr_list.addAttribute(0x0201, 0xF010).setUInt(1); }
              if (mode & 0x80) { attr_list.addAttribute(0x0201, 0xF080).setUInt(1); }
            }
          }
        }
        break;
      case 0x04030000:    // Pressure
        {
          int16_t pressure = attr.getInt();
          int16_t pressure_sealevel = (pressure / FastPrecisePow(1.0 - ((float)Settings.altitude / 44330.0f), 5.255f)) - 21.6f;
          attr_list.addAttribute(0x0403, 0xFFF0).setInt(pressure_sealevel);
          // We create a synthetic attribute 0403/FFF0 to indicate sea level
        }
        break;
      case 0x05000002:    // ZoneStatus
        const Z_Data_Alarm & alarm = (const Z_Data_Alarm&) zigbee_devices.getShortAddr(_srcaddr).data.find(Z_Data_Type::Z_Alarm, _srcendpoint);
        if (&alarm != nullptr) {
          alarm.convertZoneStatus(attr_list, attr.getUInt());
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
          uint32_t pir_timer = OCCUPANCY_TIMEOUT;
          const Z_Data_PIR & pir_found = (const Z_Data_PIR&) zigbee_devices.getShortAddr(_srcaddr).data.find(Z_Data_Type::Z_PIR, _srcendpoint);
          if (&pir_found != nullptr) {
            pir_timer = pir_found.getTimeoutSeconds() * 1000;
          }
          if (pir_timer > 0) {
            zigbee_devices.setTimer(_srcaddr, 0 /* groupaddr */, pir_timer, _cluster_id, _srcendpoint, Z_CAT_VIRTUAL_OCCUPANCY, 0, &Z_OccupancyCallback);
          }
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

  attr_list.addAttributePMEM(PSTR(D_CMND_ZIGBEE_CLUSTER)).setUInt(_cluster_id);

  JsonGeneratorArray attr_numbers;
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
  attr_list.addAttributePMEM(PSTR("Read")).setStrRaw(attr_numbers.toString().c_str());
  attr_list.addAttributePMEM(PSTR("ReadNames")).setStrRaw(attr_names.toString(true).c_str());

  // call auto-responder
  autoResponder(read_attr_ids, len/2);
}

// ZCL_CONFIGURE_REPORTING_RESPONSE
void ZCLFrame::parseConfigAttributes(Z_attribute_list& attr_list) {
  uint32_t len = _payload.len();

  Z_attribute_list attr_config_list;
  for (uint32_t i=0; len >= i+4; i+=4) {
    uint8_t  status = _payload.get8(i);
    uint16_t attr_id = _payload.get8(i+2);

    Z_attribute_list attr_config_response;
    attr_config_response.addAttributePMEM(PSTR("Status")).setUInt(status);
    attr_config_response.addAttributePMEM(PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(status).c_str());

    const __FlashStringHelper* attr_name = zigbeeFindAttributeById(_cluster_id, attr_id, nullptr, nullptr);
    if (attr_name) {
      attr_config_list.addAttribute(attr_name).setStrRaw(attr_config_response.toString(true).c_str());
    } else {
      attr_config_list.addAttribute(_cluster_id, attr_id).setStrRaw(attr_config_response.toString(true).c_str());
    }
  }

  Z_attribute &attr_1 = attr_list.addAttributePMEM(PSTR("ConfigResponse"));
  attr_1.setStrRaw(attr_config_list.toString(true).c_str());
}

// ZCL_WRITE_ATTRIBUTES_RESPONSE
void ZCLFrame::parseWriteAttributesResponse(Z_attribute_list& attr_list) {
  parseResponse_inner(ZCL_WRITE_ATTRIBUTES_RESPONSE, false, _payload.get8(0));
}

// ZCL_READ_REPORTING_CONFIGURATION_RESPONSE
void ZCLFrame::parseReadConfigAttributes(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = _payload.len();

  Z_attribute &attr_root = attr_list.addAttributePMEM(PSTR("ReadConfig"));
  Z_attribute_list attr_1;

  while (len >= i + 4) {
    uint8_t  status = _payload.get8(i);
    uint8_t  direction = _payload.get8(i+1);
    uint16_t attrid = _payload.get16(i+2);

    Z_attribute_list attr_2;
    if (direction) {
      attr_2.addAttributePMEM(PSTR("DirectionReceived")).setBool(true);
    }

    // find the attribute name
    int8_t multiplier = 1;
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint16_t conv_attribute = pgm_read_word(&converter->attribute);

      if ((conv_cluster == _cluster_id) && (conv_attribute == attrid)) {
        const char * attr_name = Z_strings + pgm_read_word(&converter->name_offset);
        attr_2.addAttribute(attr_name, true).setBool(true);
        multiplier = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
        break;
      }
    }
    i += 4;
    if (0 != status) {
      attr_2.addAttributePMEM(PSTR("Status")).setUInt(status);
      attr_2.addAttributePMEM(PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(status).c_str());
    } else {
      // no error, decode data
      if (direction) {
        // only Timeout period is present
        uint16_t attr_timeout = _payload.get16(i);
        i += 2;
        attr_2.addAttributePMEM(PSTR("TimeoutPeriod")).setUInt((0xFFFF == attr_timeout) ? -1 : attr_timeout);
      } else {
        // direction == 0, we have a data type
        uint8_t attr_type = _payload.get8(i);
        bool attr_discrete = Z_isDiscreteDataType(attr_type);
        uint16_t attr_min_interval = _payload.get16(i+1);
        uint16_t attr_max_interval = _payload.get16(i+3);
        i += 5;
        attr_2.addAttributePMEM(PSTR("MinInterval")).setUInt((0xFFFF == attr_min_interval) ? -1 : attr_min_interval);
        attr_2.addAttributePMEM(PSTR("MaxInterval")).setUInt((0xFFFF == attr_max_interval) ? -1 : attr_max_interval);
        if (!attr_discrete) {
          // decode Reportable Change
          Z_attribute &attr_change = attr_2.addAttributePMEM(PSTR("ReportableChange"));
          i += parseSingleAttribute(attr_change, _payload, i, attr_type);
          if ((1 != multiplier) && (0 != multiplier)) {
            float fval = attr_change.getFloat();
            if (multiplier > 0) { fval =  fval * multiplier; }
            else                { fval =  fval / (-multiplier); }
            attr_change.setFloat(fval);
          }
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


void ZCLFrame::parseResponse_inner(uint8_t cmd, bool cluster_specific, uint8_t status) {
  Z_attribute_list attr_list;

  // "Device"
  char s[12];
  snprintf_P(s, sizeof(s), PSTR("0x%04X"), _srcaddr);
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_DEVICE)).setStr(s);
  // "Name"
  const char * friendlyName = zigbee_devices.getFriendlyName(_srcaddr);
  if (friendlyName) {
    attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_NAME)).setStr(friendlyName);
  }
  // "Command"
  snprintf_P(s, sizeof(s), PSTR("%04X%c%02X"), _cluster_id, cluster_specific ? '!' : '_', cmd);
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_CMD)).setStr(s);
  // "Status"
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_STATUS)).setUInt(status);
  // "StatusMessage"
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_STATUS_MSG)).setStr(getZigbeeStatusMessage(status).c_str());
  // Add Endpoint
  attr_list.addAttributePMEM(PSTR(D_CMND_ZIGBEE_ENDPOINT)).setUInt(_srcendpoint);
  // Add Group if non-zero
  if (_groupaddr) {     // TODO what about group zero
    attr_list.group_id = _groupaddr;
  }
  // Add linkquality
  attr_list.lqi = _linkquality;

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_RESPONSE "\":%s}"), attr_list.toString(true).c_str());
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
}

// ZCL_DEFAULT_RESPONSE
void ZCLFrame::parseResponse(void) {
  if (_payload.len() < 2) { return; }   // wrong format
  uint8_t cmd = _payload.get8(0);
  uint8_t status = _payload.get8(1);

  parseResponse_inner(cmd, true, status);
}

// Parse non-normalized attributes
void ZCLFrame::parseClusterSpecificCommand(Z_attribute_list& attr_list) {
  convertClusterSpecific(attr_list, _cluster_id, _cmd_id, _frame_control.b.direction, _srcaddr, _srcendpoint, _payload);
  if (!Settings.flag5.zb_disable_autoquery) {
  // read attributes unless disabled
    if (!_frame_control.b.direction) {    // only handle server->client (i.e. device->coordinator)
      if (_wasbroadcast) {                // only update for broadcast messages since we don't see unicast from device to device and we wouldn't know the target
        sendHueUpdate(BAD_SHORTADDR, _groupaddr, _cluster_id);
      }
    }
  }
}

// ======================================================================
// Convert AnalogValue according to the device type
void ZCLFrame::syntheticAnalogValue(Z_attribute_list &attr_list, class Z_attribute &attr) {
  const char * modelId_c = zigbee_devices.getModelId(_srcaddr);  // null if unknown
  String modelId((char*) modelId_c);

  if (modelId.startsWith(F("lumi.sensor_cube"))) {
    attr.setKeyId(0x000C, 0xFF55);    // change to AqaraRotate
  }
  if (modelId.startsWith(F("lumi.plug"))) {
    attr.setKeyId(0x0702, 0x0000);    // change to EnergyTotal
  }
  if (modelId.startsWith(F("lumi.ctrl"))) {
    attr.setKeyId(0x0B04, 0x050B);    // change to ActivePower
  }
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
      uint32_t uval32 = attr.getUInt();
      bool translated = false;    // were we able to translate to a known format?
      if (0x01 == attrid) {
        float batteryvoltage = attr.getFloat() / 100;
        attr_list.addAttribute(0x0001, 0x0020).setFloat(batteryvoltage);
        uint8_t batterypercentage = toPercentageCR2032(uval32);
        attr_list.addAttribute(0x0001, 0x0021).setUInt(batterypercentage * 2);
      } else if ((nullptr != modelId) && ((0 == getManufCode()) || (0x115F == getManufCode()))) {
        translated = true;
        if (modelId.startsWith(F("lumi.sensor_magnet"))) {   // door / window sensor
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0500, 0xFFF0 + ZA_Contact).copyVal(attr);   // Contact
          }
        } else if (modelId.startsWith(F("lumi.sensor_smoke"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttributePMEM(PSTR("SmokeDensity")).copyVal(attr);
          }
        } else if (modelId.startsWith(F("lumi.sensor_natgas"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttributePMEM(PSTR("GasDensity")).copyVal(attr);
          }
        } else if (modelId.startsWith(F("lumi.sensor_ht")) ||
            modelId.equals(F("lumi.sens")) ||
            modelId.startsWith(F("lumi.weather"))) {     // Temp sensor
          // Filter according to prefix of model name
          // onla Aqara Temp/Humidity has manuf_code of zero. If non-zero we skip the parameters
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0402, 0x0000).setInt(ival32);   // Temperature
          } else if (0x65 == attrid) {
            attr_list.addAttribute(0x0405, 0x0000).setUInt(uval32);         // Humidity * 100
          } else if (0x66 == attrid) {
            attr_list.addAttribute(0x0403, 0x0000).setUInt((ival32 + 50) / 100);  // Pressure
          }
        } else if (modelId.startsWith(F("lumi.plug")) || modelId.startsWith(F("lumi.ctrl"))) {
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0006, 0x0000).setInt(uval32);          // Power (on/off)
          } else if (0x98 == attrid) {
            attr_list.addAttribute(0x0B04, 0x050B).setInt(ival32);          // Active Power
          } else if (0x95 == attrid) {
            attr_list.addAttribute(0x0702, 0x0000).setUInt(uval32);         // EnergyDelivered
          }
        } else {
          translated = false;     // we didn't find a match
        }
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
  } else if (modelId.startsWith(F("lumi.remote")) || modelId.startsWith(F("lumi.sensor_swit"))) {   // only for Aqara buttons WXKG11LM & WXKG12LM, 'swit' because of #9923
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
          attr_list.addAttributePMEM(PSTR("AqaraAngles")).setStrRaw(temp);
        }
      }
      break;
  }
}

/// Publish a message for `"Occupancy":0` when the timer expired
void Z_OccupancyCallback(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  Z_attribute_list attr_list;
  attr_list.addAttribute(0x0406, 0x0000).setUInt(0);        // Occupancy
  Z_postProcessAttributes(shortaddr, endpoint, attr_list);  // make sure all is updated accordingly
  zigbee_devices.jsonPublishNow(shortaddr, attr_list);
}

// ======================================================================
void Z_postProcessAttributes(uint16_t shortaddr, uint16_t src_ep, class Z_attribute_list& attr_list) {
  Z_Device & device = zigbee_devices.getShortAddr(shortaddr);
  uint8_t count_ep = device.countEndpoints();

  for (auto &attr : attr_list) {
    // add endpoint suffix if needed
    if ((Settings.flag4.zb_index_ep) && (src_ep != 1) && (count_ep > 1)) {
      // we need to add suffix if the suffix is not already different from 1
      if (attr.key_suffix == 1) {
        attr.key_suffix = src_ep;
      }
    }

    // attr is Z_attribute&
    if (!attr.key_is_str) {
      uint16_t cluster = attr.key.id.cluster;
      uint16_t attribute = attr.key.id.attr_id;
      uint32_t ccccaaaa = (attr.key.id.cluster << 16) | attr.key.id.attr_id;

      // Look for an entry in the converter table
      bool found = false;
      const char * conv_name;
      Z_Data_Type map_type = Z_Data_Type::Z_Unknown;
      uint8_t map_offset = 0;
      uint8_t zigbee_type = Znodata;
      int8_t conv_multiplier;
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
        uint16_t conv_attribute = pgm_read_word(&converter->attribute);

        if ((conv_cluster == cluster) &&
            ((conv_attribute == attribute) || (conv_attribute == 0xFFFF)) ) {
          conv_multiplier = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
          zigbee_type = pgm_read_byte(&converter->type);
          uint8_t mapping = pgm_read_byte(&converter->mapping);
          map_type = (Z_Data_Type) ((mapping & 0xF0)>>4);
          map_offset = (mapping & 0x0F);
          conv_name = Z_strings + pgm_read_word(&converter->name_offset);
          found = true;
          break;
        }
      }

      float    fval   = attr.getFloat();
      if (found && (map_type != Z_Data_Type::Z_Unknown)) {
        // We apply an automatic mapping to Z_Data_XXX object
        // First we find or instantiate the correct Z_Data_XXX accorfing to the endpoint
        // Then store the attribute at the attribute addres (via offset) and according to size 8/16/32 bits

        // add the endpoint if it was not already known
        device.addEndpoint(src_ep);
        // we don't apply the multiplier, but instead store in Z_Data_XXX object
        Z_Data & data = device.data.getByType(map_type, src_ep);
        uint8_t *attr_address = ((uint8_t*)&data) + sizeof(Z_Data) + map_offset;
        uint32_t uval32 = attr.getUInt();     // call converter to uint only once
        int32_t  ival32 = attr.getInt();     // call converter to int only once
        // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "Mapping type=%d offset=%d zigbee_type=%02X value=%d\n"), (uint8_t) map_type, map_offset, zigbee_type, ival32);
        switch (ccccaaaa) {
          case 0xEF000202:
          case 0xEF000203:    // need to convert Tuya temperatures from 1/10 to 1/00 °C
            ival32 = ival32 * 10;
            break;
        }

        switch (zigbee_type) {
          case Zenum8:
          case Zmap8:
          case Zbool:
          case Zuint8:  *(uint8_t*)attr_address  = uval32;          break;
          case Zenum16:
          case Zmap16:
          case Zuint16: *(uint16_t*)attr_address = uval32;          break;
          case Zuint32: *(uint32_t*)attr_address = uval32;          break;
          case Zint8:   *(int8_t*)attr_address   = ival32;           break;
          case Zint16:  *(int16_t*)attr_address  = ival32;           break;
          case Zint32:  *(int32_t*)attr_address  = ival32;           break;
        }
        if (Z_Data_Set::updateData(data)) {
          zigbee_devices.dirty();
        }
      }

      uint16_t uval16 = attr.getUInt();     // call converter to uint only once
      int16_t  ival16 = attr.getInt();     // call converter to int only once
      // update any internal structure
      switch (ccccaaaa) {
        case 0x00000004: device.setManufId(attr.getStr());                            break;
        case 0x00000005: device.setModelId(attr.getStr());                            break;
        case 0x00010021: device.setBatteryPercent(uval16 / 2);                        break;
        case 0x00060000:
        case 0x00068000: device.setPower(attr.getBool(), src_ep);                     break;
      }

      // now apply the multiplier to make it human readable
      if (found) {
        if (0 == conv_multiplier)  { attr_list.removeAttribute(&attr); continue; }      // remove attribute if multiplier is zero
        if (1 != conv_multiplier) {
          if (conv_multiplier > 0) { fval =  fval * conv_multiplier; }
          else                     { fval =  fval / (-conv_multiplier); }
          attr.setFloat(fval);
        }
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

// Internal search function
void Z_parseAttributeKey_inner(class Z_attribute & attr, uint16_t preferred_cluster) {
  // scan attributes to find by name, and retrieve type
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    uint16_t local_attr_id = pgm_read_word(&converter->attribute);
    uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
    uint8_t  local_type_id = pgm_read_byte(&converter->type);
    int8_t   local_multiplier = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR("Try cluster = 0x%04X, attr = 0x%04X, type_id = 0x%02X"), local_cluster_id, local_attr_id, local_type_id);

    if (!attr.key_is_str) {
      if ((attr.key.id.cluster == local_cluster_id) && (attr.key.id.attr_id == local_attr_id)) {
        attr.attr_type = local_type_id;
        break;
      }
    } else if (pgm_read_word(&converter->name_offset)) {
      const char * key = attr.key.key;
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR("Comparing '%s' with '%s'"), attr_name, converter->name);
      if (0 == strcasecmp_P(key, Z_strings + pgm_read_word(&converter->name_offset))) {
        if ((preferred_cluster == 0xFFFF) ||    // any cluster
            (local_cluster_id == preferred_cluster)) {
          // match
          attr.setKeyId(local_cluster_id, local_attr_id);
          attr.attr_type = local_type_id;
          attr.attr_multiplier = local_multiplier;
          break;
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
bool Z_parseAttributeKey(class Z_attribute & attr, uint16_t preferred_cluster) {
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
  // AddLog_P(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X"), cluster_id, attr_id);

  // do we already know the type, i.e. attribute and cluster are also known
  if ((Zunk == attr.attr_type) && (preferred_cluster != 0xFFFF)) {
    Z_parseAttributeKey_inner(attr, preferred_cluster);   // try to find with the selected cluster
  }
  if (Zunk == attr.attr_type) {
    Z_parseAttributeKey_inner(attr, 0xFFFF);    // try again with any cluster
  }
  return (Zunk != attr.attr_type) ? true : false;
}

// generic toAttributes() based on declaration in the attribute array
// can be overloaded for specific objects
// Input:
//  the Json object to add attributes to
//  the type of object (necessary since the type system is unaware of the actual sub-type)
void Z_Data::toAttributes(Z_attribute_list & attr_list) const {
  Z_Data_Type type = getType();
  // iterate through attributes to see which ones need to be exported
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    uint8_t conv_export = pgm_read_byte(&converter->multiplier_idx) & Z_EXPORT_DATA;
    uint8_t conv_mapping = pgm_read_byte(&converter->mapping);
    int8_t  multiplier = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
    Z_Data_Type map_type = (Z_Data_Type) ((conv_mapping & 0xF0)>>4);
    uint8_t map_offset = (conv_mapping & 0x0F);

    if ((conv_export != 0) && (map_type == type)) {
      // we need to export this attribute
      const char * conv_name = Z_strings + pgm_read_word(&converter->name_offset);
      uint8_t zigbee_type = pgm_read_byte(&converter->type);                    // zigbee type to select right size 8/16/32 bits
      uint8_t * attr_address = ((uint8_t*)this) + sizeof(Z_Data) + map_offset;   // address of attribute in memory

      int32_t data_size = 0;
      int32_t ival32;
      uint32_t uval32;
      switch (zigbee_type) {
        case Zenum8:
        case Zmap8:
        case Zbool:
        case Zuint8:  uval32 = *(uint8_t*)attr_address;   if (uval32 !=  0x000000FF) data_size = 8;   break;
        case Zmap16:
        case Zenum16:
        case Zuint16: uval32 = *(uint16_t*)attr_address;  if (uval32 !=  0x0000FFFF) data_size = 16;  break;
        case Zuint32: uval32 = *(uint32_t*)attr_address;  if (uval32 !=  0xFFFFFFFF) data_size = 32;  break;
        case Zint8:   ival32 = *(int8_t*)attr_address;    if (ival32 != -0x80)       data_size = -8;  break;
        case Zint16:  ival32 = *(int16_t*)attr_address;   if (ival32 != -0x8000)     data_size = -16; break;
        case Zint32:  ival32 = *(int32_t*)attr_address;   if (ival32 != -0x80000000) data_size = -32; break;
      }
      if (data_size != 0) {
        Z_attribute & attr = attr_list.addAttribute(conv_name);

        float fval;
        if (data_size > 0) { fval = uval32; }
        else               { fval = ival32; }
        if ((1 != multiplier) && (0 != multiplier)) {
          if (multiplier > 0) { fval =  fval * multiplier; }
          else                { fval =  fval / (-multiplier); }
        }
        attr.setFloat(fval);
      }
    }
  }
}

//
// Check if this device needs Battery reporting
// This is usefule for IKEA device that tend to drain battery quickly when Battery reporting is set
//
bool Z_BatteryReportingDeviceSpecific(uint16_t shortaddr) {
  const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  if (device.manufacturerId) {
    String manuf_c(device.manufacturerId);
    if (manuf_c.startsWith(F("IKEA"))) {
      return false;
    }
  }
  return true;
}

#endif // USE_ZIGBEE
