/*
  xsns_12_ads1115_ada.ino - ADS1115 A/D Converter support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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
#ifdef USE_ADS1115
/*********************************************************************************************\
 * ADS1115 - 4 channel 16BIT A/D converter
 *
 * Required library: none but based on Adafruit Industries ADS1015 library
 *
 * I2C Address: 0x48, 0x49, 0x4A or 0x4B
 *
 * The ADC input range (or gain) can be changed via the following
 * defines, but be careful never to exceed VDD +0.3V max, or to
 * exceed the upper and lower limits if you adjust the input range!
 * Setting these values incorrectly may destroy your ADC!
 *                                                                 ADS1115
 *                                                                 -------
 * ADS1115_REG_CONFIG_PGA_6_144V  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
 * ADS1115_REG_CONFIG_PGA_4_096V  // 1x gain   +/- 4.096V  1 bit = 0.125mV
 * ADS1115_REG_CONFIG_PGA_2_048V  // 2x gain   +/- 2.048V  1 bit = 0.0625mV
 * ADS1115_REG_CONFIG_PGA_1_024V  // 4x gain   +/- 1.024V  1 bit = 0.03125mV
 * ADS1115_REG_CONFIG_PGA_0_512V  // 8x gain   +/- 0.512V  1 bit = 0.015625mV
 * ADS1115_REG_CONFIG_PGA_0_256V  // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
\*********************************************************************************************/

#define XSNS_12                         12

#define ADS1115_ADDRESS_ADDR_GND        0x48      // address pin low (GND)
#define ADS1115_ADDRESS_ADDR_VDD        0x49      // address pin high (VCC)
#define ADS1115_ADDRESS_ADDR_SDA        0x4A      // address pin tied to SDA pin
#define ADS1115_ADDRESS_ADDR_SCL        0x4B      // address pin tied to SCL pin

#define ADS1115_CONVERSIONDELAY         (8)       // CONVERSION DELAY (in mS)

/*======================================================================
POINTER REGISTER
-----------------------------------------------------------------------*/
#define ADS1115_REG_POINTER_MASK        (0x03)
#define ADS1115_REG_POINTER_CONVERT     (0x00)
#define ADS1115_REG_POINTER_CONFIG      (0x01)
#define ADS1115_REG_POINTER_LOWTHRESH   (0x02)
#define ADS1115_REG_POINTER_HITHRESH    (0x03)

/*======================================================================
CONFIG REGISTER
-----------------------------------------------------------------------*/
#define ADS1115_REG_CONFIG_OS_MASK      (0x8000)
#define ADS1115_REG_CONFIG_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
#define ADS1115_REG_CONFIG_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
#define ADS1115_REG_CONFIG_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

#define ADS1115_REG_CONFIG_MUX_MASK     (0x7000)
#define ADS1115_REG_CONFIG_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
#define ADS1115_REG_CONFIG_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
#define ADS1115_REG_CONFIG_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
#define ADS1115_REG_CONFIG_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
#define ADS1115_REG_CONFIG_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
#define ADS1115_REG_CONFIG_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
#define ADS1115_REG_CONFIG_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
#define ADS1115_REG_CONFIG_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

#define ADS1115_REG_CONFIG_PGA_MASK     (0x0E00)
#define ADS1115_REG_CONFIG_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3 (default)
#define ADS1115_REG_CONFIG_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
#define ADS1115_REG_CONFIG_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2
#define ADS1115_REG_CONFIG_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
#define ADS1115_REG_CONFIG_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
#define ADS1115_REG_CONFIG_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16

#define ADS1115_REG_CONFIG_MODE_MASK    (0x0100)
#define ADS1115_REG_CONFIG_MODE_CONTIN  (0x0000)  // Continuous conversion mode
#define ADS1115_REG_CONFIG_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

#define ADS1115_REG_CONFIG_DR_MASK      (0x00E0)
#define ADS1115_REG_CONFIG_DR_128SPS    (0x0000)  // 128 samples per second
#define ADS1115_REG_CONFIG_DR_250SPS    (0x0020)  // 250 samples per second
#define ADS1115_REG_CONFIG_DR_490SPS    (0x0040)  // 490 samples per second
#define ADS1115_REG_CONFIG_DR_920SPS    (0x0060)  // 920 samples per second
#define ADS1115_REG_CONFIG_DR_1600SPS   (0x0080)  // 1600 samples per second (default)
#define ADS1115_REG_CONFIG_DR_2400SPS   (0x00A0)  // 2400 samples per second
#define ADS1115_REG_CONFIG_DR_3300SPS   (0x00C0)  // 3300 samples per second
#define ADS1115_REG_CONFIG_DR_6000SPS   (0x00E0)  // 6000 samples per second

#define ADS1115_REG_CONFIG_CMODE_MASK   (0x0010)
#define ADS1115_REG_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1115_REG_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator

#define ADS1115_REG_CONFIG_CPOL_MASK    (0x0008)
#define ADS1115_REG_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1115_REG_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

