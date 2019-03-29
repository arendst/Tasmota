/*
  xdrv_08_serial_bridge.ino - serial bridge support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends and Dániel Zoltán Tolnai

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

#ifdef USE_NODE_BRIDGE
#ifdef USE_SERIAL_BRIDGE
#undef USE_SERIAL_BRIDGE
#endif

/*********************************************************************************************\
 * Serial Bridge using Software Serial library (TasmotaSerial)
\*********************************************************************************************/
#define NODE_BRIDGE_BUFFER_SIZE 160
#define NODE10
//#define NODE4
#define NODE2
#include <TasmotaSerial.h>


enum NodeBridgeCommands { CMND_SSERIALSEND, CMND_SBAUDRATE };
const char kNodeBridgeCommands[] PROGMEM = D_CMND_SSERIALSEND "|" D_CMND_SBAUDRATE;


#ifdef USE_WEBSERVER
#define D_UPDATED "Aktualizacja"
#define D_NO_NODE "Node"
#define D_VOLTAGE_NODE "Napięcie noda"


#ifdef NODE10
//15:36:14 MQT: tele/woda/NODE = {"Time":"2018-05-03T15:36:14","Node":{"No":10,"Volt":4.987,"Fi1":0.98,"Fi2":0.90,"Fi3":0.93,"PCurr":3.16,"PVolt":230.03,"Pow":689.99,"Papp":729.96,"PpMinute":0.0116,"Tank":"LOW"}}

//#define D_UNIT_APPWATT "Var"
#define D_CURRENTP "Prąd suma"
#define D_APPPOWERUSAGE "Moc pozorna"
#define D_ENERGY_MIN "Zużycie na minutę"
#define D_TANK "Szambo"
#define IDX_FLAG_NODE10_TANK 888

const char HTTP_NODE10_SNS[] PROGMEM =
  "%s{s}" D_NO_NODE "{m} %d{e}"
  "{s}" D_UPDATED "{m} %s{e}"
  "{s}" D_VOLTAGE_NODE "{m} %s" D_UNIT_VOLT "{e}"
  "{s}" D_POWER_FACTOR "1{m} %s{e}"
  "{s}" D_POWER_FACTOR "2{m} %s{e}"
  "{s}" D_POWER_FACTOR "3{m} %s{e}"
  "{s}" D_CURRENTP "{m} %s" D_UNIT_AMPERE "{e}"
  "{s}" D_VOLTAGE "{m} %s" D_UNIT_VOLT "{e}"
  "{s}" D_POWERUSAGE "{m} %s" D_UNIT_WATT "{e}"
  //"{s}" D_APPPOWERUSAGE "{m} %sVar{e}"
  "{s}" D_ENERGY_MIN  "{m} %s" D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_TANK "{m} %s{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
char volt10[6];
char *Time10;
char pf1[5];
char pf2[5];
char pf3[5];
char scurrent[6];
char voltage[7];
char spower[10];
//char sapppower[10];
char energypmin[8];
char *tank;

#endif //NODE10


#ifdef NODE2

//15:35:19 MQT: tele/woda/NODE = {"Time":"2018-05-03T15:35:19","Node":{"No":2,"Volt":4.098,"Gas":14347.50}}

#define D_UNIT_GAS "m³"

const char HTTP_NODE2_SNS[] PROGMEM =
  "%s{s}" D_NO_NODE "{m} %d{e}"
  "{s}" D_UPDATED "{m} %s{e}"
  "{s}" D_VOLTAGE_NODE "{m} %s" D_UNIT_VOLT "{e}"
  "{s}" D_GAS "{m} %s" D_UNIT_GAS "{e}";
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

char nodegas[14];
char volt2[6];
char *Time2;

#endif //NODE2

