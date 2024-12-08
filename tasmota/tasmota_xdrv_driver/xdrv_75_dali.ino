/*
  xdrv_75_dali.ino - DALI support for Tasmota

  Copyright (C) 2022  Andrei Kazmirtsuk aka eeak and Theo Arends

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

  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  1.1.0.1 20241101  update    - Enable DALI if another light is already claimed
  1.1.0.0 20241031  update    - Add GUI sliders with feedback when `DaliLight 0`
                              - Add command `DaliGroupSliders 0..16` to show GUI sliders
  1.0.0.2 20241025  update    - Fix GPIO detection
                              - Fix ESP32(C3) transmit stability by disabling interrupts
  1.0.0.1 20241024  update    - Change from signal invert defines to GPIO config DALI RX_i/DALI TX_i
                              - Fix inverted DALI signal support
                              - Experimental support for Shelly DALI Dimmer Gen3
  1.0.0.0 20241022  update    - Refactor commission
                              - Add receive collision detection
  0.1.0.8 20241019  update    - Rename command `DaliCommission` to `DaliScan`
                              - Rename command `DaliWeb` to `DaliLight`
                              - Add command `DaliGear` to set max found gear to speed up scan response
  0.1.0.7 20241017  update    - Add command `DaliCommission 1|2` assigning short addresses
                              - Add command `DaliTarget 0, 1..64, 101..116` to select light control address
                              - Add command `DaliGroup1..16 +<devices>|-<devices>` to add/remove devices from group
                              - Extend commands `DaliPower` and `DaliDimmer` with index to control short address or group
                              - Remove non-functional MQTT interface
  0.1.0.6 20241014  update    - Fix received light command loop
                              - Add send collision detection
  0.1.0.5 20241014  update    - Add command `DaliSend [repeat]<address>,<command>`
                              - Add command `DaliQuery [repeat]<address>,<command>`
                              - Send frame twice (repeat) for DALI defined commands
                              - Add support for receiving backward frame
  0.1.0.4 20241013  update    - Fix intermittent bad send timing
  0.1.0.3 20241010  update    - Change DaliDimmer range from 0..254 to 0..100
                              - Add command `DaliWeb 0|1` to enable persistent Web light controls
  0.1.0.2 20241008  update    - Better receive error detection
  0.1.0.1 20241007  update    - To stablizie communication send DALI datagram twice like Busch-Jaeger does 
                              - Change DaliPower 0..2 to act like Tasmota Power (Off, On, Toggle)
                              - Keep last Dimmer value as default power on
  0.1.0.0 20241006  rewrite   - Add support for ESP8266
                              - Fix decoding of received DALI data
                              - Refactor command `DaliPower 0..254` controlling Broadcast devices
                              - Add command `DaliDimmer 0..254` controlling Broadcast devices
  0.0.0.1 20221027  publish   - Initial version
*/

#ifdef USE_DALI

/*********************************************************************************************\
 * DALI support for Tasmota
 * 
 * Available commands:
 *   <byte1> or <byte3> = 0..255 or 0x00..0xFF + 256/0x100 for optional repeat (send twice)
 *   <byte2> or <byte4> = 0..255 or 0x00..0xFF   - Both decimal and hexadecimal is supported
 *   <broadcast> = 0                             - DALI default
 *   <device> = 1..64                            - DALI short address + 1
 *   <group> = 101..116                          - DALI group + 101
 * DaliSend <byte1>,<byte2>                      - Execute DALI code and do not expect a DALI backward frame
 * DaliSend <0xA3>,<byte2>,<byte3>,<byte4>       - Set DALI parameter using DTR0 and do not expect a DALI backward frame
 * DaliQuery <byte1>,<byte2>                     - Execute DALI code and report result (DALI backward frame)
 * DaliScan 1|2                                  - Reset (0) or (1)/and commission device short addresses
 * DaliGear 1..64                                - Set max short address to speed up scanning - default 64
 * DaliGroup<1..16> [+]|-<device>,<device>...    - Add(+) or Remove(-) devices to/from group
 * DaliPower<broadcast>|<device>|<group> 0..254  - Control power (0 = Off, 1 = Last dimmer, 2 = Toggle, 3..254 = absolute light brightness)
 * DaliDimmer<broadcast>|<device>|<group> 0..100 - Control dimmer (0 = Off, 1..100 = precentage of brightness)
 * DaliLight 0|1                                 - Enable Tasmota light control for DaliTarget device - default 1
 * DaliTarget <broadcast>|<device>|<group>       - Set Tasmota light control device (0, 1..64, 101..116) - default 0
 * 
 * DALI background information
 * Address type        Address byte
 * ------------------  --------------------
 * Broadcast address   1111111S
 * 64 short address    0AAAAAAS
 * 16 group address    100AAAAS
 * Special command     101CCCC1 to 110CCCC1
 * A = Address bit, S = 0 Direct Arc Power control, S = 1 Command, C = Special command
 * 
 * Shelly DALI Dimmer Gen3 (ESP32C3-8M) - GPIO3 controls DALI power. In following template it is always ON. Max output is 16V/10mA (= 5 DALI gear)
 * Template {"NAME":"Shelly DALI Dimmer Gen3","GPIO":[34,4736,0,3840,11360,11392,128,129,0,1,576,0,0,0,0,0,0,0,0,1,1,1],"FLAG":0,"BASE":1}
 * AdcGpio1 10000,10000,4000        <- Temperature parameters
 * Backlog ButtonTopic 0; SetOption1 1; SetOption11 0; SetOption32 20; DimmerStep 5; LedTable 0
 * rule1 on button1#state=2 do dimmer + endon on button2#state=2 do dimmer - endon on button1#state=3 do power 2 endon on button2#state=3 do power 2 endon
\*********************************************************************************************/

#define XDRV_75                    75

#ifndef DALI_INIT_STATE
#define DALI_INIT_STATE            50      // DALI init dimmer state 50/254
#endif
#ifndef DALI_INIT_FADE 
#define DALI_INIT_FADE             1       // Fade between light states in number of seconds
#endif
#ifndef DALI_TIMEOUT
#define DALI_TIMEOUT               50      // DALI backward frame receive timeout (ms)
#endif

//#define DALI_DEBUG
#ifndef DALI_DEBUG_PIN
#define DALI_DEBUG_PIN             4       // Debug GPIO
#endif

// Control commands - Send as second byte without repeat
#define DALI_OFF                               0x0000  //   0 - Turns off lighting.
#define DALI_UP                                0x0001  //   1 - Increases the lighting control level for 200 ms according to the Fade rate.
#define DALI_DOWN                              0x0002  //   2 - Decreases the lighting control level for 200 ms according to the Fade rate.
#define DALI_STEP_UP                           0x0003  //   3 - Increments the lighting control level (without fade).
#define DALI_STEP_DOWN                         0x0004  //   4 - Decrements the lighting control level (without fade).
#define DALI_RECALL_MAX_LEVEL                  0x0005  //   5 - Maximizes the lighting control level (without fade).
#define DALI_RECALL_MIN_LEVEL                  0x0006  //   6 - Minimizes the lighting control level (without fade)
#define DALI_STEP_DOWN_AND_OFF                 0x0007  //   7 - Decrements the lighting control level and turns off lighting if the level is at the minimum (without fade).
#define DALI_ON_AND_STEP_UP                    0x0008  //   8 - Increments the lighting control level and turns on lighting if lighting is off (with fade). 
#define DALI_ENABLE_DAPC_SEQUENCE              0x0009  //   9 - It shows the repeat start of the DAPC command.
#define DALI_GO_TO_LAST_ACTIVE_LEVEL           0x000A  //  10 - DALI-2 Adjusts the lighting control level to the last light control level according to the Fade time.
#define DALI_RESERVED11                        0x000B  //  11 - [Reserved]
#define DALI_RESERVED15                        0x000F  //  15 - [Reserved]
#define DALI_GO_TO_SCENE0                      0x0010  //  16 - Adjusts the lighting control level for Scene XXXX according to the fade time.
#define DALI_GO_TO_SCENE15                     0x001F  //  31 - Adjusts the lighting control level for Scene XXXX according to the fade time.

