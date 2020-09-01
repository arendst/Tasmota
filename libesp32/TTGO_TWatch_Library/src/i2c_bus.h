#ifndef TTGO_I2CBUF_H
#define TTGO_I2CBUF_H

#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
class I2CBus
{
public:
    I2CBus(TwoWire &port = Wire, int sda = 21, int scl = 22)
    {
        _port = &port;
        _port->begin(sda, scl);
        _i2c_mux = xSemaphoreCreateRecursiveMutex();
    };
    void scan();
    uint16_t readBytes(uint8_t addr, uint8_t *data, uint16_t len, uint16_t delay_ms = 0);
    uint16_t readBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
    uint16_t writeBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
    bool deviceProbe(uint8_t addr);
private:
    TwoWire *_port;
    SemaphoreHandle_t _i2c_mux = NULL;
};

#endif