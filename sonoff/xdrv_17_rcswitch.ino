/*
  xdrv_17_rcswitch.ino - RF transceiver using RcSwitch library for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#ifdef USE_RC_SWITCH
/*********************************************************************************************\
 * RF send and receive using RCSwitch library
\*********************************************************************************************/

#define D_JSON_RF_PROTOCOL "Protocol"
#define D_JSON_RF_BITS "Bits"
#define D_JSON_RF_DATA "Data"

#define D_CMND_RFSEND "RFSend"
#define D_JSON_RF_PULSE "Pulse"
#define D_JSON_RF_REPEAT "Repeat"

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

#define RF_TIME_AVOID_DUPLICATE 500 // Milliseconds

unsigned long rf_lasttime = 0;

void RfReceiveCheck()
{
  if (mySwitch.available()) {

    unsigned long value = mySwitch.getReceivedValue();
    unsigned int bit_length = mySwitch.getReceivedBitlength();
    unsigned int delay = mySwitch.getReceivedDelay();
    unsigned int protocol = mySwitch.getReceivedProtocol();

    snprintf_P(log_data, sizeof(log_data), PSTR("RFR: BitLen %d, Delay %d, Protocol %d, Value %lX (%u)"),
      bit_length, delay, protocol, value, value);
    AddLog(LOG_LEVEL_DEBUG);

    unsigned long now = millis();
    if ((now - rf_lasttime > RF_TIME_AVOID_DUPLICATE) && (value > 0)) {
      rf_lasttime = now;

      char stemp[16];
      if (Settings.flag.rf_receive_decimal) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)value);
      } else {
        snprintf_P(stemp, sizeof(stemp), PSTR("\"%lX\""), (uint32_t)value);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_RFRECEIVED "\":{\"" D_JSON_RF_PROTOCOL "\":%d,\"" D_JSON_RF_BITS "\":%d,\"" D_JSON_RF_DATA "\":%s}}"),
          protocol, bit_length, stemp);
      MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_RFRECEIVED));
      XdrvRulesProcess();
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_COUNT, value);  // Send value as Domoticz Counter value
#endif  // USE_DOMOTICZ
    }
    mySwitch.resetAvailable();
  }
}

void RfInit()
{
  if (pin[GPIO_RFSEND] < 99) {
    mySwitch.enableTransmit(pin[GPIO_RFSEND]);
  }
  if (pin[GPIO_RFRECV] < 99) {
    mySwitch.enableReceive(pin[GPIO_RFRECV]);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

/*
 * ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
 RFsend:
 { "protocol":1, "pulse":320, "repeat":15, "bits":24, "data":551502015 }
*/

boolean RfSendCommand()
{
  boolean serviced = true;

  boolean error = false;
  char dataBufUc[XdrvMailbox.data_len];

  uint32_t protocol = 0;
  uint32_t pulse = 0;
  uint32_t repeat = 0;
  uint32_t bits = 0;
  uint32_t data = 0;

  UpperCase(dataBufUc, XdrvMailbox.data);
  if (!strcasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_RFSEND))) {
    if (XdrvMailbox.data_len) {
      StaticJsonBuffer<128> jsonBuf;
      JsonObject &root = jsonBuf.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_INVALID_JSON "\"}")); // JSON decode failed
      }
      else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_DONE "\"}"));

        char parm_uc[10];
        protocol = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_PROTOCOL))];
        pulse = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_PULSE))];
        repeat = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_REPEAT))];
        bits = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_BITS))];
        data = strtoul(root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_DATA))], NULL, 0);

        if (!protocol) { protocol = 1; }
        mySwitch.setProtocol(protocol);
        if (!pulse) { pulse = 350; }      // Default pulse length for protocol 1
        mySwitch.setPulseLength(pulse);
        if (!repeat) { repeat = 10; }     // Default at init
        mySwitch.setRepeatTransmit(repeat);
        if (!bits) { bits = 24; }         // Default 24 bits
        if (data) {
          mySwitch.send(data, bits);
        }
        else {
          error = true;
        }
      }
    }
    else {
      error = true;
    }
    if (error) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_NO " " D_JSON_RF_PROTOCOL ", " D_JSON_RF_PULSE ", " D_JSON_RF_REPEAT ", " D_JSON_RF_BITS " " D_JSON_OR " " D_JSON_RF_DATA "\"}"));
    }
  }
  else serviced = false; // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_17

boolean Xdrv17(byte function)
{
  boolean result = false;

  if ((pin[GPIO_RFSEND] < 99) || (pin[GPIO_RFRECV] < 99)) {
    switch (function) {
      case FUNC_INIT:
        RfInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        if (pin[GPIO_RFRECV] < 99) {
          RfReceiveCheck();
        }
        break;
      case FUNC_COMMAND:
        if (pin[GPIO_RFSEND] < 99) {
          result = RfSendCommand();
        }
        break;
    }
  }
  return result;
}

#endif  // USE_RC_SWITCH
