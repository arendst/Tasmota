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
 * ZCL Command Structures
\*********************************************************************************************/

typedef struct Z_CommandConverter {
  uint16_t     tasmota_cmd_offset;
  uint16_t     cluster;
  uint8_t      cmd;         // normally 8 bits, 0xFF means it's a parameter
  uint8_t      direction;   // direction of the command. 0x01 client->server, 0x02 server->client, 0x03 both, 0x80 requires custom decoding
  uint16_t     param_offset;
} Z_CommandConverter;

typedef struct Z_XYZ_Var {    // Holds values for vairables X, Y and Z
  uint32_t    x = 0;
  uint32_t    y = 0;
  uint32_t    z = 0;
  uint8_t     x_type = 0;     // 0 = no value, 1 = 1 bytes, 2 = 2 bytes
  uint8_t     y_type = 0;
  uint8_t     z_type = 0;
} Z_XYZ_Var;

// Cluster specific commands
// Note: the table is both for sending commands, but also displaying received commands
// - tasmota_cmd: the human-readable name of the command as entered or displayed, use '|' to split into multiple commands when displayed
// - cluster: cluster number of the command
// - cmd: the command number, of 0xFF if it's actually a variable to be assigned from 'xx'
// - direction: the direction of the command (bit field). 0x01=from client to server (coord to device), 0x02= from server to client (response), 0x80=needs specific decoding
// - param: the paylod template, x/y/z are substituted with arguments, little endian. For command display, payload must match until x/y/z character or until the end of the paylod. '_' means custom converter.
const Z_CommandConverter Z_Commands[] PROGMEM = {
  // Identify cluster
  { Z_(Identify),       0x0003, 0x00, 0x01,   Z_(xxxx) },         // Identify device, time in seconds
  { Z_(IdentifyQuery),  0x0003, 0x01, 0x01,   Z_() },             // Identify Query (no param)
  // Group adress commands
  { Z_(AddGroup),       0x0004, 0x00, 0x01,   Z_(xxxx00) },       // Add group id, group name is not supported
  { Z_(ViewGroup),      0x0004, 0x01, 0x01,   Z_(xxxx) },         // Ask for the group name
  { Z_(GetGroup),       0x0004, 0x02, 0x01,   Z_(01xxxx) },       // Get one group membership
  { Z_(GetAllGroups),   0x0004, 0x02, 0x01,   Z_(00) },           // Get all groups membership
  { Z_(RemoveGroup),    0x0004, 0x03, 0x01,   Z_(xxxx) },         // Remove one group
  { Z_(RemoveAllGroups),0x0004, 0x04, 0x01,   Z_() },             // Remove all groups
  // Scenes
  //{ "AddScene",       0x0005, 0x00, 0x01,   "xxxxyy0100" },
  { Z_(ViewScene),      0x0005, 0x01, 0x01,   Z_(xxxxyy) },
  { Z_(RemoveScene),    0x0005, 0x02, 0x01,   Z_(xxxxyy) },
  { Z_(RemoveAllScenes),0x0005, 0x03, 0x01,   Z_(xxxx) },
  { Z_(RecallScene),    0x0005, 0x05, 0x01,   Z_(xxxxyy) },
  { Z_(GetSceneMembership),0x0005, 0x06, 0x01,   Z_(xxxx) },
  // Light & Shutter commands
  { Z_(PowerOffEffect), 0x0006, 0x40, 0x81,   Z_(xxyy) },         // Power Off With Effect
  { Z_(PowerOnRecall),  0x0006, 0x41, 0x81,   Z_() },             // Power On With Recall Global Scene
  { Z_(PowerOnTimer),   0x0006, 0x42, 0x81,   Z_(xxyyyyzzzz) },   // Power On with Timed Off
  { Z_(LidlPower),      0x0006, 0xFD, 0x01,   Z_(xx) },           // Lidl specific encoding
  { Z_(Power),          0x0006, 0xFF, 0x01,   Z_() },             // 0=Off, 1=On, 2=Toggle
  { Z_(Dimmer),         0x0008, 0x04, 0x01,   Z_(xx0A00) },       // Move to Level with On/Off, xx=0..254 (255 is invalid)
  { Z_(DimmerUp),       0x0008, 0x06, 0x01,   Z_(00190200) },       // Step up by 10%, 0.2 secs
  { Z_(DimmerDown),     0x0008, 0x06, 0x01,   Z_(01190200) },       // Step down by 10%, 0.2 secs
  { Z_(DimmerStop),     0x0008, 0x03, 0x01,   Z_() },             // Stop any Dimmer animation
  { Z_(ResetAlarm),     0x0009, 0x00, 0x01,   Z_(xxyyyy) },       // Reset alarm (alarm code + cluster identifier)
  { Z_(ResetAllAlarms), 0x0009, 0x01, 0x01,   Z_() },             // Reset all alarms
  { Z_(Hue),            0x0300, 0x00, 0x01,   Z_(xx000A00) },     // Move to Hue, shortest time, 1s
  { Z_(Sat),            0x0300, 0x03, 0x01,   Z_(xx0A00) },       // Move to Sat
  { Z_(HueSat),         0x0300, 0x06, 0x01,   Z_(xxyy0A00) },     // Hue, Sat
  { Z_(Color),          0x0300, 0x07, 0x01,   Z_(xxxxyyyy0A00) }, // x, y (uint16)
  { Z_(CT),             0x0300, 0x0A, 0x01,   Z_(xxxx0A00) },     // Color Temperature Mireds (uint16)
  { Z_(RGB),            0x0300, 0xF0, 0x81,   Z_(_) },            // synthetic commands converting RGB to XY
  { Z_(ShutterOpen),    0x0102, 0x00, 0x01,   Z_() },
  { Z_(ShutterClose),   0x0102, 0x01, 0x01,   Z_() },
  { Z_(ShutterStop),    0x0102, 0x02, 0x01,   Z_() },
  { Z_(ShutterLift),    0x0102, 0x05, 0x01,   Z_(xx) },            // Lift percentage, 0%=open, 100%=closed
  { Z_(ShutterTilt),    0x0102, 0x08, 0x01,   Z_(xx) },            // Tilt percentage
  { Z_(Shutter),        0x0102, 0xFF, 0x01,   Z_() },
  // Legrand - Manuf 1021
  { Z_(LegrandMode),    0xFC40, 0x00, 0x01,   Z_(xx) },
  // Tuya Generix
  { Z_(TuyaQuery),      0xEF00, 0x03, 0x01,   Z_()},               // Generic Tuya command to ask for reporting of all attributes
  { Z_(TuyaMCUVersion), 0xEF00, 0x10, 0x01,   Z_(0101)},           // Generic Tuya command to ask MCU version (includes a dummy seq number for now)
  // Blitzwolf PIR
  { Z_(Occupancy),      0xEF00, 0x01, 0x82,   Z_()},               // Specific decoder for Blitzwolf PIR, empty name means special treatment
  // Decoders only - normally not used to send, and names may be masked by previous definitions
  { Z_(Dimmer),         0x0008, 0x00, 0x01,   Z_(xx) },
  { Z_(DimmerMove),     0x0008, 0x01, 0x01,   Z_(xx0A) },
  { Z_(DimmerStepUp),   0x0008, 0x02, 0x01,   Z_(00xx0A00) },
  { Z_(DimmerStepDown), 0x0008, 0x02, 0x01,   Z_(01xx0A00) },
  { Z_(DimmerStep),     0x0008, 0x02, 0x01,   Z_(xx190A00) },
  { Z_(DimmerMove),     0x0008, 0x05, 0x01,   Z_(xx0A) },
  { Z_(DimmerUp),       0x0008, 0x06, 0x01,   Z_(00) },
  { Z_(DimmerDown),     0x0008, 0x06, 0x01,   Z_(01) },
  { Z_(DimmerStop),     0x0008, 0x07, 0x01,   Z_() },
  { Z_(HueMove),        0x0300, 0x01, 0x01,   Z_(xx19) },
  { Z_(HueStepUp),      0x0300, 0x02, 0x01,   Z_(01xx0A00) },
  { Z_(HueStepDown),    0x0300, 0x02, 0x01,   Z_(03xx0A00) },
  { Z_(HueStep),        0x0300, 0x02, 0x01,   Z_(xx190A00) },
  { Z_(SatMove),        0x0300, 0x04, 0x01,   Z_(xx19) },
  { Z_(SatStep),        0x0300, 0x05, 0x01,   Z_(xx190A) },
  { Z_(ColorMove),      0x0300, 0x08, 0x01,   Z_(xxxxyyyy) },
  { Z_(ColorStep),      0x0300, 0x09, 0x01,   Z_(xxxxyyyy0A00) },
  { Z_(ColorTempMoveUp),  0x0300, 0x4B, 0x01, Z_(01xxxx000000000000) },
  { Z_(ColorTempMoveDown),0x0300, 0x4B, 0x01, Z_(03xxxx000000000000) },
  { Z_(ColorTempMoveStop),0x0300, 0x4B, 0x01, Z_(00xxxx000000000000) },
  { Z_(ColorTempMove),  0x0300, 0x4B, 0x01,   Z_(xxyyyy000000000000) },
  { Z_(ColorTempStepUp),  0x0300, 0x4C, 0x01, Z_(01xxxx0A0000000000) },
  { Z_(ColorTempStepDown),0x0300, 0x4C, 0x01, Z_(03xxxx0A0000000000) },
  { Z_(ColorTempStep),  0x0300, 0x4C, 0x01,   Z_(xxyyyy0A0000000000) },     //xx = 0x01 up, 0x03 down, yyyy = step
  // Tradfri
  { Z_(ArrowClick),     0x0005, 0x07, 0x01,   Z_(xx) },         // xx == 0x01 = left, 0x00 = right
  { Z_(ArrowHold),      0x0005, 0x08, 0x01,   Z_(xx) },         // xx == 0x01 = left, 0x00 = right
  { Z_(ArrowRelease),   0x0005, 0x09, 0x01,   Z_() },
  // Response for Indetify cluster
  { Z_(IdentifyQuery),  0x0003, 0x00, 0x02,   Z_(xxxx) },             // timeout in seconds
  // IAS - Intruder Alarm System + leak/fire detection
  { Z_(ZoneStatusChange),0x0500, 0x00, 0x82,  Z_(xxxxyyzz) },   // xxxx = zone status, yy = extended status, zz = zone id, Delay is ignored
  // responses for Group cluster commands
  { Z_(AddGroup),       0x0004, 0x00, 0x82,   Z_(xxyyyy) },       // xx = status, yy = group id
  { Z_(ViewGroup),      0x0004, 0x01, 0x82,   Z_(xxyyyy) },       // xx = status, yy = group id, name ignored
  { Z_(GetGroup),       0x0004, 0x02, 0x82,   Z_(xxyyzzzz) },     // xx = capacity, yy = count, zzzz = first group id, following groups ignored
  { Z_(RemoveGroup),    0x0004, 0x03, 0x82,   Z_(xxyyyy) },       // xx = status, yy = group id
  // responses for Scene cluster commands
  { Z_(AddScene),       0x0005, 0x00, 0x82,   Z_(xxyyyyzz) },     // xx = status, yyyy = group id, zz = scene id
  { Z_(ViewScene),      0x0005, 0x01, 0x82,   Z_(xxyyyyzz) },     // xx = status, yyyy = group id, zz = scene id
  { Z_(RemoveScene),    0x0005, 0x02, 0x82,   Z_(xxyyyyzz) },     // xx = status, yyyy = group id, zz = scene id
  { Z_(RemoveAllScenes),0x0005, 0x03, 0x82,   Z_(xxyyyy) },     // xx = status, yyyy = group id
  { Z_(StoreScene),     0x0005, 0x04, 0x82,   Z_(xxyyyyzz) },     // xx = status, yyyy = group id, zz = scene id
  { Z_(GetSceneMembership),0x0005, 0x06, 0x82,Z_(xxyyzzzz) },     // specific
  // Tuya - Moes specific
  { Z_(),               0xEF00, 0xFF, 0x83,   Z_() },             // capture any command in 0xEF00 cluster
  // Terncy specific
  { Z_(),               0xFCCC, 0x00, 0x82,   Z_(xxyy) },         // Terncy button (multi-)press
  // Green Power - for display only
  { Z_(GPIdentify),     0xF021, 0x00, 0x01,   Z_() },
  { Z_(GPScene0),       0xF021, 0x10, 0x01,   Z_() },
  { Z_(GPScene1),       0xF021, 0x11, 0x01,   Z_() },
  { Z_(GPScene2),       0xF021, 0x12, 0x01,   Z_() },
  { Z_(GPScene3),       0xF021, 0x13, 0x01,   Z_() },
  { Z_(GPScene4),       0xF021, 0x14, 0x01,   Z_() },
  { Z_(GPScene5),       0xF021, 0x15, 0x01,   Z_() },
  { Z_(GPScene6),       0xF021, 0x16, 0x01,   Z_() },
  { Z_(GPScene7),       0xF021, 0x17, 0x01,   Z_() },
  { Z_(GPScene8),       0xF021, 0x18, 0x01,   Z_() },
  { Z_(GPScene9),       0xF021, 0x19, 0x01,   Z_() },
  { Z_(GPScene10),      0xF021, 0x1A, 0x01,   Z_() },
  { Z_(GPScene11),      0xF021, 0x1B, 0x01,   Z_() },
  { Z_(GPScene12),      0xF021, 0x1C, 0x01,   Z_() },
  { Z_(GPScene13),      0xF021, 0x1D, 0x01,   Z_() },
  { Z_(GPScene14),      0xF021, 0x1E, 0x01,   Z_() },
  { Z_(GPScene15),      0xF021, 0x1F, 0x01,   Z_() },
  { Z_(GPOff),          0xF021, 0x20, 0x01,   Z_() },
  { Z_(GPOn),           0xF021, 0x21, 0x01,   Z_() },
  { Z_(GPToggle),       0xF021, 0x22, 0x01,   Z_() },
  { Z_(GPRelease),      0xF021, 0x23, 0x01,   Z_() },
  { Z_(GPMoveUp),       0xF021, 0x30, 0x01,   Z_(xx) },
  { Z_(GPMoveDown),     0xF021, 0x31, 0x01,   Z_(xx) },
  { Z_(GPStepUp),       0xF021, 0x32, 0x01,   Z_(xx) },
  { Z_(GPStepDown),     0xF021, 0x33, 0x01,   Z_(xx) },
  { Z_(GPLevelStop),    0xF021, 0x34, 0x01,   Z_() },
  { Z_(GPMoveUpOnOff),  0xF021, 0x35, 0x01,   Z_(xx) },
  { Z_(GPMoveDownOnOff),0xF021, 0x36, 0x01,   Z_(xx) },
  { Z_(GPStepUpOnOff),  0xF021, 0x37, 0x01,   Z_(xx) },
  { Z_(GPStepDownOnOff),0xF021, 0x38, 0x01,   Z_(xx) },
  { Z_(GPHueStop),      0xF021, 0x40, 0x01,   Z_() },
  { Z_(GPMoveHueUp),    0xF021, 0x41, 0x01,   Z_(xx) },
  { Z_(GPMoveHueDown),  0xF021, 0x42, 0x01,   Z_(xx) },
  { Z_(GPStepHueUp),    0xF021, 0x43, 0x01,   Z_(xx) },
  { Z_(GPStepHueDown),  0xF021, 0x44, 0x01,   Z_(xx) },
  { Z_(GPSatStop),      0xF021, 0x45, 0x01,   Z_() },
  { Z_(GPMoveSatUp),    0xF021, 0x46, 0x01,   Z_(xx) },
  { Z_(GPMoveSatDown),  0xF021, 0x47, 0x01,   Z_(xx) },
  { Z_(GPStepSatUp),    0xF021, 0x48, 0x01,   Z_(xx) },
  { Z_(GPStepSatDown),  0xF021, 0x49, 0x01,   Z_(xx) },
  { Z_(GPMoveColor),    0xF021, 0x4A, 0x01,   Z_(xxxxyyyy) },
  { Z_(GPStepColor),    0xF021, 0x4B, 0x01,   Z_(xxxxyyyy) },
  { Z_(GPLockDoor),     0xF021, 0x50, 0x01,   Z_() },
  { Z_(GPUnlockDoor),   0xF021, 0x51, 0x01,   Z_() },
  { Z_(GPPress1of1),    0xF021, 0x60, 0x01,   Z_() },
  { Z_(GPRelease1of1),  0xF021, 0x61, 0x01,   Z_() },
  { Z_(GPPress1of2),    0xF021, 0x62, 0x01,   Z_() },
  { Z_(GPRelease1of2),  0xF021, 0x63, 0x01,   Z_() },
  { Z_(GPPress2of2),    0xF021, 0x64, 0x01,   Z_() },
  { Z_(GPRelease2of2),  0xF021, 0x65, 0x01,   Z_() },
  { Z_(GPShortPress1of1),0xF021, 0x66, 0x01,   Z_() },
  { Z_(GPShortPress1of2),0xF021, 0x67, 0x01,   Z_() },
  { Z_(GPShortPress2of2),0xF021, 0x68, 0x01,   Z_() },
};

