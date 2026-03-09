/*
 * THIS FILE WAS AUTOMATICALLY GENERATED
 *
 * Generator:     sensirion-driver-generator 1.1.2
 * Product:       sen6x
 * Model-Version: 1.6.0
 */
/*
 * Copyright (c) 2025, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "SensirionI2cSen6x.h"
#include <Arduino.h>

// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

static uint8_t communication_buffer[48] = {0};

SensirionI2cSen6x::SensirionI2cSen6x() {
}

float SensirionI2cSen6x::signalMassConcentrationPm1p0(
    uint16_t massConcentrationPm1p0Raw) {
    float massConcentrationPm1p0 = 0.0;
    massConcentrationPm1p0 = massConcentrationPm1p0Raw / 10.0;
    return massConcentrationPm1p0;
}

float SensirionI2cSen6x::signalMassConcentrationPm2p5(
    uint16_t massConcentrationPm2p5Raw) {
    float massConcentrationPm2p5 = 0.0;
    massConcentrationPm2p5 = massConcentrationPm2p5Raw / 10.0;
    return massConcentrationPm2p5;
}

float SensirionI2cSen6x::signalMassConcentrationPm4p0(
    uint16_t massConcentrationPm4p0Raw) {
    float massConcentrationPm4p0 = 0.0;
    massConcentrationPm4p0 = massConcentrationPm4p0Raw / 10.0;
    return massConcentrationPm4p0;
}

float SensirionI2cSen6x::signalMassConcentrationPm10p0(
    uint16_t massConcentrationPm10p0Raw) {
    float massConcentrationPm10p0 = 0.0;
    massConcentrationPm10p0 = massConcentrationPm10p0Raw / 10.0;
    return massConcentrationPm10p0;
}

float SensirionI2cSen6x::signalNumberConcentrationPm0p5(
    uint16_t numberConcentrationPm0p5Raw) {
    float numberConcentrationPm0p5 = 0.0;
    numberConcentrationPm0p5 = numberConcentrationPm0p5Raw / 10.0;
    return numberConcentrationPm0p5;
}

float SensirionI2cSen6x::signalNumberConcentrationPm1p0(
    uint16_t numberConcentrationPm1p0Raw) {
    float numberConcentrationPm1p0 = 0.0;
    numberConcentrationPm1p0 = numberConcentrationPm1p0Raw / 10.0;
    return numberConcentrationPm1p0;
}

float SensirionI2cSen6x::signalNumberConcentrationPm2p5(
    uint16_t numberConcentrationPm2p5Raw) {
    float numberConcentrationPm2p5 = 0.0;
    numberConcentrationPm2p5 = numberConcentrationPm2p5Raw / 10.0;
    return numberConcentrationPm2p5;
}

float SensirionI2cSen6x::signalNumberConcentrationPm4p0(
    uint16_t numberConcentrationPm4p0Raw) {
    float numberConcentrationPm4p0 = 0.0;
    numberConcentrationPm4p0 = numberConcentrationPm4p0Raw / 10.0;
    return numberConcentrationPm4p0;
}

float SensirionI2cSen6x::signalNumberConcentrationPm10p0(
    uint16_t numberConcentrationPm10p0Raw) {
    float numberConcentrationPm10p0 = 0.0;
    numberConcentrationPm10p0 = numberConcentrationPm10p0Raw / 10.0;
    return numberConcentrationPm10p0;
}

float SensirionI2cSen6x::signalTemperature(int16_t temperatureRaw) {
    float temperature = 0.0;
    temperature = temperatureRaw / 200.0;
    return temperature;
}

float SensirionI2cSen6x::signalHumidity(int16_t humidityRaw) {
    float humidity = 0.0;
    humidity = humidityRaw / 100.0;
    return humidity;
}

float SensirionI2cSen6x::signalVocIndex(int16_t vocIndexRaw) {
    float vocIndex = 0.0;
    vocIndex = vocIndexRaw / 10.0;
    return vocIndex;
}

float SensirionI2cSen6x::signalNoxIndex(int16_t noxIndexRaw) {
    float noxIndex = 0.0;
    noxIndex = noxIndexRaw / 10.0;
    return noxIndex;
}

uint16_t SensirionI2cSen6x::signalCo2(uint16_t co2Raw) {
    uint16_t co2 = 0;
    co2 = co2Raw;
    return co2;
}

float SensirionI2cSen6x::signalHcho(uint16_t hchoRaw) {
    float hcho = 0.0;
    hcho = hchoRaw / 10.0;
    return hcho;
}

int16_t SensirionI2cSen6x::readMeasuredValues(
    uint32_t model,
    float& massConcentrationPm1p0, float& massConcentrationPm2p5,
    float& massConcentrationPm4p0, float& massConcentrationPm10p0,
    float& humidity, float& temperature, float& vocIndex, float& noxIndex,
    uint16_t& co2, float& hcho) {
    uint16_t massConcentrationPm1p0Raw = 0;
    uint16_t massConcentrationPm2p5Raw = 0;
    uint16_t massConcentrationPm4p0Raw = 0;
    uint16_t massConcentrationPm10p0Raw = 0;
    int16_t humidityRaw = 0;
    int16_t temperatureRaw = 0;
    int16_t vocIndexRaw = 0;
    int16_t noxIndexRaw = 0;
    uint16_t co2Raw = 0;
    uint16_t hchoRaw = 0;
    int16_t localError = 0;
    localError = readMeasuredValuesAsIntegers(
        model,
        massConcentrationPm1p0Raw, massConcentrationPm2p5Raw,
        massConcentrationPm4p0Raw, massConcentrationPm10p0Raw, humidityRaw,
        temperatureRaw, vocIndexRaw, noxIndexRaw, co2Raw, hchoRaw);
    if (localError != NO_ERROR) {
        return localError;
    }
    massConcentrationPm1p0 = SensirionI2cSen6x::signalMassConcentrationPm1p0(
        massConcentrationPm1p0Raw);
    massConcentrationPm2p5 = SensirionI2cSen6x::signalMassConcentrationPm2p5(
        massConcentrationPm2p5Raw);
    massConcentrationPm4p0 = SensirionI2cSen6x::signalMassConcentrationPm4p0(
        massConcentrationPm4p0Raw);
    massConcentrationPm10p0 = SensirionI2cSen6x::signalMassConcentrationPm10p0(
        massConcentrationPm10p0Raw);
    humidity = SensirionI2cSen6x::signalHumidity(humidityRaw);
    temperature = SensirionI2cSen6x::signalTemperature(temperatureRaw);
    vocIndex = SensirionI2cSen6x::signalVocIndex(vocIndexRaw);
    noxIndex = SensirionI2cSen6x::signalNoxIndex(noxIndexRaw);
    co2 = SensirionI2cSen6x::signalCo2(co2Raw);
    hcho = SensirionI2cSen6x::signalHcho(hchoRaw);
    return localError;
}

int16_t SensirionI2cSen6x::readNumberConcentrationValues(
    float& numberConcentrationPm0p5, float& numberConcentrationPm1p0,
    float& numberConcentrationPm2p5, float& numberConcentrationPm4p0,
    float& numberConcentrationPm10p0) {
    uint16_t numberConcentrationPm0p5Raw = 0;
    uint16_t numberConcentrationPm1p0Raw = 0;
    uint16_t numberConcentrationPm2p5Raw = 0;
    uint16_t numberConcentrationPm4p0Raw = 0;
    uint16_t numberConcentrationPm10p0Raw = 0;
    int16_t localError = 0;
    localError = readNumberConcentrationValuesAsIntegers(
        numberConcentrationPm0p5Raw, numberConcentrationPm1p0Raw,
        numberConcentrationPm2p5Raw, numberConcentrationPm4p0Raw,
        numberConcentrationPm10p0Raw);
    if (localError != NO_ERROR) {
        return localError;
    }
    numberConcentrationPm0p5 =
        SensirionI2cSen6x::signalNumberConcentrationPm0p5(
            numberConcentrationPm0p5Raw);
    numberConcentrationPm1p0 =
        SensirionI2cSen6x::signalNumberConcentrationPm1p0(
            numberConcentrationPm1p0Raw);
    numberConcentrationPm2p5 =
        SensirionI2cSen6x::signalNumberConcentrationPm2p5(
            numberConcentrationPm2p5Raw);
    numberConcentrationPm4p0 =
        SensirionI2cSen6x::signalNumberConcentrationPm4p0(
            numberConcentrationPm4p0Raw);
    numberConcentrationPm10p0 =
        SensirionI2cSen6x::signalNumberConcentrationPm10p0(
            numberConcentrationPm10p0Raw);
    return localError;
}

int16_t SensirionI2cSen6x::startContinuousMeasurement() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x21, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(50);
    return localError;
}

int16_t SensirionI2cSen6x::stopMeasurement() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x104, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1000);
    return localError;
}

int16_t SensirionI2cSen6x::getDataReady(uint8_t& padding, bool& dataReady) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x202, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt8(padding);
    localError |= rxFrame.getBool(dataReady);
    return localError;
}

int16_t SensirionI2cSen6x::readMeasuredValuesAsIntegers(
    uint32_t model,
    uint16_t& massConcentrationPm1p0, uint16_t& massConcentrationPm2p5,
    uint16_t& massConcentrationPm4p0, uint16_t& massConcentrationPm10p0,
    int16_t& ambientHumidity, int16_t& ambientTemperature, int16_t& vOCIndex,
    int16_t& nOxIndex, uint16_t& cO2, uint16_t& hCHO) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;

    uint16_t command = 0x300;   // SEN66
    size_t bufferSize = 27;
    if (62 == model) {
      command = 0x4A3;
      bufferSize = 18;
    }
    else if (63 == model) {
      command = 0x471;
      bufferSize = 21;
    }
    else if (65 == model) {
      command = 0x446;
      bufferSize = 24;
    }
/*
    else if (66 == model) {
      command = 0x300;
      bufferSize = 27;
    }
*/
    else if (68 == model) {
      command = 0x467;
      bufferSize = 27;
    }
    else if (69 == model) {
      command = 0x4B5;
      bufferSize = 30;
    }

    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(command, buffer_ptr, bufferSize);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, bufferSize);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, bufferSize,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(massConcentrationPm1p0);
    localError |= rxFrame.getUInt16(massConcentrationPm2p5);
    localError |= rxFrame.getUInt16(massConcentrationPm4p0);
    localError |= rxFrame.getUInt16(massConcentrationPm10p0);
    localError |= rxFrame.getInt16(ambientHumidity);
    localError |= rxFrame.getInt16(ambientTemperature);
    vOCIndex = SEN6X_INT_INVALID;
    nOxIndex = SEN6X_INT_INVALID;
    if ((65 == model) || (66 == model) || (68 == model) || (69 == model)) {
      localError |= rxFrame.getInt16(vOCIndex);
      localError |= rxFrame.getInt16(nOxIndex);
    }
    cO2 = SEN6X_UINT_INVALID;
    if ((63 == model) || (66 == model) || (69 == model)) {
      localError |= rxFrame.getUInt16(cO2);
    }
    hCHO = SEN6X_UINT_INVALID;
    if ((68 == model) || (69 == model)) {
      localError |= rxFrame.getUInt16(hCHO);
    }
    return localError;
}

