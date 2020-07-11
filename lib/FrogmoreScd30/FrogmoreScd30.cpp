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
#include <Wire.h>
#include <math.h>
#include <stdio.h>
#include <twi.h>
#include <FrogmoreScd30.h>

#define COMMAND_SCD30_CONTINUOUS_MEASUREMENT      0x0010
#define COMMAND_SCD30_MEASUREMENT_INTERVAL        0x4600
#define COMMAND_SCD30_GET_DATA_READY              0x0202
#define COMMAND_SCD30_READ_MEASUREMENT            0x0300
#define COMMAND_SCD30_CALIBRATION_TYPE            0x5306
#define COMMAND_SCD30_FORCED_RECALIBRATION_FACTOR 0x5204
#define COMMAND_SCD30_TEMPERATURE_OFFSET          0x5403
#define COMMAND_SCD30_ALTITUDE_COMPENSATION       0x5102
#define COMMAND_SCD30_SOFT_RESET                  0xD304
#define COMMAND_SCD30_GET_FW_VERSION              0xD100
#define COMMAND_SCD30_STOP_MEASUREMENT            0x0104

#define SCD30_DATA_REGISTER_BYTES 2
#define SCD30_DATA_REGISTER_WITH_CRC 3
#define SCD30_MEAS_BYTES 18

#ifdef SCD30_DEBUG
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};
char scd30log_data[180];
#endif

void FrogmoreScd30::begin(TwoWire *pWire, uint8_t i2cAddress)
{
    this->i2cAddress = i2cAddress;
    if (pWire == NULL)
    {
        this->pWire = &Wire;
    }
    else
    {
        this->pWire = pWire;
    }

    co2NewDataLocation = -1; // indicates there is no data, so the 1st data point needs to fill up the median filter
#ifdef ESP8266
    this->pWire->setClockStretchLimit(200000);
#endif
    this->ambientPressure = 0;
}

void FrogmoreScd30::begin(uint8_t i2cAddress)
{
    begin(NULL, i2cAddress);
}

void FrogmoreScd30::begin(TwoWire *pWire)
{
    begin(pWire, SCD30_ADDRESS);
}

void FrogmoreScd30::begin(void)
{
    begin(NULL, SCD30_ADDRESS);
}

/*---------------------------------------------------------------------------
 Function : opt_med5() In : pointer to array of 5 values
 Out : a uint16_t which is the middle value of the sorted array
 Job : optimized search of the median of 5 values
 Notice : found on sci.image.processing cannot go faster unless assumptions are made on the nature of the input signal.
 ---------------------------------------------------------------------------*/
#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
#define PIX_SWAP(a,b) { uint16_t temp=(a);(a)=(b);(b)=temp; }

uint16_t opt_med5(uint16_t * p)
{
    PIX_SORT(p[0], p[1]);
    PIX_SORT(p[3], p[4]);
    PIX_SORT(p[0], p[3]);
    PIX_SORT(p[1], p[4]);
    PIX_SORT(p[1], p[2]);
    PIX_SORT(p[2], p[3]);
    PIX_SORT(p[1], p[2]);
    return(p[2]);
}

// twi_status() attempts to read out any data left that is holding SDA low, so a new transaction can take place
// something like (http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html)
int FrogmoreScd30::clearI2CBus(void)
{
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "clearI2CBus");
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
#ifdef ESP8266
    return (twi_status());
#else
    return 0;
#endif
}

#ifdef SCD30_DEBUG
void FrogmoreScd30::AddLog(uint8_t loglevel)
{
    if (loglevel <= LOG_LEVEL_INFO)
    {
        Serial.printf("%s\r\n", scd30log_data);
    }
}
#endif

uint8_t FrogmoreScd30::computeCRC8(uint8_t data[], uint8_t len)
// Computes the CRC that the SCD30 uses
{
  uint8_t crc = 0xFF; //Init with 0xFF

  for (uint8_t x = 0 ; x < len ; x++)
  {
    crc ^= data[x]; // XOR-in the next input byte
    for (uint8_t i = 0 ; i < 8 ; i++)
    {
      if ((crc & 0x80) != 0)
        crc = (uint8_t)((crc << 1) ^ 0x31);
      else
        crc <<= 1;
    }
  }

  return crc; //No output reflection
}