/*********************************************************************************************\
 * ZCL Read Light status based on cluster number
\*********************************************************************************************/
#define ZLE(x) ((x) & 0xFF), ((x) >> 8)     // Little Endian

// Below are the attributes we wand to read from each cluster
const uint8_t CLUSTER_0001[] = { ZLE(0x0020), ZLE(0x0021) };    // BatteryVoltage, BatteryPercentage
const uint8_t CLUSTER_0006[] = { ZLE(0x0000) };    // Power
const uint8_t CLUSTER_0008[] = { ZLE(0x0000) };    // CurrentLevel
const uint8_t CLUSTER_0009[] = { ZLE(0x0000) };    // AlarmCount
const uint8_t CLUSTER_0300[] = { ZLE(0x0000), ZLE(0x0001), ZLE(0x0003), ZLE(0x0004), ZLE(0x0007), ZLE(0x0008) };    // Hue, Sat, X, Y, CT, ColorMode

// This callback is registered after a cluster specific command and sends a read command for the same cluster
void Z_ReadAttrCallback(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  size_t         attrs_len = 0;
  const uint8_t* attrs = nullptr;

  switch (cluster) {
    case 0x0001:
      attrs = CLUSTER_0001;
      attrs_len = sizeof(CLUSTER_0001);
      break;
    case 0x0006:                              // for On/Off
      attrs = CLUSTER_0006;
      attrs_len = sizeof(CLUSTER_0006);
      break;
    case 0x0008:                              // for Dimmer
      attrs = CLUSTER_0008;
      attrs_len = sizeof(CLUSTER_0008);
      break;
    case 0x0009:                              // for Alarms
      attrs = CLUSTER_0009;
      attrs_len = sizeof(CLUSTER_0009);
      break;
    case 0x0300:                              // for Lights
      attrs = CLUSTER_0300;
      attrs_len = sizeof(CLUSTER_0300);
      break;
  }
  if (attrs) {
    if (groupaddr) {
      shortaddr = BAD_SHORTADDR;   // if group address, don't send to device
    }

    ZCLFrame zcl(attrs_len);   // message is `attrs_len` bytes
    zcl.shortaddr = shortaddr;
    zcl.groupaddr = groupaddr;
    zcl.cluster = cluster;
    zcl.dstendpoint = endpoint;
    zcl.cmd = ZCL_READ_ATTRIBUTES;
    zcl.clusterSpecific = false;
    zcl.needResponse = true;
    zcl.direct = false;   // discover route
    zcl.payload.addBuffer(attrs, attrs_len);
    zigbeeZCLSendCmd(zcl);
  }
}


