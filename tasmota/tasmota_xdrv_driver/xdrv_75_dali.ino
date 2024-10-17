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
 *   <first byte> = 0..255 or 0x00..0xFF + 256/0x100 for optional repeat (send twice)
 *   <second byte> = 0..255 or 0x00..0xFF        - Both decimal and hexadecimal is supported
 *   <broadcast> = 0                             - DALI default
 *   <device> = 1..64                            - DALI short address + 1
 *   <group> = 101..116                          - DALI group + 101
 * DaliSend <first byte>,<second byte>           - Execute DALI code and do not expect a DALI backward frame
 * DaliQuery <first byte>,<second byte>          - Execute DALI code and report result (DALI backward frame)
 * DaliCommission 1|2                            - Reset (0) or (1)/and commission device short addresses
 * DaliGroup<1..16> [+]|-<device>,<device>...    - Add(+) or Remove(-) devices to/from group
 * DaliPower<broadcast>|<device>|<group> 0..254  - Control power (0 = Off, 1 = Last dimmer, 2 = Toggle, 3..254 = absolute light brightness)
 * DaliDimmer<broadcast>|<device>|<group> 0..100 - Control dimmer (0 = Off, 1..100 = precentage of brightness)
 * DaliWeb 0|1                                   - Enable Tasmota light control for DaliTarget device
 * DaliTarget <broadcast>|<device>|<group>       - Set Tasmota light control device (0, 1..64, 101..116)
 * 
 * Address type        Address byte
 * ------------------  --------------------
 * Broadcast address   1111111S
 * 64 short address    0AAAAAAS
 * 16 group address    100AAAAS
 * Special command     101CCCC1 to 110CCCC1
 * A = Address bit, S = 0 Direct Arc Power control, S = 1 Command, C = Special command
\*********************************************************************************************/

#define XDRV_75             75

#ifndef DALI_IN_INVERT
#define DALI_IN_INVERT      0                  // DALI RX inverted
#endif
#ifndef DALI_OUT_INVERT
#define DALI_OUT_INVERT     0                  // DALI TX inverted
#endif
#ifndef DALI_INIT_STATE
#define DALI_INIT_STATE     50                 // DALI init dimmer state 50/254
#endif
#ifndef DALI_TIMEOUT
#define DALI_TIMEOUT        50                 // DALI backward frame receive timeout (ms)
#endif

//#define DALI_DEBUG
#ifndef DALI_DEBUG_PIN
#define DALI_DEBUG_PIN      4
#endif

#define DALI_BROADCAST_DP   0b11111110         // 0xFE = 254

#define DALI_TOPIC "DALI"
#define D_PRFX_DALI "Dali"

const char kDALICommands[] PROGMEM = D_PRFX_DALI "|"  // Prefix
  "|" D_CMND_POWER "|" D_CMND_DIMMER "|Target"
#ifdef USE_LIGHT
  "|Web"
#endif  // USE_LIGHT
  "|Send|Query|Commission|Group";

void (* const DALICommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliPower, &CmndDaliDimmer, &CmndDaliTarget,
#ifdef USE_LIGHT
  &CmndDaliWeb,
#endif  // USE_LIGHT
  &CmndDaliSend, &CmndDaliQuery, &CmndDaliCommission, &CmndDaliGroup };

struct DALI {
  uint32_t bit_time;
  uint16_t received_dali_data;                 // Data received from DALI bus
  uint8_t pin_rx;
  uint8_t pin_tx;
  uint8_t address;
  uint8_t command;
  uint8_t dimmer;
  uint8_t target;
  bool power;
  bool available;
  bool response;
  bool light_sync;
} *Dali = nullptr;

/*********************************************************************************************\
 * DALI low level
\*********************************************************************************************/