// Configuration commands - Send as second byte with repeat
#define DALI_RESET                             0x0020  //  32 - Makes a slave an RESET state.
#define DALI_STORE_ACTUAL_LEVEL_IN_THE_DTR0    0x0021  //  33 - Saves the current lighting control level to the DTR (DTR0).
#define DALI_SAVE_PERSISTENT_VARIABLES         0x0022  //  34 - DALI-2 Saves a variable in nonvolatile memory (NVM).
#define DALI_SET_OPERATING_MODE                0x0023  //  35 - DALI-2 Sets data of DTR0 as an operating mode.
#define DALI_RESET_MEMORY_BANK                 0x0024  //  36 - DALI-2 Changes to the reset value the specified memory bank in DTR0.
#define DALI_IDENTIFY_DEVICE                   0x0025  //  37 - DALI-2 Starts the identification state of the device.
#define DALI_RESERVED38                        0x0026  //  38 - [Reserved]
#define DALI_RESERVED41                        0x0029  //  41 - [Reserved]
#define DALI_SET_MAX_LEVEL                     0x002A  //  42 - Specifies the DTR data as the maximum lighting control level.
#define DALI_SET_MIN_LEVEL                     0x002B  //  43 - Specifies the DTR data as the minimum lighting control level.
#define DALI_SET_SYSTEM_FAILURE_LEVEL          0x002C  //  44 - Specifies the DTR data as the "FAILURELEVEL".
#define DALI_SET_POWER_ON_LEVEL                0x002D  //  45 - Specifies the DTR data as the "POWER ONLEVEL".
#define DALI_SET_FADE_TIME                     0x002E  //  46 - Specifies the DTR data as the Fade time.
#define DALI_SET_FADE_RATE                     0x002F  //  47 - Specifies the DTR data as the Fade rate.
#define DALI_SET_EXTENDED_FADE_TIME            0x0030  //  48 - DALI-2 Specifies the DTR data as the Extended Fade Time.
#define DALI_RESERVED49                        0x0031  //  49 - [Reserved]
#define DALI_RESERVED63                        0x003F  //  63 - [Reserved]
#define DALI_SET_SCENE0                        0x0040  //  64 - Specifies the DTR data as Scene XXXX.
#define DALI_SET_SCENE15                       0x004F  //  79 - Specifies the DTR data as Scene XXXX.
#define DALI_REMOVE_FROM_SCENE0                0x0050  //  80 - Deletes the Scene XXXX setting. (Specifies 1111 1111 for the scene register.)
#define DALI_REMOVE_FROM_SCENE15               0x005F  //  95 - Deletes the Scene XXXX setting. (Specifies 1111 1111 for the scene register.)
#define DALI_ADD_TO_GROUP0                     0x0060  //  96 - Adds the slave to Group XXXX.
#define DALI_ADD_TO_GROUP15                    0x006F  // 111 - Adds the slave to Group XXXX.
#define DALI_REMOVE_FROM_GROUP0                0x0070  // 112 - Deletes the slave from Group XXXX.
#define DALI_REMOVE_FROM_GROUP15               0x007F  // 127 - Deletes the slave from Group XXXX.
#define DALI_SET_SHORT_ADDRESS                 0x0080  // 128 - Specifies the DTR data as a Short Address.
#define DALI_ENABLE_WRITE_MEMORY               0x0081  // 129 - Allows writing of the memory bank.
#define DALI_RESERVED130                       0x0082  // 130 - [Reserved]
#define DALI_RESERVED143                       0x008F  // 143 - [Reserved]

// Query commands - Send as second byte
#define DALI_QUERY_STATUS                      0x0090  // 144 - Returns "STATUS INFORMATION"
#define DALI_QUERY_CONTROL_GEAR_PRESENT        0x0091  // 145 - Is there a slave that can communicate?
#define DALI_QUERY_LAMP_FAILURE                0x0092  // 146 - Is there a lamp problem?
#define DALI_QUERY_LAMP_POWER_ON               0x0093  // 147 - Is a lamp on?
#define DALI_QUERY_LIMIT_ERROR                 0x0094  // 148 - Is the specified lighting control level out of the range from the minimum to the maximum values?
#define DALI_QUERY_RESET_STATE                 0x0095  // 149 - Is the slave in 'RESET STATE'?
#define DALI_QUERY_MISSING_SHORT_ADDRESS       0x0096  // 150 - Does the slave not have a short address?
#define DALI_QUERY_VERSION_NUMBER              0x0097  // 151 - What is the corresponding IEC standard number?
#define DALI_QUERY_CONTENT_DTR0                0x0098  // 152 - What is the DTR content?
#define DALI_QUERY_DEVICE_TYPE                 0x0099  // 153 - What is the device type?  (fluorescent lamp:0000 0000) (IEC62386-207 is 6 fixed)
#define DALI_QUERY_PHYSICAL_MINIMUM_LEVEL      0x009A  // 154 - What is the minimum lighting control level specified by the hardware?
#define DALI_QUERY_POWER_FAILURE               0x009B  // 155 - Has the slave operated without the execution of reset-command or the adjustment of the lighting control level?
#define DALI_QUERY_CONTENT_DTR1                0x009C  // 156 - What is the DTR1 content?
#define DALI_QUERY_CONTENT_DTR2                0x009D  // 157 - What is the DTR2 content?
#define DALI_QUERY_OPERATING_MODE              0x009E  // 158 - DALI-2 What is the Operating Mode?
#define DALI_QUERY_LIGHT_SOURCE_TYPE           0x009F  // 159 - DALI-2 What is the Light source type?
#define DALI_QUERY_ACTUAL_LEVEL                0x00A0  // 160 - What is the "ACTUAL LEVEL" (the current lighting control level)?
#define DALI_QUERY_MAX_LEVEL                   0x00A1  // 161 - What is the maximum lighting control level?
#define DALI_QUERY_MIN_LEVEL                   0x00A2  // 162 - What is the minimum lighting control level?
#define DALI_QUERY_POWER_ON_LEVEL              0x00A3  // 163 - What is the "POWER ON LEVEL" (the lighting control level when the power is turned on)?
#define DALI_QUERY_SYSTEM_FAILURE_LEVEL        0x00A4  // 164 - What is the "SYSTEM FAILURE LEVEL" (the lighting control level when a failure occurs)?
#define DALI_QUERY_FADE_TIME_FADE_RATE         0x00A5  // 165 - What are the Fade time and Fade rate?
#define DALI_QUERY_MANUFACTURER_SPECIFIC_MODE  0x00A6  // 166 - DALI-2 What is the Specific Mode?
#define DALI_QUERY_NEXT_DEVICE_TYPE            0x00A7  // 167 - DALI-2 What is the next Device Type?
#define DALI_QUERY_EXTENDED_FADE_TIME          0x00A8  // 168 - DALI-2 What is the Extended Fade Time?
#define DALI_QUERY_CONTROL_GEAR_FAILURE        0x00A9  // 169 - DALI-2 Does a slave have the abnormality?
#define DALI_RESERVED170                       0x00AA  // 170 - [Reserved]
#define DALI_RESERVED175                       0x00AF  // 175 - [Reserved]
#define DALI_QUERY_SCENE0_LEVEL                0x00B0  // 176 - What is the lighting control level for SCENE XXXX?
#define DALI_QUERY_GROUPS_0_7                  0x00C0  // 192 - Does the slave belong to a group among groups 0 to 7? (Each bit corresponds to a group.)
#define DALI_QUERY_GROUPS_8_15                 0x00C1  // 193 - Does the slave belong to a group among groups 8 to 15? (Each bit corresponds to a group.)
#define DALI_QUERY_RANDOM_ADDRESS_H            0x00C2  // 194 - What are the higher 8 bits of the random address?
#define DALI_QUERY_RANDOM_ADDRESS_M            0x00C3  // 195 - What are the middle 8 bits of the random address?
#define DALI_QUERY_RANDOM_ADDRESS_L            0x00C4  // 196 - What are the lower 8 bits of the random address?
#define DALI_READ_MEMORY_LOCATION              0x00C5  // 197 - What is the memory location content? 
#define DALI_RESERVED198                       0x00C6  // 198 - [Reserved]
#define DALI_RESERVED223                       0x00DF  // 223 - [Reserved]

