/*
  Dali.h - DALI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifndef _DALI_H_
#define _DALI_H_

/*-------------------------------------------------------------------------------------------*\
 * DALI Address types - Send as first byte
\*-------------------------------------------------------------------------------------------*/

// Address types - Send as first byte
#define DALI_SHORT_ADDRESS0                        0x00  // 0b00000000   0 - First short address
#define DALI_SHORT_ADDRESS1                        0x02  // 0b00000010   1 - Next short address
                                                         // ...
#define DALI_SHORT_ADDRESS63                       0x7E  // 0b01111110  63 - Last short address
#define DALI_GROUP_ADDRESS0                        0x80  // 0b10000000   0 - First group address
#define DALI_GROUP_ADDRESS1                        0x82  // 0b10000010   1 - Next group address
                                                         // ...
#define DALI_GROUP_ADDRESS15                       0x9E  // 0b10011110  15 - Last group address
#define DALI_BROADCAST_DP                          0xFE  // 0b11111110 254 - Broadcast address

// Address selector bit - Send with first byte
#define DALI_SELECTOR_BIT                          0x01  // Mark second byte as standard/extended command

/*-------------------------------------------------------------------------------------------*\
 * DALI Commands for IEC62386 part 102 = Control gears - Send as first byte
\*-------------------------------------------------------------------------------------------*/

// Special commands - Send as first byte
#define DALI_102_TERMINATE                         0xA1  // 256 - All special mode processes (like DALI_102_INITIALISE) shall be terminated.
#define DALI_102_SET_DTR0                          0xA3  // 257 - Stores the data XXXX XXXX to the Data Transfer Register (Data Transfer Register 0).
                                                         //       This is a broadcast command to set the value of the DTR0 register.
#define DALI_102_INITIALISE                        0xA5  // 258 REPEAT - Sets the ballast to the INITIALISE status for 15 minutes. Commands 167 to 189 are enabled only for a ballast in this status.
                                                         //       This command shall start or re-trigger a timer for 15 minutes; the addressing commands 167..189 shall only be processed within this period.
                                                         //       All other commands shall still be processed during this period.
                                                         //       This time period shall be aborted with the "DALI_102_TERMINATE" command.
                                                         //       Ballasts shall react as follows:
                                                         //       - if broadcast is True then all ballasts shall react
                                                         //       - if broadcast is False and address is None then ballasts without a short address shall react
                                                         //       - if broadcast is False and address is an integer 0..63 then ballasts with the address supplied shall react
#define DALI_102_RANDOMISE                         0xA7  // 259 REPEAT - Generates a random address.
                                                         //       The ballast shall generate a new 24-bit random address. The new random address shall be available within a time period of 100ms.
#define DALI_102_COMPARE                           0xA9  // 260 - Is the random address smaller or equal to the search address?
                                                         //       The ballast shall compare its 24-bit random address with the combined search address stored in SearchAddrH, SearchAddrM and SearchAddrL.
                                                         //       If its random address is smaller or equal to the search address and the ballast is not withdrawn then the ballast shall generate a query "YES".
#define DALI_102_WITHDRAW                          0xAB  // 261 - Excludes ballasts for which the random address and search address match from the Compare process.
                                                         //       The ballast that has a 24-bit random address equal to the combined search address stored in SearchAddrH, SearchAddrM and SearchAddrL
                                                         //        shall no longer respond to the compare command.
                                                         //       This ballast shall not be excluded from the initialisation process.
#define DALI_102_PING                              0xAD  // 262 - DALI-2 Ignores in the ballast.
                                                         //       Transmitted at 10 minute intervals by single master application controllers (that cannot perform collision detection) to indicate their presence.
                                                         //        Ignored by control gear.
#define DALI_102_RESERVED263                       0xAF  // 263 - [Reserved]
#define DALI_102_SEARCHADDRH                       0xB1  // 264 - Specifies the higher 8 bits of the search address.
#define DALI_102_SEARCHADDRM                       0xB3  // 265 - Specifies the middle 8 bits of the search address.
#define DALI_102_SEARCHADDRL                       0xB5  // 266 - Specifies the lower 8 bits of the search address.
#define DALI_102_PROGRAM_SHORT_ADDRESS             0xB7  // 267 - The ballast shall store the received 6-bit address (AAA AAA) as a short address if it is selected.
                                                         //       It is selected if:
                                                         //       - the ballast's 24-bit random address is equal to the address in SearchAddrH, SearchAddrM and SearchAddrL
                                                         //       - physical selection has been detected (the lamp is electrically disconnected after reception of command PhysicalSelection())
                                                         //       If address is 255 ("MASK") then the short address shall be deleted.
#define DALI_102_VERIFY_SHORT_ADDRESS              0xB9  // 268 - Is the short address AAA AAA?
                                                         //       The ballast shall give an answer 255 ("YES") if the received short address is equal to its own short address.
#define DALI_102_QUERY_SHORT_ADDRESS               0xBB  // 269 - What is the short address of the ballast being selected?
                                                         //       The ballast shall send the short address if the random address is the same as the search address or the ballast is physically selected.
                                                         //       The answer will be in the format (address<<1)|1 if the short address is programmed, or "MASK" (0xff) if there is no short address stored.
#define DALI_102_PHYSICAL_SELECTION                0xBD  // 270 - not DALI-2 Sets the ballast to Physical Selection Mode and excludes the ballast from the Compare process. (Excluding IEC62386-102ed2.0)
#define DALI_102_RESERVED191                       0xBF  // 271 - [Reserved]

// Extending special commands - Send as first byte
#define DALI_102_ENABLE_DEVICE_TYPE_X              0xC1  // 272 - Adds the device XXXX (a special device).
                                                         //       This command shall be sent before an application extended command.
                                                         //       This command can be processed without the use of the Initialise() command.
                                                         //       This command shall not be used for device type 0.
#define DALI_102_SET_DTR1                          0xC3  // 273 - Stores data XXXX into Data Transfer Register 1.
                                                         //       This is a broadcast command to set the value of the DTR1 register.
#define DALI_102_SET_DTR2                          0xC5  // 274 - Stores data XXXX into Data Transfer Register 2.
                                                         //       This is a broadcast command to set the value of the DTR2 register.
#define DALI_102_WRITE_MEMORY_LOCATION             0xC7  // 275 - Write data into the specified address of the specified memory bank. (There is BW) (DTR(DTR0)：address, DTR1：memory bank number)
                                                         //       This instruction will be ignored if the addressed memory bank is not implemented, or writeEnableState is DISABLED.
                                                         //       If the instruction is executed then the control gear will write data into the memory location identified by DTR0 in bank DTR1 and return data as an answer.
                                                         //       If the location is not implemented, above the last accessible location, locked or not writeable, the answer will be NO (-1).
                                                         //       If the location addressed is below 0xff, then DTR0 will be incremented by 1.
#define DALI_102_WRITE_MEMORY_LOCATION_NO_REPLY    0xC9  // 276 - DALI-2 Write data into the specified address of the specified memory bank. (There is no BW) (DTR(DTR0)：address, TR1：memory bank number)
                                                         //       This instruction will be ignored if the addressed memory bank is not implemented, or writeEnableState is DISABLED.
                                                         //       If the instruction is executed then the control gear will write data into the memory location identified by DTR0 in bank DTR1
                                                         //       If the location is not implemented, above the last accessible location, locked or not writeable, the answer will be NO (-1).
                                                         //       If the location addressed is below 0xff, then DTR0 will be incremented by 1.
#define DALI_102_RESERVED277                       0xCB  // 277 - [Reserved]
                                                         // ...
#define DALI_102_RESERVED349                       0xFD  // 349 - [Reserved]

/*-------------------------------------------------------------------------------------------*\
 * DALI Commands for IEC62386 part 102 = Control gears - Send as second byte
\*-------------------------------------------------------------------------------------------*/

// Control commands - Send as second byte without repeat
#define DALI_102_OFF                               0x00  //   0 - Turns off lighting (without fade).
#define DALI_102_UP                                0x01  //   1 - Increases the lighting control level for 200 ms according to the Fade rate.
                                                         //       No change if the arc power output is already at the "MAX LEVEL".
                                                         //       If this command is received again while it is being executed, the execution time shall be re-triggered.
                                                         //       This command shall only affect ballasts with burning lamps.
                                                         //       No lamp shall be ignited with this command.
#define DALI_102_DOWN                              0x02  //   2 - Decreases the lighting control level for 200 ms according to the Fade rate.
                                                         //       No change if the arc power output is already at the "MIN LEVEL".
                                                         //       If this command is received again while it is being executed, the execution time shall be re-triggered.
                                                         //       Lamp shall not be switched off via this command.
#define DALI_102_STEP_UP                           0x03  //   3 - Increments the lighting control level (without fade).
                                                         //       No change if the arc power output is already at the "MAX LEVEL".
                                                         //       This command shall only affect ballasts with burning lamps. No lamp shall be ignited with this command.
#define DALI_102_STEP_DOWN                         0x04  //   4 - Decrements the lighting control level (without fade).
                                                         //       No change if the arc power output is already at the "MIN LEVEL".
                                                         //       Lamps shall not be switched off via this command.