uint32_t DaliTarget2Address(uint32_t target) {
  // 1..64    = Short address
  // 101..116 = Group address
  // Others   = Broadcast
  if ((target >= 1) && (target <= 64)) {       // 1 .. 64
    target -= 1;                               // Short address
    target <<= 1;
  }
  else if ((target >= 101) && (target <= 116)) {  // 101 .. 116
    target -= 101;
    target <<= 1;
    target |= 0x80;                            // Group address
  }
  else {                                       // Others
    target = DALI_BROADCAST_DP;                // Broadcast address
  }
  return target &0xFE;                         // Direct Arc Power Control command
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
void DaliEnableRxInterrupt(void) {
  Dali->available = false;
  attachInterrupt(Dali->pin_rx, DaliReceiveData, FALLING);
}

void DaliDisableRxInterrupt(void) {
  detachInterrupt(Dali->pin_rx);
}

/*************** R E C E I V E * P R O C E D U R E *********/

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
  uint32_t wait = ESP.getCycleCount() + (Dali->bit_time / 2);
  int bit_state = 0; 
  bool dali_read;
  uint32_t received_dali_data = 0;
  uint32_t bit_number = 0;
  while (bit_number < 38) {
    while (ESP.getCycleCount() < wait);
    wait += Dali->bit_time;                    // Auto roll-over
    dali_read = digitalRead(Dali->pin_rx);
    if (DALI_IN_INVERT) { dali_read != dali_read; }
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
      else if (abs(bit_state) > 1) {           // Invalid manchester data
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
  if (0 == bit_state) {                        // Valid Manchester encoding including start and stop bits
    if (Dali->response ||                      // Response from last message send
       (Dali->received_dali_data != received_dali_data)) {  // Skip duplicates
      Dali->received_dali_data = received_dali_data;
      Dali->available = true;                  // Valid data received
    }
  }
}

/*************** S E N D * P R O C E D U R E ***************/

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
  bool bit_value;
  bool pin_value;
  bool dali_read;
  bool collision = false;
  uint32_t bit_pos = 15;
  uint32_t wait = ESP.getCycleCount();
  uint32_t bit_number = 0;
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

    digitalWrite(Dali->pin_tx, (pin_value == DALI_OUT_INVERT) ? LOW : HIGH);
    wait += Dali->bit_time;                    // Auto roll-over
    while (ESP.getCycleCount() < wait);

    if (!collision) {
      dali_read = digitalRead(Dali->pin_rx);
      if (DALI_IN_INVERT) { dali_read != dali_read; }
      if ((HIGH == pin_value) && (LOW == dali_read)) {  // Collision if write is 1 and bus is 0
        collision = true;
        pin_value = LOW;
        bit_number = 29;                       // Keep bus low for 4 bits
        AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Send collision"));
      }
    }

    bit_number++;
  }
//  delayMicroseconds(1100);                     // Adds to total 15.8 ms
}

void DaliSendData(uint32_t adr, uint32_t cmd) {
  bool repeat = (adr &0x100);                  // Set repeat if bit 8 is set
  adr &= 0xFF;
  cmd &= 0xFF;

  Dali->address = adr;
  Dali->command = cmd;
  if (DaliTarget2Address(Dali->target) == adr) {
    repeat = true;
    Dali->power = (cmd);                       // State
    if (Dali->power) {
      Dali->dimmer = cmd;                      // Value
    }
  }

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
  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: SendData Repeat %d, Adr 0x%02X, Cmd 0x%02x"), repeat, adr, cmd);
#endif  // DALI_DEBUG

  uint16_t send_dali_data = adr << 8 | cmd;

  DaliDisableRxInterrupt();
  delay(3);                                    // Settling time between forward and backward frame
  DaliSendDataOnce(send_dali_data);            // Takes 14.7 ms
  if (repeat) {
    delay(14);                                 // As used by Busch-Jaeger and suggested by DALI protocol (> 9.17 ms)
    DaliSendDataOnce(send_dali_data);          // Takes 14.7 ms
  }
  delay(2);                                    // Block response
  DaliEnableRxInterrupt();
}

int DaliSendWaitResponse(uint32_t adr, uint32_t cmd, uint32_t timeout = DALI_TIMEOUT);
int DaliSendWaitResponse(uint32_t adr, uint32_t cmd, uint32_t timeout) {
  Dali->response = true;
  DaliSendData(adr, cmd);
  while (!Dali->available && timeout--) {      // Expect backward frame within DALI_TIMEOUT ms
    delay(1);
  };
  int result = -1;                             // DALI NO or no response
  if (Dali->available) {
    Dali->available = false;
    result = Dali->received_dali_data;
  }
  Dali->response = false;

#ifdef DALI_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: SendWaitResponse result %d = 0x%04X"), result, result);
#endif  // DALI_DEBUG

  return result;
}

/*********************************************************************************************\
 * DALI commissioning short addresses
 * 
 * Courtesy of https://github.com/qqqlab/DALI-Lighting-Interface
\*********************************************************************************************/

// Query commands - Send as second byte
#define DALI_QUERY_STATUS          0x0090  // 144  - Returns "STATUS INFORMATION"