// This callback is registered after a an attribute read command was made to a light, and fires if we don't get any response after 1000 ms
void Z_Unreachable(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  if (BAD_SHORTADDR != shortaddr) {
    zigbee_devices.getShortAddr(shortaddr).setReachable(false);     // mark device as reachable
    Z_attribute_list attr_list;
    attr_list.addAttributePMEM(PSTR("Reachable")).setBool(false);        // "Reachable":false
    zigbee_devices.jsonPublishNow(shortaddr, attr_list);
  }
}

// returns true if char is 'x', 'y' or 'z'
inline bool isXYZ(char c) {
  return (c >= 'x') && (c <= 'z');
}

// returns the Hex value of a digit [0-9A-Fa-f]
// return: 0x00-0x0F
// or -1 if cannot be parsed
inline int8_t hexValue(char c) {
  if ((c >= '0') && (c <= '9')) {
    return c - '0';
  }
  if ((c >= 'A') && (c <= 'F')) {
    return 10 + c - 'A';
  }
  if ((c >= 'a') && (c <= 'f')) {
    return 10 + c - 'a';
  }
  return -1;
}

// Parse a Big Endian suite of max_len digits, or stops when a non-hex digit is found
uint32_t parseHex_P(const char **data, size_t max_len = 8) {
  uint32_t ret = 0;
  for (uint32_t i = 0; i < max_len; i++) {
    int8_t v = hexValue(pgm_read_byte(*data));
    if (v < 0) { break; }     // non hex digit, we stop parsing
    ret = (ret << 4) | v;
    *data += 1;
  }
  return ret;
}

