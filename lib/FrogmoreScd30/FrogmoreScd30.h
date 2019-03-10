/*
#  Copyright (c) 2019 Frogmore42
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
#pragma once

#include "Arduino.h"

//#define SCD30_DEBUG

#define SCD30_ADDRESS 0x61
#define ERROR_SCD30_NO_ERROR                        0
#define ERROR_SCD30_NO_DATA                0x80000000
#define ERROR_SCD30_CO2_ZERO               0x90000000
#define ERROR_SCD30_UNKNOWN_ERROR           0x1000000
#define ERROR_SCD30_CRC_ERROR               0x2000000
#define ERROR_SCD30_NOT_ENOUGH_BYTES_ERROR  0x3000000
#define ERROR_SCD30_NOT_FOUND_ERROR         0x4000000
#define ERROR_SCD30_NOT_A_NUMBER_ERROR      0x5000000
#define ERROR_SCD30_INVALID_VALUE           0x6000000

#define SCD30_MEDIAN_FILTER_SIZE 5

class FrogmoreScd30
{
    public:
        FrogmoreScd30() {};
        // Constructors
        // the SCD30 only lists a single i2c address, so not necesary to specify
        // 
        void begin(void);
        void begin(uint8_t _i2cAddress);
        void begin(TwoWire *pWire);
        void begin(TwoWire *pWire, uint8_t _i2cAddress);

        int softReset(void);
        int clearI2CBus(void); // this is a HARD reset of the IC2 bus to restore communication, it will disrupt the bus

        int getAltitudeCompensation(uint16_t *pHeight_meter);
        int getAmbientPressure(uint16_t *pAirPressure_mbar);
        int getCalibrationType(uint16_t *pIsAuto);
        int getFirmwareVersion(uint8_t *pMajor, uint8_t *pMinor);
        int getForcedRecalibrationFactor(uint16_t *pCo2_ppm);
        int getMeasurementInterval(uint16_t *pTime_sec);
        int getTemperatureOffset(float *pOffset_degC);
        int getTemperatureOffset(uint16_t *pOffset_centiDegC);

        int setAltitudeCompensation(uint16_t height_meter);
        int setAmbientPressure(uint16_t airPressure_mbar);
        int setAutoSelfCalibration(void);
        int setCalibrationType(bool isAuto);
        int setForcedRecalibrationFactor(uint16_t co2_ppm);
        int setManualCalibration(void);
        int setMeasurementInterval(uint16_t time_sec);
        int setTemperatureOffset(float offset_degC);
        int setTemperatureOffset(uint16_t offset_centiDegC);

        int beginMeasuring(void);
        int beginMeasuring(uint16_t airPressure_mbar); // also sets ambient pressure offset in mbar/hPascal
        int isDataAvailable(bool *pIsAvailable);
        int readMeasurement(
            uint16 *pCO2_ppm,
            uint16 *pCO2EAvg_ppm,
            float *pTemperature,
            float *pHumidity
        );
        int stopMeasuring(void);

    private:
        uint8_t i2cAddress;
        TwoWire *pWire;
        uint16_t ambientPressure;
        uint16_t co2AvgExtra;
        uint16_t co2History[SCD30_MEDIAN_FILTER_SIZE];
        uint16_t co2EAverage;
        int8_t co2NewDataLocation; // location to put new CO2 data for median filter

        uint8_t computeCRC8(uint8_t data[], uint8_t len);
        int sendBytes(void *pInput, uint8_t len);
        int getBytes(void *pOutput, uint8_t len);
        int sendCommand(uint16_t command);
        int sendCommandArguments(uint16_t command, uint16_t arguments);
        int get16BitRegCheckCRC(void* pInput, uint16_t* pData);
        int get32BitRegCheckCRC(void* pInput, float* pData);
        int readRegister(uint16_t registerAddress, uint16_t* pData);
#ifdef SCD30_DEBUG
        void AddLog(uint8_t loglevel);
#endif
};