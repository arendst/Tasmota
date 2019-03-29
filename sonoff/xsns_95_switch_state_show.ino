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
//stb
#ifdef SWITCH_STATE_SHOW

#ifdef USE_WEBSERVER

const char HTTP_SWITCH_STATE[] PROGMEM = "%s{s}" D_SENSOR_SWITCH "%d{m} %s{e}";


#endif  // USE_WEBSERVER



void SwitchStateShow(bool json)
{

      if (json) {

  for (byte i = 0; i < MAX_SWITCHES; i++) {
    if (pin[GPIO_SWT1 +i] < 99) {
         bool swm = ((FOLLOW_INV == Settings.switchmode[i]) || (PUSHBUTTON_INV == Settings.switchmode[i]) || (PUSHBUTTONHOLD_INV == Settings.switchmode[i]));
          char topic[25];
          snprintf_P(topic,sizeof(topic),PSTR("stat/%s/SWITCH%d"),Settings.switch_topic,i+1);
          char *test=GetStateText(swm ^ lastwallswitch[i]);
          MqttClient.publish(topic,test,true);
        }
    }


#ifdef USE_WEBSERVER
      } else {

  for (byte i = 0; i < MAX_SWITCHES; i++) {
    if (pin[GPIO_SWT1 +i] < 99) {
      bool swm = ((FOLLOW_INV == Settings.switchmode[i]) || (PUSHBUTTON_INV == Settings.switchmode[i]) || (PUSHBUTTONHOLD_INV == Settings.switchmode[i]));
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SWITCH_STATE, mqtt_data,i +1, GetStateText(swm ^ lastwallswitch[i]));

     }
 }

#endif  // USE_WEBSERVER
 }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_95

bool Xsns95(uint8_t function)
{
  bool result = false;

  switch (function) {

    case FUNC_JSON_APPEND:
      SwitchStateShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_APPEND:
      SwitchStateShow(0);
      break;
#endif  // USE_WEBSERVER

  }
  return result;
}


#endif //SWITCH_WATER