// Parse a model like "xxyy00"
// and fill x, y and z values
// Little Endian encoding
// On exit, xyz is updated, and x_type, y_type, z_type contain the number of bytes read for each
void parseXYZ(const char *model, const SBuffer &payload, struct Z_XYZ_Var *xyz) {
  const char *p = model;    // pointer to the model character
  uint32_t   v = 0;         // index in the payload bytes buffer
  char c = pgm_read_byte(p);  // cur char
  while (c) {
    char c1 = pgm_read_byte(p+1);   // next char
    if (!c1) { break; }   // unexpected end of model
    if (isXYZ(c) && (c == c1) && (v < payload.len())) {    // if char is [x-z] and followed by same char
      uint8_t val = payload.get8(v);
      switch (c) {
        case 'x':
          xyz->x = xyz->x | (val << (xyz->x_type * 8));
          xyz->x_type++;
          break;
        case 'y':
          xyz->y = xyz->y | (val << (xyz->y_type * 8));
          xyz->y_type++;
          break;
        case 'z':
          xyz->z = xyz->z | (val << (xyz->z_type * 8));
          xyz->z_type++;
          break;
      }
    }
    p += 2;
    v++;
    c = pgm_read_byte(p);
  }
}

// Parse a cluster specific command, and try to convert into human readable
// Includes specific handling for Tuya attributes
void convertClusterSpecific(class Z_attribute_list &attr_list, uint16_t cluster, uint8_t cmd, bool direction, uint16_t shortaddr, uint8_t srcendpoint, const SBuffer &payload) {
  const char * command_name = nullptr;
  uint8_t conv_direction;
  Z_XYZ_Var xyz;

  // always report attribute in raw format
  // Format: "0001!06": "00" = "<cluster>!<cmd>": "<payload>" for commands to devices
  // Format: "0004<00": "00" = "<cluster><<cmd>": "<payload>" for commands to devices
  // char attrid_str[12];
  // snprintf_P(attrid_str, sizeof(attrid_str), PSTR("%04X%c%02X"), cluster, direction ? '?' : '!', cmd);
  // Z_attribute & attr_raw = attr_list.addAttribute(attrid_str);
  Z_attribute & attr_raw = attr_list.addAttributeCmd(cluster, cmd, direction, false /* cluster specific */);
  attr_raw.setBuf(payload, 0, payload.len());

  // Take a shotcut in case of Tuya attribute which follow a different scheme
  if (cluster == 0xEF00) {
    // Tuya Cmd
    if (convertTuyaSpecificCluster(attr_list, cluster, cmd, direction, shortaddr, srcendpoint, payload)) {
      attr_list.removeAttribute(&attr_raw);   // remove raw command
    }
    return;     // abort, normal processing doesn't apply here
  }

  //AddLog(LOG_LEVEL_INFO, PSTR(">>> len = %d - %02X%02X%02X"), payload.len(), payload.get8(0), payload.get8(1), payload.get8(2));
  for (uint32_t i = 0; i < sizeof(Z_Commands) / sizeof(Z_Commands[0]); i++) {
    const Z_CommandConverter *conv = &Z_Commands[i];
    uint16_t conv_cluster = pgm_read_word(&conv->cluster);
    if (conv_cluster == cluster) {
      // cluster match
      uint8_t conv_cmd = pgm_read_byte(&conv->cmd);
      conv_direction = pgm_read_byte(&conv->direction);
      if ((0xFF == conv_cmd) || (cmd == conv_cmd)) {
          // cmd match
        if ((direction && (conv_direction & 0x02)) || (!direction && (conv_direction & 0x01))) {
          // check if we have a match for params too
          // Match if:
          //  - payload exactly matches conv->param (conv->param may be longer)
          //  - payload matches conv->param until 'x', 'y' or 'z'
          const char * p = Z_strings + pgm_read_word(&conv->param_offset);
          //AddLog(LOG_LEVEL_INFO, PSTR(">>>++1 param = %s"), p);
          bool match = true;
          for (uint8_t i = 0; i < payload.len(); i++) {
            const char c1 = pgm_read_byte(p);
            // const char c2 = pgm_read_byte(p+1);
            //AddLog(LOG_LEVEL_INFO, PSTR(">>>++2 c1 = %c, c2 = %c"), c1, c2);
            if ((0x00 == c1) || isXYZ(c1)) {
              break;
            }
            const char * p2 = p;
            uint32_t nextbyte = parseHex_P(&p2, 2);
            //AddLog(LOG_LEVEL_INFO, PSTR(">>>++3 parseHex_P = %02X"), nextbyte);
            if (nextbyte != payload.get8(i)) {
              match = false;
              break;
            }
            p += 2;
          }
          if (match) {
            command_name = Z_strings + pgm_read_word(&conv->tasmota_cmd_offset);
            parseXYZ(Z_strings + pgm_read_word(&conv->param_offset), payload, &xyz);
            if (0xFF == conv_cmd) {
              // shift all values
              xyz.z = xyz.y;
              xyz.z_type = xyz.y_type;
              xyz.y = xyz.x;
              xyz.y_type = xyz.x_type;
              xyz.x = cmd;
              xyz.x_type = 1;   // 1 byte
            }
            break;
          }
        }
      }
    }
  }

  // TODO Berry encode command

  if (command_name) {
    // Now try to transform into a human readable format
    // if (direction & 0x80) then specific transform
    if (conv_direction & 0x80) {
      uint32_t cccc00mm = (cluster << 16) | cmd;    // format = cccc00mm, cccc = cluster, mm = command
      // IAS
      switch (cccc00mm) {
      case 0x05000000:        // "ZoneStatusChange"
        {
          attr_list.addAttribute(command_name, true).setUInt(xyz.x);
          if (0 != xyz.y) {
            attr_list.addAttribute(command_name, PSTR("Ext")).setUInt(xyz.y);
          }
          if ((0 != xyz.z) && (0xFF != xyz.z)) {
            attr_list.addAttribute(command_name, PSTR("Zone")).setUInt(xyz.z);
          }
          // Convert to "Occupancy" or to "Contact" if the device is PIR or Contact sensor
          const Z_Data_Alarm & alarm = (const Z_Data_Alarm&) zigbee_devices.getShortAddr(shortaddr).data.find(Z_Data_Type::Z_Alarm, srcendpoint);
          if (&alarm != nullptr) {
            alarm.convertZoneStatus(attr_list, xyz.x);
          }
        }
        break;
      case 0x00040000:
      case 0x00040001:
      case 0x00040003:      // AddGroupResp or ViewGroupResp (group name ignored) or RemoveGroup
        attr_list.addAttribute(command_name, true).setUInt(xyz.y);
        attr_list.addAttribute(command_name, PSTR("Status")).setUInt(xyz.x);
        attr_list.addAttribute(command_name, PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(xyz.x).c_str());
        break;
      case 0x00040002:      // GetGroupResp
        attr_list.addAttribute(command_name, PSTR("Capacity")).setUInt(xyz.x);
        attr_list.addAttribute(command_name, PSTR("Count")).setUInt(xyz.y);
        {

          JsonGeneratorArray group_list;
          for (uint32_t i = 0; i < xyz.y; i++) {
            group_list.add((uint32_t)payload.get16(2 + 2*i));
          }
          attr_list.addAttribute(command_name, true).setStrRaw(group_list.toString().c_str());
        }
        break;
      case 0x00050000:
      case 0x00050001:      // ViewScene
      case 0x00050002:
      case 0x00050004:      // AddScene or RemoveScene or StoreScene
        attr_list.addAttribute(command_name, PSTR("Status")).setUInt(xyz.x);
        attr_list.addAttribute(command_name, PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(xyz.x).c_str());
        attr_list.addAttributePMEM(PSTR("GroupId")).setUInt(xyz.y);
        attr_list.addAttributePMEM(PSTR("SceneId")).setUInt(xyz.z);
        if (0x00050001 == cccc00mm) {   // ViewScene specific
          attr_list.addAttributePMEM(PSTR("ScenePayload")).setBuf(payload, 4, payload.len()-4); // remove first 4 bytes
        }
        break;
      case 0x00050003:      // RemoveAllScenes
        attr_list.addAttribute(command_name, PSTR("Status")).setUInt(xyz.x);
        attr_list.addAttribute(command_name, PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(xyz.x).c_str());
        attr_list.addAttributePMEM(PSTR("GroupId")).setUInt(xyz.y);
        break;
      case 0x00050006:      // GetSceneMembership
        attr_list.addAttribute(command_name, PSTR("Status")).setUInt(xyz.x);
        attr_list.addAttribute(command_name, PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(xyz.x).c_str());
        attr_list.addAttributePMEM(PSTR("Capacity")).setUInt(xyz.y);
        attr_list.addAttributePMEM(PSTR("GroupId")).setUInt(xyz.z);
        attr_list.addAttributePMEM(PSTR("ScenePayload")).setBuf(payload, 4, payload.len()-4); // remove first 4 bytes
        break;
      case 0x00060040:      // Power Off With Effect
        attr_list.addAttributePMEM(PSTR("Power")).setUInt(0);
        attr_list.addAttributePMEM(PSTR("PowerEffect")).setUInt(xyz.x);
        attr_list.addAttributePMEM(PSTR("PowerEffectVariant")).setUInt(xyz.y);
        break;
      case 0x00060041:      // Power On With Recall Global Scene
        attr_list.addAttributePMEM(PSTR("Power")).setUInt(1);
        attr_list.addAttributePMEM(PSTR("PowerRecallGlobalScene")).setBool(true);
        break;
      case 0x00060042:      // Power On With Timed Off Command
        attr_list.addAttributePMEM(PSTR("Power")).setUInt(1);
        attr_list.addAttributePMEM(PSTR("PowerOnlyWhenOn")).setUInt(xyz.x);
        attr_list.addAttributePMEM(PSTR("PowerOnTime")).setFloat(xyz.y / 10.0f);
        attr_list.addAttributePMEM(PSTR("PowerOffWait")).setFloat(xyz.z / 10.0f);
        break;
      case 0xEF000000 ... 0xEF0000FF: // any Tuya - Moes command
        if (convertTuyaSpecificCluster(attr_list, cluster, cmd, direction, shortaddr, srcendpoint, payload)) {
          attr_list.removeAttribute(&attr_raw);   // remove raw command
        }
        break;
      case 0xFCCC0000:      // Terncy button (multi-)press
        attr_list.addAttributePMEM(PSTR("TerncyPress")).setUInt(xyz.y);
        attr_list.addAttributePMEM(PSTR("TerncyCount")).setUInt(xyz.x);
        break;
      }
    } else {  // general case
      if (0 == xyz.x_type) {
        attr_list.addAttribute(command_name).setBool(true);
      } else if (0 == xyz.y_type) {
        attr_list.addAttribute(command_name).setUInt(xyz.x);
      } else {
        // multiple answers, create an array
        JsonGeneratorArray arr;
        arr.add(xyz.x);
        arr.add(xyz.y);
        if (xyz.z_type) {
          arr.add(xyz.z);
        }
        attr_list.addAttribute(command_name).setStrRaw(arr.toString().c_str());
      }
    }
  }
}

