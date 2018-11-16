/*
  xdrv_17_rcswitch.ino - RF transceiver using RcSwitch library for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
 * RF send and receive using RCSwitch library https://github.com/sui77/rc-switch/
\*********************************************************************************************/

#define XDRV_17             17

#define D_JSON_RF_PROTOCOL "Protocol"
#define D_JSON_RF_BITS "Bits"
#define D_JSON_RF_DATA "Data"

#define D_CMND_RFSEND "RFSend"
#define D_JSON_RF_PULSE "Pulse"
#define D_JSON_RF_REPEAT "Repeat"

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

#define RF_TIME_AVOID_DUPLICATE 1000  // Milliseconds

uint32_t rf_lasttime = 0;

void RfReceiveCheck(void)
{
  if (mySwitch.available()) {

    unsigned long data = mySwitch.getReceivedValue();
    unsigned int bits = mySwitch.getReceivedBitlength();
    int protocol = mySwitch.getReceivedProtocol();
    int delay = mySwitch.getReceivedDelay();

    snprintf_P(log_data, sizeof(log_data), PSTR("RFR: Data %lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);
    AddLog(LOG_LEVEL_DEBUG);

    uint32_t now = millis();
    if ((now - rf_lasttime > RF_TIME_AVOID_DUPLICATE) && (data > 0)) {
      rf_lasttime = now;

      char stemp[16];
      if (Settings.flag.rf_receive_decimal) {      // SetOption28 (0 = hexadecimal, 1 = decimal)
        snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)data);
      } else {
        snprintf_P(stemp, sizeof(stemp), PSTR("\"%lX\""), (uint32_t)data);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_RFRECEIVED "\":{\"" D_JSON_RF_DATA "\":%s,\"" D_JSON_RF_BITS "\":%d,\"" D_JSON_RF_PROTOCOL "\":%d,\"" D_JSON_RF_PULSE "\":%d}}"),
        stemp, bits, protocol, delay);
      MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_RFRECEIVED));
      XdrvRulesProcess();
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_COUNT, data);  // Send data as Domoticz Counter value
#endif  // USE_DOMOTICZ
    }
    mySwitch.resetAvailable();
  }
}

void RfInit(void)
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

boolean RfSendCommand(void)
{
  boolean serviced = true;
  boolean error = false;

  if (!strcasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_RFSEND))) {
    if (XdrvMailbox.data_len) {
      unsigned long data = 0;
      unsigned int bits = 24;
      int protocol = 1;
      int repeat = 10;
      int pulse = 350;

      char dataBufUc[XdrvMailbox.data_len];
      UpperCase(dataBufUc, XdrvMailbox.data);
      StaticJsonBuffer<150> jsonBuf;  // ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(5) + 40 = 134
      JsonObject &root = jsonBuf.parseObject(dataBufUc);
      if (root.success()) {
        // RFsend {"data":0x501014,"bits":24,"protocol":1,"repeat":10,"pulse":350}
        char parm_uc[10];
        data = strtoul(root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_DATA))], NULL, 0);  // Allow decimal (5246996) and hexadecimal (0x501014) input
        bits = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_BITS))];
        protocol = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_PROTOCOL))];
        repeat = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_REPEAT))];
        pulse = root[UpperCase_P(parm_uc, PSTR(D_JSON_RF_PULSE))];
      } else {
        //  RFsend data, bits, protocol, repeat, pulse
        char *p;
        byte i = 0;
        for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 5; str = strtok_r(NULL, ", ", &p)) {
          switch (i++) {
          case 0:
            data = strtoul(str, NULL, 0);  // Allow decimal (5246996) and hexadecimal (0x501014) input
            break;
          case 1:
            bits = atoi(str);
            break;
          case 2:
            protocol = atoi(str);
            break;
          case 3:
            repeat = atoi(str);
            break;
          case 4:
            pulse = atoi(str);
          }
        }
      }

      if (!protocol) { protocol = 1; }
      mySwitch.setProtocol(protocol);
      if (!pulse) { pulse = 350; }      // Default pulse length for protocol 1
      mySwitch.setPulseLength(pulse);
      if (!repeat) { repeat = 10; }     // Default at init
      mySwitch.setRepeatTransmit(repeat);
      if (!bits) { bits = 24; }         // Default 24 bits
      if (data) {
        mySwitch.send(data, bits);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_DONE "\"}"));
      } else {
        error = true;
      }
    } else {
      error = true;
    }
    if (error) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_NO " " D_JSON_RF_DATA ", " D_JSON_RF_BITS ", " D_JSON_RF_PROTOCOL ", " D_JSON_RF_REPEAT " " D_JSON_OR " " D_JSON_RF_PULSE "\"}"));
    }
  }
  else serviced = false; // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

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
