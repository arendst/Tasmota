/*
  FrogmoreScd40.h - SCD40/SCD41 I2C CO2(+temp+RH) sensor support for Tasmota,
                    based on frogmore42's FrogmoreScd30.h

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
#include <Wire.h>
#include <math.h>
#include <stdio.h>
#include <twi.h>
#include <FrogmoreScd40.h>

// References are made to Sensirion datasheet at 
// https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/9.5_CO2/Sensirion_CO2_Sensors_SCD4x_Datasheet.pdf
//
// Basic Commands Chapter 3.5
#define COMMAND_SCD40_START_PERIODIC_MEASUREMENT             0x21b1
#define COMMAND_SCD40_READ_MEASUREMENT                       0xec05
#define COMMAND_SCD40_STOP_PERIODIC_MEASUREMENT              0x3f86

// On-chip output signal compensation Chapter 3.6
#define COMMAND_SCD40_SET_TEMPERATURE_OFFSET                 0x241d
#define COMMAND_SCD40_GET_TEMPERATURE_OFFSET                 0x2318
#define COMMAND_SCD40_SET_SENSOR_ALTITUDE                    0x2427
#define COMMAND_SCD40_GET_SENSOR_ALTITUDE                    0x2322
#define COMMAND_SCD40_SET_AMBIENT_PRESSURE                   0xe000

// Field calibration Chapter 3.7
#define COMMAND_SCD40_PERFORM_FORCED_RECALIBRATION           0x362f
#define COMMAND_SCD40_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED 0x2416
#define COMMAND_SCD40_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED 0x2313

// Low power Chapter 3.8
#define COMMAND_SCD40_START_LOW_POWER_PERIODIC_MEASUREMENT   0x21ac
#define COMMAND_SCD40_GET_DATA_READY_STATUS                  0xe4b8

// Advanced features Chapter 3.9
#define COMMAND_SCD40_PERSIST_SETTINGS                       0x3615
#define COMMAND_SCD40_GET_SERIAL_NUMBER                      0x3682
#define COMMAND_SCD40_PERFORM_SELF_TEST                      0x3639
#define COMMAND_SCD40_PERFORM_FACTORY_RESET                  0x3632
#define COMMAND_SCD40_REINIT                                 0x3646

// Low power single shot (SCD41 only) Chapter 3.10
// only for SCD41
#define COMMAND_SCD40_MEASURE_SINGLE_SHOT                    0x219d
#define COMMAND_SCD40_MEASURE_SINGLE_SHOT_RHT_ONLY           0x2196

#define SCD40_DATA_REGISTER_BYTES 2
#define SCD40_DATA_REGISTER_WITH_CRC 3
#define SCD40_MEAS_BYTES 9

#ifdef SCD40_DEBUG
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};
char scd40log_data[180];
#define SCD40_DEBUG_LOG_LEVEL LOG_LEVEL_INFO
#endif

// helper and private functions

/*---------------------------------------------------------------------------
 Function : medianfilter() 
 In : pointer to array of SCD40_MEDIAN_FILTER_SIZE values
 Out : a uint16_t which is the middle value of the array
 Job : search of the median
 Notice : replaced SCD30 alg by partial bubble-sort, slightly slower, but not fixed-size
 ---------------------------------------------------------------------------*/
#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
#define PIX_SWAP(a,b) { uint16_t temp=(a);(a)=(b);(b)=temp; }

uint16_t FrogmoreScd40::medianfilter(uint16_t * p)
{
    for (int8_t i = SCD40_MEDIAN_FILTER_SIZE-1; i >= (SCD40_MEDIAN_FILTER_SIZE-1)/2; i--)
    {
        for (uint8_t j=0; j < i; j++)
        {
            PIX_SORT(p[j], p[j+1]);
        }
    }
    return(p[(SCD40_MEDIAN_FILTER_SIZE-1)/2]);
}

