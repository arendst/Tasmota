/*
  xsns_12_ads1115.ino - ADS1x15 A/D Converter support for Sonoff-Tasmota

  Copyright (C) 2019  Stefan Bode and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_ADS1115_I2CDEV
/*********************************************************************************************\
 * ADS1115 - 4 channel 16BIT A/D converter
 *
 * Required library: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/ADS1115
 *
 * I2C Address: 0x48, 0x49, 0x4A or 0x4B
 *
 * The ADC input range (or gain) can be changed via the following
 * functions, but be careful never to exceed VDD +0.3V max, or to
 * exceed the upper and lower limits if you adjust the input range!
 * Setting these values incorrectly may destroy your ADC!
 *                                                                ADS1015  ADS1115
 *                                                                -------  -------
 * ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
 * ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
 * ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
 * ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
 * ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
 * ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
\*********************************************************************************************/

#define XSNS_12             12

#include <ADS1115.h>

ADS1115 adc0;

uint8_t ads1115_type = 0;
uint8_t ads1115_address;
uint8_t ads1115_addresses[] = {
  ADS1115_ADDRESS_ADDR_GND,  // address pin low (GND)
  ADS1115_ADDRESS_ADDR_VDD,  // address pin high (VCC)
  ADS1115_ADDRESS_ADDR_SDA,  // address pin tied to SDA pin
  ADS1115_ADDRESS_ADDR_SCL   // address pin tied to SCL pin
};

int16_t Ads1115GetConversion(uint8_t channel)
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

/********************************************************************************************/

void Ads1115Detect(void)
{
  if (ads1115_type) {
    return;
  }

  for (uint8_t i = 0; i < sizeof(ads1115_addresses); i++) {
    ads1115_address = ads1115_addresses[i];
    ADS1115 adc0(ads1115_address);
    if (adc0.testConnection()) {
      adc0.initialize();
      adc0.setGain(ADS1115_PGA_6P144);        // Set the gain (PGA) +/-6.144V
      adc0.setRate(ADS1115_RATE_860);
      adc0.setMode(ADS1115_MODE_CONTINUOUS);
      ads1115_type = 1;
      AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, "ADS1115", ads1115_address);
      break;
    }
  }
}

void Ads1115Show(bool json)
{
  if (ads1115_type) {
    char stemp[10];

    uint8_t dsxflg = 0;
    for (uint8_t i = 0; i < 4; i++) {
      int16_t adc_value = Ads1115GetConversion(i);

      if (json) {
        if (!dsxflg  ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"ADS1115\":{"), mqtt_data);
          stemp[0] = '\0';
        }
        dsxflg++;
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"A%d\":%d"), mqtt_data, stemp, i, adc_value);
        strlcpy(stemp, ",", sizeof(stemp));
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, "ADS1115", i, adc_value);
#endif  // USE_WEBSERVER
      }
    }
    if (json) {
      if (dsxflg) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns12(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        Ads1115Detect();
        break;
      case FUNC_JSON_APPEND:
        Ads1115Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Ads1115Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_ADS1115_I2CDEV
#endif  // USE_I2C