/*********************************************************************************************\
 * 
 * Tuya Zigbee specific protocol
 * 
\*********************************************************************************************/
// Parse a sinlge attribute value and convert to human readable JSON attribute value
void parseSingleTuyaAttribute(Z_attribute & attr, const SBuffer &buf,
                              uint32_t i, uint32_t len, int32_t attrtype) {

  // fallback - enter a null value
  attr.setNone();   // set to null by default

  // now parse accordingly to attr type
  switch (attrtype) {
    case 0x00:      // RAW octstr
      attr.setBuf(buf, i, len);
      break;
    case 0x01:      // Bool, values 0/1, len = 1
    case 0x04:      // enum 8 bits
      attr.setUInt(buf.get8(i));
      break;
    case 0x02:      // 4 bytes value (signed?)
      attr.setUInt(buf.get32BigEndian(i));
      break;
    case 0x03:      // String (we expect it is not ended with \00)
     {
      char str[len+1];
      strncpy(str, buf.charptr(i), len);
      str[len] = 0x00;
      attr.setStr(str);
     }
      break;
    case 0x05:      // enum in 1/2/4 bytes, Big Endian
      if (1 == len) {
        attr.setUInt(buf.get8(i));
      } else if (2 == len) {
        attr.setUInt(buf.get16BigEndian(i));
      } else if (4 == len) {
        attr.setUInt(buf.get32BigEndian(i));
      }
  }
}