#ifdef SCD40_DEBUG
void FrogmoreScd40::AddLog(uint8_t loglevel)
{
    if (loglevel <= SCD40_DEBUG_LOG_LEVEL)
    {
        Serial.printf("%s\r\n", scd40log_data);
    }
}
#endif

uint8_t FrogmoreScd40::computeCRC8(uint8_t data[], uint8_t len)
// Computes the CRC that the SCD40 uses
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
int FrogmoreScd40::sendBytes(void *pInput, uint8_t len)
{
    uint8_t *pBytes = (uint8_t *) pInput;
    int result;
    uint8_t errorBytes = 0; // number of bytes that had an error in transmission
#ifdef SCD40_DEBUG
    snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40SendBytes: len: %d data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", len, pBytes[0], pBytes[1], pBytes[2], pBytes[3], pBytes[4], pBytes[5], pBytes[6], pBytes[7], pBytes[8]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    pWire->beginTransmission(this->i2cAddress);
    errorBytes = len - (pWire->write(pBytes, len));
    result = pWire->endTransmission();
    if (errorBytes || result)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40SendBytes: errorBytes: %d | Wire.end: %d", errorBytes, result);
        AddLog(LOG_LEVEL_INFO);
#endif
    }
    result <<= 8; // leave room for error bytes number
    result |= errorBytes; // low byte has number of bytes that were not written correctly
    return (result);
}

// Gets a number of bytes from device
int FrogmoreScd40::getBytes(void *pOutput, uint8_t len)
{
    uint8_t *pBytes = (uint8_t *) pOutput;
    uint8_t result;

    result = pWire->requestFrom(this->i2cAddress, len);
    if (len != result)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40GetBytes: wire request expected %d got: %d", len, result);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (ERROR_SCD40_NOT_ENOUGH_BYTES_ERROR);
    }

    if (pWire->available())
    {
        for (int x = 0; x < len; x++)
        {
            pBytes[x] = pWire->read();
        }
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40GetBytes: len: %d data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", len, pBytes[0], pBytes[1], pBytes[2], pBytes[3], pBytes[4], pBytes[5], pBytes[6], pBytes[7], pBytes[8]);
        AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
        return (ERROR_SCD40_NO_ERROR);
    }
    return (ERROR_SCD40_UNKNOWN_ERROR);
}

//Sends just a command, no arguments, no CRC
int FrogmoreScd40::sendCommand(uint16_t command)
{
    uint8_t data[2];
    data[0] = command >> 8;
    data[1] = command & 0xFF;
    int error = sendBytes(data, sizeof(data));
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40SendCommand: sendBytes failed, error: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
    }
    return (error);
}

//Sends a command along with arguments and CRC
int FrogmoreScd40::sendCommandArguments(uint16_t command, uint16_t arguments)
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
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40SendCommandArguments: sendBytes failed, error: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
    }
    return (error);
}

// Sends a command along with arguments and CRC, wait 400ms, fetch results
// NOT TESTED - not sure whether this works
int FrogmoreScd40::sendCommandArgumentsFetchResult(uint16_t command, uint16_t arguments, uint16_t* pData)
{
    sendCommandArguments(command, arguments);
    delay(400); // the SCD30 uses clock stretching to give it time to prepare data, waiting here makes it work, seems this works also for SCD40
    uint8_t data[SCD40_DATA_REGISTER_WITH_CRC];
    int error = getBytes(data, sizeof(data));
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadRegister: Scd40GetBytes error: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    uint16 regValue;
    error = get16BitRegCheckCRC(data, &regValue);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadRegister: Scd40get16BitRegCheckCRC error: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    *pData = regValue;
    return (ERROR_SCD40_NO_ERROR);
}

