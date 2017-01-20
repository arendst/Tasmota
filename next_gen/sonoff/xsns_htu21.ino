/*
 Copyright (c) 2017 Heiko Krupp.  All rights reserved.

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
#ifdef USE_HTU
/*********************************************************************************************\
 * HTU21 - Temperature and Humidy
 *
 * Source: Heiko Krupp
\*********************************************************************************************/

#define HTU21_ADDR          0x40

#define HTU21_CHIPID        0x32

#define HTU21_READTEMP      0xE3
#define HTU21_READHUM       0xE5
#define HTU21_WRITEREG      0xE6
#define HTU21_READREG       0xE7
#define HTU21_RESET         0xFE
#define HTU21_HEATER_WRITE  0x51
#define HTU21_HEATER_READ   0x11
#define HTU21_SERIAL2_READ1 0xFC    /* Read 3rd two Serial bytes */
#define HTU21_SERIAL2_READ2 0xC9    /* Read 4th two Serial bytes */

#define HTU21_HEATER_ON     0x04
#define HTU21_HEATER_OFF    0xFB

#define HTU21_RES_RH12_T14  0x00	// Default
#define HTU21_RES_RH8_T12   0x01
#define HTU21_RES_RH10_T13  0x80
#define HTU21_RES_RH11_T11  0x81

#define HTU21_MAX_HUM       16		// 16ms max time
#define HTU21_MAX_TEMP      50		// 50ms max time

#define HTU21_CRC8_POLYNOM  0x13100

uint8_t htuaddr, htutype = 0;
char htustype[7];

uint8_t check_crc8(uint16_t data)
{
  for (uint8_t bit = 0; bit < 16; bit++)
  {
    if (data & 0x8000)
      data =  (data << 1) ^ HTU21_CRC8_POLYNOM;
    else
      data <<= 1;
  }
  return data >>= 8;
}

uint8_t htu21_readDeviceID(void)
{
  uint16_t deviceID = 0;
  uint8_t checksum = 0;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_SERIAL2_READ1);
  Wire.write(HTU21_SERIAL2_READ2);
  Wire.endTransmission();

  Wire.requestFrom(HTU21_ADDR, 3);
  deviceID  = Wire.read() << 8;
  deviceID |= Wire.read();
  checksum  = Wire.read();
  if (check_crc8(deviceID) == checksum) {
    deviceID = deviceID >> 8;
  } else {
    deviceID = 0;
  }
  return (uint8_t)deviceID;
}

void htu21_setRes(uint8_t resolution)
{
  uint8_t current = i2c_read8(HTU21_ADDR, HTU21_READREG);
  current &= 0x7E;                  // Replace current resolution bits with 0
  current |= resolution;            // Add new resolution bits to register
  i2c_write8(HTU21_ADDR, HTU21_WRITEREG, current);
}

void htu21_reset(void)
{
  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_RESET);
  Wire.endTransmission();
  delay(15);                // Reset takes 15ms
}

void htu21_heater(uint8_t heater)
{
  uint8_t current = i2c_read8(HTU21_ADDR, HTU21_READREG);

  switch(heater)
  {
    case HTU21_HEATER_ON  : current |= heater;
                            break;
    case HTU21_HEATER_OFF : current &= heater;
                            break;
    default               : current &= heater;
                            break;
  }
  i2c_write8(HTU21_ADDR, HTU21_WRITEREG, current);
}

boolean htu21_init()
{
  htu21_reset();
  htu21_heater(HTU21_HEATER_OFF);
  htu21_setRes(HTU21_RES_RH12_T14);
  return true;
}

float htu21_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

float htu21_readHumidity(void)
{
  uint8_t  checksum=0;
  uint16_t sensorval=0;
  float    humidity=0.0;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_READHUM);
  if(Wire.endTransmission() != 0) return 0.0; // In case of error
  delay(HTU21_MAX_HUM);                       // HTU21 time at max resolution

  Wire.requestFrom(HTU21_ADDR, 3);
  if(3 <= Wire.available())
  {
    sensorval = Wire.read() << 8;             // MSB
    sensorval |= Wire.read();                 // LSB
    checksum = Wire.read();
  }
  if(check_crc8(sensorval) != checksum) return 0.0; // Checksum mismatch

  sensorval ^= 0x02;      // clear status bits
  humidity = 0.001907 * (float)sensorval - 6;

  if(humidity>100) return 100.0;
  if(humidity<0) return 0.01;

  return humidity;
}

float htu21_readTemperature(bool S)
{
  uint8_t  checksum=0;
  uint16_t sensorval=0;
  float t;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_READTEMP);
  if(Wire.endTransmission() != 0) return 0.0; // In case of error
  delay(HTU21_MAX_TEMP);              // HTU21 time at max resolution

  Wire.requestFrom(HTU21_ADDR, 3);
  if(3 == Wire.available())
  {
    sensorval = Wire.read() << 8;         // MSB
    sensorval |= Wire.read();             // LSB
    checksum = Wire.read();
  }
  if(check_crc8(sensorval) != checksum) return 0.0; // Checksum mismatch

  t = (0.002681 * (float)sensorval - 46.85);
  if(S) t = htu21_convertCtoF(t);
  return t;
}

float htu21_compensatedHumidity(float humidity, float temperature)
{
  if(humidity == 0.00 && temperature == 0.00) return 0.0;
  if(temperature > 0.00 && temperature < 80.00)
    return (-0.15)*(25-temperature)+humidity;
}

uint8_t htu_detect()
{
  if (htutype) return true;

  char log[LOGSZ];
  boolean success = false;

  htuaddr = HTU21_ADDR;
  htutype = htu21_readDeviceID();
  snprintf_P(htustype, sizeof(htustype), PSTR("HTU"));
  switch (htutype) {
  case HTU21_CHIPID:
    success = htu21_init();
    snprintf_P(htustype, sizeof(htustype), PSTR("HTU21"));
  }
  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), htustype, htuaddr);
    addLog(LOG_LEVEL_DEBUG, log);
  } else {
    htutype = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void htu_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!htutype) return;

  char stemp1[10], stemp2[10];

  float t = htu21_readTemperature(TEMP_CONVERSION);
  float h = htu21_readHumidity();
  h = htu21_compensatedHumidity(h, t);
  dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
  dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
  snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\"}"), svalue, htustype, stemp1, stemp2);
  *djson = 1;
}

#ifdef USE_WEBSERVER
String htu_webPresent()
{
  String page = "";
  if (htutype) {
    char itemp[10], iconv[10];

    snprintf_P(iconv, sizeof(iconv), PSTR("&deg;%c"), (TEMP_CONVERSION) ? 'F' : 'C');
    float t_htu21 = htu21_readTemperature(TEMP_CONVERSION);
    float h_htu21 = htu21_readHumidity();
    h_htu21 = htu21_compensatedHumidity(h_htu21, t_htu21);
    dtostrf(t_htu21, 1, TEMP_RESOLUTION &3, itemp);
    page += F("<tr><td>HTU Temperature: </td><td>"); page += itemp; page += iconv; page += F("</td></tr>");
    dtostrf(h_htu21, 1, HUMIDITY_RESOLUTION &3, itemp);
    page += F("<tr><td>HTU Humidity: </td><td>"); page += itemp; page += F("%</td></tr>");
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_HTU
#endif  // USE_I2C