int16_t SensirionI2cSen6x::readNumberConcentrationValuesAsIntegers(
    uint16_t& numberConcentrationPm0p5, uint16_t& numberConcentrationPm1p0,
    uint16_t& numberConcentrationPm2p5, uint16_t& numberConcentrationPm4p0,
    uint16_t& numberConcentrationPm10p0) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x316, buffer_ptr, 15);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 15);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 15,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(numberConcentrationPm0p5);
    localError |= rxFrame.getUInt16(numberConcentrationPm1p0);
    localError |= rxFrame.getUInt16(numberConcentrationPm2p5);
    localError |= rxFrame.getUInt16(numberConcentrationPm4p0);
    localError |= rxFrame.getUInt16(numberConcentrationPm10p0);
    return localError;
}

int16_t SensirionI2cSen6x::readMeasuredRawValues(int16_t& rawHumidity,
                                                 int16_t& rawTemperature,
                                                 uint16_t& rawVOC,
                                                 uint16_t& rawNOx,
                                                 uint16_t& rawCO2) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x405, buffer_ptr, 15);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 15);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 15,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getInt16(rawHumidity);
    localError |= rxFrame.getInt16(rawTemperature);
    localError |= rxFrame.getUInt16(rawVOC);
    localError |= rxFrame.getUInt16(rawNOx);
    localError |= rxFrame.getUInt16(rawCO2);
    return localError;
}