#define DALI_102_RECALL_MAX_LEVEL                  0x05  //   5 - Maximizes the lighting control level to "MAX LEVEL" (without fade).
                                                         //       If the lamp is off it shall be ignited with this command.
#define DALI_102_RECALL_MIN_LEVEL                  0x06  //   6 - Minimizes the lighting control level to "MIN LEVEL" (without fade)
                                                         //       If the lamp is off it shall be ignited with this command.
#define DALI_102_STEP_DOWN_AND_OFF                 0x07  //   7 - Decrements the lighting control level and turns off lighting if the level is at the minimum (without fade).
#define DALI_102_ON_AND_STEP_UP                    0x08  //   8 - Increments the lighting control level and turns on lighting if lighting is off (with fade). 
#define DALI_102_DIRECT_ARC_POWER_CONTROL          0x09  //   9 Deprecated - Enable DAPC Sequence
                                                         //       Indicates the start of a command iteration of DAPC(level) commands.
                                                         //       The control gear shall temporarily use a fade time of 200ms while the command iteration is active independent of the actual fade/extended fade time.
                                                         //       The DAPC sequence shall end if 200ms elapse without the control gear receiving a DAPC(level) command.
                                                         //       The sequence shall be aborted on reception of an indirect arc power control command.
                                                         //       value 0 - Off
                                                         //       value minLevel...maxLevel
                                                         //       value 255 - Retain current level
#define DALI_102_GO_TO_LAST_ACTIVE_LEVEL           0x0A  //  10 - DALI-2 Adjusts the lighting control level to the last light control level according to the Fade time.
#define DALI_102_CONTINUOUS_UP                     0x0B  //  11 - DALI-2 Dim up using the set fade rate.
                                                         //       TargetLevel shall be set to maxLevel and a fade shall be started using the set fade rate. The fade shall stop when maxLevel is reached.
#define DALI_102_CONTINUOUS_DOWN                   0x0C  //  12 - DALI-2 Dim down using the set fade rate.
                                                         //       targetLevel shall be set to minLevel and a fade shall be started using the set fade rate. The fade shall stop when minLevel is reached.
#define DALI_102_RESERVED13                        0x0D  //  13 - [Reserved]
#define DALI_102_RESERVED14                        0x0E  //  14 - [Reserved]
#define DALI_102_RESERVED15                        0x0F  //  15 - [Reserved]
#define DALI_102_GO_TO_SCENE0                      0x10  //  16 - Adjusts the lighting control level for Scene XXXX according to the fade time.
                                                         //       If the ballast does not belong to this scene, the arc power level remains unchanged.
                                                         //       If the lamp is off, it shall be ignited with this command.
                                                         //       If the value stored for this scene is zero and the lamp is lit then the lamp shall be switched off by this command after the fade time.
                                                         // ...
#define DALI_102_GO_TO_SCENE15                     0x1F  //  31 - Adjusts the lighting control level for Scene XXXX according to the fade time.

// Configuration commands - Send as second byte with repeat
#define DALI_102_RESET                             0x20  //  32 - The variables in the persistent memory shall be changed to their reset values.
                                                         //       It is not guaranteed that any commands will be received properly within the next 300ms by a ballast acting on this command.
#define DALI_102_STORE_ACTUAL_LEVEL_IN_DTR0        0x21  //  33 - Store actual arc power level in the DTR (DTR0) without changing the current light intensity.
#define DALI_102_SAVE_PERSISTENT_VARIABLES         0x22  //  34 - DALI-2 Saves a variable in nonvolatile memory (NVM).
                                                         //       All variables identified as non-volatile shall be stored to non-volatile memory.
                                                         //       The control gear might not react to commands for up to 300ms after reception of this command.
                                                         //       This command is recommended to be used typically after commissioning.
#define DALI_102_SET_OPERATING_MODE                0x23  //  35 - DALI-2 Set operatingMode to DTR0.
                                                         //       If DTR0 does not correspond to an implemented operating mode, the command shall be ignored.
#define DALI_102_RESET_MEMORY_BANK                 0x24  //  36 - DALI-2 Reset Memory Bank according to DTR0
                                                         //       If DTR0 = 0 then all implemented and unlocked memory banks except memory bank 0 shall be reset.
                                                         //       In all other cases, the memory bank identified by DTR0 will be reset provided it is implemented and unlocked.
                                                         //       This command may cause control gear to react improperly to commands for up to 10s.
#define DALI_102_IDENTIFY_DEVICE                   0x25  //  37 - DALI-2 Starts the identification state of the device. Start or restart a 10s timer.
                                                         //       While the timer is running the device will run a procedure to enable an observer to distinguish the device from other devices in which it is not running.
                                                         //       This procedure is manufacturer-dependent.
                                                         //       Identification will be stopped immediately upon reception of any command other than Initialise, RecallMinLevel, RecallMaxLevel or IdentifyDevice.
#define DALI_102_RESERVED38                        0x26  //  38 - [Reserved]
                                                         // ...
#define DALI_102_RESERVED41                        0x29  //  41 - [Reserved]
#define DALI_102_SET_MAX_LEVEL                     0x2A  //  42 - Specifies the DTR data as the maximum lighting control level (maxLevel or "MAX LEVEL").
#define DALI_102_SET_MIN_LEVEL                     0x2B  //  43 - Specifies the DTR data as the minimum lighting control level (minLevel or "MIN LEVEL").
                                                         //       If this value is lower than the "PHYSICAL MIN LEVEL" of the ballast, then store the "PHYSICAL MIN LEVEL" as the new "MIN LEVEL".
#define DALI_102_SET_SYSTEM_FAILURE_LEVEL          0x2C  //  44 - Specifies the DTR data as the "SYSTEM FAILURE LEVEL".
#define DALI_102_SET_POWER_ON_LEVEL                0x2D  //  45 - Specifies the DTR data as the "POWER ON LEVEL".
#define DALI_102_SET_FADE_TIME                     0x2E  //  46 - Specifies the DTR data as the "FADE TIME" in seconds.
                                                         //       Formula T=0.5(sqrt(pow(2,DTR))) seconds 
                                                         //       With DTR0 in the range 1..15. If DTR0 is 0 then the extended fade time will be used.
                                                         //       The fade time specifies the time for changing the arc power level from the actual level to the requested level.
                                                         //       In the case of lamp off, the preheat and ignition time is not included in the fade time.
                                                         //       The new fade time will be used after the reception of the next arc power command.
                                                         //       If a new fade time is set during a running fade process, the running fade process is not affected.
                                                         //       value 0 - < 0.707 s
                                                         //       value 1 - 0.707 s
                                                         //       value 2 - 1.000 s
                                                         //       value 3 - 1.414 s
                                                         //       value 4 - 2.000 s
                                                         //       value 5 - 2.828 s
                                                         //       value 6 - 4.000 s
                                                         //       value 7 - 5.657 s
                                                         //       value 8 - 8.000 s
                                                         //       value 9 - 11.314 s
                                                         //       value 10 - 16.000 s
                                                         //       value 11 - 22.627 s
                                                         //       value 12 - 32.000 s
                                                         //       value 13 - 45.255 s
                                                         //       value 14 - 64.000 s
                                                         //       value 15 - 90.510 s
#define DALI_102_SET_FADE_RATE                     0x2F  //  47 - Specifies the DTR data as the "FADE RATE" in steps per second.
                                                         //       Formula F = 506/(sqrt(pow(2,DTR))) steps/s
                                                         //       With DTR in the range 1..15.
                                                         //       The new fade time will be used after the reception of the next arc power command.
                                                         //       If a new fade time is set during a running fade process, the running fade process is not affected.
                                                         //       value 0 - not permitted
                                                         //       value 1 - 357.796 steps/s
                                                         //       value 2 - 253.000 steps/s
                                                         //       value 3 - 178.898 steps/s
                                                         //       value 4 - 126.500 steps/s
                                                         //       value 5 - 89.449 steps/s
                                                         //       value 6 - 63.250 steps/s
                                                         //       value 7 - 44.725 steps/s
                                                         //       value 8 - 31.625 steps/s
                                                         //       value 9 - 22.362 steps/s
                                                         //       value 10 - 15.813 steps/s
                                                         //       value 11 - 11.181 steps/s
                                                         //       value 12 - 7.906 steps/s
                                                         //       value 13 - 5.591 steps/s
                                                         //       value 14 - 3.953 steps/s
                                                         //       value 15 - 2.795 steps/s
#define DALI_102_SET_EXTENDED_FADE_TIME            0x30  //  48 - DALI-2 Specifies the DTR data as the Extended Fade Time.
                                                         //       If DTR0 > 0x4f then extendedFadeTimeBase and extendedFadeTimeMultiplier are both set to 0.
                                                         //       Otherwise, extendedFadeTimeBase will be set to DTR0[3:0] and extendedFadeTimeMultiplier will be set to DTR0[6:4].
                                                         //       If a new fade time is set during a running fade process, the running fade process is not affected.
                                                         //       bit 0..3 - value 0..15 (E_DALIExtendedFadeTimeBase)
                                                         //       bit 4..7 - 0 = Disabled (E_DALIExtendeFadeTimeMultiplier)
                                                         //                  1 = Multiplier100ms
                                                         //                  2 = Multiplier1s
                                                         //                  3 = Multiplier10s
                                                         //                  4 = Multiplier1min