/*********************************************************************************************\
 * 
 * Reply Tuya time
 * 
\*********************************************************************************************/
static void replyTuyaTime( uint16_t cluster, uint16_t shortaddr, uint8_t dstendpoint ) {
  ZCLFrame zcl(10);   // message is `attrs_len` bytes
  zcl.shortaddr = shortaddr;
  zcl.cluster = cluster;
  zcl.dstendpoint = dstendpoint;
  zcl.cmd = 0x24;
  zcl.clusterSpecific = true;
  zcl.needResponse = false; // it is a reply
  zcl.direct = false;   // discover route
  zcl.payload.add16BigEndian(8); // Size
  if (!RtcTime.valid) {
    zcl.payload.add32(0);
    zcl.payload.add32(0);
  } else {
    zcl.payload.add32BigEndian(Rtc.utc_time);
    zcl.payload.add32BigEndian(Rtc.local_time);
  }
  zigbeeZCLSendCmd(zcl);
}

//
// Tuya - MOES specifc cluster 0xEF00
// https://developer.tuya.com/en/docs/iot-device-dev/tuya-zigbee-universal-docking-access-standard?id=K9ik6zvofpzql#subtitle-6-Private%20cluster
//
bool convertTuyaSpecificCluster(class Z_attribute_list &attr_list, uint16_t cluster, uint8_t cmd, bool direction, uint16_t shortaddr, uint8_t srcendpoint, const SBuffer &buf) {

  if ((1 == cmd) || (2 == cmd)) {   // attribute report or attribute response
    // uint16_t seq_number = buf.get16BigEndian(0)
    uint8_t dpid = buf.get8(2);   // dpid from Tuya documentation
    uint8_t attr_type = buf.get8(3);   // data type from Tuya documentation
    uint16_t len = buf.get16BigEndian(4);
    // create a synthetic attribute with id 'dpid'
    Z_attribute & attr = attr_list.addAttribute(cluster, (attr_type << 8) | dpid);
    parseSingleTuyaAttribute(attr, buf, 6, len, attr_type);
    return true;    // true = remove the original Tuya attribute
  }
  if (0x24 == cmd) {
    replyTuyaTime(cluster, shortaddr, srcendpoint);
    return true;
  }
  return false;
}


