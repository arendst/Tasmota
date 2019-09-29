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
#define NODE_BRIDGE_BUFFER_SIZE 45

#ifdef USE_WEBSERVER
#define D_UPDATED "Aktualizacja Gaz"
#define D_VOLTAGE_NODE "Napięcie noda"


//15:35:19 MQT: tele/woda/NODE = {"Time":"2018-05-03T15:35:19","Node":{"No":2,"Volt":4.098,"Gas":14347.50}}
//                                        13        22 26       34    41

#define D_UNIT_GAS "m³"

const char HTTP_NODE2_SNS[] PROGMEM =

  "%s{s}" D_UPDATED "{m} %s{e}"
  "{s}" D_VOLTAGE_NODE "{m} %s" D_UNIT_VOLT "{e}"
  "{s}" D_GAS "{m} %s" D_UNIT_GAS "{e}";
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
char mqtt_buff[NODE_BRIDGE_BUFFER_SIZE];
char nodegas[9];
char volt2[6];
char Time2[20];
uint8_t no2=0;
uint8_t flag=0;
uint8_t flag2=0;

#endif  // USE_WEBSERVER



void HardBridgeInput()

{
  flag=(uint8_t)(serial_in_buffer[13] -48);
  char check = serial_in_buffer[16]; //sizeof(log_data),PSTR("%d-%d"),serial_in_buffer[13],flag);
  //AddLog(LOG_LEVEL_INFO);
  if (flag==2 && check == 'V') {

  snprintf_P(mqtt_buff,sizeof(mqtt_buff),PSTR("%s"),serial_in_buffer);
  //BUFF=mqtt_data;
  //snprintf_P(print_buff, sizeof(print_buff), PSTR("{\"" D_JSON_TIME "\":\"%s\",%s}"), GetDateAndTime(DT_LOCAL).c_str(),node_bridge_buffer);
  //MqttPublishPrefixTopic_P(TELE, PSTR("NODE"),0);
  //"Node":{"No":2,"Volt":4.182,"Gas":14385.96}"
  delay(2);
      no2=(uint8_t)(mqtt_buff[13] -48);

      snprintf_P(Time2, sizeof(Time2), PSTR("%s"), GetDateAndTime(DT_LOCAL).c_str());
      char test[] ={ mqtt_buff[22],mqtt_buff[23],mqtt_buff[24],mqtt_buff[25],mqtt_buff[26]};
      snprintf_P(volt2,sizeof(volt2),PSTR("%s"),test);
      char test2[] ={mqtt_buff[34],mqtt_buff[35],mqtt_buff[36],mqtt_buff[37],mqtt_buff[38],mqtt_buff[39],mqtt_buff[40],mqtt_buff[41]};
      snprintf_P(nodegas,sizeof(nodegas),PSTR("%s"),test2);

     //Serial.flush();
     flag2 = 1;
   } else {

  //Serial.flush();

  }

}
/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void NodeShow(bool json)
{

if (json) {

    if( flag==2 && flag2 == 1) {

        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,%s"),mqtt_data, mqtt_buff);
        mqtt_buff[0]='\0';
        //Serial.flush();
#ifdef USE_DOMOTICZ
        if (0 == tele_period ){

          DomoticzSensor(DZ_TEMP,nodegas);
          DomoticzSensor(DZ_CURRENT, volt2);  // For Node2 Voltage


       }
#endif  // USE_DOMOTICZ
        //memset(&node_bridge_buffer[0], 0, sizeof(node_bridge_buffer));

        flag=0;
        flag2=0;
     }
#ifdef USE_WEBSERVER
 } else {

  if (no2==2 && nodegas[0] != '\0'){
  snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_NODE2_SNS,mqtt_data,
  Time2,
  volt2,
  nodegas);
  }


#endif //END WEBSERVER
 }


}




/*********************************************************************************************\
 * Interface
\*********************************************************************************************/


#define XSNS_96 96

bool Xsns96(uint8_t function)
{
  bool result = false;


     switch (function) {
      case FUNC_INIT:

        break;
      case FUNC_EVERY_SECOND:
        HardBridgeInput();
        break;

      case FUNC_JSON_APPEND:

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
