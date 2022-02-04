/*
  xsns_96_qmc5883.ino - QMC5883 3-Axis Digital Compass sensor support for Tasmota

  Copyright (C) 2022 James Sharp
  
  Code borrowed heavily from Elnur Mehdiyev's implementation for the HMC5883L

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
#ifdef USE_QMC5883

/*********************************************************************************************\
 * QMC5883 is 3-Axis Digital Compass sensor knock off of the HMC5883L
 *
 * Source: James Sharp, code heavily borrowed from Elnur Mehdiyev's project
 *
 * I2C Address: 0x1D
\*********************************************************************************************/

// Define driver ID
#define XSNS_96                          96
#define XI2C_66                          66  // See I2CDEVICES.md

#define QMC5883_ADDR                    0x0D

#define Mode_Standby    0b00000000
#define Mode_Continuous 0b00000001

#define ODR_10Hz        0b00000000
#define ODR_50Hz        0b00000100
#define ODR_100Hz       0b00001000
#define ODR_200Hz       0b00001100

#define RNG_2G          0b00000000
#define RNG_8G          0b00010000

#define OSR_512         0b00000000
#define OSR_256         0b01000000
#define OSR_128         0b10000000
#define OSR_64          0b11000000


//Mag field
struct
{
int   MX;
int   MY;
int   MZ;
int   HG;
uint8_t   status;
bool      ready;
uint8_t   i2c_address;

} QMC5883;


//Initialise the device
void QMC5883_Init()
{
  
  if (!I2cSetDevice(QMC5883_ADDR)) { return; }
  I2cSetActiveFound(QMC5883_ADDR, "QMC5883");
  QMC5883.i2c_address = QMC5883_ADDR;
  QMC5883.ready = true;

  //Put the QMC5883 IC into the correct operating mode
  Wire.beginTransmission(QMC5883_ADDR);
  Wire.write(0x0B);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(QMC5883_ADDR);
  Wire.write(0x09);
  Wire.write(Mode_Continuous|ODR_200Hz|RNG_8G|OSR_512);
  Wire.endTransmission();

}

//Read the magnetic data
void QMC5883_read_data(void){

  if(!QMC5883.ready)
    return;

  //Tell the QMC5883 where to begin reading data
  Wire.beginTransmission(QMC5883_ADDR);            //QMC5883_ADDR
  Wire.write(0x00);                          
  Wire.endTransmission();
  Wire.beginTransmission(QMC5883_ADDR);                    //QMC5883_ADDRESS_MAG

  //Read data from each axis, 2 registers per axis plus overflow
  //uint8_t buff[7];

  Wire.requestFrom(QMC5883_ADDR,7);
  /*
  for(uint8_t i = 0; i < 7; i++)
	{
	  buff[i] = Wire.read();
	}
  AddLog(LOG_LEVEL_DEBUG, PSTR("QMC5883: buffer %x %x %x %x %x %x %x"), buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6]);
  Wire.endTransmission();
  QMC5883.MX=(int)(int16_t)(buff[0] | buff[1] << 8);
  QMC5883.MZ=(int)(int16_t)(buff[2] | buff[3] << 8);
  QMC5883.MY=(int)(int16_t)(buff[4] | buff[5] << 8);
  */
  QMC5883.MX = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  QMC5883.MY = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  QMC5883.MZ = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  byte overflow = Wire.read() & 0x02;
  
  overflow = overflow << 2;
  

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading=atan2((int)QMC5883.MY, (int)QMC5883.MX) * 180.0/M_PI;

  if (heading < 0)
    heading += 360.0;

  /*
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
  */
  // Convert radians to degrees for readability.
  QMC5883.HG = (int)heading; 

}


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_QMC5883[] PROGMEM =
  "{s}QMC5883 " D_MX "{m}%d " D_UNIT_MICROTESLA  "{e}"      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}QMC5883 " D_MY "{m}%d " D_UNIT_MICROTESLA  "{e}"      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}QMC5883 " D_MZ "{m}%d " D_UNIT_MICROTESLA  "{e}"      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}QMC5883 " D_HG "{m}%d " D_UNIT_DEGREE      "{e}";     // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

const char HTTP_SNS_QMC5883_ERROR[] PROGMEM =
  "{s}QMC5883 {m} %s {e}";
#endif


void QMC5883_Show(uint8_t json)
{
  QMC5883_read_data();

  if (json) {
    if (!QMC5883.ready){
      AddLog(LOG_LEVEL_INFO, PSTR("QMC5883: " D_ERROR " %x" ),QMC5883.status);
      return;
  }
  else {
    ResponseAppend_P(PSTR(",\"QMC5883\":{\"" D_JSON_MX "\":%i,\"" D_JSON_MY "\":%i,\"" D_JSON_MZ "\":%i,\"" D_JSON_HEADING "\":%i}"), QMC5883.MX, QMC5883.MY, QMC5883.MZ, QMC5883.HG);
    }
#ifdef USE_WEBSERVER
    } else {
      switch(QMC5883.ready){
        case true:
          WSContentSend_PD(HTTP_SNS_QMC5883, QMC5883.MX, QMC5883.MY, QMC5883.MZ, QMC5883.HG);
          break;
        case false:
          WSContentSend_PD(HTTP_SNS_QMC5883_ERROR, D_START);
          break;
        default:
          WSContentSend_PD(HTTP_SNS_QMC5883_ERROR, D_ERROR);
      }
#endif
  }
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
 
bool Xsns96(byte function)
{
  if (!I2cEnabled(XI2C_66)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    QMC5883_Init();
  }
  else if (QMC5883.ready) {
    switch (function) {
      case FUNC_JSON_APPEND:
        QMC5883_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        QMC5883_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}
#endif // USE_QMC5883
#endif // USE_I2C
