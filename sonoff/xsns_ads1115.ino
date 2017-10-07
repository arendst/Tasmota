/*
Copyright (c) 2017 Stefan Bode.  All rights reserved.

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
#include <ADS1115.h>

ADS1115 adc0;

uint8_t ads1115addr;
uint8_t ads1115type = 0;
char ads1115stype[8];

uint8_t ads1115addresses[] = {
  ADS1115_ADDRESS_ADDR_GND, // address pin low (GND)
  ADS1115_ADDRESS_ADDR_VDD, // address pin high (VCC)
  ADS1115_ADDRESS_ADDR_SDA, // address pin tied to SDA pin
  ADS1115_ADDRESS_ADDR_SCL // address pin tied to SCL pin
};

int16_t ads1115_getConversion(byte channel)
{
  switch (channel) {
    case 0:
      adc0.getConversionP0GND();
      break;
    case 1:
      adc0.getConversionP1GND();
      break;
    case 2:
      adc0.getConversionP2GND();
      break;
    case 3:
      adc0.getConversionP3GND();
       break;
     }
}

boolean ads1115_detect()
{
  if (ads1115type) {
    return true;
  }
  uint8_t status;
  boolean success = false;

  for (byte i = 0; i < 4; i++) {
    ads1115addr = ads1115addresses[i];
    ADS1115 adc0(ads1115addr);

    snprintf_P(log_data, sizeof(log_data), PSTR("I2C: Probing addr 0x%x for ADS1115."), ads1115addr);
    addLog(LOG_LEVEL_DEBUG);

    if(adc0.testConnection()) {
      adc0.initialize();
      adc0.setGain(ADS1115_PGA_2P048); // Set the gain (PGA) +/-4.096V
      adc0.setRate(ADS1115_RATE_860);
      adc0.setMode(ADS1115_MODE_CONTINUOUS);
      success = true;
      ads1115type = 1;
      strcpy(ads1115stype, "ADS1115");
      break;
    }
  }

  if (success) {
    snprintf_P(log_data, sizeof(log_data), PSTR("I2C: %s found at address 0x%x"), ads1115stype, ads1115addr);
    addLog(LOG_LEVEL_DEBUG);
  }

  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void ads1115_mqttPresent(uint8_t* djson)
{
  if (!ads1115type) {
    return;
  }
  char stemp1[10];
  byte dsxflg = 0;

  for (byte i = 0; i < 4; i++) {
    int32_t val;

    val = ads1115_getConversion(i);

    if (!dsxflg  ) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, \"%s\":{"), mqtt_data, ads1115stype);
      *djson = 1;
      stemp1[0] = '\0';
    }
    dsxflg++;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"AnalogInput%d\":%d"), mqtt_data, stemp1, i, val);
    strcpy(stemp1, ", ");

  }
  *djson = 1;
  if (dsxflg) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  }
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
      int16_t val = ads1115_getConversion(i);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_ADS, i, val);
      page += sensor;
    }
  }
  return page;
}

#endif  // USE_WEBSERVER
#endif  // USE_ADS1115
#endif  // USE_I2C