#define ADS1115_REG_CONFIG_CLAT_MASK    (0x0004)  // Determines if ALERT/RDY pin latches once asserted
#define ADS1115_REG_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1115_REG_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator

#define ADS1115_REG_CONFIG_CQUE_MASK    (0x0003)
#define ADS1115_REG_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1115_REG_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1115_REG_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1115_REG_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)

uint8_t ads1115_type = 0;
uint8_t ads1115_address;
uint8_t ads1115_addresses[] = { ADS1115_ADDRESS_ADDR_GND, ADS1115_ADDRESS_ADDR_VDD, ADS1115_ADDRESS_ADDR_SDA, ADS1115_ADDRESS_ADDR_SCL };
uint8_t ads1115_found[] = {false,false,false,false};
int16_t ads1115_values[4];
//Ads1115StartComparator(channel, ADS1115_REG_CONFIG_MODE_SINGLE);
//Ads1115StartComparator(channel, ADS1115_REG_CONFIG_MODE_CONTIN);
void Ads1115StartComparator(uint8_t channel, uint16_t mode)
{
  // Start with default values
  uint16_t config = mode |
                    ADS1115_REG_CONFIG_CQUE_NONE    | // Comparator enabled and asserts on 1 match
                    ADS1115_REG_CONFIG_CLAT_NONLAT  | // Non Latching mode
                    ADS1115_REG_CONFIG_PGA_6_144V   | // ADC Input voltage range (Gain)
                    ADS1115_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1115_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1115_REG_CONFIG_DR_6000SPS;    // 6000 samples per second

  // Set single-ended input channel
  config |= (ADS1115_REG_CONFIG_MUX_SINGLE_0 + (0x1000 * channel));

  // Write config register to the ADC
  I2cWrite16(ads1115_address, ADS1115_REG_POINTER_CONFIG, config);
}

int16_t Ads1115GetConversion(uint8_t channel)
{
  Ads1115StartComparator(channel, ADS1115_REG_CONFIG_MODE_SINGLE);
  // Wait for the conversion to complete
  delay(ADS1115_CONVERSIONDELAY);
  // Read the conversion results
  I2cRead16(ads1115_address, ADS1115_REG_POINTER_CONVERT);

  Ads1115StartComparator(channel, ADS1115_REG_CONFIG_MODE_CONTIN);
  delay(ADS1115_CONVERSIONDELAY);
  // Read the conversion results
  uint16_t res = I2cRead16(ads1115_address, ADS1115_REG_POINTER_CONVERT);
  return (int16_t)res;
}

/********************************************************************************************/

void Ads1115Detect(void)
{
  uint16_t buffer;
  for (uint8_t i = 0; i < sizeof(ads1115_addresses); i++) {
    if (!ads1115_found[i]) {
      ads1115_address = ads1115_addresses[i];
      if (I2cValidRead16(&buffer, ads1115_address, ADS1115_REG_POINTER_CONVERT) &&
          I2cValidRead16(&buffer, ads1115_address, ADS1115_REG_POINTER_CONFIG)) {
        Ads1115StartComparator(i, ADS1115_REG_CONFIG_MODE_CONTIN);
        ads1115_type = 1;
        ads1115_found[i] = 1;
        AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, "ADS1115", ads1115_address);
      }
    }
  }
}

void Ads1115GetValues(uint8_t address)
{
  uint8_t old_address = ads1115_address;
  ads1115_address = address;
  for (uint8_t i = 0; i < 4; i++) {
    ads1115_values[i] = Ads1115GetConversion(i);
    //AddLog_P2(LOG_LEVEL_INFO, "Logging ADS1115 %02x (%i) = %i", address, i, ads1115_values[i] );
  }
  ads1115_address = old_address;
}

void Ads1115toJSON(char *comma_j)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s{"), mqtt_data,comma_j);
  char *comma = (char*)"";
  for (uint8_t i = 0; i < 4; i++) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"A%d\":%d"), mqtt_data, comma, i, ads1115_values[i]);
    comma = (char*)",";
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
}

void Ads1115toString(uint8_t address)
{
  char label[15];
  snprintf_P(label, sizeof(label), "ADS1115(%02x)", address);

  for (uint8_t i = 0; i < 4; i++) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, label, i, ads1115_values[i]);
  }
}

void Ads1115Show(bool json)
{
  if (!ads1115_type) { return; }

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"ADS1115\":["), mqtt_data);
  }

  char *comma = (char*)"";

  for (uint8_t t = 0; t < sizeof(ads1115_addresses); t++) {
    //AddLog_P2(LOG_LEVEL_INFO, "Logging ADS1115 %02x", ads1115_addresses[t]);
    if (ads1115_found[t]) {
      Ads1115GetValues(ads1115_addresses[t]);
      if (json) {
        Ads1115toJSON(comma);
        comma = (char*)",";
      }
#ifdef USE_WEBSERVER
      else {
        Ads1115toString(ads1115_addresses[t]);
      }
#endif  // USE_WEBSERVER
    }
  }

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s]"), mqtt_data);
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

#endif  // USE_ADS1115
#endif  // USE_I2C
