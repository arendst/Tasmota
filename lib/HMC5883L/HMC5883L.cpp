/*
HMC5833L magnetometer library

Brice Dubost

GPL v2 or higher
 */


#include "HMC5883L.h"

//Constructor
HMC5883L::HMC5883L()
{
  address = MAGaddr;
  gain=DefaultGain;
  average=DefaultAverage;
  rate=DefaultRate;
  timeout=DefaultTimeout;
  
}

void HMC5883L::setAddr(uint8_t _address)
{
    address = _address; 
}

void HMC5883L::setgain(uint8_t usergain)
{
  gain=usergain;
}


void HMC5883L::setrate(uint8_t userrate)
{
  if(userrate<7)
    rate=userrate;
}

void HMC5883L::setaverage(uint8_t useraverage)
{
  if(useraverage<4)
    average=useraverage;
}

//Initialise the device
uint8_t HMC5883L::init()
{
  isconnected = check_conn();
  if(!isconnected)
    return 0;

  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address);
  Wire.write((uint8_t)CONF_REG_A); //Configuration register A
  Wire.write((uint8_t)((average&0x03)<<5)|((rate&0x07)<<2)); //Set the avg and rate
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.write((uint8_t)CONF_REG_B); //Configuration register B
  Wire.write((uint8_t)((gain&0x07)<<5)); //Set the gain
  Wire.endTransmission();
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write((uint8_t)MODE_REG); //select mode register
  Wire.write((uint8_t)0x00); //continuous measurement mode
  Wire.endTransmission();

  return 1;

}

//Wait for data
uint8_t HMC5883L::waitavail(uint8_t numbytes)
{
  uint8_t waittime = 0;
  while(Wire.available()<numbytes)
    {
      waittime += 10;
      if(waittime>timeout)
	{
	  isconnected=0;
	  return 0;
	}
      delay(1);
    }
  return 1;

}

//Check if we are able to communicate
uint8_t HMC5883L::check_conn()
{

  uint8_t ira,irb,irc;
  Wire.beginTransmission(address);
  Wire.write((uint8_t)0x0A); //select register 10, Identification register A register
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.requestFrom(address,(uint8_t)3); //open communication with HMC5883
  if(waitavail(3))//wait for data
    {    
      ira=(Wire.read()& 0xff);
      irb=(Wire.read()& 0xff);
      irc=(Wire.read()& 0xff);
      Wire.endTransmission();
      if(ira==0x48 && irb==0x34 && irc==0x33)
	return 1;
    }
  Wire.endTransmission();
  return 0;
}

//Read the magnetic data
uint8_t HMC5883L::read_data(){

  if(!isconnected)
    return 0;

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write((uint8_t)MAG_X_MSB); //select register 03, X MSB register
  Wire.endTransmission();
  Wire.beginTransmission(address);

  //Read data from each axis, 2 registers per axis
  uint8_t buff[6];
 
  Wire.requestFrom(address,(uint8_t) 6);
  if(waitavail(6))//wait for data
    {    
      for(uint8_t i = 0; i < 6; i++)
	{
	  buff[i] = Wire.read();
	}
      Wire.endTransmission();
      meas.MX=((int)(buff[0]<<8 | buff[1]&0xFFFF));
      meas.MZ=((int)(buff[2]<<8 | buff[3]&0xFFFF));
      meas.MY=((int)(buff[4]<<8 | buff[5]&0xFFFF));
      return 1;
    }
  Wire.endTransmission();
  return 0;
}


void HMC5883L::convert()
{
 // Conversion to gauss depends on gain
 meas.f_MX=((int)meas.MX)/(float)Magconv[gain];
 meas.f_MY=((int)meas.MY)/(float)Magconv[gain];
 meas.f_MZ=((int)meas.MZ)/(float)Magconv[gain];
}

uint8_t HMC5883L::sleep()
{
  if(!isconnected)
    return 0;
  //Put the HMC5883L IC into the idle mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write((uint8_t)MODE_REG); //select mode register
  Wire.write((uint8_t)0x03); //idle measurement mode
  Wire.endTransmission();
}

uint8_t HMC5883L::unsleep()
{
  if(!isconnected)
    return 0;
  //Put the HMC5883L IC into the correct mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write((uint8_t)MODE_REG); //select mode register
  Wire.write((uint8_t)0x00); //idle measurement mode
  Wire.endTransmission();
}