/*
#ifdef NODE4
//18:38:19 MQT: tele/woda/NODE = {"Time":"2018-05-05T18:38:19","Node":{"No":4,"GHumi":86.1,"Humi":30.2,"Temp":18.4,"Light":66.9,"Voltage":4.225,,"DSamm":1,"DS1":-127.00}} (Zachowane)
#define D_HUMIDITY_G "Wilgotność gleby"
#define D_TEMPERATURE_G "Temperatura gleby"
const char HTTP_NODE4_SNS[] PROGMEM =
  "%s{s}" D_NO_NODE "{m} %d{e}"
  "{s}" D_UPDATED "{m} %s{e}"
  "{s}" D_VOLTAGE_NODE "{m} %s" D_UNIT_VOLT "{e}"
  "{s}" D_ILLUMINANCE "{m}%d%%{e}"
  "{s}" D_HUMIDITY "{m} %s%%{e}"
  "{s}" D_HUMIDITY_G "{m} %s%%{e}"
  "{s}" D_TEMPERATURE "{m}%s&deg;%c{e}"
  "{s}" D_TEMPERATURE_G "{m}%s&deg;%c{e}";
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

char humi4[6];
char humig4[6];
char temp4[6];
char tempg4[6];
char light4[6];
char volt4[6];
char *Time4;

#endif //NODE4
*/
int no2=0;
int no10=0;
int no4=0;

char print_buff[200];
StaticJsonBuffer<300> jsonBuffer;

#define NODE_NAME "NODE"
#endif  // USE_WEBSERVER


TasmotaSerial *NodeBridgeSerial;

uint8_t node_bridge_active = 1;
uint8_t node_bridge_in_byte_counter = 0;
unsigned long node_bridge_polling_window = 0;
char node_bridge_buffer[NODE_BRIDGE_BUFFER_SIZE];
//bool result_node =false;
void NodeBridgeInput()
{

  //char topic[]="dupa/dupa/result";
//  topic = "dupa/dupa/result";
  while (NodeBridgeSerial->available()) {
    yield();
    uint8_t node_in_byte = NodeBridgeSerial->read();
    //
    if (node_in_byte > 127) {                   // binary data...
      node_bridge_in_byte_counter = 0;
      NodeBridgeSerial->flush();
      return;
    }
    if (node_in_byte) {
      if ((serial_in_byte_counter < sizeof(node_bridge_buffer) -1) && (node_in_byte != Settings.serial_delimiter)) {  // add char to string if it still fits

        node_bridge_buffer[node_bridge_in_byte_counter++] = node_in_byte;

        node_bridge_polling_window = millis();  // Wait for more data
      } else {
        node_bridge_polling_window = 0;         // Publish now
        break;
      }
    }

  }

  if (node_bridge_in_byte_counter && (millis() > (node_bridge_polling_window + 80))) {
    node_bridge_buffer[node_bridge_in_byte_counter] = 0;  // serial data completed
    //mqtt_data[0] = '\0';
#ifdef USE_WEBSERVER
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\",%s}"), GetDateAndTime(DT_LOCAL).c_str(),node_bridge_buffer);
    //BUFF=mqtt_data;
    //snprintf_P(print_buff, sizeof(print_buff), PSTR("{\"" D_JSON_TIME "\":\"%s\",%s}"), GetDateAndTime(DT_LOCAL).c_str(),node_bridge_buffer);

    UpperCase(print_buff,mqtt_data);
    yield();
    delay(10);
#endif
    //MqttShowSensor();
    MqttPublishPrefixTopic_P(TELE, PSTR("NODE"), Settings.flag.mqtt_sensor_retain);
    delay(10);
    //if (MqttShowSensor()) { MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain); }
    //MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR));
    node_bridge_in_byte_counter = 0;

  }
}

/********************************************************************************************/

