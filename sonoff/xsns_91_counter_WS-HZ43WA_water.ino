/*
  xsns_91_counter_WS-HZ43WA_water.ino modyfied xsns_01_counter.ino by RL - Counter sensors (water meters, electricity meters etc.), designed for HZ43WA sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Maarten Damen and Theo Arends

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

/*********************************************************************************************\
 * Counter sensors (water meters, electricity meters etc.)
 *Modyfied by RL Model: HZ43WA HZ43WB
 You have to use COUNTER Sensor 1, and set proper COUNTERDEVIDER - by default is set for 10L = 4100 pulse. Additionaly there is idx value for Domoticz nideed before compilation in IDX_FLAG and DELIMETER for m3.
 See:
 http://www.mojo-hk.com/mjhz43wb-245lmin-p-136
 F = [8.1Q-5] ±10%,F dla wartość chwilowa impulsu (HZ), Q dla przemijające przepływu (L/min) 4100 pulses for 10L
 *https://pl.aliexpress.com/item/Free-Shipping-Copper-G3-4-Water-Flow-Hall-Sensor-Switch-Flow-Meter-Flowmeter-Counter-2-45L/32223530631.html?spm=a2g17.search0104.3.2.231673f9tGvynE&ws_ab_test=searchweb0_0,searchweb201602_1_10152_10151_10065_10068_10344_10342_10343_10340_10341_10698_10696_10084_10083_10618_10304_10307_10301_10059_308_100031_10103_10624_10623_10622_10621_10620,searchweb201603_2,ppcSwitch_5&algo_expid=67cdaaff-761b-4abc-a822-6508e3fddd68-0&algo_pvid=67cdaaff-761b-4abc-a822-6508e3fddd68&priceBeautifyAB=0
\*********************************************************************************************/

#ifdef COUNTER_WATER
#if defined D_COUNTER
#undef D_COUNTER
//set your language
#define D_COUNTER "Zużycie wody"
#endif
#define DELIMETER (100)
//set counterdevider
//#if COUNTERDEVIDER !=4100
//#undef COUNTERDEVIDER
#define COUNTERDEVIDER_HZ 4100

//#endif
#ifdef USE_DOMOTICZ
#define IDX_FLAG (666) //set idx flag
#endif  // USE_DOMOTICZ

#ifdef USE_WEBSERVER


  const char HTTP_SNS_COUNTER_HZ43WB[] PROGMEM =
    "%s{s}" D_COUNTER "{m}%sm³{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

#endif  // USE_WEBSERVER


void hz43wb_CounterShow(boolean json)
{


  char countingd[10];
  dtostrf((((float)RtcSettings.pulse_counter[0]/COUNTERDEVIDER_HZ)/DELIMETER),4,2,countingd);

  if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"WATER\":{\"CW\":%s}"), mqtt_data,countingd);


#ifdef USE_DOMOTICZ
        if (0 == tele_period) {

           char data[70];
           int idx=IDX_FLAG;
            snprintf_P(data, sizeof(data), PSTR("{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\",\"Battery\":%d,\"RSSI\":%d}"),IDX_FLAG,countingd, DomoticzBatteryQuality(),DomoticzRssiQuality());
            //  {"idx":IDX_FLAG,"nvalue":688,"Battery":200,"RSSI":10}
            MqttClient.publish(domoticz_in_topic, data, 1);

        }
#endif  // USE_DOMOTICZ

#ifdef USE_WEBSERVER
      } else {

        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_COUNTER_HZ43WB, mqtt_data, countingd);

#endif  // USE_WEBSERVER

  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_91

boolean Xsns91(byte function)
{
  boolean result = false;

  switch (function) {

    case FUNC_JSON_APPEND:
      hz43wb_CounterShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_APPEND:
      hz43wb_CounterShow(0);
      break;
#endif  // USE_WEBSERVER

  }
  return result;
}
#endif //COUNTER_WATER
