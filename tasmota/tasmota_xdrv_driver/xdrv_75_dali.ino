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
  0.1.0.4 20241013  update    - Fix intermittent bad send timing
  0.1.0.3 20241010  update    - Change DaliDimmer range from 0..254 to 0..100
                              - Add command DaliWeb 0|1 to enable persistent Web light controls
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
#define DALI_IN_INVERT      0                  // DALI RX inverted ?
#endif
#ifndef DALI_OUT_INVERT
#define DALI_OUT_INVERT     0                  // DALI TX inverted
#endif
#ifndef DALI_INIT_STATE
#define DALI_INIT_STATE     50                 // DALI init dimmer state 50/254
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
  "|" D_CMND_DIMMER ;

void (* const DALICommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliPower,
#ifdef USE_LIGHT
  &CmndDaliWeb,
#endif  // USE_LIGHT
  &CmndDaliDimmer };

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
} *Dali = nullptr;

/*********************************************************************************************\
 * DALI low level
\*********************************************************************************************/

void DaliEnableRxInterrupt(void) {
  attachInterrupt(Dali->pin_rx, DaliReceiveData, FALLING);
}

void DaliDisableRxInterrupt(void) {
  detachInterrupt(Dali->pin_rx);
}

/*************** R E C E I V E * P R O C E D U R E *********/

void IRAM_ATTR DaliReceiveData(void);          // Fix ESP8266 ISR not in IRAM! exception
void DaliReceiveData(void) {
  if (Dali->available) { return; }             // Skip if last input is not yet handled
  uint32_t wait = ESP.getCycleCount() + (Dali->bit_time / 2);
  int bit_state = 0; 
  bool dali_read;
  uint32_t received_dali_data = 0;
  uint32_t bit_pos = 15;
  for (uint32_t i = 0; i < 36; i++) {          // (1 Start bit, 16 data bits, 1 stop bit) * 2 bits/bit (manchester encoding)
    while (ESP.getCycleCount() < wait);
    wait += Dali->bit_time;                    // Auto roll-over
    if (abs(bit_state) <= 2) {                 // Manchester encoding max 2 consecutive equal bits
      dali_read = digitalRead(Dali->pin_rx);
#ifdef DALI_DEBUG
      digitalWrite(DALI_DEBUG_PIN, i&1);       // Add LogicAnalyzer poll indication
#endif  // DALI_DEBUG
      bit_state += (dali_read) ? 1 : -1;
      if ((i >= 2) && (i <= 34)) {             // 32 manchester encoded data bits
        if (i &1) {                            // 16 data bits
          received_dali_data |= ((DALI_IN_INVERT) ? !dali_read : dali_read << bit_pos--);
        }
      }
    }
  }
  if (abs(bit_state) <= 2) {                   // Valid Manchester encoding including start and stop bits
    if (Dali->received_dali_data != received_dali_data) {  // Skip duplicates
      Dali->received_dali_data = received_dali_data;
      Dali->available = true;                  // Valid data received
    }
  }
}

/*************** S E N D * P R O C E D U R E ***************/

void DaliSendDataOnce(uint16_t send_dali_data) {
  bool bit_value;
  uint32_t bit_pos = 15;
  uint32_t wait = ESP.getCycleCount();
  for (uint32_t i = 0; i < 35; i++) {
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
}

void DaliSendData(uint8_t firstByte, uint8_t secondByte) {
  Dali->address = firstByte;
  Dali->command = secondByte;
  if (DALI_BROADCAST_DP == firstByte) {
    Dali->power = (secondByte);                // State
    if (Dali->power) {
      Dali->dimmer = secondByte;               // Value
    }
  }
  uint16_t send_dali_data = firstByte << 8;
  send_dali_data += secondByte & 0xff;

  DaliDisableRxInterrupt();
  delay(3);                                    // Settling time between forward and backward frame
  DaliSendDataOnce(send_dali_data);            // Takes 14.5 ms
  if (DALI_BROADCAST_DP == firstByte) {
    delay(14);                                 // As used by Busch-Jaeger and suggested by DALI protocol (> 9.17 ms)
    DaliSendDataOnce(send_dali_data);          // Takes 14.7 ms
  }
  delay(3);                                    // Block response
  DaliEnableRxInterrupt();
}

void DaliPower(uint8_t val) {
  DaliSendData(DALI_BROADCAST_DP, val);
}

/***********************************************************/

void ResponseAppendDali(void) {
  uint8_t dimmer = changeUIntScale(Dali->dimmer, 0, 254, 0, 100);
  ResponseAppend_P(PSTR("\"" D_PRFX_DALI "\":{\"Power\":\"%s\",\"Dimmer\":%d,\"Address\":%d,\"Command\":%d}"), 
    GetStateText(Dali->power), dimmer, Dali->address, Dali->command);
}

void ResponseDali(void) {
  Response_P(PSTR("{"));
  ResponseAppendDali();
  ResponseJsonEnd();
}

void DaliInput(void) {
  if (Dali->available) {
    Dali->address = Dali->received_dali_data >> 8;
    Dali->command = Dali->received_dali_data;

#ifdef USE_LIGHT
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
      }
    }
    if (!Settings->sbflag1.dali_web) {         // DaliWeb 0
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
  Dali->bit_time = ESP.getCpuFreqMHz() * 1000000 / 2400;  // Manchester twice 1200 bps = 2400 bps = 417 ms

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
    uint8_t DALIaddr = DALI_BROADCAST_DP;

    JsonParserToken val = root[PSTR("cmd")];
    if (val) {
      uint8_t cmd = val.getUInt();
      val = root[PSTR("addr")];
      if (val) {
        uint8_t addr = val.getUInt();
        AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: cmd = %d, addr = %d"), cmd, addr);
        DaliSendData(addr, cmd);
        return true;
      } else {
        return false;
      }
    }
    val = root[PSTR("addr")];
    if (val) {
      uint8_t addr = val.getUInt();
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