// Special commands - Send as first byte
#define DALI_TERMINATE             0x00A1  // 256  - Releases the INITIALISE state.
#define DALI_INITIALISE            0x01A5  // 258 REPEAT - Sets the slave to the INITIALISE status for15 minutes. Commands 259 to 270 are enabled only for a slave in this status.
#define DALI_RANDOMISE             0x01A7  // 259 REPEAT - Generates a random address.
#define DALI_COMPARE               0x00A9  // 260  - Is the random address smaller or equal to the search address?
#define DALI_WITHDRAW              0x00AB  // 261  - Excludes slaves for which the random address and search address match from the Compare process.
#define DALI_SEARCHADDRH           0x00B1  // 264  - Specifies the higher 8 bits of the search address.
#define DALI_SEARCHADDRM           0x00B3  // 265  - Specifies the middle 8 bits of the search address.
#define DALI_SEARCHADDRL           0x00B5  // 266  - Specifies the lower 8 bits of the search address.
#define DALI_PROGRAM_SHORT_ADDRESS 0x00B7  // 267  - The slave shall store the received 6-bit address (AAA AAA) as a short address if it is selected.

void DaliSetSearchAddress(uint32_t adr) {
  // Set search address
  DaliSendData(DALI_SEARCHADDRH, adr>>16);
  DaliSendData(DALI_SEARCHADDRM, adr>>8);
  DaliSendData(DALI_SEARCHADDRL, adr);
}

void DaliSetSearchAddressDifference(uint32_t adr_new, uint32_t adr_current) {
  // Set search address, but set only changed bytes (takes less time)
  if ( (uint8_t)(adr_new>>16) !=  (uint8_t)(adr_current>>16) ) DaliSendData(DALI_SEARCHADDRH, adr_new>>16);
  if ( (uint8_t)(adr_new>>8)  !=  (uint8_t)(adr_current>>8)  ) DaliSendData(DALI_SEARCHADDRM, adr_new>>8);
  if ( (uint8_t)(adr_new)     !=  (uint8_t)(adr_current)     ) DaliSendData(DALI_SEARCHADDRL, adr_new);
}

bool DaliCompare() {
  // Is the random address smaller or equal to the search address?
  // As more than one device can reply, the reply gets garbled
  uint8_t retry = 2;
  while (retry > 0) {
    // Compare is true if we received any activity on the bus as reply.
    // Sometimes the reply is not registered... so only accept retry times 'no reply' as a real false compare
    int rv = DaliSendWaitResponse(DALI_COMPARE, 0x00);
    if (rv == 0xFF) return true;               // Yes reply
    retry--;
  }
  return false;
}

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

void DaliProgramShortAddress(uint8_t shortadr) {
  // The slave shall store the received 6-bit address (AAAAAA) as a short address if it is selected.
  DaliSendData(DALI_PROGRAM_SHORT_ADDRESS, (shortadr << 1) | 0x01);

  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Set short address %d"), shortadr +1);
}

uint32_t DaliCommission(uint8_t init_arg) {
  // init_arg=11111111 : all without short address
  // init_arg=00000000 : all 
  // init_arg=0AAAAAA1 : only for this shortadr
  // returns number of new short addresses assigned
  DaliSendData(DALI_BROADCAST_DP, 0);          // Turn all OFF
  delay(100);                                  // Need 100ms pause before starting commissioning

  uint8_t arr[64];
  uint32_t sa;
  for (sa = 0; sa < 64; sa++) {
    arr[sa] = 0;
  }

  // Start commissioning
  DaliSendData(DALI_INITIALISE, init_arg);
  DaliSendData(DALI_RANDOMISE, 0x00);
  delay(100);                                  // Need 100ms pause after RANDOMISE

  // Find used short addresses (run always, seems to work better than without...)
  for (sa = 0; sa < 64; sa++) {
    int rv = DaliSendWaitResponse(sa << 1 | 1, DALI_QUERY_STATUS);
    if (rv >= 0) {
      if (init_arg != 0b00000000) {
        arr[sa] = 1;                           // Remove address from list if not in "all" mode
      }
    }
  }

  uint32_t cnt = 0;
  while (true) {                               // Find random addresses and assign unused short addresses
    uint32_t adr = DaliFindAddress();
    if (adr > 0xffffff) { break; }             // No more random addresses found -> exit
    for (sa = 0; sa < 64; sa++) {              // Find first unused short address
      if (0 == arr[sa]) { break; }
    }
    if( sa >= 64) { break; }                   // All 64 short addresses assigned -> exit
    arr[sa] = 1;                               // Mark short address as used
    cnt++;
 
    DaliProgramShortAddress(sa);               // Assign short address
    DaliSendData(DALI_WITHDRAW, 0x00);         // Remove the device from the search

    delay(1);
    OsWatchLoop();
  }

  DaliSendData(DALI_TERMINATE, 0x00);          // Terminate the DALI_INITIALISE command

  for (sa = 0; sa < cnt; sa++) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Flash short address %d"), sa +1);

    DaliSendData(sa << 1, 200);                // Flash assigned lights
    delay(1000);
    OsWatchLoop();
    DaliSendData(sa << 1, 0);
  }

  return cnt;
}