// Application extended configuration commands - Send as second byte
#define DALI_REFERENCE_SYSTEM_POWER            0x00E0  // 224 - IEC62386-207 Starts power measurement.
#define DALI_ENABLE_CURRENT_PROTECTOR          0x00E1  // 225 - IEC62386-207 Enables the current protection.
#define DALI_DISABLE_CURRENT_PROTECTOR         0x00E2  // 226 - IEC62386-207 Disables the current protection.
#define DALI_SELECT_DIMMING_CURVE              0x00E3  // 227 - IEC62386-207 Selects Dimming curve.
#define DALI_STORE_DTR_AS_FAST_FADE_TIME       0x00E4  // 228 - IEC62386-207 Sets the DTR of the data as Fast Fade Time.
#define DALI_RESERVED229                       0x00E5  // 229 - [Reserved]
#define DALI_RESERVED236                       0x00EC  // 236 - [Reserved]

// Application extended query commands - Send as second byte
#define DALI_QUERY_GEAR_TYPE                   0x00ED  // 237 - IEC62386-207 Returns ‘GEAR TYPE’
#define DALI_QUERY_DIMMING_CURVE               0x00EE  // 238 - IEC62386-207 Returns ’Dimming curve’in use
#define DALI_QUERY_POSSIBLE_OPERATING_MODE     0x00EF  // 239 - IEC62386-207 Returns ‘POSSIBLE OPERATING MODE’
#define DALI_QUERY_FEATURES                    0x00F0  // 240 - IEC62386-207 Returns ‘FEATURES’
#define DALI_QUERY_FAILURE_STATUS              0x00F1  // 241 - IEC62386-207 Returns ‘FAILURE STATUS’
#define DALI_QUERY_SHORT_CIRCUIT               0x00F2  // 242 - IEC62386-207 Returns bit0 short circuit of ‘FAILURE STATUS’
#define DALI_QUERY_OPEN_CIRCUIT                0x00F3  // 243 - IEC62386-207 Returns bit1 open circuit of ‘FAILURE STATUS’
#define DALI_QUERY_LOAD_DECREASE               0x00F4  // 244 - IEC62386-207 Returns bit2 load decrease of ‘FAILURE STATUS’
#define DALI_QUERY_LOAD_INDREASE               0x00F5  // 245 - IEC62386-207 Returns bit3 load increase of‘FAILURE STATUS’
#define DALI_QUERY_CURRENT_PROTECTOR_ACTIVE    0x00F6  // 246 - IEC62386-207 Returns bit4 current protector active of ‘FAILURE STATUS’
#define DALI_QUERY_THERMAL_SHUTDOWN            0x00F7  // 247 - IEC62386-207 Returns bit5 thermal shut down of ‘FAILURE STATUS’
#define DALI_QUERY_THERMAL_OVERLOAD            0x00F8  // 248 - IEC62386-207 Returns bit6 thermal overload with light level reduction of ‘FAILURE STATUS’
#define DALI_QUERY_REFARENCE_RUNNING           0x00F9  // 249 - IEC62386-207 Returns whetherReference System Power is in operation.
#define DALI_QUERY_REFERENCE_MEASURMENT_FAILED 0x00FA  // 250 - IEC62386-207 Returns bit7 reference measurement failed  of ‘FAILURE STATUS’
#define DALI_QUERY_CURRENT_PROTECTOR_ENABLE    0x00FB  // 251 - IEC62386-207 Returns state of Curent protector
#define DALI_QUERY_OPERATING_MODE2             0x00FC  // 252 - IEC62386-207 Returns ‘OPERATING MODE’
#define DALI_QUERY_FAST_FADE_TIME              0x00FD  // 253 - IEC62386-207 Returns set Fast fade time.
#define DALI_QUERY_MIN_FAST_FADE_TIME          0x00FE  // 254 - IEC62386-207 Returns set Minimum fast fade time
#define DALI_QUERY_EXTENDED_VERSION_NUMBER     0x00FF  // 255 - IEC62386-207 The version number of the extended support? IEC62386-207: 1, Other: NO(no response)

// Address types - Send as first byte
#define DALI_SHORT_ADDRESS0                    0x0000  // 0b00000000   0 - First short address
#define DALI_SHORT_ADDRESS1                    0x0002  // 0b00000010   1 - Next short address
#define DALI_SHORT_ADDRESS63                   0x007E  // 0b01111110  63 - Last short address
#define DALI_GROUP_ADDRESS0                    0x0080  // 0b10000000   0 - First group address
#define DALI_GROUP_ADDRESS1                    0x0082  // 0b10000010   1 - Next group address
#define DALI_GROUP_ADDRESS15                   0x009E  // 0b10011110  15 - Last group address

// Special commands - Send as first byte
#define DALI_TERMINATE                         0x00A1  // 256 - Releases the INITIALISE state.
#define DALI_DATA_TRANSFER_REGISTER0           0x00A3  // 257 - Stores the data XXXX XXXX to the DTR(DTR0).
#define DALI_INITIALISE                        0x01A5  // 258 REPEAT - Sets the slave to the INITIALISE status for15 minutes. Commands 259 to 270 are enabled only for a slave in this status.
#define DALI_RANDOMISE                         0x01A7  // 259 REPEAT - Generates a random address.
#define DALI_COMPARE                           0x00A9  // 260 - Is the random address smaller or equal to the search address?
#define DALI_WITHDRAW                          0x00AB  // 261 - Excludes slaves for which the random address and search address match from the Compare process.
#define DALI_RESERVED262                       0x00AD  // 262 - [Reserved]
#define DALI_PING                              0x00AF  // 263 - DALI-2 Ignores in the slave.
#define DALI_SEARCHADDRH                       0x00B1  // 264 - Specifies the higher 8 bits of the search address.
#define DALI_SEARCHADDRM                       0x00B3  // 265 - Specifies the middle 8 bits of the search address.
#define DALI_SEARCHADDRL                       0x00B5  // 266 - Specifies the lower 8 bits of the search address.
#define DALI_PROGRAM_SHORT_ADDRESS             0x00B7  // 267 - The slave shall store the received 6-bit address (AAA AAA) as a short address if it is selected.
#define DALI_VERIFY_SHORT_ADDRESS              0x00B9  // 268 - Is the short address AAA AAA?
#define DALI_QUERY_SHORT_ADDRESS               0x00BB  // 269 - What is the short address of the slaveNote 2being selected?
#define DALI_PHYSICAL_SELECTION                0x00BD  // 270 - not DALI-2 Sets the slave to Physical Selection Mode and excludes the slave from the Compare process. (Excluding IEC62386-102ed2.0)
#define DALI_RESERVED271                       0x00BF  // 271 - [Reserved]

// Extending special commands - Send as first byte
#define DALI_ENABLE_DEVICE_TYPE_X              0x00C1  // 272 - Adds the device XXXX (a special device).
#define DALI_DATA_TRANSFER_REGISTER1           0x00C3  // 273 - Stores data XXXX into DTR1.
#define DALI_DATA_TRANSFER_REGISTER2           0x00C5  // 274 - Stores data XXXX into DTR2.
#define DALI_WRITE_MEMORY_LOCATION             0x00C7  // 275 - Write data into the specified address of the specified memory bank. (There is BW) (DTR(DTR0)：address, DTR1：memory bank number)
#define DALI_WRITE_MEMORY_LOCATION_NO_REPLY    0x00C9  // 276 - DALI-2 Write data into the specified address of the specified memory bank. (There is no BW) (DTR(DTR0)：address, TR1：memory bank number)
#define DALI_RESERVED277                       0x00CB  // 277 - [Reserved]
#define DALI_RESERVED349                       0x00FD  // 349 - [Reserved]

// Address type - Send as first byte
#define DALI_BROADCAST_DP                      0x00FE  // 0b11111110 254 - Broadcast address

#define DALI_MAX_STORED                        17      // Store broadcast and group states

#define DALI_TOPIC "DALI"
#define D_PRFX_DALI "Dali"

/*********************************************************************************************/

const char kDALICommands[] PROGMEM = D_PRFX_DALI "|"  // Prefix
  "|" D_CMND_POWER "|" D_CMND_DIMMER "|Target"
