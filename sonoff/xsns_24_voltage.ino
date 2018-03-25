/*
  Voltage measure
  Copyright (C) 2018 RL

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

#if !defined USE_ADC_VCC || !defined USE_MQ_SENSOR
#ifdef USE_VOLTAGE
#define VCC_MEASUR 5.0 // value could be betwen for voltage divider ADC in A0 need to be measured as max when 1023
/*********************************************************************************************\
 * sensor supply is 5V the additional voltage devider measurment is needed, and separate supply
 for ESP by i.e. voltage converter 3.3 => 5v
 https://www.google.pl/search?q=3.3V+to+5V+DC-DC+converter&dcr=0&tbm=isch&tbo=u&source=univ&sa=X&ved=0ahUKEwiQ_rW6-q7ZAhUJ3CwKHWGfCxsQsAQIOw&biw=1350&bih=576
 and
 http://www.ohmslawcalculator.com/voltage-divider-calculator
 For cler ADC imput there is: R1=1mohm R2=250kohm (1% tolerancy) devider that gives 1v max output (ESP ADC max value) when input is 5V
 ESP wemeos D1 - devider has  max 3.3v input A0,
 For module without devider you have to count new values for adding devider to ADC
 D1 wemeos that has 330kohm sumarised value of devider - need to be replaced or added additional resistor for double devider
 For 5V you have to use only one serial imput A0 resistor 165kohm betwen output of MQ sensor and imput A0
 it gives 3.299 V for A0.
 See https://escapequotes.net/wp-content/uploads/2017/06/esp8266-wemos-d1-mini-pro-schematics.png
 https://electronics.stackexchange.com/questions/197105/multiple-voltage-dividers
\*********************************************************************************************/



const char HTTP_SNS_ANALOG2[] PROGMEM = "%s{s} " "Pomiar napiecia ADC" "{m}%dmV{e}";                       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>



uint32_t Voltage()
{
  uint32_t analog = 0;
  for (byte i = 0; i < 32; i++) {
    analog += analogRead(A0);
    //delay(1);
    delayMicroseconds(2000);
  }
  analog >>= 5;
  delayMicroseconds(1000);
  analog = (((VCC_MEASUR / 1023) * analog)*1000)-187; //for 1Mohm/250k A0 wemeos -187 is correction - ADC is pure in ESP
  delayMicroseconds(500);
  return analog;
}



void measureVolt(boolean json)
{

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_JSON_VCC "\":%d"), mqtt_data, Voltage());
  #ifdef USE_WEBSERVER
  } else {

    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG2, mqtt_data, Voltage());
  #endif  // USE_WEBSERVER
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_24

boolean Xsns24(byte function)
{
  boolean result = false;


    switch (function) {
      case FUNC_INIT:
      //Voltage();
      break;
     case FUNC_PREP_BEFORE_TELEPERIOD:
       //Voltage();
        break;
      case FUNC_JSON_APPEND:
        measureVolt(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        measureVolt(0);
        break;
#endif // USE_WEBSERVER
    }

  return result;
}


#endif // USE_VOLTAGE
#endif //end #ifndef USE_ADC_VCC
