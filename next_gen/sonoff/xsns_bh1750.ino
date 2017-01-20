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
#ifdef USE_BH1750
/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
\*********************************************************************************************/

#define BH1750_ADDR1         0x23
#define BH1750_ADDR2         0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10 // Start measurement at 1lx resolution. Measurement time is approx 120ms.

uint8_t bh1750addr, bh1750type = 0;
char bh1750stype[7];

uint16_t bh1750_readLux(void)
{
  Wire.requestFrom(bh1750addr, (uint8_t)2);
  byte msb = Wire.read();
  byte lsb = Wire.read();
  uint16_t value = ((msb << 8) | lsb) / 1.2;
  return value;
}

boolean bh1750_detect()
{
  if (bh1750type) return true;

  char log[LOGSZ];
  uint8_t status;
  boolean success = false;

  bh1750addr = BH1750_ADDR1;
  Wire.beginTransmission(bh1750addr);
  Wire.write(BH1750_CONTINUOUS_HIGH_RES_MODE);
  status = Wire.endTransmission();
  if (status) {
    bh1750addr = BH1750_ADDR2;
    Wire.beginTransmission(bh1750addr);
    Wire.write(BH1750_CONTINUOUS_HIGH_RES_MODE);
    status = Wire.endTransmission();
  }
  if (!status) {
    success = true;
    bh1750type = 1;
    snprintf_P(bh1750stype, sizeof(bh1750stype), PSTR("BH1750"));
  }
  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), bh1750stype, bh1750addr);
    addLog(LOG_LEVEL_DEBUG, log);
  } else {
    bh1750type = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void bh1750_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!bh1750type) return;
  
  uint16_t l = bh1750_readLux();
  snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Illuminance\":%d}"), svalue, bh1750stype, l);
  *djson = 1;
}

#ifdef USE_WEBSERVER
String bh1750_webPresent()
{
  String page = "";
  if (bh1750type) {
    uint16_t l = bh1750_readLux();
    page += F("<tr><td>Illuminance: </td><td>"); page += String(l); page += F(" lx</td></tr>");
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_BH1750
#endif  // USE_I2C

