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
  "|" D_CMND_POWER 
#ifdef USE_LIGHT
  "|Web"
#endif  // USE_LIGHT
  "|" D_CMND_DIMMER "|Send|Query"  ;

void (* const DALICommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliPower,
#ifdef USE_LIGHT
  &CmndDaliWeb,
#endif  // USE_LIGHT
  &CmndDaliDimmer, &CmndDaliSend, &CmndDaliQuery };

struct DALI {
  uint32_t bit_time;
  uint16_t received_dali_data;                 // Data received from DALI bus
  uint8_t pin_rx;
  uint8_t pin_tx;
  uint8_t address;
  uint8_t command;
  uint8_t dimmer;
  bool power;
  bool available;
  bool response;
} *Dali = nullptr;

/*********************************************************************************************\
 * DALI low level
\*********************************************************************************************/

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
#ifdef DALI_DEBUG
    digitalWrite(DALI_DEBUG_PIN, bit_number&1);  // Add LogicAnalyzer poll indication
#endif  // DALI_DEBUG
    if (bit_number < 34) {                     // 34 manchester encoded bits
      bit_state += (dali_read) ? 1 : -1;
      if (0 == bit_state) {                    // Manchester encoding total 2 bits is always 0
        if (bit_number > 2) {                  // Skip start bit
          received_dali_data <<= 1;
          received_dali_data |= (DALI_IN_INVERT) ? !dali_read : dali_read;
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
  uint32_t bit_pos = 15;
  uint32_t wait = ESP.getCycleCount();
  for (uint32_t i = 0; i < 35; i++) {          // 417 * 35 = 14.7 ms
    if (0 == (i &1)) {                         // Even bit
      //          Start bit,     Stop bit,       Data bits
      bit_value = (0 == i) ? 1 : (34 == i) ? 0 : (bool)((send_dali_data >> bit_pos--) &1);  // MSB first
    } else {                                   // Odd bit
      bit_value = !bit_value;                  // Complement bit
    }
    bool pin_value = bit_value ? LOW : HIGH;   // Invert bit
    digitalWrite(Dali->pin_tx, (pin_value == DALI_OUT_INVERT) ? LOW : HIGH);
    wait += Dali->bit_time;                    // Auto roll-over
    while (ESP.getCycleCount() < wait);
  }
//  delayMicroseconds(1100);                     // Adds to total 15.8 ms
}

void DaliSendData(uint32_t adr, uint32_t cmd) {
  bool repeat = (adr &0x100);                  // Set repeat if bit 8 is set
  adr &= 0xFF;
  cmd &= 0xFF;

  Dali->address = adr;
  Dali->command = cmd;
  if (DALI_BROADCAST_DP == adr) {
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
  return result;
}

void DaliPower(uint32_t val) {
  DaliSendData(DALI_BROADCAST_DP, val);
}

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
  if (DALI_BROADCAST_DP == Dali->address) {
    uint8_t dimmer_old = changeUIntScale(Dali->dimmer, 0, 254, 0, 100);
    uint8_t power_old = Dali->power;
    Dali->power = (Dali->command);           // State
    if (Dali->power) {
      Dali->dimmer = Dali->command;          // Value
    }
    if (Settings->sbflag1.dali_web) {        // DaliWeb 1
      uint8_t dimmer_new = changeUIntScale(Dali->dimmer, 0, 254, 0, 100);
      if (power_old != Dali->power) {
        ExecuteCommandPower(LightDevice(), Dali->power, SRC_SWITCH);
      }
      else if (dimmer_old != dimmer_new) {
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), dimmer_new);
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
  if (DALI_BROADCAST_DP == Dali->address) {
    Dali->power = (Dali->command);           // State
    if (Dali->power) {
      Dali->dimmer = Dali->command;          // Value
    }
  }
  ResponseDali();
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_PRFX_DALI));
#endif  // USE_LIGHT

  Dali->available = false;
}

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

#ifdef USE_LIGHT
bool DaliSetChannels(void) {
  if (Settings->sbflag1.dali_web) {            // DaliWeb 1
    uint8_t value = ((uint8_t*)XdrvMailbox.data)[0];
    if (255 == value) { value = 254; }         // Max Dali value
    DaliPower(value);
  }
  return true;
}
#endif  // USE_LIGHT

/*********************************************************************************************\
 * Experimental - Not functioning
\*********************************************************************************************/

bool DaliMqtt(void) {
/*
  XdrvMailbox.topic = topic;
  XdrvMailbox.index = strlen(topic);
  XdrvMailbox.data = (char*)data;
  XdrvMailbox.data_len = data_len;

  This won't work as there is currently no subscribe done
*/
  char stopic[TOPSZ];
  strncpy(stopic, XdrvMailbox.topic, TOPSZ);
  XdrvMailbox.topic[TOPSZ - 1] = 0;

  char *items[10];
  char *p = stopic;
  int cnt = 0;
  do {
    items[cnt] = strtok(p, "/");
    cnt++;
    p = nullptr;
  } while (items[cnt - 1]);
  cnt--; // represents the number of items

  AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Cnt %d, Topic '%s', Payload '%s'"), cnt, XdrvMailbox.topic, XdrvMailbox.data);

  if (cnt < 3) { // not for us?
    AddLog(LOG_LEVEL_INFO, PSTR("DLI: Cnt %d < 3"), cnt);
    return false;
  }

  int DALIindex = 0;
  int ADRindex = 0;
  int CMDindex = 0;
  uint8_t DALIaddr = DALI_BROADCAST_DP;

  if (strcasecmp_P(items[cnt - 3], PSTR(DALI_TOPIC)) != 0) {      // dali
    // cmnd
    if (strcasecmp_P(items[cnt - 2], PSTR(DALI_TOPIC)) != 0) {    // dali
      // device
      return false; // not for us
    } else {
      // cmnd/dali/percent
      DALIindex = cnt - 2;
      CMDindex = cnt - 1;
    }
  } else {
    // dali/percent/2 20 
    DALIindex = cnt - 3;
    CMDindex = cnt - 2;
    ADRindex = cnt - 1;
    DALIaddr = ((int)CharToFloat(items[ADRindex]))  << 1;
  }

  uint8_t level;
  uint8_t value = (uint8_t)CharToFloat(XdrvMailbox.data);
  if (strcasecmp_P(items[CMDindex], PSTR("percent")) == 0) {
    // dali/percent/
    float percent = (float)(254 * value * 0.01);
    level = (uint8_t)percent;
  }
  else if (strcasecmp_P(items[CMDindex], PSTR("level")) == 0) {
    level = value;
  }
  else {
    AddLog(LOG_LEVEL_INFO,PSTR("DLI: Command not recognized: %s"), items[CMDindex]);
    return false; // not for us
  }

  AddLog(LOG_LEVEL_INFO,PSTR("DLI: Dali value %d on address %d"), value, DALIaddr);
  DaliSendData(DALIaddr, level);

  return true;
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

void CmndDaliPower(void) {
  // DaliPower 0      - Power off
  // DaliPower 1      - Power on to last dimmer state
  // DaliPower 2      - Toggle power off or last dimmer state
  // DaliPower 3..254 - Equals DaliDimmer command
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 254)) {
    if (XdrvMailbox.payload <= 2) {
      if (2 == XdrvMailbox.payload) {
        XdrvMailbox.payload = (Dali->power) ? 0 : 1;
      }
      if (1 == XdrvMailbox.payload) {
        XdrvMailbox.payload = Dali->dimmer;
      }
    }
    DaliPower(XdrvMailbox.payload);
  }
  ResponseDali();
}

void CmndDaliDimmer(void) {
  // DaliDimmer 0..100 - Set power off or dimmer state
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    uint8_t dimmer = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 254);
    DaliPower(dimmer);
  }
  ResponseDali();
}

void CmndDaliSend(void) {
  // Send command
  // Setting bit 8 will repeat command twice
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
  // Setting bit 8 will repeat command twice
  // DaliQuery 0xff,0x90  - DALI Query status
  // DaliQuery 0xff,144   - DALI Query status
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  if (2 == params) {
    int result = DaliSendWaitResponse(values[0] &0x1FF, values[1] &0xFF);
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
      case FUNC_MQTT_DATA:
        result = DaliMqtt();
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
