/*!
 * @file DFRobot_MICS.cpp
 * @brief Define the basic structure of class DFRobot_MicsSensor
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.1
 * @date 2020-4-20
 * @url https://github.com/DFRobot/DFRobot_MICS
 */
#include "DFRobot_MICS.h"

DFRobot_MICS::DFRobot_MICS(){}
DFRobot_MICS::~DFRobot_MICS(){}

bool DFRobot_MICS::warmUpTime(uint8_t minute)
{
  uint32_t tempTime  = 0;
  uint32_t tempOX    = 0; 
  uint32_t tempRED   = 0;
  uint16_t oxData[1]    = {0x00};
  uint16_t redData[1]   = {0x00};
  uint16_t powerData[1] = {0x00};
  uint32_t delayTime    = 0;
  uint32_t excessTime   = 0;
  if(__flag == 0){
    __flag = 1;
    __nowTime = millis();
  }
  delayTime  = minute * 60000;
  excessTime = millis() - __nowTime;
  if(excessTime < delayTime){
    return false;
  }
  if(getSensorData(oxData, redData, powerData) == ERROR){
    return false;
  }
  __r0_ox  = powerData[0] - oxData[0];
  __r0_red = powerData[0] - redData[0];
  return true;
}

int16_t DFRobot_MICS::getADCData(uint8_t mode)
{
  uint16_t oxData[1]    = {0x00};
  uint16_t redData[1]   = {0x00};
  uint16_t powerData[1] = {0x00};
  float RS_R0_RED_data  = 0;
  float RS_R0_OX_data   = 0;
  getSensorData(oxData, redData, powerData);
  if(powerData[0] <= redData[0]){
    RS_R0_RED_data = 0;
  }else{
    RS_R0_RED_data = (float)(powerData[0] - redData[0]);
  }
  if(powerData[0] <= oxData[0]){
    RS_R0_OX_data = 0;
  }else{
    RS_R0_OX_data = (float)(powerData[0] - oxData[0]);
  }
  if(mode == OX_MODE){
    return RS_R0_OX_data;
  }else{
    return RS_R0_RED_data;
  }
}

void DFRobot_MICS::sleepMode(void)
{
  uint8_t regData = SLEEP_MODE;
  writeData(POWER_MODE_REGISTER, &regData, 1);
  delay(100);
}

void DFRobot_MICS::wakeUpMode(void)
{
  uint8_t regData = WAKE_UP_MODE;
  writeData(POWER_MODE_REGISTER, &regData, 1);
  delay(100);
}

uint8_t DFRobot_MICS::getPowerState(void)
{
  uint8_t regData;
  readData(POWER_MODE_REGISTER, &regData, (uint8_t)1);
  return regData;
}