/*********************************************************************************************\
 * 
 * Find commands
 * 
\*********************************************************************************************/
// Find the command details by command name
// Only take commands outgoing, i.e. direction == 0
// If not found:
//  - returns nullptr
//  - return PROGMEM string
const char * zigbeeFindCommand(const char *command, uint16_t *cluster, uint16_t *cmd) {
  if (nullptr == command) { return nullptr; }
  for (uint32_t i = 0; i < sizeof(Z_Commands) / sizeof(Z_Commands[0]); i++) {
    const Z_CommandConverter *conv = &Z_Commands[i];
    uint8_t conv_direction = pgm_read_byte(&conv->direction);
    uint8_t conv_cmd = pgm_read_byte(&conv->cmd);
    uint16_t conv_cluster = pgm_read_word(&conv->cluster);
    // conv_direction must be client (coord) -> server (device), we can only send commands to end devices
    if ((conv_direction & 0x01) && (0 == strcasecmp_P(command, Z_strings + pgm_read_word(&conv->tasmota_cmd_offset)))) {
      *cluster = conv_cluster;
      *cmd = conv_cmd;
      return Z_strings + pgm_read_word(&conv->param_offset);
    }
  }

  return nullptr;
}

// take the lower 4 bits and turn it to an hex char
inline char hexDigit(uint32_t h) {
  uint32_t nybble = h & 0x0F;
  return (nybble > 9) ? 'A' - 10 + nybble : '0' + nybble;
}