int16_t SensirionI2cSen6x::startFanCleaning() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x5607, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::setTemperatureOffsetParameters(int16_t offset,
                                                          int16_t slope,
                                                          uint16_t timeConstant,
                                                          uint16_t slot) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x60b2, buffer_ptr, 14);
    localError |= txFrame.addInt16(offset);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(slope);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addUInt16(timeConstant);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addUInt16(slot);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::setVocAlgorithmTuningParameters(
    int16_t indexOffset, int16_t learningTimeOffsetHours,
    int16_t learningTimeGainHours, int16_t gatingMaxDurationMinutes,
    int16_t stdInitial, int16_t gainFactor) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x60d0, buffer_ptr, 20);
    localError |= txFrame.addInt16(indexOffset);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(learningTimeOffsetHours);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(learningTimeGainHours);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(gatingMaxDurationMinutes);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(stdInitial);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(gainFactor);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::getVocAlgorithmTuningParameters(
    int16_t& indexOffset, int16_t& learningTimeOffsetHours,
    int16_t& learningTimeGainHours, int16_t& gatingMaxDurationMinutes,
    int16_t& stdInitial, int16_t& gainFactor) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x60d0, buffer_ptr, 18);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 18);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 18,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getInt16(indexOffset);
    localError |= rxFrame.getInt16(learningTimeOffsetHours);
    localError |= rxFrame.getInt16(learningTimeGainHours);
    localError |= rxFrame.getInt16(gatingMaxDurationMinutes);
    localError |= rxFrame.getInt16(stdInitial);
    localError |= rxFrame.getInt16(gainFactor);
    return localError;
}