int8_t DFRobot_MICS::getGasExist(uint8_t gas)
{
  uint16_t oxData[1]    = {0x00};
  uint16_t redData[1]   = {0x00};
  uint16_t powerData[1] = {0x00};
  getSensorData(oxData, redData, powerData);
  float RS_R0_RED_data = (float)(powerData[0] - redData[0]) / (float)__r0_red;
  float RS_R0_OX_data = (float)(powerData[0] - oxData[0]) / (float)__r0_ox;
  switch(gas)
  {
    case C3H8:
      if(existPropane(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case C4H10:
      if(existIsoButane(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case H2S:
      if(existHydrothion(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case NO:
      if(existNitricOxide(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case CO:
      if(existCarbonMonoxide(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case CH4:
      if(existMethane(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case C2H5OH:
      if(existEthanol(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case H2:
      if(existHydrogen(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case NH3:
      if(existAmmonia(RS_R0_RED_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    case NO2:
      if(existNitrogenDioxide(RS_R0_OX_data)){
        return EXIST;
      }else{
        return NO_EXIST;
      }
      break;
    default:
      return ERROR;
      break;
  }
}

float DFRobot_MICS::getGasData(uint8_t type)
{
  uint16_t oxData[1]    = {0x00};
  uint16_t redData[1]   = {0x00};
  uint16_t powerData[1] = {0x00};
  getSensorData(oxData, redData, powerData);
  float RS_R0_RED_data = (float)(powerData[0] - redData[0]) / (float)__r0_red;
  float RS_R0_OX_data = (float)(powerData[0] - oxData[0]) / (float)__r0_ox;
  switch(type){
    case CO:
      return getCarbonMonoxide(RS_R0_RED_data);
      break;
    case CH4:
      return getMethane(RS_R0_RED_data);
      break;
    case C2H5OH:
      return getEthanol(RS_R0_RED_data);
      break;
    case H2:
      return getHydrogen(RS_R0_RED_data);
      break;
    case NH3:
      return getAmmonia(RS_R0_RED_data);
      break;
    case NO2:
      return getNitrogenDioxide(RS_R0_OX_data);
      break;
    default:
      return ERROR;
      break;
  }
}

float DFRobot_MICS::getCarbonMonoxide(float data)
{
  if(data > 0.425)
    return 0.0;
  float co = (0.425 - data) / 0.000405;
  if(co > 1000.0)
    return 1000.0;
  if(co < 1.0) 
    return 0.0;
  return co;
}

float DFRobot_MICS::getEthanol(float data)
{
  if(data > 0.306)
    return 0.0;
  float ethanol = (0.306 - data) / 0.00057;
  if(ethanol < 10.0) 
    return 0.0;
  if(ethanol > 500.0) 
    return 500.0;
  return ethanol;
}

float DFRobot_MICS::getMethane(float data)
{
  if(data > 0.786)
    return 0.0;
  float methane = (0.786 - data) / 0.000023;
  if(methane < 1000.0) methane = 0.0;
  if(methane > 25000.0) methane = 25000.0;
  return methane;
}

float DFRobot_MICS::getNitrogenDioxide(float data)
{
  if(data < 1.1) return 0;
  float nitrogendioxide = (data - 0.045) / 6.13;
  if(nitrogendioxide < 0.1)
    return 0.0;
  if(nitrogendioxide > 10.0)
    return 10.0;
  return nitrogendioxide;
}

float DFRobot_MICS::getHydrogen(float data)
{
  if(data > 0.279)
    return 0.0;
  float hydrogen = (0.279 - data) / 0.00026;
  if(hydrogen < 1.0) 
    return 0.0;
  if(hydrogen > 1000.0) 
    return 1000.0;
  return hydrogen;
}

float DFRobot_MICS::getAmmonia(float data)
{
  if(data > 0.8)
    return 0.0;
  float ammonia = (0.8 - data) / 0.0015;
  if(ammonia < 1.0) 
    return 0.0;
  if(ammonia > 500.0) 
    return 500.0;
  return ammonia;
}

bool DFRobot_MICS::existIsoButane(float data)
{
  if(data > 0.65){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existPropane(float data)
{
  if(data > 0.18){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existHydrothion(float data)
{
  if(data > 0.58 && data < 0.69){
    return true;
  }
  if(data < 0.201){
    return true;
  }
  return false;
}

bool DFRobot_MICS::existNitricOxide(float data)
{
  if(data > 0.8){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existCarbonMonoxide(float data)
{
  if(data > 0.425){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existMethane(float data)
{
  if(data > 0.786){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existEthanol(float data)
{
  if(data > 0.306){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existHydrogen(float data)
{
  if(data > 0.279){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existAmmonia(float data)
{
  if(data > 0.8){
    return false;
  }else{
    return true;
  }
}

bool DFRobot_MICS::existNitrogenDioxide(float data)
{
  if(data < 1.1){
    return false;
  }else{
    return true;
  }
}

int16_t DFRobot_MICS::getSensorData(uint16_t *oxData, uint16_t *redData, uint16_t *powerData)
{
  uint8_t recv_data[20] = {0x00};
  readData(OX_REGISTER_HIGH, recv_data, (uint8_t)6);
  oxData[0]    = (((uint16_t)recv_data[0] << 8) + (uint16_t)recv_data[1]);
  redData[0]   = (((uint16_t)recv_data[2] << 8) + (uint16_t)recv_data[3]);
  powerData[0] = (((uint16_t)recv_data[4] << 8) + (uint16_t)recv_data[5]);
  return 0;
}

DFRobot_MICS_I2C::DFRobot_MICS_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
}

bool DFRobot_MICS_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  if(_pWire->endTransmission() == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void DFRobot_MICS_I2C::writeData(uint8_t reg, uint8_t *data, uint8_t len)
{
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  for(uint8_t i = 0; i < len; i++)
  {
    _pWire->write(data[i]);
  }
  _pWire->endTransmission();
}

int16_t DFRobot_MICS_I2C::readData(uint8_t reg, uint8_t *data, uint8_t len)
{
  int i=0;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  if(_pWire->endTransmission() != 0)
  {
    return ERROR;
  }
  _pWire->requestFrom((uint8_t)this->_I2C_addr, (uint8_t)len);
  while (_pWire->available())
  {
    data[i++]=_pWire->read();
  }
  return 0;
}

DFRobot_MICS_ADC::DFRobot_MICS_ADC(uint8_t adcPin, uint8_t powerPin)
{
  __powerPin = powerPin;
  __adcPin   = adcPin;
}

bool DFRobot_MICS_ADC::begin()
{
  pinMode(__powerPin, OUTPUT);
  delay(10);
  digitalWrite(__powerPin, HIGH);        // sleep mode
  return true;
}

void DFRobot_MICS_ADC::writeData(uint8_t reg, uint8_t *data, uint8_t len)
{
  uint8_t length = len;
  if(reg == POWER_MODE_REGISTER){
    if(data[0] == 0){
      digitalWrite(__powerPin, HIGH);        // sleep
    }else{
      digitalWrite(__powerPin, LOW);
    }
  }
}

int16_t DFRobot_MICS_ADC::readData(uint8_t reg,uint8_t *data,uint8_t len)
{
  switch(reg)
  {
    case POWER_MODE_REGISTER:
      if(HIGH == digitalRead(__powerPin)){ data[0] = 0;}
      else{ data[0] = 1;}
      break;
    case OX_REGISTER_HIGH:
      int16_t ADCData = 1023 - analogRead(__adcPin);
      if(len == 6){
        data[0] = data[1] = data[2] = data[3] = 0;
        data[4] = uint8_t((ADCData>>8)&0x00FF);
        data[5] = uint8_t(ADCData&0x00FF);
      }
      break;
  }
  return 0;
}