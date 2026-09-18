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
#include <Arduino.h>
#include <SensirionI2cSps30.h>
#include <Wire.h>

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cSps30 sensor;

static char errorMessage[64];
static int16_t error;

void setup() {

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }
    Wire.begin();
    sensor.begin(Wire, SPS30_I2C_ADDR_69);

    sensor.stopMeasurement();
    int8_t serialNumber[32] = {0};
    int8_t productType[8] = {0};
    sensor.readSerialNumber(serialNumber, 32);
    Serial.print("serialNumber: ");
    Serial.print((const char*)serialNumber);
    Serial.println();
    sensor.readProductType(productType, 8);
    Serial.print("productType: ");
    Serial.print((const char*)productType);
    Serial.println();
    sensor.startMeasurement(SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_FLOAT);
    delay(100);
}

void loop() {

    uint16_t dataReadyFlag = 0;
    float mc1p0 = 0;
    float mc2p5 = 0;
    float mc4p0 = 0;
    float mc10p0 = 0;
    float nc0p5 = 0;
    float nc1p0 = 0;
    float nc2p5 = 0;
    float nc4p0 = 0;
    float nc10p0 = 0;
    float typicalParticleSize = 0;
    delay(1000);
    error = sensor.readDataReadyFlag(dataReadyFlag);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute readDataReadyFlag(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("dataReadyFlag: ");
    Serial.print(dataReadyFlag);
    Serial.println();
    error = sensor.readMeasurementValuesFloat(mc1p0, mc2p5, mc4p0, mc10p0,
                                              nc0p5, nc1p0, nc2p5, nc4p0,
                                              nc10p0, typicalParticleSize);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute readMeasurementValuesFloat(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("mc1p0: ");
    Serial.print(mc1p0);
    Serial.print("\t");
    Serial.print("mc2p5: ");
    Serial.print(mc2p5);
    Serial.print("\t");
    Serial.print("mc4p0: ");
    Serial.print(mc4p0);
    Serial.print("\t");
    Serial.print("mc10p0: ");
    Serial.print(mc10p0);
    Serial.print("\t");
    Serial.print("nc0p5: ");
    Serial.print(nc0p5);
    Serial.print("\t");
    Serial.print("nc1p0: ");
    Serial.print(nc1p0);
    Serial.print("\t");
    Serial.print("nc2p5: ");
    Serial.print(nc2p5);
    Serial.print("\t");
    Serial.print("nc4p0: ");
    Serial.print(nc4p0);
    Serial.print("\t");
    Serial.print("nc10p0: ");
    Serial.print(nc10p0);
    Serial.print("\t");
    Serial.print("typicalParticleSize: ");
    Serial.print(typicalParticleSize);
    Serial.println();
}
