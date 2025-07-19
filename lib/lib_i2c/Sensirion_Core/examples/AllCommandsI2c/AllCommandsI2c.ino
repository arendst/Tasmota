#include <SensirionCore.h>
#include <Wire.h>
#include <stdint.h>

uint8_t txBuffer[256];
uint8_t rxBuffer[256];

SensirionI2CTxFrame txFrame(txBuffer, 256);
SensirionI2CRxFrame rxFrame(rxBuffer, 256);

void setup() {
    Wire.begin();
}

void loop() {
    uint16_t mockCommand = 42;
    uint16_t error = txFrame.addCommand(mockCommand);

    uint32_t mockUInt32 = 42;
    error |= txFrame.addUInt32(mockUInt32);

    int32_t mockInt32 = 42;
    error |= txFrame.addInt32(mockInt32);

    uint16_t mockUInt16 = 42;
    error |= txFrame.addUInt16(mockUInt16);

    int16_t mockInt16 = 42;
    error |= txFrame.addInt16(mockInt16);

    uint8_t mockUInt8 = 42;
    error |= txFrame.addUInt8(mockUInt8);

    int8_t mockInt8 = 42;
    error |= txFrame.addInt8(mockInt8);

    float mockFloat = 42.0f;
    error |= txFrame.addFloat(mockFloat);

    bool mockBool = true;
    error |= txFrame.addBool(mockBool);

    uint8_t mockBytes[] = {42, 42, 42, 42};
    error |= txFrame.addBytes(mockBytes, 4);

    uint8_t mockAddress = 42;

    error |= SensirionI2CCommunication::sendFrame(mockAddress, txFrame, Wire);

    size_t mockNumBytes = 42;
    error |= SensirionI2CCommunication::receiveFrame(mockAddress, mockNumBytes,
                                                     rxFrame, Wire);

    error |= rxFrame.getUInt32(mockUInt32);
    error |= rxFrame.getInt32(mockInt32);
    error |= rxFrame.getUInt16(mockUInt16);
    error |= rxFrame.getInt16(mockInt16);
    error |= rxFrame.getUInt8(mockUInt8);
    error |= rxFrame.getInt8(mockInt8);
    error |= rxFrame.getFloat(mockFloat);
    error |= rxFrame.getBytes(mockBytes, 4);
}