#ifdef USE_LIGHT
  "|Light"
#endif  // USE_LIGHT
  "|Send|Query|Scan|Group"
  "|GroupSliders|Gear";

void (* const DALICommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliPower, &CmndDaliDimmer, &CmndDaliTarget,
#ifdef USE_LIGHT
  &CmndDaliLight,
#endif  // USE_LIGHT
  &CmndDaliSend, &CmndDaliQuery, &CmndDaliScan, &CmndDaliGroup,
  &CmndDaliGroupSliders, &CmndDaliGear };

struct DALI {
  uint32_t bit_cycles;
  uint32_t last_activity;
  uint32_t received_dali_data;                 // Data received from DALI bus
  uint8_t pin_rx;
  uint8_t pin_tx;
  uint8_t max_short_address;
  uint8_t address;
  uint8_t command;
  uint8_t last_dimmer;
  uint8_t dimmer[DALI_MAX_STORED];
  uint8_t web_dimmer[DALI_MAX_STORED];
  uint8_t target;
  bool allow_light;
  bool last_power;
  bool power[DALI_MAX_STORED];
  bool available;
  bool response;
  bool light_sync;
  bool probe;
  bool invert_rx;
  bool invert_tx;
} *Dali = nullptr;

/*********************************************************************************************\
 * DALI low level
\*********************************************************************************************/

uint32_t DaliTarget2Address(void) {
  // 1..64    = Short address
  // 101..116 = Group address
  // Others   = Broadcast
  if ((Dali->target >= 1) && (Dali->target <= 64)) {  // 1 .. 64
    Dali->target -= 1;                         // Short address
    Dali->target <<= 1;
  }
  else if ((Dali->target >= 101) && (Dali->target <= 116)) {  // 101 .. 116
    Dali->target -= 101;
    Dali->target <<= 1;
    Dali->target |= 0x80;                      // Group address
  }
  else {                                       // Others
    Dali->target = DALI_BROADCAST_DP;          // Broadcast address
  }
  return Dali->target &0xFE;                   // Direct Arc Power Control command
}

/*
uint32_t DaliAddress2Target(uint32_t adr) {
  if (adr >= 254) {                            // 0b1111111S
    return 0;                                  // Broadcast address (0)
  }
  else if ((adr >= 128) && (adr <= 159)) {     // 0b1000000S .. 0b1001111S
    return (adr >> 1) +101;                    // Group address (101 .. 116)
  }
  return (adr >> 1) +1;                        // 0b0000000S .. 0b0111111S Short address (1 .. 64)
}
*/

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliSaveState(uint32_t adr, uint32_t cmd) {
  if (adr &0x01) { return 0; }                 // No address
  int index = -1;
  if (DALI_BROADCAST_DP == adr) {              // Broadcast address
    index = 0;
  }
  adr >>= 1;
  if ((adr >= 0x40) && (adr <= 0x4F)) {        // Group address 0 to 15
    index = adr -0x3F;
  }
  if (index >= 0) {
    Dali->last_power = Dali->power[index];
    Dali->power[index] = (cmd);                // State
    if (Dali->power[index]) {
      Dali->last_dimmer = Dali->dimmer[index];
      Dali->dimmer[index] = cmd;               // Value
    }
    if ((0 == index) && !Dali->power[0]) {     // Only on Broadcast change to power Off
      for (uint32_t i = 0; i < DALI_MAX_STORED; i++) {
        Dali->power[i] = false;                // Log all group power as Off when Broadcast is Off
      }
    }
  } else {
    index = 0;                                 // Use broadcast
  }
  return index;
}

/*-------------------------------------------------------------------------------------------*/

void DaliEnableRxInterrupt(void) {
  Dali->available = false;
  attachInterrupt(Dali->pin_rx, DaliReceiveData, (Dali->invert_rx) ? RISING : FALLING);
}

void DaliDisableRxInterrupt(void) {
  detachInterrupt(Dali->pin_rx);
}

/*-------------------------------------------------------------------------------------------*\
 * DALI receive
\*-------------------------------------------------------------------------------------------*/

void IRAM_ATTR DaliReceiveData(void);          // Fix ESP8266 ISR not in IRAM! exception
void DaliReceiveData(void) {
  /*
  Forward frame (1 Start bit + 16 data bits) * 2 bits/bit (manchester encoding) + 2 * 2 Stop bits = 38 bits
  DALI data 0xFE64       1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0       Forward frame
  Start and Stop bits  1                                 1 1
  Manchester data     0101010101010101101001011010011010
  Stop bits                                             1111                  

  Backward frame (1 Start bit + 8 data bits) * 2 bits/bit (manchester encoding) + 2 * 2 Stop bits = 22 bits
  DALI data 0x64         0 1 1 0 0 1 0 0                       Backward frame
  Start and Stop bits  1                 1 1
  Manchester data     011001011010011010
  Stop bits                             1111                  

  Bit number          01234567890123456789012345678901234567
                                1         2         3
  */
  if (Dali->available) { return; }             // Skip if last input is not yet handled
  uint32_t gap_time = millis() - Dali->last_activity;
  uint32_t wait = ESP.getCycleCount() + (Dali->bit_cycles / 2);
  int bit_state = 0; 
  bool dali_read;
  uint32_t received_dali_data = 0;
  uint32_t bit_number = 0;
  while (bit_number < 38) {
    while (ESP.getCycleCount() < wait);
    wait += Dali->bit_cycles;                  // Auto roll-over
    dali_read = (digitalRead(Dali->pin_rx) != Dali->invert_rx);
#ifdef DALI_DEBUG
    digitalWrite(DALI_DEBUG_PIN, bit_number&1);  // Add LogicAnalyzer poll indication
#endif  // DALI_DEBUG
    if (bit_number < 34) {                     // 34 manchester encoded bits
      bit_state += (dali_read) ? 1 : -1;
      if (0 == bit_state) {                    // Manchester encoding total 2 bits is always 0
        if (bit_number > 2) {                  // Skip start bit
          received_dali_data <<= 1;
          received_dali_data |= dali_read;
        }
      }
      else if ((2 == bit_state) &&
               (bit_number == 19)) {           // Possible backward frame detected - Chk stop bits
        bit_state = 0;
        bit_number = 35;
      }
      else if (abs(bit_state) > 1) {           // Invalid manchester data (too many 0 or 1)
        break;
      }
    } else {                                   // 4 high Stop bits
      if (bit_state != 0) {                    // Invalid manchester data
        break;
      }
      else if (dali_read != 1) {               // Invalid level of stop bit
        bit_state = 1;
        break;
      }
    }
    bit_number++;
  }
  Dali->last_activity = millis();

  if (bit_state != 0) {                        // Invalid Manchester encoding including start and stop bits               
    received_dali_data |= 0x00010000;          // Possible collision or invalid reply of repeated frame due to handling of first frame
    if (Dali->response) {                      // Expect backward frame with no collision
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Rx collision"));
    }
  }
  if (Dali->probe) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Rx %05X %d"), received_dali_data, gap_time);
  } else {
    if (Dali->response ||                      // Response from last message send
       (Dali->received_dali_data != received_dali_data)) {  // Skip duplicates
      Dali->received_dali_data = received_dali_data;
      Dali->available = true;                  // Any data received
    }
  }
}

/*-------------------------------------------------------------------------------------------*\
 * DALI send
\*-------------------------------------------------------------------------------------------*/

