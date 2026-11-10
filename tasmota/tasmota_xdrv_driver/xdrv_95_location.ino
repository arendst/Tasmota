/*
  xdrv_95_location.ino - Device location and pose support for Tasmota

  SPDX-FileCopyrightText: 2026 Theo Arends and Clemens Arth

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_LOCATION
/*********************************************************************************************\
 * Device location and pose
 *
 * A device can store where it is and how it is oriented, so that tools (e.g. 3D building
 * views) do not need to derive location from MQTT topic naming conventions.
 * See https://github.com/arendst/Tasmota/discussions/25222
 *
 * Global anchor (WGS84) - reuses existing settings, also used by sunrise/sunset timers:
 *   Latitude <deg>          - Settings->latitude  (int32, degrees * 1e6)
 *   Longitude <deg>         - Settings->longitude (int32, degrees * 1e6)
 *   Altitude <m>            - Settings->altitude  (int16, metres)
 *
 * Local pose (house/room frame):
 *   Location                - Show global anchor and local pose
 *   Location x,y,z          - Set local position (float, in metres when scale is 3)
 *   Location x,y,z,a,b,c    - Set local position and orientation (degrees)
 *   LocationRot a,b,c       - Set orientation alpha/beta/gamma (yaw/pitch/roll, ZYX) in degrees
 *   LocationScale 0..6      - Decimal shift for x/y/z (3 = mm resolution, 6 = um).
 *                             Values are rescaled, so the represented position is kept.
 *   LocationClear           - Clear local pose
 *   SetOption166 1          - Add "Location" to tele/STATE
 *
 * Pose storage (two tier):
 * - With a mounted file system (USE_UFILESYS) as JSON in /.drvset000 using key "drvset95" with full
 *   precision: x/y/z int32 in units of 10^-scale (+/-2147 km at mm), rotation int16 centidegrees.
 *   {"drvset95":{"Scale":3,"Flags":1,"Pos":[1250,3400,900],"Rot":[1235,-50,-17000]}}
 *   Config backup and restore include it.
 * - Without a mounted file system (ESP8266) packed into 96 bits in Settings->location_packed (12 bytes
 *   at 0x3D0, ex gpio16_converted): x/y/z 21 bit (+/-1048575 units, +/-1048 m at mm), rotation 10 bit
 *   (360/1024 = ~0.35 degree steps), scale 3 bit. Values are reduced to this precision when
 *   set, so what is shown is what survives a restart.
 * - On ESP8266 a packed pose is moved to the file once a file system is mounted.
 * - ESP32 always has a file system.
\*********************************************************************************************/

#define XDRV_95                     95

#if defined(USE_UFILESYS) || defined(ESP8266)
#define LOCATION_HAS_POSE                    // Pose needs a file system or the ESP8266 settings area
#endif

#define LOCATION_FLAG_SET           0x01
#define LOCATION_SCALE_MAX          6
#define LOCATION_SCALE_DEFAULT      3

const char kLocationCommands[] PROGMEM = "|"  // No prefix
  D_CMND_LATITUDE "|" D_CMND_LONGITUDE "|" D_CMND_LOCATION
#ifdef LOCATION_HAS_POSE
  "|" D_CMND_LOCATIONROT "|" D_CMND_LOCATIONSCALE "|" D_CMND_LOCATIONCLEAR
#endif  // LOCATION_HAS_POSE
  ;

void (* const LocationCommand[])(void) PROGMEM = {
  &CmndLatitude, &CmndLongitude, &CmndLocation
#ifdef LOCATION_HAS_POSE
  , &CmndLocationRot, &CmndLocationScale, &CmndLocationClear
#endif  // LOCATION_HAS_POSE
  };

#ifdef LOCATION_HAS_POSE
/*********************************************************************************************\
 * Pose storage
\*********************************************************************************************/

typedef struct {
  int32_t  xyz[3];                         // Local position x/y/z in units of 10^-scale
  int16_t  rot[3];                         // Local orientation alpha/beta/gamma in centidegrees
  uint8_t  scale;                          // Decimal shift for xyz (0..6)
  uint8_t  flags;                          // bit0 = pose set
} tLocationPose;
tLocationPose Location;
bool location_loaded = false;              // Do not save before load (SettingsDelta() saves before FUNC_INIT)