#define DALI_102_RESERVED49                        0x31  //  49 - [Reserved]
                                                         // ...
#define DALI_102_RESERVED63                        0x3F  //  63 - [Reserved]
#define DALI_102_SET_SCENE0                        0x40  //  64 - Specifies the DTR data as Scene XXXX.
                                                         //       The value 255 ("MASK") removes the ballast from the scene.
                                                         // ...
#define DALI_102_SET_SCENE15                       0x4F  //  79 - Specifies the DTR data as Scene XXXX.
#define DALI_102_REMOVE_FROM_SCENE0                0x50  //  80 - Deletes the Scene XXXX setting.
                                                         //       This stores 255 ("MASK") in the specified scene register.
                                                         // ...
#define DALI_102_REMOVE_FROM_SCENE15               0x5F  //  95 - Deletes the Scene XXXX setting.
#define DALI_102_ADD_TO_GROUP0                     0x60  //  96 - Adds the ballast to Group XXXX.
                                                         // ...
#define DALI_102_ADD_TO_GROUP15                    0x6F  // 111 - Adds the ballast to Group XXXX.
#define DALI_102_REMOVE_FROM_GROUP0                0x70  // 112 - Deletes the ballast from Group XXXX.
                                                         // ...
#define DALI_102_REMOVE_FROM_GROUP15               0x7F  // 127 - Deletes the ballast from Group XXXX.
#define DALI_102_SET_SHORT_ADDRESS                 0x80  // 128 - Specifies the DTR data as a Short Address.
                                                         //       The DTR must contain either:
                                                         //       - (address<<1)|1 (i.e. 0AAAAAA1) to set a short address
                                                         //       - 255 (i.e. 11111111) to remove the short address
#define DALI_102_ENABLE_WRITE_MEMORY               0x81  // 129 - Allows writing of the memory bank.
                                                         //       writeEnableState shall be set to ENABLED.
                                                         //       NB there is no command to explicitly disable memory write access; any command that is not directly involved with writing to
                                                         //        memory banks will set writeEnableState to DISABLED.
                                                         //       The commands that do not set writeEnableState to DISABLED are:
                                                         //       - WriteMemoryLocation
                                                         //       - WriteMemoryLocationNoReply
                                                         //       - DTR0
                                                         //       - DTR1
                                                         //       - DTR2
                                                         //       - QueryContentDTR0
                                                         //       - QueryContentDTR1
                                                         //       - QueryContentDTR2
#define DALI_102_RESERVED130                       0x82  // 130 - [Reserved]
                                                         // ...
#define DALI_102_RESERVED143                       0x8F  // 143 - [Reserved]

// Query commands - Send as second byte
#define DALI_102_QUERY_STATUS                      0x90  // 144 - Returns "STATUS INFORMATION"
                                                         //       bit 0 - Status of the control gear. 0: OK.
                                                         //       bit 1 - Lamp failure. 0: OK.
                                                         //       bit 2 - Lamp power on. 0: OFF.
                                                         //       bit 3 - Limit value error. 0: the most recently requested lamp power was either between MIN LEVEL and MAX LEVEL or was OFF.
                                                         //       bit 4 - Fading completed: 0: fading finished. 1: fading active.
                                                         //       bit 5 - Reset status. 0: no.
                                                         //       bit 6 - Short address missing. 0: no.
                                                         //       bit 7 - Power supply fault. 0: No. A reset or a lamp power control command has been received since the last switch-on.
#define DALI_102_QUERY_CONTROL_GEAR_PRESENT        0x91  // 145 - Returns bit0 - Is there a ballast that can communicate?
#define DALI_102_QUERY_LAMP_FAILURE                0x92  // 146 - Returns bit1 - Is there a lamp problem?
#define DALI_102_QUERY_LAMP_POWER_ON               0x93  // 147 - Returns bit2 - Is a lamp on?
#define DALI_102_QUERY_LIMIT_ERROR                 0x94  // 148 - Returns bit3 - Is the specified lighting control level out of the range from the minimum to the maximum values?
#define DALI_102_QUERY_RESET_STATE                 0x95  // 149 - Returns bit5 - Is the ballast in 'RESET STATE'?
#define DALI_102_QUERY_MISSING_SHORT_ADDRESS       0x96  // 150 - Returns bit6 - Does the ballast not have a short address?
#define DALI_102_QUERY_VERSION_NUMBER              0x97  // 151 - What is the corresponding IEC standard number?
                                                         //       The high four bits of the answer represent the version number of the standard.
                                                         //       IEC-60929 is version number 0; the 2009 version of IEC-62386 is version number 1.
                                                         //       As of the 2014 version of IEC-62386, the answer shall be the content of memory bank 0 location 0x16.
                                                         //       bit 0..3 - nMinorVersion
                                                         //       bit 4..7 - nMajorVersion
#define DALI_102_QUERY_CONTENT_DTR0                0x98  // 152 - What is the DTR content?
#define DALI_102_QUERY_DEVICE_TYPE                 0x99  // 153 - What is the device type?
                                                         //       The device type affects which application extended commands the device will respond to.
                                                         //       XXX this is updated for IEC 62386-102 and interacts with QueryNextDeviceType. In this case:
                                                         //       - If the device does not implement any part 2xx device type then the response will be 254;
                                                         //       - If the device implements one part 2xx device type then the response will be the device type number;
                                                         //       - If the device implements multiple part 2xx device types then the response will be MASK (0xff).
                                                         //       value 0 - Part 201: Standard device
                                                         //       value 1 - Part 202: Device for emergency lighting.
                                                         //       value 2 - Part 203: Device for discharge lamps.
                                                         //       value 3 - Part 204: Device for low-voltage halogen lamps.
                                                         //       value 4 - Part 205: Device for dimming incandescent lamps.
                                                         //       value 5 - Part 206: Device for converting digital signals into DC signals.
                                                         //       value 6 - Part 207: Device for light emitting diodes (LEDs).
                                                         //       value 7 - Part 208: Device for switching functions.
                                                         //       value 8 - Part 209: Device for color/color temperature control.
                                                         //       value 9 - Part 210: Sequencer
                                                         //       value 15 - Part 216: Load referencing
                                                         //       value 16 - Part 217: Thermal gear protection
                                                         //       value 17 - Part 218: Dimming curve selection
                                                         //       value 19 - Part 220: Centrally supplied emergency operation
                                                         //       value 20 - Part 221: Load shedding
                                                         //       value 21 - Part 222: Thermal lamp protection
                                                         //       value 23 - Part 224: Non-replaceable light sources
                                                         //       value 49 - Part 250: Devices with integrated DALI bus power supply
                                                         //       value 50 - Part 251: Further information and parameters for DALI control gears in memory bank 1
                                                         //       value 51 - Part 252: Further parameters for the creation of an energy report
                                                         //       value 52 - Part 253: Further parameters with diagnostic and maintenance information for DALI control gears
                                                         //       value 53 - Part 254: Extended information for DALI control gears for emergency lighting
                                                         //       value 254 - None or end
                                                         //       value 255 - Multiple
#define DALI_102_QUERY_PHYSICAL_MINIMUM_LEVEL      0x9A  // 154 - Return the physical minimum level for this device.
#define DALI_102_QUERY_POWER_FAILURE               0x9B  // 155 - Ask whether the device has not received a "RESET" or arc power control command since the last power-on.
#define DALI_102_QUERY_CONTENT_DTR1                0x9C  // 156 - Return the contents of DTR1.
#define DALI_102_QUERY_CONTENT_DTR2                0x9D  // 157 - Return the contents of DTR2.
#define DALI_102_QUERY_OPERATING_MODE              0x9E  // 158 - DALI-2 What is the Operating Mode?
#define DALI_102_QUERY_LIGHT_SOURCE_TYPE           0x9F  // 159 - DALI-2 What is the Light source type (E_DALILightSourceType)
                                                         //              "unknown" will typically be used in case of signal conversion, for example to 1-10v dimming
                                                         //              "none" will be used where no light source is connected, for example a relay
                                                         //              When the response is "multiple" then the light source types shall be placed into DTR0, DTR1 and DTR2.
                                                         //              If there are exactly two light source types, DTR2 shall be "none".  If there are more than three then DTR2 shall be MASK.
                                                         //       value 0 = LowPressureFluorescent
                                                         //       value 2 = HID
                                                         //       value 3 = LowVoltageHalogen
                                                         //       value 4 = Incandescent
                                                         //       value 6 = LED
                                                         //       value 7 = OLED
                                                         //       value 252 = Other
                                                         //       value 253 = Unknown (typicaly signal conversion like 1-10V dimming)
                                                         //       value 254 = NoLightSource
                                                         //       value 255 = Multiple (Type in DTR0, DTR1 and DTR2 (= none if two types, = 255 if > 3))
#define DALI_102_QUERY_ACTUAL_LEVEL                0xA0  // 160 - What is the "ACTUAL LEVEL" (the current lighting control level)?
                                                         //       During preheating and if a lamp error occurs the answer will be 0xff ("MASK").