int16_t SensirionI2cSen6x::setNoxAlgorithmTuningParameters(
    int16_t indexOffset, int16_t learningTimeOffsetHours,
    int16_t learningTimeGainHours, int16_t gatingMaxDurationMinutes,
    int16_t stdInitial, int16_t gainFactor) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x60e1, buffer_ptr, 20);
    localError |= txFrame.addInt16(indexOffset);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(learningTimeOffsetHours);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(learningTimeGainHours);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(gatingMaxDurationMinutes);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(stdInitial);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addInt16(gainFactor);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::getNoxAlgorithmTuningParameters(
    int16_t& indexOffset, int16_t& learningTimeOffsetHours,
    int16_t& learningTimeGainHours, int16_t& gatingMaxDurationMinutes,
    int16_t& stdInitial, int16_t& gainFactor) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x60e1, buffer_ptr, 18);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 18);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 18,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getInt16(indexOffset);
    localError |= rxFrame.getInt16(learningTimeOffsetHours);
    localError |= rxFrame.getInt16(learningTimeGainHours);
    localError |= rxFrame.getInt16(gatingMaxDurationMinutes);
    localError |= rxFrame.getInt16(stdInitial);
    localError |= rxFrame.getInt16(gainFactor);
    return localError;
}

int16_t SensirionI2cSen6x::setTemperatureAccelerationParameters(uint16_t k,
                                                                uint16_t p,
                                                                uint16_t t1,
                                                                uint16_t t2) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6100, buffer_ptr, 14);
    localError |= txFrame.addUInt16(k);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addUInt16(p);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addUInt16(t1);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addUInt16(t2);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::setVocAlgorithmState(const uint8_t state[],
                                                uint16_t stateSize) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6181, buffer_ptr, 14);
    localError |= txFrame.addBytes(state, stateSize);
    if (localError != NO_ERROR) {
        return localError;
    }

    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    return localError;
}