#ifdef ESP8266
// Packed pose in 96 bits: scale code 3 bit (0 = no pose), x/y/z 21 bit signed, alpha/beta/gamma 10 bit signed
#define LOCATION_PACK_XYZ_BITS      21
#define LOCATION_PACK_ROT_BITS      10
#define LOCATION_PACK_XYZ_MAX       ((1L << (LOCATION_PACK_XYZ_BITS -1)) -1)  // 1048575

void LocationBitsPut(uint8_t* buf, uint32_t* pos, uint32_t value, uint32_t bits) {
  for (uint32_t i = 0; i < bits; i++) {
    if (value & (1UL << i)) { buf[(*pos + i) >> 3] |= 1 << ((*pos + i) & 7); }
  }
  *pos += bits;
}

int32_t LocationBitsGet(const uint8_t* buf, uint32_t* pos, uint32_t bits) {
  uint32_t value = 0;
  for (uint32_t i = 0; i < bits; i++) {
    if (buf[(*pos + i) >> 3] & (1 << ((*pos + i) & 7))) { value |= 1UL << i; }
  }
  *pos += bits;
  if (value & (1UL << (bits -1))) { value |= ~((1UL << bits) -1); }  // Sign extend
  return (int32_t)value;
}

void LocationPack(void) {
  uint8_t* buf = Settings->location_packed;
  memset(buf, 0, sizeof(Settings->location_packed));
  if (!(Location.flags & LOCATION_FLAG_SET)) { return; }
  uint32_t pos = 0;
  LocationBitsPut(buf, &pos, Location.scale +1, 3);
  bool clamped = false;
  for (uint32_t i = 0; i < 3; i++) {
    int32_t value = Location.xyz[i];
    if (value > LOCATION_PACK_XYZ_MAX) { value = LOCATION_PACK_XYZ_MAX; clamped = true; }
    if (value < -LOCATION_PACK_XYZ_MAX) { value = -LOCATION_PACK_XYZ_MAX; clamped = true; }
    LocationBitsPut(buf, &pos, (uint32_t)value, LOCATION_PACK_XYZ_BITS);
  }
  for (uint32_t i = 0; i < 3; i++) {
    int32_t index = lroundf((float)Location.rot[i] * 1024.0f / 36000.0f);  // -512..512
    if (index > 511) { index -= 1024; }    // +180 equals -180
    LocationBitsPut(buf, &pos, (uint32_t)index, LOCATION_PACK_ROT_BITS);
  }
  if (clamped) {
    AddLog(LOG_LEVEL_INFO, PSTR("LOC: Position limited to +/-%d units without file system"), LOCATION_PACK_XYZ_MAX);
  }
}

bool LocationUnpack(void) {
  const uint8_t* buf = Settings->location_packed;
  uint32_t pos = 0;
  uint32_t code = LocationBitsGet(buf, &pos, 3) & 0x07;
  if (!code || (code -1 > LOCATION_SCALE_MAX)) { return false; }
  Location.scale = code -1;
  Location.flags = LOCATION_FLAG_SET;
  for (uint32_t i = 0; i < 3; i++) {
    Location.xyz[i] = LocationBitsGet(buf, &pos, LOCATION_PACK_XYZ_BITS);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Location.rot[i] = lroundf((float)LocationBitsGet(buf, &pos, LOCATION_PACK_ROT_BITS) * 36000.0f / 1024.0f);
  }
  return true;
}
#endif  // ESP8266

#ifdef USE_UFILESYS
#define XDRV_95_KEY                 "drvset95"

uint32_t location_crc32 = 0;               // Crc of pose saved to file