#define DALI_102_QUERY_MAX_LEVEL                   0xA1  // 161 - What is the maximum lighting "MAX LEVEL" control level?
#define DALI_102_QUERY_MIN_LEVEL                   0xA2  // 162 - What is the minimum lighting "MIN LEVEL" control level?
#define DALI_102_QUERY_POWER_ON_LEVEL              0xA3  // 163 - What is the "POWER ON LEVEL" (the lighting control level when the power is turned on)?
#define DALI_102_QUERY_SYSTEM_FAILURE_LEVEL        0xA4  // 164 - What is the "SYSTEM FAILURE LEVEL" (the lighting control level when a failure occurs)?
#define DALI_102_QUERY_FADE_TIME_FADE_RATE         0xA5  // 165 - What are the Fade time and Fade rate? (see 47 and 48)
                                                         //       The fade time set by "DALI_102_SET_FADE_TIME" is in the upper four bits of the response.
                                                         //       The fade rate set by "DALI_102_SET_FADE_RATE" is in the lower four bits of the response.
#define DALI_102_QUERY_MANUFACTURER_SPECIFIC_MODE  0xA6  // 166 - DALI-2 Query Manufacturer Specific Mode
                                                         //       The answer shall be YES when operatingMode is in the range 0x80..0xff and NO otherwise.
#define DALI_102_QUERY_NEXT_DEVICE_TYPE            0xA7  // 167 - DALI-2 What is the next Device Type?
                                                         //       If directly preceded by DALI_102_QUERY_DEVICE_TYPE and more than one device type is supported, returns the first and lowest device type number.
                                                         //       If directly preceded by DALI_102_QUERY_NEXT_DEVICE_TYPE and not all device types have been reported, returns the next lowest device type number.
                                                         //       If directly preceded by DALI_102_QUERY_NEXT_DEVICE_TYPE and all device types have been reported, returns 254.
                                                         //       In all other cases returns NO (no response).
                                                         //       Multi-master transmitters shall send the sequence DALI_102_QUERY_DEVICE_TYPE,DALI_102_QUERY_NEXT_DEVICE_TYPE,... as a transaction.
#define DALI_102_QUERY_EXTENDED_FADE_TIME          0xA8  // 168 - DALI-2 What is the Extended Fade Time?
                                                         //       bit 0..3 - value 0..15 (E_DALIExtendedFadeTimeBase)
                                                         //       bit 4..7 - 0 = Disabled (E_DALIExtendeFadeTimeMultiplier)
                                                         //                  1 = Multiplier100ms
                                                         //                  2 = Multiplier1s
                                                         //                  3 = Multiplier10s
                                                         //                  4 = Multiplier1min
#define DALI_102_QUERY_CONTROL_GEAR_FAILURE        0xA9  // 169 - DALI-2 Does a ballast have the abnormality (0 = No, 1 = Yes)?
                                                         //       The answer shall be YES if controlGearFailure is TRUE and NO otherwise.
#define DALI_102_RESERVED170                       0xAA  // 170 - [Reserved]
                                                         // ...
#define DALI_102_RESERVED175                       0xAF  // 175 - [Reserved]
#define DALI_102_QUERY_SCENE0_LEVEL                0xB0  // 176 - Return the level set for scene 0, or 255 ("MASK") if the device is not part of the scene.
                                                         // ...
#define DALI_102_QUERY_SCENE15_LEVEL               0xBF  // 191 - Return the level set for scene 15, or 255 ("MASK") if the device is not part of the scene.
#define DALI_102_QUERY_GROUPS_0_7                  0xC0  // 192 - Return the device membership of groups 0-7 with group 0 in the least-significant bit of the response.
#define DALI_102_QUERY_GROUPS_8_15                 0xC1  // 193 - Return the device membership of groups 8-15 with group 8 in the least-significant bit of the response.
#define DALI_102_QUERY_RANDOM_ADDRESS_H            0xC2  // 194 - What are the high 8 bits of the random address?
#define DALI_102_QUERY_RANDOM_ADDRESS_M            0xC3  // 195 - What are the middle 8 bits of the random address?
#define DALI_102_QUERY_RANDOM_ADDRESS_L            0xC4  // 196 - What are the low 8 bits of the random address?
#define DALI_102_READ_MEMORY_LOCATION              0xC5  // 197 - What is the memory location content (Uses DTR0 and DTR1) 
                                                         //       The query is ignored if the addressed memory bank is not implemented.
                                                         //       If executed, the answer will be the content of the memory location identified by DTR0 in bank DTR1.
                                                         //       If the addressed location is below 0xff, then DTR0 is incremented by 1.
#define DALI_102_RESERVED198                       0xC6  // 198 - [Reserved]
                                                         // ...
#define DALI_102_RESERVED223                       0xDF  // 223 - [Reserved]

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 201 = DT0 - Send as second byte
 * Standard device like fluorescent lamps
\*-------------------------------------------------------------------------------------------*/

#define DALI_201_DEVICE_TYPE                       0

// Application extended query commands - Send as second byte
#define DALI_201_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 202 = DT1 - Send as second byte
 * Device for emergency lighting
\*-------------------------------------------------------------------------------------------*/

#define DALI_202_DEVICE_TYPE                       1

// Application extended query commands - Send as second byte
#define DALI_202_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 203 = DT2 - Send as second byte
 * Device for discharge lamps excluding fluorescent lamps
\*-------------------------------------------------------------------------------------------*/

#define DALI_203_DEVICE_TYPE                       2

// Application extended query commands - Send as second byte
#define DALI_203_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 204 = DT3 - Send as second byte
 * Device for low-voltage halogen lamps
\*-------------------------------------------------------------------------------------------*/

#define DALI_204_DEVICE_TYPE                       3

// Application extended query commands - Send as second byte
#define DALI_204_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 205 = DT4 - Send as second byte
 * Device for dimming incandescent lamps
\*-------------------------------------------------------------------------------------------*/

#define DALI_205_DEVICE_TYPE                       4

// Application extended configuration commands - Send as second byte with repeat
#define DALI_205_REFERENCE_SYSTEM_POWER            0xE0  // 224 - Reference System Power
                                                         //       The control gear shall measure and store system power levels in order to detect load increase or load decrease.
                                                         //       The measurement may take up to 15 minutes.
                                                         //       Measured power levels will be stored in non-volatile memory, Commands received during the measuring period will be ignored except query commands and Terminate.
                                                         //       The process will be aborted if DALI_102_TERMINATE is received.
#define DALI_205_SELECT_DIMMING_CURVE              0xE1  // 225 - Select Dimming Curve
                                                         //       If DTR0 is 0 then selects the standard logarithmic curve
                                                         //       If DTR0 is 1 then selects a linear dimming curve
                                                         //       Other values of DTR0 are reserved and will not change the dimming curve.
                                                         //       The setting is stored in non-volatile memory and is not cleared by the Reset command.
#define DALI_205_RESERVED226                       0xE2  // 226 - [Reserved]
                                                         // ...
#define DALI_205_RESERVED237                       0xED  // 237 - [Reserved]

// Application extended query commands - Send as second byte
#define DALI_205_QUERY_DIMMING_CURVE               0xEE  // 238 - Returns Dimming curve in use
                                                         //       value 0 = Standard - Standard logarithmic dimming curve
                                                         //       value 1 = Linear - Linear dimming curve
                                                         //       value 255 = Unknown
#define DALI_205_QUERY_DIMMER_STATUS               0xEF  // 239 - Returns dimmer status
                                                         //       bit 0 - leading edge mode running
                                                         //       bit 1 - trailing edge mode running
                                                         //       bit 2 - reference measurement running
                                                         //       bit 3 - None
                                                         //       bit 4 - non-logarithmic dimming curve active
                                                         //       bit 5..7 - Not used
#define DALI_205_QUERY_FEATURES                    0xF0  // 240 - Return feature bytes. There are three bytes of feature information.
                                                         //       Byte 1 is the reply to this command.
                                                         //         bit 0 - load over-current shutdown can be queried
                                                         //         bit 1 - open circuit detection can be queried
                                                         //         bit 2 - detection of load decrease can be queried
                                                         //         bit 3 - detection of load increase can be queried
                                                         //         bit 4 - None
                                                         //         bit 5 - thermal shutdown can be queried
                                                         //         bit 6 - thermal overload with output level reduction can be queried
                                                         //         bit 7 - physical selection supported
                                                         //       Byte 2 is transferred to DTR0.
                                                         //         bit 0 - temperature can be queried
                                                         //         bit 1 - supply voltage can be queried
                                                         //         bit 2 - supply frequency can be queried
                                                         //         bit 3 - load voltage can be queried
                                                         //         bit 4 - load current can be queried
                                                         //         bit 5 - real load power can be queried
                                                         //         bit 6 - load rating can be queried
                                                         //         bit 7 - load current overload with output level reduction can be queried
                                                         //       Byte 3 is transferred to DTR1.
                                                         //         bit 0..1 - Dimming method
                                                         //           value 0 = leading & trailing
                                                         //           value 1 = leading only
                                                         //           value 2 = trailing only
                                                         //           value 3 = sine wave
                                                         //         bit 2 - None
                                                         //         bit 3 - non-logarithmic dimming curve can be selected
                                                         //         bit 4..6 - None
                                                         //         bit 7 - load unsuitable can be queried