// Sends stream of bytes to device
int FrogmoreScd30::sendBytes(void *pInput, uint8_t len)
{
    uint8_t *pBytes = (uint8_t *) pInput;
    int result;
    uint8_t errorBytes = 0; // number of bytes that had an error in transmission
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30SendBytes: data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", pBytes[0], pBytes[1], pBytes[2], pBytes[3], pBytes[4], pBytes[5], pBytes[6], pBytes[7], pBytes[8]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    pWire->beginTransmission(this->i2cAddress);
    errorBytes = len - (pWire->write(pBytes, len));
    result = pWire->endTransmission();
    if (errorBytes || result)
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30SendBytes: errorBytes: %d | Wire.end: %d", errorBytes, result);
        AddLog(LOG_LEVEL_INFO);
#endif
    }

    result <<= 8; // leave room for error bytes number
    result |= errorBytes; // low byte has number of bytes that were not written correctly
    return (result);
}

// Gets a number of bytes from device
int FrogmoreScd30::getBytes(void *pOutput, uint8_t len)
{
  uint8_t *pBytes = (uint8_t *) pOutput;
  uint8_t result;

  result = pWire->requestFrom(this->i2cAddress, len);
  if (len != result)
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30GetBytes: wire request expected %d got: %d", len, result);
    AddLog(LOG_LEVEL_INFO);
#endif
    return (ERROR_SCD30_NOT_ENOUGH_BYTES_ERROR);
  }

  if (pWire->available())
  {
    for (int x = 0; x < len; x++)
    {
      pBytes[x] = pWire->read();
    }
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30GetBytes: data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", pBytes[0], pBytes[1], pBytes[2], pBytes[3], pBytes[4], pBytes[5], pBytes[6], pBytes[7], pBytes[8]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    return (ERROR_SCD30_NO_ERROR);
  }

  return (ERROR_SCD30_UNKNOWN_ERROR);
}

//Sends just a command, no arguments, no CRC
int FrogmoreScd30::sendCommand(uint16_t command)
{
  uint8_t data[2];
  data[0] = command >> 8;
  data[1] = command & 0xFF;
  int error = sendBytes(data, sizeof(data));
  if (error)
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30SendCommand: Scd30SendBytes failed: 0x%lX", error);
    AddLog(LOG_LEVEL_INFO);
#endif
  }
  return (error);
}

//Sends a command along with arguments and CRC
int FrogmoreScd30::sendCommandArguments(uint16_t command, uint16_t arguments)
{
  uint8_t data[5];
  data[0] = command >> 8;
  data[1] = command & 0xFF;
  data[2] = arguments >> 8;
  data[3] = arguments & 0xFF;
  data[4] = computeCRC8(&data[2], 2); //Calc CRC on the arguments only, not the command
  int error = sendBytes(data, sizeof(data));
  if (error)
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30SendCommandArguments: Scd30SendBytes failed: 0x%lX", error);
    AddLog(LOG_LEVEL_INFO);
#endif
  }
  return (error);
}

int FrogmoreScd30::get16BitRegCheckCRC(void* pInput, uint16_t *pData)
{
  uint8_t *pBytes = (uint8_t *) pInput;
  uint8_t expectedCRC = computeCRC8(pBytes, SCD30_DATA_REGISTER_BYTES);
  if (expectedCRC != pBytes[SCD30_DATA_REGISTER_BYTES])
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30get16BitRegCheckCRC: expected: 0x%02X, but got: 0x%02X", expectedCRC, pBytes[SCD30_DATA_REGISTER_BYTES]);
    AddLog(LOG_LEVEL_INFO);
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30get16BitRegCheckCRC: data: 0x%02X, 0x%02X, 0x%02X", pBytes[0], pBytes[1], pBytes[2]);
    AddLog(LOG_LEVEL_INFO);
#endif
    return (ERROR_SCD30_CRC_ERROR);
  }
  *pData = (uint16_t) pBytes[0] << 8 | pBytes[1]; // data from SCD30 is Big-Endian
  return (ERROR_SCD30_NO_ERROR);
}