int16_t SensirionI2cSen6x::getVocAlgorithmState(uint8_t state[],
                                                uint16_t stateSize) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6181, buffer_ptr, 12);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 12);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 12,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getBytes((uint8_t*)state, stateSize);
    return localError;
}

int16_t SensirionI2cSen6x::performForcedCo2Recalibration(
    uint16_t targetCo2Concentration, uint16_t& correction) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6707, buffer_ptr, 5);
    localError |= txFrame.addUInt16(targetCo2Concentration);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(500);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 5);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(correction);
    return localError;
}

int16_t
SensirionI2cSen6x::setCo2SensorAutomaticSelfCalibration(uint16_t status) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6711, buffer_ptr, 5);
    localError |= txFrame.addUInt16(status);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t
SensirionI2cSen6x::getCo2SensorAutomaticSelfCalibration(uint8_t& padding,
                                                        bool& status) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6711, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt8(padding);
    localError |= rxFrame.getBool(status);
    return localError;
}

int16_t SensirionI2cSen6x::setAmbientPressure(uint16_t ambientPressure) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6720, buffer_ptr, 5);
    localError |= txFrame.addUInt16(ambientPressure);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::getAmbientPressure(uint16_t& ambientPressure) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6720, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(ambientPressure);
    return localError;
}

int16_t SensirionI2cSen6x::setSensorAltitude(uint16_t altitude) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6736, buffer_ptr, 5);
    localError |= txFrame.addUInt16(altitude);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::getSensorAltitude(uint16_t& altitude) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6736, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(altitude);
    return localError;
}

int16_t SensirionI2cSen6x::activateShtHeater() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6765, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    return localError;
}

int16_t SensirionI2cSen6x::getShtHeaterMeasurements(int16_t& humidity,
                                                    int16_t& temperature) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x6790, buffer_ptr, 6);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 6);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 6,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getInt16(humidity);
    localError |= rxFrame.getInt16(temperature);
    return localError;
}

int16_t SensirionI2cSen6x::getProductName(int8_t productName[],
                                          uint16_t productNameSize) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xd014, buffer_ptr, 48);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 48);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 48,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getBytes((uint8_t*)productName, productNameSize);
    return localError;
}

int16_t SensirionI2cSen6x::getSerialNumber(int8_t serialNumber[],
                                           uint16_t serialNumberSize) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xd033, buffer_ptr, 48);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 48);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 48,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getBytes((uint8_t*)serialNumber, serialNumberSize);
    return localError;
}

int16_t SensirionI2cSen6x::getVersion(uint8_t& firmwareMajor,
                                      uint8_t& firmwareMinor) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xd100, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt8(firmwareMajor);
    localError |= rxFrame.getUInt8(firmwareMinor);
    return localError;
}

int16_t SensirionI2cSen6x::readDeviceStatus(SEN6XDeviceStatus& deviceStatus) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xd206, buffer_ptr, 6);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 6);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 6,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt32(deviceStatus.value);
    return localError;
}

int16_t
SensirionI2cSen6x::readAndClearDeviceStatus(SEN6XDeviceStatus& deviceStatus) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xd210, buffer_ptr, 6);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(20);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 6);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 6,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt32(deviceStatus.value);
    return localError;
}

int16_t SensirionI2cSen6x::deviceReset() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xd304, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1200);
    return localError;
}

void SensirionI2cSen6x::begin(TwoWire& i2cBus, uint8_t i2cAddress) {
    _i2cBus = &i2cBus;
    _i2cAddress = i2cAddress;
}