#define DALI_205_QUERY_FAILURE_STATUS              0xF1  // 241 - Returns failure status. There are two bytes of failure information.
                                                         //       Responds with byte 1 of the failure status information, and transfers byte 2 of the failure status into DTR1.
                                                         //       Failure states which cause output level reduction shall only be reset by re-powering the control gear or by any command that causes the output to turn off.
                                                         //       Failure states which cause shutdown shall only be reset by re-powering the control gear or using an optional reset switch on the control gear.
                                                         //       Byte 1 is the reply to this command.
                                                         //         bit 0 - load over-current shutdown
                                                         //         bit 1 - open circuit detected
                                                         //         bit 2 - load decrease detected
                                                         //         bit 3 - load increase detected
                                                         //         bit 4 - None
                                                         //         bit 5 - thermal shutdown
                                                         //         bit 6 - thermal overload with output level reduction
                                                         //         bit 7 - reference measurement failed
                                                         //       Byte 2 is transferred to DTR1.
                                                         //         bit 0 - load not suitable for selected dimming method
                                                         //         bit 1 - supply voltage out of limits
                                                         //         bit 2 - supply frequency out of limits
                                                         //         bit 3 - load voltage out of limits
                                                         //         bit 4 - load current overload with output level reduction
                                                         //         bit 5..7 - Not used
#define DALI_205_QUERY_DIMMER_TEMPERATURE          0xF2  // 242 - Returns the temperature of the dimmer with 1 degC resolution.
                                                         //       Values of 0 to 254 represent temperatures of -40C to +214C. 
                                                         //       Below -40C, 0 is returned. Above 214C, 254 is returned. A value of 255 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_RMS_SUPPLY_VOLTAGE          0xF3  // 243 - Returns the measured supply voltage.
                                                         //       Values of 0 to 254 represent 0V to 508V RMS.
                                                         //       Voltages above 508V RMS shall be returned as 254. A value of 255 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_SUPPLY_FREQUENCY            0xF4  // 244 - Returns the supply frequency with 0.5Hz resolution.
                                                         //       Values of 0 to 254 represent 0Hz to 127Hz.
                                                         //       Frequencies above 127Hz are returned as 254. A value of 255 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_RMS_LOAD_VOLTAGE            0xF5  // 245 - Returns the measured load voltage.
                                                         //       Values of 0 to 254 represent 0V to 508V RMS.
                                                         //       Voltages above 508V RMS shall be returned as 254. A value of 255 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_RMS_LOAD_CURRENT            0xF6  // 246 - Returns the measured load current as a percentage of the rated load current given by the answer to QueryLoadRating, with 0.5% resolution.
                                                         //       Values of 0 to 254 represent 0% to 127%. 
                                                         //       Higher currents shall be returned as 254. A value of 255 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_REAL_LOAD_POWER             0xF7  // 247 - Returns the high byte of the real power supplied to the load. The low byte is transferred to DTR0.
                                                         //       Values of 0 to 65534 represent powers from 0W to 16383.5W with a resolution of 0.25W.
                                                         //       Powers above this range are returned as 65534. A value of 65535 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_LOAD_RATING                 0xF8  // 248 - Returns the maximum load current rating with 150mA resolution.
                                                         //       Values of 0 to 254 represent 0A to 38.1A RMS.
                                                         //       Currents above 38.1A shall be returned as 254. A value of 255 means "unknown".
                                                         //       Control gear without this feature shall not react.
#define DALI_205_QUERY_REFERENCE_RUNNING           0xF9  // 249 - Asks if the ReferenceSystemPower measurement is running.
#define DALI_205_QUERY_REFERENCE_FAILED            0xFA  // 250 - Asks if the reference measurement started by ReferenceSystemPower failed.
#define DALI_205_RESERVED251                       0xFB  // 251 - [Reserved]
                                                         // ...
#define DALI_205_RESERVED254                       0xFE  // 254 - [Reserved]
#define DALI_205_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 206 = DT5 - Send as second byte
 * Device for converting digital signales into DC signals
\*-------------------------------------------------------------------------------------------*/

#define DALI_206_DEVICE_TYPE                       5

// Application extended query commands - Send as second byte
#define DALI_206_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 207 = DT6 - Send as second byte
 * Device for Light Emitting Diodes (LEDs)
 * Variables
 * Name             Reset value                  Scope                                        Size    Comment
 * ---------------  ---------------------------  -------------------------------------------  ------  ---------
 * minFastFadeTime  No change                    E_DALIFastFadeTime.T100ms                    1 byte  Read only
 *                                               ...
 *                                               E_DALIFastFadeTime.T700ms
 * fastFadeTime     E_DALIFastFadeTime.Disabled  E_DALIFastFadeTime.Disabled,minFastFadeTime  1 byte
 *                                               ...
 *                                               E_DALIFastFadeTime.T700ms
 * controlGearType  No change                    0...255                                      1 byte  Read only
 * features         No change                    0...255                                      1 byte  Read only
 * failureStatus    No change                    0...255                                      1 byte  Read only
 * dimmingCurve     E_DALIDimmingCurve.Standard  E_DALIDimmingCurve.Standard,                 1 byte
 *                                               E_DALIDimmingCurve.Linear
\*-------------------------------------------------------------------------------------------*/

#define DALI_207_DEVICE_TYPE                       6

// Application extended configuration commands - Send as second byte with repeat
#define DALI_207_REFERENCE_SYSTEM_POWER            0xE0  // 224 - The DALI control gear measures and stores the performance level of the system, in order to detect load increase and decrease.
#define DALI_207_ENABLE_CURRENT_PROTECTOR          0xE1  // 225 - Enables the current protection (deleted 2018).
#define DALI_207_DISABLE_CURRENT_PROTECTOR         0xE2  // 226 - Disables the current protection (deleted 2018).
#define DALI_207_SELECT_DIMMING_CURVE              0xE3  // 227 - Selects Dimming curve (see 238).
                                                         //       If DTR0 is 0 then selects the standard logarithmic curve
                                                         //       If DTR0 is 1 then selects a linear dimming curve
                                                         //       Other values of DTR0 are reserved and will not change the dimming curve.
                                                         //       The setting is stored in non-volatile memory and is not cleared by the Reset command.
#define DALI_207_SET_FAST_FADE_TIME                0xE4  // 228 - Sets the DTR0 of the data as Fast Fade Time (see 253).
#define DALI_207_RESERVED229                       0xE5  // 229 - [Reserved]
                                                         // ...
#define DALI_207_RESERVED236                       0xEC  // 236 - [Reserved]

// Application extended query commands - Send as second byte
#define DALI_207_QUERY_GEAR_TYPE                   0xED  // 237 - Returns ‘GEAR TYPE’
                                                         //       bit 0 - LED power supply integrated
                                                         //       bit 1 - LED module integrated
                                                         //       bit 2 - AC supply possible
                                                         //       bit 3 - DC supply possible
                                                         //       bit 4..7 - Reserve
#define DALI_207_QUERY_DIMMING_CURVE               0xEE  // 238 - Returns Dimming curve in use (E_DALIDimmingCurve)
                                                         //       value 0 = Standard - Standard logarithmic dimming curve
                                                         //       value 1 = Linear - Linear dimming curve
                                                         //       value 255 = Unknown
#define DALI_207_QUERY_POSSIBLE_OPERATING_MODE     0xEF  // 239 - Returns ‘POSSIBLE OPERATING MODE’ (deleted 2018)
                                                         //       bit 0 - PWM mode possible
                                                         //       bit 1 - AM mode possible
                                                         //       bit 2 - Output is current controlled
                                                         //       bit 3 - High current pulse mode
                                                         //       bit 4..7 - Reserve
#define DALI_207_QUERY_FEATURES                    0xF0  // 240 - Returns ‘FEATURES’
                                                         //       bit 0 - Short circuit detection can be queried
                                                         //       bit 1 - Open circuit detection can be queried
                                                         //       bit 2 - Detection of the load decrease can be queried
                                                         //       bit 3 - Detection of the load increase can be queried
                                                         //       bit 4 - Current protector is implemented and can be queried
                                                         //       bit 5 - Thermal shutdown can be queried
                                                         //       bit 6 - Reduction of the output level due to thermal overload can be queried
                                                         //       bit 7 - Physical selection supported
#define DALI_207_QUERY_FAILURE_STATUS              0xF1  // 241 - Returns ‘FAILURE STATUS’
                                                         //       bit 0 - Short circuit
                                                         //       bit 1 - Open circuit
                                                         //       bit 2 - Load decrease
                                                         //       bit 3 - Load increase
                                                         //       bit 4 - Current protector active
                                                         //       bit 5 - Thermal shutdown
                                                         //       bit 6 - Thermal overload with output level reduction
                                                         //       bit 7 - Reference measurement failed