int FrogmoreScd40::get16BitRegCheckCRC(void* pInput, uint16_t *pData)
{
    uint8_t *pBytes = (uint8_t *) pInput;
    uint8_t expectedCRC = computeCRC8(pBytes, SCD40_DATA_REGISTER_BYTES);
    if (expectedCRC != pBytes[SCD40_DATA_REGISTER_BYTES])
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40get16BitRegCheckCRC: expected: 0x%02X, but got: 0x%02X", expectedCRC, pBytes[SCD40_DATA_REGISTER_BYTES]);
        AddLog(LOG_LEVEL_INFO);
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40get16BitRegCheckCRC: data: 0x%02X, 0x%02X, 0x%02X", pBytes[0], pBytes[1], pBytes[2]);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (ERROR_SCD40_CRC_ERROR);
    }
    *pData = (uint16_t) pBytes[0] << 8 | pBytes[1]; // data from SCD40 is Big-Endian
    return (ERROR_SCD40_NO_ERROR);
}

//Gets two bytes (and check CRC) from SCD40
int FrogmoreScd40::readRegisterCnt(uint16_t registerAddress, uint16_t* pData, uint8_t cnt)
{
    int error = sendCommand(registerAddress);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadRegister: SendCommand error: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    delay(1); // the SCD30 uses clock stretching to give it time to prepare data, waiting here makes it work, seems this works also for SCD40
    uint8_t data[SCD40_DATA_REGISTER_WITH_CRC];
    for (uint8_t c = 0; c < cnt; c++) {
        error = getBytes(data, sizeof(data));
        if (error)
        {
#ifdef SCD40_DEBUG
            snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadRegister: Scd40GetBytes error: 0x%lX", error);
            AddLog(LOG_LEVEL_INFO);
#endif
            return (error);
        }
        uint16 regValue;
        error = get16BitRegCheckCRC(data, &regValue);
        if (error)
        {
#ifdef SCD40_DEBUG
            snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadRegister: Scd40get16BitRegCheckCRC error: 0x%lX", error);
            AddLog(LOG_LEVEL_INFO);
#endif
            return (error);
        }
        pData[c] = regValue;
    }
    return (ERROR_SCD40_NO_ERROR);
}

int FrogmoreScd40::readRegister(uint16_t registerAddress, uint16_t* pData)
{
    int error=readRegisterCnt(registerAddress, pData, 1);
    return (error);
}

// public functions

void FrogmoreScd40::begin(TwoWire *pWire, uint8_t i2cAddress)
{
    this->duringMeasurement = 0;
    this->i2cAddress = i2cAddress;
    this->co2EAverage = 0;
    if (pWire == NULL)
    {
        this->pWire = &Wire;
    }
    else
    {
        this->pWire = pWire;
    }
    co2NewDataLocation = -1; // indicates there is no data, so the 1st non-zero data point needs to fill up the median filter
#ifdef ESP8266
    this->pWire->setClockStretchLimit(200000);
#endif
}

void FrogmoreScd40::begin(uint8_t i2cAddress)
{
    begin(NULL, i2cAddress);
}

void FrogmoreScd40::begin(TwoWire *pWire)
{
    begin(pWire, SCD40_ADDRESS);
}

void FrogmoreScd40::begin(void)
{
    begin(NULL, SCD40_ADDRESS);
}

// twi_status() attempts to read out any data left that is holding SDA low, so a new transaction can take place
// something like (http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html)
int FrogmoreScd40::clearI2CBus(void)
{
#ifdef SCD40_DEBUG
    snprintf_P(scd40log_data, sizeof(scd40log_data), "clearI2CBus");
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
#ifdef ESP8266
    return (twi_status());
#else
    return 0;
#endif
}

// Function order below follows SCD40 datasheet
// Basic Commands Chapter 3.5

int FrogmoreScd40::startPeriodicMeasurement(void)
{
#ifdef SCD40_DEBUG
    snprintf_P(scd40log_data, sizeof(scd40log_data), "Start periodic measurement");
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    if (DuringMeasurement) {
       return (ERROR_SCD40_BUSY_MEASURING);
    }
    DuringMeasurement = 1;
    return(sendCommand(COMMAND_SCD40_START_PERIODIC_MEASUREMENT));
}

