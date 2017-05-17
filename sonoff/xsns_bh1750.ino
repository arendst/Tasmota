/*
  xsns_bh1750.ino - BH1750 ambient light sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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
#ifdef USE_BH1750
/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
\*********************************************************************************************/

#define BH1750_ADDR1         0x23
#define BH1750_ADDR2         0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10 // Start measurement at 1lx resolution. Measurement time is approx 120ms.

uint8_t bh1750addr;
uint8_t bh1750type = 0;
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
  if (bh1750type) {
    return true;
  }

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
    strcpy(bh1750stype, "BH1750");
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
  if (!bh1750type) {
    return;
  }

  uint16_t l = bh1750_readLux();
  snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Illuminance\":%d}"), svalue, bh1750stype, l);
  *djson = 1;
#ifdef USE_DOMOTICZ
  domoticz_sensor5(l);
#endif  // USE_DOMOTICZ
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_ILLUMINANCE[] PROGMEM =
  "<tr><th>BH1750 Illuminance</th><td>%d lx</td></tr>";

String bh1750_webPresent()
{
  String page = "";
  if (bh1750type) {
    char sensor[80];
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_ILLUMINANCE, bh1750_readLux());
    page += sensor;
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_BH1750
#endif  // USE_I2C