#define DALI_207_QUERY_SHORT_CIRCUIT               0xF2  // 242 - Returns bit0 short circuit of ‘FAILURE STATUS’ (deleted 2018)
#define DALI_207_QUERY_OPEN_CIRCUIT                0xF3  // 243 - Returns bit1 open circuit of ‘FAILURE STATUS’ (deleted 2018)
#define DALI_207_QUERY_LOAD_DECREASE               0xF4  // 244 - Returns bit2 load decrease of ‘FAILURE STATUS’
#define DALI_207_QUERY_LOAD_INDREASE               0xF5  // 245 - Returns bit3 load increase of‘FAILURE STATUS’
#define DALI_207_QUERY_CURRENT_PROTECTOR_ACTIVE    0xF6  // 246 - Returns bit4 current protector active of ‘FAILURE STATUS’ (deleted 2018)
#define DALI_207_QUERY_THERMAL_SHUTDOWN            0xF7  // 247 - Returns bit5 thermal shut down of ‘FAILURE STATUS’
#define DALI_207_QUERY_THERMAL_OVERLOAD            0xF8  // 248 - Returns bit6 thermal overload with light level reduction of ‘FAILURE STATUS’
#define DALI_207_QUERY_REFERENCE_RUNNING           0xF9  // 249 - Returns whether Reference System Power is in operation.
#define DALI_207_QUERY_REFERENCE_MEASURMENT_FAILED 0xFA  // 250 - Returns bit7 reference measurement failed  of ‘FAILURE STATUS’
#define DALI_207_QUERY_CURRENT_PROTECTOR_ENABLE    0xFB  // 251 - Returns state of Curent protector (deleted 2018)
#define DALI_207_QUERY_OPERATING_MODE2             0xFC  // 252 - Returns ‘OPERATING MODE’ (deleted 2018)
                                                         //       bit 0 - PWM mode active
                                                         //       bit 1 - AM mode active
                                                         //       bit 2 - Output is current controlled
                                                         //       bit 3 - High current pulse mode is active
                                                         //       bit 4 - non-logarithmic dimming curve active
                                                         //       bit 5..7 - Reserve
#define DALI_207_QUERY_FAST_FADE_TIME              0xFD  // 253 - Returns set Fast fade time (E_DALIFastFadeTime)
                                                         //       value 0 = Disabled
                                                         //       value 4 = T100ms
                                                         //       value 8 = T200ms
                                                         //       value 9 = T225ms
                                                         //       value 12 = T300ms
                                                         //       value 16 = T400ms
                                                         //       value 20 = T500ms
                                                         //       value 24 = T600ms
                                                         //       value 27 = T700ms
                                                         //       value 255 = Unknown
#define DALI_207_QUERY_MIN_FAST_FADE_TIME          0xFE  // 254 - Returns set Minimum fast fade time (variable minFastFadeTime)
#define DALI_207_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 208 = DT7 - Send as second byte
 * Device for switching functions
\*-------------------------------------------------------------------------------------------*/

#define DALI_208_DEVICE_TYPE                       7

// Application extended query commands - Send as second byte
#define DALI_208_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 209 = DT8 - Send as second byte
 * Device for controlling colour and colour temperature
 * Tc is expressed in MIREK and can vary from 1 Mirek (1000000 Kelvin) to 65534 Mirek (15.26 Kelvin)
 * RGBWAF - Red, Green, Blue, White, Amber, Freely selectable color
\*-------------------------------------------------------------------------------------------*/

#define DALI_209_DEVICE_TYPE                       8

// Application extended control commands - Send as second byte with repeat
#define DALI_209_SET_TEMPORARY_X_COORDINATE        0xE0  // 224 - Set temporary x-COORDINATE (Uses DTR0 (LSB) and DTR1 (MSB))
                                                         //       The value is expressed in units of 1/65536. The maximum x-COORDINATE value is 0,99997
#define DALI_209_SET_TEMPORARY_Y_COORDINATE        0xE1  // 225 - Set temporary y-COORDINATE (Uses DTR0 (LSB) and DTR1 (MSB))
                                                         //       The value is expressed in units of 1/65536. The maximum y-COORDINATE value is 0,99997.
#define DALI_209_ACTIVATE                          0xE2  // 226 - Transfer buffered values of temporary registers to the lamp. 
                                                         //       It ends a running cross-fade and starts a new cross-fade for the respective color/color temperature.
#define DALI_209_X_COORDINATE_STEP_UP              0xE3  // 227 - The x-COORDINATE shall be set 256 steps higher (256/65536) immediately without fading.
                                                         //       This command shall only be executed when ‘Colour type xy-coordinate active’ bit, bit 4 of the ‘COLOUR STATUS’, is set.
                                                         //       If the new colour value does not correspond to a colour attainable by the control gear, this shall be indicated by
                                                         //        the ‘xy-coordinate colour point out of range’ bit, bit 0 of the ‘COLOUR STATUS’.
#define DALI_209_X_COORDINATE_STEP_DOWN            0xE4  // 228 - The x-COORDINATE shall be set 256 steps lower (256/65536) immediately without fading.
                                                         //       This command shall only be executed when ‘Colour type xy-coordinate active’ bit, bit 4 of the ‘COLOUR STATUS’, is set.
                                                         //       If the new colour value does not correspond to a colour attainable by the control gear, this shall be indicated by
                                                         //        the ‘xy-coordinate colour point out of range’ bit, bit 0 of the ‘COLOUR STATUS’. 
#define DALI_209_Y_COORDINATE_STEP_UP              0xE5  // 229 - The y-COORDINATE shall be set 256 steps higher (256/65536) immediately without fading.
                                                         //       This command shall only be executed when ‘Colour type xy-coordinate active’ bit, bit 4 of the ‘COLOUR STATUS’, is set.
                                                         //       If the new colour value does not correspond to a colour attainable by the control gear, this shall be indicated by
                                                         //        the ‘xy-coordinate colour point out of range’ bit, bit 0 of the ‘COLOUR STATUS’. 
#define DALI_209_Y_COORDINATE_STEP_DOWN            0xE6  // 230 - The y-COORDINATE shall be set 256 steps lower (256/65536) immediately without fading.
                                                         //       This command shall only be executed when ‘Colour type xy-coordinate active’ bit, bit 4 of the ‘COLOUR STATUS’, is set.
                                                         //       If the new colour value does not correspond to a colour attainable by the control gear, this shall be indicated by 
                                                         //        the ‘xy-coordinate colour point out of range’ bit, bit 0 of the ‘COLOUR STATUS’. 
#define DALI_209_SET_TEMPORARY_COLOUR_TEMP_TC      0xE7  // 231 - Set temporary colour temperature Tc (Uses DTR0 (LSB) and DTR1 (MSB))
                                                         //       The value is expressed in units of 1 Mirek.
                                                         //       A value of 0 for Tc shall be ignored and therefore not stored in memory.
                                                         //       NOTE Colour temperature TC can vary from 1 Mirek (1 000 000 K) to 65 534 Mirek (15,26 K).
#define DALI_209_COLOUR_TEMP_TC_STEP_COOLER        0xE8  // 232 - The ‘COLOUR TEMPERATURE TC’ shall be set 1 Mirek lower immediately without fading.
                                                         //       This command shall only be executed when ‘Colour type colour temperature TC active’ bit, bit 5 of the ‘COLOUR STATUS’ is set.
                                                         //       No change shall occur if ‘COLOUR TEMPERATURE TC’ is already at ‘COLOUR TEMPERATURE TC COOLEST’.
                                                         //       If the new colour value does not correspond to a colour temperature attainable by the control gear, this shall be indicated by 
                                                         //        the ‘Colour temperature TC out of range’ bit, bit 1 of the ‘COLOUR STATUS’. 
#define DALI_209_COLOUR_TEMP_TC_STEP_WARMER        0xE9  // 233 - The ‘COLOUR TEMPERATURE TC’ shall be set 1 Mirek higher immediately without fading.
                                                         //       This command shall only be executed when ‘Colour type colour temperature TC active’ bit, bit 5 of the ‘COLOUR STATUS’ is set.
                                                         //       No change shall occur if ‘COLOUR TEMPERATURE TC’ is already at ‘COLOUR TEMPERATURE TC WARMEST’.
                                                         //       If the new colour value does not correspond to a colour temperature attainable by the control gear, this shall be indicated by 
                                                         //        the ‘Colour temperature TC out of range’ bit, bit 1 of the ‘COLOUR STATUS’.
#define DALI_209_SET_TEMPORARY_PRIMARY_N_DIMLEVEL  0xEA  // 234 Deprecated - Set temporary primary N dimlevel (Uses DTR0 (LSB), DTR1 (MSB) and DTR2 (N))
                                                         //       The value is expressed in units of 1/65536.
                                                         //       The maximum ‘PRIMARY N DIMLEVEL’ value is 0,99997 and shall be interpreted on a linear scale.
                                                         //       N depends on DTR2 and shall be in the range from 0 to 5 depending upon the available number of primaries.
                                                         //       For any other value of DTR2 the command shall be ignored.