int FrogmoreScd40::readMeasurement(
    uint16 *pCO2_ppm,
    uint16 *pCO2EAvg_ppm,
    float *pTemperature,
    float *pHumidity
)
{
// Should only be called in DuringMeasurement mode or
// after calling measure_single_hot{,_rht_only}
// but this is currently not verified
    bool isAvailable = false;
    int error = 0;
    uint16 tempCO2;
    uint16 tempHumidity;
    uint16 tempTemperature;

    error = getDataReadyStatus(&isAvailable);
    if (error)
    {
        return (error);
    }
    if (!isAvailable)
    {
        return (ERROR_SCD40_NO_DATA);
    }
#ifdef SCD40_DEBUG
    snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: have data");
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    error = sendCommand(COMMAND_SCD40_READ_MEASUREMENT);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: send command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    delay(1); // the SCD40 uses clock streching to give it time to prepare data, waiting here makes it work
    uint8_t bytes[SCD40_MEAS_BYTES];
    // there are (3) 16-bit values, each with a CRC in the measurement data
    // the chip sends all of these, unless stopped by an early NACK - not supported here
    error = getBytes(bytes, SCD40_MEAS_BYTES);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: Scd40GetBytes command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
#ifdef SCD40_DEBUG
    snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: Scd40GetBytes data: 0x %02X %02X %02X | 0x %02X %02X %02X | 0x %02X %02X %02X", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7], bytes[8]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    error = get16BitRegCheckCRC(&bytes[0], &tempCO2);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: Scd40Get16BitsCheckCRC 1st command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    error = get16BitRegCheckCRC(&bytes[3], &tempTemperature);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: Scd40Get16BitsCheckCRC 2nd command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    error = get16BitRegCheckCRC(&bytes[6], &tempHumidity);
    if (error)
    {
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement: Scd40Get16BitsCheckCRC 3rd command failed: 0x%lX", error);
        AddLog(LOG_LEVEL_INFO);
#endif
        return (error);
    }
    // tempCO2 = 0 occurs after Measure_single_shot_RHT_only; no reason for error like for SCD30, but don't add to history,
    // and take care to handle special case where no CO2 measurement was seen yet
    if (tempCO2 > 0) 
    {
        // add tempCO2 measurement to history
        if (co2NewDataLocation < 0)
        {
            co2EAverage = tempCO2;
            for (int x = 0; x < SCD40_MEDIAN_FILTER_SIZE; x++)
            {
                co2History[x] = tempCO2;
                co2NewDataLocation = 1;
            }
        }
        else
        {
            co2History[co2NewDataLocation++] = tempCO2;
            if (co2NewDataLocation >= SCD40_MEDIAN_FILTER_SIZE)
            {
                co2NewDataLocation = 0;
            }
        }
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement:  co2History: %ld, %ld, %ld, %ld, %ld", co2History[0], co2History[1], co2History[2], co2History[3], co2History[4]);
        AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    }
    if ((tempCO2 > 0) || (co2NewDataLocation >= 0)) {
        // find median of history; copy array since the median filter function will re-arrange it
        uint16_t temp[SCD40_MEDIAN_FILTER_SIZE];
        for (int x = 0; x < SCD40_MEDIAN_FILTER_SIZE; x++)
        {
            temp[x] = co2History[x];
        }
#ifdef SCD40_DEBUG
        snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement:  temp: %ld, %ld, %ld, %ld, %ld", temp[0], temp[1], temp[2], temp[3], temp[4]);
        AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
        *pCO2_ppm = medianfilter(temp);
    } else {
        *pCO2_ppm = 0; // never seen real CO2 measurement, but need to return something: return 0
    }
#ifdef SCD40_DEBUG
    snprintf_P(scd40log_data, sizeof(scd40log_data), "Scd40ReadMeasurement:  CO2_ppm: %ld", *pCO2_ppm);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
    if ((pCO2EAvg_ppm) && (tempCO2 > 0))
    {
        int16_t delta = (int16_t) *pCO2_ppm - (int16_t) co2EAverage;
        int16_t change = delta / 32;
        co2EAverage += change;
        *pCO2EAvg_ppm = co2EAverage;
    }
    *pTemperature = (175.0 * tempTemperature) / 65536 - 45;
    *pHumidity = (100.0 * tempHumidity) / 65536;
    return (ERROR_SCD40_NO_ERROR);
}