// gets 32 bits, (2) 16-bit chunks, and validates the CRCs
//
int FrogmoreScd30::get32BitRegCheckCRC(void *pInput, float *pData)
{
    uint16_t tempU16High;
    uint16_t tempU16Low;
    uint8_t *pBytes = (uint8_t *) pInput;
    uint32_t rawInt = 0;

    int error = get16BitRegCheckCRC(pBytes, &tempU16High);
    if (error) {
        return (error);
    }

    error = get16BitRegCheckCRC(pBytes + SCD30_DATA_REGISTER_WITH_CRC, &tempU16Low);
    if (error) {
        return (error);
    }

    // data from SCD is Big-Endian
    rawInt |= tempU16High;
    rawInt <<= 16;
    rawInt |= tempU16Low;

    *pData =  * (float *) &rawInt;
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "get32BitRegCheckCRC: got: tempUs 0x%lX, %lX", tempU16High, tempU16Low);
    AddLog(LOG_LEVEL_DEBUG);
#endif

    if (isnan(*pData) || isinf(*pData))
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "get32BitRegCheckCRC: not a floating point number: rawInt 0x%lX", rawInt);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (ERROR_SCD30_NOT_A_NUMBER_ERROR);
    }

    return (ERROR_SCD30_NO_ERROR);
}

//Gets two bytes (and check CRC) from SCD30
int FrogmoreScd30::readRegister(uint16_t registerAddress, uint16_t* pData)
{
  int error = sendCommand(registerAddress);
  if (error)
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadRegister: SendCommand error: 0x%lX", error);
    AddLog(LOG_LEVEL_INFO);
#endif
    return (error);
  }
  delay(1); // the SCD30 uses clock streching to give it time to prepare data, waiting here makes it work
  uint8_t data[SCD30_DATA_REGISTER_WITH_CRC];
  error = getBytes(data, sizeof(data));
  if (error)
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadRegister: Scd30GetBytes error: 0x%lX", error);
    AddLog(LOG_LEVEL_INFO);
#endif
    return (error);
  }
  uint16 regValue;
  error = get16BitRegCheckCRC(data, &regValue);
  if (error)
  {
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadRegister: Scd30get16BitRegCheckCRC error: 0x%lX", error);
    AddLog(LOG_LEVEL_INFO);
#endif
    return (error);
  }

  *pData = regValue;
  return (ERROR_SCD30_NO_ERROR);
}

int FrogmoreScd30::softReset(void)
{
    return (sendCommand(COMMAND_SCD30_SOFT_RESET));
}

int FrogmoreScd30::getAltitudeCompensation(uint16_t *pHeight_meter)
{
  return (readRegister(COMMAND_SCD30_ALTITUDE_COMPENSATION, pHeight_meter));
}

int FrogmoreScd30::getAmbientPressure(uint16_t *pAirPressure_mbar)
{
  *pAirPressure_mbar = ambientPressure;
  return (ERROR_SCD30_NO_ERROR);
}

int FrogmoreScd30::getCalibrationType(uint16_t *pIsAuto)
{
    uint16_t value = 0;
    int error = readRegister(COMMAND_SCD30_CALIBRATION_TYPE, &value);
    if (!error)
    {
        *pIsAuto = value != 0;
    }
    return (error);
}

int FrogmoreScd30::getFirmwareVersion(uint8_t *pMajor, uint8_t *pMinor)
{
    uint16_t value;
    int error = readRegister(COMMAND_SCD30_GET_FW_VERSION, &value);
    if (!error)
    {
        *pMajor = value >> 8;
        *pMinor = value & 0xFF;
    }
    return (error);
}