bool LocationLoadData(void) {
  char key[] = XDRV_95_KEY;
  String json = UfsJsonSettingsRead(key);
  if (json.length() == 0) { return false; }

  // {"Scale":3,"Flags":1,"Pos":[1250,3400,900],"Rot":[1235,-50,-17000]}
  JsonParser parser((char*)json.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  uint32_t scale = root.getUInt(PSTR("Scale"), LOCATION_SCALE_DEFAULT);
  Location.scale = (scale > LOCATION_SCALE_MAX) ? LOCATION_SCALE_DEFAULT : scale;
  Location.flags = root.getUInt(PSTR("Flags"), 0);
  JsonParserArray arr = root[PSTR("Pos")];
  if (arr) {
    for (uint32_t i = 0; i < 3; i++) {
      if (arr[i]) { Location.xyz[i] = arr[i].getInt(); }
    }
  }
  arr = root[PSTR("Rot")];
  if (arr) {
    for (uint32_t i = 0; i < 3; i++) {
      if (arr[i]) { Location.rot[i] = arr[i].getInt(); }
    }
  }
  return true;
}

bool LocationSaveData(void) {
  char json[128];
  snprintf_P(json, sizeof(json), PSTR("{\"" XDRV_95_KEY "\":{\"Scale\":%u,\"Flags\":%u,\"Pos\":[%d,%d,%d],\"Rot\":[%d,%d,%d]}}"),
    Location.scale, Location.flags, Location.xyz[0], Location.xyz[1], Location.xyz[2],
    Location.rot[0], Location.rot[1], Location.rot[2]);
  return UfsJsonSettingsWrite(json);
}
#endif  // USE_UFILESYS

bool LocationUseFile(void) {
  // A mounted file system is preferred. Without it ESP8266 uses the packed settings area
#ifdef USE_UFILESYS
  return (ffs_type != 0);
#else
  return false;
#endif  // USE_UFILESYS
}

void LocationSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart, and after pose changes
  if (!location_loaded) { return; }        // Pose not loaded yet: keep stored data untouched
#ifdef USE_UFILESYS
  if (LocationUseFile()) {
    uint32_t crc32 = GetCfgCrc32((uint8_t*)&Location, sizeof(tLocationPose));
    if (crc32 != location_crc32) {
      bool saved;
      if (!(Location.flags & LOCATION_FLAG_SET) && (LOCATION_SCALE_DEFAULT == Location.scale)) {
        char key[] = XDRV_95_KEY;
        UfsJsonSettingsDelete(key);        // No pose: no key, so a packed pose can still be migrated
        saved = true;
      } else {
        saved = LocationSaveData();
      }
      if (saved) {
        location_crc32 = crc32;
#ifdef ESP8266
        memset(Settings->location_packed, 0, sizeof(Settings->location_packed));  // File is the only copy
#endif  // ESP8266
      }
    }
    return;
  }
#endif  // USE_UFILESYS
#ifdef ESP8266
  // No file system
  uint8_t scale = Location.scale;
  LocationPack();                          // Saved with Settings
  if (!LocationUnpack()) {                 // Show what survives a restart
    Location.scale = scale;                // No pose: keep scale until restart
  }
#endif  // ESP8266
}

void LocationSettingsLoad(bool erase) {
  // Called from FUNC_INIT (erase = 0) once at restart
  // Called from FUNC_RESET_SETTINGS (erase = 1) after command reset 4, 5, or 6
  memset(&Location, 0, sizeof(tLocationPose));
  Location.scale = LOCATION_SCALE_DEFAULT;
  location_loaded = true;
#ifdef ESP8266
  if (erase) {
    memset(Settings->location_packed, 0, sizeof(Settings->location_packed));
  } else {
    LocationUnpack();                      // Settings area value is default or the only storage
  }
#endif  // ESP8266

#ifdef USE_UFILESYS
  char key[] = XDRV_95_KEY;
  if (erase) {
    UfsJsonSettingsDelete(key);            // Use defaults
    location_crc32 = GetCfgCrc32((uint8_t*)&Location, sizeof(tLocationPose));  // Nothing to save
  }
  else if (!LocationUseFile()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Location in settings as no file system"));
  }
  else if (LocationLoadData()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Location loaded from file"));
    location_crc32 = GetCfgCrc32((uint8_t*)&Location, sizeof(tLocationPose));
#ifdef ESP8266
    memset(Settings->location_packed, 0, sizeof(Settings->location_packed));  // Drop stale packed copy
#endif  // ESP8266
  }
  else {
    // Key not found. A pose from the settings area is saved to file on next save
    location_crc32 = (Location.flags & LOCATION_FLAG_SET) ? 0 : GetCfgCrc32((uint8_t*)&Location, sizeof(tLocationPose));
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CFG: Location use %s as key not found"),
      (Location.flags & LOCATION_FLAG_SET) ? PSTR("settings") : PSTR("defaults"));
  }
