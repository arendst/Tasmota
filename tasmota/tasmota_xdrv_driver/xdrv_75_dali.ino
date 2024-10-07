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
  0.1.0.1 20241007  update    - To stablizie communication send Dali datagram twice like Busch-Jaeger does 
                              - Change DaliPower 0..2 to act like Tasmota Power (Off, On, Toggle)
                              - Keep last Dimmer value as default power on
  0.1.0.0 20241006  rewrite   - Add support for ESP8266
                              - Fix decoding of received Dali 1 data
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

//#define DALI_DEBUG
#ifndef DALI_DEBUG_PIN
#define DALI_DEBUG_PIN      27
#endif

#define BROADCAST_DP        0b11111110         // 0xFE = 254

#define DALI_TOPIC "DALI"
#define D_PRFX_DALI "Dali"

const char kDALICommands[] PROGMEM = D_PRFX_DALI "|"  // Prefix
  "|" D_CMND_POWER "|" D_CMND_DIMMER;

void (* const DALICommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliPower, &CmndDaliDimmer };

struct DALI {
  uint32_t bit_time;
  uint16_t received_dali_data;                 // Data received from DALI bus
  uint8_t pin_rx;
  uint8_t pin_tx;
  uint8_t address;
  uint8_t command;
  uint8_t dimmer;
  bool power;
  bool input_ready;
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

#define DALI_WAIT_RCV { while (ESP.getCycleCount() < (wait + start)); wait += bit_time; }

void IRAM_ATTR DaliReceiveData(void);
void DaliReceiveData(void) {
  if (Dali->input_ready) { return; }
  uint32_t start = ESP.getCycleCount();
  uint32_t bit_time = Dali->bit_time;
  // Advance the starting point for the samples but compensate for the
  // initial delay which occurs before the interrupt is delivered
  uint32_t wait = bit_time / 2;
  int bit_state = 0; 
  bool dali_read;
  uint32_t received_dali_data = 0;

  DALI_WAIT_RCV;
  DALI_WAIT_RCV;                               // Start bit
  for (uint32_t i = 0; i < 32; i++) {
    DALI_WAIT_RCV;
    if (abs(bit_state) <= 2) {                 // Manchester encoding max 2 consequtive equal bits
      dali_read = digitalRead(Dali->pin_rx);
#ifdef DALI_DEBUG
      digitalWrite(DALI_DEBUG_PIN, i&1);       // Add LogicAnalyzer poll indication
#endif  // DALI_DEBUG
      bit_state += (dali_read) ? 1 : -1;
      if (i &1) {
        uint32_t j = i >>1;
        received_dali_data |= ((DALI_IN_INVERT) ? !dali_read : dali_read << (15 -j));
      }
    }
  }
  DALI_WAIT_RCV;
  DALI_WAIT_RCV;                               // Stop bit

  if (abs(bit_state) <= 2) {                   // Valid Manchester encoding
    Dali->received_dali_data = received_dali_data;
    Dali->input_ready = true;                  // Valid data received
  }

#ifdef ESP8266
  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << Dali->pin_rx);
#endif  // ESP8266
}

/*************** S E N D * P R O C E D U R E ***************/

#define DALI_WAIT_SND { while (ESP.getCycleCount() < (wait + start)) optimistic_yield(1); wait += bit_time; }  // Watchdog timeouts

void DaliDigitalWrite(bool pin_value) {
  digitalWrite(Dali->pin_tx, (pin_value == DALI_OUT_INVERT) ? LOW : HIGH);
}

void DaliSendDataOnce(uint16_t send_dali_data) {
  uint32_t bit_time = Dali->bit_time;
  uint32_t wait = bit_time;
//  digitalWrite(Dali->pin_tx, HIGH);            // Already in HIGH mode
  uint32_t start = ESP.getCycleCount();

  // Settling time between forward and backward frame
  for (uint32_t i = 0; i < 8; i++) {
    DALI_WAIT_SND;
  }
  // Start bit;
  DaliDigitalWrite(LOW);
  DALI_WAIT_SND;
  DaliDigitalWrite(HIGH);
  DALI_WAIT_SND;
  for (uint32_t i = 0; i < 16; i++) {
    // Bit value (edge) selection
    bool bit_value = (bool)((send_dali_data >> (15 - i)) & 0x01);  // MSB first
    // Every half bit -> Manchester coding
    DaliDigitalWrite(bit_value ? LOW : HIGH);  // Manchester 
    DALI_WAIT_SND;
    DaliDigitalWrite(bit_value ? HIGH : LOW);  // Value
    DALI_WAIT_SND;
  }
  // Stop bit
  DaliDigitalWrite(HIGH);
}

void DaliSendData(uint8_t firstByte, uint8_t secondByte) {
  Dali->address = firstByte;
  Dali->command = secondByte;
  if (BROADCAST_DP == firstByte) {
    Dali->power = (secondByte);                // State
    if (Dali->power) {
      Dali->dimmer = secondByte;               // Value
    }
  }
  uint16_t send_dali_data = firstByte << 8;
  send_dali_data += secondByte & 0xff;

  DaliDisableRxInterrupt();
  DaliSendDataOnce(send_dali_data);            // Takes 14.5 ms
  delay(15);                                   // As used by Busch-Jaeger
  DaliSendDataOnce(send_dali_data);            // Takes 14.5 ms
  delay(1);                                    // Block response
  DaliEnableRxInterrupt();
}

void DaliPower(uint8_t val) {
  DaliSendData(BROADCAST_DP, val);
}

/***********************************************************/

void ResponseAppendDali(void) {
  ResponseAppend_P(PSTR("\"" D_PRFX_DALI "\":{\"Power\":\"%s\",\"Dimmer\":%d,\"Address\":%d,\"Command\":%d}"), 
    GetStateText(Dali->power), Dali->dimmer, Dali->address, Dali->command);
}

void ResponseDali(void) {
  Response_P(PSTR("{"));
  ResponseAppendDali();
  ResponseJsonEnd();
}

void DaliInput(void) {
  if (Dali->input_ready) {
    Dali->address = Dali->received_dali_data >> 8;
    Dali->command = Dali->received_dali_data;
    if (BROADCAST_DP == Dali->address) {
      Dali->power = (Dali->command);           // State
      if (Dali->power) {
        Dali->dimmer = Dali->command;          // Value
      }
    }

//    AddLog(LOG_LEVEL_DEBUG, PSTR("DLI: Received 0x%04X"), Dali->received_dali_data);
    ResponseDali();
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_PRFX_DALI));

    Dali->input_ready = false;
  }
}

void DaliInit(void) {
  if (!PinUsed(GPIO_DALI_TX) || !PinUsed(GPIO_DALI_RX)) { return; }

  Dali = (DALI*)calloc(sizeof(DALI), 1);
  if (!Dali) { return; }

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

  Dali->bit_time = ESP.getCpuFreqMHz() * 1000000 / 2400;  // Manchester twice 1200 bps

  DaliEnableRxInterrupt();
}

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
  uint8_t DALIaddr = BROADCAST_DP;

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
    uint8_t DALIaddr = BROADCAST_DP;

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
  // DaliDimmer 0..254 - Set power off or dimmer state
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 254)) {
    DaliPower(XdrvMailbox.payload);
  }
  ResponseDali();
}

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

  if (FUNC_INIT == function) {
    DaliInit();
  }
  else if (Dali) {
    switch (function) {
      case FUNC_LOOP:
        DaliInput();
        break;
      case FUNC_MQTT_DATA:
        result = DaliMqtt();
        break;
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