int FrogmoreScd30::getForcedRecalibrationFactor(uint16_t *pCo2_ppm)
{
    return (readRegister(COMMAND_SCD30_FORCED_RECALIBRATION_FACTOR, pCo2_ppm));
}

int FrogmoreScd30::getMeasurementInterval(uint16_t *pTime_sec)
{
    return (readRegister(COMMAND_SCD30_MEASUREMENT_INTERVAL, pTime_sec));
}

int FrogmoreScd30::getTemperatureOffset(float *pOffset_degC)
{
    uint16_t value;
    int error = readRegister(COMMAND_SCD30_TEMPERATURE_OFFSET, &value);
    if (!error)
    {
        // result is in centi-degrees, need to convert to degrees
        *pOffset_degC = (float) value / 100.0;
    }
    return (error);
}

int FrogmoreScd30::getTemperatureOffset(uint16_t *pOffset_centiDegC)
{
    uint16_t value;
    int error = readRegister(COMMAND_SCD30_TEMPERATURE_OFFSET, &value);
    if (!error)
    {
        // result is in centi-degrees, need to convert to degrees
        *pOffset_centiDegC = value;
    }
    return (error);
}

int FrogmoreScd30::setAltitudeCompensation(uint16_t height_meter)
{
    return (sendCommandArguments(COMMAND_SCD30_ALTITUDE_COMPENSATION, height_meter));
}

int FrogmoreScd30::setAmbientPressure(uint16_t airPressure_mbar)
{
    ambientPressure = airPressure_mbar;
    return (beginMeasuring(ambientPressure));
}

int FrogmoreScd30::setAutoSelfCalibration(void)
{
    bool isAuto = true;
    return (setCalibrationType(isAuto));
}

int FrogmoreScd30::setCalibrationType(bool isAuto)
{
    bool value = !!isAuto; // using NOT operator twice makes sure value is 0 or 1
    return (sendCommandArguments(COMMAND_SCD30_CALIBRATION_TYPE, value));
}

int FrogmoreScd30::setForcedRecalibrationFactor(uint16_t co2_ppm)
{
    return (sendCommandArguments(COMMAND_SCD30_FORCED_RECALIBRATION_FACTOR, co2_ppm));
}

int FrogmoreScd30::setManualCalibration(void)
{
    bool isAuto = false;
    return (setCalibrationType(isAuto));
}

int FrogmoreScd30::setMeasurementInterval(uint16_t time_sec)
{
    if (time_sec < 2) time_sec = 2;
    if (time_sec > 1800) time_sec = 1800;
    return (sendCommandArguments(COMMAND_SCD30_MEASUREMENT_INTERVAL, time_sec));
}

int FrogmoreScd30::setTemperatureOffset(float offset_degC)
{
    uint16_t offset_centiDegC;
    if (offset_degC >= 0)
    {
        offset_centiDegC = (uint16_t) offset_degC * 100;
        return (sendCommandArguments(COMMAND_SCD30_TEMPERATURE_OFFSET, offset_centiDegC));
    }
    else
    {
        return (ERROR_SCD30_INVALID_VALUE);
    }

}

int FrogmoreScd30::setTemperatureOffset(uint16_t offset_centiDegC)
{
    return (sendCommandArguments(COMMAND_SCD30_TEMPERATURE_OFFSET, offset_centiDegC));
}

int FrogmoreScd30::beginMeasuring(void)
{
    return (beginMeasuring(ambientPressure));
}

int FrogmoreScd30::beginMeasuring(uint16_t airPressure_mbar)
{
    ambientPressure = airPressure_mbar;
    return(sendCommandArguments(COMMAND_SCD30_CONTINUOUS_MEASUREMENT, ambientPressure));
}

int FrogmoreScd30::isDataAvailable(bool *pIsAvailable)
{
    uint16_t isDataAvailable = false;
    int error = readRegister(COMMAND_SCD30_GET_DATA_READY, &isDataAvailable);
    if (!error)
    {
        *pIsAvailable = isDataAvailable != 0;
    }
    return (error);
}