/*********************************************************************************************\
 * DALI group management
\*********************************************************************************************/

// Configuration commands - Send as second byte
#define DALI_ADD_TO_GROUP0         0x0060  //  96  - Adds the slave to Group XXXX.
#define DALI_REMOVE_FROM_GROUP0    0x0070  // 112  - Deletes the slave from Group XXXX.

/***********************************************************/

void ResponseAppendDali(void) {
  uint8_t dimmer = changeUIntScale(Dali->dimmer, 0, 254, 0, 100);
  ResponseAppend_P(PSTR("\"DALI\":{\"Power\":\"%s\",\"Dimmer\":%d,\"Address\":%d,\"Command\":%d}"), 
    GetStateText(Dali->power), dimmer, Dali->address, Dali->command);
}

void ResponseDali(void) {
  Response_P(PSTR("{"));
  ResponseAppendDali();
  ResponseJsonEnd();
}

void DaliInput(void) {
  if (!Dali->available || Dali->response) { return; }

  Dali->address = Dali->received_dali_data >> 8;
  Dali->command = Dali->received_dali_data;

#ifdef USE_LIGHT
  bool show_response = true;
  if (DaliTarget2Address(Dali->target) == Dali->address) {
    uint8_t dimmer_old = changeUIntScale(Dali->dimmer, 0, 254, 0, 100);
    uint8_t power_old = Dali->power;
    Dali->power = (Dali->command);             // State
    if (Dali->power) {
      Dali->dimmer = Dali->command;            // Value
    }
    if (Settings->sbflag1.dali_web) {          // DaliWeb 1
      uint8_t dimmer_new = changeUIntScale(Dali->dimmer, 0, 254, 0, 100);
      if (power_old != Dali->power) {
        Dali->light_sync = true;               // Block local loop
        ExecuteCommandPower(LightDevice(), Dali->power, SRC_SWITCH);
      }
      else if (dimmer_old != dimmer_new) {
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), dimmer_new);
        Dali->light_sync = true;               // Block local loop
        ExecuteCommand(scmnd, SRC_SWITCH);
      }
      show_response = false;
    }
  }
  if (show_response) {
    ResponseDali();
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_PRFX_DALI));
  }
#else
  if (DaliTarget2Address(Dali->target) == Dali->address) {
    Dali->power = (Dali->command);             // State
    if (Dali->power) {
      Dali->dimmer = Dali->command;            // Value
    }
  }
  ResponseDali();
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_PRFX_DALI));
#endif  // USE_LIGHT

  Dali->available = false;
}

#ifdef USE_LIGHT
bool DaliSetChannels(void) {
  if (Settings->sbflag1.dali_web) {            // DaliWeb 1
    if (Dali->light_sync) {                    // Block local loop
      Dali->light_sync = false;
    } else {
      uint8_t value = ((uint8_t*)XdrvMailbox.data)[0];
      if (255 == value) { value = 254; }       // Max Dali value
      DaliSendData(DaliTarget2Address(Dali->target), value);
    }
  }
  return true;
}
#endif  // USE_LIGHT