void NodeBridgeInit(void)
{
  node_bridge_active = 0;
  if ((pin[GPIO_SBR_RX] < 99) && (pin[GPIO_SBR_TX] < 99)) {
    NodeBridgeSerial = new TasmotaSerial(pin[GPIO_SBR_RX], pin[GPIO_SBR_TX]);
    if (NodeBridgeSerial->begin(Settings.sbaudrate * 1200)) {  // Baud rate is stored div 1200 so it fits into one byte
      node_bridge_active = 1;
      NodeBridgeSerial->flush();
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool NodeBridgeCommand()
{
  char command [CMDSZ];
  bool serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kNodeBridgeCommands);
  if ((CMND_SSERIALSEND == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
    if (XdrvMailbox.data_len > 0) {
      if (1 == XdrvMailbox.index) {
        NodeBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len);
        NodeBridgeSerial->write("\n");
      }
      else if (2 == XdrvMailbox.index) {
        NodeBridgeSerial->write(XdrvMailbox.data, XdrvMailbox.data_len);
      }
      else if (3 == XdrvMailbox.index) {
        NodeBridgeSerial->write(Unescape(XdrvMailbox.data, &XdrvMailbox.data_len), XdrvMailbox.data_len);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
    }
  }
  else if (CMND_SBAUDRATE == command_code) {
    char *p;
    int baud = strtol(XdrvMailbox.data, &p, 10);
    if (baud > 0) {
      baud /= 1200;  // Make it a valid baudrate
      Settings.sbaudrate = (1 == XdrvMailbox.payload) ? SOFT_BAUDRATE / 1200 : baud;
      NodeBridgeSerial->begin(Settings.sbaudrate * 1200);  // Reinitialize serial port with new baud rate
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE, command, Settings.sbaudrate * 1200);
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}


void NodeConvert()
{


  JsonObject& root = jsonBuffer.parseObject(print_buff);
  yield();
  delay(10);
  if (root.success()) {
  int no;
  //result_node=true;
  no = root[NODE_NAME]["NO"];
  //char* Time;

  //delay(1);
  switch (no) {

#ifdef NODE2
    case 2:
      no2=no;
      Time2=strdup(root["TIME"]);
      dtostrf(root[NODE_NAME]["VOLT"],5,3,&volt2[0]);
      delay(1);
      dtostrf(root[NODE_NAME]["GAS"],8,2,&nodegas[0]);

    break;
#endif// NODE2

/*
#ifdef NODE4

//{"Time":"2018-05-05T18:38:19","Node":{"No":4,"GHumi":86.1,"Humi":30.2,"Temp":18.4,"Light":66.9,"Voltage":4.225,,"DSamm":1,"DS1":-127.00}} (Zachowane)


    case 4:
      no4=no;
      Time4=strdup(root["TIME"]);
      dtostrf(root[NODE_NAME]["VOLTAGE"],5,3,&volt4[0]);
      dtostrf(root[NODE_NAME]["LIGHT"],5,2,&light4[0]);
      dtostrf(root[NODE_NAME]["GHUMI"],5,2,&humig4[0]);
      dtostrf(root[NODE_NAME]["HUMI"],5,2,&humi4[0]);
      dtostrf(root[NODE_NAME]["TEMP"],5,2,&temp4[0]);
      dtostrf(root[NODE_NAME]["DS1"],5,2,&tempg4[0]);
      result_node=true;
    break;
#endif// NODE4
*/

#ifdef NODE10

  case 10:
     Time10=strdup(root["TIME"]);
     no10=no;
     //Time10=Time;
     dtostrf(root[NODE_NAME]["VOLT"],5,3,&volt10[0]);
     delay(1);
     dtostrf(root[NODE_NAME]["FI1"],4,2,&pf1[0]);
     delay(1);
     dtostrf(root[NODE_NAME]["FI2"],4,2,&pf2[0]);
     delay(1);
     dtostrf(root[NODE_NAME]["FI3"],4,2,&pf3[0]);
     delay(1);
     dtostrf(root[NODE_NAME]["PCURR"],5,2,&scurrent[0]);
     delay(1);
     dtostrf(root[NODE_NAME]["PVOLT"],6,2,&voltage[0]);
     delay(1);
     dtostrf(root[NODE_NAME]["POW"],9,2,&spower[0]);
     delay(1);
     //dtostrf(root[NODE_NAME]["PAPP"],9,2,&sapppower[0]);
     dtostrf(root[NODE_NAME]["PPMINUTE"],7,4,&energypmin[0]);
     delay(1);
     tank=strdup(root[NODE_NAME]["TANK"]);

   break;
#endif// NODE10

   }


 }

 jsonBuffer.clear();
 delay(5);
 memset(&print_buff[0], 0, sizeof(print_buff));
 delay(5);
 print_buff[0]= '\0';

}

void NodeShow(bool json)
{

if (json) {
    if( node_bridge_buffer[0] != '\0' &&(no2 !=0 || no10 !=0 || no4 !=0)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,%s"),mqtt_data, node_bridge_buffer);
#ifdef USE_DOMOTICZ
            if (0 == tele_period ){

#ifdef NODE2

          if (no2==2){
          DomoticzSensor(DZ_AIRQUALITY,atof(nodegas));
          delay(5);
          DomoticzSensor(DZ_CURRENT, volt2);  // For Node2 Voltage
        }

#endif

#ifdef NODE10
         if (no10==10) {
          char data[70];
          int idx=IDX_FLAG_NODE10_TANK;
           snprintf_P(data, sizeof(data), PSTR("{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\",\"Battery\":%d,\"RSSI\":%d}"),IDX_FLAG_NODE10_TANK,tank, DomoticzBatteryQuality(),DomoticzRssiQuality());
           //  {"idx":IDX_FLAG,"nvalue":688,"Battery":200,"RSSI":10}
           MqttClient.publish(domoticz_in_topic, data, 1);
           delay(10);
           data[0]='\0';
         }
#endif

            }
#endif  // USE_DOMOTICZ
        memset(&node_bridge_buffer[0], 0, sizeof(node_bridge_buffer));
        delay(5);
        node_bridge_buffer[0]= '\0';
     }
#ifdef USE_WEBSERVER
 } else {
#ifdef NODE10

  if(no10==10) {
    //yield();
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_NODE10_SNS,mqtt_data,
    no10,
    Time10,
    volt10,
    pf1,
    pf2,
    pf3,
    scurrent,
    voltage,
    spower,
    //sapppower,
    energypmin,
    tank);
  }
#endif //NODE10

#ifdef NODE2
  if (no2==2){
  //yield();
  snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_NODE2_SNS,mqtt_data,
  no2,
  Time2,
  volt2,
  nodegas);
  }

#endif //NODE2
/*
#ifdef NODE4

if(no4==4) {
  //yield();
  snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_NODE4_SNS,mqtt_data,
  no4,
  Time4,
  volt4,
  light4,
  humi4,
  humig4,
  temp4,
  tempg4
  );
}


#endif
*/
#endif //END WEBSERVER
 }


}




/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_96

bool Xdrv96(byte function)
{
  bool result = false;

  if (node_bridge_active) {
    switch (function) {
      case FUNC_INIT:
        NodeBridgeInit();
        break;
      case FUNC_LOOP:
        NodeBridgeInput();
        break;

      case FUNC_COMMAND:
        result = NodeBridgeCommand();
        break;
    }
  }
  return result;
}


#define XSNS_96

bool Xsns96(uint8_t function)
{
  bool result = false;


    switch (function) {
      case FUNC_FUNC_INIT:

        break;
      case FUNC_EVERY_SECOND:
        //NodeShow(0);
        break;
      case FUNC_JSON_APPEND:
        NodeConvert();
        NodeShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:

        NodeShow(0);
        break;
#endif  // USE_WEBSERVER

    }

  return result;
}

#endif // USE_NODE_BRIDGE
