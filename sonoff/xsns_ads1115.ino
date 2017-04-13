/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_I2C
#ifdef USE_ADS1115

/*********************************************************************************************\
 * ADS1115 - 4 channel 16BIT A/D converter
// The ADC input range (or gain) can be changed via the following
// functions, but be careful never to exceed VDD +0.3V max, or to
// exceed the upper and lower limits if you adjust the input range!
// Setting these values incorrectly may destroy your ADC!
//                                                                ADS1015  ADS1115
//                                                                -------  -------
// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
// ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
// ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
// ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
// ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
// ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

\*********************************************************************************************/

#define ADS1115_ADDR1        0x48
Adafruit_ADS1115 ads;


uint8_t ads1115addr, ads1115type = 0;
char ads1115stype[8];


boolean ads1115_detect()
{
  if (ads1115type) return true;

  char log[LOGSZ];
  //uint8_t status;
  boolean success = false;

  ads1115addr = ADS1115_ADDR1;
  ads.begin();
  int16_t val = -1;
  for (byte i = 0; i < 4; i++) {
    int16_t val2 = ads.readADC_SingleEnded(i);
    if (val2 != -1) {val = 0;}
  }

  if (!val) {
    success = true;
    ads1115type = 1;
    strcpy(ads1115stype, "ADS1115");
  }
  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), ads1115stype, ads1115addr);
    addLog(LOG_LEVEL_DEBUG, log);
  } else {
    ads1115type = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void ads1115_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!ads1115type) return;
  for (byte i = 0; i < 4; i++) {
    int16_t val = ads.readADC_SingleEnded(i);
    snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"AnalogInput%d\":%d}"), svalue, ads1115stype, i, val);
  }
  *djson = 1;
#ifdef USE_DOMOTICZ
  domoticz_sensor5(l);
#endif  // USE_DOMOTICZ
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_ADS[] PROGMEM =
  "<tr><th>ADS1115 AnalogInput%d</th><td>%d</td></tr>";

String ads1115_webPresent()
{
  String page = "";
  if (ads1115type) {
    char sensor[80];
    for (byte i = 0; i < 4; i++) {
      int16_t val = ads.readADC_SingleEnded(i);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_ADS, i, val);
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_ADS1115
#endif  // USE_I2C
