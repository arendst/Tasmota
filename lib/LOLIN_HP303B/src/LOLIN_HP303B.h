#ifndef __LOLIN_HP303B_H
#define __LOLIN_HP303B_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <SPI.h>
#include "util/hp303b_consts.h"

class LOLIN_HP303B
{
public:
  //constructor
  LOLIN_HP303B(void);
  //destructor
  ~LOLIN_HP303B(void);
  //begin
  void begin(TwoWire &bus, uint8_t slaveAddress);
  void begin(uint8_t slaveAddress=HP303B__STD_SLAVE_ADDRESS);
  void begin(SPIClass &bus, int32_t chipSelect);
  void begin(SPIClass &bus, int32_t chipSelect, uint8_t threeWire);
  //end
  void end(void);

  //general
  uint8_t getProductId(void);
  uint8_t getRevisionId(void);

  //Idle Mode
  int16_t standby(void);

  //Command Mode
  int16_t measureTempOnce(float &result);
  int16_t measureTempOnce(float &result, uint8_t oversamplingRate);
  int16_t startMeasureTempOnce(void);
  int16_t startMeasureTempOnce(uint8_t oversamplingRate);
  int16_t measurePressureOnce(float &result);
  int16_t measurePressureOnce(float &result, uint8_t oversamplingRate);
  int16_t startMeasurePressureOnce(void);
  int16_t startMeasurePressureOnce(uint8_t oversamplingRate);
  int16_t getSingleResult(float &result);

  //Background Mode
  int16_t startMeasureTempCont(uint8_t measureRate, uint8_t oversamplingRate);
  int16_t startMeasurePressureCont(uint8_t measureRate, uint8_t oversamplingRate);
  int16_t startMeasureBothCont(uint8_t tempMr, uint8_t tempOsr, uint8_t prsMr, uint8_t prsOsr);
  int16_t getContResults(float *tempBuffer, uint8_t &tempCount, float *prsBuffer, uint8_t &prsCount);

  //Interrupt Control
  int16_t setInterruptPolarity(uint8_t polarity);
  int16_t setInterruptSources(uint8_t fifoFull, uint8_t tempReady, uint8_t prsReady);
  int16_t getIntStatusFifoFull(void);
  int16_t getIntStatusTempReady(void);
  int16_t getIntStatusPrsReady(void);

  //function to fix a hardware problem on some devices
  int16_t correctTemp(void);

private:
  //scaling factor table
  static const int32_t scaling_facts[HP303B__NUM_OF_SCAL_FACTS];

  //enum for operating mode
  enum Mode
  {
    IDLE = 0x00,
    CMD_PRS = 0x01,
    CMD_TEMP = 0x02,
    INVAL_OP_CMD_BOTH = 0x03,  //invalid
    INVAL_OP_CONT_NONE = 0x04, //invalid
    CONT_PRS = 0x05,
    CONT_TMP = 0x06,
    CONT_BOTH = 0x07
  };
  Mode m_opMode;

  //flags
  uint8_t m_initFail;
  uint8_t m_productID;
  uint8_t m_revisionID;

  //settings
  uint8_t m_tempMr;
  uint8_t m_tempOsr;
  uint8_t m_prsMr;
  uint8_t m_prsOsr;
  uint8_t m_tempSensor;

  //compensation coefficients
  int32_t m_c0Half;
  int32_t m_c1;
  int32_t m_c00;
  int32_t m_c10;
  int32_t m_c01;
  int32_t m_c11;
  int32_t m_c20;
  int32_t m_c21;
  int32_t m_c30;
  //last measured scaled temperature
  //(necessary for pressure compensation)
  double m_lastTempScal;

  //bus specific
  uint8_t m_SpiI2c; //0=SPI, 1=I2C
                    //used for I2C
  TwoWire *m_i2cbus;
  uint8_t m_slaveAddress;
  //used for SPI
  SPIClass *m_spibus;
  int32_t m_chipSelect;
  uint8_t m_threeWire;

  //measurement
  void init(void);
  int16_t readcoeffs(void);
  int16_t setOpMode(uint8_t background, uint8_t temperature, uint8_t pressure);
  int16_t setOpMode(uint8_t opMode);
  int16_t configTemp(uint8_t temp_mr, uint8_t temp_osr);
  int16_t configPressure(uint8_t prs_mr, uint8_t prs_osr);
  uint16_t calcBusyTime(uint16_t temp_rate, uint16_t temp_osr);
  int16_t getTemp(float *result);
  int16_t getPressure(float *result);
  int16_t getFIFOvalue(float *value);
  float calcTemp(float raw);
  float calcPressure(float raw);

  //bus specific
  int16_t readByte(uint8_t regAddress);
  int16_t readByteSPI(uint8_t regAddress);
  int16_t readBlock(uint8_t regAddress, uint8_t length, uint8_t *buffer);
  int16_t readBlockSPI(uint8_t regAddress, uint8_t length, uint8_t *readbuffer);
  int16_t writeByte(uint8_t regAddress, uint8_t data);
  int16_t writeByte(uint8_t regAddress, uint8_t data, uint8_t check);
  int16_t writeByteSpi(uint8_t regAddress, uint8_t data, uint8_t check);
  int16_t writeByteBitfield(uint8_t data, uint8_t regAddress, uint8_t mask, uint8_t shift);
  int16_t writeByteBitfield(uint8_t data, uint8_t regAddress, uint8_t mask, uint8_t shift, uint8_t check);
  int16_t readByteBitfield(uint8_t regAddress, uint8_t mask, uint8_t shift);
};

#endif