void DaliSendDataOnce(uint16_t send_dali_data) {
  /*
  DALI protocol forward frame
  DALI data 0xFE64       1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0
  Start and Stop bits  1                                 1 1
  Manchester data     0101010101010101101001011010011010
  Stop bits                                             1111                  

  Bit number          01234567890123456789012345678901234567
                                1         2         3
  */
  Dali->last_activity += 14;                   // As suggested by DALI protocol (> 9.17 ms)
  while (!TimeReached(Dali->last_activity)) {
    delay(1);                                  // Wait for bus to be free if needed
  }
  bool bit_value;
  bool pin_value;
  bool dali_read;
  bool collision = false;
  uint32_t bit_pos = 15;
  uint32_t bit_number = 0;

#ifdef ESP32
  {portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
  portENTER_CRITICAL(&mux);
#endif

  uint32_t wait = ESP.getCycleCount();
  while (bit_number < 35) {                    // 417 * 35 = 14.7 ms
    if (!collision) {
      if (0 == (bit_number &1)) {              // Even bit
        //          Start bit,              Stop bit,                Data bits
        bit_value = (0 == bit_number) ? 1 : (34 == bit_number) ? 0 : (bool)((send_dali_data >> bit_pos--) &1);  // MSB first
      } else {                                 // Odd bit
        bit_value = !bit_value;                // Complement bit
      }
      pin_value = bit_value ? LOW : HIGH;      // Invert bit
    } else {
      if (34 == bit_number) {
        pin_value = HIGH;                      // Set to idle
      }
    }

    digitalWrite(Dali->pin_tx, (Dali->invert_tx) ? !pin_value : pin_value);
    wait += Dali->bit_cycles;                  // Auto roll-over
    while (ESP.getCycleCount() < wait);

    if (!collision) {
      dali_read = (digitalRead(Dali->pin_rx) != Dali->invert_rx);
      if ((HIGH == pin_value) && (LOW == dali_read)) {  // Collision if write is 1 and bus is 0
        collision = true;
        pin_value = LOW;
        bit_number = 29;                       // Keep bus low for 4 bits
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Tx collision"));
      }
    }

    bit_number++;
  }

#ifdef ESP32
  portEXIT_CRITICAL(&mux);}
#endif

//  delayMicroseconds(1100);                     // Adds to total 15.8 ms
  Dali->last_activity = millis();
}

/*-------------------------------------------------------------------------------------------*/

void DaliSendData(uint32_t adr, uint32_t cmd) {
  bool repeat = (adr &0x100);                  // Set repeat if bit 8 is set
  adr &= 0xFF;
  cmd &= 0xFF;

  Dali->address = adr;
  Dali->command = cmd;
  DaliSaveState(adr, cmd);

  if (!repeat && (adr &0x01)) {                // YAAAAAA1 Commands where user didn't set repeat
    if ((adr >= 0xA1) && (adr <= 0xFD)) {      // Special commands
      repeat = ((0xA5 == adr) || (0xA7 == adr));
    } else {
      // ((cmd >=0) && (cmd <= 31))            // Arc power control commands
      repeat = (((cmd >=32) && (cmd <= 143)) ||  // Configuration commands
                ((cmd >=224) && (cmd <= 236)));  // Extended configuration commands
      // ((cmd >=144) && (cmd <= 223))         // Query commands
      // ((cmd >=237) && (cmd <= 255))         // Extended query commands
    }
  }

#ifdef DALI_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Tx 0x%d%02X%02X"), repeat, adr, cmd);
#endif  // DALI_DEBUG

  uint16_t send_dali_data = adr << 8 | cmd;

  DaliDisableRxInterrupt();
  DaliSendDataOnce(send_dali_data);            // Takes 14.7 ms
  if (repeat) {
    DaliSendDataOnce(send_dali_data);          // Takes 14.7 ms
  }
  delay(2);                                    // Block response
  DaliEnableRxInterrupt();
}

/*-------------------------------------------------------------------------------------------*/

int DaliSendWaitResponse(uint32_t adr, uint32_t cmd, uint32_t timeout = DALI_TIMEOUT);
int DaliSendWaitResponse(uint32_t adr, uint32_t cmd, uint32_t timeout) {
  Dali->response = true;
  DaliSendData(adr, cmd);
  while (!Dali->available && timeout--) {      // Expect backward frame within DALI_TIMEOUT ms
    delay(1);
  };
  int result = -1;                             // DALI NO or no response
  if (Dali->available) {
    Dali->available = false;                   // DALI collision (-2) or valid data (>=0)
    result = (Dali->received_dali_data &0x00010000) ? -2 : Dali->received_dali_data;
  }
  Dali->response = false;

#ifdef DALI_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Rx 0x%05X"), result);
#endif  // DALI_DEBUG

  return result;
}

/*********************************************************************************************\
 * DALI tools
 * 
 * Courtesy of https://github.com/qqqlab/DALI-Lighting-Interface
\*********************************************************************************************/

bool DaliSetValue(uint32_t adr, uint32_t getcmd, uint32_t setcmd, uint32_t v) {
  // Set a parameter value, returns true on success
  int current_v = DaliSendWaitResponse(adr, getcmd);  // Get current parameter value
  if (current_v == v) { return true; }         // Already set
  DaliSendData(DALI_DATA_TRANSFER_REGISTER0, v);  // Store value in DTR
  int dtr = DaliSendWaitResponse(adr, DALI_QUERY_CONTENT_DTR0);  // Get DTR value
  if (dtr != v) { return false; }
  DaliSendData(adr, setcmd);                   // Set parameter value = DTR
  current_v = DaliSendWaitResponse(adr, getcmd);  // Get current parameter value
  if (current_v != v) { return false; }        // Set failed
  return true;
}

bool DaliSetOperatingMode(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_QUERY_OPERATING_MODE, DALI_SET_OPERATING_MODE, v);
}

bool DaliSetMaxLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_QUERY_MAX_LEVEL, DALI_SET_MAX_LEVEL, v);
}

bool DaliSetMinLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_QUERY_MIN_LEVEL, DALI_SET_MIN_LEVEL, v);
}

bool DaliSetSystemFailureLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_QUERY_SYSTEM_FAILURE_LEVEL, DALI_SET_SYSTEM_FAILURE_LEVEL, v);
}

bool DaliSetPowerOnLevel(uint32_t adr, uint32_t v) {
  return DaliSetValue(adr, DALI_QUERY_POWER_ON_LEVEL, DALI_SET_POWER_ON_LEVEL, v);
}

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliGearPresent(void) {
  uint32_t count = 0;
  for (uint32_t sa = 0; sa < Dali->max_short_address; sa++) {  // Scanning 64 addresses takes about 2500 ms
    if (DaliSendWaitResponse(sa << 1 | 1, DALI_QUERY_CONTROL_GEAR_PRESENT, 20) >= 0) {
      count++;
    }
  }
  return count;
}

/*-------------------------------------------------------------------------------------------*/

void DaliInitLight(void) {
  // Taken from Shelly Dali Dimmer ;-)
  DaliSendData(DALI_DATA_TRANSFER_REGISTER0, DALI_INIT_FADE);  // Fade x second
  DaliSendData(0xFF, DALI_SET_FADE_TIME);
  DaliSendData(DALI_DATA_TRANSFER_REGISTER0, 0);     // Power off after gear power restore
  DaliSendData(0xFF, DALI_SET_POWER_ON_LEVEL);
  DaliSendData(DALI_DATA_TRANSFER_REGISTER0, 0xFE);  // Reset all but short circuit
  DaliSendData(0xFF, DALI_SET_SYSTEM_FAILURE_LEVEL);
}

/*********************************************************************************************\
 * DALI commissioning short addresses
 * 
 * Courtesy of https://github.com/qqqlab/DALI-Lighting-Interface
\*********************************************************************************************/

void DaliSetSearchAddress(uint32_t adr) {
  // Set search address
  DaliSendData(DALI_SEARCHADDRH, adr>>16);
  DaliSendData(DALI_SEARCHADDRM, adr>>8);
  DaliSendData(DALI_SEARCHADDRL, adr);
}

/*-------------------------------------------------------------------------------------------*/

void DaliSetSearchAddressDifference(uint32_t adr_new, uint32_t adr_current) {
  // Set search address, but set only changed bytes (takes less time)
  if ( (uint8_t)(adr_new>>16) !=  (uint8_t)(adr_current>>16) ) DaliSendData(DALI_SEARCHADDRH, adr_new>>16);
  if ( (uint8_t)(adr_new>>8)  !=  (uint8_t)(adr_current>>8)  ) DaliSendData(DALI_SEARCHADDRM, adr_new>>8);
  if ( (uint8_t)(adr_new)     !=  (uint8_t)(adr_current)     ) DaliSendData(DALI_SEARCHADDRL, adr_new);
}

/*-------------------------------------------------------------------------------------------*/