// replace all xx/yy/zz substrings with unsigned ints, and the corresponding len (8, 16 or 32 bits)
// Returns a SBuffer allocated object, it is the caller's responsibility to delete it
void zigbeeCmdAddParams(SBuffer & buf, const char *zcl_cmd_P, uint32_t x, uint32_t y, uint32_t z) {
  size_t hex_len = strlen_P(zcl_cmd_P);
  buf.reserve((hex_len + 1)/2);

  const char * p = zcl_cmd_P;
  char c0, c1;
  while ((c0 = pgm_read_byte(p)) && (c1 = pgm_read_byte(p+1))) {
    if (isXYZ(c0) && (c0 == c1)) {    // if char is [x-z] and followed by same char
      uint8_t val = 0;
      switch (pgm_read_byte(p)) {
        case 'x':
          val = x & 0xFF;
          x = x >> 8;
          break;
        case 'y':
          val = y & 0xFF;
          y = y >> 8;
          break;
        case 'z':
          val = z & 0xFF;
          z = z >> 8;
          break;
      }
      buf.add8(val);
      // *p = hexDigit(val >> 4);
      // *(p+1) = hexDigit(val);
    } else {
      char hex[4];
      hex[0] = c0;
      hex[1] = c1;
      hex[2] = 0;
      buf.add8(strtoul(hex, nullptr, 16) & 0xFF);
    }
    p += 2;
  }
}

const char kZ_Alias[] PROGMEM = "OFF|" D_OFF "|" D_FALSE "|" D_STOP  "|" "OPEN" "|"           // 0
                                "ON|"  D_ON  "|" D_TRUE  "|" D_START "|" "CLOSE" "|"          // 1
                                "TOGGLE|" D_TOGGLE "|"                                        // 2
                                "ALL" ;                                                       // 255

const uint8_t kZ_Numbers[] PROGMEM = { 0,0,0,0,0,
                                       1,1,1,1,1,
                                       2,2,
                                       255 };

// Convert an alias like "On" to the corresponding number
uint32_t ZigbeeAliasOrNumber(const char *state_text) {
  char command[16];
  int state_number = GetCommandCode(command, sizeof(command), state_text, kZ_Alias);
  if (state_number >= 0) {
    // found an alias, get its value
    return pgm_read_byte(kZ_Numbers + state_number);
  } else {
    // no alias found, convert it as number
    return strtoul(state_text, nullptr, 0);
  }
}


/*********************************************************************************************\
 * 
 * Send Zigbee commands
 * 
\*********************************************************************************************/

#endif // USE_ZIGBEE