#endif  // USE_UFILESYS
}

/*********************************************************************************************\
 * Helpers
\*********************************************************************************************/

int32_t LocationPow10(uint32_t exp) {
  int32_t result = 1;
  while (exp--) { result *= 10; }
  return result;
}

uint8_t LocationScale(void) {
  // Guard against corrupt data
  return (Location.scale > LOCATION_SCALE_MAX) ? LOCATION_SCALE_DEFAULT : Location.scale;
}

int32_t LocationToFixed(float value) {
  // Convert float (in base unit) to fixed point using current scale, with saturation
  float scaled = value * (float)LocationPow10(LocationScale());
  if (scaled >= 2147483520.0f) { return INT32_MAX; }
  if (scaled <= -2147483520.0f) { return INT32_MIN; }
  return (int32_t)lroundf(scaled);
}

int16_t LocationToCentiDegree(float degrees) {
  // Normalize to -180.00 .. +180.00 degrees and store in centidegrees
  degrees = fmodf(degrees, 360.0f);
  if (degrees > 180.0f) { degrees -= 360.0f; }
  if (degrees < -180.0f) { degrees += 360.0f; }
  return (int16_t)lroundf(degrees * 100.0f);
}

void LocationSetRotation(const char* a, const char* b, const char* c) {
  Location.rot[0] = LocationToCentiDegree(CharToFloat(a));
  Location.rot[1] = LocationToCentiDegree(CharToFloat(b));
  Location.rot[2] = LocationToCentiDegree(CharToFloat(c));
  Location.flags |= LOCATION_FLAG_SET;
}

bool LocationIsSet(void) {
  return (Location.flags & LOCATION_FLAG_SET);
}

// Print fixed point value with decimals without float rounding loss
void LocationAppendFixed(const char* key, int32_t value, uint32_t decimals) {
  if (0 == decimals) {
    ResponseAppend_P(PSTR(",\"%s\":%d"), key, value);
    return;
  }
  uint32_t div = LocationPow10(decimals);
  uint32_t abs_value = (value < 0) ? (uint32_t)(-(int64_t)value) : (uint32_t)value;
  char number[16];
  snprintf_P(number, sizeof(number), PSTR("%s%u.%0*u"), (value < 0) ? "-" : "",
    abs_value / div, (int)decimals, abs_value % div);
  ResponseAppend_P(PSTR(",\"%s\":%s"), key, number);
}
#endif  // LOCATION_HAS_POSE

// Append "Location":{...} without leading comma
void LocationResponseAppend(void) {
  char lat[16];
  char lon[16];
  dtostrfd((double)Settings->latitude / 1000000, 6, lat);
  dtostrfd((double)Settings->longitude / 1000000, 6, lon);
  ResponseAppend_P(PSTR("\"" D_JSON_LOCATION "\":{\"Lat\":%s,\"Lon\":%s,\"Alt\":%d"),
    lat, lon, Settings->altitude);
#ifdef LOCATION_HAS_POSE
  if (LocationIsSet()) {
    uint32_t scale = LocationScale();
    LocationAppendFixed("X", Location.xyz[0], scale);
    LocationAppendFixed("Y", Location.xyz[1], scale);
    LocationAppendFixed("Z", Location.xyz[2], scale);
    LocationAppendFixed("Alpha", Location.rot[0], 2);
    LocationAppendFixed("Beta", Location.rot[1], 2);
    LocationAppendFixed("Gamma", Location.rot[2], 2);
    ResponseAppend_P(PSTR(",\"Scale\":%d"), scale);
  }
#endif  // LOCATION_HAS_POSE
  ResponseAppend_P(PSTR("}"));
}