bool DaliCompare() {
  // Is the random address smaller or equal to the search address?
  // As more than one device can reply, the reply gets garbled
  uint8_t retry = 2;
  while (retry > 0) {
    // Compare is true if we received any activity on the bus as reply.
    // Sometimes the reply is not registered... so only accept retry times 'no reply' as a real false compare
    int rv = DaliSendWaitResponse(DALI_COMPARE, 0x00);
    if (rv == 0xFF) return true;               // Yes reply
    if (rv == -2) return true;                 // Reply but collision
    retry--;
  }
  return false;
}

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliFindAddress(void) {
  // Find addr with binary search
  uint32_t adr = 0x800000;
  uint32_t addsub = 0x400000;
  uint32_t adr_last = adr;
  DaliSetSearchAddress(adr);
  
  while (addsub) {
    DaliSetSearchAddressDifference(adr, adr_last);
    adr_last = adr;
    if (DaliCompare()) {                       // Returns true if searchadr > adr
      adr -= addsub;
    } else {
      adr += addsub;
    }
    addsub >>= 1;
  }
  DaliSetSearchAddressDifference(adr, adr_last);
  adr_last = adr;
  if (!DaliCompare()) {
    adr++;
    DaliSetSearchAddressDifference(adr, adr_last);
  }
  return adr;
}

/*-------------------------------------------------------------------------------------------*/

void DaliProgramShortAddress(uint8_t shortadr) {
  // The slave shall store the received 6-bit address (AAAAAA) as a short address if it is selected.
  DaliSendData(DALI_PROGRAM_SHORT_ADDRESS, (shortadr << 1) | 0x01);

  AddLog(LOG_LEVEL_INFO, PSTR("DLI: Set short address %d"), shortadr +1);
}

/*-------------------------------------------------------------------------------------------*/

uint32_t DaliCommission(uint8_t init_arg) {
  // Based on Shelly DALI Dimmer Gen3 received frames
  // init_arg=11111111 : all without short address
  // init_arg=00000000 : all 
  // init_arg=0AAAAAA1 : only for this shortadr
  // returns number of new short addresses assigned
  DaliSendData(0xFF, DALI_RESET);              // Turns ON all lights
  uint8_t arr[64];
  uint32_t sa;
  for (sa = 0; sa < 64; sa++) {
    arr[sa] = 0;
  }
  delay(450);
  DaliSendData(DALI_DATA_TRANSFER_REGISTER0, 0xFF);
  DaliSendData(0xFF, DALI_SET_SHORT_ADDRESS);
  DaliSendData(DALI_TERMINATE, 0x00);          // Terminate the DALI_INITIALISE command
  delay(15);
  // Start commissioning
  DaliSendData(DALI_INITIALISE, init_arg);
  DaliSendData(DALI_RANDOMISE, 0x00);
  delay(65);                                   // Need pause after RANDOMISE

  uint32_t cnt = 0;
  while (true) {                               // Find random addresses and assign unused short addresses
    uint32_t adr = DaliFindAddress();
    if (adr > 0xffffff) { break; }             // No more random addresses found -> exit
    for (sa = 0; sa < 64; sa++) {              // Find first unused short address
      if (0 == arr[sa]) { break; }
    }
    if (sa >= 64) { break; }                   // All 64 short addresses assigned -> exit
    arr[sa] = 1;                               // Mark short address as used
    cnt++;
 
    DaliProgramShortAddress(sa);               // Assign short address
    DaliSendData(DALI_WITHDRAW, 0x00);         // Remove the device from the search
    DaliSendData(sa << 1, DALI_OFF);           // Turns OFF latest short address light

    delay(1);
    OsWatchLoop();                             // Feed blocked-loop watchdog
  }

  delay(100);
  DaliSendData(DALI_TERMINATE, 0x00);          // Terminate the DALI_INITIALISE command

#ifdef USE_LIGHT
  DaliInitLight();
  uint32_t address = (Settings->sbflag1.dali_light) ? DaliTarget2Address() : DALI_BROADCAST_DP;  // DaliLight 1
  DaliSendData(address, Dali->power[0]);       // Restore lights
#else
  DaliSendData(DALI_BROADCAST_DP, Dali->power[0]);  // Restore lights
#endif  // USE_LIGHT
  return cnt;
}

/*********************************************************************************************/

void ResponseAppendDali(uint32_t index) {
  char number[12];
  uint8_t dimmer = changeUIntScale(Dali->dimmer[index], 0, 254, 0, 100);
  ResponseAppend_P(PSTR("\"DALI\":{\"Power%s\":\"%s\",\"Dimmer%s\":%d,\"Address\":%d,\"Command\":%d}"), 
    (0==index)?"":itoa(index+100, number, 10), GetStateText(Dali->power[index]),
    (0==index)?"":itoa(index+100, number, 10), dimmer,
    Dali->address, Dali->command);
}

void ResponseDali(uint32_t index) {
  Response_P(PSTR("{"));
  ResponseAppendDali(index);
  ResponseJsonEnd();
}

/*-------------------------------------------------------------------------------------------*/

void DaliLoop(void) {
  if (!Dali->available || Dali->response) { return; }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Rx 0x%05X"), Dali->received_dali_data);

  if (Dali->received_dali_data &0x00010000) { 
    Dali->available = false;
    return;                                    // Rx collision
  }

  Dali->address = Dali->received_dali_data >> 8;
  Dali->command = Dali->received_dali_data;
  uint32_t index = DaliSaveState(Dali->address, Dali->command);  // Update dimmer and power

  bool show_response = true;
#ifdef USE_LIGHT
  if (Dali->allow_light && (DaliTarget2Address() == Dali->address)) {
    if (Settings->sbflag1.dali_light) {        // DaliLight 1
      uint8_t dim_old = changeUIntScale(Dali->last_dimmer, 0, 254, 0, 100);
      uint8_t dim_new = changeUIntScale(Dali->dimmer[index], 0, 254, 0, 100);
      if (Dali->last_power != Dali->power[index]) {
        Dali->light_sync = true;               // Block local loop
        ExecuteCommandPower(LightDevice(), Dali->power[index], SRC_SWITCH);
      }
      else if (dim_old != dim_new) {
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), dim_new);
        Dali->light_sync = true;               // Block local loop
        ExecuteCommand(scmnd, SRC_SWITCH);
      }
      show_response = false;
    }
  }
#endif  // USE_LIGHT
  if (show_response) {
    ResponseDali(index);
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_PRFX_DALI));
  }

  Dali->available = false;
}

/*-------------------------------------------------------------------------------------------*/

void DaliEverySecond(void) {
  if (5 == TasmotaGlobal.uptime) {
    DaliInitLight();
  }
}

/*-------------------------------------------------------------------------------------------*/

#ifdef USE_LIGHT
bool DaliSetChannels(void) {
  if (Settings->sbflag1.dali_light) {          // DaliLight 1
    Settings->light_fade = 0;                  // Use Dali fading
    Settings->light_correction = 0;            // Use Dali light correction
    if (Dali->light_sync) {                    // Block local loop
      Dali->light_sync = false;
    } else {
      uint8_t value = ((uint8_t*)XdrvMailbox.data)[0];
      if (255 == value) { value = 254; }       // Max Dali value
      DaliSendData(DaliTarget2Address(), value);
    }
  }
  return true;
}
#endif  // USE_LIGHT

/*-------------------------------------------------------------------------------------------*/

