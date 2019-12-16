/*
  xdrv_23_zigbee_converters.ino - zigbee support for Tasmota

  Copyright (C) 2019  Theo Arends and Stephan Hadinger

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

//typedef int32_t (*Z_AttrConverter)(uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const char *new_name, void * param);
typedef struct Z_CommandConverter {
  const char * tasmota_cmd;
  const char * zcl_cmd;
} Z_CommandConverter;

// list of post-processing directives
const Z_CommandConverter Z_Commands[] = {
  { "Power",        "0006!xx" },              // 0=Off, 1=On, 2=Toggle
  { "Dimmer",       "0008!04/xx0A00" },       // Move to Level with On/Off, xx=0..254 (255 is invalid)
  { "Dimmer+",      "0008!06/001902" },       // Step up by 10%, 0.2 secs
  { "Dimmer-",      "0008!06/011902" },       // Step down by 10%, 0.2 secs
  { "DimmerStop",   "0008!03" },              // Stop any Dimmer animation
  { "ResetAlarm",   "0009!00/xxyyyy" },       // Reset alarm (alarm code + cluster identifier)
  { "ResetAllAlarms","0009!01" },             // Reset all alarms
  { "Hue",          "0300!00/xx000A00" },     // Move to Hue, shortest time, 1s
  { "Sat",          "0300!03/xx0A00" },       // Move to Sat
  { "HueSat",       "0300!06/xxyy0A00" },     // Hue, Sat
  { "Color",        "0300!07/xxxxyyyy0A00" }, // x, y (uint16)
  { "CT",           "0300!0A/xxxx0A00" },     // Color Temperature Mireds (uint16)
  { "Shutter",      "0102!xx" },
  { "ShutterOpen",  "0102!00"},
  { "ShutterClose", "0102!01"},
  { "ShutterStop",  "0102!02"},
  { "ShutterLift",  "0102!05xx"},           // Lift percentage, 0%=open, 100%=closed
  { "ShutterTilt",  "0102!08xx"},           // Tilt percentage
};

#define ZLE(x) ((x) & 0xFF), ((x) >> 8)     // Little Endian

// Below are the attributes we wand to read from each cluster
const uint8_t CLUSTER_0006[] = { ZLE(0x0000) };    // Power
const uint8_t CLUSTER_0008[] = { ZLE(0x0000) };    // CurrentLevel
const uint8_t CLUSTER_0009[] = { ZLE(0x0000) };    // AlarmCount
const uint8_t CLUSTER_0300[] = { ZLE(0x0000), ZLE(0x0001), ZLE(0x0003), ZLE(0x0004), ZLE(0x0007) };    // Hue, Sat, X, Y, CT

int32_t Z_ReadAttrCallback(uint16_t shortaddr, uint16_t cluster, uint16_t endpoint, uint32_t value) {
  size_t         attrs_len = 0;
  const uint8_t* attrs = nullptr;

  switch (cluster) {
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
    ZigbeeZCLSend(shortaddr, cluster, endpoint, ZCL_READ_ATTRIBUTES, false, attrs, attrs_len, false /* we do want a response */);
  }
}


// set a timer to read back the value in the future
void zigbeeSetCommandTimer(uint16_t shortaddr, uint16_t cluster, uint16_t endpoint) {
  uint32_t wait_ms = 0;

  switch (cluster) {
    case 0x0006:        // for On/Off
    case 0x0009:        // for Alamrs
      wait_ms = 200;    // wait 0.2 s
      break;
    case 0x0008:        // for Dimmer
    case 0x0300:        // for Color
      wait_ms = 1050;   // wait 1.0 s
      break;
    case 0x0102:        // for Shutters
      wait_ms = 10000;  // wait 10.0 s
      break;
  }
  if (wait_ms) {
    zigbee_devices.setTimer(shortaddr, wait_ms, cluster, endpoint, 0 /* value */, &Z_ReadAttrCallback);
  }
}

const __FlashStringHelper* zigbeeFindCommand(const char *command) {
  char parm_uc[16];   // used to convert JSON keys to uppercase
  for (uint32_t i = 0; i < sizeof(Z_Commands) / sizeof(Z_Commands[0]); i++) {
    const Z_CommandConverter *conv = &Z_Commands[i];
    if (0 == strcasecmp_P(command, conv->tasmota_cmd)) {
      return (const __FlashStringHelper*) conv->zcl_cmd;
    }
  }

  return nullptr;
}

inline bool isXYZ(char c) {
  return (c >= 'x') && (c <= 'z');
}

// take the lower 4 bits and turn it to an hex char
inline char hexDigit(uint32_t h) {
  uint32_t nybble = h & 0x0F;
  return (nybble > 9) ? 'A' - 10 + nybble : '0' + nybble;
}

// replace all xx/yy/zz substrings with unsigned ints, and the corresponding len (8, 16 or 32 bits)
String zigbeeCmdAddParams(const char *zcl_cmd_P, uint32_t x, uint32_t y, uint32_t z) {
  size_t len = strlen_P(zcl_cmd_P);
  char zcl_cmd[len+1];
  strcpy_P(zcl_cmd, zcl_cmd_P);     // copy into RAM

  char *p = zcl_cmd;
  while (*p) {
    if (isXYZ(*p) && (*p == *(p+1))) {    // if char is [x-z] and followed by same char
      uint8_t val;
      switch (*p) {
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
      *p = hexDigit(val >> 4);
      *(p+1) = hexDigit(val);
      p++;
    }
    p++;
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SendZCLCommand_P: zcl_cmd = %s"), zcl_cmd);

  return String(zcl_cmd);
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

#endif // USE_ZIGBEE