int FrogmoreScd40::forceStopPeriodicMeasurement(void)
{
    DuringMeasurement = 0;
    return (sendCommand(COMMAND_SCD40_STOP_PERIODIC_MEASUREMENT));
}

int FrogmoreScd40::stopPeriodicMeasurement(void)
{
    if (!DuringMeasurement) {
        return (ERROR_SCD40_NOT_IN_MEASUREMENT_MODE);
    }
    DuringMeasurement = 0;
    return (sendCommand(COMMAND_SCD40_STOP_PERIODIC_MEASUREMENT));
}

// On-chip output signal compensation Chapter 3.6

int FrogmoreScd40::setTemperatureOffset(float offset_degC)
// influences RH and T readings. Does not influence CO2 measurement. Default is 4 degrees Celcius.
// to save setting to the EEPROM, call persistSetting()
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    if (offset_degC >= 0)
    {
        uint16_t offset_xDegC = (uint16_t) (offset_degC * 374.491);
        return (sendCommandArguments(COMMAND_SCD40_SET_TEMPERATURE_OFFSET, offset_xDegC));
    }
    else
    {
        return (ERROR_SCD40_INVALID_VALUE);
    }
}

int FrogmoreScd40::setTemperatureOffset(uint16_t offset_centiDegC)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    uint16_t offset_xDegC = (uint16_t) (offset_centiDegC * 3.74491);
    return (sendCommandArguments(COMMAND_SCD40_SET_TEMPERATURE_OFFSET, offset_xDegC));
}

int FrogmoreScd40::getTemperatureOffset(float *pOffset_degC)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    uint16_t value;
    int error = readRegister(COMMAND_SCD40_GET_TEMPERATURE_OFFSET, &value);
    if (!error)
    {
        // result is 175 * value/2^16, need to convert to degrees
        *pOffset_degC = (float) value / 374.491;
    }
    return (error);
}

int FrogmoreScd40::getTemperatureOffset(uint16_t *pOffset_centiDegC)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    uint16_t value;
    int error = readRegister(COMMAND_SCD40_GET_TEMPERATURE_OFFSET, &value);
    if (!error)
    {
        // result is 175 * value/2^16, need to convert to degrees
        *pOffset_centiDegC = (uint16_t) (value / 3.74491);
    }
    return (error);
}

int FrogmoreScd40::setSensorAltitude(uint16_t height_meter)
// Default is 0 meter above sea-level;
// to save setting to the EEPROM, call persistSetting()
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommandArguments(COMMAND_SCD40_SET_SENSOR_ALTITUDE, height_meter));
}

int FrogmoreScd40::getSensorAltitude(uint16_t *pHeight_meter)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (readRegister(COMMAND_SCD40_GET_SENSOR_ALTITUDE, pHeight_meter));
}

int FrogmoreScd40::setAmbientPressure(uint16_t airPressure_mbar)
// Overrides any pressure compensation based on a previously set sensor altitude
{
    // allowed DuringMeasurement
    return (sendCommandArguments(COMMAND_SCD40_SET_AMBIENT_PRESSURE, airPressure_mbar));
}

// Field calibration Chapter 3.7

int FrogmoreScd40::performForcedRecalibration(uint16_t co2_ppm)
// Calibrates with a CO2 reference value immediately
// Use only in planned operation mode, for >3 minutes in constant CO2 environment
// issue stop_periodic_measurement, and then wait 500ms, before calling this function
// it takes 400ms for this command to complete
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    uint16_t FRC_result;
    sendCommandArgumentsFetchResult(COMMAND_SCD40_PERFORM_FORCED_RECALIBRATION, co2_ppm, &FRC_result);
    if (FRC_result == 0xffff) {
        // a return value of 0xffff indicates failure
        return(ERROR_SCD40_FRC_FAILED);
    }
    return (ERROR_SCD40_NO_ERROR);
}