int FrogmoreScd30::readMeasurement(
    uint16 *pCO2_ppm,
    uint16 *pCO2EAvg_ppm,
    float *pTemperature,
    float *pHumidity
)
{
    bool isAvailable = false;
    int error = 0;
    float tempCO2;
    float tempHumidity;
    float tempTemperature;

    error = isDataAvailable(&isAvailable);
    if (error)
    {
        return (error);
    }

    if (!isAvailable)
    {
        return (ERROR_SCD30_NO_DATA);
    }

#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: have data");
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    error = sendCommand(COMMAND_SCD30_READ_MEASUREMENT);
    if (error)
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: send command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    delay(1); // the SCD30 uses clock streching to give it time to prepare data, waiting here makes it work

    uint8_t bytes[SCD30_MEAS_BYTES];
    // there are (6) 16-bit values, each with a CRC in the measurement data
    // the chip does not seem to like sending this data, except all at once
    error = getBytes(bytes, SCD30_MEAS_BYTES);
    if (error)
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: Scd30GetBytes command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }

#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: Scd30GetBytes data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7], bytes[8]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: Scd30GetBytes data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15], bytes[16], bytes[17]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    error = get32BitRegCheckCRC(&bytes[0], &tempCO2);
    if (error)
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: Scd30Get32BitsCheckCRC 1st command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }

    error = get32BitRegCheckCRC(&bytes[6], &tempTemperature);
    if (error)
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: Scd30Get32BitsCheckCRC 2nd command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }

    error = get32BitRegCheckCRC(&bytes[12], &tempHumidity);
    if (error)
    {
#ifdef SCD30_DEBUG
        snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement: Scd30Get32BitsCheckCRC 3rd command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }

    if (tempCO2 == 0)
    {
        return (ERROR_SCD30_CO2_ZERO);
    }

    if (co2NewDataLocation < 0)
    {
        co2EAverage = tempCO2;
        for (int x = 0; x < SCD30_MEDIAN_FILTER_SIZE; x++)
        {
            co2History[x] = tempCO2;
            co2NewDataLocation = 1;
        }
    }
    else
    {
        co2History[co2NewDataLocation++] = tempCO2;
        if (co2NewDataLocation >= SCD30_MEDIAN_FILTER_SIZE)
        {
            co2NewDataLocation = 0;
        }
    }

#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement:  co2History: %ld, %ld, %ld, %ld, %ld", co2History[0], co2History[1], co2History[2], co2History[3], co2History[4]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    // copy array since the median filter function will re-arrange it
    uint16_t temp[SCD30_MEDIAN_FILTER_SIZE];
    for (int x = 0; x < SCD30_MEDIAN_FILTER_SIZE; x++)
    {
        temp[x] = co2History[x];
    }
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement:  temp: %ld, %ld, %ld, %ld, %ld", temp[0], temp[1], temp[2], temp[3], temp[4]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    *pCO2_ppm = opt_med5(temp);
#ifdef SCD30_DEBUG
    snprintf_P(scd30log_data, sizeof(scd30log_data), "Scd30ReadMeasurement:  CO2_ppm: %ld", *pCO2_ppm);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    if (pCO2EAvg_ppm)
    {
        int16_t delta = (int16_t) *pCO2_ppm - (int16_t) co2EAverage;
        int16_t change = delta / 32;
        co2EAverage += change;
#if 0
        uint16_t remain = co2EAverage % 5;
        uint16_t dividend = co2EAverage / 5;
        uint16_t co2EAReported = dividend * 5;
        if (remain > 2)
        {
            co2EAReported += 5;
        }
        *pCO2EAvg_ppm = co2EAReported;
#else
        *pCO2EAvg_ppm = co2EAverage;
#endif

    }

    *pTemperature = tempTemperature;
    *pHumidity = tempHumidity;
    return (ERROR_SCD30_NO_ERROR);
}

int FrogmoreScd30::stopMeasuring(void)
{
    return (sendCommand(COMMAND_SCD30_STOP_MEASUREMENT));
}
