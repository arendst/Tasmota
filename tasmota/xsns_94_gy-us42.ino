/*
  xsns_94_gy-us42.ino - GY-US42 distance sensor support for Tasmota

  Copyright (C) 2021  Theo Arends, Conrad Sachweh

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
#ifdef USE_GY_US42
/*********************************************************************************************\
 * GY-US42 - Proximity Sensor (max useful range from 20cm to 4m, value of 720 means invalid result)
 * Inspired by https://www.maxbotix.com/documents/Arduino%20Codes/I2C_for_MaxSonar.txt
 *
 * I2C Address: 0x70
\*********************************************************************************************/

#define XSNS_94             94
#define XI2C_64             64         // See I2CDEVICES.md

#define GYUS42_ADDR         0x70       // address for GY-US42 sensor


struct  {
  uint16_t distance = 0;
  bool ready = false;
} gyus42_sensor;

bool GyUs42Read(uint16_t &range, uint8_t gyus42_address)
{
  range = 0;

  Wire.beginTransmission(gyus42_address);
  takeRangeReading(gyus42_address);
  delay(100);  // wait until new value is available
  Wire.requestFrom(gyus42_address, (uint8_t)2);
  if (Wire.available() >=2) {
      int reading = Wire.read();  //read the first data
      reading = reading << 8;     // shift reading left by 8 bits, equivalent to reading * 256
      reading |= Wire.read();     // reading = reading | Wire.read()
      Wire.endTransmission();
      range = reading;
  }
  
  return (range != 0);
}

//Commands the sensor to take a range reading
void takeRangeReading(uint8_t gyus42_address)
{
  Wire.beginTransmission(gyus42_address); //Start addressing
  Wire.write(0x51); //send range command
  Wire.endTransmission();
}

/********************************************************************************************/

void GyUs42Detect(void)
{
  uint16_t range;
  if (GyUs42Read(range, GYUS42_ADDR)) {
    I2cSetActiveFound(GYUS42_ADDR, "GY-US42");
    gyus42_sensor.ready = true;
  }
}

void GyUs42Show(bool json) {
  uint16_t range;
  GyUs42Read(range, GYUS42_ADDR);
  if (json) {
    ResponseAppend_P(PSTR(",\"GY-US42\":{\"" D_JSON_DISTANCE "\":%d}"), gyus42_sensor.distance);
#ifdef USE_WEBSERVER
  }
  else {
    // default distance shown is in mm --> convert cm in mm
    WSContentSend_PD(HTTP_SNS_DISTANCE, PSTR("GY-US42"), gyus42_sensor.distance*10);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns94(uint8_t function)
{
  if (!I2cEnabled(XI2C_64)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    GyUs42Detect();
  }
  else if (gyus42_sensor.ready) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      GyUs42Read(gyus42_sensor.distance, GYUS42_ADDR);
      break;
    case FUNC_JSON_APPEND:
      GyUs42Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      GyUs42Show(0);
      break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_GY_US42
#endif  // USE_I2C