#define DALI_209_SET_TEMPORARY_RGB_DIMLEVEL        0xEB  // 235 - Set temporary RGB dimlevel
                                                         //       The data in the DTR shall be set as ‘TEMPORARY RED DIMLEVEL’.
                                                         //       The data in DTR1 shall be set as ‘TEMPORARY GREEN DIMLEVEL’.
                                                         //       The data in DTR2 shall be set as ‘TEMPORARY BLUE DIMLEVEL’.
#define DALI_209_SET_TEMPORARY_WAF_DIMLEVEL        0xEC  // 236 - Set temporary WAF dimlevel
                                                         //       The data in the DTR shall be set as ‘TEMPORARY WHITE DIMLEVEL’.
                                                         //       The data in DTR1 shall be set as ‘TEMPORARY AMBER DIMLEVEL’.
                                                         //       The data in DTR2 shall be set as ‘TEMPORARY FREECOLOUR DIMLEVEL’.
#define DALI_209_SET_TEMPORARY_RGBWAF_CONTROL      0xED  // 237 - Set temporary RGBWAF control via DTR0
                                                         //       The data in the DTR shall be stored as ‘TEMPORARY RGBWAF CONTROL’.
                                                         //       bit 0 - output channel 0/Red; '0' = Unlinked, '1' = Linked
                                                         //       bit 1 - output channel 1/Green; '0' = Unlinked, '1' = Linked
                                                         //       bit 2 - output channel 2/Blue; '0' = Unlinked, '1' = Linked
                                                         //       bit 3 - output channel 3/White; '0' = Unlinked, '1' = Linked
                                                         //       bit 4 - output channel 4/Amber; '0' = Unlinked, '1' = Linked
                                                         //       bit 5 - output channel 5/Freecolour; '0' = Unlinked, '1' = Linked
                                                         //       bits 6..7 control type
                                                         //         value 0 = Channel control
                                                         //         value 1 = Colour control
                                                         //         value 2 = Normalised colour control
                                                         //         value 3 = reserved
                                                         //       Bit 0 to bit 5 sets the appropriate output channel(s)/colour(s) linked or unlinked.
                                                         //       Bit 6 and bit 7: The control type defines how the gear shall react to Arc Power Commands.
                                                         //       The linked channels shall all be set to unlinked on any colour activation with colour type xy-coordinate, colour temperature TC or primary N.
                                                         //       NOTE More than one channel can be linked at the same time
#define DALI_209_COPY_REPORT_TO_TEMPORARY          0xEE  // 238 - Copy the contents of the variables with the color settings to the variables for the temporary color settings.

// Application extended configuration commands - Send as second byte with repeat
#define DALI_209_RESERVED239                       0xEF  // 239 - [Reserved]
#define DALI_209_STORE_TY_PRIMARY_N                0xF0  // 240 Deprecated - Store TY Primary N via DTR0/1/2
                                                         //       The value is expressed in units of 0,5 lumen resulting in a possible range of TYmin = 0 lumen, to TYmax = 32767 lumen.
                                                         //       A value of 65535 (“MASK”) means unknown.
                                                         //       N depends on DTR2 and shall be in the range from 0 to 5 depending upon the available number of primaries.
                                                         //       For any other value of DTR2 the command shall be ignored.
                                                         //       A value of “MASK” means that this primary is undefined and calibration is needed.
#define DALI_209_STORE_XY_COORDINATE_PRIMARY_N     0xF1  // 241 Deprecated - Store XY coord primary channel N via DTR2
                                                         //       The ‘TEMPORARY x-COORDINATE’ and the ‘TEMPORARY y-COORDINATE’, given by command 224 and command 225 shall be stored as ‘x-COORDINATE PRIMARY N’ 
                                                         //        respectively ‘y-COORDINATE PRIMARY N’ of primary N given by the value of DTR2, and shall be in the range from 0 to 5 depending upon the available number of primaries.
                                                         //       For any other value of DTR2 the command shall be ignored.
                                                         //       A value of “MASK” in one of the “temporary colour value” values shall be stored which means that this primary is undefined and calibration is needed.
                                                         //       NOTE 1 The intended use of this command is to store the actual xy-coordinate belonging to the primary. Any other use can lead to unexpected results (colours).
                                                         //       NOTE 2 xy-coordinates outside the CIE 1931 colour space chromaticity diagram are not meaningful. 
#define DALI_209_STORE_COLOUR_TEMP_TC_LIMIT        0xF2  // 242 - Set color temperature Tc limit in Mirek (Uses DTR0 (LSB), DTR1 (MSB) and DTR2 (TcLimit type))
                                                         //       The value shall be stored in a variable defined by DTR2.
                                                         //       For any other value of DTR2 the command shall be ignored.
                                                         //       DTR2 = 0 - ColorTemperatureTcCoolest (lowest possible value but always equal to or warmer than the lowest possible physical value)
                                                         //       DTR2 = 1 - ColorTemperatureTcWarmest (highest possible value but always equal to or cooler than the highest possible physical value)
                                                         //       DTR2 = 2 - ColorTemperatureTcPhysicalCoolest (lowest possible physical value)
                                                         //       DTR2 = 3 - ColorTemperatureTcPhysicalWarmest (highest possible physical value)
#define DALI_209_STORE_GEAR_FEATURES_STATUS        0xF3  // 243 - Set gear features / status (See 247) (Uses DTR0)
                                                         //       The data in the DTR shall be interpreted as follows:
                                                         //       bit 0 - Automatic Activation; '0' = No
                                                         //       bit 1..7 - reserved; ‘0’ = No
                                                         //       If bit 0, the ‘Automatic Activation’ bit, is set to 1, all arc power control commands except “ENABLE DAPC SEQUENCE” shall automatically trigger a colour transition.
                                                         //       If the ‘Automatic Activation’ bit is set, bit 0 of the “GEAR FEATURES/STATUS” byte shall be set.
#define DALI_209_RESERVED244                       0xF4  // 244 - [Reserved]
#define DALI_209_ASSIGN_COLOUR_TO_LINKED_COMMAND   0xF5  // 245 - Assign to linked channel via DTR0
                                                         //       The data held in the DTR in the range 0 to 6 shall be used to assign any/all linked output channel(s) to the given colour.
                                                         //       DTR0 = 0 - No colour assigned
                                                         //       DTR0 = 1 - Red
                                                         //       DTR0 = 2 - Green
                                                         //       DTR0 = 3 - Blue
                                                         //       DTR0 = 4 - White
                                                         //       DTR0 = 5 - Amber
                                                         //       DTR0 = 6 - Freecolour
                                                         //       For any other value of the DTR the command shall be ignored.
                                                         //       The linked channels are given by bit 0 to bit 5 held in “TEMPORARY RGBWAF CONTROL”.
                                                         //       If “TEMPORARY RGBWAF CONTROL” holds “MASK” the channel assignment shall not be changed.
                                                         //       The “TEMPORARY COLOUR SETTINGS” are set to “MASK” after use of this command.
#define DALI_209_START_AUTO_CALIBRATION            0xF6  // 246 - Start auto calibration
                                                         //       The command shall start or re-trigger a 15 min timer. Bit 2 of “COLOUR STATUS” shall be set to “1” whilst this timer is running.
                                                         //       When the timer stops the most recent colour type, colour value and arc power level shall be restored immediately
                                                         //       Whilst the timer is running the control gear shall run a calibration procedure in order to measure the x-coordinate, y-coordinate and the TY-value of
                                                         //        all supported primaries and bit 3 of “COLOUR STATUS” shall be set to “0”.
                                                         //       While the calibration procedure is running, the control gear shall not react to any command except “TERMINATE”, “QUERY COLOUR STATUS” and “START AUTO CALIBRATION”.
                                                         //       The "TERMINATE" command shall cause the procedure to be aborted and the timer to be stopped.
                                                         //       If the calibration is successful then bit 3 of “COLOUR STATUS” shall be set to “1” and the timer shall be stopped.
                                                         //       If the calibration is not successful (bit 3 of “COLOUR STATUS” is “0”) and the control gear is capable of recovering the
                                                         //        last successful calibration data it shall do so.
                                                         //       In this case bit 3 of “COLOUR STATUS” shall bet set to “1”.
                                                         //       The capability to recover the last successful calibration data is a gear feature; see command 247.
                                                         //       Auto calibration is a gear feature; see command 247. If this feature is not supported the control gear shall not react in any way.
                                                         //       NOTE Due to the fact that the calibration process may take longer than 15 min, the control device should check the status of the auto calibration by 
                                                         //        command 248 ‘QUERY COLOUR STATUS’ periodically and re-trigger the calibration process timer by command 246 (START AUTO CALIBRATION) if necessary

// Application extended query commands - Send as second byte
#define DALI_209_QUERY_GEAR_FEATURES_STATUS        0xF7  // 247 - Returns gear features / status
                                                         //       bit 0 - Automatic activation
                                                         //       bit 1..5 - reserved
                                                         //       bit 6 - Automatic calibration is supported
                                                         //       bit 7 - Restoration of the automatic calibration is supported
