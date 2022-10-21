/* 
  FrogmoreScd40.cpp - SCD40/SCD41 I2C CO2(+temp+RH) sensor support for Tasmota,
                      based on frogmore42's FrogmoreScd30.cpp

  Copyright (C) 2019-2021  Frogmore42, Arnold-n

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// SCD40/SCD41 code based on SCD30 code

#pragma once

#include "Arduino.h"

//#define SCD40_DEBUG

#define SCD40_ADDRESS 0x62
#define ERROR_SCD40_NO_ERROR                        0
#define ERROR_SCD40_NO_DATA                0x80000000
#define ERROR_SCD40_UNKNOWN_ERROR           0x1000000
#define ERROR_SCD40_CRC_ERROR               0x2000000
#define ERROR_SCD40_NOT_ENOUGH_BYTES_ERROR  0x3000000
#define ERROR_SCD40_INVALID_VALUE           0x6000000
#define ERROR_SCD40_BUSY_MEASURING          0x7000000
#define ERROR_SCD40_NOT_IN_MEASUREMENT_MODE 0x8000000
#define ERROR_SCD40_FRC_FAILED              0x9000000

#define SCD40_MEDIAN_FILTER_SIZE 1

class FrogmoreScd40
{
    public:
        FrogmoreScd40() {};
        // Constructors
        // the SCD40 only lists a single i2c address, so not necesary to specify
        // 
        void begin(void);
        void begin(uint8_t _i2cAddress);
        void begin(TwoWire *pWire);
        void begin(TwoWire *pWire, uint8_t _i2cAddress);

        int reinit(void);
        int clearI2CBus(void); // this is a HARD reset of the IC2 bus to restore communication, it will disrupt the bus

        int getSensorAltitude(uint16_t *pHeight_meter);
        int getAutomaticSelfCalibrationEnabled(uint16_t *pIsAuto);
        int getTemperatureOffset(float *pOffset_degC);
        int getTemperatureOffset(uint16_t *pOffset_centiDegC);

        int setSensorAltitude(uint16_t height_meter);
        int setAmbientPressure(uint16_t airPressure_mbar);
        int setTemperatureOffset(float offset_degC);
        int setTemperatureOffset(uint16_t offset_centiDegC);
        int setAutomaticSelfCalibrationDisabled(void);
        int setAutomaticSelfCalibrationEnabled(void);
        int setAutomaticSelfCalibrationEnabled(bool isAuto);
        int performForcedRecalibration(uint16_t co2_ppm);
        int getSerialNumber(uint16_t *pSerialNumberArray);
        int startLowPowerPeriodicMeasurement(void);
        int persistSettings(void);
        int performSelfTest(uint16_t *pMalfunction);
        int performFactoryReset(void);

        int startPeriodicMeasurement(void);
        int getDataReadyStatus(bool *pIsAvailable);
        int readMeasurement(
            uint16 *pCO2_ppm,
            uint16 *pCO2EAvg_ppm,
            float *pTemperature,
            float *pHumidity
        );
        int stopPeriodicMeasurement(void);
        int forceStopPeriodicMeasurement(void);
// SCD41 only:
        int measureSingleShot(void);
        int measureSingleShotRhtOnly(void);

    private:
        bool duringMeasurement;
        uint8_t i2cAddress;
        TwoWire *pWire;
        uint16_t co2History[SCD40_MEDIAN_FILTER_SIZE];
        uint16_t co2EAverage;
        int8_t co2NewDataLocation; // location to put new CO2 data for median filter

        uint8_t computeCRC8(uint8_t data[], uint8_t len);
        uint16_t medianfilter(uint16_t * p);
        int sendBytes(void *pInput, uint8_t len);
        int getBytes(void *pOutput, uint8_t len);
        int sendCommand(uint16_t command);
        int sendCommandArguments(uint16_t command, uint16_t arguments);
        int sendCommandArgumentsFetchResult(uint16_t command, uint16_t arguments, uint16_t* pData);
        int get16BitRegCheckCRC(void* pInput, uint16_t* pData);
        int readRegisterCnt(uint16_t registerAddress, uint16_t* pData, uint8_t cnt);
        int readRegister(uint16_t registerAddress, uint16_t* pData);
#ifdef SCD40_DEBUG
        void AddLog(uint8_t loglevel);
#endif
};