int FrogmoreScd40::setAutomaticSelfCalibrationDisabled(void)
// to save setting to the EEPROM, call persistSetting()
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    bool isAuto = false;
    return (setAutomaticSelfCalibrationEnabled(isAuto));
}

int FrogmoreScd40::setAutomaticSelfCalibrationEnabled(void)
// to save setting to the EEPROM, call persistSetting()
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    bool isAuto = true;
    return (setAutomaticSelfCalibrationEnabled(isAuto));
}

int FrogmoreScd40::setAutomaticSelfCalibrationEnabled(bool isAuto)
// to save setting to the EEPROM, call persistSetting()
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    bool value = !!isAuto; // using NOT operator twice makes sure value is 0 or 1
    return (sendCommandArguments(COMMAND_SCD40_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED, value));
}

int FrogmoreScd40::getAutomaticSelfCalibrationEnabled(uint16_t *pIsAuto)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    uint16_t value = 0;
    int error = readRegister(COMMAND_SCD40_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED, &value);
    if (!error)
    {
        *pIsAuto = value != 0;
    }
    return (error);
}

// Low power Chapter 3.8

int FrogmoreScd40::startLowPowerPeriodicMeasurement(void)
// Comment: unclear how to stop this operation mode?
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommand(COMMAND_SCD40_START_LOW_POWER_PERIODIC_MEASUREMENT));
    DuringMeasurement = 1;
}

int FrogmoreScd40::getDataReadyStatus(bool *pIsAvailable)
{
    // allowed DuringMeasurement
    uint16_t isDataAvailable = false;
    int error = readRegister(COMMAND_SCD40_GET_DATA_READY_STATUS, &isDataAvailable);
    if (!error)
    {
        *pIsAvailable = (isDataAvailable & 0x07ff) != 0;
    }
    return (error);
}

// Advanced features Chapter 3.9

int FrogmoreScd40::persistSettings(void)
// Store configuration settings such as temperature offset,
// sensor altitude, and ASC enable/disabled parameter
// EEPROM is guaranteed to endure at least 2000 write cycles before failure.
// The field calibration history (FRC and ASC) is stored in a separate EEPROM.
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommand(COMMAND_SCD40_PERSIST_SETTINGS));
}

int FrogmoreScd40::getSerialNumber(uint16_t *pSerialNumberArray)
// Serialnr is 48 bits = 3 16-bit words
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    uint16_t value;
    int error = readRegisterCnt(COMMAND_SCD40_GET_SERIAL_NUMBER, pSerialNumberArray, 3);
    return (error);
}

int FrogmoreScd40::performSelfTest(uint16_t *pMalfunction)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (readRegister(COMMAND_SCD40_PERFORM_SELF_TEST, pMalfunction));
}

int FrogmoreScd40::performFactoryReset(void)
// resets all configuration settings in EEPROM and
// erases FRC and ASC algorithm history
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommand(COMMAND_SCD40_PERFORM_FACTORY_RESET));
}

int FrogmoreScd40::reinit(void)
// reinitailizes sensor from EEPROM user settings
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommand(COMMAND_SCD40_REINIT));
}

// Low power single shot (SCD41 only) Chapter 3.10
// (on-demand measurements)

int FrogmoreScd40::measureSingleShot(void)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommand(COMMAND_SCD40_MEASURE_SINGLE_SHOT));
}

int FrogmoreScd40::measureSingleShotRhtOnly(void)
{
    if (DuringMeasurement) {
         return (ERROR_SCD40_BUSY_MEASURING);
    }
    return (sendCommand(COMMAND_SCD40_MEASURE_SINGLE_SHOT_RHT_ONLY));
}