bool DaliInit(void) {
  if (!PinUsed(GPIO_DALI_TX) || !PinUsed(GPIO_DALI_RX)) { return false; }

  Dali = (DALI*)calloc(sizeof(DALI), 1);
  if (!Dali) { return false; }

  Dali->pin_rx = Pin(GPIO_DALI_RX);
  Dali->pin_tx = Pin(GPIO_DALI_TX);

  AddLog(LOG_LEVEL_INFO, PSTR("DLI: GPIO%d(RX) and GPIO%d(TX)"), Dali->pin_rx, Dali->pin_tx);

  pinMode(Dali->pin_tx, OUTPUT);
  digitalWrite(Dali->pin_tx, HIGH);
  pinMode(Dali->pin_rx, INPUT);
#ifdef DALI_DEBUG
  pinMode(DALI_DEBUG_PIN, OUTPUT);
  digitalWrite(DALI_DEBUG_PIN, HIGH);
#endif  // DALI_DEBUG

  Dali->dimmer = DALI_INIT_STATE;
  // Manchester twice 1200 bps = 2400 bps = 417 (protocol 416.76 +/- 10%) us
  Dali->bit_time = ESP.getCpuFreqMHz() * 1000000 / 2400;

  DaliEnableRxInterrupt();

#ifdef USE_LIGHT
  if (!Settings->sbflag1.dali_web) {           // DaliWeb 0
    return false;
  }

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

void CmndDali(void) {
  // Dali {"addr":254,"cmd":100} - Any address and/or command
  if (XdrvMailbox.data_len > 0) {
    DaliJsonParse();
  }
  ResponseDali();
}

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

void CmndDaliPower(void) {
  // DaliPower 0       - Broadcast power off
  // DaliPower 1       - Broadcast power on to last dimmer state
  // DaliPower 2       - Broadcast toggle power off or last dimmer state
  // DaliPower 3..254  - Broadcast equals DaliDimmer command
  // DaliPower 0..254  - Broadcast control
  // DaliPower0 0..254 - Broadcast control (= DaliPower)
  // DaliPower1 0..254 - Short address 0 control
  // DaliPower3 0..254 - Short address 2 control
  if (((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= 64)) ||
      ((XdrvMailbox.index >= 101) && (XdrvMailbox.index <= 116))) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 254)) {
      if (XdrvMailbox.payload <= 2) {
        if (2 == XdrvMailbox.payload) {
          XdrvMailbox.payload = (Dali->power) ? 0 : 1;
        }
        if (1 == XdrvMailbox.payload) {
          XdrvMailbox.payload = Dali->dimmer;
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
  ResponseDali();
}

void CmndDaliDimmer(void) {
  // DaliDimmer 0..100  - Broadcast set power off or dimmer state
  // DaliDimmer0 0..100 - Broadcast set power off or dimmer state
  // DaliDimmer1 0..100 - Short address 0 set power off or dimmer state
  // DaliDimmer3 0..100 - Short address 2 set power off or dimmer state
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
  ResponseDali();
}

void CmndDaliGroup(void) {
  // DaliGroup1 1,2   - Add device 1 and 2 to group 1
  // DaliGroup1 -1,2  - Remove device 1 and 2 to group 1
  if ((XdrvMailbox.index >= 1) && (XdrvMailbox.index <= 16)) {
    if (XdrvMailbox.data_len) {
      uint32_t command = DALI_ADD_TO_GROUP0;
      if ('+' == XdrvMailbox.data[0]) {        // Add devices
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
      }
      else if ('-' == XdrvMailbox.data[0]) {   // Remove devices
        command = DALI_REMOVE_FROM_GROUP0;
        XdrvMailbox.data++;
        XdrvMailbox.data_len--;
      }
      uint32_t argc = ArgC();                  // Number of devices
      if (argc) {
        command |= (XdrvMailbox.index -1);
        uint32_t sas[argc];
        ParseParameters(argc, sas);
        for (uint32_t arg = 0; arg < argc; arg++) {
          uint32_t sa = sas[arg] -1;
          if (sa <= 63) {
            DaliSendData(sa << 1 | 0x01, command);
          }
        }
        ResponseCmndDone();
      }
    }
  }
}

void CmndDaliSend(void) {
  // Send command
  // Setting bit 8 will repeat command once
  // DaliSend 0x1a5,255  - DALI Initialise (send twice)
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  if (2 == params) {
    DaliSendData(values[0] &0x1FF, values[1] &0xFF);
    ResponseCmndDone();
  }
}

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

void CmndDaliCommission(void) {
  // Commission short addresses
  // DaliCommission 1     - Reset and commission short addresses
  // DaliCommission 2     - Commission unassigned short addresses
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 2)) {
    uint32_t init_arg = 0x00;                  // Commission all
    if (2 == XdrvMailbox.payload) {
      init_arg = 0xFF;                         // Commission all without short addresses
    }
    int result = DaliCommission(init_arg);
    ResponseCmndNumber(result);
  }
}

#ifdef USE_LIGHT
void CmndDaliWeb(void) {
  // DaliWeb 0  - Disable GUI light controls
  // DaliWeb 1  - Enable GUI light controls
  if (XdrvMailbox.data_len > 0) {
    Settings->sbflag1.dali_web = XdrvMailbox.payload &1;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndStateText(Settings->sbflag1.dali_web);
}
#endif  // USE_LIGHT

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void DaliShow(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(","));
    ResponseAppendDali();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv75(uint32_t function) {
  bool result = false;

  if (FUNC_MODULE_INIT == function) {
    result = DaliInit();
  }
  else if (Dali) {
    switch (function) {
      case FUNC_LOOP:
        DaliInput();
        break;
#ifdef USE_LIGHT
      case FUNC_SET_CHANNELS:
        result = DaliSetChannels();
        break;
#endif  // USE_LIGHT
      case FUNC_JSON_APPEND:
        DaliShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        DaliShow(false);
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