void LocationResponse(void) {
  Response_P(PSTR("{"));
  LocationResponseAppend();
  ResponseJsonEnd();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndLongitude(void) {
  if (XdrvMailbox.data_len) {
    Settings->longitude = (int)(CharToFloat(XdrvMailbox.data) *1000000);
  }
  ResponseCmndFloat((float)(Settings->longitude) /1000000, 6);
}

void CmndLatitude(void) {
  if (XdrvMailbox.data_len) {
    Settings->latitude = (int)(CharToFloat(XdrvMailbox.data) *1000000);
  }
  ResponseCmndFloat((float)(Settings->latitude) /1000000, 6);
}

void CmndLocation(void) {
  // Location                 - Show
  // Location x,y,z           - Set local position
  // Location x,y,z,a,b,c     - Set local position and orientation
#ifdef LOCATION_HAS_POSE
  uint32_t argc = ArgC();
  if (argc) {
    if ((3 != argc) && (6 != argc)) {
      ResponseCmndChar_P(PSTR("Usage: Location x,y,z[,alpha,beta,gamma]"));
      return;
    }
    char argument[XdrvMailbox.data_len +1];
    for (uint32_t i = 0; i < 3; i++) {
      Location.xyz[i] = LocationToFixed(CharToFloat(ArgV(argument, i +1)));
    }
    Location.flags |= LOCATION_FLAG_SET;
    if (6 == argc) {
      char arg_b[XdrvMailbox.data_len +1];
      char arg_c[XdrvMailbox.data_len +1];
      LocationSetRotation(ArgV(argument, 4), ArgV(arg_b, 5), ArgV(arg_c, 6));
    }
    LocationSettingsSave();
  }
#endif  // LOCATION_HAS_POSE
  LocationResponse();
}

#ifdef LOCATION_HAS_POSE
void CmndLocationRot(void) {
  // LocationRot alpha,beta,gamma  - Orientation in degrees
  if (ArgC()) {
    if (3 != ArgC()) {
      ResponseCmndChar_P(PSTR("Usage: LocationRot alpha,beta,gamma"));
      return;
    }
    char arg_a[XdrvMailbox.data_len +1];
    char arg_b[XdrvMailbox.data_len +1];
    char arg_c[XdrvMailbox.data_len +1];
    LocationSetRotation(ArgV(arg_a, 1), ArgV(arg_b, 2), ArgV(arg_c, 3));
    LocationSettingsSave();
  }
  LocationResponse();
}

void CmndLocationScale(void) {
  // LocationScale 0..6  - Rescale local position keeping the represented value
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= LOCATION_SCALE_MAX)) {
    int32_t old_scale = LocationScale();
    int32_t new_scale = XdrvMailbox.payload;
    for (uint32_t i = 0; i < 3; i++) {
      int64_t value = Location.xyz[i];
      if (new_scale > old_scale) {
        value *= LocationPow10(new_scale - old_scale);
      } else if (new_scale < old_scale) {
        int32_t div = LocationPow10(old_scale - new_scale);
        value = (value >= 0) ? (value + div / 2) / div : (value - div / 2) / div;
      }
      if (value > INT32_MAX) { value = INT32_MAX; }
      if (value < INT32_MIN) { value = INT32_MIN; }
      Location.xyz[i] = (int32_t)value;
    }
    Location.scale = new_scale;
    LocationSettingsSave();
  }
  ResponseCmndNumber(LocationScale());
}

void CmndLocationClear(void) {
  memset(Location.xyz, 0, sizeof(Location.xyz));
  memset(Location.rot, 0, sizeof(Location.rot));
  Location.flags = 0;
  LocationSettingsSave();
  LocationResponse();
}
#endif  // LOCATION_HAS_POSE

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv95(uint32_t function) {
  bool result = false;

  switch (function) {
#ifdef LOCATION_HAS_POSE
    case FUNC_INIT:
      LocationSettingsLoad(0);
      break;
    case FUNC_RESET_SETTINGS:
      LocationSettingsLoad(1);
      break;
    case FUNC_SAVE_SETTINGS:
      LocationSettingsSave();
      break;
#endif  // LOCATION_HAS_POSE
    case FUNC_COMMAND:
      result = DecodeCommand(kLocationCommands, LocationCommand);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_LOCATION