bool DaliInit(uint32_t function) {
  int pin_tx = -1;
  bool invert_tx = false;
  if (PinUsed(GPIO_DALI_TX)) {
    pin_tx = Pin(GPIO_DALI_TX);
  }
  else if (PinUsed(GPIO_DALI_TX_INV)) {
    pin_tx = Pin(GPIO_DALI_TX_INV);
    invert_tx = true;
  }
  int pin_rx = -1;
  bool invert_rx = false;
  if (PinUsed(GPIO_DALI_RX)) {
    pin_rx = Pin(GPIO_DALI_RX);
  }
  else if (PinUsed(GPIO_DALI_RX_INV)) {
    pin_rx = Pin(GPIO_DALI_RX_INV);
    invert_rx = true;
  }
  if ((-1 == pin_tx) || (-1 == pin_rx)) { return false; }

  Dali = (DALI*)calloc(sizeof(DALI), 1);
  if (!Dali) { return false; }

  Dali->pin_tx = pin_tx;
  Dali->invert_tx = invert_tx;
  Dali->pin_rx = pin_rx;
  Dali->invert_rx = invert_rx;

  Dali->allow_light = (FUNC_MODULE_INIT == function);  // Light control is possible

  AddLog(LOG_LEVEL_INFO, PSTR("DLI: GPIO%d(RX%s) and GPIO%d(TX%s)%s"),
    Dali->pin_rx, (Dali->invert_rx)?"i":"", Dali->pin_tx, (Dali->invert_tx)?"i":"", (Dali->allow_light)?" as light":"");

  pinMode(Dali->pin_tx, OUTPUT);
  digitalWrite(Dali->pin_tx, (Dali->invert_tx) ? LOW : HIGH);  // Idle
  pinMode(Dali->pin_rx, INPUT);
#ifdef DALI_DEBUG
  pinMode(DALI_DEBUG_PIN, OUTPUT);
  digitalWrite(DALI_DEBUG_PIN, HIGH);
#endif  // DALI_DEBUG

  Dali->max_short_address = 64;
  for (uint32_t i = 0; i < DALI_MAX_STORED; i++) {
    Dali->dimmer[i] = DALI_INIT_STATE;
  }

  // Manchester twice 1200 bps = 2400 bps = 417 (protocol 416.76 +/- 10%) us
  Dali->bit_cycles = ESP.getCpuFreqMHz() * 1000000 / 2400;

  DaliEnableRxInterrupt();

  if (!Dali->allow_light) {
    Settings->sbflag1.dali_light = false;      // No light control possible
  }
#ifdef USE_LIGHT
  if (!Settings->sbflag1.dali_light) {         // DaliLight 0
    return false;
  }

  Settings->light_fade = 0;                    // Use Dali fading instead
  Settings->light_correction = 0;              // Use Dali light correction
  UpdateDevicesPresent(1);
  TasmotaGlobal.light_type = LT_SERIAL1;       // Single channel
  return true;
#else
  return false;
#endif  // USE_LIGHT
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool DaliJsonParse(void) {
  // {"addr":254,"cmd":100}
  // {"addr":2}
  // {"dim":3}

  bool served = false;
  JsonParser parser((char *)XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (root) {
    int DALIindex = 0;
    int ADRindex = 0;
    int8_t DALIdim = -1;
    uint32_t DALIaddr = DALI_BROADCAST_DP;

    JsonParserToken val = root[PSTR("cmd")];
    if (val) {
      uint32_t cmd = val.getUInt();
      val = root[PSTR("addr")];
      if (val) {
        uint32_t addr = val.getUInt();
        AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: cmd = %d, addr = %d"), cmd, addr);
        DaliSendData(addr, cmd);
        return true;
      } else {
        return false;
      }
    }
    val = root[PSTR("addr")];
    if (val) {
      uint32_t addr = val.getUInt();
      if ((addr >= 0) && (addr < 64)) {
        DALIaddr = addr  << 1;
      }
    }
    val = root[PSTR("dim")];
    if (val) {
      uint8_t dim = val.getUInt();
      if (dim < 255) {
        DALIdim = dim;
      }
    }
    DaliSendData(DALIaddr, DALIdim);
    served = true;
  }
  return served;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void CmndDali(void) {
  // Dali {"addr":254,"cmd":100} - Any address and/or command
  // Dali 0|1                    - Enable DALI receive probe
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
      Dali->probe = XdrvMailbox.payload;
      ResponseCmndNumber(Dali->probe);
      return;
    }
    DaliJsonParse();
  }
  ResponseDali(0);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliTarget(void) {
  // DaliTarget          - Set transmit target
  // DaliTarget 0        - Set target to broadcast address
  // DaliTarget 1..64    - Set target to short address
  // DaliTarget 101..116 - Set target to group address
  if (((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 64)) ||
      ((XdrvMailbox.payload >= 101) && (XdrvMailbox.payload <= 116)) ||
      (XdrvMailbox.payload == 0)) {
    Dali->target = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Dali->target);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliPower(void) {
  // DaliPower 0       - Broadcast power off
  // DaliPower 1       - Broadcast power on to last dimmer state
  // DaliPower 2       - Broadcast toggle power off or last dimmer state
  // DaliPower 3..254  - Broadcast equals DaliDimmer command
  // DaliPower 0..254  - Broadcast control
  // DaliPower0 0..254 - Broadcast control (= DaliPower)
  // DaliPower1 0..254 - Short address 0 control
  // DaliPower3 0..254 - Short address 2 control
  uint32_t index = 0;                          // Broadcast
  if ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116)) {
    index = XdrvMailbox.index - 100;           // Group1 to 16
  }
  if (((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= 64)) ||
      ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116))) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 254)) {
      if (XdrvMailbox.payload <= 2) {
        if (2 == XdrvMailbox.payload) {
          XdrvMailbox.payload = (Dali->power[index]) ? 0 : 1;
        }
        if (1 == XdrvMailbox.payload) {
          XdrvMailbox.payload = Dali->dimmer[index];
        }
      }
      uint32_t DALIaddr = DALI_BROADCAST_DP;
      if (XdrvMailbox.index >= 101) {
        DALIaddr = ((XdrvMailbox.index -101) << 1) | 0x80;  // Group address
      }
      else if ((XdrvMailbox.index > 0) && XdrvMailbox.usridx) {
        DALIaddr = (XdrvMailbox.index -1) << 1;  // Short address
      }
      DaliSendData(DALIaddr, XdrvMailbox.payload);
    }
  }
  ResponseDali(index);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliDimmer(void) {
  // DaliDimmer 0..100  - Broadcast set power off or dimmer state
  // DaliDimmer0 0..100 - Broadcast set power off or dimmer state
  // DaliDimmer1 0..100 - Short address 0 set power off or dimmer state
  // DaliDimmer3 0..100 - Short address 2 set power off or dimmer state
  uint32_t index = 0;                          // Broadcast
  if ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116)) {
    index = XdrvMailbox.index - 100;           // Group1 to 16
  }
  if (((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= 64)) ||
      ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116))) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      uint8_t dimmer = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 254);
      uint32_t DALIaddr = DALI_BROADCAST_DP;
      if (XdrvMailbox.index >= 101) {
        DALIaddr = ((XdrvMailbox.index -101) << 1) | 0x80;  // Group address
      }
      else if ((XdrvMailbox.index > 0) && XdrvMailbox.usridx) {
        DALIaddr = (XdrvMailbox.index -1) << 1;  // Short address
      }
      DaliSendData(DALIaddr, dimmer);
    }
  }
  ResponseDali(index);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliGroup(void) {
  // DaliGroup1 1,2   - Add device 1 and 2 to group 1
  // DaliGroup1 -1,2  - Remove device 1 and 2 to group 1
  if ((XdrvMailbox.index >= 1) && (XdrvMailbox.index <= 16)) {
    uint32_t group = XdrvMailbox.index -1;
    bool more = false;
    char temp[200] = { 0 };
    if (XdrvMailbox.data_len) {
      uint32_t command = DALI_ADD_TO_GROUP0;
      temp[0] = '+';
      if ('+' == XdrvMailbox.data[0]) {        // Add devices
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
      }
      else if ('-' == XdrvMailbox.data[0]) {   // Remove devices
        command = DALI_REMOVE_FROM_GROUP0;
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
        temp[0] = '-';
      }
      uint32_t argc = ArgC();                  // Number of devices
      if (argc) {
        command |= group;
        uint32_t sas[argc];
        ParseParameters(argc, sas);
        for (uint32_t arg = 0; arg < argc; arg++) {
          uint32_t sa = sas[arg] -1;
          if (sa < 64) {
            snprintf_P(temp, sizeof(temp), PSTR("%s%s%d"), temp, (more)?",":"", sa +1);
            more = true;
            DaliSendData(sa << 1 | 1, command);
          }
        }
        ResponseCmndIdxChar(temp);
      }
    } else {
      uint32_t command = DALI_QUERY_GROUPS_0_7;
      uint32_t bitmask = 1 << group;
      if (group > 7) {
        command = DALI_QUERY_GROUPS_8_15;
        bitmask = 1 << group - 8;
      }
      for (uint32_t sa = 0; sa < Dali->max_short_address; sa++) {   // Scanning 64 addresses takes about 2500 ms
        int result = DaliSendWaitResponse(sa << 1 | 1, command, 20);
        if ((result >= 0) && (result & bitmask)) {
          snprintf_P(temp, sizeof(temp), PSTR("%s%s%d"), temp, (more)?",":"", sa +1);
          more = true;
        }
      }
      if (!strlen(temp)) {
        snprintf_P(temp, sizeof(temp), PSTR("None"));
      }
      ResponseCmndIdxChar(temp);
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliGear(void) {
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 64)) {
    Dali->max_short_address = XdrvMailbox.payload;
  }
  uint32_t count = DaliGearPresent();
  ResponseCmnd();
  ResponseAppend_P(PSTR("%d,\"Present\":%d}"), Dali->max_short_address, count);
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliSend(void) {
  // Send command
  // Setting bit 8 will repeat command once
  // DaliSend 0x1a5,255  - DALI Initialise (send twice)
  // DaliSend 0x01,0xa3,0x2d,254 - Set Power On level
  uint32_t values[4] = { 0 };
  uint32_t params = ParseParameters(4, values);
  if (2 == params) {
    DaliSendData(values[0] &0x1FF, values[1] &0xFF);
    ResponseCmndDone();
  }
  else if (4 == params) {
    if (DaliSetValue(values[0] &0x1FF, values[1] &0xFF, values[2] &0xFF, values[3] &0xFF)) {
      ResponseCmndDone();
    } else {
      ResponseCmndFailed();
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliQuery(void) {
  // Send command and return response or -1 (no response within DALI_TIMEOUT)
  // Setting bit 8 will repeat command once
  // DaliQuery 0xff,0x90  - DALI Query status
  // DaliQuery 0xff,144   - DALI Query status
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  if (2 == params) {
    int result = DaliSendWaitResponse(values[0] &0x1FF, values[1] &0xFF);
    ResponseCmndNumber(result);
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliScan(void) {
  // Scan short addresses
  // DaliScan 1     - Reset and commission short addresses
  // DaliScan 2     - Commission unassigned short addresses
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 2)) {
    uint32_t init_arg = 0x00;                  // Commission all
    if (2 == XdrvMailbox.payload) {
      init_arg = 0xFF;                         // Commission all without short addresses
    }
    int result = DaliCommission(init_arg);
    ResponseCmndNumber(result);
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndDaliGroupSliders(void) {
  // DaliGroupSliders 0..16  - Add group sliders
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 16)) {
    Settings->mbflag2.dali_group_sliders = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;            // Restart to update GUI
  }
  ResponseCmndNumber(Settings->mbflag2.dali_group_sliders);
}

#ifdef USE_LIGHT
/*-------------------------------------------------------------------------------------------*/

void CmndDaliLight(void) {
  // DaliLight 0  - Disable light controls
  // DaliLight 1  - Enable light controls
  if (Dali->allow_light && (XdrvMailbox.data_len > 0)) {
    Settings->sbflag1.dali_light = XdrvMailbox.payload &1;  // DaliLight 0/1
    TasmotaGlobal.restart_flag = 2;            // Restart to update GUI
  }
  ResponseCmndStateText(Settings->sbflag1.dali_light);  // DaliLight 0/1
}
#endif  // USE_LIGHT

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_MSG_SLIDER_DALI[] PROGMEM =
  "<tr>"
  "<td style='width:15%%'><button id='k75%d' style='background:#%06x;' onclick='la(\"&k75=%d\");'>%s%s</button></td>"
  "<td style='width:85%%'><div class='r' style='background-image:linear-gradient(to right,#000,#FFF);'>"
  "<input id='i75%d' type='range' min='1' max='100' value='%d' onchange='lc(\"i\",75%d,value)'></div></td>"
  "</tr>";

void DaliWebAddMainSlider(void) {
  WSContentSend_P(HTTP_TABLE100);
  char number[12];
  for (uint32_t i = Settings->sbflag1.dali_light; i <= Settings->mbflag2.dali_group_sliders; i++) {  // DaliLight 0/1, DaliGroupSliders
    Dali->web_dimmer[i] = Dali->dimmer[i];
    WSContentSend_P(HTTP_MSG_SLIDER_DALI,      // Brightness - Black to White
      i,                                       // k75<i>
      WebColor((Dali->power[i]) ? COL_BUTTON : COL_BUTTON_OFF),
      i,                                       // k75=<i>
      (0==i)?"B":"G",                          // B (Broadcast) or G1 to G16 (Group)
      (0==i)?"":itoa(i, number, 10),
      i,                                       // i75<i>
      changeUIntScale(Dali->web_dimmer[i], 0, 254, 0, 100),
      i                                        // i75<i>
    );
  }
  WSContentSend_P(PSTR("</table>"));
}

/*********************************************************************************************/

void DaliWebGetArg(void) {
  char tmp[8];                                 // WebGetArg numbers only
  char svalue[32];                             // Command and number parameter
  char webindex[8];                            // WebGetArg name

  uint32_t index;
  for (uint32_t i = Settings->sbflag1.dali_light; i <= Settings->mbflag2.dali_group_sliders; i++) {  // DaliLight 0/1, DaliGroupSliders
    snprintf_P(webindex, sizeof(webindex), PSTR("i75%d"), i);
    WebGetArg(webindex, tmp, sizeof(tmp));     // 0 - 100 percent
    if (strlen(tmp)) {
      index = i;
      if (index > 0) { index += 100; }         // Group
      snprintf_P(svalue, sizeof(svalue), PSTR("DaliDimmer%d %s"), index, tmp);
      ExecuteWebCommand(svalue);
    }
  }
  WebGetArg(PSTR("k75"), tmp, sizeof(tmp));
  if (strlen(tmp)) {
    index = atoi(tmp);
    if (index > 0) { index += 100; }           // Group
    snprintf_P(svalue, sizeof(svalue), PSTR("DaliPower%d 2"), index);
    ExecuteWebCommand(svalue);
  }
}

/*********************************************************************************************/

void DaliWebShow(void) {
  WSContentSend_P(PSTR("</table>"));         // Terminate current {t}
  WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);  // "<img style='display:none;' src onerror=\""
  for (uint32_t i = Settings->sbflag1.dali_light; i <= Settings->mbflag2.dali_group_sliders; i++) {  // DaliLight 0/1, DaliGroupSliders
    WSContentSend_P(PSTR("eb('k75%d').style='background:#%06x';"),
      i, WebColor((Dali->power[i]) ? COL_BUTTON : COL_BUTTON_OFF));
    if (Dali->dimmer[i] != Dali->web_dimmer[i]) {
      if (WebUpdateSliderTime()) {
        Dali->web_dimmer[i] = Dali->dimmer[i];
      }
      WSContentSend_P(PSTR("eb('i75%d').value='%d';"),
        i, changeUIntScale(Dali->dimmer[i], 0, 254, 0, 100));
    }
  }
  WSContentSend_P(PSTR("\">{t}"));           // Restart {t} = <table style='width:100%'>
  WSContentSeparator(3);                     // Don't print separator on next WSContentSeparator(1)
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv75(uint32_t function) {
  bool result = false;

  if (FUNC_MODULE_INIT == function) {          // Try to claim DALI as light
    result = DaliInit(function);
  }
  else if ((FUNC_PRE_INIT == function) && !Dali) {  // If claim failed then use DALI controls only
    DaliInit(function);
  }
  else if (Dali) {
    switch (function) {
      case FUNC_LOOP:
        DaliLoop();
        break;
      case FUNC_EVERY_SECOND:
        DaliEverySecond();
        break;
#ifdef USE_LIGHT
      case FUNC_SET_CHANNELS:
        result = DaliSetChannels();
        break;
#endif  // USE_LIGHT
      case FUNC_JSON_APPEND:
        ResponseAppend_P(PSTR(","));
        ResponseAppendDali(0);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        DaliWebShow();
        break;
      case FUNC_WEB_ADD_MAIN_BUTTON:
        DaliWebAddMainSlider();
        break;
      case FUNC_WEB_GET_ARG:
        DaliWebGetArg();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kDALICommands, DALICommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_DALI