#define DALI_209_QUERY_COLOUR_STATUS               0xF8  // 248 - Returns color status
                                                         //       bit 0 - xy coordinate color point is outside the valid range.
                                                         //       bit 1 - Color temperature Tc lies outside the valid range
                                                         //       bit 2 - Automatic calibration is active.
                                                         //       bit 3 - Automatic calibration was successful.
                                                         //       bit 4 - Color representation xy-coordinate active.
                                                         //       bit 5 - Color representation color temperature Tc active.
                                                         //       bit 6 - Color representation primary N active.
                                                         //       bit 7 - Color representation RGBWAF active.
#define DALI_209_QUERY_COLOUR_TYPE_FEATURES        0xF9  // 249 - Returns color type
                                                         //       bit 0 - Supports color representation through xy coordinates.
                                                         //       bit 1 - Supports color representation through color temperature Tc.
                                                         //       bit 2..4 - Number of primary N colors supported. A value of 0 means that this color representation by primary colors is not supported.
                                                         //       bit 5..7 - Number of RGBWAF channels supported. A value of 0 means that this color representation by RGBWAF is not supported.
#define DALI_209_QUERY_COLOUR_VALUE                0xFA  // 250 - Returns color value (DTR0 = E_DALIColourValue) (MSB in response, Uses DTR0 for LSB).
                                                         //       The answer depends on the DTR0 Value.
                                                         //       Most responses involve a 16-bit number, in such cases the reponse is the MSB and LSB is loaded into DTR0.
                                                         //       Answers corresponding to the DTR values related to an active colour type are only valid if the colour type of the requested colour value is
                                                         //        active (see command 248) or if the control gear is capable of recalculating the requested colour value from the active colour type into a colour
                                                         //        value of another colour type. If recalculation is not possible this shall be indicated by a value of “MASK” as answer.
                                                         //       Querying the number of primaries, the x-coordinate, y-coordinate and TY of primary N shall be independent of the implemented colour type.
                                                         //        If the control gear does not know the coordinates, or the primary is not there, the answer shall be “MASK”.
                                                         //       E_DALIColourValue (DTR0):
                                                         //         0 - XCoordinate
                                                         //         1 - YCoordinate
                                                         //         2 - ColourTemperatureTC
                                                         //         3 - PrimaryNDimLevel0
                                                         //         4 - PrimaryNDimLevel1
                                                         //         5 - PrimaryNDimLevel2
                                                         //         6 - PrimaryNDimLevel3
                                                         //         7 - PrimaryNDimLevel4
                                                         //         8 - PrimaryNDimLevel5
                                                         //         9 - RedDimLevel
                                                         //        10 - GreenDimLevel
                                                         //        11 - BlueDimLevel
                                                         //        12 - WhiteDimLevel
                                                         //        13 - AmberDimLevel
                                                         //        14 - FreecolourDimLevel
                                                         //        15 - RGBWAFControl
                                                         //        64 - XCoordinatePrimaryN0
                                                         //        65 - YCoordinatePrimaryN0
                                                         //        66 - TYPrimaryN0
                                                         //        67 - XCoordinatePrimaryN1
                                                         //        68 - YCoordinatePrimaryN1
                                                         //        69 - TYPrimaryN1
                                                         //        70 - XCoordinatePrimaryN2
                                                         //        71 - YCoordinatePrimaryN2
                                                         //        72 - TYPrimaryN2
                                                         //        73 - XCoordinatePrimaryN3
                                                         //        74 - YCoordinatePrimaryN3
                                                         //        75 - TYPrimaryN3
                                                         //        76 - XCoordinatePrimaryN4
                                                         //        77 - YCoordinatePrimaryN4
                                                         //        78 - TYPrimaryN4
                                                         //        79 - XCoordinatePrimaryN5
                                                         //        80 - YCoordinatePrimaryN5
                                                         //        81 - TYPrimaryN5
                                                         //        82 - NumberOfPrimaries
                                                         //       128 - ColourTemperatureTcCoolest
                                                         //       129 - ColourTemperatureTcPhysicalCoolest
                                                         //       130 - ColourTemperatureTcWarmest
                                                         //       131 - ColourTemperatureTcPhysicalWarmest
                                                         //       192 - TemporaryXCoordinate
                                                         //       193 - TemporaryYCoordinate
                                                         //       194 - TemporaryColourTemperature
                                                         //       195 - TemporaryPrimaryNDimLevel0
                                                         //       196 - TemporaryPrimaryNDimLevel1
                                                         //       197 - TemporaryPrimaryNDimLevel2
                                                         //       198 - TemporaryPrimaryNDimLevel3
                                                         //       199 - TemporaryPrimaryNDimLevel4
                                                         //       200 - TemporaryPrimaryNDimLevel5
                                                         //       201 - TemporaryRedDimLevel
                                                         //       202 - TemporaryGreenDimLevel
                                                         //       203 - TemporaryBlueDimLevel
                                                         //       204 - TemporaryWhiteDimLevel
                                                         //       205 - TemporaryAmberDimLevel
                                                         //       206 - TemporaryFreecolourDimLevel
                                                         //       207 - TemporaryRgbwafControl
                                                         //       208 - TemporaryColourType
                                                         //       224 - ReportXCoordinate
                                                         //       225 - ReportYCoordinate
                                                         //       226 - ReportColourTemperatureTc
                                                         //       227 - ReportPrimaryNDimLevel0
                                                         //       228 - ReportPrimaryNDimLevel1
                                                         //       229 - ReportPrimaryNDimLevel2
                                                         //       230 - ReportPrimaryNDimLevel3
                                                         //       231 - ReportPrimaryNDimLevel4
                                                         //       232 - ReportPrimaryNDimLevel5
                                                         //       233 - ReportRedDimLevel
                                                         //       234 - ReportGreenDimLevel
                                                         //       235 - ReportBlueDimLevel
                                                         //       236 - ReportWhiteDimLevel
                                                         //       237 - ReportAmberDimLevel
                                                         //       238 - ReportFreecolourDimLevel
                                                         //       239 - ReportRgbwafControl
                                                         //       240 - ReportColourType
                                                         //       If, for colour type RGBWAF, more output channels are assigned to one colour (Red, Green, Blue, White, Amber or Freecolour) and the actual levels
                                                         //        of these output channels are different the answer to that query shall be “MASK”.
                                                         //       For all other DTR values and for unsupported colour types no answer shall be sent and neither DTR1 nor the DTR shall be changed.
                                                         //       NOTE 1 The actual level of an output channel can be queried by linking only this output channel and sending QUERY ACTUAL LEVEL.
                                                         //       NOTE 2 A control device should always use command 160 (“QUERY ACTUAL LEVEL”) to update the report colour setting before querying it.
                                                         //       NOTE 3 A value of “MASK” for any of “x-COORDINATE PRIMARY N”, “y-COORDINATE PRIMARY N” or “TY PRIMARY N” means that this primary is
                                                         //              undefined and calibration is needed.
#define DALI_209_QUERY_RGBWAF_CONTROL              0xFB  // 251 - RGBWAFControl contains further information about the assignment between output channel and color.
                                                         //       bit 0 - Output channel 0 / red
                                                         //       bit 1 - Output channel 1 / green
                                                         //       bit 2 - Output channel 2 / blue
                                                         //       bit 3 - Output channel 3 / white
                                                         //       bit 4 - Output channel 4 / amber
                                                         //       bit 5 - Output channel 5 / free selectable color
                                                         //       bit 6..7 - 0 = channel control
                                                         //                  1 = color control
                                                         //                  2 = standardized color control
                                                         //                  3 = reserved
                                                         //       If an output channel/colour is not supported the appropriate bit shall read '0'
#define DALI_209_QUERY_ASSIGNED_COLOUR             0xFC  // 252 - Returns the color assigned to the specified output channel (DTR0 = 0...5). If a non-existent channel number is specified, 255 (MASK) is returned. (Uses DTR0)
                                                         //       Assigned colour for a given channel (as specified in DTR0). Refer to Command 252 "QUERY ASSIGNED COLOUR".
                                                         //       Note that a response may indicate an assigned channel, or could also be "MASK" if the queried channel is not supported or is invalid.
                                                         //       value 0 - No color assigned
                                                         //       value 1 - Red
                                                         //       value 2 - Green
                                                         //       value 3 - Blue
                                                         //       value 4 - White
                                                         //       value 5 - Amber
                                                         //       value 6 - Freely selectable color
#define DALI_209_RESERVED253                       0xFD  // 253 - [Reserved]
#define DALI_209_RESERVED254                       0xFE  // 254 - [Reserved]
#define DALI_209_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

/*-------------------------------------------------------------------------------------------*\
 * DALI Application extended commands for IEC62386 part 210 = DT9 - Send as second byte
 * Sequencer
\*-------------------------------------------------------------------------------------------*/

#define DALI_210_DEVICE_TYPE                       9

// Application extended query commands - Send as second byte
#define DALI_210_QUERY_EXTENDED_VERSION_NUMBER     0xFF  // 255 - The version number of the extended support.
                                                         //       This command must be preceded by an appropriate DALI_102_ENABLE_DEVICE_TYPE_X command; if it is not then it will be ignored.
                                                         //       Returns the version number of Part 2xx of IEC 62386 for the corresponding device type as an 8-bit number.
                                                         //       Device type implementations must provide their own implementation of QueryExtendedVersionNumber using this mixin.

#endif  // _DALI_H_
